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
  int GetTRUPedestal(int mod, int TRURow, int branch, int xIdx, int zIdx) const;
  bool GetTriggerSummeryReadoutOn(int mod, int TRURow, int branch) const;
  bool GetTRUSignalReadoutOn(int mod, int TRURow, int branch) const;
  int GetTRUSignalTimeBinFrom(int mod, int TRURow, int branch) const;
  int GetTRUSignalTimeBinTo(int mod, int TRURow, int branch) const;
  
  // Setters
  int SetTRUPedestal(int pedestal, int mod, int int xIdx, int zIdx);
  bool SetTriggerSummeryReadoutOn(bool isOn, int mod, int TRURow, int branch);
  bool SetTRUSignalReadoutOn(bool isOn, int mod, int TRURow, int branch);
  int SetTRUSignalTimeBinFrom(int fromBin, int mod, int TRURow, int branch);
  int SetTRUSignalTimeBinTo(int toBin, int mod, int TRURow, int branch);
  
  // Misc
  virtual void Print(Option_t *option = "") const; 
  void Reset();

  static const int kNMods = 5;
  static const int kNTRURows = 4;
  static const int kNBranches = 2;
  static const int kNTRUX = 8;
  static const int kNTRUZ = 14;
  
 protected:
  UShort_t fTRUPedestals[kNMods][kNTRURows][kNBranches][kNTRUX][kNTRUZ];
  Bool_t fTRUTriggerBitReadoutOn[kNMods][kNTRURows][kNBranches];
  Bool_t fTRUSignalReadoutOn[kNMods][kNTRURows][kNBranches];
  UChar_t fTRUSignalTimeBinFrom[kNMods][kNTRURows][kNBranches];
  UChar_t fTRUSignalTimeBinTo[kNMods][kNTRURows][kNBranches];
  
  ClassDef(AliPHOSTRUCalibData, 0)
};

#endif
