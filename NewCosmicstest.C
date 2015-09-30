//much of the lastest SHiP CMBG for testing

#include <iostream>

#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TStopwatch.h>

#include "func.h"
using namespace std;



//double NEWs(const double * x, const double *pa){
	//double p = x[0];
	//double theta = pa[0];
	//if (p < 100){
		//theta = 180*theta/TMath::Pi();
		//double a = -0.8816/10000 /(1/theta  -0.1117/1000 * theta) - 0.1096 - 0.01966*TMath::Exp(-0.02040*theta);
		//double b = 0.4169/100 /(1/theta  -0.9891/10000 * theta) + 4.0395 - 4.3118*TMath::Exp(0.9235/1000*theta);
		//double c = -0.3516/1000 * theta*theta + 0.8861/100 * theta - 2.5985 -0.8745/100000*TMath::Exp(0.1457*theta);
		//return pa[1] * 10000*TMath::Power(10,(a * TMath::Power(TMath::Log(p),2) + b * TMath::Log(p) + c ));		
	//}
	//else{return 1400*TMath::Power(p,-2.7)*(1/(1+p/115)+0.054/(1+p/850));} 
//}


class Co3Rng{
 public:
   Co3Rng() {
		rng = new TRandom3(gRandom->GetSeed());
		fTheta = new TF1("f2","cos(x)*cos(x)",0,1.3734); // theta_zenith up to 78.7deg
		fSpectrumH = new TF1("f4","TMath::Power(x,-2.7) * 1400 *(1/(1+x/115)+0.054/(1+x/850))",100,1000); // momentum above 100GeV
		fTheta->SetNpx(100);
	};
   virtual ~Co3Rng() {delete rng; delete fTheta; delete fSpectrumH;};
   double Uniform(Float_t min, Float_t max){return rng->Uniform(min,max);};
   TF1 *fSpectrumH;
   TF1 *fTheta;
   double NEWstest(double theta);
 private:
   TRandom3 *rng; //!
};

double Co3Rng::NEWstest(double theta){
	
	theta = 180*theta/TMath::Pi();
	double a = -0.8816/10000 /(1/theta  -0.1117/1000 * theta) - 0.1096 - 0.01966*TMath::Exp(-0.02040*theta);
	double b = 0.4169/100 /(1/theta  -0.9891/10000 * theta) + 4.0395 - 4.3118*TMath::Exp(0.9235/1000*theta);
	double bstrich = b  + 1/TMath::Ln10();
	double gamma = sqrt(-TMath::Ln10()*a), offset = 0.5*bstrich/a;
	double norm = TMath::Erf(gamma*(TMath::Log(100)+offset)) - TMath::Erf(gamma*offset);
	double r3 = rng->Uniform();
	return exp(TMath::ErfInverse(r3*norm+TMath::Erf(gamma*offset))/gamma-offset);
	
}

void NewCosmicstest(){
	//gROOT->Reset();	
	
	TStopwatch *clock0 = new TStopwatch();
	TFile *fout = new TFile("Cosmictest.root","RECREATE");
	bool high = 1;
	
	//Float_t z0 = 1400;
	Float_t z0 = 0;
	Float_t yTop = 600;
	Float_t xTop = 300;
	Float_t zTop = 3650;
	
	Float_t xdist = 3000;
	Float_t zdist = 9000;
	
	TH2D *StartXZ = new TH2D("xz","xz;x[cm];z[cm]",30,-(xdist/2),xdist/2,90,z0 - 4500,z0 + 4500);
	TH1D *StartTheta = new TH1D("#theta","#theta; #theta_Zenith",100,0,2);
	TH1D *StartPhi = new TH1D("#phi","#phi; #phi",50,0,7);
	TH1D *StartPHigh = new TH1D("PHigh","P; P[GeV]",100,-1,3);
	TH1D *StartP = new TH1D("P","P;P[GeV]",100,-1,3);
	TH1D *StartPLow = new TH1D("PLow","P;P[GeV]",100,-1,3);
	//TH1D *StartP = new TH1D("P","P;P[GeV]",100,0.1,1000);
	BinLogX(StartP);
	BinLogX(StartPHigh);
	BinLogX(StartPLow);
	
	TH2D *StartPTheta = new TH2D("P,Theta","P-Theta;P[GeV];#theta",150,-1,3,50,0,2);
	BinLogX(StartPTheta);
	
	TH2D *MCXZ = new TH2D("MCxz","xz;x[cm];z[cm]",30,-(xdist/2),xdist/2,90,z0 - 4500,z0 + 4500);
	TH1D *MCTheta = new TH1D("MC#theta","#theta; #theta",100,0,2);
	TH1D *MCPhi = new TH1D("MC#phi","#phi, #phi",50,0,7);
	
	TH1I *MCnTry = new TH1I("nTry","nTry",100,4.6,5);
	
	Double_t totalweightsum=0;
	Double_t px,py,pz,x,y,z,w, weighttest, weight;
	Int_t nTry,nInside,nEvent,nTest;	
	Co3Rng *fRandomEngine = new Co3Rng();
	
	int EVENTS = 400000;
	Int_t kmax = 40000;
	float weight1,weight1Low,weight1High;
	weight1Low = 123*xdist*zdist/EVENTS/10000; // expected #muons per spill/ #simulated events per spill 174*30*90/500000
	cout<<weight1Low<<endl;
	
	double I = fRandomEngine->fSpectrumH->Integral(100,1000);
	weight1High = 2*TMath::Pi()/3*I*xdist*zdist/EVENTS/10000;
		//weight2 = 900/I; // 1/(mean momentum weight), P_max-P_min/(3*0.3044/2pi)
	
	cout<< weight1High<<endl;
	Float_t weight3 = 4.833931503; // MC average of nTry/nEvents 4.833949997 +- 0.000010494			
	weight1 = 1;								
	weight = weight1 / weight3;
	
	nInside = 0; nEvent = 0; nTest = 0; weighttest = 0;
	y = 1900; //20m over beam axis

	w = weight/kmax;
	clock0->Start();
	for(Int_t k = 0;k<kmax;k++){
		cout<<k<<endl;
		nTry =0;
		for(Int_t i=0;i<EVENTS;i++){
		   Bool_t hit = 0;   
		   do{
				// shower characteristics
			   double phi = fRandomEngine->Uniform(0,2*TMath::Pi());
			   double theta = fRandomEngine->fTheta->GetRandom();
			  				  			    		 
	   	   //momentum components
				px = TMath::Sin(phi)*TMath::Sin(theta); 
				pz = TMath::Cos(phi)*TMath::Sin(theta);
				py = -TMath::Cos(theta);
				
				// start position, area 1120m^2
				x = fRandomEngine->Uniform(-xdist/2,xdist/2);
				z = fRandomEngine->Uniform(z0 - zdist/2, z0 + zdist/2);
				
				// claim for flight close to the actual detector
				if((abs(x-(y+yTop)*px/py) < xTop && abs(z-z0-(y+yTop)*pz/py) < zTop) || (abs(x-(y-yTop)*px/py) < xTop && abs(z-z0-(y-yTop)*pz/py) <  zTop)|| abs(y-(x+xTop)*py/px)<yTop && abs(z-z0-(x+xTop)*pz/px)<zTop || abs(y-(x-xTop)*py/px)<yTop && abs(z-z0-(x-xTop)*pz/px)<zTop){
					
					// muon momentum
					double P;
					//if (!high) {P = fRandomEngine->NEWstest(theta);}
					//else {P = fRandomEngine->fSpectrumH->GetRandom();}
					// high
					P = fRandomEngine->fSpectrumH->GetRandom();
					w = weight1High/weight3/kmax;
					StartP->Fill(P,w);
					StartPHigh->Fill(P,w);
					StartPTheta->Fill(P,theta,w);
					// low
					P = fRandomEngine->NEWstest(theta);
					w = weight1Low/weight3/kmax;
					StartP->Fill(P,w);
					StartPLow->Fill(P,w);
					StartPTheta->Fill(P,theta,w);
					
					
					px = px*P;
					py = py*P;
					pz = pz*P;
					
					//muon or anti-muon
					hit = 1; nInside++; 
					// StartP->Fill(P,w);
					
					w = weight1Low/weight3/kmax + weight1High/weight3/kmax;
					
					StartTheta->Fill(theta,w); // kein Weight!
					StartPhi->Fill(phi,w);	// kein Weight!	
					StartXZ->Fill(x,z,w); // kein Weight!	
					StartPTheta->Fill(P,theta,w);	
			   }
			   nTry++;
				weighttest += w;
				MCTheta->Fill(theta,w); // kein Weight!
				MCPhi->Fill(phi,w);
				MCXZ->Fill(x,z,w);
				nTest++;
			}while(!hit);
			nEvent++;
		}
		MCnTry->Fill(1.0*nTry/EVENTS);
	}
	clock0->Stop();
	
	delete fRandomEngine;
	cout<<nEvent<<" events have been generated."<<endl;
	cout<<"There is a total of "<<nInside<<"/"<<nTest<<" muons that passed close enough to the detector."<<endl;
	cout<<"Including the given weight this corresponds to ";
	cout<<kmax*weighttest/xdist/zdist*10000/123.3044<<" spills (1 spill = "<<xdist*zdist*123.3044/10000;
	cout<<" real cosmic muons = "<<EVENTS<<" simulated events)."<<endl;
   
   cout<<weighttest<<endl;
   clock0->Print();
   
   Double_t meanflux = 0;
   Int_t binsum = 0;
   for (Int_t ix = 2; ix<29;ix++){
	   for (Int_t iz = 2; iz<89;iz++){
		 	binsum++;
		   meanflux += MCXZ->GetBinContent(ix,iz);
		}
	}
	cout<< "meanflux: "<<meanflux/binsum<<"  "<< meanflux<<endl<<endl;
   printf("MCnTry: %.9f +- %.9f",MCnTry->GetMean(),MCnTry->GetMeanError());
   cout<<endl<<endl;
     
    TCanvas *c1 = new TCanvas("c1","c1",400,400);
	 c1->Divide(1,1);
	 c1->cd(1);
	 MCnTry->DrawCopy();
	
	 TCanvas *c4 = new TCanvas("c4","c4",400,400);
	 c4->Divide(1,1);
	 c4->cd(1);
	 StartPTheta->DrawCopy("SURF2");
	 gPad->SetLogx();
	
	 //TCanvas *c2 = new TCanvas("c2","c2",400,400);
	 //c2->Divide(1,1);
	 //c2->cd(1);
	 //gPad->SetLogy();
	 //wei->DrawCopy();
	
    TCanvas *c3 = new TCanvas("c3","c3",1600,800);
	 c3->Divide(4,2);
	 c3->cd(1);
	 StartXZ->DrawCopy("COLZ");
	 c3->cd(2);
	 //MCP->SetLineColor(kGreen);
	// MCP->DrawCopy();
	 //StartP->DrawCopy();
	 //TF1 *fs = new TF1("fs",NEWs,1,100,2);
	 //fs->FixParameter(0, 0);
	 //fs->FixParameter(1, 500);
	 //StartP->Add(fs,-1);
	 StartP->DrawCopy();
	 //StartP->Fit(fs,"I");
	 
	 
	// 
	 //fs->DrawCopy("SAME");
	 
	 //gPad->SetLogy();
	 gPad->SetLogx();
	 c3->cd(3);
	 MCTheta->SetLineColor(kGreen);
	 MCTheta->DrawCopy();
	 StartTheta->DrawCopy("SAME");
	 //TF1 *f1 = new TF1("f1","[0]*cos(x)*cos(x)",1.57,3.14);
	 //StartTheta->Fit(f1,"","",1.57,3.14);	
	 gPad->SetLogy(0);
	 gPad->SetLogx(0);
	 c3->cd(4);
	 MCPhi->SetLineColor(kGreen);
	 MCPhi->DrawCopy();
	 StartPhi->DrawCopy("SAME");
	//TF1 *f1 = new TF1("f1","[0]*cos(x)*cos(x)",1.57,3.14);
	//StartTheta->Fit(f1,"","",1.57,3.14);	
	 gPad->SetLogy(0);
	 gPad->SetLogx(0);
	 c3->cd(5);
	 MCXZ->DrawCopy("COLZ");
	 c3->cd(6);
	 //MCP->Divide(StartP);
	// MCP->DrawCopy();
	 gPad->SetLogy(0);
	 gPad->SetLogx(0);
	 c3->cd(7);
	 //MCTheta->Divide(StartTheta);
	 MCTheta->DrawCopy();
	 gPad->SetLogy();
	 gPad->SetLogx(0);
	 c3->cd(8);
	 //MCPhi->Divide(StartPhi);
	 MCPhi->DrawCopy();
	 gPad->SetLogy(0);
	 gPad->SetLogx(0);
	 c3->Update();
	 c3->SaveAs("Start.png");
	 
	 
	 
	 
	 StartXZ->Write();
	 StartTheta->Write();
	 StartPhi->Write();
	 StartP->Write();
	 StartPLow->Write();
	 StartPHigh->Write();
	
    StartPTheta->Write();
		
	 MCXZ->Write();
	 MCTheta->Write();
	 MCPhi->Write();
	
	 MCnTry->Write();
	 fout ->Close();
	 
	 
  
}
