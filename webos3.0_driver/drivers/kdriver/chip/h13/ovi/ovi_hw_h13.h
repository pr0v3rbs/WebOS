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
 
#ifndef  OVI_HW_H13_INC
#define  OVI_HW_H13_INC
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
int OVI_HW_H13_Initialize(void);
int OVI_HW_H13_Close(void);
int OVI_HW_H13_Resume(void);

int OVI_HW_H13_SetCombination(UINT32 param);
int OVI_HW_H13_SetSpreadSpectrum(LX_OVI_SPREAD_SPECTRUM_T *pstParams);
int OVI_HW_H13_SetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams);
int OVI_HW_H13_SetFrameOdcEnable(BOOLEAN bParam);
int OVI_HW_H13_SetColorTempEnable(BOOLEAN bParam);
int OVI_HW_H13_SetGammaEnable(BOOLEAN bParam);
int OVI_HW_H13_SetDitherEnable(BOOLEAN bParam);
int OVI_HW_H13_SetLineOdcMode(LX_OVI_TCON_LINEOD_MODE_T *pstParams);
int OVI_HW_H13_SetDitherMode(LX_OVI_TCON_DITHER_T enParam);
int OVI_HW_H13_SetColorTemp(LX_OVI_TCON_COLOR_TEMP_T *pstParams);
int OVI_HW_H13_SetGamma(LX_OVI_TCON_GAMMA_T *pstParams);
int OVI_HW_H13_GetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams);
int OVI_HW_H13_SetOdByteData(LX_OVI_TCON_OD_LUT_DEBUG_T *pstParams);
int OVI_HW_H13_SetTconReverse(BOOLEAN bParam);
int OVI_HW_H13_SetLvdsReverse(BOOLEAN bParam);
int OVI_HW_H13_SetPatternGenerator(LX_OVI_TCON_TPG_MODE_T enParam);
int OVI_HW_H13_SetChannelPower(BOOLEAN bParam);
int OVI_HW_H13_SetLvdsFormat(LX_OVI_LVDS_OUT_STD_T enParam);
int OVI_HW_H13_SetLvdsBitDepth(LX_OVI_BIT_DEPTH_T enParam);
int OVI_HW_H13_SetLvdsBlack(BOOLEAN bParam);
int OVI_HW_H13_SetClock(BOOLEAN bParam);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef OVI_HW_H13_INC  ----- */
/**  @} */

