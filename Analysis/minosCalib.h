//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 30 17:35:19 2014 by ROOT version 5.34/11
// from TTree tree/ridf tree
// found on file: ../rootfiles/psp140040_MINOSVDrift.root
//////////////////////////////////////////////////////////

#ifndef minosCalib_h
#define minosCalib_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include <TNamed.h>

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxfitdata = 3604;
const Int_t kMaxEventInfo = 1;

class minosCalib {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           fitdata_;
   UInt_t          fitdata_fUniqueID[kMaxfitdata];   //[fitdata_]
   UInt_t          fitdata_fBits[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_x_mm[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_y_mm[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_t_ns[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_z_mm[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_Chargemax[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_n_Cluster[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_n_Pads[kMaxfitdata];   //[fitdata_]
   Double_t        fitdata_Chi2[kMaxfitdata];   //[fitdata_]
   Int_t           Rpadnumber_max;
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

   // List of branches
   TBranch        *b_fitdata_;   //!
   TBranch        *b_fitdata_fUniqueID;   //!
   TBranch        *b_fitdata_fBits;   //!
   TBranch        *b_fitdata_x_mm;   //!
   TBranch        *b_fitdata_y_mm;   //!
   TBranch        *b_fitdata_t_ns;   //!
   TBranch        *b_fitdata_z_mm;   //!
   TBranch        *b_fitdata_Chargemax;   //!
   TBranch        *b_fitdata_n_Cluster;   //!
   TBranch        *b_fitdata_n_Pads;   //!
   TBranch        *b_fitdata_Chi2;   //!
   TBranch        *b_Rpadnumber_max;   //!
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

   minosCalib(TTree *tree=0);
   virtual ~minosCalib();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef minosCalib_cxx
minosCalib::minosCalib(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../rootfiles/psp140040_MINOSVDrift.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../rootfiles/psp140040_MINOSVDrift.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

minosCalib::~minosCalib()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t minosCalib::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t minosCalib::LoadTree(Long64_t entry)
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

void minosCalib::Init(TTree *tree)
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

   fChain->SetBranchAddress("fitdata", &fitdata_, &b_fitdata_);
   fChain->SetBranchAddress("fitdata.fUniqueID", fitdata_fUniqueID, &b_fitdata_fUniqueID);
   fChain->SetBranchAddress("fitdata.fBits", fitdata_fBits, &b_fitdata_fBits);
   fChain->SetBranchAddress("fitdata.x_mm", fitdata_x_mm, &b_fitdata_x_mm);
   fChain->SetBranchAddress("fitdata.y_mm", fitdata_y_mm, &b_fitdata_y_mm);
   fChain->SetBranchAddress("fitdata.t_ns", fitdata_t_ns, &b_fitdata_t_ns);
   fChain->SetBranchAddress("fitdata.z_mm", fitdata_z_mm, &b_fitdata_z_mm);
   fChain->SetBranchAddress("fitdata.Chargemax", fitdata_Chargemax, &b_fitdata_Chargemax);
   fChain->SetBranchAddress("fitdata.n_Cluster", fitdata_n_Cluster, &b_fitdata_n_Cluster);
   fChain->SetBranchAddress("fitdata.n_Pads", fitdata_n_Pads, &b_fitdata_n_Pads);
   fChain->SetBranchAddress("fitdata.Chi2", fitdata_Chi2, &b_fitdata_Chi2);
   fChain->SetBranchAddress("Rpadnumber_max", &Rpadnumber_max, &b_Rpadnumber_max);
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
   Notify();
}

Bool_t minosCalib::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void minosCalib::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t minosCalib::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef minosCalib_cxx
