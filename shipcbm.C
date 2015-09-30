//makes histograms for most detector components
// use "CloneSignal" to reject events with less than 3 strawtube stations. 

#define shipcbmsim_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <iostream>
#include <TF1.h>
#include <list>
#include <TMath.h>
#include <numeric>

#include <func.h>
#include <shipnewDet.h>
#include <shipnewDetfunctions.h>
#include <BranchAddress.h>
void CloneSignal(std::vector<Int_t> SignalList, TString path, TString filename);
	
using namespace std;
   
void shipcbm(TString filename, TString path){
	
	Bool_t writeOption = 0;

	if (filename==""){filename = "10.0.Cosmics-TGeant4";}
	if (path ==""){path = ".";}
   
	cout<<path+"/ship."+filename+".root"<<endl;
	
	TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(path+"/ship."+filename+".root");
	if (!f || !f->IsOpen()) {
		f = new TFile(path+"/ship."+filename+".root");
	}
   
	TTree *tree = (TTree*)f->Get("cbmsim");
	shipcbmsim *t = new shipcbmsim(tree, writeOption, filename);
	
	if (writeOption) TFile *saveHist = new TFile("Histograms." + filename + ".root","RECREATE");
	Int_t correctSize = 1;
	
	if (writeOption) {saveHist->Close();	correctSize = t->TestArraySize(); 	saveHist = new TFile("Histograms." + filename + ".root","UPDATE");}
	if(correctSize){     
		//t->HistPlot();
		//t->Histveto();
		//t->newCosmics();
		//t->newMC();
		//if (writeOption) saveHist->Close();
		//std::vector<Int_t> SignalList = t->newVeto(); CloneSignal(SignalList,path,filename);
		//t->newGenie();
		//--------------------
		//t->newPlots();	
		std::vector<Int_t> SignalList = t->Signalquick(); CloneSignal(SignalList,path,filename);
	}
	f->Close();
}

void CloneSignal(std::vector<Int_t> SignalList, TString path, TString filename){		
		cout<<endl<<"Start writing Signalevents"<<endl;
		TFile *g = new TFile(path+"/ship."+filename+".root","OPEN");
		TTree *tree = g->Get("cbmsim");
		
		TFile *signalfile = new TFile("Signal."+filename+".root","recreate");
		TTree *signalevents = tree->CloneTree(0);
		Int_t K = 0;
		
		cout<<endl<<"Start Loop"<<endl;
		for (std::vector<Int_t>::iterator it = SignalList.begin(); it != SignalList.end(); ++it){
			if (!(K%1000)) cout<<K<<endl;
			tree->GetEntry(*it);
			signalevents->Fill();
			K++;
		}
		cout<<endl<<"End Loop"<<endl;
		signalevents->Write();
		signalfile->Close();
		cout<<K<<" Signal Events in this sample"<<endl;
}
