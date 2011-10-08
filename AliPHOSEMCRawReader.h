#ifndef ALIPHOSEMCRAWREADER_H
#define ALIPHOSEMCRAWREADER_H

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
  vector<vector<vector<vector< Short_t >>>> fSignals; // [mod][x][z][t]
  
  const static size_type kDefaultNTimeBins = 62;
};

#endif
