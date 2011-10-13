#include "AliPHOSTriggerAnalysis.h"
#include "AliPHOSRawReader.h"
#include "AliCentralTrigger.h"

#include "TChain.h"
#include "TString.h"
#include "AliRawReaderChain.h"
#include "AliCaloRawStreamV3.h"
#include "AliLog.h"
#include "AliTriggerClass.h"
#include "AliTriggerConfiguration.h"
#include "AliCDBManager.h"
#include "TMath.h"

#include <iostream>
#include <fstream>
using namespace std;

void analyseRawChain(TChain* );
void addFilesToChain(const TString fileName, TChain* );
TString GetTriggerClass(AliRawReaderChain* );


void phosTriggerAnalysis(TString rawFileList = "files.txt")
{
  TChain *rawChain = new TChain("RAW");
  addFilesToChain(rawFileList, rawChain );
  
  analyseRawChain(rawChain);
}


// analyseRawChain.
// A macro that takes a chain of raw files and does the analysis of its PHOS raw Stream
void analyseRawChain(TChain* chain)
{
  AliCDBManager::Instance()->SetDefaultStorage("raw://");

  // Create the analysis objects
  AliPHOSTriggerAnalysis* rawAnalysisCINT7 = 
    new AliPHOSTriggerAnalysis();

  AliPHOSTriggerAnalysis* rawAnalysisCPHI7 = 
    new AliPHOSTriggerAnalysis();


  // Create an AliPHOSRawReader to read the stream
  AliPHOSRawReader* phosRawReader = new AliPHOSRawReader;


  // Prepare and loop over the chain
  AliRawReaderChain* rawChain = new AliRawReaderChain(chain);
  rawChain->Reset();
  AliCaloRawStreamV3* phosRawStream = new AliCaloRawStreamV3(rawChain,"PHOS");
  UInt_t runNumber = -1;
  Int_t event_count = 0;
  while (rawChain->NextEvent()) {
    std::cout << "\revent: " << event_count++ << "  "<< flush;
    if( rawChain->GetRunNumber() != runNumber ){
      runNumber = rawChain->GetRunNumber();
      AliCDBManager::Instance()->SetRun(runNumber);
      Printf("New run number, current run number is: %d", runNumber);
    }
    
    TString triggers = GetTriggerClass(rawChain);
    if(triggers.Contains("CINT7") || triggers.Contains("CPHI7")){
      phosRawReader->ReadFromStream( phosRawStream );
      
      if( triggers.Contains("CINT7") )
	rawAnalysisCINT7->ProcessEvent(phosRawReader);
      if( triggers.Contains("CPHI7") )
	rawAnalysisCPHI7->ProcessEvent(phosRawReader);
    }
  }
  
  delete phosRawStream;
  delete phosRawReader;
  
  rawAnalysisCINT7->SaveResults("rawResultsCINT7.root");
  rawAnalysisCPHI7->SaveResults("rawResultsCPHI7.root");
}



void addFilesToChain(const TString rawFileList, TChain* chain)
{
  ifstream inList;
  inList.open( rawFileList.Data() );
  char rawFile[256];
  while( inList.good() ) {
    inList >> rawFile;
    if (rawFile[0] == '!') continue;
   printf("Add file %s to chain\n",rawFile);
   chain->Add(rawFile);
 }
  inList.close();
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
      rawChain->LoadTriggerClass(trclass->GetName(),trindex);
      if (triggerMask & (1ull << trindex)) {
	trclasses += " ";
	trclasses += trclass->GetName();
	trclasses += " ";
      }
    }
  }
  return trclasses;
}
