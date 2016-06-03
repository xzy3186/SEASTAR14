#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtDALIParameters.hh"
#include "TArtCalibDALI.hh"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TClonesArray.h"

#include "signal.h"

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt()
{
  printf("keyboard interrupt\n");
  stoploop = true;
}

void MakeDALITree(char *infile, char *outfile="dalia.root"){
  gSystem->Load("libXMLParser.so");
  gSystem->Load("libanaroot.so");

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop);
  estore->Open(infile);
  //estore->Open();

  TArtDALIParameters *dpara = TArtDALIParameters::Instance();
  dpara->LoadParameter("db/DALI.xml");
  TArtCalibDALI *dalicalib= new TArtCalibDALI();

  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *tree = new TTree("tree","tree");

  // define data nodes which are supposed to be dumped to tree 
  TClonesArray * info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
  std::cout<<info_array->GetName()<<std::endl;
  tree->Branch(info_array->GetName(),&info_array);

  TClonesArray * dali_array=
     (TClonesArray *)sman->FindDataContainer("DALINaI");
  tree->Branch(dali_array->GetName(),&dali_array);

  Int_t dalimultwithoutt = 0;
  Int_t dalimult = 0;
  Int_t dalitimetruemult = 0;
  Int_t dalimultthres = 0;
  Int_t dalitimetruemultthres = 0;

  tree->Branch("dalimultwithoutt",&dalimultwithoutt,"dalimultwithoutt/I");
  tree->Branch("dalimult",&dalimult,"dalimult/I");
  tree->Branch("dalitimetruemult",&dalitimetruemult,"dalitimetruemult/I");
  tree->Branch("dalimultthres",&dalimultthres,"dalimultthres/I");
  tree->Branch("dalitimetruemultthres",&dalitimetruemultthres,"dalitimetruemultthres/I");
  

  int neve = 0;
  while(estore->GetNextEvent()){
    if(neve%1000==0)
      std::cout << "event: " << neve << std::endl;

    dalicalib->ClearData();
    dalicalib->ReconstructData();

    dalimultwithoutt = dalicalib->GetMultWithoutT();
    dalimult = dalicalib->GetMult();
    dalitimetruemult = dalicalib->GetTimeTrueMult();
    dalimultthres = dalicalib->GetMultThres();
    dalitimetruemultthres = dalicalib->GetTimeTrueMultThres();

    tree->Fill();
    neve ++;
  }
  fout->Write();
  //fout->Close();

 

}

