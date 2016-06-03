// Code to decode the ridf format file for MINOS
// Calibration, Analysis and Tracking

// > make
// > ./MakeMINOSOffline ../ridf/FileName.ridf              -> Creates a root file: ../rootfiles/FileName.root
// > ./MakeMINOSOffline ../ridf/FileName.ridf test.root
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "TSystem.h"
#include "TArtStoreManager.hh"
#include "TArtMINOSParameters.hh"
#include "TArtEventStore.hh"
#include "TArtCalibMINOS.hh"
#include "TArtCalibMINOSData.hh"
#include "TArtAnalyzedMINOS.hh"
#include "TArtTrackMINOS.hh"
#include "TArtVertexMINOS.hh"
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
#include "Math/Vector3D.h"
#include "TMinosClust.h"
#include "TMinosResult.h"

using namespace std;
using namespace ROOT::Math;

char *ROOTFILEDIR = "../rootfiles/";

TClonesArray data_result;
TMinosResult *minosdata_result;

double conv_fit(double *x, double *p);
int Obertelli_filter(vector<double> *x,vector<double> *y, vector<double> *x_out,vector<double> *y_out, vector<int> *ringbool);
double FitFunction(double *x, double *p);
void FindStart(double pStart[4], double chi[2],  int fitStatus[2],TGraph *grxz, TGraph *gryz);
double distance2(double x,double y,double z, double *p);
void SumDistance1(int &, double *, double & sum, double * par,  int);
void SumDistance2(int &, double *, double & sum, double * par, int);
void Hough_filter(vector<double> *x,vector<double> *y,vector<double> *z,vector<double> *q,vector<double> *x_out,vector<double> *y_out,vector<double> *z_out,vector<double> *q_out);
void vertex(double *p, double *pp, double &xv,double &yv,double &zv);

int main(int argc, char** argv) {

   // Filename
   char* ridffile;
   char* rootfile;
   if(argc < 2)
   {
      cerr << "Missing RIDF file argument" << endl;
   }
   ridffile = argv[1];

   cout << " *** RIDF file: " << ridffile << endl;

   TArtStoreManager *sman = TArtStoreManager::Instance();

   TArtMINOSParameters *setup = new TArtMINOSParameters("MINOSParameters","MINOSParameters");
   setup->LoadParameters("./db/MINOS.xml");

   TArtEventStore *estore = new TArtEventStore();
   estore->Open(ridffile);
   //TArtRawEventObject *rawevent = estore->GetRawEventObject();

   TArtCalibMINOS *CalibMINOS = new TArtCalibMINOS();

   if(argv[2]==NULL) {
      char*  pch;
      char* pch2;
      char* infile;
      pch = strtok(ridffile, "/");
      while (pch != NULL)
      {
         infile = pch;
         pch = strtok (NULL, "/");
      }
      cerr << infile << endl;
      pch2 = strtok(infile, ".");

      char OutFile[200]="";
      strcat(OutFile, ROOTFILEDIR);
      strcat(OutFile, pch2);
      strcat(OutFile, "_MINOS.root");

      rootfile=OutFile;
   }
   else rootfile=argv[2];

   cout << endl;
   cout << " *** ROOT file: " << rootfile << endl;

   TFile *fout = new TFile(rootfile,"RECREATE");
   TTree * tree = new TTree("tree","ridf tree");
   //tree->Branch("rawdata",&rawevent);
   //TClonesArray fitdata;
   //fitdata.SetClass("TMinosClust");
   //tree->Branch("fitdata",&fitdata);
   int trackNbr;
   //tree->Branch("trackNbr",&trackNbr,"trackNbr/I");
   int trackNbr_FINAL;
   tree->Branch("trackNbr_FINAL",&trackNbr_FINAL,"trackNbr_FINAL/I");
   int evtOrig;
   tree->Branch("evtOrig",&evtOrig,"evtOrig/I");
   int padsleft;
   //tree->Branch("padsleft",&padsleft,"padsleft/I");
   data_result.SetClass("TMinosResult");
   //tree->Branch("data_result",&data_result);
   double z_vertex=0., x_vertex=0., y_vertex=0., r_vertex=0., phi_vertex = 0.;
   // Variables only filled when trackNbr_FINAL==2
   tree->Branch("x_vertex",&x_vertex,"x_vertex/D");
   tree->Branch("y_vertex",&y_vertex,"y_vertex/D");
   tree->Branch("z_vertex",&z_vertex,"z_vertex/D");
   tree->Branch("r_vertex",&r_vertex,"r_vertex/D");
   tree->Branch("phi_vertex",&phi_vertex,"phi_vertex/D"); // angle between two tracks in 3D in degrees
   double thetaz1=0., thetaz2=0.;
   tree->Branch("thetaz1",&thetaz1,"thetaz1/D"); // angle between 1st track and z axis in 3D in degrees
   tree->Branch("thetaz2",&thetaz2,"thetaz2/D"); // angle between 2nd track and z axis in 3D in degrees
   double par1[4], par2[4];
   tree->Branch("parFit_1",&par1,"parFit_1[4]/D");
   tree->Branch("parFit_2",&par2,"parFit_2[4]/D");
   double beta=0., beta1=0., beta2=0., betam=0.;
   tree->Branch("beta",&beta,"beta/D");
   tree->Branch("beta1",&beta1,"beta1/D"); // beta of beam projectile at the beginning of target
   tree->Branch("beta2",&beta2,"beta2/D"); // beta of beam projectile at the end of target

   // Parameters for the MINOS ANALYSIS
   double MINOSthresh;
   double TimeBinElec; //in ns
   double VDrift; //in cm/µs
   double Tshaping; // in ns
   double DelayTrig; // in bins
   double DALIOffset;
   double TargetLength; // in mm
   int BeamA, BeamZ;
   string ConfigBeam;
   double BeamEnergy;

   ifstream ConfigFile;
   ConfigFile.open("./../ConfigFileSetup.txt");
   string Header;
   ConfigFile >> ConfigBeam;
   ConfigFile >> Header >> MINOSthresh;
   ConfigFile >> Header >> TimeBinElec;
   ConfigFile >> Header >> VDrift;
   ConfigFile >> Header >> Tshaping;
   ConfigFile >> Header >> DelayTrig;
   ConfigFile >> Header >> DALIOffset;
   ConfigFile >> Header >> TargetLength;
   ConfigFile >> Header >> BeamA;
   ConfigFile >> Header >> BeamZ;
   ConfigFile >> Header >> BeamEnergy;
   ConfigFile >> Header >> beta1;
   ConfigFile >> Header >> betam;
   ConfigFile >> Header >> beta2;
   ConfigFile.close();

   cout << endl;
   cout << " *** MINOS Configuration Parameters for beam of *** " << ConfigBeam << endl;
   cout << " Electronics        :::   MINOSthresh = " << MINOSthresh << " (bins)  ;  TimeBinElec = " << TimeBinElec << " (ns)  ;  VDrift = " << VDrift << " (cm/micros)  ;  Tshaping = " << Tshaping << " (ns)  ;  DelayTrig = " << DelayTrig << " (bins)" << endl;
   cout << " Doppler correction :::   TargetLength = " << TargetLength << " (mm)  ;  Beam (A,Z) = (" << BeamA << "," << BeamZ << ") of E = " << BeamEnergy << " (MeV/u)  ;  beta1 = " << beta1 << " (beginning of target)  ; betam = " << betam << " (middle of target)  ;  beta2 = " << beta2 << " (end of target)" << endl;
   cout << endl;

   double PI = TMath::Pi();

   int neve = 0;

   double maxCharge=0.;
   int filled = 0;
   vector<double> Xpad, Ypad, XpadNew, YpadNew, QpadNew, ZpadNew;
   vector<int> clusterringbool;
   vector<int> clusternbr;
   vector<int> clusterpads;
   int Iteration=0;
   int filter_result=0;
   int indexfill=0;
   bool fitbool = false;
   int fit2DStatus = 0;
   double Chi2=0.;
   double x_mm,y_mm,z_mm,q_pad,t_pad;

   TF1 *fit_function = new TF1("fit_function",conv_fit, 0, 511, 3); // only 3 param. because baseline is fixed
   double hfit_max, hfit_max_T, T_min, T_max;
   TH1F *hfit = new TH1F("hfit","hfit",512,0,512);


   //2nd step variables
   int npoint_temp=0, cluster_temp=0;
   int padsleft2=0;
   int cluster1=0, cluster2=0;
   int ringsum=0;
   int ringtouch[18]={0};
   double zmax=0.;
   int array_final=0;
   bool padsnbr1=false, padsnbr2=false;
   double Qmean1=0., Qmean2=0.;
   TGraph * gryz_1;
   TGraph * grxz_1;
   TGraph * gryz_2;
   TGraph * grxz_2;
   vector<double> xin, yin, zin, qin, xout, yout, zout, qout;
   vector<int> cluster;

   TMinuit *min ;
   TH1F *h_vertex=new TH1F("h_vertex","h_vertex",2000,-500,500);

   while(estore->GetNextEvent()){
      if(neve%1000==0) cout << "Event " << neve << endl;

      //Clear & Reset variables
      //fitdata.Clear();
      data_result.Clear();
      Xpad.clear();
      Ypad.clear();
      XpadNew.clear();
      YpadNew.clear();
      QpadNew.clear();
      clusterringbool.clear();
      clusternbr.clear();
      clusterpads.clear();
      hfit->Reset();

      filled=0;
      indexfill = 0;
      maxCharge = 0.;
      Iteration=0;
      filter_result=0;
      fit2DStatus=0;
      trackNbr=0;
      trackNbr_FINAL=0;
      padsleft=0;
      x_mm = 0.; y_mm = 0.; z_mm = 0.; q_pad = 0.; t_pad = 0.;
      fitbool = false;
      array_final=0;
      ringsum=0;
      cluster1=0; cluster2=0;
      z_vertex=-1000.; x_vertex=0.; y_vertex=0.; r_vertex=0.;
      zmax=0.;
      padsnbr1=false;
      padsnbr2=false;
      Qmean1=0.;
      Qmean2=0.;
      padsleft2=0;     
      thetaz1=-10.; thetaz2=-10.;  
      phi_vertex=0.;

      int npoint1=0,npoint2=0;
      gryz_1 = new TGraph();
      grxz_1 = new TGraph();
      gryz_2 = new TGraph();
      grxz_2 = new TGraph();


      evtOrig = neve;
      CalibMINOS->ClearData();
      CalibMINOS->ReconstructData();

      TArtCalibMINOSData *minos = new TArtCalibMINOSData();
      //TMinosClust *minosfitdata;

      //1:MINOS://///////////////// Filling vectors with (x,y) information ///////////////////:MINOS://

      for(Int_t i=0;i<CalibMINOS->GetNumCalibMINOS();i++) {
         minos = CalibMINOS->GetCalibMINOS(i);
         maxCharge = 0.;
         x_mm = minos->GetX();
         y_mm = minos->GetY();

         if( !(abs(x_mm)<0.01 && abs(y_mm)<0.01) ) { // NON connected MINOS channels...
            for(Int_t j=0; j<minos->GetNData(); j++) {
               if(minos->GetCalibValue(j)>maxCharge) maxCharge = minos->GetCalibValue(j);
               //cerr << "Event " << neve << ", x=" << x_mm << ", y_mm=" << y_mm << ", t=" << minos->GetCalibTime(j) << ", q=" << minos->GetCalibValue(j)+250. << endl;
            }

            if(maxCharge>=MINOSthresh) {
               Xpad.push_back(minos->GetX());
               Ypad.push_back(minos->GetY());
               filled++;
            }
         }
      }


      //2:MINOS://////////////////////// Modified (xy) Hough transform ////////////////////////:MINOS://
      if(filled>0) {
         padsleft = Xpad.size();

         while(Xpad.size()>=10 && Iteration<20) {
            filter_result = 0;
            Iteration++;
            filter_result = Obertelli_filter(&Xpad, &Ypad, &XpadNew, &YpadNew, &clusterringbool);
            //cout << "Obertelli filter n°" << Iteration << " ::: size of cluster=" << filter_result << " , " << XpadNew.size() << " , size of old pads=" << Xpad.size() << endl; 

            for(int ik=0; ik<filter_result; ik++) {
               clusternbr.push_back(Iteration);
               clusterpads.push_back(filter_result);
            }
            if(filter_result>10 && clusterringbool.back()==1) trackNbr++;
         }
      }

      for(unsigned int il=0; il<XpadNew.size(); il++)
      {
         //minosfitdata = (TMinosClust*)fitdata.ConstructedAt(il);
         //minosfitdata->Set(XpadNew[il], YpadNew[il],-10000, -10000, -10000, clusternbr[il],clusterpads[il], 0.);
         ZpadNew.push_back(-10000.);
         QpadNew.push_back(-10000.);
      }


      ////////////////////        following analysis ONLY if #tracks = 1 || 2 || 3 || 4         ///////////////////

      if(trackNbr>0 && trackNbr<5)
      {
         if(filled==0) cerr << "Error !!!" << endl;

         //3:MINOS://////////// Fitting the taken pads for Qmax and Ttrig information ////////////:MINOS://

         padsleft = padsleft - XpadNew.size();

         for(Int_t i=0;i<CalibMINOS->GetNumCalibMINOS();i++) {
            minos = CalibMINOS->GetCalibMINOS(i);
            x_mm = minos->GetX();
            y_mm = minos->GetY();
            hfit->Reset();
            fitbool = false;

            for(unsigned int jj=0; jj<XpadNew.size(); jj++) {
               if( abs(XpadNew[jj]-x_mm)<0.01 && abs(YpadNew[jj]-y_mm)<0.01) {
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
                     z_mm = -10000;
                  }
                  else {
                     // Add to the variables the fit parameters
                     t_pad = fit_function_Tpad;//trigger time
                     z_mm = ((t_pad-DelayTrig)*TimeBinElec*1e-3*VDrift*10); //time bin*(ns/us)*vdrift(cm/us)*(mm/cm) =>mm
                     q_pad = fit_function_max-250.;  // Max charge/event
                  }

                  //minosfitdata = (TMinosClust*)fitdata.ConstructedAt(indexfill);
                  //minosfitdata->Set(XpadNew[indexfill], YpadNew[indexfill], t_pad*TimeBinElec, z_mm , q_pad, clusternbr[indexfill], clusterpads[indexfill], Chi2);
                  //Fill the z and q information for next steps (3D Hough filter & 3D fit weighted by charge)
                  ZpadNew[indexfill] = z_mm;
                  QpadNew[indexfill] = q_pad;

               }//end if histogram not empty

               //cerr << "Event " << neve << "::: x=" << XpadNew[indexfill] << ", y=" << YpadNew[indexfill] << ", Qmax=" << q_pad << ", t=" << t_pad << ", z=" << z_mm << endl;

            }// end if fitbool==true
            else continue; 
         }//END of entries in tclonesarray for the event


         //4:MINOS://////////// Filtering the tracks off possible noise with Hough3D (3*2D planes) ////////////:MINOS://

         padsleft2 = XpadNew.size();
         for(unsigned int i=0;i<(XpadNew.size());i++)
         {
            if(xin.size()>0 && ((cluster_temp!=int(clusternbr[i]) && i!=0) || i==(XpadNew.size() - 1)))
            {
               Hough_filter(&xin, &yin, &zin, &qin, &xout, &yout, &zout, &qout);
               for(unsigned int ij=0; ij<xout.size();ij++)
               {
                  if(zout[ij]>zmax) zmax = zout[ij];
                  ringtouch[int((sqrt(xout[ij]*xout[ij]+yout[ij]*yout[ij])-45.2)/2.1)]++;
               }
               for(int ko=0; ko<18; ko++)
               {
                  if(ringtouch[ko]>0) ringsum++;
               }
               if(zmax>290) ringsum=16;
               if(xout.size()>10 && ringsum>=15)
               {
                  trackNbr_FINAL++;
                  if(trackNbr_FINAL==1)
                  {
                     cluster1 = cluster_temp;
                     for(unsigned int ij=0; ij<xout.size(); ij++)
                     {
                        grxz_1->SetPoint(npoint1,zout[ij],xout[ij]);
                        gryz_1->SetPoint(npoint1,zout[ij],yout[ij]);
                        minosdata_result = (TMinosResult*)data_result.ConstructedAt(array_final);
                        minosdata_result->Set(xout[ij], yout[ij], zout[ij], qout[ij], 1, xout.size(), zmax);
                        array_final++;
                        npoint1++;
                     }
                  }
                  else if(trackNbr_FINAL==2)
                  {
                     cluster2 = cluster_temp;
                     for(unsigned int ij=0; ij<xout.size(); ij++)
                     {
                        grxz_2->SetPoint(npoint2,zout[ij],xout[ij]);
                        gryz_2->SetPoint(npoint2,zout[ij],yout[ij]);
                        minosdata_result = (TMinosResult*)data_result.ConstructedAt(array_final);
                        minosdata_result->Set(xout[ij], yout[ij], zout[ij], qout[ij], 2, xout.size(), zmax);
                        array_final++;
                        npoint2++;
                     }
                  }
               }
               //cout<<"Evt "<<entry<<" data point "<<i<<" track Nr = "<<trackNbr_FINAL<<" xout.size ="<<xout.size()<<" data cluster "<<minosdata->n_Cluster<<endl;

               xin.clear();
               yin.clear();
               zin.clear();
               qin.clear();
               xout.clear();
               yout.clear();
               zout.clear();
               qout.clear();
               npoint_temp=0;
               ringsum=0;// angle between 1st track and z axis in 3D in degrees
               zmax=0.;
               for(int ko=0; ko<18; ko++) ringtouch[ko] = 0;

            }

            cluster_temp = clusternbr[i];

            if(!(clusterpads[i]>=10 && clusterringbool[i]==1 && ZpadNew[i]>-10000 && ZpadNew[i]<=320)) continue;
            else
            {
               //				cout<<minosdata->x_mm<<" "<<minosdata->z_mm<<" "<<minosdata->Phi<<endl;
               xin.push_back(XpadNew[i]);
               yin.push_back(YpadNew[i]);
               zin.push_back(ZpadNew[i]);
               qin.push_back(QpadNew[i]);
               npoint_temp++;
            }

         }//end of loop on pads 

         //5:MINOS://////////// Fitting the filtered tracks in 3D (weight by charge, TMinuit) ////////////:MINOS://


         //For 1 track found or more (less than 5 in total)
         if(trackNbr_FINAL>=1)
         {
            padsleft2 = padsleft2 - npoint1 - npoint2;

            Double_t pStart_1[4]={0,1,0,1};
            Double_t pStart_2[4]={0,1,0,1};
            Double_t parFit_1[4],err_1[4],parFit_2[4],err_2[4],chi1[2],chi2[2];
            Int_t fitStatus[2];

            // 1st track fitting in TMinuit 
            min = new TMinuit(4);
            min->SetPrintLevel(-1);
            Double_t arglist[10];
            arglist[0] = 3;
            Int_t iflag;
            int nvpar,nparx;
            double amin,edm, errdef;
            double chi2res1, chi2res2;

            FindStart(pStart_1,chi1,fitStatus, grxz_1,gryz_1);
            min->SetFCN(SumDistance1);
            // Set starting values and step sizes for parameters
            min->mnparm(0,"x0",pStart_1[0],0.1,-500,500,iflag);
            min->mnparm(1,"Ax",pStart_1[1],0.1,-10,10,iflag);
            min->mnparm(2,"y0",pStart_1[2],0.1,-500,500,iflag);
            min->mnparm(3,"Ay",pStart_1[3],0.1,-10,10,iflag);
            arglist[0] = 100; // number of function calls
            arglist[1] = 0.000001; // tolerance
            min->mnexcm("MIGRAD",arglist,2,iflag); // minimization with MIGRAD

            min->mnstat(amin,edm,errdef,nvpar,nparx,iflag);  //returns current status of the minimization
            // get fit parameters
            for (int i = 0; i <4; i++) min->GetParameter(i,parFit_1[i],err_1[i]);

            if(trackNbr_FINAL==1)
            {
               for(int iop=0; iop<4;iop++) parFit_2[iop]=0;
            }
            else
            {
               FindStart(pStart_2,chi2,fitStatus,grxz_2,gryz_2);
               min->SetFCN(SumDistance2);
               // Set starting values and step sizes for parameters
               min->mnparm(0,"x0",pStart_2[0],0.1,-500,500,iflag);
               min->mnparm(1,"Ax",pStart_2[1],0.1,-10,10,iflag);
               min->mnparm(2,"y0",pStart_2[2],0.1,-500,500,iflag);
               min->mnparm(3,"Ay",pStart_2[3],0.1,-10,10,iflag);
               arglist[0] = 100; // number of function calls
               arglist[1] = 0.000001; // tolerance
               min->mnexcm("MIGRAD",arglist,2,iflag); // minimization with MIGRAD

               min->mnstat(amin,edm,errdef,nvpar,nparx,iflag);  //returns current status of the minimization
               // get fit parameters
               for (int i = 0; i <4; i++) min->GetParameter(i,parFit_2[i],err_2[i]);	
            }

            int sumerr_2=0;
            int sumerr_1=0;

            for(int index=0; index<4; index++) {
               sumerr_2+=err_2[index];
               sumerr_1+=err_1[index];
            }

            //cout<< "Entry " << evtOrig << " --- chi2 for 1st cluster=" << chi1[0]/npoint1 <<", "<< chi1[1]/npoint1 <<" -- chi2 for 2nd cluster="<<  chi2[0]/npoint2 <<", "<< chi2[1]/npoint2 <<endl;

            if(npoint1>0) chi2res1 = (chi1[0]+chi1[1])/npoint1;
            else chi2res1 = -1000;
            if(npoint2>0) chi2res2 = (chi2[0]+chi2[1])/npoint2;
            else chi2res2 = -1000;

            vertex(parFit_1, parFit_2, x_vertex, y_vertex, z_vertex);

            r_vertex = sqrt(x_vertex*x_vertex + y_vertex*y_vertex);

            thetaz1 = acos(1/(1 + parFit_1[1]*parFit_1[1] + parFit_1[3]*parFit_1[3]))*180./PI;
            if(thetaz1>180) cerr << "thetaz1 cond &&&" << endl;
            thetaz2 = acos(1/(1 + parFit_2[1]*parFit_2[1] + parFit_2[3]*parFit_2[3]))*180./PI;
            if(thetaz2>180) cerr << "thetaz2 cond @@@@" << endl;

            //phi_vertex: angle in 3D between 2 tracks
            phi_vertex = acos((parFit_1[1]*parFit_2[1] + parFit_1[3]*parFit_2[3]+1)/(sqrt(parFit_1[1]*parFit_1[1]+parFit_1[3]*parFit_1[3]+1)*sqrt(parFit_2[1]*parFit_2[1]+parFit_2[3]*parFit_2[3]+1)))*180./PI;

            for(int pari=0;pari<4;pari++) {
               par1[pari] = parFit_1[pari];
               par2[pari] = parFit_2[pari];
            }


            h_vertex->Fill(z_vertex);

            delete min;

            //6:MINOS://////////// Calculate the beta corresponding to z_vertex found ////////////:MINOS://

            // estimated beam track parallel to the z axis
            if(z_vertex>=-10 && z_vertex<0) beta = beta1;
            else if(z_vertex>=0 && z_vertex<=100) beta = beta1 - z_vertex*(beta1-beta2)/TargetLength;
            else if(z_vertex>100 && z_vertex<=110) beta = beta2;
            else beta = betam;

         }// end if trackNbr_FINAL>=1

         delete grxz_1;
         delete gryz_1;
         delete grxz_2;
         delete gryz_2;

         xin.clear();
         yin.clear();
         zin.clear();
         qin.clear();
         xout.clear();
         yout.clear();
         zout.clear();
         qout.clear();
         cluster.clear();

      }//loop for E(T) fits for less than 5 track evts

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

int Obertelli_filter(vector<double> *x,vector<double> *y, vector<double> *x_out,vector<double> *y_out, vector<int> *ringbool) {
   double bint1=2.;
   double bint2=2.;
   int maxt = 360.;
   int mint = 0.;
   int nt1=(maxt-mint)/bint1;
   int nt2=(maxt-mint)/bint1;

   double PI = TMath::Pi();
   double Rint = 45.2;
   double Rext = 45.2 + 18*2.1;
   int filter_result = 0;

   TH2F *hp_xy = new TH2F("hp_xy","hp_xy",nt1,mint,maxt,nt2,mint,maxt);
   TH2F *hpDiag_xy = new TH2F("hpDiag_xy","hpDiag_xy",nt1,mint,maxt,nt2,mint,maxt);

   double max_xy;

   vector<double> xTemp, yTemp;

   double theta1, theta2, xt1, yt1, xt2, yt2;
   double line0=0., line1=0.;
   double delta=0., AA=0., BB=0., CC=0.;
   double maxtheta1=0., maxtheta2=0., xmax1=0., ymax1=0., xmax2=0., ymax2=0.;
   double par0=0., par1=0.;
   double r_mm=0.;
   int ringsum=0;
   bool maxfound = false;

   for(unsigned int i=0;i<x->size();i++)
   {
      xTemp.push_back(x->at(i));
      yTemp.push_back(y->at(i));

      //Fill coordinate space histograms for plots
      //Loop of indices
      for(int j=0;j<nt1;j++)
      {
         theta1 = (j+0.5)*bint1 + mint;
         xt1 = Rint * TMath::Cos(theta1*PI/180.);
         yt1 = Rint * TMath::Sin(theta1*PI/180.);
         line1 = (yt1 - y->at(i))/(xt1 - x->at(i));
         line0 = yt1 - xt1 * line1;
         AA = 1 + line1*line1;
         BB = 2*line0*line1;
         CC = line0*line0 - Rext*Rext;

         delta = BB*BB - 4*AA*CC;
         if(delta>=0)
         {
            xt2 = (-BB - sqrt(delta))/(2*AA);
            yt2 = line0 + line1*xt2;
            if(xt2<=0)	      theta2=  180 - asin(yt2/Rext)*180/PI;
            else if(xt2>0)
            {
               if(yt2>0)	      theta2=  asin(yt2/Rext)*180/PI;
               else if(yt2<=0)	      theta2=  360 + asin(yt2/Rext)*180/PI;
            }


            if( (xt1*x->at(i) + yt1*y->at(i))>=0 && (xt2*x->at(i) + yt2*y->at(i))>=0  && (xt1*xt2+yt1*yt2)>=0)
            {
               hp_xy->Fill(theta1,theta2);
               if(abs(theta1-theta2)<=10) hpDiag_xy->Fill(theta1,theta2);
            }
            else
            {
               if(delta!=0)
               {
                  xt2 = (-BB + sqrt(delta))/(2*AA);
                  yt2 = line0 + line1*xt2;
                  if(xt2<=0)	      theta2=  180 - asin(yt2/Rext)*180/PI;
                  else if(xt2>0)
                  {
                     if(yt2>0)	      theta2=  asin(yt2/Rext)*180/PI;
                     else if(yt2<=0)	      theta2=  360 + asin(yt2/Rext)*180/PI;
                  }

                  if( (xt1*x->at(i) + yt1*y->at(i))>=0 && (xt2*x->at(i) + yt2*y->at(i))>=0  && (xt1*xt2+yt1*yt2)>=0)
                  {
                     hp_xy->Fill(theta1,theta2);
                     if(abs(theta1-theta2)<=10) hpDiag_xy->Fill(theta1,theta2);
                  }
               }
            }

         }

      }
   }
   x->clear(); 
   y->clear();

   if(hpDiag_xy->GetMaximum()>=10) max_xy = hpDiag_xy->GetMaximum();
   else max_xy = hp_xy->GetMaximum();

   for(int ii=0; ii<nt1; ii++)
   {
      if(maxfound ==true) break;
      for(int jj=0; jj<nt2; jj++)
      {
         if(hp_xy->GetBinContent(ii+1, jj+1) == max_xy)
         {
            maxtheta1 = (ii+0.5)*bint1 + mint;
            maxtheta2 = (jj+0.5)*bint2 + mint;
            maxfound = true;

         }
         if(maxfound ==true) break;
      }
   }

   xmax1 = Rint * TMath::Cos(maxtheta1*PI/180.);
   ymax1 = Rint * TMath::Sin(maxtheta1*PI/180.);
   xmax2 = Rext * TMath::Cos(maxtheta2*PI/180.);
   ymax2 = Rext * TMath::Sin(maxtheta2*PI/180.);

   // xy PEAK
   par1 = (ymax2-ymax1)/(xmax2-xmax1);
   par0 = (ymax1 - xmax1*par1);

   //Selection of x,y points IN the maxmean+/-1 found in Obertelli transform of xy plane
   for(unsigned int i=0;i<xTemp.size();i++)
   {
      if( (abs(par1*xTemp[i]-yTemp[i]+par0)/sqrt(1+par1*par1))<= 6 && ((xmax1*xTemp[i] + ymax1*yTemp[i]) >= 0) && ((xmax2*xTemp[i] + ymax2*yTemp[i]) >= 0) && ((xmax1*xmax2 + ymax1*ymax2) >= 0))
      {
         x_out->push_back(xTemp[i]); 
         y_out->push_back(yTemp[i]);
         filter_result++;
         r_mm = sqrt(xTemp[i]*xTemp[i]+yTemp[i]*yTemp[i]);
         if(r_mm<(45.2+5*2.1)) ringsum++;
      }
      else
      {	
         x->push_back(xTemp[i]);
         y->push_back(yTemp[i]);

      }

   }

   for(int ip=0; ip<filter_result; ip++)
   {
      if(ringsum>2) ringbool->push_back(1);
      else ringbool->push_back(0);
   }
   delete hp_xy;
   delete hpDiag_xy;

   return filter_result;

}

double FitFunction(double *x, double *p) {
   double val=p[0]+p[1]*x[0];
   return(val);
}
void FindStart(double pStart[4], double chi[2],  int fitStatus[2],TGraph *grxz, TGraph *gryz) {
   double par1D[2];
   TF1 *myfit1 = new TF1("myfit1",FitFunction, -100,500,2);
   myfit1->SetParameter(0,0);
   myfit1->SetParameter(1,10);
   fitStatus[0] =0;
   grxz->Fit(myfit1,"RQM");
   chi[0]=myfit1->GetChisquare();
   par1D[0]=myfit1->GetParameter(0); 
   par1D[1]=myfit1->GetParameter(1);
   pStart[0]=par1D[0];
   pStart[1]=par1D[1];
   fitStatus[1] =0;
   gryz->Fit(myfit1,"RQM");
   chi[1]=myfit1->GetChisquare();
   par1D[0]=myfit1->GetParameter(0);
   par1D[1]=myfit1->GetParameter(1);
   pStart[2]=par1D[0];
   pStart[3]=par1D[1];
}
// Calculation of the distance line-point

// Calculation of the distance line-point
double distance2(double x,double y,double z, double *p) {
   // distance line point is D= | (xp-x0) cross  ux |
   // where ux is direction of line and x0 is a point in the line (like t = 0)
   XYZVector xp(x,y,z); //point of the track
   XYZVector x0(p[0], p[2], 0. );
   XYZVector x1(p[0] + p[1], p[2] + p[3], 1. ); //line
   XYZVector u = (x1-x0).Unit();
   double d2 = ((xp-x0).Cross(u)) .Mag2();
   return d2;
}
/// Function to be minimized
void SumDistance1(int &, double *, double & sum, double * par,  int) {
   int nused=0;
   double qtot=0;
   sum = 0;
   for(int i=0; i<data_result.GetEntriesFast(); i++)
   {
      minosdata_result = (TMinosResult*)data_result.At(i);
      if(minosdata_result->n_Cluster==1)
      {
         float x=minosdata_result->x_mm;
         float y=minosdata_result->y_mm;
         float z=minosdata_result->z_mm;
         float q=minosdata_result->Chargemax;
         double d = distance2(x, y, z, par);
         sum += d*q;
         nused++;
         qtot+=q;
      }
   }
   sum/=qtot;
}

/// Function to be minimized
void SumDistance2(int &, double *, double & sum, double * par, int) {
   int nused=0;
   double qtot=0;
   sum = 0;
   for(int i=0; i<data_result.GetEntriesFast(); i++)
   {
      minosdata_result = (TMinosResult*)data_result.At(i);
      if(minosdata_result->n_Cluster==2) 
      {
         float x=minosdata_result->x_mm;
         float y=minosdata_result->y_mm;
         float z=minosdata_result->z_mm;
         float q=minosdata_result->Chargemax;
         double d = distance2(x, y, z, par);
         sum += d*q;
         nused++;
         qtot+=q;
      }
   }
   sum/=qtot;
}

void Hough_filter(vector<double> *x,vector<double> *y,vector<double> *z,vector<double> *q,vector<double> *x_out,vector<double> *y_out,vector<double> *z_out,vector<double> *q_out) {
   int nt_xy=180;
   int nt_xz=180;
   int nt_yz=180;
   int nr_xy=45;
   int nr_xz=300;
   int nr_yz=300;
   double bint_xy=2.;
   double bint_xz=2.;
   double bint_yz=2.;
   double binr_xy=3.;
   double binr_xz=3.;
   double binr_yz=3.;
   int nt,nr;
   double PI = TMath::Pi();

   double rho_xy,rho_xz,rho_yz;
   double theta_xy,theta_xz,theta_yz;

   TH2F *hp_xy = new TH2F("hp_xy","hp_xy",nt_xy,0,180,nr_xy,-1*nr_xy,nr_xy);
   TH2F *hp_xz = new TH2F("hp_xz","hp_xz",nt_xz,0,180,nr_xz,-1*nr_xz,nr_xz);
   TH2F *hp_yz = new TH2F("hp_yz","hp_yz",nt_yz,0,180,nr_yz,-1*nr_yz,nr_yz);

   vector<double> thetapeaks_xy, rpeaks_xy, thetapeaks_xz, rpeaks_xz, thetapeaks_yz, rpeaks_yz;
   double max_xy, max_xz, max_yz;
   double rmean_xy=0, thetamean_xy=0, rmean_xz=0, thetamean_xz=0, rmean_yz=0, thetamean_yz=0;

   double r0_xy=0., r0_xz=0., r0_yz=0., rmin_xy=0., rmin_xz=0., rmin_yz=0., rmax_xy=0., rmax_xz=0., rmax_yz=0.;
   double tmin=0., tmax=0.;
   double rinf=0., rsup=0.;

   nt=nt_xy;
   nr=nr_xy;
   if(nt<nt_xz)nt=nt_xz;
   if(nr<nr_xz)nr=nr_xz;
   if(nt<nt_yz)nt=nt_yz;
   if(nr<nr_yz)nr=nr_yz;

   for(unsigned int i=0;i<x->size();i++)
   {
      //Loop of indices and fill Histograms
      for(int j=0;j<nt;j++)
      {
         //xy
         theta_xy = j*180./nt_xy;
         rho_xy = x->at(i)*TMath::Cos(theta_xy*PI/180.)+y->at(i)*TMath::Sin(theta_xy*PI/180.);
         if(abs(theta_xy)<180. && abs(rho_xy)<nr_xy) hp_xy->Fill(theta_xy,rho_xy);

         //xz
         theta_xz = j*180./nt_xz;
         rho_xz = z->at(i)*TMath::Cos(theta_xz*PI/180.)+x->at(i)*TMath::Sin(theta_xz*PI/180.);
         if(abs(theta_xz)<180. && abs(rho_xz)<nr_xz) hp_xz->Fill(theta_xz,rho_xz);

         //yz
         theta_yz = j*180./nt_yz;
         rho_yz = z->at(i)*TMath::Cos(theta_yz*PI/180.)+y->at(i)*TMath::Sin(theta_yz*PI/180.);
         if(abs(theta_yz)<180. && abs(rho_yz)<nr_yz) hp_yz->Fill(theta_yz,rho_yz);
      }
   }


   max_xy = hp_xy->GetMaximum();
   max_xz = hp_xz->GetMaximum();
   max_yz = hp_yz->GetMaximum();

   for(int ii=0; ii<nt; ii++)
   {
      for(int jj=0; jj<nr; jj++)
      {
         if(hp_xy->GetBinContent(ii+1, jj+1) == max_xy && jj<nr_xy)
         {
            thetapeaks_xy.push_back((ii+0.5)*nt_xy/nt);
            rpeaks_xy.push_back((jj+0.5)*2 - nr_xy);
            rmean_xy += rpeaks_xy.back();
            thetamean_xy += thetapeaks_xy.back();
         }
         if(hp_xz->GetBinContent(ii+1, jj+1) == max_xz)
         {
            thetapeaks_xz.push_back((ii+0.5)*nt_xz/nt);
            rpeaks_xz.push_back((jj+0.5)*2 - nr_xz);
            rmean_xz += rpeaks_xz.back();
            thetamean_xz += thetapeaks_xz.back();
         }
         if(hp_yz->GetBinContent(ii+1, jj+1) == max_yz)
         {
            thetapeaks_yz.push_back((ii+0.5)*nt_yz/nt);
            rpeaks_yz.push_back((jj+0.5)*2 - nr_yz);
            rmean_yz += rpeaks_yz.back();
            thetamean_yz += thetapeaks_yz.back();
         }
      }
   }

   // xy PEAK
   rmean_xy = rmean_xy / rpeaks_xy.size();
   thetamean_xy = thetamean_xy / thetapeaks_xy.size();

   // xz PEAK
   rmean_xz = rmean_xz / rpeaks_xz.size();
   thetamean_xz = thetamean_xz / thetapeaks_xz.size();

   // yz PEAK
   rmean_yz = rmean_yz / rpeaks_yz.size();
   thetamean_yz = thetamean_yz / thetapeaks_yz.size();

   rmean_xy = rpeaks_xy[0];
   thetamean_xy = thetapeaks_xy[0];
   rmean_xz = rpeaks_xz[0];
   thetamean_xz = thetapeaks_xz[0];
   rmean_yz = rpeaks_yz[0];
   thetamean_yz = thetapeaks_yz[0];

   //Selection of x,y,z points COMMON to the 3 maxmean+/-1 found in Hough spaces for xy, xz and yz spaces
   for(unsigned int i=0;i<x->size();i++)
   {
      r0_xy = x->at(i)*TMath::Cos(thetamean_xy*PI/180.)+y->at(i)*TMath::Sin(thetamean_xy*PI/180.);
      tmin = thetamean_xy-bint_xy;
      tmax = thetamean_xy+bint_xy;
      if((tmin)<0) tmin = tmin + 180.;
      if((tmax)>180) tmax = tmax - 180.;
      rmin_xy = x->at(i)*TMath::Cos(tmin*PI/180.)+y->at(i)*TMath::Sin(tmin*PI/180.);
      rmax_xy = x->at(i)*TMath::Cos(tmax*PI/180.)+y->at(i)*TMath::Sin(tmax*PI/180.);

      rinf = min( rmean_xy - binr_xy, rmean_xy + binr_xy);
      rsup = max( rmean_xy - binr_xy, rmean_xy + binr_xy);
      if((r0_xy>=rinf || rmin_xy>=rinf || rmax_xy>=rinf) && (r0_xy<=rsup || rmin_xy<=rsup || rmax_xy<=rsup))
      {
         r0_xz = z->at(i)*TMath::Cos(thetamean_xz*PI/180.)+x->at(i)*TMath::Sin(thetamean_xz*PI/180.);
         tmin = thetamean_xz-bint_xz;
         tmax = thetamean_xz+bint_xz;
         if((tmin)<0) tmin = tmin + 180.;
         if((tmax)>180) tmax = tmax - 180.;
         rmin_xz = z->at(i)*TMath::Cos(tmin*PI/180.)+x->at(i)*TMath::Sin(tmin*PI/180.);
         rmax_xz = z->at(i)*TMath::Cos(tmax*PI/180.)+x->at(i)*TMath::Sin(tmax*PI/180.);

         rinf = min( rmean_xz - binr_xz, rmean_xz + binr_xz);
         rsup = max( rmean_xz - binr_xz, rmean_xz + binr_xz);

         if((r0_xz>=rinf || rmin_xz>=rinf || rmax_xz>=rinf) && (r0_xz<=rsup || rmin_xz<=rsup || rmax_xz<=rsup))
         {
            r0_yz = z->at(i)*TMath::Cos(thetamean_yz*PI/180.)+y->at(i)*TMath::Sin(thetamean_yz*PI/180.);
            tmin = thetamean_yz-bint_yz;
            tmax = thetamean_yz+bint_yz;
            if((tmin)<0) tmin = tmin + 180.;
            if((tmax)>180) tmax = tmax - 180.;
            rmin_yz = z->at(i)*TMath::Cos(tmin*PI/180.)+y->at(i)*TMath::Sin(tmin*PI/180.);
            rmax_yz = z->at(i)*TMath::Cos(tmax*PI/180.)+y->at(i)*TMath::Sin(tmax*PI/180.);

            rinf = min( rmean_yz - binr_yz, rmean_yz + binr_yz);
            rsup = max( rmean_yz - binr_yz, rmean_yz + binr_yz);

            if((r0_yz>=rinf || rmin_yz>=rinf || rmax_yz>=rinf) && (r0_yz<=rsup || rmin_yz<=rsup || rmax_yz<=rsup))
            {
               x_out->push_back(x->at(i));
               y_out->push_back(y->at(i));
               z_out->push_back(z->at(i));
               q_out->push_back(q->at(i));
            }

         }
      }


   }
   delete hp_xy;
   delete hp_xz;
   delete hp_yz;

}

// Calculation of the minimal distance between 2 lines in 3D space & calculation of mid-point=>vertex of interaction
void vertex(double *p, double *pp, double &xv,double &yv,double &zv) {
   double a1 = p[0];
   double a2 = p[2];
   double b1 = p[1];
   double b2 = p[3];
   double ap1 = pp[0];
   double ap2 = pp[2];
   double bp1 = pp[1];
   double bp2 = pp[3];

   double alpha, beta, A, B, C;

   alpha = (bp1*(a1-ap1)+bp2*(a2-ap2))/(bp1*bp1 + bp2*bp2 + 1);
   beta = (bp1*b1+bp2*b2+1)/(bp1*bp1 + bp2*bp2 + 1);

   A = beta*(bp1*bp1 + bp2*bp2 + 1) - (bp1*b1 + bp2*b2 + 1);
   B = (b1*b1 + b2*b2 + 1) - beta*(bp1*b1+bp2*b2+1);
   C = beta*(bp1*(ap1-a1) + bp2*(ap2-a2)) - (b1*(ap1-a1) + b2*(ap2-a2));

   double sol1, solf1;
   double x,y,z,xp,yp,zp;


   sol1 = -(A*alpha + C)/(A*beta + B);
   solf1 = alpha + beta* sol1;

   x = a1 + b1*sol1;
   y = a2 + b2*sol1;
   z = sol1;
   xp = ap1 + bp1*solf1;
   yp = ap2 + bp2*solf1;
   zp = solf1;

   xv = (x+xp)/2.;
   yv = (y+yp)/2.;
   zv = (z+zp)/2.;

}


