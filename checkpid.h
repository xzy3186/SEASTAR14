//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri May  9 18:49:13 2014 by ROOT version 5.34/18
// from TTree tree/tree
// found on file: rootfiles/brzddali220.root
//////////////////////////////////////////////////////////

#ifndef checkpid_h
#define checkpid_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include <TNamed.h>
#include "./src/include/TArtEventInfo.hh"
#include "./src/include/TArtDataObject.hh"
#include "./src/include/TArtDALINaI.hh"

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxEventInfo = 1;
const Int_t kMaxDALINaI = 159;

class checkpid {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           EventInfo_;
   UInt_t          EventInfo_fUniqueID[kMaxEventInfo];   //[EventInfo_]
   UInt_t          EventInfo_fBits[kMaxEventInfo];   //[EventInfo_]
   TString         EventInfo_fName[kMaxEventInfo];
   TString         EventInfo_fTitle[kMaxEventInfo];
   Int_t           EventInfo_runnumber[kMaxEventInfo];   //[EventInfo_]
   Int_t           EventInfo_eventnumber[kMaxEventInfo];   //[EventInfo_]
   TString         EventInfo_subsysname[kMaxEventInfo];
   ULong64_t       EventInfo_timestamp[kMaxEventInfo];   //[EventInfo_]
   Int_t           EventInfo_comp_val[kMaxEventInfo];   //[EventInfo_]
   UInt_t          EventInfo_fBit[kMaxEventInfo];   //[EventInfo_]
   Int_t           DALINaI_;
   UInt_t          DALINaI_fUniqueID[kMaxDALINaI];   //[DALINaI_]
   UInt_t          DALINaI_fBits[kMaxDALINaI];   //[DALINaI_]
   Int_t           DALINaI_id[kMaxDALINaI];   //[DALINaI_]
   Int_t           DALINaI_fpl[kMaxDALINaI];   //[DALINaI_]
   TString         DALINaI_name[kMaxDALINaI];
   Int_t           DALINaI_fDataState[kMaxDALINaI];   //[DALINaI_]
   Int_t           DALINaI_fADC[kMaxDALINaI];   //[DALINaI_]
   Int_t           DALINaI_fTDC[kMaxDALINaI];   //[DALINaI_]
   Int_t           DALINaI_layer[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_theta[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fXPos[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fYPos[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fZPos[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_costheta[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fEnergy[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fDoppCorEnergy[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fEnergyWithoutT[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTime[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTimeOffseted[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTimeTrueEnergy[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTimeTrueDoppCorEnergy[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTimeTrueDoppCorVertexEnergy[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTimeTrueTime[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTimeTrueTimeOffseted[kMaxDALINaI];   //[DALINaI_]
   Double_t        xtar;
   Double_t        ytar;
   Int_t           fgoodppacfocus[12];
   Int_t           fgoodppacfocusor[12];
   Double_t        F3X;
   Double_t        F3A;
   Double_t        F3Y;
   Double_t        F3B;
   Double_t        F5X;
   Double_t        F5A;
   Double_t        F5Y;
   Double_t        F5B;
   Double_t        F7X;
   Double_t        F7A;
   Double_t        F7Y;
   Double_t        F7B;
   Double_t        F8X;
   Double_t        F8A;
   Double_t        F8Y;
   Double_t        F8B;
   Double_t        F9X;
   Double_t        F9A;
   Double_t        F9Y;
   Double_t        F9B;
   Double_t        F11X;
   Double_t        F11A;
   Double_t        F11Y;
   Double_t        F11B;
   Double_t        tof[4];
   Double_t        beta[4];
   Double_t        zet[4];
   Double_t        aoq[4];
   Double_t        zetc[4];
   Double_t        aoqc[4];
   Double_t        delta[4];
   Int_t           dalimultwotime;
   Int_t           dalimult;
   Int_t           dalitimetruemult;
   Int_t           dalimultthres;
   Int_t           dalitimetruemultthres;

   // List of branches
   TBranch        *b_EventInfo_;   //!
   TBranch        *b_EventInfo_fUniqueID;   //!
   TBranch        *b_EventInfo_fBits;   //!
   TBranch        *b_EventInfo_fName;   //!
   TBranch        *b_EventInfo_fTitle;   //!
   TBranch        *b_EventInfo_runnumber;   //!
   TBranch        *b_EventInfo_eventnumber;   //!
   TBranch        *b_EventInfo_subsysname;   //!
   TBranch        *b_EventInfo_timestamp;   //!
   TBranch        *b_EventInfo_comp_val;   //!
   TBranch        *b_EventInfo_fBit;   //!
   TBranch        *b_DALINaI_;   //!
   TBranch        *b_DALINaI_fUniqueID;   //!
   TBranch        *b_DALINaI_fBits;   //!
   TBranch        *b_DALINaI_id;   //!
   TBranch        *b_DALINaI_fpl;   //!
   TBranch        *b_DALINaI_name;   //!
   TBranch        *b_DALINaI_fDataState;   //!
   TBranch        *b_DALINaI_fADC;   //!
   TBranch        *b_DALINaI_fTDC;   //!
   TBranch        *b_DALINaI_layer;   //!
   TBranch        *b_DALINaI_theta;   //!
   TBranch        *b_DALINaI_fXPos;   //!
   TBranch        *b_DALINaI_fYPos;   //!
   TBranch        *b_DALINaI_fZPos;   //!
   TBranch        *b_DALINaI_costheta;   //!
   TBranch        *b_DALINaI_fEnergy;   //!
   TBranch        *b_DALINaI_fDoppCorEnergy;   //!
   TBranch        *b_DALINaI_fEnergyWithoutT;   //!
   TBranch        *b_DALINaI_fTime;   //!
   TBranch        *b_DALINaI_fTimeOffseted;   //!
   TBranch        *b_DALINaI_fTimeTrueEnergy;   //!
   TBranch        *b_DALINaI_fTimeTrueDoppCorEnergy;   //!
   TBranch        *b_DALINaI_fTimeTrueDoppCorVertexEnergy;   //!
   TBranch        *b_DALINaI_fTimeTrueTime;   //!
   TBranch        *b_DALINaI_fTimeTrueTimeOffseted;   //!
   TBranch        *b_fXTar;   //!
   TBranch        *b_fYTar;   //!
   TBranch        *b_fGoodPPACFocus;   //!
   TBranch        *b_fGoodPPACFocusOr;   //!
   TBranch        *b_F3X;   //!
   TBranch        *b_F3A;   //!
   TBranch        *b_F3Y;   //!
   TBranch        *b_F3B;   //!
   TBranch        *b_F5X;   //!
   TBranch        *b_F5A;   //!
   TBranch        *b_F5Y;   //!
   TBranch        *b_F5B;   //!
   TBranch        *b_F7X;   //!
   TBranch        *b_F7A;   //!
   TBranch        *b_F7Y;   //!
   TBranch        *b_F7B;   //!
   TBranch        *b_F8X;   //!
   TBranch        *b_F8A;   //!
   TBranch        *b_F8Y;   //!
   TBranch        *b_F8B;   //!
   TBranch        *b_F9X;   //!
   TBranch        *b_F9A;   //!
   TBranch        *b_F9Y;   //!
   TBranch        *b_F9B;   //!
   TBranch        *b_F11X;   //!
   TBranch        *b_F11A;   //!
   TBranch        *b_F11Y;   //!
   TBranch        *b_F11B;   //!
   TBranch        *b_tof;   //!
   TBranch        *b_beta;   //!
   TBranch        *b_zet;   //!
   TBranch        *b_aoq;   //!
   TBranch        *b_zetc;   //!
   TBranch        *b_aoqc;   //!
   TBranch        *b_delta;   //!
   TBranch        *b_dalimultwotime;   //!
   TBranch        *b_dalimult;   //!
   TBranch        *b_dalitimetruemult;   //!
   TBranch        *b_dalimultthres;   //!
   TBranch        *b_dalitimetruemultthres;   //!

   checkpid(TTree *tree=0);
   virtual ~checkpid();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef checkpid_cxx
checkpid::checkpid(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("rootfiles/brzddali220.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("rootfiles/brzddali220.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

checkpid::~checkpid()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t checkpid::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t checkpid::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void checkpid::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EventInfo", &EventInfo_, &b_EventInfo_);
   fChain->SetBranchAddress("EventInfo.fUniqueID", EventInfo_fUniqueID, &b_EventInfo_fUniqueID);
   fChain->SetBranchAddress("EventInfo.fBits", EventInfo_fBits, &b_EventInfo_fBits);
   fChain->SetBranchAddress("EventInfo.fName", EventInfo_fName, &b_EventInfo_fName);
   fChain->SetBranchAddress("EventInfo.fTitle", EventInfo_fTitle, &b_EventInfo_fTitle);
   fChain->SetBranchAddress("EventInfo.runnumber", EventInfo_runnumber, &b_EventInfo_runnumber);
   fChain->SetBranchAddress("EventInfo.eventnumber", EventInfo_eventnumber, &b_EventInfo_eventnumber);
   fChain->SetBranchAddress("EventInfo.subsysname", EventInfo_subsysname, &b_EventInfo_subsysname);
   fChain->SetBranchAddress("EventInfo.timestamp", EventInfo_timestamp, &b_EventInfo_timestamp);
   fChain->SetBranchAddress("EventInfo.comp_val", EventInfo_comp_val, &b_EventInfo_comp_val);
   fChain->SetBranchAddress("EventInfo.fBit", EventInfo_fBit, &b_EventInfo_fBit);
   fChain->SetBranchAddress("DALINaI", &DALINaI_, &b_DALINaI_);
   fChain->SetBranchAddress("DALINaI.fUniqueID", DALINaI_fUniqueID, &b_DALINaI_fUniqueID);
   fChain->SetBranchAddress("DALINaI.fBits", DALINaI_fBits, &b_DALINaI_fBits);
   fChain->SetBranchAddress("DALINaI.id", DALINaI_id, &b_DALINaI_id);
   fChain->SetBranchAddress("DALINaI.fpl", DALINaI_fpl, &b_DALINaI_fpl);
   fChain->SetBranchAddress("DALINaI.name", DALINaI_name, &b_DALINaI_name);
   fChain->SetBranchAddress("DALINaI.fDataState", DALINaI_fDataState, &b_DALINaI_fDataState);
   fChain->SetBranchAddress("DALINaI.fADC", DALINaI_fADC, &b_DALINaI_fADC);
   fChain->SetBranchAddress("DALINaI.fTDC", DALINaI_fTDC, &b_DALINaI_fTDC);
   fChain->SetBranchAddress("DALINaI.layer", DALINaI_layer, &b_DALINaI_layer);
   fChain->SetBranchAddress("DALINaI.theta", DALINaI_theta, &b_DALINaI_theta);
   fChain->SetBranchAddress("DALINaI.fXPos", DALINaI_fXPos, &b_DALINaI_fXPos);
   fChain->SetBranchAddress("DALINaI.fYPos", DALINaI_fYPos, &b_DALINaI_fYPos);
   fChain->SetBranchAddress("DALINaI.fZPos", DALINaI_fZPos, &b_DALINaI_fZPos);
   fChain->SetBranchAddress("DALINaI.costheta", DALINaI_costheta, &b_DALINaI_costheta);
   fChain->SetBranchAddress("DALINaI.fEnergy", DALINaI_fEnergy, &b_DALINaI_fEnergy);
   fChain->SetBranchAddress("DALINaI.fDoppCorEnergy", DALINaI_fDoppCorEnergy, &b_DALINaI_fDoppCorEnergy);
   fChain->SetBranchAddress("DALINaI.fEnergyWithoutT", DALINaI_fEnergyWithoutT, &b_DALINaI_fEnergyWithoutT);
   fChain->SetBranchAddress("DALINaI.fTime", DALINaI_fTime, &b_DALINaI_fTime);
   fChain->SetBranchAddress("DALINaI.fTimeOffseted", DALINaI_fTimeOffseted, &b_DALINaI_fTimeOffseted);
   fChain->SetBranchAddress("DALINaI.fTimeTrueEnergy", DALINaI_fTimeTrueEnergy, &b_DALINaI_fTimeTrueEnergy);
   fChain->SetBranchAddress("DALINaI.fTimeTrueDoppCorEnergy", DALINaI_fTimeTrueDoppCorEnergy, &b_DALINaI_fTimeTrueDoppCorEnergy);
   fChain->SetBranchAddress("DALINaI.fTimeTrueDoppCorVertexEnergy", DALINaI_fTimeTrueDoppCorVertexEnergy, &b_DALINaI_fTimeTrueDoppCorVertexEnergy);
   fChain->SetBranchAddress("DALINaI.fTimeTrueTime", DALINaI_fTimeTrueTime, &b_DALINaI_fTimeTrueTime);
   fChain->SetBranchAddress("DALINaI.fTimeTrueTimeOffseted", DALINaI_fTimeTrueTimeOffseted, &b_DALINaI_fTimeTrueTimeOffseted);
   fChain->SetBranchAddress("xtar", &xtar, &b_fXTar);
   fChain->SetBranchAddress("ytar", &ytar, &b_fYTar);
   fChain->SetBranchAddress("fgoodppacfocus", fgoodppacfocus, &b_fGoodPPACFocus);
   fChain->SetBranchAddress("fgoodppacfocusor", fgoodppacfocusor, &b_fGoodPPACFocusOr);
   fChain->SetBranchAddress("F3X", &F3X, &b_F3X);
   fChain->SetBranchAddress("F3A", &F3A, &b_F3A);
   fChain->SetBranchAddress("F3Y", &F3Y, &b_F3Y);
   fChain->SetBranchAddress("F3B", &F3B, &b_F3B);
   fChain->SetBranchAddress("F5X", &F5X, &b_F5X);
   fChain->SetBranchAddress("F5A", &F5A, &b_F5A);
   fChain->SetBranchAddress("F5Y", &F5Y, &b_F5Y);
   fChain->SetBranchAddress("F5B", &F5B, &b_F5B);
   fChain->SetBranchAddress("F7X", &F7X, &b_F7X);
   fChain->SetBranchAddress("F7A", &F7A, &b_F7A);
   fChain->SetBranchAddress("F7Y", &F7Y, &b_F7Y);
   fChain->SetBranchAddress("F7B", &F7B, &b_F7B);
   fChain->SetBranchAddress("F8X", &F8X, &b_F8X);
   fChain->SetBranchAddress("F8A", &F8A, &b_F8A);
   fChain->SetBranchAddress("F8Y", &F8Y, &b_F8Y);
   fChain->SetBranchAddress("F8B", &F8B, &b_F8B);
   fChain->SetBranchAddress("F9X", &F9X, &b_F9X);
   fChain->SetBranchAddress("F9A", &F9A, &b_F9A);
   fChain->SetBranchAddress("F9Y", &F9Y, &b_F9Y);
   fChain->SetBranchAddress("F9B", &F9B, &b_F9B);
   fChain->SetBranchAddress("F11X", &F11X, &b_F11X);
   fChain->SetBranchAddress("F11A", &F11A, &b_F11A);
   fChain->SetBranchAddress("F11Y", &F11Y, &b_F11Y);
   fChain->SetBranchAddress("F11B", &F11B, &b_F11B);
   fChain->SetBranchAddress("tof", tof, &b_tof);
   fChain->SetBranchAddress("beta", beta, &b_beta);
   fChain->SetBranchAddress("zet", zet, &b_zet);
   fChain->SetBranchAddress("aoq", aoq, &b_aoq);
   fChain->SetBranchAddress("zetc", zetc, &b_zetc);
   fChain->SetBranchAddress("aoqc", aoqc, &b_aoqc);
   fChain->SetBranchAddress("delta", delta, &b_delta);
   fChain->SetBranchAddress("dalimultwotime", &dalimultwotime, &b_dalimultwotime);
   fChain->SetBranchAddress("dalimult", &dalimult, &b_dalimult);
   fChain->SetBranchAddress("dalitimetruemult", &dalitimetruemult, &b_dalitimetruemult);
   fChain->SetBranchAddress("dalimultthres", &dalimultthres, &b_dalimultthres);
   fChain->SetBranchAddress("dalitimetruemultthres", &dalitimetruemultthres, &b_dalitimetruemultthres);
   Notify();
}

Bool_t checkpid::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void checkpid::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t checkpid::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef checkpid_cxx
