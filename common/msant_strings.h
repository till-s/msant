#ifndef MSANT_STRINGS_H
#define MSANT_STRINGS_H

#define mSelOnlyOnePoint (msant_strings[0])
#define mPlaceExcitation (msant_strings[1])
#define mDrawPoints (msant_strings[2])
#define mNoSelection (msant_strings[3])
#define fKlon_g_g (msant_strings[4])
#define mInvalidRad (msant_strings[5])
#define mSelFrstPoint (msant_strings[6])
#define mSelScndPoint (msant_strings[7])
#define mMoveVector (msant_strings[8])
#define mCloneVector (msant_strings[9])
#define mUndelNotPoss (msant_strings[10])
#define fCantWrtFile (msant_strings[11])
#define fDefFileName (msant_strings[12])
#define mFileNotFound (msant_strings[13])
#define fFileVersionErr (msant_strings[14])
#define mNoFileLoaded (msant_strings[15])
#define mSelRefPoint (msant_strings[16])
#define fTestVersion (msant_strings[17])
#define fDxDy (msant_strings[18])
#define fCmplxVolt (msant_strings[19])
#define mMove (msant_strings[20])
#define mPlaceCopy (msant_strings[21])
#define mPlaceClone (msant_strings[22])
#define mWrnFileNotSaved (msant_strings[23])
#define mAckSave (msant_strings[24])
#define mAckCancel (msant_strings[25])
#define mAckOverride (msant_strings[26])
#define mAckYes (msant_strings[27])
#define mAckNo (msant_strings[28])
#define mAckIgnore (msant_strings[29])
#define mAckCont (msant_strings[30])
#define mWrnLooseResult (msant_strings[31])
#define mWrnGeoModified (msant_strings[32])
#define mWrnFileExists (msant_strings[33])
#define mNoExcitation (msant_strings[34])
#define mWrnSubstHeight (msant_strings[35])
#define mAckNoCoupling (msant_strings[36])
#define mWrnFreqNeg (msant_strings[37])
#define mErInvalid (msant_strings[38])
#define mOnlyOneExcit (msant_strings[39])
#define mNoMoreWarn (msant_strings[40])
#define fNotSegmented (msant_strings[41])
#define fWrnH_GT_len (msant_strings[42])
#define fNoMem (msant_strings[43])
#define fBusyZMatDim (msant_strings[44])
#define mGeoTooBig (msant_strings[45])
#define fBusyMatMult (msant_strings[46])
#define mBusySolveEqn (msant_strings[47])
#define fMaxBorderVolt (msant_strings[48])
#define fImpedance (msant_strings[49])
#define fAdmittance (msant_strings[50])
#define mNeedBordVolt (msant_strings[51])
#define mWrnOverlap (msant_strings[52])
#define fBusyCplAdm (msant_strings[53])
#define mSelectExcit (msant_strings[54])
#define mBusyCalcField (msant_strings[55])
#define mBusyDrawField (msant_strings[56])
#define mBusyCalc (msant_strings[57])
#define nAbrtNticeName (msant_strings[58])
#define mWrnPGListNotEty (msant_strings[59])
#define fWrnActionPars (msant_strings[60])
#define fPosData (msant_strings[61])
#define fWrnActionEv (msant_strings[62])
#define fValidateMess (msant_strings[63])
#define fWrnDimension (msant_strings[64])
#define fWrnPars (msant_strings[65])
#define fWrnEqnSolSngErr (msant_strings[66])
#define fWrnEqnSolInacc (msant_strings[67])
#define fWrnUnknown (msant_strings[68])
#define nEqnSolCGEFS (msant_strings[69])
#define nEqnSolSGEFS (msant_strings[70])
#define mWrnFreqLow  (msant_strings[71])
#define mModulusShown (msant_strings[72])
#define mChrgDensShown (msant_strings[73])

#define N_MESSAGES 74

extern char *msant_strings[];

/* read \002 terminated strings (except the last one which is
 * \000 terminated from the string 'raw'
 * and put pointers to them into the msant_strings[] array
 * changing the terminating character to \000
 *
 * returns 0 upon success (raw contains exactly N_MESSAGES strings)
 *        the difference of N_MESSAGES and the messages read (if positive)
 *        -1 if there are more messages than N_MESSAGES.
 * 
 */

int ReadStrings(/* String raw */);

/* These are the strings that cannot be initialized by the
 * resource mechanism
 */
#include "fixed_strings.h"

#endif
