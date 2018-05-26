/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 

/** @file
 *
 *  driver interface header for demod device. ( used only within kdriver )
 *	demod device will teach you how to make device driver with new platform.
 *
 *  @author		jeongpil.yun (jeongpil.yun@lge.com)
 *  @version	1.0
 *  @date		2013.07.31
 *
 *  @addtogroup lg1150_demod
 *	@{
 */


/*****************************************************************************
**
**  Name:demod_analog_m14a0.h
**
**  Description:    ABB tuning process control and lock status monitoring.
**
**  Functions
**  Implemented:   char*	DEMOD_M14B0_ANALOG_WhatAudioSystem
**                  int DEMOD_M14B0_ANALOG_Demod_Open
**                  int DEMOD_M14B0_ANALOG_Demod_Open
**                  int DEMOD_M14B0_ANALOG_Set_IF_Frq
**                  int DEMOD_M14B0_ANALOG_Set_AftRange
**                  int DEMOD_M14B0_ANALOG_ChangeAudioSystem
**                  int DEMOD_M14B0_ANALOG_SetAudioSystem
**                  int DEMOD_M14B0_ANALOG_SetAudioSystem
**                  int DEMOD_M14B0_ANALOG_SetLowIFinput
**                  LX_ADEMOD_VideoStandard DEMOD_M14B0_ANALOG_GetVideoStandard
**                  int DEMOD_M14B0_ANALOG_SetSIFMode
**                  int DEMOD_M14B0_ANALOG_SetAGCTOP
**                  int DEMOD_M14B0_ANALOG_SmartTune
**                  int DEMOD_M14B0_ANALOG_CheckVideoCarrier
**                  int DEMOD_M14B0_ANALOG_CheckVideoSync
**                  int DEMOD_M14B0_ANALOG_ControlOutput
**                  int DEMOD_M14B0_ANALOG_Get_FreqOffset
**                  int DEMOD_M14B0_ANALOG_ChangeFilter4SIF
**                  int DEMOD_M14B0_ANALOG_CarrierlockScan
**                  int DEMOD_M14B0_ANALOG_SoftReset
**                  int DEMOD_M14B0_ANALOG_Set_CvbsDecCtrl
**                  int DEMOD_M14B0_ANALOG_Set_CvbsRateConversion
**                  int DEMOD_M14B0_ANALOG_Set_HighCvbsRateOffset
**                  int DEMOD_M14B0_ANALOG_Set_SifCtrl
**                  int DEMOD_M14B0_ANALOG_Set_SifPathCtrl
**                  int DEMOD_M14B0_ANALOG_Set_SpectrumInv
**                  int DEMOD_M14B0_ANALOG_Obtaining_Signal_Lock
**                  int DEMOD_M14B0_ANALOG_Monitoring_Signal_Lock
**
**                  int DEMOD_M14B0_ANALOG_Dbg_Get_RegDump
**                  int DEMOD_M14B0_ANALOG_Dbg_Set_RegisterValue
**                  int DEMOD_M14B0_ANALOG_Dbg_SmartTune
**                  int DEMOD_M14B0_ANALOG_Dbg_Get_RegisterValue
**                  int DEMOD_M14B0_ANALOG_Dbg_TestFunctions
**                  int DEMOD_M14B0_ANALOG_OptimizingTunerInBrazil
**                  int DEMOD_M14B0_ANALOG_OptimizingTunerInBrazil
**
**  References:    
**
**  Exports:       
**
**  Dependencies:   demod_impl.h for system configuration data.
**				 demod_analog_m14a0.h
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#ifndef	_DEMOD_ANALOG_M14B0_H_
#define	_DEMOD_ANALOG_M14B0_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "demod_impl.h"

#include "ademod_common_Menu.h"
#include "ademod_common_Presets.h"
#include "ademod_common_device.h"
#include "ademod_common_userdef.h"
#include "ademod_common_UserUtilities.h"

#include "ademod_m14_demod.h"
#include "ademod_m14_hostI2C.h"
#include "ademod_m14_Presets.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define VIDEO_SYNC_LOCK_SELECTED_FOR_FINAL_LOCK  0

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/

extern char* DEMOD_M14B0_ANALOG_WhatAudioSystem(LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem);
extern int DEMOD_M14B0_ANALOG_Demod_Open(UINT32 chipDevID, UINT32 handleI2C,  Handle_t *phDevice);
extern int DEMOD_M14B0_ANALOG_Init(Handle_t phDevice, LX_DEMOD_RF_MODE_T rfMode);
extern int DEMOD_M14B0_ANALOG_Set_IF_Frq(Handle_t phDevice, UINT32 pllClkSource, UINT32 ifFrq);
extern int DEMOD_M14B0_ANALOG_Set_AftRange(Handle_t phDevice, LX_DEMOD_ANALOG_ATFRANGE_PARAM_T aftRange);
extern int DEMOD_M14B0_ANALOG_Set_SifPathCtrl(Handle_t phDevice, UINT32 pllClkSource, LX_DEMOD_ANALOG_SIFPATH_SEL_T sel );
extern int DEMOD_M14B0_ANALOG_ChangeAudioSystem(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam);
extern int DEMOD_M14B0_ANALOG_SetAudioSystem(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam);
extern int DEMOD_M14B0_ANALOG_CheckIFLevel(Handle_t phDevice,  UINT32 *pIFLevel);
extern int DEMOD_M14B0_ANALOG_SetLowIFinput(Handle_t phDevice, UINT32 sample_rate, int lif_freq, UINT8 isdefault);
extern LX_ADEMOD_VideoStandard DEMOD_M14B0_ANALOG_GetVideoStandard(LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem);
extern int DEMOD_M14B0_ANALOG_SetSIFMode(Handle_t phDevice);
extern int DEMOD_M14B0_ANALOG_SetAGCTOP(Handle_t phDevice, UINT32 frequency, LX_DEMOD_RF_MODE_T rfMode );
extern int DEMOD_M14B0_ANALOG_SmartTune(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam,  BOOLEAN bForce);
extern int DEMOD_M14B0_ANALOG_CheckVideoCarrier(Handle_t phDevice, LX_DEMOD_LOCK_STATE_T *pCarrierLockStatus);
extern int DEMOD_M14B0_ANALOG_ControlOutput(Handle_t phDevice,  BOOLEAN bOutputON);
extern int DEMOD_M14B0_ANALOG_Get_FreqOffset(DEMOD_CTX_T *pDemodInfo, SINT32 *pFreqOffset);
extern int DEMOD_M14B0_ANALOG_ChangeFilter4SIF(Handle_t	phDevice,  LX_DEMOD_ANALOG_SIF_SOUNDSYSTEM_T audioSystem);
extern int DEMOD_M14B0_ANALOG_CarrierlockScan(DEMOD_CTX_T *pDemodInfo, LX_DEMOD_FLAG_T ResetOn, LX_DEMOD_FLAG_T *pScanProcessing, LX_DEMOD_LOCK_STATE_T *pScanLock);
extern int DEMOD_M14B0_ANALOG_SoftReset(Handle_t	phDevice);
extern int DEMOD_M14B0_ANALOG_Set_CvbsRateConversion(Handle_t	phDevice, UINT32 pllClkSource);
extern int DEMOD_M14B0_ANALOG_Set_HighCvbsRateOffset(Handle_t	phDevice, BOOLEAN bInitOffsetHigh);
extern int DEMOD_M14B0_ANALOG_Set_CvbsDecCtrl(Handle_t phDevice, LX_DEMOD_ANALOG_CVBSDEC_SEL_T sel );
extern int DEMOD_M14B0_ANALOG_Set_ClampingCtrl(Handle_t phDevice, UINT8 clampCount, UINT16 stepsize);
extern int DEMOD_M14B0_ANALOG_Set_SifCtrl(Handle_t phDevice, UINT32 pllClkSource);
extern int DEMOD_M14B0_ANALOG_Set_SpectrumInv(Handle_t phDevice, BOOLEAN bInv, BOOLEAN bforce);
extern int DEMOD_M14B0_ANALOG_ClampingCtrlEnable(Handle_t phDevice, BOOLEAN Enable);
extern int DEMOD_M14B0_ANALOG_Obtaining_Signal_Lock(DEMOD_CTX_T *pDemodInfo);
extern int DEMOD_M14B0_ANALOG_Monitoring_Signal_Lock(DEMOD_CTX_T *pDemodInfo);

extern int DEMOD_M14B0_ANALOG_Dbg_Get_RegDump(Handle_t phDevice);
extern int DEMOD_M14B0_ANALOG_Dbg_Set_RegisterValue(Handle_t phDevice,  UINT32 RegAddr, UINT32 RegSize, UINT32 RegData);
extern int DEMOD_M14B0_ANALOG_Dbg_SmartTune(LX_DEMOD_SMARTTUNE_PARAM_T *pSmartTuneParam);
extern int DEMOD_M14B0_ANALOG_Dbg_Get_RegisterValue(Handle_t phDevice, UINT32 RegAddr, UINT32 RegSize, UINT32 *pRegData);
extern int DEMOD_M14B0_ANALOG_Dbg_TestFunctions(Handle_t phDevice, UINT32 argc, UINT32 *val);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _DEMOD_ANALOG_M14B0_H_ */

/** @} */
