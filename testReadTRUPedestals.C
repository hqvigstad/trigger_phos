void testReadTRUPedestals()
{
  gROOT->LoadMacro("AliPHOSTriggerParameters.cxx+g");
  gROOT->LoadMacro("readTRUPedestals.C+g");

  AliPHOSTriggerParameters* parameters = new AliPHOSTriggerParameters();

  readTRUPedestals(parameters);
  drawTRUPedestals(parameters);
}
  
