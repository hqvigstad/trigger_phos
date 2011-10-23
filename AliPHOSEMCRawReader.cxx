#include "AliPHOSEMCRawReader.h"

#include "AliPHOSGeometry.h"
#include "AliCaloRawStreamV3.h"

AliPHOSEMCRawReader::AliPHOSEMCRawReader()
  : fSignals(),
    fActive()
{
  if( ! AliPHOSGeometry::GetInstance() )
    AliPHOSGeometry::GetInstance("PHOS", "PHOS");
  
  const int nM = AliPHOSGeometry::GetInstance()->GetNModules();
  const int nX = AliPHOSGeometry::GetInstance()->GetNPhi();
  const int nZ = AliPHOSGeometry::GetInstance()->GetNZ();
  const int nT = AliPHOSEMCRawReader::kDefaultNTimeBins;

  // Creating fSignals from lower to higher type
  vector<Short_t> sT(nT, 0);
  vector<vector<Short_t> > sZ(nZ, sT);
  vector<vector<vector<Short_t> > > sX(nX, sZ);
  fSignals = vector<vector<vector<vector<Short_t> > > >(nM, sX);

  fActive = vector<bool>(nM, false);

  vector<bool> aZ(nZ, false);
  vector<vector<bool> > aX(nX, aZ);
  fActiveCell = vector<vector<vector<bool> > >(nM, aX);
}

AliPHOSEMCRawReader::~AliPHOSEMCRawReader()
{
}

void AliPHOSEMCRawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  // Unknown Author (Henrik Qvigstad <henrik.qvigstad@cern.ch>)

  while (rawStream->NextBunch()) {
    const UShort_t *sig = rawStream->GetSignals();
    Int_t sigStart = rawStream->GetStartTimeBin();
    Int_t sigLength = rawStream->GetBunchLength();
    Int_t mod = rawStream->GetModule();
    Int_t rowx = rawStream->GetCellX();
    Int_t colz = rawStream->GetCellZ();
    UInt_t value = rawStream->GetAltroCFG1();
    Short_t offset=0,threshold=0;
    Bool_t ZeroSuppressionEnabled = (value >> 15) & 0x1;
    if(ZeroSuppressionEnabled) {
      offset = (value >> 10) & 0xf;
      threshold = value & 0x3ff;
    }
    //Int_t timeBin = rawStream->GetHWAddress() & 0x7f;

    for (Int_t i = 0; i < sigLength; i++) {
      if((sigStart-i-3<64) and (sigStart-i-3 > 0))
	{
	  // fSignals[mod][rowx][colz-1][0]=sig[i]-offset;
	  fSignals[mod][rowx][colz][sigStart-i-3] = sig[i]-offset;
	}//-------------------???????????????//type?
    }
    
    fActive[mod] = true;
    fActiveCell[mod][rowx][colz] = true;
  }
}
  
void AliPHOSEMCRawReader::Reset()
{
  const int nM = AliPHOSGeometry::GetInstance()->GetNModules();
  const int nX = AliPHOSGeometry::GetInstance()->GetNPhi();
  const int nZ = AliPHOSGeometry::GetInstance()->GetNZ();

  for(int mod = 0; mod < nM; ++mod) {
    if(fActive[mod]){
      for(int rowx = 0; rowx < nX; ++rowx) {
	for(int colz = 0; colz < nZ; ++colz) {
	  if( fActiveCell[mod][rowx][colz] ) {
	    for(unsigned int timeBin = 0; timeBin < fSignals[mod][rowx][colz].size(); ++timeBin){
	      fSignals[mod][rowx][colz][timeBin] = 0;
	    } // time
	    fActiveCell[mod][rowx][colz] = false;
	  } // end if fActive Cell
	} // colz
      } // rowx 
    } //end if fActive
    fActive[mod] = false;
  } // mod
}
