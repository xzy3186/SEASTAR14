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

#define numberOfDetectors 186

FILE *fCalibrationResult;
double PeakPositions[2][numberOfDetectors] = {{0.0}};
double PeakErrors[2][numberOfDetectors] = {{0.0}};
TFile* inFile;
TFile* outFile;
TH2F* h2;
TCanvas *fCanvas = new TCanvas("Canvas", "Canvas", 600, 400);
TH1F* h[numberOfDetectors];
TH1F* h_new[numberOfDetectors];
int if_fit[numberOfDetectors];

int FindPeaks(int det_id, int num_peaks, float chmin, float chmax, float resolution = 1, double sigma = 2, double threshold = 0.3, int rebin = 1){
   int nFound;
   if(num_peaks!=1 && num_peaks!=2){
      cout<<"Wrong number of gamma peaks!"<<endl;
      return 0;
   }
   TSpectrum* s;
   TF1 *f;
   s = new TSpectrum(num_peaks+1);
   s->SetResolution(resolution);
   h_new[det_id] = (TH1F*)h[det_id]->Clone();
   h_new[det_id]->Rebin(rebin);
   h_new[det_id]->GetXaxis()->SetRangeUser(chmin,chmax);

   nFound = s->Search(h_new[det_id],sigma,"new",threshold);
   printf("In spectra [%i] I found %i candidate peaks to fit\n",det_id,nFound);
   if(nFound!=num_peaks){
      for(int i=0; i<num_peaks; i++){
         PeakPositions[i][det_id]=0;
         PeakErrors[i][det_id]=0;
      }
      cout<<"Strange spectrum"<<endl;
      if_fit[det_id]=1;
      return 0;
   }
   Float_t *xPeaks = s->GetPositionX(); 
   vector<float> Data, height;
   for(int i=0; i<nFound; i++){
      Data.push_back( *(xPeaks+i) );
   }
   sort( Data.begin(), Data.end() );
   Double_t minValue, maxValue;
   Double_t rangeMin, rangeMax;
   for(int i=0; i<nFound; i++){
      int counts = h_new[det_id]->GetBinContent(Data[0]);
      height.push_back(counts);
   }
   Double_t minValue, maxValue;
   Double_t rangeMin, rangeMax;
   minValue = 0.82*Data[0];
   maxValue = 1.12*Data[num_peaks-1];
   rangeMin = 0.80*Data[0];
   rangeMax = 1.22*Data[num_peaks-1];
   fCanvas->cd();
   h_new[det_id]->Draw();
   h_new[det_id]->GetXaxis()->SetRangeUser(rangeMin,rangeMax);
   if(num_peaks==1){
      f = new TF1("f", "gaus(0)+pol2(3)", rangeMin, rangeMax);
      f->SetParameter(0,height[0]);  f->SetParameter(1,Data[0]);  f->SetParameter(2,10);
      f->SetParLimits(1,0.97*Data[0],1.03*Data[0]);
      f->SetParLimits(2,0,35);
   }else{
      f = new TF1("f", "gaus(0)+gaus(3)+pol2(6)", rangeMin, rangeMax);
      f->SetParameter(0,height[0]);  f->SetParameter(1,Data[0]);  f->SetParameter(2,10);
      f->SetParLimits(1,0.97*Data[0],1.03*Data[0]);
      f->SetParLimits(2,0,35);
      f->SetParameter(3,height[1]);  f->SetParameter(4,Data[1]);  f->SetParameter(5,10);
      f->SetParLimits(4,0.97*Data[1],1.03*Data[1]);
      f->SetParLimits(5,0,35);
   }
   h_new[det_id]->Fit("f","QR");
   if(num_peaks==1){
      PeakPositions[0][det_id] = f->GetParameter(1);
      PeakErrors[0][det_id] = f->GetParameter(2);
   }else{
      PeakPositions[0][det_id] = f->GetParameter(1);
      PeakPositions[1][det_id] = f->GetParameter(4);
      PeakErrors[0][det_id] = f->GetParameter(2);
      PeakErrors[1][det_id] = f->GetParameter(5);
   }
   fCanvas->Update();
   if_fit[det_id]=1;
}

int OpenRoot(int calibrationDate, int source){
   char dummyText[1000];
   char rootFile[200];
   ifstream out_calib;
   int id;
   float peak1, peak2;
   float perr1, perr2;
   cout<<"Opening root files."<<endl;
   if(source==1){
      sprintf(rootFile,"./rootfiles/calib/137cs%i.root",calibrationDate);
      sprintf(dummyText,"./CalibResults/PeakPositions_Cs.txt.%i",calibrationDate);
      //for a temp workaround, use first peak of 88y instead of 137cs
      //sprintf(rootFile,"./rootfiles/calib/88y%i.root",calibrationDate);
   }else if(source==2){
      sprintf(rootFile,"./rootfiles/calib/60co%i.root",calibrationDate);
      sprintf(dummyText,"./CalibResults/PeakPositions_Co.txt.%i",calibrationDate);
   }else if(source==3){
      sprintf(rootFile,"./rootfiles/calib/88y%i.root",calibrationDate);
      sprintf(dummyText,"./CalibResults/PeakPositions_Y1.txt.%i",calibrationDate);
   }else if(source==4){
      sprintf(rootFile,"./rootfiles/calib/88y%i.root",calibrationDate);
      sprintf(dummyText,"./CalibResults/PeakPositions_Y2.txt.%i",calibrationDate);
   }else{
      cout<<"source 1: 137Cs, 2: 60Co, 3: 88Y (898), 4: 88Y (1836)"<<endl;
      return 0;
   }
   for(int j=0;j<numberOfDetectors;j++)  {
      for(int i=0; i<2; i++){
         PeakPositions[i][j]=0;
         PeakErrors[i][j]=0;
      }
   }

   out_calib.open(dummyText, ios::in);
   while(out_calib.good()){
      if(source==1 || source==3 || source==4){
         out_calib >> id>>peak1>>perr1;
         PeakPositions[0][id] = peak1;
         PeakErrors[0][id] = perr1;
      }else{
         out_calib >> id>>peak1>>perr1>>peak2>>perr2;
         PeakPositions[0][id] = peak1;
         PeakErrors[0][id] = perr1;
         PeakPositions[1][id] = peak2;
         PeakErrors[1][id] = perr2;
      }
   }

   inFile = new TFile(rootFile,"READ");
   inFile->GetObject("h101",h2);
   int yBins= h2->GetYaxis()->GetNbins();
   int yMin = h2->GetYaxis()->GetXmin();
   int yMax = h2->GetYaxis()->GetXmax();
   for(int j=0;j<numberOfDetectors;j++)  {
      if_fit[j]=0;
      sprintf(dummyText,"h[%i]",j);
      h[j] = new TH1F(dummyText,"",yBins,yMin,yMax);
      for(int k=0;k<yBins;k++) {
         int bin = h2->GetBinContent(j+1,k);
         h[j]->SetBinContent(k,bin);
      }
   }
   return 1;
}

int SavePeaks(int calibrationDate, int source){
   char rootFile[200];
   char dummyText[1000];
   if(source==1){
      sprintf(dummyText,"./CalibResults/PeakPositions_Cs.txt.%i",calibrationDate);
      sprintf(rootFile,"./CalibResults/FitResult_137cs%i.root",calibrationDate);
   }else if(source==2){
      sprintf(dummyText,"./CalibResults/PeakPositions_Co.txt.%i",calibrationDate);
      sprintf(rootFile,"./CalibResults/FitResult_60co%i.root",calibrationDate);
   }else if(source==3){
      sprintf(dummyText,"./CalibResults/PeakPositions_Y1.txt.%i",calibrationDate);
      sprintf(rootFile,"./CalibResults/FitResult_88y1%i.root",calibrationDate);
   }else if(source==4){
      sprintf(dummyText,"./CalibResults/PeakPositions_Y2.txt.%i",calibrationDate);
      sprintf(rootFile,"./CalibResults/FitResult_88y2%i.root",calibrationDate);
   }else{
      cout<<"source 1: 137Cs, 2: 60Co, 3: 88Y (898), 4: 88Y (1836)"<<endl;
      return 0;
   }
   double peak1, peak2, error1, error2;

   fCalibrationResult= fopen(dummyText,"w");
   for(int j=0;j<numberOfDetectors;j++){
      if(source==1||source==3||source==4){
         peak1 = PeakPositions[0][j];
         error1 = PeakErrors[0][j];
         fprintf(fCalibrationResult,"%i %10.3f %10.3f\n",j,peak1,error1);
      }else{
         peak1 = PeakPositions[0][j];
         error1 = PeakErrors[0][j];
         peak2 = PeakPositions[1][j];
         error2 = PeakErrors[1][j];
         fprintf(fCalibrationResult,"%i %10.3f %10.3f %10.3f %10.3f\n",j,peak1,error1,peak2,error2);
      }
   }
   fclose(fCalibrationResult);

   outFile = new TFile(rootFile,"UPDATE");
   for(int j=0;j<numberOfDetectors;j++)  {
      if(if_fit[j]==1){
         h_new[j]->Write();
      }
   }
   outFile->Write();
   outFile->Close();
   return 1;
}

void FitSlopeOffset(int calibrationDate){
   //double y[4]={661.66, 1173.24, 1332.50, 1836.06}, x[numberOfDetectors][4];
   //double y[5]={661.66, 898.04, 1173.24, 1332.50, 1836.06}, x[numberOfDetectors][5]={0.0};
   double y[5]={660.26, 897.02, 1170.72, 1333.06, 1837.30}, x[numberOfDetectors][5]={0.0}; //energies taken from geant4 simulation
   double yerr[5] = {0.0}, xerr[numberOfDetectors][5];
   ifstream cs_calib, co_calib, y1_calib, y2_calib;
   char dummyText[1000];
   int id;
   float peak1, peak2, peak3, peak4, peak5;
   float perr1, perr2, perr3, perr4, perr5;
   sprintf(dummyText,"./CalibResults/PeakPositions_Cs.txt.%i",calibrationDate);
   cs_calib.open(dummyText, ios::in);
   sprintf(dummyText,"./CalibResults/PeakPositions_Co.txt.%i",calibrationDate);
   co_calib.open(dummyText, ios::in);
   sprintf(dummyText,"./CalibResults/PeakPositions_Y1.txt.%i",calibrationDate);
   y1_calib.open(dummyText, ios::in);
   sprintf(dummyText,"./CalibResults/PeakPositions_Y2.txt.%i",calibrationDate);
   y2_calib.open(dummyText, ios::in);
   double offset[numberOfDetectors];
   double gain[numberOfDetectors];
   double chi2[numberOfDetectors];
   while(cs_calib.good()){
      cs_calib >> id>>peak1>>perr1;
      x[id][0] = peak1;
      xerr[id][0] = perr1;
   }
   while(y1_calib.good()){
      y1_calib >> id>>peak2>>perr2;
      x[id][1] = peak2;
      xerr[id][1] = perr2;
   }
   while(co_calib.good()){
      co_calib >> id>>peak3>>perr3>>peak4>>perr4;
      x[id][2] = peak3;
      x[id][3] = peak4;
      xerr[id][2] = perr3;
      xerr[id][3] = perr4;
   }
   while(y2_calib.good()){
      y2_calib >> id>>peak5>>perr5;
      x[id][4] = peak5;
      xerr[id][4] = perr5;
   }
   TGraph* gr[numberOfDetectors];
   //TCanvas *c1=new TCanvas("c1","c1",500,800);

   sprintf(dummyText,"./CalibResults/CalibSlopeOffset.txt.%i",calibrationDate);
   fCalibrationResult= fopen(dummyText,"w");
   sprintf(dummyText,"./CalibResults/CalibSlopeOffset.%i.root",calibrationDate);
   TFile fRoot(dummyText,"RECREATE");

   for(int m=0; m<numberOfDetectors; m++){
      double xfit[5]={0.}, yfit[5]={0.};
      int numpeaks=0;
      for(int i=0; i<5; i++){
         if(x[m][i]>0){
            xfit[numpeaks]=x[m][i];
            cout<<i<<", "<<m<<", "<<x[m][i]<<endl;
            yfit[numpeaks]=y[i];
            numpeaks++;
         }
      }
      if(numpeaks>=2){
         fCanvas->cd();
         gr[m] = new TGraph(numpeaks,xfit,yfit);
         f = new TF1("f","pol1", 0,2000);
         f->SetParameters(0,3);
         gr[m]->Fit("f","QR");
         f = gr[m]->GetFunction("f");
         f->SetLineColor(kBlue);
         f->SetLineWidth(1);
         offset[m] = f->GetParameter(0);
         gain[m] = f->GetParameter(1);
         chi2[m] = f->GetChisquare();
         cout<<"Good, id = "<<m<<", k = "<<gain[m]<<", b = "<<offset[m]<<", chi2 = "<<chi2[m]/2<<endl;
         gr[m]->Draw("AP*");
         fCanvas->Update();
         //c1->WaitPrimitive();
         fprintf(fCalibrationResult,"%i %10.3f %10.3f %10.3f\n",m,gain[m],offset[m],chi2[m]/2);
         gr[m]->Write();
      }else{
         gr[m] = new TGraph(5,x[m],y);
         gr[m]->Draw("A*");
         gr[m]->Write();
         fprintf(fCalibrationResult,"%i %10.3f %10.3f %10.3f\n",m,0,0,10000);
      }
   }
   fclose(fCalibrationResult);
   fRoot.Write();
   fRoot.Close();
}

void FitSigma(int calibrationDate){
   double y[5]={661.66, 898.04, 1173.24, 1332.50, 1836.06}, x[numberOfDetectors][5]={0.0};
   double yerr[5] = {0.0}, xerr[numberOfDetectors][5];
   ifstream cs_calib, co_calib, y1_calib, y2_calib, slopeoffset;
   char dummyText[1000];
   int id;
   float peak1, peak2, peak3, peak4, peak5;
   float perr1, perr2, perr3, perr4, perr5;
   sprintf(dummyText,"./CalibResults/PeakPositions_Cs.txt.%i",calibrationDate);
   cs_calib.open(dummyText, ios::in);
   sprintf(dummyText,"./CalibResults/PeakPositions_Co.txt.%i",calibrationDate);
   co_calib.open(dummyText, ios::in);
   sprintf(dummyText,"./CalibResults/PeakPositions_Y1.txt.%i",calibrationDate);
   y1_calib.open(dummyText, ios::in);
   sprintf(dummyText,"./CalibResults/PeakPositions_Y2.txt.%i",calibrationDate);
   y2_calib.open(dummyText, ios::in);
   sprintf(dummyText,"./CalibResults/CalibSlopeOffset.txt.%i",calibrationDate);
   slopeoffset.open(dummyText, ios::in);
   double pow0[numberOfDetectors];
   double pow1[numberOfDetectors];
   double gain[numberOfDetectors]={0.0}, slope, offset, chi2;
   while(cs_calib.good()){
      cs_calib >> id>>peak1>>perr1;
      x[id][0] = perr1;
   }
   while(y1_calib.good()){
      y1_calib >> id>>peak2>>perr2;
      x[id][1] = perr2;
   }
   while(co_calib.good()){
      co_calib >> id>>peak3>>perr3>>peak4>>perr4;
      x[id][2] = perr3;
      x[id][3] = perr4;
   }
   while(y2_calib.good()){
      y2_calib >> id>>peak5>>perr5;
      x[id][4] = perr5;
   }
   while(slopeoffset.good()){
      slopeoffset >> id>>slope>>offset>>chi2;
      cout<<id<<" "<<slope<<endl;
      gain[id] = slope;
   }
   TGraph* gr[numberOfDetectors];
   //TCanvas *c1=new TCanvas("c1","c1",500,800);

   sprintf(dummyText,"./CalibResults/CalibSigma.txt.%i",calibrationDate);
   fCalibrationResult= fopen(dummyText,"w");
   sprintf(dummyText,"./CalibResults/CalibSigma.%i.root",calibrationDate);
   TFile fRoot(dummyText,"RECREATE");

   for(int m=0; m<numberOfDetectors; m++){
      double xfit[5]={0.}, yfit[5]={0.};
      int numpeaks=0;
      if(gain[m]==0){
         //cout<<"problem"<<endl;
         fprintf(fCalibrationResult,"%i %10.3f %10.3f\n",m+1,0,0);
         gr[m] = new TGraph(5,x[m],y);
         gr[m]->Draw("A*");
         gr[m]->Write();
         continue;
      }
      for(int i=0; i<5; i++){
         if(x[m][i]>0){
            if(i==3 && (x[m][i]<x[m][i-1] || x[m][i]>x[m][i-1]+3)){
               xfit[numpeaks-1]=(xfit[numpeaks-1]+x[m][i])/2;
               yfit[numpeaks-1]=(yfit[numpeaks-1]+y[i])/2;
            }else{
               xfit[numpeaks]=x[m][i];
               yfit[numpeaks]=y[i];
               numpeaks++;
            }
         }
      }
      if(numpeaks>=2){
         fCanvas->cd();
         gr[m] = new TGraph(numpeaks,yfit,xfit);
         f = new TF1("f","[0]*pow(x,[1])", 0,2000);
         f->SetParameters(1,1);
         gr[m]->Fit("f","QR");
         f = gr[m]->GetFunction("f");
         f->SetLineColor(kBlue);
         f->SetLineWidth(1);
         pow0[m] = f->GetParameter(0);
         pow1[m] = f->GetParameter(1);
         //pow1[m] = 0.5;
         cout<<"Good, NP = "<<numpeaks<<", id = "<<m<<", pow0 = "<<pow0[m]<<", pow1 = "<<pow1[m]<<endl;
         gr[m]->Draw("AP*");
         fCanvas->Update();
         //c1->WaitPrimitive();
         fprintf(fCalibrationResult,"%i %10.3f %10.3f\n",m+1,pow0[m]*gain[m],pow1[m]);
         gr[m]->Write();
      }else{
         gr[m] = new TGraph(5,x[m],y);
         gr[m]->Draw("A*");
         gr[m]->Write();
         fprintf(fCalibrationResult,"%i %10.3f %10.3f\n",m+1,0,0);
      }
   }
   fclose(fCalibrationResult);
   fRoot.Write();
   fRoot.Close();
}

int FindPeaks_All_crystals(int calibrationDate, int source, float chmin, float chmax, float resolution = 1, double sigma = 2, double threshold = 0.3, int rebin = 1){
   int if_good=OpenRoot(calibrationDate,source);
   if(if_good!=1){
      cout<<"Error"<<endl;
      return 0;
   }

   //start calibration work
   fCanvas->SetFillColor(0);
   fCanvas->SetFrameFillColor(0);
   fCanvas->SetBorderSize(0.);
   fCanvas->SetBorderMode(0);
   fCanvas->SetFrameBorderMode(0);
   fCanvas->SetTopMargin(0.05);
   fCanvas->SetBottomMargin(0.05);
   fCanvas->SetLeftMargin(0.05);
   fCanvas->SetRightMargin(0.05);
   int num_peaks;
   if(source==1||source==3||source==4){
      num_peaks=1;
   }else{
      num_peaks=2;
   }

   for(int m=0;m<numberOfDetectors;m++){
      if(PeakPositions[0][m]==0){
         FindPeaks(m,num_peaks,chmin,chmax,resolution,sigma,threshold,rebin);
      }
   }
   SavePeaks(calibrationDate,source);
}
