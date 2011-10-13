#ifndef ALIPHOSRAWREADER_H
#define ALIPHOSRAWREADER_H

class AliPHOSEMCRawReader;
class AliPHOSTRURawReader;

class AliCaloRawStreamV3;

class AliPHOSRawReader
{
 public:
  AliPHOSRawReader();
  ~AliPHOSRawReader();

  void ReadFromStream(AliCaloRawStreamV3* );

  AliPHOSEMCRawReader* GetHGReader() {return fHGReader;}
  AliPHOSEMCRawReader* GetLGReader() {return fLGReader;}
  AliPHOSTRURawReader* GetTRUReader() {return fTRUReader;}

 protected:
  AliPHOSEMCRawReader* fHGReader;
  AliPHOSEMCRawReader* fLGReader;
  AliPHOSTRURawReader* fTRUReader;
};

#endif
