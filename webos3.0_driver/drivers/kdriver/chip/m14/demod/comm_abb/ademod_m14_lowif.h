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
**  Name: ademod_m14_lowif.h
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

#ifndef _ADEMOD_M14_LOWIF_H_
#define _ADEMOD_M14_LOWIF_H_

#include "ademod_common.h"
#include "ademod_common_video.h"
#include "ademod_common_dvbt.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

#define LX_ADEMOD_BER_THRSH_SIZE		(5)
#define LX_ADEMOD_FFT_BER_SIZE			(4)


/*
**  LGT10 DVBT Context
*/
typedef struct LX_ADEMOD_M14_LowIF_tag {
	LX_ADEMOD_DvbtDemod	common;
	UINT32			state;
	UINT16			gain;
	UINT16			offset;
} LX_ADEMOD_M14_LowIF, *pLX_ADEMOD_M14_LowIF;



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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Factory_Set(pLX_ADEMOD_M14_LowIF pContext);

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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Flags_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 Flags, UINT32 mask, Handle_t hUser, BOOL Update);

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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Flags_Get(pLX_ADEMOD_M14_LowIF pContext, UINT32* pFlags);

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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Bandwidth_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 Bandwidth);

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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Status_Get(pLX_ADEMOD_M14_LowIF pContext, UINT32 *pValue, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Disable(pLX_ADEMOD_M14_LowIF pContext, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_LowIF_Enable
**
**  Description:    Starts LOW IF demodulator block.
**
**  Parameters:     pContext     - Pointer to the LOW IF demodulator context.
**                  hUser		 - Handle to user data.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Enable(pLX_ADEMOD_M14_LowIF pContext, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Gain_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

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
LX_ADEMOD_Result DEMOD_M14_Analog_LowIF_Offset_Set(pLX_ADEMOD_M14_LowIF pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

#if defined( __cplusplus )
}
#endif

#endif  //_ADEMOD_M14_LOWIF_H_
