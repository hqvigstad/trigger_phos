#ifndef ALIPHOSTRIGGERPARAMETERS_H
#define ALIPHOSTRIGGERPARAMETERS_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */


#include "TNamed.h"

/*  class for TRU Calib Data: Pedestals, etc...
 */
class AliPHOSTriggerParameters : public TNamed {

 public:
  AliPHOSTriggerParameters();
  AliPHOSTriggerParameters(const char* name);
  AliPHOSTriggerParameters(const AliPHOSTriggerParameters& );
  AliPHOSTriggerParameters& operator= (const AliPHOSTriggerParameters& );
  virtual ~AliPHOSTriggerParameters();
  
  // Getters
  UShort_t GetTRUPedestal(int mod, int TRURow, int branch, int xIdx, int zIdx) const;
  Bool_t GetTRUReadoutOn(int mod, int TRURow, int branch) const;
  Bool_t GetTRUSignalReadoutOn(int mod, int TRURow, int branch) const;
  UShort_t GetTRUSignalTimeBinFrom(int mod, int TRURow, int branch) const;
  UShort_t GetTRUSignalTimeBinTo(int mod, int TRURow, int branch) const;
  UShort_t GetTRUThreshold(int mod, int TRURow, int branch) const;
  UShort_t GetTRUMaskChannel(int mod, int TRURow, int branch) const;
  // MaskArrayType GetTORMaskArray(int mod, int tor) const;
  // ReadoutMaskType GetTORReadoutMask(int mod, int tor) const;
  
  
  // Setters
  void SetTRUPedestal(UShort_t pedestal, int mod, int TRURow, int branch, int xIdx, int zIdx);
  void SetTRUReadoutOn(Bool_t isOn, int mod, int TRURow, int branch);
  void SetTRUSignalReadoutOn(Bool_t isOn, int mod, int TRURow, int branch);
  void SetTRUSignalTimeBinFrom(UShort_t fromBin, int mod, int TRURow, int branch);
  void SetTRUSignalTimeBinTo(UShort_t toBin, int mod, int TRURow, int branch);
  void SetTRUThreshold(UShort_t threshold, int mod, int TRURow, int branch);
  void SetTRUMaskChannel(UShort_t mask, int mod, int TRURow, int branch);
  // void SetTORMaskArray(MaskArrayType ma, int mod, int tor);
  // void SetTORReadoutMask(ReadoutMaskType rm, int mod, int tor);
  
  // Misc
  virtual void Print(Option_t *option = "") const; 
  void Reset();

  static const int kNMods = 5; // Number of PHOS Modules
  static const int kNTORs = 2; // Number of TORs per Module
  static const int kNTRURows = 4; // Number of TRU rows
  static const int kNBranches = 2; // Number of Branches
  static const int kNTRUX = 8; // Number of 2x2 per TRU in x
  static const int kNTRUZ = 14; // Number of 2x2 per TRU in z
  static const UShort_t kIdealTRUPedestal = 512; // Ideal TRU Pedestal
  static const int kDefaultNTRUTimeBins = 128; // Number of timebins
  
 protected:
  // TRU Parameters:
  UShort_t fTRUPedestals[kNMods][kNTRURows][kNBranches][kNTRUX][kNTRUZ]; // TRU Pedestals
  Bool_t fTRUTriggerBitReadoutOn[kNMods][kNTRURows][kNBranches]; // TRU TriggerBit Readout is on
  Bool_t fTRUSignalReadoutOn[kNMods][kNTRURows][kNBranches]; // TRU Signal Readout is on
  UChar_t fTRUSignalTimeBinFrom[kNMods][kNTRURows][kNBranches]; // TRU from (including) timebin
  UChar_t fTRUSignalTimeBinTo[kNMods][kNTRURows][kNBranches]; // TRU to (including) timebin
  UShort_t  fTRUThreshold[kNMods][kNTRURows][kNBranches]; // TRU Threshold
  UShort_t fTRUMaskChannel[kNMods][kNTRURows][kNBranches]; // TRU Mask Channel
  
  // TOR Parameters:
  // MaskArrayType fTORMaskArray[kNMods][kNTORs];
  // ReadoutMaskType fTORReadoutMask[kNMods][kNTORs];
  
  ClassDef(AliPHOSTriggerParameters, 0)
};

#endif
