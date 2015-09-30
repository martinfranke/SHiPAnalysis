//Float_t spectrum(Float_t x){
	//if (x<100){return 22*TMath::Power(x,-0.6 -0.285*TMath::Log(x));}
	//else {return 1400*TMath::Power(x,-2.7)*(1/(1+x/115)+0.054/(1+x/850));}
//};

void BinLogX(TH1*h){
	// Transforms X-Axis of 1D Histogram into log scale binning
	TAxis *axis = h->GetXaxis();
	int bins = axis->GetNbins();
	 
	Axis_t from = axis->GetXmin();
	Axis_t to = axis->GetXmax();
	Axis_t width = (to - from) / bins;
	Axis_t *new_bins = new Axis_t[bins + 1];
	 
	for (int i = 0; i <= bins; i++) {
		new_bins[i] = TMath::Power(10, from + i * width);
	}
	axis->Set(bins, new_bins);
	delete new_bins;
	new_bins = NULL;
} 
//void BinLogX(TH2*h){
	//// Transforms X-Axis of 1D Histogram into log scale binning
	//TAxis *axis = h->GetXaxis();
	//int bins = axis->GetNbins();
	 
	//Axis_t from = axis->GetXmin();
	//Axis_t to = axis->GetXmax();
	//Axis_t width = (to - from) / bins;
	//Axis_t *new_bins = new Axis_t[bins + 1];
	 
	//for (int i = 0; i <= bins; i++) {
		//new_bins[i] = TMath::Power(10, from + i * width);
	//}
	//axis->Set(bins, new_bins);
	//delete new_bins;
	//new_bins = NULL;
//} 
void BinLogY(TH2*h){
	// Transforms X-Axis of 1D Histogram into log scale binning
	TAxis *axis = h->GetYaxis();
	int bins = axis->GetNbins();
	 
	Axis_t from = axis->GetXmin();
	Axis_t to = axis->GetXmax();
	Axis_t width = (to - from) / bins;
	Axis_t *new_bins = new Axis_t[bins + 1];
	 
	for (int i = 0; i <= bins; i++) {
		new_bins[i] = TMath::Power(10, from + i * width);
	}
	axis->Set(bins, new_bins);
	delete new_bins;
	new_bins = NULL;
} 
void SetPDGLabel(TH2*PDG){
	PDG->GetXaxis()->SetBinLabel(1,"#mu");
	PDG->GetXaxis()->SetBinLabel(2,"#pi^{#pm}");
	PDG->GetXaxis()->SetBinLabel(3,"e");
	PDG->GetXaxis()->SetBinLabel(4,"#nu");
	PDG->GetXaxis()->SetBinLabel(5,"#gamma");
	PDG->GetXaxis()->SetBinLabel(6,"N");
	PDG->GetXaxis()->SetBinLabel(7,"#pi^{0}");
	PDG->GetXaxis()->SetBinLabel(8,"K_{L}");
	PDG->GetXaxis()->SetBinLabel(9,"K_{S}");
	PDG->GetXaxis()->SetBinLabel(10,"K^{#pm}");
	PDG->GetXaxis()->SetBinLabel(11,"p");
	PDG->GetXaxis()->SetBinLabel(12,"n");
	PDG->GetXaxis()->SetBinLabel(13,"#Lambda");
	PDG->GetXaxis()->SetBinLabel(14,"others");
	PDG->SetLabelSize(0.07);
	PDG->SetLabelOffset(0.01);
//	PDG->SetMinimum(1);		
}
void SetPDGLabel(TH1*PDG){
	PDG->GetXaxis()->SetBinLabel(1,"#mu");
	PDG->GetXaxis()->SetBinLabel(2,"#pi^{#pm}");
	PDG->GetXaxis()->SetBinLabel(3,"e");
	PDG->GetXaxis()->SetBinLabel(4,"#nu");
	PDG->GetXaxis()->SetBinLabel(5,"#gamma");
	PDG->GetXaxis()->SetBinLabel(6,"N");
	PDG->GetXaxis()->SetBinLabel(7,"#pi^{0}");
	PDG->GetXaxis()->SetBinLabel(8,"K_{L}");
	PDG->GetXaxis()->SetBinLabel(9,"K_{S}");
	PDG->GetXaxis()->SetBinLabel(10,"K^{#pm}");
	PDG->GetXaxis()->SetBinLabel(11,"p");
	PDG->GetXaxis()->SetBinLabel(12,"n");
	PDG->GetXaxis()->SetBinLabel(13,"#Lambda");
	PDG->GetXaxis()->SetBinLabel(14,"others");
	PDG->SetLabelSize(0.07);
	PDG->SetLabelOffset(0.01);
//	PDG->SetMinimum(1);		
}

void SetHistLabel(TH1*h){
	h->GetXaxis()->SetLabelSize(0.04);
	h->GetYaxis()->SetLabelSize(0.04);
	
	h->GetXaxis()->SetTitleSize(0.04);	
//	h->GetXaxis()->SetTitleOffset(0.9);
	h->GetYaxis()->SetTitleSize(0.04);	
//	h->GetYaxis()->SetTitleOffset(0.9);
}
void SetHistLabel(TH2*h){
	h->GetXaxis()->SetLabelSize(0.04);
	h->GetYaxis()->SetLabelSize(0.04);
	
	h->GetXaxis()->SetTitleSize(0.04);	
//	h->GetXaxis()->SetTitleOffset(0.9);
	h->GetYaxis()->SetTitleSize(0.04);	
//	h->GetYaxis()->SetTitleOffset(0.9);
}

Int_t PDGbin(Int_t x){
	Int_t bin;
	switch(x){
		case 13:  bin = 0; break;
		case 211: bin = 1; break;
		case 11:  bin = 2; break;
		case 22:  bin = 4; break;
		case 130: bin = 7; break;
		case 321: bin = 9; break;
		case 2212:bin = 10; break;
		case 2112:bin = 11; break;
		case 3122:bin = 12; break;
		case 111: bin = 6; break;
		case 310: bin = 8; break;
		case 9900014: bin = 5; break;
		case 12: case 14: case 16:  bin = 3; break;
		default: bin = 13; //cout<<MCTrack_fPdgCode[iTrack]<<endl;
	}
	return bin;
}
