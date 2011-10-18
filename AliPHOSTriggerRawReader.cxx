#include "AliPHOSTriggerRawReader.h"

#include "AliCaloRawStreamV3.h"
#include "AliPHOSTRURawReader.h"
#include "AliPHOSGeometry.h"


AliPHOSTriggerRawReader::AliPHOSTriggerRawReader()
  : fTRUs()
{
  vector<AliPHOSTRURawReader*> branches(kNBranches, NULL);
  vector<vector<AliPHOSTRURawReader*> > rows(kNTRURows, branches);
  fTRUs = vector<vector<vector<AliPHOSTRURawReader*> > >(kNMods, rows);
}
    
    
AliPHOSTriggerRawReader::~AliPHOSTriggerRawReader()
{
  for(int mod = 0; mod < kNMods; ++mod)
    for(int row = 0; row < kNTRURows; ++row)
      for(int branch = 0; branch < kNBranches; branch++)
	delete fTRUs[mod][row][branch];
}
 

AliPHOSTRURawReader* AliPHOSTriggerRawReader::GetTRU(int mod, int truRow, int branch)
{
  // if( ! fTRUs.at(mod).at(truRow).at(branch) )
  if( ! fTRUs[mod][truRow][branch] )
    fTRUs[mod][truRow][branch] = new AliPHOSTRURawReader();
  return fTRUs[mod][truRow][branch];
}
 

void AliPHOSTriggerRawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  while (rawStream->NextBunch()) {
    Int_t module = rawStream->GetModule();
    Int_t rcuRow = rawStream->GetRow();
    Int_t branch = 1 - rawStream->GetBranch(); // !!! Found this to be necessary, -Henrik Qvigstad <henrik.qvigstad@cern.ch>
    
    GetTRU(module, rcuRow, branch)->ReadFromStream(rawStream);
  } // end while 
}

void AliPHOSTriggerRawReader::Reset()
{
  const int nMod = AliPHOSGeometry::GetInstance()->GetNModules();
  
  for(int mod = 0; mod < nMod; ++mod)
    for(int truRow = 0; truRow < kNTRURows; ++truRow)
      for(int branch = 0; branch < kNBranches; branch++)
	if( fTRUs[mod][truRow][branch] )
	  fTRUs[mod][truRow][branch]->Reset();
}
