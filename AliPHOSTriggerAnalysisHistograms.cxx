#include "AliPHOSTriggerAnalysisHistograms.h"

#include "TH1I.h"
#include "TH2I.h"
#include "TFile.h"

AliPHOSTriggerAnalysisHistograms::AliPHOSTriggerAnalysisHistograms()
  : fLGTSPeakCorrelation(0),
    fHGTSPeakCorrelation(0),
    fHGTSPeakRatio(0),
    fHGTSPeakCorrelationUS(0),
    fTriggerTime(0),
    fTriggeredSWLGTSPeakCorrelation(0),
    fTriggeredSWHGTSPeakCorrelation(0),
    fTriggeredSWHGTSPeakRatio(0),
    fTriggeredSWHGTSPeakCorrelationUS(0)
{
}


AliPHOSTriggerAnalysisHistograms::~AliPHOSTriggerAnalysisHistograms()
{
}


TH2I* AliPHOSTriggerAnalysisHistograms::GetLGTSPeakCorrelation()
{
  if( ! fLGTSPeakCorrelation ) {
    fLGTSPeakCorrelation = new TH2I("fLGTSPeakCorrelation", "LG-TS Peak Correlation", 
				    4092/4, 0, 4092,  1024/4, 0, 1024);
    fLGTSPeakCorrelation->GetXaxis()->SetTitle("LG 2x2");
    fLGTSPeakCorrelation->GetYaxis()->SetTitle("TS 2x2");
  }
  return fLGTSPeakCorrelation;
}

TH2I* AliPHOSTriggerAnalysisHistograms::GetHGTSPeakCorrelation()
{
  if( ! fHGTSPeakCorrelation ) {
    fHGTSPeakCorrelation = new TH2I("fHGTSPeakCorrelation", "HG-TS Peak Correlation", 
				    4092/4, 0, 4092,  1024/4, 0, 1024);
    fHGTSPeakCorrelation->GetXaxis()->SetTitle("HG 2x2");
    fHGTSPeakCorrelation->GetYaxis()->SetTitle("TS 2x2");
  }
  return fHGTSPeakCorrelation;
}


TH2I* AliPHOSTriggerAnalysisHistograms::GetHGTSPeakRatio()
{
  if( ! fHGTSPeakRatio ) {
    fHGTSPeakRatio = new TH2I("fHGTSPeakRatio", "HG-TS Peak Ratio", 
			      4092/4, 0, 4092,  1000, 0, 20);
    fHGTSPeakRatio->GetXaxis()->SetTitle("TS 2x2");
    fHGTSPeakRatio->GetYaxis()->SetTitle("HG 2x2 / TS 2x2");
  }
  return fHGTSPeakRatio;
}


TH2I* AliPHOSTriggerAnalysisHistograms::GetHGTSPeakCorrelationUS()
{
  if( ! fHGTSPeakCorrelationUS ) {
    fHGTSPeakCorrelationUS = new TH2I("fHGTSPeakCorrelationUS", "HG-TS Peak Correlation, UnSaturated", 
				    4092/4, 0, 4092,  1024, 0, 1024);
    fHGTSPeakCorrelationUS->GetXaxis()->SetTitle("HG 2x2");
    fHGTSPeakCorrelationUS->GetYaxis()->SetTitle("TS 2x2");
  }
  return fHGTSPeakCorrelationUS;
}


TH1I* AliPHOSTriggerAnalysisHistograms::GetTriggerTime()
{
  if( ! fTriggerTime ) {
    fTriggerTime = new TH1I("fTriggerTime", "TimeBin of Trigger", 
				    128, 0, 128);
    fTriggerTime->GetXaxis()->SetTitle("TimeBin");
  }
  return fTriggerTime;
}


TH2I* AliPHOSTriggerAnalysisHistograms::GetTriggeredSWLGTSPeakCorrelation()
{
  if( ! fTriggeredSWLGTSPeakCorrelation ) {
    fTriggeredSWLGTSPeakCorrelation = new TH2I("fTriggeredSWLGTSPeakCorrelation", 
					       "Triggered Sliding Windows LG-TS Peak Correlation", 
					       16368/4, 0, 16368,  4092/4, 0, 4092);
    fTriggeredSWLGTSPeakCorrelation->GetXaxis()->SetTitle("LG 4x4");
    fTriggeredSWLGTSPeakCorrelation->GetYaxis()->SetTitle("TS 4x4");
  }
  return fTriggeredSWLGTSPeakCorrelation;
}


TH2I* AliPHOSTriggerAnalysisHistograms::GetTriggeredSWHGTSPeakCorrelation()
{
  if( ! fTriggeredSWHGTSPeakCorrelation ) {
    fTriggeredSWHGTSPeakCorrelation = new TH2I("fTriggeredSWHGTSPeakCorrelation", 
					       "Triggered Sliding Windows HG-TS Peak Correlation", 
					       16368/4, 0, 16368,  4092/4, 0, 4092);
    fTriggeredSWHGTSPeakCorrelation->GetXaxis()->SetTitle("HG 4x4");
    fTriggeredSWHGTSPeakCorrelation->GetYaxis()->SetTitle("TS 4x4");
  }
  return fTriggeredSWHGTSPeakCorrelation;
}


TH2I* AliPHOSTriggerAnalysisHistograms::GetTriggeredSWHGTSPeakCorrelationUS()
{
  if( ! fTriggeredSWHGTSPeakCorrelationUS ) {
    fTriggeredSWHGTSPeakCorrelationUS = new TH2I("fTriggeredSWHGTSPeakCorrelationUS", 
					       "Triggered Sliding Windows HG-TS Peak Correlation, UnSaturated", 
					       16368/4, 0, 16368,  4092/4, 0, 4092);
    fTriggeredSWHGTSPeakCorrelationUS->GetXaxis()->SetTitle("HG 4x4");
    fTriggeredSWHGTSPeakCorrelationUS->GetYaxis()->SetTitle("TS 4x4");
  }
  return fTriggeredSWHGTSPeakCorrelationUS;
}


TH2I* AliPHOSTriggerAnalysisHistograms::GetTriggeredSWHGTSPeakRatio()
{
  if( ! fTriggeredSWHGTSPeakRatio ) {
    fTriggeredSWHGTSPeakRatio = new TH2I("fTriggeredSWHGTSPeakRatio", 
					 "Triggered Sliding Window HG-TS Peak Ratio", 
					 4092/4, 0, 4092,  1000, 0, 20);
    fTriggeredSWHGTSPeakRatio->GetXaxis()->SetTitle("TS 2x2");
    fTriggeredSWHGTSPeakRatio->GetYaxis()->SetTitle("HG 2x2 / TS 2x2");
  }
  return fTriggeredSWHGTSPeakRatio;
}




void AliPHOSTriggerAnalysisHistograms::SaveResults(TString fileName, TString options)
{
  TFile* file = new TFile(fileName, options);
  file->cd();
  
  GetLGTSPeakCorrelation()->Write();
  GetHGTSPeakCorrelation()->Write();
  GetHGTSPeakRatio()->Write();
  GetHGTSPeakCorrelationUS()->Write();
  GetTriggerTime()->Write();
  GetTriggeredSWLGTSPeakCorrelation()->Write();
  GetTriggeredSWHGTSPeakCorrelation()->Write();
  GetTriggeredSWHGTSPeakCorrelationUS()->Write();
  GetTriggeredSWHGTSPeakRatio()->Write();

  file->Close();
}

