// DALI2 Analysis WITH MINOS vertex from offline code (macros/MakeMinosOffline.cpp)
//Example how to run Analysis
// make -f MakefileCut
// ./runAnalysisCut -35 -25 15
// with ./runAnalysisCut timeCutLow timeCutHigh maxaddbackdistance
#include "EventCut.h"
#include "TChain.h"
#include "TCanvas.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include "TROOT.h"
#include "TRint.h"
#include "TVector3.h"

#include "TMath.h"

#include "TF1.h"
#include "TFormula.h"
#include "TCutG.h"

#include <vector>

#include <fstream>
#include <iostream>
#include <strstream>
#include <stdlib.h> 

#include <TFile.h>

#include <search.h>

using namespace std;

const int NUMBEROFDALICRYSTALS = 188;//186;
const int NUMBEROFDALIADDBACKCRYSTALS = 30;
//float ADDBACKDISTANCE = 10;

//For addback
int fAddbackTable[NUMBEROFDALICRYSTALS][NUMBEROFDALIADDBACKCRYSTALS];
int fNumberOfAddbackPartners[NUMBEROFDALICRYSTALS];
bool crystalUsedForAddback[NUMBEROFDALICRYSTALS] = {false};

float fTheta[NUMBEROFDALICRYSTALS];
float fPosX[NUMBEROFDALICRYSTALS];
float fPosY[NUMBEROFDALICRYSTALS];
float fPosZ[NUMBEROFDALICRYSTALS];

int fDaliFold   = 0;//Fold
int fDaliFoldTa = 0;//Fold at Target time
int fDaliMult   = 0;//multiplicity
int fDaliMultTa = 0;//multiplicity at target time

//This is only for Add-back based analysis
struct dali{
   int id;
   int layer;
   float theta;   //Angle for Doppler Correction
   float x;
   float y;
   float z;
   float e;       //Energy
   float t;       //Time
   bool  ttrue;   //Bool if time is true
   float dopp; //Doppler energy.  
   float doppwa; //Doppler energy with true multiplicity and addback.
   float idwa;
   //float twa;
   //float ttruewa;
}fDali[NUMBEROFDALICRYSTALS];

//__________________________________________________________
Double_t DopplerCorrect(Double_t beta, Double_t theta, Double_t energy) {

   if(energy <= 0.) return -1;
   else return energy * (1-beta*TMath::Cos(theta))/TMath::Sqrt((1.0-beta*beta));
}

//__________________________________________________________
//Only used for psp
Double_t CalculateTheta(Double_t x, Double_t y, Double_t z, Double_t x_vertex, Double_t y_vertex, Double_t z_vertex) {

   TVector3 dali(x-x_vertex,y-y_vertex,z-z_vertex);

   return dali.Theta();
}

//__________________________________________________________
bool IncludeAddbackTable(int detid[2],Double_t maxDistance, TVector3 fPos[]) {

   float distance = TMath::Sqrt(TMath::Power(fPos[detid[0]].X()-fPos[detid[1]].X(),2) +
         TMath::Power(fPos[detid[0]].Y()-fPos[detid[1]].Y(),2) + 
         TMath::Power(fPos[detid[0]].Z()-fPos[detid[1]].Z(),2));

   //cout<<"Distance: "<<distance<<endl;
   if( distance > maxDistance ) return false;
   else return true;
}

//__________________________________________________________
void CreateAddBackTable(Double_t maxDistance) {

   cout<<"Creating the add-back table"<<endl;

   TVector3 detPos[NUMBEROFDALICRYSTALS];
   float x,y,z;
   int number;
   float angle;

   FILE *fdetPos  = fopen("./input/AverageInteractionPoint.out","r");
   char dummy[100];

   for(int i=0;i<NUMBEROFDALICRYSTALS;i++) {
      fgets(dummy,100,fdetPos);
      sscanf(dummy,"%f %f %f %i %i %f",&x,&y,&z,&number,&number,&angle);
      detPos[i].SetX(x);
      detPos[i].SetY(y);
      detPos[i].SetZ(z);
      fTheta[i] = angle*3.14159/180.;
      fPosX[i] = x;
      fPosY[i] = y;
      fPosZ[i] = z;
      cout<<detPos[i].X()<<" "<<detPos[i].Y()<<" "<<detPos[i].Z()<<endl; 
   }
   FILE *fAddbackTableOut = fopen("./input/AddbackTable.out","w");

   int detid[2];
   bool inTable;
   int counter = 0;
   for(int i=0;i<NUMBEROFDALICRYSTALS;i++) {
      fNumberOfAddbackPartners[i] = 0;   
      fprintf(fAddbackTableOut," %i",i);

      for(int j=i+1;j<NUMBEROFDALICRYSTALS;j++) {
         detid[0] = i;
         detid[1] = j;

         inTable = IncludeAddbackTable(detid,maxDistance,detPos); 

         if(inTable && counter< NUMBEROFDALIADDBACKCRYSTALS) {
            fprintf(fAddbackTableOut," %i",j);
            fAddbackTable[i][counter] = j;
            fNumberOfAddbackPartners[i]++;
            counter++;
         }
         if(counter == NUMBEROFDALIADDBACKCRYSTALS)  { //Too many detectors 
            cout<<"You have to increase the variable NUMBEROFDALI2ADDBACKCRYSTALS!!!"<<endl;
            //STD::abort();
         }
      }
      counter = 0;
      fprintf(fAddbackTableOut," \n");
   }
   fclose(fAddbackTableOut);
}

//__________________________________________________________
int CompareByEnergy(const void *element1, const void *element2) {
   return((dali*)element1)->e < ((dali*)element2)->e? 1: -1;    //CompareByEnergy()>0: el1 goes after el2 --- CompareByEnergy()<0: el1 goes before el2 --- 
}

//__________________________________________________________
void SortData(int count, dali fDali[]) {
   qsort(fDali,count,sizeof(dali),CompareByEnergy);
}

//__________________________________________________________  
int main(int argc, char **argv) {

   if(argc!=4) {
      std::cout << "Invalid arguments..." << endl;
      return 1;
   }

   double timeCutLow = atof(argv[1]);
   double timeCutHigh = atof(argv[2]);

   // cout << " Time Cut " << timeCutLow  <<"    " <<  timeCutHigh  <<endl;
   //Create the add back table
   CreateAddBackTable(atof(argv[3]));

   // Create interactive interface
   TRint *theApp = new TRint("ROOT example", &argc, argv, NULL, 0);
   int nb_beta = 1;
   double betaMax = 0.; 
   double betaPas = 0.; 

   //Define spectra:
   int minBin = 0;
   int maxBin = 4000;
   int binning = 10;
   int numBin = (maxBin-minBin)/binning;


   TH1F *h_EFit[nb_beta][nb_beta];  // pour stoquer energie CD somme sur tout ID à fitter pour positionEMax
   TH1F *h_EbackwardFit[nb_beta][nb_beta];  // pour stoquer energie CD somme sur ID < 60
   TH1F *h_EforwardFit[nb_beta][nb_beta];   // pour stoquer energie CD somme sur ID >120
   TH1F *h_EZbasFit[nb_beta][nb_beta];  // pour stoquer energie CD somme sur Z >0 < 20
   TH1F *h_EZhautFit[nb_beta][nb_beta];   // pour stoquer energie CD somme sur Z>80 <100
   char nom[100], nom2[100], nom3[100], nom4[100], nom5[100];
   for(int i=0;i<nb_beta;i++) {
      for(int j=0;j<nb_beta;j++) {
         sprintf(nom,"h_E_PourFit[%i][%i]",i,j);
         sprintf(nom2,"h_Eback_PourFit[%i][%i]",i,j);
         sprintf(nom3,"h_Efor_PourFit[%i][%i]",i,j);
         sprintf(nom4,"h_EZbas_PourFit[%i][%i]",i,j);
         sprintf(nom5,"h_EZhaut_PourFit[%i][%i]",i,j);
         h_EFit[i][j] = new TH1F(nom,nom,numBin,minBin,maxBin);
         h_EbackwardFit[i][j] = new TH1F(nom2,nom2,numBin,minBin,maxBin);
         h_EforwardFit[i][j] = new TH1F(nom3,nom3,numBin,minBin,maxBin);
         h_EZbasFit[i][j] = new TH1F(nom4,nom4,numBin,minBin,maxBin);
         h_EZhautFit[i][j] = new TH1F(nom5,nom5,numBin,minBin,maxBin);
      }
   }


   TF1 *f1 = new TF1("f1","pol1(0)+gaus(2)",500,1500);
   double beta_vertex_mov;
   double sigma2[nb_beta][nb_beta];
   double mean2[nb_beta][nb_beta];
   double meanFor2[nb_beta][nb_beta];
   double meanBack2[nb_beta][nb_beta];
   double sigma4[nb_beta][nb_beta];
   double errFor4[nb_beta][nb_beta];
   double errBack4[nb_beta][nb_beta];
   double mean4[nb_beta][nb_beta];
   double meanFor4[nb_beta][nb_beta];
   double meanBack4[nb_beta][nb_beta];
   double meanZbas2[nb_beta][nb_beta];
   double meanZhaut2[nb_beta][nb_beta];
   double meanZbas4[nb_beta][nb_beta];
   double meanZhaut4[nb_beta][nb_beta];
   double Edc_mov[NUMBEROFDALICRYSTALS][nb_beta][nb_beta];

   TH2F *h_EMax = new TH2F("Fit_Energy","Fit_Energy",nb_beta,0,nb_beta,nb_beta,0,nb_beta);
   TH2F *h_DiffE = new TH2F("Fit_DiffE_back_forward","Fit_DiffE_back_forward",nb_beta,0,nb_beta,nb_beta,0,nb_beta);
   TH2F *h_DeltaE = new TH2F("resolution","resolution",nb_beta,0,nb_beta,nb_beta,0,nb_beta);

   int mult_try = 0;

   //#########  fin vitesse coco



   // Parameters for the MINOS ANALYSIS && Reading of ConfigFileSetup.txt for beta values
   double Test;
   double DALIOffset;
   double TargetLength; // in mm
   double BetaMin, BetaMax;
   string ConfigBeam;
   double BeamEnergy;
   double betaLISE_before, betaLISE_middle, betaLISE_after, betaBefore_average, betaAfter_average, beta_before, beta_after, beta_vertex, beta_br, beta_zd;
   //double betaLISE_before, betaLISE_middle, betaLISE_after, beta_vertex;
   //coco
   double betaBR, betaZD;
   double x_vertex,y_vertex,z_vertex;
   double Angle_CD[NUMBEROFDALICRYSTALS];
   int nb_trace=0;

   ifstream ConfigFile;
   ConfigFile.open("./../ConfigFileSetup.txt");
   string Header;
   ConfigFile >> ConfigBeam;
   ConfigFile >> Header >> Test;
   ConfigFile >> Header >> Test;
   ConfigFile >> Header >> Test;
   ConfigFile >> Header >> Test;
   ConfigFile >> Header >> Test;
   ConfigFile >> Header >> DALIOffset;
   ConfigFile >> Header >> TargetLength;
   ConfigFile >> Header >> BetaMin;
   ConfigFile >> Header >> BetaMax;
   ConfigFile >> Header >> BeamEnergy; //Plays no rol at all in the analysis!
   ConfigFile >> Header >> betaLISE_before;
   ConfigFile >> Header >> betaLISE_middle;
   ConfigFile >> Header >> betaLISE_after;
   ConfigFile >> Header >> betaBefore_average;
   ConfigFile >> Header >> betaAfter_average;
   ConfigFile.close();

   //  cout<<"The beta values are: Before target="<<betaLISE_before<<", Middle of target="<<betaLISE_middle<<", End of target="<<betaLISE_after<<endl;
   //  cout << " bornes beta " << BetaMin << "    " << BetaMax << endl;

   EventCut *TreeCut = new EventCut();

   Long64_t nentries = TreeCut->fChainCut->GetEntriesFast();

   //TFile *rootfile = new TFile("output_test_Zn78.root","RECREATE");
   //TFile *rootfile = new TFile("output_test_Ni77.root","RECREATE");
   //TFile *rootfile = new TFile("output_test_Ni75.root","RECREATE");
   TFile *rootfile = new TFile("output_test_Ni73.root","RECREATE");
   TTree *tree = new TTree("tree","analyzed tree"); 

   //TFile *BRcut = new TFile("../cut/brcut.root","READ");
   //TCutG *brcut;
   //BRcut->GetObject("CUTG",brcut);
   TFile *BRcut1 = new TFile("../cut/brcut1.root","READ");
   TFile *BRcut2 = new TFile("../cut/brcut2.root","READ");
   TCutG *brcut1, *brcut2;
   BRcut1->GetObject("CUTG",brcut1);
   BRcut2->GetObject("CUTG",brcut2);
   rootfile->cd();


   //Specific variables;
   int daliMult;
   int daliTimeTrueMult;
   int daliFold;
   int daliTimeTrueFold;

   Double_t DopplerAngle,costheta;

   //________________________________________________________________________
   //Spectra:
   char name[100];  

   TH2F *h_doppler_id[3];
   TH2F *h_dopplerAB_id[3];
   TH1F *h_EtudeMult[10];
   TH1F *h_EtudeMultSup[10];
   TH1F *h_EtudeMultInf[10];
   TH1F *h_Mult[4];

   TH2F *hBR_aoq_z = new TH2F("ID_BR_plot","ID_BR_plot",200,2.4,2.8,200,20,35);
   TH2F *hZD_aoq_z = new TH2F("ID_ZD_plot","ID_ZD_plot",200,2.4,2.8,200,20,35);


   //coco
   TH1F *h_doppler1D[2];
   TH2F *h_GG_sansAddback = new TH2F("hGG","hGG",numBin,minBin,maxBin,numBin,minBin,maxBin);
   TH2F *h_GG_AvecAddback = new TH2F("hGGAddBack","hGGAddBack",numBin,minBin,maxBin,numBin,minBin,maxBin);
   TH2F *h_GG_AB_AllMult = new TH2F("hGGAB_AllMult","hGGAB_AllMult",numBin,minBin,maxBin,numBin,minBin,maxBin);
   TH2F *h_GG_AB_Mult2 = new TH2F("hGGAB_Mult2","hGGAB_Mult2",numBin,minBin,maxBin,numBin,minBin,maxBin);
   TH2F *h_GG_AB_Mult3 = new TH2F("hGGAB_Mult3","hGGAB_Mult3",numBin,minBin,maxBin,numBin,minBin,maxBin);
   TH1F *h_distTemps = new TH1F("h_Temps","h_Temps",100,-50,50);

   TH1F *h_betaVertex = new TH1F("h_betaVertex","h_betaVertex",400,0,1);
   TH1F *h_betaBR = new TH1F("h_betaBR","h_betaBR",800,0,1);
   TH1F *h_betaZD = new TH1F("h_betaZD","h_betaZD",800,0,1);
   TH1F *h_ECD_GrandAngle = new TH1F("h_ECD_GrandAngle","h_ECD_GrandAngle",numBin,minBin,maxBin);
   TH1F *h_ECD_PetitAngle = new TH1F("h_ECD_PetitAngle","h_ECD_PetitAngle",numBin,minBin,maxBin);
   TH1F *h_ECD_GrandZ = new TH1F("h_ECD_GrandZ","h_ECD_GrandZ",numBin,minBin,maxBin);
   TH1F *h_ECD_PetitZ = new TH1F("h_ECD_PetitZ","h_ECD_PetitZ",numBin,minBin,maxBin);
   TH1F *h_Zvertex = new TH1F("h_Zvertex","h_Zvertex",300,-100,200);
   TH1F *h_Xvertex = new TH1F("h_Xvertex","h_Xvertex",100,-50,50);
   TH1F *h_Yvertex = new TH1F("h_Yvertex","h_Yvertex",100,-50,50);
   TH2F *hEnergyZ = new TH2F("hEnergyZ","hEnergyZ",300,-100,200,numBin,minBin,maxBin);
   TH2F *hEnergyZ_forward = new TH2F("hEnergyZ_forward","hEnergyZ_forward",300,-100,200,numBin,minBin,maxBin);
   TH2F *hEnergyZ_backward = new TH2F("hEnergyZ_backward","hEnergyZ_backward",300,-100,200,numBin,minBin,maxBin);
   TH2F *h_energyDopp_time = new TH2F("hTime_EDC","hTime_EDC",200,-100,100,numBin,minBin,maxBin);
   TH2F *h_energy_time = new TH2F("hTime_E","hTime_E",200,-100,100,numBin,minBin,maxBin);
   TH2F *h_energy_angle = new TH2F("hAngle_E","hAngle_E",180,0,180,numBin,minBin,maxBin);
   TH2F *h_ECD_angle = new TH2F("hAngle_ECD","hAngle_ECD",180,0,180,numBin,minBin,maxBin);
   TH2F *h_time_id = new TH2F("hId_Time","hId_Time",186,0,186,200,-100,100);  // les id 186 et 187 ne nous interesse pas
   TH1F *h_nb_trace = new TH1F("hNb_trace","hNb_trace",10,0,10);
   TH1F *hEDC_forward = new TH1F("EDC_forward","EDC_forward",numBin,minBin,maxBin);
   TH1F *hEDC_backward = new TH1F("EDC_backward","EDC_backward",numBin,minBin,maxBin);

   for(int i=0;i<3;i++) {
      sprintf(name,"h_doppler_id[%i]",i);
      h_doppler_id[i] = new TH2F(name,name,186,0,186,numBin,minBin,maxBin);
   }

   for(int i=0;i<10;i++) {
      sprintf(name,"h_EtudeMult[%i]",i);
      h_EtudeMult[i] = new TH1F(name,name,numBin,minBin,maxBin);
   }

   for(int i=0;i<10;i++) {
      sprintf(name,"h_EtudeMultSup[%i]",i);
      h_EtudeMultSup[i] = new TH1F(name,name,numBin,minBin,maxBin);
   }

   for(int i=0;i<10;i++) {
      sprintf(name,"h_EtudeMultInf[%i]",i);
      h_EtudeMultInf[i] = new TH1F(name,name,numBin,minBin,maxBin);
   }

   for(int i=0;i<3;i++) {
      sprintf(name,"h_dopplerAB_id[%i]",i);
      h_dopplerAB_id[i] = new TH2F(name,name,186,0,186,numBin,minBin,maxBin);
   }

   for(int i=0;i<2;i++) {
      sprintf(name,"h_doppler1D[%i]",i);
      h_doppler1D[i] = new TH1F(name,name,numBin,minBin,maxBin);
   }

   for(int i=0;i<4;i++) {
      sprintf(name,"h_Mult[%i]",i);
      h_Mult[i] = new TH1F(name,name,20,0,20);
   }

   TH2F *hdifftheta = new TH2F("hdifftheta","hdifftheta",100,0,3,100,0,3);



   //##################### 
   // variable essai gamma gamma matrix clem
   //#####################

   double Coinc1_min, Coinc1_max, Coinc2_min, Coinc2_max, Coinc3_min, Coinc3_max, Coinc0_min, Coinc0_max;
   Coinc1_min = 300.;
   Coinc1_max = 420.;
   Coinc2_min = 620.;
   Coinc2_max = 740.;
   Coinc3_min = 840.;
   Coinc3_max = 980.;
   Coinc0_min = 140.;
   Coinc0_max = 220.;

   vector<double> EnergyCoinc1, EnergyCoinc2, EnergyCoinc3, EnergyCoinc0;
   tree->Branch("EnergyCoinc1","vector<double>",&EnergyCoinc1);
   tree->Branch("EnergyCoinc2","vector<double>",&EnergyCoinc2);
   tree->Branch("EnergyCoinc3","vector<double>",&EnergyCoinc3);
   tree->Branch("EnergyCoinc0","vector<double>",&EnergyCoinc0);

   Double_t EcdForTree[50];  // mult 50 max ... to be safe ...
   Int_t MultForTree;

   tree->Branch("EcdForTree",EcdForTree,"EcdForTree[50]/D");
   tree->Branch("MultForTree",&MultForTree,"MultForTree/I");

   //Tree added by Xu
   Double_t zet, aoq;
   Int_t dali_mult;
   Int_t daliAB_mult;
   Int_t daliGG_mult;
   Int_t runnumber;
   Double_t dali_E[186];
   Double_t dali_dopp[186];
   Double_t dali_T[186];
   Int_t dali_id[186];
   Int_t dali_layer[186];
   Double_t daliAB_dopp[186];
   Double_t daliGG_dopp1[34410];
   Double_t daliGG_dopp2[34410];
   Int_t daliAB_id[186];
   Int_t daliAB_layer[186];
   Double_t z_vertex_real;
   tree->Branch("zet",&zet,"zet/D");
   tree->Branch("aoq",&aoq,"aoq/D");
   tree->Branch("runnumber",&runnumber,"runnumber/I");
   tree->Branch("beta",&beta_vertex,"beta_vertex/D");
   tree->Branch("z_vertex",&z_vertex_real,"z_vertex/D");
   tree->Branch("dali_mult",&dali_mult,"dali_mult/I");
   tree->Branch("dali_E",dali_E,"dali_E[dali_mult]/D");
   tree->Branch("dali_dopp",dali_dopp,"dali_dopp[dali_mult]/D");
   tree->Branch("dali_layer",dali_layer,"dali_layer[dali_mult]/I");
   tree->Branch("dali_T",dali_T,"dali_T[dali_mult]/D");
   tree->Branch("dali_id",dali_id,"dali_id[dali_mult]/I");
   tree->Branch("dali_dopplerangle",&DopplerAngle,"dali_dopplerangle/D");
   tree->Branch("daliAB_mult",&daliAB_mult,"daliAB_mult/I");
   tree->Branch("daliAB_dopp",daliAB_dopp,"daliAB_dopp[daliAB_mult]/D");
   tree->Branch("daliAB_id",daliAB_id,"daliAB_id[daliAB_mult]/I");
   tree->Branch("daliAB_layer",daliAB_layer,"daliAB_layer[daliAB_mult]/I");
   tree->Branch("daliGG_mult",&daliGG_mult,"daliGG_mult/I");
   tree->Branch("daliGG_dopp1",daliGG_dopp1,"daliGG_dopp1[daliGG_mult]/D");
   tree->Branch("daliGG_dopp2",daliGG_dopp2,"daliGG_dopp2[daliGG_mult]/D");
   tree->Branch("nb_trace",&nb_trace,"nb_trace/I");


   //##################### 
   // fin variable essai gamma gamma matrix clem
   //#####################



   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
   //Start looping through data;

   //Long64_t i=0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // for (Long64_t jentry=0; jentry<2;jentry++) {
      if(jentry%1000 ==0) cout << jentry <<"/"<<nentries<<" Events DONE!"<<endl;
      TreeCut->fChainCut->GetEvent(jentry);

      //for BigRIPS PID cut
      //if(!(brcut1->IsInside(TreeCut->BigRIPSBeam_aoq[0],TreeCut->BigRIPSBeam_zet[0])||brcut2->IsInside(TreeCut->BigRIPSBeam_aoq[0],TreeCut->BigRIPSBeam_zet[0]))){
      //if(!brcut1->IsInside(TreeCut->BigRIPSBeam_aoq[0],TreeCut->BigRIPSBeam_zet[0])){
      if(brcut1->IsInside(TreeCut->BigRIPSBeam_aoq[0],TreeCut->BigRIPSBeam_zet[0])){
         continue;
      }
      zet = TreeCut->BigRIPSBeam_zet[0];
      aoq = TreeCut->BigRIPSBeam_aoq[0];

      //cout << " Nouvelle entrée  " << jentry <<endl;   
      // tree
      for (int iTree = 0; iTree<50; iTree++) {EcdForTree[iTree] = -1.;}
      MultForTree = 0;

      EnergyCoinc1.clear();
      EnergyCoinc2.clear();
      EnergyCoinc3.clear();
      EnergyCoinc0.clear();


      //PID
      //    hBR_aoq_z->Fill(TreeCut->BigRIPSBeam_aoq[0],TreeCut->BigRIPSBeam_zet[0]);
      //    hZD_aoq_z->Fill(TreeCut->BigRIPSBeam_aoq[3],TreeCut->BigRIPSBeam_zet[3]);


      x_vertex = TreeCut->x_vertex;
      y_vertex = TreeCut->y_vertex;
      z_vertex = TreeCut->z_vertex;
      runnumber = TreeCut->EventInfo_runnumber[0];
      //for 78Zn //
      Double_t zoffsetB = -7;
      Double_t zoffsetA = -17;
      Double_t zoffset=0.5*(zoffsetB+zoffsetA);
      Double_t betaB_offset = +0.000;
      Double_t betaA_offset = +0.005;

      ////for test //
      //Double_t zoffsetB = -7;
      //Double_t zoffsetA = -17;
      //Double_t zoffset=0.5*(zoffsetB+zoffsetA);
      //Double_t betaB_offset = +0.00;
      //Double_t betaA_offset = +0.00;

      Double_t life_time = 0;
      Double_t clight = 0.299792458; //in unit of mm/ps
      beta_br = TreeCut->BigRIPSBeam_beta[0];
      //cout<<"beta br = "<<betaBefore_average<<endl;
      beta_zd = TreeCut->BigRIPSBeam_beta[3];
      beta_before = betaLISE_before + (beta_br - betaBefore_average) + betaB_offset;
      beta_after = betaLISE_after + (beta_zd - betaAfter_average) + betaA_offset;
      if(z_vertex>-9000. && z_vertex+11.0<0){
         beta_vertex=beta_before;
         zoffset = zoffsetB;
      }else if(z_vertex+11.0>TargetLength){
         beta_vertex=beta_after;
         zoffset = zoffsetA;
      }else if(z_vertex<-9000.){
         beta_vertex = betaLISE_middle;
         Double_t zoffset=0.5*(zoffsetB+zoffsetA);
         z_vertex = 39.;  //
         //continue;
      }else{
         beta_vertex = beta_before-(z_vertex+11.0)*(beta_before-beta_after)/TargetLength;
         zoffset = zoffsetB-(z_vertex+11.0)*(zoffsetB-zoffsetA)/TargetLength;
         //cout<<"beta_before = "<<beta_before<<", beta_after = "<<beta_after<<endl;
         //cout<<"z_vertex = "<<z_vertex<<", beta_vertex = "<<beta_vertex<<endl;
      }
      //beta_vertex = betaLISE_middle+0.5*(betaB_offset+betaA_offset);
      //beta_vertex = beta_vertex;
      //beta_vertex=0.65;//to test various dependencies on beta of doppler correction

      z_vertex = (z_vertex - DALIOffset + zoffset + beta_vertex*clight*life_time)/10.; //Convert z in the DALI2 referential (in cm)
      z_vertex_real = z_vertex - zoffset/10.;
      h_Zvertex->Fill(z_vertex_real*10.);

      for (int k=0; k<nb_beta; k++){
         for(int l=0; l<nb_beta; l++){
            for(int m=0; m<NUMBEROFDALICRYSTALS;m++) {Edc_mov[m][k][l]=-999.;}
         }
      }

      nb_trace = TreeCut->trackNbr_FINAL;

      betaBR = TreeCut->BigRIPSBeam_beta[0];
      betaZD = TreeCut->BigRIPSBeam_beta[3];

      //   if( betaBR<0.58 || betaBR>0.67) continue;  // pour 67Fe

      h_betaBR->Fill(betaBR);  
      h_betaZD->Fill(betaZD);  

      //pour lise++  : betaLise++ - beta average BR + beta event/event +- offset optimise sur 74Ni

      //pour 80Zn p2p
      betaBR = 0.6243 - 0.6291 + betaBR -0.015; 
      betaZD = 0.5675 - 0.5533 + betaZD +0.01 ;
      //pour 80Zn pp'
      //betaBR = 0.623393 - 0.6291 + betaBR;// -0.015; 
      //betaZD = 0.554496 - 0.5533 + betaZD;// +0.01 ;
      //pour 80Zn (p,3p)
      //betaBR = 0.6385574 - 0.6291 + betaBR;// -0.015; 
      //betaZD = 0.5702273 - 0.5533 + betaZD;// +0.01 ;

      //pour 78Zn (p,2p1n) --80Ga
      //betaBR = 0.6718286 - 0.6291 + betaBR;// -0.015; 
      //betaZD = 0.5696139 - 0.5533 + betaZD;// +0.01 ;
      //pour 78Zn (p,3p2n) --81Ga

      double beta_mov[nb_beta][nb_beta];
      double beta_1[nb_beta];
      double beta_2[nb_beta];

      // ###################   velocity test coco	  
      for (int k=0; k<nb_beta; k++){
         for(int l=0; l<nb_beta; l++){
            beta_1[k] = betaBR+betaMax-betaPas*k;
            beta_2[l] = betaZD+betaMax-betaPas*l;
         }
      }

      double OffsetZ = 7.+1.;  // offset -1 pour etre comme clementine et -39 = +13-51 -1

      //    if(nb_trace!=2) continue;  // a commenter si sans minos
      //    if(nb_trace<2) continue; 
      //if(nb_trace<1) continue; 

      hZD_aoq_z->Fill(TreeCut->BigRIPSBeam_aoq[3],TreeCut->BigRIPSBeam_zet[3]);
      hBR_aoq_z->Fill(TreeCut->BigRIPSBeam_aoq[0],TreeCut->BigRIPSBeam_zet[0]);

      /*
         z_vertex = 51.-DALIOffset;   // milieu de cible a decommenter pour traitement sans minos
         x_vertex = 0.;
         y_vertex = 0.;
         */

      if ((z_vertex>=0.) && (z_vertex<=TargetLength))
      {
         for (int k=0; k<nb_beta; k++){
            for(int l=0; l<nb_beta; l++){beta_mov[k][l] = beta_1[k]-z_vertex/TargetLength*(beta_1[k]-beta_2[l]);}}
      }

      if ((z_vertex<0.) && (z_vertex>=-10.)){
         for (int k=0; k<nb_beta; k++){
            for(int l=0; l<nb_beta; l++){beta_mov[k][l] = beta_1[k];}}
      }

      if ((z_vertex>TargetLength) && (z_vertex<=(TargetLength+10.))){
         for (int k=0; k<nb_beta; k++){
            for(int l=0; l<nb_beta; l++){beta_mov[k][l] = beta_2[l];}}
      }

      //if ((z_vertex<-10.) || (z_vertex>(TargetLength+10.))) continue;  // on enlève les mauvaises reconstruction en Z


      //z_vertex = (z_vertex-TargetLength/2.-OffsetZ)/10.; //Convert z in the DALI2 referential (milieu de cible et en cm)
      x_vertex = x_vertex/10.;
      y_vertex = y_vertex/10.;


      h_Xvertex->Fill(x_vertex*10);
      h_Yvertex->Fill(y_vertex*10);
      h_betaVertex->Fill(beta_vertex);
      h_nb_trace->Fill(nb_trace);
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
      //Sorting the DALI2 data
      fDaliFold   = 0;//Fold
      fDaliFoldTa = 0;//Fold
      fDaliMultTa = 0;//multiplicity
      for(int j=0;j<NUMBEROFDALICRYSTALS;j++) {
         crystalUsedForAddback[j] = false;
      }

      //    cout<<"DALINaI: "<<TreeCut->DALINaI_<<endl;
      for(int j=0;j<TreeCut->DALINaI_;j++){
         fDali[j].id         = TreeCut->DALINaI_id[j]-1; 
         fDali[j].layer      = TreeCut->DALINaI_layer[j];
         fDali[j].theta      = fTheta[fDali[j].id];
         fDali[j].x          = fPosX[fDali[j].id];
         fDali[j].y          = fPosY[fDali[j].id];
         fDali[j].z          = fPosZ[fDali[j].id];
         fDali[j].e          = TreeCut->DALINaI_fEnergy[j];
         fDali[j].t          = TreeCut->DALINaI_fTimeOffseted[j];

         if(fDali[j].e>0){
            //      cout << "boucle energy positive " <<endl;
            DopplerAngle = CalculateTheta(fDali[j].x,fDali[j].y,fDali[j].z,x_vertex,y_vertex,z_vertex);
            //cout << " Theta : DALI=" << fDali[j].theta << ", w/MINOS=" << DopplerAngle << endl;
            hdifftheta->Fill(fDali[j].theta, DopplerAngle);
            Angle_CD[fDali[j].id]=DopplerAngle*180./3.14159;
            fDali[j].dopp = DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);
            // cout <<  fDali[j].id <<  "  energy pieter = " << fDali[j].dopp << endl;
            fDaliFold++;
            fDali[j].ttrue=false;
            //	 cout <<  fDali[j].id << "  temps = " << fDali[j].t << endl;

            // ###################   velocity test coco	  
            for (int k=0; k<nb_beta; k++){
               for(int l=0; l<nb_beta; l++){
                  Edc_mov[fDali[j].id][k][l] = DopplerCorrect(beta_mov[k][l],DopplerAngle,fDali[j].e);	
                  //	cout <<  fDali[j].id << "  energy mod = " << Edc_mov[fDali[j].id][k][l]  << "  energy pieter = " << fDali[j].dopp << endl;					
               }		
            }
            // #################	fin vitesse coco

            if(fDali[j].t>timeCutLow && fDali[j].t<timeCutHigh){
               fDali[j].ttrue = true;
               fDaliFoldTa++;
               //cout <<  " boucle sur temps " << endl;
               //cout <<  fDali[j].id << "  energy mod = " << Edc_mov[fDali[j].id][0][0]  << "  energy pieter = " << fDali[j].dopp << endl;		  
            }

         }else {
            fDali[j].dopp = -999.;
            fDali[j].ttrue = false;
         } // fin boucle "if/else" sur energie positive
      }  // fin for sur dali jusquà DALINaI

      for(int j=TreeCut->DALINaI_;j<NUMBEROFDALICRYSTALS;j++){
         fDali[j].id         = -1;
         fDali[j].layer      = -1;
         fDali[j].theta      = -1;
         fDali[j].x          = -999;
         fDali[j].y          = -999;
         fDali[j].z          = -999;
         fDali[j].e          = -999;
         fDali[j].t          = -999;
         fDali[j].ttrue      = false;
         fDali[j].dopp	  = -999;     
      }

      SortData(TreeCut->DALINaI_,fDali);    //Sort DALI2 crystals by decreasing energies

      //cout << "sorting energy " << endl;

      //for (int j=0; j<TreeCut->DALINaI_;j++){
      //cout <<  fDali[j].id  << "   energy mod "<< Edc_mov[fDali[j].id][0][0] << "   energy pieter Non CD"<< fDali[j].e <<endl;
      //}

      //Going to the add-back:
      float dummyEnergy[NUMBEROFDALICRYSTALS] = {0.};
      //Making add-back and true multiplicity:
      //The Energy must be sorted already according to the highest detected one.
      //cout<<"Starting addback"<<endl;

      daliAB_mult=0;
      for(int i=0;i<fDaliFold;i++){
         if(crystalUsedForAddback[fDali[i].id] == true || fDali[i].ttrue == false) continue;
         DopplerAngle = CalculateTheta(fDali[i].x,fDali[i].y,fDali[i].z,x_vertex,y_vertex,z_vertex);

         dummyEnergy[fDaliMultTa] = DopplerCorrect(beta_vertex,DopplerAngle,fDali[i].e);

         crystalUsedForAddback[fDali[i].id]=true;
         fDali[fDaliMultTa].idwa = fDali[i].id;
         daliAB_id[fDaliMultTa] = fDali[i].id;
         daliAB_layer[fDaliMultTa] = fDali[i].layer;

         for(int j = i+1;j<fDaliFold;j++)  {
            if(crystalUsedForAddback[fDali[j].id]==false && fDali[j].ttrue==true)  {
               for(int k = 0;k<fNumberOfAddbackPartners[fDali[i].id] ;k++) {
                  if(fDali[j].id == fAddbackTable[fDali[i].id][k+1])  {
                     crystalUsedForAddback[fDali[j].id]=true;
                     DopplerAngle = CalculateTheta(fDali[i].x,fDali[i].y,fDali[i].z,x_vertex,y_vertex,z_vertex);
                     dummyEnergy[fDaliMultTa] += DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);

                  }
               }
            }
         }
         fDaliMultTa++;
      }
      daliAB_mult = fDaliMultTa;
      for(int i = 0;i<fDaliMultTa;i++) {
         fDali[i].doppwa = dummyEnergy[i];
         daliAB_dopp[i] = dummyEnergy[i];
      }
      for(int i = fDaliMultTa;i<NUMBEROFDALICRYSTALS;i++) {
         fDali[i].doppwa = -999;
         fDali[i].idwa   = -999;
      }

      daliGG_mult = 0;
      if(daliAB_mult>0 && daliAB_mult<10){
         for(int i=0; i<daliAB_mult; i++){
            for(int j=0; j<daliAB_mult; j++){
               if(i==j) continue;
               daliGG_dopp1[daliGG_mult] = daliAB_dopp[i];
               daliGG_dopp2[daliGG_mult] = daliAB_dopp[j];
               daliGG_mult++;
            }
         }
      }

      // ######### velocity test coco    
      // on ne prend que les eve de mult 3, par contre on doit soimmer les energies de touts les detecteur car comme il n'y a pas de sort sur les energies, ca n'est pas forcement les trois premiere energie qui sont les bonnes, ie il y aura beaucoup de -999 gamma

      // on prend une vitesse
      //for (int j=0;j<NUMBEROFDALICRYSTALS;j++){
      //if(fDali[j].ttrue) {
      //cout << fDali[j].id << " -- > energy = " << Edc_mov[fDali[j].id][0][0] << endl ;
      //}
      //}

      if(fDaliFoldTa<=3){
         for (int k=0; k<nb_beta; k++){
            for (int l=0; l<nb_beta; l++){ 
               // on boucle sur tous les detecterus car pas de sorting en energie
               for (int j=0;j<NUMBEROFDALICRYSTALS;j++){
                  if(fDali[j].ttrue) {
                     h_EFit[k][l]->Fill(Edc_mov[fDali[j].id][k][l]);
                     if(fDali[j].id>120) h_EforwardFit[k][l]->Fill(Edc_mov[fDali[j].id][k][l]);
                     if(fDali[j].id<60) h_EbackwardFit[k][l]->Fill(Edc_mov[fDali[j].id][k][l]);
                     if(z_vertex>0. && z_vertex<20.) h_EZbasFit[k][l]->Fill(Edc_mov[fDali[j].id][k][l]);
                     if(z_vertex>80. && z_vertex<100.) h_EZhautFit[k][l]->Fill(Edc_mov[fDali[j].id][k][l]);
                  }
               }
            }
         }
      }


      // #################  fin vitesse coco


      // #######################    w/o addback
      h_Mult[0]->Fill(fDaliFold);
      h_Mult[1]->Fill(fDaliFoldTa);

      dali_mult=0;
      for(int j=0;j<fDaliFold;j++){
         if(fDali[j].ttrue) {
            h_doppler_id[0]->Fill(fDali[j].id,fDali[j].dopp);
            
            //for tree
            dali_E[dali_mult] = fDali[j].e;
            dali_dopp[dali_mult] = fDali[j].dopp;
            dali_layer[dali_mult] = fDali[j].layer;
            dali_T[dali_mult] = fDali[j].t;
            dali_id[dali_mult] = fDali[j].id;
            dali_mult++;

            if(fDali[j].id>120) hEnergyZ_forward->Fill(z_vertex,fDali[j].dopp);
            if(fDali[j].id<60) hEnergyZ_backward->Fill(z_vertex,fDali[j].dopp);

            if(fDaliFoldTa>3)  h_doppler_id[2]->Fill(fDali[j].id,fDali[j].dopp);  

            if(fDaliFoldTa<=3) 
            { 
               //	hEnergyZ->Fill(z_vertex,fDali[j].dopp);

               h_doppler_id[1]->Fill(fDali[j].id,fDali[j].dopp);

               if(fDali[j].id>120) hEDC_forward->Fill(fDali[j].dopp);

               if(fDali[j].id<60) hEDC_backward->Fill(fDali[j].dopp);

               h_doppler1D[0]->Fill(fDali[j].dopp);  // coco pour superposition facil avec root
               h_ECD_angle->Fill(Angle_CD[fDali[j].id],fDali[j].dopp);
               if (Angle_CD[fDali[j].id]>70 && Angle_CD[fDali[j].id]<80) h_ECD_GrandAngle->Fill(fDali[j].dopp);
               if (Angle_CD[fDali[j].id]>20. && Angle_CD[fDali[j].id]<30.) h_ECD_PetitAngle->Fill(fDali[j].dopp);
               if (z_vertex>0. && z_vertex<10.) h_ECD_PetitZ->Fill(fDali[j].dopp);
               if (z_vertex>70. && z_vertex<80.) h_ECD_GrandZ->Fill(fDali[j].dopp);

            }

            h_energyDopp_time->Fill(fDali[j].t,fDali[j].dopp);
            h_energy_time->Fill(fDali[j].t,fDali[j].e);
         }
      } 
      // #####################################  fin w/o addback


      //#########################################################################
      //essai clem gamma gamma
      //#########################################################################
      int detCoinc1=-1, detCoinc2=-1, detCoinc3=-1, detCoinc0=-1;



      for(int j=0;j<fDaliMultTa;j++){
         if(fDaliMultTa>=2) {
            // 	cout << "energy :"<< "j ="<<j << ", E=" << fDali[j].doppwa << endl; 


            //  cout << "mult>2" << "mult = " << fDaliMultTa << endl;

            if(detCoinc1==-1) { 
               if(fDali[j].doppwa>Coinc1_min && fDali[j].doppwa<Coinc1_max) detCoinc1 = j;

               //if(fDali[j].doppwa>=Coinc1_min && fDali[j].doppwa<=Coinc1_max)  cout << "gate1"<<endl;
            }

            if(detCoinc2==-1) { 
               if(fDali[j].doppwa>=Coinc2_min && fDali[j].doppwa<=Coinc2_max) detCoinc2 = j;
            }

            if(detCoinc3==-1) { 
               if(fDali[j].doppwa>=Coinc3_min && fDali[j].doppwa<=Coinc3_max) detCoinc3 = j;
            }

            if(detCoinc0==-1) { 
               if(fDali[j].doppwa>=Coinc0_min && fDali[j].doppwa<=Coinc0_max) detCoinc0 = j;
            }
         }

      }

      for(int j=0;j<fDaliMultTa;j++){


         if(detCoinc1!=-1 && j!=detCoinc1) EnergyCoinc1.push_back(fDali[j].doppwa);

         //if(detCoinc1!=-1 && j!=detCoinc1) cout <<"ecriture coinc gate1"<<endl;


         if(detCoinc2!=-1 && j!=detCoinc2) EnergyCoinc2.push_back(fDali[j].doppwa);
         if(detCoinc3!=-1 && j!=detCoinc3) EnergyCoinc3.push_back(fDali[j].doppwa);
         if(detCoinc0!=-1 && j!=detCoinc0) EnergyCoinc0.push_back(fDali[j].doppwa);

      }



      //#########################################################################
      // fin essai clem gamma gamma
      //#########################################################################





      // ####################################     w/ addback
      h_Mult[2]->Fill(fDaliMult);
      h_Mult[3]->Fill(fDaliMultTa);

      MultForTree = fDaliMultTa;

      for(int j=0;j<fDaliMultTa;j++){

         EcdForTree[j] = fDali[j].doppwa;

         h_dopplerAB_id[0]->Fill(fDali[j].idwa,fDali[j].doppwa);	//all mult

         h_EtudeMult[0]->Fill(fDali[j].doppwa);
         h_EtudeMultSup[0]->Fill(fDali[j].doppwa);
         h_EtudeMultInf[0]->Fill(fDali[j].doppwa);

         if(fDaliMultTa>1) h_EtudeMultSup[1]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>2) h_EtudeMultSup[2]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>3) h_EtudeMultSup[3]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>4) h_EtudeMultSup[4]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>5) h_EtudeMultSup[5]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>6) h_EtudeMultSup[6]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>7) h_EtudeMultSup[7]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>8) h_EtudeMultSup[8]->Fill(fDali[j].doppwa);
         if(fDaliMultTa>9) h_EtudeMultSup[9]->Fill(fDali[j].doppwa);

         if(fDaliMultTa<=1) h_EtudeMultInf[1]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=2) h_EtudeMultInf[2]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=3) h_EtudeMultInf[3]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=4) h_EtudeMultInf[4]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=5) h_EtudeMultInf[5]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=6) h_EtudeMultInf[6]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=7) h_EtudeMultInf[7]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=8) h_EtudeMultInf[8]->Fill(fDali[j].doppwa);
         if(fDaliMultTa<=9) h_EtudeMultInf[9]->Fill(fDali[j].doppwa);

         if(fDaliMultTa==1) h_EtudeMult[1]->Fill(fDali[j].doppwa);  //before fDali[0]
         if(fDaliMultTa==2) h_EtudeMult[2]->Fill(fDali[j].doppwa);
         if(fDaliMultTa==3) h_EtudeMult[3]->Fill(fDali[j].doppwa);
         if(fDaliMultTa==4) h_EtudeMult[4]->Fill(fDali[j].doppwa);
         if(fDaliMultTa==5) h_EtudeMult[5]->Fill(fDali[j].doppwa);
         if(fDaliMultTa==6) h_EtudeMult[6]->Fill(fDali[j].doppwa);
         if(fDaliMultTa==7) h_EtudeMult[7]->Fill(fDali[j].doppwa);
         if(fDaliMultTa==8) h_EtudeMult[8]->Fill(fDali[j].doppwa);
         if(fDaliMultTa==9) h_EtudeMult[9]->Fill(fDali[j].doppwa);

         if(fDaliMultTa>3)  h_dopplerAB_id[2]->Fill(fDali[j].idwa,fDali[j].doppwa);
         if(fDaliMultTa<=3) {

            hEnergyZ->Fill(z_vertex,fDali[j].doppwa);


            h_dopplerAB_id[1]->Fill(fDali[j].idwa,fDali[j].doppwa);
            h_doppler1D[1]->Fill(fDali[j].doppwa);  // coco pour superposition facil avec new TBrowser
            //	if(fDali[j].t>timeCutLow && fDali[j].t<timeCutHigh){h_distTemps->Fill(fDali[j].t);}


         }
      }
      // ##############################################  fin w/ addback

      //#############################################  GG matrix essai coco
      // w/o adddback

      if(fDaliFoldTa<=4){
         for(int igg = 0; igg<fDaliFold; igg++){
            if(fDali[igg].ttrue){
               for(int jgg = igg+1; jgg<fDaliFold; jgg++){
                  if(fDali[jgg].ttrue)	h_GG_sansAddback->Fill(fDali[igg].dopp,fDali[jgg].dopp);
                  if(fDali[jgg].ttrue)	h_GG_sansAddback->Fill(fDali[jgg].dopp,fDali[igg].dopp);
               }
            }
         }     
      }

      // w/ addback
      if(fDaliMultTa<=4){
         for(int igg = 0; igg<fDaliMultTa; igg++){
            if(fDali[igg].ttrue){
               for(int jgg = igg+1; jgg<fDaliMultTa; jgg++){
                  if(fDali[jgg].ttrue)	h_GG_AvecAddback->Fill(fDali[igg].doppwa,fDali[jgg].doppwa);
                  if(fDali[jgg].ttrue)	h_GG_AvecAddback->Fill(fDali[jgg].doppwa,fDali[igg].doppwa);
               }
            }   
         }
      } 

      // w/ addback all mult
      if(fDaliMultTa>=2){
         for(int igg = 0; igg<fDaliMultTa; igg++){
            //	if(fDali[igg].ttrue){
            for(int jgg = igg+1; jgg<fDaliMultTa; jgg++){
               h_GG_AB_AllMult->Fill(fDali[igg].doppwa,fDali[jgg].doppwa);
               h_GG_AB_AllMult->Fill(fDali[jgg].doppwa,fDali[igg].doppwa);
            }
         }   
         //	}
      } 



      //addback condition gamma-gamma

      // w/ addback all mult
      if(fDaliMultTa==2){
         for(int igg = 0; igg<fDaliMultTa; igg++){
            for(int jgg = igg+1; jgg<fDaliMultTa; jgg++){
               h_GG_AB_Mult2->Fill(fDali[igg].doppwa,fDali[jgg].doppwa);
               h_GG_AB_Mult2->Fill(fDali[jgg].doppwa,fDali[igg].doppwa);
            }
         }   
      } 

      // w/ addback all mult
      if(fDaliMultTa<=3){
         for(int igg = 0; igg<fDaliMultTa; igg++){
            for(int jgg = igg+1; jgg<fDaliMultTa; jgg++){
               h_GG_AB_Mult3->Fill(fDali[igg].doppwa,fDali[jgg].doppwa);
               h_GG_AB_Mult3->Fill(fDali[jgg].doppwa,fDali[igg].doppwa);
            }
         }   
      } 



      //###################################   fin GG

      //cout << "fill tree" << endl;
      tree->Fill();



   }  // fin boucle sur evenement


   // ######## velocity test coco
   int compteur1 = 0;

   for(int i=0;i<10;i++) h_EtudeMult[i]->Write();
   for(int i=0;i<10;i++) h_EtudeMultInf[i]->Write();
   for(int i=0;i<10;i++) h_EtudeMultSup[i]->Write();
   for(int i=0;i<3;i++) h_doppler_id[i]->Write();
   for(int i=0;i<3;i++) h_dopplerAB_id[i]->Write();
   for(int i=0;i<4;i++) h_Mult[i]->Write();

   for(int i=0;i<2;i++) h_doppler1D[i]->Write();
   h_GG_sansAddback->Write();
   h_GG_AvecAddback->Write();
   h_GG_AB_AllMult->Write();
   h_GG_AB_Mult2->Write();
   h_GG_AB_Mult3->Write();
   h_distTemps->Write();

   h_betaVertex->Write();
   h_betaBR->Write();
   h_betaZD->Write();
   h_Zvertex->Write();
   h_Xvertex->Write();
   h_Yvertex->Write();
   hdifftheta->Write();
   hEnergyZ->Write();
   hEnergyZ_forward->Write();
   hEnergyZ_backward->Write();
   h_energy_time->Write();
   h_energyDopp_time->Write();
   //  h_energy_angle->Write();
   h_ECD_angle->Write();
   h_ECD_GrandAngle->Write();
   h_ECD_PetitAngle->Write();
   h_ECD_GrandZ->Write();
   h_ECD_PetitZ->Write();
   h_time_id->Write();
   h_nb_trace->Write();
   hEDC_forward->Write();
   hEDC_backward->Write();
   hBR_aoq_z->Write();
   hZD_aoq_z->Write();

   // rootfile->Write();
   // rootfile->Close();
   tree->Write();
   return 0;
}
