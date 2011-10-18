void runPHOSTriggerAnalysis()
{
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_ROOT/RAW");
  gSystem->AddIncludePath("-I$ALICE_ROOT/PHOS");
  gSystem->AddIncludePath("-I$ALICE_ROOT/STEER");
  
  gROOT->LoadMacro("AliPHOSTRURawReader.cxx+g");
  gROOT->LoadMacro("AliPHOSTriggerRawReader.cxx+g");
  gROOT->LoadMacro("AliPHOSEMCRawReader.cxx+g");
  gROOT->LoadMacro("AliPHOSRawReader.cxx+g");
  //gROOT->LoadMacro("AliPHOSTriggerCalibData.cxx+g");
  gROOT->LoadMacro("AliPHOSTriggerParameters.cxx+g");
  
  gROOT->LoadMacro("AliPHOSTriggerAnalysisHistograms.cxx+g");
  gROOT->LoadMacro("AliPHOSTriggerAnalysis.cxx++g");

  gROOT->LoadMacro("phosTriggerAnalysis.C+g");
  
  
  TGrid::Connect("alien://");
  phosTriggerAnalysis("files.txt");
}
