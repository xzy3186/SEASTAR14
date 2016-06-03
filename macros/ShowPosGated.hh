#ifndef SHOW_POSGATED_HH
#define SHOW_POSGATED_HH

#include "TArtAnaLoop.hh"
#include "TArtStoreManager.hh"
#include "TArtCore.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TGraph.h"
#include "TTree.h"

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

class ShowPosGated : public virtual TArtAnaLoop {

public:
  ShowPosGated();
  virtual ~ShowPosGated();

protected:
  virtual void Construct();
  virtual void Calculate();
  virtual void Destruct();

  TTree *tree;
  int neve;

  TArtStoreManager * sman;
  //  TArtEventStore *estore;
  //  TArtRawEventObject *rawevent;

  //For Online
  TCanvas *cPos;

  TH2F *h_br_aoq35_zed;
  TH2F *h_br_aoq57_zed;
  
  TH2F *h_zd_aoq89_zed;
  TH2F *h_zd_aoq911_zed;

  TH2F *h_zd_aoq89_zed_gated;
  TH2F *h_zd_aoq911_zed_gated;

  TH2F *h_F3X_aoq35_gated;
  TH2F *h_F5X_aoq35_gated;
  
  TH2F *h_F8X_aoq911_gated;
  TH2F *h_F9X_aoq911_gated;
  TH2F *h_F11X_aoq911_gated;

  TH2F *h_F5X_F9X_gated;
  
  TH2F *h_F3X_F3A_gated;
  TH2F *h_F5X_F5A_gated;
  TH2F *h_F7X_F7A_gated;
  TH2F *h_F8X_F8A_gated;
  TH2F *h_F9X_F9A_gated;
  TH2F *h_F11X_F11A_gated;

  //PID cuts:
  TFile *BRcut[3];
  TFile *ZDcut[3];

  TCutG *brcut[3];
  TCutG *zdcut[3]; 
    
  //parameter
  TArtBigRIPSParameters *bigripsparameters;
  //TArtDALIParameters *daliparameters;
  
  // calibration classes
  TArtCalibPID *brcalib; 
  TArtCalibPPAC *ppaccalib;
  TArtCalibPlastic *plasticcalib;
  TArtCalibFocalPlane *focalcalib;

  //TArtCalibDALI *dalicalib;
  
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

private:
  
public:
  virtual const char* ClassName() const;

  ClassDef(ShowPosGated,1);
};

#endif
