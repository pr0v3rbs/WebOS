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
**  Name: ademod_m14_audio.h
**
**  Description:    ABB M14 Audio Processing block.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Get
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Get
**
**  Dependencies:   ademod_common.h   - for basic FM types,
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
*   31-07-2013   Jeongpil Yun    Initial draft.
*
*****************************************************************************/
#ifndef _ADEMOD_M14_AUDIO_H_
#define _ADEMOD_M14_AUDIO_H_

#include "ademod_common.h"
#include "ademod_common_video.h"
#include "ademod_common_audio.h"


#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  Feature flags:
**  These flags are for features that require on/off control
*/
#define LX_ADEMOD_FFLAG_DEEMPHASIS_EN			(0x00000001L)
#define LX_ADEMOD_FFLAG_FORCEMUTE_EN			(0x00000002L)
#define LX_ADEMOD_FFLAG_AUTOMUTE_FRLOOP_EN		(0x00000004L)
#define LX_ADEMOD_FFLAG_AUTOMUTE_PHLOOP_EN		(0x00000008L)
#define LX_ADEMOD_FFLAG_BSEBND_FILT_EN			(0x00000010L)
#define LX_ADEMOD_FFLAG_SIF_MIX_UP				(0x00000020L)
#define LX_ADEMOD_FFLAG_AGC_CLSLOOP_EN			(0x00000040L)
#define LX_ADEMOD_FFLAG_AGC_HARDCLIP_EN		(0x00000080L)
#define LX_ADEMOD_FFLAG_AGC_SOFTCLIP_EN		(0x00000100L)
#define LX_ADEMOD_FFLAG_AUD_AGC_EN				(0x00000200L)
#define LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN		(0x00000400L)
#define LX_ADEMOD_FFLAG_DCREMOVAL_EN			(0x00000800L)
#define LX_ADEMOD_FFLAG_AUD_REAL_OUT_EN		(0x00001000L)
#define LX_ADEMOD_FFLAG_BUZZ_FIX_EN			(0x00002000L)
#define LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN		(0x00004000L)
#define LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN		(0x00008000L)
#define LX_ADEMOD_FFLAG_BUZZ_MIXFLIP_EN		(0x00010000L)
#define LX_ADEMOD_FFLAG_SIF_DCOTEST_EN			(0x00020000L)
#define LX_ADEMOD_FFLAG_POWER_METER_EN			(0x00040000L)
#define LX_ADEMOD_FFLAG_NOT_VIDSLAVE_STICKY	(0x00080000L)
#define LX_ADEMOD_FFLAG_AUDDAC_INSTAB_EN   	(0x00100000L)
#define LX_ADEMOD_FFLAG_AUDDAC_DITHER_EN		(0x00200000L)
#define LX_ADEMOD_FFLAG_AUDDAC_DEM_EN			(0x00400000L)


/*
**  Audio DAC
*/
typedef struct LX_ADEMOD_AudioDAC_tag
{
	UINT32		ExtraGain;
	int			CH1_gain_SIF;
	int			CH1_gain_MonoFM;
	int			CH1_gain_MonoAM;
	int			CH1_offs;
	int			CH1_cliper;
	UINT32		MixDn;
	UINT32		MixUp;
}  LX_ADEMOD_AudioDAC, *pLX_ADEMOD_AudioDAC;

/*
**  Audio Mono
*/
typedef struct LX_ADEMOD_AudioGain_tag
{
	UINT8		Gain1;
	UINT8		Gain2;
	UINT8		Gain3;
	UINT8		Shift1;
	UINT8		Shift2;
	UINT8		Shift3;
}  LX_ADEMOD_AudioGain, *pLX_ADEMOD_AudioGain;

/*
**  Audio Processor
*/

typedef struct LX_ADEMOD_AudioProc_tag
{
	UINT32		flags;
	UINT32		update;
	UINT32		factory;
	UINT32      audio_std;
	UINT32		options;
	UINT32		mute;
	UINT32      Mode;
	UINT32		Sif_UpperFreq;
	UINT32		Sif_LowerFreq;
	UINT32		MonoSC_UpperFreq;
	UINT32		MonoSC_LowerFreq;
	UINT32		Deemphasis;
	UINT32		Deviation;
	UINT32		BBSampRate;
	UINT32		PwrLoThres;
	UINT32		PwrHiThres;
	UINT32		DCRemovalShift;
	UINT32		MuteOnSpeed;
	UINT32		MuteOffSpeed;
	UINT32		BasebandExtraGain;
	UINT32		DirectDeemphasis;
	UINT32		SifBW;
	UINT32		SC1_freq;
	UINT32		SC1_min;
	UINT32		SC1_max;
	UINT32		BuzzDelayOffs;
	UINT32		BuzzDelayFudge;
	UINT32      Fs_div;
	UINT64		AGCBWTarget;
	UINT64		AGCManualGain;
	UINT64		AGCTarget;
	UINT64		AGCMaxGain;
	UINT64		AGCMinGain;
	INT64		AdcDacFreq;
	INT64		FinalUp_TincInv;
	UINT8		offset_tweak;
	UINT8		mute_speed;
	UINT8		SrcDR; // Source delivery rate
	UINT8		PF1_tinc_prec; // PF1 tinc precision
	UINT8		PF1_delivery_rate_prec; // PF1 delivery rate precision
	UINT8		SIF1_prec; // SIF1 precision
	UINT8		Nom_freq_prec; // nominal frequency precision
	LX_ADEMOD_AudioDAC audio_dac;
	LX_ADEMOD_AudioDAC audio_dac_rc;
	LX_ADEMOD_AudioGain Mono;
}  LX_ADEMOD_AudioProc, *pLX_ADEMOD_AudioProc;


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Factory_Set
**
**  Description:    Initializes Audio Processor's context with factory settings,
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Factory_Set(pLX_ADEMOD_AudioProc pContext);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Options_Set
**
**  Description:    Sets audio options.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	options
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Options_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Options_Get
**
**  Description:    Returns audio options.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	options
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Options_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_InputFreq_Set
**
**  Description:    Sets input frequency.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the input frequency data.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_InputFreq_Get
**
**  Description:    Returns actual input frequency.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue		 - Pointer to the input frequency data:
**						pValue[0]	SC1 frequency	In Hertz
**						pValue[1]	SC1 min frequency	In Hertz
**						pValue[2]	SC1 max frequency	In Hertz
**						pValue[3]	SC2 frequency		In Hertz
**						pValue[4]	SC2 min frequency	In Hertz
**						pValue[5]	SC2 max frequency	In Hertz
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Use macro LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_FRQ) when allocating memory.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Mode_Set
**
**  Description:    Sets Audio mode.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the input frequency data.
**                  hUser		 - Handle to user data.
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mode_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Mode_Get
**
**  Description:    Returns Audio mode.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue		 - Pointer to the input frequency data:
**						pValue[0]	Audio Mode
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Use macro LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_AUD_FRQ) when allocating memory.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mode_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Mute_Set
**
**  Description:    Sets audio mute.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	mute
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mute_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Mute_Get
**
**  Description:    Returns audio mute.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	mute
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mute_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Gain_Set
**
**  Description:    Sets audio gain thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 gain
**						pValue[1]	CH2 gain
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Gain_Get
**
**  Description:    Returns actual audio gain.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 gain,
**						pValue[1]	CH2 gain.
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Gain_Set
**
**  Description:    Sets audio gain thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 gain
**						pValue[1]	CH2 gain
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Gain_Get
**
**  Description:    Returns actual audio gain.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 gain,
**						pValue[1]	CH2 gain.
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Offset_Set
**
**  Description:    Sets audio offset.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 offset,
**						pValue[1]	CH2 offset.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Offset_Get
**
**  Description:    Returns actual audio gain.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 offset,
**						pValue[1]	CH2 offset.
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Offset_Set
**
**  Description:    Sets audio offset.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 offset,
**						pValue[1]	CH2 offset.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Offset_Get
**
**  Description:    Returns actual audio gain.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 offset,
**						pValue[1]	CH2 offset.
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Clip_Set
**
**  Description:    Sets audio clipping thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 clip min,
**						pValue[1]	CH1 clip max,
**						pValue[2]	CH2 clip min,
**						pValue[3]	CH2 clip max.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Clip_Get
**
**  Description:    Returns audio clipping thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 clip min,
**						pValue[1]	CH1 clip max,
**						pValue[2]	CH2 clip min,
**						pValue[3]	CH2 clip max.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Clip_Set
**
**  Description:    Sets audio clipping thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 clip min,
**						pValue[1]	CH1 clip max,
**						pValue[2]	CH2 clip min,
**						pValue[3]	CH2 clip max.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Clip_Get
**
**  Description:    Returns audio clipping thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 clip min,
**						pValue[1]	CH1 clip max,
**						pValue[2]	CH2 clip min,
**						pValue[3]	CH2 clip max.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  VideoStandard  - The video standard,
**                  AudioStandard  - The audio standard.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
**  Note:           Parameter values set by the user preside over the settings
**                  derived from video standard. Therefore if FmLg_Parameter_Set
**                  function was called to set appropriate parameters prior to
**                  Standard_Set then the values given to the registers will be
**                  those defined by FmLg_Parameter_Set
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Set(pLX_ADEMOD_AudioProc pContext, UINT32 VideoStandard, UINT32 AudioStandard);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Standard_Get
**
**  Description:    Returns audio standard.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pAudioStandard  - The audio standard.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Get(pLX_ADEMOD_AudioProc pContext, UINT32* pAudioStandard);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SifBw_Set
**
**  Description:    Sets SIF bandwidth.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifBw_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SifBw_Get
**
**  Description:    Returns SIF bandwidth.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifBw_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SifFreq_Set
**
**  Description:    Sets SIF frequency thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	lower frequency,
**						pValue[1]	upper frequency.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifFreq_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SifFreq_Get
**
**  Description:    Returns SIF frequency thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	lower frequency,
**						pValue[1]	upper frequency.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifFreq_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_MonoFreq_Set
**
**  Description:    Sets mono frequency.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	lower frequency,
**						pValue[1]	upper frequency.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoFreq_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_MonoFreq_Get
**
**  Description:    Returns mono frequency.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	lower frequency,
**						pValue[1]	upper frequency.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoFreq_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_MonoGain_Set
**
**  Description:    Sets mono gain value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Gain1,
**						pValue[1]	Gain2,
**						pValue[2]	Shift1,
**						pValue[3]	Shift2,
**						pValue[4]	Shift3.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoGain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_MonoGain_Get
**
**  Description:    Returns mono gain value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Gain1,
**						pValue[1]	Gain2,
**						pValue[2]	Shift1,
**						pValue[3]	Shift2,
**						pValue[4]	Shift3.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoGain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Deviation_Set
**
**  Description:    Sets mono deviation.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	deviation.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deviation_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Deviation_Get
**
**  Description:    Returns mono deviation.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	deviation.
**
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deviation_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_BBSampleRate_Set
**
**  Description:    Sets mono BBSampleRate.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	BBSampleRate.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BBSampleRate_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_BBSampleRate_Get
**
**  Description:    Sets mono BBSampleRate.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	BBSampleRate.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BBSampleRate_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Deemphasis_Set
**
**  Description:    Sets mono Deemphasis.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Deemphasis.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deemphasis_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Deemphasis_Get
**
**  Description:    Returns mono Deemphasis.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Deemphasis.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deemphasis_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Set
**
**  Description:    Sets mono BasebandExtraGain.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	BasebandExtraGain.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Get
**
**  Description:    Returns mono BasebandExtraGain.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	BasebandExtraGain.
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_AGCTarget_Set
**
**  Description:    Sets AGC target value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	AGC Target * 10**10,
**						pValue[1]	AGC BW Target * 10**6.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCTarget_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_AGCTarget_Get
**
**  Description:    Returns AGC target value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	AGC Target * 10**10,
**						pValue[1]	AGC BW Target * 10**6.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCTarget_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_AGCGainThresh_Get
**
**  Description:    Returns AGC gain thresholds value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	AGC min gain * 10**12,
**						pValue[1]	AGC max gain.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainThresh_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_AGCGainPreset_Get
**
**  Description:    Returns AGC gain preset value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	AGC manual gain * 2**10,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainPreset_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_AGCGainThresh_Set
**
**  Description:    Sets AGC gain thresholds value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	AGC min gain,
**						pValue[1]	AGC max gain.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainThresh_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_AGCGainPreset_Set
**
**  Description:    Sets AGC gain preset value.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	AGC manual gain * 2**10,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainPreset_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Mix_Set
**
**  Description:    Sets Audio upmix/downmix values.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	down mix,
**						pValue[1]	up mix.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mix_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Mix_Get
**
**  Description:    Returns SIF bandwidth.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	down mix,
**						pValue[1]	up mix.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mix_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_BuzzDelay_Set
**
**  Description:    Sets audio buzz delay parameters.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	buzz delay offset,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BuzzDelay_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_BuzzDelay_Get
**
**  Description:    Returns audio buzz delay parameters.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	buzz delay offset,
**						pValue[1]	buzz delay fudge.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BuzzDelay_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_MuteSpeed_Set
**
**  Description:    Sets audio mute.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	mode: /slow/fast/middle/none,
**						pValue[1] - actual MuteOn speed, when mode is "none".
**						pValue[2] - actual MuteOff speed, when mode is "none".
**
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MuteSpeed_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_MuteSpeed_Get
**
**  Description:    Returns audio mute speed.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	mode: /slow/fast/middle/none,
**						pValue[1] - actual MuteOn speed, when mode is "none".
**						pValue[2] - actual MuteOff speed, when mode is "none".
**
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MuteSpeed_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Flags_Set
**
**  Description:    Sets audio processing flags.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  Flags        - The flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Set(pLX_ADEMOD_AudioProc pContext, UINT32 Flags);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Flags_Get
**
**  Description:    Returns audio processing flags.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Get(pLX_ADEMOD_AudioProc pContext, UINT32* pFlags);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SampleRateDivider_Set
**
**  Description:    Sets sampling rate divider.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  Value        - The new sampling rate divider value:
**						pValue[0]	when 0 - divider = 10, 1 - divider = 12,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SampleRateDivider_Set(pLX_ADEMOD_AudioProc pContext, UINT32 Value);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SampleRateDivider_Get
**
**  Description:    Returns actual sampling rate divider.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the actual sampling rate value.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SampleRateDivider_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Init
**
**  Description:    Initializes Audio Processor's hardware.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Init(pLX_ADEMOD_AudioProc pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Start
**
**  Description:    Starts Audio processing block.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pVideo_freq  - Pointer to the Video carrier frequency,
**                  flags        - Flags,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Updates the registers according to the lates state of the
**                  variables.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Start(pLX_ADEMOD_AudioProc pContext, UINT32 *pVideo_freq, UINT32 flags, Handle_t hUser);

#if defined( __cplusplus )
}
#endif


#endif  //_ADEMOD_M14_AUDIO_H_
