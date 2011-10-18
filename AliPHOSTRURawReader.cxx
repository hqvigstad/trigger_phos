#include "AliPHOSTRURawReader.h"

#include "AliCaloRawStreamV3.h"
#include "AliPHOSGeometry.h"

#include <bitset>
#include <iostream>
#include <vector>
using namespace std;


AliPHOSTRURawReader::AliPHOSTRURawReader()
  : fSignals(),
    fFlags(),
    fActive(false),
    fActiveTime(kNTimeBins, false)
{
  // fSignals:
  vector<Short_t> sTime(kNTimeBins, 0);
  vector<vector<Short_t> > sZ(kN2x2ZPrBranch, sTime);
  fSignals = vector<vector<vector<Short_t> > >(kN2x2XPrTRURow, sZ);

  // fFlags()
  vector<Bool_t> bTime(kNTimeBins, 0);
  vector<vector<Bool_t> > bZ(kN4x4ZPrBranch, bTime);
  fFlags = vector<vector<vector<Bool_t> > >(kN4x4XPrTRURow, bZ);
}


AliPHOSTRURawReader::~AliPHOSTRURawReader()
{}

void AliPHOSTRURawReader::ReadFromStream(AliCaloRawStreamV3* rawStream)
{
  const UShort_t * const signal = rawStream->GetSignals(); // stream of 10-bit words, buffered as 16-bit words
  const Int_t signalLength = rawStream->GetBunchLength();
  const Int_t timeBin = rawStream->GetHWAddress() & 0x7f;

  fActive = true;
  fActiveTime[timeBin] = true;

  /* There are 91 4x4 Sliding Window signal sum trigger flags.
   * We read the trigger location information from first 12 10-bit words of the signal
   * (these are buffered to 12 16-bit words in the rawstream object)
   * , but only 10 of the 10-bit words are used for trigger location within the TRU.
   * The flags are found in word_2[0] to word_11[9].
   * We read the information as following. */

  for(int wIdx = 2; wIdx < 12; ++wIdx) { // words
    const bitset<10> word(signal[wIdx]);
    for(int bIdx = 0; bIdx < 10; ++bIdx) { // bits
      //const int index = 119 - (wIdx*10 + (9-bi)); // index used in TRU documentation,
      const int index = 110 + bIdx - wIdx*10; // equivalent
	if( index < 91 ) { // we are only interrested in these words/bits
	  const int xIdx = index % 7; // x index in TRU internal 2x2 coordinate system
	  const int zIdx = index / 7; // z index in TRU internal 2x2 coordinate system
	  // fFlags[xIdx][zIdx][time] = (signal[wIdx] & (0x01 * 2**i)) != 1;
	  fFlags[xIdx][zIdx][timeBin] = (bool) word[bIdx];
	}
    } // end bit loop
  }// end word loop



  /* The 2x2 trigger signal sum may follow.
   * If so, it is found in the following 112 10-bit words
   * (, 16-bit words in buffer.)
   * We read the signal as following. */

  // if( 16 < signalLength ) {
  if( 16+112 == signalLength) {
    for (Int_t idx = 0; idx < 112; idx++)
      {
	const int xIdx = 7 - idx % 8;  // x index in TRU
	const int zIdx = 13 - idx / 8; // z index in TRU
	const int wIdx = idx + 16;     // word index in signal array
	fSignals[xIdx][zIdx][timeBin] = signal[wIdx];
      }
  }
}


void AliPHOSTRURawReader::Reset()
{
  if( ! fActive )
    return;

  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    if( fActiveTime[timeBin] ) {
      for(int xIdx = 0; xIdx < kN2x2XPrTRURow; ++xIdx) {
	for(int zIdx = 0; zIdx < kN2x2ZPrBranch; ++zIdx) {
	  fSignals[xIdx][zIdx][timeBin] = 0;
	} // zIdx
      } // xIdx
      for(int xIdx = 0; xIdx < kN4x4XPrTRURow; ++xIdx) {
	for(int zIdx = 0; zIdx < kN4x4ZPrBranch; ++zIdx) {
	  fFlags[xIdx][zIdx][timeBin] = false;
	} // zIdx
      } // xIdx
    }// end if fActiveTime
    fActiveTime[timeBin] = false;
  } // timeBin

  fActive = false;
}
