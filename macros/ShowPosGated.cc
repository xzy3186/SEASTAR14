#include "ShowPosGated.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtFocalPlane.hh"
#include "TArtIC.hh"
#include "TArtPlastic.hh"
#include "TArtPPAC.hh"
#include "TArtTOF.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"

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

ShowPosGated::ShowPosGated() : TArtAnaLoop(), bigripsparameters(0){
  char name[100]; 

  for(int i=0;i<3;i++) {
    sprintf(name,"./cut/brcut%i.root",i);
    BRcut[i] = new TFile(name,"READ");
  }
  
  for(int i=0;i<3;i++) {
    sprintf(name,"./cut/zdcut%i.root",i);
    ZDcut[i] = new TFile(name,"READ");
  }

  for(int i=0;i<3;i++) {
    sprintf(name,"brcut%i",i);
    BRcut[i]->GetObject(name,brcut[i]);
  }
  
  for(int i=0;i<3;i++) {
    sprintf(name,"zdcut%i",i);
    ZDcut[i]->GetObject(name,zdcut[i]); 
  }
}

ShowPosGated::~ShowPosGated() {
}

void ShowPosGated::Construct() {
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  TFile *fout = new TFile("PosGated.root","RECREATE");
  tree = new TTree("tree","tree");

  //For Online
  cPos = new TCanvas("Pos","Pos",1400,900);
  cPos->Divide(5,4);

  h_br_aoq35_zed = new TH2F("BR A/Q35 Zed","BR A/Q35 Zed",200,2.0,2.5,200,20,60); 
  h_br_aoq57_zed = new TH2F("ZD A/Q57 Zed","ZD A/Q57 Zed",200,2.0,2.5,200,20,60); 

  h_zd_aoq89_zed = new TH2F("BR A/Q89 Zed","BR A/Q89 Zed",200,2.0,2.5,200,20,60); 
  h_zd_aoq911_zed = new TH2F("ZD A/Q911 Zed","ZD A/Q911 Zed",200,2.0,2.5,200,20,60); 

  h_zd_aoq89_zed_gated = new TH2F("BR A/Q89 Zed BRgated","BR A/Q89 Zed BRgated",200,2.0,2.5,200,20,60); 
  h_zd_aoq911_zed_gated = new TH2F("ZD A/Q911 Zed BRgated","ZD A/Q911 Zed BRgated",200,2.0,2.5,200,20,60); 
  
  h_F3X_aoq35_gated = new TH2F("F3X aoq35 BRgated","F3X aoq35 BRgated",200,2.0,2.5,200,-50,50); 
  h_F5X_aoq35_gated = new TH2F("F5X aoq35 BRgated","F5X aoq35 BRgated",200,2.0,2.5,200,-150,150); 
  
  h_F8X_aoq911_gated = new TH2F("F8X aoq911 BRZDgated","F8X aoq911 BRZDgated",200,2.0,2.5,200,-50,50); 
  h_F9X_aoq911_gated = new TH2F("F9X aoq911 BRZDgated","F9X aoq911 BRZDgated",200,2.0,2.5,200,-150,150); 
  h_F11X_aoq911_gated = new TH2F("F11X aoq911 BRZDgated","F11X aoq911 BRZDgated",200,2.0,2.5,200,-50,50); 

  h_F5X_F9X_gated = new TH2F("F5X F9X BRZDgated","F5X F9X BR35ZDgated",200,-100,100,200,-100,100); 
 
  h_F3X_F3A_gated = new TH2F("F3X F3A BRZDgated","F3X F3A BR35ZD911gated",200,-100,100,200,-100,100); 
  h_F5X_F5A_gated = new TH2F("F5X F5A BRZDgated","F5X F5A BR35ZD911gated",200,-100,100,200,-100,100); 
  h_F7X_F7A_gated = new TH2F("F7X F7A BRZDgated","F7X F7A BR35ZD911gated",200,-100,100,200,-100,100); 
  h_F8X_F8A_gated = new TH2F("F8X F8A BRZDgated","F8X F8A BR35ZD911gated",200,-100,100,200,-100,100); 
  h_F9X_F9A_gated = new TH2F("F3X F9A BRZDgated","F9X F9A BR35ZD911gated",200,-100,100,200,-100,100); 
  h_F11X_F11A_gated = new TH2F("F3X F11A BRZDgated","F11X F11A BR35ZD911gated",200,-100,100,200,-100,100); 

  bigripsparameters  = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter("db/BigRIPSPPAC.xml");
  bigripsparameters->LoadParameter("db/BigRIPSPlastic.xml");
  bigripsparameters->LoadParameter("db/BigRIPSIC.xml");
  bigripsparameters->LoadParameter("db/FocalPlane.xml");
  bigripsparameters->SetFocusPosOffset(8,138.5);

  brcalib      = new TArtCalibPID();
  ppaccalib    = brcalib->GetCalibPPAC();
  plasticcalib = brcalib->GetCalibPlastic();
  focalcalib   = brcalib->GetCalibFocalPlane(); 

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
  
  //TClonesArray *tof_array  = 
  //(TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
  //std::cout<<tof_array->GetName()<<std::endl;     
  //tree->Branch(tof_array->GetName(),&tof_array);  

  //TClonesArray * ic_array = 
  //  (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  //tree->Branch(ic_array->GetName(),&ic_array);

  TClonesArray * fpl_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSFocalPlane");
  tree->Branch(fpl_array->GetName(),&fpl_array);

  //TClonesArray * pla_array = 
  //  (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  //tree->Branch(pla_array->GetName(),&pla_array);
  
   neve=0; 
}

void ShowPosGated::Calculate()  {
  //cout<<"call Calculate()"<<endl;
  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  int id = 0;
  int eventnum = rawevent->GetEventNumber();
   
  brcalib->ReconstructData();
  recopid->ReconstructData();
    
  tree->Fill();
  neve++;

  //if(neve%1000==0)
  //  std::cout << "event: " << neve << std::endl;
 
  h_br_aoq35_zed->Fill(beam_br_35->GetAoQ(),beam_br_35->GetZet());
  h_br_aoq57_zed->Fill(beam_br_57->GetAoQ(),beam_br_57->GetZet());

  h_zd_aoq89_zed->Fill(beam_zd_89->GetAoQ(),beam_zd_89->GetZet());
  h_zd_aoq911_zed->Fill(beam_zd_911->GetAoQ(),beam_zd_911->GetZet());
  
  if(brcut[0]->IsInside(beam_br_35->GetAoQ(),beam_br_35->GetZet())){
    h_zd_aoq89_zed_gated->Fill(beam_zd_89->GetAoQ(),beam_zd_89->GetZet());
    h_zd_aoq911_zed_gated->Fill(beam_zd_911->GetAoQ(),beam_zd_911->GetZet());
  }  

  TArtFocalPlane *fpl3;
  TArtFocalPlane *fpl5;
  
  TArtFocalPlane *fpl7;
  TArtFocalPlane *fpl8;
  TArtFocalPlane *fpl9;
  TArtFocalPlane *fpl11;

  TVectorD *fpl3v;
  TVectorD *fpl5v;
  TVectorD *fpl7v;
  
  TVectorD *fpl8v;
  TVectorD *fpl9v;
  TVectorD *fpl11v;

  float F3X = 0;
  float F5X = 0;
  float F7X = 0;

  float F8X = 0;
  float F9X = 0;
  float F11X = 0;

  float F3A = 0;
  float F5A = 0;
  float F7A = 0;

  float F8A = 0;
  float F9A = 0;
  float F11A = 0;



  fpl3  = focalcalib->FindFocalPlane(3);
  if(fpl3) {
    fpl3v = fpl3->GetOptVector();
    F3X = (*fpl3v)(0);
    F3A = (*fpl3v)(1);
  }

  fpl5  = focalcalib->FindFocalPlane(5);
  if(fpl5) {
    fpl5v = fpl5->GetOptVector();
    F5X = (*fpl5v)(0);
    F5A = (*fpl5v)(1);
  }
  
  fpl7  = focalcalib->FindFocalPlane(7);
  if(fpl7) {
    fpl7v = fpl7->GetOptVector();
    F7X = (*fpl7v)(0);
    F7A = (*fpl7v)(1);
  }

  fpl8  = focalcalib->FindFocalPlane(8);
  if(fpl8) {
    fpl8v = fpl8->GetOptVector();
    F8X = (*fpl8v)(0);
    F8A = (*fpl8v)(1);
  }

  fpl9  = focalcalib->FindFocalPlane(9);
  if(fpl9) {
    fpl9v = fpl9->GetOptVector();
    F9X = (*fpl9v)(0);
    F9A = (*fpl9v)(1);
  }

  fpl11 = focalcalib->FindFocalPlane(11);
  if(fpl11) {
    fpl11v = fpl11->GetOptVector();
    F11X = (*fpl11v)(0);
    F11A = (*fpl11v)(1);
  }

  if(brcut[0]->IsInside(beam_br_35->GetAoQ(),beam_br_35->GetZet())){
    h_F3X_aoq35_gated->Fill(beam_br_35->GetAoQ(),F3X);
    h_F5X_aoq35_gated->Fill(beam_br_35->GetAoQ(),F5X);
  }

  if(brcut[0]->IsInside(beam_br_35->GetAoQ(),beam_br_35->GetZet()) &&
     zdcut[0]->IsInside(beam_zd_911->GetAoQ(),beam_zd_911->GetZet())){
    h_F8X_aoq911_gated->Fill(beam_zd_911->GetAoQ(),F8X);
    h_F9X_aoq911_gated->Fill(beam_zd_911->GetAoQ(),F9X);
    h_F11X_aoq911_gated->Fill(beam_zd_911->GetAoQ(),F11X);
    h_F5X_F9X_gated->Fill(F5X,F9X);

    h_F3X_F3A_gated->Fill(F3X,F3A);
    h_F5X_F5A_gated->Fill(F5X,F5A);
    h_F7X_F7A_gated->Fill(F7X,F7A);
    h_F8X_F8A_gated->Fill(F8X,F8A);
    h_F9X_F9A_gated->Fill(F9X,F9A);
    h_F11X_F11A_gated->Fill(F11X,F11A);
  }

  //For monitoring
  if(neve%200==0){

    cPos->cd(1);
    h_br_aoq35_zed->Draw("colz");
    
    cPos->cd(2);
    h_br_aoq57_zed->Draw("colz");
    
    cPos->cd(3);
    h_zd_aoq89_zed->Draw("colz");
    
    cPos->cd(4);
    h_zd_aoq911_zed->Draw("colz");

    cPos->cd(5);
    h_zd_aoq89_zed_gated->Draw("colz");
    
    cPos->cd(6);
    h_zd_aoq911_zed_gated->Draw("colz");

    cPos->cd(7);
    h_F3X_aoq35_gated->Draw("colz");

    cPos->cd(8);
    h_F5X_aoq35_gated->Draw("colz");

    cPos->cd(9);
    h_F8X_aoq911_gated->Draw("colz");

    cPos->cd(10);
    h_F9X_aoq911_gated->Draw("colz");

    cPos->cd(11);
    h_F11X_aoq911_gated->Draw("colz");
    
    cPos->cd(12);
    h_F5X_F9X_gated->Draw("colz");
    
    cPos->cd(13);
    h_F3X_F3A_gated->Draw("colz");
    
    cPos->cd(14);
    h_F5X_F5A_gated->Draw("colz");
    
    cPos->cd(15);
    h_F7X_F7A_gated->Draw("colz");
    
    cPos->cd(16);
    h_F8X_F8A_gated->Draw("colz");

    cPos->cd(17);
    h_F9X_F9A_gated->Draw("colz");
    
    cPos->cd(18);
    h_F11X_F11A_gated->Draw("colz");


 

    cPos->Update();
  }
  
  //********************* Clear *****************
  brcalib->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void ShowPosGated::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowPosGated::ClassName() const  {
  return "ShowPosGated";
}
