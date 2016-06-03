#include "ShowBRPPACEfficiency.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtPPAC.hh"
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

ShowBRPPACEfficiency::ShowBRPPACEfficiency() : TArtAnaLoop(), bigripsparameters(0){
}

ShowBRPPACEfficiency::~ShowBRPPACEfficiency() {
}

void ShowBRPPACEfficiency::Construct() {
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");

  //For Online
  cAOQ = new TCanvas("AOQ","AOQ in BR",1400,900);
  cAOQ->Divide(2,1);
  
  cBRPPAC = new TCanvas("PPAC","PPAC",1400,900);
  cBRPPAC->Divide(3,3);

  h_br_zed     = new TH1F("Zed","Zed",1000,0,100); 

  h_br_aoq_zed = new TH2F("A/Q Zed","A/Q Zed",200,2.0,2.5,200,40,60); 
  
  h_br_sum_x = new TH2F("PPAXSumx","PPAXSumx",12,0,12,1000,-1000,1000); 
  h_br_sum_y = new TH2F("PPAXSumy","PPAXSumy",12,0,12,1000,-1000,1000); 

  h_br_zed_dummy  = new TH2F("ZedDummy","ZedDummy",12,0,12,100,40,60);
  h_br_zed_ppac_x = new TH2F("ZedGateX","ZedGateX",12,0,12,100,40,60); 
  h_br_zed_ppac_y = new TH2F("ZedGatey","ZedGateY",12,0,12,100,40,60); 

  h_br_ppac_eff_x = new TH2F("PPACEffX","PPACEffX",12,0,12,100,40,60); 
  h_br_ppac_eff_y = new TH2F("PPACEffY","PPACEffY",12,0,12,100,40,60); 

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

  TClonesArray * ppac_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
  std::cout<<ppac_array->GetName()<<std::endl;
  tree->Branch(ppac_array->GetName(),&ppac_array);

   neve=0; 
}

void ShowBRPPACEfficiency::Calculate()  {
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
  h_br_aoq_zed->Fill(beam_br_35->GetAoQ(),beam_br_35->GetZet());

  TArtPPAC *ppac;

  char *name_ppac[12]={"F3PPAC-1A","F3PPAC-1B","F3PPAC-2A","F3PPAC-2B",
                       "F5PPAC-1A","F5PPAC-1B","F5PPAC-2A","F5PPAC-2B",
                       "F7PPAC-1A","F7PPAC-1B","F7PPAC-2A","F7PPAC-2B"};
  
  for(int i =0;i<12;i++){
    ppac = ppaccalib->FindPPAC((char*)name_ppac[i]);
  
    if(ppac){ 
      h_br_sum_x->Fill(i,ppac->GetTSumX());
      h_br_sum_y->Fill(i,ppac->GetTSumY());
      if(ppac->GetX() > -120 && ppac->GetX() < 120){
        h_br_zed_ppac_x->Fill(i,beam_br_35->GetZet());
      }
      if(ppac->GetY() > -120 && ppac->GetY() < 120){
        h_br_zed_ppac_y->Fill(i,beam_br_35->GetZet());
      }    
    }
  }
  for(int i =0;i<12;i++){
    h_br_zed_dummy->Fill(i,beam_br_35->GetZet());
  }

  //For monitoring
  if(neve%200==0){

    cAOQ->cd(1);
    h_br_zed->Draw();
    cAOQ->cd(2);
    h_br_aoq_zed->Draw("colz");
    cAOQ->Update();

    cBRPPAC->cd(1);
    h_br_sum_x->Draw("colz");
    cBRPPAC->cd(2);
    h_br_sum_y->Draw("colz");
    cBRPPAC->cd(3);
    h_br_zed_dummy->Draw("colz");
    cBRPPAC->cd(4);
    h_br_zed_ppac_x->Draw("colz");
    cBRPPAC->cd(5);
    h_br_zed_ppac_y->Draw("colz");

    cBRPPAC->cd(6);
    h_br_ppac_eff_x->Divide(h_br_zed_ppac_x,h_br_zed_dummy);
    h_br_ppac_eff_x->Draw("colz");

    cBRPPAC->cd(7);
    h_br_ppac_eff_y->Divide(h_br_zed_ppac_y,h_br_zed_dummy);
    h_br_ppac_eff_y->Draw("colz");

    cBRPPAC->Update();
  }
  
  //********************* Clear *****************
  brcalib->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void ShowBRPPACEfficiency::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowBRPPACEfficiency::ClassName() const  {
  return "ShowBRPPACEfficiency";
}
