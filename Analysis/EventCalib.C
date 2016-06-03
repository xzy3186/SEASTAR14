#include "EventCalib.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using namespace std;

EventCalib::EventCalib() {
  fChain = new TChain();

// ################
// All events : June 10th
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140201_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140202_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140203_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140204_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140205_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140206_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140207_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140208_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140209_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140210_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140211_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140212_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140213_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140214_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140215_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140216_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140217_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140218_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140219_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140220_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140221_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140222_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140223_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140224_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140225_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140226_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140227_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140228_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140229_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140230_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140231_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140232_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140233_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140234_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140235_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140236_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140237_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140238_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140239_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140240_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140241_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140242_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140243_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140244_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140245_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140246_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140247_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140248_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140249_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140250_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140251_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140252_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140253_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140254_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140255_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140256_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140257_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140258_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140259_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140260_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140261_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140262_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140263_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140264_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140265_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140266_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140267_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140268_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140269_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140270_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140271_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140272_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140273_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140274_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140275_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140276_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140277_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140278_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140279_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140280_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140281_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140282_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140283_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140284_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140285_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140286_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140287_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140288_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140289_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140290_VDrift.root",0,"tree");
  fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140291_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140292_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140293_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140294_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140295_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140296_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140297_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140298_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140299_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140300_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140301_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140302_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140303_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140304_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140305_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140306_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140307_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140308_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140309_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140310_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140311_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140312_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140313_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140314_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140315_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140316_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140317_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140318_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140319_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140320_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140321_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140322_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140323_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140324_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140325_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140326_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140327_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140328_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140329_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140330_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140332_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140333_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140334_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140335_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140336_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140337_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140338_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140339_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140340_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140341_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140342_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140343_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140344_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140345_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140346_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140347_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140348_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140349_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140350_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140351_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140352_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140353_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140354_VDrift.root",0,"tree");
  //fChain->AddFile("../rootfiles/seastar-2014/78Ni/psp140355_VDrift.root",0,"tree");

  Init();
}

EventCalib::~EventCalib(){
}

Int_t EventCalib::GetEntry(Long64_t entry){
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

void EventCalib::Init() {
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
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fitdata", &fitdata_, &b_fitdata_);
   fChain->SetBranchAddress("fitdata.fUniqueID", fitdata_fUniqueID, &b_fitdata_fUniqueID);
   fChain->SetBranchAddress("fitdata.fBits", fitdata_fBits, &b_fitdata_fBits);
   fChain->SetBranchAddress("fitdata.x_mm", fitdata_x_mm, &b_fitdata_x_mm);
   fChain->SetBranchAddress("fitdata.y_mm", fitdata_y_mm, &b_fitdata_y_mm);
   fChain->SetBranchAddress("fitdata.t_ns", fitdata_t_ns, &b_fitdata_t_ns);
   fChain->SetBranchAddress("fitdata.z_mm", fitdata_z_mm, &b_fitdata_z_mm);
   fChain->SetBranchAddress("fitdata.Chargemax", fitdata_Chargemax, &b_fitdata_Chargemax);
   fChain->SetBranchAddress("fitdata.n_Cluster", fitdata_n_Cluster, &b_fitdata_n_Cluster);
   fChain->SetBranchAddress("fitdata.n_Pads", fitdata_n_Pads, &b_fitdata_n_Pads);
   fChain->SetBranchAddress("fitdata.Chi2", fitdata_Chi2, &b_fitdata_Chi2);
   fChain->SetBranchAddress("Rpadnumber_max", &Rpadnumber_max, &b_Rpadnumber_max);
   fChain->SetBranchAddress("EventInfo", &EventInfo_, &b_EventInfo_);
   fChain->SetBranchAddress("EventInfo.fUniqueID", EventInfo_fUniqueID, &b_EventInfo_fUniqueID);
   fChain->SetBranchAddress("EventInfo.fBits", EventInfo_fBits, &b_EventInfo_fBits);
   fChain->SetBranchAddress("EventInfo.fName", EventInfo_fName, &b_EventInfo_fName);
   fChain->SetBranchAddress("EventInfo.fTitle", EventInfo_fTitle, &b_EventInfo_fTitle);
   fChain->SetBranchAddress("EventInfo.runnumber", EventInfo_runnumber, &b_EventInfo_runnumber);
   fChain->SetBranchAddress("EventInfo.eventnumber", EventInfo_eventnumber, &b_EventInfo_eventnumber);
   fChain->SetBranchAddress("EventInfo.subsysname", EventInfo_subsysname, &b_EventInfo_subsysname);
   fChain->SetBranchAddress("EventInfo.timestamp", EventInfo_timestamp, &b_EventInfo_timestamp);
   fChain->SetBranchAddress("EventInfo.comp_val", EventInfo_comp_val, &b_EventInfo_comp_val);
   fChain->SetBranchAddress("EventInfo.fBit", EventInfo_fBit, &b_EventInfo_fBit);
   Notify();
}

Bool_t EventCalib::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

