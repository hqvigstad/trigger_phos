#include "TChain.h"

void truPedestalAnalysis(TString rawFileList = "files.txt")
{
  TChain *rawChain = new TChain("RAW");
  addFilesToChain(rawFileList, rawChain );

  AliTRUPedestalAnalysis* anaObj = new AliTRUPedestalAnalysis();
}