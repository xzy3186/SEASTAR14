#include "MakeOnlineBigRIPSTree.hh"

#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtFocalPlane.hh"
#include "TArtIC.hh"
#include "TArtPlastic.hh"
#include "TArtPPAC.hh"
#include "TArtTOF.hh"
#include "TArtRIPS.hh"

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

MakeOnlineBigRIPSTree::MakeOnlineBigRIPSTree() : TArtAnaLoop(), bigripsparameters(0) {
}

MakeOnlineBigRIPSTree::~MakeOnlineBigRIPSTree() {
}

void MakeOnlineBigRIPSTree::Construct() {
  cout<<"call constructor!!"<<endl; 
 
  sman = TArtStoreManager::Instance();

  //fout = new TFile("OnlineBigRIPSTree.root","RECREATE");
  tree = new TTree("tree","tree");

  bigripsparameters  = TArtBigRIPSParameters::Instance();
  bigripsparameters->LoadParameter("db/BigRIPSPPAC.xml");
  bigripsparameters->LoadParameter("db/BigRIPSPlastic.xml");
  bigripsparameters->LoadParameter("db/BigRIPSIC.xml");
  bigripsparameters->LoadParameter("db/FocalPlane.xml");
  bigripsparameters->SetFocusPosOffset(8,138.5);

  cpid  = new TArtCalibPID();
  cppac = cpid->GetCalibPPAC();
  cpla  = cpid->GetCalibPlastic();
  cic   = cpid->GetCalibIC();
  cfpl  = cpid->GetCalibFocalPlane(); 

  std::cout << "Defining bigrips parameters" << std::endl;
   
  recopid = new TArtRecoPID();
  rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat",5.978); // F3 - F5
  rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat",5.046); // F5 - F7
  rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat",4.7464); // F8 - F9 
  rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat",4.7411); // F9 - F11  
  
  // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
  tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",302.2,5); // F3 - F7
  tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-161.2,9); // F8 - F11
  
  // Reconstruction of IC observables for ID
  //beam_br_37 = recopid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
  beam_br_35 = recopid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
  beam_br_57 = recopid->DefineNewBeam(rips5to7,tof3to7,"F7IC");   
  beam_zd_89 = recopid->DefineNewBeam(rips8to9,tof8to11,"F11IC");
  beam_zd_911 = recopid->DefineNewBeam(rips9to11,tof8to11,"F11IC");
 
  tree->Branch("fbit",&fbit,"fbit/I");

  tree->Branch("F3PPAC1A_X",&F3PPAC1A_X,"F3PPAC1A_X/D");
  tree->Branch("F3PPAC1A_Y",&F3PPAC1A_Y,"F3PPAC1A_Y/D");
  tree->Branch("F3PPAC1B_X",&F3PPAC1B_X,"F3PPAC1B_X/D");
  tree->Branch("F3PPAC1B_Y",&F3PPAC1A_Y,"F3PPAC1B_Y/D");
  tree->Branch("F3PPAC2A_X",&F3PPAC2A_X,"F3PPAC2A_X/D");
  tree->Branch("F3PPAC2A_Y",&F3PPAC2A_Y,"F3PPAC2A_Y/D");
  tree->Branch("F3PPAC2B_X",&F3PPAC2B_X,"F3PPAC2B_X/D");
  tree->Branch("F3PPAC2B_Y",&F3PPAC2A_Y,"F3PPAC2B_Y/D");
  
  tree->Branch("F3PPAC1A_sum_x",&F3PPAC1A_sum_x,"F3PPAC1A_sum_x/D");
  tree->Branch("F3PPAC1A_sum_y",&F3PPAC1A_sum_y,"F3PPAC1A_sum_y/D");
  tree->Branch("F3PPAC1B_sum_x",&F3PPAC1B_sum_x,"F3PPAC1B_sum_x/D");
  tree->Branch("F3PPAC1B_sum_y",&F3PPAC1B_sum_y,"F3PPAC1B_sum_y/D");
  tree->Branch("F3PPAC2A_sum_x",&F3PPAC2A_sum_x,"F3PPAC2A_sum_x/D");
  tree->Branch("F3PPAC2A_sum_y",&F3PPAC2A_sum_y,"F3PPAC2A_sum_y/D");
  tree->Branch("F3PPAC2B_sum_x",&F3PPAC2B_sum_x,"F3PPAC2B_sum_x/D");
  tree->Branch("F3PPAC2B_sum_y",&F3PPAC2B_sum_y,"F3PPAC2B_sum_y/D");

  tree->Branch("F3PPAC1A_diff_x",&F3PPAC1A_diff_x,"F3PPAC1A_diff_x/D");
  tree->Branch("F3PPAC1A_diff_y",&F3PPAC1A_diff_y,"F3PPAC1A_diff_y/D");
  tree->Branch("F3PPAC1B_diff_x",&F3PPAC1B_diff_x,"F3PPAC1B_diff_x/D");
  tree->Branch("F3PPAC1B_diff_y",&F3PPAC1B_diff_y,"F3PPAC1B_diff_y/D");
  tree->Branch("F3PPAC2A_diff_x",&F3PPAC2A_diff_x,"F3PPAC2A_diff_x/D");
  tree->Branch("F3PPAC2A_diff_y",&F3PPAC2A_diff_y,"F3PPAC2A_diff_y/D");
  tree->Branch("F3PPAC2B_diff_x",&F3PPAC2B_diff_x,"F3PPAC2B_diff_x/D");
  tree->Branch("F3PPAC2B_diff_y",&F3PPAC2B_diff_y,"F3PPAC2B_diff_y/D");

  tree->Branch("F3X",&F3X,"F3X/D");
  tree->Branch("F3A",&F3A,"F3A/D");
  tree->Branch("F3Y",&F3Y,"F3Y/D");
  tree->Branch("F3B",&F3B,"F3B/D");

  tree->Branch("F3PLA_TL_raw",&F3PLA_TL_raw,"F3PLA_TL_raw/D");
  tree->Branch("F3PLA_TR_raw",&F3PLA_TR_raw,"F3PLA_TR_raw/D");
  tree->Branch("F3PLA_TL",&F3PLA_TL,"F3PLA_TL/D");
  tree->Branch("F3PLA_TR",&F3PLA_TR,"F3PLA_TR/D");
  tree->Branch("F3PLA_T",&F3PLA_T,"F3PLA_T/D");

  tree->Branch("F3PLA_QL_raw",&F3PLA_QL_raw,"F3PLA_QL_raw/I");
  tree->Branch("F3PLA_QR_raw",&F3PLA_QR_raw,"F3PLA_QR_raw/I");
  tree->Branch("F3PLA_Q_ave",&F3PLA_Q_ave,"F3PLA_Q_ave/I");

  tree->Branch("F5PPAC1A_X",&F5PPAC1A_X,"F5PPAC1A_X/D");
  tree->Branch("F5PPAC1A_Y",&F5PPAC1A_Y,"F5PPAC1A_Y/D");
  tree->Branch("F5PPAC1B_X",&F5PPAC1B_X,"F5PPAC1B_X/D");
  tree->Branch("F5PPAC1B_Y",&F5PPAC1A_Y,"F5PPAC1B_Y/D");
  tree->Branch("F5PPAC2A_X",&F5PPAC2A_X,"F5PPAC2A_X/D");
  tree->Branch("F5PPAC2A_Y",&F5PPAC2A_Y,"F5PPAC2A_Y/D");
  tree->Branch("F5PPAC2B_X",&F5PPAC2B_X,"F5PPAC2B_X/D");
  tree->Branch("F5PPAC2B_Y",&F5PPAC2A_Y,"F5PPAC2B_Y/D");

  tree->Branch("F5PPAC1A_sum_x",&F5PPAC1A_sum_x,"F5PPAC1A_sum_x/D");
  tree->Branch("F5PPAC1A_sum_y",&F5PPAC1A_sum_y,"F5PPAC1A_sum_y/D");
  tree->Branch("F5PPAC1B_sum_x",&F5PPAC1B_sum_x,"F5PPAC1B_sum_x/D");
  tree->Branch("F5PPAC1B_sum_y",&F5PPAC1B_sum_y,"F5PPAC1B_sum_y/D");
  tree->Branch("F5PPAC2A_sum_x",&F5PPAC2A_sum_x,"F5PPAC2A_sum_x/D");
  tree->Branch("F5PPAC2A_sum_y",&F5PPAC2A_sum_y,"F5PPAC2A_sum_y/D");
  tree->Branch("F5PPAC2B_sum_x",&F5PPAC2B_sum_x,"F5PPAC2B_sum_x/D");
  tree->Branch("F5PPAC2B_sum_y",&F5PPAC2A_sum_y,"F5PPAC2B_sum_y/D");

  tree->Branch("F5PPAC1A_diff_x",&F5PPAC1A_diff_x,"F5PPAC1A_diff_x/D");
  tree->Branch("F5PPAC1A_diff_y",&F5PPAC1A_diff_y,"F5PPAC1A_diff_y/D");
  tree->Branch("F5PPAC1B_diff_x",&F5PPAC1B_diff_x,"F5PPAC1B_diff_x/D");
  tree->Branch("F5PPAC1B_diff_y",&F5PPAC1B_diff_y,"F5PPAC1B_diff_y/D");
  tree->Branch("F5PPAC2A_diff_x",&F5PPAC2A_diff_x,"F5PPAC2A_diff_x/D");
  tree->Branch("F5PPAC2A_diff_y",&F5PPAC2A_diff_y,"F5PPAC2A_diff_y/D");
  tree->Branch("F5PPAC2B_diff_x",&F5PPAC2B_diff_x,"F5PPAC2B_diff_x/D");
  tree->Branch("F5PPAC2B_diff_y",&F5PPAC2B_diff_y,"F5PPAC2B_diff_y/D");
  
  tree->Branch("F5X",&F5X,"F5X/D");
  tree->Branch("F5A",&F5A,"F5A/D");
  tree->Branch("F5Y",&F5Y,"F5Y/D");
  tree->Branch("F5B",&F5B,"F5B/D");

  tree->Branch("F7PPAC1A_X",&F7PPAC1A_X,"F7PPAC1A_X/D");
  tree->Branch("F7PPAC1A_Y",&F7PPAC1A_Y,"F7PPAC1A_Y/D");
  tree->Branch("F7PPAC1B_X",&F7PPAC1B_X,"F7PPAC1B_X/D");
  tree->Branch("F7PPAC1B_Y",&F7PPAC1A_Y,"F7PPAC1B_Y/D");
  tree->Branch("F7PPAC2A_X",&F7PPAC2A_X,"F7PPAC2A_X/D");
  tree->Branch("F7PPAC2A_Y",&F7PPAC2A_Y,"F7PPAC2A_Y/D");
  tree->Branch("F7PPAC2B_X",&F7PPAC2B_X,"F7PPAC2B_X/D");
  tree->Branch("F7PPAC2B_Y",&F7PPAC2A_Y,"F7PPAC2B_Y/D");
  
  tree->Branch("F7PPAC1A_sum_x",&F7PPAC1A_sum_x,"F7PPAC1A_sum_x/D");
  tree->Branch("F7PPAC1A_sum_y",&F7PPAC1A_sum_y,"F7PPAC1A_sum_y/D");
  tree->Branch("F7PPAC1B_sum_x",&F7PPAC1B_sum_x,"F7PPAC1B_sum_x/D");
  tree->Branch("F7PPAC1B_sum_y",&F7PPAC1B_sum_y,"F7PPAC1B_sum_y/D");
  tree->Branch("F7PPAC2A_sum_x",&F7PPAC2A_sum_x,"F7PPAC2A_sum_x/D");
  tree->Branch("F7PPAC2A_sum_y",&F7PPAC2A_sum_y,"F7PPAC2A_sum_y/D");
  tree->Branch("F7PPAC2B_sum_x",&F7PPAC2B_sum_x,"F7PPAC2B_sum_x/D");
  tree->Branch("F7PPAC2B_sum_y",&F7PPAC2B_sum_y,"F7PPAC2B_sum_y/D");
		 
  tree->Branch("F7PPAC1A_diff_x",&F7PPAC1A_diff_x,"F7PPAC1A_diff_x/D");
  tree->Branch("F7PPAC1A_diff_y",&F7PPAC1A_diff_y,"F7PPAC1A_diff_y/D");
  tree->Branch("F7PPAC1B_diff_x",&F7PPAC1B_diff_x,"F7PPAC1B_diff_x/D");
  tree->Branch("F7PPAC1B_diff_y",&F7PPAC1B_diff_y,"F7PPAC1B_diff_y/D");
  tree->Branch("F7PPAC2A_diff_x",&F7PPAC2A_diff_x,"F7PPAC2A_diff_x/D");
  tree->Branch("F7PPAC2A_diff_y",&F7PPAC2A_diff_y,"F7PPAC2A_diff_y/D");
  tree->Branch("F7PPAC2B_diff_x",&F7PPAC2B_diff_x,"F7PPAC2B_diff_x/D");
  tree->Branch("F7PPAC2B_diff_y",&F7PPAC2B_diff_y,"F7PPAC2B_diff_y/D");

  tree->Branch("F7X",&F7X,"F7X/D");
  tree->Branch("F7A",&F7A,"F7A/D");
  tree->Branch("F7Y",&F7Y,"F7Y/D");
  tree->Branch("F7B",&F7B,"F7B/D");

  tree->Branch("F7PLA_TL_raw",&F7PLA_TL_raw,"F7PLA_TL_raw/D");
  tree->Branch("F7PLA_TR_raw",&F7PLA_TR_raw,"F7PLA_TR_raw/D");
  tree->Branch("F7PLA_TL",&F7PLA_TL,"F7PLA_TL/D");
  tree->Branch("F7PLA_TR",&F7PLA_TR,"F7PLA_TR/D");
  tree->Branch("F7PLA_T",&F7PLA_T,"F7PLA_T/D");

  tree->Branch("F7PLA_QL_raw",&F7PLA_QL_raw,"F7PLA_QL_raw/I");
  tree->Branch("F7PLA_QR_raw",&F7PLA_QR_raw,"F7PLA_QR_raw/I");
  tree->Branch("F7PLA_Q_ave",&F7PLA_Q_ave,"F7PLA_Q_ave/I");

  tree->Branch("F8PPAC1A_X",&F8PPAC1A_X,"F8PPAC1A_X/D");
  tree->Branch("F8PPAC1A_Y",&F8PPAC1A_Y,"F8PPAC1A_Y/D");
  tree->Branch("F8PPAC1B_X",&F8PPAC1B_X,"F8PPAC1B_X/D");
  tree->Branch("F8PPAC1B_Y",&F8PPAC1B_Y,"F8PPAC1B_Y/D");
  tree->Branch("F8PPAC2A_X",&F8PPAC2A_X,"F8PPAC2A_X/D");
  tree->Branch("F8PPAC2A_Y",&F8PPAC2A_Y,"F8PPAC2A_Y/D");
  tree->Branch("F8PPAC2B_X",&F8PPAC2B_X,"F8PPAC2B_X/D");
  tree->Branch("F8PPAC2B_Y",&F8PPAC2A_Y,"F8PPAC2B_Y/D");
  tree->Branch("F8PPAC3A_X",&F8PPAC3A_X,"F8PPAC3A_X/D");
  tree->Branch("F8PPAC3A_Y",&F8PPAC3A_Y,"F8PPAC3A_Y/D");
  tree->Branch("F8PPAC3B_X",&F8PPAC3B_X,"F8PPAC3B_X/D");
  tree->Branch("F8PPAC3B_Y",&F8PPAC3B_Y,"F8PPAC3B_Y/D");

  tree->Branch("F8PPAC1A_sum_x",&F8PPAC1A_sum_x,"F8PPAC1A_sum_x/D");
  tree->Branch("F8PPAC1A_sum_y",&F8PPAC1A_sum_y,"F8PPAC1A_sum_y/D");
  tree->Branch("F8PPAC1B_sum_x",&F8PPAC1B_sum_x,"F8PPAC1B_sum_x/D");
  tree->Branch("F8PPAC1B_sum_y",&F8PPAC1B_sum_y,"F8PPAC1B_sum_y/D");
  tree->Branch("F8PPAC2A_sum_x",&F8PPAC2A_sum_x,"F8PPAC2A_sum_x/D");
  tree->Branch("F8PPAC2A_sum_y",&F8PPAC2A_sum_y,"F8PPAC2A_sum_y/D");
  tree->Branch("F8PPAC2B_sum_x",&F8PPAC2B_sum_x,"F8PPAC2B_sum_x/D");
  tree->Branch("F8PPAC2B_sum_y",&F8PPAC2A_sum_y,"F8PPAC2B_sum_y/D");
  tree->Branch("F8PPAC3A_sum_x",&F8PPAC3A_sum_x,"F8PPAC3A_sum_x/D");
  tree->Branch("F8PPAC3A_sum_y",&F8PPAC3A_sum_y,"F8PPAC3A_sum_y/D");
  tree->Branch("F8PPAC3B_sum_x",&F8PPAC3B_sum_x,"F8PPAC3B_sum_x/D");
  tree->Branch("F8PPAC3B_sum_y",&F8PPAC3B_sum_y,"F8PPAC3B_sum_y/D");

  tree->Branch("F8PPAC1A_diff_x",&F8PPAC1A_diff_x,"F8PPAC1A_diff_x/D");
  tree->Branch("F8PPAC1A_diff_y",&F8PPAC1A_diff_y,"F8PPAC1A_diff_y/D");
  tree->Branch("F8PPAC1B_diff_x",&F8PPAC1B_diff_x,"F8PPAC1B_diff_x/D");
  tree->Branch("F8PPAC1B_diff_y",&F8PPAC1B_diff_y,"F8PPAC1B_diff_y/D");
  tree->Branch("F8PPAC2A_diff_x",&F8PPAC2A_diff_x,"F8PPAC2A_diff_x/D");
  tree->Branch("F8PPAC2A_diff_y",&F8PPAC2A_diff_y,"F8PPAC2A_diff_y/D");
  tree->Branch("F8PPAC2B_diff_x",&F8PPAC2B_diff_x,"F8PPAC2B_diff_x/D");
  tree->Branch("F8PPAC2B_diff_y",&F8PPAC2B_diff_y,"F8PPAC2B_diff_y/D");
  tree->Branch("F8PPAC3A_diff_x",&F8PPAC3A_diff_x,"F8PPAC3A_diff_x/D");
  tree->Branch("F8PPAC3A_diff_y",&F8PPAC3A_diff_y,"F8PPAC3A_diff_y/D");
  tree->Branch("F8PPAC3B_diff_x",&F8PPAC3B_diff_x,"F8PPAC3B_diff_x/D");
  tree->Branch("F8PPAC3B_diff_y",&F8PPAC3B_diff_y,"F8PPAC3B_diff_y/D");

  tree->Branch("F8X",&F8X,"F8X/D");
  tree->Branch("F8A",&F8A,"F8A/D");
  tree->Branch("F8Y",&F8Y,"F8Y/D");
  tree->Branch("F8B",&F8B,"F8B/D");

  tree->Branch("F8PLA_TL_raw",&F8PLA_TL_raw,"F8PLA_TL_raw/D");
  tree->Branch("F8PLA_TR_raw",&F8PLA_TR_raw,"F8PLA_TR_raw/D");
  tree->Branch("F8PLA_TL",&F8PLA_TL,"F8PLA_TL/D");
  tree->Branch("F8PLA_TR",&F8PLA_TR,"F8PLA_TR/D");
  tree->Branch("F8PLA_T",&F8PLA_T,"F8PLA_T/D");

  tree->Branch("F8PLA_QL_raw",&F8PLA_QL_raw,"F8PLA_QL_raw/I");
  tree->Branch("F8PLA_QR_raw",&F8PLA_QR_raw,"F8PLA_QR_raw/I");
  tree->Branch("F8PLA_Q_ave",&F8PLA_Q_ave,"F8PLA_Q_ave/I");

  tree->Branch("F9PPAC1A_X",&F9PPAC1A_X,"F9PPAC1A_X/D");
  tree->Branch("F9PPAC1A_Y",&F9PPAC1A_Y,"F9PPAC1A_Y/D");
  tree->Branch("F9PPAC1B_X",&F9PPAC1B_X,"F9PPAC1B_X/D");
  tree->Branch("F9PPAC1B_Y",&F9PPAC1A_Y,"F9PPAC1B_Y/D");
  tree->Branch("F9PPAC2A_X",&F9PPAC2A_X,"F9PPAC2A_X/D");
  tree->Branch("F9PPAC2A_Y",&F9PPAC2A_Y,"F9PPAC2A_Y/D");
  tree->Branch("F9PPAC2B_X",&F9PPAC2B_X,"F9PPAC2B_X/D");
  tree->Branch("F9PPAC2B_Y",&F9PPAC2A_Y,"F9PPAC2B_Y/D");
  
  tree->Branch("F9PPAC1A_sum_x",&F9PPAC1A_sum_x,"F9PPAC1A_sum_x/D");
  tree->Branch("F9PPAC1A_sum_y",&F9PPAC1A_sum_y,"F9PPAC1A_sum_y/D");
  tree->Branch("F9PPAC1B_sum_x",&F9PPAC1B_sum_x,"F9PPAC1B_sum_x/D");
  tree->Branch("F9PPAC1B_sum_y",&F9PPAC1B_sum_y,"F9PPAC1B_sum_y/D");
  tree->Branch("F9PPAC2A_sum_x",&F9PPAC2A_sum_x,"F9PPAC2A_sum_x/D");
  tree->Branch("F9PPAC2A_sum_y",&F9PPAC2A_sum_y,"F9PPAC2A_sum_y/D");
  tree->Branch("F9PPAC2B_sum_x",&F9PPAC2B_sum_x,"F9PPAC2B_sum_x/D");
  tree->Branch("F9PPAC2B_sum_y",&F9PPAC2B_sum_y,"F9PPAC2B_sum_y/D");
		 
  tree->Branch("F9PPAC1A_diff_x",&F9PPAC1A_diff_x,"F9PPAC1A_diff_x/D");
  tree->Branch("F9PPAC1A_diff_y",&F9PPAC1A_diff_y,"F9PPAC1A_diff_y/D");
  tree->Branch("F9PPAC1B_diff_x",&F9PPAC1B_diff_x,"F9PPAC1B_diff_x/D");
  tree->Branch("F9PPAC1B_diff_y",&F9PPAC1B_diff_y,"F9PPAC1B_diff_y/D");
  tree->Branch("F9PPAC2A_diff_x",&F9PPAC2A_diff_x,"F9PPAC2A_diff_x/D");
  tree->Branch("F9PPAC2A_diff_y",&F9PPAC2A_diff_y,"F9PPAC2A_diff_y/D");
  tree->Branch("F9PPAC2B_diff_x",&F9PPAC2B_diff_x,"F9PPAC2B_diff_x/D");
  tree->Branch("F9PPAC2B_diff_y",&F9PPAC2B_diff_y,"F9PPAC2B_diff_y/D");

  tree->Branch("F9X",&F9X,"F9X/D");
  tree->Branch("F9A",&F9A,"F9A/D");
  tree->Branch("F9Y",&F9Y,"F9Y/D");
  tree->Branch("F9B",&F9B,"F9B/D");
    
  tree->Branch("F11PPAC1A_X",&F11PPAC1A_X,"F11PPAC1A_X/D");
  tree->Branch("F11PPAC1A_Y",&F11PPAC1A_Y,"F11PPAC1A_Y/D");
  tree->Branch("F11PPAC1B_X",&F11PPAC1B_X,"F11PPAC1B_X/D");
  tree->Branch("F11PPAC1B_Y",&F11PPAC1A_Y,"F11PPAC1B_Y/D");
  tree->Branch("F11PPAC2A_X",&F11PPAC2A_X,"F11PPAC2A_X/D");
  tree->Branch("F11PPAC2A_Y",&F11PPAC2A_Y,"F11PPAC2A_Y/D");
  tree->Branch("F11PPAC2B_X",&F11PPAC2B_X,"F11PPAC2B_X/D");
  tree->Branch("F11PPAC2B_Y",&F11PPAC2A_Y,"F11PPAC2B_Y/D");

  tree->Branch("F11PPAC1A_sum_x",&F11PPAC1A_sum_x,"F11PPAC1A_sum_x/D");
  tree->Branch("F11PPAC1A_sum_y",&F11PPAC1A_sum_y,"F11PPAC1A_sum_y/D");
  tree->Branch("F11PPAC1B_sum_x",&F11PPAC1B_sum_x,"F11PPAC1B_sum_x/D");
  tree->Branch("F11PPAC1B_sum_y",&F11PPAC1B_sum_y,"F11PPAC1B_sum_y/D");
  tree->Branch("F11PPAC2A_sum_x",&F11PPAC2A_sum_x,"F11PPAC2A_sum_x/D");
  tree->Branch("F11PPAC2A_sum_y",&F11PPAC2A_sum_y,"F11PPAC2A_sum_y/D");
  tree->Branch("F11PPAC2B_sum_x",&F11PPAC2B_sum_x,"F11PPAC2B_sum_x/D");
  tree->Branch("F11PPAC2B_sum_y",&F11PPAC2B_sum_y,"F11PPAC2B_sum_y/D");
		 
  tree->Branch("F11PPAC1A_diff_x",&F11PPAC1A_diff_x,"F11PPAC1A_diff_x/D");
  tree->Branch("F11PPAC1A_diff_y",&F11PPAC1A_diff_y,"F11PPAC1A_diff_y/D");
  tree->Branch("F11PPAC1B_diff_x",&F11PPAC1B_diff_x,"F11PPAC1B_diff_x/D");
  tree->Branch("F11PPAC1B_diff_y",&F11PPAC1B_diff_y,"F11PPAC1B_diff_y/D");
  tree->Branch("F11PPAC2A_diff_x",&F11PPAC2A_diff_x,"F11PPAC2A_diff_x/D");
  tree->Branch("F11PPAC2A_diff_y",&F11PPAC2A_diff_y,"F11PPAC2A_diff_y/D");
  tree->Branch("F11PPAC2B_diff_x",&F11PPAC2B_diff_x,"F11PPAC2B_diff_x/D");
  tree->Branch("F11PPAC2B_diff_y",&F11PPAC2B_diff_y,"F11PPAC2B_diff_y/D");

  tree->Branch("F11X",&F11X,"F11X/D");
  tree->Branch("F11A",&F11A,"F11A/D");
  tree->Branch("F11Y",&F11Y,"F11Y/D");
  tree->Branch("F11B",&F11B,"F11B/D");

  tree->Branch("F11PLA_TL_raw",&F11PLA_TL_raw,"F11PLA_TL_raw/D");
  tree->Branch("F11PLA_TR_raw",&F11PLA_TR_raw,"F11PLA_TR_raw/D");
  tree->Branch("F11PLA_TL",&F11PLA_TL,"F11PLA_TL/D");
  tree->Branch("F11PLA_TR",&F11PLA_TR,"F11PLA_TR/D");
  tree->Branch("F11PLA_T",&F11PLA_T,"F11PLA_T/D");

  tree->Branch("F11PLA_QL_raw",&F11PLA_QL_raw,"F11PLA_QL_raw/I");
  tree->Branch("F11PLA_QR_raw",&F11PLA_QR_raw,"F11PLA_QR_raw/I");
  tree->Branch("F11PLA_Q_ave",&F11PLA_Q_ave,"F11PLA_Q_ave/I");

  tree->Branch("F7ICRawADC",F7ICRawADC,"F7ICRawADC[8]/I");
  tree->Branch("F7ICNumHit",&F7ICNumHit,"F7ICNumHit/I");
  tree->Branch("F7ICEnergySqSum",&F7ICEnergySqSum,"F7ICEnergySqSum/D");
  tree->Branch("F7ICEnergyAvSum",&F7ICEnergyAvSum,"F7ICEnergyAvSum/D");

  tree->Branch("F11ICRawADC",F11ICRawADC,"F11ICRawADC[8]/I");
  tree->Branch("F11ICNumHit",&F11ICNumHit,"F11ICNumHit/I");
  tree->Branch("F11ICEnergySqSum",&F11ICEnergySqSum,"F11ICEnergySqSum/D");
  tree->Branch("F11ICEnergyAvSum",&F11ICEnergyAvSum,"F11ICEnergyAvSum/D");
  
  tree->Branch("tof",tof,"tof[4]/D");
  tree->Branch("beta",beta,"beta[4]/D");
 
  tree->Branch("zet",zet,"zet[4]/D");
  tree->Branch("aoq",aoq,"aoq[4]/D");
  tree->Branch("delta",delta,"delta[4]/D");

  neve=0; 
}

void MakeOnlineBigRIPSTree::Calculate()  {
  //cout<<"call Calculate()"<<endl;
  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  int id = 0;
  int eventnum = rawevent->GetEventNumber();
  
  TClonesArray *info = (TClonesArray*)sman->FindDataContainer("EventInfo");
  fbit = (Int_t)((TArtEventInfo *)info->At(0))->GetTriggerBit();
 
  cpid->ReconstructData();
  recopid->ReconstructData();

  //The ppacs
  F3PPAC1A_X=-9999; F3PPAC1A_Y=-9999; F3PPAC1B_X=-9999; F3PPAC1B_Y=-9999;
  F3PPAC2A_X=-9999; F3PPAC2A_Y=-9999; F3PPAC2B_X=-9999; F3PPAC2B_Y=-9999;
  
  F3PPAC1A_sum_x=-9999; F3PPAC1A_sum_y=-9999; F3PPAC1B_sum_x=-9999; F3PPAC1B_sum_y=-9999;
  F3PPAC2A_sum_x=-9999; F3PPAC2A_sum_y=-9999; F3PPAC2B_sum_x=-9999; F3PPAC2B_sum_y=-9999;

  F3PPAC1A_diff_x=-9999; F3PPAC1A_diff_y=-9999; F3PPAC1B_diff_x=-9999; F3PPAC1B_diff_y=-9999;
  F3PPAC2A_diff_x=-9999; F3PPAC2A_diff_y=-9999; F3PPAC2B_diff_x=-9999; F3PPAC2B_diff_y=-9999;
  
  F5PPAC1A_X=-9999; F5PPAC1A_Y=-9999; F5PPAC1B_X=-9999; F5PPAC1B_Y=-9999;
  F5PPAC2A_X=-9999; F5PPAC2A_Y=-9999; F5PPAC2B_X=-9999; F5PPAC2B_Y=-9999;

  F5PPAC1A_sum_x=-9999; F5PPAC1A_sum_y=-9999; F5PPAC1B_sum_x=-9999; F5PPAC1B_sum_y=-9999;
  F5PPAC2A_sum_x=-9999; F5PPAC2A_sum_y=-9999; F5PPAC2B_sum_x=-9999; F5PPAC2B_sum_y=-9999;
   
  F5PPAC1A_diff_x=-9999; F5PPAC1A_diff_y=-9999; F5PPAC1B_diff_x=-9999; F5PPAC1B_diff_y=-9999;
  F5PPAC2A_diff_x=-9999; F5PPAC2A_diff_y=-9999; F5PPAC2B_diff_x=-9999; F5PPAC2B_diff_y=-9999;
  
  F7PPAC1A_X=-9999; F7PPAC1A_Y=-9999; F7PPAC1B_X=-9999; F7PPAC1B_Y=-9999;
  F7PPAC2A_X=-9999; F7PPAC2A_Y=-9999; F7PPAC2B_X=-9999; F7PPAC2B_Y=-9999;
  
  F7PPAC1A_sum_x=-9999; F7PPAC1A_sum_y=-9999; F7PPAC1B_sum_x=-9999; F7PPAC1B_sum_y=-9999;
  F7PPAC2A_sum_x=-9999; F7PPAC2A_sum_y=-9999; F7PPAC2B_sum_x=-9999; F7PPAC2B_sum_y=-9999;
   
  F7PPAC1A_diff_x=-9999; F7PPAC1A_diff_y=-9999; F7PPAC1B_diff_x=-9999; F7PPAC1B_diff_y=-9999;
  F7PPAC2A_diff_x=-9999; F7PPAC2A_diff_y=-9999; F7PPAC2B_diff_x=-9999; F7PPAC2B_diff_y=-9999;

  F8PPAC1A_X=-9999; F8PPAC1A_Y=-9999; F8PPAC1B_X=-9999; F8PPAC1B_Y=-9999;
  F8PPAC2A_X=-9999; F8PPAC2A_Y=-9999; F8PPAC2B_X=-9999; F8PPAC2B_Y=-9999;
  F8PPAC3A_X=-9999; F8PPAC3A_Y=-9999; F8PPAC3B_X=-9999; F8PPAC3B_Y=-9999;

  F8PPAC1A_sum_x=-9999; F8PPAC1A_sum_y=-9999; F8PPAC1B_sum_x=-9999; F8PPAC1B_sum_y=-9999;
  F8PPAC2A_sum_x=-9999; F8PPAC2A_sum_y=-9999; F8PPAC2B_sum_x=-9999; F8PPAC2B_sum_y=-9999;
  F8PPAC3A_sum_x=-9999; F8PPAC3A_sum_y=-9999; F8PPAC3B_sum_x=-9999; F8PPAC3B_sum_y=-9999;
   
  F8PPAC1A_diff_x=-9999; F8PPAC1A_diff_y=-9999; F8PPAC1B_diff_x=-9999; F8PPAC1B_diff_y=-9999;
  F8PPAC2A_diff_x=-9999; F8PPAC2A_diff_y=-9999; F8PPAC2B_diff_x=-9999; F8PPAC2B_diff_y=-9999;
  F8PPAC2A_diff_x=-9999; F8PPAC2A_diff_y=-9999; F8PPAC2B_diff_x=-9999; F8PPAC2B_diff_y=-9999;

  F9PPAC1A_X=-9999; F9PPAC1A_Y=-9999; F9PPAC1B_X=-9999; F9PPAC1B_Y=-9999;
  F9PPAC2A_X=-9999; F9PPAC2A_Y=-9999; F9PPAC2B_X=-9999; F9PPAC2B_Y=-9999;

  F9PPAC1A_sum_x=-9999; F9PPAC1A_sum_y=-9999; F9PPAC1B_sum_x=-9999; F9PPAC1B_sum_y=-9999;
  F9PPAC2A_sum_x=-9999; F9PPAC2A_sum_y=-9999; F9PPAC2B_sum_x=-9999; F9PPAC2B_sum_y=-9999;
   
  F9PPAC1A_diff_x=-9999; F9PPAC1A_diff_y=-9999; F9PPAC1B_diff_x=-9999; F9PPAC1B_diff_y=-9999;
  F9PPAC2A_diff_x=-9999; F9PPAC2A_diff_y=-9999; F9PPAC2B_diff_x=-9999; F9PPAC2B_diff_y=-9999;
  
  F11PPAC1A_X=-9999; F11PPAC1A_Y=-9999; F11PPAC1B_X=-9999; F11PPAC1B_Y=-9999;
  F11PPAC2A_X=-9999; F11PPAC2A_Y=-9999; F11PPAC2B_X=-9999; F11PPAC2B_Y=-9999;

  F11PPAC1A_sum_x=-9999; F11PPAC1A_sum_y=-9999; F11PPAC1B_sum_x=-9999; F11PPAC1B_sum_y=-9999;
  F11PPAC2A_sum_x=-9999; F11PPAC2A_sum_y=-9999; F11PPAC2B_sum_x=-9999; F11PPAC2B_sum_y=-9999;
   
  F11PPAC1A_diff_x=-9999; F7PPAC1A_diff_y=-9999; F11PPAC1B_diff_x=-9999; F11PPAC1B_diff_y=-9999;
  F11PPAC2A_diff_x=-9999; F7PPAC2A_diff_y=-9999; F11PPAC2B_diff_x=-9999; F11PPAC2B_diff_y=-9999;


  TArtPPAC *tppac;
  tppac = cppac->FindPPAC("F3PPAC-1A");
  if(tppac){F3PPAC1A_X = tppac->GetX(); F3PPAC1A_Y = tppac->GetY();
    F3PPAC1A_sum_x = tppac->GetTSumX(); F3PPAC1A_sum_y = tppac->GetTSumY();
    F3PPAC1A_diff_x = tppac->GetTDiffX(); F3PPAC1A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F3PPAC-1B");
  if(tppac){F3PPAC1B_X = tppac->GetX(); F3PPAC1B_Y = tppac->GetY();
    F3PPAC1B_sum_x = tppac->GetTSumX(); F3PPAC1B_sum_y = tppac->GetTSumY();
    F3PPAC1B_diff_x = tppac->GetTDiffX(); F3PPAC1B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F3PPAC-2A");
  if(tppac){F3PPAC2A_X = tppac->GetX(); F3PPAC2A_Y = tppac->GetY();
    F3PPAC2A_sum_x = tppac->GetTSumX(); F3PPAC2A_sum_y = tppac->GetTSumY();
    F3PPAC2A_diff_x = tppac->GetTDiffX(); F3PPAC2A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F3PPAC-2B");
  if(tppac){F3PPAC2B_X = tppac->GetX(); F3PPAC2B_Y = tppac->GetY();
    F3PPAC2B_sum_x = tppac->GetTSumX(); F3PPAC2B_sum_y = tppac->GetTSumY();
    F3PPAC2B_diff_x = tppac->GetTDiffX(); F3PPAC2B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F5PPAC-1A");
  if(tppac){F5PPAC1A_X = tppac->GetX(); F5PPAC1A_Y = tppac->GetY();
    F5PPAC1A_sum_x = tppac->GetTSumX(); F5PPAC1A_sum_y = tppac->GetTSumY();
    F5PPAC1A_diff_x = tppac->GetTDiffX(); F5PPAC1A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F5PPAC-1B");
  if(tppac){F5PPAC1B_X = tppac->GetX(); F5PPAC1B_Y = tppac->GetY();
    F5PPAC1B_sum_x = tppac->GetTSumX(); F5PPAC1B_sum_y = tppac->GetTSumY();
    F5PPAC1B_diff_x = tppac->GetTDiffX(); F5PPAC1B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F5PPAC-2A");
  if(tppac){F5PPAC2A_X = tppac->GetX(); F5PPAC2A_Y = tppac->GetY();
    F5PPAC2A_sum_x = tppac->GetTSumX(); F5PPAC2A_sum_y = tppac->GetTSumY();
    F5PPAC2A_diff_x = tppac->GetTDiffX(); F5PPAC2A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F5PPAC-2B");
  if(tppac){F5PPAC2B_X = tppac->GetX(); F5PPAC2B_Y = tppac->GetY();
    F5PPAC2B_sum_x = tppac->GetTSumX(); F5PPAC2B_sum_y = tppac->GetTSumY();
    F5PPAC2B_diff_x = tppac->GetTDiffX(); F5PPAC2B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F7PPAC-1A");
  if(tppac){F7PPAC1A_X = tppac->GetX(); F7PPAC1A_Y = tppac->GetY();
    F7PPAC1A_sum_x = tppac->GetTSumX(); F7PPAC1A_sum_y = tppac->GetTSumY();
    F7PPAC1A_diff_x = tppac->GetTDiffX(); F7PPAC1A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F7PPAC-1B");
  if(tppac){F7PPAC1B_X = tppac->GetX(); F7PPAC1B_Y = tppac->GetY();
    F5PPAC1B_sum_x = tppac->GetTSumX(); F7PPAC1B_sum_y = tppac->GetTSumY();
    F5PPAC1B_diff_x = tppac->GetTDiffX(); F7PPAC1B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F7PPAC-2A");
  if(tppac){F7PPAC2A_X = tppac->GetX(); F7PPAC2A_Y = tppac->GetY();
    F7PPAC2A_sum_x = tppac->GetTSumX(); F7PPAC2A_sum_y = tppac->GetTSumY();
    F7PPAC2A_diff_x = tppac->GetTDiffX(); F7PPAC2A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F7PPAC-2B");
  if(tppac){F7PPAC2B_X = tppac->GetX(); F7PPAC2B_Y = tppac->GetY();
    F7PPAC2B_sum_x = tppac->GetTSumX(); F7PPAC2B_sum_y = tppac->GetTSumY();
    F7PPAC2B_diff_x = tppac->GetTDiffX(); F7PPAC2B_diff_y = tppac->GetTDiffY();
  }
  
  tppac = cppac->FindPPAC("F8PPAC-1A");
  if(tppac){F8PPAC1A_X = tppac->GetX(); F8PPAC1A_Y = tppac->GetY();
    F8PPAC1A_sum_x = tppac->GetTSumX(); F8PPAC1A_sum_y = tppac->GetTSumY();
    F8PPAC1A_diff_x = tppac->GetTDiffX(); F8PPAC1A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F8PPAC-1B");
  if(tppac){F8PPAC1B_X = tppac->GetX(); F8PPAC1B_Y = tppac->GetY();
    F8PPAC1B_sum_x = tppac->GetTSumX(); F8PPAC1B_sum_y = tppac->GetTSumY();
    F8PPAC1B_diff_x = tppac->GetTDiffX(); F8PPAC1B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F8PPAC-2A");
  if(tppac){F8PPAC2A_X = tppac->GetX(); F8PPAC2A_Y = tppac->GetY();
    F8PPAC2A_sum_x = tppac->GetTSumX(); F8PPAC2A_sum_y = tppac->GetTSumY();
    F8PPAC2A_diff_x = tppac->GetTDiffX(); F8PPAC2A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F8PPAC-2B");
  if(tppac){F8PPAC2B_X = tppac->GetX(); F8PPAC2B_Y = tppac->GetY();
    F8PPAC2B_sum_x = tppac->GetTSumX(); F8PPAC2B_sum_y = tppac->GetTSumY();
    F8PPAC2B_diff_x = tppac->GetTDiffX(); F8PPAC2B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F8PPAC-3A");
  if(tppac){F8PPAC3A_X = tppac->GetX(); F8PPAC3A_Y = tppac->GetY();
    F8PPAC3A_sum_x = tppac->GetTSumX(); F8PPAC3A_sum_y = tppac->GetTSumY();
    F8PPAC3A_diff_x = tppac->GetTDiffX(); F8PPAC3A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F8PPAC-3B");
  if(tppac){F8PPAC3B_X = tppac->GetX(); F8PPAC3B_Y = tppac->GetY();
    F8PPAC3B_sum_x = tppac->GetTSumX(); F8PPAC3B_sum_y = tppac->GetTSumY();
    F8PPAC3B_diff_x = tppac->GetTDiffX(); F8PPAC3B_diff_y = tppac->GetTDiffY();
  }

  tppac = cppac->FindPPAC("F9PPAC-1A");
  if(tppac){F9PPAC1A_X = tppac->GetX(); F9PPAC1A_Y = tppac->GetY();
    F9PPAC1A_sum_x = tppac->GetTSumX(); F9PPAC1A_sum_y = tppac->GetTSumY();
    F9PPAC1A_diff_x = tppac->GetTDiffX(); F9PPAC1A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F9PPAC-1B");
  if(tppac){F9PPAC1B_X = tppac->GetX(); F9PPAC1B_Y = tppac->GetY();
    F9PPAC1B_sum_x = tppac->GetTSumX(); F9PPAC1B_sum_y = tppac->GetTSumY();
    F9PPAC1B_diff_x = tppac->GetTDiffX(); F9PPAC1B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F9PPAC-2A");
  if(tppac){F9PPAC2A_X = tppac->GetX(); F9PPAC2A_Y = tppac->GetY();
    F9PPAC2A_sum_x = tppac->GetTSumX(); F9PPAC2A_sum_y = tppac->GetTSumY();
    F9PPAC2A_diff_x = tppac->GetTDiffX(); F9PPAC2A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F9PPAC-2B");
  if(tppac){F9PPAC2B_X = tppac->GetX(); F9PPAC2B_Y = tppac->GetY();
    F9PPAC2B_sum_x = tppac->GetTSumX(); F9PPAC2B_sum_y = tppac->GetTSumY();
    F9PPAC2B_diff_x = tppac->GetTDiffX(); F9PPAC2B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F11PPAC-1A");
  if(tppac){F11PPAC1A_X = tppac->GetX(); F11PPAC1A_Y = tppac->GetY();
    F11PPAC1A_sum_x = tppac->GetTSumX(); F11PPAC1A_sum_y = tppac->GetTSumY();
    F11PPAC1A_diff_x = tppac->GetTDiffX(); F11PPAC1A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F11PPAC-1B");
  if(tppac){F11PPAC1B_X = tppac->GetX(); F11PPAC1B_Y = tppac->GetY();
    F11PPAC1B_sum_x = tppac->GetTSumX(); F11PPAC1B_sum_y = tppac->GetTSumY();
    F11PPAC1B_diff_x = tppac->GetTDiffX(); F11PPAC1B_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F11PPAC-2A");
  if(tppac){F11PPAC2A_X = tppac->GetX(); F11PPAC2A_Y = tppac->GetY();
    F11PPAC2A_sum_x = tppac->GetTSumX(); F11PPAC2A_sum_y = tppac->GetTSumY();
    F11PPAC2A_diff_x = tppac->GetTDiffX(); F11PPAC2A_diff_y = tppac->GetTDiffY();
  }
  tppac = cppac->FindPPAC("F11PPAC-2B");
  if(tppac){F11PPAC2B_X = tppac->GetX(); F11PPAC2B_Y = tppac->GetY();
    F11PPAC2B_sum_x = tppac->GetTSumX(); F11PPAC2B_sum_y = tppac->GetTSumY();
    F11PPAC2B_diff_x = tppac->GetTDiffX(); F11PPAC2B_diff_y = tppac->GetTDiffY();
  }

  //The Focalplane:
  F3X=-9999; F3A=-9999; F3Y=-9999; F3B=-9999;
  F5X=-9999; F5A=-9999; F5Y=-9999; F5B=-9999;
  F7X=-9999; F7A=-9999; F7Y=-9999; F7B=-9999;
  F8X=-9999; F8A=-9999; F8Y=-9999; F8B=-9999;
  F9X=-9999; F9A=-9999; F9Y=-9999; F9B=-9999;
  F11X=-9999; F11A=-9999; F11Y=-9999; F11B=-9999;
  
  TArtFocalPlane *tfpl;
  TVectorD *vec;
  tfpl = cfpl->FindFocalPlane(3); 
  if(tfpl){vec=tfpl->GetOptVector(); F3X=(*vec)(0); F3A=(*vec)(1); F3Y=(*vec)(2); F3B=(*vec)(3);}
  tfpl = cfpl->FindFocalPlane(5); 
  if(tfpl){vec=tfpl->GetOptVector(); F5X=(*vec)(0); F5A=(*vec)(1); F5Y=(*vec)(2); F5B=(*vec)(3);}
  tfpl = cfpl->FindFocalPlane(7); 
  if(tfpl){vec=tfpl->GetOptVector(); F7X=(*vec)(0); F7A=(*vec)(1); F7Y=(*vec)(2); F7B=(*vec)(3);}
  tfpl = cfpl->FindFocalPlane(8); 
  if(tfpl){vec=tfpl->GetOptVector(); F8X=(*vec)(0); F8A=(*vec)(1); F8Y=(*vec)(2); F8B=(*vec)(3);}
  tfpl = cfpl->FindFocalPlane(9); 
  if(tfpl){vec=tfpl->GetOptVector(); F9X=(*vec)(0); F9A=(*vec)(1); F9Y=(*vec)(2); F9B=(*vec)(3);}
  tfpl = cfpl->FindFocalPlane(11); 
  if(tfpl){vec=tfpl->GetOptVector(); F11X=(*vec)(0); F11A=(*vec)(1); F11Y=(*vec)(2); F11B=(*vec)(3);}

  //The plastics
  F3PLA_TL_raw=-9999; F3PLA_TR_raw=-9999; F3PLA_TL=-9999; F3PLA_TR=-9999; F3PLA_T=-9999; 
  F3PLA_QL_raw=-9999; F3PLA_QR_raw=-9999; F3PLA_Q_ave=-9999; 
 
  F7PLA_TL_raw=-9999; F7PLA_TR_raw=-9999; F7PLA_TL=-9999; F7PLA_TR=-9999; F7PLA_T=-9999; 
  F7PLA_QL_raw=-9999; F7PLA_QR_raw=-9999; F7PLA_Q_ave=-9999;  
  
  F8PLA_TL_raw=-9999; F8PLA_TR_raw=-9999; F8PLA_TL=-9999; F8PLA_TR=-9999; F8PLA_T=-9999;
  F8PLA_QL_raw=-9999; F8PLA_QR_raw=-9999; F8PLA_Q_ave=-9999;
 
  F8PLA_QL_raw=-9999; F8PLA_QR_raw=-9999; F11PLA_TL=-9999; F11PLA_TR=-9999; F11PLA_T=-9999;
  F11PLA_QL_raw=-9999; F11PLA_QR_raw=-9999; F11PLA_Q_ave=-9999; 

  TArtPlastic *tpla;
  tpla = cpla->FindPlastic("F3pl");
  if(tpla){
    F3PLA_TL_raw = tpla->GetTLRaw(); F3PLA_TR_raw = tpla->GetTRRaw(); 
    F3PLA_TL = tpla->GetTimeL(); F3PLA_TR = tpla->GetTimeR(); 
    F3PLA_T = tpla->GetTime();

    F3PLA_QL_raw = tpla->GetQLRaw(); F3PLA_QR_raw = tpla->GetQRRaw();
    F3PLA_Q_ave = tpla->GetQAveRaw();
  }

  tpla = cpla->FindPlastic("F7pl");
  if(tpla){
    F7PLA_TL_raw = tpla->GetTLRaw(); F7PLA_TR_raw = tpla->GetTRRaw(); 
    F7PLA_TL = tpla->GetTimeL(); F7PLA_TR = tpla->GetTimeR(); 
    F7PLA_T = tpla->GetTime();

    F7PLA_QL_raw = tpla->GetQLRaw(); F7PLA_QR_raw = tpla->GetQRRaw(); 
    F7PLA_Q_ave = tpla->GetQAveRaw();
  }

  tpla = cpla->FindPlastic("F8pl");
  if(tpla){
    F8PLA_TL_raw = tpla->GetTLRaw(); F8PLA_TR_raw = tpla->GetTRRaw(); 
    F8PLA_TL = tpla->GetTimeL(); F8PLA_TR = tpla->GetTimeR(); 
    F8PLA_T = tpla->GetTime();

    F8PLA_QL_raw = tpla->GetQLRaw(); F8PLA_QR_raw = tpla->GetQRRaw(); 
    F8PLA_Q_ave = tpla->GetQAveRaw();
  }

  tpla = cpla->FindPlastic("F11pl");
  if(tpla){
    F11PLA_TL_raw = tpla->GetTLRaw(); F11PLA_TR_raw = tpla->GetTRRaw(); 
    F11PLA_TL = tpla->GetTimeL(); F11PLA_TR = tpla->GetTimeR(); 
    F11PLA_T = tpla->GetTime();

    F11PLA_QL_raw = tpla->GetQLRaw(); F11PLA_QR_raw = tpla->GetQRRaw(); 
    F11PLA_Q_ave = tpla->GetQAveRaw();
  }
    
  //The MUSICs
  for(int i=0;i<8;i++) F7ICRawADC[i]=-9999;
  F7ICNumHit=-9999;
  F7ICEnergySqSum=-9999;
  F7ICEnergyAvSum=-9999;
  
  for(int i=0;i<8;i++) F11ICRawADC[i]=-9999;
  F11ICNumHit=-9999;
  F11ICEnergySqSum=-9999;
  F11ICEnergyAvSum=-9999;

  TArtIC *tic;  
  tic = cic->FindIC("F7IC");
  if(tic) {
    for(int i=0;i<8;i++)F7ICRawADC[i] = tic->GetRawADC(i);
    F7ICNumHit=tic->GetNumHit();
    F7ICEnergySqSum=tic->GetEnergySqSum();
    F7ICEnergyAvSum=tic->GetEnergyAvSum();
  }

  tic = cic->FindIC("F11IC");
  if(tic) {
    for(int i=0;i<8;i++)F11ICRawADC[i] = tic->GetRawADC(i);
    F11ICNumHit=tic->GetNumHit();
    F11ICEnergySqSum=tic->GetEnergySqSum();
    F11ICEnergyAvSum=tic->GetEnergyAvSum();
  }

  //The TOF
  tof[0] = tof3to7->GetTOF();
  beta[0] = tof3to7->GetBeta();
  
  tof[1] = tof8to11->GetTOF();
  beta[1] = tof8to11->GetBeta();

  //The PID
  zet[0] = beam_br_35->GetZet();
  aoq[0] = beam_br_35->GetAoQ();
  
  zet[1] = beam_br_57->GetZet();
  aoq[1] = beam_br_57->GetAoQ();

  zet[2] = beam_zd_89->GetZet();
  aoq[2] = beam_zd_89->GetAoQ();
  
  zet[3] = beam_zd_911->GetZet();
  aoq[3] = beam_zd_911->GetAoQ();

  //Delta 
  delta[0] = rips3to5->GetDelta();
  delta[1] = rips5to7->GetDelta();
  delta[2] = rips8to9->GetDelta();
  delta[3] = rips9to11->GetDelta();

  tree->Fill();
  neve++;

  //if(neve%1000==0)
  //  std::cout << "event: " << neve << std::endl;
  
  //********************* Clear *****************
  cpid->ClearData();
  recopid->ClearData();
  rawevent->Clear();
}

void MakeOnlineBigRIPSTree::Destruct()  {
  cout<<"call Destruct() !!" <<endl;

  delete cpid;
  cpid = 0;
  delete recopid;
  recopid = 0;
  
  if(bigripsparameters)   bigripsparameters->Delete();
  bigripsparameters= 0;
}

const char* MakeOnlineBigRIPSTree::ClassName() const  {
  return "MakeOnlineBigRIPSTree";
}
