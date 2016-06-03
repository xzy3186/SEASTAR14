#ifndef EventMINOS_h
#define EventMINOS_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TMatrixT.h>
#include <TVectorT.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include <TNamed.h>


class EventMINOS {
 public :
  TChain          *fChainMINOS;   //!pointer to the analyzed TTree or TChain


   // Declaration of leaf types
   Double_t	  x_vertex;
   Double_t	  y_vertex;
   Double_t	  z_vertex;
   Double_t	  r_vertex;
   Double_t	  phi_vertex;
   Double_t	  thetaz1;
   Double_t	  thetaz2;

   // List of branches
   TBranch	  *b_Xv;
   TBranch	  *b_Yv;
   TBranch	  *b_Zv;
   TBranch	  *b_Rv;
   TBranch	  *b_Phiv;
   TBranch	  *b_Thetaz1;
   TBranch	  *b_Thetaz2;

   EventMINOS();
   virtual ~EventMINOS();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual void     Init();
   virtual Bool_t   Notify();
};


#endif 
