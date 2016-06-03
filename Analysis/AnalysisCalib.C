// For vdrift ::: merging of all files to look at drift times for all files at the same time...
#include "EventCalib.h"
#include "TChain.h"

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

#include "TMinosClust.h"

using namespace std;


//__________________________________________________________  
int main(int argc, char **argv) {

  // Create interactive interface
  TRint *theApp = new TRint("ROOT example", &argc, argv, NULL, 0);

  EventCalib *Tree = new EventCalib();
  
  Long64_t nentries = Tree->fChain->GetEntriesFast();
  cout<<"nentries = "<<nentries <<endl;

  TFile *rootfile = new TFile("Ni77_each_run.root","RECREATE");
  rootfile->cd();

  //Define spectra:
  int minBin = 0;
  int maxBin = 10000;
  int binning = 10;
  int numBin = (maxBin-minBin)/binning;

  //________________________________________________________________________
  //Spectra:

  TH1F *h_DriftTime = new TH1F("h_DriftTime","h_DriftTime",numBin,minBin,maxBin);
  TH1F *h_DriftTimeSelect = new TH1F("h_DriftTimeSelect","h_DriftTimeSelect",numBin,minBin,maxBin);

  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //Start looping through data;
  
  Long64_t i=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    if(jentry%10000 ==0) cout << jentry <<"/"<<nentries<<" Events DONE!"<<endl;
    Tree->fChain->GetEvent(jentry);   

	for(int ii=0; ii<Tree->fitdata_; ii++) {
		if(Tree->fitdata_t_ns[ii]>-10000 && Tree->fitdata_Chargemax[ii]>100) {
			h_DriftTime->Fill(Tree->fitdata_t_ns[ii]);
			if(Tree->Rpadnumber_max<10) h_DriftTimeSelect->Fill(Tree->fitdata_t_ns[ii]);
		}
		
	}

  }


  h_DriftTime->Write();
  h_DriftTimeSelect->Write();

  theApp->Run();
  return 0;
}
