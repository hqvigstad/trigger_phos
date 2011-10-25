#ifndef ALIPHOSTRIGGERANALYSISHISTOGRAMS_H
#define ALIPHOSTRIGGERANALYSISHISTOGRAMS_H

#include "TString.h"
class TH1I;
class TH2I;


class AliPHOSTriggerAnalysisHistograms
{
 public:
  AliPHOSTriggerAnalysisHistograms();
  ~AliPHOSTriggerAnalysisHistograms();
  
  TH1I* GetTRUActive();
  TH2I* GetLGTSPeakCorrelation();
  TH2I* GetLGTSPeakCorrelationA();
  TH2I* GetHGTSPeakCorrelation();
  TH2I* GetHGTSPeakCorrelationA();
  TH2I* GetHGTSPeakRatio();
  TH2I* GetHGTSPeakCorrelationUS();
  TH1I* GetTriggerTime();
  TH2I* GetTriggeredSWLGTSPeakCorrelation();
  TH2I* GetTriggeredSWHGTSPeakCorrelation();
  TH2I* GetTriggeredSWHGTSPeakCorrelationUS();
  TH2I* GetTriggeredSWHGTSPeakRatio();
  
  void SaveResults(TString filename, TString options = "RECREATE");

  const static int kNMods = 5;
  const static int kNTRURows = 4;
  const static int kNBranches = 2;

 protected:
  TH1I* fTRUActive;
  TH2I* fLGTSPeakCorrelation;
  TH2I* fLGTSPeakCorrelationA;
  TH2I* fHGTSPeakCorrelation;
  TH2I* fHGTSPeakCorrelationA;
  TH2I* fHGTSPeakRatio;
  TH2I* fHGTSPeakCorrelationUS;
  TH1I* fTriggerTime;
  TH2I* fTriggeredSWLGTSPeakCorrelation;
  TH2I* fTriggeredSWHGTSPeakCorrelation;
  TH2I* fTriggeredSWHGTSPeakRatio;
  TH2I* fTriggeredSWHGTSPeakCorrelationUS;

};


#endif
