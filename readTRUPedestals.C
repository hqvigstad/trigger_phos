#include "AliPHOSTriggerParameters.h"

#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"

#include <iostream>
#include <fstream>
using namespace std;

void readTRUPedestals(AliPHOSTriggerParameters* parameters)
{
  for (Int_t mod = 2; mod < 5; ++mod) { // module
    for (Int_t tru = 0; tru < 4; tru++) { // tru row
      for (Int_t branch = 0; branch < 2; branch++) { // branch
	
	// Open the Appropriate pedestal file
	TString fileName("pedestal_m");
	fileName = fileName += mod;
	fileName+="_r";
	fileName+=tru;
	fileName+="_b";
	fileName+=branch;
	fileName+=".dat";
	std::ifstream instream;
	instream.open(fileName.Data());
	
	  
	// Read pedestals from file
	if( ! instream.is_open() )
	  Printf("E-TRUPedestals: could not open %s", fileName.Data());
	else
	  {
	    Int_t ped[112];
	      
	    char ch_s[35];
	    char *ch_s_p = ch_s;
	    //Int_t nlines = 0;
	      
	    Int_t t_ped_0 =0;
	    Int_t t_ped_1 =0;
	    Int_t t_ped_2 =0;
	      
	    for(Int_t n=0; n<112; n++)
	      {
		instream.getline(ch_s_p,36);
		if (ch_s_p[23]<='9' && ch_s_p[23]>='0')
		  {
		    t_ped_0 = ch_s_p[23]-'0';
		  }
		else if (ch_s_p[23]>='A' && ch_s_p[23]<='Z')
		  {
		    t_ped_0 = ch_s_p[23]-'A'+10;
		      
		  }
		  
		if (ch_s_p[22]<='9' && ch_s_p[22]>='0')
		  {
		    t_ped_1 = ch_s_p[22]-'0';
		  }
		else if (ch_s_p[22]<='Z' && ch_s_p[22]>='A')
		  {
		    t_ped_1 = ch_s_p[22]-'A'+10;
		  }
		  
		if (ch_s_p[21]<='9' && ch_s_p[21]>='0')
		  {
		    t_ped_2 = ch_s_p[21]-'0';
		  }
		else if (ch_s_p[21]<='Z' && ch_s_p[21]>='A')
		  {
		    t_ped_2 = ch_s_p[21]-'A'+10;
		  }
		  
		ped[n]=t_ped_2*256+t_ped_1*16+t_ped_0;
		  
		  
	      }
	    for (Int_t xrow = 0; xrow < 8; xrow++){
	      for (Int_t zcol = 0; zcol < 14; zcol++){
		Int_t pedestal = ped[zcol*8+xrow];

		if( pedestal < 612 && pedestal > 412 ) // resonable
		  parameters->SetTRUPedestal(pedestal, mod, tru, branch, xrow, zcol);
		else // unresonable
		  continue;
	      }
	    }
	  } // Ends read of pedestals from branch from file.
	instream.close();
      }// end branch
    }// end tru
      
  }// end for mod
}


void drawTRUPedestals(AliPHOSTriggerParameters* parameters)
{
  for (Int_t mod = 2; mod < 5; ++mod) { // module
    // Make some Histograms:
    TString name("TRU_pedestals_m");
    name += mod;
    TH1F* ped_hist = new TH1F(name, name, 1000, 0, 1000 );
    name += "_2d";
    TH2F* ped_hist_2d = new TH2F(name, name, 32, 0, 32, 28, 0, 28);
    
    for (Int_t tru = 0; tru < 4; tru++) { // tru row
       for (Int_t branch = 0; branch < 2; branch++) { // branch
	 for (Int_t xrow = 0; xrow < 8; xrow++){
	   for (Int_t zcol = 0; zcol < 14; zcol++){
	     const int gxrow = tru*8+xrow;
	     const int gzcol = branch*14+zcol;
	     Int_t pedestal = parameters->GetTRUPedestal(mod, tru, branch, xrow, zcol);
	
	     ped_hist->Fill(pedestal);
	     ped_hist_2d->Fill(gxrow, gzcol, pedestal);
	   }
	 }
       }
     }
    (new TCanvas)->SetLogy();
    ped_hist->GetZaxis()->SetRangeUser(500, 520);
    ped_hist->DrawCopy(); // Todo, move this write biz. to HistHandler
    new TCanvas;
    ped_hist_2d->DrawCopy("colz");
  }
}
