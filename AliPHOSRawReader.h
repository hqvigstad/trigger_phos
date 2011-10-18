#ifndef ALIPHOSRAWREADER_H
#define ALIPHOSRAWREADER_H

class AliPHOSEMCRawReader;
class AliPHOSTriggerRawReader;

class AliCaloRawStreamV3;

class AliPHOSRawReader
{
 public:
  AliPHOSRawReader();
  ~AliPHOSRawReader();

  void ReadFromStream(AliCaloRawStreamV3* );

  AliPHOSEMCRawReader* GetHGReader() {return fHGReader;}
  AliPHOSEMCRawReader* GetLGReader() {return fLGReader;}
  AliPHOSTriggerRawReader* GetTriggerReader() {return fTriggerReader;}

  
  const static int kNMods = 5;
  const static int kNTRURows = 4;
  const static int kNBranches = 2;

 protected:
  AliPHOSEMCRawReader* fHGReader;
  AliPHOSEMCRawReader* fLGReader;
  AliPHOSTriggerRawReader* fTriggerReader;
};

#endif
