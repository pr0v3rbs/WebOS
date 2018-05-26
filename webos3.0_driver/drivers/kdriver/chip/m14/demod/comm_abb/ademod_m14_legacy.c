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
**  Name: ademod_m14_legacy.c
**
**  Description:    ABB M14 Legacy Demodulation block.
**
**  Functions
**  Implemented:   LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Factory_Set
**                    	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Standard_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Init
**                 		LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Start
**
**  References:     
**
**  Exports:        	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Factory_Set
** 				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Standard_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Set
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Get
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set
**				LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Init
**                  	LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Start
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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include <asm/div64.h> //do_div

#include "ademod_m14_config.h"

#include "ademod_m14_demod.h"
#include "ademod_m14_video.h"
#include "ademod_m14_legacy.h"
#include "ademod_m14_regaddress.h"

/*
**  Group Delay LUTs
*/
static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_SYSTEM_M_N_LUT =
	{   -2,   45,  -89,  217,  674,  315, -238,  178,  -80,   17,
	    13,  -11,   -7,   17,  -15,    5,    3,   -7,    4,   -1  };

static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_SYSTEM_B_G_LUT =
	{  -18,  -62,  -97,  666,  543, -142,    4,  231, -258,  238,
	  -154,   89,  -35,   13,   -5,    9,  -17,   19,  -16,   11  };

static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_SYSTEM_D_K_LUT =
	{  -12, -136,   43,  576,  662, -344,  274,  -36,  -32,   86,
       -83,   74,  -56,   41,  -32,   23,  -17,   11,   -8,    5  };

static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_SYSTEM_I_LUT =
	{  -12, -136,   43,  576,  662, -344,  274,  -36,  -32,   86,
       -83,   74,  -56,   41,  -32,   23,  -17,   11,   -8,    5  };

static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_SYSTEM_L_LUT =
	{   38,  -27,    8,   17,  985,   69,  -88,   98,  -98,  103,
	   -88,   70,  -53,   40,  -31,   25,  -23,   21,  -20,   19  };

static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_SYSTEM_L1_LUT =
	{  0x0015, 0x07F7, 0x0029, 0x0793, 0x03F1, 0x007C, 0x07BB, 0x0022, 0x07CD, 0x0035,
	   0x07D0, 0x0027, 0x07DE, 0x001F, 0x07E4, 0x0019, 0x07EA, 0x0014, 0x07EE, 0x0011  };

static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_SYSTEM_D_K_CHINA_LUT =
	{  -12, -136,   43,  576,  662, -344,  274,  -36,  -32,   86,
       -83,   74,  -56,   41,  -32,   23,  -17,   11,   -8,    5  };

static const LX_ADEMOD_GroupDelayLUT LX_ADEMOD_GROUP_DELAY_BYPASS_LUT =
	{    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
         0,    0,    0,    0, 1023,    0,    0,    0,    0,    0  };

/*
**  Local Function Prototypes - not available for external access.
*/

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Reset(pLX_ADEMOD_LegacyDemod pContext, BOOL on_off, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_GroupDelay_Set(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_M_N_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_B_G_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_D_K_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_H_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_L_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_L1_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_I_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_KickVSyncSlicer(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_DAC_Start(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_SyncDecoder_Start(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pVideo_freq, UINT32 flags, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_GroupDelayLUT_Default(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT);

/*
**  API implementation.
*/

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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Factory_Set(pLX_ADEMOD_LegacyDemod pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// Video DAC
	pContext->video_dac.clipper       = LX_ADEMOD_LEG_DAC_CLIPPER_FCT;
	pContext->video_dac.gain          = LX_ADEMOD_LEG_DAC_GAIN_FCT;
	pContext->video_dac.offset        = LX_ADEMOD_LEG_DAC_OFFS_FCT;
	pContext->video_dac.dither		  = (pContext->factory & LX_ADEMOD_FFLAG_DBG_1) ? LX_ADEMOD_LEG_DAC_DITH_FCT : 0;

	// SYNC Decoder
	pContext->sync_dec.attack         = LX_ADEMOD_SYNCD_ATTACK_FCT;
	pContext->sync_dec.decay          = LX_ADEMOD_SYNCD_DECAY_FCT;
	pContext->sync_dec.decay_thresh   = LX_ADEMOD_SYNCD_DECAYTHR_FCT;
	pContext->sync_dec.decay_window   = LX_ADEMOD_SYNCD_DECAYWIN_FCT;
	pContext->sync_dec.fast_decay     = LX_ADEMOD_SYNCD_FASTDECAY_FCT;
	pContext->sync_dec.peak_thr_min   = LX_ADEMOD_SYNCD_PEAKMIN_FCT;
	pContext->sync_dec.peak_thr_max   = LX_ADEMOD_SYNCD_PEAKMAX_FCT;
	pContext->sync_dec.ser_thresh     = LX_ADEMOD_SYNCD_SER_THR_FCT;
	pContext->sync_dec.ser_lockout    = LX_ADEMOD_SYNCD_SER_LOCKOUT_FCT;
	pContext->sync_dec.ser_max_dur    =	LX_ADEMOD_SYNCD_SER_MAX_DUR_FCT;

	pContext->sync_dec.bp_avg_sel        = LX_ADEMOD_SYNCD_BP_AVG_SEL_FCT;
	pContext->sync_dec.bp_ser_win_start  = LX_ADEMOD_SYNCD_BP_SER_WIN_START_FCT;
	pContext->sync_dec.bp_ser_win_period = LX_ADEMOD_SYNCD_BP_SER_WIN_PERIOD_FCT;
	pContext->sync_dec.bp_ser_win_wgt    = LX_ADEMOD_SYNCD_BP_SER_WIN_WGT_FCT;

	pContext->sync_dec.hs.tip_avg_sel   = LX_ADEMOD_SYNCTIP_AVG_SEL_FCT;
	pContext->sync_dec.hs.tip_ser_win_start = LX_ADEMOD_SYNCTIP_SER_WIN_START_FCT;
	pContext->sync_dec.hs.tip_ser_win_period = LX_ADEMOD_SYNCTIP_SER_WIN_PERIOD;
	pContext->sync_dec.hs.lock_thr    = LX_ADEMOD_SYNCD_HS_LOCK_THR_FCT;
	pContext->sync_dec.hs.unlock_thr  = LX_ADEMOD_SYNCD_HS_UNLOCK_THR_FCT;
	pContext->sync_dec.hs.tc_sel      = LX_ADEMOD_SYNCD_HS_TCSEL_FCT;
	pContext->sync_dec.hs.min_sync_height =	LX_ADEMOD_SYNCD_HS_MIN_SYNC_HEIGHT_FCT;
	pContext->sync_dec.hs.height_th =	LX_ADEMOD_SYNCD_HS_SYNC_HEIGHT_TH_FCT;
	pContext->sync_dec.hs.min_thr     = LX_ADEMOD_SYNCD_HS_MIN_THR_FCT;
	pContext->sync_dec.hs.max_thr     = LX_ADEMOD_SYNCD_HS_MAX_THR_FCT;
	pContext->sync_dec.hs.min_cnt_thr = LX_ADEMOD_SYNCD_HS_MIN_CNT_THR_FCT;
	pContext->sync_dec.hs.max_cnt_thr = LX_ADEMOD_SYNCD_HS_MAX_CNT_THR_FCT;
	pContext->sync_dec.hs.lpf_coeffb  = LX_ADEMOD_SYNCD_HS_LPF_COEFFB_FCT;
	pContext->sync_dec.hs.safe_agc_line_start = LX_ADEMOD_SYNCD_HS_SAFE_AGC_LINE_START_FCT;
	pContext->sync_dec.hs.safe_agc_line_stop  = LX_ADEMOD_SYNCD_HS_SAFE_AGC_LINE_STOP_FCT;
	pContext->sync_dec.hs.safe_agc_mask       = LX_ADEMOD_SYNCD_HS_SAFE_AGC_MASK_FCT;

	pContext->sync_dec.hs.max_err     = LX_ADEMOD_SYNCD_HS_MAX_ERR_FCT;
	pContext->sync_dec.hs.stable_thr  = LX_ADEMOD_SYNCD_HS_STABLE_THR_FCT;
	pContext->sync_dec.hs.unstbl_thr  = LX_ADEMOD_SYNCD_HS_UNSTBL_THR_FCT;

	pContext->sync_dec.hs.adj_sel_fine    = LX_ADEMOD_HSLICE_ADJSELFINE_FCT;
	pContext->sync_dec.hs.adj_sel_coarse  = LX_ADEMOD_HSLICE_ADJSELCOARSE_FCT;

	pContext->sync_dec.vs.lock_thr    = LX_ADEMOD_SYNCD_VS_LOCK_THR_FCT;
	pContext->sync_dec.vs.unlock_thr  = LX_ADEMOD_SYNCD_VS_UNLOCK_THR_FCT;
	pContext->sync_dec.vs.tc_sel      = LX_ADEMOD_SYNCD_VS_TCSEL_FCT;

	pContext->sync_dec.vs.max_err     = LX_ADEMOD_SYNCD_VS_MAX_ERR_FCT;
	pContext->sync_dec.vs.stable_thr  = LX_ADEMOD_SYNCD_VS_STABLE_THR_FCT;
	pContext->sync_dec.vs.unstbl_thr  = LX_ADEMOD_SYNCD_VS_UNSTBL_THR_FCT;

	pContext->sync_dec.h_slice.level_sel       = LX_ADEMOD_HSLICE_LVL_SEL_FCT;
	pContext->sync_dec.h_slice.level_sel_ser   = LX_ADEMOD_HSLICE_LVL_SELSER_FCT;
	pContext->sync_dec.h_slice.ser_exit_line   = LX_ADEMOD_HSLICE_SEREXITLINE_FCT;

	pContext->sync_dec.v_slice.level_sel       = LX_ADEMOD_VSLICE_LVL_SEL_FCT;
	pContext->sync_dec.v_slice.thresh          = LX_ADEMOD_VSLICE_THRESH_FCT;
	pContext->sync_dec.v_slice.level_sel_bp_st = LX_ADEMOD_VSLICE_LVL_SELBPST_FCT;


	// initialize mixing values
	pContext->sync_dec.hs.period = 0;
	pContext->sync_dec.hs.rate = 0;
	pContext->sync_dec.hs.tip_win_start = 1;
	pContext->sync_dec.hs.tip_duration = 0;
	pContext->sync_dec.hs.tip_samples = 0;
	pContext->sync_dec.frontporch_duration = 0;
	pContext->sync_dec.backporch_duration = 0;
	pContext->sync_dec.backporch_samples = 0;
	pContext->sync_dec.breezeway_duration = 0;
	pContext->sync_dec.breezeway_samples = 0;
	pContext->sync_dec.burst_duration = 0;
	pContext->sync_dec.burst_samples = 0;
	pContext->sync_dec.line_per_frame = 525;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  VideoStandard  - The video standard,
**                  AudioStandard  - The audio standard.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Standard_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 VideoStandard, UINT32 AudioStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// clear China DK flag
	pContext->flags &= ~LX_ADEMOD_FFLAG_DK_CHINA_EN;
	pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_SRCSEL_EN;  //jeongpil.yun - for brazil
	pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN;  //jeongpil.yun - for brazil
	pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_NEGMOD_EN; // negative modulation

//	pContext->flags &= ~LX_ADEMOD_FFLAG_SYNCD_SRCSEL_EN; //jeongpil.yun - for brazil, blocked
//	pContext->flags &= ~LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN; //jeongpil.yun - for brazil, blocked

	switch (VideoStandard)
	{
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M_J:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_433:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_50:
			// update video standard
			pContext->video_std = VideoStandard;
			pContext->sync_dec.hs.lock_thr    = 0xB;
			pContext->sync_dec.hs.unlock_thr  = 0x1FF;
			// set UPDATE flag
			pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
		break;
		case LX_ADEMOD_VIDEO_STANDARD_PAL_B:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_G:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_H:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_I:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_D:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_K:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_M:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_N:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_60:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_NC:
			pContext->sync_dec.hs.lock_thr    = 0xB;
			pContext->sync_dec.hs.unlock_thr  = 0x1FF;
			// update video standard
			pContext->video_std = VideoStandard;
			// set UPDATE flag
			pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
		break;
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_B:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_G:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_H:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_D:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K:
			// update video standard
			pContext->video_std = VideoStandard;
			pContext->sync_dec.hs.lock_thr    = 0x10;
			pContext->sync_dec.hs.unlock_thr  = 0x1FF;
			// set UPDATE flag
			pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
		break;

 		case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
 			// update video standard
 			pContext->video_std = VideoStandard;
 			// set UPDATE flag
			pContext->sync_dec.hs.lock_thr    = 0xB;
			pContext->sync_dec.hs.unlock_thr  = 0x1FF;
 			pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
 			pContext->flags |= LX_ADEMOD_FFLAG_DK_CHINA_EN;
 		break;


		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
			// update video standard
			pContext->video_std = VideoStandard;
			pContext->sync_dec.hs.lock_thr    = 0x10;
			pContext->sync_dec.hs.unlock_thr  = 0x1FF;
			pContext->flags |=  LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN; // Bug1088: set sync decoder to use phase rotator input in order to handle > 107% overmod
			pContext->flags &= ~LX_ADEMOD_FFLAG_SYNCD_NEGMOD_EN; // positive modulation
			// set UPDATE flag
			pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
		break;

		default:
			status = retErrBadParam;
		break;

	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_SyncOptions_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_SYNC_OPTIONS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_SYNC_OPTIONS_USR;
	}

	pContext->sync_options = pValue[0];


	// this flag is mode is controlled by the API
	//if (pContext->sync_options & LX_ADEMOD_SYNC_OPTION_SRCSEL_EN) {
	//	pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_SRCSEL_EN;
	//}
	//else {
	//	pContext->flags &= ~LX_ADEMOD_FFLAG_SYNCD_SRCSEL_EN;
	//}

	// this flag is mode is controlled by the API
	//if (pContext->sync_options & LX_ADEMOD_SYNC_OPTION_VSYNC_SLICER_EN) {
	//	pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN;
	//}
	//else {
	//	pContext->flags &= ~LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN;
	//}

	if (pContext->sync_options & LX_ADEMOD_SYNC_OPTION_SYNC_HEIGHT_NBP) {
		pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_HEIGHT_NBP;
	}
	else {
		pContext->flags &= ~LX_ADEMOD_FFLAG_SYNCD_HEIGHT_NBP;
	}

	if (pContext->sync_options & LX_ADEMOD_SYNC_OPTION_SYNC_AGCMASKOVR_EN) {
		pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_AGCMASKOVR;
	}
	else {
		pContext->flags &= ~LX_ADEMOD_FFLAG_SYNCD_AGCMASKOVR;
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_SyncOptions_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->sync_options;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Flags_Set
**
**  Description:    Sets video processing flags.
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Flags_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 Flags, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 mask = 0;

	if (!pContext) {
		return retErrBadContext;
	}
    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	mask = pContext->flags ^ Flags;
	pContext->flags = Flags;
	if (mask) {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	if (Update) {
		if (mask & LX_ADEMOD_FFLAG_DAC_POSTSAW_EN) {
			UINT8 DAC_En_Ctrl = (UINT8)(
				(((pContext->flags & LX_ADEMOD_FFLAG_AUD_UPSAMPLE_EN)      ? 1 : 0) << 0) +
				(((pContext->flags & LX_ADEMOD_FFLAG_VID_UPSAMPLE_EN)      ? 1 : 0) << 1) +
				(((pContext->flags & LX_ADEMOD_FFLAG_AUD_LOWIF_EN)         ? 1 : 0) << 2) +
				(((pContext->flags & LX_ADEMOD_FFLAG_VID_LOWIF_EN)         ? 1 : 0) << 3) +
				(((pContext->flags & LX_ADEMOD_FFLAG_DAC_POSTSAW_EN)       ? 1 : 0) << 4)
			);
			ADEMOD_WriteSub( hUser, DAC_EN_CTRL, DAC_En_Ctrl, 1);
		}

		if (mask & LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN) {
			UINT8 SyncD_ctrl = (UINT8)(
				(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_NEGMOD_EN)      ? 1 : 0) << 0) +
				(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_SRCSEL_EN)      ? 1 : 0) << 1) +
				(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN)      ? 1 : 0) << 2)
			);
			ADEMOD_WriteSub( hUser, SYNCD_CTRL, SyncD_ctrl, 1);
		}
	}
    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_MixedMode_Set
**
**  Description:    Sets audio processing mixed mode.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  Mode         - 0- wideband mode,
**								   1- mixed mode,
**								   2- narrowband mode,
**								   3- hybrid mode.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_MixedMode_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 Mode, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_DAC_MIXMODE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_DAC_MIXMODE_USR;
	}

	if (Mode == LX_ADEMOD_AGC_LPF_MODE_WIDEBAND)
	{
		pContext->flags &= ~LX_ADEMOD_FFLAG_DAC_POSTSAW_EN;
	}
	else
	{// mix/narrowband/hybrid
		pContext->flags |= LX_ADEMOD_FFLAG_DAC_POSTSAW_EN;
	}

	if (Mode == LX_ADEMOD_AGC_LPF_MODE_HYBRID)
	{
		if (!(pContext->factory & LX_ADEMOD_FFLAG_DBG_1)) {
			return retErrBadDevice; // applicable to FM1151 only
		}
		pContext->flags |= LX_ADEMOD_FFLAG_SYNCD_HYBRID_EN;
	}

	if (Update) {
		{
			UINT8 DAC_En_Ctrl = (UINT8)(
				(((pContext->flags & LX_ADEMOD_FFLAG_AUD_UPSAMPLE_EN)      ? 1 : 0) << 0) +
				(((pContext->flags & LX_ADEMOD_FFLAG_VID_UPSAMPLE_EN)      ? 1 : 0) << 1) +
				(((pContext->flags & LX_ADEMOD_FFLAG_AUD_LOWIF_EN)         ? 1 : 0) << 2) +
				(((pContext->flags & LX_ADEMOD_FFLAG_VID_LOWIF_EN)         ? 1 : 0) << 3) +
				(((pContext->flags & LX_ADEMOD_FFLAG_DAC_POSTSAW_EN)       ? 1 : 0) << 4)
			);
			ADEMOD_WriteSub( hUser, DAC_EN_CTRL, DAC_En_Ctrl, 1);
		}

		{
			UINT8 SyncReserved = (UINT8)(
				(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_HYBRID_EN)	   ? 1 : 0) << 1)
			);
			ADEMOD_WriteSub( hUser, SYNCD_RESERVED, SyncReserved, 1);
		}
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Flags_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32* pFlags)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pFlags = pContext->flags;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Gain_Set
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_GAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_GAIN_USR;
	}

	pContext->video_dac.gain = pValue[0];

	if (Update) {
		status = ADEMOD_WriteSub( hUser, VIDEOGAIN, pContext->video_dac.gain, 2);
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}



/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Gain_Get
**
**  Description:    Returns actual video gain.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context,
**                  pValue       - Pointer to the value:
**						pValue[0]	Video gain
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Gain_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->video_dac.gain;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_GAIN_RC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_GAIN_RC_USR;
	}

	pContext->rcMode = pValue[0];

	if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
		pContext->video_rc.gain = pValue[1];
	else
		pContext->video_rc_intpl.gain = pValue[1];

	if (Update) {
		ADEMOD_WriteSub( hUser, SEL_RC, pContext->rcMode, 1);
		
		if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
			ADEMOD_WriteSub( hUser, VIDEOGAIN_RC, pContext->video_rc.gain, 2);
		else
			ADEMOD_WriteSub( hUser, VIDEOGAIN_INTPL, pContext->video_rc_intpl.gain, 2);
		
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Gain_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	pValue[0] = pContext->rcMode;

	
	if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
		pValue[1] = pContext->video_rc.gain;
	else
		pValue[1] = pContext->video_rc_intpl.gain;
		

	return status;
}



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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_OFFS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_OFFS_USR;
	}

	pContext->video_dac.offset = pValue[0];

	if (Update) {
		status = ADEMOD_WriteSub( hUser, VIDEOOFFSET, pContext->video_dac.offset, 2);
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Offset_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->video_dac.offset;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_OFFS_RC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_OFFS_RC_USR;
	}

	pContext->rcMode = pValue[0];

	if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
		pContext->video_rc.offset = pValue[1];
	else
		pContext->video_rc_intpl.offset = pValue[1];
		

	if (Update) {
		ADEMOD_WriteSub( hUser, SEL_RC, pContext->rcMode, 1);

		if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
		{
			ADEMOD_WriteSub( hUser, VIDEOOFFSET_RC, pContext->video_rc.offset, 2);
			DEMOD_PRINT("pContext->video_rc.offset = %d(0x%x)  \n",pContext->video_rc.offset,pContext->video_rc.offset);
		}
		else
		{
			ADEMOD_WriteSub( hUser, VIDEOOFFSET_INTPL, pContext->video_rc_intpl.offset, 2);
			DEMOD_PRINT("pContext->video_rc_intpl.offset = %d(0x%x)	\n",pContext->video_rc_intpl.offset,pContext->video_rc_intpl.offset);
		}
			
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Offset_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	pValue[0] = pContext->rcMode;

	if(LX_ADEMOD_VID_RC_MODE_5X == pContext->rcMode){
		pValue[1] = pContext->video_rc.offset;
		DEMOD_PRINT("pContext->video_rc.offset = %d(0x%x)  \n",pContext->video_rc.offset,pContext->video_rc.offset);
	}
	else {
		pValue[1] = pContext->video_rc_intpl.offset;
		DEMOD_PRINT("pContext->video_rc_intpl.offset = %d(0x%x)  \n",pContext->video_rc_intpl.offset,pContext->video_rc_intpl.offset);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Clip_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_CLIPPER_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_CLIPPER_USR;
	}

	pContext->video_dac.clipper = pValue[0];

	if (Update) {
		ADEMOD_WriteSub( hUser, VIDEOMAXCLIPPING, (INT16)pContext->video_dac.clipper , 2);
		ADEMOD_WriteSub( hUser, VIDEOMINCLIPPING, (INT16)-pContext->video_dac.clipper , 2);
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Clip_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->video_dac.clipper;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Clip_Set
**
**  Description:    Sets video clipper.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video Rate converter clipper
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Clip_Set(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_CLIPPER_RC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_CLIPPER_RC_USR;
	}

	pContext->rcMode = pValue[0];

	if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
	{
		pContext->video_rc.clipper = pValue[1];
	}
	else
	{
		pContext->video_rc_intpl.clipper = pValue[1];
	}	

	if (Update) {
		ADEMOD_WriteSub( hUser, SEL_RC, pContext->rcMode, 1);

		if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
		{
			ADEMOD_WriteSub( hUser, VIDEOMAXCLIPPING_RC, (INT16)pContext->video_rc.clipper , 2);
			ADEMOD_WriteSub( hUser, VIDEOMINCLIPPING_RC, (INT16)-pContext->video_rc.clipper , 2);
		}
		else
		{
			ADEMOD_WriteSub( hUser, VIDEOMAXCLIPPING_INTPL, (INT16)pContext->video_rc_intpl.clipper , 2);
			ADEMOD_WriteSub( hUser, VIDEOMINCLIPPING_INTPL, (INT16)-pContext->video_rc_intpl.clipper , 2);

		}
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_RC_Clip_Get
**
**  Description:    Sets video clipper.
**
**  Parameters:     pContext     - Pointer to the Legacy demodulator context,
**                  pValue       - Pointer to the the value:
**						pValue[0]	Video rate converter clipper
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_RC_Clip_Get(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->rcMode;
	if(pContext->rcMode == LX_ADEMOD_VID_RC_MODE_5X )
		pValue[1] = pContext->video_rc.clipper;
	else
		pValue[1] = pContext->video_rc_intpl.clipper;
		

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Set(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_GRP_DELAY_USR;
	}

	if (pLUT) {
		memcpy(pContext->GroupDelayLUT, pLUT, sizeof(UINT16) * ((pContext->factory & LX_ADEMOD_FFLAG_DBG_1) ? LX_ADEMOD_GROUP_DELAY_LUT_SIZE_deBug_1 : LX_ADEMOD_GROUP_DELAY_LUT_SIZE));
	}
	else {
		status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_BYPASS_LUT);
	}

	if (Update)
	{
		status = DEMOD_M14_Analog_GroupDelay_Set(pContext, (UINT16 *)pContext->GroupDelayLUT, hUser);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_GroupDelay_Get(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	memcpy(pLUT, pContext->GroupDelayLUT, sizeof(LX_ADEMOD_GroupDelayLUT));

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Init(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// InitDemodTable
	ADEMOD_WriteSub( hUser, SLICE_LVL_CTRL, 0x02, 1); //jeongpil.yun- for brazil
	ADEMOD_WriteSub( hUser, BP_WIN_START, 0x007A, 2);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Start
**
**  Description:    Starts legacy demodulator block.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  pVideo_freq  - Pointer to the Video frequency aray,
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
LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Start(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pVideo_freq, UINT32 flags, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if (!pContext) {
		return retErrBadContext;
	}

	// Update Legacy Video Registers
	DEMOD_M14_Analog_LegacyDemod_Reset(pContext, TRUE, hUser);
	switch (pContext->video_std) {
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M_J:
			status = DEMOD_M14_Analog_LegacyDemod_System_M_N_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_B:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_G:
			status = DEMOD_M14_Analog_LegacyDemod_System_B_G_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_D:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_K:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
			status = DEMOD_M14_Analog_LegacyDemod_System_D_K_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_I:
			status = DEMOD_M14_Analog_LegacyDemod_System_I_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
			status = DEMOD_M14_Analog_LegacyDemod_System_L_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
			status = DEMOD_M14_Analog_LegacyDemod_System_L1_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_NTSC_433:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_50:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_H:
			status = DEMOD_M14_Analog_LegacyDemod_System_H_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_M:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_N:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_NC:
			status = DEMOD_M14_Analog_LegacyDemod_System_M_N_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_B:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_G:
			status = DEMOD_M14_Analog_LegacyDemod_System_B_G_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_60:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_H:
			status = DEMOD_M14_Analog_LegacyDemod_System_H_Set(pContext, hUser);
			break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_D:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K1:
			status = DEMOD_M14_Analog_LegacyDemod_System_D_K_Set(pContext, hUser);
			break;

		default:
			status = retErrBadMode;
		break;
	}

	// program group delay coefficients
	DEMOD_M14_Analog_GroupDelay_Set(pContext, (UINT16 *)pContext->GroupDelayLUT, hUser);

	//  Video DAC
	DEMOD_M14_Analog_LegacyDemod_DAC_Start(pContext, hUser);

	// SYNC Decoder
	DEMOD_M14_Analog_LegacyDemod_SyncDecoder_Start(pContext, pVideo_freq, flags, hUser);

	// Kick VSync Slicer
	DEMOD_M14_Analog_LegacyDemod_KickVSyncSlicer(pContext, hUser);

	DEMOD_M14_Analog_LegacyDemod_Reset(pContext, FALSE, hUser);

	if (retOK == status) {
		// clear update flag
		pContext->update &= ~LX_ADEMOD_UFLAG_UPDATE;
	}

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;

}



/*
** Static functions implementation
*/



/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_Reset
**
**  Description:    Executes soft reset on legacy demodulator block.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**					on_off       - When TRUE the reset state is entered,
**                                 when FALSE the reset state is terminated.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_Reset(pLX_ADEMOD_LegacyDemod pContext, BOOL on_off, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (on_off) {
		// set soft reset mask:   01101111b = 0x6f
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xc0);
		// set fifo reset mask FIFO_CLR0:   10000000b = 0x80;
		//ADEMOD_WriteByte( hUser, FIFO_CLR0, 0x80);
	}
	else {
		// clear soft reset mask: 01111111b = 0x7f
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xdf);
		// set fifo reset mask FIFO_CLR0:   00000000b = 0x0;
		//ADEMOD_WriteByte( hUser, FIFO_CLR0, 0x0);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_GroupDelay_Set
**
**  Description:    Sets group delay LUT.
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
static LX_ADEMOD_Result DEMOD_M14_Analog_GroupDelay_Set(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	DEMOD_PRINT("%s start  \n",__F__);
	DEMOD_PRINT("pContext->factory = %d \n",pContext->factory);

	if (pContext->factory & LX_ADEMOD_FFLAG_DBG_1) {
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF0,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF1,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF2,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF3,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF4,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF5,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF6,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF7,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF8,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF9,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF10, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF11, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF12, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF13, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF14, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF15, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF16, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF17, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF18, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF19, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF20, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF21, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF22, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF23, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF24, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF25, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF26, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF27, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF28, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF29, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF30, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF31, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF32, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF33, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF34, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF35, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF36, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF37, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF38, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF39, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF40, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF41, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF42, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF43, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF44, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF45, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF46, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF47, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF48, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF49, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF50, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF51, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF52, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF53, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF54, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF55, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF56, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF57, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF58, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF59, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF60, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF61, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF62, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF63, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF64, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF65, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF66, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF67, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF68, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF69, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF70, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF71, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF72, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF73, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF74, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF75, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF76, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF77, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF78, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, DBG_1_GROUPDELAY_COEFF79, *pLUT++, 2);
	}
	else {
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF0,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF1,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF2,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF3,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF4,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF5,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF6,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF7,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF8,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF9,  *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF10, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF11, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF12, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF13, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF14, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF15, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF16, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF17, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF18, *pLUT++, 2);
		ADEMOD_WriteSub( hUser, GROUPDELAY_COEFF19, *pLUT++, 2);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_System_M_N_Set
**
**  Description:    Sets legacy demodulator registers for NTSC_M_N standard.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_M_N_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->sync_dec.hs.rate = 157340;
	pContext->sync_dec.line_per_frame = 5250;
	pContext->sync_dec.frontporch_duration = 150;
	pContext->sync_dec.hs.tip_duration = 470;
	pContext->sync_dec.breezeway_duration = 60;
	pContext->sync_dec.burst_duration = 250;
	pContext->sync_dec.backporch_duration = 160;

	// set group delay coeffitients
	if (!(pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR))
	{
		status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_M_N_LUT);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_System_B_G_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Sets legacy demodulator registers for PAL_B_G standard.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_B_G_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->sync_dec.hs.rate = 156250;
	pContext->sync_dec.line_per_frame = 6250;
	pContext->sync_dec.frontporch_duration = 150;
	pContext->sync_dec.hs.tip_duration = 470;
	pContext->sync_dec.breezeway_duration = 60;
	pContext->sync_dec.burst_duration = 225;
	pContext->sync_dec.backporch_duration = 295;

	// set group delay coeffitients
	if (!(pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR))
	{
		status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_B_G_LUT);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_System_D_K_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Sets legacy demodulator registers for PAL_D_K standard.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_D_K_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->sync_dec.hs.rate = 156250;
	pContext->sync_dec.line_per_frame = 6250;
	pContext->sync_dec.frontporch_duration = 150;
	pContext->sync_dec.hs.tip_duration = 470;
	pContext->sync_dec.breezeway_duration = 60;
	pContext->sync_dec.burst_duration = 225;
	pContext->sync_dec.backporch_duration = 295;

	// set group delay coeffitients
	if (!(pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR))
	{
		if (pContext->flags & LX_ADEMOD_FFLAG_DK_CHINA_EN) {
			status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_D_K_CHINA_LUT);
		}
		else {
			status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_D_K_LUT);
		}
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_System_H_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Sets legacy demodulator registers for PAL_D_K standard.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_H_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->sync_dec.hs.rate = 156250;
	pContext->sync_dec.line_per_frame = 6250;
	pContext->sync_dec.frontporch_duration = 150;
	pContext->sync_dec.hs.tip_duration = 470;
	pContext->sync_dec.breezeway_duration = 60;
	pContext->sync_dec.burst_duration = 225;
	pContext->sync_dec.backporch_duration = 295;

	// set group delay coeffitients
	if (!(pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR))
	{
		status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_D_K_LUT);
	}

	return status;
}



/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_System_I_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Sets legacy demodulator registers for PAL_I standard.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_I_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->sync_dec.hs.rate = 156250;
	pContext->sync_dec.line_per_frame = 6250;
	pContext->sync_dec.frontporch_duration = 150;
	pContext->sync_dec.hs.tip_duration = 470;
	pContext->sync_dec.breezeway_duration = 60;
	pContext->sync_dec.burst_duration = 225;
	pContext->sync_dec.backporch_duration = 295;

	// set group delay coeffitients
DEMOD_PRINT("%s start  \n",__F__);
DEMOD_PRINT("pContext->update = %d  \n",pContext->update);
	if (!(pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR))
	{
		status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_I_LUT);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_System_L_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Sets legacy demodulator registers for SECAM_L standard.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_L_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->sync_dec.hs.rate = 156250;
	pContext->sync_dec.line_per_frame = 6250;
	pContext->sync_dec.frontporch_duration = 150;
	pContext->sync_dec.hs.tip_duration = 470;
	pContext->sync_dec.breezeway_duration = 60;
	pContext->sync_dec.burst_duration = 225;
	pContext->sync_dec.backporch_duration = 295;

	// set group delay coeffitients
	if (!(pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR))
	{
		status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_L_LUT);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_System_L1_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Sets legacy demodulator registers for SECAM_L standard.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_System_L1_Set(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->sync_dec.hs.rate = 156250;
	pContext->sync_dec.line_per_frame = 6250;
	pContext->sync_dec.frontporch_duration = 150;
	pContext->sync_dec.hs.tip_duration = 470;
	pContext->sync_dec.breezeway_duration = 60;
	pContext->sync_dec.burst_duration = 225;
	pContext->sync_dec.backporch_duration = 295;

	// set group delay coeffitients
	if (!(pContext->update & LX_ADEMOD_UFLAG_GRP_DELAY_USR))
	{
		status = DEMOD_M14_Analog_GroupDelayLUT_Default(pContext, (UINT16 *)LX_ADEMOD_GROUP_DELAY_SYSTEM_L1_LUT);
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_DAC_Start
**
**  Description:    Starts legacy demodulator's DAC block.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Updates the registers according to the lates state of the
**                  variables.
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_DAC_Start(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 DAC_En_Ctrl = 0;

	if (!pContext) {
		return retErrBadContext;
	}
	// apply gain
	ADEMOD_WriteSub( hUser, VIDEOGAIN, (INT16)pContext->video_dac.gain, 2);
	ADEMOD_WriteSub( hUser, VIDEOGAIN_RC, (INT16)pContext->video_rc.gain, 2);
	// apply offset
	ADEMOD_WriteSub( hUser, VIDEOOFFSET, (INT16)pContext->video_dac.offset, 2);
	ADEMOD_WriteSub( hUser, VIDEOOFFSET_RC, (INT16)pContext->video_rc.offset, 2);
	// apply clipping
	ADEMOD_WriteSub( hUser, VIDEOMAXCLIPPING, (INT16)pContext->video_dac.clipper, 2);
	ADEMOD_WriteSub( hUser, VIDEOMINCLIPPING, (INT16)-pContext->video_dac.clipper, 2);
	ADEMOD_WriteSub( hUser, VIDEOMAXCLIPPING_RC, (INT16)pContext->video_rc.clipper, 2);
	ADEMOD_WriteSub( hUser, VIDEOMINCLIPPING_RC, (INT16)-pContext->video_rc.clipper, 2);

	DAC_En_Ctrl = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_AUD_UPSAMPLE_EN)      ? 1 : 0) << 0) +
		(((pContext->flags & LX_ADEMOD_FFLAG_VID_UPSAMPLE_EN)      ? 1 : 0) << 1) +
		(((pContext->flags & LX_ADEMOD_FFLAG_AUD_LOWIF_EN)         ? 1 : 0) << 2) +
		(((pContext->flags & LX_ADEMOD_FFLAG_VID_LOWIF_EN)         ? 1 : 0) << 3) +
		(((pContext->flags & LX_ADEMOD_FFLAG_DAC_POSTSAW_EN)       ? 1 : 0) << 4)
	);
	ADEMOD_WriteSub( hUser, DAC_EN_CTRL, DAC_En_Ctrl, 1);

	return status;

}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_KickVSyncSlicer
**
**  Description:    Executes soft reset on AGC block.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_KickVSyncSlicer(pLX_ADEMOD_LegacyDemod pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
#if 0 // TBD: do we need this function in spyder?
	if ((pContext->video_std == LX_ADEMOD_VIDEO_STANDARD_SECAM_L) ||
		(pContext->video_std == LX_ADEMOD_VIDEO_STANDARD_SECAM_L1))
	{
		ADEMOD_WriteSub( hUser, SAFE_AGC_MASK, 0x01, 1);
		ADEMOD_WriteSub( hUser, SAFE_AGC_MASK, 0x02, 1);
		ADEMOD_WriteSub( hUser, SAFE_AGC_MASK, 0x01, 1);
	}
	else {
		ADEMOD_WriteSub( hUser, SAFE_AGC_MASK, 0x02, 1);
		ADEMOD_WriteSub( hUser, SAFE_AGC_MASK, 0x03, 1);
		ADEMOD_WriteSub( hUser, SAFE_AGC_MASK, 0x02, 1);
	}
#endif
	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LegacyDemod_SyncDecoder_Start
**
**  Description:    Starts legacy demodulator's DAC block.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  pVideo_freq  - Pointer to the Video frequency aray,
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
static LX_ADEMOD_Result DEMOD_M14_Analog_LegacyDemod_SyncDecoder_Start(pLX_ADEMOD_LegacyDemod pContext, UINT32 *pVideo_freq, UINT32 flags, Handle_t hUser)
{
#define VIRTUALFS_DIVIDER	(100000000000LL)

	LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 virtualfs = 0;
	UINT64 temp = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	virtualfs = (UINT64)pVideo_freq[0] + ((UINT64)pVideo_freq[1] << 32);

	if (pContext->sync_dec.hs.rate == 0) {
		return retErrZeroDiv;
	}
	if (pContext->sync_dec.hs.rate) {
		temp = virtualfs;
		do_div(temp,pContext->sync_dec.hs.rate );
		do_div(temp, 100);
		pContext->sync_dec.hs.period = (UINT32)temp;
		
//		pContext->sync_dec.hs.period = (UINT32)(virtualfs / (UINT64)pContext->sync_dec.hs.rate / 100);
	}

	temp = (UINT64)pContext->sync_dec.hs.tip_duration * virtualfs;
	do_div(temp,1000000000);
	do_div(temp,100);
	pContext->sync_dec.hs.tip_samples    = (UINT32)temp;
	pContext->sync_dec.hs.tip_win_period = pContext->sync_dec.hs.tip_samples * 3 / 10;

	temp = (UINT64)pContext->sync_dec.backporch_duration * virtualfs;
	do_div(temp,1000000000);
	do_div(temp,100);
	pContext->sync_dec.backporch_samples = (UINT32)temp;

	temp = (UINT64)pContext->sync_dec.breezeway_duration * virtualfs;
	do_div(temp,1000000000);
	do_div(temp,100);
	pContext->sync_dec.breezeway_samples = (UINT32)temp;

	temp = (UINT64)pContext->sync_dec.burst_duration * virtualfs;
	do_div(temp,1000000000);
	do_div(temp,100);

	pContext->sync_dec.burst_samples     = (UINT32)temp;
	pContext->sync_dec.h_slice.thresh	 = pContext->sync_dec.hs.tip_samples / 5;
	pContext->sync_dec.hs.width			 = pContext->sync_dec.hs.tip_samples * 273 / 100;
	pContext->sync_dec.vs.int_period	 = pContext->sync_dec.hs.period / 4;
	pContext->sync_dec.vs.width			 = pContext->sync_dec.line_per_frame * 6 / 200;
	pContext->sync_dec.vs.period		 = pContext->sync_dec.line_per_frame / 20;
	pContext->sync_dec.vs.nom_period	 = pContext->sync_dec.line_per_frame / 20;

	temp = (UINT64)(pContext->sync_dec.line_per_frame);
	do_div(temp,200);
	pContext->sync_dec.hs.safe_agc_line_start = ((UINT32)temp > 15) ? (UINT32)temp : 15;
	pContext->sync_dec.hs.safe_agc_line_stop = pContext->sync_dec.line_per_frame * 9 / 200;
	pContext->sync_dec.bp_win_start		 = pContext->sync_dec.hs.tip_samples + pContext->sync_dec.breezeway_samples;
	pContext->sync_dec.bp_win_period	 = pContext->sync_dec.backporch_samples * 8 / 10;

	if (pContext->sync_dec.bp_win_period == 0) {
		return retErrZeroDiv;
	}

	temp = (UINT64)(1 << 8); 
	if (pContext->sync_dec.bp_win_period) {
		do_div(temp, (pContext->sync_dec.bp_win_period));
		pContext->sync_dec.bp_win_wgt		  = (UINT32)temp;
	}
	if (pContext->sync_dec.hs.tip_win_period == 0) {
		return retErrZeroDiv;
	}

	temp = (UINT64)(1 << 8); 
	if (pContext->sync_dec.hs.tip_win_period) {
		do_div(temp, (pContext->sync_dec.hs.tip_win_period));

		pContext->sync_dec.hs.tip_win_wgt	  = (UINT32)temp;
	}
	if (pContext->sync_dec.hs.tip_ser_win_period == 0) {
		return retErrZeroDiv;
	}

	temp = (UINT64)(1 << 8); 
	if (pContext->sync_dec.hs.tip_ser_win_period) {
		do_div(temp, (pContext->sync_dec.hs.tip_ser_win_period));
		pContext->sync_dec.hs.tip_ser_win_wgt = (UINT32)temp;
	}

	{
		UINT8 SyncD_ctrl = (UINT8)(
			(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_NEGMOD_EN)      ? 1 : 0) << 0) +
			(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_SRCSEL_EN)      ? 1 : 0) << 1) +
			(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_VSLICE_EN)      ? 1 : 0) << 2)
		);
		ADEMOD_WriteSub( hUser, SYNCD_CTRL, SyncD_ctrl, 1);
	}

	{
		UINT8 SyncD_misc_ctrl = (UINT8)(
			(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_HEIGHT_NBP)     ? 1 : 0) << 0) +
			(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_VSLICE_OVR)     ? 1 : 0) << 1) +
			(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_AGCMASKOVR)     ? 1 : 0) << 2)
		);
		ADEMOD_WriteSub( hUser, SYNCD_MISC_CTRL, SyncD_misc_ctrl, 1);
	}

	{
		UINT8 SyncReserved = (UINT8)(
			(((pContext->flags & LX_ADEMOD_FFLAG_SYNCD_HYBRID_EN)	   ? 1 : 0) << 1)
		);
		ADEMOD_WriteSub( hUser, SYNCD_RESERVED, SyncReserved, 1);
	}

#if 0
	// these registers are now left as hardware defaults according to Gary
	ADEMOD_WriteSub( hUser, SNR_LINE_START, 0x0006, 2);
	ADEMOD_WriteSub( hUser, SNR_LINE_STOP, 0x0006, 2);
	ADEMOD_WriteSub( hUser, SNR_SAMPLE_START, 0x0190, 2);
	ADEMOD_WriteSub( hUser, SNR_SAMPLE_STOP, 0x0384, 2);
	ADEMOD_WriteSub( hUser, SNR_WIN_SEL, 0x02, 1);
#endif

	// from script
	//$hwRegs{VS_SLICE_CTRL} = 0x84;

	ADEMOD_WriteSub( hUser, SYNCD_ATTACK,			pContext->sync_dec.attack, 1);
	ADEMOD_WriteSub( hUser, SYNCD_DECAY,			pContext->sync_dec.decay, 2);
	ADEMOD_WriteSub( hUser, SYNCD_FASTDECAY,		pContext->sync_dec.fast_decay, 2);
	ADEMOD_WriteSub( hUser, SYNCD_DECAYTHRESH,		pContext->sync_dec.decay_thresh, 2);
	ADEMOD_WriteSub( hUser, SYNCD_DECAYWIN,			pContext->sync_dec.decay_window, 2);
	ADEMOD_WriteSub( hUser, SYNCD_MINPEAKTHRESH,	pContext->sync_dec.peak_thr_min, 2);
	ADEMOD_WriteSub( hUser, SYNCD_MAXPEAKTHRESH,	pContext->sync_dec.peak_thr_max, 2);
	ADEMOD_WriteSub( hUser, SYNCD_LPF_COEFFB,		pContext->sync_dec.hs.lpf_coeffb, 2);
	ADEMOD_WriteSub( hUser, SER_TH,					pContext->sync_dec.ser_thresh, 1);
	ADEMOD_WriteSub( hUser, SER_LOCKOUT,			pContext->sync_dec.ser_lockout, 1);
	ADEMOD_WriteSub( hUser, SER_MAX_DUR,			pContext->sync_dec.ser_max_dur, 1);
	ADEMOD_WriteSub( hUser, BP_AVG_SEL,				pContext->sync_dec.bp_avg_sel, 1);
	ADEMOD_WriteSub( hUser, BP_SER_WIN_PERIOD,		pContext->sync_dec.bp_ser_win_period, 2);
	ADEMOD_WriteSub( hUser, BP_SER_WIN_START,		pContext->sync_dec.bp_ser_win_start, 2);
	ADEMOD_WriteSub( hUser, BP_SER_WIN_WGT,			pContext->sync_dec.bp_ser_win_wgt, 1);


	{
		UINT8 Slice_level_ctrl = (UINT8)(
			(pContext->sync_dec.h_slice.level_sel			<< 0) +
			(pContext->sync_dec.h_slice.level_sel_ser		<< 2)
		);
		ADEMOD_WriteSub( hUser, SLICE_LVL_CTRL, Slice_level_ctrl, 1);
	}

	ADEMOD_WriteSub( hUser, SLICE_TH,			pContext->sync_dec.h_slice.thresh, 1);

	{
		UINT8 VS_Slice_ctrl = (UINT8)(
			(pContext->sync_dec.v_slice.level_sel			<< 0) +
			(pContext->sync_dec.v_slice.thresh				<< 3)
		);
		ADEMOD_WriteSub( hUser, VS_SLICE_CTRL1,		VS_Slice_ctrl, 1);
	}
	ADEMOD_WriteSub( hUser, VS_SLICE_CTRL2,		pContext->sync_dec.v_slice.level_sel_bp_st, 1);

	ADEMOD_WriteSub( hUser, HS_PERIOD,			pContext->sync_dec.hs.period, 2);
//	ADEMOD_WriteSub( hUser, HS_NOM_PERIOD,		pContext->sync_dec.hs.period, 2);
	ADEMOD_WriteSub( hUser, HS_WIDTH,			pContext->sync_dec.hs.width, 2);
	ADEMOD_WriteSub( hUser, HS_PW,				pContext->sync_dec.hs.tip_samples, 1);
	ADEMOD_WriteSub( hUser, HS_FINE_ADJ_SEL,	pContext->sync_dec.hs.adj_sel_fine, 1);
	ADEMOD_WriteSub( hUser, HS_COARSE_ADJ_SEL,	pContext->sync_dec.hs.adj_sel_coarse, 1);
	ADEMOD_WriteSub( hUser, HS_LOCK_TH,			pContext->sync_dec.hs.lock_thr, 2);
	ADEMOD_WriteSub( hUser, HS_UNLOCK_TH,		pContext->sync_dec.hs.unlock_thr, 2);
	ADEMOD_WriteSub( hUser, HS_TC_SEL,			pContext->sync_dec.hs.tc_sel, 1);
	ADEMOD_WriteSub( hUser, HS_MAX_ERR,			pContext->sync_dec.hs.max_err, 2);
	ADEMOD_WriteSub( hUser, HS_STABLE_TH,		pContext->sync_dec.hs.stable_thr, 2);
	ADEMOD_WriteSub( hUser, HS_UNSTABLE_TH,		pContext->sync_dec.hs.unstbl_thr, 2);
	ADEMOD_WriteSub( hUser, SYNCTIP_WIN_START,	pContext->sync_dec.hs.tip_win_start, 2);
	ADEMOD_WriteSub( hUser, SYNCTIP_WIN_PERIOD,	pContext->sync_dec.hs.tip_win_period, 2);
	ADEMOD_WriteSub( hUser, SYNCTIP_WIN_WGT,	pContext->sync_dec.hs.tip_win_wgt, 2);
	ADEMOD_WriteSub( hUser, SYNCTIP_AVG_SEL,	pContext->sync_dec.hs.tip_avg_sel, 1);
	ADEMOD_WriteSub( hUser, SYNCTIP_SER_WIN_START,	pContext->sync_dec.hs.tip_ser_win_start, 2);
	ADEMOD_WriteSub( hUser, SYNCTIP_SER_WIN_PERIOD,	pContext->sync_dec.hs.tip_ser_win_period, 2);
	ADEMOD_WriteSub( hUser, SYNCTIP_SER_WIN_WGT,pContext->sync_dec.hs.tip_ser_win_wgt, 1);
	ADEMOD_WriteSub( hUser, SYNC_MIN_TH,		pContext->sync_dec.hs.min_thr, 2);
	ADEMOD_WriteSub( hUser, SYNC_MAX_TH,		pContext->sync_dec.hs.max_thr, 2);
	ADEMOD_WriteSub( hUser, SYNC_MIN_CNT_TH,	pContext->sync_dec.hs.min_cnt_thr, 1);
	ADEMOD_WriteSub( hUser, SYNC_MAX_CNT_TH,	pContext->sync_dec.hs.max_cnt_thr, 1);
	ADEMOD_WriteSub( hUser, SYNC_HEIGHT_TH,		pContext->sync_dec.hs.height_th, 1);
	ADEMOD_WriteSub( hUser, MIN_SYNC_HEIGHT,	pContext->sync_dec.hs.min_sync_height, 2);
	ADEMOD_WriteSub( hUser, SAFE_AGC_LINE_START, pContext->sync_dec.hs.safe_agc_line_start, 2);
	ADEMOD_WriteSub( hUser, SAFE_AGC_LINE_STOP,  pContext->sync_dec.hs.safe_agc_line_stop, 2);
	ADEMOD_WriteSub( hUser, SAFE_AGC_MASK,		 pContext->sync_dec.hs.safe_agc_mask, 1);

//	ADEMOD_WriteSub( hUser, VS_NOM_PERIOD,		pContext->sync_dec.vs.nom_period, 2);
	ADEMOD_WriteSub( hUser, VS_LOCK_TH,			pContext->sync_dec.vs.lock_thr, 2);
	ADEMOD_WriteSub( hUser, VS_UNLOCK_TH,		pContext->sync_dec.vs.unlock_thr, 2);
	ADEMOD_WriteSub( hUser, VS_TC_SEL,			pContext->sync_dec.vs.tc_sel, 1);
	ADEMOD_WriteSub( hUser, VS_MAX_ERR,			pContext->sync_dec.vs.max_err, 2);
	ADEMOD_WriteSub( hUser, VS_STABLE_TH,		pContext->sync_dec.vs.stable_thr, 2);
	ADEMOD_WriteSub( hUser, VS_UNSTABLE_TH,		pContext->sync_dec.vs.unstbl_thr, 2);

	ADEMOD_WriteSub( hUser, VS_INT_PERIOD,		pContext->sync_dec.vs.int_period, 2);
	ADEMOD_WriteSub( hUser, VS_GATE_WIDTH,		pContext->sync_dec.vs.width, 2);
	ADEMOD_WriteSub( hUser, VS_PERIOD,			pContext->sync_dec.vs.period, 2);

	ADEMOD_WriteSub( hUser, BP_WIN_START,		pContext->sync_dec.bp_win_start, 2);
	ADEMOD_WriteSub( hUser, BP_WIN_PERIOD,		pContext->sync_dec.bp_win_period, 2);
	ADEMOD_WriteSub( hUser, BP_WIN_WGT,			pContext->sync_dec.bp_win_wgt, 1);

	return status;

}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_GroupDelayLUT_Default
**
**  Description:    Copies Group Delay default LUT into the context.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  pLUT		 - Pointer to the LUT.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_GroupDelayLUT_Default(pLX_ADEMOD_LegacyDemod pContext, UINT16 *pLUT)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	DEMOD_PRINT("%s start  \n",__F__);
	DEMOD_PRINT("pContext->factory = %d  \n",pContext->factory);

	if (pContext->factory & LX_ADEMOD_FFLAG_DBG_1) {
		int i;
		memset(pContext->GroupDelayLUT, 0, sizeof(UINT16) * LX_ADEMOD_GROUP_DELAY_LUT_SIZE_deBug_1);
		for (i = 0; i < LX_ADEMOD_GROUP_DELAY_LUT_SIZE; i++) {
			pContext->GroupDelayLUT[40 + i] = pLUT[i] * 4;
		}
	}
	else {
		memcpy(pContext->GroupDelayLUT, pLUT, sizeof(UINT16) * LX_ADEMOD_GROUP_DELAY_LUT_SIZE);
	}
	DEMOD_PRINT("%s end  \n",__F__);

	return status;
}


