#include "EventMINOS.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using namespace std;

EventMINOS::EventMINOS() {
  fChainMINOS = new TChain();

  fChainMINOS->AddFile("../testMINOS.root",0,"tree");
  
  Init();
}

EventMINOS::~EventMINOS(){
}

Int_t EventMINOS::GetEntry(Long64_t entry){
  if (!fChainMINOS) return 0;
  return fChainMINOS->GetEntry(entry);
}

void EventMINOS::Init() {
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   //if (!tree) return;
   //fChainMINOS = tree;
   fChainMINOS->SetMakeClass(1);

   fChainMINOS->SetBranchAddress("x_vertex",&x_vertex,&b_Xv);
   fChainMINOS->SetBranchAddress("y_vertex",&y_vertex,&b_Yv);
   fChainMINOS->SetBranchAddress("z_vertex",&z_vertex,&b_Zv);
   fChainMINOS->SetBranchAddress("r_vertex",&r_vertex,&b_Rv);
   fChainMINOS->SetBranchAddress("phi_vertex",&phi_vertex,&b_Phiv); // angle between two tracks in 3D in degrees
   fChainMINOS->SetBranchAddress("thetaz1",&thetaz1,&b_Thetaz1); // angle between 1st track and z axis in 3D in degrees
   fChainMINOS->SetBranchAddress("thetaz2",&thetaz2,&b_Thetaz2);



   Notify();
}

Bool_t EventMINOS::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

