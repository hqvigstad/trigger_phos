#ifndef ALIPHOSTRIGGERANALYSISHISTOGRAMS_H
#define ALIPHOSTRIGGERANALYSISHISTOGRAMS_H

#include "TString.h"
class TH1I;
class TH2I;


class AliPHOSTriggerAnalysisHistograms
{
  AliPHOSTriggerAnalysisHistograms();
  AliPHOSTriggerAnalysisHistograms(TString fileName);
  ~AliPHOSTriggerAnalysisHistograms();
  
  TH2I* GetLGTSPeakCorrelation();
  TH2I* GetHGTSPeakCorrelation();
  TH2I* GetHGTSPeakRatio();
  TH2I* GetHGTSPeakCorrelationUS();
  TH1I* GetTriggerTime();
  TH2I* GetTriggeredSWLGTSPeakCorrelation();
  TH2I* GetTriggeredSWHGTSPeakCorrelation();
  TH2I* GetTriggeredSWHGTSPeakRatio();
  TH2I* GetTriggeredSWHGTSPeakCorrelationUS();
  
  int SaveResults(TString filename, TString options = "RECREATE");

 protected:
  TH2I* fLGTSPeakCorrelation;
  TH2I* fHGTSPeakCorrelation;
  TH1I* fHGTSPeakRatio;
  TH2I* fHGTSPeakCorrelationUS;
  TH1I* fTriggerTime;
  TH2I* fTriggeredSWLGTSPeakCorrelation;
  TH2I* fTriggeredSWHGTSPeakCorrelation;
  TH1I* fTriggeredSWHGTSPeakRatio;
  TH2I* fTriggeredSWHGTSPeakCorrelationUS;

};


#endif
