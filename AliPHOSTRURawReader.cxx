#include "AliPHOSTRURawReader.h"

#include "AliCaloRawStreamV3.h"
#include "AliPHOSTRURegionRawReader.h"
#include "AliPHOSGeometry.h"


AliPHOSTRURawReader::AliPHOSTRURawReader()
  : fRegions()
{
  const int nMod = AliPHOSGeometry::GetInstance()->GetNModules();
  
  vector<AliPHOSTRURegionRawReader*> sBranches(kNBranches, NULL);
  vector<vector<AliPHOSTRURegionRawReader*> > sRows(kNTRURows, sBranches);
  fRegions = vector<vector<vector<AliPHOSTRURegionRawReader*> > >(nMod, sRows);
}
    
    
AliPHOSTRURawReader::~AliPHOSTRURawReader()
{
  const int nMod = AliPHOSGeometry::GetInstance()->GetNModules();

  for(int mod = 0; mod < nMod; ++mod)
    for(int row = 0; row < kNTRURows; ++row)
      for(int branch = 0; branch < kNBranches; branch++)
	delete fRegions[mod][row][branch];
}
 

AliPHOSTRURegionRawReader* AliPHOSTRURawReader::GetTRURegion(int mod, int truRow, int branch)
{
  if( ! fRegions[mod][truRow][branch] )
    fRegions[mod][truRow][branch] = new AliPHOSTRURegionRawReader();
  return fRegions[mod][truRow][branch];
}
 

void AliPHOSTRURawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  while (rawStream->NextBunch()) {
    Int_t module = rawStream->GetModule();
    Int_t rcuRow = rawStream->GetRow();
    Int_t branch = 1 - rawStream->GetBranch(); // !!! Found this to be necessary, -Henrik Qvigstad <henrik.qvigstad@cern.ch>
    
    GetTRURegion(module, rcuRow, branch)->ReadFromStream(rawStream);
  } // end while 
}

void AliPHOSTRURawReader::Reset()
{
  const int nMod = AliPHOSGeometry::GetInstance()->GetNModules();
  
  for(int mod = 0; mod < nMod; ++mod)
    for(int truRow = 0; truRow < kNTRURows; ++truRow)
      for(int branch = 0; branch < kNBranches; branch++)
	if( fRegions[mod][truRow][branch] )
	  fRegions[mod][truRow][branch]->Reset();
}
