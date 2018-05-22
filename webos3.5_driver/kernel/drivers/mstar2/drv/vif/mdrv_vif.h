////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2009 MStar Semiconductor, Inc.
//
// You can redistribute it and/or modify it under the terms of the GNU General Public
// License version 2 as published by the Free Foundation. This program is distributed
// in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_vif.h
/// @brief  VIF Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_VIF_H_
#define _MDRV_VIF_H_

//#include <asm-mips/types.h>
#include "mdrv_vif_st.h"
//#include "mdrv_vif_io.h"
#include "mdrv_types.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//Duplicate from mdrv_vif_io.h for preventing compile error
#define VIF_IOC_MAGIC                'v'

#define MDRV_VIF_INIT                       _IOR(VIF_IOC_MAGIC, 0, VIF_Init_Info_st)
#define MDRV_VIF_SETCLOCK                   _IOR(VIF_IOC_MAGIC, 1, BOOL)
#define MDRV_VIF_RESET                       _IO(VIF_IOC_MAGIC, 2)
#define MDRV_VIF_HANDLER                    _IOR(VIF_IOC_MAGIC, 3, BOOL)
#define MDRV_VIF_SET_SOUND_SYSTEM           _IOR(VIF_IOC_MAGIC, 4, VIFSoundSystem)
#define MDRV_VIF_SET_IF_FREQ                _IOR(VIF_IOC_MAGIC, 5, IfFrequencyType)
#define MDRV_VIF_READ_CR_FOE                _IOW(VIF_IOC_MAGIC, 6, U8)
#define MDRV_VIF_READ_CR_LOCK_STATUS        _IOW(VIF_IOC_MAGIC, 7, U8)
#define MDRV_VIF_BYPASS_DBB_AUDIO_FLT      _IOR(VIF_IOC_MAGIC, 8, BOOL)
#define MDRV_VIF_SET_FREQ_BAND              _IOR(VIF_IOC_MAGIC, 9, FrequencyBand)
#define MDRV_VIF_GET_INPUT_LEVEL_INDICATOR  _IOW(VIF_IOC_MAGIC, 10, BOOL)
#define MDRV_VIF_SET_PARAMETER             _IOWR(VIF_IOC_MAGIC, 11, VIF_Para_Info_st)
#define MDRV_VIF_SHIFTCLK               _IOR(VIF_IOC_MAGIC, 12, BYTE)
#define VIF_IOC_MAXNR                   13


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//void MDRV_VIF_Version(void);//TODO: check if needed , Luke 20110127
void MDRV_VIF_SetClock(BOOL bEnable);
void MDRV_VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen);
void MDRV_VIF_Reset(void);
void MDRV_VIF_Handler(BOOL bAutoScan);
void MDRV_VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem);
void MDRV_VIF_SetIfFreq(IfFrequencyType u16IfFreq);

void MDRV_VIF_BypassDBBAudioFilter(BOOL bEnable);
void MDRV_VIF_SetFreqBand(FrequencyBand u8FreqBand);

U8 MDRV_VIF_Read_CR_FOE(void);
U8 MDRV_VIF_Read_CR_LOCK_STATUS(void);
BOOL MDRV_VIF_GetInputLevelIndicator(void);

BOOL MDRV_VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen);
void MDRV_VIF_ShiftClk(BYTE VifShiftClk);

void MDRV_VIF_WriteByte(U32 u32Reg, U8 u8Val );
U8 MDRV_VIF_ReadByte(U32 u32Reg );


#endif // _MDRV_VIF_H_
