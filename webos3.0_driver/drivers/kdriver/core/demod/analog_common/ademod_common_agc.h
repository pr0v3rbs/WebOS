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
**  Name: ademod_common_agc.h
**
**  Description:    ABB Automatic Gain Control (AGC) block.
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
*****************************************************************************/
#ifndef _ADEMOD_COMMOM_AGC_H_
#define _ADEMOD_COMMOM_AGC_H_

#include "ademod_common.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  Update flags:
**  USR flags indicate that parameter has been modified from factory default to user preference.
**  These flags are used to assure presidence of user settings over the factory defaults.
**  They will be cleared when appropriate Factory_Set function is called.
**  NOTE: Do not use flag 0x80000000 because it is taken by LX_ADEMOD_UFLAG_UPDATE
*/
#define LX_ADEMOD_UFLAG_AGC_MODTYPE_USR		(0x00000001L) 
#define LX_ADEMOD_UFLAG_AGC_LINE_USR			(0x00000002L) 
#define LX_ADEMOD_UFLAG_AGC_GAIN_USR			(0x00000004L) 
#define LX_ADEMOD_UFLAG_AGC_FLAG_USR			(0x00000008L) 
#define LX_ADEMOD_UFLAG_AGC_PARM_USR			(0x00000010L) 
#define LX_ADEMOD_UFLAG_AGC_RFEN_USR			(0x00000020L) 
#define LX_ADEMOD_UFLAG_AGC_IFEN_USR			(0x00000040L) 
#define LX_ADEMOD_UFLAG_AGC_EXT_USR			(0x00000080L) 
#define LX_ADEMOD_UFLAG_AGC_OPENDRAIN			(0x00000100L) 
#define LX_ADEMOD_UFLAG_AGC_POSTSAW_USR		(0x00000200L) 
#define LX_ADEMOD_UFLAG_AGC_BNDW_USR			(0x00000400L) 
#define LX_ADEMOD_UFLAG_AGC_LPD_SPEED_USR	    (0x00000800L) 
#define LX_ADEMOD_UFLAG_AGC_LPD_DECAY_USR	    (0x00001000L) 
#define LX_ADEMOD_UFLAG_AGC_LPD_FASTDECAY_USR	(0x00002000L) 
#define LX_ADEMOD_UFLAG_AGC_LPD_DECAYWIN_USR	(0x00004000L) 
#define LX_ADEMOD_UFLAG_AGC_LPD_DECAYTHRSH_USR (0x00008000L) 
#define LX_ADEMOD_UFLAG_AGC_DECFACTOR_USR		(0x00010000L) 
#define LX_ADEMOD_UFLAG_AGC_INSTAB_USR			(0x00020000L) 
#define LX_ADEMOD_UFLAG_AGC_INSTAB_GAIN_USR	(0x00040000L) 
#define LX_ADEMOD_UFLAG_AGC_LPFBNDW_USR		(0x00080000L) 
#define LX_ADEMOD_UFLAG_AGC_LPFSPEED_USR		(0x00100000L) 
#define LX_ADEMOD_UFLAG_AGC_BACKOFF_USR		(0x00200000L) 
#define LX_ADEMOD_UFLAG_AGC_RFGAIN_USR			(0x00400000L) 
#define LX_ADEMOD_UFLAG_AGC_IFGAIN_USR			(0x00800000L) 
#define LX_ADEMOD_UFLAG_AGC_OPTIONS_USR		(0x01000000L)   //jeongpil.yun for M14
#define LX_ADEMOD_UFLAG_AGC_MIXMODE_USR		(0x02000000L)   //jeongpil.yun for M14
//#define LX_ADEMOD_UFLAG_AGC_DIGAGCEN_USR		(0x01000000L) 
//#define LX_ADEMOD_UFLAG_AGC_DIGAGCBYPASS_USR	(0x02000000L) 
//#define LX_ADEMOD_UFLAG_AGC_DIGAGCINP_USR	(0x04000000L) 
#define LX_ADEMOD_UFLAG_AGC_RFDITHER_USR		(0x08000000L) 
#define LX_ADEMOD_UFLAG_AGC_IFDITHER_USR		(0x10000000L) 
//#define LX_ADEMOD_UFLAG_AGC_INSTAB_COMSIG_USR	(0x20000000L) 
//#define LX_ADEMOD_UFLAG_AGC_BITSPLIT_USR		(0x40000000L) 
#define LX_ADEMOD_UFLAG_AGC_SMP_RATE_USR		(0x40000000L) 


/*
**  Feature flags:
**  These flags are for features that require on/off control
*/
#define LX_ADEMOD_FFLAG_AGCMOD_TYPE			(0x00000001L) // 1 = positive modulation, 0 = negative modulation
#define LX_ADEMOD_FFLAG_AGC_SOFTRESET			(0x00000002L)
#define LX_ADEMOD_FFLAG_CVBSAGC_EN				(0x00000004L)
#define LX_ADEMOD_FFLAG_CVBSAGC_INPSLCT		(0x00000008L)
#define LX_ADEMOD_FFLAG_CVBSAGC_ACCUMEN		(0x00000010L)
#define LX_ADEMOD_FFLAG_AGC_CNTL_GAINSEL		(0x00000020L)
#define LX_ADEMOD_FFLAG_AGCAUD_EN				(0x00000040L)
#define LX_ADEMOD_FFLAG_AGCAUD_ERSET_ACC		(0x00000080L)
#define LX_ADEMOD_FFLAG_AGCRF_EN				(0x00000100L)
#define LX_ADEMOD_FFLAG_AGCIF_EN				(0x00000200L)
#define LX_ADEMOD_FFLAG_AGCRF_GAIN_INV_EN		(0x00000400L)
#define LX_ADEMOD_FFLAG_AGCIF_GAIN_INV_EN		(0x00000800L)
#define LX_ADEMOD_FFLAG_AGC_EXTDSB				(0x00001000L)
#define LX_ADEMOD_FFLAG_AGC_OPENDRAIN			(0x00002000L)
#define LX_ADEMOD_FFLAG_AGC_POSTSAW_EN			(0x00004000L)
#define LX_ADEMOD_FFLAG_AGC_INSTAB_EN			(0x00008000L)
#define LX_ADEMOD_FFLAG_AGC_INSTAB_COMSIG_EN	(0x00010000L) // Qn and AGC instability values are 0-ANDed, 1-ORed
#define LX_ADEMOD_FFLAG_AGC_LOWIF_EN			(0x00020000L)
#define LX_ADEMOD_FFLAG_DIGAGCMOD_TYPE			(0x00040000L) // 1 = positive modulation, 0 = negative modulation
#define LX_ADEMOD_FFLAG_DIGAGC_DSB				(0x00080000L) // Digital AGC disabled
#define LX_ADEMOD_FFLAG_DIGAGCRF_EN			(0x00100000L) // PostSAW RF AGC Enable
#define LX_ADEMOD_FFLAG_DIGAGCIF_EN			(0x00200000L) // PostSAW IF AGC Enable
#define LX_ADEMOD_FFLAG_AGC_BITSPLIT_EN		(0x00400000L) // enable/disable the bit-splitter for the 1-bit sigma-delta modulator: 0 - disabled, 1 - enabled
#define LX_ADEMOD_FFLAG_AGCAUD_SDM_PWROFF_EN	(0x00800000L) // sigma-delta modulator 1 - power off, 0 - power on
//#define LX_ADEMOD_FFLAG_AGC_FASTMODE_EN		(0x01000000L) // Fast mode Enable


#define LX_ADEMOD_FFLAG_DIGAGC_EN			LX_ADEMOD_FFLAG_CVBSAGC_EN
#define LX_ADEMOD_FFLAG_DIGAGC_INPSLCT		LX_ADEMOD_FFLAG_CVBSAGC_INPSLCT
#define LX_ADEMOD_FFLAG_DIGAGC_ACCUMEN		LX_ADEMOD_FFLAG_CVBSAGC_ACCUMEN
#define LX_ADEMOD_FFLAG_DIGAGC_GAINSEL		LX_ADEMOD_FFLAG_AGC_CNTL_GAINSEL


/* M14 */  //jeongpil.yun for M14
#define LX_ADEMOD_FFLAG_DIGAGC_HYBRID_EN		(0x02000000L) // Hybrid mode enabled
#define LX_ADEMOD_FFLAG_DIGAGC_VSYNCTILT_EN	(0x01000000L) // VSync Tilt reduction
#define LX_ADEMOD_FFLAG_DIGAGC_LVLDEP_EN		(0x04000000L) // Level-dependent AGC enabled
#define LX_ADEMOD_FFLAG_AGC_QNOISEFILTER_EN	(0x08000000L) // Quantization Noise Filter enabled



/*
**  AGC context
*/
typedef struct LX_ADEMOD_AGC_tag
{
	UINT32		flags;
	UINT32		update;
	UINT32		factory;
	UINT32		options;
	UINT32		mode;
	UINT32		input_mode;
}  LX_ADEMOD_AGC, *pLX_ADEMOD_AGC;


#if defined( __cplusplus )
}
#endif

#endif  //_ADEMOD_COMMOM_AGC_H_
