#include "ShowDALIOnline.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtPPAC.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"

#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
//#include "TClonesArray.h"

#include "segidlist.hh"

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt(int) {
  printf("keyboard interrupt\n");
  stoploop = true;
}

ShowDALIOnline::ShowDALIOnline() : TArtAnaLoop(), bigripsparameters(0), daliparameters(0){
}

ShowDALIOnline::~ShowDALIOnline() {
}

void ShowDALIOnline::Construct() {
  cout<<"call const!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");

  //For Online
  cDALI = new TCanvas("DALI2","ADC all",1400,900);
  cDALI->Divide(2,3);
  h_dali_Araw     = new TH1I("Araw","Araw sum",4096,0,4096); 
  h_dali_energy   = new TH1F("Energy","Energy sum",512,0,4096); 
  h_dali_Araw_id  = new TH2I("Araw_id","Araw vs ID",186,0.5,186.5,512,0,4096); 
  h_dali_energy_id= new TH2F("Energy_id","energy vs ID",186,0.5,186.5,512,0,4096); 
  h_dali_time_id  = new TH2F("Time_id","time vs ID",186,0.5,186.5,512,-2048,2048); 

  hogehoge = 0;

  daliparameters = TArtDALIParameters::Instance();
  daliparameters->LoadParameter("db/DALI.xml");
  dalicalib= new TArtCalibDALI();
  //dalicalib->LoadData();

  //Need to load the plastic if we want to remove the F8 plastic timing:
  bigripsparameters  = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter("db/BigRIPSPlastic.xml");

  brcalib     = new TArtCalibPID();

  plasticcalib = brcalib->GetCalibPlastic();

  // define data nodes which are supposed to be dumped to tree 
  TClonesArray *info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
  //std::cout<<info_array->GetName()<<std::endl;
  //tree->Branch(info_array->GetName(),&info_array);

  TClonesArray *dali_array = (TClonesArray *)sman->FindDataContainer("DALINaI");
  std::cout<<"  Create DALI Branch || name : "<<dali_array->GetName()<<std::endl;
  //tree->Branch(dali_array->GetName(),&dali_array);

   neve=0;
}

void ShowDALIOnline::Calculate()  {
  //cout<<"call Calculate()"<<endl;
  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  int id = 0;
  int eventnum = rawevent->GetEventNumber();
 
  for(int i=0;i<rawevent->GetNumScaler();i++){
    
    TArtRawScalerObject *sca = rawevent->GetScaler(i);
    
    int scaID   = sca->GetScalerID();
    int scaDate = sca->GetScalerDate();
    int scaNum  = sca->GetNumChannel();
    
    if(GGDAQ01SCALER==scaID){
      scaData[0]  = sca->GetScaler(0);
      scaData[1]  = sca->GetScaler(1);
      scaData[2]  = sca->GetScaler(2);
      scaData[3]  = sca->GetScaler(3);
    }
  }
  
  brcalib->ReconstructData();
  dalicalib->SetPlTime(plasticcalib->FindPlastic("F8pl")->GetTime());
  //Add this to remove the reference time from the plastic
  dalicalib->ReconstructData();

  Int_t NumDALI = dalicalib->GetNumNaI();

  for(Int_t j=0;j<NumDALI;j++){
    TArtDALINaI *nai = (TArtDALINaI*)dalicalib->GetNaI(j);
    TArtDALINaIPara *naip = (TArtDALINaIPara*)dalicalib->GetNaIPara(j);
  
    if(nai->GetID()!=187&&nai->GetID()!=188){
      h_dali_Araw->Fill((int)nai->GetRawADC());
      h_dali_Araw_id->Fill((int)nai->GetID(),(int)nai->GetRawADC());
      if(dalicalib->GetMult()>=1){
        h_dali_energy->Fill(nai->GetEnergy());
        h_dali_energy_id->Fill((int)nai->GetID(),nai->GetEnergy());
        h_dali_time_id->Fill((int)nai->GetID(),nai->GetTimeOffseted());
      }
    }
  }
  
  tree->Fill();
  neve ++;
  
  //For monitoring
  if(neve%200==0){

    allevents[hogehoge]=neve;
    fallevents[hogehoge]=neve;
    multi0[hogehoge]=NumDALI;
    multi1[hogehoge]=dalicalib->GetMult();
    ratio_DAQ[hogehoge]=1.*scaData[0]/scaData[1]*100;
    
    gmulti0=new TGraph(neve/200,allevents,multi0);
    gmulti1=new TGraph(neve/200,allevents,multi1);
    gratio =new TGraph(neve/200,fallevents,ratio_DAQ);
    
    hogehoge++;

 
    cDALI->cd(1);
    h_dali_Araw->Draw();
    cDALI->cd(2);
    h_dali_energy->Draw();
    cDALI->cd(3);
    h_dali_Araw_id->Draw("colz");
    cDALI->cd(4);
    h_dali_energy_id->Draw("colz");
    cDALI->cd(5);
    h_dali_time_id->Draw("colz");
    
    cDALI->cd(6);
    gratio->Draw("AL*");
    gratio->GetYaxis()->SetRangeUser(0,100);
    gratio->SetTitle("Ratio of Gated/Ungated");
    
    cDALI->Update();
  }
  
  //********************* Clear *****************
  dalicalib->ClearData();
  rawevent->Clear();
}

void ShowDALIOnline::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowDALIOnline::ClassName() const  {
  return "ShowDALIOnline";
}
