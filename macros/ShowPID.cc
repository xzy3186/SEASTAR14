#include "ShowPID.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtPPAC.hh"
#include "TArtTOF.hh"
#include "TArtCore.hh"
#include "TArtStoreManager.hh"

#include "TArtDALINaI.hh"
#include "TArtDALINaIPara.hh"

#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"

#include "TArtRecoBeam.hh"
#include "TArtBeam.hh"
//#include "TClonesArray.h"

#include "segidlist.hh"

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt(int) {
  printf("keyboard interrupt\n");
  stoploop = true;
}

ShowPID::ShowPID() : TArtAnaLoop(), bigripsparameters(0){
}

ShowPID::~ShowPID() {
}

void ShowPID::Construct() {
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");

  //For Online
  cPID = new TCanvas("PID","PID",1400,900);
  cPID->Divide(3,4);
  
  h_br_zed     = new TH1F("BRZed","BRZed",1000,0,100); 
  h_zd_zed     = new TH1F("ZDZed","ZDZed",1000,0,100); 

  h_br_tof     = new TH1F("BRTOF","BRTOF",2000,0,500); 
  h_zd_tof     = new TH1F("ZDTOF","ZDTOF",2000,0,500); 

  h_br_beta     = new TH1F("BRBeta","BRBeta",500,0.3,0.7); 
  h_zd_beta     = new TH1F("ZDBeta","ZDBeta",500,0.3,0.7); 
  
  h_br_tof_zed = new TH2F("BRTOFZed","BRTOFZed",1000,0,500,200,20,60); 
  h_zd_tof_zed = new TH2F("ZDTOFZed","ZDTOFZed",1000,0,500,200,20,60); 
  
  h_br_aoq_zed = new TH2F("BR A/Q Zed","BR A/Q Zed",200,2.0,2.5,200,20,60); 
  h_zd_aoq_zed = new TH2F("ZD A/Q Zed","ZD A/Q Zed",200,2.0,2.5,200,20,60); 
  
  h_zd_sum_x = new TH2F("PPAXSumx","PPAXSumx",26,0,26,1000,-1000,1000); 
  h_zd_sum_y = new TH2F("PPAXSumy","PPAXSumy",26,0,26,1000,-1000,1000); 

  bigripsparameters  = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter("db/BigRIPSPPAC.xml");
  bigripsparameters->LoadParameter("db/BigRIPSPlastic.xml");
  bigripsparameters->LoadParameter("db/BigRIPSIC.xml");
  bigripsparameters->LoadParameter("db/FocalPlane.xml");
  bigripsparameters->SetFocusPosOffset(8,138.5);

  brcalib     = new TArtCalibPID();
  ppaccalib  = brcalib->GetCalibPPAC();
  plasticcalib = brcalib->GetCalibPlastic();

  std::cout << "Defining bigrips parameters" << std::endl;
  recopid = new TArtRecoPID();
  rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat",5.45); // F3 - F5
  rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat",4.665); // F5 - F7
  rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat",3.831); // F8 - F9
  rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat",3.805); // F9 - F11  

  // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
  tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",302,5); // F3 - F7
  tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-749.2,9); // F8 - F11
  // Reconstruction of IC observables for ID
  
  //beam_br_37 = recopid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
  beam_br_35 = recopid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
  beam_br_57 = recopid->DefineNewBeam(rips5to7,tof3to7,"F7IC");   
  beam_zd_89 = recopid->DefineNewBeam(rips8to9,tof8to11,"F11IC");
  beam_zd_911 = recopid->DefineNewBeam(rips9to11,tof8to11,"F11IC");
 
  // define data nodes which are supposed to be dumped to tree 
  TClonesArray *info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
  //std::cout<<info_array->GetName()<<std::endl;
  //tree->Branch(info_array->GetName(),&info_array);
 
  TClonesArray *beam_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");	
  std::cout<<beam_array->GetName()<<std::endl;     
  tree->Branch(beam_array->GetName(),&beam_array); 
  
  /*
  TClonesArray *tof_array  = 
  (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
  std::cout<<tof_array->GetName()<<std::endl;     
  tree->Branch(tof_array->GetName(),&tof_array);  

  TClonesArray * ic_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  tree->Branch(ic_array->GetName(),&ic_array);

  TClonesArray * pla_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  tree->Branch(pla_array->GetName(),&pla_array);
  */
   neve=0; 
}

void ShowPID::Calculate()  {
  //cout<<"call Calculate()"<<endl;
  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  int id = 0;
  int eventnum = rawevent->GetEventNumber();
   
  brcalib->ReconstructData();
  recopid->ReconstructData();
    
  tree->Fill();
  neve ++;

  //if(neve%1000==0)
  //  std::cout << "event: " << neve << std::endl;
 
  h_br_zed->Fill(beam_br_35->GetZet());
  h_zd_zed->Fill(beam_zd_911->GetZet());
 
  h_br_tof->Fill(tof3to7->GetTOF());
  h_zd_tof->Fill(tof8to11->GetTOF());

  h_br_beta->Fill(tof3to7->GetBeta());
  h_zd_beta->Fill(tof8to11->GetBeta());
  
  h_br_tof_zed->Fill(tof3to7->GetTOF(),beam_br_35->GetZet());
  h_zd_tof_zed->Fill(tof8to11->GetTOF(),beam_zd_911->GetZet());

  h_br_aoq_zed->Fill(beam_br_35->GetAoQ(),beam_br_35->GetZet());
  h_zd_aoq_zed->Fill(beam_zd_911->GetAoQ(),beam_zd_911->GetZet());

  TArtPPAC *ppac;

  char *name_ppac[26]={"F3PPAC-1A","F3PPAC-1B","F3PPAC-2A","F3PPAC-2B",
                       "F5PPAC-1A","F5PPAC-1B","F5PPAC-2A","F5PPAC-2B",
                       "F7PPAC-1A","F7PPAC-1B","F7PPAC-2A","F7PPAC-2B",
                       "F8PPAC-1A","F8PPAC-1B","F8PPAC-2A","F8PPAC-2B","F8PPAC-3A","F8PPAC-3B",
                       "F9PPAC-1A","F9PPAC-1B","F9PPAC-2A","F9PPAC-2B",
                       "F11PPAC-1A","F11PPAC-1B","F11PPAC-2A","F11PPAC-2B"};
  
  for(int i =0;i<26;i++){
    ppac = ppaccalib->FindPPAC((char*)name_ppac[i]);
  
    if(ppac){ 
      h_zd_sum_x->Fill(i,ppac->GetTSumX());
      h_zd_sum_y->Fill(i,ppac->GetTSumY());
    }
  }

  //For monitoring
  if(neve%200==0){

    cPID->cd(1);
    h_br_zed->Draw();
    
    cPID->cd(2);
    h_zd_zed->Draw();
    
    cPID->cd(3);
    h_br_tof->Draw();

    cPID->cd(4);
    h_zd_tof->Draw();

    cPID->cd(5);
    h_br_beta->Draw();

    cPID->cd(6);
    h_zd_beta->Draw();
    
    cPID->cd(7);
    h_br_tof_zed->Draw("colz");

    cPID->cd(8);
    h_zd_tof_zed->Draw("colz");
    
    cPID->cd(9);
    h_br_aoq_zed->Draw("colz");

    cPID->cd(10);
    h_zd_aoq_zed->Draw("colz");

    cPID->cd(11);
    h_zd_sum_x->Draw("colz");

    cPID->cd(12);
    h_zd_sum_y->Draw("colz");
   
    cPID->Update();
  }
  
  //********************* Clear *****************
  brcalib->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void ShowPID::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowPID::ClassName() const  {
  return "ShowPID";
}
