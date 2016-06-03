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

TGraph *peakg[4];

Double_t resp1(Double_t *x, Double_t *par){
   return peakg[0]->Eval(x[0])*par[0];
}

Double_t resp2(Double_t *x, Double_t *par){
   return peakg[1]->Eval(x[0])*par[0];
}

Double_t resp3(Double_t *x, Double_t *par){
   return peakg[2]->Eval(x[0])*par[0];
}

Double_t resp4(Double_t *x, Double_t *par){
   return peakg[3]->Eval(x[0])*par[0];
}

Double_t resp5(Double_t *x, Double_t *par){
   return peakg[4]->Eval(x[0])*par[0];
}

Double_t resp6(Double_t *x, Double_t *par){
   return peakg[5]->Eval(x[0])*par[0];
}

Double_t resp7(Double_t *x, Double_t *par){
   return peakg[6]->Eval(x[0])*par[0];
}

Double_t expf1(Double_t *x, Double_t *par){
   return TMath::Exp(par[0]+par[1]*x[0])+TMath::Exp(par[2]+par[3]*x[0]);
}

Double_t ex_respf1(Double_t *x, Double_t *par){
   //return resp1(x,par)+resp2(x,par+1)+resp3(x,par+2)+resp4(x,par+3)+resp5(x,par+4)+resp6(x,par+5)+resp7(x,par+6)+expf1(x,par+7);
   return resp1(x,par)+resp2(x,par+1)+resp3(x,par+2)+resp4(x,par+3)+expf1(x,par+4);
}

void gg_analysis(){
   gStyle->SetOptStat(kFALSE);
   gStyle->SetPadGridX(false);
   gStyle->SetPadGridY(false);
   //gStyle->SetHistMinimumZero(1);

   const Double_t fitmin = 150;
   const Double_t fitmax = 1300;

   TFile *sim[4];
   sim[0] = new TFile("./geant4spec/81ga_h_78zn_730.root");
   sim[1] = new TFile("./geant4spec/81ga_h_78zn_890.root");
   sim[2] = new TFile("./geant4spec/81ga_h_78zn_908.root");
   sim[3] = new TFile("./geant4spec/81ga_h_78zn_576.root");
   //sim[4] = new TFile("./geant4spec/78cu_h_77ni_2880.root");
   //sim[5] = new TFile("./geant4spec/78cu_h_77ni_2650.root");
   //sim[6] = new TFile("./geant4spec/78cu_h_77ni_2880.root");

   TH2F *hsim_id_doppler[4];
   TH1D *hsim[4];
   char temp[300];
   for(int i=0; i<4; i++){
      hsim_id_doppler[i] = (TH2F*)sim[i]->Get("addback_mult_4;1");
      sprintf(temp,"hsim[%i]",i);
      hsim[i] = new TH1D(temp,temp,500,0,5000);
      sprintf(temp,"crystal_fired_doppler_py[%i]",i);
      hsim[i] = (TH1D*)hsim_id_doppler[i]->ProjectionY(temp,1,186);
      peakg[i] = new TGraph(hsim[i]);
   }

   TFile *gg_exp;
   gg_exp = new TFile("./gg_test_Zn78.root");

   TH2F *hh;
   hh = (TH2F*)gg_exp->Get("h_gg;1");

   // ========= GG analysis with Fit =========

   // define fit function (gaus+pol0)
   TF1 *whole1 = new TF1( "whole1", ex_respf1,fitmin,fitmax,8);
   // prepare histogram (TGraphError is also ok)
   TFile *gg_result = new TFile("gg_result.root","RECREATE");
   TGraph *hgg[4];
   for(int i=0; i<4; i++){
      sprintf(temp,"hgg_%i",i);
      hgg[i] = new TGraph(200);
   }

   // fit
   TH1D *htmp[200];
   for(int bin=1;bin<=200-1;bin++){ // 25 keV/bin from 0 to 5000 keV
      sprintf(temp,"htmp_%i",bin-1);
      htmp[bin] = new TH1D(temp,temp,200,0.,2000.);
      hh->ProjectionY(temp,bin,bin+1);
      if(htmp[bin]->Integral(1,200)<=10){
         for(int i=0; i<4; i++){
            //hgg[i]->SetBinContent(bin, 0);
            hgg[i]->SetPoint(bin-1, (bin-1)*10+5, 0);
         }
      }else{
         htmp[bin]->Sumw2();

         whole1->SetParameters(1,1,1,1,1.9,-0.000080,1.9,-0.000080);
         whole1->SetParLimits(0,1e-9,1000.1);
         whole1->SetParLimits(1,1e-9,1000.1);
         whole1->SetParLimits(2,1e-9,1000.1);
         whole1->SetParLimits(3,1e-9,1000.1);
         whole1->SetParLimits(4,0,100);
         whole1->SetParLimits(5,-0.1,0);
         whole1->SetParLimits(6,0,100);
         whole1->SetParLimits(7,-0.1,0);
         whole1->SetLineColor(1);
         whole1->SetLineWidth(2);

         htmp[bin]->Fit(whole1,"LRM");
         for(int i=0; i<4; i++){
            //hgg[i]->SetBinContent(bin, whole1->GetParameter(i)*50000);
            hgg[i]->SetPoint(bin-1, (bin-1)*10+5, whole1->GetParameter(i));
         }
      }
      cout<<"bin = "<<bin<<" finished."<<endl;
      //hgg->SetBinError(bin, whole1->GetParError(0));
   }

   // draw spectrum
   //TCanvas *fCanvas=new TCanvas("Canvas","77Ni",700,500);
   //fCanvas->SetBorderSize(0);
   //fCanvas->SetBorderMode(0);
   //fCanvas->SetFrameBorderMode(0);
   //fCanvas->SetFrameFillColor(0);
   //fCanvas->SetBottomMargin(0.15);
   for(int i=0; i<4; i++){
      hgg[i]->Write();
   }
   gg_result->Write();
   gg_result->Close();
   //fCanvas->cd();
   //hgg->Draw();
}
