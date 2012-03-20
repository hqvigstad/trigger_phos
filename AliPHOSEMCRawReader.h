#ifndef ALIPHOSEMCRAWREADER_H
#define ALIPHOSEMCRAWREADER_H

#include "AliCaloRawStreamV3.h";

#include <vector>
using namespace std;

class AliPHOSEMCRawReader
{
 public:
  AliPHOSEMCRawReader();
  ~AliPHOSEMCRawReader();

  Short_t GetSignal(int mod, int rowx, int colz, int timeBin) {return fSignals[mod][rowx][colz][timeBin];}
  Short_t IsActive(int mod, int rowx, int colz) {return fActiveCell[mod][rowx][colz];}

  void ReadFromStream(AliCaloRawStreamV3* );
  void Reset();
  
 private:
  vector<vector<vector<vector< Short_t > > > > fSignals; // [mod][x][z][t]
  vector<bool> fActive; // fActive[mod]
  vector<vector<vector<bool> > > fActiveCell; // fActive[mod][x][z]
  
  const static Short_t kDefaultNTimeBins = 62;
};

#endif
