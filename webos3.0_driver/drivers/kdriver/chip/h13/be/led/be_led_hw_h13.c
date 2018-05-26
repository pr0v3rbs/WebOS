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
 *	BE LED module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
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
#include <linux/fcntl.h>		/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>	/**< For request_region, check_region etc */
#include <asm/io.h>		/**< For ioremap_nocache */
#include "os_util.h"

#include "be_dbg.h"
#include "be_def.h"
#include "../fwi/ipc_def_h13.h"
#include "ipc_drv.h"
#include "be_led_hw_h13.h"
#include "../reg/be_reg_def_h13.h"
#include "../reg/be_reg_dppb_h13.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	UINT8	u8DECMode;
	UINT8	u8DCMode;
	UINT8	u8DCBypass;
	UINT8	u8DCsc;
	UINT8	u8OSD;
	UINT8	u8Store;
	UINT8	u8DRC_shp;
	UINT8	_rsvd;
} BE_LED_HDR_HW_H13_INFO_T;

typedef struct {
	UINT32	uWCGMode;
	UINT32	uWCGControl;
} BE_LED_WCG_HW_H13_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_LED_HDR_HW_H13_INFO_T _beHDRHwH13Info = {0,};
static BE_LED_WCG_HW_H13_INFO_T _beWCGHwH13Info = {0xFF,0xFF};

/*========================================================================================
	Implementation Group
========================================================================================*/

int BE_LED_HW_H13_Initialize(void)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( H13, B0))
		{

		}
		else
		{

		}
	} while (0);

	return ret;
}

int BE_LED_HW_H13_Close(void)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( H13, B0))
	{

	}
	else
	{

	}

	return ret;
}

int BE_LED_HW_H13_Resume(void)
{
	int ret = RET_OK;

	return ret;
}

int BE_LED_HW_H13_Get_Version(LX_BE_LED_VERSION_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_LED_VERSION_T, stHwParams);

	do {
		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_LED_GET_VERSION, &stHwParams, sizeof(BE_IPC_LED_VERSION_T));
		if (ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_LED_VERSION_T));
		if (ret) BREAK_WRONG(ret);

		/* change value BE_IPC to BE_IPCLX_BE */
		pstParams->u8LEDLutVersion	= stHwParams.u8LEDLutVersion;
		pstParams->u8LEDSubVersion	= stHwParams.u8LEDSubVersion;
		pstParams->u8PanelSize		= stHwParams.u4PanelSize;
		pstParams->u8PanelType		= stHwParams.u4PanelType;
	} while (0);

	return ret;
}

int BE_LED_HW_H13_Set_Mode(LX_BE_LED_MODE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_LED_MODE_T, stHwParams);

	do {
		/* change value LX_BE to BE_IPC */
		stHwParams.u4PictureMode	= pstParams->u32PictureMode;

		/* return unexpected value */
		if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_LED_SET_MODE, &stHwParams, sizeof(BE_IPC_LED_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_HW_H13_Control(LX_BE_LED_CONTROL_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_LED_CONTROL_T, stHwParams);

	do {
		/* change value LX_BE to BE_IPC */
		stHwParams.u4DimminOn	= pstParams->u16DimminOn;
		stHwParams.bStoreMode	= pstParams->u16StoreMode;

		/* return unexpected value */
		if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_LED_CONTROL, &stHwParams, sizeof(BE_IPC_LED_CONTROL_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_HW_H13_Get_RegInfo(LX_BE_LED_REG_INFO_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_NULL(pstParams);

		pstParams->u32CpuBaseAddr = DPPB_LED_REG_H13_BASE;
		pstParams->u32McuBaseAddr = DPPB_LED_REG_H13_BASE + 0x2FFE0000;
		pstParams->regOffset.AD_MEM_ADDR_CLR = offsetof(DPPB_LED_REG_H13A0_T, led_mem_addr_clr);
		pstParams->regOffset.AD_MEM_DL_SBI_HCOEFF = offsetof(DPPB_LED_REG_H13A0_T, led_mem_dl_sbi_hcoeff);
		pstParams->regOffset.AD_MEM_DL_SBI_VCOEFF = offsetof(DPPB_LED_REG_H13A0_T, led_mem_dl_sbi_vcoeff);
		pstParams->regOffset.AD_FC_TAB1 = offsetof(DPPB_LED_REG_H13A0_T, led_fc_tab1);
		pstParams->regOffset.AD_FC_TAB5 = offsetof(DPPB_LED_REG_H13A0_T, led_fc_tab5);
		pstParams->regOffset.AD_UPPER_DC_LUT = offsetof(DPPB_LED_REG_H13A0_T, led_upper_dc_lut);
		pstParams->regOffset.AD_LOWER_DC_LUT = offsetof(DPPB_LED_REG_H13A0_T, led_lower_dc_lut);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_A = offsetof(DPPB_LED_REG_H13A0_T, led_mem_dl_pixel_comp_a);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_B = offsetof(DPPB_LED_REG_H13A0_T, led_mem_dl_pixel_comp_b);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_C = offsetof(DPPB_LED_REG_H13A0_T, led_mem_dl_pixel_comp_c);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_D = offsetof(DPPB_LED_REG_H13A0_T, led_mem_dl_pixel_comp_d);
		pstParams->regOffset.AD_UMASK_WR_COEFF = offsetof(DPPB_LED_REG_H13A0_T, led_lsf_wr_coeff);
	} while (0);

	return ret;
}

int BE_HDR_HW_H13_Get_Version(LX_BE_HDR_VERSION_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_HDR_VERSION_T, stHwParams);

	do {
		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_HDR_GET_VERSION, &stHwParams, sizeof(BE_IPC_HDR_VERSION_T));
		if (ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_HDR_VERSION_T));
		if (ret) BREAK_WRONG(ret);

		/* change value BE_IPC to BE_IPCLX_BE */
		pstParams->u32HDRLutVersion	= stHwParams.u32HDRLutVersion;
	} while (0);

	return ret;
}

int BE_HDR_HW_H13_Set_Mode(LX_BE_HDR_MODE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_HDR_MODE_T, stHwParams);

	do {
		/* change value LX_BE to BE_IPC */
		stHwParams.u4DECMode= pstParams->u8DECMode;
		stHwParams.u4DCMode	= pstParams->u8DCMode;
		stHwParams.bDCBypass	= pstParams->u8DCBypass;
		stHwParams.bDCsc		= pstParams->u8DCsc;
		stHwParams.bOSD		= pstParams->u8OSD;
		stHwParams.bStore	= pstParams->u8Store;
		stHwParams.bDRC_shp	= pstParams->u8DRC_shp;

		/* Set value for argument */
		if (stHwParams.u4DECMode != 0xF)
			_beHDRHwH13Info.u8DECMode = (UINT8)stHwParams.u4DECMode;

		if (stHwParams.u4DCMode != 0xF)
			_beHDRHwH13Info.u8DCMode	= (UINT8)stHwParams.u4DCMode;

		if (stHwParams.bStore != 0xF)
			_beHDRHwH13Info.u8Store 	= (UINT8)stHwParams.bStore;

		if (stHwParams.bOSD != 0xF)
			_beHDRHwH13Info.u8OSD 	= (UINT8)stHwParams.bOSD;

		if (stHwParams.bDRC_shp != 0xF)
			_beHDRHwH13Info.u8DRC_shp = (UINT8)stHwParams.bDRC_shp;

		if (_beHDRHwH13Info.u8DCBypass)
		{
			if (stHwParams.bDCBypass == LX_BE_HDR_DCR_BYPASS_OFF)
			{
				_beHDRHwH13Info.u8DCBypass = FALSE;

				stHwParams.u4DECMode= _beHDRHwH13Info.u8DECMode;
				stHwParams.u4DCMode	= _beHDRHwH13Info.u8DCMode ;
				stHwParams.bStore 	= _beHDRHwH13Info.u8Store;
				stHwParams.bOSD 		= _beHDRHwH13Info.u8OSD;
				stHwParams.bDRC_shp 	= _beHDRHwH13Info.u8DRC_shp;
			}
			else
			{
				if (stHwParams.bOSD == 0xF)
					return RET_OK;
			}
		}
		else
		{
			if (stHwParams.bDCBypass == LX_BE_HDR_DCR_BYPASS_ON)
				_beHDRHwH13Info.u8DCBypass = TRUE;

			if (stHwParams.bDCBypass == LX_BE_HDR_DCR_BYPASS_INTERNAL_OFF)
				stHwParams.bDCBypass = FALSE;
			else if (stHwParams.bDCBypass == LX_BE_HDR_DCR_BYPASS_INTERNAL_ON)
				stHwParams.bDCBypass = TRUE;
		}

		/* return unexpected value */
		if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_HDR_SET_MODE, &stHwParams, sizeof(BE_IPC_HDR_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_HDR_HW_H13_Control(LX_BE_HDR_CONTROL_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_HDR_CONTROL_T, stHwParams);

	do {
		/* change value LX_BE to BE_IPC */
		stHwParams.bDECEnable	= pstParams->u16DECEnable;
		stHwParams.bDRCEnable	= pstParams->u16DRCEnable;

		/* return unexpected value */
		if (*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_HDR_CONTROL, &stHwParams, sizeof(BE_IPC_HDR_CONTROL_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_WCG_HW_H13_Get_Version(LX_BE_WCG_VERSION_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_WCG_VERSION_T, stHwParams);

	do {
		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_WCG_GET_VERSION, &stHwParams, sizeof(BE_IPC_WCG_VERSION_T));
		if (ret) BREAK_WRONG(ret);
		ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_WCG_VERSION_T));
		if (ret) BREAK_WRONG(ret);

		/* change value BE_IPC to BE_IPCLX_BE */
		pstParams->u32WCGLutVersion	= stHwParams.u32WCGLutVersion;
	} while (0);

	return ret;
}

int BE_WCG_HW_H13_Set_Mode(LX_BE_WCG_MODE_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_WCG_MODE_T, stHwParams);

	do {
		/* change value LX_BE to BE_IPC */
		stHwParams.u4PictureMode = pstParams->u32PictureMode;

		if ( (stHwParams.u4PictureMode != 0xF) && (_beWCGHwH13Info.uWCGMode != pstParams->u32PictureMode) )
			_beWCGHwH13Info.uWCGMode = pstParams->u32PictureMode;
		else
			stHwParams.u4PictureMode = 0xF;

		if ( stHwParams.u4PictureMode == 0xF )
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_WCG_SET_MODE, &stHwParams, sizeof(BE_IPC_WCG_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_WCG_HW_H13_Control(LX_BE_WCG_CONTROL_T *pstParams)
{
	int ret = RET_OK;
	IPC_SET_GET_PARAM(BE_IPC_WCG_CONTROL_T, stHwParams);

	do {
		/* change value LX_BE to BE_IPC */
		stHwParams.bEnable	= pstParams->u32Enable;

		if ( (stHwParams.bEnable != 0xF) && (_beWCGHwH13Info.uWCGControl != pstParams->u32Enable) )
			_beWCGHwH13Info.uWCGControl = pstParams->u32Enable;
		else
			stHwParams.bEnable = 0xF;

		if ( stHwParams.bEnable == 0xF )
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(BE_WCG_CONTROL, &stHwParams, sizeof(BE_IPC_WCG_CONTROL_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

/** @} */
