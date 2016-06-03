/*
 * ShowMINOSOnline.cc
 * written by A. Obertelli, 28/03/2014
 */
#include <iostream>

#include "TArtAnaFile.hh"
#include "TArtAnaClock.hh"

#include "EWNum.hh"

#include "TArtCore.hh"
#include "TArtStoreManager.hh"

#include "TArtMINOSParameters.hh"
#include "TArtMINOSPara.hh"
#include "TArtCalibMINOS.hh"
#include "TArtCalibMINOSData.hh"

#include "ShowMINOSOnline.hh"

#include "TMath.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TClonesArray.h"

#include "segidlist.hh"

// function to exit loop at keyboard interrupt. 
bool stoploop = false;
void stop_interrupt(int){
  printf("keyboard interrupt\n");
  stoploop = true;
}

void instanceProj(TH2Poly *hxypol){

    double PI = TMath::Pi();
	double theta[256], r[18];
	int k=0;
	TGraph *g[4608];
	float anglepad=2.*PI/256.;
	
	for(int i=0; i<18; i++)    // loop on r (y direction in pad numbering)
    	{
        	for(int j=0; j<256; j++)  // loop on theta (x direction in pad numbering)
        	{
			r[i]=(45.2+(i+0.5)*2.1);
            		theta[j]=(j-31.5)*anglepad + 1.5*PI;
            		double ypoly[4]={(r[i]-1.05)*sin(theta[j]-anglepad/2.),(r[i]-1.05)*sin(theta[j]+anglepad/2.),(r[i]+1.05)*sin(theta[j]+anglepad/2.),(r[i]+1.05)*sin(theta[j]-anglepad/2.)};
            		double xpoly[4]={(r[i]-1.05)*cos(theta[j]-anglepad/2.),(r[i]-1.05)*cos(theta[j]+anglepad/2.),(r[i]+1.05)*cos(theta[j]+anglepad/2.),(r[i]+1.05)*cos(theta[j]-anglepad/2.)};
            		g[k]=new TGraph(4,xpoly,ypoly);
            		hxypol->AddBin(g[k]);
           		k++;
        	}
        
    	}

}

void instanceCst(TH2Poly *hxypol){

    double PI = TMath::Pi();
	double r,thetaperring;
    TGraph *g[3604]; 
    float anglepad;
    int start;
    int padsPring[18]={36,38,39,41,43,44,46,48,49,51,53,54,56,57,59,61,62,64};
   	int k = 0;

    for(int i=0; i<18; i++)    // loop on r (y direction in pad numbering)
    {
	
		r = (45.2+(i+0.5)*2.1);	
		anglepad = (2.*PI/(4*padsPring[i]));
		if((padsPring[i])%2==0) start = -(int)(padsPring[i]/2);
		else start = -(int)((padsPring[i]+1)/2);

		for(int j=0; j<(padsPring[i]*4); j++)  // loop on theta (x direction in pad numbering)
	  	{
	   		thetaperring = ((j+start)*anglepad+3*PI/2.); //to be inizialized
	    		double ypoly[4]={(r-1.05)*sin(thetaperring-anglepad/2.),(r-1.05)*sin(thetaperring+anglepad/2.),(r+1.05)*sin(thetaperring+anglepad/2.),(r+1.05)*sin(thetaperring-anglepad/2.)};
	    		double xpoly[4]={(r-1.05)*cos(thetaperring-anglepad/2.),(r-1.05)*cos(thetaperring+anglepad/2.),(r+1.05)*cos(thetaperring+anglepad/2.),(r+1.05)*cos(thetaperring-anglepad/2.)};
	    		g[k]=new TGraph(4,xpoly,ypoly);
	    		hxypol->AddBin(g[k]);
	   		k++;
		}
        
   }

}

ShowMINOSOnline::ShowMINOSOnline() : TArtAnaLoop(), minosparameters(0) {
}

ShowMINOSOnline::~ShowMINOSOnline() {
}

void ShowMINOSOnline::Construct() {
  cout<<"call const!!"<<endl; 

  sman = TArtStoreManager::Instance();

  tree = new TTree("tree","tree");

  //Canvases for Online
  cMINOS = new TCanvas("cMINOS","                 1 Event: XY, Q(t) & Asics                                     ---                                      Accumulated 2D views                                           ---                                         Accumulated Histogram statistics ",1500,1000);
  cMINOS->Divide(3,3);

  hpoly1 = new TH2Poly("hpoly1","XY view - 1 event",-85,85,-85,85);
  //instanceProj(hpoly1);
  instanceCst(hpoly1);
  hQt1 = new TH2F("hQt1","Q(t) signal - 1 event",516,-2,514,500,0,4000);

  hpoly = new TH2Poly("hpoly","Accumulated XY view",-85,85,-85,85);
  //instanceProj(hpoly);
  instanceCst(hpoly);
  hrt = new TH2F("hrt","Accumulated RT view",560,-10,550,40,3.2,87.2);

  hasic1 = new TH1I("hasic1","Asic hit pattern - 1evt",100,0,100);
  hasic = new TH1I("hasic","Asic hit pattern",100,0,100);
  htest = new TH1I("htest","htest",100,0,100);
  hasicmax = new TH1I("hasicmax","Maximum hit Asic pattern",70,0,70);

  hxyvertex = new TH2F("hxyvertex","Vertex reconstruction xy",50,-50,50,50,-50,50);
  hzvertex = new TH1F("hzvertex","Vertex reconstruction along beam",400,-50,350);


  minosparameters = TArtMINOSParameters::Instance();
  minosparameters->LoadParameters("db/MINOS.xml");

  CalibMINOS = new TArtCalibMINOS();

  //TClonesArray *info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
  //std::cout<<info_array->GetName()<<std::endl;
  //tree->Branch(info_array->GetName(),&info_array);
  //TClonesArray *minos_array = (TClonesArray *)sman->FindDataContainer("MINOS");

  AnalyzedMINOS = new TArtAnalyzedMINOS(CalibMINOS);
  TrackMINOS = new TArtTrackMINOS();
  VertexMINOS = new TArtVertexMINOS();

  neve = 0;

}

void ShowMINOSOnline::Calculate()  {
  //cout<<"call Calculate()"<<endl;
  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  TArtRawEventObject *rawevent = (TArtRawEventObject *)sman->FindDataContainer("RawEvent");

  int id = 0;
  int Refresh = 100;
  int eventnum = rawevent->GetEventNumber();

  Double_t xPad=0., yPad=0.;

  CalibMINOS->ReconstructData();

  TArtCalibMINOSData *minos = new TArtCalibMINOSData();

  cout<<"Event : "<<neve<<" Size : "<<CalibMINOS->GetNumCalibMINOS()<<endl;

  if(neve%Refresh==0) {hQt1->Reset(); hpoly1->Reset(""); hasic1->Reset();}

  //read all Pads
  for(Int_t i=0;i<CalibMINOS->GetNumCalibMINOS();i++) {
	minos = CalibMINOS->GetCalibMINOS(i);
	xPad = minos->GetX();
	yPad = minos->GetY();

	if( !(xPad==0 && yPad==0) ) {
		Double_t maxCharge = 0.;
		hasic->Fill(minos->GetFec()*4+minos->GetAsic());
		if(neve%Refresh==0) hasic1->Fill(minos->GetFec()*4+minos->GetAsic());
		htest->Fill(minos->GetFec()*4+minos->GetAsic());
		Double_t Ring = sqrt(xPad*xPad + yPad*yPad);
		for(Int_t j=0;j<minos->GetNData();j++) 
		{
			if(minos->GetCalibValue(j)>maxCharge) maxCharge = minos->GetCalibValue(j);
			hrt->Fill(minos->GetCalibTime(j),Ring);
			if(neve%Refresh==0) hQt1->Fill(minos->GetCalibTime(j),minos->GetCalibValue(j));
		}
		hpoly->Fill(xPad, yPad, maxCharge);
		if(neve%Refresh==0) hpoly1->Fill(xPad, yPad, maxCharge);
	}

	//cout<<"Fec : "<<minos->GetFec()<<"   Asic :"<<minos->GetAsic()<<" ch : "<<minos->GetChannel()<<endl;
  }
  //cerr << "Max=" << htest->GetMaximum() << endl;
  hasicmax->Fill(htest->GetMaximum());

  AnalyzedMINOS->ReconstructData(); 

  //Tracks
  TrackMINOS->ReconstructData();
  //fTrackMINOSArray=TrackMINOS->GetTrackMINOSArray();
  
  //Vertex
  VertexMINOS->ReconstructVertex();
  hxyvertex->Fill(VertexMINOS->GetXv(), VertexMINOS->GetYv());
  hzvertex->Fill(VertexMINOS->GetZv());


  tree->Fill();
  neve ++;

  if(neve%Refresh==0) {
  	cMINOS->cd(1);
    	hpoly1->SetMinimum(1);
   	hpoly1->Draw("colz");
  	cMINOS->cd(4);
  	hQt1->Draw();
  	cMINOS->cd(7);
  	hasic1->Draw();

  	cMINOS->cd(2);
    	hpoly->SetMinimum(1);
   	hpoly->Draw("colz");
  	cMINOS->cd(5);
  	hrt->Draw("colz");
  
  	cMINOS->cd(3);
  	hasic->Draw();
  	cMINOS->cd(6);
 	hasicmax->Draw();
  

  	cMINOS->cd(8);
 	hxyvertex->Draw("colz");
  	cMINOS->cd(9);
 	hzvertex->Draw();
  
  	cMINOS->Update();

  }

  //********************* Clear *****************
  CalibMINOS->ClearData();
  AnalyzedMINOS->ClearData();
  TrackMINOS->ClearData();
  VertexMINOS->ClearData();
  rawevent->Clear();
  htest->Reset();

}

void ShowMINOSOnline::Destruct()  {
cout<<"call Destruct() !!" <<endl;
}

const char* ShowMINOSOnline::ClassName() const  {
  return "ShowMINOSOnline";
}
