Int_t shipcbmsim::TestArraySize(){
   // function tests array sizes by comparing kMaxcbmroot_X_XPoint and XPoint_
   // returns 0 with kMaxcbmroot_X_XPoint is too small for the given sample
   // returns 1 if array size is large enough
	cout<<"Test Array Sizes"<<endl;
	fChain->SetBranchStatus("*",0);
 	fChain->SetBranchStatus("vetoPoint",1);
	fChain->SetBranchStatus("EcalPoint",1);
	fChain->SetBranchStatus("EcalPointLite",1);
	fChain->SetBranchStatus("muonPoint",1);
	fChain->SetBranchStatus("MCTrack",1);
	fChain->SetBranchStatus("MCTrack.fW",1);
	fChain->SetBranchStatus("strawtubesPoint",1);
	fChain->SetBranchStatus("HcalPoint",1);
	fChain->SetBranchStatus("HcalPointLite",1);
	fChain->SetBranchStatus("ShipRpcPoint",1);
	fChain->SetBranchStatus("TargetPoint",1);
	 
	TH1D *vetoHits = new TH1D("ArraySize-vetoHits","vetoHits",100,0,6.602);
	BinLogX(vetoHits);

	Long64_t nentries = fChain->GetEntries();
   
	Int_t tmax = 0; Int_t t0 = 0; int sumt = 0;// MCTrack
	Int_t vmax = 0; Int_t v0 = 0; int sumv = 0;// veto
	Int_t smax = 0; Int_t s0 = 0; int sums = 0;// strawtubes
	Int_t emax = 0; Int_t e0 = 0; int sume = 0;// ECAL
	Int_t elmax = 0; Int_t el0 = 0; int sumel = 0;// ECALLite
	Int_t mmax = 0; Int_t m0 = 0; int summ = 0;// MUON
	Int_t SRmax = 0; Int_t SR0 = 0; int sumSR = 0; //SHiPRpc
	Int_t hmax = 0; Int_t h0 = 0;  int sumh = 0;//Hcal
	Int_t hlmax = 0; Int_t hl0 = 0; int sumhl = 0;//HcalLite
	Int_t tarmax = 0; Int_t tar0 = 0; int sumtar = 0;//Target
   
	Int_t noHitsOverall = 0;
	
	
	Int_t nIntense = 0;
 //start loop
	for(Int_t j = 0; j<nentries;j++){
		fChain->GetEntry(j);
	    
		if (MCTrack_ > tmax) tmax = MCTrack_;
		if (vetoPoint_ > vmax) vmax = vetoPoint_;
		if (strawtubesPoint_ > smax) smax = strawtubesPoint_;
		if (EcalPoint_ > emax) emax = EcalPoint_;
		if (EcalPointLite_ > elmax) elmax = EcalPointLite_;
		if (muonPoint_ > mmax) mmax = muonPoint_;
		if (ShipRpcPoint_ > SRmax) SRmax = ShipRpcPoint_;
		if (HcalPoint_ > hmax) hmax = HcalPoint_;
		if (HcalPointLite_ > hlmax) hlmax = HcalPointLite_;
		if (TargetPoint_ > tarmax) tarmax = TargetPoint_;
		
		if (MCTrack_ == 0 ) t0++; else sumt += MCTrack_;
		if (vetoPoint_ == 0 ) v0++; else sumv += vetoPoint_;
		if (strawtubesPoint_ == 0 ) s0++; else sums += strawtubesPoint_;
		if (EcalPoint_ == 0 ) e0++; else sume += EcalPoint_;
		if (EcalPointLite_ == 0 ) el0++; else sumel += EcalPointLite_;
		if (muonPoint_ == 0 ) m0++; else summ += muonPoint_;
		if (ShipRpcPoint_ == 0 ) SR0++; else sumSR += ShipRpcPoint_;
		if (HcalPoint_ == 0 ) h0++; else sumh += HcalPoint_;
		if (HcalPointLite_ == 0 ) hl0++; else sumhl += HcalPointLite_;
		if (TargetPoint_ == 0 ) tar0++; else sumtar += TargetPoint_;
		  
		if (vetoPoint_ == 0 && strawtubesPoint_ == 0 && EcalPointLite_ == 0 && muonPoint_ == 0 && HcalPointLite_ == 0 && ShipRpcPoint_ == 0 && TargetPoint_ == 0 ) noHitsOverall++;
		vetoHits->Fill(vetoPoint_*MCTrack_fW[0]);
	}
	
	sumt = sumt/(nentries - t0);
	sumv = sumv/(nentries - v0);
	sume = sume/(nentries - e0);
	sumel = sumel/(nentries - el0);
	summ = summ/(nentries - m0);
	sums = sums/(nentries - s0);
	sumSR = sumSR/(nentries - SR0);
	sumh = sumh/(nentries - h0);
	sumhl = sumhl/(nentries - hl0);
	sumtar = sumtar/(nentries - tar0);
		
	t0 = 100.0*t0/nentries;
	v0 = 100.0*v0/nentries;
	e0 = 100.0*e0/nentries;
	el0 = 100.0*el0/nentries;
	m0 = 100.0*m0/nentries;
	s0 = 100.0*s0/nentries;
	SR0 = 100.0*SR0/nentries;
	h0 = 100.0*h0/nentries;
	hl0 = 100.0*hl0/nentries;
	tar0 = 100.0*tar0/nentries;
	
	cout<<"Number of entries: "<<nentries<<endl<<endl;
	cout<<"tmax   vmax    smax    emax     elmax    mmax    SRmax    hmax    hlmax    tarmax"<<endl;
	cout<<tmax<<"  "<<vmax<<"  "<<smax<<"  "<<emax<<"  "<<elmax<<"  "<<mmax<<"  "<<SRmax<<"  "<<hmax<<"  "<<hlmax<<"  "<<tarmax<<endl<<endl;
  
	cout<<"t0  v0  s0  e0   el0  m0   SR0  h0   hl0   tar0"<<endl;
	cout<<t0<<"\%  "<<v0<<"\%  "<<s0<<"\%  "<<e0<<"\%  "<<el0<<"\%  "<<m0<<"\%  "<<SR0<<"\%  "<<h0<<"\%  "<<hl0<<"\%  "<<tar0<<"\%"<<endl<<endl;
   
   cout<<"mean t   v   s   e    el   m    SR   h    hl    tar "<<endl;
	cout<<"    "<<sumt<<"  "<<sumv<<"  "<<sums<<"  "<<sume<<"  "<<sumel<<"  "<<summ<<"  "<<sumSR<<"  "<<sumh<<"  "<<sumhl<<"  "<<sumtar<<""<<endl<<endl;
	cout<<"NoHitsOverall: "<<100.0*noHitsOverall/nentries<<"\%"<<endl<<endl; 
	
	if (writeOption){
		TFile *saveHist = new TFile("Histograms." + filename + ".root","UPDATE");
		vetoHits->Write();
		saveHist->Close();
	}
	
	delete vetoHits;

	if(tmax>kMaxcbmroot_Stack_MCTrack || vmax>kMaxcbmroot_veto_vetoPoint || smax > kMaxcbmroot_strawtubes_strawtubesPoint || emax>kMaxcbmroot_Ecal_EcalPoint || elmax>kMaxcbmroot_EcalLite_EcalPointLite || mmax>kMaxcbmroot_muon_muonPoint || SRmax>kMaxcbmroot_ShipRpc_ShipRpcPoint || hmax>kMaxcbmroot_Hcal_HcalPoint ||hlmax >kMaxcbmroot_HcalLite_HcalPointLite || tarmax >kMaxcbmroot_Target_TargetPoint){
		if(tmax>kMaxcbmroot_Stack_MCTrack ) cout<<"tmax!!! "<<tmax<<endl;
		if(vmax>kMaxcbmroot_veto_vetoPoint) cout<<"vmax!!! "<<vmax<<endl;
		if(smax > kMaxcbmroot_strawtubes_strawtubesPoint) cout<<"smax!!! "<<smax<<endl;
		if(emax>kMaxcbmroot_Ecal_EcalPoint) cout<<"emax!!! "<<emax<<endl;
		if(elmax>kMaxcbmroot_EcalLite_EcalPointLite) cout<<"elmax!!! "<<elmax<<endl;
		if(mmax>kMaxcbmroot_muon_muonPoint) cout<<"mmax!!! "<<mmax<<endl;
		if(SRmax > kMaxcbmroot_ShipRpc_ShipRpcPoint) cout<<"SRmax!!! "<<SRmax<<endl;
		if(hmax > kMaxcbmroot_Hcal_HcalPoint) cout<<"hmax!!! "<<hmax<<endl;
		if(hlmax >kMaxcbmroot_HcalLite_HcalPointLite )cout<<"hlmax!!! "<<hlmax<<endl;
		if(tarmax >kMaxcbmroot_Target_TargetPoint )cout<<"tarmax!!! "<<tarmax<<endl;
		return 0;
	}
	return 1;
}

//----------------------------------------------------------------------------------------------
void shipcbmsim::HistPlot(){
   // function creates Histograms and plots them
   // strawtubesPoint TH2, y:0 V, 1 T1, 2 T2, 3 T3, 4 T4, 5 Ecal, 6 muon
   // EcalPoint TH1
   // mounPoint TH1
   //plots (4,2), strawtubes, ecal, muon, v, T1, T2, T3, T4
	cout<<"Hist Plot"<<endl;
	fChain->SetBranchStatus("*",0);
	fChain->SetBranchStatus("strawtubes*",1);
	fChain->SetBranchStatus("Ecal*",1);
	fChain->SetBranchStatus("muon*",1);
	fChain->SetBranchStatus("MCTrack*",1);
	BranchAddresses();

	gStyle->SetOptStat("oumren");
	Float_t weight;
  
	Int_t det,bin;
	
   TH2D *strawtubesELoss = new TH2D("HistPlotELoss","Eloss;ELoss[GeV]",50,0,0.004,5,0,5);
   TH2D *Length = new TH2D("HistPlotLength","Length;Length[cm]",50,0,8000,7,0,7);
   TH2D *Px = new TH2D("HistPlotPx","Px;Px[GeV]",50,-1.5,1.5,7,0,7);
   TH2D *Py = new TH2D("HistPlotPy","Py;Py[GeV]",50,-1.5,1.5,7,0,7);
   TH2D *Pz = new TH2D("HistPlotPz","Pz;Pz[GeV]",50,0,180,7,0,7);
   TH2D *Time = new TH2D("HistPlotTime","Time;t[ns]",50,0,1200,7,0,7);
   TH2D *x = new TH2D("HistPlotx","x;x[cm]",50,-350,350,7,0,7);
   TH2D *y = new TH2D("HistPloty","y;y[cm]",50,-620,620,7,0,7);
   TH2D *strawtubesz = new TH2D("HistPlotz","z;z[cm]",50,-3600,3600,5,0,5);
   TH2D *PDG = new TH2D("HistPlotPDG","PDG",14,0,14,7,0,7); // Y: 1-5:strawtubes,6:ecal,7:muon

   TH1D *ecalELoss = new TH1D("HistPlotELoss_Ecal","ELoss_Ecal;Eloss[GeV]",50,0,3);
   TH1D *ecalz = new TH1D("HistPlotz_Ecal","z_Ecal;z[cm]",50,3500,3600);
  
   TH1D *muonELoss = new TH1D("HistPlotELoss_muon","ELoss_muon;Eloss[GeV]",50,0,0.5);
   TH1D *muonz = new TH1D("HistPlotz_muon","z_muon;z[cm]",50,3500,4000);
   
   SetPDGLabel(PDG);
   

   Long64_t nentries = fChain->GetEntriesFast();
   if (nentries > 800000) {Float_t scale = 1/10.0;cout<<"scale: "<<scale<<endl;}
   else Float_t scale = 1;
//----------------------------------------------------------------------
	for(Int_t j = 0; j<nentries;j++){
		fChain->GetEntry(j);

	    for(Int_t i = 0; i<strawtubesPoint_;i++){
			Int_t iTrack = strawtubesPoint_fTrackID[i];
			if (iTrack>0){
				weight = MCTrack_fW[iTrack] * scale;
				if (weight == 0) weight = 1;
				Float_t posz = strawtubesPoint_fZ[i];
				if(posz < -1957){det = 0; } //veto, Delta_z = 20
				else if(posz<2619){det = 1;} //T1, Delta_z = 40
				else if(posz<2819){det = 2;} //T2, Delta_z = 40
				else if(posz<3359){det = 3;} //T3, Delta_z = 40
				else if(posz<3559){det = 4;} //T4, Delta_z = 40
				Int_t iPID = TMath::Abs(MCTrack_fPdgCode[iTrack]);
				bin = PDGbin(iPID);
				PDG->Fill(bin,det,weight);
				//if(iPID == 13){
					strawtubesELoss->Fill(strawtubesPoint_fELoss[i],det,weight);
					Length->Fill(strawtubesPoint_fLength[i],det,weight);
					Px->Fill(strawtubesPoint_fPx[i],det,weight);
					Py->Fill(strawtubesPoint_fPy[i],det,weight);
					Pz->Fill(strawtubesPoint_fPz[i],det,weight);
					Time->Fill(strawtubesPoint_fTime[i],det,weight);
					x->Fill(strawtubesPoint_fX[i],det,weight);
					y->Fill(strawtubesPoint_fY[i],det,weight);
					strawtubesz->Fill(strawtubesPoint_fZ[i],det,weight);
				//}
			} 
	    }
//----------------------------------------------------------------------
	    det = 5; // Ecal
	    for(Int_t i = 0; i<EcalPoint_;i++){
	      Int_t iTrack = EcalPoint_fTrackID[i];
	      if(iTrack>0){
				weight = MCTrack_fW[iTrack] * scale;
				if (weight == 0) weight = 1;
		      Int_t iPID = TMath::Abs(MCTrack_fPdgCode[iTrack]);
		     // if(iPID == 13){
					Length->Fill(EcalPoint_fLength[i],det,weight);
					Px->Fill(EcalPoint_fPx[i],det,weight);
					Py->Fill(EcalPoint_fPy[i],det,weight);
					Pz->Fill(EcalPoint_fPz[i],det,weight);
					x->Fill(EcalPoint_fX[i],det,weight);
					y->Fill(EcalPoint_fY[i],det,weight);
					ecalz->Fill(EcalPoint_fZ[i],weight);
		     // }
		    }
		 }
		 for(Int_t i = 0; i<EcalPointLite_;i++){
	      Int_t iTrack = EcalPointLite_fTrackID[i];
	      if(iTrack>0){
				weight = MCTrack_fW[iTrack]* scale;
				if (weight == 0) weight = 1;
		      Int_t iPID = TMath::Abs(MCTrack_fPdgCode[iTrack]);
		      bin = PDGbin(iPID);
				PDG->Fill(bin,det,weight);
		      //if(iPID == 13){
						ecalELoss->Fill(EcalPointLite_fELoss[i],weight);  
						Time->Fill(EcalPointLite_fTime[i],det,weight);
		      //}
		   }
		 }
//----------------------------------------------------------------------
	    det = 6; // muon
	    for(Int_t i = 0; i<muonPoint_;i++){
	      Int_t iTrack = muonPoint_fTrackID[i];
	      if (iTrack >0){
				weight = MCTrack_fW[iTrack]*scale;
				if (weight == 0) weight = 1;
		      Int_t iPID = TMath::Abs(MCTrack_fPdgCode[iTrack]);
	         bin = PDGbin(iPID);
				PDG->Fill(bin,det,weight);
		      //if(iPID == 13){
					muonELoss->Fill(muonPoint_fELoss[i],weight); 
					Length->Fill(muonPoint_fLength[i],det,weight);
					Px->Fill(muonPoint_fPx[i],det,weight);
					Py->Fill(muonPoint_fPy[i],det,weight);
					Pz->Fill(muonPoint_fPz[i],det,weight);
					Time->Fill(muonPoint_fTime[i],det,weight);
					x->Fill(muonPoint_fX[i],det,weight);
					y->Fill(muonPoint_fY[i],det,weight);
					muonz->Fill(muonPoint_fZ[i],weight);
		     //}
		  }
		}
   }
//----------------------------------------------------------------------
	
  if (writeOption){
	   Length->Write();
	   Px->Write();
	   Py->Write();
	   Pz->Write();
	   Time->Write();
	   x->Write();
	   y->Write();
	   PDG->Write();
	   
	   strawtubesELoss->Write(); 
	   strawtubesz->Write();
	   ecalELoss->Write();
	   ecalz->Write();
	   muonELoss->Write();
	   muonz->Write();
	}
	
	delete strawtubesELoss;   delete Length;   delete Px;   delete Py;   delete Pz;
   delete Time;   delete x;   delete y;   delete strawtubesz;   delete PDG;
   delete ecalELoss;   delete ecalz;   delete muonELoss;   delete muonz;
}
void shipcbmsim::Histveto(){
   // function creates Histograms and plots them
	cout<<"Hist veto"<<endl;
	fChain->SetBranchStatus("*",0);
	fChain->SetBranchStatus("veto*",1);
	fChain->SetBranchStatus("MCTrack*",1);
	BranchAddresses();
	
	gStyle->SetOptStat("oumren");
	Float_t weight;
  
	Int_t det,bin;
	
   TH1D *ELoss = new TH1D("HistvetoELoss","Eloss;ELoss[GeV]",50,0,1);
   TH1D *Length = new TH1D("HistvetoLength","Length;Length[cm]",50,0,8000);
   TH1D *Px = new TH1D("HistvetoPx","Px;Px[GeV]",50,-200,200);
   TH1D *Py = new TH1D("HistvetoPy","Py;Py[GeV]",50,-400,0);
   TH1D *Pz = new TH1D("HistvetoPz","Pz;Pz[GeV]",50,-200,200);
   TH1D *Time = new TH1D("HistvetoTime","Time;t[ns]",50,0,1200);
   TH2D *xy = new TH2D("Histvetoxy","xy;x[cm];y[cm]",45,-300,300,90,-600,600);
   TH1D *z = new TH1D("Histvetoz","z;z[cm]",35,-3000,4000);
   TH1D *zveto = new TH1D("Histvetozveto","zveto;z[cm]",70,-3000,-1000); //zoom in
   TH1D *PDG = new TH1D("HistvetoPDG","PDG",14,0,14);
   
   SetPDGLabel(PDG);
   
   Long64_t nentries = fChain->GetEntriesFast();
   if (nentries > 800000) {Float_t scale = 1/10.0;cout<<"scale: "<<scale<<endl;}
   else Float_t scale = 1;
//----------------------------------------------------------------------
	for(Int_t j = 0; j<nentries;j++){
		fChain->GetEntry(j);

	    for(Int_t i = 0; i<vetoPoint_;i++){
			Int_t iTrack = vetoPoint_fTrackID[i];
			if (iTrack>0){
				if (vetoPoint_fZ[i]<3500 && vetoPoint_fZ[i]>-2500){ // cut to get rid of additional active planes
					weight = MCTrack_fW[iTrack]* scale;
					if (weight == 0) weight = 1;
					Int_t iPID = TMath::Abs(MCTrack_fPdgCode[iTrack]);
					bin = PDGbin(iPID);
					PDG->Fill(bin,weight);
					//if(iPID == 13){
						ELoss->Fill(vetoPoint_fELoss[i],weight);
						Length->Fill(vetoPoint_fLength[i],weight);
						Px->Fill(vetoPoint_fPx[i],weight);
						Py->Fill(vetoPoint_fPy[i],weight);
						Pz->Fill(vetoPoint_fPz[i],weight);
						Time->Fill(vetoPoint_fTime[i],weight);
						xy->Fill(vetoPoint_fX[i],vetoPoint_fY[i],weight);
						z->Fill(vetoPoint_fZ[i],weight);
						zveto->Fill(vetoPoint_fZ[i],weight);
					//}
					//if (vetoPoint_fLength[i]>7000){
						//cout<<iTrack<<"  "<<MCTrack_fPdgCode[iTrack]<<"  "<<vetoPoint_fTime[i]<<"  "<<vetoPoint_fLength[i]<<"  ";
						//cout<<MCTrack_fStartX[iTrack]<<"  "<<MCTrack_fStartY[iTrack]<<"  "<<MCTrack_fStartZ[iTrack]<<"  ";
						//cout<<MCTrack_fPx[iTrack]<<"  "<<MCTrack_fPy[iTrack]<<"  "<<MCTrack_fPz[iTrack]<<endl;						
					//}
					
				}
			} 
	    }
	}

//----------------------------------------------------------------------
	if (writeOption){
		ELoss->Write();
	   Length->Write();
	   Px->Write();
	   Py->Write();
	   Pz->Write();
	   Time->Write();
	   xy->Write();
	   z->Write();
	   zveto->Write();
	   PDG->Write();
	}
	
	delete ELoss;	delete Length;	delete Px;	delete Py;	delete Py;
	delete Time;	delete PDG;	   delete xy;	delete z;
}
//-----------------------------------------------------------------------------------------------
std::vector<Int_t> shipcbmsim::newVeto(){
	cout<<"newVeto"<<endl;
	fChain->SetBranchStatus("*",0); 
	
	fChain->SetBranchStatus("veto*",1);
	fChain->SetBranchStatus("MCTrack*",1);
	fChain->SetBranchStatus("straw*",1);
	fChain->SetBranchStatus("EcalPoint*",1);
	fChain->SetBranchStatus("muonPoint*",1);
	fChain->SetBranchStatus("HcalPoint*",1);
	
	{BranchAddresses();
		fChain->SetBranchStatus("strawtubesPoint.fPx",0);
		fChain->SetBranchStatus("strawtubesPoint.fPy",0);
		fChain->SetBranchStatus("strawtubesPoint.fPz",0);
		fChain->SetBranchStatus("strawtubesPoint.fTime",0);
		fChain->SetBranchStatus("strawtubesPoint.fLength",0);	
		fChain->SetBranchStatus("EcalPoint.fPx",0);
		fChain->SetBranchStatus("EcalPoint.fPy",0);
		fChain->SetBranchStatus("EcalPoint.fPz",0);
		fChain->SetBranchStatus("EcalPoint.fTime",0);
		fChain->SetBranchStatus("EcalPoint.fLength",0);
		fChain->SetBranchStatus("EcalPoint.fELoss",0);
		fChain->SetBranchStatus("EcalPoint.fX",0);
		fChain->SetBranchStatus("EcalPoint.fY",0);
		fChain->SetBranchStatus("EcalPoint.fZ",0);
		fChain->SetBranchStatus("EcalPoint.fPdgCode",0);
		fChain->SetBranchStatus("EcalPointLite.fPx",0);
		fChain->SetBranchStatus("EcalPointLite.fPy",0);
		fChain->SetBranchStatus("EcalPointLite.fPz",0);
		fChain->SetBranchStatus("EcalPointLite.fTime",0);
		fChain->SetBranchStatus("EcalPointLite.fLength",0);
		fChain->SetBranchStatus("EcalPointLite.fELoss",0);
		fChain->SetBranchStatus("EcalPointLite.fX",0);
		fChain->SetBranchStatus("EcalPointLite.fY",0);
		fChain->SetBranchStatus("EcalPointLite.fZ",0);
		fChain->SetBranchStatus("EcalPointLite.fPdgCode",0);
		fChain->SetBranchStatus("muonPoint.fPx",0);
		fChain->SetBranchStatus("muonPoint.fPy",0);
		fChain->SetBranchStatus("muonPoint.fPz",0);
		fChain->SetBranchStatus("muonPoint.fTime",0);
		fChain->SetBranchStatus("muonPoint.fLength",0);
		fChain->SetBranchStatus("muonPoint.fELoss",0);
		fChain->SetBranchStatus("muonPoint.fX",0);
		fChain->SetBranchStatus("muonPoint.fY",0);
		fChain->SetBranchStatus("muonPoint.fZ",0);
		fChain->SetBranchStatus("muonPoint.fPdgCode",0);
	}
		
	Float_t weight, t;
	Int_t mu,bin, signalclass, phisegment, zsegment, segment;
	
	Long64_t nentries = fChain->GetEntriesFast();
	//if (nentries > 800000) {Float_t scale = 1.0*500000/nentries;cout<<"scale "<<scale<<endl;}
	if (nentries > 800000) {Float_t scale = 1/10.0;cout<<"scale: "<<scale<<endl;}
	else Float_t scale = 1;
	Float_t pi = TMath::Pi();
	
	Float_t zmin = 0;
	Float_t zmax = 0;
	
	Float_t a = 250; //radius x
	Float_t asmall = 186; //radius x
	Float_t b = 500; //radius y
	Float_t r = 30;
					
	Float_t zminus = -2478; //Detektorausmaße
	Float_t zplus = 3678; // hinteres Ende des Taggers
	Float_t zstepsize = 100;
	
	Int_t N = 16; //even number >15
	Float_t dphi = 2*pi/N;
	Float_t phistart = dphi/2; 
	
	const Int_t nBins = N*TMath::CeilNint((zplus-zminus)/zstepsize);	
	TH1D *segmentHits = new TH1D("newVetosegmentHits","segmentHits; segment",nBins,0,nBins);
	TH1D *PhisegmentHits = new TH1D("newVetoPhisegmentHits","PhisegmentHits; Phisegment",N,0,N);
	TH2D *segmentHits2 = new TH2D("newVetosegmentHits2","segmentHits2; phisegment; zsegment",N,0,N,nBins/N,0,nBins/N);
		
	TH2D *segmentEnergy = new TH2D("newVetosegmentEnergy","segmentEnergy; phisegment; zsegment",N,0,N,nBins/N,0,nBins/N);
	TH2D *ELosstop = new TH2D("newVetoElosstop","Elosstop 6-9; |P| [GeV]; ELoss [Gev]", 50,-1,3,50,-3,0.301); //log
	TH2D *ELoss_Hist = new TH2D("newVetoEloss","Eloss;|P| [GeV]; ELoss [Gev]", 50,-1,3,50,-3,0.301); //log
	TH2D *ELossleft = new TH2D("newVetoElossleft","Elossleft 12-15; |P| [GeV];ELoss [Gev] ", 50,-1,3,50,-3,0.301); //log
	BinLogY(ELosstop);
	BinLogY(ELossleft);
	BinLogY(ELoss_Hist);
	BinLogX(ELosstop);
	BinLogX(ELossleft);
	BinLogX(ELoss_Hist);
	
	Int_t BinsForSegments = 20;// hits by muons only
	Float_t SegmentIsHit [992] = {}; //nBins geht nicht als Arraylänge
	Int_t TrackStationIsHit [5] = {}; 
	
	TH2D *HitsperTS = new TH2D("newVetoHitsperTS","HitsperTS; Hits per TS; ",50,0,50,5,0,5);
	
	TH1D *N_o_Segments = new TH1D("newVetoN.o.Segments","N.o.Segments; # segment ",BinsForSegments,0,BinsForSegments); 
		
	TH1D *N_o_Segments_V = new TH1D("newVetoN.o.Segments_V","N.o.Segments_V; # segment",BinsForSegments,0,BinsForSegments); 
	TH1D *N_o_Segments_T1 = new TH1D("newVetoN.o.Segments_T1","N.o.Segments_T1; # segment",BinsForSegments,0,BinsForSegments); 
	TH1D *N_o_Segments_T2 = new TH1D("newVetoN.o.Segments_T2","N.o.Segments_T2; # segment",BinsForSegments,0,BinsForSegments); 
	TH1D *N_o_Segments_T3 = new TH1D("newVetoN.o.Segments_T3","N.o.Segments_T3; # segment",BinsForSegments,0,BinsForSegments); 
	TH1D *N_o_Segments_T4 = new TH1D("newVetoN.o.Segments_T4","N.o.Segments_T4; # segment",BinsForSegments,0,BinsForSegments); 
	
	TH1D *N_o_Segments_ECAL = new TH1D("newVetoN.o.Segments_ECAL","N.o.Segments_ECAL; # segment",BinsForSegments,0,BinsForSegments); 
	TH1D *N_o_Segments_all = new TH1D("newVetoN.o.Segments_E_M_T1-4","N.o.Segments_all; # segment",BinsForSegments,0,BinsForSegments); 	
	TH1D *N_o_Segments_allT = new TH1D("newVetoN.o.Segments_T1-4","N.o.Segments_all; # segment",BinsForSegments,0,BinsForSegments); 
	TH1D *N_o_Segments_Signal = new TH1D("newVetoN.o.Segments_Signal","N.o.Segments_Signal; # segment",BinsForSegments,0,BinsForSegments); 
	
	TH1D *Signal_P = new TH1D("newVetoSignal_P","Signal_P;|P_{#mu}| [GeV]",50,-1,3); 
	BinLogX(Signal_P);
	TH1D *Signal_PDG = new TH1D("newVetoSignal_PDG","Signal_PDG",14,0,14);
	SetPDGLabel(Signal_PDG);
	
	TH1D *NoTracks = new TH1D("newVetoNoTrack","Number of Tracks",6,0,6);
	TH1D *NoTracks_KL = new TH1D("newVetoNoTrackKL","Number of Tracks",6,0,6);
	TH1D *NoTracks_KLW = new TH1D("newVetoNoTrackKLW","Number of Tracks unweighted",6,0,6);
	TH1D *NoTracksunweighted = new TH1D("newVetoNoTrackunweighted","Number of Tracks unweighted",6,0,6);
	
	std::vector<Int_t> SignalList;
	
	Double_t sum[6]={};//weighted
	Double_t sumW[6]={};//unweighted
	//Double_t neutron[6]={};//weighted
	Double_t KL[6]={};//weighted
	Double_t KLW[6]={};//weighted
	// Double_t KS[6]={};//weighted
	
	TH1D *TwoTrack = new TH1D("newVetoTwoTrack","Number of hit segments, TwoTrack;Number of hit segments",15,0,15);
	TH1D *OneTrack = new TH1D("newVetoOneTrack","Number of hit segments, OneTrack;Number of hit segments",15,0,15);
	TH1D *ZeroTrack = new TH1D("newVetoZeroTrack","Number of hit segments, ZeroTrack;Number of hit segments",15,0,15);
	
	Double_t cat1 = 0;
	Double_t cat2 = 0;
	

//----------------------------------------------------------------------
	Int_t multiplicity = 1;
	for(Int_t j = 0; j<nentries;j++){
		if (!(j % 100000)) cout<<j<<endl;
		fChain->GetEntry(j);
		
		Bool_t has_it=0;
		// Bool_t has_n = 0;
		
		Float_t weight0 = MCTrack_fW[0];
		weight = weight0 * scale;
		if (weight == 0) weight = 1;
		
		for(Int_t i = 0; i<MCTrack_;i++){
			if (MCTrack_fMotherId[i] == -1 && MCTrack_fStartY[i] == 2000){ //info about starting muons from Cosmics
				multiplicity++; 
			}
			Int_t iPDG = TMath::Abs(MCTrack_fPdgCode[i]);
			if (iPDG == 2112 || iPDG == 130 || iPDG == 310){if (MCTrack_fStartZ[i] < 3678 && (MCTrack_fStartX[i]*MCTrack_fStartX[i] + MCTrack_fStartY[i]*MCTrack_fStartY[i]< 640000)){ has_it = 1;}}
			// if (iPDG == 2112) has_n = 1;
		}
		
		// Float_t Theta = TMath::ACos(MCTrack_fPy[0]/TMath::Sqrt(MCTrack_fPx[0]*MCTrack_fPx[0]+MCTrack_fPy[0]*MCTrack_fPy[0]+MCTrack_fPz[0]*MCTrack_fPz[0]));
		
		for(Int_t i = 0; i<vetoPoint_;i++){
			Int_t iTrack = vetoPoint_fTrackID[i];
			mu = 0;
			if (iTrack + 2){ 
				if (multiplicity != 1) weight = MCTrack_fW[iTrack] * scale;
				if (TMath::Abs(MCTrack_fPdgCode[iTrack]) == 13) mu = 1;
			}	
			
			Float_t z = vetoPoint_fZ[i];
			//if(z < zplus && z > zminus && x*x/((a+35)*(a+35)) + y*y/((b+35)*(b+35))<1){ //cut überprüfen!
			if(z < zplus && z > zminus){ //cut überprüfen!
				Float_t x = vetoPoint_fX[i];
				Float_t y = vetoPoint_fY[i];
				Float_t px = vetoPoint_fPx[i];
				Float_t py = vetoPoint_fPy[i];
				Float_t pz = vetoPoint_fPz[i];
				Float_t p = TMath::Sqrt(px*px + py*py + pz*pz);
				Float_t ELoss = vetoPoint_fELoss[i];
				if (z > -1968) t = TMath::ATan2(a*y,b*x)+2*pi; // t [pi,3pi]	
				else if (z < -1968) t = TMath::ATan2(asmall*y,b*x)+2*pi; // t [pi,3pi]
					
				phisegment = TMath::FloorNint((t+phistart)/dphi)%N;
				zsegment = TMath::FloorNint((z-zminus)/zstepsize);
				segment = zsegment*N + phisegment;

				if(mu){	
					if (phisegment>=3 && phisegment <=5) {ELosstop->Fill(p,ELoss,weight);}
					else if (phisegment>=7 && phisegment <=9) ELossleft->Fill(p,ELoss,weight);
					ELoss_Hist->Fill(p,ELoss,weight);
				}	
				segmentEnergy->Fill(phisegment,zsegment,ELoss*weight);
				SegmentIsHit[segment]  += ELoss;	
					
				// test every now and then!
				//if (z<zmin) zmin = z;
				//if (z>zmax) zmax = z;
				//if (x*x/(asmall*asmall) + y*y/(b*b)<1){cout<<"veto: "<<z<<"  "<<x<<"  "<<y<<"  "<<detID<<endl;} 
			}
			// else cout<<z<<"  "<< x*x/(asmall*asmall) + y*y/(b*b)<<"  "<<detID<<endl;		
		}
		
		Int_t hitSegments  = 0;
		for (Int_t n = 0;n<nBins;n++){
			if (SegmentIsHit[n] > 0.045) {
				hitSegments++; 
				segmentHits->Fill(n,weight); 
				zsegment = n/N;
				phisegment = n%N;
				PhisegmentHits->Fill(phisegment,weight); 
				segmentHits2->Fill(phisegment,zsegment,weight);
				SegmentIsHit[n] = 0;
			}			
		}
		
		Bool_t TS1pluscharge = 0;		Bool_t TS1minuscharge = 0;
		Bool_t TS2pluscharge = 0;		Bool_t TS2minuscharge = 0;
		Bool_t TS3pluscharge = 0;		Bool_t TS3minuscharge = 0;
		Bool_t TS4pluscharge = 0;		Bool_t TS4minuscharge = 0;
				
		for(Int_t i = 0; i<strawtubesPoint_;i++){
			Int_t iTrack = strawtubesPoint_fTrackID[i];
			if(iTrack+2){
				Float_t x = strawtubesPoint_fX[i];
				Float_t y = strawtubesPoint_fY[i];
				Float_t z = strawtubesPoint_fZ[i];
				Int_t iPDG = MCTrack_fPdgCode[iTrack];
				if(TMath::Abs(iPDG) == 13 || TMath::Abs(iPDG) == 211){ 
					if (x*x/((a+r/2)*(a+r/2)) + y*y/((b+r/2)*(b+r/2)) < 1){	// z values anpassen für Y = 6m
						if(z < -1957){TrackStationIsHit[0]++; } //veto, Delta_z = 20
						else if(z<2619){
							TrackStationIsHit[1]++;
							if (iPDG < 0) TS1minuscharge = 1;
							if (iPDG > 0) TS1pluscharge = 1;
						} //T1, Delta_z = 40
						else if(z<2819){
							TrackStationIsHit[2]++; 
							if (iPDG < 0) TS2minuscharge = 1;
							if (iPDG > 0) TS2pluscharge = 1;
						} //T2, Delta_z = 40
						else if(z<3359){
							TrackStationIsHit[3]++; 
							if (iPDG < 0) TS3minuscharge = 1;
							if (iPDG > 0) TS3pluscharge = 1;
						} //T3, Delta_z = 40
						else if(z<3559){
							TrackStationIsHit[4]++;
							if (iPDG < 0) TS4minuscharge = 1;
							if (iPDG > 0) TS4pluscharge = 1;
						} //T4, Delta_z = 40
						//if(z > -1979 && z < -1957){TrackStationIsHit[0]++; } //veto, Delta_z = 20
						//else if(z>2577 && z<2619){TrackStationIsHit[1]++; } //T1, Delta_z = 40
						//else if(z>2777 && z<2819){TrackStationIsHit[2]++; } //T2, Delta_z = 40
						//else if(z>3217 && z<3259){TrackStationIsHit[3]++; } //T3, Delta_z = 40
						//else if(z>3417 && z<3459){TrackStationIsHit[4]++; } //T4, Delta_z = 40
						else{cout<<z<<endl;}
					}
				}
			}
		}

		//What to do with weights?
		weight = weight0*scale; if (weight == 0) weight = 1;
		Bool_t allTS = TrackStationIsHit[1]&&TrackStationIsHit[2]&&TrackStationIsHit[3]&&TrackStationIsHit[4];
		Bool_t threeTS = TrackStationIsHit[1]&&TrackStationIsHit[2]&&TrackStationIsHit[3] || TrackStationIsHit[2]&&TrackStationIsHit[3]&&TrackStationIsHit[4] || TrackStationIsHit[1]&&TrackStationIsHit[3]&&TrackStationIsHit[4] || TrackStationIsHit[2]&&TrackStationIsHit[1]&&TrackStationIsHit[4];
		
		N_o_Segments->Fill(hitSegments,weight); //Tagger
		if (EcalPointLite_) {N_o_Segments_ECAL->Fill(hitSegments,weight);} //ECAL + Tagger
		if (allTS) {N_o_Segments_allT->Fill(hitSegments,weight);} //TrackStation 1..4 + Tagger
		if (allTS && EcalPointLite_ && muonPoint_) {N_o_Segments_all->Fill(hitSegments,weight);}	// ECAL + HCAL + Muon + Tagger + TrackStation 1..4
		//if (allTS && EcalPointLite_){
		if (threeTS){
			N_o_Segments_Signal->Fill(hitSegments,weight);
			
			Signal_P->Fill(TMath::Sqrt(MCTrack_fPx[0]*MCTrack_fPx[0]+MCTrack_fPy[0]*MCTrack_fPy[0]+MCTrack_fPz[0]*MCTrack_fPz[0]),weight);
			for(Int_t i = 0; i<MCTrack_;i++){
				Int_t iPID = TMath::Abs(MCTrack_fPdgCode[i]);
				bin = PDGbin(iPID);
				Signal_PDG->Fill(bin, weight);			
			}	
			Bool_t charge = TS1pluscharge*TS1minuscharge*TS2pluscharge*TS2minuscharge*TS3pluscharge*TS3minuscharge*TS4pluscharge*TS4minuscharge;
			
			if (TrackStationIsHit[1]>12 && TrackStationIsHit[2]>12 && TrackStationIsHit[3]>12 && TrackStationIsHit[4]>12 && charge) {
				if (hitSegments) signalclass = 5;
				else signalclass = 4;	
				TwoTrack->Fill(hitSegments,weight);		
			}
			else if (TrackStationIsHit[1]>5 && TrackStationIsHit[2]>5 && TrackStationIsHit[3]>5 && TrackStationIsHit[4]>5){
				if (hitSegments) signalclass = 3;
				else signalclass = 2;
				OneTrack->Fill(hitSegments,weight);	
			}
			else{
				if (hitSegments) signalclass = 1;
				else signalclass = 0;
				ZeroTrack->Fill(hitSegments,weight);	
			}	
			if(writeOption) SignalList.push_back(j);	
		}	// ECAL + Tagger + TrackStation 1..4 - Veto
		else{
			if (hitSegments) {signalclass = 1; cat2 += weight;}
			else {signalclass = 0; cat1 += weight;}
			ZeroTrack->Fill(hitSegments,weight);	
		}
		
		if (TrackStationIsHit[0]) {HitsperTS->Fill(TrackStationIsHit[0],0,weight); N_o_Segments_V->Fill(hitSegments,weight); TrackStationIsHit[0] = 0;}
		if (TrackStationIsHit[1]) {HitsperTS->Fill(TrackStationIsHit[1],1,weight);N_o_Segments_T1->Fill(hitSegments,weight); TrackStationIsHit[1] = 0;}
		if (TrackStationIsHit[2]) {HitsperTS->Fill(TrackStationIsHit[2],2,weight);N_o_Segments_T2->Fill(hitSegments,weight); TrackStationIsHit[2] = 0;}
		if (TrackStationIsHit[3]) {HitsperTS->Fill(TrackStationIsHit[3],3,weight);N_o_Segments_T3->Fill(hitSegments,weight); TrackStationIsHit[3] = 0;}
		if (TrackStationIsHit[4]) {HitsperTS->Fill(TrackStationIsHit[4],4,weight);N_o_Segments_T4->Fill(hitSegments,weight); TrackStationIsHit[4] = 0;}
		
		sum[signalclass]+=weight;
		sumW[signalclass]++;
		
		if (has_it) {KL[signalclass]+=weight; KLW[signalclass]++;}
		//if (has_it) KS[signalclass]+=weight;
		//if (has_it) neutron[signalclass]+=weight;
	}	
	
	NoTracks->Fill(0,sum[0]);
	NoTracks->Fill(1,sum[1]);
	NoTracks->Fill(2,sum[2]);
	NoTracks->Fill(3,sum[3]);
	NoTracks->Fill(4,sum[4]);
	NoTracks->Fill(5,sum[5]);
	NoTracks_KL->Fill(0,KL[0]);
	NoTracks_KL->Fill(1,KL[1]);
	NoTracks_KL->Fill(2,KL[2]);
	NoTracks_KL->Fill(3,KL[3]);
	NoTracks_KL->Fill(4,KL[4]);
	NoTracks_KL->Fill(5,KL[5]);
	NoTracks_KLW->Fill(0,KLW[0]);
	NoTracks_KLW->Fill(1,KLW[1]);
	NoTracks_KLW->Fill(2,KLW[2]);
	NoTracks_KLW->Fill(3,KLW[3]);
	NoTracks_KLW->Fill(4,KLW[4]);
	NoTracks_KLW->Fill(5,KLW[5]);
	NoTracksunweighted->Fill(0,sumW[0]);
	NoTracksunweighted->Fill(1,sumW[1]);
	NoTracksunweighted->Fill(2,sumW[2]);
	NoTracksunweighted->Fill(3,sumW[3]);
	NoTracksunweighted->Fill(4,sumW[4]);
	NoTracksunweighted->Fill(5,sumW[5]);
	
	// cout<<"zmin: "<<zmin<<"  , zmax: "<<zmax<<endl;
	cout<<endl;
	cout<<"1. 0Tracks: "<<sum[0] + sum[1]<<endl;
	cout<<"2. 1Tracks: "<<sum[2] + sum[3]<<endl;
	cout<<"3. 2Tracks: "<<sum[4] + sum[5]<<endl;
	cout<<"4. 0Tracks, no Tagger: "<<sum[0]<<endl;
	cout<<"5. 0Tracks and Tagger: "<<sum[1]<<endl;
	cout<<"6. 1Tracks, no Tagger: "<<sum[2]<<endl;
	cout<<"7. 1Tracks and Tagger: "<<sum[3]<<endl;
	cout<<"8. 2Tracks, no Tagger: "<<sum[4]<<endl;
	cout<<"9. 2Tracks and Tagger: "<<sum[5]<<endl;
	Double_t totalweight =sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5];
	cout<<totalweight<<endl<<endl;
	
	cout<<"1. 0Tracks: "<<KL[0] + KL[1]<<endl;
	cout<<"2. 1Tracks: "<<KL[2] + KL[3]<<endl;
	cout<<"3. 2Tracks: "<<KL[4] + KL[5]<<endl;
	cout<<"4. 0Tracks, no Tagger: "<<KL[0]<<endl;
	cout<<"5. 0Tracks and Tagger: "<<KL[1]<<endl;
	cout<<"6. 1Tracks, no Tagger: "<<KL[2]<<endl;
	cout<<"7. 1Tracks and Tagger: "<<KL[3]<<endl;
	cout<<"8. 2Tracks, no Tagger: "<<KL[4]<<endl;
	cout<<"9. 2Tracks and Tagger: "<<KL[5]<<endl;
	Double_t KLtotalweight = KL[0] + KL[1] + KL[2] + KL[3] + KL[4] + KL[5];
	cout<<KLtotalweight<<" = "<<KLtotalweight/totalweight<<endl;
	
	cout<<endl;
	cout<<endl;
	cout<<"cat1 = "<<cat1<<"  cat2 = "<<cat2<<endl;
//----------------------------------------------------------------------
	
	if(writeOption){
		NoTracks->GetXaxis()->SetBinLabel(1,"no Tracks, no Tagger");
		NoTracks->GetXaxis()->SetBinLabel(2,"no Tracks, Tagger");
		NoTracks->GetXaxis()->SetBinLabel(3,"1 Track, no Tagger");
		NoTracks->GetXaxis()->SetBinLabel(4,"1 Track, Tagger");
		NoTracks->GetXaxis()->SetBinLabel(5,"2 Tracks, noTagger");
		NoTracks->GetXaxis()->SetBinLabel(6,"2 Tracks, Tagger");
		
		NoTracksunweighted->GetXaxis()->SetBinLabel(1,"no Tracks, no Tagger");
		NoTracksunweighted->GetXaxis()->SetBinLabel(2,"no Tracks, Tagger");
		NoTracksunweighted->GetXaxis()->SetBinLabel(3,"1 Track, no Tagger");
		NoTracksunweighted->GetXaxis()->SetBinLabel(4,"1 Track, Tagger");
		NoTracksunweighted->GetXaxis()->SetBinLabel(5,"2 Tracks, noTagger");
		NoTracksunweighted->GetXaxis()->SetBinLabel(6,"2 Tracks, Tagger");
		
		TFile *saveHist = new TFile("Histograms." + filename + ".root","UPDATE");
		segmentHits->Write();
		segmentHits2->Write();
		PhisegmentHits->Write();
		
		TwoTrack->Write();
		OneTrack->Write();
		ZeroTrack->Write();
		
		segmentEnergy->Write();
		ELosstop->Write();
		ELossleft->Write();
		ELoss_Hist->Write();
		
		HitsperTS->Write();
		
		N_o_Segments->Write();
		
		N_o_Segments_V->Write();
		N_o_Segments_T1->Write();
		N_o_Segments_T2->Write();
		N_o_Segments_T3->Write();
		N_o_Segments_T4->Write();
		N_o_Segments_ECAL->Write();
		N_o_Segments_all->Write();
		N_o_Segments_allT->Write();
		N_o_Segments_Signal->Write();
		
		Signal_P->Write();
		Signal_PDG->Write();
		
		NoTracks->Write();
		NoTracks_KL->Write();
		NoTracks_KLW->Write();
		NoTracksunweighted->Write();
		
		saveHist->Close();	
	}
	
	delete segmentHits; delete PhisegmentHits;
		
	delete segmentEnergy; delete ELosstop;	delete ELossleft;
		
	delete N_o_Segments;	delete N_o_Segments_V; delete N_o_Segments_T1;
	delete N_o_Segments_T2;	delete N_o_Segments_T3;	delete N_o_Segments_T4;	
	delete N_o_Segments_ECAL; delete N_o_Segments_all; delete N_o_Segments_allT;
	delete N_o_Segments_Signal;
	delete Signal_P; delete Signal_PDG;
	return SignalList;
}

void shipcbmsim::newMC(){
	cout<<"newMC"<<endl;
	fChain->SetBranchStatus("*",0); 
	fChain->SetBranchStatus("MCTrack*",1); 
	BranchAddresses();
	
	Float_t weight;
	
	Long64_t nentries = fChain->GetEntriesFast();
	if (nentries > 800000) {Float_t scale = 1/10.0;cout<<"scale: "<<scale<<endl;}
   else Float_t scale = 1;
   Float_t pi = TMath::Pi();
   
   TH1D *PDGMC = new TH1D("newMCPDGMC","PDGMC",14,0,14);
	
	TH1D *Mom_neutron = new TH1D("newMCMom_neutron","Mom_neutron; |P| [GeV]",40,-0.5,2.3);
	BinLogX(Mom_neutron);
	TH2D *StartXY_neutron = new TH2D("newMCStartXY_neutron","StartXY_neutron;x [cm];y [cm]",40,-1500,1500,40,-1500,1500);
	TH2D *StartXY_neutron_Zoom = new TH2D("newMCStartXY_neutron_Zoom","StartXY_neutron_Zoom;x [cm];y [cm]",50,-300,300,50,-600,600);
	TH1D *StartZ_neutron = new TH1D("newMCStartZ_neutron","StartZ_neutron; z [cm]",40,-3500,5000);
	TH1D *Mother_neutron = new TH1D("newMCMother_neutron","Mother_neutron",14,0,14);
	
	TH2D *DecayXY_neutron = new TH2D("newMCDecayXY_neutron","DecayXY_neutron;x [cm];y [cm]",40,-1500,1500,40,-1500,1500);
	TH1D *DecayZ_neutron = new TH1D("newMCDecayZ_neutron","DecayZ_neutron; z [cm]",40,-3500,5000);
	TH1D *Daughter_neutron = new TH1D("newMCDaughter_neutron","Daughter_neutron",14,0,14);
	
	TH1D *Mom_KL = new TH1D("newMCMom_KL","Mom_KL; |P| [GeV]",40,-0.5,2.3);
	BinLogX(Mom_KL);
	TH2D *StartXY_KL = new TH2D("newMCStartXY_KL","StartXY_KL;x [cm];y [cm]",40,-1500,1500,40,-1500,1500);
	TH1D *StartZ_KL = new TH1D("newMCStartZ_KL","StartZ_KL; z [cm]",40,-3500,5000);
	TH1D *Mother_KL = new TH1D("newMCMother_KL","Mother_KL",14,0,14);
	
	TH2D *DecayXY_KL = new TH2D("newMCDecayXY_KL","DecayXY_KL;x [cm];y [cm]",40,-1500,1500,40,-1500,1500);
	TH1D *DecayZ_KL = new TH1D("newMCDecayZ_KL","DecayZ_KL; z [cm]",40,-3500,5000);
	TH1D *Daughter_KL = new TH1D("newMCDaughter_KL","Daughter_KL",14,0,14);
	
	TH1D *Mom_KS = new TH1D("newMCMom_KS","Mom_KS; |P| [GeV]",40,-0.5,2.3);
	BinLogX(Mom_KS);
	TH2D *StartXY_KS = new TH2D("newMCStartXY_KS","StartXY_KS;x [cm];y [cm]",40,-1500,1500,40,-1500,1500);
	TH1D *StartZ_KS = new TH1D("newMCStartZ_KS","StartZ_KS; z [cm]",40,-3500,5000);
	TH1D *Mother_KS = new TH1D("newMCMother_KS","Mother_KS",14,0,14);
	
	TH2D *DecayXY_KS = new TH2D("newMCDecayXY_KS","DecayXY_KS;x [cm];y [cm]",40,-1500,1500,40,-1500,1500);
	TH1D *DecayZ_KS = new TH1D("newMCDecayZ_KS","DecayZ_KS; z [cm]",40,-3500,5000);
	TH1D *Daughter_KS = new TH1D("newMCDaughter_KS","Daughter_KS",14,0,14);
	
	Float_t n_neutron = 0;
	Float_t n_neutron_Zoom = 0;
	Float_t n_KL = 0;
	Float_t n_KS = 0;
	Int_t bin;
	
	SetPDGLabel(Mother_neutron);
	SetPDGLabel(Mother_KL);
	SetPDGLabel(Mother_KS);
	SetPDGLabel(PDGMC);
	SetPDGLabel(Daughter_neutron);
	SetPDGLabel(Daughter_KL);
	SetPDGLabel(Daughter_KS);
	
	Int_t number_of_Neutrons;Int_t number_of_KL;Int_t number_of_KS;
	Double_t interestingEvents=0;
//----------------------------------------------------------------------	
	for(Int_t j = 0; j<nentries;j++){
		fChain->GetEntry(j);
		Bool_t has_it = 0;
		
		number_of_Neutrons = 0;number_of_KL = 0;number_of_KS = 0;
		for(Int_t i = 0; i<MCTrack_;i++){
			Int_t iPDG = TMath::Abs(MCTrack_fPdgCode[i]);
			Int_t m_iPD = MCTrack_fMotherId[i];
			
			weight = MCTrack_fW[i]*scale;
			if (weight == 0) weight = 1;
			bin = PDGbin(TMath::Abs(MCTrack_fPdgCode[i]));
			PDGMC->Fill(bin,weight);
			
			
			if (m_iPD + 1 && m_iPD + 2){
				if (TMath::Abs(MCTrack_fPdgCode[m_iPD]) == 2112){
					DecayXY_neutron->Fill(MCTrack_fStartX[i],MCTrack_fStartY[i],weight);
					DecayZ_neutron->Fill(MCTrack_fStartZ[i],weight);
					number_of_Neutrons++;	
					Daughter_neutron->Fill(bin,weight);
									
				}
				if (TMath::Abs(MCTrack_fPdgCode[m_iPD]) == 130){
					DecayXY_KL->Fill(MCTrack_fStartX[i],MCTrack_fStartY[i],weight);
					DecayZ_KL->Fill(MCTrack_fStartZ[i],weight);
					number_of_KL++;	
					Daughter_KL->Fill(bin,weight);
									
				}
				if (TMath::Abs(MCTrack_fPdgCode[m_iPD]) == 310){
					DecayXY_KS->Fill(MCTrack_fStartX[i],MCTrack_fStartY[i],weight);
					DecayZ_KS->Fill(MCTrack_fStartZ[i],weight);
					number_of_KS++;	
					Daughter_KS->Fill(bin,weight);				
				}
			}
			
			
			if(iPDG == 2112 || iPDG == 310 || iPDG == 130){
				has_it  = 1;
				Float_t Px = MCTrack_fPx[i];
				Float_t Py = MCTrack_fPy[i];
				Float_t Pz = MCTrack_fPz[i];
				Float_t x = MCTrack_fStartX[i];
				Float_t y = MCTrack_fStartY[i];
				Float_t z = MCTrack_fStartZ[i];
				weight = MCTrack_fW[i]*scale;	if (weight == 0) weight = 1;
								
				Float_t P = TMath::Sqrt(Px*Px+Py*Py+Pz*Pz);
				Int_t mID = TMath::Abs(MCTrack_fMotherId[i]);
				
				if(iPDG == 2112){//neutron
					Mom_neutron->Fill(P,weight);
					StartXY_neutron->Fill(x,y,weight);
					if (z>-2500 && z<3500 && TMath::Abs(x) < 300&&TMath::Abs(y)<600){
						StartXY_neutron_Zoom->Fill(x,y,weight);
						n_neutron_Zoom = n_neutron_Zoom+weight;
					}
					StartZ_neutron->Fill(z,weight);
				
					if (mID + 2){
						Int_t mPID = TMath::Abs(MCTrack_fPdgCode[mID]);
						if(mPID != 2112)	n_neutron = n_neutron + weight;
						bin = PDGbin(mPID);				     
						Mother_neutron->Fill(bin, weight);
					}	
				}
				if(iPDG == 130){ // K_Long
					Mom_KL->Fill(P,weight);
					StartXY_KL->Fill(MCTrack_fStartX[i],MCTrack_fStartY[i],weight);
					StartZ_KL->Fill(MCTrack_fStartZ[i],weight);
					if (mID + 2){
						Int_t mPID = TMath::Abs(MCTrack_fPdgCode[mID]);
						
						if(mPID != 130) n_KL = n_KL + weight;
					   bin = PDGbin(mPID);	
						Mother_KL->Fill(bin, weight);
					}	
				}
				if(iPDG == 310){ // K_Short
					Mom_KS->Fill(P,weight);
					StartXY_KS->Fill(MCTrack_fStartX[i],MCTrack_fStartY[i],weight);
					StartZ_KS->Fill(MCTrack_fStartZ[i],weight);
					if (mID + 2){
						Int_t mPID = TMath::Abs(MCTrack_fPdgCode[mID]);
						if(mPID != 130) n_KS = n_KS + weight;
						bin = PDGbin(mPID);	
						Mother_KS->Fill(bin, weight);
					}	
				}
			}
		}
		//if (number_of_Neutrons) cout<<number_of_Neutrons<<endl;
		if (has_it) interestingEvents+=weight;
	}	
//----------------------------------------------------------------------
	cout<<n_neutron<<"  "<<n_neutron_Zoom<<"  "<<n_KL<<"  "<<n_KS<<endl;
	cout<<interestingEvents<<endl;
				
//----------------------------------------------------------------------	
	
	if(writeOption){
		PDGMC->Write();
		Mom_neutron->Write();
		StartXY_neutron->Write();
		StartXY_neutron_Zoom->Write();
		StartZ_neutron->Write();
		Mother_neutron->Write();
		
		DecayXY_neutron->Write();
		DecayZ_neutron->Write();
		Daughter_neutron->Write();
		
		Mom_KL->Write();
		StartXY_KL->Write();
		StartZ_KL->Write();
		Mother_KL->Write();
		
		DecayXY_KL->Write();
		DecayZ_KL->Write();
		Daughter_KL->Write();
		
		Mom_KS->Write();
		StartXY_KS->Write();
		StartZ_KS->Write();
		Mother_KS->Write();
		
		DecayXY_KS->Write();
		DecayZ_KS->Write();
		Daughter_KS->Write();
	}
//----------------------------------------------------------------------	
	delete PDGMC; 
	
	delete Mom_neutron; delete StartXY_neutron;
	delete StartXY_neutron_Zoom; delete StartZ_neutron;
	delete Mother_neutron; 
	
	delete Mom_KL;	delete StartXY_KL;
	delete StartZ_KL;	delete Mother_KL;

	delete Mom_KS;	delete StartXY_KS;
	delete StartZ_KS;	delete Mother_KS;
	
}

void shipcbmsim::newGenie(){
	cout<<"newGenie"<<endl;
	fChain->SetBranchStatus("MCTrack*",1); 
	BranchAddresses();
	
	TH2D *StartXY = new TH2D("newGenieStartXY","xy;x[cm];y[cm]",100,-1000,1000,100,-1050,1050);
	TH2D *StartXY1 = new TH2D("newGenieStartXY1","xy;x[cm];y[cm]",1000,-300,300,1000,-600,600);
	TH2D *StartXY2 = new TH2D("newGenieStartXY2","xy;x[cm];y[cm]",1000,-300,300,1000,-600,600);
	TH2D *StartXY3 = new TH2D("newGenieStartXY3","xy;x[cm];y[cm]",1000,-300,300,1000,-600,600);
	TH2D *StartXY4 = new TH2D("newGenieStartXY4","xy;x[cm];y[cm]",1000,-300,300,1000,-600,600);
	TH1D *StartZ = new TH1D("newGenieStartZ","z;z[cm]",100,-2500,3700);
	Long64_t nentries = fChain->GetEntriesFast();
	
	Double_t delta = 1;
	Double_t r = 10;
	Double_t a = 250 + delta;
	Double_t asmall = 186 + delta;
	Double_t b = 500 + delta;
	
	
	
	
	std::vector<Int_t> InnerVesselList;
	std::vector<Int_t> OuterVesselList;
	
	for(Int_t j = 0; j<nentries;j++){
		fChain->GetEntry(j);
		
		Float_t z = MCTrack_fStartZ[0];
		Float_t x = MCTrack_fStartX[0];
		Float_t y = MCTrack_fStartY[0];
		Int_t iPDG = TMath::Abs(MCTrack_fPdgCode[0]);
		
		//if (z>-2440 && z< 3478){
			StartXY->Fill(x,y);
			StartZ->Fill(z);
			//if (z < -1979){
				//if (x*x/(asmall*asmall) + y*y/(b*b)<1){ StartXY1->Fill(x,y); InnerVesselList.push_back(j);}
				//else{StartXY2->Fill(x,y);OuterVesselList.push_back(j);}
			//}
			//else if(z > -1957){
				//if (x*x/(a*a) + y*y/(b*b)<1){StartXY3->Fill(x,y);InnerVesselList.push_back(j);}
				//else{StartXY4->Fill(x,y);OuterVesselList.push_back(j);}
			//}	
		//}
		//if (!(iPDG == 12 || iPDG == 14 || iPDG == 16)) cout<<iPDG<<endl;	
	}
	
	//cout<<"Start writing Vesselevents"<<endl;
	//fChain->SetBranchStatus("*",1);
	//TFile *innervesselfile = new TFile("InnerVessel."+filename+".root","recreate");
	//cout<<"Copy tree"<<endl;
	//TTree *innervesselevents = fChain->CloneTree(0);
	
	//cout<<"loop over Vessel events"<<endl;
	//for (std::vector<Int_t>::iterator it = InnerVesselList.begin(); it != InnerVesselList.end(); ++it){
		//fChain->GetEntry(*it);
		//innervesselevents->Fill();
	//}
	//cout<<"Loop done"<<endl;
	//innervesselevents->Write();
	//cout<<"tree written"<<endl;
	//innervesselfile->Close();
	//cout<<"File closed"<<endl;

	//cout<<"Start writing Vesselevents"<<endl;
	//fChain->SetBranchStatus("*",1);
	//TFile *outervesselfile = new TFile("OuterVessel."+filename+".root","recreate");
	//cout<<"Copy tree"<<endl;
	//TTree *outervesselevents = fChain->CloneTree(0);
	
	//cout<<"loop over Vessel events"<<endl;
	//for (std::vector<Int_t>::iterator it = OuterVesselList.begin(); it != OuterVesselList.end(); ++it){
		//fChain->GetEntry(*it);
		//outervesselevents->Fill();
	//}
	//cout<<"Loop done"<<endl;
	//outervesselevents->Write();
	//cout<<"tree written"<<endl;
	//outervesselfile->Close();
	//cout<<"File closed"<<endl;
	
	{
		TCanvas *c6 = new TCanvas("c6","c6",1600,400);
		c6->Divide(4,1);
		c6->cd(1);
		StartXY1->DrawCopy("COLZ");
		c6->cd(2);
		StartXY2->DrawCopy("COLZ");
		c6->cd(3);
		StartXY3->DrawCopy("COLZ");
		c6->cd(4);
		StartXY4->DrawCopy("COLZ");
		c6->SaveAs("NewGenie-StartXY1...4.png");
		c6->Close();
		
		TCanvas *c5 = new TCanvas("c5","c5",800,400);
		c5->Divide(2,1);
		c5->cd(1);
		StartZ->DrawCopy();
		gPad->SetLogy();
		c5->cd(2);
		StartXY->DrawCopy("COLZ");
		gPad->SetLogy(0);
		gPad->SetLogx(0);
		c5->Update();
		c5->SaveAs("NewGenie-StartVessel.png");
		c5->Close();
	}
	
}
//-----------------------------------------------------------------------------------------------
void shipcbmsim::newCosmics(){
	cout<<"newCosmics"<<endl;
	
	fChain->SetBranchStatus("*",0);
	fChain->SetBranchStatus("vetoPoint",1);
	fChain->SetBranchStatus("muonPoint",1);
	fChain->SetBranchStatus("strawtubesPoint",1);
	fChain->SetBranchStatus("EcalPointLite",1);
	fChain->SetBranchStatus("vetoPoint.fTrackID",1);
	fChain->SetBranchStatus("muonPoint.fTrackID",1);
	fChain->SetBranchStatus("strawtubesPoint.fTrackID",1);
	fChain->SetBranchStatus("EcalPointLite.fTrackID",1);
	fChain->SetBranchStatus("MCTrack*",1);
	BranchAddresses();
	
	TH2D *ENOH = new TH2D("newCosmicsE-NoH","E-NoH; |P_{#mu}| [GeV];\# Hits",500,0.1,1000,20,0,4);
	//TH2D *ENOH = new TH2D("newCosmicsE-NoH","E-NoH; |P_{#mu}| [GeV];\# Hits",100,-1,3,20,0,4);
	//BinLogX(ENOH);
	BinLogY(ENOH);
	//TH1D *E0Hs = new TH1D("newCosmicsE-0Hs","E-0Hs; |P_{#mu}| [GeV]",50,-1,3);
	TH1D *E0Hs = new TH1D("newCosmicsE-0Hs","E-0Hs; |P_{#mu}| [GeV]",100,0.1,3);
	//BinLogX(E0Hs);
	TH1D *allE = new TH1D("newCosmicsallE","allE; |P_{#mu}| [GeV]",100,0.1,3);
	
	TH2D *StartXZ = new TH2D("newCosmicsxz","xz;x[cm];z[cm]",40,-1600,1600,88,-5000,6000);
	TH1D *StartP = new TH1D("newCosmicsP","P;P[GeV]",100,-1,3);
	BinLogX(StartP);
	TH1D *StartTheta = new TH1D("newCosmics#theta","#theta; #theta",50,0,4);
	TH1D *StartMulti = new TH1D("newCosmicsMulti","Multi; multiplicity",20,1,21);
	
	TH1D *PDGDaughter = new TH1D("newCosmicsPDGDaughter","PDGDaughter",14,0,14);
	
	TH1I numberofhits("newCosmicsnumberofhits","numberofhits",20,1,21);
	
	
	TH1D *z = new TH1D("newCosmicsz","z;z[cm]",35,-5000,-2000);
	
	Long64_t nentries = fChain->GetEntriesFast();
	if (nentries > 800000) {Float_t scale = 1/10.0;cout<<"scale: "<<scale<<endl;}
   else Float_t scale = 1;
	
	SetPDGLabel(PDGDaughter);
//----------------------------------------------------------------------			
	for(Int_t j = 0; j<nentries;j++){
		fChain->GetEntry(j);

		// determine multiplicity, find all mothers	
		Int_t multiplicity = 0;
		for(Int_t i = 0; i<MCTrack_;i++){
			Float_t weight = MCTrack_fW[i]*scale;
			if (MCTrack_fMotherId[i] == -1 && MCTrack_fStartY[i] > 1899){ //info about starting muons
				multiplicity++; 
				StartXZ->Fill(MCTrack_fStartX[i],MCTrack_fStartZ[i],weight); //MCTrack_fW[i]
				z->Fill(MCTrack_fStartZ[i],weight);
			}
			
			if (MCTrack_fMotherId[i] == 0){ // daughter particles of starting muons
				Int_t bin = PDGbin(TMath::Abs(MCTrack_fPdgCode[i]));
				PDGDaughter->Fill(bin,weight);
			}
		}
		
	   StartMulti->Fill(multiplicity,scale);           
//----------------------------------------------------------------------
		if (multiplicity != 1){
			Int_t mothersarray [20];
			for(Int_t i = 0; i<multiplicity;i++){
				Int_t iTrack = -1;
				do{
					iTrack++;
				}while(MCTrack_fMotherId[iTrack] != -1 && iTrack < MCTrack_);
				mothersarray[i] = iTrack;
			}
			
			numberofhits.Reset();
			
		   for(Int_t i = 0; i<vetoPoint_;i++){
				Int_t iTrack = vetoPoint_fTrackID[i];
				Int_t tempTrack = -999;
		      do{
					tempTrack = iTrack;
					if(iTrack == -2){break;}
					iTrack = MCTrack_fMotherId[iTrack];
				}while(iTrack != -1);
					
				if(tempTrack !=-2){ 					 
					Int_t tmp = 0;
					for(Int_t k = 0; k<multiplicity;k++){
						 tmp++;
						 if (tempTrack == mothersarray[k]) break;
					}
					numberofhits.Fill(tmp);
				}
			}	
			for(Int_t i = 0; i<muonPoint_;i++){
				Int_t iTrack = muonPoint_fTrackID[i];
				Int_t tempTrack = -999;
		      do{
					tempTrack = iTrack;
					if(iTrack ==-2){break;}
					iTrack = MCTrack_fMotherId[iTrack];
				}while(iTrack != -1);
				if(tempTrack !=-2){ 					 
					Int_t tmp = 0;
					for(Int_t k = 0; k<multiplicity;k++){
						 tmp++;
						 if (tempTrack == mothersarray[k]) break;
					}
					numberofhits.Fill(tmp);
				}
			}
			for(Int_t i = 0; i<EcalPointLite_;i++){
				Int_t iTrack = EcalPointLite_fTrackID[i];
				Int_t tempTrack = -999;
		      do{
					tempTrack = iTrack;
					if(iTrack ==-2){break;}
					iTrack = MCTrack_fMotherId[iTrack];
				}while(iTrack != -1);
				if(tempTrack !=-2){ 					 
					Int_t tmp = 0;
					for(Int_t k = 0; k<multiplicity;k++){
						 tmp++;
						 if (tempTrack == mothersarray[k]) break;
					}
					numberofhits.Fill(tmp);
				}
			}
			for(Int_t i = 0; i<strawtubesPoint_;i++){
				Int_t iTrack = strawtubesPoint_fTrackID[i];
				Int_t tempTrack = -999;
		      do{
					tempTrack = iTrack;
					if(iTrack == -2){break;}
					iTrack = MCTrack_fMotherId[iTrack];
				}while(iTrack != -1);
				if(tempTrack !=-2){ 					 
					Int_t tmp = 0;
					for(Int_t k = 0; k<multiplicity;k++){
						 tmp++;
						 if (tempTrack == mothersarray[k]) break;
					}
					numberofhits.Fill(tmp);
				}
			}

//----------------------------------------------------------------------			
			Int_t temp = 0;
			for(Int_t i = 0; i<multiplicity;i++){
				Int_t mo = mothersarray[i];
				temp++;	
				Float_t Px = MCTrack_fPx[mo];
				Float_t Py = MCTrack_fPy[mo];
				Float_t Pz = MCTrack_fPz[mo];
				Float_t P = TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);
				StartP->Fill(P,MCTrack_fW[mo]*scale);
				StartTheta->Fill(TMath::ACos(Py/P),scale);
				Int_t hits = numberofhits.GetBinContent(temp);
				if (!hits) {E0Hs->Fill(P,scale);}//,MCTrack_fW[mo]
				else{ENOH->Fill(P,hits,scale);}//,MCTrack_fW[mo]
				allE->Fill(P,scale);
			}	
		}
		else{
			Float_t Px = MCTrack_fPx[0];
			Float_t Py = MCTrack_fPy[0];
			Float_t Pz = MCTrack_fPz[0];
			Float_t P = TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);
			StartP->Fill(P,MCTrack_fW[0]*scale);
			StartTheta->Fill(TMath::ACos(Py/P),scale);
			Int_t hits = vetoPoint_+muonPoint_+EcalPointLite_+strawtubesPoint_;
			if (!hits) {E0Hs->Fill(P,scale);}//,MCTrack_fW[0]
			else {ENOH->Fill(P,hits,scale);}//,MCTrack_fW[0]
			allE->Fill(P,scale);
		}
	}
		
//----------------------------------------------------------------------
	
	if(writeOption){
		ENOH->Write();
		E0Hs->Write(); 
		allE->Write();
		StartXZ->Write();
		StartP->Write();
		StartTheta->Write();
		StartMulti->Write();
		
		PDGDaughter->Write();
		z->Write();
	}
	
	delete ENOH; delete E0Hs; delete allE;	delete StartXZ; delete StartP;
	delete StartTheta; delete StartMulti; delete PDGDaughter; delete z;
}
//-----------------------------------------------------------------------------------------------
void shipcbmsim::newPlots(){
	cout<<"new Plots"<<endl;
	fChain->SetBranchStatus("*",0);
	fChain->SetBranchStatus("veto*",1);
	fChain->SetBranchStatus("muon*",1);
	fChain->SetBranchStatus("Ecal*",1);
	fChain->SetBranchStatus("MCTrack*",1);
	fChain->SetBranchStatus("EcalPointLite*",0);
	BranchAddresses();
	
	Long64_t nentries = fChain->GetEntriesFast();
	Int_t det, bin;
	Float_t weight;
	Int_t vfDet = 40; //first DetID for veto
	Int_t firstDetID=89; //to be changed for new Detector configuration MuVeto

	TH2D *XY = new TH2D("xy","xy;x[cm];y[cm]",50,-1000,1000,50,-1000,1000);
	
	
	TH1D *Momentum_N = new TH1D("Momentum_N","Momentum_N;|P|[GeV]",50,0,250);
	TH1D *Momentum_mu = new TH1D("Momentum_mu","Momentum_mu;|P|[GeV]",150,0,200);
	TH1D *Momentum_pi = new TH1D("Momentum_pi","Momentum_pi;|P|[GeV]",150,0,200);
	
	TH1D *Phi_mu = new TH1D("Phi_mu","Phi_mu",20,-4,4);
	TH1D *Phi_pi = new TH1D("Phi_pi","Phi_pi",20,-4,4);
	TH1D *Theta_mu = new TH1D("Theta_mu","Theta_mu",20,0,4);
	TH1D *Theta_pi = new TH1D("Theta_pi","Theta_pi",20,0,4);
  
	TH2D *MotherXY = new TH2D("Mxy","Mxy;x[cm];y[cm]",50,-500,500,50,-500,500);
	TH1D *MotherZ = new TH1D("Mz","Mz;z[cm]",50,-2600,2600);
  
	Int_t out = 0; Int_t in =0;
	for(Int_t j = 0; j<nentries;j++){
		fChain->GetEntry(j);
	  //-------------------------------------
		for(Int_t i = 0; i<vetoPoint_;i++){
			Int_t iTrack = vetoPoint_fTrackID[i];
			if(iTrack>0){
				weight = MCTrack_fW[iTrack];
				if (weight == 0) weight = 1;
				if (vetoPoint_fDetectorID[i] == vfDet + 7){
					Int_t iMother = MCTrack_fMotherId[iTrack];
					if (MCTrack_fPdgCode[iTrack] == 11){
						if (iMother >0){
							MotherXY->Fill(MCTrack_fStartX[iMother],MCTrack_fStartY[iMother],weight);
							MotherZ->Fill(MCTrack_fStartZ[iMother],weight);
						}
					}
				}	
			}	
		}
	//--------------------------------------------------	
		//det = 5;
		//for(Int_t i = 0; i<EcalPointLite_;i++){
			//Int_t iTrack = EcalPointLite_fTrackID[i];
			//if(iTrack>0){	
			//} 
		//}
	//--------------------------------------------------	
		//det = 6;
		//for(Int_t i = 0; i<muonPoint_;i++){
			//Int_t iTrack = muonPoint_fTrackID[i];
			//if(iTrack>0){
			//} 
		//}
				
		if(MCTrack_fPdgCode[1] == 9900014){
			Bool_t decayavailable = 0;
			
			Double_t Px_N = MCTrack_fPx[1];
			Double_t Py_N = MCTrack_fPy[1];
			Double_t Pz_N = MCTrack_fPz[1];			
			Double_t P_N = TMath::Sqrt(Px_N*Px_N + Py_N*Py_N + Pz_N*Pz_N);
			Momentum_N->Fill(P_N);			 
			TLorentzVector *L_N = new TLorentzVector(Px_N, Py_N, Pz_N, TMath::Sqrt(P_N*P_N + 1));
			
			TLorentzVector *L_mu = new TLorentzVector();
			TLorentzVector *L_pi = new TLorentzVector();
			if(P_N > 0){
				Float_t Px = MCTrack_fPx[2];
				Float_t Py = MCTrack_fPy[2];
				Float_t Pz = MCTrack_fPz[2];
				Int_t PDG2 = TMath::Abs(MCTrack_fPdgCode[2]);
				if(PDG2 == 13){
					Float_t P_mu = TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);
					Momentum_mu->Fill(P_mu);	
					TLorentzVector *L_mu = new TLorentzVector(Px,Py,Pz,TMath::Sqrt(P_mu*P_mu + 0.1056*0.1056));		 
					*L_pi = *L_N - *L_mu;
					Float_t P_pi = L_pi->P();
					Momentum_pi->Fill(P_pi);
					decayavailable = 1;	
					//if (P_mu < 4 || P_pi<4){
						//cout<<P_N<<" "<<P_mu<<" "<<P_pi<<endl;
					//}
				}
				else if(PDG2 == 211){
					Float_t P_pi = TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);
					TLorentzVector *L_pi = new TLorentzVector(Px,Py,Pz,TMath::Sqrt(P_pi*P_pi + 0.1395*0.1395));
					Momentum_pi->Fill(P_pi);	
					*L_mu = *L_N - *L_pi;
					Float_t P_mu = L_mu->P();
					Momentum_mu->Fill(P_mu);
					decayavailable = 1;
					//if (P_mu < 4 || P_pi < 4){
						//cout<<P_N<<" "<<P_mu<<" "<<P_pi<<endl;
					//}
					//cout<<P_mu<<" "<<P_pi<<endl;
				}
				if(decayavailable){
					// TLorentzVector temp = *L_N - *L_mu - *L_pi;
					// cout<<temp.Px()<<" "<<temp.Py()<<" "<<temp.Pz()<<" "<<temp.E()<<" "<<endl<<endl;
					// cout<<L_mu->Px()<<" "<<L_mu->Py()<<" "<<L_mu->Pz()<<" "<<L_mu->E()<<" "<<endl;
					L_mu->Boost(-L_N->BoostVector());
					L_pi->Boost(-L_N->BoostVector());
					Phi_mu->Fill(L_mu->Phi());
					Phi_pi->Fill(L_pi->Phi());
					Theta_mu->Fill(L_mu->Theta());
					Theta_pi->Fill(L_pi->Theta());
				}
			}
			delete L_N;
			delete L_mu;
			delete L_pi;
		}
	}
		
   {//Plotting done here
		
	
		
		TCanvas *c2 = new TCanvas("c2","c2",800,400);
		c2->Divide(2,1);	
		c2->cd(1);
		if (MCTrack_fPdgCode[1] == 9900014){
			Phi_mu->DrawCopy();
			gPad->SetLogy(0);
			c2->cd(2);
			Phi_pi->DrawCopy();
			gPad->SetLogy(0);
			c2->Update();
			c2->SaveAs("Phi.png");
			c2->cd(1);
			//TF1 *fa1 = new TF1("fa","[0]*sin(x)*sin(x)",0,3.14);
			//Theta_mu->Fit(fa,"","SAME");
			Theta_mu->DrawCopy();
			//fa->DrawCopy("SAME");
			gPad->SetLogy(0);
			c2->cd(2);
			//TF1 *fa2 = new TF1("fa1","[0]*sin(x)",0,3.14);
			//Theta_pi->Fit(fa2,"","SAME");
			Theta_pi->DrawCopy();
			//fa2->DrawCopy("SAME");
			gPad->SetLogy(0);
			c2->Update();
			c2->SaveAs("Theta.png");
		}
		c2->cd(1);
		MotherXY->DrawCopy();
		c2->cd(2);
		MotherZ->DrawCopy();
		c2->Update();
		c2->SaveAs("Mother.png");
		c2->Close();
		
		//TCanvas *c3 = new TCanvas("c3","c3",400,400);
		//c3->cd(1);
		//XY->DrawCopy("COLZ");
		//c3->SaveAs("XY.png");
		
		if (MCTrack_fPdgCode[1] == 9900014){
			TCanvas *c4 = new TCanvas("c4","c4",1600,400);
			c4->Divide(4,1);
		}
		else{
			TCanvas *c4 = new TCanvas("c4","c4",400,400);
			c4->Divide(1,1);
		}
		c4->cd(1);
		
		if (MCTrack_fPdgCode[1] == 9900014){
			c4->cd(2);
			Momentum_N->DrawCopy();	
			gPad->SetLogy(1);	
			c4->cd(3);
			Momentum_mu->DrawCopy();	
			gPad->SetLogy(1);	
			c4->cd(4);
			Momentum_pi->DrawCopy();	
			gPad->SetLogy(1);	
		}
		c4->Update();
		c4->SaveAs("MC.png");
		c4->Close();
	
   }
}

void shipcbmsim::testClone(){
	
	//cout<<"testClone"<<endl;
	//fChain->SetBranchStatus("*",1); 
		
	//Long64_t nentries = fChain->GetEntriesFast();
   
////----------------------------------------------------------------------
	//TFile *f = fChain->GetCurrentFile();
	//f->ls();
	
	//TFolder *a = f->Get("cbmroot");
	//TList *b = f->Get("BranchList");
	//FairFileHeader *c = f->Get("FileHeader");
	//TProcessID *d = f->Get("ProcessID0");
	
	
	


	
	
	//TFile *signalfile = new TFile("testsignalFile2.root","recreate");
	//TTree *signalevents = fChain->CloneTree(0);
	
	//for(Int_t j = 0; j<nentries;j++){
		//fChain->GetEntry(j);
		//signalevents->Fill();
	//}	
	
	//////cout<<signalevents->GetEntries()<<endl;
	//////cout<<fChain->GetEntries()<<endl;
	
	//a->Write();
	//b->Write("BranchList", TObject::kSingleKey);
	//c->Write("FileHeader");
	//d->Write();
	
	//signalevents->Write();
	//cout<<"-----------------------------------------------------------"<<endl
	//signalfile->ls();
	//signalfile->Close();


	//Double_t x = 250;
	//Double_t y = 0;
	//Double_t z = 4000;
	//Double_t px = 0;
	//Double_t py = 10;
	//Double_t pz = 250;
	
	
	
	//Double_t theta=TMath::ATan(1/z*TMath::Sqrt(x*x+y*y) );
  //Double_t c=TMath::Cos(theta);
  //Double_t s=TMath::Sin(theta);
  ////rotate around y-axis
  //Double_t px1=c*px+s*pz;
  //Double_t pzr=-s*px+c*pz;
  //Double_t phi=TMath::ATan2(y,x);
  //c=TMath::Cos(phi);
  //s=TMath::Sin(phi);
  ////rotate around z-axis
  //Double_t pxr=c*px1-s*py;
  //Double_t pyr=s*px1+c*py;
  
  //cout<<pxr<<"  "<<pyr<<"  "<<pzr<<endl;
}

std::vector<Int_t> shipcbmsim::Signalquick(){
	cout<<"Signalquick"<<endl;
	fChain->SetBranchStatus("*",0); 
	
	fChain->SetBranchStatus("veto*",1);
	fChain->SetBranchStatus("straw*",1);
	fChain->SetBranchStatus("MCTrack*",1);
	
	{BranchAddresses();
		fChain->SetBranchStatus("strawtubesPoint.fPx",0);
		fChain->SetBranchStatus("strawtubesPoint.fPy",0);
		fChain->SetBranchStatus("strawtubesPoint.fPz",0);
		fChain->SetBranchStatus("strawtubesPoint.fTime",0);
		fChain->SetBranchStatus("strawtubesPoint.fLength",0);	
		fChain->SetBranchStatus("strawtubesPoint.fDetectorID",0);
		fChain->SetBranchStatus("vetoPoint.fPx",0);
		fChain->SetBranchStatus("vetoPoint.fPy",0);
		fChain->SetBranchStatus("vetoPoint.fPz",0);
		fChain->SetBranchStatus("vetoPoint.fTime",0);
		fChain->SetBranchStatus("vetoPoint.fLength",0);	
		fChain->SetBranchStatus("MCTrack.fUniqueID",0);	
		fChain->SetBranchStatus("MCTrack.fBits", 0);	
		fChain->SetBranchStatus("MCTrack.fPdgCode", 0);	
		fChain->SetBranchStatus("MCTrack.fMotherId", 0);	
		fChain->SetBranchStatus("MCTrack.fPx", 0);	
		fChain->SetBranchStatus("MCTrack.fPy", 0);	
		fChain->SetBranchStatus("MCTrack.fPz",0);	
		fChain->SetBranchStatus("MCTrack.fStartX",0);	
		fChain->SetBranchStatus("MCTrack.fStartY",0);	
		fChain->SetBranchStatus("MCTrack.fStartZ",0);	
		fChain->SetBranchStatus("MCTrack.fStartT",0);	
		fChain->SetBranchStatus("MCTrack.fNPoints",0);	
	}
		
	Float_t weight, t;
	Int_t bin, phisegment, zsegment, segment;
	
	Long64_t nentries = fChain->GetEntriesFast();
	
	Float_t pi = TMath::Pi();
	
	Float_t a = 250; //radius x
	Float_t asmall = 186; //radius x
	Float_t b = 500; //radius y
	Float_t r = 30;
					
	Float_t zminus = -2478; //Detektorausmaße
	Float_t zplus = 3678; // hinteres Ende des Taggers
	Float_t zstepsize = 100;
	
	Int_t N = 16; //even number >15
	Float_t dphi = 2*pi/N;
	Float_t phistart = dphi/2; 
	
	const Int_t nBins = N*TMath::CeilNint((zplus-zminus)/zstepsize);	
	Float_t SegmentIsHit [992] = {}; //nBins geht nicht als Arraylänge
	
	std::vector<Int_t> SignalList;
	
	Double_t cat1 = 0;
	Double_t cat2 = 0;
	
	Double_t cat1un = 0;
	Double_t cat2un = 0;
	
	Int_t TrackStationIsHit [5] = {}; 

//----------------------------------------------------------------------
	for(Int_t j = 0; j<nentries;j++){
		if (!(j % 100000)) cout<<j<<endl;
		fChain->GetEntry(j);
				
		for(Int_t i = 0; i<vetoPoint_;i++){
			Float_t z = vetoPoint_fZ[i];
			if(z < zplus && z > zminus){ //cut überprüfen!
				Float_t x = vetoPoint_fX[i];
				Float_t y = vetoPoint_fY[i];
				Float_t ELoss = vetoPoint_fELoss[i];
				if (z > -1968) t = TMath::ATan2(a*y,b*x)+2*pi; // t [pi,3pi]	
				else if (z < -1968) t = TMath::ATan2(asmall*y,b*x)+2*pi; // t [pi,3pi]	
				phisegment = TMath::FloorNint((t+phistart)/dphi)%N;
				zsegment = TMath::FloorNint((z-zminus)/zstepsize);
				segment = zsegment*N + phisegment;
				SegmentIsHit[segment]  += ELoss;				
			}	
		}
		
		Int_t hitSegments  = 0;
		for (Int_t n = 0;n<nBins;n++){
			if (SegmentIsHit[n] > 0.045) {
				hitSegments++; 
				SegmentIsHit[n] = 0;
			}			
		}
		
		for(Int_t i = 0; i<strawtubesPoint_;i++){
			Float_t x = strawtubesPoint_fX[i];
			Float_t y = strawtubesPoint_fY[i];
			Float_t z = strawtubesPoint_fZ[i];
			if (x*x/((a+r/2)*(a+r/2)) + y*y/((b+r/2)*(b+r/2)) < 1){	// z values anpassen für Y = 6m
				if(z < -1957){TrackStationIsHit[0]++; } //veto, Delta_z = 20
				else if(z<2619){TrackStationIsHit[1]++;} //T1, Delta_z = 40
				else if(z<2819){TrackStationIsHit[2]++;} //T2, Delta_z = 40
				else if(z<3359){TrackStationIsHit[3]++;} //T3, Delta_z = 40
				else if(z<3559){TrackStationIsHit[4]++;} //T4, Delta_z = 40
				else{cout<<z<<endl;}
			}
		}

		Bool_t threeTS = TrackStationIsHit[1]&&TrackStationIsHit[2]&&TrackStationIsHit[3] || TrackStationIsHit[2]&&TrackStationIsHit[3]&&TrackStationIsHit[4] || TrackStationIsHit[1]&&TrackStationIsHit[3]&&TrackStationIsHit[4] || TrackStationIsHit[2]&&TrackStationIsHit[1]&&TrackStationIsHit[4];
		
		weight = MCTrack_fW[0];
		
		if (threeTS){
			SignalList.push_back(j);	
		}
		else{
			if (hitSegments) {cat2un++; cat2 += weight;}
			else {cat1un++; cat1 += weight;}
		}
		
		if (TrackStationIsHit[0]) {TrackStationIsHit[0] = 0;}
		if (TrackStationIsHit[1]) {TrackStationIsHit[1] = 0;}
		if (TrackStationIsHit[2]) {TrackStationIsHit[2] = 0;}
		if (TrackStationIsHit[3]) {TrackStationIsHit[3] = 0;}
		if (TrackStationIsHit[4]) {TrackStationIsHit[4] = 0;}
	}	
	
	cout<<endl;
	cout<<"cat1 = "<<cat1<<"  cat2 = "<<cat2<<endl;
	cout<<"cat1un = "<<cat1un<<"  cat2un = "<<cat2un<<endl;
		
	return SignalList;
}
