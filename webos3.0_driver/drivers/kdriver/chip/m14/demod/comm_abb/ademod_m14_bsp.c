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
**  Name: ademod_m14_bsp.c
**
**  Description:    ABB M14 Board Support Package (BSP) module.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PowerUp
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Sleep_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Mode_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Clock_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Get
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Init
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PowerUp
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Sleep_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Mode_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Clock_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Get
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

#include "ademod_m14_config.h"
#include "ademod_common_userdef.h"
#include "ademod_common_bsp.h"


#include "ademod_m14_bsp.h"
#include "ademod_m14_regaddress.h"
#include "ademod_common_agc.h"

/*
**  Local Function Prototypes - not available for external access.
*/

static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_TunerQuite_Init(pLX_ADEMOD_Bsp pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_SoftwareVersion_Init(pLX_ADEMOD_Bsp pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PadOut_Enable(pLX_ADEMOD_Bsp pContext, Handle_t hUser);

static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PadWand_Enable(pLX_ADEMOD_Bsp pContext, Handle_t hUser);

/*
**  API implementation.
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Factory_Set
**
**  Description:    Initializes BSP context with factory settings.
**
**  Parameters:     pContext     - Pointer to the BSP context.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Factory_Set(pLX_ADEMOD_Bsp pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->version = LX_ADEMOD_VER;
	pContext->i2c_mode = LX_ADEMOD_I2CMODE_FAST_NONE;
	pContext->c_vc_tune = 2;
	pContext->c_ic_tune = 2;
	pContext->cvbs_dac_out = 0;
	pContext->func_oe = 0x71ff;
	pContext->fast_agc = 0;
	pContext->out_agc = 1;
	pContext->wand_agc = 1;
	pContext->sif_dac_out_dig = 2;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Flags_Set
**
**  Description:    Sets configuration flags.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  Flags        - The flags' bitmap,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Set(pLX_ADEMOD_Bsp pContext, UINT32 Flags, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 mask = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_CONFIG_OPTIONS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_CONFIG_OPTIONS_USR;
	}

	mask = pContext->flags ^ Flags;
	pContext->flags = Flags;
	if (mask) {
		pContext->update |= LX_ADEMOD_UFLAG_UPDATE;
	}

	if (Update) {
		// Update VGA bypass
		if ((pContext->factory & LX_ADEMOD_FFLAG_A2) && pContext->flags & LX_ADEMOD_CONFIG_FLAG_VGA_EN)
		{ // A2 and VGA enabled
			pContext->flags |= LX_ADEMOD_CONFIG_FLAG_VGA_STATUS; // Bug 1098: VGA status for read back
		}
		else {
			pContext->flags &= ~LX_ADEMOD_CONFIG_FLAG_VGA_STATUS; // Bug 1098: VGA status for read back
		}
	}

#if LX_ADEMOD_REGISTER_CACHE_ENABLED
	if (Flags & LX_ADEMOD_CONFIG_FLAG_I2C_OPTIM_EN) { // use cache to minimize I2C traffic.
		UINT32 cache_options = 0;
		RegisterCacheOptionsGet(&cache_options);
		cache_options |= LX_ADEMOD_CACHE_OPTIONS_WRITE;
		cache_options |= LX_ADEMOD_CACHE_OPTIONS_OPTIM;
		RegisterCacheOptionsSet(cache_options);
	}
	if (Flags & LX_ADEMOD_CONFIG_FLAG_I2C_MUTE) { // use this flag to mute I2C traffic to and from Spyder
		UINT32 cache_options = 0;
		RegisterCacheOptionsGet(&cache_options);
		cache_options |= LX_ADEMOD_CACHE_OPTIONS_I2C_MUTE;
		RegisterCacheOptionsSet(cache_options);
	}
#endif

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Flags_Get
**
**  Description:    Returns configuration flags.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Get(pLX_ADEMOD_Bsp pContext, UINT32* pFlags)
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
**  Name: DEMOD_M14_Analog_Bsp_FastAGC_Set
**
**  Description:    Sets Fast AGC mode.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - Fast AGC mode 1- on, 0- off
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_FastAGC_Set(pLX_ADEMOD_Bsp pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_FAST_AGC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_FAST_AGC_USR;
	}

	pContext->fast_agc = (UINT8)pValue[0];

	if (Update) {
		status = DEMOD_M14_Analog_Bsp_PadOut_Enable(pContext, hUser);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_FastAGC_Get
**
**  Description:    Returns Fast AGC mode.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  pMode        - Pointer to the fast AGC mode,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_FastAGC_Get(pLX_ADEMOD_Bsp pContext, UINT32* pMode, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pMode = pContext->fast_agc;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_OutAGC_Set
**
**  Description:    Sets AGC output mode.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC output mode 1- enabled, 0- disabled
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_OutAGC_Set(pLX_ADEMOD_Bsp pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_OUT_AGC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_OUT_AGC_USR;
	}

	pContext->out_agc = (UINT8)pValue[0];

	if (Update) {
		status = DEMOD_M14_Analog_Bsp_PadOut_Enable(pContext, hUser);
		if (pContext->out_agc == 0) {
			// The wand_en control has precedence over the pad_oen control in the pad logic.
			// This means that as long as wand_en = 1, the pad works as an open-drain output pad.
			// To put the RF AGC output pad to the HiZ state, you will need to set
			// AGC_OUT_func_oe = 0 and AGC_OUT_wand_en to 0.
			pContext->wand_agc = 0;
		}
		status = DEMOD_M14_Analog_Bsp_PadWand_Enable(pContext, hUser);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_OutAGC_Get
**
**  Description:    Returns AGC output mode.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  pMode        - Pointer to the AGC output mode,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_OutAGC_Get(pLX_ADEMOD_Bsp pContext, UINT32* pMode, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pMode = pContext->out_agc;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_WandAGC_Set
**
**  Description:    Sets AGC wand mode.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  pValue       - Pointer to the value,
**						where pValue[0] - AGC wand mode 1- enabled, 0- disabled
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_WandAGC_Set(pLX_ADEMOD_Bsp pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->update & LX_ADEMOD_UFLAG_WAND_AGC_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->update |= LX_ADEMOD_UFLAG_WAND_AGC_USR;
	}

	pContext->wand_agc = (UINT8)pValue[0];

	if (Update) {
		status = DEMOD_M14_Analog_Bsp_PadWand_Enable(pContext, hUser);
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_WandAGC_Get
**
**  Description:    Returns AGC wand mode.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  pMode        - Pointer to the AGC wand mode,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_WandAGC_Get(pLX_ADEMOD_Bsp pContext, UINT32* pMode, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pMode = pContext->wand_agc;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Init
**
**  Description:    Initializes BSP hardware.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Init(pLX_ADEMOD_Bsp pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// power up sequence
	status = DEMOD_M14_Analog_Bsp_PowerUp(pContext, hUser);

	// Disable watchdog
	//lreg[DOGCNTR/4] = 0x00;


	status = DEMOD_M14_Analog_Bsp_SoftwareVersion_Init(pContext, hUser);

	status = DEMOD_M14_Analog_Bsp_TunerQuite_Init(pContext, hUser);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Mode_Set
**
**  Description:    Sets input mode.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  VideoMode    - Video input mode,
**                  AudioMode    - Audio input mode,
**                  Divider      - Sample rate divider,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Mode_Set(pLX_ADEMOD_Bsp pContext, UINT32 VideoMode, UINT32 AudioMode, UINT32 Divider, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT32 c_dac_bias_tune = 0;
	UINT32 c_CVBS_ctrl = 0;
	UINT32 analog_ctrl3 = 0;
//	UINT32 sif_dac_out = 0;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if (!pContext) {
		return retErrBadContext;
	}

	switch (VideoMode)
	{
		case LX_ADEMOD_INPUTMODE_LEGACY:
			// power down DVB-T clock
			pContext->clocks = LX_ADEMOD_DVB_CLK_POWER_DOWN;
			status = DEMOD_M14_Analog_Bsp_Clock_Set(pContext, pContext->clocks, hUser);
			pContext->c_vc_tune = 0x2;
			pContext->c_ic_tune = 0x2;
			c_dac_bias_tune = 0x13;
			c_CVBS_ctrl = 0x3;
			analog_ctrl3 = 0x2;
			break;

		case LX_ADEMOD_INPUTMODE_DIGITAL:
			// power down Legacy video and audio clocks
			pContext->clocks = LX_ADEMOD_DVB_CLK_POWER_DOWN;
			c_dac_bias_tune = 0x12;
			analog_ctrl3 = 0;
			status = DEMOD_M14_Analog_Bsp_Clock_Set(pContext, pContext->clocks, hUser);
			// Digital Mode: digital IF is enabled.

			if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
				c_CVBS_ctrl = 0x7;
				pContext->c_vc_tune = 0x2;
				pContext->c_ic_tune = 0x1;
			}
			else {
				c_CVBS_ctrl = 0xf;
				pContext->c_vc_tune = 0x2;
				pContext->c_ic_tune = 0x2;
			}
			// Turn VGA off
			pContext->flags &= ~LX_ADEMOD_CONFIG_FLAG_VGA_STATUS; // Bug 1098: VGA status for read back
		break;

		case LX_ADEMOD_INPUTMODE_ATSC:
			// not supported yet
		default:
			status = retErrBadMode;
		break;

	}

	// Apply CVBS DAC output level

	// Apply output mode
    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Standard_Set
**
**  Description:    Sets video standard.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  VideoStandard  - The video standard,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK            - No errors
**                      retErrBadContext - Invalid context
**                      retErrBadMode    - Invalid input mode.
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Standard_Set(pLX_ADEMOD_Bsp pContext, UINT32 VideoStandard, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	BOOL VGAEnable = FALSE;

	if (!pContext) {
		return retErrBadContext;
	}

	VGAEnable = pContext->flags & LX_ADEMOD_CONFIG_FLAG_VGA_EN;

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

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
		case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
		break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:
			if (pContext->factory & LX_ADEMOD_FFLAG_A2) {
				if (pContext->flags & LX_ADEMOD_CONFIG_FLAG_MONO_SENS_L_EN) {
					// Bug 978: mono has noise limited sensitivity in standard L
					VGAEnable = TRUE;
				}
			}
		break;

		default:
			status = retErrBadParam;
		break;

	}

	// Turn VGA on/off
	if ((pContext->factory & LX_ADEMOD_FFLAG_A2) && VGAEnable)
	{ // A2 and VGA enabled
		pContext->flags |= LX_ADEMOD_CONFIG_FLAG_VGA_STATUS; // Bug 1098: VGA status for read back
	}
	else {
		pContext->flags &= ~LX_ADEMOD_CONFIG_FLAG_VGA_STATUS; // Bug 1098: VGA status for read back
	}

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_PowerUp
**
**  Description:    LGT10 Power up sequence.
**
**  Parameters:     pContext     - Pointer to the BSP context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PowerUp(pLX_ADEMOD_Bsp pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// soft reset digital core
	ADEMOD_WriteSub( hUser, SOFTRESETN0, 0xc0, 1);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Sleep_Set
**
**  Description:    LGT10 low power state enabling sequence.
**
**  Parameters:     pContext     - Pointer to the BSP context.
**                  Mode         - The sleep mode.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Sleep_Set(pLX_ADEMOD_Bsp pContext, UINT32 Mode, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (Mode) {
		case LX_ADEMOD_SLEEP_MODE_NONE:
			// wake up:
			// restore clocks
			status |= DEMOD_M14_Analog_Bsp_Clock_Set(pContext, pContext->clocks, hUser);
			break;
		case LX_ADEMOD_SLEEP_MODE_LEVEL_1:
			// low-power mode 1
			// store current clocks, then power down
			status |= DEMOD_M14_Analog_Bsp_Clock_Set(pContext, LX_ADEMOD_ALL_CLK_POWER_DOWN, hUser);
			break;
		default:
			status = retErrBadMode;
			break;
	}

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Start
**
**  Description:    System start.
**
**  Parameters:     pContext     - Pointer to the BSP context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Start(pLX_ADEMOD_Bsp pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

    DEMOD_PRINT( "[ABB M14](%s:%d) start !!!\n",__F__,__L__);

	if (!pContext) {
		return retErrBadContext;
	}

	status = DEMOD_M14_Analog_Bsp_PadOut_Enable(pContext, hUser);
	if (pContext->out_agc == 0) {
		// The wand_en control has precedence over the pad_oen control in the pad logic.
		// This means that as long as wand_en = 1, the pad works as an open-drain output pad.
		// To put the RF AGC outptu pad to the HiZ state, you will need to set
		// AGC_OUT_func_oe = 0 and AGC_OUT_wand_en to 0.
		pContext->wand_agc = 0;
	}
	status = DEMOD_M14_Analog_Bsp_PadWand_Enable(pContext, hUser);

    DEMOD_PRINT( "[ABB M14](%s:%d) end !!!\n",__F__,__L__);

	return status;
}


/*
**  Static functions implementation.
*/


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_SoftwareVersion_Init
**
**  Description:    LGT10 Software version initialization.
**
**  Parameters:     pContext     - Pointer to the BSP context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_SoftwareVersion_Init(pLX_ADEMOD_Bsp pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	// Write Software Version - format VBYTE2.BYTE1.BYTE0
	ADEMOD_WriteByte(hUser, RESERVED_TEST1, (UINT8)((pContext->version)       & 0xff));
	ADEMOD_WriteByte(hUser, RESERVED_TEST2, (UINT8)((pContext->version >> 8)  & 0xff));
	ADEMOD_WriteByte(hUser, RESERVED_TEST3, (UINT8)((pContext->version >> 16) & 0xff));
	ADEMOD_WriteByte(hUser, RESERVED_TEST4, (UINT8)((pContext->version >> 24) & 0xff));

	// Write Software Flags
	ADEMOD_WriteByte(hUser, RESERVED_TEST5, (UINT8)(pContext->flags));

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_TunerQuite_Init
**
**  Description:    LGT10 quite tuner mode initialization.
**
**  Parameters:     pContext     - Pointer to the BSP context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_TunerQuite_Init(pLX_ADEMOD_Bsp pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 pad_oen = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	ADEMOD_WriteSub( hUser, GPIO_MUXCONTROL, 0x03AAAAAA, 4);
	ADEMOD_WriteSub( hUser, TWA_MASTERBAUDRATE, LX_ADEMOD_TWA_MASTER_BAUDRATE_FCT, 1);
	pad_oen = (UINT8)pContext->func_oe & 0xff;
	ADEMOD_WriteSub( hUser, PAD_OEN0, pad_oen, 1);
	status = DEMOD_M14_Analog_Bsp_PadWand_Enable(pContext, hUser);
	//ADEMOD_WriteSub( hUser, TWA_MASTERCTRL, 0x00, 1); // wr ack - prep for write BUG1081

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_PadWand_Enable
**
**  Description:    Enables PAD WAND register.
**
**  Parameters:     pContext     - Pointer to the BSP context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PadWand_Enable(pLX_ADEMOD_Bsp pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 pad_wand_en1 = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	pad_wand_en1 = (UINT8)(
		(((pContext->flags & LX_ADEMOD_CONFIG_FLAG_SDA_WAND_EN)			? 1 : 0) << 0) +
		(((pContext->flags & LX_ADEMOD_CONFIG_FLAG_SCL_WAND_EN)			? 1 : 0) << 1) +
		(((pContext->wand_agc)										? 1 : 0) << 2) +
		(((pContext->flags & LX_ADEMOD_CONFIG_FLAG_SCL_TUN_WAND_DIS)		? 0 : 1) << 3) +
		(((pContext->flags & LX_ADEMOD_CONFIG_FLAG_SDA_TUN_WAND_DIS)		? 0 : 1) << 4)
	);
	ADEMOD_WriteSub( hUser, PAD_WAND_EN1, pad_wand_en1, 1);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_PadOut_Enable
**
**  Description:    Enables/disables PADs.
**
**  Parameters:     pContext     - Pointer to the legacy demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
static LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PadOut_Enable(pLX_ADEMOD_Bsp pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 pad_oen = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	pad_oen = (UINT8)pContext->func_oe & 0xff;

	if (pContext->out_agc) {
		if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
			pad_oen &= ~0x4;
		}
		else {
			pad_oen |= 0x4;
		}
	}
	else {
		if (pContext->factory & LX_ADEMOD_FFLAG_A2) { // A2
			pad_oen |= 0x4;
		}
		else {
			pad_oen &= ~0x4;
		}
	}

	//Bug1042: enable VGA with off-chip noise filtering
	if (pContext->flags & LX_ADEMOD_CONFIG_FLAG_OFFCHIP_NF_EN) {
		// disable ATB1/2 GPIO digital buffers
		pad_oen &= ~0x3;
	}
	else {
		pad_oen |= 0x3;
	}

	// Bug1060: pad_oen0 bit 3 (D_TBUS_3_func_oe)
	if (pContext->fast_agc) {
		pad_oen &= ~0x8;
	}
	else {
		pad_oen |= 0x8;
	}

	pContext->func_oe &= ~0xff;
	pContext->func_oe |= pad_oen;
	ADEMOD_WriteSub( hUser, PAD_OEN0, pad_oen, 1);

	pad_oen = (UINT8)((pContext->func_oe >> 8) & 0xff);
	ADEMOD_WriteSub( hUser, PAD_OEN1, pad_oen, 1);

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Clock_Set
**
**  Description:    Sets LGT10 clock.
**
**  Parameters:     pContext     - Pointer to the BSP context,
**                  Mask         - Bit mask of clocks to be set,
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Clock_Set(pLX_ADEMOD_Bsp pContext, UINT32 Mask, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
	UINT8 ClkPower = 0;

	if (!pContext) {
		return retErrBadContext;
	}

	ClkPower = (UINT8)(
		(((Mask & LX_ADEMOD_ADC_CLK_POWER_DOWN)     ? 1 : 0) << 0) +
		(((Mask & LX_ADEMOD_DAC_CLK_POWER_DOWN)     ? 1 : 0) << 1) +
		(((Mask & LX_ADEMOD_PREPROC_CLK_POWER_DOWN) ? 1 : 0) << 2) +
		(((Mask & LX_ADEMOD_VIDEO_CLK_POWER_DOWN)   ? 1 : 0) << 3) +
		(((Mask & LX_ADEMOD_AUDIO_CLK_POWER_DOWN)   ? 1 : 0) << 4) +
		(((Mask & LX_ADEMOD_MICRO_CLK_POWER_DOWN)   ? 1 : 0) << 5) +
		(((Mask & LX_ADEMOD_DVB_CLK_POWER_DOWN)     ? 1 : 0) << 6)
	);
	ADEMOD_WriteSub( hUser, CLOCKCTRL1, ClkPower, 1);

	return status;
}



