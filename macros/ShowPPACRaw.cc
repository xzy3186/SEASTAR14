#include "ShowPPACRaw.hh"

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

ShowPPACRaw::ShowPPACRaw() : TArtAnaLoop(), bigripsparameters(0){
}

ShowPPACRaw::~ShowPPACRaw() {
}

void ShowPPACRaw::Construct() {
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");
  
  cPPAC = new TCanvas("PPAC","PPAC",1400,900);
  cPPAC->Divide(3,3);

  h_x1 = new TH2F("PPAC TX1","PPAC TX1",26,0,26,4000,0,25000); 
  h_x2 = new TH2F("PPAC TX2","PPAC TX2",26,0,26,4000,0,25000); 

  h_y1 = new TH2F("PPAC TY1","PPAC TY1",26,0,26,4000,0,25000); 
  h_y2 = new TH2F("PPAC TY2","PPAC TY2",26,0,26,4000,0,25000); 

  h_a = new TH2F("PPAC A","PPAC A",26,0,26,4000,0,25000) ;

  h_diff_x = new TH2F("PPAC Diff X","PPAC Diff X",26,0,26,1000,-4000,4000); 
  h_diff_y = new TH2F("PPAC Diff Y","PPAC Diff Y",26,0,26,1000,-4000,4000); 

  h_sum_x = new TH2F("PPAXSumx","PPAXSumx",26,0,26,1000,-1000,1000); 
  h_sum_y = new TH2F("PPAXSumy","PPAXSumy",26,0,26,1000,-1000,1000); 
  
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

void ShowPPACRaw::Calculate()  {
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
 
  TArtPPAC *ppac;

  char *name_ppac[26]={
    "F3PPAC-1A","F3PPAC-1B","F3PPAC-2A","F3PPAC-2B",
    "F5PPAC-1A","F5PPAC-1B","F5PPAC-2A","F5PPAC-2B",
    "F7PPAC-1A","F7PPAC-1B","F7PPAC-2A","F7PPAC-2B",
    "F8PPAC-1A","F8PPAC-1B","F8PPAC-2A","F8PPAC-2B","F8PPAC-3A","F8PPAC-3B",
    "F9PPAC-1A","F9PPAC-1B","F9PPAC-2A","F9PPAC-2B",
    "F11PPAC-1A","F11PPAC-1B","F11PPAC-2A","F11PPAC-2B"};
  
  for(int i =0;i<26;i++){
    ppac = ppaccalib->FindPPAC((char*)name_ppac[i]);
  
    if(ppac){ 
      h_x1->Fill(i,ppac->GetTX1Raw());
      h_x2->Fill(i,ppac->GetTX2Raw());

      h_y1->Fill(i,ppac->GetTY1Raw());
      h_y2->Fill(i,ppac->GetTY2Raw());

      h_a->Fill(i,ppac->GetTARaw());

      h_diff_x->Fill(i,ppac->GetTDiffX());
      h_diff_y->Fill(i,ppac->GetTDiffY());

      h_sum_x->Fill(i,ppac->GetTSumX());
      h_sum_y->Fill(i,ppac->GetTSumY());
      
    }
  }

  //For monitoring
  if(neve%200==0){
    cPPAC->cd(1);
    h_x1->Draw("colz");
    cPPAC->cd(2);
    h_x2->Draw("colz");

    cPPAC->cd(3);
    h_y1->Draw("colz");
    cPPAC->cd(4);
    h_y2->Draw("colz");
    
    cPPAC->cd(5);
    h_a->Draw("colz");

    cPPAC->cd(6);
    h_diff_x->Draw("colz");
    cPPAC->cd(7);
    h_diff_y->Draw("colz");

    cPPAC->cd(8);
    h_sum_x->Draw("colz");
    cPPAC->cd(9);
    h_sum_y->Draw("colz");
    
    cPPAC->Update();
  }
  
  //********************* Clear *****************
  brcalib->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void ShowPPACRaw::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowPPACRaw::ClassName() const  {
  return "ShowPPACRaw";
}
