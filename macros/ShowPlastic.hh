#ifndef SHOW_PLASTIC_HH
#define SHOW_PLASTIC_HH

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
#include "TArtCalibPlastic.hh"
#include "TArtCalibIC.hh"

#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"

class ShowPlastic : public virtual TArtAnaLoop {

public:
  ShowPlastic();
  virtual ~ShowPlastic();

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
  TCanvas *cPlastic;
  
  TH2F * h_time_l_raw;
  TH2F * h_time_r_raw;

  TH2F * h_time_l;
  TH2F * h_time_r;

  TH2F * h_time;

  TH2F * h_ql_raw;
  TH2F * h_qr_raw;

  TH2F * h_q_ave_raw;

  TH1F *h_tof_br;
  TH1F *h_tof_zd;

  TH2F *h_tof_br_aoq35;
  TH2F *h_tof_br_aoq57;
  TH2F *h_tof_zd_aoq89;
  TH2F *h_tof_zd_aoq911;

  //parameter
  TArtBigRIPSParameters *bigripsparameters;
  //TArtDALIParameters *daliparameters;
  
  // calibration classes 
  TArtCalibPID *brcalib;
  TArtCalibIC *iccalib;
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

  ClassDef(ShowPlastic,1);
};
#endif
