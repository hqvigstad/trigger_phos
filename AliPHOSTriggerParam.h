#ifndef ALIPHOSTRUCALIBDATA_H
#define ALIPHOSTRUCALIBDATA_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

/*
 *  class for TRU Calib Data: Pedestals, etc...
 */
 
#include "TNamed.h"

class AliPHOSTRUData: public TNamed {

 public:
  AliPHOSTRUCalibData();
  AliPHOSTRUCalibData(const char* name);
  AliPHOSTRUCalibData(const AliPHOSTRUCalibData &calibda);
  AliPHOSTRUCalibData& operator= (const AliPHOSTRUCalibData &calibda);
  virtual ~AliPHOSTRUCalibData();

  // Getters
  int GetTRUPedestal(Int_t mod, Int_t xIdx, Int_t zIdx);

  // Setters
  void SetADCchannelTRU(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetADCpedestalTRU(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetHighLowRatioTRU(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetTimeShiftTRU(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetAltroOffsetTRU(Int_t module, Int_t column, Int_t row, Int_t value);
  void SetSampleTimeStep(Float_t step) ;
  
  // Misc
  void Reset();
  virtual void Print(Option_t *option = "") const; 

 protected:
  UShort_t fTRUPedestals[5][28][32];
  UChar_t

  ClassDef(AliPHOSTRUCalibData, 0)
};

#endif
