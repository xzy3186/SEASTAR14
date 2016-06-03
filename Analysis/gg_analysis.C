#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>

#include "TH2.h"
#include "TH1.h"
#include "TH1F.h"
#include "TLine.h"
#include "TFile.h"
#include "TGraph.h"
#include "TSpectrum.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TVirtualPad.h"
#include "TText.h"
#include "TMath.h"
#include "TF1.h"
#include "TRandom.h"
#include "TStyle.h"

TGraph *peakg[1];

Double_t resp1(Double_t *x, Double_t *par){
   return peakg[0]->Eval(x[0])*par[0];
}

Double_t expf1(Double_t *x, Double_t *par){
   return TMath::Exp(par[0]+par[1]*x[0])+TMath::Exp(par[2]+par[3]*x[0]);
}

Double_t ex_respf1(Double_t *x, Double_t *par){
   return resp1(x,par)+expf1(x,par+1);
}

void gg_analysis(){
   gStyle->SetOptStat(kFALSE);
   gStyle->SetPadGridX(false);
   gStyle->SetPadGridY(false);
   //gStyle->SetHistMinimumZero(1);

   const Double_t fitmin = 500;
   const Double_t fitmax = 700;

   TFile *sim;
   sim = new TFile("./geant4spec/78cu_h_77ni_610.root");
   TH2F *hsim_id_doppler;
   hsim_id_doppler = (TH2F*)sim->Get("addback_mult_4;1");
   TH1D *hsim;
   hsim = new TH1D("hsim","hsim",200,0,5000);
   hsim = (TH1D*)hsim_id_doppler->ProjectionY("hsim",1,186);
   peakg[0] = new TGraph(hsim);

   TFile *gg_exp;
   gg_exp = new TFile("./gg_test_Ni77.root");

   TH2F *hh;
   hh = (TH2F*)gg_exp->Get("h_gg;1");

   // ========= GG analysis with Fit =========

   // define fit function (gaus+pol0)
   TF1 *whole1 = new TF1( "whole1", ex_respf1,fitmin,fitmax,5);
   whole1->SetParameters(1,1.9,-0.000080,1.9,-0.000080);
   whole1->SetParLimits(0,0.0001,1000.1);
   whole1->SetParLimits(1,0,10);
   whole1->SetParLimits(2,-0.1,0);
   whole1->SetParLimits(3,0,10);
   whole1->SetParLimits(4,-0.1,0);
   whole1->SetLineColor(1);
   whole1->SetLineWidth(2);

   // prepare histogram (TGraphError is also ok)
   TH1F *hgg = new TH1F("hgg","GG spectrum gated on the first peak (fit method)",200,0.,5000.);

   // fit
   for(int bin=1;bin<=200;bin++){ // 25 keV/bin from 0 to 5000 keV
      hh->ProjectionY("htmp",bin,bin);
      htmp->Fit(whole1,"rLwwq");
      cout<<"bin = "<<bin<<", counts = "<<whole1->GetParameter(0)*50000<<endl;
      hgg->SetBinContent(bin, whole1->GetParameter(0)*50000);
      //hgg->SetBinError(bin, whole1->GetParError(0));
   }

   // draw spectrum
   TCanvas *fCanvas=new TCanvas("Canvas","77Ni",700,500);
   fCanvas->SetBorderSize(0);
   fCanvas->SetBorderMode(0);
   fCanvas->SetFrameBorderMode(0);
   fCanvas->SetFrameFillColor(0);
   fCanvas->SetBottomMargin(0.15);

   fCanvas->cd();
   hgg->Draw();
}
