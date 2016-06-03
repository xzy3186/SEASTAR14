#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"
#include "TArtMINOSParameters.hh"
#include "TArtMINOSPara.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibDALI.hh"
#include "TArtCalibMINOS.hh"
#include "TArtAnalyzedMINOS.hh"
#include "TArtTrackMINOS.hh"
#include "TArtVertexMINOS.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtEventInfo.hh"
#include "TArtPlastic.hh"
#include "TArtPPAC.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"

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

// void MakeAllTree(char *infile, char *outfile="run0025.root"){
int main(int argc, char** argv){

  if(argc!=2){
    cout << "Usage: ./maketree <run_number>" << endl;
    exit(-1);
  }

  //  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  // ========= Define input and output files
  Int_t RunNumber = std::atoi(argv[argc-1]);
  cout << endl << "---> Start analyzing Run# = " << RunNumber << endl;
  char* inputfilename  = (char*)Form("./ridf/psp14%04d.ridf",RunNumber);
  char* outputfilename = (char*)Form("./rootfiles/test_momi_run%04d.root",RunNumber);

  // Create StoreManager both for calibration "TArtCalib..." and treatment "TArtReco..."
  //------------------------------------------------------------------------------------
  TArtStoreManager * sman = TArtStoreManager::Instance();
  
  // Create EventStore to control the loop and get the EventInfo
  //------------------------------------------------------------
/*
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop); 
  estore->Open(infile);
  std::cout<<"estore ->"<< infile <<std::endl;
*/

  cout << "---> Open file name = " << inputfilename << endl << endl;
  TArtEventStore *estore = new TArtEventStore();
  estore->SetInterrupt(&stoploop); 
  int es = estore->Open(inputfilename);
  if(es==0){
    cout << endl << " === No data file === " << endl << endl;
    exit(-1);
  }
  cout << endl << endl;

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

  // Create RecoPID to get calibrated data and to reconstruct TOF, AoQ, Z, ... (RecoPID -> 
  //[ RecoTOF , RecoRIPS , RecoBeam] )
  TArtRecoPID *recopid = new TArtRecoPID();

  //para->PrintListOfPPACPara();
  //return;

  // Definition of observables we want to reconstruct
  std::cout << "Defining bigrips parameters" << std::endl;  
  //TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat",5.45); // F3 - F5
  //TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat",4.665); // F5 - F7
  //TArtRIPS *rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat",3.831); // F8 - F9
  //TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat",3.805); // F9 - F11  
  TArtRIPS *rips3to5 =  recopid->DefineNewRIPS(3, 5,"matrix/mat1.mat","D3"); // F3 - F5  
  TArtRIPS *rips5to7 =  recopid->DefineNewRIPS(5, 7,"matrix/mat2.mat","D5"); // F5 - F7
  TArtRIPS *rips8to9 =  recopid->DefineNewRIPS(8, 9,"matrix/F8F9_LargeAccAchr.mat","D7"); // F8 - F9
  TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat","D8"); // F9 - F11 

/*
  TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3,5,"matrix/mat1.mat",6.303); // F3 - F5
  TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5,7,"matrix/mat2.mat",5.788); // F5 - F7
  TArtRIPS *rips8to9 = recopid->DefineNewRIPS(8,9,"matrix/F8F9_LargeAccAchr.mat",4.609); // F8 - F9
  TArtRIPS *rips9to11 = recopid->DefineNewRIPS(9,11,"matrix/F9F11_LargeAccAchr.mat",4.566); // F9 - F11  
*/
  // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
  TArtTOF *tof3to7  = recopid->DefineNewTOF("F3pl","F7pl",301.,5); // F3 - F7
  //TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-160.5,9); // F8 - F11
  TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",-160.-3,9); // F8 - F11
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
  //TArtCalibDALI *dalicalib2 = new TArtCalibDALI();

  // Create MINOSParameters to get ".xml"
  //------------------------------------

  TArtMINOSParameters *mpara = new TArtMINOSParameters("MINOSParameters","MINOSParameters");
  mpara->LoadParameters("./db/MINOS.xml");

  // Parameters for the MINOS ANALYSIS
  string ConfigBeam;
  double MINOSthresh;
  double TimeBinElec; //in ns
  double VDrift; //in cm/µs
  double Tshaping; // in ns
  double DelayTrig; // in ns
  double DALIOffset;

  ifstream ConfigFile;
  ConfigFile.open("./../ConfigFileSetup.txt");
  string Header;
  ConfigFile >> ConfigBeam;
  ConfigFile >> Header >> MINOSthresh;
  ConfigFile >> Header >> TimeBinElec;
  ConfigFile >> Header >> VDrift;
  ConfigFile >> Header >> Tshaping;
  ConfigFile >> Header >> DelayTrig;
  ConfigFile >> Header >> DALIOffset;
  ConfigFile.close();

  
  // Create CalibMINOS to get and calibrate raw data
  //-----------------------------------------------
  TArtCalibMINOS *minoscalib = new TArtCalibMINOS();
  TArtAnalyzedMINOS *minosanalyzed = new TArtAnalyzedMINOS(minoscalib);
  TArtTrackMINOS *minostrack = new TArtTrackMINOS();
  //TArtTrackMINOSData *minostrackdata = new TArtTrackMINOSData();
  TArtVertexMINOS *minosvertex = new TArtVertexMINOS();

  std::cout<<"para"<<std::endl;

/*
  TFile *fout = new TFile(outfile,"RECREATE");
  TTree *tree = new TTree("tree","tree");
*/

  cout << endl << "---> Output file name = " << outputfilename << endl << endl;
  TFile *fout = new TFile(outputfilename,"RECREATE");
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

  TClonesArray *analyzedminos_array = 
    (TClonesArray *)sman->FindDataContainer("AnalyzedMINOS");
  std::cout<<analyzedminos_array->GetName()<<std::endl;
  tree->Branch(analyzedminos_array->GetName(),&analyzedminos_array);

  TClonesArray *trackminos_array = 
    (TClonesArray *)sman->FindDataContainer("TrackMINOS");
  std::cout<<trackminos_array->GetName()<<std::endl;
  tree->Branch(trackminos_array->GetName(),&trackminos_array);

/*
  //minos
  anaminos_array =(TClonesArray *)sman->FindDataContainer("AnalyzedMINOS");
  tree->Branch(anaminos_array->GetName(),&anaminos_array);
  trackminos_array =(TClonesArray *)sman->FindDataContainer("TrackMINOS");
  tree->Branch(trackminos_array->GetName(),&trackminos_array);
*/

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

  tree->Branch("xtar",&fXTar,"fXTar/D");
  tree->Branch("ytar",&fYTar,"fYTar/D");
  //tree->Branch("f8ppacx",fF8PPACX,"fF8PPACX[6]/D");
  //tree->Branch("f8ppacy",fF8PPACY,"fF8PPACY[6]/D");
  //tree->Branch("f8posx",fF8PosX,"fF8PosX[3]/D");
  //tree->Branch("f8posy",fF8PosY,"fF8PosY[3]/D");
  tree->Branch("fgoodppacfocus",fGoodPPACFocus,"fGoodPPACFocus[12]/I");
  tree->Branch("fgoodppacfocusor",fGoodPPACFocusOr,"fGoodPPACFocusOr[12]/I");
    
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

  Double_t z_vertex=0.;
  tree->Branch("z_vertex",&z_vertex,"z_vertex/D");
  Double_t beta_vertex=0.;
  tree->Branch("beta_vertex",&beta_vertex,"beta_vertex/D");

  int neve = 0;
  while(estore->GetNextEvent()){
    if(neve%1000==0)
      std::cout << "event: " << neve << std::endl;
      
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Making the BigRIPS tree calibration
    brcalib->ClearData();
    brcalib->ReconstructData();
    
    //Reconstructiong the PID
    recopid->ClearData();
    recopid->ReconstructData();
  
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
    
    bool  fired[14] = {false};
    bool  posRec = false;
    
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

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Making MINOS
    minoscalib->ClearData();
    minosanalyzed->ClearData();
    minostrack->ClearData();
    minosvertex->ClearData();
    minoscalib->ReconstructData();
    //Added on 3rd May 18:47
    minosanalyzed->SetConfig(VDrift/100., TimeBinElec, DelayTrig); // RUN minosdali9048.ridf
    minosanalyzed->ReconstructData(); 
    minostrack->ReconstructData();
    minosvertex->ReconstructVertex();
    // Convert the reconstructed vertex in z (mm)
    //z_vertex = minosvertex->GetZvCal(1e-3*VDrift, TimeBinElec, DelayTrig); //time bin*(ns/us)*vdrift(cm/us) == z_vertex in cm
    z_vertex = minosvertex->GetZv(); //time bin*(ns/us)*vdrift(cm/us) == z_vertex in cm
    beta_vertex = minosvertex->GetBetaV();
    //cout << minostrackdata->GetEntriesFast() << endl;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Making DALI
    dalicalib->ClearData();
    //dalicalib->SetPlTime(plasticcalib->FindPlastic("F8pl")->GetTime());
    dalicalib->SetVertex(z_vertex-DALIOffset/10.);
    dalicalib->SetBeta(beta_vertex);

/*
    dalicalib2->ClearData();
    //dalicalib2->SetPlTime(plasticcalib->FindPlastic("F8pl")->GetTime());
    //dalicalib2->SetVertex(z_vertex-DALIOffset/10.);
    //dalicalib2->SetBeta(beta_vertex);
*/
       //Add above to remove F8plastic tof.
    dalicalib->ReconstructData();
    //dalicalib2->ReconstructData();
    
    dalimultwotime = dalicalib->GetMultWithoutT();
    dalimult = dalicalib->GetMult();
    dalitimetruemult = dalicalib->GetTimeTrueMult();
    dalimultthres = dalicalib->GetMultThres();
    dalitimetruemultthres = dalicalib->GetTimeTrueMultThres();
 


    tree->Fill();
    neve++;
  }
  cout<<"Writing the tree."<<endl;
  
  fout->Write();
  fout->Close();
}
