#include "AliPHOSRawAnalysis.h"

#include <iostream>
using namespace std;

void analyseRawChain(TChain* );
void addFilesToChain(string fileName, TChain* );
TString GetTriggerClass(AliRawReaderChain* );


void phosRawAnalysis(string rawFileList = "files.txt";)
{				
  TChain *rawChain = new TChain("rawChain");
  addFilesToChain(const string rawFileList, rawChain );
  
  analyseRawChain(rawChain);
}


// analyseRawChain.
// A macro that takes a chain of raw files and does the analysis of its PHOS raw Stream
void analyseRawChain(TChain* chain)
{
  // Create the analysis objects
  AliPHOSRawAnalysis* rawAnalysisCINT7 = 
    new AliPHOSRawAnalysis("rawAnalysisCINT7", "Analysis of raw CINT7 events");
  rawAnalysisCINT7->SetVerbose(1);
  rawAnalysisCINT7->PlotEventSpecifics(false);

  AliPHOSRawAnalysis* rawAnalysisCPHI7 = 
    new AliPHOSRawAnalysis("rawAnalysisCPHI7", "Analysis of raw CPHI7 events");
  rawAnalysisCPHI7->SetVerbose(1);
  rawAnalysisCPHI7->PlotEventSpecifics(false);


  // Create an AliPHOSRawReader to read the stream
  AliPHOSRawReader* phosRawReader = new AliPHOSRawReader;
  phosRawReader->SetVerbose(1);

  // Prepare and loop over the chain
  AliRawReaderChain* rawChain = new AliRawReaderChain(chain);
  rawAnalysis->Process(rawChain);
  rawReaderChain->Reset();
  AliCaloRawStreamV3* phosRawStream = new AliCaloRawStreamV3(rawChain,"PHOS");
  Int_t runNumber = -1;
  Int_t event_count = 0;
  while (rawChain->NextEvent()) {
    std::cout << "\revent: " << event_count++ << "  "<< flush;
    if( rawReaderChain->GetRunNumber() != runNumber ){
      runNumber = rawReaderChain->GetRunNumber() 
      Printf("New run number, current run number is: %d", runNumber);
    }
    
    TString triggers = GetTriggerClass(rawChain);
    if(triggers.Contains("CINT7") || triggers.Contains("CPHI7")){
      phosRawReader->ReadEvent( phosRawStream );
      
      if( triggers.Contains("CINT7") )
	rawAnalysisCINT7->ProcessEvent(phosRawReader);
      if( triggers.Contains("CPHI7") )
	rawAnalysisCPHI7->ProcessEvent(phosRawReader);
    }
  }
  
  delete rawStream;
  delete phosRawReader
  
  rawAnalysis->SaveResults("rawResults.root");
}



void addFilesToChain(string rawFileList, TChain* chain)
{
  ifstream inList;
  inList.open( rawFileList.c_str() );
  char rawFile[256];
  while( inList.good() ) {
    inList >> rawFile;
    if (rawFile[0] == '!') continue;
   printf("Add file %s to chain\n",rawFile);
   rawChain->Add(rawFile);
 }
  inList.close();

  analyseRawChain(rawChain);
}


TString GetTriggerClass(AliRawReaderChain* rawChain)
{
  ULong64_t triggerMask = rawChain->GetClassMask();

  // Convert a trigger mask to a trigger class
  AliCentralTrigger aCTP;
  TString configstr("");
  TString trclasses;
  if (!aCTP.LoadConfiguration(configstr)) { // Load CTP config from OCDB
    AliInfoGeneral("","No trigger configuration found in OCDB! The trigger configuration information will not be used!");
    return trclasses;
  }
  aCTP.SetClassMask(triggerMask);
  AliTriggerConfiguration *config = aCTP.GetConfiguration();
  const TObjArray& classesArray = config->GetClasses();
  Int_t nclasses = classesArray.GetEntriesFast();
  for( Int_t iclass=0; iclass < nclasses; iclass++ ) {
    AliTriggerClass* trclass = (AliTriggerClass*)classesArray.At(iclass);
    if (trclass && trclass->GetMask()>0) {
      Int_t trindex = TMath::Nint(TMath::Log2(trclass->GetMask()));
      reader->LoadTriggerClass(trclass->GetName(),trindex);
      if (triggerMask & (1ull << trindex)) {
	trclasses += " ";
	trclasses += trclass->GetName();
	trclasses += " ";
      }
    }
  }
  return trclasses;
}
