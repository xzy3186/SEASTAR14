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

TGraph *peakg[6];

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

Double_t expf1(Double_t *x, Double_t *par){
   return TMath::Exp(par[0]+par[1]*x[0])+TMath::Exp(par[2]+par[3]*x[0]);
}

Double_t ex_respf1(Double_t *x, Double_t *par){
   return resp1(x,par)+resp2(x,par+1)+resp3(x,par+2)+resp4(x,par+3)+resp5(x,par+4)+resp6(x,par+5)+expf1(x,par+6);
}

void geant4_fit(){
   gStyle->SetOptStat(kFALSE);
   gStyle->SetPadGridX(false);
   gStyle->SetPadGridY(false);

   char temp[300];
   int minBin = 0;
   int maxBin = 5000;
   int binning = 25;
   int numBin = (maxBin-minBin)/binning;

   /********************** function definition ************************/
   //const Double_t fitmin = 150;
   //const Double_t fitmax = 2000;
   const Double_t fitmin = 250;
   const Double_t fitmax = 5000;

   int daliIDMin = 1;
   int daliIDMax = 186;

   /*******************************************************************/
   //Simulated peaks
   TFile *sim[6];
   //sim[0] = new TFile("./geant4spec/81ga_h_78zn_730.root");
   //sim[1] = new TFile("./geant4spec/81ga_h_78zn_890.root");
   sim[0] = new TFile("./geant4spec/78cu_h_77ni_540.root");
   sim[1] = new TFile("./geant4spec/78cu_h_77ni_610.root");
   sim[2] = new TFile("./geant4spec/78cu_h_77ni_690.root");
   sim[3] = new TFile("./geant4spec/78cu_h_77ni_920.root");
   sim[4] = new TFile("./geant4spec/78cu_h_77ni_2720.root");
   sim[5] = new TFile("./geant4spec/78cu_h_77ni_2940.root");
   //sim[0] = new TFile("./geant4spec/88y_898.root");
   //sim[1] = new TFile("./geant4spec/88y_1836.root");

   //Experimental data
   TFile *exp[1];
   //exp[0] = new TFile("./RootOutputFiles/hist_Ga81_p2p_Zn78.root");
   //exp[0] = new TFile("./hist_test_77Ni.root");
   exp[0] = new TFile("./RootOutputFiles/hist_Cu78_p2p_Ni77.root");
   //exp[0] = new TFile("./RootInputFiles/calib/hist_Y88.root");

   TCanvas *fCanvas=new TCanvas("Canvas","Canvas",700,500);
   fCanvas->SetBorderSize(0);
   fCanvas->SetBorderMode(0);
   fCanvas->SetFrameBorderMode(0);
   fCanvas->SetFrameFillColor(0);
   fCanvas->SetBottomMargin(0.15);

   fCanvas->cd();

   /*******************************************************************/
   //Simulated spectra:
   //The hpeak is only a projection of the two-dimensional spectra
   TH1F *hsim[6];
   for(int i=0; i<6; i++){
      sprintf(temp,"hsim[%i]",i);
      hsim[i] = new TH1F(temp,temp,numBin,minBin,maxBin);
   }

   TH2F *hsim_id_doppler[6];
   for(int i=0; i<6; i++){
      //hsim_id_doppler[i] = (TH2F*)sim[i]->Get("crystal_fired_doppler;1");
      hsim_id_doppler[i] = (TH2F*)sim[i]->Get("addback_mult_4;1");
      sprintf(temp,"crystal_fired_doppler_py[%i]",i);
      hsim[i] = (TH1F*)hsim_id_doppler[i]->ProjectionY(temp,daliIDMin,daliIDMax);
      peakg[i] = new TGraph(hsim[i]);
   }
   //peakg[1]->Draw();

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
      sprintf(temp,"h_exp_py_%i",i);
      hexp[i] = (TH1F*)hexp_id_doppler[i]->ProjectionY(temp,daliIDMin,daliIDMax);
      //hexp[i]->Sumw2();
   }

   for(int i=0; i<1; i++){
      hexp[i]->SetStats(0);
      hexp[i]->SetFillColor(0);
      hexp[i]->SetLineColor(1);  

      //hexp[i]->GetXaxis()->SetRangeUser(0,1500);
      hexp[i]->GetXaxis()->SetRangeUser(0,5000);
      //hexp[i]->GetYaxis()->SetRangeUser(0,3000);
      hexp[i]->GetYaxis()->SetRangeUser(1,60);

      //hexp[i]->GetXaxis()->SetNdivisions(305);
      //hexp[i]->GetYaxis()->SetNdivisions(305);

      //How to get error bar from each bin
      hexp[i]->SetDefaultSumw2(kTRUE);
   }

   hexp[0]->SetLineColor(1);
   //hexp[0]->Draw();
   TF1 *whole1 = new TF1( "whole1", ex_respf1,fitmin,fitmax,10);
   whole1->SetParameters(1,1,1,1,1,1,1.9,-0.000080,3.02128,-1.61704e-3);
   whole1->SetParLimits(0,0.0001,1000.1);
   whole1->SetParLimits(1,0.0001,1000.1);
   whole1->SetParLimits(2,0.0001,1000.1);
   whole1->SetParLimits(3,0.0001,1000.1);
   whole1->SetParLimits(4,0.0001,1000.1);
   whole1->SetParLimits(5,0.0001,1000.1);
   //whole1->SetParLimits(6,0,100);
   //whole1->SetParLimits(7,-0.1,0);
   //whole1->SetParLimits(8,0,100);
   //whole1->SetParLimits(9,-0.1,0);
   whole1->FixParameter(6,6.25275);
   whole1->FixParameter(7,-9.39665e-3);
   whole1->FixParameter(8,2.60897);
   whole1->FixParameter(9,-5.11620e-4);
   whole1->SetLineColor(1);
   whole1->SetLineWidth(2);
   whole1->SetNpx(200);

   cout<<"come here"<<endl;
   //whole1->Draw();
   hexp[0]->Draw();
   hexp[0]->Fit(whole1,"LRW");

   TF1 *peak1f= new TF1( "peak1f",resp1,fitmin,fitmax,1);
   peak1f->SetParameter(0,whole1->GetParameter(0));
   peak1f->SetLineColor(2);
   peak1f->SetLineWidth(2);
   peak1f->SetLineStyle(9);
   peak1f->Draw("same");

   TF1 *peak2f= new TF1( "peak2f",resp2,fitmin,fitmax,1);
   peak2f->SetParameter(0,whole1->GetParameter(1));
   peak2f->SetLineColor(2);
   peak2f->SetLineWidth(2);
   peak2f->SetLineStyle(9);
   peak2f->Draw("same");

   TF1 *peak3f= new TF1( "peak3f",resp3,fitmin,fitmax,1);
   peak3f->SetParameter(0,whole1->GetParameter(2));
   peak3f->SetLineColor(2);
   peak3f->SetLineWidth(2);
   peak3f->SetLineStyle(9);
   peak3f->Draw("same");

   TF1 *peak4f= new TF1( "peak4f",resp4,fitmin,fitmax,1);
   peak4f->SetParameter(0,whole1->GetParameter(3));
   peak4f->SetLineColor(2);
   peak4f->SetLineWidth(2);
   peak4f->SetLineStyle(9);
   peak4f->Draw("same");

   TF1 *peak5f= new TF1( "peak5f",resp5,fitmin,fitmax,1);
   peak5f->SetParameter(0,whole1->GetParameter(4));
   peak5f->SetLineColor(2);
   peak5f->SetLineWidth(2);
   peak5f->SetLineStyle(9);
   peak5f->Draw("same");

   TF1 *peak6f= new TF1( "peak6f",resp6,fitmin,fitmax,1);
   peak6f->SetParameter(0,whole1->GetParameter(5));
   peak6f->SetLineColor(2);
   peak6f->SetLineWidth(2);
   peak6f->SetLineStyle(9);
   peak6f->Draw("same");

   TF1 *expon1= new TF1( "expon1",expf1,fitmin,fitmax,4);
   expon1->SetParameters(whole1->GetParameter(6),whole1->GetParameter(7),
                         whole1->GetParameter(8),whole1->GetParameter(9));
   expon1->SetLineColor(4);
   expon1->SetLineWidth(2);
   expon1->SetLineStyle(7);
   expon1->Draw("same");
   //hexp[0]->Draw("same");
   cout<<"Chi2/NDF = "<<whole1->GetChisquare()/(numBin-10)<<endl;
}
