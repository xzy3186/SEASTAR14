#ifndef SHOWH_BR_PPAC_EFFICIENCY_HH
#define SHOWH_BR_PPAC_EFFICIENCY_HH

#include "TArtAnaLoop.hh"
#include "TArtStoreManager.hh"
#include "TArtCore.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TGraph.h"
#include "TTree.h"

#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"

#include "TArtCalibPID.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibDALI.hh"

#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"

class ShowBRPPACEfficiency : public virtual TArtAnaLoop {

public:
  ShowBRPPACEfficiency();
  virtual ~ShowBRPPACEfficiency();

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
  TCanvas *cBRPPAC;
  TCanvas *cAOQ;

  TH1F *h_br_zed;
  TH2F *h_br_aoq_zed;

  TH2F *h_br_zed_dummy;  //spectrum to divide Z
  TH2F *h_br_zed_ppac_x;
  TH2F *h_br_zed_ppac_y;

  TH2F *h_br_sum_y;
  TH2F *h_br_sum_x;

  TH2F *h_br_ppac_eff_x;
  TH2F *h_br_ppac_eff_y;
  
  //parameter
  TArtBigRIPSParameters *bigripsparameters;
  //TArtDALIParameters *daliparameters;
  
  // calibration classes
  TArtCalibPID *brcalib; 
  TArtCalibPPAC *ppaccalib;
  TArtCalibPlastic *plasticcalib;

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

  ClassDef(ShowBRPPACEfficiency,1);
};

#endif
