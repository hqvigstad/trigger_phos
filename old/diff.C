void diff()
{
  const int kNMods = 5;
  const int kNTRURows = 4;
  const int kNBranches = 2;
  const int kN2x2X = 64/2;
  const int kN2x2Z = 56/2;
  const int kN2x2XPrTRURow = kN2x2X / kNTRURows;
  const int kN2x2ZPrBranch = kN2x2Z / kNBranches;
 

  gROOT->LoadMacro("AliPHOSTriggerAnalysisHistograms.cxx++g");

  AliPHOSTriggerAnalysisHistograms* hists163756 = new AliPHOSTriggerAnalysisHistograms("trigger_163756_All.root");
  AliPHOSTriggerAnalysisHistograms* hists163759 = new AliPHOSTriggerAnalysisHistograms("trigger_163759_All.root");
  
  
  TH1F* diffHist = new TH1F("diffHist", "relative mean, channel-by-channel (163759-163756)", 200, -1, 1);
  TH1I* rms_163759 = new TH1I("rms_163759", "rm_163759", 1000, 0, 10);
  TH1I* rms_163756 = new TH1I("rms_163756", "rm_163756", 1000, 0, 10);

  for(int mod = 2; mod < kNMods; ++mod) {
    for(int truRow = 0; truRow < kNTRURows; ++truRow) {
      for(int branch = 0; branch < kNBranches; ++branch) {
	if( (mod==3 && truRow==0 && branch==1)
	    || (mod==3 && truRow==2 && branch==0)
	    || (mod==3 && truRow==2 && branch==1)
	    || (mod==4 && truRow==0 && branch==0) )
	  continue;
	for(int xIdx = 0; xIdx < kN2x2XPrTRURow; ++xIdx) {
	  for(int zIdx = 0; zIdx < kN2x2ZPrBranch; ++zIdx) {
	    if( hists163756 -> IsInitTRUSignal(mod, truRow, branch, xIdx, zIdx) ){
	      const double mean56 = hists163756->GetTRUSignal(mod, truRow, branch, xIdx, zIdx)->GetMean();
	      const double mean59 = hists163759->GetTRUSignal(mod, truRow, branch, xIdx, zIdx)->GetMean();
	      const double rms56 = hists163756->GetTRUSignal(mod, truRow, branch, xIdx, zIdx)->GetRMS();
	      const double rms59 = hists163759->GetTRUSignal(mod, truRow, branch, xIdx, zIdx)->GetRMS();
	      
	      diffHist->Fill(mean59 - mean56);
	      rms_163756->Fill(rms56);
	      rms_163759->Fill(rms59);
	    }
	  }
	}
      }
    }
  }
  cout << diffHist->GetMean() << endl;
  diffHist->GetXaxis()->SetRangeUser(-10,10);
  cout << diffHist->GetMean() << endl;
  diffHist->DrawCopy();

  new TCanvas;
  rms_163756->Draw();
  rms_163759->SetLineColor(kRed);
  rms_163759->Draw("same");
}
