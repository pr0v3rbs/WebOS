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
**  Name: ademod_m14_legacy.h
**
**  Description:    ABB M14 Legacy Demodulation block.
**
**  Functions
**  Implemented:  LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Factory_Set
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Standard_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Set
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Get
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Init
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Start
**
**  References:     
**
**  Exports:        	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Factory_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Standard_Set
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Get
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Init
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Start
**
**  Dependencies:  ademod_common.h   - for basic FM types,
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
#ifndef _ADEMOD_M14_LEGACY_H_
#define _ADEMOD_M14_LEGACY_H_

#include "ademod_common.h"
#include "ademod_common_video.h"
#include "ademod_common_legacy.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  Feature flags:
**  These flags are for features that require on/off control
*/
#define LX_ADEMOD_FFLAG_DK_CHINA_EN		(0x00000001L)
#define LX_ADEMOD_FFLAG_AUD_DEM_EN			(0x00000800L)
#define LX_ADEMOD_FFLAG_IF_DEM_EN			(0x00002000L)
#define LX_ADEMOD_FFLAG_AUD_UPSAMPLE_EN	(0x00010000L)
#define LX_ADEMOD_FFLAG_VID_UPSAMPLE_EN	(0x00020000L)
#define LX_ADEMOD_FFLAG_AUD_LOWIF_EN		(0x00040000L)
#define LX_ADEMOD_FFLAG_VID_LOWIF_EN		(0x00080000L)
#define LX_ADEMOD_FFLAG_DAC_POSTSAW_EN		(0x00100000L)
#define LX_ADEMOD_FFLAG_SYNCD_NEGMOD_EN	(0x00200000L)
#define LX_ADEMOD_FFLAG_SYNCD_SRCSEL_EN	(0x00400000L)
#define LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN	(0x00800000L)
#define LX_ADEMOD_FFLAG_SYNCD_HEIGHT_NBP	(0x01000000L)
#define LX_ADEMOD_FFLAG_SYNCD_VSLICE_OVR	(0x02000000L)
#define LX_ADEMOD_FFLAG_SYNCD_AGCMASKOVR	(0x04000000L)
#define LX_ADEMOD_FFLAG_SYNCD_HYBRID_EN	(0x10000000L)

/*
**  Update flags:
**  USR flags indicate that parameter has been modified from factory default to user preference.
**  These flags are used to assure presidence of user settings over the factory defaults.
**  They will be cleared when appropriate Factory_Set function is called.
**  NOTE: Do not use flag 0x80000000 because it is taken by LX_ADEMOD_UFLAG_UPDATE
*/

#define LX_ADEMOD_UFLAG_VID_GAIN_USR		(0x00000001L) // video gain user update
#define LX_ADEMOD_UFLAG_VID_OFFS_USR		(0x00000002L) // video offset user update
#define LX_ADEMOD_UFLAG_GRP_DELAY_USR		(0x00000004L) // group delay user update
#define LX_ADEMOD_UFLAG_DAC_INST_THR_USR	(0x00000008L) // video instability threshold user update
#define LX_ADEMOD_UFLAG_VID_CLIPPER_USR	(0x00000010L) // video clipper update
#define LX_ADEMOD_UFLAG_DAC_SRCSEL_USR		(0x00000020L) // cross-bar source select
#define LX_ADEMOD_UFLAG_DAC_OPTIONS_USR	(0x00000040L) // dac options
#define LX_ADEMOD_UFLAG_DAC_POSTSAW_USR	(0x00000080L) // post-saw DAC user update

#define LX_ADEMOD_UFLAG_VID_GAIN_RC_USR		(0x00000100L) // video gain user update
#define LX_ADEMOD_UFLAG_VID_OFFS_RC_USR		(0x00000200L) // video offset user update
#define LX_ADEMOD_UFLAG_DAC_MIXMODE_USR		(0x00000400L) // mixed mode
#define LX_ADEMOD_UFLAG_SYNC_OPTIONS_USR	(0x00000800L) // sync options
#define LX_ADEMOD_UFLAG_VID_CLIPPER_RC_USR	(0x00001000L) // video clipper update

/*
**  Legacy Demodulator
*/

#define LX_ADEMOD_GROUP_DELAY_LUT_SIZE				(20)
#define LX_ADEMOD_GROUP_DELAY_LUT_SIZE_deBug_1		(80)

typedef INT16 LX_ADEMOD_GroupDelayLUT [LX_ADEMOD_GROUP_DELAY_LUT_SIZE_deBug_1];

/*
** CVBS DAC
*/
typedef struct LX_ADEMOD_VideoDAC_tag
{
	int		gain;
	int		offset;
	int		clipper;
	int		dither;
}  LX_ADEMOD_VideoDAC, *pLX_ADEMOD_VideoDAC;

/*
** Horizontal Sync
*/
typedef struct LX_ADEMOD_HSync_tag
{
	UINT32		period;
	UINT32		rate;
	UINT32		width;
	UINT32		max_err;
	UINT32		min_thr;
	UINT32		max_thr;
	UINT32		min_cnt_thr;
	UINT32		max_cnt_thr;
	UINT32		lock_thr;
	UINT32		unlock_thr;
	UINT32		stable_thr;
	UINT32		unstbl_thr;
	UINT32		hs_pw;
	UINT32		tc_sel;
	UINT32		tip_duration;
	UINT32		tip_samples;
	UINT32		tip_avg_sel;
	UINT32		tip_win_start;
	UINT32		tip_win_period;
	UINT32		tip_win_wgt;
	UINT32		tip_ser_win_start;
	UINT32		tip_ser_win_period;
	UINT32		tip_ser_win_wgt;
	UINT32		lpf_coeffb;
	UINT32		safe_agc_line_start;
	UINT32		safe_agc_line_stop;
	UINT32		safe_agc_mask;
	UINT32		min_sync_height;
	UINT32		height_th;
	UINT32		adj_sel_fine;
	UINT32		adj_sel_coarse;
}  LX_ADEMOD_HSync, *pLX_ADEMOD_HSync;

/*
** Vertical Sync
*/
typedef struct LX_ADEMOD_VSync_tag
{
	UINT32		period;
	UINT32		nom_period;
	UINT32		width;
	UINT32		max_err;
	UINT32		lock_thr;
	UINT32		unlock_thr;
	UINT32		stable_thr;
	UINT32		unstbl_thr;
	UINT32		int_period;
	UINT32		tc_sel;
}  LX_ADEMOD_VSync, *pLX_ADEMOD_VSync;

/*
** Horizontal Slicer
*/
typedef struct LX_ADEMOD_HSlicer_tag
{
	UINT32		thresh;
	UINT32		level_sel;
	UINT32		level_sel_ser;
	UINT32		ser_exit_line;
}  LX_ADEMOD_HSlicer, *pLX_ADEMOD_HSlicer;

/*
** Vertical Slicer
*/
typedef struct LX_ADEMOD_VSlicer_tag
{
	UINT32		thresh;
	UINT32		level_sel;
	UINT32		level_sel_bp_st;
}  LX_ADEMOD_VSlicer, *pLX_ADEMOD_VSlicer;


/*
** Sync Decoder
*/
typedef struct LX_ADEMOD_SyncDec_tag
{
	UINT32			line_per_frame;
	UINT32			attack;
	UINT32			decay;
	UINT32			decay_thresh;
	UINT32			decay_window;
	UINT32			fast_decay;
	UINT32			peak_thr_min;
	UINT32			peak_thr_max;
	UINT32			ser_thresh;
	UINT32			ser_lockout;
	UINT32			ser_max_dur;
	UINT32			bp_avg_sel;
	UINT32			bp_win_start;
	UINT32			bp_win_period;
	UINT32			bp_win_wgt;
	UINT32			bp_ser_win_start;
	UINT32			bp_ser_win_period;
	UINT32			bp_ser_win_wgt;
	UINT32			frontporch_duration;
	UINT32			breezeway_duration;
	UINT32			breezeway_samples;
	UINT32			burst_duration;
	UINT32			burst_samples;
	UINT32			backporch_duration;
	UINT32			backporch_samples;
	LX_ADEMOD_HSync	hs;
	LX_ADEMOD_VSync	vs;
	LX_ADEMOD_HSlicer	h_slice;
	LX_ADEMOD_VSlicer	v_slice;
}  LX_ADEMOD_SyncDec, *pLX_ADEMOD_SyncDec;

/*
** Legacy Video Context
*/
typedef struct LX_ADEMOD_LegacyDemod_tag
{
	UINT32					flags;
	UINT32					update;
	UINT32					factory;
	UINT32					video_std;
	UINT32					sync_options;
	UINT32 					rcMode;
	LX_ADEMOD_VideoDAC			video_dac;
	LX_ADEMOD_VideoDAC			video_rc;
	LX_ADEMOD_VideoDAC			video_rc_intpl;
	LX_ADEMOD_GroupDelayLUT	GroupDelayLUT;
	LX_ADEMOD_SyncDec			sync_dec;
}  LX_ADEMOD_LegacyDemod, *pLX_ADEMOD_LegacyDemod;

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Factory_Set
**
**  Description:    Initializes Legacy demodulator's context with factory settings.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Factory_Set(pLX_ADEMOD_LegacyDemod pContext);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  VideoStandard  - The video standard,
**                  AudioStandard  - The audio standard.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Standard_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 VideoStandard, UINT32 AudioStandard);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Flags_Set
**
**  Description:    Sets audio processing flags.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  Flags        - The flags' bitmap,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Flags_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 Flags, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_SyncOptions_Set
**
**  Description:    Sets video Sync Options.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	sync options
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_SyncOptions_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_SyncOptions_Get
**
**  Description:    Returns actual video Options.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context,
**                  pValue       - Pointer to the value:
**						pValue[0]	sync options
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_SyncOptions_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_MixedMode_Set
**
**  Description:    Sets audio processing mixed mode.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  Mode         - 1- mixed mode ON, 0- mixed mode OFF,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_MixedMode_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 Mode, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Flags_Get
**
**  Description:    Returns audio processing flags.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Flags_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32* pFlags);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Gain_Set
**
**  Description:    Sets video gain.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pGain        - Pointer to the gain value:
**						pGain[0]	Video gain
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pGain, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Gain_Get
**
**  Description:    Returns actual video gain.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context,
**                  pGain        - Pointer to the gain value:
**						pGain[0]	Video gain
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pGain, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set
**
**  Description:    Sets video gain.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video gain
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get
**
**  Description:    Returns actual video gain.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Video Rate converter gain
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser);


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Offset_Set
**
**  Description:    Sets video Offset.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video Offset
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Offset_Get
**
**  Description:    Returns actual video Offset.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Video Offset
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set
**
**  Description:    Sets video Offset.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video Rate converter Offset
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get
**
**  Description:    Returns actual video Offset.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Video Rate converter Offset
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Clip_Set
**
**  Description:    Sets video clipper.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video clipper
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Clip_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Clip_Get
**
**  Description:    Sets video clipper.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video clipper
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Clip_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Clip_Set
**
**  Description:    Sets video clipper.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video clipper
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Clip_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Clip_Get
**
**  Description:    Sets video clipper.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video clipper
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Clip_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set
**
**  Description:    Sets group delay LUT.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**					pLUT         - Pointer to the Group Delay LUT.
**                  hUser		 - Handle to user data.
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get
**
**  Description:    Returns group delay LUT.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**					pLUT         - Pointer to the Group Delay LUT.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Init
**
**  Description:    Initializes Legacy demodulator's hardware.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Init(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_PAD_Set
**
**  Description:    Enables/disables Transport Stream.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_PAD_Set(pLX_ADEMOD_LegacyDemod pContext, BOOL OhOff, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_TS_Get
**
**  Description:    Returns Transport Stream status.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_TS_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32* pVal, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Start
**
**  Description:    Starts legacy demodulator block.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  pVideo_freq  - Pointer to the Video carrier frequency,
**                  flags        - Video flags,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Updates the registers according to the lates state of the
**                  variables.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Start(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pVideo_freq, UINT32 flags, Handle_t hUser);

#if defined( __cplusplus )
}
#endif

#endif //_ADEMOD_M14_LEGACY_H_
