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
**  Name: ademod_m14_bsp.h
**
**  Description:   ABB M14 Board Support Package (BSP) module.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Init
**                  LX_ADEMOD_Result LX_ADEMOD_Bsp_PowerUp
**                  LX_ADEMOD_Result LX_ADEMOD_Bsp_Sleep_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Mode_Set
**                  LX_ADEMOD_Result LX_ADEMOD_Bsp_Clock_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Get
**
**  References:     FMSW-0001: Fresco SDK. Software Design Document.
**
**  Exports:        LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Factory_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Init
**                  LX_ADEMOD_Result LX_ADEMOD_Bsp_PowerUp
**                  LX_ADEMOD_Result LX_ADEMOD_Bsp_Sleep_Set
**                  LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Mode_Set
**                  LX_ADEMOD_Result LX_ADEMOD_Bsp_Clock_Set
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
#ifndef _ADEMOD_M14_BSP_H_
#define _ADEMOD_M14_BSP_H_

#include "ademod_common.h"
#include "ademod_common_bsp.h"

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif


//typedef LX_ADEMOD_Bsp	LX_ADEMOD_Bsp;
//typedef pLX_ADEMOD_Bsp	pLX_ADEMOD_Bsp;

#define LX_ADEMOD_ALL_CLK_POWER_UP			(0x00)
#define LX_ADEMOD_ADC_CLK_POWER_DOWN		(0x01)
#define LX_ADEMOD_DAC_CLK_POWER_DOWN		(0x02)
#define LX_ADEMOD_PREPROC_CLK_POWER_DOWN	(0x04)
#define LX_ADEMOD_VIDEO_CLK_POWER_DOWN		(0x08)
#define LX_ADEMOD_AUDIO_CLK_POWER_DOWN		(0x10)
#define LX_ADEMOD_MICRO_CLK_POWER_DOWN		(0x20)
#define LX_ADEMOD_DVB_CLK_POWER_DOWN		(0x40)
#define LX_ADEMOD_ALL_CLK_POWER_DOWN		(0x7f)

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Factory_Set(pLX_ADEMOD_Bsp pContext);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Init(pLX_ADEMOD_Bsp pContext, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_PowerUp(pLX_ADEMOD_Bsp pContext, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Mode_Set(pLX_ADEMOD_Bsp pContext, UINT32 VideoMode, UINT32 AudioMode, UINT32 Divider, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Standard_Set(pLX_ADEMOD_Bsp pContext, UINT32 VideoStandard, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_FastAGC_Set(pLX_ADEMOD_Bsp pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_FastAGC_Get(pLX_ADEMOD_Bsp pContext, UINT32* pMode, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_OutAGC_Set(pLX_ADEMOD_Bsp pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_OutAGC_Get(pLX_ADEMOD_Bsp pContext, UINT32* pMode, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_WandAGC_Set(pLX_ADEMOD_Bsp pContext, UINT32 *pValue, Handle_t hUser, BOOL Update);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_WandAGC_Get(pLX_ADEMOD_Bsp pContext, UINT32* pMode, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Start(pLX_ADEMOD_Bsp pContext, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Sleep_Set(pLX_ADEMOD_Bsp pContext, UINT32 Mode, Handle_t hUser);

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
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Clock_Set(pLX_ADEMOD_Bsp pContext, UINT32 Mask, Handle_t hUser);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Flags_Set
**
**  Description:    Sets configuration flags.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  Flags        - The flags' bitmap,
**                  hUser		 - Handle to user data,
**                  Update       - The update flag.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Set(pLX_ADEMOD_Bsp pContext, UINT32 Flags, Handle_t hUser, BOOL Update);

/******************************************************************************
**
**  Name: DEMOD_M14_Analog_Bsp_Flags_Get
**
**  Description:    Returns configuration flags.
**
**  Parameters:     pContext     - Pointer to the Video Processor context,
**                  pFlags       - Pointer to the flags' bitmap.
**
**  Returns:        status:
**                      retOK           - No errors
**                      retErrBadContext - Invalid context
**
******************************************************************************/
LX_ADEMOD_Result DEMOD_M14_Analog_Bsp_Flags_Get(pLX_ADEMOD_Bsp pContext, UINT32* pFlags);


#if defined( __cplusplus )
}
#endif

#endif  //_ADEMOD_M14_BSP_H_
