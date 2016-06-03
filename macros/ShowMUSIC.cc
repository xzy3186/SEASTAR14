#include "ShowMUSIC.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtIC.hh"
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

ShowMUSIC::ShowMUSIC() : TArtAnaLoop(), bigripsparameters(0){
}

ShowMUSIC::~ShowMUSIC() {
}

void ShowMUSIC::Construct() {
  
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");

  //For Online
  cMUSIC = new TCanvas("MUSIC","MUSIC",1400,900);
  cMUSIC->Divide(3,5);

  h_adc[0] = new TH2F("BRADC","BR MUSIC ADC",10,0,10,8000,0,8000); 
  h_adc[1] = new TH2F("ZDADC","ZD MUSIC ADC",10,0,10,8000,0,8000); 

  h_adc_sq_sum[0] = new TH1F("BRADCSQSUM","BR ADC SQ SUM",1000,0,8000); 
  h_adc_sq_sum[1] = new TH1F("ZDADCSQSUM","ZD ADC SQ SUM",1000,0,8000); 

  h_de_sq_sum[0] = new TH1F("BRDESQSUM","BR DE SQ SUM",1000,0,3000); 
  h_de_sq_sum[1] = new TH1F("ZDDESQSUM","ZD DE SQ SUM",1000,0,3000); 
  
  h_de_av_sum[0] = new TH1F("BRDEAVSUM","BR DE AV SUM",1000,0,3000); 
  h_de_av_sum[1] = new TH1F("ZDDEAVSUM","ZD DE AV SUM",1000,0,3000); 

  h_num_hit[0] = new TH1F("BRNumHit","BR Num Hit",10,0,10); 
  h_num_hit[1] = new TH1F("ZDNumHit","ZD Num Hit",10,0,10); 

  h_zed[0] = new TH1F("BRZed","BRZed",1000,0,100); 
  h_zed[1] = new TH1F("ZDZed","ZDZed",1000,0,100); 

  h_tof_de[0] = new TH2F("BRTOFdE","BRTOF dE",500,0,500,200,0,2000); 
  h_tof_de[1] = new TH2F("ZDTOFdE","ZDTOF dE",500,0,500,200,0,2000); 

  h_zed_zed = new TH2F("BRvsZDZed","BR vs ZD Zed",500,0,100,500,0,100); 

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

  TClonesArray * ic_array = 
    (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  tree->Branch(ic_array->GetName(),&ic_array);


   neve=0; 
}

void ShowMUSIC::Calculate()  {
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
 
  h_zed[0]->Fill(beam_br_35->GetZet());
  h_zed[1]->Fill(beam_zd_911->GetZet());
 
  h_zed_zed->Fill(beam_br_35->GetZet(),beam_zd_911->GetZet());

  TArtIC *ic;
  
  char *name_ic[2]={"F7IC","F11IC"};
  
  for(int i =0;i<2;i++){
    ic = iccalib->FindIC((char*)name_ic[i]);
  
    if(ic){ 
      for(int j=0;j<10;j++){
        h_adc[i]->Fill(j,ic->GetRawADC(j));
      }
      h_adc_sq_sum[i]->Fill(ic->GetRawADCSqSum());
      h_de_sq_sum[i]->Fill(ic->GetEnergySqSum());
      h_de_av_sum[i]->Fill(ic->GetEnergyAvSum());
      h_num_hit[i]->Fill(ic->GetNumHit());
      if(i==0)
        h_tof_de[0]->Fill(tof3to7->GetTOF(),ic->GetEnergySqSum());
      if(i==1)
        h_tof_de[1]->Fill(tof8to11->GetTOF(),ic->GetEnergySqSum());
    }
  }

  //For monitoring
  if(neve%200==0){

    cMUSIC->cd(1);
    h_adc[0]->Draw("colz");
    
    cMUSIC->cd(2);
    h_adc_sq_sum[0]->Draw();
    
    cMUSIC->cd(3);
    h_de_sq_sum[0]->Draw();

    cMUSIC->cd(4);
    h_de_av_sum[0]->Draw();

    cMUSIC->cd(5);
    h_num_hit[0]->Draw();

    cMUSIC->cd(6);
    h_zed[0]->Draw();

    cMUSIC->cd(7);
    h_tof_de[0]->Draw("colz");

    cMUSIC->cd(8);
    h_adc[1]->Draw("colz");
    
    cMUSIC->cd(9);
    h_adc_sq_sum[1]->Draw();
    
    cMUSIC->cd(10);
    h_de_sq_sum[1]->Draw();

    cMUSIC->cd(11);
    h_de_av_sum[1]->Draw();

    cMUSIC->cd(12);
    h_num_hit[1]->Draw();

    cMUSIC->cd(13);
    h_zed[1]->Draw();

    cMUSIC->cd(14);
    h_tof_de[1]->Draw("colz");

    cMUSIC->cd(15);
    h_zed_zed->Draw("colz");

    cMUSIC->Update();
  }
  
  //********************* Clear *****************
  brcalib->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void ShowMUSIC::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowMUSIC::ClassName() const  {
  return "ShowMUSIC";
}
