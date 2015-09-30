void shipcbmsim::BranchAddresses(){
fChain->SetBranchStatus("MCTrack.fUniqueID",0);
fChain->SetBranchStatus("MCTrack.fBits",0);
fChain->SetBranchStatus("MCTrack.fNPoints",0);

fChain->SetBranchStatus("vetoPoint.fUniqueID",0);
fChain->SetBranchStatus("vetoPoint.fBits",0);
fChain->SetBranchStatus("vetoPoint.fRefToLinks",0);
fChain->SetBranchStatus("vetoPoint.fEventId",0);

fChain->SetBranchStatus("ShipRpcPoint.f*",0);

fChain->SetBranchStatus("strawtubesPoint.fUniqueID",0);
fChain->SetBranchStatus("strawtubesPoint.fBits",0);
fChain->SetBranchStatus("strawtubesPoint.fRefToLinks",0);
fChain->SetBranchStatus("strawtubesPoint.fEventId",0);
fChain->SetBranchStatus("strawtubesPoint.fdist2Wire",0);

fChain->SetBranchStatus("EcalPoint.fUniqueID",0);
fChain->SetBranchStatus("EcalPoint.fBits",0);
fChain->SetBranchStatus("EcalPoint.fRefToLinks",0);
fChain->SetBranchStatus("EcalPoint.fEventId",0);

fChain->SetBranchStatus("EcalPointLite.fUniqueID",0);
fChain->SetBranchStatus("EcalPointLite.fBits",0);
fChain->SetBranchStatus("EcalPointLite.fRefToLinks",0);
fChain->SetBranchStatus("EcalPointLite.fEventId",0);

fChain->SetBranchStatus("muonPoint.fUniqueID",0);
fChain->SetBranchStatus("muonPoint.fBits",0);
fChain->SetBranchStatus("muonPoint.fRefToLinks",0);
fChain->SetBranchStatus("muonPoint.fEventId",0);

fChain->SetBranchStatus("HcalPoint.f*",0);

fChain->SetBranchStatus("HcalPointLite.f*",0);

fChain->SetBranchStatus("MCEventHeader.*",0);
}
