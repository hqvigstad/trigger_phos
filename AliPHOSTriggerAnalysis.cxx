#include "AliPHOSTriggerAnalysis.h"

#include "AliPHOSTriggerAnalysisHistograms.h"
#include "AliRawReaderChain.h"
#include "AliPHOSRawReader.h"
#include "AliPHOSEMCRawReader.h"
#include "AliPHOSTRURawReader.h"
#include "AliPHOSTRURegionRawReader.h"
#include "AliPHOSTriggerParameters.h"

#include "AliPHOSGeometry.h"
#include "TH1I.h"
#include "TH2I.h"

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
  // Loop over modules
  for(unsigned int mod = 0; mod < fModules.size(); ++mod) {
    if( fModules[mod] ) {
      
      // Loop over 2x2 cells
      for(int xIdx = 0; xIdx < kN2x2X; ++xIdx) {
	for(int zIdx = 0; zIdx < kN2x2Z; ++zIdx) {
	  // Get peak values
	  const int TSmax = Get2x2Max(rawReader->GetTRUReader(), fParameters, mod, xIdx, zIdx);
	  const int LGmax = Get2x2Max(rawReader->GetLGReader(), mod, xIdx, zIdx);
	  const int HGmax = Get2x2Max(rawReader->GetHGReader(), mod, xIdx, zIdx);
	  const bool HGSaturated = Is2x2Saturated(rawReader->GetHGReader(), mod, xIdx, zIdx, fSaturationThreshold);
	  
	  // Plot Values
	  fHistograms->GetLGTSPeakCorrelation()->Fill(LGmax, TSmax);
	  fHistograms->GetHGTSPeakCorrelation()->Fill(HGmax, TSmax);
	  if( TSmax > 0)
	    fHistograms->GetHGTSPeakRatio()->Fill( TSmax, ((double)HGmax) / TSmax );
	  if( ! HGSaturated )
	    fHistograms->GetHGTSPeakCorrelationUS()->Fill(HGmax, TSmax);
	} // end zIdx loop
      } // end xIdx loop

      // Loop over 4x4 cells
      for(int TRURow = 0; TRURow < kNTRURows; ++TRURow) {
	for(int branch = 0; branch < kNBranches; ++branch) {
	  for(int xIdx=0; xIdx < n4x4XPrTRURow; ++xIdx) {
	    for(int zIdx=0; zIdx < n4x4ZPrBranch; ++zIdx) {

	      // Determin if Trigger is flagged for any timeBin
	      bool triggered = false;
	      for(int timeBin; timeBin < kNTimeBins; ++timeBin){
		if( rawReader->GetTRUReader()->GetTRURegion(mod, TRURow, branch)->GetTriggerFlag(xIdx, zIdx, timeBin) ){
		  triggered = true;
		  fHistograms->GetTriggerTime()->Fill(timeBin);
		} // end "if TriggerBit"
	      }// end TimeBin loop

	      if( triggered ){
		// Get peak values
		const int TSmax = Get4x4Max(rawReader->GetTRUReader(), fParameters, mod, TRURow, branch, xIdx, zIdx);
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


int AliPHOSTriggerAnalysis::Get2x2Signal(AliPHOSEMCRawReader* reader, int mod, int xIdx, int zIdx, int timeBin)
{
  const Short_t signal
    = reader->GetSignal(mod, xIdx*2  , zIdx*2  , timeBin)
    + reader->GetSignal(mod, xIdx*2+1, zIdx*2  , timeBin)
    + reader->GetSignal(mod, xIdx*2  , zIdx*2+1, timeBin)
    + reader->GetSignal(mod, xIdx*2+1, zIdx*2+1, timeBin);
  return signal;
}


int AliPHOSTriggerAnalysis::Get2x2Signal(AliPHOSTRURawReader* reader, AliPHOSTriggerParameters* parameters, int mod, int xIdx, int zIdx, int timeBin)
{
  const int RCURow = xIdx / kN2x2XPrTRURow;
  const int branch = zIdx / kN2x2ZPrBranch;
  const int RCUX = xIdx % kN2x2XPrTRURow; // 2x2 coordinates
  const int RCUZ = zIdx % kN2x2ZPrBranch; // 2x2 coordinates

  const Short_t signal = reader->GetTRURegion(mod, RCURow, branch)->GetTriggerSignal( RCUX, RCUZ, timeBin);
  if(parameters)
    return signal - parameters->GetTRUPedestal(mod, RCURow, branch, RCUX, RCUZ);
  else
    return signal;
}


int AliPHOSTriggerAnalysis::Get2x2Max(AliPHOSEMCRawReader* reader, int mod, int xIdx, int zIdx)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    const int signal = Get2x2Signal(reader, mod, xIdx, zIdx, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get2x2Max(AliPHOSTRURawReader* reader, AliPHOSTriggerParameters* params, int mod, int xIdx, int zIdx)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    const int signal = Get2x2Signal(reader, params, mod, xIdx, zIdx, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get4x4Max(AliPHOSTRURawReader* reader, AliPHOSTriggerParameters* params, int mod, int TRURow, int branch, int xIdx, int zIdx)
{
  int modX = xIdx + TRURow * (AliPHOSGeometry::GetInstance()->GetNPhi() / kNTRURows) /2;
  int modZ = zIdx + branch * (AliPHOSGeometry::GetInstance()->GetNZ() / kNBranches) /2;

  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    const int signal
      = Get2x2Signal(reader, params, mod, modX  , modZ  , timeBin)
      + Get2x2Signal(reader, params, mod, modX+1, modZ  , timeBin)
      + Get2x2Signal(reader, params, mod, modX  , modZ+1, timeBin)
      + Get2x2Signal(reader, params, mod, modX+1, modZ+1, timeBin);
    if( max < signal ){
      max = signal;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get4x4Max(AliPHOSEMCRawReader* reader, int mod, int TRURow, int branch, int xIdx, int zIdx)
{
  int modX = xIdx + TRURow * (AliPHOSGeometry::GetInstance()->GetNPhi() / kNTRURows) /2;
  int modZ = zIdx + branch * (AliPHOSGeometry::GetInstance()->GetNZ() / kNBranches) /2;
  
  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
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


bool AliPHOSTriggerAnalysis::Is2x2Saturated(AliPHOSEMCRawReader* reader, int mod, int xIdx, int zIdx, int satThreshold)
{
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
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
  int modX = xIdx + TRURow * (AliPHOSGeometry::GetInstance()->GetNPhi() / kNTRURows) /2;
  int modZ = zIdx + branch * (AliPHOSGeometry::GetInstance()->GetNZ() / kNBranches) /2;
  return Is2x2Saturated(reader, mod, modX  , modZ  , satThreshold)
    ||   Is2x2Saturated(reader, mod, modX+1, modZ  , satThreshold)
    ||   Is2x2Saturated(reader, mod, modX  , modZ+1, satThreshold)
    ||   Is2x2Saturated(reader, mod, modX+1, modZ+1, satThreshold);
}


void AliPHOSTriggerAnalysis::SaveResults(TString filename) const
{
  fHistograms->SaveResults(filename);
}


