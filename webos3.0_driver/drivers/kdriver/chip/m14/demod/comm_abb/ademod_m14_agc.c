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
**  Name: ademod_m14_agc.c
**
**  Description:    ABB M14 AGC block.
**
**  Functions
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
**  References:     
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
**  Dependencies:  ademod_common.h   - for basic FM types,
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

#include "ademod_common.h"
#include "ademod_m14_config.h"
#include "ademod_m14_demod.h"
#include "ademod_m14_regaddress.h"
#include "ademod_m14_agc.h"


/*
**  Backoff LUTs
*/
#define LX_ADEMOD_RFAGC_BACKOFF_LUT_SIZE	(201)
static UINT32 LX_ADEMOD_RFAGC_BackoffLUT[LX_ADEMOD_RFAGC_BACKOFF_LUT_SIZE] = {

/* Backoff(dB)  +0     +0.1     +0.2     +0.3     +0.4     +0.5     +0.6     +0.7     +0.8     +0.9   */
/* --------  -------  -------  -------  -------  -------  -------  -------  -------  -------  ------- */
/* 0 dB  */  2097152, 2049415, 2002765, 1957176, 1912625, 1869089, 1826543, 1784966, 1744335, 1704629,
/* 1 dB  */  1665827, 1627908, 1590852, 1554640, 1519252, 1484670, 1450875, 1417849, 1385575, 1354035,
/* 2 dB  */  1323213, 1293093, 1263659, 1234895, 1206785, 1179315, 1152471, 1126237, 1100601, 1075548,
/* 3 dB  */  1051066, 1027141, 1003760,  980912,  958583,  936763,  915440,  894602,  874238,  854338,
/* 4 dB  */   834891,  815887,  797315,  779166,  761430,  744098,  727160,  710608,  694432,  678625,
/* 5 dB  */   663178,  648082,  633330,  618913,  604825,  591058,  577604,  564456,  551607,  539051,
/* 6 dB  */   526781,  514790,  503072,  491620,  480430,  469494,  458807,  448363,  438157,  428183,
/* 7 dB  */   418437,  408912,  399604,  390508,  381619,  372932,  364443,  356148,  348041,  340118,
/* 8 dB  */   332376,  324810,  317417,  310192,  303131,  296231,  289488,  282898,  276458,  270166,
/* 9 dB  */   264016,  258006,  252133,  246394,  240785,  235304,  229948,  224714,  219599,  214600,
/* 10 dB */   209715,  204941,  200276,  195718,  191263,  186909,  182654,  178497,  174434,  170463,
/* 11 dB */   166583,  162791,  159085,  155464,  151925,  148467,  145087,  141785,  138557,  135404,
/* 12 dB */   132321,  129309,  126366,  123489,  120679,  117932,  115247,  112624,  110060,  107555,
/* 13 dB */   105107,  102714,  100376,   98091,   95858,   93676,   91544,   89460,   87424,   85434,
/* 14 dB */    83489,   81589,   79731,   77917,   76143,   74410,   72716,   71061,   69443,   67863,
/* 15 dB */    66318,   64808,   63333,   61891,   60483,   59106,   57760,   56446,   55161,   53905,
/* 16 dB */    52678,   51479,   50307,   49162,   48043,   46949,   45881,   44836,   43816,   42818,
/* 17 dB */    41844,   40891,   39960,   39051,   38162,   37293,   36444,   35615,   34804,   34012,
/* 18 dB */    33238,   32481,   31742,   31019,   30313,   29623,   28949,   28290,   27646,   27017,
/* 19 dB */    26402,   25801,   25213,   24639,   24079,   23530,   22995,   22471,   21960,   21460,
/* 20 dB */    20972 };



/*
**  Local Function Prototypes - not available for external access.
*/
static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Flags_Set(pLX_ADEMOD_Agc pContext, UINT32 Flags, Handle_t hUser, BOOL Update);

static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Reset(pLX_ADEMOD_Agc pContext, BOOL on_off, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_RFGainThrsh_Set(pLX_ADEMOD_Agc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_IFGainThrsh_Set(pLX_ADEMOD_Agc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RFGainPreset_Set(pLX_ADEMOD_Agc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_IFGainPreset_Set(pLX_ADEMOD_Agc pContext);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_RfIIR_Shifts_Set(pLX_ADEMOD_Agc pContext, UINT32 RfIIR_BW);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_LPF(pLX_ADEMOD_Agc pContext, UINT32 LPF_BW);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_LPFGain_Shift(pLX_ADEMOD_Agc pContext, UINT32 LPFGain);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_LoopGain(pLX_ADEMOD_Agc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_BackOff(pLX_ADEMOD_Agc pContext, int BackOff_dB);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_Attack(pLX_ADEMOD_Agc pContext, UINT32 Attack);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_FastDecay(pLX_ADEMOD_Agc pContext, UINT32 DecayTimeOut_us);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_Decay(pLX_ADEMOD_Agc pContext, UINT32 Decay_us);

static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_DecFactor(pLX_ADEMOD_Agc pContext, UINT32 Dec_fs);

//static LX_ADEMOD_Result DEMOD_M14_Analog_DigAGC_FastDecay(pLX_ADEMOD_Agc pContext, UINT32 DecayTimeOut_us);

//static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_RFGainDAC_Start(pLX_ADEMOD_Agc pContext, Handle_t hUser);

//static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_IFGainDAC_Start(pLX_ADEMOD_Agc pContext, Handle_t hUser);

/*
**  API implementation.
*/

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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Factory_Set(pLX_ADEMOD_Agc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// RF AGC
	pContext->rf.rf_mode = LX_ADEMOD_AGC_TOP_MODE_FCT;
	pContext->rf.if_mode = LX_ADEMOD_AGC_TOP_MODE_FCT;
	pContext->rf.rf_gain = LX_ADEMOD_AGC_MANUAL_GAIN_FCT;
	pContext->rf.if_gain = LX_ADEMOD_AGC_MANUAL_GAIN_FCT;
	pContext->rf.rfiir_bw = LX_ADEMOD_AGC_RFIIR_WIDE_FCT;
	pContext->rf.lpf_gain_nerr_rf = LX_ADEMOD_AGC_OFF_GAIN_FCT;
	pContext->rf.lpf_gain_perr_rf = LX_ADEMOD_AGC_OFF_GAIN_FCT;
	pContext->rf.lpf_gain_nerr_if = LX_ADEMOD_AGC_OFF_GAIN_FCT;
	pContext->rf.lpf_gain_perr_if = LX_ADEMOD_AGC_OFF_GAIN_FCT;
	pContext->rf.instab_gain_adj = LX_ADEMOD_AGC_INSTABGAIN_NOMINAL_FCT;
	pContext->rf.leaky_pd_attack = LX_ADEMOD_AGC_ATTACK_FAST_FCT;
	pContext->rf.leaky_pd_decay = LX_ADEMOD_AGC_DECAY_SLOW_FCT;
	pContext->rf.fast_decay = LX_ADEMOD_AGC_FASTDECAY_FAST_FCT;
	pContext->rf.leaky_pd_decaywin = LX_ADEMOD_AGC_DECAYWIN_SHORT_FCT;
	pContext->rf.dec_fs = LX_ADEMOD_AGC_DECFACTOR_HIGH_FCT;
	pContext->rf.lpf_bw = LX_ADEMOD_AGC_RFBNDW_NARROW_FCT;
	pContext->rf.backoff = LX_ADEMOD_AGC_BACKOFF_HIGH_FCT;
	pContext->rf.speed = LX_ADEMOD_PRESET_OPTION_NONE;

	// RF DAC
	pContext->rf_dac.clipper = LX_ADEMOD_RF_DAC_CLIPPER_FCT;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Set(pLX_ADEMOD_Agc pContext, UINT32 options, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 flags = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	flags = pContext->common.flags;

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_OPTIONS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_OPTIONS_USR;
	}

	pContext->common.options = options;

	if (options & LX_ADEMOD_AGC_OPTION_VLTGPOL) {
	}

	// controlled by wideband/narrowband
	//if (options & LX_ADEMOD_AGC_OPTION_RFDSBL) {
	//	// disable RF AGC
	//	flags &= ~LX_ADEMOD_FFLAG_AGCRF_EN;
	//}
	//else {
	//	// enable RF AGC
	//	flags |= LX_ADEMOD_FFLAG_AGCRF_EN;
	//}

	if (options & LX_ADEMOD_AGC_OPTION_IFDSBL) {
		// disable IF AGC
		flags &= ~LX_ADEMOD_FFLAG_AGCIF_EN;
	}
	else {
		// enable IF AGC
		flags |= LX_ADEMOD_FFLAG_AGCIF_EN;
	}

	if (options & LX_ADEMOD_AGC_OPTION_RFGAIN_INV) {
		// enable RF AGC gain inversion
		flags |= LX_ADEMOD_FFLAG_AGCRF_GAIN_INV_EN;
	}
	else {
		// disable RF AGC gain inversion
		flags &= ~LX_ADEMOD_FFLAG_AGCRF_GAIN_INV_EN;
	}

	if (options & LX_ADEMOD_AGC_OPTION_IFGAIN_INV) {
		// enable RF AGC gain inversion
		flags |= LX_ADEMOD_FFLAG_AGCIF_GAIN_INV_EN;
	}
	else {
		// disable RF AGC gain inversion
		flags &= ~LX_ADEMOD_FFLAG_AGCIF_GAIN_INV_EN;
	}

	if (options & LX_ADEMOD_AGC_OPTION_EXTDSBL) {
		// disable external AGC
		flags |= LX_ADEMOD_FFLAG_AGC_EXTDSB;
	}
	else {
		// enable external AGC
		flags &= ~LX_ADEMOD_FFLAG_AGC_EXTDSB;
	}

	if (options & LX_ADEMOD_AGC_OPTION_BYPASSDIG) {
		// Digital AGC Bypass enabled
		flags &= ~LX_ADEMOD_FFLAG_DIGAGC_EN;
	}
	else {
		// Digital AGC Bypass disabled
		flags |= LX_ADEMOD_FFLAG_DIGAGC_EN;
	}

	if (options & LX_ADEMOD_AGC_OPTION_DSBLDIG) {
		// Digital AGC disabled
		flags &= ~LX_ADEMOD_FFLAG_DIGAGC_ACCUMEN;
	}
	else {
		// Digital AGC enabled
		flags |= LX_ADEMOD_FFLAG_DIGAGC_ACCUMEN;
	}

	// controlled by wideband/narrowband
	//if (options & LX_ADEMOD_AGC_OPTION_RFDSBLDIG) {
	//	// disable RF AGC
	//	flags &= ~LX_ADEMOD_FFLAG_DIGAGCRF_EN;
	//}
	//else {
	//	// enable RF AGC
	//	flags |= LX_ADEMOD_FFLAG_DIGAGCRF_EN;
	//}

	if (options & LX_ADEMOD_AGC_OPTION_IFDSBLDIG) {
		// disable IF AGC
		flags &= ~LX_ADEMOD_FFLAG_DIGAGCIF_EN;
	}
	else {
		// enable IF AGC
		flags |= LX_ADEMOD_FFLAG_DIGAGCIF_EN;
	}

	if (options & LX_ADEMOD_AGC_OPTION_DIG_GAINSEL) {
		flags |= LX_ADEMOD_FFLAG_DIGAGC_GAINSEL;
	}
	else {
		flags &= ~LX_ADEMOD_FFLAG_DIGAGC_GAINSEL;
	}

	if (options & LX_ADEMOD_AGC_OPTION_BITSPLIT_EN) {
		flags |= LX_ADEMOD_FFLAG_AGC_BITSPLIT_EN;
	}
	else {
		flags &= ~LX_ADEMOD_FFLAG_AGC_BITSPLIT_EN;
	}

	if (options & LX_ADEMOD_AGC_OPTION_QNOISEFILTER_EN) {
		flags |= LX_ADEMOD_FFLAG_AGC_QNOISEFILTER_EN;
	}
	else {
		flags &= ~LX_ADEMOD_FFLAG_AGC_QNOISEFILTER_EN;
	}

	// update AGC flags
	status = DEMOD_M14_Analog_AGC_Flags_Set(pContext, flags, hUser, Update);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Options_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->common.options;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_MixedMode_Set
**
**  Description:    Sets AGC mixed mode.
**
**  Parameters:     pContext     - Pointer to the AGC context,
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_MixedMode_Set(pLX_ADEMOD_Agc pContext, UINT32 Mode, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 flags = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	flags = pContext->common.flags;

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_MIXMODE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_MIXMODE_USR;
	}

	pContext->common.mode = Mode;

	if (Mode == LX_ADEMOD_AGC_LPF_MODE_WIDEBAND)
	{
		flags &= ~LX_ADEMOD_FFLAG_AGC_POSTSAW_EN;
		flags &= ~LX_ADEMOD_FFLAG_DIGAGCRF_EN;
		if (pContext->common.options & LX_ADEMOD_AGC_OPTION_RFDSBL) {
			flags &= ~LX_ADEMOD_FFLAG_AGCRF_EN;
		}
		else
		{
			flags |=  LX_ADEMOD_FFLAG_AGCRF_EN;
		}
	}
	else if (Mode == LX_ADEMOD_AGC_LPF_MODE_MIXED)
	{
		flags |= LX_ADEMOD_FFLAG_AGC_POSTSAW_EN;
		if (pContext->common.options & LX_ADEMOD_AGC_OPTION_RFDSBL) {
			flags &= ~LX_ADEMOD_FFLAG_AGCRF_EN;
			flags &= ~LX_ADEMOD_FFLAG_DIGAGCRF_EN;
		}
		else
		{
			flags |=  LX_ADEMOD_FFLAG_AGCRF_EN;
			flags |=  LX_ADEMOD_FFLAG_DIGAGCRF_EN;
		}
	}
	else if (Mode == LX_ADEMOD_AGC_LPF_MODE_NARROWBAND)
	{
		flags |= LX_ADEMOD_FFLAG_AGC_POSTSAW_EN;
		flags &= ~LX_ADEMOD_FFLAG_AGCRF_EN;
		if (pContext->common.options & LX_ADEMOD_AGC_OPTION_RFDSBL) {
			flags &= ~LX_ADEMOD_FFLAG_DIGAGCRF_EN;
		}
		else
		{
			flags |=  LX_ADEMOD_FFLAG_DIGAGCRF_EN;
		}
	}
	else if (Mode == LX_ADEMOD_AGC_LPF_MODE_HYBRID)
	{
		if (!(pContext->factory & LX_ADEMOD_FFLAG_DBG_1)) {
			return retErrBadDevice; // applicable to FM1151 only
		}
		flags |= LX_ADEMOD_FFLAG_AGC_POSTSAW_EN;
		if (pContext->common.options & LX_ADEMOD_AGC_OPTION_RFDSBL) {
			flags &= ~LX_ADEMOD_FFLAG_AGCRF_EN;
			flags &= ~LX_ADEMOD_FFLAG_DIGAGCRF_EN;
		}
		else
		{
			flags |=  LX_ADEMOD_FFLAG_AGCRF_EN;
			flags |=  LX_ADEMOD_FFLAG_DIGAGCRF_EN;
		}
	}
	else {
		return retErrBadParam;
	}

	status = DEMOD_M14_Analog_AGC_Flags_Set(pContext, flags, hUser, Update);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_MixedMode_Get
**
**  Description:    Returns AGC mixed mode.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  Mode         - 0- wideband mode,
**								   1- mixed mode,
**								   2- narrowband mode,
**                  hUser		 - Handle to user data,
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_MixedMode_Get(pLX_ADEMOD_Agc pContext, UINT32 *pMode, Handle_t hUsere)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pMode = pContext->common.mode;

	return status;
}

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
**  Note:           The positive/negative demodulation mode used in this function
**                  can be affected by DEMOD_M14_Analog_AGC_Standard_Set. Therefore it is a good idea
**                  to call DEMOD_M14_Analog_AGC_Standard_Set prior to this function.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Agc_Mode_Set(pLX_ADEMOD_Agc pContext, UINT32 InputMode)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	pContext->common.input_mode = InputMode;

	switch (InputMode)
	{
		case LX_ADEMOD_INPUTMODE_LEGACY:
			// TBD add those parameters into factory settings
			pContext->legacy.backporch_ref = LX_ADEMOD_AGC_BACKPORCH_FCT;
			pContext->legacy.min_gain = LX_ADEMOD_AGC_LEG_MIN_GAIN_FCT;
			pContext->legacy.max_gain = LX_ADEMOD_AGC_LEG_MAX_GAIN_FCT;
			pContext->rf.vga_attn = LX_ADEMOD_AGC_LEG_VGA_ATTN_FCT;
			pContext->common.flags |= LX_ADEMOD_FFLAG_DIGAGC_INPSLCT;
			pContext->common.flags &= ~LX_ADEMOD_FFLAG_AGC_LOWIF_EN;
		break;

		case LX_ADEMOD_INPUTMODE_DIGITAL:
			// TBD add those parameters into factory settings
			// Set Audio AGC registers to 0
			pContext->legacy.backporch_ref = 0;
			pContext->legacy.min_gain = 0;
			pContext->legacy.max_gain = 0;
			pContext->rf.vga_attn = LX_ADEMOD_AGC_DVB_VGA_ATTN_FCT;
			pContext->common.flags &= ~LX_ADEMOD_FFLAG_DIGAGC_INPSLCT;
			pContext->common.flags |= LX_ADEMOD_FFLAG_AGC_LOWIF_EN;
		break;

		case LX_ADEMOD_INPUTMODE_ATSC:
			// not supported yet
		default:
			status = retErrBadMode;
		break;

	}
    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to AGC context,
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Standard_Set(pLX_ADEMOD_Agc pContext, UINT32 VideoStandard)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (VideoStandard)
	{
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M_J:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_433:
		case LX_ADEMOD_VIDEO_STANDARD_NTSC_50:
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
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_B:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_G:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_H:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_D:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_K1:
		case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
			// set negative modulation
			pContext->common.flags &= ~LX_ADEMOD_FFLAG_AGCMOD_TYPE;
			pContext->common.flags |= LX_ADEMOD_FFLAG_AGCAUD_EN;
//			pContext->legacy.line_start = LX_ADEMOD_AGC_LINE_START_FCT;
//			pContext->legacy.line_stop = LX_ADEMOD_AGC_LINE_END_FCT;
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
			// set positive modulation
			pContext->common.flags |= LX_ADEMOD_FFLAG_AGCMOD_TYPE;
			pContext->common.flags &= ~LX_ADEMOD_FFLAG_AGCAUD_EN;
//			pContext->legacy.line_start = LX_ADEMOD_AGC_LINE_START_SECAM_FCT;
//			pContext->legacy.line_stop = LX_ADEMOD_AGC_LINE_END_SECAM_FCT;
		break;

		default:
			status = retErrBadParam;
		break;

	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory setting
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_GAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user update
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_GAIN_USR;
	}

	pContext->legacy.min_gain =  pValue[1] & 0xff;
	pContext->legacy.min_gain <<= 32;
	pContext->legacy.min_gain += pValue[0];
	pContext->legacy.max_gain =  pValue[3] & 0xff;
	pContext->legacy.max_gain <<= 32;
	pContext->legacy.max_gain += pValue[2];
	pContext->rf.max_gain_rf =   pValue[4];
	pContext->rf.min_gain_rf =   pValue[5];
	pContext->rf.pres_gain_rf =  pValue[6];
	pContext->rf.max_gain_if =   pValue[7];
	pContext->rf.min_gain_if =   pValue[8];
	pContext->rf.pres_gain_if =  pValue[9];
	pContext->rf.lpf_gain_nerr_rf = pValue[10];
	pContext->rf.lpf_gain_perr_rf = pValue[11];
	pContext->rf.instab_gain_adj = pValue[12];

	if (Update) 
	{

	}
	else 
	{
		pContext->common.update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Gain_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] =  (UINT32)(pContext->legacy.min_gain & 0xffffffff);
	pValue[1] =  (UINT32)((pContext->legacy.min_gain >> 32) & 0xff);
	pValue[2] =  (UINT32)(pContext->legacy.max_gain & 0xffffffff);
	pValue[3] =  (UINT32)((pContext->legacy.max_gain >> 32) & 0xff);
	pValue[4] =  pContext->rf.max_gain_rf;
	pValue[5] =  pContext->rf.min_gain_rf;
	pValue[6] =  pContext->rf.pres_gain_rf;
	pValue[7] =  pContext->rf.max_gain_if;
	pValue[8] =  pContext->rf.min_gain_if;
	pValue[9] =  pContext->rf.pres_gain_if;
	pValue[10] = pContext->rf.lpf_gain_nerr_rf;
	pValue[11] = pContext->rf.lpf_gain_perr_rf;
	pValue[12] = pContext->rf.instab_gain_adj;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainThrsh_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory setting
				if (pContext->rf.update & LX_ADEMOD_UFLAG_RFAGC_RFGAIN_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user setting
				pContext->rf.update |= LX_ADEMOD_UFLAG_RFAGC_RFGAIN_THR_USR;
			}
			pContext->rf.rf_mode = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->rf.min_gain_rf = pValue[2];
				pContext->rf.max_gain_rf = pValue[3];
			}
			else {
				status = DEMOD_M14_Analog_RFAGC_RFGainThrsh_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteSub( hUser, RFAGC_MAXGAIN_RF,        pContext->rf.max_gain_rf, 3);
				ADEMOD_WriteSub( hUser, RFAGC_MINGAIN_RF,        pContext->rf.min_gain_rf, 3);
			}
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory setting
				if (pContext->rf.update & LX_ADEMOD_UFLAG_RFAGC_IFGAIN_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user setting
				pContext->rf.update |= LX_ADEMOD_UFLAG_RFAGC_IFGAIN_THR_USR;
			}
			pContext->rf.if_mode = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->rf.min_gain_if = pValue[2];
				pContext->rf.max_gain_if = pValue[3];
			}
			else {
				status = DEMOD_M14_Analog_RFAGC_IFGainThrsh_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteSub( hUser, RFAGC_MAXGAIN_IF,        pContext->rf.max_gain_if, 3);
				ADEMOD_WriteSub( hUser, RFAGC_MINGAIN_IF,        pContext->rf.min_gain_if, 3);
			}
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainThrsh_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[2] = pContext->rf.min_gain_if;
	pValue[3] = pContext->rf.max_gain_if;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainPreset_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_RFGAIN_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->common.update |= LX_ADEMOD_UFLAG_AGC_RFGAIN_USR;
			}
			pContext->rf.rf_gain = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->rf.pres_gain_rf = pValue[2];
			}
			else {
				status = DEMOD_M14_Analog_AGC_RFGainPreset_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteSub( hUser, RFAGC_PRESETGAIN_RF,     pContext->rf.pres_gain_rf, 3);
			}
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_IFGAIN_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->common.update |= LX_ADEMOD_UFLAG_AGC_IFGAIN_USR;
			}
			pContext->rf.if_gain = pValue[1];
			if (pValue[1] == LX_ADEMOD_PRESET_OPTION_NONE) {
				pContext->rf.pres_gain_if = pValue[2];
			}
			else {
				status = DEMOD_M14_Analog_AGC_IFGainPreset_Set(pContext);
			}
			if (Update) {
				ADEMOD_WriteSub( hUser, RFAGC_PRESETGAIN_IF,     pContext->rf.pres_gain_if, 3);
			}
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_GainPreset_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			pValue[1] = pContext->rf.rf_gain;
			pValue[2] = pContext->rf.pres_gain_rf;
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			pValue[1] = pContext->rf.if_gain;
			pValue[2] =	pContext->rf.pres_gain_if;
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RFIIR_Bndw_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_BNDW_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_BNDW_USR;
	}

	pContext->rf.rfiir_mode = (UINT8)pValue[0];

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.rfiir_bw = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.rfiir_bw = LX_ADEMOD_AGC_RFIIR_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.rfiir_bw = LX_ADEMOD_AGC_RFIIR_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NARROW:
			pContext->rf.rfiir_bw = LX_ADEMOD_AGC_RFIIR_NARROW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_WIDE:
			pContext->rf.rfiir_bw = LX_ADEMOD_AGC_RFIIR_WIDE_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_RFAGC_RfIIR_Shifts_Set(pContext, pContext->rf.rfiir_bw);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_IIRCOEFF_BSHIFTS,  pContext->rf.IIRCoeff_BShifts, 2);
	}

	return status;
}

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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RFIIR_Bndw_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->rf.rfiir_mode;
	pValue[1] = pContext->rf.rfiir_bw;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_Speed_Set
**
**  Description:    Sets AGC RF Leaky Peak Detector speed.
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_Speed_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 temp = 0;
	UINT64 temp1 = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_LPD_SPEED_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_LPD_SPEED_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.leaky_pd_attack = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.leaky_pd_attack = LX_ADEMOD_AGC_ATTACK_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.leaky_pd_attack = LX_ADEMOD_AGC_ATTACK_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->rf.leaky_pd_attack = LX_ADEMOD_AGC_ATTACK_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->rf.leaky_pd_attack = LX_ADEMOD_AGC_ATTACK_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	temp1 = (UINT64)pContext->sampling_rate;
	do_div(temp1, 10);
	
	temp = (UINT64)pContext->rf.leaky_pd_attack * 288000000;
	do_div(temp, temp1);
	
	pContext->rf.leaky_pd_attack = (UINT32)temp; // note: scale by the same factor as decay for now; but unlike decay, attack is proportional to err

	if (status == retOK) {
		status = DEMOD_M14_Analog_RFAGC_Attack(pContext, pContext->rf.leaky_pd_attack);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_ATTACK_1,          pContext->rf.attack_1, 1);
		ADEMOD_WriteSub( hUser, RFAGC_ATTACK_2,          pContext->rf.attack_2, 1);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_Decay_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_LPD_DECAY_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user update
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_LPD_DECAY_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.leaky_pd_decay = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.leaky_pd_decay = LX_ADEMOD_AGC_DECAY_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.leaky_pd_decay = LX_ADEMOD_AGC_DECAY_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->rf.leaky_pd_decay = LX_ADEMOD_AGC_DECAY_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->rf.leaky_pd_decay = LX_ADEMOD_AGC_DECAY_FAST_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_RFAGC_Decay(pContext, pContext->rf.leaky_pd_decay);
	}


	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_DECAY,             pContext->rf.Leaky_Decay, 4);
		ADEMOD_WriteSub( hUser, RFAGC_PRESETDECAYCONST,  pContext->rf.Leaky_Decay, 4);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_FastDecay_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory setting
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_LPD_FASTDECAY_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user setting
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_LPD_FASTDECAY_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.fast_decay = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.fast_decay = LX_ADEMOD_AGC_FASTDECAY_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.fast_decay = LX_ADEMOD_AGC_FASTDECAY_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->rf.fast_decay = LX_ADEMOD_AGC_FASTDECAY_SLOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->rf.fast_decay = LX_ADEMOD_AGC_FASTDECAY_FAST_FCT;
		break;

		default:
			// should not get here
			pContext->rf.fast_decay = LX_ADEMOD_AGC_FASTDECAY_FAST_FCT;
			status = retErrBadMode;
			break;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_FASTDECAY,         pContext->rf.fast_decay, 2);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_DecayWindow_Set
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_DecayWindow_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_LPD_DECAYWIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_LPD_DECAYWIN_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.leaky_pd_decaywin = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.leaky_pd_decaywin = LX_ADEMOD_AGC_DECAYWIN_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.leaky_pd_decaywin = LX_ADEMOD_AGC_DECAYWIN_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_SHORT:
			pContext->rf.leaky_pd_decaywin = LX_ADEMOD_AGC_DECAYWIN_SHORT_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LONG:
			pContext->rf.leaky_pd_decaywin = LX_ADEMOD_AGC_DECAYWIN_LONG_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_RFAGC_FastDecay(pContext, pContext->rf.leaky_pd_decaywin);
	}


	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_WIN,               pContext->rf.win, 3);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_LPD_DecayThresh_Set
**
**  Description:    Sets AGC RF IIR Leaky Peak Detector decay threshold.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - mode: /min/max/nominal/none,
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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_DecayThresh_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_LPD_DECAYTHRSH_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_LPD_DECAYTHRSH_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.decay_thr = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.decay_thr = LX_ADEMOD_AGC_DECAYTHRSH_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.decay_thr = LX_ADEMOD_AGC_DECAYTHRSH_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->rf.decay_thr = LX_ADEMOD_AGC_DECAYTHRSH_NOMINAL_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_DECAYTHRESH,       pContext->rf.decay_thr, 4);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_LPD_DecFactor_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_DECFACTOR_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_DECFACTOR_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.dec_fs = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.dec_fs = LX_ADEMOD_AGC_DECFACTOR_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.dec_fs = LX_ADEMOD_AGC_DECFACTOR_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LOW:
			pContext->rf.dec_fs = LX_ADEMOD_AGC_DECFACTOR_LOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_HIGH:
			pContext->rf.dec_fs = LX_ADEMOD_AGC_DECFACTOR_HIGH_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_RFAGC_DecFactor(pContext, pContext->rf.dec_fs);
	}


	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_DECFACTOR,         pContext->rf.dec_factor, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_InstabGainAdj_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_INSTAB_GAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_INSTAB_GAIN_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.instab_gain_adj = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.instab_gain_adj = LX_ADEMOD_AGC_INSTABGAIN_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.instab_gain_adj = LX_ADEMOD_AGC_INSTABGAIN_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->rf.instab_gain_adj = LX_ADEMOD_AGC_INSTABGAIN_NOMINAL_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_INSTABGAINADJ,     pContext->rf.instab_gain_adj, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Bndw_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_LPFBNDW_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_LPFBNDW_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.lpf_bw = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.lpf_bw = LX_ADEMOD_AGC_RFBNDW_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.lpf_bw = LX_ADEMOD_AGC_RFBNDW_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_NARROW:
			pContext->rf.lpf_bw = LX_ADEMOD_AGC_RFBNDW_NARROW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_WIDE:
			pContext->rf.lpf_bw = LX_ADEMOD_AGC_RFBNDW_WIDE_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_RFAGC_LPF(pContext, pContext->rf.lpf_bw);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_LPFCOEFF_BSHIFTS,  pContext->rf.LPF_BShifts, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Speed_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_LPFSPEED_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_LPFSPEED_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.resp_time_poserr_RF = pValue[1];
			pContext->rf.resp_time_negerr_RF = pValue[2];
			pContext->rf.resp_time_poserr_IF = pValue[3];
			pContext->rf.resp_time_negerr_IF = pValue[4];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.resp_time_poserr_RF = 5000000;
			pContext->rf.resp_time_negerr_RF = 5000000;
			pContext->rf.resp_time_poserr_IF = 5000000;
			pContext->rf.resp_time_negerr_IF = 5000000;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.resp_time_poserr_RF = 100;
			pContext->rf.resp_time_negerr_RF = 100;
			pContext->rf.resp_time_poserr_IF = 100;
			pContext->rf.resp_time_negerr_IF = 100;
		break;

		case LX_ADEMOD_PRESET_OPTION_SLOW:
			pContext->rf.resp_time_poserr_RF = 300000;
			pContext->rf.resp_time_negerr_RF = 300000;
			pContext->rf.resp_time_poserr_IF = 300000;
			pContext->rf.resp_time_negerr_IF = 300000;
		break;

		case LX_ADEMOD_PRESET_OPTION_FAST:
			pContext->rf.resp_time_poserr_RF = 523;
			pContext->rf.resp_time_negerr_RF = 1046;
			pContext->rf.resp_time_poserr_IF = 2790;
			pContext->rf.resp_time_negerr_IF = 4000;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		pContext->rf.speed = pValue[0];
		status = DEMOD_M14_Analog_RFAGC_LoopGain(pContext,
			pContext->rf.resp_time_poserr_RF,
			pContext->rf.resp_time_negerr_RF,
			pContext->rf.resp_time_poserr_IF,
			pContext->rf.resp_time_negerr_IF);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_NEGERR_RF, pContext->rf.lpf_gain_nerr_rf, 2);
		ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_POSERR_RF, pContext->rf.lpf_gain_perr_rf, 2);
		ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_NEGERR_IF, pContext->rf.lpf_gain_nerr_if, 2);
		ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_POSERR_IF, pContext->rf.lpf_gain_perr_if, 2);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Speed_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] =  (UINT32)(pContext->rf.speed);
	pValue[1] =  (UINT32)(pContext->rf.resp_time_poserr_RF);
	pValue[2] =  (UINT32)(pContext->rf.resp_time_negerr_RF);
	pValue[3] =  (UINT32)(pContext->rf.resp_time_poserr_IF);
	pValue[4] =  (UINT32)(pContext->rf.resp_time_negerr_IF);

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Backoff_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_BACKOFF_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_BACKOFF_USR;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_PRESET_OPTION_NONE:
			pContext->rf.backoff = pValue[1];
			break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.backoff = LX_ADEMOD_AGC_BACKOFF_MIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.backoff = LX_ADEMOD_AGC_BACKOFF_MAX_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_LOW:
			pContext->rf.backoff = LX_ADEMOD_AGC_BACKOFF_LOW_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_HIGH:
			pContext->rf.backoff = LX_ADEMOD_AGC_BACKOFF_HIGH_FCT;
		break;

		default:
			// should not get here
			status = retErrBadMode;
			break;
	}

	if (status == retOK) {
		status = DEMOD_M14_Analog_RFAGC_BackOff(pContext, pContext->rf.backoff);
	}

	if (Update) {
		ADEMOD_WriteSub( hUser, RFAGC_REF,               pContext->rf.ref, 3);
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RF_Backoff_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pValue[0] = pContext->rf.backoff;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Dither_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_RFDITHER_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->common.update |= LX_ADEMOD_UFLAG_AGC_RFDITHER_USR;
			}
			pContext->rf_dac.DAC_Dither  = pValue[1];
			pContext->rf_dac.Dither_LFSR = pValue[2];
			pContext->rf_dac.Dither_init = pValue[3];

			if (Update) {
				// dither for RF gain
				ADEMOD_WriteSub( hUser, RFGAIN_DAC_DITHER,  pContext->rf_dac.DAC_Dither,  2);
				ADEMOD_WriteSub( hUser, RFGAIN_DITHER_LFSR, pContext->rf_dac.Dither_LFSR, 4);
				ADEMOD_WriteSub( hUser, RFGAIN_DITHER_INIT, pContext->rf_dac.Dither_init, 4);
			}
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_IFDITHER_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->common.update |= LX_ADEMOD_UFLAG_AGC_IFDITHER_USR;
			}
			pContext->if_dac.DAC_Dither  = pValue[1];
			pContext->if_dac.Dither_LFSR = pValue[2];
			pContext->if_dac.Dither_init = pValue[3];

			if (Update) {
				// dither for video, audio, IF gain
				ADEMOD_WriteSub( hUser, FOURBIT_DAC_DITHER,  pContext->if_dac.DAC_Dither,  2);
				ADEMOD_WriteSub( hUser, FOURBIT_DITHER_LFSR, pContext->if_dac.Dither_LFSR, 4);
				ADEMOD_WriteSub( hUser, FOURBIT_DITHER_INIT, pContext->if_dac.Dither_init, 4);
			}
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_InstabThresh_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pValue[0]) {

		case LX_ADEMOD_AGC_MODE_RF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->rf_dac.update & LX_ADEMOD_UFLAG_RFDAC_INST_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->rf_dac.update |= LX_ADEMOD_UFLAG_RFDAC_INST_THR_USR;
			}
			pContext->rf_dac.instab_thresh  = pValue[1];
			pContext->rf_dac.instab_min = (int)(-pContext->rf_dac.instab_thresh);
			if (pContext->rf_dac.instab_thresh >= (INT64)(1 << (LX_ADEMOD_RF_DAC_INSTAB_PREC_FCT + 1)))
			{
				pContext->rf_dac.instab_thresh = (INT64)(1 << (LX_ADEMOD_RF_DAC_INSTAB_PREC_FCT + 1)) - 1;
			}
			pContext->rf_dac.instab_max = (int)pContext->rf_dac.instab_thresh;

			if (Update) {
				ADEMOD_WriteSub( hUser, RFDAC_INSTAB_MAX, pContext->rf_dac.instab_max, 2);
				ADEMOD_WriteSub( hUser, RFDAC_INSTAB_MIN, pContext->rf_dac.instab_min, 2);
			}
			break;

		case LX_ADEMOD_AGC_MODE_IF:
			if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
				if (pContext->if_dac.update & LX_ADEMOD_UFLAG_IFDAC_INST_THR_USR) {
					// when user sticky bit is raised, don't reset to factory
					break;
				}
			}
			else { // user settings
				pContext->if_dac.update |= LX_ADEMOD_UFLAG_IFDAC_INST_THR_USR;
			}
			pContext->if_dac.instab_thresh  = pValue[1];
			pContext->if_dac.instab_min = (int)(-pContext->if_dac.instab_thresh);
			if (pContext->if_dac.instab_thresh >= (INT64)(1 << (LX_ADEMOD_IF_DAC_INSTAB_PREC_FCT + 1)))
			{
				pContext->if_dac.instab_thresh = (INT64)(1 << (LX_ADEMOD_IF_DAC_INSTAB_PREC_FCT + 1)) - 1;
			}
			pContext->if_dac.instab_max = (int)pContext->if_dac.instab_thresh;

			if (Update) {
				ADEMOD_WriteSub( hUser, IFDAC_INSTAB_MAX, pContext->if_dac.instab_max, 2);
				ADEMOD_WriteSub( hUser, IFDAC_INSTAB_MIN, pContext->if_dac.instab_min, 2);
			}
			break;

		default:
			status = retErrBadMode;
			break;
	}

	return status;
}



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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Parameters_Set(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_AGC_PARM_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_AGC_PARM_USR;
	}

	// Legacy AGC
	pContext->legacy.backporch_ref	= pValue[0];
//	pContext->legacy.line_start		= pValue[1];
//	pContext->legacy.line_stop		= pValue[2];
	// RF AGC
	pContext->rf.lpf_coeff_a1		= pValue[3];
	pContext->rf.lpf_coeff_b0		= pValue[4];
	pContext->rf.fast_decay			= pValue[5];
	pContext->rf.decay_thr			= pValue[6];
	pContext->rf.win				= pValue[7];
	pContext->rf.ref				= pValue[8];
	pContext->rf.iir_coeff_ashifts	= pValue[9];
//	pContext->rf.iir_coeff_a1; // obsolete, iir_coeff_ashifts is used instead
	pContext->rf.iir_coeff_b0		= pValue[10];
	pContext->rf.attack_1			= pValue[11];
	pContext->rf.attack_2			= pValue[12];
	pContext->rf.dec_factor			= pValue[13];
	// Audio AGC
//	pContext->audio.non_agc_gain	= pValue[14];
//	pContext->audio.magerror_coeff	= pValue[15];
//	pContext->audio.mag_ref			= pValue[16];
//	pContext->audio.fraction		= pValue[17];
//	pContext->audio.update_rate		= pValue[18];
//	pContext->audio.soft_clip_thr	= pValue[19];
//	pContext->audio.peak_decay		= pValue[20];
//	pContext->audio.peak_adj		= pValue[21];

	if (Update) {
		status  = DEMOD_M14_Analog_AGC_Reset(pContext, TRUE, hUser);
//		ADEMOD_WriteSub( hUser, SAFE_AGC_LINE_START,     pContext->legacy.line_start, 2);
//		ADEMOD_WriteSub( hUser, SAFE_AGC_LINE_STOP,      pContext->legacy.line_stop, 2);
		ADEMOD_WriteSub( hUser, RFAGC_ATTACK_1,          pContext->rf.attack_1, 2);
		ADEMOD_WriteSub( hUser, RFAGC_ATTACK_2,          pContext->rf.attack_2, 2);
		ADEMOD_WriteSub( hUser, RFAGC_FASTDECAY,         pContext->rf.fast_decay, 2);
		ADEMOD_WriteSub( hUser, RFAGC_DECAYTHRESH,       pContext->rf.decay_thr, 4);
		ADEMOD_WriteSub( hUser, RFAGC_WIN,               pContext->rf.win, 3);
		ADEMOD_WriteSub( hUser, RFAGC_DECFACTOR,         pContext->rf.dec_factor, 2);
		ADEMOD_WriteSub( hUser, RFAGC_REF,               pContext->rf.ref, 3);
//		ADEMOD_WriteSub( hUser, NONAGCGAIN_SC1,          pContext->audio.non_agc_gain, 1);
//		ADEMOD_WriteSub( hUser, AGCMAGREFERENCE_SC1,     pContext->audio.mag_ref, 2);
		status  = DEMOD_M14_Analog_AGC_Reset(pContext, FALSE, hUser);

	}
	else {
		pContext->common.update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Parameters_Get(pLX_ADEMOD_Agc pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// Legacy AGC
	pValue[0] = pContext->legacy.backporch_ref;
//	pValue[1] = pContext->legacy.line_start;
//	pValue[2] = pContext->legacy.line_stop;
	// RF AGC
	pValue[3] = pContext->rf.lpf_coeff_a1;
	pValue[4] = pContext->rf.lpf_coeff_b0;
	pValue[5] = pContext->rf.fast_decay;
	pValue[6] = pContext->rf.decay_thr;
	pValue[7] = pContext->rf.win;
	pValue[8] = pContext->rf.ref;
	pValue[9] = pContext->rf.iir_coeff_ashifts;
//	pContext->rf.iir_coeff_a1; // obsolete, iir_coeff_ashifts is used instead
	pValue[10] = pContext->rf.iir_coeff_b0;
	pValue[11] = pContext->rf.attack_1;
	pValue[12] = pContext->rf.attack_2;
	pValue[13] = pContext->rf.dec_factor;
	// Audio AGC
//	pValue[14] = pContext->audio.non_agc_gain;
//	pValue[15] = pContext->audio.magerror_coeff;
//	pValue[16] = pContext->audio.mag_ref;
//	pValue[17] = pContext->audio.fraction;
//	pValue[18] = pContext->audio.update_rate;
//	pValue[19] = pContext->audio.soft_clip_thr;
//	pValue[20] = pContext->audio.peak_decay;
//	pValue[21] = pContext->audio.peak_adj;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Init(pLX_ADEMOD_Agc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}
	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Start
**
**  Description:    Starts AGC block.
**
**  Parameters:     pContext     - Pointer to the AGC context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode   - Invalid mode
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Start(pLX_ADEMOD_Agc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 AgcCntr;
	UINT8 AgcCntr_SC1;
	UINT8 MixBits;
	UINT8 PostSAW_CntrBits;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	
	if (!pContext) {
		return retErrBadContext;
	}

	status  = DEMOD_M14_Analog_AGC_Reset(pContext, TRUE, hUser);

	if (pContext->common.input_mode == LX_ADEMOD_INPUTMODE_DIGITAL) {
		// in Legacy mode these registers are programmed by Audio module.
		// In Digital mode Audio is not being programmed, therefore we have to program these registeers here
		ADEMOD_WriteSub( hUser, NONAGCGAIN_SC1, 0, 1);
		ADEMOD_WriteSub( hUser, AGCMAGREFERENCE_SC1, 0, 2);
	}

	AgcCntr = (UINT8)(
		((((pContext->common.flags & LX_ADEMOD_FFLAG_AGCMOD_TYPE) ||
		   (pContext->common.flags & LX_ADEMOD_FFLAG_AGC_LOWIF_EN))     ? 0 : 1) << 0) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_LOWIF_EN)       ? 1 : 0) << 1) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_SOFTRESET)      ? 1 : 0) << 2) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_EN)          ? 1 : 0) << 3) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_INPSLCT)     ? 1 : 0) << 4) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_ACCUMEN)     ? 1 : 0) << 5) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_GAINSEL)     ? 1 : 0) << 6)
	);
	ADEMOD_WriteSub( hUser, AGC_CNTRBITS, AgcCntr, 1);

	// DigAGC PostSAW Enable
	PostSAW_CntrBits = (UINT8)(
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_POSTSAW_EN)     ? 1 : 0) << 0) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGCRF_EN)        ? 1 : 0) << 1) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGCIF_EN)        ? 1 : 0) << 2)
	);
	ADEMOD_WriteSub( hUser, POSTSAW_CNTRBITS, PostSAW_CntrBits, 1);

	AgcCntr_SC1 = (UINT8)(
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCAUD_EN)          ? 1 : 0) << 0) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCAUD_ERSET_ACC)   ? 1 : 0) << 4)
	);

#if (LX_ADEMOD_CONFIG_FLAGS & LX_ADEMOD_CONFIG_FLAG_SECAM_AM)
	// When Audio AGC (Secam AM) feature is Enabled,
	// then bit 0 of this register is controlled by the embedded firmware.
	// This code should not attempt to modify it.
#else
	ADEMOD_WriteSub( hUser, AGC_CTRL_SC1, AgcCntr_SC1, 1);
#endif

	MixBits = (UINT8)(
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCRF_EN)           ? 1 : 0) << 0) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCIF_EN)           ? 1 : 0) << 1) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCRF_GAIN_INV_EN)  ? 1 : 0) << 4) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCIF_GAIN_INV_EN)  ? 1 : 0) << 5) +
		(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_POSTSAW_EN)     ? 1 : 0) << 6)
	);
	ADEMOD_WriteSub( hUser, RFAGC_MIXBITS, MixBits, 1);

	ADEMOD_WriteSub( hUser, RFAGC_INSTABGAINADJ,     pContext->rf.instab_gain_adj, 2);
	ADEMOD_WriteSub( hUser, RFAGC_MAXGAIN_RF,        pContext->rf.max_gain_rf, 3);
	ADEMOD_WriteSub( hUser, RFAGC_MINGAIN_RF,        pContext->rf.min_gain_rf, 3);
	ADEMOD_WriteSub( hUser, RFAGC_PRESETGAIN_RF,     pContext->rf.pres_gain_rf, 3);
	ADEMOD_WriteSub( hUser, RFAGC_MAXGAIN_IF,        pContext->rf.max_gain_if, 3);
	ADEMOD_WriteSub( hUser, RFAGC_MINGAIN_IF,        pContext->rf.min_gain_if, 3);
	ADEMOD_WriteSub( hUser, RFAGC_PRESETGAIN_IF,     pContext->rf.pres_gain_if, 3);
	ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_NEGERR_RF, pContext->rf.lpf_gain_nerr_rf, 2);
	ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_POSERR_RF, pContext->rf.lpf_gain_perr_rf, 2);
	ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_NEGERR_IF, pContext->rf.lpf_gain_nerr_if, 2);
	ADEMOD_WriteSub( hUser, RFAGC_LPFGAIN_POSERR_IF, pContext->rf.lpf_gain_perr_if, 2);
	ADEMOD_WriteSub( hUser, RFAGC_ATTACK_1,          pContext->rf.attack_1, 1);
	ADEMOD_WriteSub( hUser, RFAGC_ATTACK_2,          pContext->rf.attack_2, 1);
	ADEMOD_WriteSub( hUser, RFAGC_FASTDECAY,         pContext->rf.fast_decay, 2);
	ADEMOD_WriteSub( hUser, RFAGC_DECAYTHRESH,       pContext->rf.decay_thr, 4);
	ADEMOD_WriteSub( hUser, RFAGC_WIN,               pContext->rf.win, 3);
	ADEMOD_WriteSub( hUser, RFAGC_DECFACTOR,         pContext->rf.dec_factor, 2);
	ADEMOD_WriteSub( hUser, RFAGC_REF,               pContext->rf.ref, 3);
	ADEMOD_WriteSub( hUser, RFAGC_IIRCOEFF_BSHIFTS,  pContext->rf.IIRCoeff_BShifts, 2);
	ADEMOD_WriteSub( hUser, RFAGC_DECAY,             pContext->rf.Leaky_Decay, 4);
	ADEMOD_WriteSub( hUser, RFAGC_PRESETDECAYCONST,  pContext->rf.Leaky_Decay, 4);
	ADEMOD_WriteSub( hUser, RFAGC_LPFCOEFF_BSHIFTS,  pContext->rf.LPF_BShifts, 2);

	status  = DEMOD_M14_Analog_AGC_Reset(pContext, FALSE, hUser);

	// clear update flag
	pContext->common.update &= ~LX_ADEMOD_UFLAG_UPDATE;

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


/*
** Static functions implementation
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Flags_Set
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
static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Flags_Set(pLX_ADEMOD_Agc pContext, UINT32 Flags, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->common.flags = Flags;

	if (Update) {
		{
			UINT8 MixBits = (UINT8)(
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCRF_EN)           ? 1 : 0) << 0) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCIF_EN)           ? 1 : 0) << 1) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCRF_GAIN_INV_EN)  ? 1 : 0) << 4) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGCIF_GAIN_INV_EN)  ? 1 : 0) << 5) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_POSTSAW_EN)     ? 1 : 0) << 6)
			);
			ADEMOD_WriteSub( hUser, RFAGC_MIXBITS, MixBits, 1);
		}

		{
			// DigAGC PostSAW Enable
			UINT8 PostSAW_CntrBits = (UINT8)(
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_POSTSAW_EN)     ? 1 : 0) << 0) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGCRF_EN)        ? 1 : 0) << 1) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGCIF_EN)        ? 1 : 0) << 2)
			);
			ADEMOD_WriteSub( hUser, POSTSAW_CNTRBITS, PostSAW_CntrBits, 1);
		}

		{
			UINT8 AgcCntr = (UINT8)(
				((((pContext->common.flags & LX_ADEMOD_FFLAG_AGCMOD_TYPE) ||
				   (pContext->common.flags & LX_ADEMOD_FFLAG_AGC_LOWIF_EN))     ? 0 : 1) << 0) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_LOWIF_EN)       ? 1 : 0) << 1) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_AGC_SOFTRESET)      ? 1 : 0) << 2) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_EN)          ? 1 : 0) << 3) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_INPSLCT)     ? 1 : 0) << 4) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_ACCUMEN)     ? 1 : 0) << 5) +
				(((pContext->common.flags & LX_ADEMOD_FFLAG_DIGAGC_GAINSEL)     ? 1 : 0) << 6)
			);
			ADEMOD_WriteSub( hUser, AGC_CNTRBITS, AgcCntr, 1);
		}

	}

	pContext->common.update |= LX_ADEMOD_UFLAG_AGC_FLAG_USR;

	return status;
}

#if 0
/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_RFGainDAC_Start
**
**  Description:    Starts RF gain DAC block.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Updates the registers according to the lates state of the
**                  variables.
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_RFGainDAC_Start(pLX_ADEMOD_Agc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (!(pContext->rf_dac.update & LX_ADEMOD_UFLAG_RFDAC_INST_THR_USR)) {
		pContext->rf_dac.instab_min = (int)(-pContext->rf_dac.instab_thresh);
		if (pContext->rf_dac.instab_thresh >= (INT64)(1 << (LX_ADEMOD_RF_DAC_INSTAB_PREC_FCT + 1)))
		{
			pContext->rf_dac.instab_thresh = (INT64)(1 << (LX_ADEMOD_RF_DAC_INSTAB_PREC_FCT + 1)) - 1;
		}
		pContext->rf_dac.instab_max = (int)pContext->rf_dac.instab_thresh;
	}
	ADEMOD_WriteSub( hUser, RFDAC_INSTAB_MAX, pContext->rf_dac.instab_max, 3);
	ADEMOD_WriteSub( hUser, RFDAC_INSTAB_MIN, pContext->rf_dac.instab_min, 3);

	// apply clipping
	ADEMOD_WriteSub( hUser, RFGAIN_MAXCLIPPING, (int)pContext->rf_dac.clipper, 3);
	ADEMOD_WriteSub( hUser, RFGAIN_MINCLIPPING, (int)-pContext->rf_dac.clipper, 3);

	// dither for RF gain
	ADEMOD_WriteSub( hUser, RFGAIN_DAC_DITHER,  (INT16)pContext->rf_dac.DAC_Dither, 2);
	ADEMOD_WriteSub( hUser, RFGAIN_DITHER_LFSR, (int)pContext->rf_dac.Dither_LFSR, 4);
	ADEMOD_WriteSub( hUser, RFGAIN_DITHER_INIT, (int)pContext->rf_dac.Dither_init, 4);


	return status;

}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_IFGainDAC_Start
**
**  Description:    Starts IF gain DAC block.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
**  Note:           Updates the registers according to the lates state of the
**                  variables.
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_IFGainDAC_Start(pLX_ADEMOD_Agc pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (!(pContext->if_dac.update & LX_ADEMOD_UFLAG_IFDAC_INST_THR_USR)) {
		pContext->if_dac.instab_min = (int)(-pContext->if_dac.instab_thresh);
		if (pContext->if_dac.instab_thresh >= (INT64)(1 << (LX_ADEMOD_RF_DAC_INSTAB_PREC_FCT + 1)))
		{
			pContext->if_dac.instab_thresh = (INT64)(1 << (LX_ADEMOD_RF_DAC_INSTAB_PREC_FCT + 1)) - 1;
		}
		pContext->if_dac.instab_max = (int)pContext->if_dac.instab_thresh;
	}
	ADEMOD_WriteSub( hUser, IFDAC_INSTAB_MAX, pContext->if_dac.instab_max, 2);
	ADEMOD_WriteSub( hUser, IFDAC_INSTAB_MIN, pContext->if_dac.instab_min, 2);

	// dither for video, audio, IF gain
	ADEMOD_WriteSub( hUser, FOURBIT_DAC_DITHER,  pContext->if_dac.DAC_Dither, 2);
	ADEMOD_WriteSub( hUser, FOURBIT_DITHER_LFSR, pContext->if_dac.Dither_LFSR, 4);
	ADEMOD_WriteSub( hUser, FOURBIT_DITHER_INIT, pContext->if_dac.Dither_init, 4);


	return status;

}

#endif
/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_RFGainThrsh_Set
**
**  Description:    Sets RF AGC min/max gain according to the top mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_RFGainThrsh_Set(pLX_ADEMOD_Agc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->rf.rf_mode)
	{
		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->rf.max_gain_rf = LX_ADEMOD_AGC_MAX_GAIN_RFNOM_FCT;
			pContext->rf.min_gain_rf = LX_ADEMOD_AGC_MIN_GAIN_RFNOM_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.max_gain_rf = LX_ADEMOD_AGC_MAX_GAIN_RFMIN_FCT;
			pContext->rf.min_gain_rf = LX_ADEMOD_AGC_MIN_GAIN_RFMIN_FCT;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.max_gain_rf = LX_ADEMOD_AGC_MAX_GAIN_RFMAX_FCT;
			pContext->rf.min_gain_rf = LX_ADEMOD_AGC_MIN_GAIN_RFMAX_FCT;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_IFGainThrsh_Set
**
**  Description:    Sets IF AGC min/max gain according to the top mode.
**
**  Parameters:     pContext     - Pointer to AGC context
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_IFGainThrsh_Set(pLX_ADEMOD_Agc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->rf.if_mode)
	{
		case LX_ADEMOD_PRESET_OPTION_NOMINAL:
			pContext->rf.max_gain_if = LX_ADEMOD_AGC_MAX_GAIN_IFNOM_FCT;
			if (pContext->common.flags & LX_ADEMOD_FFLAG_AGCRF_EN) {
				// when RF is on, then IF gain has higher min value
				pContext->rf.min_gain_if = LX_ADEMOD_AGC_HIGHMIN_GAIN_IFNOM_FCT;
			}
			else {
				pContext->rf.min_gain_if = LX_ADEMOD_AGC_MIN_GAIN_IFNOM_FCT;
			}
		break;

		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.max_gain_if = LX_ADEMOD_AGC_MAX_GAIN_IFMIN_FCT;
			if (pContext->common.flags & LX_ADEMOD_FFLAG_AGCRF_EN) {
				// when RF is on, then IF gain has higher min value
				pContext->rf.min_gain_if = LX_ADEMOD_AGC_HIGHMIN_GAIN_IFMIN_FCT;
			}
			else {
				pContext->rf.min_gain_if = LX_ADEMOD_AGC_MIN_GAIN_IFMIN_FCT;
			}
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.max_gain_if = LX_ADEMOD_AGC_MAX_GAIN_IFMAX_FCT;
			if (pContext->common.flags & LX_ADEMOD_FFLAG_AGCRF_EN) {
				// when RF is on, then IF gain has higher min value
				pContext->rf.min_gain_if = LX_ADEMOD_AGC_HIGHMIN_GAIN_IFMAX_FCT;
			}
			else {
				pContext->rf.min_gain_if = LX_ADEMOD_AGC_MIN_GAIN_IFMAX_FCT;
			}
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_RFGainPreset_Set
**
**  Description:    Sets RF AGC preset gain according to manual gain mode.
**
**  Parameters:     pContext     - Pointer to AGC context.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_RFGainPreset_Set(pLX_ADEMOD_Agc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->rf.rf_gain)
	{
		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.pres_gain_rf = pContext->rf.min_gain_rf;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.pres_gain_rf = pContext->rf.max_gain_rf;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIDDLE:
			pContext->rf.pres_gain_rf = 0;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_IFGainPreset_Set
**
**  Description:    Sets IF AGC preset gain according to manual gain mode.
**
**  Parameters:     pContext     - Pointer to AGC context,
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_IFGainPreset_Set(pLX_ADEMOD_Agc pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (pContext->rf.if_gain)
	{
		case LX_ADEMOD_PRESET_OPTION_MIN:
			pContext->rf.pres_gain_if = pContext->rf.min_gain_if;
		break;

		case LX_ADEMOD_PRESET_OPTION_MAX:
			pContext->rf.pres_gain_if = pContext->rf.max_gain_if;
		break;

		case LX_ADEMOD_PRESET_OPTION_MIDDLE:
			pContext->rf.pres_gain_if = 0;
		break;

		default:
			status = retErrBadMode;
		break;

	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_RfIIR_Shifts_Set
**
**  Description:    Sets RF IIR shift according to the bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  RfIIR_BW     - RF IIR Bandwidth.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_RfIIR_Shifts_Set(pLX_ADEMOD_Agc pContext, UINT32 RfIIR_BW)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 RfIIR_Coeff;
	INT64 RfIIR_Shift1;
	INT64 RfIIR_Shift2;
	INT64 RfIIR_Shift3;

	RfIIR_Coeff = ((RfIIR_BW * 26 * 2880 / 597) / (pContext->sampling_rate / 1000000) )/ 8;	//in kHz

	RfIIR_Shift1 = 7;
	while ((RfIIR_Coeff < (UINT64)(1 << RfIIR_Shift1)) && (RfIIR_Shift1 > 0))
	{
		RfIIR_Shift1--;
	}

	RfIIR_Coeff -= (UINT64)(1 << RfIIR_Shift1);

	RfIIR_Shift2 = RfIIR_Shift1;
	while ((RfIIR_Coeff < (UINT64)(1 << RfIIR_Shift2)) && (RfIIR_Shift2 > 0))
	{
		RfIIR_Shift2 = RfIIR_Shift2 - 1;
	}

	RfIIR_Coeff -= (UINT64)(1 << RfIIR_Shift2);

	RfIIR_Shift3 = RfIIR_Shift2;
	while ((RfIIR_Coeff < (UINT64)(1 << RfIIR_Shift3)) && (RfIIR_Shift3 > 0))
	{
		RfIIR_Shift3 = RfIIR_Shift3 - 1;
	}

	RfIIR_Shift1 -= 8;
	RfIIR_Shift2 -= 8;
	RfIIR_Shift3 -= 8;

	//pContext->rf.IIRCoeff_BShifts = (UINT32)((-RfIIR_Shift3-4)*64+(-RfIIR_Shift2-4)*8+(-RfIIR_Shift1-4));
	pContext->rf.IIRCoeff_BShifts = (UINT32)((-RfIIR_Shift3-1)*64+(-RfIIR_Shift2-1)*8+(-RfIIR_Shift1-1)); // implicit shift by 1 in Forza

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_LPF
**
**  Description:    Sets RF LPF shift according to the bandwidth.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  LPF_BW       - LPF Bandwidth.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_LPF(pLX_ADEMOD_Agc pContext, UINT32 LPF_BW)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 LPF_Coeff;
	UINT64 LPF_Shift1;
	UINT64 LPF_Shift2;

	LPF_Coeff =  (UINT64)LPF_BW * 3282858;
	do_div(LPF_Coeff, 117500);

	LPF_Shift1 = 31;

	while ((LPF_Coeff < (UINT64)1 << LPF_Shift1) && (LPF_Shift1 > 16))
	{
		LPF_Shift1--;
	}

	LPF_Coeff -= (UINT64)(1 << LPF_Shift1);

	LPF_Shift2 = LPF_Shift1;
	while ((LPF_Coeff < (UINT64)1 << LPF_Shift2) && (LPF_Shift2 > 16))
	{
		LPF_Shift2--;
	}

	LPF_Shift1 = 31 - LPF_Shift1;
	LPF_Shift2 = 31 - LPF_Shift2;

	pContext->rf.LPF_BShifts = (UINT16)(LPF_Shift2*32 + LPF_Shift1);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_LPFGain_Shift
**
**  Description:    Sets RF LPF gain shift according to the LPF gain.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  LPFGain      - LPF Bandwidth.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_LPFGain_Shift(pLX_ADEMOD_Agc pContext, UINT32 LPFGain)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 LPFGain_Shift1;
	UINT64 LPFGain_Shift2;

	LPFGain_Shift1 = 31;
	while ((LPFGain < (UINT64)1 << LPFGain_Shift1) && (LPFGain_Shift1 > 13))
	{
		LPFGain_Shift1--;
	}
	LPFGain -= (UINT32)(1 << LPFGain_Shift1);

	LPFGain_Shift2 = LPFGain_Shift1;
	while ((LPFGain < (UINT64)1 << LPFGain_Shift2) && (LPFGain_Shift2 > 13))
	{
		LPFGain_Shift2--;
	}

	pContext->rf.LPFGain_Shift1 = 31 - (UINT16)LPFGain_Shift1;
	pContext->rf.LPFGain_Shift2 = 31 - (UINT16)LPFGain_Shift2;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_LoopGain
**
**  Description:    Sets RF loop gain.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  LPFGain      - LPF Bandwidth.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_LoopGain(pLX_ADEMOD_Agc pContext,
									   UINT32 resp_time_poserr_RF,
									   UINT32 resp_time_negerr_RF,
									   UINT32 resp_time_poserr_IF,
									   UINT32 resp_time_negerr_IF)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 LPFGain_poserr_RF;
	UINT64 LPFGain_negerr_RF;
	UINT64 LPFGain_poserr_IF;
	UINT64 LPFGain_negerr_IF;

	LPFGain_poserr_RF = (UINT64)11160 * (UINT64)2097152;
	do_div(LPFGain_poserr_RF, (resp_time_poserr_RF));	//in us

	LPFGain_negerr_RF = (UINT64)11160 * (UINT64)2097152;
	do_div(LPFGain_negerr_RF, (resp_time_negerr_RF));	//in us

	LPFGain_poserr_IF = (UINT64)11160 * (UINT64)2097152 * 3;
	do_div(LPFGain_poserr_IF, (resp_time_poserr_IF));	//in us

	LPFGain_negerr_IF = (UINT64)11160 * (UINT64)2097152 * 3;
	do_div(LPFGain_negerr_IF, (resp_time_negerr_IF));	//in us

	DEMOD_M14_Analog_RFAGC_LPFGain_Shift(pContext, (UINT32)LPFGain_poserr_RF);
	pContext->rf.lpf_gain_perr_rf = (UINT16)(pContext->rf.LPFGain_Shift2 * 32 + pContext->rf.LPFGain_Shift1);

	DEMOD_M14_Analog_RFAGC_LPFGain_Shift(pContext, (UINT32)LPFGain_negerr_RF);
	pContext->rf.lpf_gain_nerr_rf = (UINT16)(pContext->rf.LPFGain_Shift2 * 32 + pContext->rf.LPFGain_Shift1);

	DEMOD_M14_Analog_RFAGC_LPFGain_Shift(pContext, (UINT32)LPFGain_poserr_IF);
	pContext->rf.lpf_gain_perr_if = (UINT16)(pContext->rf.LPFGain_Shift2 * 32 + pContext->rf.LPFGain_Shift1);

	DEMOD_M14_Analog_RFAGC_LPFGain_Shift(pContext, (UINT32)LPFGain_negerr_IF);
	pContext->rf.lpf_gain_nerr_if = (UINT16)(pContext->rf.LPFGain_Shift2 * 32 + pContext->rf.LPFGain_Shift1);

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_BackOff
**
**  Description:    Sets RF AGC backoff.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  BackOff_dB   - Backoff in dB.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_BackOff(pLX_ADEMOD_Agc pContext, int BackOff_dB)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (BackOff_dB < LX_ADEMOD_RFAGC_BACKOFF_LUT_SIZE) {
		pContext->rf.ref = LX_ADEMOD_RFAGC_BackoffLUT[BackOff_dB];
	}
	else {
		status = retErrBadParam;
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_Attack
**
**  Description:    Sets RF AGC attack.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  Attack       - Attack.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_Attack(pLX_ADEMOD_Agc pContext, UINT32 Attack)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 Attack_Shift1;
	UINT64 Attack_Shift2;

	Attack_Shift1 = 31;
	while ((Attack < (UINT64)1 << Attack_Shift1) && (Attack > 8))
	{
		Attack_Shift1--;
	}

	Attack -= (UINT32)(1 << Attack_Shift1);

	Attack_Shift2 = Attack_Shift1;
	while ((Attack < (UINT64)1 << Attack_Shift2) && (Attack_Shift2 > 8))
	{
		Attack_Shift2--;
	}

	pContext->rf.attack_1 = 31 - (UINT16)Attack_Shift1;
	pContext->rf.attack_2 = 31 - (UINT16)Attack_Shift2;

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_FastDecay
**
**  Description:    Sets RF AGC fast decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  DecayTimeOut_us - Decay timeout.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_FastDecay(pLX_ADEMOD_Agc pContext, UINT32 DecayTimeOut_us)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	UINT64 samplingRate;
	UINT64 FastDecayWin;

	samplingRate = (UINT64)(pContext->sampling_rate);
	do_div(samplingRate,1000000);
	
	//FastDecayWin = DecayTimeOut_us * 288 / 4096;
	FastDecayWin = DecayTimeOut_us * samplingRate;
	do_div(FastDecayWin, 2560); // reduced the decimation from 4096 to 256

	pContext->rf.win = (UINT32)FastDecayWin;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_Decay
**
**  Description:    Sets RF AGC decay.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  DecayTimeOut_us - Decay timeout.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_Decay(pLX_ADEMOD_Agc pContext, UINT32 Decay_us)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT64 temp = 0;
	UINT64 samplingRate;
	
	samplingRate = (UINT64)(pContext->sampling_rate );
	do_div(samplingRate, 10);

	//pContext->rf.Leaky_Decay = (UINT32)((10239 * 310)/Decay_us);
	temp = (UINT64)10239 * (UINT64)310 * (UINT64)288000000; 
	do_div(temp, Decay_us);
	do_div(temp, (UINT32)samplingRate);
	
	pContext->rf.Leaky_Decay = (UINT32)temp;

	return status;
}


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
LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_SampleRate_Set(pLX_ADEMOD_Agc pContext, UINT32 Value)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_AGC_SMP_RATE_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_AGC_SMP_RATE_USR;
	}

	pContext->sampling_rate = Value;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_RFAGC_DecFactor
**
**  Description:    Sets Dec factor.
**
**  Parameters:     pContext     - Pointer to AGC context,
**                  Dec_fs       - Dec factor (Hz).
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_RFAGC_DecFactor(pLX_ADEMOD_Agc pContext, UINT32 Dec_fs)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	//pContext->rf.dec_factor = (UINT32)(288000000/Dec_fs);
	pContext->rf.dec_factor = (UINT32)(pContext->sampling_rate/10/Dec_fs);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_AGC_Reset
**
**  Description:    Executes soft reset on AGC block.
**
**  Parameters:     pContext     - Pointer to the AGC context.
**					on_off       - When TRUE the reset state is entered,
**                                 when FALSE the reset state is terminated.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_AGC_Reset(pLX_ADEMOD_Agc pContext, BOOL on_off, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// FIFO reset AGC,
	// register FIFO_CLR0:
	// bit 4 - dac_if_rfgain_fifo_clr
	// bit 5 - dac_if_ifgain_fifo_clr

	if (on_off) {
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xc0);
	}
	else {
		ADEMOD_WriteByte( hUser, SOFTRESETN0, 0xdf);
	}

	return status;
}


