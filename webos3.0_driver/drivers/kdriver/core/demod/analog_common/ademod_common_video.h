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
**  Name: ademod_common_video.h
**
**  Description:    ABB Video Processing block.
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
#ifndef _ADEMOD_COMMON_VIDEO_H_
#define _ADEMOD_COMMON_VIDEO_H_

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

#define LX_ADEMOD_UFLAG_VID_BNDW_USR				(0x00000001L) // video bandwidth user update
#define LX_ADEMOD_UFLAG_INP_FREQ_USR				(0x00000002L) // video frequency user update 
#define LX_ADEMOD_UFLAG_SMP_RATE_USR				(0x00000004L) // sample rate user update 
#define LX_ADEMOD_UFLAG_PIN_RANG_USR				(0x00000008L) // pull-in range user update 
#define LX_ADEMOD_UFLAG_VSB_ENRG_USR				(0x00000010L) // VSB energy user update 
#define LX_ADEMOD_UFLAG_CTF_MAIN_USR				(0x00000020L) // cut off user update 
#define LX_ADEMOD_UFLAG_CAR_OFFS_USR				(0x00000040L) // carrier offset user update 
#define LX_ADEMOD_UFLAG_VID_FR_LOOPSPEED_USR		(0x00000080L) // FR loop speed
#define LX_ADEMOD_UFLAG_VID_FR_LOOPGAIN_USR		(0x00000100L) // FR loop gain
#define LX_ADEMOD_UFLAG_VID_PH_LOOPSPEED_USR		(0x00000200L) // PH loop speed
#define LX_ADEMOD_UFLAG_VID_PH_LOOPGAIN_USR		(0x00000400L) // PH loop gain
#define LX_ADEMOD_UFLAG_VID_LOCKDETERR_USR			(0x00000800L) // CR lock detection errors
#define LX_ADEMOD_UFLAG_VID_LOCKDETLINES_USR		(0x00001000L) // CR lock detection lines
#define LX_ADEMOD_UFLAG_VID_UNLOCKDETERR_USR		(0x00002000L) // CR unlock detection errors
#define LX_ADEMOD_UFLAG_VID_UNLOCKDETLINES_USR		(0x00004000L) // CR unlock detection lines
#define LX_ADEMOD_UFLAG_VID_MUTELOCKDETERR_USR		(0x00008000L) // CR mute lock detection errors
#define LX_ADEMOD_UFLAG_VID_MUTELOCKDETLINES_USR	(0x00010000L) // CR mute lock detection lines
#define LX_ADEMOD_UFLAG_VID_MUTEUNLOCKDETERR_USR	(0x00020000L) // CR mute unlock detection errors
#define LX_ADEMOD_UFLAG_VID_MUTEUNLOCKDETLINES_USR	(0x00040000L) // CR mute unlock detection lines
#define LX_ADEMOD_UFLAG_VID_VIRTUALFS_USR			(0x00080000L) // virtual FS
#define LX_ADEMOD_UFLAG_VID_PF1TINK_USR			(0x00100000L) // PF1 Tink
#define LX_ADEMOD_UFLAG_VID_PF2TINK_USR			(0x00200000L) // PF2 Tink
#define LX_ADEMOD_UFLAG_VID_PF2TINKINV_USR			(0x00400000L) // PF2 Tink Inv
#define LX_ADEMOD_UFLAG_OUT_FREQ_USR				(0x00800000L) // output center frequency user update 
#define LX_ADEMOD_UFLAG_VID_OVERMODPHASEREV_USR	(0x01000000L) // overmodulation phase reversal 
#define LX_ADEMOD_UFLAG_VID_CRRESET_USR			(0x02000000L) // CR reset 
#define LX_ADEMOD_UFLAG_VID_ROTATORFLIP_USR		(0x04000000L) // Rotator Flip 
#define LX_ADEMOD_UFLAG_VID_LPDNBND_OFFS_USR		(0x08000000L) // Narrowband AGC Piece-wise LPF offset (A2)
#define LX_ADEMOD_UFLAG_VID_LPDNBND_MODE_USR		(0x10000000L) // Narrowband AGC mode
#define LX_ADEMOD_UFLAG_VID_OFFS_TWEAK_USR			(0x20000000L) // Video offset tweak
#define LX_ADEMOD_UFLAG_VID_LOWIFBNDW_USR			(0x40000000L) // Low IF bandwidth

/*
**  Update1 flags:
*/
#define LX_ADEMOD_UFLAG_CR_OVERMOD_DISWEIGHT_USR	(0x00000001L) // overmodulation disable weight
#define LX_ADEMOD_UFLAG_CR_OVERMOD_USR				(0x00000002L) // overmodulation parameters
#define LX_ADEMOD_UFLAG_SMP_RATE_DIV_USR			(0x00000004L) // sample rate divider 


/*
**  Feature flags:
**  These flags are for features that require on/off control
*/
#define LX_ADEMOD_FFLAG_FR1_OPENED_LOOP	(0x00000001L) // FR1 mode is open loop
#define LX_ADEMOD_FFLAG_FR2_OPENED_LOOP	(0x00000002L) // FR2 mode is open loop
#define LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP	(0x00000004L) // DMUX rotator flip
#define LX_ADEMOD_FFLAG_CR_OVERMOD_EN		(0x00000008L) // Carrier Recovery Loop Enable
#define LX_ADEMOD_FFLAG_CR_OVERMOD_MODE	(0x00000010L) // Carrier Recovery Overmodulation Mode
#define LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT	(0x00000020L) // Carrier Recovery Overmodulation Weighting
#define LX_ADEMOD_FFLAG_CR_PH_OVERRIDE		(0x00000040L) // Carrier Recovery Phase Loop Override
#define LX_ADEMOD_FFLAG_SIF_LEAK			(0x00000080L) // Video SIF leak
#define LX_ADEMOD_FFLAG_LEGACY_SIG_MON		(0x00000100L) // Legacy Signal Monitor feature
#define LX_ADEMOD_FFLAG_DVBT_RELOCK		(0x00000200L) // DVBT re-lock feature
#define LX_ADEMOD_FFLAG_44n36				(0x00000400L) // 44MHz vs 36MHz
#define LX_ADEMOD_FFLAG_F5GAIN_1X_N2X		(0x00000800L) // TBD
#define LX_ADEMOD_FFLAG_CR_ALGDIS			(0x00001000L) // 0 == enables CR Reset, 1 == disables CR Reset
#define LX_ADEMOD_FFLAG_DIGAGC_NBND_MODE	(0x00002000L) // Post-SAW AGC mode 0 == wide band, 1 == narrow band

#if defined( __cplusplus )
}
#endif

#endif  //_ADEMOD_COMMON_VIDEO_H_
