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
**  Name: ademod_common_Presets.h
**
**  Description:    ABB Preset settings.
**
**  Functions
**
**  References:     
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

#ifndef _ADEMOD_H14_PRESET_H_
#define _ADEMOD_H14_PRESET_H_

#include "ademod_common_demod.h"
#include "ademod_common_SmartTune.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Preset_Set
**
**  Description:    Initializes demodulator's parameters from user preset.
**
**  Parameters:     hDevice       - Device Handle returned by ADEMOD_Demod_Open,
**					pSmartTune	  - Pointer to Smart Tune descriptor,
**					Section		  - 0- System Init, 1- Video Mode, 2- User registers.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadHandle - Invalid handle
**
******************************************************************************/
extern LX_ADEMOD_Result DEMOD_M14_Analog_Preset_Set(Handle_t hDevice, UINT32 smartTuneFilter, UINT32 Section);


#if defined( __cplusplus )
}
#endif

#endif //_ADEMOD_COMMON_PRESET_H_
