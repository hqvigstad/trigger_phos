#ifndef PHOSRAWANALYSIS_H
#define PHOSRAWANALYSIS_H


class AliRawReaderChain;
class AliPHOSTriggerAnalysisHistograms;
class AliPHOSRawReader;

#include "TString.h"

#include <vector>

class PHOSRawAnalysis
{
 public:
  PHOSRawAnalysis();
  ~PHOSRawAnalysis();
  
  void ProcessEvent(AliPHOSRawReader* );

  int SaveResults(TString filename = "PHOSRawAnalysisResults.root") const;

  void SetVerbose(int vValue = 1) {fVerbose = vValue;}
  void SetAnalyseModule(int mod, bool analyse = true) {fModules[mod] = analyse;}
  
    
 protected:
  Int_t fVerbose; // , level of verbosity, 0="silent", 1=moderately, 2=excessively 
  std::vector<bool> fModules; // , per module: should analyser analyse module
  Int_t fSaturationThreshold;
  
  AliPHOSTriggerAnalysisHistograms* fHistograms; // , where histograms reside
  
  const static int kNTimeBins = 128;
  const static int kNTRURows = 4;
  const static int kNBranches = 2;

  static int Get2x2Signal(AliPHOSEMCReader* reader, int mod, int xIdx, int zIdx, int& maxIsAtTime);
  static int Get2x2Max(AliPHOSEMCReader* reader, int mod, int xIdx, int zIdx, int& maxIsAtTime);
  static int Get2x2Max(AliPHOSTRUReader* reader, int mod, int xIdx, int zIdx, int& maxIsAtTime);
  static int Get4x4Max(AliPHOSEMCReader* reader, int mod, int TORRow, int branch, int xIdx, int zIdx, int& maxIsAtTime);
  static int Get4x4Max(AliPHOSTRUReader* reader, int mod, int TORRow, int branch, int xIdx, int zIdx, int& maxIsAtTime);
  static bool Is2x2Saturated(AliPHOSEMCReader* reader, int mod, int xIdx, int zIdx, int satThreshold);
  static bool Is4x4Saturated(AliPHOSEMCReader* reader, int mod, int TORRow, int branch, int xIdx, int zIdx, int satThreshold);
};

#endif
