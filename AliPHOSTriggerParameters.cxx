#include "AliPHOSTriggerParameters.h"
#include "TString.h"

ClassImp(AliPHOSTriggerParameters)


AliPHOSTriggerParameters::AliPHOSTriggerParameters()
  : TNamed()
{
  // default constructor. Initialises with Reset().
  Reset();
}


AliPHOSTriggerParameters::AliPHOSTriggerParameters(const char* name)
  : TNamed(name, name)
{
  // Specific constructor, passes name to TNamed base. Initialises with Reset().

  Reset();
}


AliPHOSTriggerParameters::AliPHOSTriggerParameters(const AliPHOSTriggerParameters& other)
  : TNamed(other)
{
  // copy constructor
  
  for(int mod = 0; mod < kNMods; ++mod) {
    for(int TRURow = 0; TRURow < kNTRURows; TRURow++) {
      for(int branch = 0; branch < kNBranches; branch++) {
	for(int xIdx = 0; xIdx < kNTRUX; xIdx++) {
	  for(int zIdx = 0; zIdx < kNTRUZ; zIdx++) {
	    fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = other.fTRUPedestals[mod][TRURow][branch][xIdx][zIdx];
	  } // end zIdx
	} // end xIdx
	fTRUTriggerBitReadoutOn[mod][TRURow][branch] = other.fTRUTriggerBitReadoutOn[mod][TRURow][branch];
	fTRUSignalReadoutOn[mod][TRURow][branch] = other.fTRUSignalReadoutOn[mod][TRURow][branch];
	fTRUSignalTimeBinFrom[mod][TRURow][branch] = other.fTRUSignalTimeBinFrom[mod][TRURow][branch];
	fTRUSignalTimeBinTo[mod][TRURow][branch] = other.fTRUSignalTimeBinTo[mod][TRURow][branch];
	fTRUThreshold[mod][TRURow][branch] = other.fTRUThreshold[mod][TRURow][branch];
	fTRUMaskChannel[mod][TRURow][branch] = other.fTRUMaskChannel[mod][TRURow][branch];
      } // end branch 
    } // end tru
    // for(int tor = 0; tor < kNTORs; tor++) {
    //   fTORMaskArray[mod][tor] = other.fTORMaskArray[mod][tor] ;
    //   fTORReadoutMask[mod][tor] = other.fTORReadoutMask[mod][tor];
    // }
  } // end mod
}


AliPHOSTriggerParameters& AliPHOSTriggerParameters::operator= (const AliPHOSTriggerParameters& other)
{
  // assignement
  
  SetName(other.GetName());
  SetTitle(other.GetTitle());

  for(int mod = 0; mod < kNMods; ++mod) {
    for(int TRURow = 0; TRURow < kNTRURows; TRURow++) {
      for(int branch = 0; branch < kNBranches; branch++) {
	for(int xIdx = 0; xIdx < kNTRUX; xIdx++) {
	  for(int zIdx = 0; zIdx < kNTRUZ; zIdx++) {
	    fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = other.fTRUPedestals[mod][TRURow][branch][xIdx][zIdx];
	  }
	}
	fTRUTriggerBitReadoutOn[mod][TRURow][branch] = other.fTRUTriggerBitReadoutOn[mod][TRURow][branch];
	fTRUSignalReadoutOn[mod][TRURow][branch] = other.fTRUSignalReadoutOn[mod][TRURow][branch];
	fTRUSignalTimeBinFrom[mod][TRURow][branch] = other.fTRUSignalTimeBinFrom[mod][TRURow][branch];
	fTRUSignalTimeBinTo[mod][TRURow][branch] = other.fTRUSignalTimeBinTo[mod][TRURow][branch];
	fTRUThreshold[mod][TRURow][branch] = other.fTRUThreshold[mod][TRURow][branch];
	fTRUMaskChannel[mod][TRURow][branch] = other.fTRUMaskChannel[mod][TRURow][branch];
      }
    }
    // for(int tor = 0; tor < kNTORs; tor++) {
    //   fTORMaskArray[mod][tor] = other.fTORMaskArray[mod][tor] ;
    //   fTORReadoutMask[mod][tor] = other.fTORReadoutMask[mod][tor];
    // }
  }
  return *this;
}


AliPHOSTriggerParameters::~AliPHOSTriggerParameters()
{
  // destructor
}



UShort_t AliPHOSTriggerParameters::GetTRUPedestal(int mod, int TRURow, int branch, int xIdx, int zIdx) const
{
  // Returns the pedestals of the TRU FakeALTRO Trigger signal, is in the range
  // spanned by 10 bits, i.e. [0,1023]. Ideal value is is given by.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  // xIdx: [0,7], TRU 2x2 cell x index, indexed in rising number in PHOS x direction
  // zIdx: [0,13], TRU 2x2 cell z index, indexed in rising number in PHOS z direction
  
  return fTRUPedestals[mod][TRURow][branch][xIdx][zIdx];
}
Bool_t AliPHOSTriggerParameters::GetTRUReadoutOn(int mod, int TRURow, int branch) const
{
  // Returns the TRU specific flag that specifies for the TRU whether 
  // or not it should be read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUTriggerBitReadoutOn[mod][TRURow][branch];
}
Bool_t AliPHOSTriggerParameters::GetTRUSignalReadoutOn(int mod, int TRURow, int branch) const
{
  // Returns the TRU specific flag that specifies for the TRU whether 
  // or not it readout should include full  FakeALTRO Trigger Signal.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUSignalReadoutOn[mod][TRURow][branch];
}
UShort_t AliPHOSTriggerParameters::GetTRUSignalTimeBinFrom(int mod, int TRURow, int branch) const
{
  // Returns the TRU specific value that specifies from (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUSignalTimeBinFrom[mod][TRURow][branch];
}
UShort_t AliPHOSTriggerParameters::GetTRUSignalTimeBinTo(int mod, int TRURow, int branch) const
{
  // Returns the TRU specific value that specifies to (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUSignalTimeBinTo[mod][TRURow][branch];
}
UShort_t AliPHOSTriggerParameters::GetTRUThreshold(int mod, int TRURow, int branch) const
{
  // Returns the TRU specific value that specifies the trigger threshold, 
  // the 4x4 Sliding Window (signal - pedestal) value to which the tru 
  // sends a L0.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  
  return fTRUThreshold[mod][TRURow][branch];
}
UShort_t AliPHOSTriggerParameters::GetTRUMaskChannel(int mod, int TRURow, int branch) const
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  return fTRUMaskChannel[mod][TRURow][branch];
}
// MaskArrayType GetTORMaskArray(int mod, int tor) const
// {
//   return fTORMaskArray[mod][TRURow][branch];
// }
// ReadoutMaskType GetTORReadoutMask(int mod, int tor) const
// {
//   return fTORReadoutMask[mod][TRURow][branch];
// }



void AliPHOSTriggerParameters::SetTRUPedestal(UShort_t pedestal, int mod, int TRURow, int branch, int xIdx, int zIdx)
{
  // Returns the pedestals of the TRU FakeALTRO Trigger signal, 
  // is in the range spanned by 10 bits
  // , i.e. [0,1023]. Ideal value is is given by.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  // xIdx: [0,7], TRU 2x2 cell x index, indexed in rising number in PHOS x direction
  // zIdx: [0,7], TRU 2x2 cell z index, indexed in rising number in PHOS z direction
  
  fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = pedestal;
}
void AliPHOSTriggerParameters::SetTRUReadoutOn(Bool_t isOn, int mod, int TRURow, int branch)
{
  // Returns the TRU specific flag that specifies for the TRU 
  // whether or not it should be read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUTriggerBitReadoutOn[mod][TRURow][branch] = isOn;}
void AliPHOSTriggerParameters::SetTRUSignalReadoutOn(Bool_t isOn, int mod, int TRURow, int branch)
{
  // Returns the TRU specific flag that specifies for the TRU whether or not it
  // readout should include full FakeALTRO Trigger Signal.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

   fTRUSignalReadoutOn[mod][TRURow][branch] = isOn;
}
void AliPHOSTriggerParameters::SetTRUSignalTimeBinFrom(UShort_t fromBin, int mod, int TRURow, int branch)
{
  // Returns the TRU specific value that specifies from (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUSignalTimeBinFrom[mod][TRURow][branch] = fromBin;
}
void AliPHOSTriggerParameters::SetTRUSignalTimeBinTo(UShort_t toBin, int mod, int TRURow, int branch)
{
  // Returns the TRU specific value that specifies to (including), 
  // which timebin TRU should read out.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUSignalTimeBinTo[mod][TRURow][branch] = toBin;
}
void AliPHOSTriggerParameters::SetTRUThreshold(UShort_t threshold, int mod, int TRURow, int branch)
{
  // Returns the TRU specific value that specifies the trigger threshold, 
  // the (signal - pedestal) value to which the tru sends a L0.
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction
  
  fTRUThreshold[mod][TRURow][branch] = threshold;
}
void AliPHOSTriggerParameters::SetTRUMaskChannel(UShort_t mask, int mod, int TRURow, int branch)
{
  // TODO: description
  //
  // mod: [0,4], module index, clockwise, as of 2011 only 2:4 exists
  // TRURow: [0,3], TRU Row, indexed in rising number in PHOS x direction
  // branch: [0,1], branch, indexed in rising number in z direction

  fTRUMaskChannel[mod][TRURow][branch] = mask;
}
// void AliPHOSTriggerParameters::SetTORMaskArray(MaskArrayType ma, int mod, int tor)
// {
//   fTORMaskArray[mod][tor] = ma;
// }
// void AliPHOSTriggerParameters::SetTORReadoutMask(ReadoutMaskType rm, int mod, int tor)
// {
//   fTORReadoutMask[mod][tor] = rm;
// }



void AliPHOSTriggerParameters::Print(Option_t *option) const
{
  // Will Write uppon request (Henrik Qvigstad <henrik.qvigstad@cern.ch>)
  if(option)
    Printf("AliPHOSTriggerParameters::Print is not implemented.");
}


void AliPHOSTriggerParameters::Reset()
{
  // Resets the values to Null/Ideal

    for(int mod = 0; mod < kNMods; ++mod) {
    for(int TRURow = 0; TRURow < kNTRURows; TRURow++) {
      for(int branch = 0; branch < kNBranches; branch++) {
	fTRUTriggerBitReadoutOn[mod][TRURow][branch] = 0;
	fTRUSignalReadoutOn[mod][TRURow][branch] = 0;
	fTRUSignalTimeBinFrom[mod][TRURow][branch] = 0;
	fTRUSignalTimeBinTo[mod][TRURow][branch] = kDefaultNTRUTimeBins-1;
	fTRUThreshold[mod][TRURow][branch] = 0;
	fTRUMaskChannel[mod][TRURow][branch] = 0;
	for(int xIdx = 0; xIdx < kNTRUX; xIdx++) {
	  for(int zIdx = 0; zIdx < kNTRUZ; zIdx++) {
	    fTRUPedestals[mod][TRURow][branch][xIdx][zIdx] = kIdealTRUPedestal;
	  }
	}
      }
    }
    // for(int tor = 0; tor < kNTORs; tor++) {
    //   fTORMaskArray[mod][tor] = 0;
    //   fTORReadoutMask[mod][tor] = 0;
    // }
  }
}

