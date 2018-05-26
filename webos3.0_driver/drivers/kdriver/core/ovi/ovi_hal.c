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
 *  author     dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_ovi
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>	/**< printk() */
#include <linux/slab.h> 	/**< kmalloc() */
#include <linux/fs.h> 		/**< everything\ldots{} */
#include <linux/types.h> 	/**< size_t */
#include <linux/fcntl.h>	/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/timer.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>			/**< For ioremap_nocache */
#include <linux/interrupt.h>
#include "os_util.h"

#include "ovi_dbg.h"
#include "ovi_hal.h"
#include "h13/ovi/ovi_hw_h13.h"
#include "h14/ovi/ovi_hw_h14.h"
#include "m14/ovi/ovi_hw_m14.h"
#include "h15/ovi/ovi_hw_h15.h"

// Add here for register header file
/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	int (*pfnOviHwInitialize)(void);
	int (*pfnOviHwClose)(void);
	int (*pfnOviHwResume)(void);

	int (*pfnOviHwSetCombination)(UINT32 param);
	int (*pfnOviHwSetSpreadSpectrum)(LX_OVI_SPREAD_SPECTRUM_T *pstParams);
	int (*pfnOviHwSetOdLut)(LX_OVI_TCON_OD_DWLD_T *pstParams);
	int (*pfnOviHwSetFrameOdcEnable)(UINT32 param);
	int (*pfnOviHwSetColorTempEnable)(UINT32 param);
	int (*pfnOviHwSetGammaEnable)(UINT32 param);
	int (*pfnOviHwSetDitherEnable)(UINT32 param);
	int (*pfnOviHwSetLineOdcMode)(LX_OVI_TCON_LINEOD_MODE_T *pstParams);
	int (*pfnOviHwSetDitherMode)(LX_OVI_TCON_DITHER_T enParam);
	int (*pfnOviHwSetColorTemp)(LX_OVI_TCON_COLOR_TEMP_T *pstParams);
	int (*pfnOviHwSetGamma)(LX_OVI_TCON_GAMMA_T *pstParams);
	int (*pfnOviHwGetOdLut)(LX_OVI_TCON_OD_DWLD_T *pstParams);
	int (*pfnOviHwSetOdByteData)(LX_OVI_TCON_OD_LUT_DEBUG_T *pstParams);
	int (*pfnOviHwSetTconReverse)(UINT32 param);
	int (*pfnOviHwSetLvdsReverse)(UINT32 param);
	int (*pfnOviHwSetPatternGenerator)(UINT32 param);
	int (*pfnOviHwSetChannelPower)(UINT32 param);
	int (*pfnOviHwSetLvdsFormat)(LX_OVI_LVDS_OUT_STD_T enParam);
	int (*pfnOviHwSetLvdsBitDepth)(LX_OVI_BIT_DEPTH_T enParam);
	int (*pfnOviHwSetLvdsBlack)(UINT32 param);
	int (*pfnOviHwSetClock)(UINT32 param);
	int (*pfnOviHwDisplayResume)(LX_OVI_DISPLAY_INFO_T *pstParams);
} OVI_HW_FUNCTION_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
static OVI_HW_FUNCTION_T _gstOviHwFunction;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
* OVI Module Initialize
*
* @param void
* @return int
*/
int OVI_HAL_Init(void)
{
	int ret = RET_OK;

	if(0)
	{

	}
#ifdef INCLUDE_H15_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H15, A0) )
	{
		OVI_PRINT("OVI chip revision is set to H15 A0\n");
		_gstOviHwFunction.pfnOviHwInitialize    			= OVI_HW_H15_Initialize;
		_gstOviHwFunction.pfnOviHwResume	    			= OVI_HW_H15_Resume;
		_gstOviHwFunction.pfnOviHwSetCombination		= OVI_HW_H15_SetCombination;
		_gstOviHwFunction.pfnOviHwSetSpreadSpectrum 	= OVI_HW_H15_SetSpreadSpectrum;
		_gstOviHwFunction.pfnOviHwSetOdLut				= NULL;
		_gstOviHwFunction.pfnOviHwSetFrameOdcEnable		= NULL;
		_gstOviHwFunction.pfnOviHwSetColorTempEnable	= OVI_HW_H15_SetColorTempEnable;
		_gstOviHwFunction.pfnOviHwSetGammaEnable		= OVI_HW_H15_SetGammaEnable;
		_gstOviHwFunction.pfnOviHwSetDitherEnable		= NULL;
		_gstOviHwFunction.pfnOviHwSetLineOdcMode		= NULL;
		_gstOviHwFunction.pfnOviHwSetDitherMode			= NULL;
		_gstOviHwFunction.pfnOviHwSetColorTemp			= OVI_HW_H15_SetColorTemp;
		_gstOviHwFunction.pfnOviHwSetGamma			= OVI_HW_H15_SetGamma;
		_gstOviHwFunction.pfnOviHwGetOdLut				= NULL;
		_gstOviHwFunction.pfnOviHwSetOdByteData		= NULL;
		_gstOviHwFunction.pfnOviHwSetTconReverse		= NULL;
		_gstOviHwFunction.pfnOviHwSetLvdsReverse		= OVI_HW_H15_SetLvdsReverse;
		_gstOviHwFunction.pfnOviHwSetPatternGenerator	= NULL;
		_gstOviHwFunction.pfnOviHwSetChannelPower		= OVI_HW_H15_SetChannelPower;
		_gstOviHwFunction.pfnOviHwSetLvdsFormat			= OVI_HW_H15_SetLvdsFormat;
		_gstOviHwFunction.pfnOviHwSetLvdsBitDepth		= OVI_HW_H15_SetLvdsBitDepth;
		_gstOviHwFunction.pfnOviHwSetLvdsBlack			= OVI_HW_H15_SetLvdsBlack;
		_gstOviHwFunction.pfnOviHwSetClock			= NULL;
		_gstOviHwFunction.pfnOviHwDisplayResume			= OVI_HW_H15_DisplayResume;
		_gstOviHwFunction.pfnOviHwClose				= OVI_HW_H15_Close;
	}
#endif
#ifdef INCLUDE_H14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( H14, A0) )
	{
		OVI_PRINT("OVI chip revision is set to H14 A0\n");
		_gstOviHwFunction.pfnOviHwInitialize	    			= OVI_HW_H14_Initialize;
		_gstOviHwFunction.pfnOviHwResume	    			= OVI_HW_H14_Resume;
		_gstOviHwFunction.pfnOviHwSetCombination		= OVI_HW_H14_SetCombination;
		_gstOviHwFunction.pfnOviHwSetSpreadSpectrum 	= OVI_HW_H14_SetSpreadSpectrum;
		_gstOviHwFunction.pfnOviHwSetOdLut				= OVI_HW_H14_SetOdLut;
		_gstOviHwFunction.pfnOviHwSetFrameOdcEnable		= OVI_HW_H14_SetFrameOdcEnable;
		_gstOviHwFunction.pfnOviHwSetColorTempEnable	= OVI_HW_H14_SetColorTempEnable;
		_gstOviHwFunction.pfnOviHwSetGammaEnable		= OVI_HW_H14_SetGammaEnable;
		_gstOviHwFunction.pfnOviHwSetDitherEnable		= OVI_HW_H14_SetDitherEnable;
		_gstOviHwFunction.pfnOviHwSetLineOdcMode		= OVI_HW_H14_SetLineOdcMode;
		_gstOviHwFunction.pfnOviHwSetDitherMode			= OVI_HW_H14_SetDitherMode;
		_gstOviHwFunction.pfnOviHwSetColorTemp			= OVI_HW_H14_SetColorTemp;
		_gstOviHwFunction.pfnOviHwSetGamma			= OVI_HW_H14_SetGamma;
		_gstOviHwFunction.pfnOviHwGetOdLut				= OVI_HW_H14_GetOdLut;
		_gstOviHwFunction.pfnOviHwSetOdByteData		= OVI_HW_H14_SetOdByteData;
		_gstOviHwFunction.pfnOviHwSetTconReverse		= OVI_HW_H14_SetTconReverse;
		_gstOviHwFunction.pfnOviHwSetLvdsReverse		= OVI_HW_H14_SetLvdsReverse;
		_gstOviHwFunction.pfnOviHwSetPatternGenerator	= OVI_HW_H14_SetPatternGenerator;
		_gstOviHwFunction.pfnOviHwSetChannelPower		= OVI_HW_H14_SetChannelPower;
		_gstOviHwFunction.pfnOviHwSetLvdsFormat			= OVI_HW_H14_SetLvdsFormat;
		_gstOviHwFunction.pfnOviHwSetLvdsBitDepth		= OVI_HW_H14_SetLvdsBitDepth;
		_gstOviHwFunction.pfnOviHwSetLvdsBlack			= OVI_HW_H14_SetLvdsBlack;
		_gstOviHwFunction.pfnOviHwSetClock			= OVI_HW_H14_SetClock;
		_gstOviHwFunction.pfnOviHwDisplayResume			= NULL;
		_gstOviHwFunction.pfnOviHwClose				= OVI_HW_H14_Close;
	}
#endif
#ifdef INCLUDE_M14_CHIP_KDRV
	else if( lx_chip_rev( ) >= LX_CHIP_REV( M14, A0) )
	{
		_gstOviHwFunction.pfnOviHwInitialize	    			= OVI_HW_M14_Initialize;
		_gstOviHwFunction.pfnOviHwResume	    			= OVI_HW_M14_Resume;
		_gstOviHwFunction.pfnOviHwSetCombination		= OVI_HW_M14_SetCombination;
		_gstOviHwFunction.pfnOviHwSetSpreadSpectrum 	= OVI_HW_M14_SetSpreadSpectrum;
		_gstOviHwFunction.pfnOviHwSetOdLut				= OVI_HW_M14_SetOdLut;
		_gstOviHwFunction.pfnOviHwSetFrameOdcEnable		= OVI_HW_M14_SetFrameOdcEnable;
		_gstOviHwFunction.pfnOviHwSetColorTempEnable	= OVI_HW_M14_SetColorTempEnable;
		_gstOviHwFunction.pfnOviHwSetGammaEnable		= OVI_HW_M14_SetGammaEnable;
		_gstOviHwFunction.pfnOviHwSetDitherEnable		= OVI_HW_M14_SetDitherEnable;
		_gstOviHwFunction.pfnOviHwSetLineOdcMode		= OVI_HW_M14_SetLineOdcMode;
		_gstOviHwFunction.pfnOviHwSetDitherMode			= OVI_HW_M14_SetDitherMode;
		_gstOviHwFunction.pfnOviHwSetColorTemp			= OVI_HW_M14_SetColorTemp;
		_gstOviHwFunction.pfnOviHwSetGamma			= OVI_HW_M14_SetGamma;
		_gstOviHwFunction.pfnOviHwGetOdLut				= OVI_HW_M14_GetOdLut;
		_gstOviHwFunction.pfnOviHwSetOdByteData		= OVI_HW_M14_SetOdByteData;
		_gstOviHwFunction.pfnOviHwSetTconReverse		= OVI_HW_M14_SetTconReverse;
		_gstOviHwFunction.pfnOviHwSetLvdsReverse		= OVI_HW_M14_SetLvdsReverse;
		_gstOviHwFunction.pfnOviHwSetPatternGenerator	= OVI_HW_M14_SetPatternGenerator;
		_gstOviHwFunction.pfnOviHwSetChannelPower		= OVI_HW_M14_SetChannelPower;
		_gstOviHwFunction.pfnOviHwSetLvdsFormat			= OVI_HW_M14_SetLvdsFormat;
		_gstOviHwFunction.pfnOviHwSetLvdsBitDepth		= OVI_HW_M14_SetLvdsBitDepth;
		_gstOviHwFunction.pfnOviHwSetLvdsBlack			= OVI_HW_M14_SetLvdsBlack;
		_gstOviHwFunction.pfnOviHwSetClock			= OVI_HW_M14_SetClock;
		_gstOviHwFunction.pfnOviHwDisplayResume			= OVI_HW_M14_DisplayResume;
		_gstOviHwFunction.pfnOviHwClose				= OVI_HW_M14_Close;
	}
#endif
#ifdef INCLUDE_H13_CHIP_KDRV
	else if ( lx_chip_rev( ) >= LX_CHIP_REV( H13, B0) )
	{
		OVI_PRINT("OVI chip revision is set to H13 B0\n");
		_gstOviHwFunction.pfnOviHwInitialize	    			= OVI_HW_H13_Initialize;
		_gstOviHwFunction.pfnOviHwResume	    			= OVI_HW_H13_Resume;
		_gstOviHwFunction.pfnOviHwSetCombination		= OVI_HW_H13_SetCombination;
		_gstOviHwFunction.pfnOviHwSetSpreadSpectrum 	= OVI_HW_H13_SetSpreadSpectrum;
		_gstOviHwFunction.pfnOviHwSetOdLut				= OVI_HW_H13_SetOdLut;
		_gstOviHwFunction.pfnOviHwSetFrameOdcEnable		= OVI_HW_H13_SetFrameOdcEnable;
		_gstOviHwFunction.pfnOviHwSetColorTempEnable	= OVI_HW_H13_SetColorTempEnable;
		_gstOviHwFunction.pfnOviHwSetGammaEnable		= OVI_HW_H13_SetGammaEnable;
		_gstOviHwFunction.pfnOviHwSetDitherEnable		= OVI_HW_H13_SetDitherEnable;
		_gstOviHwFunction.pfnOviHwSetLineOdcMode		= OVI_HW_H13_SetLineOdcMode;
		_gstOviHwFunction.pfnOviHwSetDitherMode			= OVI_HW_H13_SetDitherMode;
		_gstOviHwFunction.pfnOviHwSetColorTemp			= OVI_HW_H13_SetColorTemp;
		_gstOviHwFunction.pfnOviHwSetGamma			= OVI_HW_H13_SetGamma;
		_gstOviHwFunction.pfnOviHwGetOdLut				= OVI_HW_H13_GetOdLut;
		_gstOviHwFunction.pfnOviHwSetOdByteData		= OVI_HW_H13_SetOdByteData;
		_gstOviHwFunction.pfnOviHwSetTconReverse		= OVI_HW_H13_SetTconReverse;
		_gstOviHwFunction.pfnOviHwSetLvdsReverse		= OVI_HW_H13_SetLvdsReverse;
		_gstOviHwFunction.pfnOviHwSetPatternGenerator	= OVI_HW_H13_SetPatternGenerator;
		_gstOviHwFunction.pfnOviHwSetChannelPower		= OVI_HW_H13_SetChannelPower;
		_gstOviHwFunction.pfnOviHwSetLvdsFormat			= OVI_HW_H13_SetLvdsFormat;
		_gstOviHwFunction.pfnOviHwSetLvdsBitDepth		= OVI_HW_H13_SetLvdsBitDepth;
		_gstOviHwFunction.pfnOviHwSetLvdsBlack			= OVI_HW_H13_SetLvdsBlack;
		_gstOviHwFunction.pfnOviHwSetClock			= OVI_HW_H13_SetClock;
		_gstOviHwFunction.pfnOviHwDisplayResume			= NULL;
		_gstOviHwFunction.pfnOviHwClose				= OVI_HW_H13_Close;
	}
#endif
	else
	{
		OVI_ERROR("OVI ERROR! Unknown chip revision at OVI module\n");
		_gstOviHwFunction.pfnOviHwInitialize    			= NULL;
		_gstOviHwFunction.pfnOviHwResume	    			= NULL;
		_gstOviHwFunction.pfnOviHwSetCombination		= NULL;
		_gstOviHwFunction.pfnOviHwSetSpreadSpectrum 	= NULL;
		_gstOviHwFunction.pfnOviHwSetOdLut				= NULL;
		_gstOviHwFunction.pfnOviHwSetFrameOdcEnable		= NULL;
		_gstOviHwFunction.pfnOviHwSetColorTempEnable	= NULL;
		_gstOviHwFunction.pfnOviHwSetGammaEnable		= NULL;
		_gstOviHwFunction.pfnOviHwSetDitherEnable		= NULL;
		_gstOviHwFunction.pfnOviHwSetLineOdcMode		= NULL;
		_gstOviHwFunction.pfnOviHwSetDitherMode			= NULL;
		_gstOviHwFunction.pfnOviHwSetColorTemp			= NULL;
		_gstOviHwFunction.pfnOviHwSetGamma			= NULL;
		_gstOviHwFunction.pfnOviHwGetOdLut				= NULL;
		_gstOviHwFunction.pfnOviHwSetOdByteData		= NULL;
		_gstOviHwFunction.pfnOviHwSetTconReverse		= NULL;
		_gstOviHwFunction.pfnOviHwSetLvdsReverse		= NULL;
		_gstOviHwFunction.pfnOviHwSetPatternGenerator	= NULL;
		_gstOviHwFunction.pfnOviHwSetChannelPower		= NULL;
		_gstOviHwFunction.pfnOviHwSetLvdsFormat			= NULL;
		_gstOviHwFunction.pfnOviHwSetLvdsBitDepth		= NULL;
		_gstOviHwFunction.pfnOviHwSetLvdsBlack			= NULL;
		_gstOviHwFunction.pfnOviHwSetClock			= NULL;
		_gstOviHwFunction.pfnOviHwDisplayResume			= NULL;
		_gstOviHwFunction.pfnOviHwClose				= NULL;
		ret = RET_ERROR;
	}

	if(_gstOviHwFunction.pfnOviHwInitialize)
	{
		ret = _gstOviHwFunction.pfnOviHwInitialize();
	}

	return ret;
}

int OVI_HAL_Resume(void)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwResume)
	{
		ret = _gstOviHwFunction.pfnOviHwResume();
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_GetDateInfo(void)
{
	int ret = RET_OK;

	printk("No more support!\n");

	return ret;
}

int OVI_HAL_SetCombination(UINT32 param)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetCombination)
	{
		ret = _gstOviHwFunction.pfnOviHwSetCombination(param);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetSpreadSpectrum(LX_OVI_SPREAD_SPECTRUM_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetSpreadSpectrum)
	{
		ret = _gstOviHwFunction.pfnOviHwSetSpreadSpectrum(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetOdLut)
	{
		ret = _gstOviHwFunction.pfnOviHwSetOdLut(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetFrameOdcEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetFrameOdcEnable)
	{
		ret = _gstOviHwFunction.pfnOviHwSetFrameOdcEnable(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetColorTempEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetColorTempEnable)
	{
		ret = _gstOviHwFunction.pfnOviHwSetColorTempEnable(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetGammaEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetGammaEnable)
	{
		ret = _gstOviHwFunction.pfnOviHwSetGammaEnable(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetDitherEnable(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetDitherEnable)
	{
		ret = _gstOviHwFunction.pfnOviHwSetDitherEnable(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetLineOdcMode(LX_OVI_TCON_LINEOD_MODE_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetLineOdcMode)
	{
		ret = _gstOviHwFunction.pfnOviHwSetLineOdcMode(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetDitherMode(LX_OVI_TCON_DITHER_T enParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetDitherMode)
	{
		ret = _gstOviHwFunction.pfnOviHwSetDitherMode(enParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetColorTemp(LX_OVI_TCON_COLOR_TEMP_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetColorTemp)
	{
		ret = _gstOviHwFunction.pfnOviHwSetColorTemp(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetGamma(LX_OVI_TCON_GAMMA_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetGamma)
	{
		ret = _gstOviHwFunction.pfnOviHwSetGamma(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_GetOdLut(LX_OVI_TCON_OD_DWLD_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwGetOdLut)
	{
		ret = _gstOviHwFunction.pfnOviHwGetOdLut(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetOdByteData(LX_OVI_TCON_OD_LUT_DEBUG_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetOdByteData)
	{
		ret = _gstOviHwFunction.pfnOviHwSetOdByteData(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetTconReverse(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetTconReverse)
	{
		ret = _gstOviHwFunction.pfnOviHwSetTconReverse(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetLvdsReverse(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetLvdsReverse)
	{
		ret = _gstOviHwFunction.pfnOviHwSetLvdsReverse(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetPatternGenerator(LX_OVI_TCON_TPG_MODE_T enParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetPatternGenerator)
	{
		ret = _gstOviHwFunction.pfnOviHwSetPatternGenerator(enParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetChannelPower(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetChannelPower)
	{
		ret = _gstOviHwFunction.pfnOviHwSetChannelPower(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetLvdsFormat(LX_OVI_LVDS_OUT_STD_T enParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetLvdsFormat)
	{
		ret = _gstOviHwFunction.pfnOviHwSetLvdsFormat(enParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetLvdsBitDepth(LX_OVI_BIT_DEPTH_T enParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetLvdsBitDepth)
	{
		ret = _gstOviHwFunction.pfnOviHwSetLvdsBitDepth(enParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetLvdsBlack(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetLvdsBlack)
	{
		ret = _gstOviHwFunction.pfnOviHwSetLvdsBlack(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_SetClock(BOOLEAN bParam)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwSetClock)
	{
		ret = _gstOviHwFunction.pfnOviHwSetClock(bParam);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

int OVI_HAL_DisplayResume(LX_OVI_DISPLAY_INFO_T *pstParams)
{
	int ret = RET_OK;

	if(_gstOviHwFunction.pfnOviHwDisplayResume)
	{
		ret = _gstOviHwFunction.pfnOviHwDisplayResume(pstParams);
	}
	else
	{
		OVI_ERROR("OVI ERROR! This function is not supported!\n");
		ret = RET_ERROR;
	}

	return ret;
}

/** @} */
