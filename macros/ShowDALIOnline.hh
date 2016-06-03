#ifndef SHOWH_DALI_ONLINE_HH
#define SHOWH_DALI_ONLINE_HH

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
#include "TArtCalibDALI.hh"

#include "TArtPlastic.hh"
#include "TArtDALINaI.hh"
#include "TArtDALINaIPara.hh"


class ShowDALIOnline : public virtual TArtAnaLoop {

public:
  ShowDALIOnline();
  virtual ~ShowDALIOnline();

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
  TCanvas *cDALI;
  TH1I *h_dali_Araw;
  TH1F *h_dali_energy;
  TH2I *h_dali_Araw_id;
  TH2F *h_dali_energy_id;
  TH2F *h_dali_time_id;
  
  TGraph *gmulti0;
  TGraph *gmulti1;
  TGraph *gratio;
  
  int hogehoge;
  int allevents[10000];
  float fallevents[10000];
  int multi0[10000];
  int multi1[10000];
  float ratio_DAQ[10000];
  int scaData[32];

  //parameter
  TArtBigRIPSParameters *bigripsparameters;
  TArtDALIParameters *daliparameters;

  // calibration classes 
  TArtCalibPID *brcalib;
  TArtCalibPlastic *plasticcalib;

  TArtCalibDALI *dalicalib;

private:


public:
  virtual const char* ClassName() const;

  ClassDef(ShowDALIOnline,1);
};

#endif
