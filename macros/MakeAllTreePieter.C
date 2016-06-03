#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"
#include "TArtMINOSParameters.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibDALI.hh"
#include "TArtCalibMINOS.hh"

#include "TArtAnalyzedMINOS.hh"
#include "TArtTrackMINOS.hh"
#include "TArtVertexMINOS.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtEventInfo.hh"
#include "TArtPlastic.hh"
#include "TArtPPAC.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtFocalPlane.hh"
#include "TArtTOF.hh"
#include "TArtRIPS.hh"
#include "TArtBeam.hh"

#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "signal.h"
#include "fstream"
#include "iostream"

using namespace std;

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt(){
   printf("keyboard interrupt\n");
   stoploop = true;
}

void MakeAllTreePieter(char *infile, char *outfile="run0025.root"){

   //  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

   // Create StoreManager both for calibration "TArtCalib..." and treatment "TArtReco..."
   //------------------------------------------------------------------------------------
   TArtStoreManager * sman = TArtStoreManager::Instance();

   // Create EventStore to control the loop and get the EventInfo
   //------------------------------------------------------------
   TArtEventStore *estore = new TArtEventStore();
   estore->SetInterrupt(&stoploop); 
   estore->Open(infile);
   std::cout<<"estore ->"<< infile <<std::endl;

   // Create BigRIPSParameters to get Plastics, PPACs, ICs and FocalPlanes parameters from ".xml" files
   //--------------------------------------------------------------------------------------------------	
   TArtBigRIPSParameters *para = TArtBigRIPSParameters::Instance();
   para->LoadParameter("db/BigRIPSPPAC.xml");
   para->LoadParameter("db/BigRIPSPlastic.xml");
   para->LoadParameter("db/BigRIPSIC.xml");
   para->LoadParameter("db/FocalPlane.xml");
   para->SetFocusPosOffset(8,138.5);

   // Create CalibPID to get and calibrate raw data ( CalibPID -> 
   //[CalibPPAC , CalibIC, CalibPlastic , CalibFocalPlane] 
   TArtCalibPID *brcalib     = new TArtCalibPID();
   TArtCalibPPAC *ppaccalib  = brcalib->GetCalibPPAC();
   TArtCalibPlastic *plasticcalib = brcalib->GetCalibPlastic();
   TArtCalibFocalPlane *cfpl = brcalib->GetCalibFocalPlane();

   // Create RecoPID to get calibrated data and to reconstruct TOF, AoQ, Z, ... (RecoPID -> 
   //[ RecoTOF , RecoRIPS , RecoBeam] )
   TArtRecoPID *recopid = new TArtRecoPID();

   //para->PrintListOfPPACPara();
   //return;

   // Definition of observables we want to reconstruct
   std::cout << "Defining bigrips parameters" << std::endl;  
   TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat","D3"); // F3 - F5
   TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat","D5"); // F5 - F7
   TArtRIPS *rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat","D7"); // F8 - F9
   TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat","D8"); // F9 - F11  

   // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
   //TArtTOF *tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",300.1,5); // F3 - F7
   TArtTOF *tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",300.25,5); // F3 - F7
   //TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-160.5,9); // F8 - F11
   //original TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-159.3,9); // F8 - F11
   TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-159.78,9); // F8 - F11
   // Reconstruction of IC observables for ID
   TArtBeam *beam_br_35 = recopid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
   TArtBeam *beam_br_57 = recopid->DefineNewBeam(rips5to7,tof3to7,"F7IC");   
   TArtBeam *beam_zd_89 = recopid->DefineNewBeam(rips8to9,tof8to11,"F11IC");
   TArtBeam *beam_zd_911 = recopid->DefineNewBeam(rips9to11,tof8to11,"F11IC");

   //to get trigger pattern
   //TArtEventInfo *evtinfo=new TArtEventInfo();

   // Create DALIParameters to get ".xml"
   //------------------------------------

   TArtDALIParameters *dpara = TArtDALIParameters::Instance();
   dpara->LoadParameter("db/DALI.xml");

   // Create CalibDALI to get and calibrate raw data
   //-----------------------------------------------
   TArtCalibDALI *dalicalib = new TArtCalibDALI();

   std::cout<<"para"<<std::endl;

   TFile *fout = new TFile(outfile,"RECREATE");
   TTree *tree = new TTree("tree","tree");

   // define data nodes which are supposed to be dumped to tree 
   //EventInfo is importand for the fBit information to know the trigger!
   TClonesArray * info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
   std::cout<<info_array->GetName()<<std::endl;
   tree->Branch(info_array->GetName(),&info_array);

   TClonesArray * ppac_array = 
      (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
   std::cout<<ppac_array->GetName()<<std::endl;
   tree->Branch(ppac_array->GetName(),&ppac_array);

   TClonesArray * pla_array = 
      (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
   tree->Branch(pla_array->GetName(),&pla_array);

   TClonesArray * ic_array = 
      (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
   tree->Branch(ic_array->GetName(),&ic_array);

   TClonesArray * fpl_array = 
      (TClonesArray *)sman->FindDataContainer("BigRIPSFocalPlane");
   tree->Branch(fpl_array->GetName(),&fpl_array);

   //Dali data
   TClonesArray * dali_array=
      (TClonesArray *)sman->FindDataContainer("DALINaI");
   tree->Branch(dali_array->GetName(),&dali_array);

   //PID reconstructed data:
   TClonesArray *rips_array = 
      (TClonesArray *)sman->FindDataContainer("BigRIPSRIPS");
   std::cout<<rips_array->GetName()<<std::endl;     
   tree->Branch(rips_array->GetName(),&rips_array); 

   TClonesArray *tof_array  = 
      (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
   std::cout<<tof_array->GetName()<<std::endl;     
   tree->Branch(tof_array->GetName(),&tof_array);   

   TClonesArray *beam_array = 
      (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");	
   std::cout<<beam_array->GetName()<<std::endl;     
   tree->Branch(beam_array->GetName(),&beam_array); 

   //_________________________________________________________________________
   //Making new branches

   //%%%%%%%%%%%%%%%%%%%%%%
   //BigRIPS
   Double_t fF8PPACX[4]   = {-999.};
   Double_t fF8PPACY[4]   = {-999.};
   Double_t fF8PPACZ[4]   = {-999.};
   Double_t fF8PosX[2]    = {-999.};
   Double_t fF8PosY[2]    = {-999.};
   Double_t fF8PosZ[2]    = {-999.};

   Double_t fXTar        = -999.;
   Double_t fYTar        = -999.;

   Int_t fGoodPPACFocus[12] = {0};
   Int_t fGoodPPACFocusOr[12] = {0};
   Int_t fGoodPPACTSumX[16] = {0}, TSumX_Low[16], TSumX_High[16];//F5(4), F7(4), F9(4) and F11(4)
   Int_t fGoodFPTSumX[4] = {0};//F5, F7, F9 and F11

   TSumX_Low[0]=174; TSumX_Low[1]=172; TSumX_Low[2]=171; TSumX_Low[3]=176;
   TSumX_Low[4]=167.5; TSumX_Low[5]=171; TSumX_Low[6]= 84.5; TSumX_Low[7]= 96;
   TSumX_Low[8]=135; TSumX_Low[9]=138.5; TSumX_Low[10]=141.5; TSumX_Low[11]=133.5;
   TSumX_Low[12]=152.5; TSumX_Low[13]=144.5; TSumX_Low[14]=153; TSumX_Low[15]=143;
   TSumX_High[0]=178; TSumX_High[1]=176; TSumX_High[2]=175; TSumX_High[3]=180;
   TSumX_High[4]=171.5; TSumX_High[5]=176; TSumX_High[6]=88.5; TSumX_High[7]=103;
   TSumX_High[8]=141.5; TSumX_High[9]=144.5; TSumX_High[10]=147.5; TSumX_High[11]=140.5;
   TSumX_High[12]=157.5; TSumX_High[13]=149.5; TSumX_High[14]=158; TSumX_High[15]=149;

   tree->Branch("xtar",&fXTar,"fXTar/D");
   tree->Branch("ytar",&fYTar,"fYTar/D");
   //tree->Branch("f8ppacx",fF8PPACX,"fF8PPACX[6]/D");
   //tree->Branch("f8ppacy",fF8PPACY,"fF8PPACY[6]/D");
   //tree->Branch("f8posx",fF8PosX,"fF8PosX[3]/D");
   //tree->Branch("f8posy",fF8PosY,"fF8PosY[3]/D");
   tree->Branch("fgoodppacfocus",fGoodPPACFocus,"fGoodPPACFocus[12]/I");
   tree->Branch("fgoodppacfocusor",fGoodPPACFocusOr,"fGoodPPACFocusOr[12]/I");
   tree->Branch("fgoodppactsumx",fGoodPPACTSumX,"fGoodPPACTSumX[16]/I");
   tree->Branch("fgoodfptsumx",fGoodFPTSumX,"fGoodFPTSumX[4]/I");

   //  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   //The Focalplane:
   Double_t F3X=-9999; Double_t F3A=-9999; Double_t F3Y=-9999; Double_t F3B=-9999;
   Double_t F5X=-9999; Double_t F5A=-9999; Double_t F5Y=-9999; Double_t F5B=-9999;
   Double_t F7X=-9999; Double_t F7A=-9999; Double_t F7Y=-9999; Double_t F7B=-9999;
   Double_t F8X=-9999; Double_t F8A=-9999; Double_t F8Y=-9999; Double_t F8B=-9999;
   Double_t F9X=-9999; Double_t F9A=-9999; Double_t F9Y=-9999; Double_t F9B=-9999;
   Double_t F11X=-9999; Double_t F11A=-9999; Double_t F11Y=-9999; Double_t F11B=-9999;

   //Plastic
   Double_t F8QL=-9999; Double_t F8QR=-9999; Double_t F8TL=-9999; Double_t F8TR=-9999;
   Double_t F11QL=-9999; Double_t F11QR=-9999; Double_t F11TL=-9999; Double_t F11TR=-9999;

   tree->Branch("F3X",&F3X,"F3X/D");
   tree->Branch("F3A",&F3A,"F3A/D");
   tree->Branch("F3Y",&F3Y,"F3Y/D");
   tree->Branch("F3B",&F3B,"F3B/D");

   tree->Branch("F5X",&F5X,"F5X/D");
   tree->Branch("F5A",&F5A,"F5A/D");
   tree->Branch("F5Y",&F5Y,"F5Y/D");
   tree->Branch("F5B",&F5B,"F5B/D");

   tree->Branch("F7X",&F7X,"F7X/D");
   tree->Branch("F7A",&F7A,"F7A/D");
   tree->Branch("F7Y",&F7Y,"F7Y/D");
   tree->Branch("F7B",&F7B,"F7B/D");

   tree->Branch("F8X",&F8X,"F8X/D");
   tree->Branch("F8A",&F8A,"F8A/D");
   tree->Branch("F8Y",&F8Y,"F8Y/D");
   tree->Branch("F8B",&F8B,"F8B/D");

   tree->Branch("F8QL",&F8QL,"F8QL/D");
   tree->Branch("F8QR",&F8QR,"F8QR/D");
   tree->Branch("F8TL",&F8TL,"F8TL/D");
   tree->Branch("F8TR",&F8TR,"F8TR/D");

   tree->Branch("F9X",&F9X,"F9X/D");
   tree->Branch("F9A",&F9A,"F9A/D");
   tree->Branch("F9Y",&F9Y,"F9Y/D");
   tree->Branch("F9B",&F9B,"F9B/D");

   tree->Branch("F11X",&F11X,"F11X/D");
   tree->Branch("F11A",&F11A,"F11A/D");
   tree->Branch("F11Y",&F11Y,"F11Y/D");
   tree->Branch("F11B",&F11B,"F11B/D");

   tree->Branch("F11QL",&F11QL,"F11QL/D");
   tree->Branch("F11QR",&F11QR,"F11QR/D");
   tree->Branch("F11TL",&F11TL,"F11TL/D");
   tree->Branch("F11TR",&F11TR,"F11TR/D");

   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   // PID:
   Double_t tof[2];
   Double_t beta[2];

   Double_t zet[4];
   Double_t aoq[4];
   Double_t zetc[4];  //Corrected PID
   Double_t aoqc[4];  //Corrected PID

   Double_t delta[4];

   tree->Branch("tof",tof,"tof[2]/D");
   tree->Branch("beta",beta,"beta[2]/D");

   tree->Branch("zet",zet,"zet[4]/D");
   tree->Branch("aoq",aoq,"aoq[4]/D");
   tree->Branch("zetc",zetc,"zetc[4]/D");
   tree->Branch("aoqc",aoqc,"aoqc[4]/D");
   tree->Branch("delta",delta,"delta[4]/D");

   // flag of TSumX/Y
   Int_t flagTSum;
   tree->Branch("flagTSum", &flagTSum, "flagTSum/I");

   //%%%%%%%%%%%%%%%%%%%%%%
   //DALI
   Int_t dalimultwotime = 0;
   Int_t dalimult = 0;
   Int_t dalitimetruemult = 0;
   Int_t dalimultthres = 0;
   Int_t dalitimetruemultthres = 0;

   tree->Branch("dalimultwotime",&dalimultwotime,"dalimultwotime/I");
   tree->Branch("dalimult",&dalimult,"dalimult/I");
   tree->Branch("dalitimetruemult",&dalitimetruemult,"dalitimetruemult/I");
   tree->Branch("dalimultthres",&dalimultthres,"dalimultthres/I");
   tree->Branch("dalitimetruemultthres",&dalitimetruemultthres,"dalitimetruemultthres/I");

   //%%%%%%%%%%%%%%%%%%%%%%
   //MINOS

   //Double_t z_vertex=0.;
   //tree->Branch("z_vertex",&z_vertex,"z_vertex/D");

   int neve = 0;
   while(estore->GetNextEvent()){
      //if(neve>2e5) break;
      if(neve%10==0)
         std::cout << "event: " << neve << std::endl;

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //Making the BigRIPS tree calibration
      brcalib->ClearData();
      brcalib->ReconstructData();

      //Reconstructiong the PID
      recopid->ClearData();
      recopid->ReconstructData();

      //if(beam_zd_911->GetZet()>29.6|| beam_zd_911->GetZet()<22) {
      if(beam_zd_911->GetZet()<22) {
         neve++;
         continue;
      }

      //Reconstructing the scattering angle:
      //Beam spot on target reconstruction
      fXTar=-999.;
      fYTar=-999.;

      for(int i=0;i<4;i++){
         fF8PPACX[i] = -999.;
         fF8PPACY[i] = -999.;
         fF8PPACZ[i] = -999.;
      }

      for(int i=0;i<2;i++){
         fF8PosX[i] = -999.;
         fF8PosY[i] = -999.; 
         fF8PosZ[i] = -999.;
      }

      for(int i=0;i<12;i++) {
         fGoodPPACFocus[i] = 0;
         fGoodPPACFocusOr[i] = 0;
      }

      bool  fired[12] = {false};
      bool  posRec = false;

      TArtPPAC *fppactsumx[16];
      fppactsumx[0] = ppaccalib->FindPPAC("F5PPAC-1A");
      fppactsumx[1] = ppaccalib->FindPPAC("F5PPAC-1B");
      fppactsumx[2] = ppaccalib->FindPPAC("F5PPAC-2A");
      fppactsumx[3] = ppaccalib->FindPPAC("F5PPAC-2B");
      fppactsumx[4] = ppaccalib->FindPPAC("F7PPAC-1A");
      fppactsumx[5] = ppaccalib->FindPPAC("F7PPAC-1B");
      fppactsumx[6] = ppaccalib->FindPPAC("F7PPAC-2A");
      fppactsumx[7] = ppaccalib->FindPPAC("F7PPAC-2B");
      fppactsumx[8] = ppaccalib->FindPPAC("F9PPAC-1A");
      fppactsumx[9] = ppaccalib->FindPPAC("F9PPAC-1B");
      fppactsumx[10] = ppaccalib->FindPPAC("F9PPAC-2A");
      fppactsumx[11] = ppaccalib->FindPPAC("F9PPAC-2B");
      fppactsumx[12] = ppaccalib->FindPPAC("F11PPAC-2A");
      fppactsumx[13] = ppaccalib->FindPPAC("F11PPAC-2B");
      fppactsumx[14] = ppaccalib->FindPPAC("F11PPAC-1A");
      fppactsumx[15] = ppaccalib->FindPPAC("F11PPAC-1B");

      for(int i=0; i<16; i++){
         if(fppactsumx[i]->GetTSumX()>TSumX_Low[i] && fppactsumx[i]->GetTSumX()<TSumX_High[i]){
            fGoodPPACTSumX[i]=1;
         }else{
            fGoodPPACTSumX[i]=0;
         }
      }
      for(int i=0; i<4; i++){
         if((fGoodPPACTSumX[4*i]==1 || fGoodPPACTSumX[4*i+1]==1) && (fGoodPPACTSumX[4*i+2]==1 || fGoodPPACTSumX[4*i+3]==1)){
            fGoodFPTSumX[i]=1;
         }else{
            fGoodFPTSumX[i]=0;
         }
      }

      TArtPPAC *fppac[12];
      fppac[0] = ppaccalib->FindPPAC("F8PPAC-1A");
      fppac[1] = ppaccalib->FindPPAC("F8PPAC-1B");
      fppac[2] = ppaccalib->FindPPAC("F8PPAC-2A");
      fppac[3] = ppaccalib->FindPPAC("F8PPAC-2B");
      fppac[4] = ppaccalib->FindPPAC("F9PPAC-1A");
      fppac[5] = ppaccalib->FindPPAC("F9PPAC-1B");
      fppac[6] = ppaccalib->FindPPAC("F9PPAC-2A");
      fppac[7] = ppaccalib->FindPPAC("F9PPAC-2B");
      fppac[8] = ppaccalib->FindPPAC("F11PPAC-1A");
      fppac[9] = ppaccalib->FindPPAC("F11PPAC-1B");
      fppac[10] = ppaccalib->FindPPAC("F11PPAC-2A");
      fppac[11] = ppaccalib->FindPPAC("F11PPAC-2B");

      for(int i=0;i<12;i++){
         if(fppac[i]!=0){
            if(fppac[i]->IsFiredX()&&fppac[i]->IsFiredY()) fired[i] = true;
         } 
      }

      if(fppac[0]!=0&&fppac[1]!=0&&fppac[2]!=0&&fppac[3]!=0){
         if(fppac[0]->IsFiredX()&&fppac[1]->IsFiredX()&&fppac[2]->IsFiredX()&&fppac[3]->IsFiredX()) fGoodPPACFocus[8] = 1;
         if((fppac[0]->IsFiredX()||fppac[1]->IsFiredX())&&(fppac[2]->IsFiredX()||fppac[3]->IsFiredX())) 
            fGoodPPACFocusOr[8] = 1;
      }
      if(fppac[4]!=0&&fppac[5]!=0&&fppac[6]!=0&&fppac[7]!=0){
         if(fppac[4]->IsFiredX()&&fppac[5]->IsFiredX()&&fppac[6]->IsFiredX()&&fppac[7]->IsFiredX()) fGoodPPACFocus[9] = 1;
         if((fppac[4]->IsFiredX()||fppac[5]->IsFiredX())&&(fppac[6]->IsFiredX()||fppac[7]->IsFiredX())) 
            fGoodPPACFocusOr[9] = 1;
      }
      if(fppac[8]!=0&&fppac[9]!=0&&fppac[10]!=0&&fppac[11]!=0){
         if(fppac[8]->IsFiredX()&&fppac[9]->IsFiredX()&&fppac[10]->IsFiredX()&&fppac[11]->IsFiredX()) fGoodPPACFocus[11] = 1;
         if((fppac[8]->IsFiredX()||fppac[9]->IsFiredX())&&(fppac[10]->IsFiredX()||fppac[11]->IsFiredX())) 
            fGoodPPACFocusOr[11] = 1;
      }

      for(int i=0;i<4;i++){
         if(fired[i]==false) continue;
         fF8PPACX[i] = fppac[i]->GetX();
         fF8PPACY[i] = fppac[i]->GetY();
         fF8PPACZ[i] = fppac[i]->GetXZPos();
      }

      //For the F8 scattering angle:
      for(int i=0;i<2;i++){
         if(fired[2*i]==true&&fired[2*i+1]==true) {
            fF8PosX[i] = (fppac[2*i]->GetX()+fppac[2*i+1]->GetX())/2; 
            fF8PosY[i] = (fppac[2*i]->GetY()+fppac[2*i+1]->GetY())/2; 
            fF8PosZ[i] = (fppac[2*i]->GetXZPos()+fppac[2*i+1]->GetXZPos())/2;
            //posRec = true;
         }
         else if (fired[2*i]==true||fired[2*i+1]==true){
            int j;
            posRec = true;
            if (fired[2*i]==true) j = 2*i;
            else j = 2*i+1;

            fF8PosX[i] = fppac[j]->GetX(); 
            fF8PosY[i] = fppac[j]->GetY(); 
            fF8PosZ[i] = fppac[j]->GetXZPos(); 
         }  
         else continue;
         //cout<<"posX: "<<posX[i]<<endl;
      }

      if( (fired[0]==true||fired[1]==true) && 
            (fired[2]==true||fired[3]==true)) posRec = true;

      if(posRec) {

         double mX = (fF8PosX[1]-fF8PosX[0])/(fF8PosZ[1]-fF8PosZ[0]);
         double mY = (fF8PosY[1]-fF8PosY[0])/(fF8PosZ[1]-fF8PosZ[0]);

         fXTar = fF8PosX[1] + mX * 880;
         fYTar = fF8PosY[1] + mY * 880;
      }

      //for ZDS plastic
      TArtPlastic *fplastic[2];
      fplastic[0] = plasticcalib->FindPlastic("F8pl");
      fplastic[1] = plasticcalib->FindPlastic("F11pl-1");
      F8QL = fplastic[0]->GetQLRaw();
      F8QR = fplastic[0]->GetQRRaw();
      F8TL = fplastic[0]->GetTLRaw();
      F8TR = fplastic[0]->GetTRRaw();
      F11QL = fplastic[1]->GetQLRaw();
      F11QR = fplastic[1]->GetQRRaw();
      F11TL = fplastic[1]->GetTLRaw();
      F11TR = fplastic[1]->GetTRRaw();

      //The Focalplane:
      F3X=-9999; F3A=-9999; F3Y=-9999; F3B=-9999;
      F5X=-9999; F5A=-9999; F5Y=-9999; F5B=-9999;
      F7X=-9999; F7A=-9999; F7Y=-9999; F7B=-9999;
      F8X=-9999; F8A=-9999; F8Y=-9999; F8B=-9999;
      F9X=-9999; F9A=-9999; F9Y=-9999; F9B=-9999;
      F11X=-9999; F11A=-9999; F11Y=-9999; F11B=-9999;

      TArtFocalPlane *tfpl;
      TVectorD *vec;
      tfpl = cfpl->FindFocalPlane(3); 
      if(tfpl){vec=tfpl->GetOptVector(); F3X=(*vec)(0); F3A=(*vec)(1); F3Y=(*vec)(2); F3B=(*vec)(3);}
      tfpl = cfpl->FindFocalPlane(5); 
      if(tfpl){vec=tfpl->GetOptVector(); F5X=(*vec)(0); F5A=(*vec)(1); F5Y=(*vec)(2); F5B=(*vec)(3);}
      tfpl = cfpl->FindFocalPlane(7); 
      if(tfpl){vec=tfpl->GetOptVector(); F7X=(*vec)(0); F7A=(*vec)(1); F7Y=(*vec)(2); F7B=(*vec)(3);}
      tfpl = cfpl->FindFocalPlane(8); 
      if(tfpl){vec=tfpl->GetOptVector(); F8X=(*vec)(0); F8A=(*vec)(1); F8Y=(*vec)(2); F8B=(*vec)(3);}
      tfpl = cfpl->FindFocalPlane(9); 
      if(tfpl){vec=tfpl->GetOptVector(); F9X=(*vec)(0); F9A=(*vec)(1); F9Y=(*vec)(2); F9B=(*vec)(3);}
      tfpl = cfpl->FindFocalPlane(11); 
      if(tfpl){vec=tfpl->GetOptVector(); F11X=(*vec)(0); F11A=(*vec)(1); F11Y=(*vec)(2); F11B=(*vec)(3);}

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

      //The TOF
      tof[0] = tof3to7->GetTOF();
      beta[0] = tof3to7->GetBeta();

      tof[1] = tof8to11->GetTOF();
      beta[1] = tof8to11->GetBeta();


      //The PID
      zet[0] = beam_br_35->GetZet();
      aoq[0] = beam_br_35->GetAoQ();

      zet[1] = beam_br_57->GetZet();
      aoq[1] = beam_br_57->GetAoQ();

      zet[2] = beam_zd_89->GetZet();
      aoq[2] = beam_zd_89->GetAoQ();

      zet[3] = beam_zd_911->GetZet();
      aoq[3] = beam_zd_911->GetAoQ();

      //The corrected AoQ:
      aoqc[0] = aoq[0];
      zetc[0] = zet[0];
      aoqc[1] = aoq[1];
      zetc[1] = zet[1];
      aoqc[2] = aoq[2]+0.0006*F8X+0.000001*F9X*F9X-0.000018*F9X+0.0001*F8A-0.0001*F9A;
      zetc[2] = zet[2];
      ///******** for setting 1 ********/
      //aoqc[3] = aoq[3]
      //   -0.000001*F9X*F9X+0.00008*F9X
      //   -0.00004*F9A*F9A-0.0003*F9A
      //   +0.000015*F11X*F11X-0.0006*F11X
      //   +0.00003*F11A*F11A+0.00025*F11A
      //   -0.00001*F8A*F8A-0.0002*F8A
      //   +0.0002*F8X
      //   -0.044;
      //zetc[3] = zet[3]-0.019*tof[1]+4.45;
      ///******** for setting 2 ********/
      //aoqc[3] = aoq[3]
      //   -0.00004*F9X
      //   -0.0001*F9A*F9A-0.0003*F9A
      //   +0.00001*F11X*F11X-0.0008*F11X
      //   +0.00003*F11A*F11A+0.0005*F11A
      //   -0.0001*F8X
      //   -0.0006*F8A
      //   +0.002;
      //zetc[3] = zet[3]-0.019*tof[1]+4.18;
      /******** for setting 3 ********/
      aoqc[3] = aoq[3]
         -5e-7*F9X*F9X-5e-5*F9X
         -5e-5*F9A*F9A+5e-4*F9A
         -8e-4*F11X
         +2e-5*F11A*F11A-2.5e-4*F11A
         +8e-6*(sqrt(F11QL*F11QR)-2200)
         -2e-5*(sqrt(F8QL*F8QR)-500)
         +0.0085;
      zetc[3] = 0.9217*(zet[3]-0.033*(tof[1]-223))+2.15;

      //Delta 
      delta[0] = rips3to5->GetDelta();
      delta[1] = rips5to7->GetDelta();
      delta[2] = rips8to9->GetDelta();
      delta[3] = rips9to11->GetDelta();

      ////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      ////Making DALI
      //dalicalib->ClearData();
      ////dalicalib->SetPlTime(plasticcalib->FindPlastic("F8pl")->GetTime());
      ////dalicalib->SetVertex(z_vertex-3.9);
      //dalicalib->SetVertex(0.0);
      ////Add above to remove F8plastic tof.
      //dalicalib->ReconstructData();

      //dalimultwotime = dalicalib->GetMultWithoutT();
      ////cout<<dalimultwotime<<endl;
      //dalimult = dalicalib->GetMult();
      //dalitimetruemult = dalicalib->GetTimeTrueMult();
      //dalimultthres = dalicalib->GetMultThres();
      //dalitimetruemultthres = dalicalib->GetTimeTrueMultThres();

      tree->Fill();
      neve++;
   }
   cout<<"Writing the tree."<<endl;

   fout->Write();
   fout->Close();
}
