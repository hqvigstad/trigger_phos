#include "TH1I.h"

void extractTRUPedestals(TString fileName = "")
{			
  gStyle->SetOptStat(10);
  
  const int kNMods = 5;
  const int kNTRURows = 4;
  const int kNBranches = 2;
  const int kN2x2X = 64/2;
  const int kN2x2Z = 56/2;
  const int kN2x2XPrTRURow = kN2x2X / kNTRURows;
  const int kN2x2ZPrBranch = kN2x2Z / kNBranches;

  TFile* file = new TFile(fileName, "READ");
  
  TH1I* rmsHist = new TH1I("RMS", "RMS,", 1000, 0, 10);
  TH1I* meanHist = new TH1I("Mean", "Mean,", 10000, 0, 1000);
  TH2I* mrHist = new TH2I("Mean-RMS", "Mean-RMS", 1000, 0, 1000, 1000, 0, 10);

  TH2F* rms_2d[5] = {0};
  TH2F* mean_2d[5] = {0};

  
  for(int mod = 0; mod < kNMods; ++mod) {
    for(int truRow = 0; truRow < kNTRURows; ++truRow) {
      for(int branch = 0; branch < kNBranches; ++branch) {
	for(int xIdx = 0; xIdx < kN2x2XPrTRURow; ++xIdx) {
	  for(int zIdx = 0; zIdx < kN2x2ZPrBranch; ++zIdx) {
	    const int modX = kN2x2XPrTRURow * truRow + xIdx;
	    const int modZ = kN2x2ZPrBranch * branch + zIdx;

	    TH1I* pedHist = 0;
	    char name[256];
	    sprintf(name, "fTRUSignal_m%02d_t%02d_b%02d_x%02d_z%02d", mod, truRow, branch, xIdx, zIdx);
	    file->GetObject(name, pedHist);

	    if(pedHist) {
	      const double rms = pedHist->GetRMS();
	      const double mean = pedHist->GetMean();
	  
	      rmsHist->Fill(rms);
	      meanHist->Fill(mean);
	      mrHist->Fill(mean, rms );
	      
	      if( ! rms_2d[mod] ){
		char rms_2dname[256];
		sprintf(rms_2dname, "rms_2d[%d]", mod);
		char rms_2dtitle[256];
		sprintf(rms_2dtitle, "RMS mod:%d", mod);
		rms_2d[mod] = new TH2F(rms_2dname, rms_2dtitle, kN2x2X, 0, kN2x2X, kN2x2Z, 0, kN2x2Z);
		rms_2d[mod]->GetXaxis()->SetTitle("2x2 x");
		rms_2d[mod]->GetYaxis()->SetTitle("2x2 z");
	      }
	      rms_2d[mod]->SetBinContent(modX+1, modZ+1, rms);

	      if( ! mean_2d[mod] ){
		char mean_2dname[256];
		sprintf(mean_2dname, "mean_2d[%d]", mod);
		char mean_2dtitle[256];
		sprintf(mean_2dtitle, "Mean mod:%d", mod);
		mean_2d[mod] = new TH2F(mean_2dname, mean_2dtitle, kN2x2X, 0, kN2x2X, kN2x2Z, 0, kN2x2Z);
		mean_2d[mod]->GetXaxis()->SetTitle("2x2 x");
		mean_2d[mod]->GetYaxis()->SetTitle("2x2 z");
	      }
	      mean_2d[mod]->SetBinContent(modX+1, modZ+1, mean);
	    
	    } // if(pedHist)
	  } // x
	} // z
      } // branch
    } // tru
  }// mod
  
  new TCanvas;
  rmsHist->DrawCopy();
  new TCanvas;
  meanHist->DrawCopy();

  TCanvas* canv = new TCanvas;
  canv->Divide(3, 2);
  for(int x = 0; x < 3; ++x){
    canv->cd( x+1 );
    mean_2d[x+2]->GetZaxis()->SetRangeUser(512-5, 512+5);
    mean_2d[x+2]->DrawCopy("colz");
    
    canv->cd( x+4 );
    gPad->SetLogz();
    rms_2d[x+2]->DrawCopy("colz");
  }

  
  // (new TCanvas)->SetLogz();
  // (new TCanvas)->SetLogz(false);
  // mean_2d[3]->GetZaxis()->SetRangeUser(512-5, 512+5);
  // mean_2d[3]->DrawCopy("colz");

  
  
  
  //TH1I* fTRUSignal[kNMods][kNTRURows][kNBranches][kN2x2XPrTRURow][kN2x2ZPrBranch];
  //fTRUSignal[mod][truRow][branch][xIdx][zIdx] = pedHist;
}

