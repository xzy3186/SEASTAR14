#ifndef EventCalib_h
#define EventCalib_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TMatrixT.h>
#include <TVectorT.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include <TNamed.h>

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxfitdata = 3604;
const Int_t kMaxEventInfo = 1;

class EventCalib {
 public :
  TChain          *fChain;   //!pointer to the analyzed TTree or TChain

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

   EventCalib();
   virtual ~EventCalib();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual void     Init();
   virtual Bool_t   Notify();
};


#endif 
