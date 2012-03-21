#include "AliPHOSTriggerAnalysis.h"

#include "AliPHOSTriggerAnalysisHistograms.h"
#include "AliRawReaderChain.h"
#include "AliPHOSRawReader.h"
#include "AliPHOSEMCRawReader.h"
#include "AliPHOSTriggerRawReader.h"
#include "AliPHOSTRURawReader.h"
#include "AliPHOSTriggerParameters.h"

#include "AliPHOSGeometry.h"
#include "TH1I.h"
#include "TH2I.h"
#include "TCanvas.h"

#include <iostream>
using namespace std;

AliPHOSTriggerAnalysis::AliPHOSTriggerAnalysis()
  :fVerbose(0),
   fModules(kNMods, false),
   fSaturationThreshold(950),
   fParameters(0),
   fHistograms(new AliPHOSTriggerAnalysisHistograms)
{
  SetAnalyseModule(2);
  SetAnalyseModule(3);
  SetAnalyseModule(4);
}


AliPHOSTriggerAnalysis::~AliPHOSTriggerAnalysis()
{
  delete fHistograms;
}

void AliPHOSTriggerAnalysis::ProcessEvent(AliPHOSRawReader* rawReader)
{
  AliPHOSEMCRawReader* readerLG = rawReader->GetLGReader();
  AliPHOSEMCRawReader* readerHG = rawReader->GetHGReader();
  AliPHOSTriggerRawReader* readerT = rawReader->GetTriggerReader();

  // Loop over modules
  for(unsigned int mod = 0; mod < fModules.size(); ++mod) {
    if( fModules[mod] ) {

      // Loop over 2x2 cells
      for(int xIdx = 0; xIdx < kN2x2X; ++xIdx) {
	for(int zIdx = 0; zIdx < kN2x2Z; ++zIdx) {

	  // Get peak values
	  const int TSmax = Get2x2Max(readerT, fParameters, mod, xIdx, zIdx);
	  const int LGmax = Get2x2Max(readerLG, mod, xIdx, zIdx);
	  const int HGmax = Get2x2Max(readerHG, mod, xIdx, zIdx);
	  const bool HGSaturated = Is2x2Saturated(rawReader->GetHGReader(), mod, xIdx, zIdx, fSaturationThreshold);

	  // Plot Values
	  fHistograms->GetLGTSPeakCorrelation()->Fill(LGmax, TSmax);
	  fHistograms->GetHGTSPeakCorrelation()->Fill(HGmax, TSmax);
	  if( TSmax > 0)
	    fHistograms->GetHGTSPeakRatio()->Fill( TSmax, ((double)HGmax) / TSmax );
	  if( ! HGSaturated )
	    fHistograms->GetHGTSPeakCorrelationUS()->Fill(HGmax, TSmax);

	  // Plot Active Values
	  if( Is2x2Active(readerT, mod, xIdx, zIdx) ){
	    if( Is2x2Active(readerLG, mod, xIdx, zIdx ) )
	      fHistograms->GetLGTSPeakCorrelationA()->Fill(LGmax, TSmax);
	    if( Is2x2Active(readerHG, mod, xIdx, zIdx ) )
	      fHistograms->GetHGTSPeakCorrelationA()->Fill(HGmax, TSmax);

	    // LG / HG, signal - time
	    for(int timeBin = 0; timeBin < kNDefaultNEMCTimeBins; ++timeBin) {
	      fHistograms->GetLGSignalTime()->Fill( timeBin, Get2x2Signal(readerLG, mod, xIdx, zIdx, timeBin) );
	      fHistograms->GetHGSignalTime()->Fill( timeBin, Get2x2Signal(readerHG, mod, xIdx, zIdx, timeBin) );
	    }

	    // TRU, signal - time
	    for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin)
	      if( Is2x2Active(readerT, mod, xIdx, zIdx, timeBin) ){
		fHistograms->GetTRUSignalTime()->Fill( timeBin, Get2x2Signal(readerT, fParameters, mod, xIdx, zIdx, timeBin) );
	      }
	  }

	} // end zIdx loop
      } // end xIdx loop
      for(int TRURow = 0; TRURow < kNTRURows; ++TRURow) {
	for(int branch = 0; branch < kNBranches; ++branch) {
	  AliPHOSTRURawReader* truReader = readerT->GetTRU(mod, TRURow, branch);
	  if( truReader->IsActive() )
	    for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin) {
	      if( truReader->IsActive(timeBin) )
		for(int xIdx = 0; xIdx < kN2x2XPrTRURow; ++xIdx) {
		  for(int zIdx = 0; zIdx < kN2x2ZPrBranch; ++zIdx) {
		    fHistograms->GetTRUSignal(mod, TRURow, branch, xIdx, zIdx)
		      ->Fill( truReader->GetTriggerSignal(xIdx, zIdx, timeBin) );
		  } // z
		} // x
	    } // timeBin
	} // branch
      } // tru



      // Loop over 4x4 cells
      for(int TRURow = 0; TRURow < kNTRURows; ++TRURow) {
	for(int branch = 0; branch < kNBranches; ++branch) {
	  if( readerT->GetTRU(mod, TRURow, branch)->IsActive() )
	    fHistograms->GetTRUActive()->Fill( mod*kNTRURows*kNBranches + TRURow*kNBranches + branch );
	  for(int xIdx = 0; xIdx < kN4x4XPrTRURow; ++xIdx) {
	    for(int zIdx = 0; zIdx < kN4x4ZPrBranch; ++zIdx) {

	      // Determin if Trigger is flagged for any timeBin
	      bool triggered = false;
	      for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin){
		if( rawReader->GetTriggerReader()->GetTRU(mod, TRURow, branch)->GetTriggerFlag(xIdx, zIdx, timeBin) ){
		  triggered = true;
		  fHistograms->GetTriggerTime()->Fill(timeBin);
		} // end "if TriggerBit"
	      }// end TimeBin loop

	      if( triggered ){
		// Get peak values
		const int TSmax = Get4x4Max(rawReader->GetTriggerReader(), fParameters, mod, TRURow, branch, xIdx, zIdx);
		const int LGmax = Get4x4Max(rawReader->GetLGReader(), mod, TRURow, branch, xIdx, zIdx);
		const int HGmax = Get4x4Max(rawReader->GetHGReader(), mod, TRURow, branch, xIdx, zIdx);
		const bool HGSaturated = Is4x4Saturated(rawReader->GetHGReader(), mod, TRURow, branch, xIdx, zIdx, fSaturationThreshold );

		// Plot values
		fHistograms->GetTriggeredSWLGTSPeakCorrelation()->Fill(LGmax, TSmax);
		fHistograms->GetTriggeredSWHGTSPeakCorrelation()->Fill(HGmax, TSmax);
		if( TSmax >0 )
		  fHistograms->GetTriggeredSWHGTSPeakRatio()->Fill( TSmax, ((double)HGmax) / TSmax );
		if( ! HGSaturated )
		  fHistograms->GetTriggeredSWHGTSPeakCorrelationUS()->Fill(HGmax, TSmax);
	      }// end  "if triggered"
	    } // end zIdx loop
	  } // end xIdx loop
	} // end branch loop
      } // end tru loop
    } // end "if module"
  } // end mod loop
}



int AliPHOSTriggerAnalysis::Get2x2Max(AliPHOSEMCRawReader* reader, int mod, int xIdx, int zIdx)
{
  if( reader->IsActive(mod, xIdx, zIdx )  ) {
    int max = 0;
    // find max:
    for(int timeBin = 0; timeBin < kNDefaultNEMCTimeBins; ++timeBin) {
      const int signal = Get2x2Signal(reader, mod, xIdx, zIdx, timeBin);
      if( max < signal ){
	max = signal;
      }
    }
    return max;
  }
  else
    return 0;
}


int AliPHOSTriggerAnalysis::Get2x2Max(AliPHOSTriggerRawReader* reader, AliPHOSTriggerParameters* params, int mod, int xIdx, int zIdx)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin) {
    const int signal = Get2x2Signal(reader, params, mod, xIdx, zIdx, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get2x2Signal(AliPHOSEMCRawReader* reader, int mod, int xIdx, int zIdx, int timeBin)
{
  const Short_t signal
    = reader->GetSignal(mod, xIdx*2  , zIdx*2  , timeBin)
    + reader->GetSignal(mod, xIdx*2+1, zIdx*2  , timeBin)
    + reader->GetSignal(mod, xIdx*2  , zIdx*2+1, timeBin)
    + reader->GetSignal(mod, xIdx*2+1, zIdx*2+1, timeBin);
  return signal;
}


int AliPHOSTriggerAnalysis::Get2x2Signal(AliPHOSTriggerRawReader* reader, AliPHOSTriggerParameters* parameters, int mod, int xIdx, int zIdx, int timeBin)
{
  const int TRURow = xIdx / kN2x2XPrTRURow;
  const int branch = zIdx / kN2x2ZPrBranch;
  const int TRUX = xIdx % kN2x2XPrTRURow; // 2x2 coordinates
  const int TRUZ = zIdx % kN2x2ZPrBranch; // 2x2 coordinates

  if( reader->GetTRU(mod, TRURow, branch)->IsActive() ){
    const int signal = reader->GetTRU(mod, TRURow, branch)->GetTriggerSignal( TRUX, TRUZ, timeBin);
    if( parameters )
      return signal - parameters->GetTRUPedestal(mod, TRURow, branch, TRUX, TRUZ);
    else
      return signal - AliPHOSTRURawReader::kDefaultSignalValue;
  }
  else
    return 0;
}




int AliPHOSTriggerAnalysis::Get4x4Max(AliPHOSEMCRawReader* reader, int mod, int TRURow, int branch, int xIdx, int zIdx)
{
  const int modX = xIdx + TRURow * kN2x2XPrTRURow;
  const int modZ = zIdx + branch * kN2x2ZPrBranch;

  int max = 0;
  for(int timeBin = 0; timeBin < kNDefaultNEMCTimeBins; ++timeBin) {
    const int signal
      = Get2x2Signal(reader, mod, modX  , modZ  , timeBin)
      + Get2x2Signal(reader, mod, modX+1, modZ  , timeBin)
      + Get2x2Signal(reader, mod, modX  , modZ+1, timeBin)
      + Get2x2Signal(reader, mod, modX+1, modZ+1, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get4x4Max(AliPHOSTriggerRawReader* reader, AliPHOSTriggerParameters* params, int mod, int TRURow, int branch, int xIdx, int zIdx)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTRUTimeBins; ++timeBin) {
    const int signal = Get4x4Signal(reader, params, mod, TRURow, branch, xIdx, zIdx, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get4x4Signal(AliPHOSTriggerRawReader* reader, AliPHOSTriggerParameters* params, int mod, int TRURow, int branch, int xIdx, int zIdx, int timeBin)
{
  const int modX = xIdx + TRURow * kN2x2XPrTRURow;
  const int modZ = zIdx + branch * kN2x2ZPrBranch;

  const int signal
    = Get2x2Signal(reader, params, mod, modX  , modZ  , timeBin)
      + Get2x2Signal(reader, params, mod, modX+1, modZ  , timeBin)
      + Get2x2Signal(reader, params, mod, modX  , modZ+1, timeBin)
      + Get2x2Signal(reader, params, mod, modX+1, modZ+1, timeBin);
  return signal;
}


bool AliPHOSTriggerAnalysis::Is2x2Active(AliPHOSEMCRawReader* reader, int mod, int xIdx, int zIdx)
{
  return reader->IsActive(mod, xIdx*2  , zIdx*2  )
    ||   reader->IsActive(mod, xIdx*2+1, zIdx*2  )
    ||   reader->IsActive(mod, xIdx*2  , zIdx*2+1)
    ||   reader->IsActive(mod, xIdx*2+1, zIdx*2+1);
}


bool AliPHOSTriggerAnalysis::Is2x2Active(AliPHOSTriggerRawReader* reader, int mod, int xIdx, int zIdx)
{
  const int TRURow = xIdx / kN2x2XPrTRURow;
  const int branch = zIdx / kN2x2ZPrBranch;

  return reader->GetTRU(mod, TRURow, branch)->IsActive();
}

bool AliPHOSTriggerAnalysis::Is2x2Active(AliPHOSTriggerRawReader* reader, int mod, int xIdx, int zIdx, int timeBin)
{
  const int TRURow = xIdx / kN2x2XPrTRURow;
  const int branch = zIdx / kN2x2ZPrBranch;

  return reader->GetTRU(mod, TRURow, branch)->IsActive(timeBin);
}


bool AliPHOSTriggerAnalysis::Is4x4Active(AliPHOSEMCRawReader* reader, int mod, int TRURow, int branch, int xIdx, int zIdx)
{
  const int modX = xIdx + TRURow * kN2x2XPrTRURow;
  const int modZ = zIdx + branch * kN2x2ZPrBranch;

  return reader->IsActive(mod, modX  , modZ  )
    &&   reader->IsActive(mod, modX+1, modZ  )
    &&   reader->IsActive(mod, modX  , modZ+1)
    &&   reader->IsActive(mod, modX+1, modZ+1);
}


bool AliPHOSTriggerAnalysis::Is2x2Saturated(AliPHOSEMCRawReader* reader, int mod, int xIdx, int zIdx, int satThreshold)
{
  for(int timeBin = 0; timeBin < kNDefaultNEMCTimeBins; ++timeBin) {
    // if any of the 4 signal above threshold:
    if( satThreshold <= reader->GetSignal(mod, xIdx*2  , zIdx*2  , timeBin) ||
	satThreshold <= reader->GetSignal(mod, xIdx*2+1, zIdx*2  , timeBin) ||
	satThreshold <= reader->GetSignal(mod, xIdx*2  , zIdx*2+1, timeBin) ||
	satThreshold <= reader->GetSignal(mod, xIdx*2+1, zIdx*2+1, timeBin)    )
      return true; // signal is saturated
  }
  // else signal is not saturated
  return false;
}


bool AliPHOSTriggerAnalysis::Is4x4Saturated(AliPHOSEMCRawReader* reader, int mod, int TRURow, int branch, int xIdx, int zIdx, int satThreshold)
{
  const int modX = xIdx + TRURow * kN2x2XPrTRURow;
  const int modZ = zIdx + branch * kN2x2ZPrBranch;

  return Is2x2Saturated(reader, mod, modX  , modZ  , satThreshold)
    ||   Is2x2Saturated(reader, mod, modX+1, modZ  , satThreshold)
    ||   Is2x2Saturated(reader, mod, modX  , modZ+1, satThreshold)
    ||   Is2x2Saturated(reader, mod, modX+1, modZ+1, satThreshold);
}


void AliPHOSTriggerAnalysis::SaveResults(TString filename) const
{
  new TCanvas;
  fHistograms->GetHGTSPeakCorrelation()->DrawCopy("colz");
  new TCanvas;
  fHistograms->GetHGTSPeakCorrelationA()->DrawCopy("colz");

  fHistograms->SaveResults(filename);
}


