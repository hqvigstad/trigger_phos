#ifndef ALIPHOSEMCRAWREADER_H
#define ALIPHOSEMCRAWREADER_H

class AliCaloRawStreamV3;

#include <vector>
using namespace std;

class AliPHOSEMCRawReader
{
 public:
  AliPHOSEMCRawReader();
  ~AliPHOSEMCRawReader();

  Short_t GetSignal(int mod, int rowx, int colz, int timeBin) {return fSignals[mod][rowx][colz][timeBin];}

  void ReadFromStream(AliCaloRawStreamV3* );
  void Reset();
  
 private:
  vector<vector<vector<vector< Short_t > > > > fSignals; // [mod][x][z][t]
  vector<bool> fActive; // fActive[mod]
  
  const static Short_t kDefaultNTimeBins = 62;
};

#endif
