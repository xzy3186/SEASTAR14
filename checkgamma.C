#define checkgamma_cxx
#include "checkgamma.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>


// -- ROOT headers
#include <TTree.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCutG.h>
#include <TString.h>

// -- General headers
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

void checkgamma::Loop()
{
   //   In a ROOT session, you can do:
   //      Root > .L checkgamma.C
   //      Root > checkgamma t
   //      Root > t.GetEntry(12); // Fill t data members with entry number 12
   //      Root > t.Show();       // Show values of entry 12
   //      Root > t.Show(16);     // Read and show values of entry 16
   //      Root > t.Loop();       // Loop on all entries
   //

   //     This is the loop skeleton where:
   //    jentry is the global entry number in the chain
   //    ientry is the entry number in the current Tree
   //  Note that the argument to GetEntry must be:
   //    jentry for TChain::GetEntry
   //    ientry for TTree::GetEntry and TBranch::GetEntry
   //
   //       To read only selected branches, Insert statements like:
   // METHOD1:
   //    fChain->SetBranchStatus("*",0);  // disable all branches
   //    fChain->SetBranchStatus("branchname",1);  // activate branchname
   // METHOD2: replace line
   //    fChain->GetEntry(jentry);       //read all branches
   //by  b_branchname->GetEntry(ientry); //read only this branch

   fChain->SetBranchStatus("*",0);  // disable all branches
   fChain->SetBranchStatus("zet",1);  // activate branchname
   fChain->SetBranchStatus("aoq",1);  // activate branchname
   fChain->SetBranchStatus("zetc",1);  // activate branchname
   fChain->SetBranchStatus("aoqc",1);  // activate branchname
   fChain->SetBranchStatus("DALINaI.fDoppCorEnergy",1);  // activate branchname
   fChain->SetBranchStatus("DALINaI",1);  // activate branchname
   fChain->SetBranchStatus("dalimult",1);  // activate branchname
   fChain->SetBranchStatus("BigRIPSPPAC.fTSumX",1);  // activate branchname
   fChain->SetBranchStatus("BigRIPSPPAC.fTSumY",1);  // activate branchname
   if (fChain == 0) return;

   ////for setting 1
   //TH2F *pid_br     = new TH2F("pid_br",    "PID__br",    500,2.30,2.80,500,20,30);
   //TH2F *pid_br70ni = new TH2F("pid_br70ni","PID__br70ni",500,2.30,2.80,500,20,30);
   //TH2F *pid_br69co = new TH2F("pid_br69co","PID__br69co",500,2.30,2.80,500,20,30);
   //TH2F *pid_br68co = new TH2F("pid_br68co","PID__br68co",500,2.30,2.80,500,20,30);

   //TH2F *pid_zd     = new TH2F("pid_zd",    "PID__zd",    500,2.40,2.90,500,20,30);
   //TH2F *pid_zd69co = new TH2F("pid_zd69co","PID__zd69co",500,2.40,2.90,500,20,30);
   //TH2F *pid_zd67co = new TH2F("pid_zd67co","PID__zd67co",500,2.40,2.90,500,20,30);
   //TH2F *pid_zd68fe = new TH2F("pid_zd68fe","PID__zd68fe",500,2.40,2.90,500,20,30);

   //TH1F *g_zd69co        = new TH1F("g_zd69co","Gamma_zd69co",1000,0.,5000.);
   //TH1F *g_zd67co        = new TH1F("g_zd67co","Gamma_zd67co",1000,0.,5000.);
   //TH1F *g_zd68fe        = new TH1F("g_zd68fe","Gamma_zd68fe",1000,0.,5000.);
   //TH1F *g_br70ni_zd69co = new TH1F("g_br70ni_zd69co","Gamma_br70ni_zd69co", 1000,0.,5000.);
   //TH1F *g_br69co_zd69co = new TH1F("g_br69co_zd69co","Gamma_br69co_zd69co", 1000,0.,5000.);
   //TH1F *g_br68co_zd67co = new TH1F("g_br68co_zd67co","Gamma_br68co_zd67co", 1000,0.,5000.);
   //TH1F *g_br70ni_zd68fe = new TH1F("g_br70ni_zd68fe","Gamma_br70ni_zd68fe", 1000,0.,5000.);
   //TH1F *g_br69co_zd68fe = new TH1F("g_br69co_zd68fe","Gamma_br69co_zd68fe", 1000,0.,5000.);

   //TH1F *g_zd69co_m1        = new TH1F("g_zd69co_m1","Gamma_zd69co",1000,0.,5000.);
   //TH1F *g_zd67co_m1        = new TH1F("g_zd67co_m1","Gamma_zd67co",1000,0.,5000.);
   //TH1F *g_zd68fe_m1        = new TH1F("g_zd68fe_m1","Gamma_zd68fe",1000,0.,5000.);
   //TH1F *g_br70ni_zd69co_m1 = new TH1F("g_br70ni_zd69co_m1","Gamma_br70ni_zd69co", 1000,0.,5000.);
   //TH1F *g_br69co_zd69co_m1 = new TH1F("g_br69co_zd69co_m1","Gamma_br69co_zd69co", 1000,0.,5000.);
   //TH1F *g_br68co_zd67co_m1 = new TH1F("g_br68co_zd67co_m1","Gamma_br68co_zd67co", 1000,0.,5000.);
   //TH1F *g_br70ni_zd68fe_m1 = new TH1F("g_br70ni_zd68fe_m1","Gamma_br70ni_zd68fe", 1000,0.,5000.);
   //TH1F *g_br69co_zd68fe_m1 = new TH1F("g_br69co_zd68fe_m1","Gamma_br69co_zd68fe", 1000,0.,5000.);

   //TH1F *g_zd69co_m2        = new TH1F("g_zd69co_m2","Gamma_zd69co",1000,0.,5000.);
   //TH1F *g_zd67co_m2        = new TH1F("g_zd67co_m2","Gamma_zd67co",1000,0.,5000.);
   //TH1F *g_zd68fe_m2        = new TH1F("g_zd68fe_m2","Gamma_zd68fe",1000,0.,5000.);
   //TH1F *g_br70ni_zd69co_m2 = new TH1F("g_br70ni_zd69co_m2","Gamma_br70ni_zd69co", 1000,0.,5000.);
   //TH1F *g_br69co_zd69co_m2 = new TH1F("g_br69co_zd69co_m2","Gamma_br69co_zd69co", 1000,0.,5000.);
   //TH1F *g_br68co_zd67co_m2 = new TH1F("g_br68co_zd67co_m2","Gamma_br68co_zd67co", 1000,0.,5000.);
   //TH1F *g_br70ni_zd68fe_m2 = new TH1F("g_br70ni_zd68fe_m2","Gamma_br70ni_zd68fe", 1000,0.,5000.);
   //TH1F *g_br69co_zd68fe_m2 = new TH1F("g_br69co_zd68fe_m2","Gamma_br69co_zd68fe", 1000,0.,5000.);

   //TH1F *g_zd69co_m3        = new TH1F("g_zd69co_m3","Gamma_zd69co",1000,0.,5000.);
   //TH1F *g_zd67co_m3        = new TH1F("g_zd67co_m3","Gamma_zd67co",1000,0.,5000.);
   //TH1F *g_zd68fe_m3        = new TH1F("g_zd68fe_m3","Gamma_zd68fe",1000,0.,5000.);
   //TH1F *g_br70ni_zd69co_m3 = new TH1F("g_br70ni_zd69co_m3","Gamma_br70ni_zd69co", 1000,0.,5000.);
   //TH1F *g_br69co_zd69co_m3 = new TH1F("g_br69co_zd69co_m3","Gamma_br69co_zd69co", 1000,0.,5000.);
   //TH1F *g_br68co_zd67co_m3 = new TH1F("g_br68co_zd67co_m3","Gamma_br68co_zd67co", 1000,0.,5000.);
   //TH1F *g_br70ni_zd68fe_m3 = new TH1F("g_br70ni_zd68fe_m3","Gamma_br70ni_zd68fe", 1000,0.,5000.);
   //TH1F *g_br69co_zd68fe_m3 = new TH1F("g_br69co_zd68fe_m3","Gamma_br69co_zd68fe", 1000,0.,5000.);

   ////for setting 2
   //TH2F *pid_br     = new TH2F("pid_br",    "PID__br",    500,2.30,2.80,500,24,34);
   //TH2F *pid_br75cu = new TH2F("pid_br75cu","PID__br75cu",500,2.30,2.80,500,24,34);
   //TH2F *pid_br74cu = new TH2F("pid_br74cu","PID__br74cu",500,2.30,2.80,500,24,34);
   //TH2F *pid_br74ni = new TH2F("pid_br74ni","PID__br74ni",500,2.30,2.80,500,24,34);
   //TH2F *pid_br73ni = new TH2F("pid_br73ni","PID__br73ni",500,2.30,2.80,500,24,34);
   //TH2F *pid_br72ni = new TH2F("pid_br72ni","PID__br72ni",500,2.30,2.80,500,24,34);
   //TH2F *pid_br73co = new TH2F("pid_br73co","PID__br73co",500,2.30,2.80,500,24,34);
   //TH2F *pid_br72co = new TH2F("pid_br72co","PID__br72co",500,2.30,2.80,500,24,34);

   //TH2F *pid_zd     = new TH2F("pid_zd",    "PID__zd",    500,2.30,2.80,500,24,34);
   //TH2F *pid_zd71co = new TH2F("pid_zd71co","PID__zd71co",500,2.30,2.80,500,24,34);
   //TH2F *pid_zd73co = new TH2F("pid_zd73co","PID__zd73co",500,2.30,2.80,500,24,34);
   //TH2F *pid_zd74ni = new TH2F("pid_zd74ni","PID__zd74ni",500,2.30,2.80,500,24,34);

   //TH1F *g_zd71co        = new TH1F("g_zd71co","Gamma_zd71co",1000,0.,5000.);
   //TH1F *g_zd73co        = new TH1F("g_zd73co","Gamma_zd73co",1000,0.,5000.);
   //TH1F *g_zd74ni        = new TH1F("g_zd74ni","Gamma_zd74ni",1000,0.,5000.);
   //TH1F *g_br75cu_zd71co = new TH1F("g_br75cu_zd71co","Gamma_br75cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd73co = new TH1F("g_br75cu_zd73co","Gamma_br75cu_zd73co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd74ni = new TH1F("g_br75cu_zd74ni","Gamma_br75cu_zd74ni", 1000,0.,5000.);
   //TH1F *g_br74cu_zd71co = new TH1F("g_br74cu_zd71co","Gamma_br74cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd71co = new TH1F("g_br74ni_zd71co","Gamma_br74ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd73co = new TH1F("g_br74ni_zd73co","Gamma_br74ni_zd73co", 1000,0.,5000.);
   //TH1F *g_br73ni_zd71co = new TH1F("g_br73ni_zd71co","Gamma_br73ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br72ni_zd71co = new TH1F("g_br72ni_zd71co","Gamma_br72ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd71co = new TH1F("g_br73co_zd71co","Gamma_br73co_zd71co", 1000,0.,5000.);
   //TH1F *g_br72co_zd71co = new TH1F("g_br72co_zd71co","Gamma_br72co_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd73co = new TH1F("g_br73co_zd73co","Gamma_br73co_zd73co", 1000,0.,5000.);

   //TH1F *g_zd71co_m1        = new TH1F("g_zd71co_m1","Gamma_zd71co",1000,0.,5000.);
   //TH1F *g_zd73co_m1        = new TH1F("g_zd73co_m1","Gamma_zd73co",1000,0.,5000.);
   //TH1F *g_zd74ni_m1        = new TH1F("g_zd74ni_m1","Gamma_zd74ni",1000,0.,5000.);
   //TH1F *g_br75cu_zd71co_m1 = new TH1F("g_br75cu_zd71co_m1","Gamma_br75cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd73co_m1 = new TH1F("g_br75cu_zd73co_m1","Gamma_br75cu_zd73co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd74ni_m1 = new TH1F("g_br75cu_zd74ni_m1","Gamma_br75cu_zd74ni", 1000,0.,5000.);
   //TH1F *g_br74cu_zd71co_m1 = new TH1F("g_br74cu_zd71co_m1","Gamma_br74cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd71co_m1 = new TH1F("g_br74ni_zd71co_m1","Gamma_br74ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd73co_m1 = new TH1F("g_br74ni_zd73co_m1","Gamma_br74ni_zd73co", 1000,0.,5000.);
   //TH1F *g_br73ni_zd71co_m1 = new TH1F("g_br73ni_zd71co_m1","Gamma_br73ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br72ni_zd71co_m1 = new TH1F("g_br72ni_zd71co_m1","Gamma_br72ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd71co_m1 = new TH1F("g_br73co_zd71co_m1","Gamma_br73co_zd71co", 1000,0.,5000.);
   //TH1F *g_br72co_zd71co_m1 = new TH1F("g_br72co_zd71co_m1","Gamma_br72co_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd73co_m1 = new TH1F("g_br73co_zd73co_m1","Gamma_br73co_zd73co", 1000,0.,5000.);

   //TH1F *g_zd71co_m2        = new TH1F("g_zd71co_m2","Gamma_zd71co",1000,0.,5000.);
   //TH1F *g_zd73co_m2        = new TH1F("g_zd73co_m2","Gamma_zd73co",1000,0.,5000.);
   //TH1F *g_zd74ni_m2        = new TH1F("g_zd74ni_m2","Gamma_zd74ni",1000,0.,5000.);
   //TH1F *g_br75cu_zd71co_m2 = new TH1F("g_br75cu_zd71co_m2","Gamma_br75cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd73co_m2 = new TH1F("g_br75cu_zd73co_m2","Gamma_br75cu_zd73co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd74ni_m2 = new TH1F("g_br75cu_zd74ni_m2","Gamma_br75cu_zd74ni", 1000,0.,5000.);
   //TH1F *g_br74cu_zd71co_m2 = new TH1F("g_br74cu_zd71co_m2","Gamma_br75cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd71co_m2 = new TH1F("g_br74ni_zd71co_m2","Gamma_br74ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd73co_m2 = new TH1F("g_br74ni_zd73co_m2","Gamma_br74ni_zd73co", 1000,0.,5000.);
   //TH1F *g_br73ni_zd71co_m2 = new TH1F("g_br73ni_zd71co_m2","Gamma_br73ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br72ni_zd71co_m2 = new TH1F("g_br72ni_zd71co_m2","Gamma_br72ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd71co_m2 = new TH1F("g_br73co_zd71co_m2","Gamma_br73co_zd71co", 1000,0.,5000.);
   //TH1F *g_br72co_zd71co_m2 = new TH1F("g_br72co_zd71co_m2","Gamma_br72co_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd73co_m2 = new TH1F("g_br73co_zd73co_m2","Gamma_br73co_zd73co", 1000,0.,5000.);

   //TH1F *g_zd71co_m3        = new TH1F("g_zd71co_m3","Gamma_zd71co",1000,0.,5000.);
   //TH1F *g_zd73co_m3        = new TH1F("g_zd73co_m3","Gamma_zd73co",1000,0.,5000.);
   //TH1F *g_zd74ni_m3        = new TH1F("g_zd74ni_m3","Gamma_zd74ni",1000,0.,5000.);
   //TH1F *g_br75cu_zd71co_m3 = new TH1F("g_br75cu_zd71co_m3","Gamma_br75cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd73co_m3 = new TH1F("g_br75cu_zd73co_m3","Gamma_br75cu_zd73co", 1000,0.,5000.);
   //TH1F *g_br75cu_zd74ni_m3 = new TH1F("g_br75cu_zd74ni_m3","Gamma_br75cu_zd74ni", 1000,0.,5000.);
   //TH1F *g_br74cu_zd71co_m3 = new TH1F("g_br74cu_zd71co_m3","Gamma_br74cu_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd71co_m3 = new TH1F("g_br74ni_zd71co_m3","Gamma_br74ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br74ni_zd73co_m3 = new TH1F("g_br74ni_zd73co_m3","Gamma_br74ni_zd73co", 1000,0.,5000.);
   //TH1F *g_br73ni_zd71co_m3 = new TH1F("g_br73ni_zd71co_m3","Gamma_br73ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br72ni_zd71co_m3 = new TH1F("g_br72ni_zd71co_m3","Gamma_br72ni_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd71co_m3 = new TH1F("g_br73co_zd71co_m3","Gamma_br73co_zd71co", 1000,0.,5000.);
   //TH1F *g_br72co_zd71co_m3 = new TH1F("g_br72co_zd71co_m3","Gamma_br72co_zd71co", 1000,0.,5000.);
   //TH1F *g_br73co_zd73co_m3 = new TH1F("g_br73co_zd73co_m3","Gamma_br73co_zd73co", 1000,0.,5000.);

   //for setting 3
   TH2F *pid_br     = new TH2F("pid_br",    "PID__br",    500,2.40,2.90,500,24,34);
   TH2F *pid_br81ga = new TH2F("pid_br81ga","PID__br81ga",500,2.40,2.90,500,24,34);
   TH2F *pid_br80zn = new TH2F("pid_br80zn","PID__br80zn",500,2.40,2.90,500,24,34);
   TH2F *pid_br79cu = new TH2F("pid_br79cu","PID__br79cu",500,2.40,2.90,500,24,34);
   TH2F *pid_br78cu = new TH2F("pid_br78cu","PID__br78cu",500,2.40,2.90,500,24,34);

   TH2F *pid_zd     = new TH2F("pid_zd",    "PID__zd",    500,2.40,2.90,500,23,33);
   TH2F *pid_zd80zn = new TH2F("pid_zd80zn","PID__zd80zn",500,2.40,2.90,500,23,33);
   TH2F *pid_zd79zn = new TH2F("pid_zd79zn","PID__zd79zn",500,2.40,2.90,500,23,33);
   TH2F *pid_zd79cu = new TH2F("pid_zd79cu","PID__zd79cu",500,2.40,2.90,500,23,33);
   TH2F *pid_zd78cu = new TH2F("pid_zd78cu","PID__zd78cu",500,2.40,2.90,500,23,33);
   TH2F *pid_zd78ni = new TH2F("pid_zd78ni","PID__zd78ni",500,2.40,2.90,500,23,33);
   TH2F *pid_zd77ni = new TH2F("pid_zd77ni","PID__zd77ni",500,2.40,2.90,500,23,33);

   TH1F *g_zd80zn        = new TH1F("g_zd80zn","Gamma_zd80zn",1000,0.,5000.);
   TH1F *g_zd79zn        = new TH1F("g_zd79zn","Gamma_zd79zn",1000,0.,5000.);
   TH1F *g_zd79cu        = new TH1F("g_zd79cu","Gamma_zd79cu",1000,0.,5000.);
   TH1F *g_zd78cu        = new TH1F("g_zd78cu","Gamma_zd78cu",1000,0.,5000.);
   TH1F *g_zd78ni        = new TH1F("g_zd78ni","Gamma_zd78ni",1000,0.,5000.);
   TH1F *g_zd77ni        = new TH1F("g_zd77ni","Gamma_zd77ni",1000,0.,5000.);
   TH1F *g_br81ga_zd80zn = new TH1F("g_br81ga_zd80zn","Gamma_br81ga_zd80zn", 1000,0.,5000.);
   TH1F *g_br81ga_zd79zn = new TH1F("g_br81ga_zd79zn","Gamma_br81ga_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79zn = new TH1F("g_br80zn_zd79zn","Gamma_br80zn_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79cu = new TH1F("g_br80zn_zd79cu","Gamma_br80zn_zd79cu", 1000,0.,5000.);
   TH1F *g_br80zn_zd78cu = new TH1F("g_br80zn_zd78cu","Gamma_br80zn_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78cu = new TH1F("g_br79cu_zd78cu","Gamma_br79cu_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78ni = new TH1F("g_br79cu_zd78ni","Gamma_br79cu_zd78ni", 1000,0.,5000.);
   TH1F *g_br79cu_zd77ni = new TH1F("g_br79cu_zd77ni","Gamma_br79cu_zd77ni", 1000,0.,5000.);
   TH1F *g_br78cu_zd77ni = new TH1F("g_br78cu_zd77ni","Gamma_br78cu_zd77ni", 1000,0.,5000.);

   TH1F *g_zd80zn_m1        = new TH1F("g_zd80zn_m1","Gamma_zd80zn",1000,0.,5000.);
   TH1F *g_zd79zn_m1        = new TH1F("g_zd79zn_m1","Gamma_zd79zn",1000,0.,5000.);
   TH1F *g_zd79cu_m1        = new TH1F("g_zd79cu_m1","Gamma_zd79cu",1000,0.,5000.);
   TH1F *g_zd78cu_m1        = new TH1F("g_zd78cu_m1","Gamma_zd78cu",1000,0.,5000.);
   TH1F *g_zd78ni_m1        = new TH1F("g_zd78ni_m1","Gamma_zd78ni",1000,0.,5000.);
   TH1F *g_zd77ni_m1        = new TH1F("g_zd77ni_m1","Gamma_zd77ni",1000,0.,5000.);
   TH1F *g_br81ga_zd80zn_m1 = new TH1F("g_br81ga_zd80zn_m1","Gamma_br81ga_zd80zn", 1000,0.,5000.);
   TH1F *g_br81ga_zd79zn_m1 = new TH1F("g_br81ga_zd79zn_m1","Gamma_br81ga_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79zn_m1 = new TH1F("g_br80zn_zd79zn_m1","Gamma_br80zn_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79cu_m1 = new TH1F("g_br80zn_zd79cu_m1","Gamma_br80zn_zd79cu", 1000,0.,5000.);
   TH1F *g_br80zn_zd78cu_m1 = new TH1F("g_br80zn_zd78cu_m1","Gamma_br80zn_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78cu_m1 = new TH1F("g_br79cu_zd78cu_m1","Gamma_br79cu_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78ni_m1 = new TH1F("g_br79cu_zd78ni_m1","Gamma_br79cu_zd78ni", 1000,0.,5000.);
   TH1F *g_br79cu_zd77ni_m1 = new TH1F("g_br79cu_zd77ni_m1","Gamma_br79cu_zd77ni", 1000,0.,5000.);
   TH1F *g_br78cu_zd77ni_m1 = new TH1F("g_br78cu_zd77ni_m1","Gamma_br78cu_zd77ni", 1000,0.,5000.);

   TH1F *g_zd80zn_m2        = new TH1F("g_zd80zn_m2","Gamma_zd80zn",1000,0.,5000.);
   TH1F *g_zd79zn_m2        = new TH1F("g_zd79zn_m2","Gamma_zd79zn",1000,0.,5000.);
   TH1F *g_zd79cu_m2        = new TH1F("g_zd79cu_m2","Gamma_zd79cu",1000,0.,5000.);
   TH1F *g_zd78cu_m2        = new TH1F("g_zd78cu_m2","Gamma_zd78cu",1000,0.,5000.);
   TH1F *g_zd78ni_m2        = new TH1F("g_zd78ni_m2","Gamma_zd78ni",1000,0.,5000.);
   TH1F *g_zd77ni_m2        = new TH1F("g_zd77ni_m2","Gamma_zd77ni",1000,0.,5000.);
   TH1F *g_br81ga_zd80zn_m2 = new TH1F("g_br81ga_zd80zn_m2","Gamma_br81ga_zd80zn", 1000,0.,5000.);
   TH1F *g_br81ga_zd79zn_m2 = new TH1F("g_br81ga_zd79zn_m2","Gamma_br81ga_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79zn_m2 = new TH1F("g_br80zn_zd79zn_m2","Gamma_br80zn_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79cu_m2 = new TH1F("g_br80zn_zd79cu_m2","Gamma_br80zn_zd79cu", 1000,0.,5000.);
   TH1F *g_br80zn_zd78cu_m2 = new TH1F("g_br80zn_zd78cu_m2","Gamma_br80zn_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78cu_m2 = new TH1F("g_br79cu_zd78cu_m2","Gamma_br79cu_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78ni_m2 = new TH1F("g_br79cu_zd78ni_m2","Gamma_br79cu_zd78ni", 1000,0.,5000.);
   TH1F *g_br79cu_zd77ni_m2 = new TH1F("g_br79cu_zd77ni_m2","Gamma_br79cu_zd77ni", 1000,0.,5000.);
   TH1F *g_br78cu_zd77ni_m2 = new TH1F("g_br78cu_zd77ni_m2","Gamma_br78cu_zd77ni", 1000,0.,5000.);

   TH1F *g_zd80zn_m3        = new TH1F("g_zd80zn_m3","Gamma_zd80zn",1000,0.,5000.);
   TH1F *g_zd79zn_m3        = new TH1F("g_zd79zn_m3","Gamma_zd79zn",1000,0.,5000.);
   TH1F *g_zd79cu_m3        = new TH1F("g_zd79cu_m3","Gamma_zd79cu",1000,0.,5000.);
   TH1F *g_zd78cu_m3        = new TH1F("g_zd78cu_m3","Gamma_zd78cu",1000,0.,5000.);
   TH1F *g_zd78ni_m3        = new TH1F("g_zd78ni_m3","Gamma_zd78ni",1000,0.,5000.);
   TH1F *g_zd77ni_m3        = new TH1F("g_zd77ni_m3","Gamma_zd77ni",1000,0.,5000.);
   TH1F *g_br81ga_zd80zn_m3 = new TH1F("g_br81ga_zd80zn_m3","Gamma_br81ga_zd80zn", 1000,0.,5000.);
   TH1F *g_br81ga_zd79zn_m3 = new TH1F("g_br81ga_zd79zn_m3","Gamma_br81ga_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79zn_m3 = new TH1F("g_br80zn_zd79zn_m3","Gamma_br80zn_zd79zn", 1000,0.,5000.);
   TH1F *g_br80zn_zd79cu_m3 = new TH1F("g_br80zn_zd79cu_m3","Gamma_br80zn_zd79cu", 1000,0.,5000.);
   TH1F *g_br80zn_zd78cu_m3 = new TH1F("g_br80zn_zd78cu_m3","Gamma_br80zn_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78cu_m3 = new TH1F("g_br79cu_zd78cu_m3","Gamma_br79cu_zd78cu", 1000,0.,5000.);
   TH1F *g_br79cu_zd78ni_m3 = new TH1F("g_br79cu_zd78ni_m3","Gamma_br79cu_zd78ni", 1000,0.,5000.);
   TH1F *g_br79cu_zd77ni_m3 = new TH1F("g_br79cu_zd77ni_m3","Gamma_br79cu_zd77ni", 1000,0.,5000.);
   TH1F *g_br78cu_zd77ni_m3 = new TH1F("g_br78cu_zd77ni_m3","Gamma_br78cu_zd77ni", 1000,0.,5000.);

   Long64_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0){
         break;
      }
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (jentry%1000==0){
         cout << jentry << "/" << nentries << " (" << Form("%0.2f",float(jentry)/float(nentries)*100) << " %)\r" << flush;
      }

      int if_continue=1;
      //cout<<BigRIPSPPAC_fTSumX[ 4]<<", "<<BigRIPSPPAC_fTSumY[ 4]<<endl;
      if(
            ((BigRIPSPPAC_fTSumX[ 4]> 88 && BigRIPSPPAC_fTSumX[ 4]<103) ||
            (BigRIPSPPAC_fTSumX[ 5]> 83 && BigRIPSPPAC_fTSumX[ 5]< 98)) &&
            ((BigRIPSPPAC_fTSumX[ 6]>163 && BigRIPSPPAC_fTSumX[ 6]<178) ||
            (BigRIPSPPAC_fTSumX[ 7]>169 && BigRIPSPPAC_fTSumX[ 7]<184)) &&
            //BigRIPSPPAC_fTSumY[ 4]> 85 && BigRIPSPPAC_fTSumY[ 4]<100 &&
            //BigRIPSPPAC_fTSumY[ 5]> 85 && BigRIPSPPAC_fTSumY[ 5]<100 &&
            //BigRIPSPPAC_fTSumY[ 6]> 90 && BigRIPSPPAC_fTSumY[ 6]<105 &&
            //BigRIPSPPAC_fTSumY[ 7]> 95 && BigRIPSPPAC_fTSumY[ 7]<110 &&
            ((BigRIPSPPAC_fTSumX[ 9]>168 && BigRIPSPPAC_fTSumX[ 9]<182) ||
            (BigRIPSPPAC_fTSumX[10]>165 && BigRIPSPPAC_fTSumX[10]<180)) &&
            ((BigRIPSPPAC_fTSumX[11]>165 && BigRIPSPPAC_fTSumX[11]<180) ||
            (BigRIPSPPAC_fTSumX[12]>170 && BigRIPSPPAC_fTSumX[12]<185)) &&
            //BigRIPSPPAC_fTSumY[ 9]> 87 && BigRIPSPPAC_fTSumY[ 9]<102 &&
            //BigRIPSPPAC_fTSumY[10]> 88 && BigRIPSPPAC_fTSumY[10]<103 &&
            //BigRIPSPPAC_fTSumY[11]> 90 && BigRIPSPPAC_fTSumY[11]<105 &&
            //BigRIPSPPAC_fTSumY[12]> 95 && BigRIPSPPAC_fTSumY[12]<110 &&
            ((BigRIPSPPAC_fTSumX[18]>172 && BigRIPSPPAC_fTSumX[18]<187) ||
            (BigRIPSPPAC_fTSumX[19]>173 && BigRIPSPPAC_fTSumX[19]<188)) &&
            ((BigRIPSPPAC_fTSumX[20]>175 && BigRIPSPPAC_fTSumX[20]<190) ||
            (BigRIPSPPAC_fTSumX[21]>167 && BigRIPSPPAC_fTSumX[21]<182)) &&
            //BigRIPSPPAC_fTSumY[18]> 97 && BigRIPSPPAC_fTSumY[18]<112 &&
            //BigRIPSPPAC_fTSumY[19]> 95 && BigRIPSPPAC_fTSumY[19]<110 &&
            //BigRIPSPPAC_fTSumY[20]> 92 && BigRIPSPPAC_fTSumY[20]<107 &&
            //BigRIPSPPAC_fTSumY[21]> 89 && BigRIPSPPAC_fTSumY[21]<104 &&
            ((BigRIPSPPAC_fTSumX[22]>130 && BigRIPSPPAC_fTSumX[22]<145) ||
            (BigRIPSPPAC_fTSumX[23]>133 && BigRIPSPPAC_fTSumX[23]<148)) &&
            ((BigRIPSPPAC_fTSumX[24]>137 && BigRIPSPPAC_fTSumX[24]<152) &&
            (BigRIPSPPAC_fTSumX[25]>129 && BigRIPSPPAC_fTSumX[25]<144))
            //BigRIPSPPAC_fTSumY[22]> 57 && BigRIPSPPAC_fTSumY[22]< 72 &&
            //BigRIPSPPAC_fTSumY[23]> 59 && BigRIPSPPAC_fTSumY[23]< 74 &&
            //BigRIPSPPAC_fTSumY[24]> 66 && BigRIPSPPAC_fTSumY[24]< 81 &&
            //BigRIPSPPAC_fTSumY[25]> 48 && BigRIPSPPAC_fTSumY[25]< 63
            )
            {
               if_continue=0;
            }
      if(if_continue==1){
         continue;
      }

      // ===== checkgamma start =====

      // cout << DALINaI_ << "   ";
      // for(int i=0;i<DALINaI_;i++){
      // 	cout << DALINaI_fDoppCorEnergy[i] << "  ";
      // }
      // cout << endl;

      // -- BIGRIPS PID --

      ////for setting 1
      ////cout<<"come here"<<endl;
      //Int_t BRZet=0, BRMass=0;

      //if(zet[0]>27.5 && zet[0]<28.6){
      //   if(aoq[0]>2.480 && aoq[0]<2.490){
      //      BRZet=28;
      //      BRMass=70;
      //   }
      //}
      //if(zet[0]>26.4 && zet[0]<27.8){
      //   if(aoq[0]>2.534 && aoq[0]<2.546){
      //      BRZet=27;
      //      BRMass=69;
      //   }
      //}
      //if(zet[0]>26.2 && zet[0]<27.9){
      //   if(aoq[0]>2.497 && aoq[0]<2.510){
      //      BRZet=27;
      //      BRMass=68;
      //   }
      //}

      //pid_br->Fill(aoq[0],zet[0]);
      //if(BRZet==28 && BRMass==70){
      //   pid_br70ni->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==27 && BRMass==69){
      //   pid_br69co->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==27 && BRMass==68){
      //   pid_br68co->Fill(aoq[0],zet[0]);
      //}

      //// -- ZDS PID --
      //Int_t ZDZet=0, ZDMass=0;

      //if(zetc[3]>26.5 && zetc[3]<27.5){ // Zn
      //   if(aoqc[3]>2.540 && aoqc[3]<2.565){
      //      ZDZet=27;
      //      ZDMass=69;
      //   }
      //}
      //if(zetc[3]>26.6 && zetc[3]<27.5){ // Zn
      //   if(aoqc[3]>2.470 && aoqc[3]<2.490){
      //      ZDZet=27;
      //      ZDMass=67;
      //   }
      //}
      //if(zetc[3]>25.5 && zetc[3]<26.5){ // Zn
      //   if(aoqc[3]>2.595 && aoqc[3]<2.625){
      //      ZDZet=26;
      //      ZDMass=68;
      //   }
      //}

      //pid_zd->Fill(aoqc[3],zetc[3]);
      //if(ZDZet==0 && ZDMass==0){
      //   continue;
      //}

      //if(ZDZet==27 && ZDMass==69){
      //   pid_zd69co->Fill(aoqc[3],zetc[3]);
      //}
      //if(ZDZet==27 && ZDMass==67){
      //   pid_zd67co->Fill(aoqc[3],zetc[3]);
      //}
      //if(ZDZet==26 && ZDMass==68){
      //   pid_zd68fe->Fill(aoqc[3],zetc[3]);
      //}

      //// -- gamma spectra
      //for(int i=0;i<DALINaI_;i++){
      //   if(DALINaI_fDoppCorEnergy>0){
      //      if(ZDZet==27 && ZDMass==69) g_zd69co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(ZDZet==27 && ZDMass==67) g_zd67co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(ZDZet==26 && ZDMass==68) g_zd68fe->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==28 && BRMass==70 && ZDZet==27 && ZDMass==69) g_br70ni_zd69co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==27 && BRMass==69 && ZDZet==27 && ZDMass==69) g_br69co_zd69co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==27 && BRMass==68 && ZDZet==27 && ZDMass==67) g_br68co_zd67co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==28 && BRMass==70 && ZDZet==26 && ZDMass==68) g_br70ni_zd68fe->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==27 && BRMass==69 && ZDZet==26 && ZDMass==68) g_br69co_zd68fe->Fill(DALINaI_fDoppCorEnergy[i]);

      //      if(dalimult>=1 && dalimult<=1 &&ZDZet==27 && ZDMass==69) g_zd69co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&ZDZet==27 && ZDMass==67) g_zd67co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&ZDZet==26 && ZDMass==68) g_zd68fe_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==28 && BRMass==70 && ZDZet==27 && ZDMass==69) g_br70ni_zd69co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==27 && BRMass==69 && ZDZet==27 && ZDMass==69) g_br69co_zd69co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==27 && BRMass==68 && ZDZet==27 && ZDMass==67) g_br68co_zd67co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==28 && BRMass==70 && ZDZet==26 && ZDMass==68) g_br70ni_zd68fe_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==27 && BRMass==69 && ZDZet==26 && ZDMass==68) g_br69co_zd68fe_m1->Fill(DALINaI_fDoppCorEnergy[i]);

      //      if(dalimult>=1 && dalimult<=2 &&ZDZet==27 && ZDMass==69) g_zd69co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&ZDZet==27 && ZDMass==67) g_zd67co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&ZDZet==26 && ZDMass==68) g_zd68fe_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==28 && BRMass==70 && ZDZet==27 && ZDMass==69) g_br70ni_zd69co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==27 && BRMass==69 && ZDZet==27 && ZDMass==69) g_br69co_zd69co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==27 && BRMass==68 && ZDZet==27 && ZDMass==67) g_br68co_zd67co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==28 && BRMass==70 && ZDZet==26 && ZDMass==68) g_br70ni_zd68fe_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==27 && BRMass==69 && ZDZet==26 && ZDMass==68) g_br69co_zd68fe_m2->Fill(DALINaI_fDoppCorEnergy[i]);

      //      if(dalimult>=1 && dalimult<=3 &&ZDZet==27 && ZDMass==69) g_zd69co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&ZDZet==27 && ZDMass==67) g_zd67co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&ZDZet==26 && ZDMass==68) g_zd68fe_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==28 && BRMass==70 && ZDZet==27 && ZDMass==69) g_br70ni_zd69co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==27 && BRMass==69 && ZDZet==27 && ZDMass==69) g_br69co_zd69co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==27 && BRMass==68 && ZDZet==27 && ZDMass==67) g_br68co_zd67co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==28 && BRMass==70 && ZDZet==26 && ZDMass==68) g_br70ni_zd68fe_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==27 && BRMass==69 && ZDZet==26 && ZDMass==68) g_br69co_zd68fe_m3->Fill(DALINaI_fDoppCorEnergy[i]);

      //   }
      //}

      ////for setting 2
      //Int_t BRZet=0, BRMass=0;

      //if(zet[0]>28.2 && zet[0]<29.8){
      //   if(aoq[0]>2.564 && aoq[0]<2.577){
      //      BRZet=29;
      //      BRMass=75;
      //   }
      //   if(aoq[0]>2.530 && aoq[0]<2.543){
      //      BRZet=29;
      //      BRMass=74;
      //   }
      //}
      //if(zet[0]>27.4 && zet[0]<28.6){
      //   if(aoq[0]>2.620 && aoq[0]<2.632){
      //      BRZet=28;
      //      BRMass=74;
      //   }
      //   if(aoq[0]>2.584 && aoq[0]<2.598){
      //      BRZet=28;
      //      BRMass=73;
      //   }
      //}
      //if(zet[0]>27.3 && zet[0]<28.6){
      //   if(aoq[0]>2.550 && aoq[0]<2.561){
      //      BRZet=28;
      //      BRMass=72;
      //   }
      //}
      //if(zet[0]>26.5 && zet[0]<27.6){
      //   if(aoq[0]>2.681 && aoq[0]<2.693){
      //      BRZet=27;
      //      BRMass=73;
      //   }
      //}
      //if(zet[0]>26.4 && zet[0]<27.8){
      //   if(aoq[0]>2.643 && aoq[0]<2.656){
      //      BRZet=27;
      //      BRMass=72;
      //   }
      //}

      //pid_br->Fill(aoq[0],zet[0]);
      //if(BRZet==29 && BRMass==75){
      //   pid_br75cu->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==29 && BRMass==74){
      //   pid_br74cu->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==28 && BRMass==74){
      //   pid_br74ni->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==28 && BRMass==73){
      //   pid_br73ni->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==28 && BRMass==72){
      //   pid_br72ni->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==27 && BRMass==73){
      //   pid_br73co->Fill(aoq[0],zet[0]);
      //}
      //if(BRZet==27 && BRMass==72){
      //   pid_br72co->Fill(aoq[0],zet[0]);
      //}

      //// -- ZDS PID --
      //Int_t ZDZet=0, ZDMass=0;

      //if(zetc[3]>26.6 && zetc[3]<27.3){
      //   if(aoqc[3]>2.618 && aoqc[3]<2.652){
      //      ZDZet=27;
      //      ZDMass=71;
      //   }
      //}
      //if(zetc[3]>26.6 && zetc[3]<27.4){
      //   if(aoqc[3]>2.694 && aoqc[3]<2.722){
      //      ZDZet=27;
      //      ZDMass=73;
      //   }
      //}
      //if(zetc[3]>27.5 && zetc[3]<28.4){
      //   if(aoqc[3]>2.633 && aoqc[3]<2.662){
      //      ZDZet=28;
      //      ZDMass=74;
      //   }
      //}

      //pid_zd->Fill(aoqc[3],zetc[3]);
      //if(ZDZet==0 && ZDMass==0){
      //   continue;
      //}

      //if(ZDZet==27 && ZDMass==71){
      //   pid_zd71co->Fill(aoqc[3],zetc[3]);
      //}
      //if(ZDZet==27 && ZDMass==73){
      //   pid_zd73co->Fill(aoqc[3],zetc[3]);
      //}
      //if(ZDZet==28 && ZDMass==74){
      //   pid_zd74ni->Fill(aoqc[3],zetc[3]);
      //}

      //// -- gamma spectra
      //for(int i=0;i<DALINaI_;i++){
      //   if(DALINaI_fDoppCorEnergy>0){
      //      if(ZDZet==27 && ZDMass==71) g_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(ZDZet==27 && ZDMass==73) g_zd73co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(ZDZet==28 && ZDMass==74) g_zd74ni->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==71) g_br75cu_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==73) g_br75cu_zd73co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==29 && BRMass==75 && ZDZet==28 && ZDMass==74) g_br75cu_zd74ni->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==29 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74cu_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74ni_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==73) g_br74ni_zd73co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==28 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73ni_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==28 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72ni_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73co_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==27 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72co_zd71co->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==73) g_br73co_zd73co->Fill(DALINaI_fDoppCorEnergy[i]);

      //      if(dalimult>=1 && dalimult<=1 &&ZDZet==27 && ZDMass==71) g_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&ZDZet==27 && ZDMass==73) g_zd73co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&ZDZet==28 && ZDMass==74) g_zd74ni_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==71) g_br75cu_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==73) g_br75cu_zd73co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==75 && ZDZet==28 && ZDMass==74) g_br75cu_zd74ni_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74cu_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74ni_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==73) g_br74ni_zd73co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==28 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73ni_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==28 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72ni_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73co_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==27 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72co_zd71co_m1->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=1 &&BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==73) g_br73co_zd73co_m1->Fill(DALINaI_fDoppCorEnergy[i]);

      //      if(dalimult>=1 && dalimult<=2 &&ZDZet==27 && ZDMass==71) g_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&ZDZet==27 && ZDMass==73) g_zd73co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&ZDZet==28 && ZDMass==74) g_zd74ni_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==71) g_br75cu_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==73) g_br75cu_zd73co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==75 && ZDZet==28 && ZDMass==74) g_br75cu_zd74ni_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74cu_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74ni_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==73) g_br74ni_zd73co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==28 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73ni_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==28 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72ni_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73co_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==27 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72co_zd71co_m2->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=2 &&BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==73) g_br73co_zd73co_m2->Fill(DALINaI_fDoppCorEnergy[i]);

      //      if(dalimult>=1 && dalimult<=3 &&ZDZet==27 && ZDMass==71) g_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&ZDZet==27 && ZDMass==73) g_zd73co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&ZDZet==28 && ZDMass==74) g_zd74ni_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==71) g_br75cu_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==75 && ZDZet==27 && ZDMass==73) g_br75cu_zd73co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==75 && ZDZet==28 && ZDMass==74) g_br75cu_zd74ni_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74cu_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==71) g_br74ni_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==28 && BRMass==74 && ZDZet==27 && ZDMass==73) g_br74ni_zd73co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==28 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73ni_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==28 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72ni_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==71) g_br73co_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==27 && BRMass==72 && ZDZet==27 && ZDMass==71) g_br72co_zd71co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //      if(dalimult>=1 && dalimult<=3 &&BRZet==27 && BRMass==73 && ZDZet==27 && ZDMass==73) g_br73co_zd73co_m3->Fill(DALINaI_fDoppCorEnergy[i]);
      //   }
      //}

      //for setting 3
      Int_t BRZet=0, BRMass=0;
      if(zet[0]>30.2 && zet[0]<31.6){
         if(aoq[0]>2.604 && aoq[0]<2.617){
            BRZet=31;
            BRMass=81;
         }
      }
      if(zet[0]>29.3 && zet[0]<30.5){
         if(aoq[0]>2.658 && aoq[0]<2.669){
            BRZet=30;
            BRMass=80;
         }
      }
      if(zet[0]>28.4 && zet[0]<29.6){
         if(aoq[0]>2.716 && aoq[0]<2.727){
            BRZet=29;
            BRMass=79;
         }
      }
      if(zet[0]>28.2 && zet[0]<29.7){
         if(aoq[0]>2.681 && aoq[0]<2.693){
            BRZet=29;
            BRMass=78;
         }
      }

      pid_br->Fill(aoq[0],zet[0]);
      if(BRZet==31 && BRMass==81){
         pid_br81ga->Fill(aoq[0],zet[0]);
      }
      if(BRZet==30 && BRMass==80){
         pid_br80zn->Fill(aoq[0],zet[0]);
      }
      if(BRZet==29 && BRMass==79){
         pid_br79cu->Fill(aoq[0],zet[0]);
      }
      if(BRZet==29 && BRMass==78){
         pid_br78cu->Fill(aoq[0],zet[0]);
      }

      // -- ZDS PID --
      Int_t ZDZet=0, ZDMass=0;

      if(zetc[3]-0.25>29.5 && zetc[3]-0.25<30.5){
         if(aoqc[3]>2.653 && aoqc[3]<2.688){
            ZDZet=30;
            ZDMass=80;
         }
      }
      if(zetc[3]-0.25>29.5 && zetc[3]-0.25<30.5){
         if(aoqc[3]>2.621 && aoqc[3]<2.649){
            ZDZet=30;
            ZDMass=79;
         }
      }
      if(zetc[3]-0.25>28.5 && zetc[3]-0.25<29.4){
         if(aoqc[3]>2.712 && aoqc[3]<2.745){
            ZDZet=29;
            ZDMass=79;
         }
      }
      if(zetc[3]-0.25>28.6 && zetc[3]-0.25<29.5){
         if(aoqc[3]>2.675 && aoqc[3]<2.709){
            ZDZet=29;
            ZDMass=78;
         }
      }
      if(zetc[3]-0.25>27.6 && zetc[3]-0.25<28.4){
         if(aoqc[3]>2.772 && aoqc[3]<2.799){
            ZDZet=28;
            ZDMass=78;
         }
      }
      if(zetc[3]-0.25>27.6 && zetc[3]-0.25<28.4){
         if(aoqc[3]>2.736 && aoqc[3]<2.765){
            ZDZet=28;
            ZDMass=77;
         }
      }

      pid_zd->Fill(aoqc[3],zetc[3]-0.25);
      if(ZDZet==0 && ZDMass==0){
         continue;
      }
      if(ZDZet==30 && ZDMass==80){
         pid_zd80zn->Fill(aoqc[3],zetc[3]-0.25);
      }
      if(ZDZet==30 && ZDMass==79){
         pid_zd79zn->Fill(aoqc[3],zetc[3]-0.25);
      }
      if(ZDZet==29 && ZDMass==79){
         pid_zd79cu->Fill(aoqc[3],zetc[3]-0.25);
      }
      if(ZDZet==29 && ZDMass==78){
         pid_zd78cu->Fill(aoqc[3],zetc[3]-0.25);
      }
      if(ZDZet==28 && ZDMass==78){
         pid_zd78ni->Fill(aoqc[3],zetc[3]-0.25);
      }
      if(ZDZet==28 && ZDMass==77){
         pid_zd77ni->Fill(aoqc[3],zetc[3]-0.25);
      }

      // -- gamma spectra
      for(int i=0;i<DALINaI_;i++){
         if(DALINaI_fDoppCorEnergy>0){
            if(ZDZet==30 && ZDMass==80) g_zd80zn->Fill(DALINaI_fDoppCorEnergy[i]);
            if(ZDZet==30 && ZDMass==79) g_zd79zn->Fill(DALINaI_fDoppCorEnergy[i]);
            if(ZDZet==29 && ZDMass==79) g_zd79cu->Fill(DALINaI_fDoppCorEnergy[i]);
            if(ZDZet==29 && ZDMass==78) g_zd78cu->Fill(DALINaI_fDoppCorEnergy[i]);
            if(ZDZet==28 && ZDMass==78) g_zd78ni->Fill(DALINaI_fDoppCorEnergy[i]);
            if(ZDZet==28 && ZDMass==77) g_zd77ni->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==80) g_br81ga_zd80zn->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==79) g_br81ga_zd79zn->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==30 && BRMass==80 && ZDZet==30 && ZDMass==79) g_br80zn_zd79zn->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==79) g_br80zn_zd79cu->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==78) g_br80zn_zd78cu->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==29 && BRMass==79 && ZDZet==29 && ZDMass==78) g_br79cu_zd78cu->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==78) g_br79cu_zd78ni->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==77) g_br79cu_zd77ni->Fill(DALINaI_fDoppCorEnergy[i]);
            if(BRZet==29 && BRMass==78 && ZDZet==28 && ZDMass==77) g_br78cu_zd77ni->Fill(DALINaI_fDoppCorEnergy[i]);

            if(dalimult>=1 && dalimult<=1 &&ZDZet==30 && ZDMass==80) g_zd80zn_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&ZDZet==30 && ZDMass==79) g_zd79zn_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&ZDZet==29 && ZDMass==79) g_zd79cu_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&ZDZet==29 && ZDMass==78) g_zd78cu_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&ZDZet==28 && ZDMass==78) g_zd78ni_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&ZDZet==28 && ZDMass==77) g_zd77ni_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==80) g_br81ga_zd80zn_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==79) g_br81ga_zd79zn_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==30 && BRMass==80 && ZDZet==30 && ZDMass==79) g_br80zn_zd79zn_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==79) g_br80zn_zd79cu_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==78) g_br80zn_zd78cu_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==79 && ZDZet==29 && ZDMass==78) g_br79cu_zd78cu_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==78) g_br79cu_zd78ni_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==77) g_br79cu_zd77ni_m1->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=1 &&BRZet==29 && BRMass==78 && ZDZet==28 && ZDMass==77) g_br78cu_zd77ni_m1->Fill(DALINaI_fDoppCorEnergy[i]);

            if(dalimult>=1 && dalimult<=2 &&ZDZet==30 && ZDMass==80) g_zd80zn_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&ZDZet==30 && ZDMass==79) g_zd79zn_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&ZDZet==29 && ZDMass==79) g_zd79cu_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&ZDZet==29 && ZDMass==78) g_zd78cu_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&ZDZet==28 && ZDMass==78) g_zd78ni_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&ZDZet==28 && ZDMass==77) g_zd77ni_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==80) g_br81ga_zd80zn_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==79) g_br81ga_zd79zn_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==30 && BRMass==80 && ZDZet==30 && ZDMass==79) g_br80zn_zd79zn_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==79) g_br80zn_zd79cu_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==78) g_br80zn_zd78cu_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==79 && ZDZet==29 && ZDMass==78) g_br79cu_zd78cu_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==78) g_br79cu_zd78ni_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==77) g_br79cu_zd77ni_m2->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=2 &&BRZet==29 && BRMass==78 && ZDZet==28 && ZDMass==77) g_br78cu_zd77ni_m2->Fill(DALINaI_fDoppCorEnergy[i]);

            if(dalimult>=1 && dalimult<=3 &&ZDZet==30 && ZDMass==80) g_zd80zn_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&ZDZet==30 && ZDMass==79) g_zd79zn_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&ZDZet==29 && ZDMass==79) g_zd79cu_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&ZDZet==29 && ZDMass==78) g_zd78cu_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&ZDZet==28 && ZDMass==78) g_zd78ni_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&ZDZet==28 && ZDMass==77) g_zd77ni_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==80) g_br81ga_zd80zn_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==31 && BRMass==81 && ZDZet==30 && ZDMass==79) g_br81ga_zd79zn_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==30 && BRMass==80 && ZDZet==30 && ZDMass==79) g_br80zn_zd79zn_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==79) g_br80zn_zd79cu_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==30 && BRMass==80 && ZDZet==29 && ZDMass==78) g_br80zn_zd78cu_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==79 && ZDZet==29 && ZDMass==78) g_br79cu_zd78cu_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==78) g_br79cu_zd78ni_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==79 && ZDZet==28 && ZDMass==77) g_br79cu_zd77ni_m3->Fill(DALINaI_fDoppCorEnergy[i]);
            if(dalimult>=1 && dalimult<=3 &&BRZet==29 && BRMass==78 && ZDZet==28 && ZDMass==77) g_br78cu_zd77ni_m3->Fill(DALINaI_fDoppCorEnergy[i]);

         }
      }
   }

   TFile *fout = TFile::Open("histograms.root","recreate");

   ////for setting 1
   //pid_br->Write();
   //pid_br70ni->Write();
   //pid_br69co->Write();
   //pid_br68co->Write();

   //pid_zd->Write();
   //pid_zd69co->Write();
   //pid_zd67co->Write();
   //pid_zd68fe->Write();

   //g_zd69co->Write();
   //g_zd67co->Write();
   //g_zd68fe->Write();
   //g_br70ni_zd69co->Write();
   //g_br69co_zd69co->Write();
   //g_br68co_zd67co->Write();
   //g_br70ni_zd68fe->Write();
   //g_br69co_zd68fe->Write();

   //g_zd69co_m1->Write();
   //g_zd67co_m1->Write();
   //g_zd68fe_m1->Write();
   //g_br70ni_zd69co_m1->Write();
   //g_br69co_zd69co_m1->Write();
   //g_br68co_zd67co_m1->Write();
   //g_br70ni_zd68fe_m1->Write();
   //g_br69co_zd68fe_m1->Write();

   //g_zd69co_m2->Write();
   //g_zd67co_m2->Write();
   //g_zd68fe_m2->Write();
   //g_br70ni_zd69co_m2->Write();
   //g_br69co_zd69co_m2->Write();
   //g_br68co_zd67co_m2->Write();
   //g_br70ni_zd68fe_m2->Write();
   //g_br69co_zd68fe_m2->Write();

   //g_zd69co_m3->Write();
   //g_zd67co_m3->Write();
   //g_zd68fe_m3->Write();
   //g_br70ni_zd69co_m3->Write();
   //g_br69co_zd69co_m3->Write();
   //g_br68co_zd67co_m3->Write();
   //g_br70ni_zd68fe_m3->Write();
   //g_br69co_zd68fe_m3->Write();

   ////for setting 2
   //pid_br->Write();
   //pid_br75cu->Write();
   //pid_br74cu->Write();
   //pid_br74ni->Write();
   //pid_br73ni->Write();
   //pid_br72ni->Write();
   //pid_br73co->Write();
   //pid_br72co->Write();

   //pid_zd->Write();
   //pid_zd71co->Write();
   //pid_zd73co->Write();
   //pid_zd74ni->Write();

   //g_zd71co->Write();
   //g_zd73co->Write();
   //g_zd74ni->Write();
   //g_br75cu_zd71co->Write();
   //g_br75cu_zd73co->Write();
   //g_br75cu_zd74ni->Write();
   //g_br74cu_zd71co->Write();
   //g_br74ni_zd71co->Write();
   //g_br74ni_zd73co->Write();
   //g_br73ni_zd71co->Write();
   //g_br72ni_zd71co->Write();
   //g_br73co_zd71co->Write();
   //g_br72co_zd71co->Write();
   //g_br73co_zd73co->Write();

   //g_zd71co_m1->Write();
   //g_zd73co_m1->Write();
   //g_zd74ni_m1->Write();
   //g_br75cu_zd71co_m1->Write();
   //g_br75cu_zd73co_m1->Write();
   //g_br75cu_zd74ni_m1->Write();
   //g_br74cu_zd71co_m1->Write();
   //g_br74ni_zd71co_m1->Write();
   //g_br74ni_zd73co_m1->Write();
   //g_br73ni_zd71co_m1->Write();
   //g_br72ni_zd71co_m1->Write();
   //g_br73co_zd71co_m1->Write();
   //g_br72co_zd71co_m1->Write();
   //g_br73co_zd73co_m1->Write();

   //g_zd71co_m2->Write();
   //g_zd73co_m2->Write();
   //g_zd74ni_m2->Write();
   //g_br75cu_zd71co_m2->Write();
   //g_br75cu_zd73co_m2->Write();
   //g_br75cu_zd74ni_m2->Write();
   //g_br74cu_zd71co_m2->Write();
   //g_br74ni_zd71co_m2->Write();
   //g_br74ni_zd73co_m2->Write();
   //g_br73ni_zd71co_m2->Write();
   //g_br72ni_zd71co_m2->Write();
   //g_br73co_zd71co_m2->Write();
   //g_br72co_zd71co_m2->Write();
   //g_br73co_zd73co_m2->Write();

   //g_zd71co_m3->Write();
   //g_zd73co_m3->Write();
   //g_zd74ni_m3->Write();
   //g_br75cu_zd71co_m3->Write();
   //g_br75cu_zd73co_m3->Write();
   //g_br75cu_zd74ni_m3->Write();
   //g_br74cu_zd71co_m3->Write();
   //g_br74ni_zd71co_m3->Write();
   //g_br74ni_zd73co_m3->Write();
   //g_br73ni_zd71co_m3->Write();
   //g_br72ni_zd71co_m3->Write();
   //g_br73co_zd71co_m3->Write();
   //g_br72co_zd71co_m3->Write();
   //g_br73co_zd73co_m3->Write();

   //for setting 3
   pid_br->Write();
   pid_br81ga->Write();
   pid_br80zn->Write();
   pid_br79cu->Write();
   pid_br78cu->Write();

   pid_zd->Write();
   pid_zd80zn->Write();
   pid_zd79zn->Write();
   pid_zd79cu->Write();
   pid_zd78cu->Write();
   pid_zd78ni->Write();
   pid_zd77ni->Write();

   g_zd80zn->Write();
   g_zd79zn->Write();
   g_zd79cu->Write();
   g_zd78cu->Write();
   g_zd78ni->Write();
   g_zd77ni->Write();
   g_br81ga_zd80zn->Write();
   g_br81ga_zd79zn->Write();
   g_br80zn_zd79zn->Write();
   g_br80zn_zd79cu->Write();
   g_br80zn_zd78cu->Write();
   g_br79cu_zd78cu->Write();
   g_br79cu_zd78ni->Write();
   g_br79cu_zd77ni->Write();
   g_br78cu_zd77ni->Write();

   g_zd80zn_m1->Write();
   g_zd79zn_m1->Write();
   g_zd79cu_m1->Write();
   g_zd78cu_m1->Write();
   g_zd78ni_m1->Write();
   g_zd77ni_m1->Write();
   g_br81ga_zd80zn_m1->Write();
   g_br81ga_zd79zn_m1->Write();
   g_br80zn_zd79zn_m1->Write();
   g_br80zn_zd79cu_m1->Write();
   g_br80zn_zd78cu_m1->Write();
   g_br79cu_zd78cu_m1->Write();
   g_br79cu_zd78ni_m1->Write();
   g_br79cu_zd77ni_m1->Write();
   g_br78cu_zd77ni_m1->Write();

   g_zd80zn_m2->Write();
   g_zd79zn_m2->Write();
   g_zd79cu_m2->Write();
   g_zd78cu_m2->Write();
   g_zd78ni_m2->Write();
   g_zd77ni_m2->Write();
   g_br81ga_zd80zn_m2->Write();
   g_br81ga_zd79zn_m2->Write();
   g_br80zn_zd79zn_m2->Write();
   g_br80zn_zd79cu_m2->Write();
   g_br80zn_zd78cu_m2->Write();
   g_br79cu_zd78cu_m2->Write();
   g_br79cu_zd78ni_m2->Write();
   g_br79cu_zd77ni_m2->Write();
   g_br78cu_zd77ni_m2->Write();

   g_zd80zn_m3->Write();
   g_zd79zn_m3->Write();
   g_zd79cu_m3->Write();
   g_zd78cu_m3->Write();
   g_zd78ni_m3->Write();
   g_zd77ni_m3->Write();
   g_br81ga_zd80zn_m3->Write();
   g_br81ga_zd79zn_m3->Write();
   g_br80zn_zd79zn_m3->Write();
   g_br80zn_zd79cu_m3->Write();
   g_br80zn_zd78cu_m3->Write();
   g_br79cu_zd78cu_m3->Write();
   g_br79cu_zd78ni_m3->Write();
   g_br79cu_zd77ni_m3->Write();
   g_br78cu_zd77ni_m3->Write();

   fout->Close();

   cout << endl;
}
