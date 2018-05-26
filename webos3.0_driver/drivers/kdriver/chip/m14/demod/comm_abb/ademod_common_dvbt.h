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
**  Name: ademod_common_dvbt.h
**
**  Description:    ABB DVB-T Demodulation block.
**
**  Functions
**  Implemented:
**
**  References:    
**
**  Exports:        N/A
**
**  Dependencies:   ademod_common.h   - for basic FM types,
**				 ademod_common_video.h    - for FM_VideoProc type.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
#ifndef _ADEMOD_COMMON_DVBT_H_
#define _ADEMOD_COMMON_DVBT_H_

#include "ademod_common.h"
#include "ademod_common_video.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  DVBT Feature flags:
**  These flags are for features that require on/off control
*/
#define LX_ADEMOD_FFLAG_DVB_TS_SERIAL_EN	(0x00000001L)
#define LX_ADEMOD_FFLAG_DVB_TS_SLAVE_EN	(0x00000002L)
#define LX_ADEMOD_FFLAG_DVB_CLK_GATING_EN	(0x00000004L)
#define LX_ADEMOD_FFLAG_DVB_K_STUFF_EN		(0x00000008L)
#define LX_ADEMOD_FFLAG_DVB_LSB_FIRST_EN	(0x00000010L)
#define LX_ADEMOD_FFLAG_DVB_POS_POL_EN		(0x00000020L)
#define LX_ADEMOD_FFLAG_DVB_SYNC_1CLK_EN	(0x00000040L)
#define LX_ADEMOD_FFLAG_DVB_FFT_FIX_EN		(0x00000080L)
#define LX_ADEMOD_FFLAG_DVB_SYNC_GATE_EN	(0x00000100L)
#define LX_ADEMOD_FFLAG_DVB_TSVAL_POL_EN	(0x00000200L)
#define LX_ADEMOD_FFLAG_DVB_TSSYNC_POL_EN	(0x00000400L)
#define LX_ADEMOD_FFLAG_DVB_TSERR_POL_EN	(0x00000800L)
#define LX_ADEMOD_FFLAG_DVB_TSKOUT_POL_EN	(0x00001000L)
#define LX_ADEMOD_FFLAG_DVB_BDI_1_RME		(0x00002000L)
#define LX_ADEMOD_FFLAG_DVB_BDI_2_RME		(0x00004000L)
#define LX_ADEMOD_FFLAG_DVB_CHC_1_RME		(0x00008000L)
#define LX_ADEMOD_FFLAG_DVB_CHC_2_RME		(0x00010000L)
#define LX_ADEMOD_FFLAG_DVB_FFT_INP_RME	(0x00020000L)
#define LX_ADEMOD_FFLAG_DVB_FFT_WRK_RME	(0x00040000L)
#define LX_ADEMOD_FFLAG_DVB_PPM_RME		(0x00080000L)
#define LX_ADEMOD_FFLAG_DVB_REED_SOL_RME	(0x00100000L)
#define LX_ADEMOD_FFLAG_DVB_SDI_RME		(0x00200000L)
#define LX_ADEMOD_FFLAG_DVB_SYR_RME		(0x00400000L)
#define LX_ADEMOD_FFLAG_DVB_VIT1_RME		(0x00800000L)
#define LX_ADEMOD_FFLAG_DVB_VIT2_RME		(0x01000000L)
#define LX_ADEMOD_FFLAG_DVB_BOX_BRIDGE		(0x02000000L)
#define LX_ADEMOD_FFLAG_DVB_VIT_BER_EN		(0x04000000L)
#define LX_ADEMOD_FFLAG_DVB_TS_EN			(0x08000000L)
#define LX_ADEMOD_FFLAG_DVB_TS_CLKINV_EN	(0x10000000L)

/*
**  Update flags:
**  USR flags indicate that parameter has been modified from factory default to user preference.
**  These flags are used to assure presidence of user settings over the factory defaults.
**  They will be cleared when appropriate Factory_Set function is called.
**  NOTE: Do not use flag 0x80000000 because it is taken by LX_ADEMOD_UFLAG_UPDATE
*/

#define LX_ADEMOD_UFLAG_LOWIF_GAIN_USR		(0x00000001L) // low IF gain
#define LX_ADEMOD_UFLAG_LOWIF_OFFS_USR		(0x00000002L) // low IF offset

/*
**  DVB-T Demodulator
*/
typedef struct LX_ADEMOD_DvbtDemod_tag
{
	UINT32		flags;
	UINT32		update;
	UINT32		factory;
}  LX_ADEMOD_DvbtDemod, *pLX_ADEMOD_DvbtDemod;


#if defined( __cplusplus )
}
#endif

#endif  //_ADEMOD_COMMON_DVBT_H_
