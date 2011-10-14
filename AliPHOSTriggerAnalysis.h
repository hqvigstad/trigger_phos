#ifndef ALIPHOSTRIGGERANALYSIS_H
#define ALIPHOSTRIGGERANALYSIS_H

class AliRawReaderChain;
class AliPHOSTriggerAnalysisHistograms;
class AliPHOSRawReader;
class AliPHOSEMCRawReader;
class AliPHOSTRURawReader;
class AliPHOSTRUCalibData;
class AliPHOSTriggerParameters;


#include "TString.h"

#include <vector>

class AliPHOSTriggerAnalysis
{
 public:
  AliPHOSTriggerAnalysis();
  ~AliPHOSTriggerAnalysis();
  
  void ProcessEvent(AliPHOSRawReader* );

  void SaveResults(TString filename = "AliPHOSTriggerAnalysisResults.root") const;

  void SetAnalyseModule(int mod, bool analyse = true) {fModules[mod] = analyse;}
  void SetVerbose(int vValue = 1) {fVerbose = vValue;}
  void SetTriggerParameters(AliPHOSTriggerParameters* parameters) {fParameters = parameters;}
    
  static int Get2x2Signal(AliPHOSEMCRawReader*, int mod, int xIdx, int zIdx, int timeBin);
  static int Get2x2Signal(AliPHOSTRURawReader*, AliPHOSTriggerParameters*, int mod, int xIdx, int zIdx, int timeBin);
  static int Get2x2Max(AliPHOSEMCRawReader*, int mod, int xIdx, int zIdx);
  static int Get2x2Max(AliPHOSTRURawReader*, AliPHOSTriggerParameters*, int mod, int xIdx, int zIdx);
  static int Get4x4Max(AliPHOSEMCRawReader*, int mod, int TRURow, int branch, int xIdx, int zIdx);
  static int Get4x4Max(AliPHOSTRURawReader*, AliPHOSTriggerParameters*, int mod, int TRURow, int branch, int xIdx, int zIdx);
  static bool Is2x2Saturated(AliPHOSEMCRawReader*, int mod, int xIdx, int zIdx, int satThreshold);
  static bool Is4x4Saturated(AliPHOSEMCRawReader*, int mod, int TRURow, int branch, int xIdx, int zIdx, int satThreshold);
    
  
  const static int kNTRURows = 4;
  const static int kNBranches = 2;
  const static int kN2x2X = 64/2;
  const static int kN2x2Z = 56/2;
  const static int kN2x2XPrTRURow = kN2x2X / kNTRURows;
  const static int kN2x2ZPrBranch = kN2x2X / kNBranches;
  const static int kN4x4XPrTRURow = kN2x2XPrRow -1;
  const static int kN4x4ZPrBranch = kN2x2ZPrBranch -1;
  const static int kNTRUTimeBins = 128;
  
 protected:
  int fVerbose; // , level of verbosity, 0="silent", 1=moderately, 2=excessively 
  std::vector<bool> fModules; // , per module: should analyser analyse module
  UShort_t fSaturationThreshold;
  AliPHOSTriggerParameters* fParameters;
  
  AliPHOSTriggerAnalysisHistograms* fHistograms; // , where histograms reside
  

};

#endif
