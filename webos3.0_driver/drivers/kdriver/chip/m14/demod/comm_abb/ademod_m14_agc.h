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
**  Name: ademod_m14_agc.h
**
**  Description:   ABB M14 AGC block.
**
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Line_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Start
**
**  References:     FMSW-0001: Fresco SDK. Software Design Document.
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Line_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Start
**
**  Dependencies:   fmlg_common.h   - for basic FM types,
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   10-05-2007    Alex Shulman    Initial draft.
**
*****************************************************************************/
#ifndef _ADEMOD_M14_AGC_H_
#define _ADEMOD_M14_AGC_H_

#include "ademod_common.h"
#include "ademod_common_agc.h"


#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif
/*
**  RF DAC Update flags:
**  USR flags indicate that parameter has been modified from factory default to user preference.
**  These flags are used to assure presidence of user settings over the factory defaults.
**  They will be cleared when appropriate Factory_Set function is called.
**  NOTE: Do not use flag 0x80000000 because it is taken by LX_ADEMOD_UFLAG_UPDATE
*/

#define LX_ADEMOD_UFLAG_RFDAC_INST_THR_USR		(0x00000001L) // RF instability user update

/*
**  IF DAC Update flags:
**  USR flags indicate that parameter has been modified from factory default to user preference.
**  These flags are used to assure presidence of user settings over the factory defaults.
**  They will be cleared when appropriate Factory_Set function is called.
**  NOTE: Do not use flag 0x80000000 because it is taken by LX_ADEMOD_UFLAG_UPDATE
*/

#define LX_ADEMOD_UFLAG_IFDAC_INST_THR_USR		(0x00000001L) // IF instability user update

/*
**  RF AGC Update flags:
**  USR flags indicate that parameter has been modified from factory default to user preference.
**  These flags are used to assure presidence of user settings over the factory defaults.
**  They will be cleared when appropriate Factory_Set function is called.
**  NOTE: Do not use flag 0x80000000 because it is taken by LX_ADEMOD_UFLAG_UPDATE
*/

#define LX_ADEMOD_UFLAG_RFAGC_RFGAIN_THR_USR	(0x00000001L)
#define LX_ADEMOD_UFLAG_RFAGC_IFGAIN_THR_USR	(0x00000002L)

typedef struct LX_ADEMOD_RFDAC_tag
{
	int			clipper;
	int			instab_min;  //jeongpil.yun M14
	int			instab_max;
	int			instab_thresh;
	UINT32		DAC_Dither;
	UINT32		Dither_LFSR;
	UINT32		Dither_init;
	UINT32		update;
}  LX_ADEMOD_RFDAC, *pLX_ADEMOD_RFDAC;

typedef struct LX_ADEMOD_IFDAC_tag
{
	int			instab_min;
	int			instab_max;
	int			instab_thresh;
	UINT32		DAC_Dither;
	UINT32		Dither_LFSR;
	UINT32		Dither_init;
	UINT32		update;
}  LX_ADEMOD_IFDAC, *pLX_ADEMOD_IFDAC;

/*
**  LGT10 AGC Context
*/

//typedef struct LX_ADEMOD_AudioAgc_tag {
//	UINT8			non_agc_gain;
//	UINT8			magerror_coeff; // coefficient for the magnitude error moving average filter within the audio AGC loop
//	UINT8			fraction;
//	UINT8			update_rate;
//	UINT32			mag_ref; // target (reference) magnitude level for the AGC loop
//	UINT32			soft_clip_thr; // the soft clipping threshold for the signal after AGC has been applied
//	UINT8			peak_decay; // sets the decay amount per clock cycle of the AGC Peak Detect
//	UINT8			peak_adj;
//} LX_ADEMOD_AudioAgc, *pLX_ADEMOD_AudioAgc;

typedef struct LX_ADEMOD_LegacyAgc_tag {
	UINT32			backporch_ref;
//	UINT16			line_start;
//	UINT16			line_stop;
	INT64			min_gain;
	INT64			max_gain;
} LX_ADEMOD_LegacyAgc, *pLX_ADEMOD_LegacyAgc;

typedef struct LX_ADEMOD_RfAgc_tag {
	int				max_gain_rf;
	int				min_gain_rf;
	int				pres_gain_rf;
	int				lpf_gain_nerr_rf;
	int				lpf_gain_perr_rf;
	int				max_gain_if;
	int				min_gain_if;
	int				pres_gain_if;
	int				lpf_gain_nerr_if;
	int				lpf_gain_perr_if;
	int				backoff;
	UINT32			resp_time_poserr_RF;
	UINT32			resp_time_negerr_RF;
	UINT32			resp_time_poserr_IF;
	UINT32			resp_time_negerr_IF;
	UINT32			update;
	UINT32			fast_decay;
	UINT32			decay_thr;
	UINT32			win;
	UINT32			ref;
	UINT32			leaky_pd_attack;
	UINT32			leaky_pd_decay;
	UINT32			leaky_pd_decaywin;
	UINT32			lpf_coeff_a1;
	UINT32			lpf_coeff_b0;
	UINT32			iir_coeff_ashifts;
	UINT32			IIRCoeff_BShifts;
	UINT32			Leaky_Decay;
	UINT32			rfiir_bw;
	UINT32			lpf_bw;
	UINT32			dec_fs;
	UINT16			attack_1;
	UINT16			attack_2;
	UINT16			dec_factor;
	UINT16			iir_coeff_a1;
	UINT16			iir_coeff_b0;
	UINT16			LPF_BShifts;
	UINT16			LPFGain_Shift1;
	UINT16			LPFGain_Shift2;
	UINT16			instab_gain_adj;
	UINT16			vga_attn;
	UINT8			rf_mode;
	UINT8			if_mode;
	UINT8			rf_gain;
	UINT8			if_gain;
	UINT8			speed;
	UINT8			rfiir_mode;
	UINT8			qnoise_filter;
} LX_ADEMOD_RfAgc, *pLX_ADEMOD_RfAgc;


typedef struct LX_ADEMOD_Agc_tag {
	UINT32				flags;
	UINT32				update;
	UINT32				factory;
	UINT32				sampling_rate;
	LX_ADEMOD_AGC				common;
	LX_ADEMOD_LegacyAgc	legacy;
//	LX_ADEMOD_AudioAgc		audio;
	LX_ADEMOD_RfAgc		rf;
	LX_ADEMOD_RFDAC		rf_dac;
	LX_ADEMOD_IFDAC		if_dac;
} LX_ADEMOD_Agc, *pLX_ADEMOD_Agc;


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Factory_Set
**
**  Description:    Initializes AGC context with factory settings.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Factory_Set(pLX_ADEMOD_Agc pContext);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_MixedMode_Set
**
**  Description:    Sets AGC mixed mode.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  Mode         - 1- mixed mode ON, 0- mixed mode OFF,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_MixedMode_Set(pLX_ADEMOD_Agc pContext, UINT32 Mode, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_MixedMode_Set
**
**  Description:    Returns AGC mixed mode.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  Mode         - 0- wideband mode,
**								   1- mixed mode,
**								   2- narrowband mode,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_MixedMode_Get(pLX_ADEMOD_Agc pContext, UINT32 *pMode, Handle_t hUsere);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Agc_Mode_Set
**
**  Description:    Sets input mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  InputMode    - The input mode.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Agc_Mode_Set(pLX_ADEMOD_Agc pContext, UINT32 InputMode);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_SampleRate_Set
**
**  Description:    Sets sampling rate.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  Value        - The new sampling rate value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_SampleRate_Set(pLX_ADEMOD_Agc pContext, UINT32 Value);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Options_Set
**
**  Description:    Sets AGC flags.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  Flags        - The flags' bitmap,
**                  mask         - Indicates, which flags where changed.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Set(pLX_ADEMOD_Agc pContext, UINT32 options, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Options_Get
**
**  Description:    Sets AGC flags.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to the AGC context,
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Standard_Set(pLX_ADEMOD_Agc pContext, UINT32 VideoStandard);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Gain_Set
**
**  Description:    Sets video gain thresholds.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Min video gain	32 Least Significant bits
**						pValue[1]	Min video gain	8 Most Significant bits
**						pValue[2]	Max video gain	32 Least Significant bits
**						pValue[3]	Max video gain	8 Most Significant bits
**						pValue[4]	Max RF gain
**						pValue[5]	Min RF gain
**						pValue[6]	Preset RF gain
**						pValue[7]	Max IF gain
**						pValue[8]	Min IF gain
**						pValue[9]	Preset IF gain
**						pValue[10]	LPF gain error negative
**						pValue[11]	LPF gain error positive
**						pValue[12]	Instability monitor gain adjustment
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Set(pLX_ADEMOD_Agc pContext, UINT32 *pGain, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Gain_Get
**
**  Description:    Returns actual video gain.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						pValue[0]	Min AGC gain	32 Least Significant bits
**						pValue[1]	Min AGC gain	8 Most Significant bits
**						pValue[2]	Max AGC gain	32 Least Significant bits
**						pValue[3]	Max AGC gain	8 Most Significant bits
**						pValue[4]	Max RF gain
**						pValue[5]	Min RF gain
**						pValue[6]	Preset RF gain
**						pValue[7]	Max IF gain
**						pValue[8]	Min IF gain
**						pValue[9]	Preset IF gain
**						pValue[10]	LPF gain error negative	loop gain for the RF-AGC accumulator
**									when the LPF gain error is negative
**						pValue[11]	LPF gain error positive	loop gain for the RF-AGC accumulator
**									when the LPF gain error is positive
**						pValue[12]	Instability monitor gain adjustment	multiplication factor (gain value)
**									applied to the instability monitor error calculation
**						pValue[13]	AGC actual gain	32 Least Significant bits
**						pValue[14]	AGC actual gain	8 Most Significant bits
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Get(pLX_ADEMOD_Agc pContext, UINT32 *pGain, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_GainThrsh_Set
**
**  Description:    Sets AGC min/max gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - explicit min threshold, when mode is "none",
**						      pValue[3] - explicit max threshold, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainThrsh_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_GainThrsh_Get
**
**  Description:    Returns AGC min/max gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - explicit min threshold, when mode is "none",
**						      pValue[3] - explicit max threshold, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainThrsh_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_GainPreset_Set
**
**  Description:    Sets AGC preset gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - explicit preset gain, when mode is "none",
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainPreset_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_GainPreset_Get
**
**  Description:    Returns AGC preset gain.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - explicit preset gain, when mode is "none",
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainPreset_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RFIIR_Bndw_Set
**
**  Description:    Sets AGC RF IIR bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/narrow/wide/max/none,
**						      pValue[1] - actual bandwidth, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RFIIR_Bndw_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RFIIR_Bndw_Get
**
**  Description:    Sets AGC RF IIR bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/narrow/wide/max/none,
**						      pValue[1] - actual bandwidth, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RFIIR_Bndw_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_Speed_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector speed.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual speed, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_Speed_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RF_Speed_Get
**
**  Description:    Returns AGC RF speed.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Speed_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_Decay_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual decay, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_Decay_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_FastDecay_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector fast decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual fast decay, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_FastDecay_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_DecayWindow_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector decay window.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual window, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_DecayWindow_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_DecayThresh_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector decay threshold.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - actual decay threshold, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_DecayThresh_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_DecFactor_Set
**
**  Description:    Sets AGC RF dec factor.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/low/high/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_DecFactor_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_InstabGainAdj_Set
**
**  Description:    Sets AGC RF Instability gain adjustment.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_InstabGainAdj_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RF_Bndw_Set
**
**  Description:    Sets AGC RF bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/narrow/wide/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Bndw_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RF_Speed_Set
**
**  Description:    Sets AGC RF speed.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Speed_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RF_Backoff_Set
**
**  Description:    Sets AGC RF backoff.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/low/high/max/none,
**						      pValue[1] - actual value, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Backoff_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RF_Backoff_Get
**
**  Description:    Sets AGC RF backoff.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Backoff_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Dither_Set
**
**  Description:    Sets AGC preset gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF,
**						      pValue[1] - DAC_Dither,
**						      pValue[2] - Dither_LFSR,
**						      pValue[3] - Dither_init,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Dither_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_InstabThresh_Set
**
**  Description:    Sets AGC instability threshold.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF,
**						      pValue[1] - instability threshold.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_InstabThresh_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Parameters_Set
**
**  Description:    Sets miscellaneous AGC parameters.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value:
**      Legacy AGC parameters
**      pValue[0]	Back-porch Reference     Back porch reference value used by the CVBS Gain Adjust.
**      pValue[1]	Start line        First line of the safe AGC region. Used in Host mode only.
**      pValue[2]	End line          Last line of the safe AGC region. Used in Host mode only.
**      RF AGC parameters
**      pValue[3]	A1 coefficient    A1 coefficient for the 1st order LPF filter in theRF-AGC loop.
**                  of LPF filter
**      pValue[4]	B0 coefficient    B0 coefficient for the 1st order LPF filter
**                  of LPF filter     in the RF-AGC loop.
**      pValue[5]	Fast Decay        Multiplication factor (shift) for fast decay for the
**                                    Leaky Peak Detector.
**      pValue[6]	Decay threshold   Sets the threshold at which the Leaky Peak Detector
**                                    transitions out of fast decay mode.
**      pValue[7]	Window            Sets the window (expressed as a number of clock cycles)
**                                    from which the Leaky Peak Detector transitions to fast decay mode.
**      pValue[8]	Reference level   Sets the reference level for the Peak Detector block within the
**                                    RF-AGC loop.
**      pValue[9]	A1 Coefficient    Sets the A1 coefficient for the 1st order bandwidth IIR filter in the RF-AGC loop.
**                  of IIR filter     This coefficient has eight 4-bit shifts concatenated.
**      pValue[10]	B0 coefficient    Sets the B0 coefficient for the 1st order IIR filter
**                                    in the RF-AGC loop.
**      pValue[11]	Attack 1          In conjunction with Attack 2, sets the attack value used by the Leaky Peak Detector
**                                    in the RF-AGC loop.
**      pValue[12]	Attack 2          In conjunction with Attack 1, sets the attack value used by the Leaky Peak Detector
**                                    in the RF-AGC loop.
**      pValue[13]	Decimation factor	Sets the decimation factor for samples sent to the RF-AGC loop Low Pass Filter.
**                                    Its value should be set to the nearest multiple of 7 such that the resulting data rate
**                                    is about 242 kHz.
**      Audio AGC parameters
**      pValue[14]	Non-AGC gain      Sets the gain value, when the AGC loop is disabled
**
**      pValue[15]	Magnitude error   Sets the coefficient for the magnitude error moving average filter within the audio
**                                    AGC loop.
**      pValue[16]	Magnitude reference	Sets the target (reference) magnitude level for the AGC loop.
**
**      pValue[17]	Fraction error    Sets the fraction of the calculated error used by the AGC loop.
**
**      pValue[18]	Update rate	      Sets the update rate of the AGC error correction loop.
**                                    At this rate (per 16 samples) a new gain value is output from the audio AGC loop.
**      pValue[19]	Soft Clipping threshold	   Sets the soft clipping threshold for the signal after AGC
**                                    has been applied.
**      pValue[20]	Peak Decay rate	  Sets the decay amount per clock cycle of the AGC Peak Detect.
**
**      pValue[21]	Peak Adjustment	  4 = 1/16
**      					          3 = 1/8
**      					          2 = 1/4
**      					          1 = Move to peak on every sample
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Parameters_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Parameters_Get
**
**  Description:    Returns actual AGC parameters.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**      Legacy AGC parameters
**      pValue[0]	Back-porch Reference     Back porch reference value used by the CVBS Gain Adjust.
**      pValue[1]	Start line        First line of the safe AGC region. Used in Host mode only.
**      pValue[2]	End line          Last line of the safe AGC region. Used in Host mode only.
**      RF AGC parameters
**      pValue[3]	A1 coefficient    A1 coefficient for the 1st order LPF filter in theRF-AGC loop.
**                  of LPF filter
**      pValue[4]	B0 coefficient    B0 coefficient for the 1st order LPF filter
**                  of LPF filter     in the RF-AGC loop.
**      pValue[5]	Fast Decay        Multiplication factor (shift) for fast decay for the
**                                    Leaky Peak Detector.
**      pValue[6]	Decay threshold   Sets the threshold at which the Leaky Peak Detector
**                                    transitions out of fast decay mode.
**      pValue[7]	Window            Sets the window (expressed as a number of clock cycles)
**                                    from which the Leaky Peak Detector transitions to fast decay mode.
**      pValue[8]	Reference level   Sets the reference level for the Peak Detector block within the
**                                    RF-AGC loop.
**      pValue[9]	A1 Coefficient    Sets the A1 coefficient for the 1st order bandwidth IIR filter in the RF-AGC loop.
**                  of IIR filter     This coefficient has eight 4-bit shifts concatenated.
**      pValue[10]	B0 coefficient    Sets the B0 coefficient for the 1st order IIR filter
**                                    in the RF-AGC loop.
**      pValue[11]	Attack 1          In conjunction with Attack 2, sets the attack value used by the Leaky Peak Detector
**                                    in the RF-AGC loop.
**      pValue[12]	Attack 2          In conjunction with Attack 1, sets the attack value used by the Leaky Peak Detector
**                                    in the RF-AGC loop.
**      pValue[13]	Decimation factor	Sets the decimation factor for samples sent to the RF-AGC loop Low Pass Filter.
**                                    Its value should be set to the nearest multiple of 7 such that the resulting data rate
**                                    is about 242 kHz.
**      Audio AGC parameters
**      pValue[14]	Non-AGC gain      Sets the gain value, when the AGC loop is disabled
**
**      pValue[15]	Magnitude error   Sets the coefficient for the magnitude error moving average filter within the audio
**                                    AGC loop.
**      pValue[16]	Magnitude reference	Sets the target (reference) magnitude level for the AGC loop.
**
**      pValue[17]	Fraction error    Sets the fraction of the calculated error used by the AGC loop.
**
**      pValue[18]	Update rate	      Sets the update rate of the AGC error correction loop.
**                                    At this rate (per 16 samples) a new gain value is output from the audio AGC loop.
**      pValue[19]	Soft Clipping threshold	   Sets the soft clipping threshold for the signal after AGC
**                                    has been applied.
**      pValue[20]	Peak Decay rate	  Sets the decay amount per clock cycle of the AGC Peak Detect.
**
**      pValue[21]	Peak Adjustment	  4 = 1/16
**      					          3 = 1/8
**      					          2 = 1/4
**      					          1 = Move to peak on every sample
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Parameters_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Init
**
**  Description:    Initializes AGC hardware.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Init(pLX_ADEMOD_Agc pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Start
**
**  Description:    Starts AGC block.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode   - Invalid mode
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Start(pLX_ADEMOD_Agc pContext, Handle_t hUser);

#if defined( __cplusplus )
}
#endif


#endif  //_ADEMOD_M14_AGC_H_