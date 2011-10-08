#include "AliPHOSEMCRawReader.h"
#include "AliPHOSGeometry.h"

AliPHOSEMCRawReader::AliPHOSEMCRawReader()
  : fSignals(0)
{
  size_type nM = AliPHOSGeometry::GetInstance()->GetNModules();
  size_type nX = AliPHOSGeometry::GetInstance()->GetNPhi();
  size_type nZ = AliPHOSGeometry::GetInstance()->GetNZ();
  size_type nT = AliPHOSEMCRawReader::kDefaultNTimeBins;

  // Creating fSignals from lower to higher type
  vector<Short_t> sT(nT, 0);
  vector<vector<Short_t>> sZ(nZ, sT);
  vector<vector<vector<Short_t>>> sX(nX, sZ);
  fSignals = vector<vector<vector<vector<Short_t>>>>(nT, sX);
}

AliPHOSEMCRawReader::~AliPHOSEMCRawReader()
{
}

AliPHOSEMCRawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  // Unknown Author (Henrik Qvigstad <henrik.qvigstad@cern.ch>)

  while (stream->NextBunch()) {
    const UShort_t *sig = stream->GetSignals();
    Int_t sigStart = stream->GetStartTimeBin();
    Int_t sigLength = stream->GetBunchLength();
    Int_t mod = stream->GetModule();
    Int_t colx = stream->GetCellX();
    Int_t rowz = stream->GetCellZ();
    UInt_t value = stream->GetAltroCFG1();
    Short_t offset=0,threshold=0;//?????????????????????????????????????????????????/
    Bool_t ZeroSuppressionEnabled = (value >> 15) & 0x1;
    if(ZeroSuppressionEnabled) {
      offset = (value >> 10) & 0xf;
      threshold = value & 0x3ff;
    }
    Int_t timeBin = stream->GetHWAddress() & 0x7f;///??????????????????????????????/
    
    for (Int_t i = 0; i < sigLength; i++) {
      if((sigStart-i-3<64) and (sigStart-i-3 > 0))
	{
	  // fSignals[mod][colx][rowz-1][0]=sig[i]-offset;
	  fSignals[mod][colx][rowz][sigStart-i-3] = sig[i]-offset;
	}//-------------------???????????????//type?
    }
  }
}
  
AliPHOSEMCRawReader::Reset()
{
  for(unsigned int mod = 0; mod < AliPHOSEMCRawReader::kDefaultNTimeBins; ++mod)
    for(unsigned int rowx; rowx < AliPHOSGeometry::GetInstance()->GetNPhi(); ++rowx)
      for(unsigned int colz; colz < AliPHOSGeometry::GetInstance()->GetNZ(); ++colz)
	for(unsigned int timeBin; timeBin < fSignals[mod][rowx][colz].size(); ++timeBin)
	  fSignals[mod][rowx][colz][timeBin] = 0;
}
