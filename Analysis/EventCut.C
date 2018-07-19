#include "EventCut.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using namespace std;

EventCut::EventCut() {
   fChainCut = new TChain();
   //  77Ni
   fChainCut->AddFile("../rootfiles/78Ni/psp14_78Cu_p2p_77Ni_new.root",0,"tree");
   //  78Zn
   //fChainCut->AddFile("../rootfiles/78Ni/psp14_78Zn_new.root",0,"tree");
   //  75Ni
   //fChainCut->AddFile("../rootfiles/78Ni/psp14_75Ni_new.root",0,"tree");
   //  73Ni
   //fChainCut->AddFile("../rootfiles/78Ni/psp14_73Ni_new.root",0,"tree");

   Init();
}

EventCut::~EventCut(){
}

Int_t EventCut::GetEntry(Long64_t entry){
   if (!fChainCut) return 0;
   return fChainCut->GetEntry(entry);
}

void EventCut::Init() {
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   //if (!tree) return;
   //fChain = tree;
   fCurrent = -1;
   fChainCut->SetMakeClass(1);

   fChainCut->SetBranchAddress("EventInfo", &EventInfo_, &b_EventInfo_);
   fChainCut->SetBranchAddress("EventInfo.fUniqueID", EventInfo_fUniqueID, &b_EventInfo_fUniqueID);
   fChainCut->SetBranchAddress("EventInfo.fBits", EventInfo_fBits, &b_EventInfo_fBits);
   fChainCut->SetBranchAddress("EventInfo.fName", EventInfo_fName, &b_EventInfo_fName);
   fChainCut->SetBranchAddress("EventInfo.fTitle", EventInfo_fTitle, &b_EventInfo_fTitle);
   fChainCut->SetBranchAddress("EventInfo.runnumber", EventInfo_runnumber, &b_EventInfo_runnumber);
   fChainCut->SetBranchAddress("EventInfo.eventnumber", EventInfo_eventnumber, &b_EventInfo_eventnumber);
   fChainCut->SetBranchAddress("EventInfo.subsysname", EventInfo_subsysname, &b_EventInfo_subsysname);
   fChainCut->SetBranchAddress("EventInfo.timestamp", EventInfo_timestamp, &b_EventInfo_timestamp);
   fChainCut->SetBranchAddress("EventInfo.comp_val", EventInfo_comp_val, &b_EventInfo_comp_val);
   fChainCut->SetBranchAddress("EventInfo.fBit", EventInfo_fBit, &b_EventInfo_fBit);

   fChainCut->SetBranchAddress("DALINaI", &DALINaI_, &b_DALINaI_);
   fChainCut->SetBranchAddress("DALINaI.fUniqueID", DALINaI_fUniqueID, &b_DALINaI_fUniqueID);
   fChainCut->SetBranchAddress("DALINaI.fBits", DALINaI_fBits, &b_DALINaI_fBits);
   fChainCut->SetBranchAddress("DALINaI.id", DALINaI_id, &b_DALINaI_id);
   fChainCut->SetBranchAddress("DALINaI.fpl", DALINaI_fpl, &b_DALINaI_fpl);
   fChainCut->SetBranchAddress("DALINaI.name", DALINaI_name, &b_DALINaI_name);
   fChainCut->SetBranchAddress("DALINaI.fDataState", DALINaI_fDataState, &b_DALINaI_fDataState);
   fChainCut->SetBranchAddress("DALINaI.fADC", DALINaI_fADC, &b_DALINaI_fADC);
   fChainCut->SetBranchAddress("DALINaI.fTDC", DALINaI_fTDC, &b_DALINaI_fTDC);
   fChainCut->SetBranchAddress("DALINaI.layer", DALINaI_layer, &b_DALINaI_layer);
   fChainCut->SetBranchAddress("DALINaI.theta", DALINaI_theta, &b_DALINaI_theta);
   fChainCut->SetBranchAddress("DALINaI.fXPos", DALINaI_fXPos, &b_DALINaI_fXPos);
   fChainCut->SetBranchAddress("DALINaI.fYPos", DALINaI_fYPos, &b_DALINaI_fYPos);
   fChainCut->SetBranchAddress("DALINaI.fZPos", DALINaI_fZPos, &b_DALINaI_fZPos);
   fChainCut->SetBranchAddress("DALINaI.costheta", DALINaI_costheta, &b_DALINaI_costheta);
   fChainCut->SetBranchAddress("DALINaI.fEnergy", DALINaI_fEnergy, &b_DALINaI_fEnergy);
   fChainCut->SetBranchAddress("DALINaI.fDoppCorEnergy", DALINaI_fDoppCorEnergy, &b_DALINaI_fDoppCorEnergy);
   fChainCut->SetBranchAddress("DALINaI.fEnergyWithoutT", DALINaI_fEnergyWithoutT, &b_DALINaI_fEnergyWithoutT);
   fChainCut->SetBranchAddress("DALINaI.fTime", DALINaI_fTime, &b_DALINaI_fTime);
   fChainCut->SetBranchAddress("DALINaI.fTimeOffseted", DALINaI_fTimeOffseted, &b_DALINaI_fTimeOffseted);
   fChainCut->SetBranchAddress("DALINaI.fTimeTrueEnergy", DALINaI_fTimeTrueEnergy, &b_DALINaI_fTimeTrueEnergy);
   fChainCut->SetBranchAddress("DALINaI.fTimeTrueDoppCorEnergy", DALINaI_fTimeTrueDoppCorEnergy, &b_DALINaI_fTimeTrueDoppCorEnergy);
   fChainCut->SetBranchAddress("DALINaI.fTimeTrueTime", DALINaI_fTimeTrueTime, &b_DALINaI_fTimeTrueTime);
   fChainCut->SetBranchAddress("DALINaI.fTimeTrueTimeOffseted", DALINaI_fTimeTrueTimeOffseted, &b_DALINaI_fTimeTrueTimeOffseted);
   fChainCut->SetBranchAddress("dalimultwotime", &dalimultwotime, &b_dalimultwotime);
   fChainCut->SetBranchAddress("dalimult", &dalimult, &b_dalimult);
   fChainCut->SetBranchAddress("dalitimetruemult", &dalitimetruemult, &b_dalitimetruemult);
   fChainCut->SetBranchAddress("dalimultthres", &dalimultthres, &b_dalimultthres);
   fChainCut->SetBranchAddress("dalitimetruemultthres", &dalitimetruemultthres, &b_dalitimetruemultthres);

   fChainCut->SetBranchAddress("BigRIPSBeam", &BigRIPSBeam_, &b_BigRIPSBeam_);
   fChainCut->SetBranchAddress("BigRIPSBeam.fUniqueID", BigRIPSBeam_fUniqueID, &b_BigRIPSBeam_fUniqueID);
   fChainCut->SetBranchAddress("BigRIPSBeam.fBits", BigRIPSBeam_fBits, &b_BigRIPSBeam_fBits);
   fChainCut->SetBranchAddress("BigRIPSBeam.id", BigRIPSBeam_id, &b_BigRIPSBeam_id);
   fChainCut->SetBranchAddress("BigRIPSBeam.fpl", BigRIPSBeam_fpl, &b_BigRIPSBeam_fpl);
   fChainCut->SetBranchAddress("BigRIPSBeam.name", BigRIPSBeam_name, &b_BigRIPSBeam_name);
   fChainCut->SetBranchAddress("BigRIPSBeam.fDataState", BigRIPSBeam_fDataState, &b_BigRIPSBeam_fDataState);
   fChainCut->SetBranchAddress("BigRIPSBeam.brho", BigRIPSBeam_brho, &b_BigRIPSBeam_brho);
   fChainCut->SetBranchAddress("BigRIPSBeam.aoq", BigRIPSBeam_aoq, &b_BigRIPSBeam_aoq);
   fChainCut->SetBranchAddress("BigRIPSBeam.zet", BigRIPSBeam_zet, &b_BigRIPSBeam_zet);
   fChainCut->SetBranchAddress("BigRIPSBeam.beta", BigRIPSBeam_beta, &b_BigRIPSBeam_beta);
   fChainCut->SetBranchAddress("BigRIPSBeam.clight", BigRIPSBeam_clight, &b_BigRIPSBeam_clight);
   fChainCut->SetBranchAddress("BigRIPSBeam.mnucleon", BigRIPSBeam_mnucleon, &b_BigRIPSBeam_mnucleon);
   fChainCut->SetBranchAddress("BigRIPSBeam.nrips", BigRIPSBeam_nrips, &b_BigRIPSBeam_nrips);
   fChainCut->SetBranchAddress("BigRIPSBeam.ripsname[2]", BigRIPSBeam_ripsname, &b_BigRIPSBeam_ripsname);
   fChainCut->SetBranchAddress("BigRIPSBeam.tofname", BigRIPSBeam_tofname, &b_BigRIPSBeam_tofname);
   fChainCut->SetBranchAddress("BigRIPSBeam.icname", BigRIPSBeam_icname, &b_BigRIPSBeam_icname);

   fChainCut->SetBranchAddress("BigRIPSPPAC", &BigRIPSPPAC_, &b_BigRIPSPPAC_);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fUniqueID", BigRIPSPPAC_fUniqueID, &b_BigRIPSPPAC_fUniqueID);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fBits", BigRIPSPPAC_fBits, &b_BigRIPSPPAC_fBits);
   fChainCut->SetBranchAddress("BigRIPSPPAC.id", BigRIPSPPAC_id, &b_BigRIPSPPAC_id);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fpl", BigRIPSPPAC_fpl, &b_BigRIPSPPAC_fpl);
   fChainCut->SetBranchAddress("BigRIPSPPAC.name", BigRIPSPPAC_name, &b_BigRIPSPPAC_name);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fDataState", BigRIPSPPAC_fDataState, &b_BigRIPSPPAC_fDataState);
   fChainCut->SetBranchAddress("BigRIPSPPAC.xzpos", BigRIPSPPAC_xzpos, &b_BigRIPSPPAC_xzpos);
   fChainCut->SetBranchAddress("BigRIPSPPAC.yzpos", BigRIPSPPAC_yzpos, &b_BigRIPSPPAC_yzpos);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTX1Raw", BigRIPSPPAC_fTX1Raw, &b_BigRIPSPPAC_fTX1Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTX2Raw", BigRIPSPPAC_fTX2Raw, &b_BigRIPSPPAC_fTX2Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTY1Raw", BigRIPSPPAC_fTY1Raw, &b_BigRIPSPPAC_fTY1Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTY2Raw", BigRIPSPPAC_fTY2Raw, &b_BigRIPSPPAC_fTY2Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTARaw", BigRIPSPPAC_fTARaw, &b_BigRIPSPPAC_fTARaw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fQX1Raw", BigRIPSPPAC_fQX1Raw, &b_BigRIPSPPAC_fQX1Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fQX2Raw", BigRIPSPPAC_fQX2Raw, &b_BigRIPSPPAC_fQX2Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fQY1Raw", BigRIPSPPAC_fQY1Raw, &b_BigRIPSPPAC_fQY1Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fQY2Raw", BigRIPSPPAC_fQY2Raw, &b_BigRIPSPPAC_fQY2Raw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fQARaw", BigRIPSPPAC_fQARaw, &b_BigRIPSPPAC_fQARaw);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTX1", BigRIPSPPAC_fTX1, &b_BigRIPSPPAC_fTX1);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTX2", BigRIPSPPAC_fTX2, &b_BigRIPSPPAC_fTX2);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTY1", BigRIPSPPAC_fTY1, &b_BigRIPSPPAC_fTY1);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTY2", BigRIPSPPAC_fTY2, &b_BigRIPSPPAC_fTY2);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTA", BigRIPSPPAC_fTA, &b_BigRIPSPPAC_fTA);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTSumX", BigRIPSPPAC_fTSumX, &b_BigRIPSPPAC_fTSumX);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTSumY", BigRIPSPPAC_fTSumY, &b_BigRIPSPPAC_fTSumY);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTDiffX", BigRIPSPPAC_fTDiffX, &b_BigRIPSPPAC_fTDiffX);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fTDiffY", BigRIPSPPAC_fTDiffY, &b_BigRIPSPPAC_fTDiffY);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fX", BigRIPSPPAC_fX, &b_BigRIPSPPAC_fX);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fY", BigRIPSPPAC_fY, &b_BigRIPSPPAC_fY);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fFiredX", BigRIPSPPAC_fFiredX, &b_BigRIPSPPAC_fFiredX);
   fChainCut->SetBranchAddress("BigRIPSPPAC.fFiredY", BigRIPSPPAC_fFiredY, &b_BigRIPSPPAC_fFiredY);

   fChainCut->SetBranchAddress("fitdata", &fitdata_, &b_fitdata_);
   fChainCut->SetBranchAddress("fitdata.fUniqueID", fitdata_fUniqueID, &b_fitdata_fUniqueID);
   fChainCut->SetBranchAddress("fitdata.fBits", fitdata_fBits, &b_fitdata_fBits);
   fChainCut->SetBranchAddress("fitdata.x_mm", fitdata_x_mm, &b_fitdata_x_mm);
   fChainCut->SetBranchAddress("fitdata.y_mm", fitdata_y_mm, &b_fitdata_y_mm);
   fChainCut->SetBranchAddress("fitdata.t_ns", fitdata_t_ns, &b_fitdata_t_ns);
   fChainCut->SetBranchAddress("fitdata.z_mm", fitdata_z_mm, &b_fitdata_z_mm);
   fChainCut->SetBranchAddress("fitdata.Chargemax", fitdata_Chargemax, &b_fitdata_Chargemax);
   fChainCut->SetBranchAddress("fitdata.n_Cluster", fitdata_n_Cluster, &b_fitdata_n_Cluster);
   fChainCut->SetBranchAddress("fitdata.n_Pads", fitdata_n_Pads, &b_fitdata_n_Pads);
   fChainCut->SetBranchAddress("fitdata.Chi2", fitdata_Chi2, &b_fitdata_Chi2);
   fChainCut->SetBranchAddress("trackNbr_FINAL", &trackNbr_FINAL, &b_trackNbr_FINAL);
   fChainCut->SetBranchAddress("data_result", &data_result_, &b_data_result_);
   fChainCut->SetBranchAddress("data_result.fUniqueID", data_result_fUniqueID, &b_data_result_fUniqueID);
   fChainCut->SetBranchAddress("data_result.fBits", data_result_fBits, &b_data_result_fBits);
   fChainCut->SetBranchAddress("data_result.x_mm", data_result_x_mm, &b_data_result_x_mm);
   fChainCut->SetBranchAddress("data_result.y_mm", data_result_y_mm, &b_data_result_y_mm);
   fChainCut->SetBranchAddress("data_result.z_mm", data_result_z_mm, &b_data_result_z_mm);
   fChainCut->SetBranchAddress("data_result.Chargemax", data_result_Chargemax, &b_data_result_Chargemax);
   fChainCut->SetBranchAddress("data_result.n_Cluster", data_result_n_Cluster, &b_data_result_n_Cluster);
   fChainCut->SetBranchAddress("data_result.n_Pads", data_result_n_Pads, &b_data_result_n_Pads);
   fChainCut->SetBranchAddress("data_result.z_max", data_result_z_max, &b_data_result_z_max);
   fChainCut->SetBranchAddress("evtOrig", &evtOrig, &b_evtOrig);
   fChainCut->SetBranchAddress("VDrift", &VDrift, &b_VDrift);
   fChainCut->SetBranchAddress("DelayTrig", &DelayTrig, &b_DelayTrig);
   fChainCut->SetBranchAddress("x_vertex", &x_vertex, &b_x_vertex);
   fChainCut->SetBranchAddress("y_vertex", &y_vertex, &b_y_vertex);
   fChainCut->SetBranchAddress("z_vertex", &z_vertex, &b_Zv);
   fChainCut->SetBranchAddress("x_vertexPPAC", &x_vertexPPAC, &b_x_vertexPPAC);
   fChainCut->SetBranchAddress("y_vertexPPAC", &y_vertexPPAC, &b_y_vertexPPAC);
   fChainCut->SetBranchAddress("z_vertexPPAC", &z_vertexPPAC, &b_z_vertexPPAC);
   fChainCut->SetBranchAddress("x_vertexDSSSD", &x_vertexDSSSD, &b_x_vertexDSSSD);
   fChainCut->SetBranchAddress("y_vertexDSSSD", &y_vertexDSSSD, &b_y_vertexDSSSD);
   fChainCut->SetBranchAddress("z_vertexDSSSD", &z_vertexDSSSD, &b_z_vertexDSSSD);
   fChainCut->SetBranchAddress("r_vertex", &r_vertex, &b_r_vertex);
   fChainCut->SetBranchAddress("phi_vertex", &phi_vertex, &b_phi_vertex);
   fChainCut->SetBranchAddress("thetaz1", &thetaz1, &b_thetaz1);
   fChainCut->SetBranchAddress("thetaz2", &thetaz2, &b_thetaz2);
   fChainCut->SetBranchAddress("parFit_1", parFit_1, &b_parFit_1);
   fChainCut->SetBranchAddress("parFit_2", parFit_2, &b_parFit_2);
   //   fChainCut->SetBranchAddress("beta", &beta, &b_beta);
   //   fChainCut->SetBranchAddress("beta1", &beta1, &b_beta1);
   //   fChainCut->SetBranchAddress("beta2", &beta2, &b_beta2);
   Notify();
}

Bool_t EventCut::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

