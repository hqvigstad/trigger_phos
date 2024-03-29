#ifndef ALIPHOSTRIGGERANALYSISHISTOGRAMS_H
#define ALIPHOSTRIGGERANALYSISHISTOGRAMS_H

#include "TString.h"
class TFile;
class TH1I;
class TH2I;


class AliPHOSTriggerAnalysisHistograms
{
 public:
  AliPHOSTriggerAnalysisHistograms();
  AliPHOSTriggerAnalysisHistograms(TString fileName);
  ~AliPHOSTriggerAnalysisHistograms();
  
  TH1I* GetTRUActive();
  TH1I* GetTRUSignal(int mod, int truRow, int branch, int xIdx, int zIdx);
  TH2I* GetTRUSignalTime();
  TH2I* GetLGSignalTime();
  TH2I* GetHGSignalTime();
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

  bool IsInitTRUSignal(int mod, int truRow, int branch, int xIdx, int zIdx) {return fTRUSignal[mod][truRow][branch][xIdx][zIdx];}
  
  void SaveResults(TString filename, TString options = "RECREATE");

  const static int kNMods = 5;
  const static int kNTRURows = 4;
  const static int kNBranches = 2;
  const static int kN2x2X = 64/2;
  const static int kN2x2Z = 56/2;
  const static int kN2x2XPrTRURow = kN2x2X / kNTRURows;
  const static int kN2x2ZPrBranch = kN2x2Z / kNBranches;
  const static int kNTRUTimeBins = 128;
  const static int kNEMCTimeBins = 62;

 protected:
  TFile* fFile;

  TH1I* fTRUActive;
  TH1I* fTRUSignal[kNMods][kNTRURows][kNBranches][kN2x2XPrTRURow][kN2x2ZPrBranch];
  TH2I* fTRUSignalTime;
  TH2I* fLGSignalTime;
  TH2I* fHGSignalTime;
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
