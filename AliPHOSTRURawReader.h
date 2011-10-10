#ifndef ALIPHOSTRURAWREADER_H
#define ALIPHOSTRURAWREADER_H

#include <vector>
using namespace std;

class AliPHOSTRURawReader
{
 public:
  AliPHOSTRURawReader();
  ~AliPHOSTRURawReader();
  
  AliPHOSTRURegionRawReader* GetTRURegion(int mod, int truRow, int branch);
  
  void ReadFromStream(AliCaloRawStreamV3* );
  void Reset();
  
 private:
  vector<vector<vector< AliPHOSTRURegionRawReader* >>> fRegions; // [mod][truRow][branch]

  const static int kNTRURows = 4;
  const static int kNBranches = 2;
};

#endif 
