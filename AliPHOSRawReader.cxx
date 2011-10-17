#include "AliPHOSRawReader.h"

#include "AliPHOSEMCRawReader.h"
#include "AliPHOSTRURawReader.h"
#include "AliCaloRawStreamV3.h"


AliPHOSRawReader::AliPHOSRawReader()
  :fHGReader(new AliPHOSEMCRawReader),
   fLGReader(new AliPHOSEMCRawReader),
   fTRUReaders({{{0}}})
{}


AliPHOSRawReader::~AliPHOSRawReader()
{
  delete fHGReader;
  delete fLGReader;

  for(int mod = 0; mod < kNMods; ++mod)
    for(int row = 0; row < kNTRURows; ++row)
      for(int branch = 0; branch < kNBranches; ++branch)
	delete fTRUReaders[mod][row][branch];
}


void AliPHOSRawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  fHGReader->Reset();
  fLGReader->Reset();
  for(int mod = 0; mod < kNMods; ++mod)
    for(int row = 0; row < kNTRURows; ++row)
      for(int branch = 0; branch < kNBranches; ++branch)
	if( fTRUReaders[mod][row][branch] )
	  fTRUReaders[mod][row][branch]->Reset();
  

  while (rawStream->NextDDL()) {
    while (rawStream->NextChannel()) {
      Int_t caloFlag = rawStream->GetCaloFlag();
      if(caloFlag == 0) // low gain
	{
	  fLGReader->ReadFromStream(rawStream);
	}
      else if(caloFlag == 1) // high gain
	{
	  fHGReader->ReadFromStream(rawStream);
	}
      else if (rawStream->IsTRUData()) // tru
	{
	  while (rawStream->NextBunch()) {
	    Int_t mod = rawStream->GetModule();
	    Int_t row = rawStream->GetRow();
	    Int_t branch = 1 - rawStream->GetBranch(); // !!! Found this to be necessary, -Henrik Qvigstad <henrik.qvigstad@cern.ch>
	    
	    fTRUReaders[mod][row][branch]->ReadFromStream(rawStream);

	  } // end NextBunch()
	}
    } // end of NextChannel()
  } // end of NextDDL()
}
