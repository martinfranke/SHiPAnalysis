import shipunit as u
from ShipGeoConfig import ConfigRegistry
import ROOT 
from ROOT import *
from ctypes import *
import codecs

def loadGeometry(geofile, dy):
    # init geometry and mag. field
    ShipGeo = ConfigRegistry.loadpy("$FAIRSHIP/geometry/geometry_config.py", Yheight = dy )
    # -----Create geometry----------------------------------------------
    import shipDet_conf
    tgeom = ROOT.TGeoManager("Geometry", "Geane geometry")
    gMan  = tgeom.Import(geofile)
    fGeo = ROOT.gGeoManager
    return {'fGeo':fGeo,'gMan':gMan, 'ShipGeo':ShipGeo}

def findDimensionBoxElement(node):
    sh = node.GetVolume().GetShape()
    if sh.IsCylType():
        #print node.GetName(), " is a cylinder", sh.GetRmin(), sh.GetRmax()
        if sh.HasRmin(): r = (sh.GetRmax() - sh.GetRmin())
        else: r = 0.
        return {'x':sh.GetDX(),
                'y':sh.GetDY(),
                'z':sh.GetDZ(),
                'r':r}
    return {'x':sh.GetDX(),
            'y':sh.GetDY(),
            'z':sh.GetDZ(),
            'r':0}

def findPositionElement(node,topPos):
    local_pos = node.GetMatrix().GetTranslation()
    pos = []
    for i in xrange(3):
        pos.append(local_pos[i]+topPos[i])
    sh = node.GetVolume().GetShape()
    if sh.IsCylType():
        if sh.HasRmin(): r = (sh.GetRmax() - sh.GetRmin())/2.
        else: r = sh.GetRmax()
        return {'x':pos[0],
                'y':pos[1],
                'z':pos[2],
                'r':r}
    return {'x':pos[0],
            'y':pos[1],
            'z':pos[2],
            'r':0}
                        
def getNextNode(d,j,k,depth,topPos, node,verbose, fGeo):
	posi = findPositionElement(node,topPos)
	dim = findDimensionBoxElement(node)
	nav = fGeo.GetCurrentNavigator()
	nav.SetCurrentPoint(posi['x'],posi['y'],posi['z'])
	ID = nav.GetCurrentNodeId()
	#ID = 0 #node.GetId()
	if verbose:
		print j,depth, k,node.GetName()
		print "            x: ", posi['x'],dim['x']
		print "            y: ", posi['y'],dim['y']
		print "            z: ", posi['z'],dim['z']
		print "            id: ", ID
		
	d[node.GetName()] = {'x': {}, 'y':{}, 'z':{}, 'r':{}}
	d[node.GetName()]['x']['pos'] = posi['x']
	d[node.GetName()]['x']['dim'] = dim['x']
	d[node.GetName()]['y']['pos'] = posi['y']
	d[node.GetName()]['y']['dim'] = dim['y']
	d[node.GetName()]['z']['pos'] = posi['z']
	d[node.GetName()]['z']['dim'] = dim['z']
	d[node.GetName()]['r']['pos'] = posi['r']
	d[node.GetName()]['r']['dim'] = dim['r']
	d[node.GetName()]['id'] = ID
	# Next children
	ns = node.GetNodes()
	if ns:
		if depth == 2: return d
		for (k,n) in enumerate(ns):
			d = getNextNode(d,j,k,depth + 1,topPos, n, verbose,fGeo)
	return d

def getNodeList(fGeo, verbose=False):
    d = {}
    nodes = fGeo.GetTopVolume().GetNodes()
    for (j,node) in enumerate(nodes):
		d = getNextNode(d,j,0,0,node.GetMatrix().GetTranslation(), node, verbose, fGeo)
    return d

def BinLogX(h):
	#Transforms X-Axis of 1D Histogram into log scale binning
	#fix Uebergabe an Pointer auf Array!
	axis = h.GetXaxis();
	bins = axis.GetNbins();
	 
	low = axis.GetXmin();
	high = axis.GetXmax();
	width = (high - low) / bins;
	
	a = map(lambda x: 10**(low + x * width), range(bins+1))
	new_bins = ((bins+1)*c_float)(*a)
	axis.Set(bins, new_bins);
	return h
def BinLogY(h):
	#Transforms X-Axis of 1D Histogram into log scale binning
	#fix Uebergabe an Pointer auf Array!
	axis = h.GetYaxis();
	bins = axis.GetNbins();
	 
	low = axis.GetXmin();
	high = axis.GetXmax();
	width = (high - low) / bins;
	
	a = map(lambda x: 10**(low + x * width), range(bins+1))
	new_bins = ((bins+1)*c_float)(*a)
	axis.Set(bins, new_bins);
	return h

def PDGbin(x):
	pdg = {13:0,211:1,11:2,22:4,130:7,321:9,2212:10,2112:11,3122:12,111:6,310:8,9900014:5,12:3,14:3,16:3}
	bin = 13
	if pdg[x]: bin = pdg[x]
	return bin;

def SetPDGLabel(h):
	h.GetXaxis().SetBinLabel(1,"#mu");
	h.GetXaxis().SetBinLabel(2,"#pi^{#pm}");
	h.GetXaxis().SetBinLabel(3,"e");
	h.GetXaxis().SetBinLabel(4,"#nu");
	h.GetXaxis().SetBinLabel(5,"#gamma");
	h.GetXaxis().SetBinLabel(6,"N");
	h.GetXaxis().SetBinLabel(7,"#pi^{0}");
	h.GetXaxis().SetBinLabel(8,"K_{L}");
	h.GetXaxis().SetBinLabel(9,"K_{S}");
	h.GetXaxis().SetBinLabel(10,"K^{#pm}");
	h.GetXaxis().SetBinLabel(11,"p");
	h.GetXaxis().SetBinLabel(12,"n");
	h.GetXaxis().SetBinLabel(13,"#Lambda");
	h.GetXaxis().SetBinLabel(14,"others");
	h.SetLabelSize(0.07);
	h.SetLabelOffset(0.01);
#	h.SetMinimum(1);		

def CalcBinError(h):
	totentries = h.GetEntries()
	weightsum = 0.0
	for x in range(1,h.GetNbinsX()+1):
		weightsum += h.GetBinContent(x) 
	
	weightperentry = weightsum/totentries
	for x in range(1,h.GetNbinsX()+1):
		h.SetBinError(x,ROOT.TMath.Sqrt(h.GetBinContent(x)/weightperentry) * weightperentry)
	
def finishandprint(outfile, weights):
	f2 = codecs.open(outfile, 'w')
	f2.write(str(weights['rec'][0]) + ", "+str(weights['rec'][1])+ ', ' + str(weights['rec'][2]) + ', ' + str(weights['rec'][3])+"\n")
	f2.write(str(weights['2tracks'][0]) + ", "+str(weights['2tracks'][1])+ ', ' + str(weights['2tracks'][2]) + ', ' + str(weights['2tracks'][3])+"\n")
	f2.write(str(weights['POCA'][0]) + ", "+str(weights['POCA'][1]) + ", "+str(weights['POCA'][2]) + ", "+str(weights['POCA'][3])+"\n")
	f2.write(str(weights['tracks'][0]) + ", "+str(weights['tracks'][1])+ ", "+str(weights['tracks'][2])+ ", "+str(weights['tracks'][3])+"\n")
	f2.write(str(weights['ndof'][0]) + ", "+str(weights['ndof'][1])+ ", "+str(weights['ndof'][2])+ ", "+str(weights['ndof'][3])+"\n")
	f2.write(str(weights['chi'][0]) + ", "+str(weights['chi'][1])+ ", "+str(weights['chi'][2])+ ", "+str(weights['chi'][3])+"\n")
	f2.write(str(weights['DOCA'][0]) + ", "+str(weights['DOCA'][1])+ ", "+str(weights['DOCA'][2])+ ", "+str(weights['DOCA'][3])+"\n")
	f2.write(str(weights['P'][0]) + ", "+str(weights['P'][1]) + ", "+str(weights['P'][2]) + ", "+str(weights['P'][3])+"\n")
	f2.write(str(weights['IP'][0]) + ", "+str(weights['IP'][1])+ ", "+str(weights['IP'][2])+ ", "+str(weights['IP'][3])+"\n")
	f2.write(str(weights['noveto'][0]) + ", "+str(weights['noveto'][1])+ ", "+str(weights['noveto'][2])+ ", "+str(weights['noveto'][3])+"\n")
	f2.write(str(weights['ECAL'][0]) + ", "+str(weights['ECAL'][1])+ ", "+str(weights['ECAL'][2])+ ", "+str(weights['ECAL'][3])+"\n")
	f2.write(str(weights['Mu1'][0]) + ", "+str(weights['Mu1'][1])+ ", "+str(weights['Mu1'][2])+ ", "+str(weights['Mu1'][3])+"\n")
	f2.write(str(weights['Mu2'][0]) + ", "+str(weights['Mu2'][1])+ ", "+str(weights['Mu2'][2])+ ", "+str(weights['Mu2'][3])+"\n")
	f2.close()
	print "################################################"
	print "Rec   ", weights['rec'][0]   , weights['rec'][1]
	print "2tracks  ", weights['2tracks'][0]  , weights['2tracks'][1], 
	if weights['rec'][0]: print 1.0*weights['2tracks'][0]/weights['rec'][0]
	else: print
	print "POCA  ", weights['POCA'][0]  , weights['POCA'][1], 
	if weights['2tracks'][0]: print 1.0*weights['POCA'][0]/weights['2tracks'][0]
	else: print
	print "tracks", weights['tracks'][0], weights['tracks'][1], 
	if weights['POCA'][0]: print 1.0*weights['tracks'][0]/weights['POCA'][0]
	else: print
	print "ndof  ", weights['ndof'][0]  , weights['ndof'][1]  , 
	if weights['tracks'][0]: print 1.0*weights['ndof'][0]/weights['tracks'][0]
	else: print
	print "chi2/n", weights['chi'][0]   , weights['chi'][1]   , 
	if weights['ndof'][0]: print 1.0*weights['chi'][0]/weights['ndof'][0]
	else: print
	print "DOCA  ", weights['DOCA'][0]  , weights['DOCA'][1]  , 
	if weights['chi'][0]: print 1.0*weights['DOCA'][0]/weights['chi'][0]
	else: print
	print "P     ", weights['P'][0]     , weights['P'][1]     , 
	if weights['DOCA'][0]: print 1.0*weights['P'][0]/weights['DOCA'][0]
	else: print
	print "IP    ", weights['IP'][0]    , weights['IP'][1]    , 
	if weights['P'][0]: print 1.0*weights['IP'][0]/weights['P'][0]
	else: print
	print "veto  ", weights['noveto'][0], weights['noveto'][1],
	if weights['IP'][0]: print 1.0*weights['noveto'][0]/weights['IP'][0]
	else: print
	print "ECAL  ", weights['ECAL'][0]  , weights['ECAL'][1]  , 
	if weights['noveto'][0]: print 1.0*weights['ECAL'][0]/weights['noveto'][0]
	else: print
	print "Mu1   ", weights['Mu1'][0]   , weights['Mu1'][1]   , 
	if weights['ECAL'][0]: print 1.0*weights['Mu1'][0]/weights['ECAL'][0]
	else: print
	print "Mu2   ", weights['Mu2'][0]   , weights['Mu2'][1]   , 
	if weights['Mu1'][0]: print 1.0*weights['Mu2'][0]/weights['Mu1'][0]
	else: print
	
	print "Rec   ", weights['rec'][2]   , weights['rec'][3]
	print "2tracks  ", weights['2tracks'][2]  , weights['2tracks'][3], 1.0*weights['2tracks'][2]/weights['rec'][2]
	print "POCA  ", weights['POCA'][2]  , weights['POCA'][3], 1.0*weights['POCA'][2]/weights['rec'][2]
	print "tracks", weights['tracks'][2], weights['tracks'][3], 1.0*weights['tracks'][2]/weights['rec'][2]
	print "ndof  ", weights['ndof'][2]  , weights['ndof'][3]  , 1.0*weights['ndof'][2]/weights['rec'][2]
	print "chi2/n", weights['chi'][2]   , weights['chi'][3]   , 1.0*weights['chi'][2]/weights['rec'][2]
	print "DOCA  ", weights['DOCA'][2]  , weights['DOCA'][3]  , 1.0*weights['DOCA'][2]/weights['rec'][2]
	print "P     ", weights['P'][2]     , weights['P'][3]   ,1.0*weights['P'][2]/weights['rec'][2]
	print "IP    ", weights['IP'][2]    , weights['IP'][3]  ,1.0*weights['IP'][2]/weights['rec'][2]
	print "veto  ", weights['noveto'][2], weights['noveto'][3], 1.0*weights['noveto'][2]/weights['rec'][2]
	print "ECAL  ", weights['ECAL'][2]  , weights['ECAL'][3],1.0*weights['ECAL'][2]/weights['rec'][2]
	print "Mu1   ", weights['Mu1'][2]   , weights['Mu1'][3] , 1.0*weights['Mu1'][2]/weights['rec'][2]
	print "Mu2   ", weights['Mu2'][2]   , weights['Mu2'][3] , 1.0*weights['Mu2'][2]/weights['rec'][2]

	
