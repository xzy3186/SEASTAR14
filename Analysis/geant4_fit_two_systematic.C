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

TGraph *peakg[5];

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

Double_t expf1(Double_t *x, Double_t *par){
   return TMath::Exp(par[0]+par[1]*x[0])+TMath::Exp(par[2]+par[3]*x[0]);
}

Double_t ex_respf1(Double_t *x, Double_t *par){
   return resp1(x,par)+resp2(x,par+1)+resp3(x,par+2)+resp4(x,par+3)+resp5(x,par+4)+expf1(x,par+5);
}

void geant4_fit(){
   gStyle->SetOptStat(kFALSE);
   gStyle->SetPadGridX(false);
   gStyle->SetPadGridY(false);

   char temp[300];
   int minBin = 0;
   int maxBin = 5000;
   int binning = 20;
   int numBin = (maxBin-minBin)/binning;

   /********************** function definition ************************/
   //const Double_t fitmin = 150;
   //const Double_t fitmax = 2000;
   const Double_t fitmin = 1500;
   const Double_t fitmax = 5000;

   int daliIDMin = 1;
   int daliIDMax = 186;

   /*******************************************************************/
   //Simulated peaks
   //sim[0] = new TFile("./geant4spec/78cu_h_77ni_540.root");
   //sim[1] = new TFile("./geant4spec/78cu_h_77ni_630.root");
   //sim[0] = new TFile("./geant4spec/88y_898.root");
   //sim[1] = new TFile("./geant4spec/88y_1836.root");

   //Experimental data
   TFile *exp[1];
   //exp[0] = new TFile("./RootOutputFiles/hist_Ga81_p2p_Zn78.root");
   //exp[0] = new TFile("./RootOutputFiles/hist_Cu78_p2p_Ni77.root");
   exp[0] = new TFile("./hist_test_77Ni.root");
   //exp[0] = new TFile("./RootInputFiles/calib/hist_Y88.root");

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
      hexp[i] = (TH1F*)hexp_id_doppler[i]->ProjectionY(temp,daliIDMin,daliIDMax);
   }

   for(int i=0; i<1; i++){
      hexp[i]->SetStats(0);
      hexp[i]->SetFillColor(0);
      hexp[i]->SetLineColor(1);  

      //hexp[i]->GetXaxis()->SetRangeUser(0,1500);
      hexp[i]->GetXaxis()->SetRangeUser(0,5000);
      //hexp[i]->GetYaxis()->SetRangeUser(0,3000);
      hexp[i]->GetYaxis()->SetRangeUser(0,200);

      //hexp[i]->GetXaxis()->SetNdivisions(305);
      //hexp[i]->GetYaxis()->SetNdivisions(305);

      //How to get error bar from each bin
      hexp[i]->SetDefaultSumw2(kTRUE);
   }
   hexp[0]->SetLineColor(1);

   TCanvas *fCanvas=new TCanvas("chi2","chi2",700,500);
   fCanvas->SetBorderSize(0);
   fCanvas->SetBorderMode(0);
   fCanvas->SetFrameBorderMode(0);
   fCanvas->SetFrameFillColor(0);
   fCanvas->SetBottomMargin(0.15);


   TFile *sim[5];
   TH1F *hsim[5];
   TH2F *hsim_id_doppler[5];

   sim[2] = new TFile("./geant4spec/78cu_h_77ni_2880.root");
   sim[3] = new TFile("./geant4spec/78cu_h_77ni_680.root");
   sim[4] = new TFile("./geant4spec/78cu_h_77ni_940.root");
   for(int i=2; i<5; i++){
      sprintf(temp,"hsim[%i]",i);
      hsim[i] = new TH1F(temp,temp,numBin,minBin,maxBin);
      hsim_id_doppler[i] = (TH2F*)sim[i]->Get("addback_mult_4;1");
      sprintf(temp,"crystal_fired_doppler_py[%i]",i);
      hsim[i] = (TH1F*)hsim_id_doppler[i]->ProjectionY(temp,daliIDMin,daliIDMax);
      peakg[i] = new TGraph(hsim[i]);
   }

   TF1 *whole1 = new TF1( "whole1", ex_respf1,fitmin,fitmax,9);
   ofstream myfile;
   myfile.open("chi2.txt");

   TFile *chi2_result = new TFile("chi2.root","RECREATE");
   TGraph2D *gchi2 = new TGraph2D(1275);
   gchi2->SetNameTitle("chi2", "chi2");
   int k=0;

   //To make systematic fitting to get 2D chi2 plot
   for(int i=0; i<44; i++){
      sprintf(temp,"./geant4spec/78cu_h_77ni_%d.root",2000+20*i);
      sim[0] = new TFile(temp);
      hsim[0] = new TH1F("hsim[0]","hsim[0]",numBin,minBin,maxBin);
      hsim_id_doppler[0] = (TH2F*)sim[0]->Get("addback_mult_4;1");
      hsim[0] = (TH1F*)hsim_id_doppler[0]->ProjectionY("crystal_fired_doppler_py[0]",daliIDMin,daliIDMax);
      peakg[0] = new TGraph(hsim[0]);
      //peakg[0]->Draw();
      for(int j=i+1; j<44; j++){
         sprintf(temp,"./geant4spec/78cu_h_77ni_%d.root",2000+20*j);
         sim[1] = new TFile(temp);
         hsim[1] = new TH1F("hsim[1]","hsim[1]",numBin,minBin,maxBin);
         hsim_id_doppler[1] = (TH2F*)sim[1]->Get("addback_mult_4;1");
         hsim[1] = (TH1F*)hsim_id_doppler[1]->ProjectionY("crystal_fired_doppler_py[1]",daliIDMin,daliIDMax);
         peakg[1] = new TGraph(hsim[1]);
         //peakg[1]->Draw("same");

         //start fitting with the current peak configuration
         whole1->SetParameters(1,1,1,1,1,1.9,-0.000080,1.9,-0.000080);
         //whole1->SetParLimits(0,1e-9,1000.1);
         //whole1->SetParLimits(1,1e-9,1000.1);
         whole1->SetParLimits(0,1e-9,1000.1);
         whole1->SetParLimits(1,1e-9,1000.1);
         whole1->SetParLimits(2,1e-9,1000.1);
         whole1->SetParLimits(5,0,10);
         whole1->SetParLimits(6,-0.1,-1e-5);
         whole1->SetParLimits(7,0,10);
         whole1->SetParLimits(8,-0.1,-1e-5);
         //whole1->FixParameter(2,2.68843e-03);
         whole1->FixParameter(3,1.95106e-03);
         whole1->FixParameter(4,9.54702e-04);
         //whole1->FixParameter(5,0);
         //whole1->FixParameter(6,0);
         whole1->SetLineColor(1);
         whole1->SetLineWidth(2);
         whole1->SetNpx(200);

         //whole1->Draw();
         hexp[0]->Fit(whole1,"LLR");

         //TF1 *peak1f= new TF1( "peak1f",resp1,fitmin,fitmax,1);
         //peak1f->SetParameter(0,whole1->GetParameter(0));
         //peak1f->SetLineColor(2);
         //peak1f->SetLineWidth(2);
         //peak1f->SetLineStyle(9);
         //peak1f->Draw("same");

         //TF1 *peak2f= new TF1( "peak1f",resp2,fitmin,fitmax,1);
         //peak2f->SetParameter(0,whole1->GetParameter(1));
         //peak2f->SetLineColor(2);
         //peak2f->SetLineWidth(2);
         //peak2f->SetLineStyle(9);
         //peak2f->Draw("same");

         //TF1 *expon1= new TF1( "expon1",expf1,fitmin,fitmax,4);
         //expon1->SetParameters(whole1->GetParameter(2),whole1->GetParameter(3),
         //      whole1->GetParameter(4),whole1->GetParameter(5));
         //expon1->SetLineColor(4);
         //expon1->SetLineWidth(2);
         //expon1->SetLineStyle(7);
         //expon1->Draw("same");

         double chi2 = whole1->GetChisquare()/whole1->GetNDF();
         //myfile<<chi2[i][j];
         gchi2->SetPoint(k,2000+i*20,2000+j*20,chi2);
         k++;
         double chi_mlh=0;
         for(int bin=1; bin<=numBin; bin++){
            float bin_center = hexp[0]->GetXaxis()->GetBinCenter(bin);
            if(bin_center>fitmin && bin_center<fitmax){
               int n_bin = hexp[0]->GetBinContent(bin);
               double y_bin = whole1->Eval(bin_center);
               if(n_bin>0){
                  chi_mlh=chi_mlh+(y_bin-n_bin+n_bin*log(n_bin/y_bin));
               }else if(n_bin==0){
                  chi_mlh=chi_mlh+(y_bin-n_bin);
               }
            }
         }
         chi_mlh=chi_mlh*2/whole1->GetNDF();
         cout<<"i = "<<i<<", j="<<j<<", chi2 = "<<chi2<<", chi_mlh = "<<chi_mlh<<endl;
         myfile<<i<<" "<<j<<" "<<chi2<<" "<<chi_mlh<<endl;
         sim[1]->Close();
      }
      //myfile<<endl;
      sim[0]->Close();
   }
   fCanvas->cd();
   gchi2->Draw("surf1");
   chi2_result->cd();
   fCanvas->Write();
   //gchi2->Write();
   //chi2_result->Write();
   chi2_result->Close();
   myfile.close();
}
