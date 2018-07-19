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
   int binning = 20;
   int numBin = (maxBin-minBin)/binning;

   /********************** function definition ************************/
   //const Double_t fitmin = 150;
   //const Double_t fitmax = 2000;
   const Double_t fitmin = 400;
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
   exp[0] = new TFile("./RootOutputFiles/hist_Cu78_p2p_Ni77.root");
   //exp[0] = new TFile("./hist_test_77Ni.root");
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
      hexp[i]->GetYaxis()->SetRangeUser(0,120);

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


   TFile *sim[6];
   TH1F *hsim[6];
   TH2F *hsim_id_doppler[6];

   sim[4] = new TFile("./geant4spec/78cu_h_77ni_2720.root");
   sim[5] = new TFile("./geant4spec/78cu_h_77ni_2940.root");
   for(int i=4; i<6; i++){
      sprintf(temp,"hsim[%i]",i);
      hsim[i] = new TH1F(temp,temp,numBin,minBin,maxBin);
      hsim_id_doppler[i] = (TH2F*)sim[i]->Get("addback_mult_4;1");
      sprintf(temp,"crystal_fired_doppler_py[%i]",i);
      hsim[i] = (TH1F*)hsim_id_doppler[i]->ProjectionY(temp,daliIDMin,daliIDMax);
      peakg[i] = new TGraph(hsim[i]);
   }

   ofstream myfile;
   myfile.open("chi2.txt");

   //TFile *chi2_result = new TFile("chi2.root","RECREATE");
   //TGraph2D *gchi2 = new TGraph2D(55);
   //gchi2->SetNameTitle("chi2", "chi2");
   //int k=0;

   //To make systematic fitting to get 2D chi2 plot
   for(int i=0; i<26; i++){
   //for(int i=0; i<1; i++){
      sprintf(temp,"./geant4spec/78cu_h_77ni_%d.root",500+10*i);
      sim[0] = new TFile(temp);
      hsim[0] = new TH1F("hsim[0]","hsim[0]",numBin,minBin,maxBin);
      hsim_id_doppler[0] = (TH2F*)sim[0]->Get("addback_mult_4;1");
      hsim[0] = (TH1F*)hsim_id_doppler[0]->ProjectionY("crystal_fired_doppler_py[0]",daliIDMin,daliIDMax);
      peakg[0] = new TGraph(hsim[0]);
      //peakg[0]->Draw();
      for(int j=i+1; j<26; j++){
      //for(int j=i+1; j<2; j++){
         sprintf(temp,"./geant4spec/78cu_h_77ni_%d.root",500+10*j);
         sim[1] = new TFile(temp);
         hsim[1] = new TH1F("hsim[1]","hsim[1]",numBin,minBin,maxBin);
         hsim_id_doppler[1] = (TH2F*)sim[1]->Get("addback_mult_4;1");
         hsim[1] = (TH1F*)hsim_id_doppler[1]->ProjectionY("crystal_fired_doppler_py[1]",daliIDMin,daliIDMax);
         peakg[1] = new TGraph(hsim[1]);
         //peakg[1]->Draw("same");
         for(int k=j+1; k<26; k++){
         //for(int k=j+1; k<10; k++){
            sprintf(temp,"./geant4spec/78cu_h_77ni_%d.root",500+10*k);
            sim[2] = new TFile(temp);
            hsim[2] = new TH1F("hsim[2]","hsim[2]",numBin,minBin,maxBin);
            hsim_id_doppler[2] = (TH2F*)sim[2]->Get("addback_mult_4;1");
            hsim[2] = (TH1F*)hsim_id_doppler[2]->ProjectionY("crystal_fired_doppler_py[2]",daliIDMin,daliIDMax);
            peakg[2] = new TGraph(hsim[2]);
            for(int l=30; l<51; l++){
               sprintf(temp,"./geant4spec/78cu_h_77ni_%d.root",500+10*l);
               sim[3] = new TFile(temp);
               hsim[3] = new TH1F("hsim[3]","hsim[3]",numBin,minBin,maxBin);
               hsim_id_doppler[3] = (TH2F*)sim[3]->Get("addback_mult_4;1");
               hsim[3] = (TH1F*)hsim_id_doppler[3]->ProjectionY("crystal_fired_doppler_py[3]",daliIDMin,daliIDMax);
               peakg[3] = new TGraph(hsim[3]);

               //start fitting with the current peak configuration
               TF1 *whole1 = new TF1( "whole1", ex_respf1,fitmin,fitmax,10);
               whole1->SetParameters(1,1,1,1,1e-3,1e-3,1.9,-0.000080,1.9,-0.000080);
               whole1->SetParLimits(0,1e-9,1000.1);
               whole1->SetParLimits(1,1e-9,1000.1);
               whole1->SetParLimits(2,1e-9,1000.1);
               whole1->SetParLimits(3,1e-9,1000.1);
               whole1->SetParLimits(4,1e-9,1000.1);
               whole1->SetParLimits(5,1e-9,1000.1);
               whole1->SetParLimits(6,0,100);
               whole1->SetParLimits(7,-0.1,-1e-5);
               whole1->SetParLimits(8,0,100);
               whole1->SetParLimits(9,-0.1,-1e-5);
               //whole1->FixParameter(3,9.68641e-04);
               //whole1->FixParameter(4,2.42040e-03);
               //whole1->FixParameter(3,1.77607e-03);
               //whole1->FixParameter(4,2.55570e-03);
               whole1->SetLineColor(1);
               whole1->SetLineWidth(2);
               //whole1->SetNpx(200);

               //cout<<"i = "<<i<<", j="<<j<<", k = "<<k<<", come here"<<endl;
               //whole1->Draw();
               hexp[0]->Fit(whole1,"LR");

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
               cout<<"i = "<<i<<", j="<<j<<", k = "<<k<<", l = "<<l<<", chi2 = "<<chi2<<", chi_mlh = "<<chi_mlh<<endl;
               myfile<<i<<" "<<j<<" "<<k<<" "<<l<<" "<<chi2<<" "<<chi_mlh<<endl;
               //gchi2->SetPoint(k,500+i*10,500+j*10,chi2[i][j]);
               //k++;
               sim[3]->Close();
               whole1->Delete();
            }
            sim[2]->Close();
         }
         sim[1]->Close();
      }
      sim[0]->Close();
   }
   //fCanvas->cd();
   //gchi2->Draw("surf1");
   //chi2_result->cd();
   //fCanvas->Write();
   //gchi2->Write();
   //chi2_result->Write();
   //chi2_result->Close();
   myfile.close();
}
