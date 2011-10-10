#include "AliPHOSTRURawReader.h"

#include "AliPHOSTRURegionRawReader.h"


AliPHOSTRURawReader::AliPHOSTRURawReader()
  : fRegions(0)
{
  int nMod = AliPHOSGeometry::GetInstance()->GetNModules();
  
  vector<AliPHOSTRURegionRawReader*> sBranches(kNBranches, NULL);
  vector<vector<Short_t>> sRows(kNTRURows, sBranches);
  fRegions = <vector<vector<Short_t>>>(nMod, sRows);
}
    
    
AliPHOSTRURawReader::~AliPHOSTRURawReader()
{
  int nMod = AliPHOSGeometry::GetInstance()->GetNModules();

  for(int mod = 0; mod < nMod; ++mod)
    for(int row = 0; row < kNTRURows; ++row)
      for(int branch = 0; branch < kNBranches; branch++)
	delete fRegions[mod][row][branch];
      
}
 

AliPHOSTRURawReader::GetTRURegion(int mod, int truRow, int branch)
{
  if( ! fRegions[mod][truRow][branch] )
    fRegions[mod][truRow][branch] = new AliPHOSTRURegionRawReader();
  return fRegions[mod][truRow][branch];
}
 

void AliPHOSTRURawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  while (rawStream->NextBunch()) {
    Int_t module = stream->GetModule();
    Int_t rcuRow = stream->GetRow();
    Int_t branch = 1 - stream->GetBranch(); // !!! Found this to be necessary, -Henrik Qvigstad <henrik.qvigstad@cern.ch>
    
    GetTRURegion(module, rcuRow, branch)->ReadFromStream(rawStream);
  } // end while 
}

void AliPHOSTRURawReader::Reset()
{
  for(int mod = 0; mod < nMod; ++mod)
    for(int row = 0; row < kNTRURows; ++row)
      for(int branch = 0; branch < kNBranches; branch++)
	if( fRegions[mod][truRow][branch] )
	  fRegions[mod][truRow][branch]->Reset();
}
