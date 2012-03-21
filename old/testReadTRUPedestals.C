void testReadTRUPedestals()
{
  gROOT->ProcessLine(".include $ALICE_ROOT/PHOS");
  gROOT->LoadMacro("readTRUPedestals.C+g");

  AliPHOSTriggerParameters* parameters = new AliPHOSTriggerParameters();

  readTRUPedestals(parameters);
  drawTRUPedestals(parameters);
}
  
