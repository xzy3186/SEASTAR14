#ifndef EventCut_h
#define EventCut_h

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
const Int_t kMaxEventInfo = 1;
const Int_t kMaxBigRIPSPlastic = 5;
const Int_t kMaxBigRIPSIC = 4;
const Int_t kMaxBigRIPSFocalPlane = 8;
const Int_t kMaxDALINaI = 188;
const Int_t kMaxBigRIPSRIPS = 4;
const Int_t kMaxBigRIPSTOF = 4;
const Int_t kMaxfitdata = 2504;
const Int_t kMaxdata_result = 136;
const Int_t kMaxBigRIPSBeam = 4;
const Int_t kMaxBigRIPSPPAC = 34;

class EventCut {
 public :
  TChain          *fChainCut;   //!pointer to the analyzed TTree or TChain

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
   Double_t        DALINaI_fTimeTrueTime[kMaxDALINaI];   //[DALINaI_]
   Double_t        DALINaI_fTimeTrueTimeOffseted[kMaxDALINaI];   //[DALINaI_]
   Int_t           BigRIPSBeam_;
   UInt_t          BigRIPSBeam_fUniqueID[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   UInt_t          BigRIPSBeam_fBits[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Int_t           BigRIPSBeam_id[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Int_t           BigRIPSBeam_fpl[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   TString         BigRIPSBeam_name[kMaxBigRIPSBeam];
   Int_t           BigRIPSBeam_fDataState[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_brho[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_aoq[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_zet[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_beta[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_clight[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Double_t        BigRIPSBeam_mnucleon[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   Int_t           BigRIPSBeam_nrips[kMaxBigRIPSBeam];   //[BigRIPSBeam_]
   TString         BigRIPSBeam_ripsname[2][kMaxBigRIPSBeam];
   TString         BigRIPSBeam_tofname[kMaxBigRIPSBeam];
   TString         BigRIPSBeam_icname[kMaxBigRIPSBeam];
   Int_t           BigRIPSPPAC_;
   UInt_t          BigRIPSPPAC_fUniqueID[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   UInt_t          BigRIPSPPAC_fBits[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_id[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fpl[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   TString         BigRIPSPPAC_name[kMaxBigRIPSPPAC];
   Int_t           BigRIPSPPAC_fDataState[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_xzpos[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_yzpos[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTX1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTX2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTY1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTY2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fTARaw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQX1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQX2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQY1Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQY2Raw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           BigRIPSPPAC_fQARaw[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTX1[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTX2[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTY1[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTY2[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTA[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTSumX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTSumY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTDiffX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fTDiffY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Double_t        BigRIPSPPAC_fY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Bool_t          BigRIPSPPAC_fFiredX[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Bool_t          BigRIPSPPAC_fFiredY[kMaxBigRIPSPPAC];   //[BigRIPSPPAC_]
   Int_t           dalimultwotime;
   Int_t           dalimult;
   Int_t           dalitimetruemult;
   Int_t           dalimultthres;
   Int_t           dalitimetruemultthres;
   Int_t           evtOrig;
   Double_t        VDrift;
   Double_t        DelayTrig;
   Double_t        x_vertex;
   Double_t        y_vertex;
   Double_t        z_vertex;
   Double_t        r_vertex;
   Double_t        x_vertexPPAC;
   Double_t        y_vertexPPAC;
   Double_t        z_vertexPPAC;
   Double_t        x_vertexDSSSD;
   Double_t        y_vertexDSSSD;
   Double_t        z_vertexDSSSD;
   Double_t        phi_vertex;
   Double_t        thetaz1;
   Double_t        thetaz2;
   Double_t        parFit_1[4];
   Double_t        parFit_2[4];
//   Double_t        beta;
//   Double_t        beta1;
//   Double_t        beta2;
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
   Int_t           trackNbr_FINAL;
   Int_t           data_result_;
   UInt_t          data_result_fUniqueID[kMaxdata_result];   //[data_result_]
   UInt_t          data_result_fBits[kMaxdata_result];   //[data_result_]
   Double_t        data_result_x_mm[kMaxdata_result];   //[data_result_]
   Double_t        data_result_y_mm[kMaxdata_result];   //[data_result_]
   Double_t        data_result_z_mm[kMaxdata_result];   //[data_result_]
   Double_t        data_result_Chargemax[kMaxdata_result];   //[data_result_]
   Double_t        data_result_n_Cluster[kMaxdata_result];   //[data_result_]
   Double_t        data_result_n_Pads[kMaxdata_result];   //[data_result_]
   Double_t        data_result_z_max[kMaxdata_result];   //[data_result_]

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
   TBranch        *b_DALINaI_fXPos;   //!
   TBranch        *b_DALINaI_fYPos;   //!
   TBranch        *b_DALINaI_fZPos;   //!
   TBranch        *b_DALINaI_theta;   //!
   TBranch        *b_DALINaI_costheta;   //!
   TBranch        *b_DALINaI_fEnergy;   //!
   TBranch        *b_DALINaI_fDoppCorEnergy;   //!
   TBranch        *b_DALINaI_fEnergyWithoutT;   //!
   TBranch        *b_DALINaI_fTime;   //!
   TBranch        *b_DALINaI_fTimeOffseted;   //!
   TBranch        *b_DALINaI_fTimeTrueEnergy;   //!
   TBranch        *b_DALINaI_fTimeTrueDoppCorEnergy;   //!
   TBranch        *b_DALINaI_fTimeTrueTime;   //!
   TBranch        *b_DALINaI_fTimeTrueTimeOffseted;   //!
   TBranch        *b_dalimultwotime;   //!
   TBranch        *b_dalimult;   //!
   TBranch        *b_dalitimetruemult;   //!
   TBranch        *b_dalimultthres;   //!
   TBranch        *b_dalitimetruemultthres;   //!
   TBranch        *b_BigRIPSBeam_;   //!
   TBranch        *b_BigRIPSBeam_fUniqueID;   //!
   TBranch        *b_BigRIPSBeam_fBits;   //!
   TBranch        *b_BigRIPSBeam_id;   //!
   TBranch        *b_BigRIPSBeam_fpl;   //!
   TBranch        *b_BigRIPSBeam_name;   //!
   TBranch        *b_BigRIPSBeam_fDataState;   //!
   TBranch        *b_BigRIPSBeam_brho;   //!
   TBranch        *b_BigRIPSBeam_aoq;   //!
   TBranch        *b_BigRIPSBeam_zet;   //!
   TBranch        *b_BigRIPSBeam_beta;   //!
   TBranch        *b_BigRIPSBeam_clight;   //!
   TBranch        *b_BigRIPSBeam_mnucleon;   //!
   TBranch        *b_BigRIPSBeam_nrips;   //!
   TBranch        *b_BigRIPSBeam_ripsname;   //!
   TBranch        *b_BigRIPSBeam_tofname;   //!
   TBranch        *b_BigRIPSBeam_icname;   //!
   TBranch        *b_BigRIPSPPAC_;   //!
   TBranch        *b_BigRIPSPPAC_fUniqueID;   //!
   TBranch        *b_BigRIPSPPAC_fBits;   //!
   TBranch        *b_BigRIPSPPAC_id;   //!
   TBranch        *b_BigRIPSPPAC_fpl;   //!
   TBranch        *b_BigRIPSPPAC_name;   //!
   TBranch        *b_BigRIPSPPAC_fDataState;   //!
   TBranch        *b_BigRIPSPPAC_xzpos;   //!
   TBranch        *b_BigRIPSPPAC_yzpos;   //!
   TBranch        *b_BigRIPSPPAC_fTX1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTX2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTY1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTY2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fTARaw;   //!
   TBranch        *b_BigRIPSPPAC_fQX1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQX2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQY1Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQY2Raw;   //!
   TBranch        *b_BigRIPSPPAC_fQARaw;   //!
   TBranch        *b_BigRIPSPPAC_fTX1;   //!
   TBranch        *b_BigRIPSPPAC_fTX2;   //!
   TBranch        *b_BigRIPSPPAC_fTY1;   //!
   TBranch        *b_BigRIPSPPAC_fTY2;   //!
   TBranch        *b_BigRIPSPPAC_fTA;   //!
   TBranch        *b_BigRIPSPPAC_fTSumX;   //!
   TBranch        *b_BigRIPSPPAC_fTSumY;   //!
   TBranch        *b_BigRIPSPPAC_fTDiffX;   //!
   TBranch        *b_BigRIPSPPAC_fTDiffY;   //!
   TBranch        *b_BigRIPSPPAC_fX;   //!
   TBranch        *b_BigRIPSPPAC_fY;   //!
   TBranch        *b_BigRIPSPPAC_fFiredX;   //!
   TBranch        *b_BigRIPSPPAC_fFiredY;   //!
   TBranch        *b_evtOrig;   //!
   TBranch        *b_VDrift;   //!
   TBranch        *b_DelayTrig;   //!
   TBranch        *b_x_vertex;   //!
   TBranch        *b_y_vertex;   //!
   TBranch	  	  *b_Zv;
   TBranch        *b_r_vertex;   //!
   TBranch        *b_x_vertexPPAC;   //!
   TBranch        *b_y_vertexPPAC;   //!
   TBranch        *b_z_vertexPPAC;   //!
   TBranch        *b_x_vertexDSSSD;   //!
   TBranch        *b_y_vertexDSSSD;   //!
   TBranch        *b_z_vertexDSSSD;   //!
   TBranch        *b_phi_vertex;   //!
   TBranch        *b_thetaz1;   //!
   TBranch        *b_thetaz2;   //!
   TBranch        *b_parFit_1;   //!
   TBranch        *b_parFit_2;   //!
//   TBranch        *b_beta;   //!
//   TBranch        *b_beta1;   //!
//   TBranch        *b_beta2;   //!
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
   TBranch        *b_trackNbr_FINAL;   //!
   TBranch        *b_data_result_;   //!
   TBranch        *b_data_result_fUniqueID;   //!
   TBranch        *b_data_result_fBits;   //!
   TBranch        *b_data_result_x_mm;   //!
   TBranch        *b_data_result_y_mm;   //!
   TBranch        *b_data_result_z_mm;   //!
   TBranch        *b_data_result_Chargemax;   //!
   TBranch        *b_data_result_n_Cluster;   //!
   TBranch        *b_data_result_n_Pads;   //!
   TBranch        *b_data_result_z_max;   //!

   
   EventCut();
   virtual ~EventCut();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual void     Init();
   virtual Bool_t   Notify();
};


#endif 
