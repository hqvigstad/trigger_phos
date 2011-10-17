#ifndef ALIPHOSTRUREGIONRAWREADER_H
#define ALIPHOSTRUREGIONRAWREADER_H

class AliCaloRawStreamV3;

#include <vector>
using namespace std;

class AliPHOSTRURegionRawReader
{
 public:
  AliPHOSTRURegionRawReader();
  ~AliPHOSTRURegionRawReader();

  Short_t GetTriggerSignal(int xIdx, int zIdx, int timeBin) const {return fSignals[xIdx][zIdx][timeBin];}
  Short_t GetTriggerFlag(int xIdx, int zIdx, int timeBin) const {return fFlags[xIdx][zIdx][timeBin];}
  bool IsActive() {return fActive;}
  bool IsAcitve(int timeBin) {return fActiveTime[timeBin];}
  
  void ReadFromStream(AliCaloRawStreamV3* );
  void Reset();
  
 private:
  vector<vector<vector< Short_t > > > fSignals; // 2x2 Trigger Signal Sum, [x][z][t]
  vector<vector<vector< Bool_t > > > fFlags; // Trigger Flag, [x][z][t]
  
  bool fActive; // Active
  vector<bool> fActiveTime; // Active [t]
  
  const static int kDefaultNTimeBins = 128;
  const static int kNTRURows = 4;
  const static int kNBranches = 2;
};

#endif
