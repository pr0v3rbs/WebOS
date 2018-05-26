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
**  Name: ademod_m14_agc_dig.h
**
**  Description:   ABB M14 Digital AGC block.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Line_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Start
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Line_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Standard_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Start
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
#ifndef _ADEMOD_M14_AGC_DIG_H_
#define _ADEMOD_M14_AGC_DIG_H_

#include "ademod_common.h"
#include "ademod_common_agc.h"


#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

/*
**  LGT10 Digital AGC Update sticky flags
*/
#define LX_ADEMOD_UFLAG_DIGAGC_MODTYPE_USR			(0x00000001L)
//#define LX_ADEMOD_UFLAG_DIGAGC_MEASUREMENT_USR		(0x00000002L)
//#define LX_ADEMOD_UFLAG_DIGAGC_OPTIONS_USR			(0x00000004L)
#define LX_ADEMOD_UFLAG_DIGAGC_VSYNCTILT_USR		(0x00000008L)
#define LX_ADEMOD_UFLAG_DIGAGC_LVLDEPCTRL_USR		(0x00000010L)
#define LX_ADEMOD_UFLAG_DIGAGC_COHERENT_MODE_USR	(0x00000020L)
#define LX_ADEMOD_UFLAG_DIGAGC_RFGAIN_USR			(0x00000100L)
#define LX_ADEMOD_UFLAG_DIGAGC_IFGAIN_USR			(0x00000200L)
#define LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_USR			(0x00000400L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPD_SPEED_USR		(0x00000800L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAY_USR		(0x00001000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPD_FASTDECAY_USR	(0x00002000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAYWIN_USR	(0x00004000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPD_DECAYTHRSH_USR	(0x00008000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPFBNDW_USR			(0x00010000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_USR		(0x00020000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPFBNDW_COH_USR		(0x00040000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_COH_USR	(0x00080000L)
#define LX_ADEMOD_UFLAG_DIGAGC_BACKOFF_USR			(0x00100000L)
#define LX_ADEMOD_UFLAG_DIGAGC_BACKOFF_COH_USR		(0x00200000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPFSPEED_HYBR_USR	(0x00400000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LVLDEP_USR			(0x00800000L)
#define LX_ADEMOD_UFLAG_DIGAGC_DECRATIO_USR		(0x01000000L)
#define LX_ADEMOD_UFLAG_DIGAGC_RFGAIN_THR_USR		(0x02000000L)
#define LX_ADEMOD_UFLAG_DIGAGC_IFGAIN_THR_USR		(0x04000000L)
#define LX_ADEMOD_UFLAG_DIGAGC_DIGGAIN_THR_USR		(0x08000000L)
#define LX_ADEMOD_UFLAG_DIGAGC_LPDNBND_THR_USR		(0x10000000L)
#define LX_ADEMOD_UFLAG_DIGAGC_MIXMODE_USR			(0x20000000L)
#define LX_ADEMOD_UFLAG_DIGAGC_HYBMODE_TARG_USR	(0x40000000L)

/*
**  LGT10 Digital AGC Context
*/

typedef struct LX_ADEMOD_DigAgc_tag {
	Handle_t		hDevice;   //jeongpil.yun for M14
	UINT32			flags;
	UINT32			update;
	UINT32			factory;
	INT64			max_gain_rf;
	INT64			min_gain_rf;
	INT64			max_gain_if;
	INT64			min_gain_if;
	INT64			max_gain_dig;
	INT64			min_gain_dig;
	INT64			max_gain_dig_nbnd;
	INT64			min_gain_dig_nbnd;
	INT64			pres_gain_rf;
	INT64			pres_gain_if;
	INT64			pres_gain_dig;
	INT64			pres_gain_dig_nbnd;
	int				backoff;
	int				backoff_coh;
	int				narrowband_thr1;
	int				narrowband_thr2;
	int				REF;
	UINT32			target;
	UINT32			decimation_ratio;
	UINT32			fast_decay;
	UINT32			decay_thr;
	UINT32			win;
	UINT32			ref;
	UINT32			ref_coh;
	UINT32			leaky_pd_attack;
	UINT32			leaky_pd_decay;
	UINT32			leaky_pd_decaywin;
	UINT32			Leaky_Decay;
	UINT32			lpf_bw;
	UINT32			lpf_bw_coh;
	INT16			attack_1;
	INT16			attack_2;
	INT16			LPF_Shift1;
	INT16			LPF_Shift2;
	INT16			LPF_Shift1_coh;
	INT16			LPF_Shift2_coh;
	UINT16			LPFGain_poserr_RF;
	UINT16			LPFGain_poserr_RF_Shift;
	UINT16			LPFGain_negerr_RF;
	UINT16			LPFGain_negerr_RF_Shift;
	UINT16			LPFGain_poserr_IF;
	UINT16			LPFGain_poserr_IF_Shift;
	UINT16			LPFGain_negerr_IF;
	UINT16			LPFGain_negerr_IF_Shift;
	UINT16			LPFGain_poserr_Dig;
	UINT16			LPFGain_poserr_Dig_Shift;
	UINT16			LPFGain_negerr_Dig;
	UINT16			LPFGain_negerr_Dig_Shift;
	UINT16			LPFGain_poserr_RF_coh;
	UINT16			LPFGain_poserr_RF_Shift_coh;
	UINT16			LPFGain_negerr_RF_coh;
	UINT16			LPFGain_negerr_RF_Shift_coh;
	UINT16			LPFGain_poserr_IF_coh;
	UINT16			LPFGain_poserr_IF_Shift_coh;
	UINT16			LPFGain_negerr_IF_coh;
	UINT16			LPFGain_negerr_IF_Shift_coh;
	UINT16			LPFGain_poserr_Dig_coh;
	UINT16			LPFGain_poserr_Dig_Shift_coh;
	UINT16			LPFGain_negerr_Dig_coh;
	UINT16			LPFGain_negerr_Dig_Shift_coh;
	UINT16			LPFGain_poserr_RF_hybr;
	UINT16			LPFGain_poserr_RF_Shift_hybr;
	UINT16			LPFGain_poserr_Dig_hybr;
	UINT16			LPFGain_poserr_Dig_Shift_hybr;
	UINT16			LPFGain_negerr_Dig_hybr;
	UINT16			LPFGain_negerr_Dig_Shift_hybr;
	UINT32			resp_time_poserr_RF;
	UINT32			resp_time_negerr_RF;
	UINT32			resp_time_poserr_IF;
	UINT32			resp_time_negerr_IF;
	UINT32			resp_time_poserr_Dig;
	UINT32			resp_time_negerr_Dig;
	UINT32			resp_time_poserr_RF_coh;
	UINT32			resp_time_negerr_RF_coh;
	UINT32			resp_time_poserr_IF_coh;
	UINT32			resp_time_negerr_IF_coh;
	UINT32			resp_time_poserr_Dig_coh;
	UINT32			resp_time_negerr_Dig_coh;
	UINT32			resp_time_poserr_RF_hybr;
	UINT32			resp_time_negerr_RF_hybr;
	UINT32			resp_time_poserr_IF_hybr;
	UINT32			resp_time_negerr_IF_hybr;
	UINT32			resp_time_poserr_Dig_hybr;
	UINT32			resp_time_negerr_Dig_hybr;
	UINT8			rf_mode;
	UINT8			if_mode;
	UINT8			dig_mode;
	UINT8			dig_mode_nbnd;
	UINT8			cvbs_mode;
	UINT8			rf_gain;
	UINT8			if_gain;
	UINT8			dig_gain;
	UINT8			dig_gain_nbnd;
	UINT8			speed;
	UINT8			speed_coherent;
	UINT8			speed_hybrid;
	UINT8			backoff_mode;
	UINT8			backoff_mode_coh;
	UINT8			narrowband_mode_thr;
} LX_ADEMOD_DigAgc, *pLX_ADEMOD_DigAgc;



/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Factory_Set
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Factory_Set(pLX_ADEMOD_DigAgc pContext);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Flags_Set
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Set(pLX_ADEMOD_DigAgc pContext, UINT32 Flags, UINT32 mask);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Flags_Get
**
**  Description:    Returns AGC flags.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Flags_Get(pLX_ADEMOD_DigAgc pContext, UINT32* pFlags);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_MixedMode_Set
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_MixedMode_Set(pLX_ADEMOD_DigAgc pContext, UINT32 Mode, Handle_t hUser, BOOL Update);

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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_Mode_Set(pLX_ADEMOD_DigAgc pContext, UINT32 InputMode);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Standard_Set
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Standard_Set(pLX_ADEMOD_DigAgc pContext, UINT32 VideoStandard);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Init
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Init(pLX_ADEMOD_DigAgc pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Start
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Start(pLX_ADEMOD_DigAgc pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_GainThrsh_Set
**
**  Description:    Sets AGC min/max gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF/Digital,
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_GainThrsh_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_GainThrsh_Get
**
**  Description:    Returns AGC min/max gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF/Digital,
**						      pValue[1] - threshold mode: nominal/min/max/none,
**						      pValue[2] - explicit min threshold (LSB), when mode is "none",
**						      pValue[3] - explicit min threshold (MSB), when mode is "none",
**						      pValue[4] - explicit max threshold (LSB), when mode is "none".
**						      pValue[5] - explicit max threshold (MSB), when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_GainThrsh_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_GainPreset_Set
**
**  Description:    Sets Digital AGC preset gain mode / value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC option: RF/IF/Digital,
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_GainPreset_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_CVBS_Mode_Set
**
**  Description:    Sets Digital AGC CVBS.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - CVBS mode: 1- non-coherent/2 -coherent (not forced)/3 - coherent (forced),
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_CVBS_Mode_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_CVBS_Mode_Get
**
**  Description:    Returns Digital AGC CVBS mode (coherent/no coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - CVBS mode: 1- non-coherent/2 -coherent (not forced)/3 - coherent (forced),
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_CVBS_Mode_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_Speed_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector speed.
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_Speed_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Speed_Get
**
**  Description:    Returns Digital AGC speed (non-coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Speed_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedCoherent_Get
**
**  Description:    Returns Digital AGC speed (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedCoherent_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedHybrid_Get
**
**  Description:    Returns Digital AGC speed (hybrid).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedHybrid_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_Target_Set
**
**  Description:    Sets Digital AGC Hybrid mode target value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - when mode = none sets digital backoff in dB,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_Target_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_Target_Get
**
**  Description:    Returns Digital AGC Hybrid mode target value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - target,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_Target_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_DecimationRatio_Set
**
**  Description:    Sets Digital AGC Decimation Ratio value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - when mode = none sets decimation ratio,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_DecimationRatio_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAgc_DecimationRatio_Get
**
**  Description:    Returns Digital AGC Decimation Ratio value.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - decimation ratio,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAgc_DecimationRatio_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_RFLevel_Get
**
**  Description:    Returns Digital AGC RF level.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: 0- RF, 1- IF, 2- Digital,
**							  pValue[1] - RF gain,
**							  pValue[2] - Digital RF gain,
**							  pValue[3] - Digital gain.
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_RFLevel_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_Decay_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector's decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_Decay_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_FastDecay_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector's fast decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_FastDecay_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_DecayWindow_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector decay window.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/short/long/max/none,
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_DecayWindow_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPD_DecayThresh_Set
**
**  Description:    Sets Digital AGC Leaky Peak Detector's decay threshold.
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPD_DecayThresh_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Bndw_Set
**
**  Description:    Sets Digital AGC non-coherent bandwidth.
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Bndw_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Speed_Set
**
**  Description:    Sets Digital AGC speed (non-coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none",
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Speed_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Gain_Get
**
**  Description:    Returns Digital AGC gain.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - DigAGC_Gain register reading,
**						      pValue[1] - DigAGC_Gain in dB*10.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Gain_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_BndwCoherent_Set
**
**  Description:    Sets Digital AGC coherent bandwidth.
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BndwCoherent_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedCoherent_Set
**
**  Description:    Sets Digital AGC speed (coherent).
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - resp_time_negerr_RF, when mode is "none",
**						      pValue[3] - resp_time_poserr_IF, when mode is "none",
**						      pValue[4] - resp_time_negerr_IF, when mode is "none".
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedCoherent_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_SpeedHybrid_Set
**
**  Description:    Sets Digital AGC speed in Hybrid mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/slow/fast/max/none,
**						      pValue[1] - resp_time_poserr_RF, when mode is "none",
**						      pValue[2] - not used,
**						      pValue[3] - not used,
**						      pValue[4] - not used.
**						      pValue[5] - resp_time_poserr_Dig, when mode is "none",
**						      pValue[6] - resp_time_negerr_Dig, when mode is "none".
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_SpeedHybrid_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Backoff_Set
**
**  Description:    Sets Digital AGC backoff (non-coherent).
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Backoff_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_Backoff_Get
**
**  Description:    Returns Digital AGC backoff (non-coherent).
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_Backoff_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Set
**
**  Description:    Sets Narrowband AGC Piece-wise LPFGain thresholds.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - thresh1 register: DigAGC_Ref_noncoh[10:5] -> signed.
**						      pValue[2] - thresh2 register: DigAGC_Ref_noncoh[4:0]  -> unsigned.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Get
**
**  Description:    Returns Narrowband AGC Piece-wise LPFGain thresholds.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
**						      pValue[1] - thresh1 register: DigAGC_Ref_noncoh[10:5] -> signed.
**						      pValue[2] - thresh2 register: DigAGC_Ref_noncoh[4:0]  -> unsigned.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_LPF_NarrowbandThresh_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_BackoffCoherent_Set
**
**  Description:    Sets Digital AGC backoff (coherent).
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BackoffCoherent_Set(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_DigAGC_BackoffCoherent_Get
**
**  Description:    Returns Digital AGC backoff (coherent).
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
LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_BackoffCoherent_Get(pLX_ADEMOD_DigAgc pContext, UINT32 *pValue, Handle_t hUser);


#if defined( __cplusplus )
}
#endif


#endif  //_ADEMOD_M14_AGC_DIG_H_
