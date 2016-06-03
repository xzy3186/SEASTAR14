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

void geant4_fit(){
   gStyle->SetOptStat(kFALSE);
   gStyle->SetPadGridX(false);
   gStyle->SetPadGridY(false);

   char temp[300];
   int minBin = 0;
   int maxBin = 4000;
   int binning = 10;
   int numBin = (maxBin-minBin)/binning;

   /********************** function definition ************************/
   //const Double_t fitmin = 150;
   //const Double_t fitmax = 2000;
   const Double_t fitmin = 300;
   const Double_t fitmax = 4000;

   int daliIDMinZn = 1;
   int daliIDMaxZn = 186;

   /*******************************************************************/
   //Simulated peaks
   TFile *sim[1];
   //sim[0] = new TFile("./geant4spec/81ga_h_78zn_730.root");
   //sim[0] = new TFile("./geant4spec/78cu_h_77ni_600.root");
   sim[0] = new TFile("./geant4spec/80zn_h_75ni_1060.root");
   //sim[1] = new TFile("./geant4spec/81ga_h_78zn_890.root");
   //sim[0] = new TFile("./geant4spec/88y_double.root");

   //Experimental data
   TFile *exp[1];
   //exp[0] = new TFile("./RootOutputFiles/hist_Ga81_p2p_Zn78.root");
   //exp[0] = new TFile("./RootInputFiles/calib/hist_Y88.root");
   //exp[0] = new TFile("./RootOutputFiles/hist_Cu78_p2p_Ni77.root");
   exp[0] = new TFile("./RootOutputFiles/hist_Zn80_p2p_Ni75.root");

   TCanvas *fCanvas=new TCanvas("Canvas","78Zn",700,500);
   fCanvas->SetBorderSize(0);
   fCanvas->SetBorderMode(0);
   fCanvas->SetFrameBorderMode(0);
   fCanvas->SetFrameFillColor(0);
   fCanvas->SetBottomMargin(0.15);

   fCanvas->cd();

   /*******************************************************************/
   //Simulated spectra:
   //The hpeak is only a projection of the two-dimensional spectra
   TH1F *hsim[1];
   for(int i=0; i<1; i++){
      sprintf(temp,"hsim[%i]",i);
      hsim[i] = new TH1F(temp,temp,numBin,minBin,maxBin);
   }

   TH2F *hsim_id_doppler[1];
   for(int i=0; i<1; i++){
      //hsim_id_doppler[i] = (TH2F*)sim[i]->Get("crystal_fired_doppler;1");
      hsim_id_doppler[i] = (TH2F*)sim[i]->Get("addback_mult_4;1");
      sprintf(temp,"crystal_fired_doppler_py[%i]",i);
      hsim[i] = (TH1F*)hsim_id_doppler[i]->ProjectionY(temp,daliIDMinZn,daliIDMaxZn);
      peakg[i] = new TGraph(hsim[i]);
   }

   //The experimental spectra:
   TH1F *hexp[1];
   for(int i=0;i<1;i++) {
      sprintf(temp,"hexp[%i]",i);
      hexp[i] = new TH1F(temp,temp,numBin,minBin,maxBin);
   }

   //Only 2d spectra are saved-> have to do the projections again
   TH2F *hexp_id_doppler[1];
   for(int i=0;i<1;i++) {
      //hexp_id_doppler[i] = (TH2F*)exp[i]->Get("h_id_doppler;1");
      hexp_id_doppler[i] = (TH2F*)exp[i]->Get("h_id_addback;1");
      sprintf(temp,"h_exp_py[%i]",i);
      hexp[i] = (TH1F*)hexp_id_doppler[i]->ProjectionY(temp,daliIDMinZn,daliIDMaxZn);
   }

   for(int i=0; i<1; i++){
      hexp[i]->SetStats(0);
      hexp[i]->SetFillColor(0);
      hexp[i]->SetLineColor(1);  

      //hexp[i]->GetXaxis()->SetRangeUser(0,1500);
      hexp[i]->GetXaxis()->SetRangeUser(0,4000);
      //hexp[i]->GetYaxis()->SetRangeUser(0,3000);
      hexp[i]->GetYaxis()->SetRangeUser(0,1000);

      //hexp[i]->GetXaxis()->SetNdivisions(305);
      //hexp[i]->GetYaxis()->SetNdivisions(305);

      //How to get error bar from each bin
      hexp[i]->SetDefaultSumw2(kTRUE);
   }

   hexp[0]->SetLineColor(1);
   //hexp[0]->Draw();
   TF1 *whole1 = new TF1( "whole1", ex_respf1,fitmin,fitmax,5);
   whole1->SetParameters(1,1.9,-0.000080,1.9,-0.000080);
   whole1->SetParLimits(0,0.0001,1000.1);
   whole1->SetParLimits(1,0,10);
   whole1->SetParLimits(2,-0.1,0);
   whole1->SetParLimits(3,0,10);
   whole1->SetParLimits(4,-0.1,0);
   whole1->SetLineColor(1);
   whole1->SetLineWidth(2);
   whole1->SetNpx(200);

   cout<<"come here"<<endl;
   //whole1->Draw();
   hexp[0]->Fit(whole1,"R");

   TF1 *peak1f= new TF1( "peak1f",resp1,fitmin,fitmax,1);
   peak1f->SetParameter(0,whole1->GetParameter(0));
   peak1f->SetLineColor(2);
   peak1f->SetLineWidth(2);
   peak1f->SetLineStyle(9);
   peak1f->Draw("same");

   TF1 *expon1= new TF1( "expon1",expf1,fitmin,fitmax,4);
   expon1->SetParameters(whole1->GetParameter(1),whole1->GetParameter(2),
                         whole1->GetParameter(3),whole1->GetParameter(4));
   expon1->SetLineColor(4);
   expon1->SetLineWidth(2);
   expon1->SetLineStyle(7);
   expon1->Draw("same");
   //hexp[0]->Draw("same");
}
