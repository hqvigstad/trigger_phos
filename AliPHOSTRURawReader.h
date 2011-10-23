#ifndef ALIPHOSTRUREGIONRAWREADER_H
#define ALIPHOSTRUREGIONRAWREADER_H

class AliCaloRawStreamV3;

#include <vector>
using namespace std;

class AliPHOSTRURawReader
{
 public:
  AliPHOSTRURawReader();
  ~AliPHOSTRURawReader();

  Short_t GetTriggerSignal(int xIdx, int zIdx, int timeBin) const {return fSignals[xIdx][zIdx][timeBin];}
  Bool_t GetTriggerFlag(int xIdx, int zIdx, int timeBin) const {return fFlags[xIdx][zIdx][timeBin];}
  bool IsActive() {return fActive;}
  bool IsAcitve(int timeBin) {return fActiveTime[timeBin];}
  
  void ReadFromStream(AliCaloRawStreamV3* );
  void Reset();
  
  const static int kNTimeBins = 128;
  const static int kNTRURows = 4;
  const static int kNBranches = 2;

  const static int kN2x2X = 64/2;
  const static int kN2x2Z = 56/2;
  const static int kN2x2XPrTRURow = kN2x2X / kNTRURows;
  const static int kN2x2ZPrBranch = kN2x2Z / kNBranches;
  const static int kN4x4XPrTRURow = kN2x2XPrTRURow -1;
  const static int kN4x4ZPrBranch = kN2x2ZPrBranch -1;
  
  const static int kDefaultSignalValue = 512;

 private:
  vector<vector<vector< Short_t > > > fSignals; // 2x2 Trigger Signal Sum, [x][z][t]
  vector<vector<vector< Bool_t > > > fFlags; // Trigger Flag, [x][z][t]
  
  bool fActive; // Active
  vector<bool> fActiveTime; // Active [t]
  
};

#endif
