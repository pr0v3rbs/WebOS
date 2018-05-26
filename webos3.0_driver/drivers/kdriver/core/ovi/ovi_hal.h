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
 *  OVI module
 *
 *  author      dj911.kim@lge.com
 *  version     1.0
 *  date
 *  note        Additional information.
 *
 *  @addtogroup lg115x_ovi
 *  @{
 */

#ifndef _OVI_HW_H_
#define _OVI_HW_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
#define TO_STR(x)	#x

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "ovi_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */


/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int OVI_HAL_Init(void);
int OVI_HAL_Resume(void);
int OVI_HAL_GetDateInfo(void);
int OVI_HAL_SetCombination(UINT32 param);
int OVI_HAL_SetSpreadSpectrum(LX_OVI_SPREAD_SPECTRUM_T *pstParams);
int OVI_HAL_SetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams);
int OVI_HAL_SetFrameOdcEnable(BOOLEAN bParam);
int OVI_HAL_SetColorTempEnable(BOOLEAN bParam);
int OVI_HAL_SetGammaEnable(BOOLEAN bParam);
int OVI_HAL_SetDitherEnable(BOOLEAN bParam);
int OVI_HAL_SetLineOdcMode(LX_OVI_TCON_LINEOD_MODE_T *pstParams);
int OVI_HAL_SetDitherMode(LX_OVI_TCON_DITHER_T enParam);
int OVI_HAL_SetColorTemp(LX_OVI_TCON_COLOR_TEMP_T *pstParams);
int OVI_HAL_SetGamma(LX_OVI_TCON_GAMMA_T *pstParams);
int OVI_HAL_GetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams);
int OVI_HAL_SetOdByteData(LX_OVI_TCON_OD_LUT_DEBUG_T *pstParams);
int OVI_HAL_SetTconReverse(BOOLEAN bParam);
int OVI_HAL_SetLvdsReverse(BOOLEAN bParam);
int OVI_HAL_SetPatternGenerator(LX_OVI_TCON_TPG_MODE_T enParam);
int OVI_HAL_SetChannelPower(BOOLEAN bParam);
int OVI_HAL_SetLvdsFormat(LX_OVI_LVDS_OUT_STD_T enParam);
int OVI_HAL_SetLvdsBitDepth(LX_OVI_BIT_DEPTH_T enParam);
int OVI_HAL_SetLvdsBlack(BOOLEAN bParam);
int OVI_HAL_SetClock(BOOLEAN bParam);
int OVI_HAL_DisplayResume(LX_OVI_DISPLAY_INFO_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/


#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _OVI_HW_H_ */

/** @} */

