void runTRUPedestalsAnalysis(char* filesList = "files.txt")
{
  gSystem->AddIncludePath("-I$ALICE_ROOT/PHOS");


  gROOT->LoadMacro("AliTRUPedestalAnalysis.cxx+g");

  gROOT->LoadMacro("truPedestalAnalysis.C+g");
  
  
  TGrid::Connect("alien://");
  AliCDBManager::Instance()->SetDefaultStorage("raw://");

  truPedestalAnalysis(filesList);
}
