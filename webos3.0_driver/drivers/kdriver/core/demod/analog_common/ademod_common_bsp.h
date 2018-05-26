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
**  Name: ademod_common_bsp.h
**
**  Description:    ABB Board Support Package (BSP) module.
**
**  Functions
**  Implemented:      
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
#ifndef _ADEMOD_COMMON_BSP_H_
#define _ADEMOD_COMMON_BSP_H_

#include "ademod_common.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

#define LX_ADEMOD_LOCK_STATUS_CHECK_MAX	(1000)

/*
**  Update flags:
**  USR flags indicate that parameter has been modified from factory default to user preference.
**  These flags are used to assure presidence of user settings over the factory defaults.
**  They will be cleared when appropriate Factory_Set function is called.
**  NOTE: Do not use flag 0x80000000 because it is taken by LX_ADEMOD_UFLAG_UPDATE
*/

#define LX_ADEMOD_UFLAG_CONFIG_OPTIONS_USR		 (0x00000001L) 
#define LX_ADEMOD_UFLAG_CVBS_DAC_OUT_USR		 (0x00000002L)  
#define LX_ADEMOD_UFLAG_SIF_DAC_OUT_USR		 (0x00000004L)  
#define LX_ADEMOD_UFLAG_FAST_AGC_USR			 (0x00000008L)  
#define LX_ADEMOD_UFLAG_OUT_AGC_USR			 (0x00000010L)  
#define LX_ADEMOD_UFLAG_WAND_AGC_USR			 (0x00000020L)  
#define LX_ADEMOD_UFLAG_SMPL_RATE_DIV_USR		 (0x00000040L)  
#define LX_ADEMOD_UFLAG_PHASE_AGC_USR			 (0x00000100L)  

/*
**  Board Support Package (BSP) data
*/
typedef struct LX_ADEMOD_Bsp_tag
{
    UINT32      flags;
	UINT32		update;
	UINT32		factory;
    UINT32      version;
	UINT32		clocks;
	UINT32		i2c_mode;
	UINT32		cvbs_dac_out;
	UINT32		sif_dac_out_dig;
	UINT32		sif_dac_out_sif;
	UINT32		sif_dac_out_mono;
	UINT32		sif_dac_out_mode;
	UINT32		func_oe;
	UINT8		c_vc_tune;
	UINT8		c_ic_tune;
	UINT8		fast_agc;
	UINT8		smpl_rate_div;
	UINT8		out_agc;
	UINT8		wand_agc;
	UINT8		phase_agc;
	UINT8		power_regs[8];
}  LX_ADEMOD_Bsp, *pLX_ADEMOD_Bsp;


#if defined( __cplusplus )
}
#endif

#endif  //_ADEMOD_COMMON_BSP_H_
