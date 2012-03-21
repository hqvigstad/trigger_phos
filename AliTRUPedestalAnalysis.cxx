/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
/* $Id$ */

#include "AliCaloRawStreamV3.h"
#include "AliTRUPedestalOutput.h"
#include "AliPHOSTriggerRawReader.h"
#include "AliPHOSTRURawReader.h"
#include "TH1I.h"

#include "AliTRUPedestalAnalysis.h"

AliTRUPedestalAnalysis::AliTRUPedestalAnalysis()
: fOutput(new AliTRUPedestalOutput),
  fTriggerReader(new AliPHOSTriggerRawReader)
{
}

AliTRUPedestalAnalysis::~AliTRUPedestalAnalysis()
{
  delete fOutput;
}

void AliTRUPedestalAnalysis::ProcessEvent ( AliCaloRawStreamV3* phosStream )
{
  fTriggerReader->Reset();

  phosStream->Reset();
  while (phosStream->NextDDL()) {
    while (phosStream->NextChannel()) {
      if (phosStream->IsTRUData()) {
	fTriggerReader->ReadFromStream(phosStream);
      }// IsTRUData
    }// NextChannel
  }//NextDDL

  for (unsigned int mod=0; mod<kNMods; ++mod) {
    for (unsigned int row=0; row<kNTRURows; ++row) {
      for (unsigned int branch=0; branch<kNBranches; ++branch) {
	if( fTriggerReader->GetTRU(mod, row, branch)->IsActive() )
	  for (unsigned int xrow=0; xrow<kN2x2XPrTRURow; ++xrow) {
	    for (unsigned int zcol=0; zcol<kN2x2ZPrBranch; ++zcol) {
	      double max = Get2x2Max(fTriggerReader, mod, row, branch, xrow, zcol);
	      fOutput->GetTRUSignals(mod, row, branch, xrow, zcol)->Fill(max);
	    } // zcol
	  } // xrow
      } // branch
    } // row
  } // mod
}


UInt_t AliTRUPedestalAnalysis::Get2x2Max ( AliPHOSTriggerRawReader* reader, int mod, int row, int branch, int x, int z )
{
  UInt_t max = 0;
  for(uint timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin) {
    const UInt_t signal = reader->GetTRU(mod, row, branch)->GetTriggerSignal(x, z, timeBin);
    if( max < signal )
      max = signal;
  }
  return max;
}
