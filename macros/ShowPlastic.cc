#include "ShowPlastic.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtIC.hh"
#include "TArtTOF.hh"
#include "TArtPlastic.hh"
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

ShowPlastic::ShowPlastic() : TArtAnaLoop(), bigripsparameters(0){
}

ShowPlastic::~ShowPlastic() {
}

void ShowPlastic::Construct() {
  
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");

  //For Online
  cPlastic = new TCanvas("Plastic","Plastic",1400,900);
  cPlastic->Divide(4,4);

  h_time_l_raw = new TH2F("TimeLeftRaw","Time Left Raw",4,0,4,4000,0,100000);
  h_time_r_raw = new TH2F("TimeRightRaw","Time Right Raw",4,0,4,4000,0,100000);

  h_time_l = new TH2F("TimeLeft","Time Left",4,0,4,8000,0,8000);
  h_time_r = new TH2F("TimeRight","Time Right",4,0,4,8000,0,8000);

  h_time = new TH2F("Time","Time",4,0,4,8000,0,8000);

  h_ql_raw = new TH2F("QLRaw","Q Left Raw",4,0,4,8000,0,8000);
  h_qr_raw = new TH2F("QRRaw","Q Right Raw",4,0,4,8000,0,8000);

  h_q_ave_raw = new TH2F("Q Ave Raw","Q Ave Raw",4,0,4,8000,0,8000);

  h_tof_br = new TH1F("TOFBR","TOF BR",1000,0,1000);
  h_tof_zd = new TH1F("TOFZD","TOF ZD",1000,0,1000);

  h_tof_br_aoq35 = new TH2F("BRTOFAOQ35","BRTOF AOQ35",200,2,3,500,0,500);
  h_tof_br_aoq57 = new TH2F("BRTOFAOQ57","BRTOF AOQ57",200,2,3,500,0,500);
  h_tof_zd_aoq89 = new TH2F("ZDTOFAOQ89","ZDTOF AOQ89",200,2,3,500,0,500);
  h_tof_zd_aoq911 = new TH2F("ZDTOFAOQ911","ZDTOF AOQ911",200,2,3,500,0,500);

  bigripsparameters  = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter("db/BigRIPSPPAC.xml");
  bigripsparameters->LoadParameter("db/BigRIPSPlastic.xml");
  bigripsparameters->LoadParameter("db/BigRIPSIC.xml");
  bigripsparameters->LoadParameter("db/FocalPlane.xml");
  bigripsparameters->SetFocusPosOffset(8,138.5);

  brcalib = new TArtCalibPID();
  
  iccalib = brcalib->GetCalibIC();
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

  TClonesArray * pla_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  tree->Branch(pla_array->GetName(),&pla_array);

   neve=0; 
}

void ShowPlastic::Calculate()  {
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
 
  TArtPlastic *plastic;
  
  char *name_plastic[4]={"F3pl","F7pl","F8pl","F11pl-1"};
  
  for(int i =0;i<4;i++){
    plastic = plasticcalib->FindPlastic((char*)name_plastic[i]);
  
    if(plastic){ 
  
      h_time_l_raw->Fill(i,plastic->GetTLRaw());
      h_time_r_raw->Fill(i,plastic->GetTRRaw());
      
      h_time_l->Fill(i,plastic->GetTimeL());
      h_time_r->Fill(i,plastic->GetTimeR());

      h_time->Fill(i,plastic->GetTime());

      h_ql_raw->Fill(i,plastic->GetQLRaw());
      h_qr_raw->Fill(i,plastic->GetQRRaw());
      
      h_q_ave_raw->Fill(i,plastic->GetQAveRaw());
    }
  }

  h_tof_br->Fill(tof3to7->GetTOF());
  h_tof_zd->Fill(tof8to11->GetTOF());

  h_tof_br_aoq35->Fill(beam_br_35->GetAoQ(),tof3to7->GetTOF());
  h_tof_br_aoq57->Fill(beam_br_57->GetAoQ(),tof3to7->GetTOF());
  h_tof_zd_aoq89->Fill(beam_zd_89->GetAoQ(),tof8to11->GetTOF());
  h_tof_zd_aoq911->Fill(beam_zd_911->GetAoQ(),tof8to11->GetTOF());

  //For monitoring
  if(neve%200==0){

    cPlastic->cd(1);
    h_time_l_raw->Draw("colz");
    
    cPlastic->cd(2);
    h_time_r_raw->Draw("colz");

    cPlastic->cd(3);
    h_time_l->Draw("colz");
    
    cPlastic->cd(4);
    h_time_r->Draw("colz");

    cPlastic->cd(5);
    h_time->Draw("colz");
    
    cPlastic->cd(6);
    h_ql_raw->Draw("colz");

    cPlastic->cd(7);
    h_qr_raw->Draw("colz");
    
    cPlastic->cd(8);
    h_q_ave_raw->Draw("colz");

    cPlastic->cd(9);
    h_tof_br->Draw();

    cPlastic->cd(10);
    h_tof_zd->Draw("");

    cPlastic->cd(11);
    h_tof_br_aoq35->Draw("colz");

    cPlastic->cd(12);
    h_tof_br_aoq57->Draw("colz");

    cPlastic->cd(13);
    h_tof_zd_aoq89->Draw("colz");

    cPlastic->cd(14);
    h_tof_zd_aoq911->Draw("colz");

    cPlastic->Update();
  }
  
  //********************* Clear *****************
  brcalib->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void ShowPlastic::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowPlastic::ClassName() const  {
  return "ShowPlastic";
}
