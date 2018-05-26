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
**  Name: ademod_common_audio.h
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
**  Dependencies:  ademod_common.h   - for basic FM types,
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
*****************************************************************************/
#ifndef _ADEMOD_COMMON_AUDIO_H_
#define _ADEMOD_COMMON_AUDIO_H_

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

#define LX_ADEMOD_UFLAG_AUD_FREQ_USR		  (0x00000001L) // audio frequency user update 
#define LX_ADEMOD_UFLAG_AUD_GAIN_USR		  (0x00000002L) // audio gain user update 
#define LX_ADEMOD_UFLAG_AUD_CLIP_USR		  (0x00000004L) // audio clip user update 
#define LX_ADEMOD_UFLAG_AUD_OFFS_USR		  (0x00000008L) // audio offset user update 
#define LX_ADEMOD_UFLAG_AUD_SIFBW_USR		  (0x00000010L) // SIF bandwidth user update 
#define LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR		  (0x00000020L) // SIF frequency user update 
#define LX_ADEMOD_UFLAG_AUD_MIX_USR		  (0x00000040L) // Up/Down mixer update 
#define LX_ADEMOD_UFLAG_AUD_AGC_TARG_USR	  (0x00000080L) // audio AGC target
#define LX_ADEMOD_UFLAG_AUD_AGC_GAINTHR_USR  (0x00000100L) // audio AGC gain threshold
#define LX_ADEMOD_UFLAG_AUD_AGC_GAINPRES_USR (0x00000100L) // audio AGC gain preset
#define LX_ADEMOD_UFLAG_AUD_DAC_INSTTHR_USR  (0x00000200L) // audio DAC instability threshold
#define LX_ADEMOD_UFLAG_AUD_OPTIONS_USR      (0x00000400L) // options
#define LX_ADEMOD_UFLAG_AUD_MODE_USR         (0x00000800L) // audio mode
#define LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR     (0x00001000L) // mono frequency user update 
#define LX_ADEMOD_UFLAG_AUD_DEVIATION_USR    (0x00002000L) // deviation 
#define LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR  (0x00004000L) // BB sample rate 
#define LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR   (0x00008000L) // Deemphasis 
#define LX_ADEMOD_UFLAG_AUD_MONOGAIN_USR     (0x00010000L) // mono gain
#define LX_ADEMOD_UFLAG_AUD_BUZZDELAY_USR	 (0x00020000L) // audio buzz delay 

//jeongpil.yun for M14
#define LX_ADEMOD_UFLAG_AUD_SMP_RATE_DIV_USR (0x00040000L) // sample rate divider 
#define LX_ADEMOD_UFLAG_AUD_MUTE_USR         (0x00080000L) // audio mute
#define LX_ADEMOD_UFLAG_AUD_MUTE_SPEED_USR   (0x00100000L) // audio mute speed


#define LX_ADEMOD_UFLAG_AUD_GAIN_RC_USR		  (0x02000002L) // audio gain user update 
#define LX_ADEMOD_UFLAG_AUD_CLIP_RC_USR		  (0x04000004L) // audio clip user update 
#define LX_ADEMOD_UFLAG_AUD_OFFS_RC_USR		  (0x08000008L) // audio offset user update 

#if defined( __cplusplus )
}
#endif

#endif  // _ADEMOD_COMMON_AUDIO_H_
