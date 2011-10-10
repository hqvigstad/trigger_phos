#include "AliPHOSTriggerAnalysisHistograms.h"

#include "AliRawReaderChain.h"
#include "AliPHOSTriggerAnalysisHistograms.h"
#include "AliPHOSRawReader.h"


AliPHOSTriggerAnalysis()
  :fVerbose(0),
   fModules(AliPHOSGeometry::GetInstance()->GetNModules(), false),
   fHistograms(new AliPHOSTriggerAnalysisHistograms),
   fSaturationThreshold(950);
{
  fModules[2] = true;
  fModules[3] = true;
  fModules[4] = true;
}


~AliPHOSTriggerAnalysis()
{
  delete fHistograms;
}

void AliPHOSTriggerAnalysis::ProcessEvent(AliPHOSRawReader* rawReader)
{
  const int n2x2X = AliPHOSGeometry::GetInstance()->GetNPhi() /2;
  const int n2x2Z = AliPHOSGeometry::GetInstance()->GetNZ() /2;
  const int n4x4XprTRU = (n2x2X /kNTRURows) -1;
  const int n4x4YprBranch = (n2x2X /kNBranches) -1;

  // Loop over modules
  for(unsigned int mod = 0; mod < fModules.size(); ++mod) {
    if( fModules[mod] ) {

      // Loop over 2x2 cells
      for(int xIdx = 0; xIdx < n2x2_X; ++xIdx) {
	for(int zIdx = 0; z_2x2 < n2x2_Z; ++xIdx) {
	  // Get peak values
	  const int TSmax = Get2x2Max(rawReader->GetTRUReader(), mod, xIdx, zIdx);
	  const int LGmax = Get2x2Max(rawReader->GetLGReader(), mod, xIdx, zIdx);
	  const int HGmax = Get2x2Max(rawReader->GetHGReader(), mod, xIdx, zIdx);
	  const bool HGSaturated = Is2x2Saturated(rawReader->GetHGReader(), mod, xIdx, zIdx, fSaturationThreshold)

	  // Plot Values
	  fHistograms->GetLGTSPeakCorrelation()->Fill(LG, TS);
	  fHistograms->GetHGTSPeakCorrelation()->Fill(HG, TS);
	  fHistograms->GetHGTSPeakRatio()->Fill( TS, ((double)HG) / TS );
	  if( ! HGSaturated )
	    fHistograms->GetHGTSPeakCorrelationUS()->Fill(HG, TS);
	} // end zIdx loop
      } // end xIdx loop

      // Loop over 4x4 cells
      for(int TRURow = 0; TRURow < kNTRUROWRows; ++TRURow){
	for(int branch = 0; branch < kNBranches; ++branch){
	  for(int xIdx=0; xIdx < n4x4XprTRU; ++xIdx){
	    for(int zIdx=0; xIdx < n4x4ZprBranch; ++xIdx){

	      // Determin if Trigger is flagged for any timeBin
	      bool triggered = false;
	      for(int timeBin; timeBin < kNTimeBins; ++timeBin){
		if( rawReader->GetTRUReader()->GetTriggerBit(mod, TRURow, branch, xIdx, zIdx, timeBin) ){
		  triggered = true;
		  fHistograms->GetTriggerTime()->Fill(timeBin);
		} // end "if TriggerBit"
	      }// end TimeBin loop

	      if( triggered ){
		// Get peak values
		const int TSmax = Get4x4Max(rawReader->GetTRUReader(), mod, TRURow, branch, xIdx, zIdx);
		const int LGmax = Get4x4Max(rawReader->GetLGReader(), mod, TRURow, branch, xIdx, zIdx);
		const int HGmax = Get4x4Max(rawReader->GetHGReader(), mod, TRURow, branch, xIdx, zIdx);
		const bool HGSaturated = Is4x4Saturated(rawReader->GetHGReader(), mod, TRURow, branch, xIdx, zIdx);
		  
		// Plot values
		fHistograms->GetTriggeredSWLGTSPeakCorrelation()->Fill(LG, TS);
		fHistograms->GetTriggeredSWHGTSPeakCorrelation()->Fill(HG, TS);
		fHistograms->GetTriggeredSWHGTSPeakRatio()->Fill( TS, ((double)HG) / TS );
		if( ! HGSaturated )
		  fHistograms->GetTriggeredSWHGTSPeakCorrelationUS()->Fill(HG, TS);
	      }// end  "if triggered"
	    } // end zIdx loop
	  } // end xIdx loop
	} // end branch loop
      } // end tru loop

    } // end "if module"
  } // end mod loop
}


int AliPHOSTriggerAnalysis::Get2x2Signal(AliPHOSModuleReader* reader, int mod, int xIdx, int zIdx);
{
  const int signal
    = reader->GetSignal(mod, xIdx*2  , zIdx*2  , timeBin)
    + reader->GetSignal(mod, xIdx*2+1, zIdx*2  , timeBin)
    + reader->GetSignal(mod, xIdx*2  , zIdx*2+1, timeBin)
    + reader->GetSignal(mod, xIdx*2+1, zIdx*2+1, timeBin);
  return signal;
}


int AliPHOSTriggerAnalysis::Get2x2Max(AliPHOSEMCReader* reader, int mod, int xIdx, int zIdx, int& maxIsAtTime)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    const int signal = Get2x2Signal(reader, mod, xIdx, zIdx);
    if( max < signal ){
      max = signal;
      maxIsAtTime = timeBin;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get2x2Max(AliPHOSTRUReader* reader, int mod, int xIdx, int zIdx, int& maxIsAtTime)
{
  const int RCURow = xIdx / kNTRURows;
  const int branch = zIdx / kNBranches;
  const int RCUX = xIdx % kNTRURows; // 2x2 coordinates
  const int RCUZ = zIdx % kNBranches; // 2x2 coordinates
  
  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    const int signal = reader->GetTRURegion(mod, RCURow, branch)->Get2x2SignalSum( RCUX, RCUZ, timeBin);
    if( max < signal ){
      max = signal;
      maxIsAtTime = timeBin;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get4x4Max(AliPHOSTRUReader* reader, int mod, int TRURow, int branch, int xIdx, int zIdx, int& maxIsAtTime)
{
  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    const int signal 
      = reader->GetTRURegion(mod, RCURow, branch)->Get2x2SignalSum(xIdx  , zIdx  , timeBin)
      + reader->GetTRURegion(mod, RCURow, branch)->Get2x2SignalSum(xIdx+1, zIdx  , timeBin)
      + reader->GetTRURegion(mod, RCURow, branch)->Get2x2SignalSum(xIdx  , zIdx+1, timeBin)
      + reader->GetTRURegion(mod, RCURow, branch)->Get2x2SignalSum(xIdx+1, zIdx+1, timeBin);
    if( max < signal ){
      max = signal;
      maxIsAtTime = timeBin;
    }
  }
  return max;
}


int AliPHOSTriggerAnalysis::Get4x4Max(AliPHOSEMCReader* reader, int mod, int TRURow, int branch, int xIdx, int zIdx, int& maxIsAtTime)
{
  int modX = xIdx + TRURow * (AliPHOSGeometry::GetInstance()->GetNPhi() / kNTRURows);
  int modZ = zIdx + branch * (AliPHOSGeometry::GetInstance()->GetNZ() / kNBranches);

  int max = 0;
  for(int timeBin = 0; timeBin < kNTimeBins; ++timeBin) {
    const int signal 
      = Get2x2Signal(reader, branch, modX  , modZ  , timeBin)
      + Get2x2Signal(reader, branch, modX+1, modZ  , timeBin)
      + Get2x2Signal(reader, branch, modX  , modZ+1, timeBin)
      + Get2x2Signal(reader, branch, modX+1, modZ+1, timeBin);
    if( max < signal ){
      max = signal;
      maxIsAtTime = timeBin;
    }
  }
  return max;
}


bool AliPHOSTriggerAnalysis::Is2x2Saturated(AliPHOSEMCReader* reader, int mod, int xIdx, int zIdx, int satThreshold)
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
  return false:
}


bool AliPHOSTriggerAnalysis::Is4x4Saturated(AliPHOSEMCReader* reader, int mod, int TRURow, int branch, int xIdx, int zIdx, int satThreshold)
{
  int modX = xIdx + TRURow * (AliPHOSGeometry::GetInstance()->GetNPhi() / kNTRURows);
  int modZ = zIdx + branch * (AliPHOSGeometry::GetInstance()->GetNZ() / kNBranches);
  return Is2x2Saturated(reader, mod, modX  , modZ  , satThreshold)
    ||   Is2x2Saturated(reader, mod, modX+1, modZ  , satThreshold)
    ||   Is2x2Saturated(reader, mod, modX  , modZ+1, satThreshold)
    ||   Is2x2Saturated(reader, mod, modX+1, modZ+1, satThreshold);
}

 
int AliPHOSTriggerAnalysis::SaveResults(TString filename = "PHOSRawAnalysisResults.root")
{
  return fHistograms->SaveResults(filename);
}


