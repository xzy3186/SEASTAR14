#ifndef MAKE_ONLINE_BIGRIPS_TREE_HH
#define MAKE_ONLINE_BIGRIPS_TREE_HH

#include "TArtAnaLoop.hh"
#include "TArtStoreManager.hh"
#include "TArtCore.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TGraph.h"
#include "TTree.h"

#include "TArtEventInfo.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"

#include "TArtCalibIC.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibFocalPlane.hh"

#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"

class MakeOnlineBigRIPSTree : public virtual TArtAnaLoop {

public:
  MakeOnlineBigRIPSTree();
  virtual ~MakeOnlineBigRIPSTree();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TFile *fout;
  TTree *tree;
  int neve;

  TArtStoreManager * sman;
  //  TArtEventStore *estore;
  //  TArtRawEventObject *rawevent;
    
  //parameter
  TArtBigRIPSParameters *bigripsparameters;
  //TArtDALIParameters *daliparameters;
  
  // calibration classes
  TArtCalibPID *cpid; 
  TArtCalibPPAC *cppac;
  TArtCalibPlastic *cpla;
  TArtCalibIC *cic;
  TArtCalibFocalPlane *cfpl;

  //TArtCalibDALI *cdali;
  
  TArtRecoPID *recopid;

  TArtRIPS *rips3to5;
  TArtRIPS *rips5to7;
  TArtRIPS *rips8to9;
  TArtRIPS *rips9to11;

  TArtTOF *tof3to7;
  TArtTOF *tof8to11;

  TArtBeam *beam_br_35;
  TArtBeam *beam_br_57;
  TArtBeam *beam_zd_89;
  TArtBeam *beam_zd_911;

  Int_t fbit;

  Double_t F3PPAC1A_X;
  Double_t F3PPAC1A_Y;
  Double_t F3PPAC1B_X;
  Double_t F3PPAC1B_Y;
  Double_t F3PPAC2A_X;
  Double_t F3PPAC2A_Y;
  Double_t F3PPAC2B_X;
  Double_t F3PPAC2B_Y;

  Double_t F3PPAC1A_sum_x;
  Double_t F3PPAC1A_sum_y;
  Double_t F3PPAC1B_sum_x;
  Double_t F3PPAC1B_sum_y;
  Double_t F3PPAC2A_sum_x;
  Double_t F3PPAC2A_sum_y;
  Double_t F3PPAC2B_sum_x;
  Double_t F3PPAC2B_sum_y;

  Double_t F3PPAC1A_diff_x;
  Double_t F3PPAC1A_diff_y;
  Double_t F3PPAC1B_diff_x;
  Double_t F3PPAC1B_diff_y;
  Double_t F3PPAC2A_diff_x;
  Double_t F3PPAC2A_diff_y;
  Double_t F3PPAC2B_diff_x;
  Double_t F3PPAC2B_diff_y;

  Double_t F3X;
  Double_t F3A;
  Double_t F3Y;
  Double_t F3B;

  Double_t F3PLA_TL_raw;
  Double_t F3PLA_TR_raw;
  Double_t F3PLA_TL;
  Double_t F3PLA_TR;
  Double_t F3PLA_T;

  Int_t F3PLA_QL_raw;   
  Int_t F3PLA_QR_raw; 
  Int_t F3PLA_Q_ave;    

  Double_t F5PPAC1A_X;
  Double_t F5PPAC1A_Y;
  Double_t F5PPAC1B_X;
  Double_t F5PPAC1B_Y;
  Double_t F5PPAC2A_X;
  Double_t F5PPAC2A_Y;
  Double_t F5PPAC2B_X;
  Double_t F5PPAC2B_Y;

  Double_t F5PPAC1A_sum_x;
  Double_t F5PPAC1A_sum_y;
  Double_t F5PPAC1B_sum_x;
  Double_t F5PPAC1B_sum_y;
  Double_t F5PPAC2A_sum_x;
  Double_t F5PPAC2A_sum_y;
  Double_t F5PPAC2B_sum_x;
  Double_t F5PPAC2B_sum_y;

  Double_t F5PPAC1A_diff_x;
  Double_t F5PPAC1A_diff_y;
  Double_t F5PPAC1B_diff_x;
  Double_t F5PPAC1B_diff_y;
  Double_t F5PPAC2A_diff_x;
  Double_t F5PPAC2A_diff_y;
  Double_t F5PPAC2B_diff_x;
  Double_t F5PPAC2B_diff_y;

  Double_t F5X;
  Double_t F5A;
  Double_t F5Y;
  Double_t F5B;

  Double_t F7PPAC1A_X;
  Double_t F7PPAC1A_Y;
  Double_t F7PPAC1B_X;
  Double_t F7PPAC1B_Y;
  Double_t F7PPAC2A_X;
  Double_t F7PPAC2A_Y;
  Double_t F7PPAC2B_X;
  Double_t F7PPAC2B_Y;

  Double_t F7PPAC1A_sum_x;
  Double_t F7PPAC1A_sum_y;
  Double_t F7PPAC1B_sum_x;
  Double_t F7PPAC1B_sum_y;
  Double_t F7PPAC2A_sum_x;
  Double_t F7PPAC2A_sum_y;
  Double_t F7PPAC2B_sum_x;
  Double_t F7PPAC2B_sum_y;

  Double_t F7PPAC1A_diff_x;
  Double_t F7PPAC1A_diff_y;
  Double_t F7PPAC1B_diff_x;
  Double_t F7PPAC1B_diff_y;
  Double_t F7PPAC2A_diff_x;
  Double_t F7PPAC2A_diff_y;
  Double_t F7PPAC2B_diff_x;
  Double_t F7PPAC2B_diff_y;

  Double_t F7X;
  Double_t F7A;
  Double_t F7Y;
  Double_t F7B;

  Double_t F7PLA_TL_raw;
  Double_t F7PLA_TR_raw;
  Double_t F7PLA_TL;
  Double_t F7PLA_TR;
  Double_t F7PLA_T;

  Int_t F7PLA_QL_raw;   
  Int_t F7PLA_QR_raw; 
  Int_t F7PLA_Q_ave; 
  
  Double_t F8PPAC1A_X;
  Double_t F8PPAC1A_Y;
  Double_t F8PPAC1B_X;
  Double_t F8PPAC1B_Y;
  Double_t F8PPAC2A_X;
  Double_t F8PPAC2A_Y;
  Double_t F8PPAC2B_X;
  Double_t F8PPAC2B_Y;
  Double_t F8PPAC3A_X;
  Double_t F8PPAC3A_Y;
  Double_t F8PPAC3B_X;
  Double_t F8PPAC3B_Y;
  
  Double_t F8PPAC1A_sum_x;
  Double_t F8PPAC1A_sum_y;
  Double_t F8PPAC1B_sum_x;
  Double_t F8PPAC1B_sum_y;
  Double_t F8PPAC2A_sum_x;
  Double_t F8PPAC2A_sum_y;
  Double_t F8PPAC2B_sum_x;
  Double_t F8PPAC2B_sum_y;
  Double_t F8PPAC3A_sum_x;
  Double_t F8PPAC3A_sum_y;
  Double_t F8PPAC3B_sum_x;
  Double_t F8PPAC3B_sum_y;

  Double_t F8PPAC1A_diff_x;
  Double_t F8PPAC1A_diff_y;
  Double_t F8PPAC1B_diff_x;
  Double_t F8PPAC1B_diff_y;
  Double_t F8PPAC2A_diff_x;
  Double_t F8PPAC2A_diff_y;
  Double_t F8PPAC2B_diff_x;
  Double_t F8PPAC2B_diff_y;
  Double_t F8PPAC3A_diff_x;
  Double_t F8PPAC3A_diff_y;
  Double_t F8PPAC3B_diff_x;
  Double_t F8PPAC3B_diff_y;

  Double_t F8X;
  Double_t F8A;
  Double_t F8Y;
  Double_t F8B;

  Double_t F8PLA_TL_raw;
  Double_t F8PLA_TR_raw;
  Double_t F8PLA_TL;
  Double_t F8PLA_TR;
  Double_t F8PLA_T;

  Int_t F8PLA_QL_raw;   
  Int_t F8PLA_QR_raw; 
  Int_t F8PLA_Q_ave; 

  Double_t F9PPAC1A_X;
  Double_t F9PPAC1A_Y;
  Double_t F9PPAC1B_X;
  Double_t F9PPAC1B_Y;
  Double_t F9PPAC2A_X;
  Double_t F9PPAC2A_Y;
  Double_t F9PPAC2B_X;
  Double_t F9PPAC2B_Y;

  Double_t F9PPAC1A_sum_x;
  Double_t F9PPAC1A_sum_y;
  Double_t F9PPAC1B_sum_x;
  Double_t F9PPAC1B_sum_y;
  Double_t F9PPAC2A_sum_x;
  Double_t F9PPAC2A_sum_y;
  Double_t F9PPAC2B_sum_x;
  Double_t F9PPAC2B_sum_y;

  Double_t F9PPAC1A_diff_x;
  Double_t F9PPAC1A_diff_y;
  Double_t F9PPAC1B_diff_x;
  Double_t F9PPAC1B_diff_y;
  Double_t F9PPAC2A_diff_x;
  Double_t F9PPAC2A_diff_y;
  Double_t F9PPAC2B_diff_x;
  Double_t F9PPAC2B_diff_y;

  Double_t F9X;
  Double_t F9A;
  Double_t F9Y;
  Double_t F9B;

  Double_t F11PPAC1A_X;
  Double_t F11PPAC1A_Y;
  Double_t F11PPAC1B_X;
  Double_t F11PPAC1B_Y;
  Double_t F11PPAC2A_X;
  Double_t F11PPAC2A_Y;
  Double_t F11PPAC2B_X;
  Double_t F11PPAC2B_Y;

  Double_t F11PPAC1A_sum_x;
  Double_t F11PPAC1A_sum_y;
  Double_t F11PPAC1B_sum_x;
  Double_t F11PPAC1B_sum_y;
  Double_t F11PPAC2A_sum_x;
  Double_t F11PPAC2A_sum_y;
  Double_t F11PPAC2B_sum_x;
  Double_t F11PPAC2B_sum_y;

  Double_t F11PPAC1A_diff_x;
  Double_t F11PPAC1A_diff_y;
  Double_t F11PPAC1B_diff_x;
  Double_t F11PPAC1B_diff_y;
  Double_t F11PPAC2A_diff_x;
  Double_t F11PPAC2A_diff_y;
  Double_t F11PPAC2B_diff_x;
  Double_t F11PPAC2B_diff_y;

  Double_t F11X;
  Double_t F11A;
  Double_t F11Y;
  Double_t F11B;

  Double_t F11PLA_TL_raw;
  Double_t F11PLA_TR_raw;
  Double_t F11PLA_TL;
  Double_t F11PLA_TR;
  Double_t F11PLA_T;

  Int_t F11PLA_QL_raw;   
  Int_t F11PLA_QR_raw; 
  Int_t F11PLA_Q_ave;

  Int_t F7ICRawADC[8];
  Int_t F7ICNumHit;
  Double_t F7ICEnergySqSum;
  Double_t F7ICEnergyAvSum;
    
  Int_t F11ICRawADC[8];
  Int_t F11ICNumHit;
  Double_t F11ICEnergySqSum;
  Double_t F11ICEnergyAvSum;

  //Double_t tof37;
  //Double_t beta37;
  
  //Double_t tof811;
  //Double_t beta811;

  Double_t tof[2];
  Double_t beta[2];

  //Double_t z_35;
  //Double_t aoq_35;

  //Double_t z_57;
  //Double_t aoq_57;

  //Double_t z_89;
  //Double_t aoq_89;

  //Double_t z_911;
  //Double_t aoq_911;

  Double_t zet[4];
  Double_t aoq[4];

  //Double_t delta_35;
  //Double_t delta_57;
  //Double_t delta_89;
  //Double_t delta_911;
  Double_t delta[4];

  Double_t tkeRawADC[10];


private:
  
public:
  virtual const char* ClassName() const;

  ClassDef(MakeOnlineBigRIPSTree,1);
};

#endif
