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
**  Name: ademod_m14_audio.c
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
**
**  Dependencies:   ademod_common.h   - for basic LX types,
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
#include "ademod_m14_audio.h"


/*
**  Local Function Prototypes - not available for external access.
*/

static LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Reset(pLX_ADEMOD_AudioProc pContext, BOOL on_off, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_ToneGen_Set(pLX_ADEMOD_AudioProc pContext, BOOL tone1_en, BOOL tone2_en, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_DAC_Start(pLX_ADEMOD_AudioProc pContext, Handle_t hUser);

/*
**  API implementation.
*/

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Factory_Set(pLX_ADEMOD_AudioProc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (retOK == status) {
		// set UPDATE flag and clear all user flags
		pContext->PF1_tinc_prec = LX_ADEMOD_PF1_TINC_PREC_FCT;
		pContext->PF1_delivery_rate_prec = LX_ADEMOD_PF1_DELRATE_PREC_FCT; // PF1 delivery rate precision
		pContext->SIF1_prec = LX_ADEMOD_SIF_OUT_PREC_FCT;
		pContext->Nom_freq_prec = LX_ADEMOD_NOM_FRQ_PREC_FCT;
		pContext->SrcDR = LX_ADEMOD_SRC_DEL_RATE_FCT;
		pContext->offset_tweak = LX_ADEMOD_AUD_OFFS_TWEAK_FCT;
//		pContext->PhaseAdj_Delay = LX_ADEMOD_AUD_PHASEADJ_DELAY_FCT;
		pContext->PwrLoThres = LX_ADEMOD_AUD_PWRTHR_LOW_FCT;
		pContext->PwrHiThres = LX_ADEMOD_AUD_PWRTHR_HI_FCT;
		pContext->DCRemovalShift = LX_ADEMOD_AUD_DCREMOV_SHFT_FCT;
		pContext->MuteOnSpeed =  LX_ADEMOD_AUD_MUTEON_SPEED_FCT;
		pContext->MuteOffSpeed = LX_ADEMOD_AUD_MUTEOFF_SPEED_FCT;
		pContext->BasebandExtraGain = LX_ADEMOD_AUD_BSEB_EXTRA_GAIN_FCT;
		pContext->Deemphasis = LX_ADEMOD_AUD_DEEMPH_FCT;
		pContext->SifBW = LX_ADEMOD_AUD_SIF_BNDW_FCT;
		pContext->AGCBWTarget = LX_ADEMOD_AUD_AGC_BWTARG_FCT; 
		pContext->AGCManualGain = LX_ADEMOD_AUD_AGC_MANUALGAIN_FCT;
		pContext->AGCTarget = LX_ADEMOD_AUD_AGC_TARGET_FCT;
		pContext->AGCMaxGain = LX_ADEMOD_AUD_AGC_MAXGAIN_FCT;
		pContext->AGCMinGain = LX_ADEMOD_AUD_AGC_MINGAIN_FCT;
		pContext->BuzzDelayOffs = LX_ADEMOD_AUD_BUZZ_DELAY_FCT;
		pContext->BuzzDelayFudge = LX_ADEMOD_AUD_BUZZ_DELAY_FUDGE_FCT;
		pContext->Fs_div = LX_ADEMOD_SAMPLING_RATE_DIV_FCT;

		// Audio DAC
		pContext->audio_dac.MixDn = LX_ADEMOD_AUD_DNMIX_FCT;
		pContext->audio_dac.MixUp = LX_ADEMOD_AUD_UPMIX_FCT;
		pContext->audio_dac.ExtraGain = LX_ADEMOD_AUD_DAC_EXTRA_GAIN_FCT;

		pContext->audio_dac.CH1_cliper = LX_ADEMOD_AUD_DAC_CLIPPER_FCT;
		pContext->audio_dac.CH1_gain_SIF = LX_ADEMOD_AUD_DAC_SIFGAIN_FCT;
		pContext->audio_dac.CH1_gain_MonoFM = LX_ADEMOD_AUD_DAC_MONOFMGAIN_FCT;
		pContext->audio_dac.CH1_gain_MonoAM = LX_ADEMOD_AUD_DAC_MONOAMGAIN_FCT;
		pContext->audio_dac.CH1_offs = LX_ADEMOD_AUD_DAC_OFFS_FCT;

	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_FREQ_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_FREQ_USR;
	}

	pContext->SC1_freq = pValue[0];
	pContext->SC1_min =  pValue[1];
	pContext->SC1_max =  pValue[2];

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_InputFreq_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->SC1_freq;
	pValue[1] = pContext->SC1_min;
	pValue[2] = pContext->SC1_max;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mode_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_MODE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_MODE_USR;
	}

	pContext->Mode = pValue[0];

	if (pContext->Mode == LX_ADEMOD_AUDIO_MODE_SIF) {
		pContext->flags &= ~LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN;
	}
	else {
		pContext->flags |= LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN;
	}


	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mode_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->Mode;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Standard_Set
**
**  Description:    Sets video and audio standards.
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Set(pLX_ADEMOD_AudioProc pContext, UINT32 VideoStandard, UINT32 AudioStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->audio_std = AudioStandard;

	if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_FREQ_USR)) {

		if (!(pContext->flags & LX_ADEMOD_FFLAG_NOT_VIDSLAVE_STICKY)) {
			// Clear Video coupling disable bit, etc audio by default is the slave of the video
			pContext->flags &= ~LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN;
		}

		// SIF for now 
//		pContext->flags &= ~LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN;
		// Mono for now    
//		pContext->flags |= LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN;
		// By default Mono AM, not FM
		pContext->flags &= ~LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN;

		switch (VideoStandard )
		{
			case LX_ADEMOD_VIDEO_STANDARD_NONE:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_NONE;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_NONE;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_NONE;
				pContext->offset_tweak = 20L;
				// A2-1, A2-2
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 0;
					pContext->Sif_LowerFreq     = 0;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 0;
					pContext->MonoSC_LowerFreq  = 0;
				}				
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 0;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 0;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 0;
				}
				if (!(pContext->flags & LX_ADEMOD_FFLAG_NOT_VIDSLAVE_STICKY)) {
					// Video standard is not defined, so de-couple audio from video
					pContext->flags |= LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz;
				pContext->offset_tweak = 0;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 96000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 90000;
					pContext->MonoSC_LowerFreq = 4500000 - 90000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_NTSC_M_J:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz;
				pContext->offset_tweak = 0;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 96000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 90000;
					pContext->MonoSC_LowerFreq = 4500000 - 90000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_NTSC_433:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz;
				pContext->offset_tweak = 0;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 96000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 90000;
					pContext->MonoSC_LowerFreq = 4500000 - 90000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_NTSC_50:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz;
				pContext->offset_tweak = 0;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 96000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 90000;
					pContext->MonoSC_LowerFreq = 4500000 - 90000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_B:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz;
				pContext->offset_tweak = 0;
				// A2, NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 5850000 + 300000;
					pContext->Sif_LowerFreq     = 5500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 5500000 + 95000;
					pContext->MonoSC_LowerFreq  = 5500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_G:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz;
				pContext->offset_tweak = 0;
				// A2, NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 5850000 + 300000;
					pContext->Sif_LowerFreq     = 5500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 5500000 + 95000;
					pContext->MonoSC_LowerFreq  = 5500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_H:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz;
				pContext->offset_tweak = 0;
				// A2, NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 5850000 + 300000;
					pContext->Sif_LowerFreq     = 5500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 5500000 + 95000;
					pContext->MonoSC_LowerFreq  = 5500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_I:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p6MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_7p1MHz;
				pContext->offset_tweak = 0;
				// NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6552000 + 420000;
					pContext->Sif_LowerFreq     = 6000000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6000000 + 95000;
					pContext->MonoSC_LowerFreq  = 6000000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_D:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p594MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_7p22MHz;
				pContext->offset_tweak = 0;
				// A2-1, A2-2,NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6742000 + 133000;
					pContext->Sif_LowerFreq     = 5742000 - 192000;
//					pContext->Sif_LowerFreq     = 6258000 - 133000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 95000;
					pContext->MonoSC_LowerFreq  = 6500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_K:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p594MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_7p22MHz;
				pContext->offset_tweak = 0;
				// A2-1, A2-2
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6742000 + 133000;
					pContext->Sif_LowerFreq     = 6258000 - 133000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 95000;
					pContext->MonoSC_LowerFreq  = 6500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p594MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_7p22MHz;
				pContext->offset_tweak = 0;
				// NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6500000 + 170000;
					pContext->Sif_LowerFreq     = 5850000 - 300000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 95000;
					pContext->MonoSC_LowerFreq  = 6500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_M:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz;
				pContext->offset_tweak = 0;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 75000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 75000;
					pContext->MonoSC_LowerFreq = 4500000 - 75000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_N:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz;
				pContext->offset_tweak = 0;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 96000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 90000;
					pContext->MonoSC_LowerFreq = 4500000 - 90000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_60:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p672MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p812MHz;
				pContext->offset_tweak = 25;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 96000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 90000;
					pContext->MonoSC_LowerFreq = 4500000 - 90000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_PAL_NC:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_4p4MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_4p3MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_4p9MHz;
				pContext->offset_tweak = 0;
				// BTSC, EIAJ
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq    = 4724000 + 96000;
					pContext->Sif_LowerFreq	   = 4500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq = 4500000 + 90000;
					pContext->MonoSC_LowerFreq = 4500000 - 90000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis       = 75;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation        = 25000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate       = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_B:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz;
				pContext->offset_tweak = 0;
				// A2, NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 5850000 + 300000;
					pContext->Sif_LowerFreq     = 5500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 5500000 + 95000;
					pContext->MonoSC_LowerFreq  = 5500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_G:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz;
				pContext->offset_tweak = 0;
				// A2, NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 5850000 + 300000;
					pContext->Sif_LowerFreq     = 5500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 5500000 + 95000;
					pContext->MonoSC_LowerFreq  = 5500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_H:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz;
				pContext->offset_tweak = 0;
				// A2, NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 5850000 + 300000;
					pContext->Sif_LowerFreq     = 5500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 5500000 + 95000;
					pContext->MonoSC_LowerFreq  = 5500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_D:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p594MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_7p22MHz;
				pContext->offset_tweak = 0;
				// A2-1, A2-2
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6742000 + 133000;
					pContext->Sif_LowerFreq     = 6258000 - 133000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 95000;
					pContext->MonoSC_LowerFreq  = 6500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_K:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p594MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_7p22MHz;
				pContext->offset_tweak = 0;
				// A2-1, A2-2
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6742000 + 133000;
					pContext->Sif_LowerFreq     = 6258000 - 133000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 95000;
					pContext->MonoSC_LowerFreq  = 6500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_K1:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p594MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_7p22MHz;
				pContext->offset_tweak = 0;
				// A2-1, A2-2
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6742000 + 133000;
					pContext->Sif_LowerFreq     = 6258000 - 133000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 95000;
					pContext->MonoSC_LowerFreq  = 6500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p5MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p812MHz;
				pContext->offset_tweak = 0;
				// NICAM
				pContext->flags |= LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN;
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6500000 + 80000;
					pContext->Sif_LowerFreq     = 5850000 - 300000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 75000;
					pContext->MonoSC_LowerFreq  = 6500000 - 75000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;

			case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_6p5MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p5MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p812MHz;
				pContext->offset_tweak = 0;
				// NICAM
				pContext->flags |= LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN;
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 6500000 + 80000;
					pContext->Sif_LowerFreq     = 5850000 - 300000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 6500000 + 75000;
					pContext->MonoSC_LowerFreq  = 6500000 - 75000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;
		    
			default: // PAL-B
				pContext->SC1_freq = LX_ADEMOD_AUDIO_FREQUENCY_5p25MHz;
				pContext->SC1_min =  LX_ADEMOD_AUDIO_FREQUENCY_5p266MHz;
				pContext->SC1_max =  LX_ADEMOD_AUDIO_FREQUENCY_6p167MHz;
				pContext->offset_tweak = 0;
				// A2, NICAM
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR)) {
					pContext->Sif_UpperFreq     = 5850000 + 300000;
					pContext->Sif_LowerFreq     = 5500000 - 170000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR)) {
					pContext->MonoSC_UpperFreq  = 5500000 + 95000;
					pContext->MonoSC_LowerFreq  = 5500000 - 95000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR)) {
					pContext->Deemphasis        = 50;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR)) {
					pContext->Deviation         = 50000;
				}
				if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR)) {
					pContext->BBSampRate        = 168;
				}
			break;
		    
		}
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Standard_Get(pLX_ADEMOD_AudioProc pContext, UINT32* pAudioStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pAudioStandard = pContext->audio_std;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Set(pLX_ADEMOD_AudioProc pContext, UINT32 Flags)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 mask = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_OPTIONS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_OPTIONS_USR;
	}

	mask = pContext->flags ^ Flags;
	pContext->flags = Flags;
	if (mask) {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Flags_Get(pLX_ADEMOD_AudioProc pContext, UINT32* pFlags)
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SampleRateDivider_Set(pLX_ADEMOD_AudioProc pContext, UINT32 Value)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_SMP_RATE_DIV_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_SMP_RATE_DIV_USR;
	}

	pContext->Fs_div = (Value) ? LX_ADEMOD_SAMPLING_RATE_DIV_24MHZ_FCT : LX_ADEMOD_SAMPLING_RATE_DIV_FCT;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SampleRateDivider_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Options_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->options = pValue[0];
	
	// NOTE: the LX_ADEMOD_AUD_OPTION_BSEBAND_NSIFF is made into parameter
	//if (options & LX_ADEMOD_AUD_OPTION_BSEBAND_NSIFF) {
	//	// baseband audio
	//	pContext->flags |= LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN;
	//}
	//else {
	//	// SIF audio
	//	pContext->flags &= ~LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN;
	//}

	if (pContext->options & LX_ADEMOD_AUD_OPTION_AGC_EN) {
		// buzz fix enabled
		pContext->flags |= LX_ADEMOD_FFLAG_AUD_AGC_EN;
	}
	else {
		// buzz fix disabled
		pContext->flags &= ~LX_ADEMOD_FFLAG_AUD_AGC_EN;
	}

	if (pContext->options & LX_ADEMOD_AUD_OPTION_AGC_CLSLOOP_EN) {
		// AGC closed loop enabled
		pContext->flags |= LX_ADEMOD_FFLAG_AGC_CLSLOOP_EN;
	}
	else {
		// AGC closed loop disabled
		pContext->flags &= ~LX_ADEMOD_FFLAG_AGC_CLSLOOP_EN;
	}

	if (pContext->options & LX_ADEMOD_AUD_OPTION_SLAVETOVIDEO_DIS) {
		// slave to video disabled
		pContext->flags |= LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN;
		pContext->flags |= LX_ADEMOD_FFLAG_NOT_VIDSLAVE_STICKY;
		// buzz fix disabled
		pContext->flags &= ~LX_ADEMOD_FFLAG_BUZZ_FIX_EN;
	}
	else {
		// slave to video enabled
		pContext->flags &= ~LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN;
		pContext->flags |= LX_ADEMOD_FFLAG_NOT_VIDSLAVE_STICKY;
		if (pContext->options & LX_ADEMOD_AUD_OPTION_BUZZ_FIX_EN) {
			// buzz fix enabled
			pContext->flags |= LX_ADEMOD_FFLAG_BUZZ_FIX_EN;
		}
		else {
			// buzz fix disabled
			pContext->flags &= ~LX_ADEMOD_FFLAG_BUZZ_FIX_EN;
		}
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Options_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->options;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mute_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_MUTE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_MUTE_USR;
	}

	pContext->mute = pValue[0];

	if (pValue[0] & 0x1) {
		pContext->flags |= LX_ADEMOD_FFLAG_FORCEMUTE_EN;
	}
	if (pValue[0] & 0x2) {
		pContext->flags |= LX_ADEMOD_FFLAG_AUTOMUTE_FRLOOP_EN;
	}
	if (pValue[0] & 0x4) {
		pContext->flags |= LX_ADEMOD_FFLAG_AUTOMUTE_PHLOOP_EN;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, MUTECTRL, pValue[0], 1);
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mute_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	pValue[0] = pContext->mute;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MuteSpeed_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	INT64 MuteVolumeOnTime;
	INT64 MuteVolumeOffTime;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_MUTE_SPEED_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_MUTE_SPEED_USR;
	}

	pContext->mute_speed = pValue[0];

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->MuteOnSpeed  = pValue[1];
			pContext->MuteOffSpeed = pValue[2];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIDDLE:
			pContext->MuteOnSpeed  = 40;
			pContext->MuteOffSpeed = 40;
		break;
	    		    
		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->MuteOnSpeed  = 80;
			pContext->MuteOffSpeed = 80;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->MuteOnSpeed  = 20;
			pContext->MuteOffSpeed = 20;
		break;	    		    

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}


	if (Update) {

		MuteVolumeOnTime = ( pContext->MuteOnSpeed  * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10;
		do_div(MuteVolumeOnTime, 1000);
		do_div(MuteVolumeOnTime,(pContext->Fs_div) );
		do_div(MuteVolumeOnTime, 4);
		do_div(MuteVolumeOnTime,(pContext->FinalUp_TincInv ) );
		do_div(MuteVolumeOnTime, 64);
		MuteVolumeOnTime = MuteVolumeOnTime + 5;
		do_div(MuteVolumeOnTime, 10);
		
//		MuteVolumeOnTime  = ( ( pContext->MuteOnSpeed  * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10 / 1000 / pContext->Fs_div /4 / pContext->FinalUp_TincInv / 64 + 5 ) / 10 ;

		MuteVolumeOffTime = ( pContext->MuteOffSpeed * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10;
		do_div(MuteVolumeOffTime, 1000);
		do_div(MuteVolumeOffTime,(pContext->Fs_div) );
		do_div(MuteVolumeOffTime, 4);
		do_div(MuteVolumeOffTime,(pContext->FinalUp_TincInv ) );
		do_div(MuteVolumeOffTime, 64);
		MuteVolumeOffTime = MuteVolumeOffTime + 5;
		do_div(MuteVolumeOffTime, 10);

//		MuteVolumeOffTime = ( ( pContext->MuteOffSpeed * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10 / 1000 / pContext->Fs_div /4 / pContext->FinalUp_TincInv / 64 + 5 ) / 10 ;
		
		if ( MuteVolumeOnTime > 255 )
		{
			MuteVolumeOnTime = 255;
		}
		else if ( MuteVolumeOnTime < 1 )
		{
			MuteVolumeOnTime = 1;
		}

		if ( MuteVolumeOffTime > 255 )
		{
			MuteVolumeOffTime = 255;
		}
		else if ( MuteVolumeOffTime < 1 )
		{
			MuteVolumeOffTime = 1;
		}
		ADEMOD_WriteSub( hUser, MUTEVOLUMEONTIME, (UINT8)MuteVolumeOnTime, 1);
		ADEMOD_WriteSub( hUser, MUTEVOLUMEOFFTIME,(UINT8)MuteVolumeOffTime, 1);
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MuteSpeed_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->mute_speed;
	pValue[1] = pContext->MuteOnSpeed;
	pValue[2] = pContext->MuteOffSpeed;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Gain_Set
**
**  Description:    Sets audio gain thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 gain,
**						pValue[1]	0- SIF, 1- AM Mono, 2- FM Mono.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_GAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_GAIN_USR;
	}

	switch (pValue[1]) {
	case 0: pContext->audio_dac.CH1_gain_SIF	= pValue[0]; break;
	case 1: pContext->audio_dac.CH1_gain_MonoAM = pValue[0]; break;
	case 2: pContext->audio_dac.CH1_gain_MonoFM = pValue[0]; break;
	default: return retErrBadParam;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, AUDIOCH1GAIN, pValue[0], 2);
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Gain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	ADEMOD_ReadSub( hUser, AUDIOCH1GAIN, &pValue[0], 2);

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Gain_Set
**
**  Description:    Sets audio gain thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 gain,
**						pValue[1]	0- SIF, 1- AM Mono, 2- FM Mono.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_GAIN_RC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_GAIN_RC_USR;
	}

	pContext->audio_dac_rc.CH1_gain_SIF	= pValue[0];

	if (Update) {
		ADEMOD_WriteSub( hUser, AUDIOCH1GAIN_RC, pContext->audio_dac_rc.CH1_gain_SIF, 2);

	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Gain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	ADEMOD_ReadSub( hUser, AUDIOCH1GAIN_RC, &pValue[0], 2);


	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_OFFS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_OFFS_USR;
	}

	pContext->audio_dac.CH1_offs = pValue[0];

	if (Update) {
		ADEMOD_WriteSub( hUser, AUDIOCH1OFFSET, pContext->audio_dac.CH1_offs, 2);
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Offset_Get
**
**  Description:    Returns actual audio gain.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 offset.
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Offset_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	ADEMOD_ReadSub( hUser, AUDIOCH1OFFSET, &pValue[0], 2);

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Offset_Set
**
**  Description:    Sets rate converter audio offset.
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_OFFS_RC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_OFFS_RC_USR;
	}

	pContext->audio_dac_rc.CH1_offs = pValue[0];

	if (Update) {
		ADEMOD_WriteSub( hUser, AUDIOCH1OFFSET_RC, pContext->audio_dac_rc.CH1_offs, 2);
	}
	else {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Offset_Get
**
**  Description:    Returns actual rate conveter audio offset.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	CH1 offset.
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Offset_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	ADEMOD_ReadSub( hUser, AUDIOCH1OFFSET_RC, &pValue[0], 2);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Clip_Set
**
**  Description:    Sets audio clipping thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	audio clipper.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_CLIP_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_CLIP_USR;
	}

	pContext->audio_dac.CH1_cliper = pValue[0];

	if (Update) {
		ADEMOD_WriteSub( hUser, AUDIOCH1MAXCLIPPING, (INT16)pContext->audio_dac.CH1_cliper, 2);
		ADEMOD_WriteSub( hUser, AUDIOCH1MINCLIPPING, (INT16)-pContext->audio_dac.CH1_cliper, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Clip_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	ADEMOD_ReadSub( hUser, AUDIOCH1MINCLIPPING, &pValue[0], 2);
	ADEMOD_ReadSub( hUser, AUDIOCH1MAXCLIPPING, &pValue[1], 2);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Clip_Set
**
**  Description:    Sets audio clipping thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	audio clipper.
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_CLIP_RC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_CLIP_RC_USR;
	}

	pContext->audio_dac_rc.CH1_cliper = pValue[0];

	if (Update) {
		ADEMOD_WriteSub( hUser, AUDIOCH1MAXCLIPPING_RC, (INT16)pContext->audio_dac_rc.CH1_cliper, 2);
		ADEMOD_WriteSub( hUser, AUDIOCH1MINCLIPPING_RC, (INT16)-pContext->audio_dac_rc.CH1_cliper, 2);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_RC_Clip_Get
**
**  Description:    Returns audio clipping thresholds.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pValue       - Pointer to the value:
**						pValue[0]	RC clip min,
**						pValue[1]	RC clip max,
**						pValue[2]	RC clip min,
**						pValue[3]	RC clip max.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_RC_Clip_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	ADEMOD_ReadSub( hUser, AUDIOCH1MINCLIPPING_RC, &pValue[0], 2);
	ADEMOD_ReadSub( hUser, AUDIOCH1MAXCLIPPING_RC, &pValue[1], 2);

	return status;
}



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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifFreq_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_SIFFRQ_USR;
	}

	if (pValue[0] > pValue[1]) {
		return retErrBadParam;
	}

	pContext->Sif_LowerFreq	   = pValue[0];
	pContext->Sif_UpperFreq    = pValue[1];

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifFreq_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->Sif_LowerFreq;
	pValue[1] = pContext->Sif_UpperFreq;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoFreq_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_MONOFREQ_USR;
	}

	pContext->MonoSC_LowerFreq = pValue[0];
	pContext->MonoSC_UpperFreq = pValue[1];

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoFreq_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->MonoSC_LowerFreq;
	pValue[1] = pContext->MonoSC_UpperFreq;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoGain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_MONOGAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_MONOGAIN_USR;
	}

	pContext->Mono.Gain1			= pValue[0];
	pContext->Mono.Gain2			= pValue[1];
	pContext->Mono.Shift1			= pValue[2];
	pContext->Mono.Shift2			= pValue[3];
	pContext->Mono.Shift3			= pValue[4];

	if (Update) {
		{
			UINT8 Gain1 = (UINT8)(
				((pContext->Mono.Gain1)     << 0) +
				((pContext->Mono.Shift1)    << 5)
			); 
			ADEMOD_WriteSub( hUser, GAIN1, Gain1, 1);
		}
		{
			UINT8 ABBF_Ctrl = (UINT8)(
				(((pContext->flags & LX_ADEMOD_FFLAG_DCREMOVAL_EN)    ? 1 : 0) << 0) +
				(((pContext->flags & LX_ADEMOD_FFLAG_DEEMPHASIS_EN)   ? 1 : 0) << 1) +
				(((pContext->flags & LX_ADEMOD_FFLAG_BSEBND_FILT_EN)  ? 0 : 1) << 2) +
				((pContext->Mono.Shift3)                                << 3)
			); 
			ADEMOD_WriteSub( hUser, ABBF_CTRL, ABBF_Ctrl, 1);
		}
		ADEMOD_WriteSub( hUser, ABBF_GAINFRAC,    (UINT8)pContext->Mono.Gain2, 1);
		ADEMOD_WriteSub( hUser, ABBF_GAINSHIFT,   (UINT8)pContext->Mono.Shift2, 1);
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_MonoGain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->Mono.Gain1;
	pValue[1] = pContext->Mono.Gain2;
	pValue[2] = pContext->Mono.Shift1;
	pValue[3] = pContext->Mono.Shift2;
	pValue[4] = pContext->Mono.Shift3;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deviation_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_DEVIATION_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_DEVIATION_USR;
	}

	pContext->Deviation = pValue[0];

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deviation_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->Deviation;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BBSampleRate_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_BBSMPLERATE_USR;
	}

	pContext->BBSampRate = pValue[0];

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BBSampleRate_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->BBSampRate;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deemphasis_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR;
	}

	pContext->Deemphasis = pValue[0];

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Deemphasis_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->Deemphasis;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_DEEMPHASIS_USR;
	}

	pContext->BasebandExtraGain = pValue[0];

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BasebandExtraGain_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->BasebandExtraGain;

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCTarget_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_AGC_TARG_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_AGC_TARG_USR;
	}

	pContext->AGCTarget   = (UINT64)pValue[0] ;
	pContext->AGCBWTarget = (UINT64)pValue[1]; 

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCTarget_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	pValue[0] = (UINT32)pContext->AGCTarget;
	pValue[1] = (UINT32)pContext->AGCBWTarget; 

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_AGCGainThresh_Set
**
**  Description:    Sets AGC gain thresholds value.
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainThresh_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_AGC_GAINTHR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_AGC_GAINTHR_USR;
	}

	pContext->AGCMinGain = pValue[0];
	pContext->AGCMaxGain = pValue[1]; 

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainThresh_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	pValue[0] = (UINT32)pContext->AGCMinGain;
	pValue[1] = (UINT32)pContext->AGCMaxGain; 

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainPreset_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_AGC_GAINPRES_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_AGC_GAINPRES_USR;
	}

	pContext->AGCManualGain = pValue[0];

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_AGCGainPreset_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	pValue[0] = (UINT32)pContext->AGCManualGain;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SifBw_Set
**
**  Description:    Sets Up/Down mixer.
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifBw_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_SIFBW_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_SIFBW_USR;
	}

	pContext->SifBW = pValue[0];

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_SifBw_Get
**
**  Description:    Returns Up/Down mixer.
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_SifBw_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	pValue[0] = pContext->SifBW;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mix_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_MIX_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_MIX_USR;
	}
	
	pContext->audio_dac.MixUp = pValue[0] * 1000000;
	pContext->audio_dac.MixDn = pValue[1] * 1000000;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Mix_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	
	pValue[0] = pContext->audio_dac.MixUp;
	pValue[1] = pContext->audio_dac.MixDn;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_BuzzDelay_Set
**
**  Description:    Sets audio buzz delay parameters.
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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BuzzDelay_Set(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AUD_BUZZDELAY_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AUD_BUZZDELAY_USR;
	}

	pContext->BuzzDelayOffs  = pValue[0];
	pContext->BuzzDelayFudge = pValue[1];

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_BuzzDelay_Get(pLX_ADEMOD_AudioProc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->BuzzDelayOffs;
	pValue[1] = pContext->BuzzDelayFudge;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Init
**
**  Description:    Initializes Audio Processor's hardware.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Init(pLX_ADEMOD_AudioProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	status = DEMOD_M14_Analog_AudioProc_Reset(pContext, TRUE, hUser);

	// Update Audio Registers
	ADEMOD_WriteSub( hUser, NOMINALFREQ_SC1, 0, 3);
	ADEMOD_WriteSub( hUser, PF1_TINC_SC1, 0, 3);
	ADEMOD_WriteSub( hUser, SIF1_OPFREQ, 0, 3);
	ADEMOD_WriteSub( hUser, AUDIOCH1GAIN, 0, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1OFFSET, 0, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1MAXCLIPPING, 0, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1MINCLIPPING, 0, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1GAIN_RC, 0, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1OFFSET_RC, 0, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1MAXCLIPPING_RC, 0, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1MINCLIPPING_RC, 0, 2);
	ADEMOD_WriteSub( hUser, MISCAUDIOCTRL, 0, 1);
	ADEMOD_WriteSub( hUser, EXTRAAUDIOCTRL, 0, 1);

	status = DEMOD_M14_Analog_AudioProc_Reset(pContext, FALSE, hUser);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Start
**
**  Description:    Starts Audio processing block.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context,
**                  pVideo_freq  - Pointer to the Video carrier frequency,
**                  flags        - Video flags,
**                  flags        - Flags,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadParam  - Invalid parameter 
**
**  Note:           Updates the registers according to the lates state of the 
**                  variables.
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Start(pLX_ADEMOD_AudioProc pContext, UINT32 *pVideo_freq, UINT32 flags, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 PC_IFFreq =0;
	UINT64 ConditionalDownsampleRatio =0;
	UINT64 PF1_Tinc =0;
	UINT64 PF1_TincInv =0;
	UINT64 DeliveryRateSoFar =0;
	BOOL F3Bypass = FALSE;
	BOOL F4Bypass = FALSE;
	BOOL F3Delay = FALSE;
	BOOL F4Delay = FALSE;
	UINT64 VideoFPolyTinc =0;
	UINT64 FR2_delta_ph_init =0;
	INT64 VideoFR2Freq =0;
	INT64 SifOffset =0;
	INT64 InpMix_Freq =0;
	INT64 OutMix_Freq =0;
	INT64 temp =0;
	UINT64 HWBandwidth =0;
	UINT64 MF_Fsfull =0;
	UINT64 SampleRateSoFar =0;
	UINT64 BuzzDownsampTinv =0;
	INT64 BuzzDelay =0;
	UINT32 BuzzLoc =0;
	INT64 PFBuzz_Tinc =0;
	INT64 PFBuzz_TincInv =0;
	INT64 BuzzDelayOffs =0;
	INT64 BuzzDelayFudge =0;
	UINT32 RequiredDownSampling =0;
	UINT32 AGC_n50dBFreq =0;
	UINT32 AGC_n50dBFactor =0;   
	UINT32 AGC_n50dBLoc =0;
	UINT32 AGCFiltShift0 =0;
	UINT32 AGCFiltShift1 =0;
	UINT32 AGCLoopShift =0;
	UINT64 AGCManualGain =0;
	UINT64 AGCTarget =0;
	UINT64 AGCMaxGain =0; 
	UINT64 AGCMinGain =0;
	UINT64 PwrLoThres =0;
	UINT64 PwrHiThres =0; 
	INT64 FinalUp_Tinc =0; 
	INT64 FinalUp_DeliveryRate =0;
	INT64 PFSif_Tinc =0;
	INT64 PFSif_TincInv =0;
	INT64 PFSif_DeliveryRate =0;
	INT64 RequiredUpSampling =0;
	INT64 BBF_Fs =0;
	INT64 demodshift1 =0;
	INT64 demodshift2 =0;
	INT64 demodshift3 =0;
	INT64 demodgain1 =0;  
	INT64 demodgain2 =0;  
	INT64 MuteVolumeOnTime =0;
	INT64 MuteVolumeOffTime =0;
	UINT8 DeemphasisCoeff =0;
	UINT8 AgcCntr_SC1 =0;
	UINT8 Gain1 =0;
	UINT8 MuteCtrl =0;
	UINT8 ExtraAudioCtrl =0;
	UINT8 MiscAudioCtrl =0;
	UINT8 SifCtrl_SC1 =0;
	UINT8 ABBF_Ctrl =0;

	INT64 calcTemp;
	INT64 calcTemp1;

	UINT64 calcTemp2;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if (!pContext) {
		return retErrBadContext;
	}

	if (pVideo_freq[3] == 0 || pVideo_freq[7] == 0) {
		return retErrBadParam;
	}

	PC_IFFreq         = pVideo_freq[2]; //picture carrier location in IF
	VideoFPolyTinc    = pVideo_freq[3];
	FR2_delta_ph_init = pVideo_freq[6];
	pContext->AdcDacFreq = pVideo_freq[7]; // the route clock seen by digital circuitry, same as Samling Rate

	if (pContext->flags & LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN)
	{
		flags |= LX_ADEMOD_FFLAG_FR1_OPENED_LOOP;
		flags |= LX_ADEMOD_FFLAG_FR2_OPENED_LOOP;
	}

	VideoFR2Freq = (FR2_delta_ph_init * pContext->AdcDacFreq);  
	do_div(VideoFR2Freq, VideoFPolyTinc);  //jeongpil.yun- check !!
	do_div(VideoFR2Freq, (1 << 10));
	do_div(VideoFR2Freq, (pContext->Fs_div));

//	VideoFR2Freq = (FR2_delta_ph_init * pContext->AdcDacFreq) / ( VideoFPolyTinc * (UINT64)(1 << 10) * pContext->Fs_div);

	//  DeliveryRateSoFar is specified in 5 clock "beats", so the initial DeliveryRateSoFar
	//  will be (144 MHz / 28.8 MS/s) / 5 clock beat = 1. (These values can change
	//  of course, depending on the actual clock rate and the IF frequency).
	DeliveryRateSoFar = 1;
	SampleRateSoFar = pContext->AdcDacFreq;


	if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_SIFBW_USR)) { // not user defined
		if (pContext->flags & LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN) 
		{
			pContext->SifBW = (pContext->MonoSC_UpperFreq - pContext->MonoSC_LowerFreq) ;
		}
		else
		{
			pContext->SifBW = (pContext->Sif_UpperFreq - pContext->Sif_LowerFreq) ;
		}
	}

	if (pContext->flags & LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN) 
	{
		SifOffset = (pContext->MonoSC_UpperFreq + pContext->MonoSC_LowerFreq);
		do_div(SifOffset, 2);
	}
	else
	{
		SifOffset = (pContext->Sif_UpperFreq + pContext->Sif_LowerFreq);
		do_div(SifOffset, 2);
	}

	// mix frequency really are "mixed up" with the 44/36, step left/right, and slavetovideo settings...
	// need to take into account where the picture carrier is at this point
	if (pContext->flags & LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN) 
	{
		if (flags & LX_ADEMOD_FFLAG_44n36) 
		{
			temp = (INT64)(pContext->AdcDacFreq * 3);
			do_div(temp, 2);
			temp = SifOffset + temp - PC_IFFreq ;
		}
		else {
			temp = (INT64)(pContext->AdcDacFreq);
			do_div(temp, 8);

			temp = SifOffset + temp - PC_IFFreq ;
		}
	}
	else 
	{
		temp = (SifOffset - VideoFR2Freq) ;
	}

	if (temp >= 0) {

		calcTemp1 = pContext->AdcDacFreq;
		do_div(calcTemp1, (pContext->Fs_div));
		
		calcTemp = temp * (INT64)(1 << 22) * 10;
		do_div(calcTemp,calcTemp1 );  //jeongpil.yun- check !!
		calcTemp = calcTemp + 5;
		do_div(calcTemp,10 );
		
		
		InpMix_Freq = (-1) * ( calcTemp ) ;
//		InpMix_Freq = (-1) * ( (temp * (UINT64)(1 << 22) * 10 / (pContext->AdcDacFreq / pContext->Fs_div) + 5 ) / 10 ) ;
	}
	else {
		calcTemp1 = pContext->AdcDacFreq;
		do_div(calcTemp1, (pContext->Fs_div));
		
		calcTemp = temp * (INT64)(1 << 22) * 10;
		do_div(calcTemp,calcTemp1 );  //jeongpil.yun- check !!
		calcTemp = calcTemp - 5;
		do_div(calcTemp,10 );
		
		
		InpMix_Freq = (-1) * ( calcTemp ) ;

//		InpMix_Freq = (-1) * ( (temp * (UINT64)(1 << 22) * 10 / (pContext->AdcDacFreq / pContext->Fs_div) - 5 ) / 10 ) ;
	}


	if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MIX_USR)) { // not user defined
		pContext->audio_dac.MixDn = (UINT32)InpMix_Freq;
	}

	// the main filter bandwidth is equal to 0.1151 of its sampling frequency (-3dB bw) so we have to adjust
	// the sample rate using the polyphase and two stages of optional down-by-2 filters so that
	// our desired bandwidth "fits" through the main filter and slaving settings at the same time.
	// [The SifBW is divided by two here because we're working with a complex signal centered at 0 Hz].
	//
	// there is an unconditional /4 in the sample rate
	//
	HWBandwidth = 115100; // -3dB bandwidth
	MF_Fsfull = ( (UINT64)(pContext->SifBW) * 1000000 * 10 );
	do_div(MF_Fsfull, 2);
	do_div(MF_Fsfull,HWBandwidth);
	MF_Fsfull = MF_Fsfull + 5;
	do_div(MF_Fsfull, 10);
	
//	MF_Fsfull = ( ( (UINT64)(pContext->SifBW) * 1000000 * 10 ) / 2 / HWBandwidth + 5 ) / 10 ;

	calcTemp1 = pContext->AdcDacFreq;
	do_div(calcTemp1, (pContext->Fs_div));
	do_div(calcTemp1, 4);
	do_div(calcTemp1, MF_Fsfull);  //jeongpil.yun- check !!

	RequiredDownSampling = (UINT32)calcTemp1;
//	RequiredDownSampling = (UINT32)((pContext->AdcDacFreq / pContext->Fs_div / 4) / MF_Fsfull);

	DeliveryRateSoFar *= 4;
	do_div(SampleRateSoFar, 4);
//	SampleRateSoFar /= 4;

	do_div(SampleRateSoFar, 4);
	BuzzDownsampTinv = (VideoFPolyTinc >> 2) ;
//	BuzzDownsampTinv = VideoFPolyTinc / 4;

	// Now, determine the F3/F4 bypasses and use that with VideoFPoly so can derive buzz delay...
	// if we need more than 2x down-sampling, we'll use some combination of the two
	// fixed down-by-2 filters to get us "closer".
	BuzzDelayOffs = pContext->BuzzDelayOffs; 
	BuzzDelayFudge = pContext->BuzzDelayFudge;
	ConditionalDownsampleRatio = 1;
	if (RequiredDownSampling >= 4) { // 4.0
		/*
		** make sure both down-by-2 filters are on
		*/
		F3Bypass = FALSE;
		F4Bypass = FALSE;
		F3Delay  = 0;
		F4Delay  = 0;
		BuzzLoc  = (pContext->flags & LX_ADEMOD_FFLAG_BUZZ_FIX_EN) ? 1 : 0;
		DeliveryRateSoFar *= 4;
		ConditionalDownsampleRatio  *= 4; 
		do_div(BuzzDownsampTinv, 4);
		//BuzzDownsampTinv  /= 4;

		calcTemp =  (VideoFPolyTinc * ( 30 * 2 + 18));
		do_div(calcTemp, (1 << 18));  //jeongpil.yun- check !!
		BuzzDelay = ( (( 16 * 2 + 5 + calcTemp) - (5 + 6 * 2 + 6 * 4 + 6 * 8) + BuzzDelayOffs) * 10 + 5 );
		do_div(BuzzDelay, 10);
		do_div(BuzzDelay, 16);
		BuzzDelay = BuzzDelay + BuzzDelayFudge;
		
	}
	else if (RequiredDownSampling >= 2) { // 2.0
		/*
		** bypass F4 as delay (use F3 as /2)
		*/
		F3Bypass = FALSE;
		F4Bypass = FALSE;
		F3Delay  = 0;
		F4Delay  = 1;
		BuzzLoc  = (pContext->flags & LX_ADEMOD_FFLAG_BUZZ_FIX_EN) ? 1 : 0;
		DeliveryRateSoFar *= 2;
		ConditionalDownsampleRatio  *= 2; 
		do_div(BuzzDownsampTinv, 2);
//		BuzzDownsampTinv  /= 2;

		calcTemp =  (VideoFPolyTinc * ( 30 * 2 + 18));
		do_div(calcTemp, (1 << 18));

		BuzzDelay = ( (( 16 * 2 + 5 + calcTemp) - (5 + 6 * 2 + 6 * 4 + 10 * 8 * F4Delay) + BuzzDelayOffs) * 10 + 5 );
		do_div(BuzzDelay, 10);
		do_div(BuzzDelay, 8);
		BuzzDelay = BuzzDelay + BuzzDelayFudge;	
		
//		BuzzDelay = ( (( 16 * 2 + 5 + ((VideoFPolyTinc * ( 30 * 2 + 18)) / (UINT64)(1 << 18))) - (5 + 6 * 2 + 6 * 4 + 10 * 8 * F4Delay) + BuzzDelayOffs) * 10 + 5 ) / 10 / 8 + BuzzDelayFudge;	

	}
	else {
		/*
		** bypass both F3 and F4 as delay
		*/
		F3Bypass = FALSE;
		F4Bypass = FALSE;
		F3Delay  = 1;
		F4Delay  = 1;
		BuzzLoc  = (pContext->flags & LX_ADEMOD_FFLAG_BUZZ_FIX_EN) ? 2 : 0;

		calcTemp =  (VideoFPolyTinc * ( 30 * 2 + 18));
		do_div(calcTemp, (1 << 18));

		BuzzDelay = ( (( 16 * 2 + 5 + calcTemp) - (5 + 6 * 2 + 10 * 4 * F3Delay + 10 * 4 * F4Delay + 20 * 4) + BuzzDelayOffs) * 10 + 5 );
		do_div(BuzzDelay, 10);
		do_div(BuzzDelay, 4);
		BuzzDelay = BuzzDelay + BuzzDelayFudge;
//		BuzzDelay = ( (( 16 * 2 + 5 + ((VideoFPolyTinc * ( 30 * 2 + 18)) / (UINT64)(1 << 18))) - (5 + 6 * 2 + 10 * 4 * F3Delay + 10 * 4 * F4Delay + 20 * 4) + BuzzDelayOffs) * 10 + 5 ) / 10 / 4 + BuzzDelayFudge;
	}


	PFBuzz_TincInv = BuzzDownsampTinv;    
	PFBuzz_Tinc = (UINT64)(1 << 18) * (UINT64)(1 << 18);
	do_div(PFBuzz_Tinc, PFBuzz_TincInv );  //jeongpil.yun- check !!
	
	if (BuzzDelay > 31) {
		BuzzDelay = 31;
	}
	else if (BuzzDelay < 0) {
		BuzzDelay = 0;
	}
	PF1_Tinc = pContext->AdcDacFreq * (UINT64)(1 << 18);
	do_div(PF1_Tinc, pContext->Fs_div );
	do_div(PF1_Tinc,4 );
	do_div(PF1_Tinc, MF_Fsfull);  //jeongpil.yun- check !!
	do_div(PF1_Tinc, ConditionalDownsampleRatio);  //jeongpil.yun- check !!

//	PF1_Tinc =  ( (pContext->AdcDacFreq * (UINT64)(1 << 18) / pContext->Fs_div) / 4 ) / MF_Fsfull / ConditionalDownsampleRatio;   


	if (pContext->flags & LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN) {
		/*
		** PF1 Tinc will have a different effective precision depending on
		** whether we're processing Sif or baseband and whether F3/F4 are bypasses or not
		*/
		if (F3Bypass && F4Bypass) {
			
			do_div(PF1_Tinc, (1 << 8) );
			PF1_Tinc = PF1_Tinc * (UINT64)(1 << 8) ;  
		}
		else if (F4Bypass || F3Bypass) {
			do_div(PF1_Tinc, (1 << 9) );
			PF1_Tinc = PF1_Tinc * (UINT64)(1 << 9) ;  

//			PF1_Tinc = ( PF1_Tinc / (UINT64)(1 << 9) ) * (UINT64)(1 << 9) ;  
		}
		else {
			// not changed
			do_div(PF1_Tinc, (1 << 10) );
			PF1_Tinc = PF1_Tinc * (UINT64)(1 << 10) ;  

//			PF1_Tinc = ( PF1_Tinc / (UINT64)(1 << 10) ) * (UINT64)(1 << 10) ;  
		}
	}
	else {
		do_div(PF1_Tinc, 2);
		PF1_Tinc = PF1_Tinc * 2 ;     
	}
		
	PF1_TincInv = (UINT64)(1 << 18) * (UINT64)(1 << 18);
	do_div(PF1_TincInv, PF1_Tinc);  //jeongpil.yun- check !!

	calcTemp2 = (pContext->Fs_div * 4 * ConditionalDownsampleRatio * PF1_Tinc )	;
	SampleRateSoFar   =  ( pContext->AdcDacFreq * (UINT64)(1 << 18) ) ;
	do_div(SampleRateSoFar,calcTemp2 );  //jeongpil.yun- check !!
	
	// AGC settings that are depended on the "SampleRateSoFar" after AudioFPoly1. 
	// (an setting of 2^-12 @ 760kHz Samplingrate ~ -50dB @ 300Hz use it as reference for now)
	AGC_n50dBFreq   = 300; // 300Hz ~ -50dB target

	calcTemp2 = 760000 * 10 ;
	do_div(calcTemp2, AGC_n50dBFreq);
	calcTemp2 = calcTemp2 + 5;
	do_div(calcTemp2, 10);
	
	AGC_n50dBFactor = (UINT32)calcTemp2;   

	calcTemp2 = (  pContext->AGCBWTarget * AGC_n50dBFactor * (UINT64)(1 << 12) ) *  PF1_Tinc * ConditionalDownsampleRatio;

	do_div(calcTemp2, (1 << 18));

	calcTemp1 = pContext->AdcDacFreq;
	do_div(calcTemp1, pContext->Fs_div );
	do_div(calcTemp1, 4);

	do_div(calcTemp2, calcTemp1);
	AGC_n50dBLoc	= (UINT32)calcTemp2 ;
	
//	AGC_n50dBLoc	= (UINT32)((  pContext->AGCBWTarget * AGC_n50dBFactor * (UINT64)(1 << 12) )  * PF1_Tinc * ConditionalDownsampleRatio / (UINT64)(1 << 18)  /  ( pContext->AdcDacFreq / pContext->Fs_div / 4 )  ) ;
	if (pContext->AGCBWTarget)
	{
		for (AGCLoopShift = 0; AGC_n50dBLoc <  ((UINT64)(1 << 12) * (UINT64)(1 << 12)) ; AGCLoopShift++) {
			AGC_n50dBLoc *= 2;
			
		}
	}
	else	
	{
		AGCLoopShift = 4;
	}

	AGCFiltShift0 = AGCLoopShift - 1;
	AGCFiltShift1 = AGCLoopShift;
	AGCManualGain = pContext->AGCManualGain;
	AGCTarget  = pContext->AGCTarget ;
	AGCMaxGain = pContext->AGCMaxGain; 
	AGCMinGain = pContext->AGCMinGain ;
	PwrLoThres = pContext->PwrLoThres * (UINT64)(1 << 28);
	PwrHiThres = pContext->PwrHiThres * (UINT64)(1 << 28); 

	if (pContext->flags & LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN) {
		// Baseband
		//
		// If we're demodulating to baseband then an intermediate
		// sample rate is required for baseband filtering. This rate
		// is typically fixed at 32x168 KHz (but I've provided an API
		// to change it).
		//

		RequiredUpSampling = pContext->AdcDacFreq * (UINT64)(1 << 18);
		do_div(RequiredUpSampling, (pContext->Fs_div ) );
		do_div(RequiredUpSampling, 4 );
		do_div(RequiredUpSampling, ConditionalDownsampleRatio );
		do_div(RequiredUpSampling, PF1_Tinc );
		RequiredUpSampling = RequiredUpSampling * (UINT64)(1 << 18);
		do_div(RequiredUpSampling, pContext->BBSampRate );
		do_div(RequiredUpSampling, 1000 );
		do_div(RequiredUpSampling, 32 );

//		RequiredUpSampling =  ( ( pContext->AdcDacFreq * (UINT64)(1 << 18) ) / pContext->Fs_div / 4 / ConditionalDownsampleRatio / PF1_Tinc * (UINT64)(1 << 18) ) / (pContext->BBSampRate * 1000 * 32) ;    

		PFSif_Tinc = RequiredUpSampling;
		do_div(PFSif_Tinc, (1 << 4));
		PFSif_Tinc = PFSif_Tinc * (UINT64)(1 << 4) ; 
		
//		PFSif_Tinc = RequiredUpSampling / (UINT64)(1 << 4) * (UINT64)(1 << 4) ;  

		PFSif_TincInv = (UINT64)(1 << 18) * (UINT64)(1 << 18);
		do_div(PFSif_TincInv, PFSif_Tinc);  //jeongpil.yun- check !!
//		PFSif_TincInv = (UINT64)(1 << 18) * (UINT64)(1 << 18) / PFSif_Tinc; // shift is too big, check

		PFSif_DeliveryRate = (DeliveryRateSoFar * PF1_Tinc * PFSif_Tinc);
		do_div(PFSif_DeliveryRate, (1 << 18));
		do_div(PFSif_DeliveryRate, (1 << 18));
//		PFSif_DeliveryRate = (DeliveryRateSoFar * PF1_Tinc * PFSif_Tinc) / (UINT64)(1 << 18) / (UINT64)(1 << 18) ;  // shift is too big, check

		pContext->FinalUp_TincInv = (PF1_Tinc * ConditionalDownsampleRatio * 32 * (UINT64)(1 << 17)) * PFSif_Tinc;
		do_div(pContext->FinalUp_TincInv, (1 << 18));
		do_div(pContext->FinalUp_TincInv, (1 << 18));
//		pContext->FinalUp_TincInv = (PF1_Tinc * ConditionalDownsampleRatio * 32 * (UINT64)(1 << 17)) * PFSif_Tinc / (UINT64)(1 << 18) / (UINT64)(1 << 18) ;    

		FinalUp_Tinc    = (UINT64)(1 << 24) * (UINT64)(1 << 17);
		do_div(FinalUp_Tinc, pContext->FinalUp_TincInv);  //jeongpil.yun- check !!

		FinalUp_DeliveryRate = 4;
		calcTemp1 = ((INT64)pContext->Fs_div * 4 * pContext->FinalUp_TincInv);
		BBF_Fs = (pContext->AdcDacFreq * (UINT64)(1 << 17));
		do_div(BBF_Fs, calcTemp1);  //jeongpil.yun- check !!
//		BBF_Fs = (pContext->AdcDacFreq * (UINT64)(1 << 17)) / (pContext->Fs_div * 4 * pContext->FinalUp_TincInv);

		//
		// baseband gains mode dependent, but depends on either agctarget (for am) and fmdeviation (for fm)
		//
		demodgain2  =  pContext->BasebandExtraGain * 25 * (UINT64)(1 << 10);
		do_div(demodgain2, 100); // 1.5625
		
		// use * 2**10 to keep extra precision in gain calculation, will reduce to 2**4 precision later    
		if (pContext->flags & LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN) {
			demodgain1 = (607252935 * (UINT64)(1 << 26));
			do_div(demodgain1, 1000000000);	
			demodgain1 = demodgain1 * (UINT64)(1 << 13);
			do_div(demodgain1, AGCTarget);
//			demodgain1 = ( (607252935 * (UINT64)(1 << 26)) / 1000000000 ) * (UINT64)(1 << 13) / (AGCTarget);
			demodshift3 = 1;
		}
		else {
			demodgain1 = ( pContext->AdcDacFreq * (UINT64)(1 << 17) * 32 * 10 * (UINT64)(1 << 4) );
			do_div(demodgain1, pContext->FinalUp_TincInv);	
			do_div(demodgain1, pContext->Deviation);	
			demodgain1 = demodgain1 *  (UINT64)(1 << 10);
			do_div(demodgain1, pContext->Fs_div );	
			do_div(demodgain1, 4 );	
			do_div(demodgain1, 2 );	
			do_div(demodgain1, 2 );	
			do_div(demodgain1, 4 );	
			demodgain1 = demodgain1 + 5;
			do_div(demodgain1, 10 );	
//			demodgain1 = ( ( pContext->AdcDacFreq * (UINT64)(1 << 17) * 32 * 10 * (UINT64)(1 << 4) ) / (pContext->FinalUp_TincInv) / (pContext->Deviation) * (UINT64)(1 << 10) / pContext->Fs_div / 4  / 2 / 2 / 4 + 5 ) / 10 ;   
			demodshift3 = 0;
		}
	
		for (demodshift1 = 0 ; demodgain1 >= (INT64)(1 << 14); demodshift1++) {
//			demodgain1 /= 2;
			demodgain1 =  demodgain1 * 10;
			do_div(demodgain1, 2);
			demodgain1 = demodgain1 + 5;
			do_div(demodgain1, 10);
//			demodgain1 = ( demodgain1 * 10 / 2 + 5 ) / 10;
		}


		if ((pContext->flags & LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN) &&
			(pContext->flags & LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN))
		{ // SECAM_L and Mono - BUG# 862
			demodshift1 = 0;
		}

		if (demodgain2 >= (INT64)(1 << 14) ) {
			for (demodshift2 = 0; demodgain2 >= (INT64)(1 << 15); demodshift2++) {
//				demodgain2 /= 2;
				demodgain2 =  demodgain2 * 10;
				do_div(demodgain2, 2);
				demodgain2 = demodgain2 + 5;
				do_div(demodgain2, 10);
//				demodgain2 = ( demodgain2 * 10 / 2 + 5 ) / 10;
			}
		}
		else {
			for (demodshift2 = 0 ; demodgain2 < (INT64)(1 << 14); demodshift2--) {
				demodgain2 *= 2;
			}
		}

		demodgain1 = (demodgain1 >= (INT64)(1 << 15)) ? ((INT64)(1 << 15) -1) : demodgain1;
		demodgain2 = (demodgain2 >= (INT64)(1 << 15)) ? ((INT64)(1 << 15) -1) : demodgain2;

		demodgain1 =  demodgain1 * (UINT64)(1 << 4) * 10;
		do_div(demodgain1, (1 << 14));
		demodgain1 = demodgain1 + 5;
		do_div(demodgain1, 10);
//		demodgain1 = ( demodgain1 * (UINT64)(1 << 4) * 10 / (UINT64)(1 << 14) + 5 ) / 10 ; 
//		demodgain2 /= (UINT64)(1 << 10);

		demodgain2 =  demodgain2 * (UINT64)(1 << 4) * 10;
		do_div(demodgain2, (1 << 14));
		demodgain2 = demodgain2 + 5;
		do_div(demodgain2, 10);

//		demodgain2 = ( demodgain2 * (UINT64)(1 << 4) * 10 / (UINT64)(1 << 14) + 5 ) / 10 ; 

		//
		// deemphasis ... depends on the demod type and the sampling rate... hardcode them for 75/50 for now
		//
		if (pContext->flags & LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN) {
			pContext->flags &= ~LX_ADEMOD_FFLAG_DEEMPHASIS_EN;
		}
		else {
			pContext->flags |= LX_ADEMOD_FFLAG_DEEMPHASIS_EN;
			if (pContext->Deemphasis == 75) {
				if ( BBF_Fs < 155000 ) {
					DeemphasisCoeff = 21;
				}
				else if ( BBF_Fs < 164000 ) {
					DeemphasisCoeff = 20;
				}
				else if ( BBF_Fs < 173000 ) {
					DeemphasisCoeff = 19;
				}
				else if ( BBF_Fs < 183000 ) {
					DeemphasisCoeff = 18;
				}
				else if ( BBF_Fs < 194000 ) {
					DeemphasisCoeff = 17;
				}
				else if ( BBF_Fs < 208000 ) {
					DeemphasisCoeff = 16;
				}
				else if ( BBF_Fs < 222000 ) {
					DeemphasisCoeff = 15;
				}
				else if ( BBF_Fs < 239000 ) {
					DeemphasisCoeff = 14;
				}
				else {
					DeemphasisCoeff = 13;
				}
			}
			else {
				if ( BBF_Fs < 153000 ) {
					DeemphasisCoeff = 31;
				}
				else if ( BBF_Fs < 159000 ) {
					DeemphasisCoeff = 30;
				}
				else if ( BBF_Fs < 165000 ) {
					DeemphasisCoeff = 29;
				}
				else if ( BBF_Fs < 171000 ) {
					DeemphasisCoeff = 28;
				}
				else if ( BBF_Fs < 178000 ) {
					DeemphasisCoeff = 27;
				}
				else if ( BBF_Fs < 186000 ) {
					DeemphasisCoeff = 26;
				}
				else if ( BBF_Fs < 194000 ) {
					DeemphasisCoeff = 25;
				}
				else if ( BBF_Fs < 203000 ) {
					DeemphasisCoeff = 24;
				}
				else if ( BBF_Fs < 212000 ) {
					DeemphasisCoeff = 23;
				}
				else if ( BBF_Fs < 222000 ) {
					DeemphasisCoeff = 22;
				}
				else if ( BBF_Fs < 234000 ) {
					DeemphasisCoeff = 21;
				}
				else {
					DeemphasisCoeff = 20;
				}
			}
		}

		//
		// mute controls only depends on the baseband sampling rate... finally an easier one.
		//
		MuteVolumeOnTime  = ( pContext->MuteOnSpeed  * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10;
		do_div(MuteVolumeOnTime, 1000);
		do_div(MuteVolumeOnTime, pContext->Fs_div);
		do_div(MuteVolumeOnTime, 4);
		do_div(MuteVolumeOnTime, pContext->FinalUp_TincInv);
		do_div(MuteVolumeOnTime, 64);
		MuteVolumeOnTime = MuteVolumeOnTime + 5;
		do_div(MuteVolumeOnTime, 10);
//		MuteVolumeOnTime  = ( ( pContext->MuteOnSpeed  * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10 / 1000 / pContext->Fs_div /4 / pContext->FinalUp_TincInv / 64 + 5 ) / 10 ;

		MuteVolumeOffTime  = ( pContext->MuteOffSpeed * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10;
		do_div(MuteVolumeOffTime, 1000);
		do_div(MuteVolumeOffTime, pContext->Fs_div);
		do_div(MuteVolumeOffTime, 4);
		do_div(MuteVolumeOffTime, pContext->FinalUp_TincInv);
		do_div(MuteVolumeOffTime, 64);
		MuteVolumeOffTime = MuteVolumeOffTime + 5;
		do_div(MuteVolumeOffTime, 10);
//		MuteVolumeOffTime = ( ( pContext->MuteOffSpeed * pContext->AdcDacFreq ) * (UINT64)(1 << 17) * 10 / 1000 / pContext->Fs_div /4 / pContext->FinalUp_TincInv / 64 + 5 ) / 10 ;

		
		if ( MuteVolumeOnTime > 255 )
		{
			MuteVolumeOnTime = 255;
		}
		else if ( MuteVolumeOnTime < 1 )
		{
			MuteVolumeOnTime = 1;
		}

		if ( MuteVolumeOffTime > 255 )
		{
			MuteVolumeOffTime = 255;
		}
		else if ( MuteVolumeOffTime < 1 )
		{
			MuteVolumeOffTime = 1;
		}
		
		//
		// output mixer isn't used in baseband mode make sure it is off
		//
		OutMix_Freq     = 0;
		pContext->flags &= ~LX_ADEMOD_FFLAG_SIF_MIX_UP; // Bypass Upmixer
	}
	else {
		// SIF
		//
		// if "just" Sif processing, then the "Final" polyphase is used
		// now to resample the signal back up to 1/4 the SampleRate
		// [because there are 2 fixed up-by-2 filters to bring the sample
		//  rate the rest of the way]. And we need to use the output
		//  mixer to center the processed Sif at the proper frequency.
		//

		pContext->FinalUp_TincInv = PF1_Tinc * (UINT64)(1 << 17) * ConditionalDownsampleRatio;
		do_div(pContext->FinalUp_TincInv, (1 << 18) );

		FinalUp_Tinc    = (UINT64)(1 << 24) * (UINT64)(1 << 17);
		do_div(FinalUp_Tinc, pContext->FinalUp_TincInv);  //jeongpil.yun- check !!
	
		//
		// essentially hard-wired because the final PF has to output at the
		// SampleRate / 4 rate
		//
		FinalUp_DeliveryRate = 4;

		OutMix_Freq = (SifOffset * (UINT64)(1 << 22) * pContext->Fs_div);
		do_div(OutMix_Freq,  pContext->AdcDacFreq);

		if (!(pContext->update & LX_ADEMOD_UFLAG_AUD_MIX_USR)) { // not user defined
			pContext->audio_dac.MixUp = (UINT32)OutMix_Freq;  
		}

		pContext->flags |= LX_ADEMOD_FFLAG_SIF_MIX_UP; // enable Upmixer

		//
		// Sif PF isn't used... 
		//
		PFSif_Tinc = 0;
		PFSif_TincInv = 0;
		PFSif_DeliveryRate = 16;

		// baseband stuffs don't care so not bother listing them for the moment (well, until power is an issue)
	}

	// Update Audio Registers
	//status = DEMOD_M14_Analog_AudioProc_Reset(pContext, TRUE, hUser); // Bug #1382

	MiscAudioCtrl = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_SIF_DCOTEST_EN)  ? 1 : 0) << 0) +
		(((pContext->flags & LX_ADEMOD_FFLAG_SIF_MIX_UP)      ? 0 : 1) << 1) + // BypassUpmixer
		(((pContext->flags & LX_ADEMOD_FFLAG_AUD_REAL_OUT_EN) ? 1 : 0) << 2) +
		(((pContext->flags & LX_ADEMOD_FFLAG_POWER_METER_EN)  ? 1 : 0) << 3)
	); 
	ADEMOD_WriteSub( hUser, MISCAUDIOCTRL, MiscAudioCtrl, 1);


	ExtraAudioCtrl = (UINT8)(
		(((F3Bypass)                                   ? 1 : 0) << 0) +
		(((F4Bypass)                                   ? 1 : 0) << 1)
	); 
	ADEMOD_WriteSub( hUser, EXTRAAUDIOCTRL, ExtraAudioCtrl, 1);

	if ((pContext->Mode == LX_ADEMOD_AUDIO_MODE_MONO) && 
		(pContext->update & LX_ADEMOD_UFLAG_AUD_MONOGAIN_USR))
	{
		demodgain1  = pContext->Mono.Gain1;
		demodgain2  = pContext->Mono.Gain2;
		demodshift1 = pContext->Mono.Shift1;
		demodshift2 = pContext->Mono.Shift2;
		demodshift3 = pContext->Mono.Shift3;
	}

	Gain1 = (UINT8)(
		((demodgain1)                                           << 0) +
		((demodshift1)                                          << 5)
	); 
	ADEMOD_WriteSub( hUser, GAIN1, Gain1, 1);

	SifCtrl_SC1 = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_NOT_VIDSLAVE_EN) ? 0 : 1) << 0) +
		((2 * F4Delay + F3Delay)                                << 1) +
		(((pContext->flags & LX_ADEMOD_FFLAG_BASEBAND_NSIF_EN)? 1 : 0) << 3) +
		(((pContext->flags & LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN) ? 1 : 0) << 4) +
		(((pContext->flags & LX_ADEMOD_FFLAG_BUZZ_FIX_EN)     ? (2 - BuzzLoc) : 0) << 5) +
		(((pContext->flags & LX_ADEMOD_FFLAG_BUZZ_FIX_EN)     ? 1 : 0) << 6) +
		(((pContext->flags & LX_ADEMOD_FFLAG_BUZZ_MIXFLIP_EN) ? 1 : 0) << 7)
	); 
	ADEMOD_WriteSub( hUser, SIFCTRL_SC1, SifCtrl_SC1, 1);

	ADEMOD_WriteSub( hUser, NOMINALFREQ_SC1,  (UINT32)InpMix_Freq, 3);
	ADEMOD_WriteSub( hUser, SIF1_OPFREQ,      (UINT32)OutMix_Freq, 3);
	ADEMOD_WriteSub( hUser, PF1_TINC_SC1,     (UINT32)PF1_Tinc, 3);
	ADEMOD_WriteSub( hUser, SIF_TINC,         (UINT32)PFSif_Tinc, 3);
	ADEMOD_WriteSub( hUser, SIF_TINCINV,      (UINT32)PFSif_TincInv, 3);
	ADEMOD_WriteSub( hUser, SIF_DELIVERYRATE, (UINT8)PFSif_DeliveryRate, 1);
	ADEMOD_WriteSub( hUser, FINALUP_TINC,     (UINT32)FinalUp_Tinc, 3);
	ADEMOD_WriteSub( hUser, FINALUP_TINCINV,  (UINT32)pContext->FinalUp_TincInv, 3);
	ADEMOD_WriteSub( hUser, FINALUP_DELIVERYRATE, (UINT32)FinalUp_DeliveryRate, 1);
	ADEMOD_WriteSub( hUser, BUZZDELAYAMOUNT,  (UINT8)BuzzDelay, 1);
	ADEMOD_WriteSub( hUser, PHASEBUZZTINC,    (UINT32)PFBuzz_Tinc, 3);
	ADEMOD_WriteSub( hUser, PHASEBUZZTINCINV, (UINT32)PFBuzz_TincInv, 3);

	AgcCntr_SC1 = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_AUD_AGC_EN)      ? 1 : 0) << 0) +
		(((pContext->flags & LX_ADEMOD_FFLAG_AGC_CLSLOOP_EN)  ? 1 : 0) << 1) +
		(((pContext->flags & LX_ADEMOD_FFLAG_AGC_HARDCLIP_EN) ? 1 : 0) << 2) +
		(((pContext->flags & LX_ADEMOD_FFLAG_AGC_SOFTCLIP_EN) ? 1 : 0) << 3)
	); 
	ADEMOD_WriteSub( hUser, AGC_CTRL_SC1, AgcCntr_SC1, 1);

	ABBF_Ctrl = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_DCREMOVAL_EN)    ? 1 : 0) << 0) +
		(((pContext->flags & LX_ADEMOD_FFLAG_DEEMPHASIS_EN)   ? 1 : 0) << 1) +
		(((pContext->flags & LX_ADEMOD_FFLAG_BSEBND_FILT_EN)  ? 0 : 1) << 2) +
		((demodshift3)                                          << 3)
	); 
	ADEMOD_WriteSub( hUser, ABBF_CTRL, ABBF_Ctrl, 1);

	ADEMOD_WriteSub( hUser, NONAGCGAIN_SC1,   (UINT32)AGCManualGain, 3);
	ADEMOD_WriteSub( hUser, AGCCOEF_B0SHIFT,  (UINT8)AGCFiltShift0, 1);
	ADEMOD_WriteSub( hUser, AGCCOEF_B1SHIFT,  (UINT8)AGCFiltShift1, 1);
	ADEMOD_WriteSub( hUser, AGCLOOPGAIN,      (UINT8)AGCLoopShift, 1);
	ADEMOD_WriteSub( hUser, AGCMAGREFERENCE_SC1, (UINT32)AGCTarget, 2);
	ADEMOD_WriteSub( hUser, AGCMAXGAIN,       (UINT32)AGCMaxGain, 2);
	ADEMOD_WriteSub( hUser, AGCMINGAIN,       (UINT32)AGCMinGain, 2);
	ADEMOD_WriteSub( hUser, PM_LOWTHRESHOLD,  (UINT32)PwrLoThres, 4);
	ADEMOD_WriteSub( hUser, PM_HITHRESHOLD,   (UINT32)PwrHiThres, 4);
	ADEMOD_WriteSub( hUser, ABBF_GAINFRAC,    (UINT8)demodgain2, 1);
	ADEMOD_WriteSub( hUser, ABBF_GAINSHIFT,   (UINT8)demodshift2, 1);
	ADEMOD_WriteSub( hUser, DCREMOVALCOEF,    (UINT8)pContext->DCRemovalShift, 1);
	ADEMOD_WriteSub( hUser, DEEMPHASISCOEF,   (UINT8)DeemphasisCoeff, 1);

	MuteCtrl = (UINT8)(
		(((pContext->flags & LX_ADEMOD_FFLAG_FORCEMUTE_EN)         ? 1 : 0) << 0) +
		(((pContext->flags & LX_ADEMOD_FFLAG_AUTOMUTE_FRLOOP_EN)   ? 1 : 0) << 1) +
		(((pContext->flags & LX_ADEMOD_FFLAG_AUTOMUTE_PHLOOP_EN)   ? 1 : 0) << 2)
	); 
	ADEMOD_WriteSub( hUser, MUTECTRL, MuteCtrl, 1);

	ADEMOD_WriteSub( hUser, MUTEVOLUMEONTIME, (UINT8)MuteVolumeOnTime, 1);
	ADEMOD_WriteSub( hUser, MUTEVOLUMEOFFTIME,(UINT8)MuteVolumeOffTime, 1);


	//
	// Set tone generation, by default tone generation is enabled
	//
	status = DEMOD_M14_Analog_AudioProc_ToneGen_Set(pContext, TRUE, TRUE, hUser);

	//  Audio DAC
	status = DEMOD_M14_Analog_AudioProc_DAC_Start(pContext, hUser);

	//
	// make sure Audio is powered up and out of reset
	//
	//status = DEMOD_M14_Analog_AudioProc_Reset(pContext, FALSE, hUser); // Bug #1382

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
**  Name: DEMOD_M14_Analog_AudioProc_DAC_Start
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
static LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_DAC_Start(pLX_ADEMOD_AudioProc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 CH1_gain = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	// apply gain according to the mode
	if (pContext->Mode == LX_ADEMOD_AUDIO_MODE_MONO) {
		if (pContext->flags & LX_ADEMOD_FFLAG_MONOAM_NOTLX_ADEMOD_EN) { // AM
			CH1_gain = pContext->audio_dac.CH1_gain_MonoAM;
		}
		else { // FM
			CH1_gain = pContext->audio_dac.CH1_gain_MonoFM;
		}
	}
	else { // SIF
		CH1_gain = pContext->audio_dac.CH1_gain_SIF;
	}
	ADEMOD_WriteSub( hUser, AUDIOCH1GAIN, (INT16)CH1_gain, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1GAIN_RC, (INT16)pContext->audio_dac_rc.CH1_gain_SIF, 2);
	// apply offset
	ADEMOD_WriteSub( hUser, AUDIOCH1OFFSET, (INT16)pContext->audio_dac.CH1_offs, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1OFFSET_RC, (INT16)pContext->audio_dac_rc.CH1_offs, 2);
	// apply clipping
	ADEMOD_WriteSub( hUser, AUDIOCH1MAXCLIPPING, (INT16)pContext->audio_dac.CH1_cliper, 2);
	ADEMOD_WriteSub( hUser, AUDIOCH1MINCLIPPING_RC, (INT16)-pContext->audio_dac_rc.CH1_cliper, 2);

	return status;

}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_Reset
**
**  Description:    Executes soft reset on video processing block.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context.
**					on_off       - When TRUE the reset state is entered, 
**                                 when FALSE the reset state is terminated. 
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_Reset(pLX_ADEMOD_AudioProc pContext, BOOL on_off, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// soft reset audio processing block,
	// register SOFTRESETN0:
	// bit 4 -audio_resetn, inverted

	// FIFO reset legacy demodulator and pre-processor, 
	// register FIFO_CLR0:
	// bit 7 - audio_Slave2Video_clr
	// register FIFO_CLR1:
	// bit 7 - audio_Slave2Video_clr
	// bit 3 - dac_if_audio2_fifo_clr
	// bit 2 - dac_if_audio1_fifo_clr

	if (on_off) {
		// set soft reset mask:   01101111b = 0x6f
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xc0); 
	}
	else {
		// clear soft reset mask: 01111111b = 0x7f
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xff); 
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AudioProc_ToneGen_Set
**
**  Description:    Sets tone generator.
**
**  Parameters:     pContext     - Pointer to the Audio Processor context.
**					tone1_en     - When TRUE tone 1 generation is enabled, otherwise disabled. 
**					tone2_en     - When TRUE tone 2 generation is enabled, otherwise disabled. 
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_AudioProc_ToneGen_Set(pLX_ADEMOD_AudioProc pContext, BOOL tone1_en, BOOL tone2_en, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}


	return status;
}

