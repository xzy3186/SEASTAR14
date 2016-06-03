//Example how to run Analysis
// ./runAnalysis 0.5  0.5  0.5 -12 12 15 0.03 0.049 
// with ./runAnalsysis beta1 beta2 beta3 timeCutLow timeCutHigh maxaddbackdistance betaDiffLow betaDiffHigh
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

  if(argc!=9) {
    std::cout << "Invalid arguments..." << endl;
    return 1;
  }

  double beta1 = atof(argv[1]);
  double beta2 = atof(argv[2]);
  double beta3 = atof(argv[3]);

  cout<<"The beta values are: "<<beta1<<" "<<beta2<<" "<<beta3<<" "<<endl;

  double timeCutLow = atof(argv[4]);
  double timeCutHigh = atof(argv[5]);

  //Create the add back table
  CreateAddBackTable(atof(argv[6]));
  
  double betaDiffLow = atof(argv[7]);
  double betaDiffHigh = atof(argv[8]);

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
    sprintf(name,"brcut%i",i);
    BRcut[i]->GetObject(name,brcut[i]);
  }
  
  for(int i=0;i<3;i++) {
    sprintf(name,"zdcut%i",i);
    ZDcut[i]->GetObject(name,zdcut[i]); 
  }
  
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
    h_aoq_z[i] = new TH2F(name,name,500,2.0,3.0,250,40,60);
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
    
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
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
        //DopplerAngle = CalculateTheta(fDali[j].x,fDali[j].y,fDali[j].z,0); //To be used only for psp
                                   
        fDali[j].dopp[0] = DopplerCorrect(beta1,fDali[j].theta,fDali[j].e);
        fDali[j].dopp[1] = DopplerCorrect(beta2,fDali[j].theta,fDali[j].e);
        fDali[j].dopp[2] = DopplerCorrect(beta3,fDali[j].theta,fDali[j].e);

        //For PSP:
        /*
        fDali[j].dopp[0] = DopplerCorrect(beta1,DopplerAngle,fDali[j].e);
        fDali[j].dopp[1] = DopplerCorrect(beta2,DopplerAngle,fDali[j].e);
        fDali[j].dopp[2] = DopplerCorrect(beta3,DopplerAngle,fDali[j].e);
        */

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
      
    if(fDali[0].e>0)
      SortData(fDaliFold,fDali);
      
    //Going to the add-back:
    float dummyEnergy[NUMBEROFDALICRYSTALS][6] = {{0.}};
    //Making add-back and true multiplicity:
    //The Energy must be sorted already according to the highest detected one.
    //cout<<"Starting addback"<<endl;
    for(int i=0;i<fDaliFold;i++){
      
      if(crystalUsedForAddback[fDali[i].id] == true || fDali[i].ttrue == false) continue;

      dummyEnergy[fDaliMultTa][0] = DopplerCorrect(beta1,fDali[i].theta,fDali[i].e);
      dummyEnergy[fDaliMultTa][1] = DopplerCorrect(beta2,fDali[i].theta,fDali[i].e);
      dummyEnergy[fDaliMultTa][2] = DopplerCorrect(beta3,fDali[i].theta,fDali[i].e);
        
      crystalUsedForAddback[fDali[i].id]=true;
      fDali[fDaliMultTa].idwa = fDali[i].id;
        
      for(int j = i+1;j<fDaliFold;j++)  {
        if(crystalUsedForAddback[fDali[j].id]==false && fDali[j].ttrue==true)  {
          for(int k = 0;k<fNumberOfAddbackPartners[fDali[i].id] ;k++) {
            if(fDali[j].id == fAddbackTable[fDali[i].id][k+1])  {
                
              crystalUsedForAddback[fDali[j].id]=true;
                
              //DopplerAngle = CalculateTheta(fDali[i].x,fDali[i].y,fDali[i].z,0); //To be used only for psp
              
              dummyEnergy[fDaliMultTa][0] += DopplerCorrect(beta1,fDali[i].theta,fDali[j].e);
              dummyEnergy[fDaliMultTa][1] += DopplerCorrect(beta2,fDali[i].theta,fDali[j].e);
              dummyEnergy[fDaliMultTa][2] += DopplerCorrect(beta3,fDali[i].theta,fDali[j].e);
              
              //For psp:
              /*
                dummyEnergy[fDaliMultTa][0] += DopplerCorrect(beta1,DopplerAngle,fDali[j].e);
                dummyEnergy[fDaliMultTa][1] += DopplerCorrect(beta2,DopplerAngle,fDali[j].e);
                dummyEnergy[fDaliMultTa][2] += DopplerCorrect(beta3,DopplerAngle,fDali[j].e);
              */
              
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
    bool br130cd = false;
    bool br131in = false;
    bool br132sn = false;
    //bool br136sn = false;
    //bool br137sb = false;
    //bool br138te = false;

    bool zd130cd = false;
    bool zd131in = false;
    bool zd132sn = false;
    //bool zd136sn = false;
    //bool zd137sb = false;
    //bool zd138te = false;

    //BRS
    if(brcut[0]->IsInside(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0])) br130cd = true;
    if(brcut[1]->IsInside(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0])) br131in = true; 
    if(brcut[2]->IsInside(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0])) br132sn = true; 
 
    //ZDS
    if(zdcut[0]->IsInside(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3])) zd130cd = true;
    if(zdcut[1]->IsInside(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3])) zd131in = true; 
    if(zdcut[2]->IsInside(Tree->BigRIPSBeam_aoq[3],Tree->BigRIPSBeam_zet[3])) zd132sn = true; 

    //Beta diff
    float beta_diff = Tree->BigRIPSBeam_beta[0]-Tree->BigRIPSBeam_beta[3];
    if(br130cd&&zd130cd) h_beta_diff[0]->Fill(beta_diff);
    if(br131in&&zd131in) h_beta_diff[1]->Fill(beta_diff);
    if(br132sn&&zd132sn) h_beta_diff[2]->Fill(beta_diff);

    //Trigger register information
    bool DSB = false;
    bool F11 = false;
    bool DaliTrigger = false;

    //Need to check the settings
    if(Tree->EventInfo_fBit[0]==13||Tree->EventInfo_fBit[0]==15) DaliTrigger = true;
    if(Tree->EventInfo_fBit[0]==10||Tree->EventInfo_fBit[0]==11||Tree->EventInfo_fBit[0]==15) DSB = true;
    if(Tree->EventInfo_fBit[0]==12||Tree->EventInfo_fBit[0]==14) F11 = true;
    
    //Introducing counter value here:
    //br130cd && zd130cd:0
    //br131in && zd131in:1
    
    //Getting the statistics for the cross-section:
    //Beam
    h_aoq_z[0]->Fill(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0]);
    if(DSB)
      h_aoq_z[1]->Fill(Tree->BigRIPSBeam_aoq[0],Tree->BigRIPSBeam_zet[0]);
    
    int counter = -1;

    //Asking for good condition of ppac and beta diff
    /*
    if(br130cd && zd130cd && Tree->fgoodppacfocusor[9]==1 && Tree->fgoodppacfocusor[11]==1 && 
       beta_diff>betaDiffLow && beta_diff < betaDiffHigh)
      counter = 0;
    else if(br131in && zd131in && Tree->fgoodppacfocusor[9]==1 && Tree->fgoodppacfocusor[11]==1 
            && beta_diff>betaDiffLow && beta_diff < betaDiffHigh)
      counter = 1;
    else if(br132sn && zd132sn && Tree->fgoodppacfocusor[9]==1 && Tree->fgoodppacfocusor[11]==1 
            && beta_diff>betaDiffLow && beta_diff < betaDiffHigh)
      counter = 2;
    else continue;
    */

    if(br130cd && zd130cd)
      counter = 0;
    else if(br131in && zd131in)
      counter = 1;
    else if(br132sn && zd132sn)
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
