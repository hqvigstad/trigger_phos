/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
/* $Id$ */


#include "AliTRUPedestalOutput.h"

#include <TH1I.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TString.h>

AliTRUPedestalOutput::AliTRUPedestalOutput()
: fRun(-1),
  fNEvents(0),
  fPedestals(0),
  fPedestals2d(0),
  fPedestalsId(0),
  fPedestals_branch({{{0}}}),
  fTRUSignals({{{{{0}}}}})
{

}

AliTRUPedestalOutput::~AliTRUPedestalOutput()
{
  // All the class contains is Histograms, they are owned by current ROOT folder.
}

void AliTRUPedestalOutput::SetRun ( Int_t run )
{
  if( fRun >= 0 ) // fRun is initilized to -1
    Error("SetRun", "Only one run allowed, i.e. run should not be set, then set to something different.");
  fRun = run;
}

void AliTRUPedestalOutput::EventAdded()
{
  fNEvents++;
}

TH1F* AliTRUPedestalOutput::GetPedestals()
{
  if( fPedestals )
    fPedestals->Delete();

  TH1F* fPedestals = new TH1F("fPedestals", "Pedestals", 10*1024, 0, 1024);
  fPedestals->GetXaxis()->SetTitle("Pedestals, <signal>");
  fPedestals->GetYaxis()->SetTitle("Count");

  for (unsigned int mod=0; mod<kNMods; ++mod) {
    for (unsigned int row=0; row<kNTRURows; ++row) {
      for (unsigned int branch=0; branch<kNBranches; ++branch) {
	for (unsigned int xrow=0; xrow<kN2x2XPrTRURow; ++xrow) {
	  for (unsigned int zcol=0; zcol<kN2x2ZPrBranch; ++zcol) {
	    if( ! fTRUSignals[mod][row][branch][xrow][zcol] )
	      continue;

	    double pedestal = GetPedestal(mod, row, branch, xrow, zcol);
	    fPedestals->Fill(pedestal);
	  }
	}
      }
    }
  }

  return fPedestals;
}

TH2F* AliTRUPedestalOutput::GetPedestals2d()
{
  if( fPedestals2d )
    fPedestals2d->Delete();

  TH2F* fPedestals2d = new TH2F("fPedestals2d", "Pedestals", 2*kN2x2X, 0, 2*kN2x2X,   2*kN2x2Z, 0, 2*kN2x2Z);
  fPedestals2d->GetXaxis()->SetTitle("TRU row, row");
  fPedestals2d->GetYaxis()->SetTitle("Branch, column");

  for (unsigned int mod=0; mod<kNMods; ++mod) {
    for (unsigned int row=0; row<kNTRURows; ++row) {
      for (unsigned int branch=0; branch<kNBranches; ++branch) {
	for (unsigned int xrow=0; xrow<kN2x2XPrTRURow; ++xrow) {
	  for (unsigned int zcol=0; zcol<kN2x2ZPrBranch; ++zcol) {
	    if( ! fTRUSignals[mod][row][branch][xrow][zcol] )
	      continue;

	    int xindex = 2*xrow + kN2x2XPrTRURow*row;
	    int zindex = 2*zcol + kN2x2ZPrBranch*branch;
	    double pedestal = GetPedestal(mod, row, branch, xrow, zcol);
	    fPedestals2d->SetBinContent(xindex, zindex, pedestal);
	  }
	}
      }
    }
  }
  fPedestals2d->SetDrawOption("E");
  return fPedestals2d;
}


TH2F* AliTRUPedestalOutput::GetPedestalsId()
{
  if( fPedestalsId )
    fPedestalsId->Delete();
  fPedestals = 0;

  // else Initilize
  int nhc = kNMods*kNTRURows*kNBranches*kN2x2XPrTRURow*kN2x2ZPrBranch;
  TString title("Id v Pedestal,   Id is offset in [mod][row][branch][x][z] Row-major order");
  fPedestalsId = new TH2F("fPedestalsId", title.Data(), nhc, 0, nhc, 10*1024, 0, 1024);
  fPedestals->GetXaxis()->SetTitle("Id ([mod][row][branch][x][z], Row-major order)");
  fPedestals->GetYaxis()->SetTitle("Pedestal");

  for (unsigned int mod=0; mod<kNMods; ++mod) {
    for (unsigned int row=0; row<kNTRURows; ++row) {
      for (unsigned int branch=0; branch<kNBranches; ++branch) {
	for (unsigned int xrow=0; xrow<kN2x2XPrTRURow; ++xrow) {
	  for (unsigned int zcol=0; zcol<kN2x2ZPrBranch; ++zcol) {
	    if( ! fTRUSignals[mod][row][branch][xrow][zcol] )
	      continue;

	    int index =
	      mod*kNTRURows*kNBranches*kN2x2XPrTRURow*kN2x2ZPrBranch
	    + row*kNBranches*kN2x2XPrTRURow*kN2x2ZPrBranch
	    + branch*kN2x2XPrTRURow*kN2x2ZPrBranch
	    + xrow*kN2x2ZPrBranch
	    + zcol;
	    double pedestal = GetPedestal(mod, row, branch, xrow, zcol);
	    double pederror = GetPedestalError(mod, row, branch, xrow, zcol);
	    fPedestalsId->SetBinContent(index+1, pedestal);
	    fPedestalsId->SetBinError(index+1, pederror);
	  }
	}
      }
    }
  }
  fPedestalsId->SetDrawOption("E");
  return fPedestalsId;
}

TH1F* AliTRUPedestalOutput::GetPedestals_branch ( UInt_t mod, UInt_t row, UInt_t branch )
{
  if( fPedestals )
    fPedestals->Delete();

  TH1F* fPedestals_branch = new TH1F("fPedestals", "Pedestals", 10*1024, 0, 1024);
  fPedestals_branch->GetXaxis()->SetTitle("Pedestals, <signal>");
  fPedestals_branch->GetYaxis()->SetTitle("Count");

  for (unsigned int xrow=0; xrow<kN2x2XPrTRURow; ++xrow) {
    for (unsigned int zcol=0; zcol<kN2x2ZPrBranch; ++zcol) {
      if( ! fTRUSignals[mod][row][branch][xrow][zcol] )
	continue;
      double pedestal = GetPedestal(mod, row, branch, xrow, zcol);
      fPedestals->Fill(pedestal);
    }
  }

  return fPedestals;
}

TH1I* AliTRUPedestalOutput::GetTRUSignals ( UInt_t mod, UInt_t row, UInt_t branch, UInt_t x, UInt_t z )
{
  if( fTRUSignals[mod][row][branch][x][z] )
    return fTRUSignals[mod][row][branch][x][z];

  // else Initilize
  char name[256];
  sprintf(name, "fTRUSignals_m:%d_r:%d_b:%d_x:%02d_z:%02d", mod, row, branch, x, z);
  char title[256];
  sprintf(title, "TRU Signal mod:%d row:%d branch:%d x:%02d z:%02d", mod, row, branch, x, z);
  
  TH1I* hist = new TH1I(name, title, 1024, 0, 1024);
  hist->GetXaxis()->SetTitle("Bin");
  hist->GetYaxis()->SetTitle("Count");
  return fTRUSignals[mod][row][branch][x][z] = hist;
}

Double_t AliTRUPedestalOutput::GetPedestal ( UInt_t mod, UInt_t row, UInt_t branch, UInt_t x, UInt_t z )
{
  
  if( fTRUSignals[mod][row][branch][x][z] )
    return fTRUSignals[mod][row][branch][x][z]->GetMean();
  else
    return -1;
}

Double_t AliTRUPedestalOutput::GetPedestalError ( UInt_t mod, UInt_t row, UInt_t branch, UInt_t x, UInt_t z )
{
  if( fTRUSignals[mod][row][branch][x][z] )
    return fTRUSignals[mod][row][branch][x][z]->GetMeanError();
  else
    return -1;

}

Double_t AliTRUPedestalOutput::GetRMS ( UInt_t mod, UInt_t row, UInt_t branch, UInt_t x, UInt_t z )
{
  if( fTRUSignals[mod][row][branch][x][z] )
    return fTRUSignals[mod][row][branch][x][z]->GetRMS();
  else
    return -1;
}
