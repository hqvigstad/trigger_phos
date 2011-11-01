void testExtractTRUPedestals()
{
  gROOT->LoadMacro("AliPHOSTriggerAnalysisHistograms.cxx+g");
  gROOT->LoadMacro("AliPHOSTriggerParameters.cxx+g");
  
  gROOT->LoadMacro("extractTRUPedestals.C+g");
  gROOT->LoadMacro("readTRUPedestals.C+g");
  
  gStyle->SetOptStat(10);
 
  AliPHOSTriggerParameters* parameters = new AliPHOSTriggerParameters();
  extractTRUPedestals("triggerResultsAll.root", false, parameters);
  drawTRUPedestals(parameters);
}
