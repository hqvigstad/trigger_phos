#include "TROOT.h"
#include "TChain.h"
#include "TString.h"
#include "AliRawReaderChain.h"
#include "AliCaloRawStreamV3.h"
#include "AliCDBManager.h"

#include <fstream>
#include <iostream>

#include "AliTRUPedestalAnalysis.h"
#include "AliTRUPedestalOutput.h"

void addFilesToChain(const TString rawFileList, TChain* chain);
void saveResults(AliTRUPedestalOutput* output, TString saveToFile);


void truPedestalAnalysis(TString rawFileList = "files.txt", TString saveToFile = "truOutput.root")
{
  // Raw Chain Initialization
  TChain *chain = new TChain("RAW");
  addFilesToChain(rawFileList, chain );
  AliRawReaderChain* rawChain = new AliRawReaderChain(chain);
  rawChain->Reset();

  // PHOS Raw Stream Initialization
  AliCaloRawStreamV3* phosRawStream = new AliCaloRawStreamV3(rawChain,"PHOS");

  // Analysis object Initialization
  AliTRUPedestalAnalysis* anaObj = new AliTRUPedestalAnalysis();

  // Loop over events in Chain
  UInt_t runNumber = -1;
  Int_t event_count = 0;
  while (rawChain->NextEvent())
  { // Print out event number:
    std::cout << "\r" << "event: " << ++event_count
  	      << "/"<< rawChain->GetNumberOfEvents() << " " << std::flush;
    if( rawChain->GetRunNumber() != runNumber ){
      // if new event number, update OCDB
      runNumber = rawChain->GetRunNumber();
      AliCDBManager::Instance()->SetRun(runNumber);
      Printf("New run number, current run number is: %d", runNumber);
    }

    // Process Event using analysis Object
    anaObj->ProcessEvent(phosRawStream);
  }

  // Save output to file, in form of single entry in tree
  saveResults(anaObj->GetOutput(), saveToFile);

  // cleanup
  delete anaObj;
  delete phosRawStream;
  delete rawChain;
  delete chain;
}

void addFilesToChain(const TString rawFileList, TChain* chain)
{
  // Open rawFileList and add files contained to chain
  std::ifstream inList;
  inList.open( rawFileList.Data() );
  std::string line;
  while( std::getline(inList, line ) ) {
    printf("Add file %s to chain\n", line.c_str());
    chain->Add(line.c_str());
 }
  inList.close();
}

void saveResults(AliTRUPedestalOutput* output, TString saveToFile)
{
  if( ! output )
    gROOT->Error("truPedestalAnalysis.C::saveResults: no ouput", "no msgfmt");

  TTree* tree = new TTree("pedestalTree", "Pedestal Analysis Tree");
  TBranch* branch = tree->Branch("pedOutput", &output);

  Printf("Filling:");
  branch->Print();;
  tree->Fill();

  tree->SaveAs(saveToFile);
}
