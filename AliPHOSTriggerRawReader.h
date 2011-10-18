#ifndef ALIPHOSTRURAWREADER_H
#define ALIPHOSTRURAWREADER_H

class AliCaloRawStreamV3;
class AliPHOSTRURawReader;

#include <vector>
using namespace std;

class AliPHOSTriggerRawReader
{
 public:
  AliPHOSTriggerRawReader();
  ~AliPHOSTriggerRawReader();
  
  AliPHOSTRURawReader* GetTRU(int mod, int truRow, int branch);
  
  void ReadFromStream(AliCaloRawStreamV3* );
  void Reset();
  
 private:
  vector<vector<vector< AliPHOSTRURawReader* > > > fTRUs; // [mod][truRow][branch]

  const static int kNMods = 5;
  const static int kNTRURows = 4;
  const static int kNBranches = 2;
};

#endif 
