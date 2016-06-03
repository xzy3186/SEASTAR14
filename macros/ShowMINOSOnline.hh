#ifndef SHOWH_MINOS_ONLINE_HH
#define SHOWH_MINOS_ONLINE_HH

#include "TArtAnaLoop.hh"
#include "TArtStoreManager.hh"
#include "TArtCore.hh"

#include "TROOT.h"
#include "TH2.h"
#include "TH1.h"
#include "TH2Poly.h"
#include "TGraph.h"
#include "TTree.h"


#include "TArtCalibMINOS.hh"
#include "TArtCalibMINOSData.hh"
#include "TArtAnalyzedMINOS.hh"
#include "TArtTrackMINOS.hh"
#include "TArtVertexMINOS.hh"

//class TArtMINOSParameters;
//class TArtCalibMINOS;

class ShowMINOSOnline : public virtual TArtAnaLoop {

public:
  ShowMINOSOnline();
  virtual ~ShowMINOSOnline();

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
  TCanvas *cMINOS;  
  TH2Poly *hpoly; 
  TH2F *hrt;
  TH1I *hasic;
  TH1I *hasic1;
  TH1I *hasicmax;
  TH1I *htest;
  TH2Poly *hpoly1; 
  TH2F *hQt1;
  TH2F *hxyvertex;
  TH1F *hzvertex;

  //parameter
  TArtMINOSParameters *minosparameters;
  // calibration classes
  TArtCalibMINOS *CalibMINOS;
  TArtAnalyzedMINOS *AnalyzedMINOS;

  TArtTrackMINOS *TrackMINOS;
  TArtVertexMINOS *VertexMINOS;

private:


public:
  virtual const char* ClassName() const;

  ClassDef(ShowMINOSOnline,1);
};

#endif
