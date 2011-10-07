#include "AliPHOSRawReader.h"
#include "AliPHOSEMCRawReader.h"
#include "AliPHOSTRURawReader.h"



AliPHOSRawReader::AliPHOSRawReader()
  :fHGReader(new AliPHOSEMCRawReader),
   fLGReader(new AliPHOSEMCRawReader),
   fTRUReader(new AliPHOSTRURawReader)
{}


AliPHOSRawReader::~AliPHOSRawReader()
{
  delete fHGReader;
  delete fLGReader;
  delete fTRUReader;
}


void AliPHOSRawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  fHGReader.Reset();
  fLGReader.Reset();
  fTRUReader.Reset();

  while (raw->NextDDL()) {
    while (raw->NextChannel()) {
      Int_t caloFlag = raw->GetCaloFlag();
      if(caloFlag == 0) // low gain
	{
	  fLGReader.ReadFromStream(rawStream);
	}
      else if(caloFlag == 1) // high gain
	{
	  fHGReader.ReadFromStream(rawStream);
	}
      else if (raw->IsTRUData()) // tru
	{
	  fTRUReader.ReadFromStream(rawStream);
	}
    } // end of NextChannel()
  } // end of NextDDL()
}
