#ifndef PHOSRAWREADER_H
#define PHOSRAWREADER_H

class AliPHOSEMCRawReader;
class AliPHOSTRURawReader;

class AliPHOSRawReader
{
  AliPHOSRawReader();
  ~AliPHOSRawReader{};
  
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
