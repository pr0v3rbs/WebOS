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


/** @file
 *
 *	OVI module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_ovi
 *	@{
 */

#ifndef  OVI_HW_H15_INC
#define  OVI_HW_H15_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ovi_kapi.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/
int OVI_HW_H15_Initialize(void);
int OVI_HW_H15_Close(void);
int OVI_HW_H15_Resume(void);
int OVI_HW_H15_SetCombination(UINT32 param);
int OVI_HW_H15_SetSpreadSpectrum(LX_OVI_SPREAD_SPECTRUM_T *pstParams);
int OVI_HW_H15_SetColorTempEnable(BOOLEAN bParam);
int OVI_HW_H15_SetGammaEnable(BOOLEAN bParam);
int OVI_HW_H15_SetColorTemp(LX_OVI_TCON_COLOR_TEMP_T *pstParams);
int OVI_HW_H15_SetGamma(LX_OVI_TCON_GAMMA_T *pstParams);
int OVI_HW_H15_SetLvdsReverse(BOOLEAN bParam);
int OVI_HW_H15_SetChannelPower(BOOLEAN bParam);
int OVI_HW_H15_SetLvdsFormat(LX_OVI_LVDS_OUT_STD_T enParam);
int OVI_HW_H15_SetLvdsBitDepth(LX_OVI_BIT_DEPTH_T enParam);
int OVI_HW_H15_SetLvdsBlack(BOOLEAN bParam);
int OVI_HW_H15_DisplayResume(LX_OVI_DISPLAY_INFO_T *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef OVI_HW_H15_INC  ----- */
/**  @} */

