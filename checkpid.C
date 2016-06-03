#define checkpid_cxx
#include "checkpid.h"
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

void checkpid::Loop()
{
   //   In a ROOT session, you can do:
   //      Root > .L checkpid.C
   //      Root > checkpid t
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
   fChain->SetBranchStatus("EventInfo.fBit",1);  // activate branchname
   fChain->SetBranchStatus("EventInfo.runnumber",1);  // activate branchname
   fChain->SetBranchStatus("zet",1);  // activate branchname
   fChain->SetBranchStatus("aoq",1);  // activate branchname
   fChain->SetBranchStatus("zetc",1);  // activate branchname
   fChain->SetBranchStatus("aoqc",1);  // activate branchname
   fChain->SetBranchStatus("DALINaI.fTimeTrueDoppCorEnergy",1);  // activate branchname
   fChain->SetBranchStatus("DALINaI",1);  // activate branchname
   fChain->SetBranchStatus("dalimult",1);  // activate branchname
   if (fChain == 0) return;

   //for setting 3
   TH2F *pid_br        = new TH2F("pid_br",        "PID__br",        500,2.30,2.80,500,23,33);
   TH2F *pid_zd_br83ga = new TH2F("pid_zd_br83ga", "PID__ZD_BR83Ga", 500,2.30,2.80,500,23,33);

   ////for setting 2
   //TH2F *pid_br         = new TH2F("pid_br",         "PID__br",         500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br76zn1 = new TH2F("pid_zd_br76zn1", "PID__ZD_BR76Zn1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br75cu1 = new TH2F("pid_zd_br75cu1", "PID__ZD_BR75Cu1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br74ni1 = new TH2F("pid_zd_br74ni1", "PID__ZD_BR74Ni1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br73co1 = new TH2F("pid_zd_br73co1", "PID__ZD_BR73Co1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br76zn2 = new TH2F("pid_zd_br76zn2", "PID__ZD_BR76Zn2", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br75cu2 = new TH2F("pid_zd_br75cu2", "PID__ZD_BR75Cu2", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br74ni2 = new TH2F("pid_zd_br74ni2", "PID__ZD_BR74Ni2", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br73co2 = new TH2F("pid_zd_br73co2", "PID__ZD_BR73Co2", 500,2.30,2.80,500,23,33);
   //
   ////for setting 1
   //TH2F *pid_br1        = new TH2F("pid_br1",        "PID__br1",       500,2.30,2.80,500,23,33);
   //TH2F *pid_br2        = new TH2F("pid_br2",        "PID__br2",       500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br70ni1 = new TH2F("pid_zd_br70ni1", "PID_ZD_BR70Ni1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br69co1 = new TH2F("pid_zd_br69co1", "PID_ZD_BR69Co1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br68fe1 = new TH2F("pid_zd_br68fe1", "PID_ZD_BR68Fe1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br67mn1 = new TH2F("pid_zd_br67mn1", "PID_ZD_BR67Mn1", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br70ni2 = new TH2F("pid_zd_br70ni2", "PID_ZD_BR70Ni2", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br69co2 = new TH2F("pid_zd_br69co2", "PID_ZD_BR69Co2", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br68fe2 = new TH2F("pid_zd_br68fe2", "PID_ZD_BR68Fe2", 500,2.30,2.80,500,23,33);
   //TH2F *pid_zd_br67mn2 = new TH2F("pid_zd_br67mn2", "PID_ZD_BR67Mn2", 500,2.30,2.80,500,23,33);

   Long64_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (jentry%1000==0)
         cout << jentry << "/" << nentries << " ("
            << Form("%0.2f",float(jentry)/float(nentries)*100)
            << " %)\r" << flush;

      // -- BIGRIPS PID --
      Int_t BRZet=0, BRMass=0;

      //for setting 3
      if(zet[0]>30.5 && zet[0]<31.5){
         if(aoq[0]>2.670 && aoq[0]<2.680){ BRZet=31 ; BRMass=83; }
      }

      ////for setting 2
      //if(zet[0]>29.3 && zet[0]<30.6){
      //   if(aoq[0]>2.512 && aoq[0]<2.523){ BRZet=30 ; BRMass=76; }
      //}
      //if(zet[0]>28.2 && zet[0]<29.8){
      //   if(aoq[0]>2.564 && aoq[0]<2.577){ BRZet=29 ; BRMass=75; }
      //}
      //if(zet[0]>27.4 && zet[0]<28.5){
      //   if(aoq[0]>2.620 && aoq[0]<2.632){ BRZet=28 ; BRMass=74; }
      //}
      //if(zet[0]>26.5 && zet[0]<27.6){
      //   if(aoq[0]>2.681 && aoq[0]<2.693){ BRZet=27 ; BRMass=73; }
      //}

      ////for setting 1
      //if(EventInfo_runnumber[0]<=15){
      //   if(zet[0]>27.5 && zet[0]<28.6){
      //      if(aoq[0]>2.503 && aoq[0]<2.514){ BRZet=28 ; BRMass=70; }
      //   }
      //   if(zet[0]>26.6 && zet[0]<27.7){
      //      if(aoq[0]>2.561 && aoq[0]<2.571){ BRZet=27 ; BRMass=69; }
      //   }
      //   if(zet[0]>25.7 && zet[0]<26.6){
      //      if(aoq[0]>2.618 && aoq[0]<2.633){ BRZet=26 ; BRMass=68; }
      //   }
      //   if(zet[0]>24.6 && zet[0]<25.6){
      //      if(aoq[0]>2.686 && aoq[0]<2.698){ BRZet=25 ; BRMass=67; }
      //   }
      //}else if(EventInfo_runnumber[0]>15){
      //   if(zet[0]>27.5 && zet[0]<28.6){
      //      if(aoq[0]>2.480 && aoq[0]<2.490){ BRZet=28 ; BRMass=70; }
      //   }
      //   if(zet[0]>26.4 && zet[0]<27.8){
      //      if(aoq[0]>2.534 && aoq[0]<2.546){ BRZet=27 ; BRMass=69; }
      //   }
      //   if(zet[0]>25.4 && zet[0]<26.8){
      //      if(aoq[0]>2.593 && aoq[0]<2.605){ BRZet=26 ; BRMass=68; }
      //   }
      //   if(zet[0]>24.6 && zet[0]<25.8){
      //      if(aoq[0]>2.658 && aoq[0]<2.669){ BRZet=25 ; BRMass=67; }
      //   }
      //}

      //for setting 3
      Int_t fBit = EventInfo_fBit[0];
      //cout << fBit <<endl;
      if(fBit==3 || fBit==7 || fBit==11 || fBit==15){
         pid_br->Fill(aoq[0],zet[0]);
         if(BRZet==31 && BRMass==83){ pid_zd_br83ga->Fill(aoqc[3],zetc[3]); }
      }

      ////for setting 2
      //if(EventInfo_runnumber[0]>102&&EventInfo_runnumber[0]<=108){
      //   Int_t fBit = EventInfo_fBit[0];
      //   //cout << fBit <<endl;
      //   if(fBit==3 || fBit==7 || fBit==11 || fBit==15){
      //      //cout<<"come here"<<endl;
      //      pid_br->Fill(aoq[0],zet[0]);
      //      if(BRZet==30 && BRMass==76){ pid_zd_br76zn1->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==29 && BRMass==75){ pid_zd_br75cu1->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==28 && BRMass==74){ pid_zd_br74ni1->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==27 && BRMass==73){ pid_zd_br73co1->Fill(aoqc[2],zet[2]); }
      //   }
      //}else if(EventInfo_runnumber[0]>108){
      //   Int_t fBit = EventInfo_fBit[0];
      //   //cout << fBit <<endl;
      //   if(fBit==3 || fBit==7 || fBit==11 || fBit==15){
      //      //cout<<"come here"<<endl;
      //      pid_br->Fill(aoq[0],zet[0]);
      //      if(BRZet==30 && BRMass==76){ pid_zd_br76zn2->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==29 && BRMass==75){ pid_zd_br75cu2->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==28 && BRMass==74){ pid_zd_br74ni2->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==27 && BRMass==73){ pid_zd_br73co2->Fill(aoqc[2],zet[2]); }
      //   }
      //}

      ////for setting 1
      //if(EventInfo_runnumber[0]<=15){
      //   if(fBit==3 || fBit==7 || fBit==11 || fBit==15){
      //      //cout<<"come here"<<endl;
      //      pid_br1->Fill(aoq[0],zet[0]);
      //      if(BRZet==28 && BRMass==70){ pid_zd_br70ni1->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==27 && BRMass==69){ pid_zd_br69co1->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==26 && BRMass==68){ pid_zd_br68fe1->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==25 && BRMass==67){ pid_zd_br67mn1->Fill(aoqc[2],zet[2]); }
      //   }
      //}else if(EventInfo_runnumber[0]>15){
      //   if(fBit==3 || fBit==7 || fBit==11 || fBit==15){
      //      //cout<<"come here"<<endl;
      //      pid_br2->Fill(aoq[0],zet[0]);
      //      if(BRZet==28 && BRMass==70){ pid_zd_br70ni2->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==27 && BRMass==69){ pid_zd_br69co2->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==26 && BRMass==68){ pid_zd_br68fe2->Fill(aoqc[2],zet[2]); }
      //      if(BRZet==25 && BRMass==67){ pid_zd_br67mn2->Fill(aoqc[2],zet[2]); }
      //   }
      //}
   }

   TFile *fout = TFile::Open("histograms.root","recreate");

   //for setting 3
   pid_br->Write();
   pid_zd_br83ga->Write();

   ////for setting 2
   //pid_br->Write();
   //pid_zd_br76zn1->Write();
   //pid_zd_br75cu1->Write();
   //pid_zd_br74ni1->Write();
   //pid_zd_br73co1->Write();
   //pid_zd_br76zn2->Write();
   //pid_zd_br75cu2->Write();
   //pid_zd_br74ni2->Write();
   //pid_zd_br73co2->Write();

   ////for setting 1
   //pid_br1->Write();
   //pid_zd_br70ni1->Write();
   //pid_zd_br69co1->Write();
   //pid_zd_br68fe1->Write();
   //pid_zd_br67mn1->Write();
   //pid_br2->Write();
   //pid_zd_br70ni2->Write();
   //pid_zd_br69co2->Write();
   //pid_zd_br68fe2->Write();
   //pid_zd_br67mn2->Write();

   fout->Close();

   cout << endl;
}
