//----------! Developed by C. Santamaria / CEA Saclay !----------
//----------!      Version date :: 2014/11/12         !----------
// Code to decode the ridf format file for MINOS
// Calibration, Analysis and Tracking

// > make
// > ./MakeMINOSOfflineCut ../ridf/FileName.ridf              -> Creates a root file: ../rootfiles/FileName.root
// > ./MakeMINOSOfflineCut ../ridf/FileName.ridf test.root
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "TSystem.h"
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtCalibFocalPlane.hh"
#include "TArtDALIParameters.hh"
#include "TArtMINOSParameters.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibDALI.hh"
#include "TArtCalibMINOS.hh"
#include "TArtCalibMINOSData.hh"
#include "TArtCalibIC.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtFocalPlane.hh"
#include "TArtEventInfo.hh"
#include "TArtPlastic.hh"
#include "TArtIC.hh"
#include "TArtPPAC.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtPPAC.hh"
#include "TArtBeam.hh"
#include "TArtTOF.hh"
#include "TArtRIPS.hh"
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
#include <time.h>
#include "Math/Vector3D.h"
#include "TMinosClust.h"
#include "TMinosResult.h"
#include "Tracking.h"

#include "TCutG.h"

using namespace std;
using namespace ROOT::Math;

char *ROOTFILEDIR = "./";
//char *ROOTFILEDIR = "../rootfilesCr/";

//Definitions to use TMinuit
Tracking *Tracking_functions;
TClonesArray data_result;
TMinosResult *minosdata_result;
void SumDistance1(int &, double *, double & sum, double * par,  int);
void SumDistance2(int &, double *, double & sum, double * par, int);

// function to exit loop at keyboard interrupt.
bool stoploop = false;
void stop_interrupt(){
   printf("keyboard interrupt\n");
   stoploop = true;
}

int main(int argc, char** argv) {

   // Variables to calculate the elapsed time of the process
   time_t start,stop;
   time(&start);

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

   TArtEventStore *estore = new TArtEventStore();
   estore->SetInterrupt(&stoploop);
   estore->Open(ridffile);
   //TArtRawEventObject *rawevent = estore->GetRawEventObject();


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

   TArtCalibPID *cpid  = new TArtCalibPID();
   TArtCalibFocalPlane *cfpl= brcalib->GetCalibFocalPlane();
   TArtCalibIC *iccalib= brcalib->GetCalibIC();

   // Create RecoPID to get calibrated data and to reconstruct TOF, AoQ, Z, ... (RecoPID ->
   //[ RecoTOF , RecoRIPS , RecoBeam] )
   TArtRecoPID *recopid = new TArtRecoPID();

   //para->PrintListOfPPACPara();
   //return;

   // Definition of observables we want to reconstruct

   /*
   //Cr66 setting
   TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat",7.0088); // F3 - F5
   TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat",6.7988); // F5 - F7
   TArtRIPS *rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat",5.788); // F8 - F9 5.8401
   TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat",5.780); // F9 - F11  5.8318
   // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
   TArtTOF *tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",302,5); // F3 - F7
   TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-160.5,9); // F8 - F11
   */
   /*
   //72Fe setting
   std::cout << "Defining bigrips parameters" << std::endl;
   TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat","6.9625"); // F3 - F5
   TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat","6.7306"); // F5 - F7
   TArtRIPS *rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat","5.5360"); // F8 - F9
   TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat","5.5155"); // F9 - F11
   // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
   TArtTOF *tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",301.,5); // F3 - F7
   TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-158.5,9); // F8 - F11

*/

   std::cout << "Defining bigrips parameters" << std::endl;
   TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat","D3"); // F3 - F5
   TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat","D5"); // F5 - F7
   TArtRIPS *rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat","D7"); // F8 - F9
   //    TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr_pieter.mat","D8"); // F9 - F11
   // C.M.Shand - Above line commented out and changed to below line (link)
   TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat","D8"); // F9 - F11
   // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
   TArtTOF *tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",300.1,5); // F3 - F7
   TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-159.78,9); // F8 - F11
   //Toffset modified to -161.35 by CS and AO on 26/05/2014 from run 0038

   // Reconstruction of IC observables for ID
   TArtBeam *beam_br_35 = recopid->DefineNewBeam(rips3to5,rips5to7,tof3to7,"F7IC");
   TArtBeam *beam_br_57 = recopid->DefineNewBeam(rips5to7,tof3to7,"F7IC");
   TArtBeam *beam_zd_89 = recopid->DefineNewBeam(rips8to9,tof8to11,"F11IC");
   TArtBeam *beam_zd_911 = recopid->DefineNewBeam(rips9to11,tof8to11,"F11IC");

   //to get trigger pattern
   //TArtEventInfo *evtinfo=new TArtEventInfo();

   //  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   //The Focalplane:
   Double_t F3X=-9999; Double_t F3A=-9999; Double_t F3Y=-9999; Double_t F3B=-9999;
   Double_t F5X=-9999; Double_t F5A=-9999; Double_t F5Y=-9999; Double_t F5B=-9999;
   Double_t F7X=-9999; Double_t F7A=-9999; Double_t F7Y=-9999; Double_t F7B=-9999;
   Double_t F8X=-9999; Double_t F8A=-9999; Double_t F8Y=-9999; Double_t F8B=-9999;
   Double_t F9X=-9999; Double_t F9A=-9999; Double_t F9Y=-9999; Double_t F9B=-9999;
   Double_t F11X=-9999; Double_t F11A=-9999; Double_t F11Y=-9999; Double_t F11B=-9999;

   //The plastic, charge and time
   Double_t F8QL=-9999; Double_t F8QR=-9999; Double_t F8TL=-9999; Double_t F8TR=-9999;
   Double_t F11QL=-9999; Double_t F11QR=-9999; Double_t F11TL=-9999; Double_t F11TR=-9999;

   //The IC
   Double_t F11ICSqSum=-9999;
   Double_t F8PLA_Q_ave = -9999.;

   //The PPAC TSum gate, focal planes 5, 7, 9, 11, and in each focal plane 1a, 1b, 2a, 2b.
   char TSumX_Name[4][4][20];
   Double_t TSumX[4][4], TSumX_Low[4][4], TSumX_High[4][4];
   bool TSumX_fplGood[4], TSumX_Good[4][4], TSumXAllGood;
   strcpy(TSumX_Name[0][0],"F5PPAC-1A");
   strcpy(TSumX_Name[0][1],"F5PPAC-1B");
   strcpy(TSumX_Name[0][2],"F5PPAC-2A");
   strcpy(TSumX_Name[0][3],"F5PPAC-2B");
   strcpy(TSumX_Name[1][0],"F7PPAC-1A");
   strcpy(TSumX_Name[1][1],"F7PPAC-1B");
   strcpy(TSumX_Name[1][2],"F7PPAC-2A");
   strcpy(TSumX_Name[1][3],"F7PPAC-2B");
   strcpy(TSumX_Name[2][0],"F9PPAC-1A");
   strcpy(TSumX_Name[2][1],"F9PPAC-1B");
   strcpy(TSumX_Name[2][2],"F9PPAC-2A");
   strcpy(TSumX_Name[2][3],"F9PPAC-2B");
   strcpy(TSumX_Name[3][0],"F11PPAC-1A");
   strcpy(TSumX_Name[3][1],"F11PPAC-1B");
   strcpy(TSumX_Name[3][2],"F11PPAC-2A");
   strcpy(TSumX_Name[3][3],"F11PPAC-2B");
   TSumX_Low[0][0]=168; TSumX_Low[0][1]=165; TSumX_Low[0][2]=165; TSumX_Low[0][3]=170;
   TSumX_Low[1][0]=162; TSumX_Low[1][1]=165; TSumX_Low[1][2]= 77; TSumX_Low[1][3]= 90;
   TSumX_Low[2][0]=130; TSumX_Low[2][1]=133; TSumX_Low[2][2]=137; TSumX_Low[2][3]=129;
   TSumX_Low[3][0]=146; TSumX_Low[3][1]=138; TSumX_Low[3][2]=147; TSumX_Low[3][3]=138;
   TSumX_High[0][0]=182; TSumX_High[0][1]=180; TSumX_High[0][2]=180; TSumX_High[0][3]=185;
   TSumX_High[1][0]=177; TSumX_High[1][1]=180; TSumX_High[1][2]= 92; TSumX_High[1][3]=105;
   TSumX_High[2][0]=145; TSumX_High[2][1]=148; TSumX_High[2][2]=152; TSumX_High[2][3]=144;
   TSumX_High[3][0]=161; TSumX_High[3][1]=153; TSumX_High[3][2]=162; TSumX_High[3][3]=153;

   // Create DALIParameters to get ".xml"
   //------------------------------------

   TArtDALIParameters *dpara = TArtDALIParameters::Instance();
   dpara->LoadParameter("db/DALI.xml");

   // Create CalibDALI to get and calibrate raw data
   //-----------------------------------------------
   TArtCalibDALI *dalicalib = new TArtCalibDALI();
   // Create MINOSParameters to get ".xml"
   //------------------------------------
   TArtMINOSParameters *setup = new TArtMINOSParameters("MINOSParameters","MINOSParameters");
   setup->LoadParameters("./db/MINOS.xml");
   //setup->PrintListOfMINOSPara();

   TArtCalibMINOS *CalibMINOS = new TArtCalibMINOS();
   //Load function library
   Tracking_functions = new Tracking();


   char* infile;
   //   if(argv[2]==NULL) {
   char*  pch;
   char* pch2;
   pch = strtok(ridffile, "/");
   while (pch != NULL)
   {
      infile = pch;
      pch = strtok (NULL, "/");
   }
   cerr << infile << endl;
   pch2 = strtok(infile, ".");
   cerr << pch2 << endl;
   //return 0;
   if(argv[2]==NULL) {      
      char OutFile[200]="";
      strcat(OutFile, ROOTFILEDIR);
      strcat(OutFile, pch2);
      //strcat(OutFile, "_MINOS68Fep2p_29112014.root");
      strcat(OutFile, "_test.root");

      rootfile=OutFile;
   }
   else rootfile=argv[2];

   cout << endl;
   cout << " *** ROOT file: " << rootfile << endl;

   TFile *fout = new TFile(rootfile,"RECREATE");
   TH2F *pid_br= new TH2F("pid_br","PID__br",500,2.40,2.90,500,24,34);
   TH2F *pid_zd= new TH2F("pid_zd","PID__zd",500,2.40,2.90,500,24,34);
   TTree * tree = new TTree("tree","ridf tree");
   //tree->Branch("rawdata",&rawevent);
   TClonesArray fitdata;
   fitdata.SetClass("TMinosClust");
   tree->Branch("fitdata",&fitdata);
   int trackNbr;
   //tree->Branch("trackNbr",&trackNbr,"trackNbr/I");
   int trackNbr_FINAL;
   tree->Branch("trackNbr_FINAL",&trackNbr_FINAL,"trackNbr_FINAL/I");
   int evtOrig;
   tree->Branch("evtOrig",&evtOrig,"evtOrig/I");
   int padsleft;
   //tree->Branch("padsleft",&padsleft,"padsleft/I");
   data_result.SetClass("TMinosResult");
   tree->Branch("data_result",&data_result);
   double z_vertex=0., x_vertex=0., y_vertex=0., r_vertex=0., phi_vertex = 0.;
   // Variables only filled when trackNbr_FINAL==2
   double VDrift; //in cm/µs
   double DelayTrig; // in ns
   double StopT;
   tree->Branch("VDrift",&VDrift,"VDrift/D");
   tree->Branch("DelayTrig",&DelayTrig,"DelayTrig/D");
   tree->Branch("x_vertex",&x_vertex,"x_vertex/D");
   tree->Branch("y_vertex",&y_vertex,"y_vertex/D");
   tree->Branch("z_vertex",&z_vertex,"z_vertex/D");
   tree->Branch("r_vertex",&r_vertex,"r_vertex/D");
   tree->Branch("phi_vertex",&phi_vertex,"phi_vertex/D"); // angle between two tracks in 3D in degrees
   double thetaz1=0., thetaz2=0.;
   tree->Branch("thetaz1",&thetaz1,"thetaz1/D"); // angle between 1st track and z axis in 3D in degrees
   tree->Branch("thetaz2",&thetaz2,"thetaz2/D"); // angle between 2nd track and z axis in 3D in degrees
   double parFit_1[4], parFit_2[4], parFit_2_F8[4], parFit_2_DSSSD[4];
   tree->Branch("parFit_1",&parFit_1,"parFit_1[4]/D");
   tree->Branch("parFit_2",&parFit_2,"parFit_2[4]/D");
   double errFit_1[4], errFit_2[4];
   tree->Branch("errFit_1",&errFit_1,"errFit_1[4]/D");
   tree->Branch("errFit_2",&errFit_2,"errFit_2[4]/D");
   tree->Branch("parFit_2_F8",&parFit_2_F8,"parFit_2_F8[4]/D");
   tree->Branch("parFit_2_DSSSD",&parFit_2_DSSSD,"parFit_2_DSSSD[4]/D");
   double x_DSSSD, y_DSSSD, z_DSSSD;
   tree->Branch("x_DSSSD",&x_DSSSD,"x_DSSSD/D");
   tree->Branch("y_DSSSD",&y_DSSSD,"y_DSSSD/D");
   tree->Branch("z_DSSSD",&z_DSSSD,"z_DSSSD/D");
   double x_F8PPAC, y_F8PPAC; //position @ z_DSSSD
   tree->Branch("x_F8PPAC",&x_F8PPAC,"x_F8PPAC/D");
   tree->Branch("y_F8PPAC",&y_F8PPAC,"y_F8PPAC/D");
   double z_vertexPPAC=0., x_vertexPPAC=0., y_vertexPPAC=0.;
   tree->Branch("x_vertexPPAC",&x_vertexPPAC,"x_vertexPPAC/D");
   tree->Branch("y_vertexPPAC",&y_vertexPPAC,"y_vertexPPAC/D");
   tree->Branch("z_vertexPPAC",&z_vertexPPAC,"z_vertexPPAC/D");
   double z_vertexDSSSD=0., x_vertexDSSSD=0., y_vertexDSSSD=0.;
   tree->Branch("x_vertexDSSSD",&x_vertexDSSSD,"x_vertexDSSSD/D");
   tree->Branch("y_vertexDSSSD",&y_vertexDSSSD,"y_vertexDSSSD/D");
   tree->Branch("z_vertexDSSSD",&z_vertexDSSSD,"z_vertexDSSSD/D");

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

   tree->Branch("F9X",&F9X,"F9X/D");
   tree->Branch("F9A",&F9A,"F9A/D");
   tree->Branch("F9Y",&F9Y,"F9Y/D");
   tree->Branch("F9B",&F9B,"F9B/D");

   tree->Branch("F11X",&F11X,"F11X/D");
   tree->Branch("F11A",&F11A,"F11A/D");
   tree->Branch("F11Y",&F11Y,"F11Y/D");
   tree->Branch("F11B",&F11B,"F11B/D");

   tree->Branch("F8PLA_Q_ave",&F8PLA_Q_ave,"F8PLA_Q_ave/D");

   // define data nodes which are supposed to be dumped to tree
   //EventInfo is important for the fBit information to know the trigger!
   TClonesArray * info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
   std::cout<<info_array->GetName()<<std::endl;
   Int_t EventInfo_fBit = (Int_t)((TArtEventInfo *)info_array->At(0))->GetTriggerBit();
   tree->Branch(info_array->GetName(),&info_array);

   //Dali data
   TClonesArray * dali_array=
      (TClonesArray *)sman->FindDataContainer("DALINaI");
   tree->Branch(dali_array->GetName(),&dali_array);

   //PID reconstructed data:
   TClonesArray *beam_array =
      (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");
   std::cout<<beam_array->GetName()<<std::endl;
   tree->Branch(beam_array->GetName(),&beam_array);

   //PPAC array
   TClonesArray * ppac_array =
      (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
   std::cout<<ppac_array->GetName()<<std::endl;
   tree->Branch(ppac_array->GetName(),&ppac_array);

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


   //--------------------------------------------------------------------
   //PID Cuts
   //TFile *BRcut = new TFile("../cut/brcut.root","READ");
   //TFile *ZDcut = new TFile("../cut/zdcut.root","READ");

   //TFile *BRcut = new TFile("../cut/Zn81newBetaTOF/brcut_82Ga.root","READ"); // 81Zn(p,2p)
   //TFile *ZDcut = new TFile("../cut/Zn81newBetaTOF/zdcut_81Zn.root","READ"); // 81Zn(p,2p)
   //TFile *BRcut = new TFile("../cut/Zn78newBetaTOF/brcut_81Ga.root","READ");   // 78Zn(p,2p2n)
   //TFile *ZDcut = new TFile("../cut/Zn78newBetaTOF/zdcut_78Zn.root","READ");   // 78Zn(p,2p2n)
   TFile *BRcut = new TFile("./cut/brcut.root","READ");
   TFile *ZDcut = new TFile("./cut/zdcut.root","READ");
   TFile *F8QT = new TFile("./cut/F8PlaQTGate.root","READ");
   TFile *F11QT = new TFile("./cut/F11PlaQTGate.root","READ");

   TCutG *brcut;
   TCutG *zdcut;
   TCutG *f11qt;
   TCutG *f8qt;

   BRcut->GetObject("CUTG",brcut); // C.M.Shand - was ("CUTG",brcut)
   ZDcut->GetObject("CUTG",zdcut); // C.M.Shand - was ("CUTG",zdcut) 
   F8QT->GetObject("CUTG",f8qt);
   F11QT->GetObject("CUTG",f11qt);
   bool F8QTGood, F11QTGood, F11XGood;


   // Parameters for the MINOS ANALYSIS
   double MINOSthresh;
   double TimeBinElec; //in ns
   double Tshaping; // in ns

   ifstream ConfigFile;
   //ConfigFile.open("./../ConfigMINOSDrift.txt");
   ConfigFile.open("./ConfigMINOSDrift.txt");
   string HeaderFile;
   string dummystring;
   bool ThisFileConfig = false;
   getline(ConfigFile,dummystring);
   ConfigFile >> MINOSthresh >> TimeBinElec >> Tshaping;
   cout << MINOSthresh << " " << TimeBinElec << " " << Tshaping << endl;
   getline(ConfigFile,dummystring, '\n'); //takes present line
   getline(ConfigFile,dummystring, '\n'); //discard comment line
   while(ConfigFile.is_open())
   {
      ConfigFile >> HeaderFile >> DelayTrig >> StopT >> VDrift;
      //cout << HeaderFile << "   "<< pch2<<endl;
      if(HeaderFile == pch2) {
         cout << HeaderFile << endl;
         break;
      }
   }

   ConfigFile.close();

   cout << endl;
   cout << " *** MINOS Configuration Parameters *** " << endl;
   cout << " Electronics        :::   MINOSthresh = " << MINOSthresh << " (bins)  ;  TimeBinElec = " << TimeBinElec << " (ns)  ;    Tshaping = " << Tshaping << " (ns)  ;  DelayTrig = " << DelayTrig << " (ns)  ;   VDrift = " << VDrift << " (cm/micros)" << endl;
   cout << endl;

   double PI = TMath::Pi();

   int neve = 0;

   //    vector<TCanvas*> Filter_canvas;
   double ChargeBin=0.,maxCharge=0.;
   int filled = 0;
   vector<double> Xpad, Ypad, Qpad, XpadNew, YpadNew, QpadNew, ZpadNew;
   vector<int> clusterringbool;
   vector<int> clusternbr;
   vector<int> clusterpads;
   int Iteration=0;
   int filter_result=0;
   //int padsleft=0;
   int indexfill=0;
   bool fitbool = false;
   int fit2DStatus = 0;
   double Chi2=0.;
   double x_mm,y_mm,z_mm,q_pad,t_pad;

   //DSSSD Variables
   vector<double> XpadDSSSD, YpadDSSSD, QXpadDSSSD, QYpadDSSSD;
   double XbaryDSSSD, YbaryDSSSD;
   double ThDSSSD = 1000.;

   TF1 *fit_function = new TF1("fit_function",Tracking_functions,&Tracking::conv_fit, 0, 511, 3,"Tracking","conv_fit"); // only 3 param. because baseline is fixed
   double hfit_max, hfit_max_T, T_min, T_max;
   TH1F *hfit = new TH1F("hfit","hfit",512,0,512);


   //2nd step variables
   int npoint_temp=0, cluster_temp=0;
   int padsleft2=0;
   int cluster1=0, cluster2=0;
   int ringsum=0;
   int ringtouch[18]={0};
   double zmax=0.;
   int allevt_2pfiltered=0, allevt_2pvertex=0;
   int array_final=0;
   //    vector<TCanvas*> Hough_canvas;
   bool padsnbr1=false, padsnbr2=false;
   double Qmean1=0., Qmean2=0.;
   TGraph * gryz_1;
   TGraph * grxz_1;
   TGraph * gryz_2;
   TGraph * grxz_2;
   vector<double> xin, yin, zin, qin, xout, yout, zout, qout;
   vector<int> cluster;

   TMinuit *min ;

   while(estore->GetNextEvent()){
      if(neve%1000==0) cout << "Event " << neve << endl;
      //if(neve>1000){
      //   break;
      //}
      //    Clear & Reset variables
      fitdata.Clear();
      data_result.Clear();
      Xpad.clear();
      Ypad.clear();
      Qpad.clear();
      XpadNew.clear();
      YpadNew.clear();
      QpadNew.clear();
      clusterringbool.clear();
      clusternbr.clear();
      clusterpads.clear();
      //	Filter_canvas.clear();
      hfit->Reset();
      XpadDSSSD.clear();
      YpadDSSSD.clear();
      QXpadDSSSD.clear();
      QYpadDSSSD.clear();

      filled=0;
      indexfill = 0;
      ChargeBin = 0.;
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
      z_vertex=-10000.; x_vertex=-10000.; y_vertex=-10000.; r_vertex=-10000.;
      z_vertexPPAC=-10000.; x_vertexPPAC=-10000.; y_vertexPPAC=-10000.;
      z_vertexDSSSD=-10000.; x_vertexDSSSD=-10000.; y_vertexDSSSD=-10000.;
      zmax=0.;
      padsnbr1=false;
      padsnbr2=false;
      Qmean1=0.; Qmean2=0.;
      padsleft2=0;
      thetaz1=-10.; thetaz2=-10.;
      phi_vertex=0.;
      XbaryDSSSD = 0.; YbaryDSSSD = 0.;
      x_DSSSD = -10000.; y_DSSSD = -10000.; z_DSSSD = -10000.;
      x_F8PPAC = -10000.; y_F8PPAC = -10000.;

      int npoint1=0,npoint2=0;
      gryz_1 = new TGraph();
      grxz_1 = new TGraph();
      gryz_2 = new TGraph();
      grxz_2 = new TGraph();

      for(int iit=0;iit<4;iit++) {
         parFit_1[iit] = 0.;
         errFit_1[iit] = 0.;
         parFit_2[iit] = 0.;
         errFit_2[iit] = 0.;
         parFit_2_F8[iit] = 0.;
         parFit_2_DSSSD[iit] = 0.;
      }

      evtOrig = neve;

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //Making the BigRIPS tree calibration and find cuts and triggers
      brcalib->ClearData();
      brcalib->ReconstructData();

      cpid->ClearData();
      cpid->ReconstructData();
      cfpl = cpid->GetCalibFocalPlane();

      //Reconstructing the PID
      recopid->ClearData();
      recopid->ReconstructData();

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

      //The plastic
      F8QL=-9999; F8QR=-9999; F8TL=-9999; F8TR=-9999;
      F11QL=-9999; F11QR=-9999; F11TL=-9999; F11TR=-9999;

      TArtPlastic *tpla;
      tpla = plasticcalib->FindPlastic("F8pl");
      if(tpla){
         F8QL=tpla->GetQLRaw();
         F8QR=tpla->GetQRRaw();
         F8TL=tpla->GetTLRaw();
         F8TR=tpla->GetTRRaw();
         F8PLA_Q_ave = tpla->GetQAveRaw();
      }
      tpla = plasticcalib->FindPlastic("F11pl-1");
      if(tpla){
         F11QL=tpla->GetQLRaw();
         F11QR=tpla->GetQRRaw();
         F11TL=tpla->GetTLRaw();
         F11TR=tpla->GetTRRaw();
      }

      //The IC
      F11ICSqSum=-9999;

      TArtIC *tic = iccalib->FindIC("F11IC");
      if(tic) F11ICSqSum = tic->GetRawADCSqSum();

      F8QTGood=false;
      if(F8TL>-9999 && F8QL>-9999 && F8TR>-9999 && F8QR>-9999){
         if(f8qt->IsInside(F8TL-F8TR,TMath::Log10(F8QL/F8QR))){
            F8QTGood=true;
         }
      }

      F11QTGood=false;
      if(F11TL>-9999 && F11QL>-9999 && F11TR>-9999 && F11QR>-9999){
         if(f11qt->IsInside(F11TL-F11TR,TMath::Log10(F11QL/F11QR))){
            F11QTGood=true;
         }
      }

      F11XGood=false;
      if(fabs(F11X)<30){
         F11XGood=true;
      }

      //The PPAC TSumX gate at F5, F7, F9 and F11.
      TArtPPAC *tppac;
      TSumXAllGood=false;
      for(int i=0; i<4; i++){
         for(int j=0; j<4; j++){
            tppac = ppaccalib->FindPPAC(TSumX_Name[i][j]);
            TSumX[i][j] = tppac->GetTSumX();
            if(TSumX[i][j]>TSumX_Low[i][j] && TSumX[i][j]<TSumX_High[i][j]){
               TSumX_Good[i][j]=true;
            }else{
               TSumX_Good[i][j]=false;
            }
         }
         if((TSumX_Good[i][0]||TSumX_Good[i][1])&&(TSumX_Good[i][2]||TSumX_Good[i][3])){
            TSumX_fplGood[i]=true;
         }else{
            TSumX_fplGood[i]=false;
         }
      }
      if(TSumX_fplGood[0]&&TSumX_fplGood[1]&&TSumX_fplGood[2]&&TSumX_fplGood[3]){
         TSumXAllGood=true;
      }else{
         TSumXAllGood=false;
      }

      if(!TSumXAllGood || !F8QTGood || !F11QTGood || !F11XGood){
         neve++;
         continue;
      }

      //Starting with the analysis conditions
      bool brCutBool = false;
      bool zdCutBool = false;

      double_t aoq, zet, aoqc, zetc, tof;
      aoq=beam_zd_911->GetAoQ();
      zet=beam_zd_911->GetZet();
      tof=tof8to11->GetTOF();
      ///******** for setting 1 ********/
      //aoqc = aoq
      //   -0.000001*F9X*F9X+0.00008*F9X
      //   -0.00004*F9A*F9A-0.0003*F9A
      //   +0.000015*F11X*F11X-0.0006*F11X
      //   +0.00003*F11A*F11A+0.00025*F11A
      //   -0.00001*F8A*F8A-0.0002*F8A
      //   +0.0002*F8X
      //   -0.044;
      //zetc = zet-0.019*tof+4.45;
      ///******** for setting 2 ********/
      //aoqc = aoq
      //   -0.00004*F9X
      //   -0.0001*F9A*F9A-0.0003*F9A
      //   +0.00001*F11X*F11X-0.0008*F11X
      //   +0.00003*F11A*F11A+0.0005*F11A
      //   -0.0001*F8X
      //   -0.0006*F8A
      //   +0.002;
      //zetc[3] = zet[3]-0.019*tof+4.18;
      /******** for setting 3 ********/
      aoqc = aoq
         -5e-7*F9X*F9X-5e-5*F9X
         -5e-5*F9A*F9A+5e-4*F9A
         -8e-4*F11X
         +2e-5*F11A*F11A-2.5e-4*F11A
         +8e-6*(sqrt(F11QL*F11QR)-2200)
         -2e-5*(sqrt(F8QL*F8QR)-500)
         +0.0085;
      zetc = 0.9217*(zet-0.033*(tof-223))+2.15;
      //cout<<aoq<<", "<<aoqc<<", "<<F9X<<", "<<F9A<<", "<<F11X<<", "<<F11A<<", "<<F8QL<<", "<<F8QR<<", "<<F11QL<<", "<<F11QR<<endl;

      pid_br->Fill(beam_br_35->GetAoQ(),beam_br_35->GetZet());
      pid_zd->Fill(aoqc,zetc);
      //cout<<"************************ come here *************************"<<endl;

      if( brcut->IsInside(beam_br_35->GetAoQ(),beam_br_35->GetZet()) ) brCutBool = true;
      if( zdcut->IsInside(aoqc,zetc) ) zdCutBool = true;
      //brCutBool = true;
      //zdCutBool = true;

      //Trigger register information
      bool F7F11DALITrigger = false;

      //Need to check the settings for PSP
      if(EventInfo_fBit==13||EventInfo_fBit==15) F7F11DALITrigger = true;

      //if(/*F7F11DALITrigger == false ||*/ brCutBool == false || zdCutBool == false){
      if(/*F7F11DALITrigger == false ||*/ zdCutBool == false){
         //if(/*F7F11DALITrigger == false ||*/ brCutBool == true || zdCutBool == false){
         neve++;
         continue;
      }

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //Making DALI
      dalicalib->ClearData();
      //dalicalib->SetPlTime(plasticcalib->FindPlastic("F8pl")->GetTime());
      //Add above to remove F8plastic tof.
      dalicalib->ReconstructData();

      dalimultwotime = dalicalib->GetMultWithoutT();
      dalimult = dalicalib->GetMult();
      dalitimetruemult = dalicalib->GetTimeTrueMult();
      dalimultthres = dalicalib->GetMultThres();
      dalitimetruemultthres = dalicalib->GetTimeTrueMultThres();


      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //Making MINOS OFFLINE Reconstruction

      CalibMINOS->ClearData();
      CalibMINOS->ReconstructData();

      TMinosClust *minosfitdata;
      TArtCalibMINOSData *minos = new TArtCalibMINOSData();


      //1:MINOS://///////////////// Filling vectors with (x,y) information for TPC & DSSSD ///////////////////:MINOS://

      for(Int_t i=0;i<CalibMINOS->GetNumCalibMINOS();i++) {
         minos = CalibMINOS->GetCalibMINOS(i);
         ChargeBin = 0.;
         maxCharge = 0.;
         x_mm = minos->GetX();
         y_mm = minos->GetY();

         if(minos->GetDetID() == 3) continue;

         // look at electronic signals from TPC
         else if(minos->GetDetID() == 0) {
            if( !(abs(x_mm)<0.0001 && abs(y_mm)<0.0001) ) { // NON connected MINOS (TPC) channels...
               for(Int_t j=0; j<minos->GetNData(); j++) {
                  if(minos->GetCalibValue(j)>maxCharge) maxCharge = minos->GetCalibValue(j);
               }
               if(maxCharge>=MINOSthresh) {
                  Xpad.push_back(x_mm);
                  Ypad.push_back(y_mm);
                  Qpad.push_back(maxCharge);
                  filled++;
               }
            }
         }

         // look at electronic signals from Si
         else {
            //cerr << "1--- x_mm=" << x_mm << ", y_mm=" << y_mm << endl;
            if( !(abs(x_mm+1)<0.0001 && abs(y_mm+1)<0.0001) ) { // NON connected MINOS (Si) channels...
               for(Int_t j=0; j<minos->GetNData(); j++) {
                  if(minos->GetDetID() == 1) ChargeBin = 3850. - minos->GetCalibValue(j);
                  else if(minos->GetDetID() == 2) ChargeBin = minos->GetCalibValue(j)-250;
                  if(ChargeBin>maxCharge) maxCharge = ChargeBin;
               }

               //cerr << "2--- x_mm=" << x_mm << ", y_mm=" << y_mm << endl;
               if( !(abs(x_mm+1)<0.0001) ) {
                  XpadDSSSD.push_back(x_mm);
                  QXpadDSSSD.push_back(maxCharge);
                  //cerr << "3--- YES x_mm !!!" << endl;
               }
               else {
                  YpadDSSSD.push_back(y_mm);
                  QYpadDSSSD.push_back(maxCharge);
                  //cerr << "3--- YES y_mm !!!" << endl;
               }
            }
         }

      }


      //2:MINOS://////////////////////// Modified (xy) Hough transform (only for TPC) ////////////////////////:MINOS://
      if(filled>0) {
         padsleft = Xpad.size();

         while(Xpad.size()>=10 && Iteration<20) {
            //cout<<"*********************** come here, Iteration = "<<Iteration<<endl;
            filter_result = 0;
            Iteration++;
            //  			Filter_canvas.push_back(new TCanvas(Form("Event%d_cluster%d", i, Iteration), Form("Event%d_cluster%d", i, Iteration)));
            //			filter_result = Hough_modified(Filter_canvas.back(), &Xpad, &Ypad, &Qpad, &XpadNew, &YpadNew, &QpadNew, &clusterringbool);
            filter_result = Tracking_functions->Hough_modified(&Xpad, &Ypad, &Qpad, &XpadNew, &YpadNew, &QpadNew, &clusterringbool);
            //cout << "Obertelli filter n°" << Iteration << " ::: size of cluster=" << filter_result << " , " << XpadNew.size() << " , size of old pads=" << Xpad.size() << endl;
            //			Filter_canvas.back()->Write();
            for(int ik=0; ik<filter_result; ik++) {
               clusternbr.push_back(Iteration);
               clusterpads.push_back(filter_result);
            }
            if(filter_result>10 && clusterringbool.back()==1) trackNbr++;
         }
      }

      for(unsigned int il=0; il<XpadNew.size(); il++)
      {
         minosfitdata = (TMinosClust*)fitdata.ConstructedAt(il);
         minosfitdata->Set(XpadNew[il], YpadNew[il],-10000, -10000, QpadNew[il], clusternbr[il],clusterpads[il], 0.);
         ZpadNew.push_back(-10000.);
      }


      //////////////////        following analysis ONLY if #tracks = 1 || 2 || 3 || 4  for TPC       ///////////////////

      if(trackNbr>0 && trackNbr<5)
      {
         if(filled==0) cerr << "Error !!!" << endl;

         //3:MINOS://////////// Fitting the taken pads for Qmax and Ttrig information ////////////:MINOS://

         padsleft = padsleft - XpadNew.size();

         for(Int_t i=0;i<CalibMINOS->GetNumCalibMINOS();i++) {
            minos = CalibMINOS->GetCalibMINOS(i);
            hfit->Reset();
            fitbool = false;

            if(minos->GetDetID() != 0) continue;

            x_mm = minos->GetX();
            y_mm = minos->GetY();

            for(unsigned int jj=0; jj<XpadNew.size(); jj++) {
               if( abs(XpadNew[jj]-x_mm)<0.0001 && abs(YpadNew[jj]-y_mm)<0.0001) {
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
                     //cerr << "    * t=" << minos->GetCalibTime(j) << " * q=" << minos->GetCalibValue(j) << endl;
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
                     z_mm = ((t_pad*TimeBinElec-DelayTrig)*VDrift); //time bin*(ns/us)*vdrift(mm/ns) =>mm
                     q_pad = fit_function_max-250.;  // Max charge/event
                  }

                  // Comment out for saving histograms of Q(t)
                  // TH1F *hfit_clone = (TH1F*)hfit->Clone(Form("E(T)_%d_%f_%f",neve,x_mm,y_mm));
                  //hfit_clone->Write();
                  //cout << " Histogram results: " << i << " --- @ (" << lastx_mm << ", " << lasty_mm << "); q_pad = " << q_pad << endl;
                  minosfitdata = (TMinosClust*)fitdata.ConstructedAt(indexfill);
                  minosfitdata->Set(XpadNew[indexfill], YpadNew[indexfill], t_pad*TimeBinElec, z_mm , q_pad, clusternbr[indexfill], clusterpads[indexfill], Chi2);
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

            if( (cluster_temp==int(clusternbr[i])) && i==(XpadNew.size() - 1) && clusterpads[i]>=10 && clusterringbool[i]==1 && ZpadNew[i]>-10000 && ZpadNew[i]<=320)
            {
               //cout<<minosdata->x_mm<<" "<<minosdata->z_mm<<" "<<minosdata->Phi<<endl;
               //cout<<"trackNbr "<<trackNbr<<endl;
               xin.push_back(XpadNew[i]);
               yin.push_back(YpadNew[i]);
               zin.push_back(ZpadNew[i]);
               qin.push_back(QpadNew[i]);
               npoint_temp++;
            }

            //cerr << "Event " << neve << ", xpadnew=" << i << endl;
            if(xin.size()>0 && ((cluster_temp!=int(clusternbr[i]) && i!=0) || i==(XpadNew.size() - 1)))
            {
               //   			Hough_canvas.push_back(new TCanvas(Form("Event%d_cluster%d", entry, cluster_temp), Form("Event%d_cluster%d", entry, cluster_temp)));
               //				Hough_3D(Hough_canvas.back(), &xin, &yin, &zin, &qin, &xout, &yout, &zout, &qout);
               Tracking_functions->Hough_3D(&xin, &yin, &zin, &qin, &xout, &yout, &zout, &qout);
               //				Hough_canvas.back()->Write();
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
               //cout<<minosdata->x_mm<<" "<<minosdata->z_mm<<" "<<minosdata->Phi<<endl;
               //cout<<"trackNbr "<<trackNbr<<endl;
               xin.push_back(XpadNew[i]);
               yin.push_back(YpadNew[i]);
               zin.push_back(ZpadNew[i]);
               qin.push_back(QpadNew[i]);
               npoint_temp++;
            }

         }//end of loop on pads


         //5:MINOS://////////// Fitting the filtered tracks in 3D (weight by charge, TMinuit) ////////////:MINOS://

         //For 1 track found or more (less than 5 in total)
         //if(trackNbr_FINAL==1 || trackNbr_FINAL==2)
         if(trackNbr_FINAL>=1)
         {

            //////////Minimization in 3D to reconstruct track lines
            allevt_2pfiltered++;
            padsleft2 = padsleft2 - npoint1 - npoint2;

            Double_t pStart_1[4]={0,1,0,1};
            Double_t pStart_2[4]={0,1,0,1};
            Double_t chi1[2],chi2[2];
            Int_t fitStatus[2];

            // 1st track fitting _ 1p in MINOS _ in TMinuit
            //cout<<"start Minuit "<<endl;
            min = new TMinuit(4);
            min->SetPrintLevel(-1);
            Double_t arglist[10];
            arglist[0] = 3;
            Int_t iflag;
            int nvpar,nparx;
            double amin,edm, errdef;
            double chi2res1, chi2res2;

            Tracking_functions->FindStart(pStart_1,chi1,fitStatus, grxz_1,gryz_1);
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
            for (int i = 0; i <4; i++) {
               min->GetParameter(i,parFit_1[i],errFit_1[i]);
               //cout << "before fit: " << i << "= " << pStart_1[i] << endl;
               //cout << "after fit:  " << i << "= " << parFit_1[i] << " ; " << err_1[i] << endl;
            }

            //////////Get beam direction from F8PPAC
            TArtPPAC *tppac;
            Double_t F8PPAC1A[3], F8PPAC1B[3], F8PPAC2A[3], F8PPAC2B[3];
            Double_t F8PPAC1[3], F8PPAC2[3];
            tppac = ppaccalib->FindPPAC("F8PPAC-1A");
            if(tppac){F8PPAC1A[0] = tppac->GetX(); F8PPAC1A[1] = tppac->GetY();F8PPAC1A[2]=-1310.7;}
            tppac = ppaccalib->FindPPAC("F8PPAC-1B");
            if(tppac){F8PPAC1B[0] = tppac->GetX(); F8PPAC1B[1] = tppac->GetY();F8PPAC1B[2]=-1273.3;}
            tppac = ppaccalib->FindPPAC("F8PPAC-2A");
            if(tppac){F8PPAC2A[0] = tppac->GetX(); F8PPAC2A[1] = tppac->GetY();F8PPAC2A[2]=-802.1;}
            tppac = ppaccalib->FindPPAC("F8PPAC-2B");
            if(tppac){F8PPAC2B[0] = tppac->GetX(); F8PPAC2B[1] = tppac->GetY();F8PPAC2B[2]=-781.9;}
            double z_F8=-16-11;
            //double z_F8=-16-50;
            double z_dsssd=-442+z_F8;
            if(tppac){
               F8PPAC1[2]=(F8PPAC1A[2]+F8PPAC1B[2])/2+z_F8;
               F8PPAC2[2]=(F8PPAC2A[2]+F8PPAC2B[2])/2+z_F8;
               bool flag1=false, flag2=false;
               bool flag1A[2] = {false}, flag1B[2] = {false}, flag2A[2] = {false}, flag2B[2] = {false};
               for(int i1=0; i1<2; i1++) {
                  if(F8PPAC1A[i1]<100 && F8PPAC1A[i1]>-100) flag1A[i1]=true;
                  if(F8PPAC1B[i1]<100 && F8PPAC1B[i1]>-100) flag1B[i1]=true;
                  if(F8PPAC2A[i1]<100 && F8PPAC2A[i1]>-100) flag2A[i1]=true;
                  if(F8PPAC2B[i1]<100 && F8PPAC2B[i1]>-100) flag2B[i1]=true;
               }

               if(flag1A[0]==true && flag1A[1]==true && flag1B[0]==true && flag1B[1]==true && flag2A[0]==false && flag2A[1]==false && flag2B[0]==false && flag2B[1]==false) {
                  F8PPAC1[0]=F8PPAC1A[0];
                  F8PPAC1[1]=F8PPAC1A[1];
                  F8PPAC2[0]=F8PPAC1B[0];
                  F8PPAC2[1]=F8PPAC1B[1];
               }
               if(flag2A[0]==true && flag2A[1]==true && flag2B[0]==true && flag2B[1]==true && flag1A[0]==false && flag1A[1]==false && flag1B[0]==false && flag1B[1]==false) {
                  F8PPAC1[0]=F8PPAC2A[0];
                  F8PPAC1[1]=F8PPAC2A[1];
                  F8PPAC2[0]=F8PPAC2B[0];
                  F8PPAC2[1]=F8PPAC2B[1];
               }


               if(flag1A[0]==true && flag1A[1]==true && flag1B[0]==true && flag1B[1]==true) {
                  F8PPAC1[0]=(F8PPAC1A[0]+F8PPAC1B[0])/2;
                  F8PPAC1[1]=(F8PPAC1A[1]+F8PPAC1B[1])/2;
               }
               else if(flag1A[0]==true && flag1A[1]==true) {
                  F8PPAC1[0]=F8PPAC1A[0];
                  F8PPAC1[1]=F8PPAC1A[1];
               }
               else if(flag1B[0]==true && flag1B[1]==true) {
                  F8PPAC1[0]=F8PPAC1B[0];
                  F8PPAC1[1]=F8PPAC1B[1];
               }
               else flag1=true;
               ////
               if(flag2A[0]==true && flag2A[1]==true && flag2B[0]==true && flag2B[1]==true) {
                  F8PPAC2[0]=(F8PPAC2A[0]+F8PPAC2B[0])/2;
                  F8PPAC2[1]=(F8PPAC2A[1]+F8PPAC2B[1])/2;
               }
               else if(flag2A[0]==true && flag2A[1]==true) {
                  F8PPAC2[0]=F8PPAC2A[0];
                  F8PPAC2[1]=F8PPAC2A[1];
               }
               else if(flag2B[0]==true && flag2B[1]==true) {
                  F8PPAC2[0]=F8PPAC2B[0];
                  F8PPAC2[1]=F8PPAC2B[1];
               }
               else flag2=true;

               if(flag1==true && flag2 == true) continue;

               if(flag2A[0]==false && flag2A[1]==false && flag2B[0]==false && flag2B[1]==false || flag1A[0]==false && flag1A[1]==false && flag1B[0]==false && flag1B[1]==false) continue;
               // PPAC offsets
               F8PPAC1[0] = F8PPAC1[0] - 2;
               F8PPAC1[1] = F8PPAC1[1] + 1;
               F8PPAC2[0] = F8PPAC2[0] - 2;
               F8PPAC2[1] = F8PPAC2[1] + 1;
               Tracking_functions->ParFor_Vertex(F8PPAC1,F8PPAC2,parFit_2_F8);//Calc line pars. used in Vertex function
               Tracking_functions->vertex(parFit_1, parFit_2_F8, x_vertexPPAC, y_vertexPPAC, z_vertexPPAC);
               //Beam profile of PPAC at z_DSSSD
               x_F8PPAC = parFit_2_F8[0] + parFit_2_F8[1]*z_dsssd;
               y_F8PPAC = parFit_2_F8[2] + parFit_2_F8[3]*z_dsssd;
            }

            //////////Get beam direction from DSSSD
            double DSSSD[3];
            if(XpadDSSSD.size()>0 && YpadDSSSD.size()>0) {
               double maxX=0, maxY=0;
               for(int ix=0;ix<XpadDSSSD.size();ix++) {
                  if(QXpadDSSSD[ix]>maxX) {
                     maxX = QXpadDSSSD[ix];
                     XbaryDSSSD = XpadDSSSD[ix];
                  }
               }
               for(int iy=0;iy<YpadDSSSD.size();iy++) {
                  if(QYpadDSSSD[iy]>maxY) {
                     maxY = QYpadDSSSD[iy];
                     YbaryDSSSD = YpadDSSSD[iy];
                  }
               }
               if(maxX>ThDSSSD && maxY>ThDSSSD) {
                  // DSSSD offsets ++ inversion of x/y axis
                  x_DSSSD = YbaryDSSSD+5;
                  y_DSSSD = XbaryDSSSD+3;
                  z_DSSSD = z_dsssd;
                  DSSSD[0] = x_DSSSD;
                  DSSSD[1] = y_DSSSD;
                  DSSSD[2] = z_DSSSD;
                  Tracking_functions->ParFor_Vertex(F8PPAC2,DSSSD,parFit_2_DSSSD);//Calc line pars. used in Vertex function
                  Tracking_functions->vertex(parFit_1, parFit_2_DSSSD, x_vertexDSSSD, y_vertexDSSSD, z_vertexDSSSD);
               }
            }

            //CASE: only 1p detected in MINOS - get PPAC/DSSSD tracking
            if(trackNbr_FINAL==1)
            {
               //PPAC - find 3Dline param.
               if(abs(x_F8PPAC+10000.)>0.0001 && abs(y_F8PPAC+10000.)>0.0001) {
                  for (int iii=0; iii<4; iii++) parFit_2[iii] = parFit_2_F8[iii];
               }
               //DSSSD - find 3Dline param.

               if(abs(x_DSSSD+10000.)>0.0001 && abs(y_DSSSD+10000.)>0.0001) {
                  for (int iii=0; iii<4; iii++) parFit_2[iii] = parFit_2_DSSSD[iii];
               }

            }

            //CASE: 2p or more detected in MINOS - get the second proton 3D line track fitted
            else
            {
               Tracking_functions->FindStart(pStart_2,chi2,fitStatus,grxz_2,gryz_2);
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
               for (int i = 0; i <4; i++) {
                  min->GetParameter(i,parFit_2[i],errFit_2[i]);
                  //cout << "before fit: " << i << "= " << pStart_2[i] << endl;
                  //cout << "after fit:  " << i << "= " << parFit_2[i] << " ; " << err_2[i] << endl;	
               }
            }

            int sumerr_2=0;
            int sumerr_1=0;

            for(int index=0; index<4; index++) {
               sumerr_2+=errFit_2[index];
               sumerr_1+=errFit_1[index];
            }

            //cout<< "Entry " << evtOrig << " --- chi2 for 1st cluster=" << chi1[0]/npoint1 <<", "<< chi1[1]/npoint1 <<" -- chi2 for 2nd cluster="<<  chi2[0]/npoint2 <<", "<< chi2[1]/npoint2 <<endl;

            if(npoint1>0) chi2res1 = (chi1[0]+chi1[1])/npoint1;
            else chi2res1 = -1000;
            if(npoint2>0) chi2res2 = (chi2[0]+chi2[1])/npoint2;
            else chi2res2 = -1000;


            // Reconstruct vertex from 2 tracks
            Tracking_functions->vertex(parFit_1, parFit_2, x_vertex, y_vertex, z_vertex);

            r_vertex = sqrt(x_vertex*x_vertex + y_vertex*y_vertex);

            thetaz1 = acos(1/sqrt(1 + parFit_1[1]*parFit_1[1] + parFit_1[3]*parFit_1[3]))*180./PI;
            if(thetaz1>180) cerr << "thetaz1 cond &&&" << endl;
            thetaz2 = acos(1/sqrt(1 + parFit_2[1]*parFit_2[1] + parFit_2[3]*parFit_2[3]))*180./PI;
            if(thetaz2>180) cerr << "thetaz2 cond @@@@" << endl;

            //phi_vertex: angle in 3D between 2 tracks
            phi_vertex = acos((parFit_1[1]*parFit_2[1] + parFit_1[3]*parFit_2[3]+1)/(sqrt(parFit_1[1]*parFit_1[1]+parFit_1[3]*parFit_1[3]+1)*sqrt(parFit_2[1]*parFit_2[1]+parFit_2[3]*parFit_2[3]+1)))*180./PI;

            if(grxz_1->GetN()>1 && grxz_2->GetN()>1) {
               if(z_vertex>=-500 && z_vertex<=500){
                  allevt_2pvertex++;
               }
            }

            delete min;

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
         //Hough_canvas.clear();

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

      time(&stop);
      printf("Elapsed time: %.1f seconds\n",difftime(stop,start));


      return 0;

   }

   /// Functions to be minimized
   void SumDistance1(int &, double *, double & sum, double * par,  int) {
      int nused=0;
      double qtot=0;
      sum = 0;
      //cout<<"sum "<<sum<<" over "<<npoints<<endl;
      //double factor;
      //cout<<"*************after fit "<<endl;
      for(int i=0; i<data_result.GetEntriesFast(); i++)
      {
         minosdata_result = (TMinosResult*)data_result.At(i);
         if(minosdata_result->n_Cluster==1)
         {
            float x=minosdata_result->x_mm;
            float y=minosdata_result->y_mm;
            float z=minosdata_result->z_mm;
            float q=minosdata_result->Chargemax;
            //if(nused<2)cout<<minosdata_result->n_Cluster<<" "<<x<<" "<<y<<" "<<z<<" "<<q<<endl;
            double d = Tracking_functions->distance2(x, y, z, par);
            sum += d*q;
            nused++;
            qtot+=q;
         }
      }
      //sum/=nused;
      sum/=qtot;
   }

   void SumDistance2(int &, double *, double & sum, double * par, int) {
      int nused=0;
      double qtot=0;
      sum = 0;
      //cout<<"sum "<<sum<<" over "<<npoints<<endl;
      //double factor;
      //cout<<"*************after fit "<<endl;
      for(int i=0; i<data_result.GetEntriesFast(); i++)
      {
         minosdata_result = (TMinosResult*)data_result.At(i);
         if(minosdata_result->n_Cluster==2) 
         {
            float x=minosdata_result->x_mm;
            float y=minosdata_result->y_mm;
            float z=minosdata_result->z_mm;
            float q=minosdata_result->Chargemax;
            //if(nused<2)cout<<minosdata_result->n_Cluster<<" "<<x<<" "<<y<<" "<<z<<" "<<q<<endl;
            double d = Tracking_functions->distance2(x, y, z, par);
            sum += d*q;
            nused++;
            qtot+=q;
         }
      }
      //sum/=nused;
      sum/=qtot;
   }
