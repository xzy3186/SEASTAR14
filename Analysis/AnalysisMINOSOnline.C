// DALI2 Analysis WITH MINOS vertex from online code (anaroot)
//Example how to run Analysis
// ./runAnalysisMINOSOnline -12 12 15 0.03 0.049 
// with ./runAnalsysisMINOSOnline timeCutLow timeCutHigh maxaddbackdistance betaDiffLow betaDiffHigh
// beta1::cut1  ----  beta2::cut2  ----  beta3::cut3
#include "Event.h"
#include "TChain.h"

#include "TCutG.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"

#include "TROOT.h"
#include "TRint.h"
#include "TVector3.h"

#include "TMath.h"

#include <fstream>
#include <iostream>
#include <strstream>
#include <stdlib.h> 

#include <TFile.h>

#include <search.h>

using namespace std;

const int NUMBEROFDALICRYSTALS = 186;
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
  float dopp[3]; //Doppler energy. Three doppler corrections for the three betas  
  float doppwa[3]; //Doppler energy with true multiplicity and addback.
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
Double_t CalculateTheta(Double_t x, Double_t y, Double_t z, Double_t z_vertex) {

  TVector3 dali(x,y,z-z_vertex);

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
    //cout<<detPos[i].X()<<" "<<detPos[i].Y()<<" "<<detPos[i].Z()<<endl; 
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
  return((dali*)element1)->e < ((dali*)element2)->e? 1: -1;
}

//__________________________________________________________
void SortData(int count, dali fDali[]) {
  qsort(fDali,count,sizeof(dali),CompareByEnergy);
}

//__________________________________________________________  
int main(int argc, char **argv) {

  if(argc!=6) {
    std::cout << "Invalid arguments..." << endl;
    return 1;
  }

  double timeCutLow = atof(argv[1]);
  double timeCutHigh = atof(argv[2]);

  //Create the add back table
  CreateAddBackTable(atof(argv[3]));
  
  double betaDiffLow = atof(argv[4]);
  double betaDiffHigh = atof(argv[5]);

  // Create interactive interface
  TRint *theApp = new TRint("ROOT example", &argc, argv, NULL, 0);

  //PID cuts:
  TFile *BRcut[3];
  TFile *ZDcut[3];

  //--------------------------------------------------------------------
  //Cuts
  char name[100];  

  for(int i=0;i<3;i++) {
    sprintf(name,"../cut/brcut%i.root",i);
    BRcut[i] = new TFile(name,"READ");
  }
  
  for(int i=0;i<3;i++) {
    sprintf(name,"../cut/zdcut%i.root",i);
    ZDcut[i] = new TFile(name,"READ");
  }
  
  TCutG *brcut[3];
  TCutG *zdcut[3];
  
  for(int i=0;i<3;i++) {
    //sprintf(name,"brcut%i",i);
    BRcut[i]->GetObject("CUTG",brcut[i]);
  }
  
  for(int i=0;i<3;i++) {
    //sprintf(name,"zdcut%i",i);
    ZDcut[i]->GetObject("CUTG",zdcut[i]); 
  }
  
  // Parameters for the MINOS ANALYSIS && Reading of ConfigFileSetup.txt for beta values
  double Test;
  double DALIOffset;
  double TargetLength; // in mm
  int BeamA, BeamZ;
  string ConfigBeam;
  double BeamEnergy;
  double betaLISE_before, betaLISE_middle, betaLISE_after, beta_vertex;
  double z_vertex;

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
  ConfigFile >> Header >> BeamA;
  ConfigFile >> Header >> BeamZ;
  ConfigFile >> Header >> BeamEnergy;
  ConfigFile >> Header >> betaLISE_before;
  ConfigFile >> Header >> betaLISE_middle;
  ConfigFile >> Header >> betaLISE_after;
  ConfigFile.close();

  cout<<"The beta values are: Before target="<<betaLISE_before<<", Middle of target="<<betaLISE_middle<<", End of target="<<betaLISE_after<<endl;


  Event *Tree = new Event();
  
  Long64_t nentries = Tree->fChain->GetEntriesFast();

  TFile *rootfile = new TFile("outputfile.root","RECREATE");
  rootfile->cd();

  //Define spectra:
  int minBin = 0;
  int maxBin = 6000;
  int binning = 25;
  int numBin = (maxBin-minBin)/binning;

  //Specific variables;
  int daliMult;
  int daliTimeTrueMult;
  int daliFold;
  int daliTimeTrueFold;

  Double_t DopplerAngle;

  int countingFilling = 0;
  //________________________________________________________________________
  //Spectra:

  TH1F *h_beta_diff[3];

  TH2F *h_doppler[40];
  TH2F *h_aoq_z[4];

  for(int i=0;i<3;i++) {
    sprintf(name,"h_beta_diff[%i]",i);
    h_beta_diff[i] = new TH1F(name,name,1000,-0.1,0.1);
  }

  for(int i=0;i<40;i++) {
    sprintf(name,"h_doppler[%i]",i);
    h_doppler[i] = new TH2F(name,name,186,0,186,numBin,minBin,maxBin);
  }

  for(int i=0;i<4;i++) {
    sprintf(name,"h_aoq_z[%i]",i);
    h_aoq_z[i] = new TH2F(name,name,500,-100,100,250,0,120);
    //h_aoq_z[i] = new TH2F(name,name,500,2.0,3.0,250,40,60);
  }  

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //Start looping through data;
  
  Long64_t i=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //for (Long64_t jentry=0; jentry<1000;jentry++) {
    //Long64_t ientry = Tree->LoadTree(jentry);
    //if (ientry < 0) break;
    Tree->fChain->GetEvent(jentry);   
    // if (Cut(ientry) < 0) continue;
    
    if(jentry%10000 ==0) cout << jentry <<"/"<<nentries<<" Events DONE!"<<endl;
        
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Sorting the MINOS data
    // Calculate beta from the z_vertex found with MINOS offline software 
    //            (estimated beam track parallel to the z axis)
    z_vertex = Tree->z_vertex;
    if(z_vertex>=-10 && z_vertex<0) beta_vertex = betaLISE_before;
    else if(z_vertex>=0 && z_vertex<=100) {
	beta_vertex = betaLISE_before - z_vertex*(betaLISE_before-betaLISE_after)/TargetLength;
    }
    else if(z_vertex>100 && z_vertex<=110) beta_vertex = betaLISE_after;
    else beta_vertex = betaLISE_middle;

    z_vertex = (z_vertex - DALIOffset)/10.; //Convert z in the DALI2 referential (in cm)


    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Sorting the DALI2 data
    fDaliFold   = 0;//Fold
    fDaliFoldTa = 0;//Fold
    fDaliMultTa = 0;//multiplicity
    for(int j=0;j<NUMBEROFDALICRYSTALS;j++) {
      crystalUsedForAddback[j] = false;
    }
  
    //cout<<"DALINaI: "<<Tree->DALINaI_<<endl;
    
    for(int j=0;j<Tree->DALINaI_;j++){
      fDali[j].id         = Tree->DALINaI_id[j]-1; 
      fDali[j].layer      = Tree->DALINaI_layer[j];
      fDali[j].theta      = fTheta[fDali[j].id];
      fDali[j].x          = fPosX[fDali[j].id];
      fDali[j].y          = fPosY[fDali[j].id];
      fDali[j].z          = fPosZ[fDali[j].id];
      fDali[j].e          = Tree->DALINaI_fEnergy[j];
      fDali[j].t          = Tree->DALINaI_fTimeOffseted[j];
      if(fDali[j].e>0){
        DopplerAngle = CalculateTheta(fDali[j].x,fDali[j].y,fDali[j].z,z_vertex);

        fDali[j].dopp[0] = DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);
        fDali[j].dopp[1] = DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);
        fDali[j].dopp[2] = DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);
        

        if(fDali[j].t>timeCutLow-500&&fDali[j].t<timeCutHigh+500)fDaliFold++;
        
        //   cout<<"DALIFold: "<<fDaliFold<<endl;
        if(fDali[j].t>timeCutLow && fDali[j].t<timeCutHigh){
          fDali[j].ttrue = true;
          fDaliFoldTa++;
        }
        else fDali[j].ttrue = false;
      }
      else {
        fDali[j].dopp[0] = -999.;
        fDali[j].dopp[1] = -999.;
        fDali[j].dopp[2] = -999.;
        fDali[j].dopp[3] = -999.;
        fDali[j].ttrue   = false;
      }

    }
      
    for(int j=Tree->DALINaI_;j<NUMBEROFDALICRYSTALS;j++){
      fDali[j].id         = -1;
      fDali[j].layer      = -1;
      fDali[j].theta      = -1;
      fDali[j].x          = -999;
      fDali[j].y          = -999;
      fDali[j].z          = -999;
      fDali[j].e          = -999;
      fDali[j].t          = -999;
      fDali[j].ttrue      = false;
      fDali[j].dopp[0]    = -999;
      fDali[j].dopp[1]    = -999;
      fDali[j].dopp[2]    = -999;
      fDali[j].dopp[3]    = -999;      
    }

    if(fDali[0].e>0) SortData(fDaliFold,fDali);

    //Going to the add-back:
    float dummyEnergy[NUMBEROFDALICRYSTALS][6] = {{0.}};
    //Making add-back and true multiplicity:
    //The Energy must be sorted already according to the highest detected one.
    //cout<<"Starting addback"<<endl;
    for(int i=0;i<fDaliFold;i++){

      if(crystalUsedForAddback[fDali[i].id] == true || fDali[i].ttrue == false) continue;

      DopplerAngle = CalculateTheta(fDali[i].x,fDali[i].y,fDali[i].z,z_vertex); 

      dummyEnergy[fDaliMultTa][0] = DopplerCorrect(beta_vertex,DopplerAngle,fDali[i].e);
      dummyEnergy[fDaliMultTa][1] = DopplerCorrect(beta_vertex,DopplerAngle,fDali[i].e);
      dummyEnergy[fDaliMultTa][2] = DopplerCorrect(beta_vertex,DopplerAngle,fDali[i].e);
        
      crystalUsedForAddback[fDali[i].id]=true;
      fDali[fDaliMultTa].idwa = fDali[i].id;
        
      for(int j = i+1;j<fDaliFold;j++)  {
        if(crystalUsedForAddback[fDali[j].id]==false && fDali[j].ttrue==true)  {
          for(int k = 0;k<fNumberOfAddbackPartners[fDali[i].id] ;k++) {
            if(fDali[j].id == fAddbackTable[fDali[i].id][k+1])  {
                
              crystalUsedForAddback[fDali[j].id]=true;
                
              DopplerAngle = CalculateTheta(fDali[i].x,fDali[i].y,fDali[i].z,z_vertex);

                dummyEnergy[fDaliMultTa][0] += DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);
                dummyEnergy[fDaliMultTa][1] += DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);
                dummyEnergy[fDaliMultTa][2] += DopplerCorrect(beta_vertex,DopplerAngle,fDali[j].e);
              
              
            }
          }
        }
      } 
      fDaliMultTa++;
    }
    for(int i = 0;i<fDaliMultTa;i++) {
      fDali[i].doppwa[0] = dummyEnergy[i][0];
      fDali[i].doppwa[1] = dummyEnergy[i][1];
      fDali[i].doppwa[2] = dummyEnergy[i][2];
    }
    for(int i = fDaliMultTa;i<NUMBEROFDALICRYSTALS;i++) {
      fDali[i].doppwa[0] = -999;
      fDali[i].doppwa[1] = -999;
      fDali[i].doppwa[2] = -999;
      fDali[i].idwa      = -999;
    }
          
     //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Starting with the analysis conditions
    // FIRST setting for Cr66
    bool br67mn = false; // (p,2p) channel
    bool br68fe = false; // (p,3p) channel
    bool br68mn = false; // (p,2pn) channel
    // SECOND setting for Fe72
    //bool br73co = false; // (p,2p) channel
    //bool br74ni = false; // (p,3p) channel
    //bool br74co = false; // (p,2pn) channel
    // THIRD setting for Ni78
    //bool br79cu = false; // (p,2p) channel
    //bool br80zn = false; // (p,3p) channel
    //bool br80cu = false; // (p,2pn) channel

    bool zd2p66cr = false;
    bool zd3p66cr = false;
    bool zd2pn66cr = false;
    //bool zd2p72fe = false;
    //bool zd3p72fe = false;
    //bool zd2pn72fe = false;
    //bool zd2p78ni = false;
    //bool zd3p78ni = false;
    //bool zd2pn78ni = false;

    //BRS
    if(brcut[0]->IsInside(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0])) br67mn = true;
    if(brcut[1]->IsInside(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0])) br68fe = true; 
    if(brcut[2]->IsInside(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0])) br68mn = true; 
 
    //ZDS
    if(zdcut[0]->IsInside(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3])) zd2p66cr = true;
    if(zdcut[1]->IsInside(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3])) zd3p66cr = true; 
    if(zdcut[2]->IsInside(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3])) zd2pn66cr = true; 

    //Beta diff
    float beta_diff = Tree->BigRIPSBeam_beta[0]-Tree->BigRIPSBeam_beta[3];
    if(br67mn&&zd2p66cr) h_beta_diff[0]->Fill(beta_diff);
    if(br68fe&&zd3p66cr) h_beta_diff[1]->Fill(beta_diff);
    if(br68mn&&zd2pn66cr) h_beta_diff[2]->Fill(beta_diff);

    //Trigger register information
    bool DSB = false;
    bool F11 = false;
    bool DaliTrigger = false;

    //Need to check the settings for PSP !!!!!!!!!!!!!!!!!!!!!!!!
    if(Tree->EventInfo_fBit[0]==13||Tree->EventInfo_fBit[0]==15) DaliTrigger = true;
    if(Tree->EventInfo_fBit[0]==10||Tree->EventInfo_fBit[0]==11||Tree->EventInfo_fBit[0]==15) DSB = true;
    if(Tree->EventInfo_fBit[0]==12||Tree->EventInfo_fBit[0]==14) F11 = true;
    
    //Introducing counter value here:
    //br67mn && zd2p66cr:0
    //br68fe && zd3p66cr:1
    //br68mn && zd2pn66cr:2
    
    //Getting the statistics for the cross-section:
    //Beam
    h_aoq_z[0]->Fill(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0]);
    if(DSB)
      h_aoq_z[1]->Fill(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0]);
    
    int counter = -1;

    //Asking for good condition of ppac and beta diff
    /*
    if(br67mn && zd2p66cr && Tree->fgoodppacfocusor[9]==1 && Tree->fgoodppacfocusor[11]==1 && 
       beta_diff>betaDiffLow && beta_diff < betaDiffHigh)
      counter = 0;
    else if(br68fe && zd3p66cr && Tree->fgoodppacfocusor[9]==1 && Tree->fgoodppacfocusor[11]==1 
            && beta_diff>betaDiffLow && beta_diff < betaDiffHigh)
      counter = 1;
    else if(br68mn && zd2pn66cr && Tree->fgoodppacfocusor[9]==1 && Tree->fgoodppacfocusor[11]==1 
            && beta_diff>betaDiffLow && beta_diff < betaDiffHigh)
      counter = 2;
    else continue;
    */

    if(br67mn && zd2p66cr)
      counter = 0;
    else if(br68fe && zd3p66cr)
      counter = 1;
    else if(br68mn && zd2pn66cr)
      counter = 2;
    else continue;

    for(int j=0;j<fDaliFold;j++){
      if(fDali[j].ttrue) {
        h_doppler[0+counter*10]->Fill(fDali[j].id,fDali[j].dopp[counter]);
        if(fDaliFold<3)  h_doppler[1+counter*10]->Fill(fDali[j].id,fDali[j].dopp[counter]);
        if(fDaliFold==1) h_doppler[2+counter*10]->Fill(fDali[j].id,fDali[j].dopp[counter]);
      }
    } 
    for(int j=0;j<fDaliMultTa;j++){
      h_doppler[3+counter*10]->Fill(fDali[j].idwa,fDali[j].doppwa[counter]);
      if(fDaliMultTa==1) h_doppler[4+counter*10]->Fill(fDali[0].idwa,fDali[0].doppwa[counter]);
      if(fDaliMultTa==2) h_doppler[5+counter*10]->Fill(fDali[j].idwa,fDali[j].doppwa[counter]);
      if(fDaliMultTa==3) h_doppler[6+counter*10]->Fill(fDali[j].idwa,fDali[j].doppwa[counter]);
      if(fDaliMultTa>3)  h_doppler[7+counter*10]->Fill(fDali[j].idwa,fDali[j].doppwa[counter]);
      if(fDaliMultTa<=2) {
        h_doppler[8+counter*10]->Fill(fDali[j].idwa,fDali[j].doppwa[counter]);
      }
      if(fDaliMultTa<=3) {
        h_doppler[9+counter*10]->Fill(fDali[j].idwa,fDali[j].doppwa[counter]);
      }
    }
    
    //ZDS
    h_aoq_z[2]->Fill(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3]);
    if(F11 && Tree->fgoodppacfocusor[9]==1 && Tree->fgoodppacfocusor[11]==1)
      h_aoq_z[3]->Fill(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3]);
  }
  
  for(int i=0;i<30;i++)
    h_doppler[i]->Write();
  
  for(int i=0;i<3;i++)
    h_beta_diff[i]->Write();
  
  for(int i=0;i<4;i++) {
    h_aoq_z[i]->Write();
  }  

  theApp->Run();
  return 0;
}
