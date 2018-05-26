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
**  Name: ademod_m14_video.c
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
#include "ademod_m14_regaddress.h"
#include "ademod_m14_video.h"

/*
**  Local static data - not available through API.
*/
//static UINT32 cutoffcoeff[4] = {500000, 351562 / 2, 141967 / 2, 39672 / 2}; // Hz

/*
**  Local Function Prototypes - not available for external access.
*/

static LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Reset(pLX_ADEMOD_VideoProc pContext, BOOL on_off, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_PLL_Reset(pLX_ADEMOD_VideoProc pContext, BOOL on_off, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_CarrierRecovery_Init(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_CarrierRecovery_Reset(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_FRLpfCoeff_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_FRLpfGain_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_PHLpfCoeff_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_PHLpfGain_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_LockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_UnlockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_UnlockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_MutelockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_MutelockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_MuteunlockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_MuteunlockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser);

/*
**  API implementation.
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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Factory_Set(pLX_ADEMOD_VideoProc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->input_mode = LX_ADEMOD_INPUTMODE_NONE;
	pContext->sampling_rate = LX_ADEMOD_SAMPLING_RATE_FCT;
	pContext->Fs_div = LX_ADEMOD_SAMPLING_RATE_DIV_FCT;
	pContext->min_range = LX_ADEMOD_RANGE_NORMAL_MIN_FCT;
	pContext->max_range = LX_ADEMOD_RANGE_NORMAL_MAX_FCT;
	pContext->offset_tweak = LX_ADEMOD_VID_OFFS_TWEAK_FCT;
	pContext->main_cutoff = LX_ADEMOD_MAIN_CUTOFF_FCT;
	pContext->vsb_energy = LX_ADEMOD_VSB_ENERGY_FCT;
	pContext->PF1_tinc_prec = LX_ADEMOD_PF1_TINC_PREC_FCT;
	pContext->PF2_tinc_prec = LX_ADEMOD_PF2_TINC_PREC_FCT;
	pContext->PF2_tincInv_prec = LX_ADEMOD_PF2_TINCINV_PREC_FCT;
	pContext->PF1_delivery_rate_prec = LX_ADEMOD_PF1_DELRATE_PREC_FCT;
	pContext->PF2_delivery_rate_prec = LX_ADEMOD_PF2_DELRATE_PREC_FCT;
	pContext->FR1_prec = LX_ADEMOD_FR1_PREC_FCT;
	pContext->FR2_prec = LX_ADEMOD_FR2_PREC_FCT;
	pContext->FR2_manual_ph = LX_ADEMOD_FR2_MPH_FCT;
	pContext->out_frequency = LX_ADEMOD_VID_OUT_FREQ_FCT;

	pContext->fr_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->fr_loop_speed = 0;
	pContext->fr_loop_gain_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->fr_loop_gain = 0;
	pContext->ph_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->ph_loop_speed = 0;
	pContext->ph_loop_gain_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->ph_loop_gain = 0;
	pContext->ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->ph_lock_det_lines = 0;
	pContext->ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->ph_lock_det_err = 0;
	pContext->ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->ph_unlock_det_lines = 0;
	pContext->ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->ph_unlock_det_err = 0;

	pContext->mute_ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->mute_ph_lock_det_lines = 0;
	pContext->mute_ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->mute_ph_lock_det_err = 0;
	pContext->mute_ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->mute_ph_unlock_det_lines = 0;
	pContext->mute_ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
	pContext->mute_ph_unlock_det_err = 0;

	pContext->overmod.mode = LX_ADEMOD_OVERMODULATION_MODE_WEIGHTED;
	pContext->overmod.phaserev = LX_ADEMOD_PRESET_OPTION_NONE;
	pContext->overmod.phaserev_angle = LX_ADEMOD_VID_OM_ANGLE_FCT;
	pContext->overmod.perc = LX_ADEMOD_VID_OM_PERC_FCT;

	pContext->Reset_Fr1_thresh = LX_ADEMOD_CR_FR1_THRESH_FCT;
	pContext->Reset_CRR_bound_TC = LX_ADEMOD_CR_CRR_BOUND_TC_FCT;
	pContext->Reset_CRR_lock_TC = LX_ADEMOD_CR_CRR_LOCK_TC_FCT;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Bandwidth)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_BNDW_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_BNDW_USR;
	}

	pContext->bandwidth = Bandwidth;
	// Update Audio Carrier
	status = DEMOD_M14_Analog_VideoProc_AudioCarrier_Set(pContext, Bandwidth);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Bandwidth_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pBandwidth)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pBandwidth = pContext->bandwidth;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Bandwidth)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_LOWIFBNDW_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_LOWIFBNDW_USR;
	}

	pContext->lowif_bandwidth = Bandwidth;
	pContext->main_cutoff = LX_ADEMOD_MAIN_LOWIFCUTOFF_FCT;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFBandwidth_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pBandwidth)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 temp=0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->update & LX_ADEMOD_UFLAG_VID_LOWIFBNDW_USR) {
		*pBandwidth = pContext->lowif_bandwidth;
	}
	else {
		 temp = ((UINT64)(pContext->bandwidth + 1100000/* VSB */) * LX_ADEMOD_MAIN_LOWIFCUTOFF_FCT);
		 do_div(temp,LX_ADEMOD_MAIN_CUTOFF_FCT );
		*pBandwidth = (UINT32)temp;
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_InputFreq_Set
**
**  Description:    Sets input frequency.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Value        - The new input frequency value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Value)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_INP_FREQ_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_INP_FREQ_USR;
	}

	pContext->inp_frequency = Value;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->inp_frequency;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_InputFreq_GetX
**
**  Description:    Returns actual input frequency and related intermediate attributes.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pValue       - Pointer to the actual input frequency value:
**						pValue[0]	Virtual FS	LSB In Hertz
**						pValue[1]	Virtual FS	MSB In Hertz
**						pValue[2]	IF	In Hertz
**						pValue[3]	PF1 tinc		In Hertz
**						pValue[4]	PF2 tinc		In Hertz
**						pValue[5]	FR1 delta phase	In Hertz
**						pValue[6]	FR2 delta phase	In Hertz
**						pValue[7]	sample rate	In Hertz
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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_InputFreq_GetX(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = (UINT32)(pContext->virtualfs);
	pValue[1] = (UINT32)(pContext->virtualfs >> 32);
	pValue[2] = pContext->inp_frequency;
	pValue[3] = pContext->PF1_tinc;
	pValue[4] = pContext->PF2_tinc;
	pValue[5] = pContext->FR1_delta_ph_min;
	pValue[6] = pContext->FR2_delta_ph_min;
	pValue[7] = pContext->sampling_rate;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Value)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_SMP_RATE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_SMP_RATE_USR;
		// Bug 1296: Low IF Center not updated if SmartTune used and CF is not factory value
		// Release LX_ADEMOD_UFLAG_OUT_FREQ_USR when LX_ADEMOD_UFLAG_SMP_RATE_USR is asserted
		pContext->update &= ~LX_ADEMOD_UFLAG_OUT_FREQ_USR;
	}

	pContext->sampling_rate = Value;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRate_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pValue = pContext->sampling_rate;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRateDivider_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Value)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update1 & LX_ADEMOD_UFLAG_SMP_RATE_DIV_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update1 |= LX_ADEMOD_UFLAG_SMP_RATE_DIV_USR;
	}

	pContext->Fs_div = (Value) ? LX_ADEMOD_SAMPLING_RATE_DIV_24MHZ_FCT : LX_ADEMOD_SAMPLING_RATE_DIV_FCT;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_SampleRateDivider_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pValue = pContext->Fs_div;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	INT64 fr2_init;
	INT64 fr2_max;
	INT64 fr2_min;


	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_OUT_FREQ_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_OUT_FREQ_USR;
	}

	if (pContext->sampling_rate == 0) {
		return retErrZeroDiv;
	}
	pContext->out_frequency = pValue[0];
	//fr2_init = (INT64)pContext->out_frequency - ((INT64)pContext->sampling_rate / 32);
	//fr2_max  = (INT64)pContext->out_frequency - ((INT64)pContext->sampling_rate / 32);
	//fr2_min  = (INT64)pContext->out_frequency - ((INT64)pContext->sampling_rate / 32);
	fr2_init = (INT64)pContext->out_frequency;
	fr2_max  = (INT64)pContext->out_frequency;
	fr2_min  = (INT64)pContext->out_frequency;

	fr2_init = (fr2_init * 2 * (INT64)(1 << 27)) * pContext->Fs_div;
	do_div(fr2_init, pContext->sampling_rate);

	fr2_min =  (fr2_min  * 2 * (INT64)(1 << 27)) * pContext->Fs_div;
	do_div(fr2_min, pContext->sampling_rate);
	
	fr2_max =  (fr2_max  * 2 * (INT64)(1 << 27)) * pContext->Fs_div;
	do_div(fr2_max,pContext->sampling_rate );

	pContext->FR2_delta_ph_init = (int)fr2_init;
	pContext->FR2_delta_ph_max  = (int)fr2_max;
	pContext->FR2_delta_ph_min  = (int)fr2_min;

	if (Update) {
		if (pContext->input_mode != LX_ADEMOD_INPUTMODE_NONE) {
			ADEMOD_WriteSub( hUser, FR2_DELTA_PH_MIN,  pContext->FR2_delta_ph_min, 4);
			ADEMOD_WriteSub( hUser, FR2_DELTA_PH_MAX,  pContext->FR2_delta_ph_max, 4);
			ADEMOD_WriteSub( hUser, FR2_DELTA_PH_INIT, pContext->FR2_delta_ph_init, 4);
		}
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LIFCenterFreq_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	INT64 temp1=0;
//	INT64 temp2=0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->update & LX_ADEMOD_UFLAG_OUT_FREQ_USR) {
		*pValue = pContext->out_frequency;
	}
	else
	{
		if (pContext->Fs_div == 0) {
			return retErrZeroDiv;
		}
		// LG3100
#if 0
		temp1 = (INT64)(pContext->FR2_delta_ph_init) * (INT64)(pContext->sampling_rate)
		do_div(temp1,  pContext->Fs_div);
		do_div(temp1, 2);
		do_div(temp1, (INT64)(1 << 27));

		temp2 = (INT64)pContext->sampling_rate;
		do_div(temp2, 32);
		
		*pValue = (UINT32)(temp1 + temp2);
#else
		temp1 = (INT64)((INT64)(pContext->FR2_delta_ph_init) * (INT64)(pContext->sampling_rate));
		do_div(temp1,  pContext->Fs_div );
		do_div(temp1, 2);
		do_div(temp1, (1 << 27));


		*pValue = (UINT32)temp1 ;
#endif
	}

	return status;
}



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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_PIN_RANG_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_PIN_RANG_USR;
	}

	pContext->option_range = pValue[0];
	switch (pValue[0]) {
		case LX_ADEMOD_PRESET_OPTION_NONE:
				pContext->min_range = pValue[1];
				pContext->max_range = pValue[2];
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
				pContext->min_range = LX_ADEMOD_RANGE_NORMAL_MIN_FCT;
				pContext->max_range = LX_ADEMOD_RANGE_NORMAL_MAX_FCT;
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
				pContext->min_range = LX_ADEMOD_RANGE_MIN_MIN_FCT;
				pContext->max_range = LX_ADEMOD_RANGE_MIN_MAX_FCT;
			break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
				pContext->min_range = LX_ADEMOD_RANGE_MAX_MIN_FCT;
				pContext->max_range = LX_ADEMOD_RANGE_MAX_MAX_FCT;
			break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update) {
		if (pContext->input_mode != LX_ADEMOD_INPUTMODE_NONE) {
			status = DEMOD_M14_Analog_VideoProc_Mode_Set(pContext, pContext->input_mode);
			status = DEMOD_M14_Analog_VideoProc_Start(pContext, hUser);
		}
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Range_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->option_range;
	pValue[1] = pContext->min_range;
	pValue[2] = pContext->max_range;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_LPDNBND_OFFS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_LPDNBND_OFFS_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->narrowband_offs1 = pValue[1];
			pContext->narrowband_offs2 = pValue[2];
			pContext->narrowband_offs3 = pValue[3];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->narrowband_offs1 = LX_ADEMOD_DIGAGC_NBNDOFFS1_MIN_FCT;
			pContext->narrowband_offs2 = LX_ADEMOD_DIGAGC_NBNDOFFS2_MIN_FCT;
			pContext->narrowband_offs3 = LX_ADEMOD_DIGAGC_NBNDOFFS3_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->narrowband_offs1 = LX_ADEMOD_DIGAGC_NBNDOFFS1_MAX_FCT;
			pContext->narrowband_offs2 = LX_ADEMOD_DIGAGC_NBNDOFFS2_MAX_FCT;
			pContext->narrowband_offs3 = LX_ADEMOD_DIGAGC_NBNDOFFS3_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->narrowband_offs1 = LX_ADEMOD_DIGAGC_NBNDOFFS1_NOM_FCT;
			pContext->narrowband_offs2 = LX_ADEMOD_DIGAGC_NBNDOFFS2_NOM_FCT;
			pContext->narrowband_offs3 = LX_ADEMOD_DIGAGC_NBNDOFFS3_NOM_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		pContext->narrowband_mode_offs = pValue[0];
	}

	if (Update) {
		if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
			UINT32 FR2_manual_ph = (UINT32)(
				((pContext->narrowband_offs1 & 0x3)	<< 0) +
				((pContext->narrowband_offs2 & 0x3)	<< 2) +
				((pContext->narrowband_offs3 & 0x3)	<< 4) +
				(pContext->FR2_manual_ph)
			);
			ADEMOD_WriteSub( hUser, FR2_MANUAL_PH,     FR2_manual_ph, 4);
		}
		else {
			// N/A
		}
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LPF_NarrowbandOffset_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->narrowband_mode_offs;
	pValue[1] = pContext->narrowband_offs1;
	pValue[2] = pContext->narrowband_offs2;
	pValue[3] = pContext->narrowband_offs3;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_VSB_Set
**
**  Description:    Sets VSB energy range.
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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VSB_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VSB_ENRG_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VSB_ENRG_USR;
	}

	pContext->vsb_energy = pValue[0];

	if (Update) {
		if (pContext->input_mode != LX_ADEMOD_INPUTMODE_NONE) {
			status = DEMOD_M14_Analog_VideoProc_Mode_Set(pContext, pContext->input_mode);
			status = DEMOD_M14_Analog_VideoProc_Start(pContext, hUser);
		}
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VSB_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->vsb_energy;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Tinc_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pValue[0]) { // direct PF1_tinc provided
		if ((pContext->update & LX_ADEMOD_UFLAG_VID_PF1TINK_USR) &&
			(pContext->factory & LX_ADEMOD_FFLAG_UPDATE)) {
			// when user sticky bit is raised, don't reset to factory
		}
		else {
			pContext->update |= LX_ADEMOD_UFLAG_VID_PF1TINK_USR;
			pContext->PF1_tinc = pValue[0];
		}
	}

	if (pValue[1]) { // direct PF2_tinc provided
		if ((pContext->update & LX_ADEMOD_UFLAG_VID_PF2TINK_USR) &&
			(pContext->factory & LX_ADEMOD_FFLAG_UPDATE)) {
			// when user sticky bit is raised, don't reset to factory
		}
		else {
			pContext->update |= LX_ADEMOD_UFLAG_VID_PF2TINK_USR;
			pContext->PF2_tinc = pValue[1];
		}
	}

	if (pValue[2]) { // direct PF2_tincInv provided
		if ((pContext->update & LX_ADEMOD_UFLAG_VID_PF2TINKINV_USR) &&
			(pContext->factory & LX_ADEMOD_FFLAG_UPDATE)) {
			// when user sticky bit is raised, don't reset to factory
		}
		else {
			pContext->update |= LX_ADEMOD_UFLAG_VID_PF2TINKINV_USR;
			pContext->PF2_tincInv = pValue[2];
		}
	}

	if (Update) {
		if (pContext->input_mode != LX_ADEMOD_INPUTMODE_NONE) {
			status = DEMOD_M14_Analog_VideoProc_Mode_Set(pContext, pContext->input_mode);
			status = DEMOD_M14_Analog_VideoProc_Start(pContext, hUser);
		}
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Tinc_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->PF1_tinc;
	pValue[1] = pContext->PF2_tinc;
	pValue[2] = pContext->PF2_tincInv;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_OffsTweak_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_OFFS_TWEAK_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_OFFS_TWEAK_USR;
	}

	pContext->offset_tweak = pValue[0];

	// Note: need to re-program the main filter if this parameter is changed

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_OffsTweak_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->offset_tweak;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Cutoff_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_CTF_MAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_CTF_MAIN_USR;
	}

	pContext->main_cutoff = pValue[0];

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Cutoff_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->main_cutoff;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_CarrierOffset_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_CAR_OFFS_USR) {
		// when user sticky bit is raised, don't reset to factory
		return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_CAR_OFFS_USR;
	}

	pContext->FR1_InputFreqOffset = pValue[0];

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_CarrierOffset_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	int ActualFr1 = 0;
	int InitFr1 = 0;
	INT64 temp = 0;
	INT64 temp1 = 0;

	INT64 calcTemp = 0;
	INT64 calcTemp1 = 0;


	if (!pContext) {
		return retErrBadContext;
	}

	if ((pContext->Fs_div == 0) || (pContext->sampling_rate == 0)) {
		return retErrZeroDiv;
	}

	ADEMOD_ReadSub( hUser, FR1_ACTUALFREQ, (UINT32 *)&ActualFr1, 4);
	ADEMOD_ReadSub( hUser, FR1_DELTA_PH_INIT, (UINT32 *)&InitFr1, 4);

	if (InitFr1 & (UINT32)(1 << 27)) { // check sign bit in 28 bit wide register InitFr1
		InitFr1 |= 0xF0000000; // update to 32-bit negative number
	}

	DEMOD_PRINT(" ****ActualFr1 = %d(0x%x) **** \n", ActualFr1,ActualFr1 );
	DEMOD_PRINT(" ****InitFr1 = %d(0x%x) **** \n", InitFr1,InitFr1 );
	DEMOD_PRINT(" ****pContext->offset_tweak = %d **** \n", pContext->offset_tweak );

	if(InitFr1 < 0) calcTemp = (-1) * (INT64)InitFr1; //* pContext->sampling_rate;
	else 			calcTemp = (INT64)InitFr1; //* pContext->sampling_rate;
	
	calcTemp = calcTemp * pContext->sampling_rate;

	do_div(calcTemp, pContext->Fs_div );
	do_div(calcTemp, 100);
	calcTemp = calcTemp >> 28;

	if(InitFr1 < 0) calcTemp = (-1) * calcTemp; //* pContext->sampling_rate;
	else 			calcTemp = calcTemp; //* pContext->sampling_rate;

	calcTemp1 = pContext->offset_tweak;
	do_div(calcTemp1, 100);
	
	temp1 = calcTemp + calcTemp1;

//	temp1 = ((INT64)InitFr1 * pContext->sampling_rate / pContext->Fs_div / 100 / (INT64)(1 << 28)) + (pContext->offset_tweak / 100);

	if( pContext->flags & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP ) {

		if(ActualFr1 < 0) calcTemp = (-1) * (INT64)ActualFr1; //* pContext->sampling_rate;
		else			calcTemp = (INT64)ActualFr1; //* pContext->sampling_rate;

		do_div(calcTemp, 16);
		calcTemp = calcTemp * (INT64)(pContext->sampling_rate);
		do_div(calcTemp, pContext->Fs_div);
		do_div(calcTemp, 100);
		calcTemp = calcTemp >> 28;

		if(ActualFr1 < 0) calcTemp = (-1) * calcTemp; //* pContext->sampling_rate;
		else			calcTemp = calcTemp; //* pContext->sampling_rate;
		
		temp = calcTemp - temp1;
//		temp = (INT64)ActualFr1 / 16 * pContext->sampling_rate / pContext->Fs_div / 100 / (INT64)(1 << 28) - temp1;
	}
	else {
		if(ActualFr1 < 0) calcTemp = (-1) * (INT64)ActualFr1; //* pContext->sampling_rate;
		else			calcTemp = (INT64)ActualFr1; //* pContext->sampling_rate;

		do_div(calcTemp, 16);
		calcTemp = calcTemp * (INT64)(pContext->sampling_rate);
		do_div(calcTemp, pContext->Fs_div);
		do_div(calcTemp, 100);
		calcTemp = calcTemp >> 28;

		if(ActualFr1 < 0) calcTemp = (-1) * calcTemp; //* pContext->sampling_rate;
		else			calcTemp = calcTemp; //* pContext->sampling_rate;

		temp = temp1 - calcTemp;
//		temp = temp1 - (INT64)ActualFr1 / 16 * pContext->sampling_rate / pContext->Fs_div / 100 / (INT64)(1 << 28);
	}

	if(temp < 0) temp1 = (-1) * temp; //* pContext->sampling_rate;
	else			temp1 = temp; //* pContext->sampling_rate;

	do_div(temp1, 10); // in KHz

	if(temp < 0) temp1 = (-1) * temp1; //* pContext->sampling_rate;
	else			temp1 = temp1; //* pContext->sampling_rate;

	pValue[0] = (UINT32)temp1;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Flags, UINT32 mask)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (mask & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP) {
			if (!(pContext->update & LX_ADEMOD_UFLAG_VID_ROTATORFLIP_USR)) {
				if (Flags & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP) {
					pContext->flags |= LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
				}
				else {
					pContext->flags &= ~LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
				}
			}
		}
		if (mask & LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT) {
			if (!(pContext->update1 & LX_ADEMOD_UFLAG_CR_OVERMOD_DISWEIGHT_USR)) {
				if (Flags & LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT) {
					pContext->flags |= LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
				}
				else {
					pContext->flags &= ~LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
				}
			}
		}
	}
	else { // user settings
		if (mask & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP) {
			pContext->update |= LX_ADEMOD_UFLAG_VID_ROTATORFLIP_USR;
			if (Flags & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP) {
				pContext->flags |= LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
			}
			else {
				pContext->flags &= ~LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP;
			}
		}
		if (mask & LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT) {
			pContext->update1 |= LX_ADEMOD_UFLAG_CR_OVERMOD_DISWEIGHT_USR;
			if (Flags & LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT) {
				pContext->flags |= LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
			}
			else {
				pContext->flags &= ~LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
			}
		}
	}

	if (mask & LX_ADEMOD_FFLAG_44n36) {
		if (Flags & LX_ADEMOD_FFLAG_44n36) {
			pContext->flags |= LX_ADEMOD_FFLAG_44n36;
		}
		else {
			pContext->flags &= ~LX_ADEMOD_FFLAG_44n36;
		}
	}

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Flags_Get(pLX_ADEMOD_VideoProc pContext, UINT32* pFlags)
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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Mode_Set(pLX_ADEMOD_VideoProc pContext, UINT32 InputMode)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 PF1_tinc;
	UINT64 PF2_tinc;
	UINT64 PF2_tincInv;
	UINT64 iffreq;
	INT64 temp = 0;
	UINT32 pf1_tinc_hwregs_adjust = 16;
	UINT32 pf2_tinc_hwregs_adjust = 16;
	UINT32 pf2_tincinv_hwregs_adjust = 16;
	UINT32 upsample_factor = 0;
	UINT32 f_adc_div = 0;
	INT64 F_null = 0;
	INT64 pc_from_if = 0;
	INT64 fr1_init;
	INT64 fr1_min;
	INT64 fr1_max;
	INT64 fr2_init;
	INT64 fr2_min;
	INT64 fr2_max;
	UINT64 f_mfs;

	UINT64 calcUintTemp = 0;
	INT64 calcIntTemp = 0;

	DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);
	if (!pContext) {
		return retErrBadContext;
	}

	switch (InputMode)
	{
		case LX_ADEMOD_INPUTMODE_LEGACY:

			// in Legacy mode. The scale factor needs to be 1.5 to take into account of 1.5x CVBS upsampler.
			upsample_factor	= 15; // 1.5

			// MICRO_REGSWEEPDONE 0x625 bit4 Legacy Signal Monitor feature Enable;
			pContext->flags |= LX_ADEMOD_FFLAG_LEGACY_SIG_MON;
			// MICRO_REGSWEEPDONE 0x625 bit0 DVBT re-lock feature Disable;
			pContext->flags &= ~LX_ADEMOD_FFLAG_DVBT_RELOCK;

			DEMOD_PRINT(" pContext->flags = 0x%x \n",pContext->flags);
		break;

		case LX_ADEMOD_INPUTMODE_DIGITAL:
			if (!(pContext->update & LX_ADEMOD_UFLAG_INP_FREQ_USR)) {
				if (pContext->flags & LX_ADEMOD_FFLAG_44n36) {
					pContext->inp_frequency = FM1160_INP_FRQ_DVBT_FCT; // PC
				}
				else {
					pContext->inp_frequency = LX_ADEMOD_INP_FRQ_DVBT_FCT; // PC
				}
			}
			if (!(pContext->update & LX_ADEMOD_UFLAG_VID_BNDW_USR)) {
				pContext->bandwidth = LX_ADEMOD_VID_BNDW_DVBT_FCT;
			}

			if (!(pContext->update & LX_ADEMOD_UFLAG_VSB_ENRG_USR)) {
				pContext->vsb_energy = LX_ADEMOD_VSB_ENERGY_FCT;
			}

			// In low if mode the scale factor can be set to 1
			upsample_factor = 10;

			// MICRO_REGSWEEPDONE 0x625 bit4 Legacy Signal Monitor feature Disable;
			pContext->flags &= ~LX_ADEMOD_FFLAG_LEGACY_SIG_MON;
			// MICRO_REGSWEEPDONE 0x625 bit0 DVBT re-lock feature Enable;
			pContext->flags |= LX_ADEMOD_FFLAG_DVBT_RELOCK;
			pContext->CRF_Index_Acq = 1;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	if (status == retOK) {
		pContext->input_mode = InputMode;
		DEMOD_PRINT(" pContext->input_mode = 0x%x \n",pContext->input_mode);
	}
	else {
		return status;
	}

	// Video Bandwidth

	// The PF Tinc and TincInv programming has certain rounding restrictions in order for the
	// resampling not to "drift". These restrictions are captued here. The following logic is
	// copied over from Edmumd's script video_bandwidth_programming.pl that is used to program the
	// the cmodel. The basic requirements are as follows:

	// PF2_Tinc has to be exactly representable by 18 fractional bits
	// (This register is currently 22 bits in h/w but audio buzz settings limit it to 18 bit programming)

	// PF1_Tinc has to be 1.5x PF2_TincInv becuase of the fixed 1.5x CVBS upsampling filter in legacy mode.
	// In low IF mode, they should be exactly the same because there is no scale factor.
	// Both numbers have to be excatly representable in 14 bits
	// (These registers are currently 18 bits in h/w but audio buzz settings limit it to 14 bit programming)

	// Due to audio buzz requirements, the Tincs are rounded to lower precisions than the actual
	// hardware register size. So set the lower precisions here and then shift it before we program
	// the hardware registers

	// Maximum samplign rate of video block
	if (pContext->Fs_div == 0) {
		return retErrZeroDiv;
	}

	calcUintTemp = (UINT64)(pContext->sampling_rate);
	DEMOD_PRINT("calcUintTemp = %lld,  pContext->sampling_rate = %d(0x%x), pContext->Fs_div = %d(0x%x) \n",calcUintTemp,pContext->sampling_rate, pContext->sampling_rate,pContext->Fs_div,pContext->Fs_div);
	do_div(calcUintTemp,(pContext->Fs_div));
	
	f_adc_div = (UINT32)calcUintTemp;

	DEMOD_PRINT("f_adc_div = %d,  pContext->sampling_rate = %d(0x%x), pContext->Fs_div = %d(0x%x) \n",f_adc_div,pContext->sampling_rate, pContext->sampling_rate,pContext->Fs_div,pContext->Fs_div);

	if ((pContext->update & LX_ADEMOD_UFLAG_VID_LOWIFBNDW_USR) && (InputMode == LX_ADEMOD_INPUTMODE_DIGITAL)) {
		calcIntTemp = (INT64)( pContext->lowif_bandwidth );
		do_div(calcIntTemp, 2);
		F_null = calcIntTemp;
	}
	else {
		calcIntTemp = (INT64)( pContext->vsb_energy + pContext->bandwidth );
		do_div(calcIntTemp, 2);
		F_null = calcIntTemp;
		
		DEMOD_PRINT("F_null = %lld,  pContext->vsb_energy = 0x%x, pContext->bandwidth = 0x%x  \n",F_null, pContext->vsb_energy,pContext->bandwidth);
	}

	// First calculate PF2 Tinc and truncate any fractional part so that it is representable by the fixed point precision
	PF2_tinc = F_null * (UINT64)(1 << pContext->PF2_tinc_prec) * upsample_factor * 1000;
	DEMOD_PRINT("PF2_tinc = %lld,  pContext->PF2_tinc_prec = 0x%x \n",PF2_tinc, pContext->PF2_tinc_prec);

	if (pContext->main_cutoff == 0) {
		return retErrZeroDiv;
	}

	do_div(PF2_tinc, (pContext->main_cutoff));

	if (f_adc_div == 0) {
		return retErrZeroDiv;
	}
	do_div(PF2_tinc, f_adc_div);

	// Now calculate PF2 Tinc Inv and truncate any fractional part. For the FIFOs in the hardware to work properly,
	// we need to satisfy the conditoin that PF2_Tinc < (1/ PF2_TincInv).
	PF2_tincInv = (UINT64)(1 << pContext->PF2_tinc_prec) * (UINT64)(1 << pContext->PF2_tincInv_prec);
	DEMOD_PRINT("PF2_tincInv = %lld,  pContext->PF2_tinc_prec = %d,  pContext->PF2_tinc_prec = %d\n",PF2_tinc, pContext->PF2_tinc_prec, pContext->PF2_tinc_prec );

	if (PF2_tinc == 0) {
		return retErrZeroDiv;
	}
	do_div(PF2_tincInv, PF2_tinc);

	// Now we have to adjust pf2_tinciinv if it is not even since pf1_tinc needs to be exactly 1.5x of pf2_tincinv
	calcUintTemp = PF2_tincInv;
	if (do_div(calcUintTemp,2) == 1) {
		// pf2_tincinv is odd, so adjust it
		PF2_tincInv--;
		DEMOD_PRINT("PF2_tincInv = %lld\n",PF2_tincInv);
	}

	// Finally, calculate PF1 Tinc
	PF1_tinc = PF2_tincInv * (UINT64)upsample_factor;
	do_div(PF1_tinc, 10);

	// Calculate the adjusted video bandwidth after fulfilling the programming requirements
	temp = PF2_tincInv * upsample_factor;
	DEMOD_PRINT("temp = %lld \n",temp);
	
	if (temp == 0) {
		return retErrZeroDiv;
	}
	f_mfs = (f_adc_div * (INT64)(1 << pContext->PF1_tinc_prec) * 10000);
	do_div(f_mfs, PF2_tincInv );
	do_div(f_mfs, upsample_factor);
	DEMOD_PRINT("f_mfs = %lld,  pContext->PF1_tinc_prec = %d, PF2_tincInv = %lld \n",f_mfs, pContext->PF1_tinc_prec,PF2_tincInv);
	
	if (!(pContext->update & LX_ADEMOD_UFLAG_VID_VIRTUALFS_USR)) {
		pContext->virtualfs = f_mfs;
	}

	// Now shift the hardware registers values to the actual hardware register sizes
	if (!(pContext->update & LX_ADEMOD_UFLAG_VID_PF1TINK_USR)) {
		pContext->PF1_tinc = (UINT32)(PF1_tinc * pf1_tinc_hwregs_adjust);
		DEMOD_PRINT("pContext->PF1_tinc = %d,  pf1_tinc_hwregs_adjust = %d \n",pContext->PF1_tinc, pf1_tinc_hwregs_adjust);
	}

	if (!(pContext->update & LX_ADEMOD_UFLAG_VID_PF2TINK_USR)) {
		pContext->PF2_tinc = (UINT32)(PF2_tinc * pf2_tinc_hwregs_adjust);
		DEMOD_PRINT("pContext->PF2_tinc = %d,  pf2_tinc_hwregs_adjust = %d \n",pContext->PF2_tinc, pf2_tinc_hwregs_adjust);
	}

	if (!(pContext->update & LX_ADEMOD_UFLAG_VID_PF2TINKINV_USR)) {
		pContext->PF2_tincInv = (UINT32)PF2_tincInv * pf2_tincinv_hwregs_adjust;
		DEMOD_PRINT("pContext->PF2_tincInv = %d,  pf2_tincinv_hwregs_adjust = %d \n",pContext->PF1_tinc, pf2_tincinv_hwregs_adjust);
	}

	pContext->PR_delivery_rate = (UINT32)pContext->PF1_tinc * 5;
	pContext->PF2_delivery_rate = 1;
	DEMOD_PRINT("pContext->PR_delivery_rate = %d \n",pContext->PR_delivery_rate);

	// We might aslo need to adjust the parameter DigAGC_DVDelay here, but I dont know how to do it yet ...


	// Video CR
	// determines IF location
	if (pContext->flags & LX_ADEMOD_FFLAG_44n36)
	{
		iffreq = (UINT64)(pContext->sampling_rate);
		do_div(iffreq, 6);
		DEMOD_PRINT("iffreq = %lld,  pContext->sampling_rate = 0x%x \n",iffreq, pContext->sampling_rate);
	}
	else
	{
		iffreq = (UINT64)(pContext->sampling_rate);
		do_div(iffreq, 8);
		DEMOD_PRINT("iffreq = %lld,  pContext->sampling_rate = 0x%x \n",iffreq, pContext->sampling_rate);
	}

	// calculates the Picture Carrier distance from IF Center
	if( pContext->flags & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP )
	{
		pc_from_if = pContext->inp_frequency - iffreq;
		DEMOD_PRINT("pc_from_if = %lld,  pContext->inp_frequency = 0x%x \n",pc_from_if, pContext->inp_frequency);
	}
	else
	{
		pc_from_if = iffreq - pContext->inp_frequency;
		DEMOD_PRINT(">>pc_from_if = %lld,  pContext->inp_frequency = 0x%x \n",pc_from_if, pContext->inp_frequency);
	}

	switch (InputMode)
	{
		case LX_ADEMOD_INPUTMODE_LEGACY:
			{

				fr1_init = F_null - pContext->vsb_energy - pc_from_if - pContext->offset_tweak;
				fr1_min  = fr1_init - pContext->min_range + pContext->offset_tweak;
				fr1_max  = fr1_init + pContext->max_range + pContext->offset_tweak;

				fr2_init = fr1_init + pc_from_if + pContext->offset_tweak;
				fr2_min  = fr2_init;
				fr2_max  = fr2_init;
				
				DEMOD_PRINT("fr1_init = %lld, pContext->vsb_energy = 0x%x, pContext->offset_tweak = 0x%x \n",fr1_init, pContext->vsb_energy,pContext->offset_tweak);
				DEMOD_PRINT("fr1_min = %lld, pContext->min_range = 0x%x, pContext->offset_tweak = 0x%x \n",fr1_min, pContext->min_range,pContext->offset_tweak);
				DEMOD_PRINT("fr1_max = %lld, pContext->max_range = 0x%x, pContext->offset_tweak = 0x%x \n",fr1_max, pContext->max_range,pContext->offset_tweak);

				DEMOD_PRINT("fr2_init = %lld, pc_from_if = 0x%llx, pContext->offset_tweak = 0x%x \n",fr2_init, pc_from_if,pContext->offset_tweak);
				DEMOD_PRINT("fr2_min = %lld \n",fr2_min);
				DEMOD_PRINT("fr1_max = %lld\n",fr1_max);

				if (pContext->sampling_rate == 0) {
					return retErrZeroDiv;
				}
				if(fr1_init > 0) 	calcIntTemp = fr1_init;
				else 				calcIntTemp =  (-1) * fr1_init;
				calcIntTemp = (calcIntTemp * pContext->Fs_div * 2 * (INT64)(1 << 27));
				do_div(calcIntTemp,(pContext->sampling_rate) );
				if(fr1_init > 0) 	fr1_init = calcIntTemp;
				else 				fr1_init = (-1) * calcIntTemp;

				if(fr1_min > 0) 	calcIntTemp = fr1_min;
				else 				calcIntTemp =  (-1) * fr1_min;
				calcIntTemp = (calcIntTemp * pContext->Fs_div * 2 * (INT64)(1 << 27));
				do_div(calcIntTemp,(pContext->sampling_rate) );
				if(fr1_min > 0) 	fr1_min = calcIntTemp;
				else 				fr1_min = (-1) * calcIntTemp;

				if(fr1_max > 0) 	calcIntTemp = fr1_max;
				else 				calcIntTemp =  (-1) * fr1_max;
				calcIntTemp = (calcIntTemp * pContext->Fs_div * 2 * (INT64)(1 << 27));
				do_div(calcIntTemp,(pContext->sampling_rate) );
				if(fr1_max > 0) 	fr1_max = calcIntTemp;
				else 				fr1_max = (-1) * calcIntTemp;

	//			fr1_init = (fr1_init * pContext->Fs_div * 2 * (INT64)(1 << 27)) / pContext->sampling_rate;
	//			fr1_min =  (fr1_min  * pContext->Fs_div * 2 * (INT64)(1 << 27)) / pContext->sampling_rate;
	//			fr1_max =  (fr1_max  * pContext->Fs_div * 2 * (INT64)(1 << 27)) / pContext->sampling_rate;

				DEMOD_PRINT("fr1_init = %lld, pContext->Fs_div = 0x%x, pContext->sampling_rate = 0x%x \n",fr1_init, pContext->Fs_div,pContext->sampling_rate);
				DEMOD_PRINT("fr1_min = %lld, pContext->Fs_div = 0x%x, pContext->sampling_rate = 0x%x \n",fr1_min, pContext->Fs_div,pContext->sampling_rate);
				DEMOD_PRINT("fr1_max = %lld, pContext->Fs_div = 0x%x, pContext->sampling_rate = 0x%x \n",fr1_max, pContext->Fs_div,pContext->sampling_rate);

				if (pContext->virtualfs == 0) {
					return retErrZeroDiv;
				}
				fr2_init = (fr2_init * 1000 * 2 * (INT64)(1 << 27));
				fr2_init = div64_64(fr2_init, pContext->virtualfs );

				fr2_min =  (fr2_min  * 1000 * 2 * (INT64)(1 << 27));
				fr2_min = div64_64(fr2_min, pContext->virtualfs );

				fr2_max =  (fr2_max  * 1000 * 2 * (INT64)(1 << 27));
				fr2_max = div64_64(fr2_max, pContext->virtualfs );

	//			fr2_init = (fr2_init * 1000 * 2 * (INT64)(1 << 27)) / pContext->virtualfs;
	//			fr2_min =  (fr2_min  * 1000 * 2 * (INT64)(1 << 27)) / pContext->virtualfs;
	//			fr2_max =  (fr2_max  * 1000 * 2 * (INT64)(1 << 27)) / pContext->virtualfs;

				DEMOD_PRINT("fr2_init = %lld, pContext->virtualfs = 0x%llx \n",fr2_init, pContext->virtualfs);
				DEMOD_PRINT("fr2_min = %lld, pContext->virtualfs = 0x%llx \n",fr2_min, pContext->virtualfs);
				DEMOD_PRINT("fr2_max = %lld, pContext->virtualfs = 0x%llx \n",fr2_max, pContext->virtualfs);
			}
			break;

		case LX_ADEMOD_INPUTMODE_DIGITAL:
			{
				if (!(pContext->update & LX_ADEMOD_UFLAG_CAR_OFFS_USR)) {
					pContext->FR1_InputFreqOffset = LX_ADEMOD_FR1_VID_FREQ_OFFS_FCT;
				}
				fr1_init = -1 * pc_from_if + pContext->FR1_InputFreqOffset;
				fr1_min  =  fr1_init;
				fr1_max  =  fr1_init;

				//fr2_init = (INT64)pContext->out_frequency - ((INT64)pContext->sampling_rate / 32);
				//fr2_max  = (INT64)pContext->out_frequency - ((INT64)pContext->sampling_rate / 32);
				//fr2_min  = (INT64)pContext->out_frequency - ((INT64)pContext->sampling_rate / 32);
				fr2_init = (INT64)pContext->out_frequency;
				fr2_max  = (INT64)pContext->out_frequency;
				fr2_min  = (INT64)pContext->out_frequency;

				

				if (pContext->sampling_rate == 0) {
					return retErrZeroDiv;
				}
				if(fr1_init > 0) 	calcIntTemp = fr1_init;
				else 				calcIntTemp =  (-1) * fr1_init;
				calcIntTemp = (calcIntTemp * pContext->Fs_div * 2 * (INT64)(1 << 27));
				do_div(calcIntTemp,(pContext->sampling_rate) );
				if(fr1_init > 0) 	fr1_init = calcIntTemp;
				else 				fr1_init = (-1) * calcIntTemp;

				if(fr1_min > 0) 	calcIntTemp = fr1_min;
				else 				calcIntTemp =  (-1) * fr1_min;
				calcIntTemp = (calcIntTemp * pContext->Fs_div * 2 * (INT64)(1 << 27));
				do_div(calcIntTemp,(pContext->sampling_rate) );
				if(fr1_min > 0) 	fr1_min = calcIntTemp;
				else 				fr1_min = (-1) * calcIntTemp;

				if(fr1_max > 0) 	calcIntTemp = fr1_max;
				else 				calcIntTemp =  (-1) * fr1_max;
				calcIntTemp = (calcIntTemp * pContext->Fs_div * 2 * (INT64)(1 << 27));
				do_div(calcIntTemp,(pContext->sampling_rate) );
				if(fr1_max > 0) 	fr1_max = calcIntTemp;
				else 				fr1_max = (-1) * calcIntTemp;

				DEMOD_PRINT("fr1_init = %lld, Digital \n",fr1_init);
				DEMOD_PRINT("fr1_min = %lld,Digital \n",fr1_min);
				DEMOD_PRINT("fr1_max = %lld, Digital \n",fr1_max);
			}
			break;

		default:
			status = retErrBadMode;
			break;

	}

	if (status != retOK) {
		DEMOD_PRINT("[M14 ABB] FAIL, (%s:%d) !!\n",__F__,__L__);
		return status;
	}

	pContext->FR1_delta_ph_init = (int)fr1_init;
	pContext->FR1_delta_ph_min  = (int)fr1_min;
	pContext->FR1_delta_ph_max  = (int)fr1_max;
	DEMOD_PRINT("pContext->FR1_delta_ph_init = %d \n",pContext->FR1_delta_ph_init);
	DEMOD_PRINT("pContext->FR1_delta_ph_min = %d \n",pContext->FR1_delta_ph_min);
	DEMOD_PRINT("pContext->FR1_delta_ph_max = %d \n",pContext->FR1_delta_ph_max);

	if (!(pContext->update & LX_ADEMOD_UFLAG_OUT_FREQ_USR))
	{
		pContext->FR2_delta_ph_init = (int)fr2_init;
		pContext->FR2_delta_ph_max  = (int)fr2_max;
		pContext->FR2_delta_ph_min  = (int)fr2_min;
	}

	// range filtering
	//{
	//	UINT64 maxcutoff;
	//	UINT32 maxrange = (pContext->max_range > pContext->min_range) ? pContext->max_range : pContext->min_range;
	//	int i;

	//	// Determine the correct filter based on the range of FR1
	//	for (i = 3; i >= 0; i --)
	//	{
	//		maxcutoff =  cutoffcoeff[i] * (UINT64)pContext->sampling_rate  / (UINT64)pContext->PF1_tinc * (UINT64)(1 << 18) / 1000000 / 10  ;
	//
	//		if (maxcutoff > maxrange)
	//		{
	//			pContext->CRF_Index_Acq = i;
	//			break;
	//		}
	//	}
	//}
	pContext->CRF_Index_Lock = 3;
	pContext->AGC_SyncDec_Index_Lock = 2;
	pContext->AGC_SyncDec_Index_Acq = 0;
	pContext->MagDet_Index_Lock = 3;
	pContext->MagDet_Index_Acq = 0;

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Standard_Set(pLX_ADEMOD_VideoProc pContext, UINT32 VideoStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (VideoStandard)
	{
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M_J:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_NTSC_433:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_NTSC_50:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_B:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_G:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_H:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_I:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_D:
			pContext->fr_unlock_thresh = 0x650;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_K:
			pContext->fr_unlock_thresh = 0x650;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_M:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_N:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_60:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_NC:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_B:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_G:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_H:
			pContext->fr_unlock_thresh = 0x1000;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_D:
			pContext->fr_unlock_thresh = 0x650;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K:
			pContext->fr_unlock_thresh = 0x650;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K1:
			pContext->fr_unlock_thresh = 0x650;
			pContext->CRF_Index_Acq = 1;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
			pContext->fr_unlock_thresh = 0x200;
			pContext->CRF_Index_Acq = 0;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
			pContext->fr_unlock_thresh = 0x200;
			pContext->CRF_Index_Acq = 0;
		break;

		default:
			status = retErrBadParam;
		break;

	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_AudioCarrier_Set(pLX_ADEMOD_VideoProc pContext, UINT32 Bandwidth)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (Bandwidth)
	{
		case LX_ADEMOD_VIDEO_BANDWIDTH_4p2MHz:
			pContext->first_audio = 4400000L;
		break;

		case LX_ADEMOD_VIDEO_BANDWIDTH_5MHz:
			pContext->first_audio = 5250000L;
		break;

		case LX_ADEMOD_VIDEO_BANDWIDTH_5p34MHz:
			pContext->first_audio = 5500000L;
		break;

		case LX_ADEMOD_VIDEO_BANDWIDTH_5p5MHz:
			pContext->first_audio = 5650000L;
		break;

		case LX_ADEMOD_VIDEO_BANDWIDTH_6MHz:
			pContext->first_audio = 6058000L;
		break;

		default:
			pContext->first_audio = Bandwidth;
		break;

	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Set(pLX_ADEMOD_VideoProc pContext, UINT32* overmod, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update1 & LX_ADEMOD_UFLAG_CR_OVERMOD_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update1 |= LX_ADEMOD_UFLAG_CR_OVERMOD_USR;
	}

	pContext->overmod.mode = overmod[0];

	if (pContext->overmod.mode == LX_ADEMOD_OVERMODULATION_MODE_DISABLED) {
		pContext->flags &= ~LX_ADEMOD_FFLAG_CR_OVERMOD_MODE;
		pContext->flags &= ~LX_ADEMOD_FFLAG_CR_OVERMOD_EN;
		pContext->flags |=  LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
	}
	else {
		pContext->flags |=  LX_ADEMOD_FFLAG_CR_OVERMOD_MODE;
		pContext->flags |=  LX_ADEMOD_FFLAG_CR_OVERMOD_EN;
		pContext->flags &= ~LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT;
	}

	if (pContext->flags & LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT) {
		pContext->overmod.CRnoise_threshold_lock      = 0;
		pContext->overmod.CRnoise_threshold_acq       = 0;
		pContext->overmod.CRover_mod_threshold_lock   = 0;
		pContext->overmod.CRover_mod_threshold_acq    = 0;
		pContext->overmod.CRover_mod_threshold2_lock  = 0;
		pContext->overmod.CRover_mod_threshold2_acq   = 0;
		pContext->overmod.CRnoise_w_acq               = 0;
		pContext->overmod.CRnoise_w_lock              = 0;
		pContext->overmod.CRnoise2_w_acq              = 0;
		pContext->overmod.CRnoise2_w_lock             = 0;
	}
	else {
		pContext->overmod.CRnoise_threshold_lock      = overmod[1];
		pContext->overmod.CRnoise_threshold_acq       = overmod[2];
		pContext->overmod.CRover_mod_threshold_lock   = overmod[3];
		pContext->overmod.CRover_mod_threshold_acq    = overmod[4];
		pContext->overmod.CRover_mod_threshold2_lock  = overmod[5];
		pContext->overmod.CRover_mod_threshold2_acq   = overmod[6];
		pContext->overmod.CRnoise_w_acq               = overmod[7];
		pContext->overmod.CRnoise_w_lock              = overmod[8];
		pContext->overmod.CRnoise2_w_acq              = overmod[9];
		pContext->overmod.CRnoise2_w_lock             = overmod[10];
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, CR_NOISE_THRESH_LOCK,     (UINT32)pContext->overmod.CRnoise_threshold_lock, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_THRESH_ACQ,      (UINT32)pContext->overmod.CRnoise_threshold_acq, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH_LOCK,  (UINT32)pContext->overmod.CRover_mod_threshold_lock, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH_ACQ,   (UINT32)pContext->overmod.CRover_mod_threshold_acq, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH2_LOCK, (UINT32)pContext->overmod.CRover_mod_threshold2_lock, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH2_ACQ,  (UINT32)pContext->overmod.CRover_mod_threshold2_acq, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_W_LOCK,          (UINT32)pContext->overmod.CRnoise_w_lock, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_W_ACQ,           (UINT32)pContext->overmod.CRnoise_w_acq, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE2_W_LOCK,         (UINT32)pContext->overmod.CRnoise2_w_lock, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE2_W_ACQ,          (UINT32)pContext->overmod.CRnoise2_w_acq, 2);

#if 1
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_LOCK, 0x0020, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_ACQ,  0x0020, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_LOCK, 0x005f, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_ACQ, 0x005f, 2);

#else
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_LOCK, 0x00CC, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_ACQ,  0x00CC, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_LOCK, 0x0083, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_ACQ, 0x0083, 2);
#endif
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_CNTR_TC,      0x1FBD00, 3); // from spec
	}

	return status;

}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_OvermodPhaseRev_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 CRover_phaserev_threshold_lock  = 0;
	UINT64 CRover_phaserev_threshold_acq   = 0;
	UINT64 CRover_phaserev_angle_lock      = 0;
	UINT64 CRover_phaserev_angle_acq       = 0;

	UINT64 temp;
	UINT64 calcTemp;

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_OVERMODPHASEREV_USR) {
		// when user sticky bit is raised, don't reset to factory
		return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_OVERMODPHASEREV_USR;
	}

	pContext->overmod.phaserev = pValue[0];

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->overmod.perc = pValue[1];
			if (pContext->overmod.perc > 100)
			{
				if (Update) {
					temp = (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_THR_PREC_FCT - 1)); // -1 is for refvalue = 0.5
					calcTemp = (UINT64)(pContext->overmod.perc + 5) * temp;
					do_div(calcTemp, 100);
					CRover_phaserev_threshold_lock	= calcTemp - temp;
					CRover_phaserev_threshold_acq	= calcTemp - temp;
				}
			}
			else
			{
				status = retErrBadParam;
			}

			pContext->overmod.phaserev_angle = pValue[2];
			if (pContext->overmod.phaserev_angle > 0)
			{
				if (Update) {
					temp = (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_ANGL_PREC_FCT));
					CRover_phaserev_angle_lock	= (UINT64)(pContext->overmod.phaserev_angle) * temp;
					do_div(CRover_phaserev_angle_lock, 180);
					CRover_phaserev_angle_acq	= (UINT64)(pContext->overmod.phaserev_angle) * temp;
					do_div(CRover_phaserev_angle_acq, 180);
				}
			}
			else
			{
				status = retErrBadParam;
			}

			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			if (Update) {
				CRover_phaserev_threshold_lock  = ((UINT64)LX_ADEMOD_VID_OM_PHREV_THR_LOCK_FCT  * (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_THR_PREC_FCT)));
				do_div(CRover_phaserev_threshold_lock, 1000000000);
				do_div(CRover_phaserev_threshold_lock, 10000);

				CRover_phaserev_threshold_acq   = ((UINT64)LX_ADEMOD_VID_OM_PHREV_THR_ACK_FCT   * (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_THR_PREC_FCT))); 
				do_div(CRover_phaserev_threshold_acq, 1000000000);
				do_div(CRover_phaserev_threshold_acq, 10000);
				CRover_phaserev_angle_lock      = ((UINT64)LX_ADEMOD_VID_OM_PHREV_ANGL_LOCK_FCT * (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_ANGL_PREC_FCT)));
				do_div(CRover_phaserev_angle_lock, 1000000000);
				do_div(CRover_phaserev_angle_lock, 10000);
				CRover_phaserev_angle_acq       = ((UINT64)LX_ADEMOD_VID_OM_PHREV_ANGL_ACK_FCT  * (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_ANGL_PREC_FCT)));
				do_div(CRover_phaserev_angle_acq, 1000000000);
				do_div(CRover_phaserev_angle_acq, 10000);
			}
		break;

		case LX_ADEMOD_PRESET_OPTION_DISABLE:
			CRover_phaserev_threshold_lock  = 0;
			CRover_phaserev_threshold_acq   = 0;
			CRover_phaserev_angle_lock      = 0;
			CRover_phaserev_angle_acq       = 0;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		pContext->overmod.phaserev = pValue[0];
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, CR_MAG_THRESH_LOCK,       (UINT32)CRover_phaserev_threshold_lock, 2);
		ADEMOD_WriteSub( hUser, CR_MAG_THRESH_ACQ,        (UINT32)CRover_phaserev_threshold_acq, 2);
		ADEMOD_WriteSub( hUser, CR_ANGLE_THRESH_LOCK,     (UINT32)CRover_phaserev_angle_lock, 2);
		ADEMOD_WriteSub( hUser, CR_ANGLE_THRESH_ACQ,      (UINT32)CRover_phaserev_angle_acq, 2);
	}

	return status;

}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_CRReset_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_CRRESET_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_CRRESET_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			// enable CR reset with direct parameters
			pContext->flags &= ~LX_ADEMOD_FFLAG_CR_ALGDIS;
			pContext->Reset_Fr1_thresh   = pValue[1];
			pContext->Reset_CRR_bound_TC = pValue[2];
			pContext->Reset_CRR_lock_TC  = pValue[3];
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			// enable CR reset with default parameters
			pContext->flags &= ~LX_ADEMOD_FFLAG_CR_ALGDIS;
			pContext->Reset_Fr1_thresh = LX_ADEMOD_CR_FR1_THRESH_FCT;
			pContext->Reset_CRR_bound_TC = LX_ADEMOD_CR_CRR_BOUND_TC_FCT;
			pContext->Reset_CRR_lock_TC = LX_ADEMOD_CR_CRR_LOCK_TC_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_DISABLE:
			// disable CR reset
			pContext->flags |= LX_ADEMOD_FFLAG_CR_ALGDIS;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		DEMOD_M14_Analog_CarrierRecovery_Reset(pContext, hUser);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Overmodulation_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	pValue[0] = pContext->overmod.mode;

	return status;

}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_FRLoopSpeed_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_FR_LOOPSPEED_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_FR_LOOPSPEED_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->fr_loop_speed_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->fr_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->fr_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->fr_loop_speed_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->fr_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->fr_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_FRLpfCoeff_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_FR_COEFF_B_ACQ,    pContext->fr_lpf_coeff_acq , 2);
		ADEMOD_WriteSub( hUser, CR_FR_COEFF_B_LOCK,   pContext->fr_lpf_coeff_lock, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_FRLoopGain_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_FR_LOOPGAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_FR_LOOPGAIN_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->fr_loop_gain_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->fr_loop_gain_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->fr_loop_gain_scale = LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->fr_loop_gain_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->fr_loop_gain_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->fr_loop_gain_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LPCOUPLE:
			pContext->fr_loop_gain_scale = LX_ADEMOD_CR_PRESET_SCALE_COUPLE_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_FRLpfGain_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_FR_FILT_GAIN_ACQ,    pContext->fr_lpf_gain_acq , 2);
		ADEMOD_WriteSub( hUser, CR_FR_FILT_GAIN_LOCK,   pContext->fr_lpf_gain_lock, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_PHLoopSpeed_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_PH_LOOPSPEED_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_PH_LOOPSPEED_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->ph_loop_speed_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->ph_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->ph_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->ph_loop_speed_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->ph_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->ph_loop_speed_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_PHLpfCoeff_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_PH_COEFF_B_ACQ,    pContext->ph_lpf_coeff_acq , 2);
		ADEMOD_WriteSub( hUser, CR_PH_COEFF_B_LOCK,   pContext->ph_lpf_coeff_lock, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_PHLoopGain_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_PH_LOOPGAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_PH_LOOPGAIN_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->ph_loop_gain_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->ph_loop_gain_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->ph_loop_gain_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->ph_loop_gain_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->ph_loop_gain_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->ph_loop_gain_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LPCOUPLE:
			pContext->ph_loop_gain_scale = (UINT8)LX_ADEMOD_CR_PRESET_SCALE_COUPLE_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_PHLpfGain_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_PH_FILT_GAIN_ACQ,    pContext->ph_lpf_gain_acq , 2);
		ADEMOD_WriteSub( hUser, CR_PH_FILT_GAIN_LOCK,   pContext->ph_lpf_gain_lock, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	status = DEMOD_M14_Analog_VideoProc_LockDetErr_Set(pContext, pValue, hUser, Update);
	status = DEMOD_M14_Analog_VideoProc_LockDetLines_Set(pContext, pValue, hUser, Update);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_LOCKDETERR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_LOCKDETERR_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->ph_lock_det_err_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_LockThresh_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_FR_LOCK_THRESH,    pContext->fr_lock_thresh, 3);
		ADEMOD_WriteSub( hUser, CR_PH_LOCK_THRESH,    pContext->ph_lock_thresh, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_LockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_LOCKDETLINES_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_LOCKDETLINES_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->ph_lock_det_lines_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_LockCounter_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_LOCK_CNTR_WINDOW,    pContext->fr_lock_counter, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_UnlockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	status = DEMOD_M14_Analog_VideoProc_UnlockDetErr_Set(pContext, pValue, hUser, Update);
	status = DEMOD_M14_Analog_VideoProc_UnlockDetLines_Set(pContext, pValue, hUser, Update);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_UnlockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_UNLOCKDETERR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_UNLOCKDETERR_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->ph_unlock_det_err_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_UnlockThresh_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_FR_UNLOCK_THRESH,    pContext->fr_unlock_thresh, 3);
		ADEMOD_WriteSub( hUser, CR_PH_UNLOCK_THRESH,    pContext->ph_unlock_thresh, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_UnlockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_UNLOCKDETLINES_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_UNLOCKDETLINES_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->ph_unlock_det_lines_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_UnlockCounter_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, CR_UNLOCK_CNTR_WINDOW,    pContext->fr_unlock_counter, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MutelockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	status = DEMOD_M14_Analog_VideoProc_MutelockDetErr_Set(pContext, pValue, hUser, Update);
	status = DEMOD_M14_Analog_VideoProc_MutelockDetLines_Set(pContext, pValue, hUser, Update);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MutelockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_MUTELOCKDETERR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_MUTELOCKDETERR_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->mute_ph_lock_det_err_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->mute_ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->mute_ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->mute_ph_lock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_MutelockThresh_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, AUDIOMUTECR_FR_LOCK_THRESH,    pContext->mute_fr_lock_thresh, 3);
		ADEMOD_WriteSub( hUser, AUDIOMUTECR_PH_LOCK_THRESH,    pContext->mute_ph_lock_thresh, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MutelockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_MUTELOCKDETLINES_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_MUTELOCKDETLINES_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->mute_ph_lock_det_lines_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->mute_ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->mute_ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->mute_ph_lock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_MutelockCounter_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, AUDIOMUTECR_LOCK_CNTR_WINDOW,    pContext->mute_fr_lock_counter, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MuteunlockDet_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	status = DEMOD_M14_Analog_VideoProc_MuteunlockDetErr_Set(pContext, pValue, hUser, Update);
	status = DEMOD_M14_Analog_VideoProc_MuteunlockDetLines_Set(pContext, pValue, hUser, Update);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MuteunlockDetErr_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_MUTEUNLOCKDETERR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_MUTEUNLOCKDETERR_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->mute_ph_unlock_det_err_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->mute_ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->mute_ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->mute_ph_unlock_det_err_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_MuteunlockThresh_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, AUDIOMUTECR_FR_UNLOCK_THRESH,    pContext->mute_fr_unlock_thresh, 3);
		ADEMOD_WriteSub( hUser, AUDIOMUTECR_PH_UNLOCK_THRESH,    pContext->mute_ph_unlock_thresh, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_MuteunlockDetLines_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_MUTEUNLOCKDETLINES_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_MUTEUNLOCKDETLINES_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			if ((pValue[1] >= LX_ADEMOD_CR_PRESET_SCALE_MIN_FCT) && (pValue[1] <= LX_ADEMOD_CR_PRESET_SCALE_MAX_FCT)) {
				pContext->mute_ph_unlock_det_lines_scale = pValue[1];
			}
			else {
				status = retErrBadParam;
			}
			break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->mute_ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_NOMINAL_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->mute_ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->mute_ph_unlock_det_lines_scale = LX_ADEMOD_CR_PRESET_SCALE_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update && status == retOK) {
		status = DEMOD_M14_Analog_MuteunlockCounter_Set(pContext, hUser);
		ADEMOD_WriteSub( hUser, AUDIOMUTECR_UNLOCK_CNTR_WINDOW,    pContext->mute_fr_unlock_counter, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VirtualFS_Set(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_VID_VIRTUALFS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_VID_VIRTUALFS_USR;
	}

	pContext->virtualfs = (UINT64)pValue[0] + ((UINT64)pValue[1] << 32);

	if (Update) {
		if (pContext->input_mode != LX_ADEMOD_INPUTMODE_NONE) {
			status = DEMOD_M14_Analog_VideoProc_Mode_Set(pContext, pContext->input_mode);
			status = DEMOD_M14_Analog_VideoProc_Start(pContext, hUser);
		}
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_VirtualFS_Get(pLX_ADEMOD_VideoProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = (UINT32)pContext->virtualfs;
	pValue[1] = (UINT32)(pContext->virtualfs >> 32);

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Init(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	DEMOD_M14_Analog_CarrierRecovery_Init(pContext, hUser);

	// To mitigate the 3MHz in-band spur,
	// disable the PR_SmoothEn bit in the PR_SmoothingBits for now. (BUG 874)
	ADEMOD_WriteSub( hUser, PR_SMOOTHINGBITS, 0, 1);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Start(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8  CrCntl = 0;
	UINT8  CRF_VSB_FiltIndex1 = 0;
	UINT8  CRF_VSB_FiltIndex2 = 0;
	UINT8  PreF1_CtrlBits = 0;
	UINT32 Param[3];
	UINT64 CRover_phaserev_threshold_lock  = 0;
	UINT64 CRover_phaserev_threshold_acq   = 0;
	UINT64 CRover_phaserev_angle_lock      = 0;
	UINT64 CRover_phaserev_angle_acq       = 0;
	UINT64 temp;
	UINT64 calcTemp=0;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if (!pContext) {
		return retErrBadContext;
	}

	status = DEMOD_M14_Analog_FRLpfCoeff_Set(pContext, hUser);

	status = DEMOD_M14_Analog_FRLpfGain_Set(pContext, hUser);

	status = DEMOD_M14_Analog_PHLpfCoeff_Set(pContext, hUser);

	status = DEMOD_M14_Analog_PHLpfGain_Set(pContext, hUser);

	status = DEMOD_M14_Analog_LockThresh_Set(pContext, hUser);

	status = DEMOD_M14_Analog_LockCounter_Set(pContext, hUser);

	status = DEMOD_M14_Analog_UnlockThresh_Set(pContext, hUser);

	status = DEMOD_M14_Analog_UnlockCounter_Set(pContext, hUser);

	status = DEMOD_M14_Analog_MutelockThresh_Set(pContext, hUser);

	status = DEMOD_M14_Analog_MutelockCounter_Set(pContext, hUser);

	status = DEMOD_M14_Analog_MuteunlockThresh_Set(pContext, hUser);

	status = DEMOD_M14_Analog_MuteunlockCounter_Set(pContext, hUser);

	// Update Video Registers
	DEMOD_M14_Analog_VideoProc_Reset(pContext, TRUE, hUser);
	DEMOD_PRINT(" pContext->PF1_tinc = %d \n",pContext->PF1_tinc);
	DEMOD_PRINT(" pContext->FR1_delta_ph_min = %d \n",pContext->FR1_delta_ph_min);
	DEMOD_PRINT(" pContext->FR1_delta_ph_max = %d \n", pContext->FR1_delta_ph_max);
	DEMOD_PRINT(" pContext->FR1_delta_ph_init = %d \n",pContext->FR1_delta_ph_init);
	DEMOD_PRINT(" pContext->PR_delivery_rate = %d \n",pContext->PR_delivery_rate);
	DEMOD_PRINT(" pContext->PF2_tinc = %d \n",pContext->PF2_tinc);
	DEMOD_PRINT(" pContext->FR2_delta_ph_min = %d \n",pContext->FR2_delta_ph_min);
	DEMOD_PRINT(" pContext->FR2_delta_ph_max = %d \n",pContext->FR2_delta_ph_max);
	DEMOD_PRINT(" pContext->FR2_delta_ph_init = %d \n",pContext->FR2_delta_ph_init);
	
	ADEMOD_WriteSub( hUser,	PF1_TINC,          pContext->PF1_tinc, 3);
	ADEMOD_WriteSub( hUser, FR1_DELTA_PH_MIN,  pContext->FR1_delta_ph_min, 4);
	ADEMOD_WriteSub( hUser, FR1_DELTA_PH_MAX,  pContext->FR1_delta_ph_max, 4);
	ADEMOD_WriteSub( hUser, FR1_DELTA_PH_INIT, pContext->FR1_delta_ph_init, 4);
	ADEMOD_WriteSub( hUser, FR1_MANUAL_PH,     pContext->FR1_delta_ph_init, 4);
	ADEMOD_WriteSub( hUser, PR_DELIVERYRATE,   pContext->PR_delivery_rate, 3);

	ADEMOD_WriteSub( hUser, PF2_TINC,          pContext->PF2_tinc, 3);
	ADEMOD_WriteSub( hUser, FR2_DELTA_PH_MIN,  pContext->FR2_delta_ph_min, 4);
	ADEMOD_WriteSub( hUser, FR2_DELTA_PH_MAX,  pContext->FR2_delta_ph_max, 4);
	ADEMOD_WriteSub( hUser, FR2_DELTA_PH_INIT, pContext->FR2_delta_ph_init, 4);

	if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
		UINT32 FR2_manual_ph = (UINT32)(
			((pContext->narrowband_offs1 & 0x3)	<< 0) +
			((pContext->narrowband_offs2 & 0x3)	<< 2) +
			((pContext->narrowband_offs3 & 0x3)	<< 4) +
			(pContext->FR2_manual_ph)
		);
		ADEMOD_WriteSub( hUser, FR2_MANUAL_PH,     FR2_manual_ph, 4);
	}
	else {
		ADEMOD_WriteSub( hUser, FR2_MANUAL_PH,     pContext->FR2_manual_ph, 4);
	}

	ADEMOD_WriteSub( hUser, PF2_DELIVERYRATE,  pContext->PF2_delivery_rate, 1);
	ADEMOD_WriteSub( hUser, PF2_TINCINV,       pContext->PF2_tincInv, 3);

	temp = (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_THR_PREC_FCT - 1)); // -1 is for refvalue = 0.5
	calcTemp = (UINT64)(pContext->overmod.perc + 5) * temp;
	do_div(calcTemp, 100);

	CRover_phaserev_threshold_lock	= calcTemp - temp;
	CRover_phaserev_threshold_acq	= calcTemp - temp;

	temp = (UINT64)(1 << (LX_ADEMOD_VID_OM_PHREV_ANGL_PREC_FCT));
	calcTemp = (UINT64)(pContext->overmod.phaserev_angle) * temp;
	do_div(calcTemp, 180);

	CRover_phaserev_angle_lock	= calcTemp;
	CRover_phaserev_angle_acq	= calcTemp;

	ADEMOD_WriteSub( hUser, CR_MAG_THRESH_LOCK,       (UINT32)CRover_phaserev_threshold_lock, 2);
	ADEMOD_WriteSub( hUser, CR_MAG_THRESH_ACQ,        (UINT32)CRover_phaserev_threshold_acq, 2);
	ADEMOD_WriteSub( hUser, CR_ANGLE_THRESH_LOCK,     (UINT32)CRover_phaserev_angle_lock, 2);
	ADEMOD_WriteSub( hUser, CR_ANGLE_THRESH_ACQ,      (UINT32)CRover_phaserev_angle_acq, 2);

	ADEMOD_WriteSub( hUser, CR_FR_COEFF_B_ACQ,    pContext->fr_lpf_coeff_acq , 2);
	ADEMOD_WriteSub( hUser, CR_FR_COEFF_B_LOCK,   pContext->fr_lpf_coeff_lock, 2);
	ADEMOD_WriteSub( hUser, CR_FR_FILT_GAIN_ACQ,  pContext->fr_lpf_gain_acq , 2);
	ADEMOD_WriteSub( hUser, CR_FR_FILT_GAIN_LOCK, pContext->fr_lpf_gain_lock, 2);

	ADEMOD_WriteSub( hUser, CR_PH_COEFF_B_ACQ,    pContext->ph_lpf_coeff_acq , 2);
	ADEMOD_WriteSub( hUser, CR_PH_COEFF_B_LOCK,   pContext->ph_lpf_coeff_lock, 2);
	ADEMOD_WriteSub( hUser, CR_PH_FILT_GAIN_ACQ,  pContext->ph_lpf_gain_acq , 2);
	ADEMOD_WriteSub( hUser, CR_PH_FILT_GAIN_LOCK, pContext->ph_lpf_gain_lock, 2);

	ADEMOD_WriteSub( hUser, CR_FR_LOCK_THRESH,    pContext->fr_lock_thresh, 3);
	ADEMOD_WriteSub( hUser, CR_PH_LOCK_THRESH,    pContext->ph_lock_thresh, 3);
	ADEMOD_WriteSub( hUser, CR_LOCK_CNTR_WINDOW,  pContext->fr_lock_counter, 3);

	ADEMOD_WriteSub( hUser, CR_FR_UNLOCK_THRESH,    pContext->fr_unlock_thresh, 3);
	ADEMOD_WriteSub( hUser, CR_PH_UNLOCK_THRESH,    pContext->ph_unlock_thresh, 3);
	ADEMOD_WriteSub( hUser, CR_UNLOCK_CNTR_WINDOW,  pContext->fr_unlock_counter, 3);

	ADEMOD_WriteSub( hUser, AUDIOMUTECR_FR_LOCK_THRESH,    pContext->mute_fr_lock_thresh, 3);
	ADEMOD_WriteSub( hUser, AUDIOMUTECR_PH_LOCK_THRESH,    pContext->mute_ph_lock_thresh, 3);
	ADEMOD_WriteSub( hUser, AUDIOMUTECR_LOCK_CNTR_WINDOW,  pContext->mute_fr_lock_counter, 3);

	ADEMOD_WriteSub( hUser, AUDIOMUTECR_FR_UNLOCK_THRESH,  pContext->mute_fr_unlock_thresh, 3);
	ADEMOD_WriteSub( hUser, AUDIOMUTECR_PH_UNLOCK_THRESH,  pContext->mute_ph_unlock_thresh, 3);
	ADEMOD_WriteSub( hUser, AUDIOMUTECR_UNLOCK_CNTR_WINDOW,  pContext->mute_fr_unlock_counter, 3);

	CRF_VSB_FiltIndex1 = (UINT8)(
		((pContext->CRF_Index_Lock)                     << 0) +
		((pContext->CRF_Index_Acq)                      << 2) +
		((pContext->AGC_SyncDec_Index_Lock)             << 4) +
		((pContext->AGC_SyncDec_Index_Acq)              << 6)
	);
	CRF_VSB_FiltIndex2 = (UINT8)(
		((pContext->MagDet_Index_Lock)                  << 0) +
		((pContext->MagDet_Index_Acq)                   << 2)
	);

	ADEMOD_WriteSub( hUser, CRF_VSB_FILTINDEX1, CRF_VSB_FiltIndex1, 1);
	ADEMOD_WriteSub( hUser, CRF_VSB_FILTINDEX2, CRF_VSB_FiltIndex2, 1);


	// filter range
	PreF1_CtrlBits = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_DMUX_ROTATORFLIP)   ? 1 : 0) << 1)
	);
	ADEMOD_WriteSub( hUser, PREF1_CTRLBITS, PreF1_CtrlBits, 1);

	// Over-modulation
	if (pContext->flags & LX_ADEMOD_FFLAG_CR_OVERMOD_DISWEIGHT) {
		ADEMOD_WriteSub( hUser, CR_NOISE_THRESH_LOCK,     0, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_THRESH_ACQ,      0, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH_LOCK,  0, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH_ACQ,   0, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH2_LOCK, 0, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH2_ACQ,  0, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_W_LOCK,          0, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_W_ACQ,           0, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE2_W_LOCK,         0, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE2_W_ACQ,          0, 2);
	}
	else {
		ADEMOD_WriteSub( hUser, CR_NOISE_THRESH_LOCK,     (UINT32)pContext->overmod.CRnoise_threshold_lock, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_THRESH_ACQ,      (UINT32)pContext->overmod.CRnoise_threshold_acq, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH_LOCK,  (UINT32)pContext->overmod.CRover_mod_threshold_lock, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH_ACQ,   (UINT32)pContext->overmod.CRover_mod_threshold_acq, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH2_LOCK, (UINT32)pContext->overmod.CRover_mod_threshold2_lock, 2);
		ADEMOD_WriteSub( hUser, CR_OVER_MOD_THRESH2_ACQ,  (UINT32)pContext->overmod.CRover_mod_threshold2_acq, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_W_LOCK,          (UINT32)pContext->overmod.CRnoise_w_lock, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE_W_ACQ,           (UINT32)pContext->overmod.CRnoise_w_acq, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE2_W_LOCK,         (UINT32)pContext->overmod.CRnoise2_w_lock, 2);
		ADEMOD_WriteSub( hUser, CR_NOISE2_W_ACQ,          (UINT32)pContext->overmod.CRnoise2_w_acq, 2);
	}

#if 1
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_LOCK, 0x0020, 2);
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_ACQ,  0x0020, 2);
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_LOCK, 0x005f, 2);
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_ACQ, 0x005f, 2);

#else
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_LOCK, 0x00CC, 2);
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF_ACQ,  0x00CC, 2);
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_LOCK, 0x0083, 2);
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_W_COEFF2_ACQ, 0x0083, 2);
#endif
	ADEMOD_WriteSub( hUser, CR_OVER_MOD_CNTR_TC,      0x1FBD00, 3); // from spec

	Param[0] = pContext->overmod.phaserev;
	Param[1] = pContext->overmod.perc;
	Param[2] = pContext->overmod.phaserev_angle;
	DEMOD_M14_Analog_VideoProc_OvermodPhaseRev_Set(pContext, Param, hUser, TRUE);

	CrCntl = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_CR_OVERMOD_EN  ) ? 1 : 0) << 0) +
		(((pContext->flags & LX_ADEMOD_FFLAG_CR_OVERMOD_MODE) ? 1 : 0) << 1) +
		(((pContext->flags & LX_ADEMOD_FFLAG_CR_PH_OVERRIDE)  ? 1 : 0) << 2) +
		(((pContext->flags & LX_ADEMOD_FFLAG_F5GAIN_1X_N2X)   ? 1 : 0) << 5) +
		(((pContext->flags & LX_ADEMOD_FFLAG_CR_ALGDIS)       ? 0 : 1) << 6)
	);
	ADEMOD_WriteByte( hUser, CR_STATUSCNTRBITS, CrCntl);

	DEMOD_M14_Analog_CarrierRecovery_Reset(pContext, hUser);

	DEMOD_M14_Analog_VideoProc_Reset(pContext, FALSE, hUser);


	// Toggle PLL reset
	DEMOD_M14_Analog_VideoProc_PLL_Reset(pContext, TRUE, hUser);
	DEMOD_M14_Analog_VideoProc_PLL_Reset(pContext, FALSE, hUser);

#if 0	// j.y. won : 2012.02.03
	if (pContext->flags & LX_ADEMOD_FFLAG_44n36) {
		ADEMOD_WriteSub( hUser, ADCIF_MIXERFREQ, 0x5554, 2); // 0.6666259765625 * 32768
	}
	else {
		ADEMOD_WriteSub( hUser, ADCIF_MIXERFREQ, 0xC000, 2); // -0.5 * 32768
	}
#endif

	// clear update flag
	pContext->update &= ~LX_ADEMOD_UFLAG_UPDATE;

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;

}

/*
** Static functions implementation
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_CarrierRecovery_Init
**
**  Description:    Initializes Carrier Recovery block.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_CarrierRecovery_Init(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	ADEMOD_WriteSub( hUser, CR_PHASE_180_DETECT_EN, 0x00, 1);
	ADEMOD_WriteSub( hUser, CR_MANUAL_PH, 0x00, 2); // Carrier recovery manual mixing freq in open loop
	ADEMOD_WriteSub( hUser, CR_MULT_MIX_ENABLEBITS, 0x0F, 1);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_CarrierRecovery_Reset
**
**  Description:    Resets Carrier Recovery loop.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_CarrierRecovery_Reset(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 temp_delta;
	UINT64 temp_BoundTC;
	UINT64 temp_LockTC;


	if (!pContext) {
		DEMOD_PRINT( "[ABB M14](%s:%d)  Bad pContext !!!\n",__F__,__L__);
		return retErrBadContext;
	}

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);
	DEMOD_PRINT("pContext->Fs_div = %d, pContext->sampling_rate = %d \n",pContext->Fs_div, pContext->sampling_rate);

	if ((pContext->Fs_div == 0) || (pContext->sampling_rate == 0)) {
		return retErrZeroDiv;
	}
	temp_delta = (pContext->Reset_Fr1_thresh * (UINT64)(1 << 16) * 2 * pContext->Fs_div + 500000);
	do_div(temp_delta,(pContext->sampling_rate) );

//	temp_delta = (pContext->Reset_Fr1_thresh * (INT64)(1 << 16) * 2 * pContext->Fs_div + 500000) / pContext->sampling_rate;

	if (temp_delta > 4095) {
		temp_delta = 4095;
	}
	temp_BoundTC  = pContext->Reset_CRR_bound_TC * (INT64)(pContext->sampling_rate);
	do_div(temp_BoundTC, ( pContext->Fs_div));
	do_div(temp_BoundTC, 1000);
	do_div(temp_BoundTC, 2048);
	temp_BoundTC  = (temp_BoundTC + 500000);
	do_div(temp_BoundTC,1000000 );

//	temp_BoundTC  = (pContext->Reset_CRR_bound_TC * (INT64)(pContext->sampling_rate / pContext->Fs_div / 1000 )) / 2048;
//	temp_BoundTC  = (temp_BoundTC + 500000) / 1000000;

	if (temp_BoundTC > 4095) {
		temp_BoundTC = 4095;
	}

	temp_LockTC  = pContext->Reset_CRR_lock_TC * (INT64)(pContext->sampling_rate);
	do_div(temp_LockTC, ( pContext->Fs_div));
	do_div(temp_LockTC, 1000);
	do_div(temp_LockTC, 2048);
	temp_LockTC  = (temp_BoundTC + 500000);
	do_div(temp_LockTC,1000000 );

//	temp_LockTC   = (pContext->Reset_CRR_lock_TC * (INT64)(pContext->sampling_rate / pContext->Fs_div / 1000 )) / 2048;
//	temp_LockTC   = (temp_LockTC + 500000) / 1000000;

	if (temp_LockTC > 4095) {
		temp_LockTC = 4095;
	}

	ADEMOD_WriteSub( hUser, FR1_DELTA_PH_BOUND, (UINT32)temp_delta, 2);
	ADEMOD_WriteSub( hUser, CRRESETBOUNDTC, (UINT32)temp_BoundTC, 2);
	ADEMOD_WriteSub( hUser, CRRESETFRLOCKTC, (UINT32)temp_LockTC, 2);

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_FRLpfCoeff_Set
**
**  Description:    Sets FR LPF Coefficients according to the FR loop speed.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_FRLpfCoeff_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64  fr_lpf_coeff_norm_acq = 0;
	UINT64  fr_lpf_coeff_norm_lock = 0;

	UINT64 calcTemp=0;

	if( pContext->fr_loop_speed_scale )  // speed scale is provided
	{
		UINT64 W_lpf_3dB_base = 0;
		UINT64 temp = 0;
		if ((pContext->Fs_div == 0) || (pContext->PF1_tinc == 0)) {
			return retErrZeroDiv;
		}
		// the strange looking code fragment below is in fact taking care of the overflow of the local variables.
		// Do not modify it unless you really need to...

		calcTemp = (UINT64)pContext->sampling_rate;
		do_div(calcTemp, (pContext->Fs_div));
		calcTemp = calcTemp * (UINT64)LX_ADEMOD_CR_LPF_3DB_FCT;
		do_div(calcTemp, 10000);
		calcTemp = calcTemp * (UINT64)(1 << 18);
		do_div(calcTemp, 1000000000 );
		do_div(calcTemp, (pContext->PF1_tinc));
		W_lpf_3dB_base = calcTemp;

//		W_lpf_3dB_base = (((UINT64)pContext->sampling_rate / pContext->Fs_div * (UINT64)LX_ADEMOD_CR_LPF_3DB_FCT / (UINT64)10000) * (UINT64)(1 << 18)) / (UINT64)1000000000 / (UINT64)pContext->PF1_tinc;

		temp = (UINT64)LX_ADEMOD_CR_FR_LOOP_NORM_ACK_FCT * 10;
		do_div(temp, W_lpf_3dB_base);
		temp = temp + 5;
		do_div(temp, 10);

//		temp = ((UINT64)LX_ADEMOD_CR_FR_LOOP_NORM_ACK_FCT * 10 / (W_lpf_3dB_base ) + 5 ) / 10 ;

		fr_lpf_coeff_norm_acq = temp * pContext->fr_loop_speed_scale;
		do_div(fr_lpf_coeff_norm_acq, 100);

		if (W_lpf_3dB_base == 0) {
			return retErrZeroDiv;
		}

		temp = (UINT64)LX_ADEMOD_CR_FR_LOOP_NORM_LOCK_FCT * 10;
		do_div(temp, W_lpf_3dB_base);
		temp = temp + 5;
		do_div(temp, 10);

// 		temp = ((UINT64)LX_ADEMOD_CR_FR_LOOP_NORM_LOCK_FCT * 10 / (W_lpf_3dB_base ) + 5 ) / 10 ;

		fr_lpf_coeff_norm_lock = temp * pContext->fr_loop_speed_scale;
		do_div(fr_lpf_coeff_norm_lock, 100);

		pContext->fr_lpf_coeff_acq  = (UINT16)fr_lpf_coeff_norm_acq;
		pContext->fr_lpf_coeff_lock = (UINT16)fr_lpf_coeff_norm_lock;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_FRLpfGain_Set
**
**  Description:    Sets FR LPF Gain according to the FR loop speed.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_FRLpfGain_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64  fr_lpf_gain_norm_acq = LX_ADEMOD_CR_FR_GAIN_NORM_ACK_FCT;
	UINT64  fr_lpf_gain_norm_lock = LX_ADEMOD_CR_FR_GAIN_NORM_LOCK_FCT;

	if( pContext->fr_loop_gain_scale )  // gain scale is provided
	{
		fr_lpf_gain_norm_acq  = fr_lpf_gain_norm_acq  * pContext->fr_loop_gain_scale;
		do_div(fr_lpf_gain_norm_acq, 100);
		fr_lpf_gain_norm_lock = fr_lpf_gain_norm_lock * pContext->fr_loop_gain_scale;
		do_div(fr_lpf_gain_norm_lock, 100);

		pContext->fr_lpf_gain_acq  = (UINT16)fr_lpf_gain_norm_acq;
		pContext->fr_lpf_gain_lock = (UINT16)fr_lpf_gain_norm_lock;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_PHLpfCoeff_Set
**
**  Description:    Sets PH LPF Coefficients according to the PH loop speed.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_PHLpfCoeff_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64  ph_lpf_coeff_norm_acq  = LX_ADEMOD_CR_PH_LOOP_NORM_ACK_FCT;
	UINT64  ph_lpf_coeff_norm_lock = LX_ADEMOD_CR_PH_LOOP_NORM_LOCK_FCT;
	UINT64 W_lpf_3dB_base = 0;

	UINT64 temp=0;

	if ((pContext->Fs_div == 0) || (pContext->PF1_tinc == 0)) {
		return retErrZeroDiv;
	}

	W_lpf_3dB_base = (UINT64)(pContext->sampling_rate);
	do_div(W_lpf_3dB_base, (pContext->Fs_div));
	W_lpf_3dB_base = W_lpf_3dB_base * (UINT64)LX_ADEMOD_CR_LPF_3DB_FCT;
	do_div(W_lpf_3dB_base, 10000);
	W_lpf_3dB_base = W_lpf_3dB_base * (UINT64)(1 << 18);
	do_div(W_lpf_3dB_base, 1000000000);
	do_div(W_lpf_3dB_base, (pContext->PF1_tinc));
	
//	W_lpf_3dB_base = (((UINT64)pContext->sampling_rate / pContext->Fs_div * (UINT64)LX_ADEMOD_CR_LPF_3DB_FCT / (UINT64)10000) * (UINT64)(1 << 18)) / (UINT64)1000000000 / (UINT64)pContext->PF1_tinc ;

	if( pContext->ph_loop_speed_scale )  // speed scale is provided
	{
		temp = (UINT64)LX_ADEMOD_CR_PH_LOOP_NORM_ACK_FCT;
		do_div(temp, W_lpf_3dB_base);

//		temp = ((UINT64)LX_ADEMOD_CR_PH_LOOP_NORM_ACK_FCT  / (W_lpf_3dB_base ));

		ph_lpf_coeff_norm_acq = temp * pContext->ph_loop_speed_scale;
		do_div(ph_lpf_coeff_norm_acq, 100);

		temp = (UINT64)LX_ADEMOD_CR_PH_LOOP_NORM_LOCK_FCT;
		do_div(temp, W_lpf_3dB_base);
		
//		temp = ((UINT64)LX_ADEMOD_CR_PH_LOOP_NORM_LOCK_FCT/ (W_lpf_3dB_base ));

		ph_lpf_coeff_norm_lock = temp * pContext->ph_loop_speed_scale;
		do_div(ph_lpf_coeff_norm_lock, 100 );

		pContext->ph_lpf_coeff_acq  = (UINT16)ph_lpf_coeff_norm_acq;
		pContext->ph_lpf_coeff_lock = (UINT16)ph_lpf_coeff_norm_lock;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_PHLpfGain_Set
**
**  Description:    Sets PH LPF Gain according to the PH loop speed.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_PHLpfGain_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64  ph_lpf_gain_norm_acq = LX_ADEMOD_CR_PH_GAIN_NORM_ACK_FCT;
	UINT64  ph_lpf_gain_norm_lock = LX_ADEMOD_CR_PH_GAIN_NORM_LOCK_FCT;

	if( pContext->ph_loop_gain_scale )  // gain scale is provided
	{
		ph_lpf_gain_norm_acq  = ph_lpf_gain_norm_acq  * pContext->ph_loop_gain_scale;
		do_div(ph_lpf_gain_norm_acq, 100);
		ph_lpf_gain_norm_lock = ph_lpf_gain_norm_lock * pContext->ph_loop_gain_scale;
		do_div(ph_lpf_gain_norm_lock, 100);

		pContext->ph_lpf_gain_acq  = (UINT16)ph_lpf_gain_norm_acq;
		pContext->ph_lpf_gain_lock = (UINT16)ph_lpf_gain_norm_lock;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LockThresh_Set
**
**  Description:    Sets Lock detection threshould according to the lock detection error.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64 fr_lock_threshold = 0;
	UINT64 ph_lock_threshold = 0;
	UINT64 fr_lock_threshold_norm = 0;
	UINT64 ph_lock_threshold_norm = 0;
	UINT64 temp = 0;

	if (pContext->sampling_rate == 0) {
		return retErrZeroDiv;
	}
	temp = (UINT64)LX_ADEMOD_CR_FR_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_LOCK_THR_PREC_FCT) );
	do_div(temp, (pContext->sampling_rate));
	do_div(temp,  (1<<18));

//	temp = (UINT64)LX_ADEMOD_CR_FR_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_LOCK_THR_PREC_FCT) )/ (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;

	fr_lock_threshold_norm = temp ;
	temp = (UINT64)LX_ADEMOD_CR_PH_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_LOCK_THR_PREC_FCT) );
	do_div(temp,(pContext->sampling_rate) );
	do_div(temp, (1<<18) );

//	temp = (UINT64)LX_ADEMOD_CR_PH_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_LOCK_THR_PREC_FCT) )/ (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;

	ph_lock_threshold_norm = temp  ;

	if( pContext->ph_lock_det_err_scale )  // lock detection error is provided
	{
		fr_lock_threshold = fr_lock_threshold_norm * pContext->ph_lock_det_err_scale;
		do_div(fr_lock_threshold, 100);
		ph_lock_threshold = ph_lock_threshold_norm * pContext->ph_lock_det_err_scale;
		do_div(ph_lock_threshold, 100);
		
		pContext->fr_lock_thresh  = (UINT32)fr_lock_threshold;
		pContext->ph_lock_thresh  = (UINT32)ph_lock_threshold;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LockCounter_Set
**
**  Description:    Sets Lock control window according to the lock detection lines.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_LockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	INT64 fr_lock_counter = 0;
	INT64 ph_lock_counter = 0;
	INT64 fr_lock_counter_norm = 0;
	INT64 ph_lock_counter_norm = 0;
	INT64 temp = 0;

	if ((pContext->PF1_tinc == 0) || (pContext->Fs_div == 0)) {
		return retErrZeroDiv;
	}
	temp = (INT64)LX_ADEMOD_CR_FR_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18);
	do_div(temp,pContext->PF1_tinc );
	do_div(temp,LX_ADEMOD_CR_HS_RATE_FCT );
	do_div(temp,LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp, pContext->Fs_div);

//	temp = (INT64)LX_ADEMOD_CR_FR_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div ;

	fr_lock_counter_norm = temp ;

	if (fr_lock_counter_norm < 1) {
		fr_lock_counter_norm = 1;
	}

	temp = (INT64)LX_ADEMOD_CR_PH_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18);
	do_div(temp, pContext->PF1_tinc );
	do_div(temp, LX_ADEMOD_CR_HS_RATE_FCT );
	do_div(temp, LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp,(pContext->Fs_div));

//	temp = (INT64)LX_ADEMOD_CR_PH_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div ;

	ph_lock_counter_norm = temp ;

	if (ph_lock_counter_norm < 1) {
		ph_lock_counter_norm = 1;
	}

	if( pContext->ph_lock_det_lines_scale )  // lock detection error is provided
	{
		fr_lock_counter = fr_lock_counter_norm * pContext->ph_lock_det_lines_scale;
		do_div(fr_lock_counter, 100);

		ph_lock_counter = ph_lock_counter_norm * pContext->ph_lock_det_lines_scale;
		do_div(ph_lock_counter, 100);
		
		pContext->fr_lock_counter  = (UINT32)fr_lock_counter;
		pContext->ph_lock_counter  = (UINT32)ph_lock_counter;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_UnlockThresh_Set
**
**  Description:    Sets Unlock detection threshould according to the unlock detection error.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_UnlockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64 fr_unlock_threshold = 0;
	UINT64 ph_unlock_threshold = 0;
	UINT64 fr_unlock_threshold_norm = 0;
	UINT64 ph_unlock_threshold_norm = 0;
	UINT64 temp = 0;

	if ((pContext->Fs_div == 0) || (pContext->sampling_rate == 0)) {
		return retErrZeroDiv;
	}
	temp = (UINT64)LX_ADEMOD_CR_FR_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc;
	do_div(temp, 1000);
	temp = temp * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_UNLOCK_THR_PREC_FCT) );
	do_div(temp, pContext->sampling_rate);
	do_div(temp, (1<<18));

//	temp = (UINT64)LX_ADEMOD_CR_FR_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc / 1000 * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_UNLOCK_THR_PREC_FCT) ) / (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;

	fr_unlock_threshold_norm = temp ;

	temp = (UINT64)LX_ADEMOD_CR_PH_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_UNLOCK_THR_PREC_FCT) );
	do_div(temp, (UINT64)pContext->sampling_rate);
	do_div(temp, (UINT64) (1<<18));

//	temp = (UINT64)LX_ADEMOD_CR_PH_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_UNLOCK_THR_PREC_FCT) )/ (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;

	ph_unlock_threshold_norm = temp ;

	if( pContext->ph_unlock_det_err_scale )  // unlock detection error is provided
	{
		fr_unlock_threshold = fr_unlock_threshold_norm * pContext->ph_unlock_det_err_scale;
		do_div(fr_unlock_threshold, 100 );
		
		ph_unlock_threshold = ph_unlock_threshold_norm * pContext->ph_unlock_det_err_scale;
		do_div(ph_unlock_threshold, 100);

		//	pContext->fr_unlock_thresh  = (UINT32)fr_unlock_threshold; // will be hard-coded on per-standard basis
		// according to the measurements for weak signal
		pContext->ph_unlock_thresh  = (UINT32)ph_unlock_threshold; // will be truncated TBD
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_UnlockCounter_Set
**
**  Description:    Sets Unlock control window according to the unlock detection lines.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_UnlockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	INT64 fr_unlock_counter = 0;
	INT64 ph_unlock_counter = 0;
	INT64 fr_unlock_counter_norm = 0;
	INT64 ph_unlock_counter_norm = 0;
	INT64 temp = 0;

	if ((pContext->Fs_div == 0) || (pContext->PF1_tinc == 0)) {
		return retErrZeroDiv;
	}
	temp = (INT64)LX_ADEMOD_CR_FR_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18);
	do_div(temp, pContext->PF1_tinc);
	do_div(temp, LX_ADEMOD_CR_HS_RATE_FCT);
	do_div(temp,LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp, 1000);

//	temp = (INT64)LX_ADEMOD_CR_FR_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div / 1000 ;

	fr_unlock_counter_norm = temp ;

	if (fr_unlock_counter_norm < 1) {
		fr_unlock_counter_norm = 1;
	}

	temp = (INT64)LX_ADEMOD_CR_PH_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18);
	do_div(temp, pContext->PF1_tinc);
	do_div(temp, LX_ADEMOD_CR_HS_RATE_FCT);
	do_div(temp, LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp, ( pContext->Fs_div));


//	temp = (INT64)LX_ADEMOD_CR_PH_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div ;

	ph_unlock_counter_norm = temp ;

	if (ph_unlock_counter_norm < 1) {
		ph_unlock_counter_norm = 1;
	}

	if( pContext->ph_unlock_det_lines_scale )  // unlock detection lines scale is provided
	{
		fr_unlock_counter = fr_unlock_counter_norm * pContext->ph_unlock_det_lines_scale;
		do_div(fr_unlock_counter, 100);
		
		ph_unlock_counter = ph_unlock_counter_norm * pContext->ph_unlock_det_lines_scale;
		do_div(ph_unlock_counter,100 );

//		fr_unlock_counter = fr_unlock_counter_norm * pContext->ph_unlock_det_lines_scale / 100;
//		ph_unlock_counter = ph_unlock_counter_norm * pContext->ph_unlock_det_lines_scale / 100;
		
		pContext->fr_unlock_counter  = (UINT32)fr_unlock_counter;
		pContext->ph_unlock_counter  = (UINT32)ph_unlock_counter;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_MutelockThresh_Set
**
**  Description:    Sets Mutelock detection threshould according to the lock detection error.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_MutelockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64 mute_fr_lock_threshold = 0;
	UINT64 mute_ph_lock_threshold = 0;
	UINT64 mute_fr_lock_threshold_norm = 0;
	UINT64 mute_ph_lock_threshold_norm = 0;
	UINT64 temp = 0;

	if (pContext->sampling_rate == 0) {
		return retErrZeroDiv;
	}
	temp = (UINT64)LX_ADEMOD_CR_FR_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_LOCK_THR_PREC_FCT) );
	do_div(temp, pContext->sampling_rate );
	do_div(temp, (1<<18));

//	temp = (UINT64)LX_ADEMOD_CR_FR_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_LOCK_THR_PREC_FCT) )/ (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;

	mute_fr_lock_threshold_norm = temp ;

	temp = (UINT64)LX_ADEMOD_CR_PH_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_LOCK_THR_PREC_FCT) );
	do_div(temp, pContext->sampling_rate );
	do_div(temp, (1<<18));

//	temp = (UINT64)LX_ADEMOD_CR_PH_LOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_LOCK_THR_PREC_FCT) )/ (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;
	mute_ph_lock_threshold_norm = temp ;

	if( pContext->mute_ph_lock_det_err_scale )  // lock detection error scale is provided
	{
		mute_fr_lock_threshold = mute_fr_lock_threshold_norm * pContext->mute_ph_lock_det_err_scale;
		do_div(mute_fr_lock_threshold, 100);
		
		mute_ph_lock_threshold = mute_ph_lock_threshold_norm * pContext->mute_ph_lock_det_err_scale;
		do_div(mute_ph_lock_threshold, 100);
		
		pContext->mute_fr_lock_thresh  = (UINT32)mute_fr_lock_threshold;
		pContext->mute_ph_lock_thresh  = (UINT32)mute_ph_lock_threshold;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_MutelockCounter_Set
**
**  Description:    Sets Mutelock control window according to the lock detection lines.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_MutelockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	INT64 mute_fr_lock_counter = 0;
	INT64 mute_ph_lock_counter = 0;
	INT64 mute_fr_lock_counter_norm = 0;
	INT64 mute_ph_lock_counter_norm = 0;
	INT64 temp = 0;

	if ((pContext->Fs_div == 0) || (pContext->PF1_tinc == 0) || (pContext->Fs_div == 0)) {
		return retErrZeroDiv;
	}
	temp = (INT64)LX_ADEMOD_CR_FR_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18);
	do_div(temp, pContext->PF1_tinc);
	do_div(temp,LX_ADEMOD_CR_HS_RATE_FCT);
	do_div(temp,LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp, pContext->Fs_div);

//	temp = (INT64)LX_ADEMOD_CR_FR_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div ;

	mute_fr_lock_counter_norm = temp ;

	if (mute_fr_lock_counter_norm < 1) {
		mute_fr_lock_counter_norm = 1;
	}

	temp = (INT64)LX_ADEMOD_CR_PH_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18); 
	do_div(temp, pContext->PF1_tinc);
	do_div(temp, LX_ADEMOD_CR_HS_RATE_FCT);
	do_div(temp, LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp,  pContext->Fs_div);

	
//	temp = (INT64)LX_ADEMOD_CR_PH_LOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div ;

	mute_ph_lock_counter_norm = temp ;

	if (mute_ph_lock_counter_norm < 1) {
		mute_ph_lock_counter_norm = 1;
	}

	if( pContext->mute_ph_lock_det_lines_scale )  // lock detection lines scale provided
	{
		mute_fr_lock_counter = mute_fr_lock_counter_norm * pContext->mute_ph_lock_det_lines_scale;
		do_div(mute_fr_lock_counter, 100);
		
		mute_ph_lock_counter = mute_ph_lock_counter_norm * pContext->mute_ph_lock_det_lines_scale;
		do_div(mute_ph_lock_counter, 100);
		
		pContext->mute_fr_lock_counter  = (UINT32)mute_fr_lock_counter;
		pContext->mute_ph_lock_counter  = (UINT32)mute_ph_lock_counter;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_MuteunlockThresh_Set
**
**  Description:    Sets Muteunlock detection threshould according to the unlock detection error.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_MuteunlockThresh_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64 mute_fr_unlock_threshold = 0;
	UINT64 mute_ph_unlock_threshold = 0;
	UINT64 mute_fr_unlock_threshold_norm = 0;
	UINT64 mute_ph_unlock_threshold_norm = 0;
	UINT64 temp = 0;

	if ((pContext->Fs_div == 0) || (pContext->sampling_rate == 0)) {
		return retErrZeroDiv;
	}
	temp = (UINT64)LX_ADEMOD_CR_FR_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc;
	do_div(temp, 1000);
	temp = temp * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_UNLOCK_THR_PREC_FCT) );
	do_div(temp, pContext->sampling_rate);
	do_div(temp, (1<<18));

//	temp = (UINT64)LX_ADEMOD_CR_FR_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc / 1000 * pContext->Fs_div * 2 * (UINT64)(1 << (LX_ADEMOD_CR_FR_UNLOCK_THR_PREC_FCT) ) / (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;

	mute_fr_unlock_threshold_norm = temp ;

	temp = (UINT64)LX_ADEMOD_CR_PH_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_UNLOCK_THR_PREC_FCT) );
	do_div(temp, pContext->sampling_rate);
	do_div(temp, (1<<18));
	
//	temp = (UINT64)LX_ADEMOD_CR_PH_UNLOCK_ERR_NORM_FCT * (UINT64)pContext->PF1_tinc * pContext->Fs_div * 1 * (UINT64)(1 << (LX_ADEMOD_CR_PH_UNLOCK_THR_PREC_FCT) ) / (UINT64)pContext->sampling_rate / (UINT64) (1<<18) ;

	mute_ph_unlock_threshold_norm = temp ;

	if( pContext->mute_ph_unlock_det_err_scale )  // unlock detection error scale is provided
	{
		mute_fr_unlock_threshold = mute_fr_unlock_threshold_norm * pContext->mute_ph_unlock_det_err_scale;
		do_div(mute_fr_unlock_threshold, 100);
		
		mute_ph_unlock_threshold = mute_ph_unlock_threshold_norm * pContext->mute_ph_unlock_det_err_scale;
		do_div(mute_ph_unlock_threshold, 100);
		
		pContext->mute_fr_unlock_thresh  = (UINT32)mute_fr_unlock_threshold;
		pContext->mute_ph_unlock_thresh  = (UINT32)mute_ph_unlock_threshold;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_MuteunlockCounter_Set
**
**  Description:    Sets Muteunlock control window according to the unlock detection lines.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_MuteunlockCounter_Set(pLX_ADEMOD_VideoProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	INT64 mute_fr_unlock_counter = 0;
	INT64 mute_ph_unlock_counter = 0;
	INT64 mute_fr_unlock_counter_norm = 0;
	INT64 mute_ph_unlock_counter_norm = 0;
	INT64 temp = 0;

	if ((pContext->Fs_div == 0) || (pContext->PF1_tinc == 0)) {
		return retErrZeroDiv;
	}
	temp = (INT64)LX_ADEMOD_CR_FR_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18);
	do_div(temp, pContext->PF1_tinc );
	do_div(temp, LX_ADEMOD_CR_HS_RATE_FCT);
	do_div(temp, LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp,  pContext->Fs_div);
	do_div(temp, 1000);

//	temp = (INT64)LX_ADEMOD_CR_FR_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div /1000 ;

	mute_fr_unlock_counter_norm = temp ;

	if (mute_fr_unlock_counter_norm < 1) {
		mute_fr_unlock_counter_norm = 1;
	}

	temp = (INT64)LX_ADEMOD_CR_PH_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18);
	do_div(temp, pContext->PF1_tinc );
	do_div(temp, LX_ADEMOD_CR_HS_RATE_FCT);
	do_div(temp, LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT);
	do_div(temp,  pContext->Fs_div);

//	temp = (INT64)LX_ADEMOD_CR_PH_UNLOCK_DET_LINE_FCT * (INT64)pContext->sampling_rate  * (INT64)(1 << 18) / (INT64)pContext->PF1_tinc / (INT64)LX_ADEMOD_CR_HS_RATE_FCT / (INT64)LX_ADEMOD_CR_DOWNSAMLE_RATIO_FCT / pContext->Fs_div ;

	mute_ph_unlock_counter_norm = temp ;

	if (mute_ph_unlock_counter_norm < 1) {
		mute_ph_unlock_counter_norm = 1;
	}

	if( pContext->mute_ph_unlock_det_lines_scale )  // unlock detection error scale is provided
	{
		mute_fr_unlock_counter = mute_fr_unlock_counter_norm * pContext->mute_ph_unlock_det_lines_scale;
		do_div(mute_fr_unlock_counter, 100);
		
		mute_ph_unlock_counter = mute_ph_unlock_counter_norm * pContext->mute_ph_unlock_det_lines_scale;
		do_div(mute_ph_unlock_counter, 100);
		
		pContext->mute_fr_unlock_counter  = (UINT32)mute_fr_unlock_counter;
		pContext->mute_ph_unlock_counter  = (UINT32)mute_ph_unlock_counter;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_Reset
**
**  Description:    Executes soft reset on video processing block.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**					on_off       - When TRUE the reset state is entered,
**                                 when FALSE the reset state is terminated.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_Reset(pLX_ADEMOD_VideoProc pContext, BOOL on_off, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// soft reset video processing block,
	// register SOFTRESETN0:
	// bit 0 -adc_if_resetn, inverted
	// bit 1 -dac_if_resetn, inverted
	// bit 2 -preproc_resetn, inverted
	// bit 3- video_resetn, inverted

	// FIFO reset legacy demodulator and pre-processor,
	// register FIFO_CLR0:
	// bit 0 - adc_if_fifo_clr
	// bit 1 - dac_if_video_fifo_clr
	// bit 6 - video_fifo_clr
	// register FIFO_CLR1:
	// bit 0 - testmux_video_fifo_clr
	// bit 5 - testmux_adc_fifo_clr
	// bit 7 - testmux_preproc_fifo_clr

	if (on_off) {
		// set soft reset mask:   01110000b = 0x70
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xc0);
	}
	else {
		// clear soft reset mask: 01111111b = 0x7f
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xdf);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_VideoProc_PLL_Reset
**
**  Description:    Executes soft reset on PLL.
**
**  Parameters:     pContext     - Pointer to the Video Processor context.
**					on_off       - When TRUE the reset state is entered,
**                                 when FALSE the reset state is terminated.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_VideoProc_PLL_Reset(pLX_ADEMOD_VideoProc pContext, BOOL on_off, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// soft reset video processing block,
	// register SOFTRESETN0:
	// bit 0 -adc_if_resetn, inverted
	// bit 1 -dac_if_resetn, inverted
	// bit 2 -preproc_resetn, inverted
	// bit 3- video_resetn, inverted

	// FIFO reset legacy demodulator and pre-processor,
	// register FIFO_CLR0:
	// bit 0 - adc_if_fifo_clr
	// bit 1 - dac_if_video_fifo_clr
	// bit 6 - video_fifo_clr
	// register FIFO_CLR1:
	// bit 0 - testmux_video_fifo_clr
	// bit 5 - testmux_adc_fifo_clr
	// bit 7 - testmux_preproc_fifo_clr

	if (on_off) {
		// set soft reset mask:   01110000b = 0x70
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0x9f);
	}
	else {
		// clear soft reset mask: 01111111b = 0x7f
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xdf);
	}

	return status;
}

