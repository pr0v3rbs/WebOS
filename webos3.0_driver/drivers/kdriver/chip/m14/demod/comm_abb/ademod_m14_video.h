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
**  Name: ademod_m14_video.h
**
**  Description:    ABB M14 Video Processing block.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Mode_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Start
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Mode_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Start
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
#ifndef _ADEMOD_M14_VIDEO_H_
#define _ADEMOD_M14_VIDEO_H_

#include "ademod_common.h"
#include "ademod_common_video.h"


#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

#define LX_ADEMOD_PF2_DELIVERY_RATE	(1 << 16)

/*
**  Overmodulation
*/
typedef struct LX_ADEMOD_Overmodulation_tag
{
	UINT8		mode;
	UINT8		phaserev;
	UINT8		perc;
	UINT8		phaserev_angle;
	UINT64		CRnoise_threshold_lock;
	UINT64		CRnoise_threshold_acq;
	UINT64		CRover_mod_threshold_lock;
	UINT64		CRover_mod_threshold_acq;
	UINT64		CRover_mod_threshold2_lock;
	UINT64		CRover_mod_threshold2_acq;
	UINT64		CRnoise_w_acq;
	UINT64		CRnoise_w_lock;
	UINT64		CRnoise2_w_acq;
	UINT64		CRnoise2_w_lock;
} LX_ADEMOD_Overmodulation, *pLX_ADEMOD_Overmodulation;

/*
**  Video Processor
*/
typedef struct LX_ADEMOD_VideoProc_tag
{
	UINT32		flags;
	UINT32		update;
	UINT32		update1;
	UINT32		factory;
	UINT32      input_mode;
	UINT32      sampling_rate;
	UINT32      Fs_div;
    UINT32      inp_frequency;
    UINT32      out_frequency;
	UINT32      bandwidth;
	UINT32      lowif_bandwidth;
	UINT32      first_audio;
	UINT32		min_range;
	UINT32		max_range;
	UINT32		vsb_energy;
	UINT32		PF1_tinc;
	UINT32		PR_delivery_rate;
	UINT32		PF2_tinc;
	UINT32		PF2_delivery_rate;
	UINT32		PF2_tincInv;
	UINT64      virtualfs;
	int			FR1_delta_ph_init;
	int			FR2_delta_ph_init;
	int			FR1_delta_ph_min;
	int			FR1_delta_ph_max;
	int			FR2_delta_ph_min;
	int			FR2_delta_ph_max;
	UINT32		offset_tweak;
	UINT32		FR1_InputFreqOffset;
	UINT32		FR2_manual_ph;
	UINT32		main_cutoff;
	UINT32		fr_loop_speed;
	UINT32		fr_loop_gain;
	UINT16		fr_lpf_coeff_acq;
	UINT16		fr_lpf_coeff_lock;
	UINT16		fr_lpf_gain_acq;
	UINT16		fr_lpf_gain_lock;
	UINT32		ph_loop_speed;
	UINT32		ph_loop_gain;
	UINT32		fr_lock_thresh;
	UINT32		ph_lock_thresh;
	UINT32		fr_lock_counter;
	UINT32		ph_lock_counter;
	UINT32		ph_lock_det_err;
	UINT32		ph_lock_det_lines;
	UINT32		fr_unlock_thresh;
	UINT32		ph_unlock_thresh;
	UINT32		fr_unlock_counter;
	UINT32		ph_unlock_counter;
	UINT32		ph_unlock_det_err;
	UINT32		ph_unlock_det_lines;
	UINT32		mute_fr_lock_thresh;
	UINT32		mute_ph_lock_thresh;
	UINT32		mute_fr_lock_counter;
	UINT32		mute_ph_lock_counter;
	UINT32		mute_ph_lock_det_err;
	UINT32		mute_ph_lock_det_lines;
	UINT32		mute_fr_unlock_thresh;
	UINT32		mute_ph_unlock_thresh;
	UINT32		mute_fr_unlock_counter;
	UINT32		mute_ph_unlock_counter;
	UINT32		mute_ph_unlock_det_err;
	UINT32		mute_ph_unlock_det_lines;
	UINT32		narrowband_offs1;
	UINT32		narrowband_offs2;
	UINT32		narrowband_offs3;
	UINT32		Reset_Fr1_thresh;
	UINT32		Reset_CRR_bound_TC;
	UINT32		Reset_CRR_lock_TC;
	UINT16		ph_lpf_coeff_acq;
	UINT16		ph_lpf_coeff_lock;
	UINT16		ph_lpf_gain_acq;
	UINT16		ph_lpf_gain_lock;
	UINT8		PF1_tinc_prec; // PF1 tinc precision
	UINT8		PF2_tincInv_prec; // PF1 tinc precision
	UINT8		PF2_tinc_prec; // PF2 tinc precision
	UINT8		PF1_delivery_rate_prec; // PF1 delivery rate precision
	UINT8		PF2_delivery_rate_prec; // PF2 delivery rate precision
	UINT8		FR1_prec; // FR1 phase precision
	UINT8		FR2_prec; // FR2 phase precision
	UINT8		CRF_Index_Acq;
	UINT8		CRF_Index_Lock;
	UINT8		AGC_SyncDec_Index_Acq;
	UINT8		AGC_SyncDec_Index_Lock;
	UINT8		MagDet_Index_Acq;
	UINT8		MagDet_Index_Lock;
	UINT8		fr_loop_speed_scale;
	UINT8		ph_loop_speed_scale;
	UINT8		fr_loop_gain_scale;
	UINT8		ph_loop_gain_scale;
	UINT8		ph_lock_det_err_scale;
	UINT8		ph_lock_det_lines_scale;
	UINT8		ph_unlock_det_err_scale;
	UINT8		ph_unlock_det_lines_scale;
	UINT8		mute_ph_lock_det_err_scale;
	UINT8		mute_ph_lock_det_lines_scale;
	UINT8		mute_ph_unlock_det_err_scale;
	UINT8		mute_ph_unlock_det_lines_scale;
	UINT8		option_range;
	UINT8		narrowband_mode_offs;
	LX_ADEMOD_Overmodulation overmod;
}  LX_ADEMOD_VideoProc, *pLX_ADEMOD_VideoProc;


/*
**  Video Processor
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Factory_Set
**
**  Description:    Initializes Video Processor's context with factory settings,
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Factory_Set(pLX_ADEMOD_VideoProc pContext);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Bandwidth_Set
**
**  Description:    Sets video bandwidth.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Bandwidth    - The new bandwidth value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Bandwidth);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Bandwidth_Get
**
**  Description:    Sets video bandwidth.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pBandwidth   - Pointer to the bandwidth value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pBandwidth);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LIFBandwidth_Set
**
**  Description:    Sets Low IF bandwidth.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Bandwidth    - The new Low IF bandwidth value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Bandwidth);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LIFBandwidth_Get
**
**  Description:    Returns Low IF bandwidth.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pBandwidth   - Pointer to the LowIF bandwidth value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pBandwidth);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_AudioCarrier_Set
**
**  Description:    Sets Audio Carrier according to video bandwidth.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Bandwidth    - The video bandwidth.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
**  Note:
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_AudioCarrier_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Bandwidth);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_InputFreq_Set
**
**  Description:    Sets input frequency.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Value	     - The new input frequency value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Value);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_InputFreq_Get
**
**  Description:    Returns actual input frequency.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the actual input frequency value:
**						pValue[0]	IF Input Frequency	In Hertz
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Use macro LX_ADEMOD_PARM_SIZE(LX_ADEMOD_PARM_VID_FRQ) when allocating memory.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_InputFreq_GetX
**
**  Description:    Returns actual input frequency and related intermediate attribute.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the actual input frequency value.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Returns two actual input frequency values, namely FR1 and FR2.
**                  The memory for pInputFreq must be allocated accordingly.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_GetX(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_SampleRate_Set
**
**  Description:    Sets sampling rate.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Value        - The new sampling rate value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Value);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_SampleRate_Get
**
**  Description:    Returns actual sampling rate.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the actual sampling rate value.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Returns two actual sampling rate values, namely FR1 and FR2.
**                  The memory for pSampleRate must be allocated accordingly.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_SampleRateDivider_Set
**
**  Description:    Sets sampling rate divider.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Value        - The new sampling rate divider value:
**						pValue[0]	when 0 - divider = 10, 1 - divider = 12,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRateDivider_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Value);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_SampleRateDivider_Get
**
**  Description:    Returns actual sampling rate divider.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the actual sampling rate value.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRateDivider_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Range_Set
**
**  Description:    Selects range.
**                  pValue       - Pointer to the value,
**						where pValue[0] - option: nominal/min/max/none,
**						      pValue[1] - actual min range, when mode is "none",
**						      pValue[2] - actual max range, when mode is "none".
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the range option: max, nominal, min, random
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Range_Get
**
**  Description:    Returns pull-in range.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the range value:
**						pValue[0]	option,
**						pValue[1]	min range,
**						pValue[2]	max range.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_VSB_Set
**
**  Description:    Sets VSB energy range.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the VSB value:
**						pValue[0]	VSB energy.
**                  max_range    - The max range,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VSB_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_VSB_Get
**
**  Description:    Returns VSB energy range.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the VSB value:
**						pValue[0]	VSB energy.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VSB_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Cutoff_Set
**
**  Description:    Sets Cutoff value.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the Cutoff value:
**						pValue[0]	Cutoff.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Cutoff_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Cutoff_Get
**
**  Description:    Returns Cutoff value.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the Cutoff value:
**						pValue[0]	Cutoff.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Cutoff_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_CarrierOffset_Set
**
**  Description:    Sets CarrierOffset value.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the CarrierOffset value:
**						pValue[0]	CarrierOffset.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_CarrierOffset_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_CarrierOffset_Get
**
**  Description:    Returns CarrierOffset value.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the CarrierOffset value:
**						pValue[0]	CarrierOffset.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_CarrierOffset_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Tinc_Set
**
**  Description:    Sets Tinc values directly.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the values:
**						pValue[0]	PF1 tinc or 0,
**						pValue[1]	PF2 tinc or 0,
**						pValue[2]	PF2 tinc inv or 0,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Tinc_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Tinc_Get
**
**  Description:    Gets Tinc values.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the values:
**						pValue[0]	PF1 tinc or 0,
**						pValue[1]	PF2 tinc or 0,
**						pValue[2]	PF2 tinc inv or 0,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Tinc_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_OffsTweak_Set
**
**  Description:    Sets Video offset tweak.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	offset tweak.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_OffsTweak_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_OffsTweak_Get
**
**  Description:    Returns Video offset tweak.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	offset tweak.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_OffsTweak_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Flags_Set
**
**  Description:    Sets video processing flags.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Flags        - The flags' bitmap.
**                  mask         - Indicates, which flags where changed.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Flags, UINT32 mask);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Flags_Get
**
**  Description:    Returns video processing flags.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Get(pLX_ADEMOD_VideoProc pContext, UINT32* pFlags);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Mode_Set
**
**  Description:    Sets input mode.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  InputMode    - The input mode.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
**  Note:           Parameter values set by the user preside over the settings
**                  derived from input mode. Therefore if FmLg_Parameter_Set
**                  function was called to set appropriate parameters prior to
**                  Standard_Set then the values given to the registers will be
**                  those defined by FmLg_Parameter_Set
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Mode_Set(pLX_ADEMOD_VideoProc pContext, UINT32 InputMode);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  VideoStandard  - The video standard.
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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Standard_Set(pLX_ADEMOD_VideoProc pContext, UINT32 VideoStandard);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Overmodulation_Set
**
**  Description:    Sets Over-modulation mode.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  overmod      - overmodulation data,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode   - Invalid mode
**  Note:           By default over-modulation is set to ENABLED.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Set(pLX_ADEMOD_VideoProc pContext, UINT32* overmod, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Set
**
**  Description:    Sets Low IF output center freq.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Value        - The new sampling rate value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Get
**
**  Description:    Returns Low IF output center freq.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Value        - The new sampling rate value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_OvermodPhaseRev_Set
**
**  Description:    Sets Over-modulation phase-reversal mode.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**						where pValue[0] - mode: nominal/disable/none(manual),
**						      pValue[1] - overmode percentage, when mode is "none",
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode   - Invalid mode
**  Note:           By default over-modulation is set to ENABLED.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_OvermodPhaseRev_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Overmodulation_Get
**
**  Description:    Returns Over-modulation mode.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Over-modulation mode.
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode   - Invalid mode
**  Note:           By default over-modulation is set to DISABLED.
**                  It can NOT be enabled during initialization time or during
**                  mode change, because of the problem described in BUG 501.
**                  Therefore over-modulation must be set when signal is already stable.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_FRLoopSpeed_Set
**
**  Description:    Sets FR Loop speed either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/min/slow/fast/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_FRLoopSpeed_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_FRLoopGain_Set
**
**  Description:    Sets FR Loop gain either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/min/slow/fast/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_FRLoopGain_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_PHLoopSpeed_Set
**
**  Description:    Sets PH Loop speed either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/min/slow/fast/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_PHLoopSpeed_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_PHLoopGain_Set
**
**  Description:    Sets PH Loop gain either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/min/slow/fast/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_PHLoopGain_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LockDet_Set
**
**  Description:    Sets Lock Detection speed either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LockDetErr_Set
**
**  Description:    Sets Lock Detection error to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LockDetLines_Set
**
**  Description:    Sets Lock Detection lines to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_UnlockDet_Set
**
**  Description:    Sets Unlock Detection speed either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_UnlockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_UnlockDetErr_Set
**
**  Description:    Sets Unlock Detection error to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_UnlockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_UnlockDetLines_Set
**
**  Description:    Sets Unlock Detection lines to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_UnlockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_MutelockDet_Set
**
**  Description:    Sets Mutelock Detection speed either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MutelockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_MutelockDetErr_Set
**
**  Description:    Sets Mutelock Detection error to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MutelockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_MutelockDetLines_Set
**
**  Description:    Sets Mutelock Detection lines to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MutelockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_MuteunlockDet_Set
**
**  Description:    Sets Muteunlock Detection speed either directly or to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MuteunlockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_MuteunlockDetErr_Set
**
**  Description:    Sets Muteunlock Detection error to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MuteunlockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_MuteunlockDetLines_Set
**
**  Description:    Sets Muteunlock Detection lines to one of preset modes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: nominal/slow/fast/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MuteunlockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_CRReset_Set
**
**  Description:    Sets CR reset parameters.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**					where pValue[0] - mode: nominal/disable/none(manual),
**						  pValue[1] - Sets threshold above min & below max used for CR reset alg in MHz, when mode is "none",
**						  pValue[2] - Time that carrier recovery can be within threshold of bounds before reset in ms, when mode is "none",
**						  pValue[3] - Time that carrier recovery can stay out of lock before reset in ms, when mode is "none",
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode   - Invalid mode
**  Note:           By default over-modulation is set to ENABLED.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_CRReset_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_VirtualFS_Set
**
**  Description:    Sets virtual FS to user-defined value.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - actual value LSB,
**						      pValue[1] - actual value MSB.
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VirtualFS_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_VirtualFS_Get
**
**  Description:    Returns virtual FS.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - actual value LSB,
**						      pValue[1] - actual value MSB.
**                  hUser		 - Handle to user data
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VirtualFS_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Init
**
**  Description:    Initializes Video Processor's hardware.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Init(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Start
**
**  Description:    Starts video processing block.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Updates the registers according to the lates state of the
**                  variables.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Start(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Set
**
**  Description:    Sets Narrowband AGC Piece-wise LPFGain offset.
**
**  Parameters:     pContext     - Pointer to Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - Offset1 register: FR2_manual_ph[1:0],
**						      pValue[2] - Offset2 register: FR2_manual_ph[3:2],
**						      pValue[3] - Offset3 register: FR2_manual_ph[5:4].
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Get
**
**  Description:    Returns Narrowband AGC Piece-wise LPFGain offset.
**
**  Parameters:     pContext     - Pointer to Video Processor context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - Offset1 register: FR2_manual_ph[1:0],
**						      pValue[2] - Offset2 register: FR2_manual_ph[3:2],
**						      pValue[3] - Offset3 register: FR2_manual_ph[5:4].
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser);

#if defined( __cplusplus )
}
#endif


#endif  //_ADEMOD_M14_VIDEO_H_
