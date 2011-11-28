void testExtractTRUPedestals(TString fileName = "triggerResultsAll.root")
{
  gROOT->LoadMacro("AliPHOSTriggerAnalysisHistograms.cxx+g");
  gROOT->LoadMacro("AliPHOSTriggerParameters.cxx+g");
  
  gROOT->LoadMacro("extractTRUPedestals.C+g");
  gROOT->LoadMacro("readTRUPedestals.C+g");
  
  
 
  AliPHOSTriggerParameters* parameters = new AliPHOSTriggerParameters();
  extractTRUPedestals(fileName, true, parameters);
  drawTRUPedestals(parameters);
}
