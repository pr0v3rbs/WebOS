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
**  Name: ademod_common_legacy.h
**
**  Description:   ABB Legacy Demodulation block.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  
**
**  References:    
**
**  Exports:        N/A
**
**  Dependencies:   ademod_common.h   - for basic FM types,
**				  ademod_common_video.h    - for LX_ADEMOD_VideoProc type.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
#ifndef _ADEMOD_COMMON_LEGACY_H_
#define _ADEMOD_COMMON_LEGACY_H_

#include "ademod_common.h"
#include "ademod_common_video.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  DAC and SYNC options
*/
typedef enum LX_ADEMOD_DAC_Options_tag {
	LX_ADEMOD_DAC_OPTION_INSTAB_EN				= 0x00000001L,
	LX_ADEMOD_DAC_OPTION_INSTAB_IF_EN			= 0x00000002L,
	LX_ADEMOD_DAC_OPTION_INSTAB_RF_EN			= 0x00000004L,
	LX_ADEMOD_DAC_OPTION_INSTAB_CH1_EN			= 0x00000008L,
	LX_ADEMOD_DAC_OPTION_DITHER_RF_EN			= 0x00000010L,
	LX_ADEMOD_DAC_OPTION_DITHER_IF_EN			= 0x00000020L,
	LX_ADEMOD_DAC_OPTION_DITHER_EN				= 0x00000040L,
	LX_ADEMOD_DAC_OPTION_DITHER_AUD_EN			= 0x00000080L,
	LX_ADEMOD_DAC_OPTION_VID_DEM_EN			= 0x00000100L,
//	LX_ADEMOD_DAC_OPTION_AUD_DEM_EN			= 0x00000200L,
	LX_ADEMOD_DAC_OPTION_IF_DEM_EN				= 0x00000400L,
	LX_ADEMOD_SYNC_OPTION_EN					= 0x00000800L,
	LX_ADEMOD_SYNC_OPTION_SRCSEL_EN			= 0x00001000L,
	LX_ADEMOD_SYNC_OPTION_VSYNC_SLICER_EN		= 0x00002000L,
	LX_ADEMOD_SYNC_OPTION_SYNC_HEIGHT_NBP		= 0x00004000L,
	LX_ADEMOD_DAC_OPTION_POSTSAW_EN			= 0x00008000L,
	LX_ADEMOD_SYNC_OPTION_SYNC_AGCMASKOVR_EN	= 0x00010000L,
	LX_ADEMOD_SYNC_OPTION_VSYNC_OUT_EN			= 0x00020000L,
} LX_ADEMOD_DAC_Options;


#if defined( __cplusplus )
}
#endif

#endif //_ADEMOD_COMMON_LEGACY_H_
