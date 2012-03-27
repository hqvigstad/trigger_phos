void runTRUPedestalAnalysis(char* filesList = "files.txt")
{
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_ROOT/PHOS");
  gSystem->AddIncludePath("-I$ALICE_ROOT/RAW");


  gROOT->LoadMacro("AliTRUPedestalOutput.cxx+g");
  gROOT->LoadMacro("AliTRUPedestalAnalysis.cxx+g");

  gROOT->LoadMacro("truPedestalAnalysis.C+g");
  
  
  //TGrid::Connect("alien://");
  //AliCDBManager::Instance()->SetDefaultStorage("raw://");

  truPedestalAnalysis(filesList);
}
