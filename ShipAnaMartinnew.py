# writes out histograms of different observables
# takes file with -f and geofile with -g
import ROOT,os,sys,getopt,math
import rootUtils as ut
import shipunit as u
from ShipGeoConfig import ConfigRegistry

from tools_Martin import *

PDG = ROOT.TDatabasePDG.Instance()

dy        = 10.0
nEvents   = 99999
measCut = 25
docaCut = 2.
fiducialCut = False
inputFile = ""
geoFile = ""

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
neutrino = DIS =  cosmics = 0
if "osmic" in inputFile: 
	cosmics = 1; print "cosmic Analysis"
elif "nu" in inputFile: 
	neutrino = 1; print "neutrino Analysis"
elif "DIS" in inputFile: 
	DIS = 1; print "DIS Analysis"


if not geoFile:
	geoFile = inputFile.replace('ship.','geofile_full.').replace('_rec.','.') 
print 'with  Geofile: ',geoFile

geometryData = loadGeometry(geoFile, dy)
ShipGeo = geometryData['ShipGeo']
gMan  = geometryData['gMan']
fGeo = geometryData['fGeo']

geoMat =  ROOT.genfit.TGeoMaterialInterface()
ROOT.genfit.MaterialEffects.getInstance().init(geoMat)

d = getNodeList(fGeo, False)
LSC = {}
for node in sorted(d.keys()):
	if 'LiSc' in node:
		#print node, d[node]['z']['pos'] -  d[node]['z']['dim'], d[node]['z']['pos'] +  d[node]['z']['dim']
		LSC[node] = {'z1': d[node]['z']['pos'] -  d[node]['z']['dim'], 'z2': d[node]['z']['pos'] +  d[node]['z']['dim']}
		
#print d['T2LiSc_10_1']['x']['dim'] - 15
#print d['T2LiSc_10_1']['y']['dim'] - 15
#print d['T1LiSc_1_1']['x']['dim'] - 15 
#print d['T1LiSc_1_1']['z']['pos'] -  d['T1LiSc_1_1']['z']['dim']
#print d['T2LiSc_1_1']['z']['pos']# - d['T2LiSc_1_1']['z']['dim'] 

bfield = ROOT.genfit.BellField(ShipGeo.Bfield.max ,ShipGeo.Bfield.z,2, ShipGeo.Yheight/2.)
fM = ROOT.genfit.FieldManager.getInstance()
fM.init(bfield)

h = {}
ut.bookHist(h,'measVSchi2','number of measurements vs chi2/meas',40,-0.5,39.5,100,0.,100.)
ut.bookHist(h,'chi2','chi2/nmeas after trackfit',100,0.,100.)

ut.bookHist(h,'N.o.Tracks','Number of Tracks; #Tracks',10,0,10)
ut.bookHist(h,'diff','diff',100,-1,1)
ut.bookHist(h,'EventCat','Event Categories',6,0,6)
ut.bookHist(h,'Seg_ZeroTrack','ZeroTracks; #Segments',30,0,30)
ut.bookHist(h,'Seg_OneTrack','OneTracks; #Segments',30,0,30)
ut.bookHist(h,'Seg_TwoTrack','TwoTracks; #Segments',30,0,30)
ut.bookHist(h,'Seg_TwoTrackPOCA','TwoTracksPOCA; #Segments',30,0,30)
ut.bookHist(h,'Seg_ID','SegID',896,0,896)
ut.bookHist(h,'Seg_2D','Seg2D;phisegment;zsegment',16,0,16,56,0,56)

ut.bookHist(h,'Doca','Doca between two tracks; DOCA [cm]',1000,0.,100.)
ut.bookHist(h,'Distx','Distx between two tracks; Distx [cm]',100,0.,10.)
ut.bookHist(h,'Multi','Multi;#mu shower mutliplicity',10,0,10)
ut.bookHist(h,'MultiPOCA','Multi;#mu shower mutliplicity',10,0,10)
ut.bookHist(h,'P_muon','P_muon; P_muon [GeV]',25,0.1,1000)
#ut.bookHist(h,'P_muon','P_muon; P_muon [GeV]',25,-1,3)
ut.bookHist(h,'P_muonU','P_muon; P_muon [GeV]',20,0.1,1000)
#ut.bookHist(h,'P_muonU','P_muon; P_muon [GeV]',100,-1,3)
#h['P_muon'] = BinLogX(h['P_muon'])
#h['P_muonU'] = BinLogX(h['P_muonU'])

ut.bookHist(h,'Poca','Poca of two tracks; z [cm]; r',100,1000,3500,100,0.,3)
ut.bookHist(h,'PocaXY','PocaXY of two tracks; x [cm]; y[cm]',500,-300,300,500,-600,600)
ut.bookHist(h,'PocaXY_z','PocaXY of two tracks; x [cm]; y[cm]',500,-300,300,500,-600,600)

ut.bookHist(h,'Px','Px/P; Px/P',100,-1,1)
ut.bookHist(h,'Py','Py/P; Py/P',100,-1,1)
ut.bookHist(h,'Pz','Pz/P; Pz/P',100,-1,1)

ut.bookHist(h,'reaction_XY','reaction_XY; x [cm]; y[cm]',30,-260,260,30,200,550)
ut.bookHist(h,'reaction_XZ','reaction_XZ; x [cm]; z[cm]',30,-260,260,30,1000,3000)
ut.bookHist(h,'reaction_P','reaction_P; P_d [GeV]; P_m[GeV]',30,0,200,30,0,1000)
ut.bookHist(h,'reaction_phi','reaction_phi; acos(v(P_d) * v(P_m) /P_m P_d)',30,0,0.05)
ut.bookHist(h,'reaction_Pt_P','reaction_Pt_P; P_T [GeV]; P [GeV]',30,0,200,30,0,200)
ut.bookHist(h,'gamma_XY','gamma_XY; x [cm]; y[cm]',30,-260,260,30,200,550)
ut.bookHist(h,'gamma_XZ','gamma_XZ; x [cm]; z[cm]',30,-260,260,30,1000,3000)
ut.bookHist(h,'gamma_P','gamma_P; P_gamma [GeV]; P_T_gamma[GeV]',30,0,200,30,0,200)
ut.bookHist(h,'gamma_P_P_muon','gamma_P; P_gamma [GeV]; P_muon[GeV]',30,0,200,30,0,1000)

ut.bookHist(h,'phi_m','phi_muon; #phi_muon',100,-7,7)
ut.bookHist(h,'theta_m','theta_muon; #theta_muon',100,0,3.5)

ut.bookHist(h,'P1','P1; P1 [Gev]',100,0,1000)
ut.bookHist(h,'P2','P2; P2 [Gev]',100,0,100)
ut.bookHist(h,'Pt1','Pt1; Pt1 [Gev]',100,0,1000)
ut.bookHist(h,'Pt2','Pt2; Pt2 [Gev]',100,0,100)
ut.bookHist(h,'eta1','#eta_1; #eta_1 ',100,-10,10)
ut.bookHist(h,'eta2','#eta_2; #eta_2 ',100,-10,10)
#ut.bookHist(h,'phi1','#phi_1; #phi_1 ',100,0,100)
#ut.bookHist(h,'phi2','#phi_2; #phi_2 ',100,0,100)

ut.bookHist(h,'IP_target','Impact Parameter to target; IP [cm]',1000,0.,10000.)
ut.bookHist(h,'HNL','reconstructed Mass; M [GeV]',1000,0.,16)
ut.bookHist(h,'HNL_ee','reconstructed Mass; M [GeV]',1000,0.,1)
ut.bookHist(h,'PDG_2Track','PDG_2Track; ee, em, other',3,0,3)

ut.bookHist(h,'cutMassTagger','cut:Mass/Tagger;mass;Tagger',2,0,2,2,0,2)
ut.bookHist(h,'cutMassPOCA','cut:Mass/POCA;mass;POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutMassDOCA','cut:Mass/DOCA;mass;DOCA',2,0,2,2,0,2)
ut.bookHist(h,'cutMassDOCAPOCA','cut:Mass/DOCA+POCA;mass;DOCA+POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutDOCAPOCA','cut:DOCA/POCA;DOCA;POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutTaggerDOCAPOCA','cut:Tagger/DOCA+POCA;Tagger;DOCA + POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutTaggerDOCA','cut:Tagger/DOCA;Tagger;DOCA',2,0,2,2,0,2)
ut.bookHist(h,'cutTaggerPOCA','cut:Tagger/POCA;Tagger;POCA',2,0,2,2,0,2)
####################
ut.bookHist(h,'cutMassTaggerU','cut:Mass/Tagger;mass;Tagger',2,0,2,2,0,2)
ut.bookHist(h,'cutMassPOCAU','cut:Mass/POCA;mass;POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutMassDOCAU','cut:Mass/DOCA;mass;DOCA',2,0,2,2,0,2)
ut.bookHist(h,'cutMassDOCAPOCAU','cut:Mass/DOCA+POCA;mass;DOCA+POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutDOCAPOCAU','cut:DOCA/POCA;DOCA;POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutTaggerDOCAPOCAU','cut:Tagger/DOCA+POCA;Tagger;DOCA + POCA',2,0,2,2,0,2)
ut.bookHist(h,'cutTaggerDOCAU','cut:Tagger/DOCA;Tagger;DOCA',2,0,2,2,0,2)
ut.bookHist(h,'cutTaggerPOCAU','cut:Tagger/POCA;Tagger;POCA',2,0,2,2,0,2)

 
def checkFiducialVolume(sTree,tkey,dy):
# to be replaced later with using track extrapolator,
# for now use MC truth
   inside = True
   if not fiducialCut: return inside 
   mcPartKey = sTree.fitTrack2MC[tkey]
   for ahit in sTree.strawtubesPoint:
     if ahit.GetTrackID() == mcPartKey:
        X,Y = ahit.GetX(),ahit.GetY()
        Rsq = (X/(2.45*u.m) )**2 + (Y/((dy/2.-0.05)*u.m) )**2
        if Rsq > 1:
         inside = False 
         print "not fiducial"     
         break
   return inside
   
def rescaleCosmics(E):
	scaling = 1
	if E < 100:
		old = 22*ROOT.TMath.Power(E,-0.6 -0.285*ROOT.TMath.Log(E))
		new = -0.1292*(ROOT.TMath.Log(E))**2 - 0.266*ROOT.TMath.Log(E) -2.6
		new = 10000 * 10**new
		scaling = new/old
	return scaling

def myVertex(t1,t2,PosDir):
 # closest distance between two tracks
    # d = |pq . u x v|/|u x v|
   a = ROOT.TVector3(PosDir[t1][0](0) ,PosDir[t1][0](1), PosDir[t1][0](2))
   u = ROOT.TVector3(PosDir[t1][1](0),PosDir[t1][1](1),PosDir[t1][1](2))
   c = ROOT.TVector3(PosDir[t2][0](0) ,PosDir[t2][0](1), PosDir[t2][0](2))
   v = ROOT.TVector3(PosDir[t2][1](0),PosDir[t2][1](1),PosDir[t2][1](2))
   pq = a-c
   uCrossv = u.Cross(v)
   dist  = pq.Dot(uCrossv)/(uCrossv.Mag()+1E-8)
   distx = (PosDir[t1][0](0) - PosDir[t2][0](0))/(uCrossv.Mag()+1E-8)
   # u.a - u.c + s*|u|**2 - u.v*t    = 0
   # v.a - v.c + s*v.u    - t*|v|**2 = 0
   E = u.Dot(a) - u.Dot(c) 
   F = v.Dot(a) - v.Dot(c) 
   A,B = u.Mag2(), -u.Dot(v) 
   C,D = u.Dot(v), -v.Mag2()
   t = -(C*E-A*F)/(B*C-A*D)
   X = c.x()+v.x()*t
   Y = c.y()+v.y()*t
   Z = c.z()+v.z()*t
   # sT = ROOT.gROOT.FindAnything('cbmsim')
   #print 'test2 ',X,Y,Z,dist
   #print 'truth',sTree.MCTrack[2].GetStartX(),sTree.MCTrack[2].GetStartY(),sTree.MCTrack[2].GetStartZ()
   return X,Y,Z,abs(dist), abs(distx)

# start event loop
def myEventLoop(NEvents):
	
	cat = [0.0,0.0,0.0,0.0,0.0,0.0]
	pi = ROOT.TMath.Pi()
	
	totalweight = 0
	weight_2Track = 0
	events_2Track = 0
	events_POCA = 0
	weight_DOCA = 0
	weight_POCA = 0
	weight_mass = 0
	weight_Tag1 = 0
	weight_Tag099 = 0
	weight_Tag09 = 0
	weight_Seg095 = 0
	weight_Seg099 = 0
	weight_noTagger = 0
	events_noTagger = 0
	weight_IP = 0
	
	a = 268
	b = 518
	asmall = 204.2

	N = 16
	dphi = 2*pi/N
	phistart = dphi/2;
	
	scaleCosmics = 5000000.0/10 # changed lately from 60 to 10!!
		
	lastEvent = {}
	lastEvent["2Track"] = -1
	lastEvent["DOCA"] = -1
	lastEvent["POCA"] = -1
	lastEvent["Mass"] = -1
	lastEvent["Tagger"] = -1
	lastEvent["noTagger"] = -1
	
	nEvents = min(sTree.GetEntries(),NEvents)	
	
	if neutrino: 
		ff = ROOT.TFile("../eosfiles/NeutrinoSamples/histoForWeights.root") 
		h_GioHans = ff.Get("h_Gio")

		if neutrino > 0: scaleNeutrino = 17591.878/nEvents # sigma*phi*N_A/nEvents
		else: scaleNeutrino = 5632.497/nEvents
		print scaleNeutrino
	if DIS:
		integraloverspectrum = 14.8414 # 1400 * int(x^(-2.7)*(1/(1+x/115) + 0.054/(1+x/850))*(log(x)/log(10))^2, x, 10, 1000)
		material = 1
		if ".T" in inputFile:
			material = 72; print "Upper concrete wall" #TOP
		if ".V" in inputFile:
			material = 25.26 ; print "Vessel material" #Vessel
		scale = 96352 * material  / integraloverspectrum
	
	n_segments = 0	
	ELOSS = {}
	for node in LSC:
		ELOSS[node] = [0]*N
		n_segments += N
	print "number of segments in this geometry: ",n_segments
	
	for n in range(nEvents): 
		if not n%10000: print "",n  
		rc = sTree.GetEntry(n)
		
		# veto Geometry!
		veto = 0		
		if sTree.ShipRpcPoint.GetEntriesFast(): veto = 1		
		for ahit in sTree.strawtubesPoint:
			if ahit.GetZ() < 0: veto = 1; break
		if veto: continue #veto!
		
		# multiplicity and weight for Cosmics: 
		multiplicity = 0
		if cosmics:
			wg = 1
			#for t in sTree.MCTrack:
				#if t.GetMotherId() == -1 and t.GetStartY() == 2000:
					#multiplicity +=1; 
					#wg = wg * t.GetWeight()
			#if multiplicity-1: 
			#	print "",multiplicity
			#wg = wg/multiplicity	
			#if multiplicity > 1: 
				#difference = wg - sTree.MCTrack[0].GetWeight()
				#h['diff'].Fill(difference)
				
			wg = sTree.MCTrack[0].GetWeight()
			wg = wg*scaleCosmics
			#wg = wg*rescaleCosmics(sTree.MCTrack[0].GetEnergy())
			#if sTree.MCTrack[0].GetEnergy()< 100: print "RS: ", rescaleCosmics(sTree.MCTrack[0].GetEnergy())
		
		if neutrino: #weight neutrinos
			E = sTree.MCTrack[0].GetEnergy()  
			Ecorr = h_GioHans.GetBinContent(h_GioHans.GetXaxis().FindBin(E))	
			wg = sTree.MCTrack[0].GetWeight()*scaleNeutrino*Ecorr*E
			
		if DIS: #weight Cosmic DIS
			E = sTree.MCTrack[0].GetEnergy()
			wg = sTree.MCTrack[0].GetWeight() * scale * rescaleCosmics(E) * (ROOT.TMath.Log10(E))**2
		
		if not neutrino and not cosmics and not DIS: wg = sTree.MCTrack[0].GetWeight()
		totalweight += wg
		if not wg>0.: wg=1.
		
# quick Tagger analysis
		for ahit in sTree.vetoPoint:
			x,y,z = (ahit.GetX(), ahit.GetY(), ahit.GetZ())
			if z < -2479: 
				veto = 1; break
			#ID = ahit.GetDetectorID()
			#Nav = fGeo.GetCurrentNavigator()
			#Nav.CdNode(ID)
			#node = Nav.GetCurrentNode().GetName()
			#print node
			node = fGeo.FindNode(x,y,z).GetName()
			if node in ELOSS:
				if (z > -1968): t = ROOT.TMath.ATan2(a*y,b*x)+2*pi
				elif (z < -1968): t = ROOT.TMath.ATan2(asmall*y,b*x)+2*pi
				phisegment = ROOT.TMath.FloorNint((t+phistart)/dphi)%N
				ELOSS[node][phisegment] += ahit.GetEnergyLoss()
		if veto: continue #veto!
		
		hitSegments = 0
		for (iz, node) in enumerate(ELOSS.keys()):
			for (i, seg) in enumerate(ELOSS[node]):
				if seg > 0.045: 
					hitSegments += 1
					h['Seg_ID'].Fill(iz*N + i,wg)
					h['Seg_2D'].Fill(i,iz,wg)
				if seg: ELOSS[node][i] = 0
					
# start Track analysis	
		n_o_tracks = 0; 
		fittedTracks = {}
		for (key, atrack) in enumerate(sTree.FitTracks):
			if not checkFiducialVolume(sTree,key,dy): continue
			fitStatus = atrack.getFitStatus()
			if not fitStatus.isFitConverged() : continue
			nmeas = fitStatus.getNdf()
			if nmeas < measCut: continue
			chi2 = fitStatus.getChi2()/nmeas
			h['chi2'].Fill(chi2,wg)
			h['measVSchi2'].Fill(atrack.getNumPoints(),chi2,wg)
			if chi2/nmeas > 5: continue
			fittedTracks[n_o_tracks] = atrack
			n_o_tracks+=1			
		if n_o_tracks > 10: print n, "  ",n_o_tracks	
		h['N.o.Tracks'].Fill(n_o_tracks,wg)
		if n_o_tracks == 0 : h['Seg_ZeroTrack'].Fill(hitSegments ,wg)	
		if n_o_tracks == 1 : h['Seg_OneTrack'].Fill(hitSegments ,wg)	
		if n_o_tracks == 2 : h['Seg_TwoTrack'].Fill(hitSegments ,wg) 
		
		if n_o_tracks>2: n_o_tracks = 2 # noch nicht optimal!
		h['EventCat'].Fill(2*n_o_tracks + (hitSegments>0) ,wg)	
		cat[2*n_o_tracks + (hitSegments>0)] += wg
# ---
# loop over particles, 2-track combinations
		for HNL in sTree.Particles:			
			if not (n_o_tracks >= 2): continue 
			Pmuon = ROOT.TMath.Sqrt(sTree.MCTrack[0].GetPx()**2 +  sTree.MCTrack[0].GetPy()**2 +  sTree.MCTrack[0].GetPz()**2)
			#print Pmuon
			if Pmuon < 100: continue #######################################
			cuts = {}
			cuts["Mass"] = 0; cuts["POCA"] = 0;	cuts["DOCA"] = 0;	cuts["Tagger"] = 0;
			
			if not (n == lastEvent["2Track"]): 
				weight_2Track += wg	
				events_2Track += 1
				h['Multi'].Fill(multiplicity)
				lastEvent["2Track"] = n
			t1,t2 = HNL.GetDaughter(0),HNL.GetDaughter(1) 
# vertexing #############################################################				
			if not checkFiducialVolume(sTree,t2,dy) or not checkFiducialVolume(sTree,t1,dy) : continue
			PosDir = {} 
			checkMeasurements = True
			for tr in [t1,t2]:
				xx  = sTree.FitTracks[tr].getFittedState()
				PosDir[tr] = [xx.getPos(),xx.getDir()]
				fitStatus  = sTree.FitTracks[tr].getFitStatus()
				nmeas = fitStatus.getNdf()
				if nmeas < measCut: checkMeasurements = False	
			if not checkMeasurements: continue
			HNLPos = ROOT.TLorentzVector()
			HNL.ProductionVertex(HNLPos)
			HNLMom = ROOT.TLorentzVector()
			HNL.Momentum(HNLMom)
			cuts["2Track"] = 1
			if HNL.GetMother(1)==99 :
				xv,yv,zv,doca  =  HNLPos.X(),HNLPos.Y(),HNLPos.Z(),HNLPos.T()
			else:
				xv,yv,zv,doca, distx = myVertex(t1,t2,PosDir)
		# as we have learned, need iterative procedure
				dz = 99999.
				reps,states,newPosDir = {},{},{}
				parallelToZ = ROOT.TVector3(0., 0., 1.)
				rc = True
				step = 0
				while dz > 0.1:
					zBefore = zv
					newPos = ROOT.TVector3(xv,yv,zv)
					# make a new rep for track 1,2
					for tr in [t1,t2]:
						xx = sTree.FitTracks[tr].getFittedState()
						reps[tr] = ROOT.genfit.RKTrackRep(xx.getPDG())
						states[tr] = ROOT.genfit.StateOnPlane(reps[tr])
						reps[tr].setPosMom(states[tr],xx.getPos(),xx.getMom())
						try:
							reps[tr].extrapolateToPoint(states[tr], newPos, False)
						except:
							#print 'SHiPAna: extrapolation did not worked'
							rc = False
							break
						newPosDir[tr] = [reps[tr].getPos(states[tr]),reps[tr].getDir(states[tr])]
					if not rc: break
					xv,yv,zv,doca, distx = myVertex(t1,t2,newPosDir)
					dz = abs(zBefore-zv)
					step+=1
					if step > 10:
						#print 'abort iteration, too many steps, pos=',xv,yv,zv,' doca=',doca,'z before and dz',zBefore,dz
						rc = False
						break
				if not rc: continue
# vertexing end #########################################################
# cuts ##################################################################
			rE = xv**2/245**2 + yv**2/495**2
			if doca < docaCut: cuts["DOCA"] = 1;
			if rE < 1 and zv < 2570*u.cm and zv > ShipGeo['vetoStation'].z: cuts["POCA"] = 1
			if not hitSegments: cuts["Tagger"] = 1
			HNLMom = ROOT.TLorentzVector()
			HNL.Momentum(HNLMom)
			if HNLMom.M() > 0.1: cuts["Mass"] = 1
			Pmuon = ROOT.TMath.Sqrt(sTree.MCTrack[0].GetPx()**2 +  sTree.MCTrack[0].GetPy()**2 +  sTree.MCTrack[0].GetPz()**2)
			
			if Pmuon > 0:
				h["cutMassTagger"].Fill(cuts["Mass"], cuts["Tagger"],wg); h["cutMassTaggerU"].Fill(cuts["Mass"], cuts["Tagger"],1)
				h["cutMassPOCA"].Fill(cuts["Mass"], cuts["POCA"],wg); h["cutMassPOCAU"].Fill(cuts["Mass"], cuts["POCA"],1)
				h["cutMassDOCA"].Fill(cuts["Mass"], cuts["DOCA"],wg); h["cutMassDOCAU"].Fill(cuts["Mass"], cuts["DOCA"],1)
				h["cutMassDOCAPOCA"].Fill(cuts["Mass"], cuts["POCA"]*cuts["DOCA"],wg); h["cutMassDOCAPOCAU"].Fill(cuts["Mass"], cuts["POCA"]*cuts["DOCA"],1)
				h["cutTaggerDOCAPOCA"].Fill(cuts["Tagger"], cuts["POCA"]*cuts["DOCA"],wg); h["cutTaggerDOCAPOCAU"].Fill(cuts["Tagger"], cuts["POCA"]*cuts["DOCA"],1)
				h["cutTaggerPOCA"].Fill(cuts["Tagger"], cuts["POCA"],wg); h["cutTaggerPOCAU"].Fill(cuts["Tagger"], cuts["POCA"],1)
				h["cutTaggerDOCA"].Fill(cuts["Tagger"], cuts["DOCA"],wg); h["cutTaggerDOCAU"].Fill(cuts["Tagger"], cuts["DOCA"],1)
				h["cutDOCAPOCA"].Fill(cuts["DOCA"], cuts["POCA"],wg); h["cutDOCAPOCAU"].Fill(cuts["DOCA"], cuts["POCA"],1)
# cuts end ##############################################################	
			h['Doca'].Fill(doca,wg) 
			#h['Distx'].Fill(distx,wg) 
			if not hitSegments and not n == lastEvent["noTagger"]: 
					weight_noTagger += wg;
					events_noTagger += 1
					lastEvent["noTagger"] = n
					
			if  doca>docaCut : continue #DOCA cut
			if not n == lastEvent["DOCA"]: 
				weight_DOCA += wg
				lastEvent["DOCA"] = n
			r = ROOT.TMath.Sqrt(xv**2+yv**2)
			h['PocaXY'].Fill(xv,yv,wg)
			h['Poca'].Fill(zv,rE, wg)
			if zv < 2570*u.cm: h['PocaXY_z'].Fill(xv,yv,wg)
			if rE > 1 or zv > 2570*u.cm or zv < ShipGeo['vetoStation'].z: continue #POCA cut ################
			#print n
			
			if not n == lastEvent["POCA"]:
				weight_POCA += wg;
				events_POCA += 1
				lastEvent["POCA"] = n
				h['MultiPOCA'].Fill(multiplicity)
				h['P_muon'].Fill(Pmuon,wg)
				h['P_muonU'].Fill(Pmuon)
				h['Seg_TwoTrackPOCA'].Fill(hitSegments ,wg)
			
			
			info={}
			info['ID_t1']= sTree.fitTrack2MC[t1]
			info['ID_t2'] = sTree.fitTrack2MC[t2]
			info['P1'] = sTree.MCTrack[info['ID_t1']].GetP()
			info['P2'] = sTree.MCTrack[info['ID_t2']].GetP()
			info['Pt1'] = sTree.MCTrack[info['ID_t1']].GetPt()
			info['Pt2'] = sTree.MCTrack[info['ID_t2']].GetPt()
			info['PDG1'] = sTree.MCTrack[info['ID_t1']].GetPdgCode()
			info['PDG2'] = sTree.MCTrack[info['ID_t2']].GetPdgCode()
			
			#print n, Pmuon, multiplicity, wg, n_o_tracks, PDG1, PDG2 ,hitSegments
			
# Analysis of mothers ###################################################
			info['m1'] = sTree.MCTrack[info['ID_t1']].GetMotherId()
			info['m2'] = sTree.MCTrack[info['ID_t2']].GetMotherId()
			if info['PDG1']*info['PDG2'] == -121:
				if info['m1'] == info['m2'] and info['m1'] > -1: # electron pairs from the same Photon
					P_mother = sTree.MCTrack[info['m1']].GetP()
					P_Tmother = (sTree.MCTrack[info['m1']].GetPx()**2 + sTree.MCTrack[info['m1']].GetPy()**2)**(.5)
					PDG_mother = sTree.MCTrack[info['m1']].GetPdgCode()
					X_mother = sTree.MCTrack[info['m1']].GetStartX()
					Y_mother = sTree.MCTrack[info['m1']].GetStartY()
					Z_mother = sTree.MCTrack[info['m1']].GetStartZ()
					if PDG_mother == 22: 
						#print n, "gamma: ", info['m1'], P_mother, PDG_mother, HNLMom.M(), X_mother, Y_mother, Z_mother
						h['gamma_XY'].Fill(X_mother,Y_mother)
						h['gamma_XZ'].Fill(X_mother,Z_mother)
						h['gamma_P'].Fill(P_mother,P_Tmother)
						h['gamma_P_P_muon'].Fill(P_mother,sTree.MCTrack[0].GetP())
					#else???
						
				else: 
					x_1 = sTree.MCTrack[info['ID_t1']].GetStartX()
					y_1 = sTree.MCTrack[info['ID_t1']].GetStartY()
					z_1 = sTree.MCTrack[info['ID_t1']].GetStartZ()
					x_2 = sTree.MCTrack[info['ID_t2']].GetStartX()
					y_2 = sTree.MCTrack[info['ID_t2']].GetStartY()
					z_2 = sTree.MCTrack[info['ID_t2']].GetStartZ()
					node_1 = fGeo.FindNode(x_1,y_1,z_1).GetName()
					node_2 = fGeo.FindNode(x_2,y_2,z_2).GetName()
					for cm in [info['m1'],info['m2']]:
						while cm > -1:
							#print cm, sTree.MCTrack[cm].GetPdgCode(), sTree.MCTrack[cm].GetStartY()
							cm = sTree.MCTrack[cm].GetMotherId()
					if node_1 == node_2:
						#print n, "same Volume", info['m1'], info['m2'], sTree.MCTrack[info['m1']].GetPdgCode(), sTree.MCTrack[info['m2']].GetPdgCode()
						h['reaction_XY'].Fill(0.5*(x_1 + x_2) ,0.5*(y_1 + y_2))
						h['reaction_XZ'].Fill(0.5*(x_1 + x_2),0.5*(z_1 + z_2))
						h['reaction_P'].Fill(max(sTree.MCTrack[info['ID_t1']].GetP(),sTree.MCTrack[info['ID_t2']].GetP()),sTree.MCTrack[0].GetP())
						h['reaction_Pt_P'].Fill(max((sTree.MCTrack[info['ID_t1']].GetPx()**2 + sTree.MCTrack[info['ID_t1']].GetPy()**2)**(.5),(sTree.MCTrack[info['ID_t2']].GetPx()**2 + sTree.MCTrack[info['ID_t2']].GetPy()**2)**(.5)),max(sTree.MCTrack[info['ID_t1']].GetP(),sTree.MCTrack[info['ID_t2']].GetP()))
						h['reaction_phi'].Fill(math.acos((sTree.MCTrack[info['ID_t1']].GetPx()*sTree.MCTrack[0].GetPx() + sTree.MCTrack[info['ID_t1']].GetPy()*sTree.MCTrack[0].GetPy() + sTree.MCTrack[info['ID_t1']].GetPz()*sTree.MCTrack[0].GetPz())/sTree.MCTrack[info['ID_t1']].GetP()/sTree.MCTrack[0].GetP()))
					#else: print n, "diff Volume", info['m1'], info['m2'], sTree.MCTrack[info['m1']].GetPdgCode(), sTree.MCTrack[info['m2']].GetPdgCode()
			elif info['m1'] == info['ID_t2'] or info['m2'] == info['ID_t1']:
				ID_d = max(info['m1'], info['ID_t1'], info['m2'], info['ID_t2'])
				ID_m = min(abs(info['m1']), abs(info['ID_t1']), abs(info['m2']), abs(info['ID_t2']))
				X_daughter = sTree.MCTrack[ID_d].GetStartX()
				Y_daughter = sTree.MCTrack[ID_d].GetStartY()
				Z_daughter = sTree.MCTrack[ID_d].GetStartZ()
				#print n, "reaction: ", sTree.MCTrack[ID_m].GetP(), sTree.MCTrack[ID_m].GetPdgCode(), info['PDG1'], info['PDG2'], HNLMom.M(), X_daughter, Y_daughter,Z_daughter
				h['reaction_XY'].Fill(X_daughter,Y_daughter)
				h['reaction_XZ'].Fill(X_daughter,Z_daughter)
				h['reaction_P'].Fill(sTree.MCTrack[ID_d].GetP(),sTree.MCTrack[ID_m].GetP())
				h['reaction_Pt_P'].Fill((sTree.MCTrack[ID_d].GetPx()**2 + sTree.MCTrack[ID_d].GetPy()**2)**(.5),sTree.MCTrack[ID_d].GetP())
				h['reaction_phi'].Fill(math.acos((sTree.MCTrack[ID_m].GetPx()*sTree.MCTrack[ID_d].GetPx() + sTree.MCTrack[ID_m].GetPy()*sTree.MCTrack[ID_d].GetPy() + sTree.MCTrack[ID_m].GetPz()*sTree.MCTrack[ID_d].GetPz())/sTree.MCTrack[ID_m].GetP()/sTree.MCTrack[ID_d].GetP()))
			else: 
				X_reac = 0
				Y_reac = 0
				Z_reac = 0
				for cm in [info['m1'],info['m2']]:
					while cm > 0:
						#print cm, sTree.MCTrack[cm].GetPdgCode(), sTree.MCTrack[cm].GetStartY()
						X_reac = sTree.MCTrack[cm].GetStartX()
						Y_reac = sTree.MCTrack[cm].GetStartY()
						Z_reac = sTree.MCTrack[cm].GetStartZ()
						blabla = sTree.MCTrack[cm].GetPdgCode()
						cm = sTree.MCTrack[cm].GetMotherId()
				node_r = fGeo.FindNode(X_reac,Y_reac,Z_reac).GetName()
				node_1 = fGeo.FindNode(sTree.MCTrack[info['ID_t1']].GetStartX(),sTree.MCTrack[info['ID_t1']].GetStartY(),sTree.MCTrack[info['ID_t1']].GetStartZ()).GetName()
				node_2 = fGeo.FindNode(sTree.MCTrack[info['ID_t2']].GetStartX(),sTree.MCTrack[info['ID_t2']].GetStartY(),sTree.MCTrack[info['ID_t2']].GetStartZ()).GetName()
				if node_r == node_1 or node_r == node_2:
					#print n, "same Volume", info['m1'], info['m2'], sTree.MCTrack[info['ID_t1']].GetPdgCode(), sTree.MCTrack[info['ID_t2']].GetPdgCode()
					h['reaction_XY'].Fill(X_reac,Y_reac)
					h['reaction_XZ'].Fill(X_reac,Z_reac)
					h['reaction_P'].Fill(sTree.MCTrack[info['ID_t2']].GetP(),sTree.MCTrack[0].GetP())
					h['reaction_Pt_P'].Fill((sTree.MCTrack[info['ID_t2']].GetPx()**2 + sTree.MCTrack[info['ID_t2']].GetPy()**2)**(.5),sTree.MCTrack[info['ID_t2']].GetP())
					h['reaction_phi'].Fill(math.acos((sTree.MCTrack[info['ID_t2']].GetPx()*sTree.MCTrack[0].GetPx() + sTree.MCTrack[info['ID_t2']].GetPy()*sTree.MCTrack[0].GetPy() + sTree.MCTrack[info['ID_t2']].GetPz()*sTree.MCTrack[0].GetPz())/sTree.MCTrack[info['ID_t2']].GetP()/sTree.MCTrack[0].GetP()))
				#else: 
				#	print n, "diff Volume", info['m1'], info['m2'], sTree.MCTrack[info['ID_t1']].GetPdgCode(), sTree.MCTrack[info['ID_t2']].GetPdgCode()
				
	# Analysis of mothers end ###############################################

			if info['PDG1']*info['PDG2'] == -121: h['PDG_2Track'].Fill(0,wg)
			elif info['PDG1']*info['PDG2'] == -143: h['PDG_2Track'].Fill(1,wg)
			else: h['PDG_2Track'].Fill(2,wg)
			
			h['P1'].Fill(max(info['P1'],info['P2']),wg)
			h['P1'].Fill(min(info['P1'],info['P2']),wg)
			if (info['P1'] > info['P2']):
				h['Pt1'].Fill(info['Pt1'],wg)
				h['Pt2'].Fill(info['Pt2'],wg)
				h['eta1'].Fill(sTree.MCTrack[info['ID_t1']].GetRapidity(),wg)
				h['eta2'].Fill(sTree.MCTrack[info['ID_t2']].GetRapidity(),wg)
				#h['phi1'].Fill(sTree.MCTrack[info['ID_t1']].GetPhi(),wg,wg)
				#h['phi2'].Fill(sTree.MCTrack[info['ID_t2']].GetPhi(),wg,wg)
			else:
				h['Pt1'].Fill(info['Pt2'],wg)
				h['Pt2'].Fill(info['Pt1'],wg)
				h['eta1'].Fill(sTree.MCTrack[info['ID_t2']].GetRapidity(),wg)
				h['eta2'].Fill(sTree.MCTrack[info['ID_t1']].GetRapidity(),wg)
				#h['phi1'].Fill(sTree.MCTrack[info['ID_t2']].GetPhi(),wg,wg)
				#h['phi2'].Fill(sTree.MCTrack[info['ID_t1']].GetPhi(),wg,wg)
			
			if not n == lastEvent["Tagger"]:
				lastEvent["Tagger"] = n
				if not hitSegments:
 					weight_Tag1 += wg
					weight_Tag099 += wg
					weight_Tag09 += wg
				else: 
					weight_Tag099 += 0.01*wg
					weight_Tag09 += 0.1*wg
				weight_Seg095 += ((0.05)**hitSegments)*wg
				weight_Seg099 += ((0.01)**hitSegments)*wg
				
				#print n, "  ", Pmuon, "  ",multiplicity, "  ",wg, "  ", n_o_tracks
				#print t1, t2
				#print sTree.fitTrack2MC[t1], sTree.fitTrack2MC[t2]
				#print sTree.MCTrack[sTree.fitTrack2MC[t1]].GetPdgCode(), sTree.MCTrack[sTree.fitTrack2MC[t2]].GetPdgCode()
				if neutrino:
					sx = sTree.MCTrack[0].GetStartX()
					sy = sTree.MCTrack[0].GetStartY()
					sz = sTree.MCTrack[0].GetStartZ()
					RE = sx**2/250**2 + sy**2/500**2
					print sx, sy, RE, sz
			
			tr = ROOT.TVector3(0,0,ShipGeo.target.z0)
			t = 0
			for i in range(3):   t += HNLMom(i)/HNLMom.P()*(tr(i)-HNLPos(i)) 
			dist = 0
			for i in range(3):   dist += (tr(i)-HNLPos(i)-t*HNLMom(i)/HNLMom.P())**2
			dist = ROOT.TMath.Sqrt(dist)
			
			h['Px'].Fill(HNLMom(0)/HNLMom.P(),wg)
			h['Py'].Fill(HNLMom(1)/HNLMom.P(),wg)
			h['Pz'].Fill(HNLMom(2)/HNLMom.P(),wg)
			h['IP_target'].Fill(dist,wg)  
			h['HNL'].Fill(HNLMom.M(),wg)
			if info['PDG1']*info['PDG2'] == -121: h['HNL_ee'].Fill(HNLMom.M(),wg)
			if HNLMom.M() > 16: print "rec. mass: ", HNLMom.M()
			
			if HNLMom.M() > 0.1: 
				if not n == lastEvent["Mass"]: 
					weight_mass += ((0.01)**hitSegments)*wg #Mass cut
					lastEvent["Mass"] = n
			
			if dist < 30*u.m: 
				weight_IP += ((0.01)**hitSegments)*wg
			
	print "totalweight = ",totalweight
	print "weight_2Track = ", weight_2Track, " (", events_2Track,")"
	print "weight_noTagger = ", weight_noTagger, " (", events_noTagger,")"
	print "weight_DOCA = ", weight_DOCA
	print "weight_POCA = ", weight_POCA, " (", events_POCA,")"
	print ""
	print "weight_mass = ", weight_mass
	print "weight_Tag1 = ", weight_Tag1
	print "weight_Tag099 = ", weight_Tag099
	print "weight_Tag09 = ", weight_Tag09
	print "weight_Seg095 = ", weight_Seg095
	print "weight_Seg099 = ", weight_Seg099
	print "weight_IP = ", weight_IP
	print
	print cat
#
myEventLoop(nEvents)
writename = "shipAna_" +outfile+ ".root"
print "End Loop, Write Histograms: ", writename
ut.writeHists(h,writename)
print
