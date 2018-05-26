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
**  Name:ademod_m14_lowif.c
**
**  Description:    ABB M14 LOW IF Demodulation block.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Bandwidth_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Status_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Disable
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Enable
**
**  References:     
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Bandwidth_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Status_Get
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Disable
**                  LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Enable
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

#include "ademod_m14_config.h"
#include "ademod_m14_demod.h"
#include "ademod_m14_video.h"
#include "ademod_m14_lowif.h"
#include "ademod_m14_regaddress.h"


/*
**  Local Function Prototypes - not available for external access.
*/


/*
**  API implementation.
*/

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Factory_Set
**
**  Description:    Initializes LOW IF demodulator's context with factory settings.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Factory_Set(pLX_ADEMOD_M14_LowIF pContext)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->gain = LX_ADEMOD_LOWIF_GAIN_FCT;
	pContext->offset = LX_ADEMOD_LOWIF_OFFS_FCT;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Flags_Set
**
**  Description:    Sets DvbtDemod flags.
**
**  Parameters:     pContext     - Pointer to the DvbtDemod context,
**                  Flags        - The flags' bitmap,
**                  mask         - Indicates, which flags where changed.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Flags_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 Flags, UINT32 mask, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	pContext->common.flags = Flags;


	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Flags_Get
**
**  Description:    Returns DvbtDemod flags.
**
**  Parameters:     pContext     - Pointer to the DvbtDemod context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Flags_Get(pLX_ADEMOD_M14_LowIF pContext, UINT32* pFlags)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	*pFlags = pContext->common.flags;

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Bandwidth_Set
**
**  Description:    Sets video bandwidth.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context,
**                  Bandwidth    - The new bandwidth value.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Bandwidth_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 Bandwidth)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	switch (Bandwidth)
	{
		case LX_ADEMOD_VIDEO_BANDWIDTH_6MHz:
			break;
		case LX_ADEMOD_VIDEO_BANDWIDTH_7MHz:
			break;
		case LX_ADEMOD_VIDEO_BANDWIDTH_8MHz:
		case LX_ADEMOD_VIDEO_BANDWIDTH_8p185MHz:
		default:
			break;
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Status_Get
**
**  Description:    Returns LOW IF demodulator's status parameters.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context.
**                  pValue       - Pointer to the LOW IF status value:
**						pValue[0]	Current Core state,
**						pValue[1]	Output of the PPM continuous pilot correlator,
**						pValue[2]	Estimated signal to noise ratio (SNR) in dB.,
**
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Status_Get(pLX_ADEMOD_M14_LowIF pContext, UINT32 *pValue, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */


	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Disable
**
**  Description:    Initializes LOW IF demodulator's context with factory settings.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Disable(pLX_ADEMOD_M14_LowIF pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}


	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Enable
**
**  Description:    Enables Low IF mode.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Enable(pLX_ADEMOD_M14_LowIF pContext, Handle_t hUser)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	ADEMOD_WriteSub( hUser, DAC_EN_CTRL, 0x8, 1);
	ADEMOD_WriteSub( hUser, VIDEOGAIN, pContext->gain, 2);
	ADEMOD_WriteSub( hUser, VIDEOOFFSET, pContext->offset, 2);

	return status;
}


/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Gain_Set
**
**  Description:    Sets video gain.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context.
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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Gain_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_LOWIF_GAIN_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_LOWIF_GAIN_USR;
	}

	pContext->gain = pValue[0];

	if (Update) {
		status = ADEMOD_WriteSub( hUser, VIDEOGAIN, pContext->gain, 2);
	}

	return status;
}

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Offset_Set
**
**  Description:    Sets video offset.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context.
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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Offset_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 *pValue, Handle_t hUser, BOOL Update)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

	if (!pContext) {
		return retErrBadContext;
	}

	if (pContext->common.factory & LX_ADEMOD_FFLAG_UPDATE) { // factory settings
		if (pContext->common.update & LX_ADEMOD_UFLAG_LOWIF_OFFS_USR) {
			// when user sticky bit is raised, don't reset to factory
			return retOK;
		}
	}
	else { // user settings
		pContext->common.update |= LX_ADEMOD_UFLAG_LOWIF_OFFS_USR;
	}

	pContext->offset = pValue[0];

	if (Update) {
		status = ADEMOD_WriteSub( hUser, VIDEOOFFSET, pContext->offset, 2);
	}

	return status;
}


