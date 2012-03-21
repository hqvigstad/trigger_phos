#include "TH1I.h"
#include "TH2I.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "AliPHOSTriggerAnalysisHistograms.h"
#include "AliPHOSTriggerParameters.h"
const int kNMods = 5;
const int kNTRURows = 4;
const int kNBranches = 2;
const int kN2x2X = 64/2;
const int kN2x2Z = 56/2;
const int kN2x2XPrTRURow = kN2x2X / kNTRURows;
const int kN2x2ZPrBranch = kN2x2Z / kNBranches;

int roundToInt(double value)
{
  return (int) value + 0.5;
}

void extractTRUPedestals(TString fileName = "", bool plottPedestals = true, AliPHOSTriggerParameters* params = 0)
{			
  AliPHOSTriggerAnalysisHistograms* triggerHistograms = new AliPHOSTriggerAnalysisHistograms(fileName);
  
  
  TH1I* rmsHist = new TH1I("RMS", "RMS,", 1000, 0, 10);
  TH1I* meanHist = new TH1I("Mean", "Mean,", 10000, 0, 1000);
  TH2I* mrHist = new TH2I("Mean-RMS", "Mean-RMS", 1000, 0, 1000, 1000, 0, 10);
  
  TH2F* rms_2d[5] = {0};
  TH2F* mean_2d[5] = {0};
  
  
  for(int mod = 0; mod < kNMods; ++mod) {
    // RMS 2d hist
    char rms_2dname[256];
    sprintf(rms_2dname, "rms_2d[%d]", mod);
    char rms_2dtitle[256];
    sprintf(rms_2dtitle, "RMS mod:%d", mod);
    rms_2d[mod] = new TH2F(rms_2dname, rms_2dtitle, kN2x2X, 0, kN2x2X, kN2x2Z, 0, kN2x2Z);
    rms_2d[mod]->GetXaxis()->SetTitle("2x2 x");
    rms_2d[mod]->GetYaxis()->SetTitle("2x2 z");
    
    // mean 2d hist
    char mean_2dname[256];
    sprintf(mean_2dname, "mean_2d[%d]", mod);
    char mean_2dtitle[256];
    sprintf(mean_2dtitle, "Mean mod:%d", mod);
    mean_2d[mod] = new TH2F(mean_2dname, mean_2dtitle, kN2x2X, 0, kN2x2X, kN2x2Z, 0, kN2x2Z);
    mean_2d[mod]->GetXaxis()->SetTitle("2x2 x");
    mean_2d[mod]->GetYaxis()->SetTitle("2x2 z");
    
    for(int truRow = 0; truRow < kNTRURows; ++truRow) {
      for(int branch = 0; branch < kNBranches; ++branch) {
	for(int xIdx = 0; xIdx < kN2x2XPrTRURow; ++xIdx) {
	  for(int zIdx = 0; zIdx < kN2x2ZPrBranch; ++zIdx) {
	    
	    if( triggerHistograms->IsInitTRUSignal(mod, truRow, branch, xIdx, zIdx) ){
	      const int modX = kN2x2XPrTRURow * truRow + xIdx;
	      const int modZ = kN2x2ZPrBranch * branch + zIdx;
	      
	      const TH1I* pedHist = triggerHistograms->GetTRUSignal(mod, truRow, branch, xIdx, zIdx);
	      const double rms = pedHist->GetRMS();
	      const double mean = pedHist->GetMean(); // pedestal
	      
	      // OCDB object
	      if( params )
		params->SetTRUPedestal(roundToInt(mean), mod, truRow, branch, xIdx, zIdx);
	      
	      // plotting
	      rmsHist->Fill(rms);
	      meanHist->Fill(mean);
	      mrHist->Fill(mean, rms );
	      
	      rms_2d[mod]->SetBinContent(modX+1, modZ+1, rms);
	      mean_2d[mod]->SetBinContent(modX+1, modZ+1, mean);
	    } // end if( pedHist )
	  } // end x
	} // end z
      } // end branch
    } // end tru
  }// end mod
  
  
  if( plottPedestals ) {
        
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
      canv->GetPad( x+4 )->SetLogz();
      rms_2d[x+2]->DrawCopy("colz");
    }
  }
}
