#include "ShowZDPPACEfficiency.hh"

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

ShowZDPPACEfficiency::ShowZDPPACEfficiency() : TArtAnaLoop(), bigripsparameters(0){
}

ShowZDPPACEfficiency::~ShowZDPPACEfficiency() {
}

void ShowZDPPACEfficiency::Construct() {
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");

  //For Online
  cAOQ = new TCanvas("AOQ","AOQ in ZD",1400,900);
  cAOQ->Divide(2,1);
  
  cZDPPAC = new TCanvas("PPAC","PPAC",1400,900);
  cZDPPAC->Divide(3,3);

  h_zd_zed     = new TH1F("Zed","Zed",1000,0,100); 

  h_zd_aoq_zed = new TH2F("A/Q Zed","A/Q Zed",200,2.0,2.5,200,20,60); 
  
  h_zd_sum_x = new TH2F("PPAXSumx","PPAXSumx",14,0,14,1000,-1000,1000); 
  h_zd_sum_y = new TH2F("PPAXSumy","PPAXSumy",14,0,14,1000,-1000,1000); 

  h_zd_zed_dummy  = new TH2F("ZedDummy","ZedDummy",14,0,14,100,40,60);
  h_zd_zed_ppac_x = new TH2F("ZedGateX","ZedGateX",14,0,14,100,40,60); 
  h_zd_zed_ppac_y = new TH2F("ZedGatey","ZedGateY",14,0,14,100,40,60); 

  h_zd_ppac_eff_x = new TH2F("PPACEffX","PPACEffX",14,0,14,100,40,60); 
  h_zd_ppac_eff_y = new TH2F("PPACEffY","PPACEffY",14,0,14,100,40,60); 

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

void ShowZDPPACEfficiency::Calculate()  {
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
 
  h_zd_zed->Fill(beam_zd_911->GetZet());
  h_zd_aoq_zed->Fill(beam_zd_911->GetAoQ(),beam_zd_89->GetZet());

  TArtPPAC *ppac;

  char *name_ppac[14]={"F8PPAC-1A","F8PPAC-1B","F8PPAC-2A","F8PPAC-2B","F8PPAC-3A","F8PPAC-3B",
                       "F9PPAC-1A","F9PPAC-1B","F9PPAC-2A","F9PPAC-2B",
                       "F11PPAC-1A","F11PPAC-1B","F11PPAC-2A","F11PPAC-2B"};
  
  for(int i =0;i<14;i++){
    ppac = ppaccalib->FindPPAC((char*)name_ppac[i]);
  
    if(ppac){ 
      h_zd_sum_x->Fill(i,ppac->GetTSumX());
      h_zd_sum_y->Fill(i,ppac->GetTSumY());
      if(ppac->GetX() > -120 && ppac->GetX() < 120){
        h_zd_zed_ppac_x->Fill(i,beam_zd_911->GetZet());
      }
      if(ppac->GetY() > -120 && ppac->GetY() < 120){
        h_zd_zed_ppac_y->Fill(i,beam_zd_911->GetZet());
      }    
    }
  }
  for(int i =0;i<14;i++){
    h_zd_zed_dummy->Fill(i,beam_zd_911->GetZet());
  }

  //For monitoring
  if(neve%200==0){

    cAOQ->cd(1);
    h_zd_zed->Draw();
    cAOQ->cd(2);
    h_zd_aoq_zed->Draw("colz");
    cAOQ->Update();

    cZDPPAC->cd(1);
    h_zd_sum_x->Draw("colz");
    cZDPPAC->cd(2);
    h_zd_sum_y->Draw("colz");
    cZDPPAC->cd(3);
    h_zd_zed_dummy->Draw("colz");
    cZDPPAC->cd(4);
    h_zd_zed_ppac_x->Draw("colz");
    cZDPPAC->cd(5);
    h_zd_zed_ppac_y->Draw("colz");

    cZDPPAC->cd(6);
    h_zd_ppac_eff_x->Divide(h_zd_zed_ppac_x,h_zd_zed_dummy);
    h_zd_ppac_eff_x->Draw("colz");

    cZDPPAC->cd(7);
    h_zd_ppac_eff_y->Divide(h_zd_zed_ppac_y,h_zd_zed_dummy);
    h_zd_ppac_eff_y->Draw("colz");

    cZDPPAC->Update();
  }
  
  //********************* Clear *****************
  brcalib->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void ShowZDPPACEfficiency::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowZDPPACEfficiency::ClassName() const  {
  return "ShowZDPPACEfficiency";
}
