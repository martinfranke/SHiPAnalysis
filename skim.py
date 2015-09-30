#rejects events with no detector activity
#veto, ECal, muon, straw, RPC 
import ROOT, os, sys

a = sys.argv[1]


print "Skim root file: ",a,".root"


file_in = ROOT.TFile(a+".root",'OPEN')
sTree = file_in.cbmsim
nEvents   = sTree.GetEntries()


newName = a + "_skim.root"
newFile = ROOT.TFile(newName,'RECREATE')
smallTree = sTree.CloneTree(0)

noHits = 0
for i in range (nEvents):
	if not i%100000: print "",i
	rc = sTree.GetEntry(i)
	
	VP = sTree.vetoPoint
	j_VP = 0
	for a in VP:
		j_VP += 1
		break
		
	EPL = sTree.EcalPointLite
	j_EPL = 0
	for a in EPL:
		j_EPL += 1	
		break
		
	MP = sTree.muonPoint
	j_MP = 0
	for a in MP:
		j_MP += 1	
		break
	
	SP = sTree.strawtubesPoint
	j_SP = 0
	for a in SP:
		j_SP += 1
		break	

	#HPL = sTree.HcalPointLite
	j_HPL = 0
	#for a in HPL:
		#j_HPL += 1	
		#break

	SRP = sTree.ShipRpcPoint
	j_SRP = 0
	for a in SRP:
		j_SRP += 1	
		break	
		
	j = j_VP + j_EPL+ j_MP + j_SP + j_HPL + j_SRP
	
	if j == 0:
		noHits += 1
	else:
		smallTree.Fill()	
	
print "noHits;",noHits
smallTree.Write()
newFile.Close()
file_in.Close()


