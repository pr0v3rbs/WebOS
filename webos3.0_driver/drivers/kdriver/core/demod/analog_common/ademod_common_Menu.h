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

/*****************************************************************************
**
**  Name: ademod_common_Menu.h
**
**  Description:   
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  
**
**  References:    
**
**  Exports:        N/A
**
**  Dependencies:   ademod_common.h   - for basic FM types,
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/

#ifndef _ADEMOD_COMMON_MENU_H_
#define _ADEMOD_COMMON_MENU_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "ademod_common_demod.h"
#include "ademod_common_SmartTune.h"

//#define DEFAULT_FREQUENCY	(77250000)
extern LX_ADEMOD_Result ADEMOD_Channel_Set(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune, BOOL Update);
extern LX_ADEMOD_Result ADEMOD_Demodulator_Set(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune);
extern LX_ADEMOD_Result ADEMOD_Channel_SetForce(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune);
extern LX_ADEMOD_Result ADEMOD_Demodulator_Set_ChannelScan(Handle_t hDemod, LX_ADEMOD_SmartTuneDescriptor *pSmartTune);


extern LX_ADEMOD_Result KDRV_Dbg_Menu(Handle_t hDemod, UINT32 argc, UINT32 *val);

#ifdef __cplusplus
}
#endif
#endif  //_ADEMOD_COMMON_MENU_H_
