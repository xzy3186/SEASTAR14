// Code to decode the ridf format file for MINOS
// MINOS Calibration of Drift Velocity

// > make
// > ./MakeMINOSVdrift ../ridf/FileName.ridf              -> Creates a root file: ../rootfiles/FileName.root
// > ./MakeMINOSVdrift ../ridf/FileName.ridf test.root
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "TSystem.h"
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtDALIParameters.hh"
#include "TArtMINOSParameters.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibDALI.hh"
#include "TArtCalibMINOS.hh"
#include "TArtCalibMINOSData.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtFocalPlane.hh"
#include "TArtEventInfo.hh"
#include "TArtPlastic.hh"
#include "TArtPPAC.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtPPAC.hh"
#include "TArtBeam.hh"
#include "TArtTOF.hh"
#include "TArtRIPS.hh"
#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"
#include <vector>
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "TGraph.h"
#include <TMinuit.h>
#include <TVirtualFitter.h>
#include <TFitter.h>
#include <time.h>
#include "Math/Vector3D.h"
#include "TMinosClust.h"

using namespace std;
using namespace ROOT::Math;

char *ROOTFILEDIR = "../rootfiles/78Ni/";

double conv_fit(double *x, double *p);

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt(){
   printf("keyboard interrupt\n");
   stoploop = true;
}

int main(int argc, char** argv) {

   // Variables to calculate the elapsed time of the process
   time_t start,stop;
   time(&start);

   // Filename
   char* ridffile;
   char* rootfile;
   if(argc < 2) cerr << "Missing RIDF file argument" << endl;
   ridffile = argv[1];

   cout << " *** RIDF file: " << ridffile << endl;

   TArtStoreManager *sman = TArtStoreManager::Instance();

   TArtEventStore *estore = new TArtEventStore();
   estore->SetInterrupt(&stoploop); 
   estore->Open(ridffile);
   //TArtRawEventObject *rawevent = estore->GetRawEventObject();

   // Create MINOSParameters to get ".xml"
   //------------------------------------
   TArtMINOSParameters *setup = new TArtMINOSParameters("MINOSParameters","MINOSParameters");
   setup->LoadParameters("./db/MINOS.xml");
   //setup->PrintListOfMINOSPara();

   TArtCalibMINOS *CalibMINOS = new TArtCalibMINOS();

   char* infile;
   if(argv[2]==NULL) {
      char*  pch;
      char* pch2;
      pch = strtok(ridffile, "/");
      while (pch != NULL) {
         infile = pch;
         pch = strtok (NULL, "/");
      }
      cerr << infile << endl;
      pch2 = strtok(infile, ".");

      char OutFile[200]="";
      strcat(OutFile, ROOTFILEDIR);
      strcat(OutFile, pch2);
      strcat(OutFile, "_MINOSVDrift.root");

      rootfile=OutFile;
   }
   else rootfile=argv[2];

   cout << endl;
   cout << " *** ROOT file: " << rootfile << endl;

   TFile *fout = new TFile(rootfile,"RECREATE");
   TTree * tree = new TTree("tree","ridf tree");
   //tree->Branch("rawdata",&rawevent);
   TClonesArray fitdata;
   fitdata.SetClass("TMinosClust");
   tree->Branch("fitdata",&fitdata);
   int Rpadnumber_max=0;
   tree->Branch("Rpadnumber_max",&Rpadnumber_max, "Rpadnumber_max/I");
   int Rpadnumber[18];


   // define data nodes which are supposed to be dumped to tree 
   //EventInfo is important for the fBit information to know the trigger!
   TClonesArray * info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
   std::cout<<info_array->GetName()<<std::endl;
   Int_t EventInfo_fBit = (Int_t)((TArtEventInfo *)info_array->At(0))->GetTriggerBit();
   tree->Branch(info_array->GetName(),&info_array);


   // Parameters for the MINOS ANALYSIS
   double MINOSthresh;
   double TimeBinElec; //in ns
   double Tshaping; // in ns


   ifstream ConfigFile;
   //ConfigFile.open("./../ConfigMINOSDrift.txt");
   ConfigFile.open("/home/analysis/analysis/anaroot/users/xuzy/ConfigMINOSDrift.txt");
   string HeaderFile;
   string dummystring;
   getline(ConfigFile,dummystring);
   ConfigFile >> MINOSthresh >> TimeBinElec >> Tshaping;
   cout << MINOSthresh << " " << TimeBinElec << " " << Tshaping << endl;
   ConfigFile.close();

   cout << endl;
   cout << " *** MINOS Configuration Parameters *** " << endl;
   cout << " Electronics        :::   MINOSthresh = " << MINOSthresh << " (bins)  ;  TimeBinElec = " << TimeBinElec << " (ns)  ;    Tshaping = " << Tshaping << endl;
   cout << endl;

   double PI = TMath::Pi();

   int neve = 0;

   //    vector<TCanvas*> Filter_canvas;
   double maxCharge=0.;
   vector<double> Xpad, Ypad, Qpad;
   vector<int> clusterringbool;
   vector<int> clusternbr;
   vector<int> clusterpads;
   int indexfill=0;
   bool fitbool = false;
   int fit2DStatus = 0;
   double Chi2=0.;
   double x_mm,y_mm,r_mm,q_pad,t_pad;

   TF1 *fit_function = new TF1("fit_function",conv_fit, 0, 511, 3); // only 3 param. because baseline is fixed
   double hfit_max, hfit_max_T, T_min, T_max;
   TH1F *hfit = new TH1F("hfit","hfit",512,0,512);

   while(estore->GetNextEvent() && neve<50000){
      if(neve%1000==0) cout << "Event " << neve << endl;

      //Clear & Reset variables
      fitdata.Clear();
      Xpad.clear();
      Ypad.clear();
      Qpad.clear();
      //	Filter_canvas.clear();
      hfit->Reset();

      indexfill = 0;
      maxCharge = 0.;
      fit2DStatus=0;
      x_mm = 0.; y_mm = 0.; r_mm = 0.; q_pad = 0.; t_pad = 0.;
      fitbool = false;
      for(int ii=0; ii<18; ii++) Rpadnumber[ii] = 0;
      Rpadnumber_max=0;



      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //Making MINOS OFFLINE Reconstruction

      CalibMINOS->ClearData();
      CalibMINOS->ReconstructData();

      TMinosClust *minosfitdata;
      TArtCalibMINOSData *minos = new TArtCalibMINOSData();

      //1:MINOS://///////////////// Filling vectors with (x,y) information ///////////////////:MINOS://

      for(Int_t i=0;i<CalibMINOS->GetNumCalibMINOS();i++) {
         minos = CalibMINOS->GetCalibMINOS(i);
         maxCharge = 0.;
         x_mm = minos->GetX();
         y_mm = minos->GetY();
         r_mm = sqrt(x_mm*x_mm + y_mm*y_mm);

         if( !(abs(x_mm)<0.01 && abs(y_mm)<0.01) ) { // NON connected MINOS channels...
            for(Int_t j=0; j<minos->GetNData(); j++) {
               if(minos->GetCalibValue(j)>maxCharge) maxCharge = minos->GetCalibValue(j);
               //cerr << "Event " << neve << ", x=" << x_mm << ", y_mm=" << y_mm << ", t=" << minos->GetCalibTime(j) << ", q=" << minos->GetCalibValue(j)+250. << endl;
            }

            if(maxCharge>=MINOSthresh) {
               Xpad.push_back(minos->GetX());
               Ypad.push_back(minos->GetY());
               Rpadnumber[int((r_mm-45.2)/2.1)]++;
               Qpad.push_back(maxCharge);
            }
         }
      }

      for(int ii=0; ii<18; ii++) {
         if(Rpadnumber[ii]>Rpadnumber_max) Rpadnumber_max = Rpadnumber[ii];
      }

      //2:MINOS://////////// Fitting the pads for Qmax and Ttrig information ////////////:MINOS://


      for(Int_t i=0;i<CalibMINOS->GetNumCalibMINOS();i++) {
         minos = CalibMINOS->GetCalibMINOS(i);
         x_mm = minos->GetX();
         y_mm = minos->GetY();
         hfit->Reset();
         fitbool = false;

         for(unsigned int jj=0; jj<Xpad.size(); jj++) {
            if( abs(Xpad[jj]-x_mm)<0.01 && abs(Ypad[jj]-y_mm)<0.01) {
               fitbool = true;
               indexfill=jj;
               break;
            }
         }

         // Check if New channel is of interest
         //(if so, we read the Q(t) signal, and we should fill the vectors w/ t & q information after fitting E(T))
         if( fitbool==true ) {
            for(Int_t j=0; j<minos->GetNData(); j++) {
               if(minos->GetCalibValue(j)>=0){
                  hfit->SetBinContent(hfit->FindBin(minos->GetCalibTime(j)), minos->GetCalibValue(j)+250);
                  //cerr << "    * t=" << minos->GetCalibTime(j) << " * q=" << minos->GetCalibValue(j) << endl;
               }
            }

            // Fitting the hfit histogram of last channel if not empty
            if(hfit->GetSumOfWeights()>0) {
               hfit->GetXaxis()->SetRange(0,510);
               hfit_max = hfit->GetMaximum();
               hfit_max_T = hfit->GetMaximumBin();
               T_min=-1;
               T_max=-1;

               // Find the T_min & T_max limits of the signal non zero
               for(int h=hfit_max_T;h>0;h--) {
                  if(T_min == -1 && (hfit->GetBinContent(h))<=250 ) {
                     T_min = h;
                     break;
                  }
               }
               for(int h=hfit_max_T;h<510;h++) {
                  if(T_max == -1 && (hfit->GetBinContent(h))==0 ) {
                     T_max = h;
                     break;
                  }
               }
               //Take only 1.5*Tshaping before the max if other signals before...
               if((hfit_max_T-3.5*(Tshaping/TimeBinElec)) > T_min) T_min = hfit_max_T-2*Tshaping/TimeBinElec;
               if((hfit_max_T+10) < T_max || T_max==-1) T_max = hfit_max_T+10.;

               T_min = max(T_min,0.);
               if(T_max>510) T_max = 510;


               // Set fit parameters			
               fit_function->SetParameter(0, hfit_max-250.);
               fit_function->SetParameter(1,hfit_max_T - Tshaping/TimeBinElec);
               fit_function->SetParameter(2, Tshaping/TimeBinElec);
               fit_function->SetParLimits(0,0,100000);
               fit_function->SetParLimits(1,-20,512);
               fit_function->SetParLimits(2,0,512);

               // Fit of the signal within the range defined: [T_min, T_max]
               fit2DStatus = hfit->Fit(fit_function,"Q","",T_min,T_max);
               //gStyle->SetOptFit(1111);

               double fit_function_max = 0., fit_function_Tpad = 0.;

               if(fit2DStatus==0) {
                  TF1 *fit_result = hfit->GetFunction("fit_function");
                  Chi2 = fit_result->GetChisquare();
                  fit_function_max = fit_function->GetMaximum();
                  fit_function_Tpad = fit_function->GetParameter(1);
               }

               //attribute q_pad and z_mm value
               if(fit2DStatus!=0 || fit_function_max<=20. || fit_function_max>=100000. || fit_function_Tpad<=0.15 || fit_function_Tpad>=513. || fit_function->GetParameter(2)<=0.15 || fit_function->GetParameter(2)>=513.) {
                  //cout << "NOT CORRECTLY FITTED !!!!!!! chi2 = " << vectout->chi2.back() << endl;
                  q_pad = hfit_max-250.;
                  t_pad = -10000;
               }
               else {
                  // Add to the variables the fit parameters
                  t_pad = fit_function_Tpad;//trigger time
                  q_pad = fit_function_max-250.;  // Max charge/event
               }

               // Comment out for saving histograms of Q(t)
               // TH1F *hfit_clone = (TH1F*)hfit->Clone(Form("E(T)_%d_%f_%f",neve,x_mm,y_mm));
               //hfit_clone->Write();
               //cout << " Histogram results: " << i << " --- @ (" << lastx_mm << ", " << lasty_mm << "); q_pad = " << q_pad << endl;
               minosfitdata = (TMinosClust*)fitdata.ConstructedAt(indexfill);
               minosfitdata->Set(Xpad[indexfill], Ypad[indexfill], t_pad*TimeBinElec, -10000 , q_pad, 0, 0, Chi2);

            }//end if histogram not empty

            else
            {
               minosfitdata = (TMinosClust*)fitdata.ConstructedAt(indexfill);
               //cout << "%Values ::: " << Xpad[index[indexfill]] << " , " << Ypad[index[indexfill]] << " , " << z_mm << " , " << q_pad << endl;
               minosfitdata->Set(Xpad[indexfill], Ypad[indexfill], -10000, -10000 , -10000, 1, 1, 0);
            }
            //cerr << "Event " << neve << "::: x=" << XpadNew[indexfill] << ", y=" << YpadNew[indexfill] << ", Qmax=" << q_pad << ", t=" << t_pad << ", z=" << z_mm << endl;

         }// end if fitbool==true
         else continue; 
      }//END of entries in tclonesarray for the event



      tree->Fill();  // fill the tree in ALL cases to obtain the same number of evts for DALI2 analysis

      estore->ClearData();
      neve ++;
   }

   tree->Print();
   cout<<"Write..."<<endl;
   fout->Write();
   cout<<"Close..."<<endl;
   fout->Close();
   cout<<"Conversion to Root done!"<<endl;

   time(&stop);
   printf("Elapsed time: %.1f seconds\n",difftime(stop,start));


   return 0;

}

// Fit function for E(T)
double conv_fit(double *x, double *p) {
   double val;
   if(!(x[0]<p[1] || x[0]>512.)) val = p[0] * exp(-3.*(x[0]-p[1])/p[2])  * sin((x[0]-p[1])/p[2]) * pow((x[0]-p[1])/p[2], 3) + 250;
   //else val = p[3];
   else val = 250;
   return(val);
}

double FitFunction(double *x, double *p) {
   double val=p[0]+p[1]*x[0];
   return(val);
}
