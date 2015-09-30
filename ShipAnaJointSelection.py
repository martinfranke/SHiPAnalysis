# writes out numbers showing the effectiveness of the single cuts
# a) for individual cuts
# b) for consecutive cuts
# takes file with -f and geofile with -g
import ROOT,os,sys,getopt
import rootUtils as ut
import shipunit as u
from ShipGeoConfig import ConfigRegistry
import codecs
from tools_Martin import finishandprint


PDG = ROOT.TDatabasePDG.Instance()
inputFile = ""
geoFile = ""
dy        = 10.0
nEvents    = 30000
fiducialCut = True
measCut = 25
docaCut = 1.
chi2CutOff  = 5.
IPCut = 250
ECALcut = .150

try:
        opts, args = getopt.getopt(sys.argv[1:], "f:g:A:i", ["geoFile="])
except getopt.GetoptError:
        # print help information and exit:
        print ' enter file name'
        sys.exit()
for o, a in opts:
        if o in ("-f"):
            inputFile = a
        if o in ("-g", "--geoFile"):
            geoFile = a
print inputFile
print
outfile = inputFile[:-5]
outfile = outfile.rpartition("/")[-1]
f       = ROOT.TFile(inputFile)
sTree   = f.cbmsim

# try to figure out which ecal geo to load
if not geoFile:
 geoFile = inputFile.replace('ship.','geofile_full.').replace('_rec.','.')
fgeo = ROOT.TFile(geoFile)
sGeo = fgeo.FAIRGeom
if sGeo.GetVolume('EcalModule3') :  ecalGeoFile = "ecal_ellipse6x12m2.geo"
else: ecalGeoFile = "ecal_ellipse5x10m2.geo" 
fgeo.Close()
ROOT.gGeoManager.Delete()
print 'found ecal geo for ',ecalGeoFile

# init geometry and mag. field
ShipGeo = ConfigRegistry.loadpy("$FAIRSHIP/geometry/geometry_config.py", Yheight = dy, EcalGeoFile = ecalGeoFile )
# -----Create geometry----------------------------------------------
import shipDet_conf
#run = ROOT.FairRunSim()
#modules = shipDet_conf.configure(run,ShipGeo)

tgeom = ROOT.TGeoManager("Geometry", "Geane geometry")
gMan  = tgeom.Import(geoFile)
geoMat =  ROOT.genfit.TGeoMaterialInterface()
ROOT.genfit.MaterialEffects.getInstance().init(geoMat)
volDict = {}
i=0
for x in ROOT.gGeoManager.GetListOfVolumes():
 volDict[i]=x.GetName()
 i+=1

bfield = ROOT.genfit.BellField(ShipGeo.Bfield.max ,ShipGeo.Bfield.z,2, ShipGeo.Yheight/2.)
fM = ROOT.genfit.FieldManager.getInstance()
fM.init(bfield)

# prepare veto decisions
import shipVeto
veto = shipVeto.Task()

def checkFiducialVolume(sTree,tkey,dy):
# to be replaced later with using track extrapolator,
# for now use MC truth
   inside = True
   if not fiducialCut: return inside 
   mcPartKey = sTree.fitTrack2MC[tkey]
   for ahit in sTree.strawtubesPoint:
		if int(ahit.GetDetectorID()/10000000) > 4 : continue
		if ahit.GetTrackID() == mcPartKey:
			X,Y = ahit.GetX(),ahit.GetY()
			Rsq = (X/250)**2 + (Y/500)**2
			if Rsq > 1:
				inside = False    
				break
   return inside
   
def getPofTrack(sTree,tkey):
	Ptruth,Ptruthz = -1.,-1.
	mcPartKey = sTree.fitTrack2MC[tkey]
	for ahit in sTree.strawtubesPoint:
		if ahit.GetTrackID() == mcPartKey:
			Ptruthz = ahit.GetPz()
			Ptruth  = ROOT.TMath.Sqrt(ahit.GetPx()**2+ahit.GetPy()**2+Ptruthz**2)
			break
	return Ptruth,Ptruthz

#weight[""][0]: sum of weights
#weight[""][1]: number of entries
#weight[""][0]: last event contributing here (stop double counting)
weights = {}
weights['rec'] = [0,0,0,0,0,0]; 
weights['2tracks'] = [0,0,0,0,0,0]; 
weights['noveto'] = [0,0,0,0,0,0]; 
weights['POCA'] = [0,0,0,0,0,0]; 
weights['tracks'] = [0,0,0,0,0,0]; 
weights['P'] = [0,0,0,0,0,0]; 
weights['chi'] = [0,0,0,0,0,0]; 
weights['ndof'] = [0,0,0,0,0,0]; 
weights['DOCA'] = [0,0,0,0,0,0]; 
weights['IP'] = [0,0,0,0,0,0]; 
weights['ECAL'] = [0,0,0,0,0,0]; 
weights['Mu1'] = [0,0,0,0,0,0]; 
weights['Mu2'] = [0,0,0,0,0,0]; 
cuts={}

sigma = 0
nEvents = min(sTree.GetEntries(),nEvents)
Scale = 1.0/nEvents
if 'osmic' in inputFile: Scale = 5000000/10
if 'T.' in inputFile:
	fcross = ROOT.TFile('../muDIScrossSec.root')
	for x in fcross.GetListOfKeys():
		sigma = x.ReadObj()
		print sigma.GetTitle()
		if sigma.GetTitle() == 'PythiaCross' : break  #'PythiaCross'

def weightDIS(sTree):
	P_muon = sTree.MCTrack[0].GetP()
	return sigma.Eval(P_muon)
	
# start event loop
def myEventLoop(n):
	rc = sTree.GetEntry(n)
# compute veto power for event
	flag_SBT,w_SBT = veto.SBT_decision(sTree)
	flag_SVT,w_SVT = veto.SVT_decision(sTree)
	flag_UVT,w_UVT = veto.UVT_decision(sTree)
	flag_Track,w_tracks,nMultCon = veto.Track_decision(sTree)
	flag_RPC,w_RPC,nHits = veto.RPC_decision(sTree)
# loop over particles, 2-track combinations
	cuts['rec'] = 0; cuts['noveto'] = 0; cuts['POCA'] = 0; cuts['tracks'] = 0
	cuts['P'] = 0; cuts['chi'] = 0; cuts['ndof'] = 0; cuts['DOCA'] = 0
	cuts['IP'] = 0; cuts['ECAL'] = 0; cuts['Mu1'] = 0; cuts['Mu2'] = 0
	cuts['2tracks'] = 0
	for HNL in sTree.Particles:
		wg = sTree.MCTrack[1].GetWeight()
		wg = 1.0*wg*Scale
		if not wg>0.: wg=1.
		if 'T.' in inputFile: wg = weightDIS(sTree)
		t1,t2 = HNL.GetDaughter(0),HNL.GetDaughter(1) 
	# reconstructed
		cuts['rec'] = 1	
	# POCA		
		HNLPos = ROOT.TLorentzVector()
		HNL.ProductionVertex(HNLPos)
		HNLMom = ROOT.TLorentzVector()
		HNL.Momentum(HNLMom)
		if HNL.GetMother(1)==99 :
			xv,yv,zv,doca  =  HNLPos.X(),HNLPos.Y(),HNLPos.Z(),HNLPos.T()
		else: print "DOCA missing"; break
		Rsq = (xv/249)**2 + (yv/499)**2
		if Rsq < 1 and zv > ShipGeo['vetoStation'].z + 20 and zv < ShipGeo['TrackStation1'].z - 20: cuts['POCA'] = 1
	# fiducial volume
		if checkFiducialVolume(sTree,t1,dy) and checkFiducialVolume(sTree,t2,dy): cuts['tracks'] = 1
	# chi2, nDOF
		fitStatus = {}
		nmeas = {}
		chi2 = {}
		for tr in [t1,t2]:
			fitStatus[tr]  = sTree.FitTracks[tr].getFitStatus()
			nmeas[tr] = fitStatus[tr].getNdf()
			chi2[tr] = fitStatus[tr].getChi2()
		if chi2[t1]/nmeas[t1] < chi2CutOff and chi2[t2]/nmeas[t2] < chi2CutOff: cuts['chi'] = 1
		if nmeas[t1] > measCut and nmeas[t2] > measCut: cuts['ndof'] = 1	
	# DOCA			
		if  doca < docaCut : cuts['DOCA'] = 1
	#number of tracks	
		if nMultCon == 2 : cuts['2tracks'] = 1	
		#if len(sTree.Particles) == 1 : cuts['2tracks'] = 1
		#else: print len(sTree.Particles),nMultCon
	# momentum of tracks
		momentum1 = getPofTrack(sTree, t1)
		momentum2 = getPofTrack(sTree, t2)
		if momentum1 > 1 and momentum2 > 1: cuts['P'] = 1
	# IP		
		tr = ROOT.TVector3(0,0,ShipGeo.target.z0)
		t = 0
		for i in range(3):   t += HNLMom(i)/HNLMom.P()*(tr(i)-HNLPos(i)) 
		dist = 0
		for i in range(3):   dist += (tr(i)-HNLPos(i)-t*HNLMom(i)/HNLMom.P())**2
		dist = ROOT.TMath.Sqrt(dist)
		if dist < IPCut: cuts['IP'] = 1
	#ECAL
		E_ECAL = 0
		for hit in sTree.EcalPointLite:
			E_ECAL += hit.GetEnergyLoss()
		for hit in sTree.HcalPointLite:
			E_ECAL += hit.GetEnergyLoss()
		if E_ECAL > ECALcut: cuts['ECAL'] = 1
	#Mu1, Mu2
		mu1 = mu2 = 0
		for ahit in sTree.muonPoint:
			x,y,z = ahit.GetX(), ahit.GetY(), ahit.GetZ()
			node = ROOT.gGeoManager.FindNode(x,y,z).GetName() 
			TID = ahit.GetTrackID()
			if TID <0: continue
			PID = sTree.MCTrack[TID].GetPdgCode()
			if "muondet0" in node and abs(PID) == 13: mu1 = 1
			if "muondet1" in node and abs(PID) == 13: mu2 = 1
		if mu1: cuts['Mu1'] = 1
		if mu2: cuts['Mu2'] = 1
	# veto components		
		w_veto = wg * w_SBT * w_SVT * w_UVT * w_RPC
		if (not(flag_SBT) and not(flag_SVT) and not(flag_UVT) and not(flag_RPC)): cuts['noveto'] = 1				
	########################################################################	
	if cuts['rec']: # veto
		if not weights['rec'][4] == n: weights['rec'][0] += wg; weights['rec'][1] += 1; weights['rec'][4] = n							
		if cuts['2tracks']: #2tracks
			if not weights['2tracks'][4] == n: weights['2tracks'][0] += wg; weights['2tracks'][1] += 1; weights['2tracks'][4] = n
			if cuts['POCA']:	#POCA
				if not weights['POCA'][4] == n: weights['POCA'][0] += wg; weights['POCA'][1] += 1; weights['POCA'][4] = n
				if cuts['tracks']: #Tracks
					if not weights['tracks'][4] == n: weights['tracks'][0] += wg; weights['tracks'][1] += 1; weights['tracks'][4] = n
					if cuts['ndof']: #ndof			
						if not weights['ndof'][4] == n: weights['ndof'][0] += wg; weights['ndof'][1] += 1; weights['ndof'][4] = n
						if cuts['chi']: #Chi2			
							if not weights['chi'][4] == n: weights['chi'][0] += wg; weights['chi'][1] += 1; weights['chi'][4] = n
							if cuts['DOCA']: #DOCA				
								if not weights['DOCA'][4] == n: weights['DOCA'][0] += wg; weights['DOCA'][1] += 1; weights['DOCA'][4] = n
								if cuts['P']:	#Momentum	
									if not weights['P'][4] == n: weights['P'][0] += wg; weights['P'][1] += 1	; weights['P'][4] = n
									#if cuts['IP']: #IP				
									#	if not weights['IP'][4] == n: weights['IP'][0] += wg; weights['IP'][1] += 1; weights['IP'][4] = n
									if not weights['noveto'][4] == n: weights['noveto'][0] += w_veto; weights['noveto'][1] += cuts['noveto']; weights['noveto'][4] = n
									if cuts['ECAL']: #ECAL
										if not weights['ECAL'][4] == n: weights['ECAL'][0] += wg; weights['ECAL'][1] += 1; weights['ECAL'][4] = n
										if cuts['Mu1']: #Mu1
											if not weights['Mu1'][4] == n: weights['Mu1'][0] += wg; weights['Mu1'][1] += 1; weights['Mu1'][4] = n
											if cuts['Mu2']: #Mu2
												if not weights['Mu2'][4] == n: weights['Mu2'][0] += wg; weights['Mu2'][1] += 1	; weights['Mu2'][4] = n
	########################################################################		
	if cuts['rec'] :# veto
		if not weights['rec'][5] == n: weights['rec'][2] += wg; weights['rec'][3] += 1; weights['rec'][5] = n	
		if not weights['noveto'][5] == n: weights['noveto'][2] += w_veto; weights['noveto'][3] += cuts['noveto']; weights['noveto'][5] = n
	if cuts['rec'] and cuts['2tracks']:	#2tracks
		if not weights['2tracks'][5] == n: weights['2tracks'][2] += wg; weights['2tracks'][3] += 1; weights['2tracks'][5] = n	
	if cuts['rec'] and cuts['POCA']:	#POCA
		if not weights['POCA'][5] == n: weights['POCA'][2] += wg; weights['POCA'][3] += 1; weights['POCA'][5] = n
	if cuts['rec'] and cuts['tracks']:	#Tracks
		if not weights['tracks'][5] == n: weights['tracks'][2] += wg; weights['tracks'][3] += 1; weights['tracks'][5] = n
	if cuts['rec'] and cuts['ndof']: #ndof			
		if not weights['ndof'][5] == n: weights['ndof'][2] += wg; weights['ndof'][3] += 1; weights['ndof'][5] = n
	if cuts['rec'] and cuts['chi']: #Chi2			
		if not weights['chi'][5] == n: weights['chi'][2] += wg; weights['chi'][3] += 1; weights['chi'][5] = n
	if cuts['rec'] and cuts['DOCA']: #DOCA				
		if not weights['DOCA'][5] == n: weights['DOCA'][2] += wg; weights['DOCA'][3] += 1; weights['DOCA'][5] = n
	if cuts['rec'] and cuts['IP']: #IP				
		if not weights['IP'][5] == n: weights['IP'][2] += wg; weights['IP'][3] += 1; weights['IP'][5] = n
	if cuts['rec'] and cuts['P']:	#Momentum	
		if not weights['P'][5] == n: weights['P'][2] += wg; weights['P'][3] += 1	; weights['P'][5] = n
	if cuts['rec'] and cuts['ECAL']: #ECAL
		if not weights['ECAL'][5] == n: weights['ECAL'][2] += wg; weights['ECAL'][3] += 1; weights['ECAL'][5] = n
	if cuts['rec'] and cuts['Mu1']: #Mu1
		if not weights['Mu1'][5] == n: weights['Mu1'][2] += wg; weights['Mu1'][3] += 1; weights['Mu1'][5] = n
	if cuts['rec'] and cuts['Mu2']: #Mu2
		if not weights['Mu2'][5] == n: weights['Mu2'][2] += wg; weights['Mu2'][3] += 1	; weights['Mu2'][5] = n
	########################################################################

 
for n in range(0, nEvents): 
	if not n%5000: print n
	myEventLoop(n)
	sTree.FitTracks.Delete()
f.Close()
		
outname = inputFile.rpartition("/")[-1]
outfile=(outname + '.csv')
finishandprint(outfile,weights)
