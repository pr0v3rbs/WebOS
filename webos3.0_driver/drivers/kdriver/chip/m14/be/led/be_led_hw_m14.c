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
#include "../fwi/ipc_def_m14.h"
#include "ipc_drv.h"
#include "be_led_hw_m14.h"
#include "../reg/be_reg_def_m14.h"
#include "../reg/be_reg_dppb_m14.h"

// For CTOP register access
#include "../../../core/sys/sys_regs.h"

#include "ipc_cmd_header.h"

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
} BE_LED_HDR_HW_M14_INFO_T;

typedef struct {
	UINT32	uWCGMode;
	UINT32	uWCGControl;
} BE_LED_WCG_HW_M14_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern DPPB_LED_REG_M14_T gDPPB_LED_M14;
extern PE1_HDR_REG_M14_T gPE1_HDR_M14B;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_LED_HDR_HW_M14_INFO_T _beHDRHwM14Info = {0,};
static BE_LED_WCG_HW_M14_INFO_T _beWCGHwM14Info = {0xFF,0xFF};

/*========================================================================================
	Implementation Group
========================================================================================*/

int BE_LED_HW_M14_Initialize(void)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Init for smart black */
			DPPB_LED_M14B0_RdFL(led_lut_offset_en);
			DPPB_LED_M14B0_Wr01(led_lut_offset_en, bv_offset_en, 0x3);
			DPPB_LED_M14B0_WrFL(led_lut_offset_en);

			DPPB_LED_M14B0_RdFL(led_ld_bv_ext_max);
			DPPB_LED_M14B0_Wr01(led_ld_bv_ext_max, bv_ext_max, 0x0);
			DPPB_LED_M14B0_WrFL(led_ld_bv_ext_max);

			DPPB_LED_M14B0_RdFL(led_lut_offset_x1_x2);
			DPPB_LED_M14B0_Wr01(led_lut_offset_x1_x2, bv_offset_x1, 200);
			DPPB_LED_M14B0_Wr01(led_lut_offset_x1_x2, bv_offset_x2, 400);
			DPPB_LED_M14B0_WrFL(led_lut_offset_x1_x2);

			DPPB_LED_M14B0_RdFL(led_lut_offset_x3_x4);
			DPPB_LED_M14B0_Wr01(led_lut_offset_x3_x4, bv_offset_x3, 600);
			DPPB_LED_M14B0_Wr01(led_lut_offset_x3_x4, bv_offset_x4, 800);
			DPPB_LED_M14B0_WrFL(led_lut_offset_x3_x4);

			DPPB_LED_M14B0_RdFL(led_lut_offset_y0_y1);
			DPPB_LED_M14B0_Wr01(led_lut_offset_y0_y1, bv_offset_y0, 0);
			DPPB_LED_M14B0_Wr01(led_lut_offset_y0_y1, bv_offset_y1, 200);
			DPPB_LED_M14B0_WrFL(led_lut_offset_y0_y1);

			DPPB_LED_M14B0_RdFL(led_lut_offset_y2_y3);
			DPPB_LED_M14B0_Wr01(led_lut_offset_y2_y3, bv_offset_y2, 400);
			DPPB_LED_M14B0_Wr01(led_lut_offset_y2_y3, bv_offset_y3, 600);
			DPPB_LED_M14B0_WrFL(led_lut_offset_y2_y3);

			DPPB_LED_M14B0_RdFL(led_lut_offset_y4_y5);
			DPPB_LED_M14B0_Wr01(led_lut_offset_y4_y5, bv_offset_y4, 800);
			DPPB_LED_M14B0_Wr01(led_lut_offset_y4_y5, bv_offset_y5, 1023);
			DPPB_LED_M14B0_WrFL(led_lut_offset_y4_y5);
		}
		else
		{

		}
	} while (0);

	return ret;
}

int BE_LED_HW_M14_Close(void)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{

	}
	else
	{

	}

	return ret;
}

int BE_LED_HW_M14_Resume(void)
{
	int ret = RET_OK;

	if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
	{
		/* Init for smart black */
		DPPB_LED_M14B0_RdFL(led_lut_offset_en);
		DPPB_LED_M14B0_Wr01(led_lut_offset_en, bv_offset_en, 0x3);
		DPPB_LED_M14B0_WrFL(led_lut_offset_en);

		DPPB_LED_M14B0_RdFL(led_ld_bv_ext_max);
		DPPB_LED_M14B0_Wr01(led_ld_bv_ext_max, bv_ext_max, 0x0);
		DPPB_LED_M14B0_WrFL(led_ld_bv_ext_max);

		DPPB_LED_M14B0_RdFL(led_lut_offset_x1_x2);
		DPPB_LED_M14B0_Wr01(led_lut_offset_x1_x2, bv_offset_x1, 200);
		DPPB_LED_M14B0_Wr01(led_lut_offset_x1_x2, bv_offset_x2, 400);
		DPPB_LED_M14B0_WrFL(led_lut_offset_x1_x2);

		DPPB_LED_M14B0_RdFL(led_lut_offset_x3_x4);
		DPPB_LED_M14B0_Wr01(led_lut_offset_x3_x4, bv_offset_x3, 600);
		DPPB_LED_M14B0_Wr01(led_lut_offset_x3_x4, bv_offset_x4, 800);
		DPPB_LED_M14B0_WrFL(led_lut_offset_x3_x4);

		DPPB_LED_M14B0_RdFL(led_lut_offset_y0_y1);
		DPPB_LED_M14B0_Wr01(led_lut_offset_y0_y1, bv_offset_y0, 0);
		DPPB_LED_M14B0_Wr01(led_lut_offset_y0_y1, bv_offset_y1, 200);
		DPPB_LED_M14B0_WrFL(led_lut_offset_y0_y1);

		DPPB_LED_M14B0_RdFL(led_lut_offset_y2_y3);
		DPPB_LED_M14B0_Wr01(led_lut_offset_y2_y3, bv_offset_y2, 400);
		DPPB_LED_M14B0_Wr01(led_lut_offset_y2_y3, bv_offset_y3, 600);
		DPPB_LED_M14B0_WrFL(led_lut_offset_y2_y3);

		DPPB_LED_M14B0_RdFL(led_lut_offset_y4_y5);
		DPPB_LED_M14B0_Wr01(led_lut_offset_y4_y5, bv_offset_y4, 800);
		DPPB_LED_M14B0_Wr01(led_lut_offset_y4_y5, bv_offset_y5, 1023);
		DPPB_LED_M14B0_WrFL(led_lut_offset_y4_y5);
	}

	return ret;
}

int BE_LED_HW_M14_Get_Version(LX_BE_LED_VERSION_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_LED_VERSION_T, stHwParams);

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
		}
	} while (0);

	return ret;
}

int BE_LED_HW_M14_Set_Mode(LX_BE_LED_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_LED_MODE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uLEDMode	= pstParams->u32PictureMode;

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_LED_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_LED_MODE_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_LED_MODE_T, stHwParams);

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
		}
	} while (0);

	return ret;
}

int BE_LED_HW_M14_Control(LX_BE_LED_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_LED_CONTROL_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bLEDControl	= pstParams->u16DimminOn == 0xFFFF ? 0xFFFFFFFF : pstParams->u16DimminOn;

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_LED_B0_CONTROL), &stHwParams, sizeof(BE_IPC_B0_LED_CONTROL_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_LED_CONTROL_T, stHwParams);

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
		}
	} while (0);

	return ret;
}

int BE_LED_HW_M14_Get_Apl(LX_BE_LED_APL_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_NULL(pstParams);

		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			DPPB_LED_M14B0_RdFL(led_bv_gen_bv_max);
			DPPB_LED_M14B0_Rd01(led_bv_gen_bv_max, bvgen_min, pstParams->u16Min);

			DPPB_LED_M14B0_RdFL(led_bv_min_max);
			DPPB_LED_M14B0_Rd01(led_bv_min_max, max_bv, pstParams->u16Max);
		}
		else
		{
			/* Do Nothing */
		}
	} while (0);

	return ret;
}

int BE_LED_HW_M14_Set_ClippingValue(UINT32 u32Param)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			DPPB_LED_M14B0_RdFL(led_ld_bv_ext_max);
			DPPB_LED_M14B0_Wr01(led_ld_bv_ext_max, bv_ext_max, (u32Param & 0x3FF));
			DPPB_LED_M14B0_WrFL(led_ld_bv_ext_max);
		}
		else
		{
			/* Do Nothing */
		}
	} while (0);

	return ret;
}

int BE_LED_HW_M14_Get_RegInfo(LX_BE_LED_REG_INFO_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_NULL(pstParams);

		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			pstParams->u32CpuBaseAddr = DPPB_LED_REG_M14B0_BASE;
			pstParams->u32McuBaseAddr = DPPB_LED_REG_M14B0_BASE;
			pstParams->regOffset.AD_MEM_ADDR_CLR = offsetof(DPPB_LED_REG_M14B0_T, led_mem_addr_clr);
			pstParams->regOffset.AD_MEM_DL_SBI_HCOEFF = offsetof(DPPB_LED_REG_M14B0_T, led_mem_dl_sbi_hcoeff);
			pstParams->regOffset.AD_MEM_DL_SBI_VCOEFF = offsetof(DPPB_LED_REG_M14B0_T, led_mem_dl_sbi_vcoeff);
			pstParams->regOffset.AD_FC_TAB1 = offsetof(DPPB_LED_REG_M14B0_T, led_fc_tab1);
			pstParams->regOffset.AD_FC_TAB5 = offsetof(DPPB_LED_REG_M14B0_T, led_fc_tab5);
			pstParams->regOffset.AD_UPPER_DC_LUT = offsetof(DPPB_LED_REG_M14B0_T, led_upper_dc_lut);
			pstParams->regOffset.AD_LOWER_DC_LUT = offsetof(DPPB_LED_REG_M14B0_T, led_lower_dc_lut);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_A = offsetof(DPPB_LED_REG_M14B0_T, led_mem_dl_pixel_comp_a);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_B = offsetof(DPPB_LED_REG_M14B0_T, led_mem_dl_pixel_comp_b);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_C = offsetof(DPPB_LED_REG_M14B0_T, led_mem_dl_pixel_comp_c);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_D = offsetof(DPPB_LED_REG_M14B0_T, led_mem_dl_pixel_comp_d);
			pstParams->regOffset.AD_UMASK_WR_COEFF = offsetof(DPPB_LED_REG_M14B0_T, led_umask_wr_coeff);
		}
		else
		{
			pstParams->u32CpuBaseAddr = DPPB_LED_REG_M14A0_BASE;
			pstParams->u32McuBaseAddr = DPPB_LED_REG_M14A0_BASE + 0x2FFE0000;
			pstParams->regOffset.AD_MEM_ADDR_CLR = offsetof(DPPB_LED_REG_M14A0_T, led_mem_addr_clr);
			pstParams->regOffset.AD_MEM_DL_SBI_HCOEFF = offsetof(DPPB_LED_REG_M14A0_T, led_mem_dl_sbi_hcoeff);
			pstParams->regOffset.AD_MEM_DL_SBI_VCOEFF = offsetof(DPPB_LED_REG_M14A0_T, led_mem_dl_sbi_vcoeff);
			pstParams->regOffset.AD_FC_TAB1 = offsetof(DPPB_LED_REG_M14A0_T, led_fc_tab1);
			pstParams->regOffset.AD_FC_TAB5 = offsetof(DPPB_LED_REG_M14A0_T, led_fc_tab5);
			pstParams->regOffset.AD_UPPER_DC_LUT = offsetof(DPPB_LED_REG_M14A0_T, led_upper_dc_lut);
			pstParams->regOffset.AD_LOWER_DC_LUT = offsetof(DPPB_LED_REG_M14A0_T, led_lower_dc_lut);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_A = offsetof(DPPB_LED_REG_M14A0_T, led_mem_dl_pixel_comp_a);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_B = offsetof(DPPB_LED_REG_M14A0_T, led_mem_dl_pixel_comp_b);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_C = offsetof(DPPB_LED_REG_M14A0_T, led_mem_dl_pixel_comp_c);
			pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_D = offsetof(DPPB_LED_REG_M14A0_T, led_mem_dl_pixel_comp_d);
			pstParams->regOffset.AD_UMASK_WR_COEFF = offsetof(DPPB_LED_REG_M14A0_T, led_umask_wr_coeff);
		}
	} while (0);

	return ret;
}

int BE_HDR_HW_M14_Get_Version(LX_BE_HDR_VERSION_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_HDR_VERSION_T, stHwParams);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_HDR_GET_VERSION, &stHwParams, sizeof(BE_IPC_HDR_VERSION_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_HDR_VERSION_T));
			if (ret) BREAK_WRONG(ret);

			/* change value BE_IPC to BE_IPCLX_BE */
			pstParams->u32HDRLutVersion	= stHwParams.u32HDRLutVersion;
		}
	} while (0);

	return ret;
}

int BE_HDR_HW_M14_Set_Mode(LX_BE_HDR_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_HDR_MODE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uHDRMode	= pstParams->u8DCMode	== 0xFF ? 0xFFFFFFFF : pstParams->u8DCMode;

			/* Set value for argument */
			if ((stHwParams.uHDRMode != 0xFFFFFFFF) && (_beHDRHwM14Info.u8DCMode != stHwParams.uHDRMode))
				_beHDRHwM14Info.u8DCMode	= (UINT8)stHwParams.uHDRMode;
			else
				stHwParams.uHDRMode = 0xFFFFFFFF;

			if ((pstParams->u8Store != 0xFF) && (_beHDRHwM14Info.u8Store !=pstParams->u8Store ))
				_beHDRHwM14Info.u8Store 	= pstParams->u8Store;
			else
				pstParams->u8Store = 0xFF;

			if ((pstParams->u8DRC_shp != 0xFF) && (_beHDRHwM14Info.u8DRC_shp != pstParams->u8DRC_shp))
				_beHDRHwM14Info.u8DRC_shp = pstParams->u8DRC_shp;
			else
				pstParams->u8DRC_shp = 0xFF;

			/* HDR bypass control */
			if (_beHDRHwM14Info.u8DCBypass)
			{
				if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_OFF)
				{
					_beHDRHwM14Info.u8DCBypass = FALSE;

					stHwParams.uHDRMode = (UINT32)_beHDRHwM14Info.u8DCMode;
					pstParams->u8Store 		= _beHDRHwM14Info.u8Store;
					pstParams->u8DRC_shp 	= _beHDRHwM14Info.u8DRC_shp;
				}
				else
				{
					return RET_OK;
				}
			}
			else
			{
				if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_ON)
					_beHDRHwM14Info.u8DCBypass = TRUE;

				if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_INTERNAL_OFF)
					pstParams->u8DCBypass = FALSE;
				else if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_INTERNAL_ON)
					pstParams->u8DCBypass = TRUE;
			}

			if (pstParams->u8Store != 0xFF)
			{
				PE1_HDR_M14B0_RdFL(pe1_fsw_ctrl_01);
				PE1_HDR_M14B0_Wr01(pe1_fsw_ctrl_01, reg_drc_store_mode, pstParams->u8Store ? 0x1:0x0);
				PE1_HDR_M14B0_WrFL(pe1_fsw_ctrl_01);
			}

			if (pstParams->u8DRC_shp != 0xFF)
			{
				PE1_HDR_M14B0_RdFL(pe1_fsw_ctrl_00);
				PE1_HDR_M14B0_Wr01(pe1_fsw_ctrl_00, reg_drc_use_shp, pstParams->u8DRC_shp ? 0x1:0x0);
				PE1_HDR_M14B0_WrFL(pe1_fsw_ctrl_00);
			}

			if (pstParams->u8DCBypass != 0xFF)
			{
				PE1_HDR_M14B0_RdFL(pe1_fsw_ctrl_01);
				PE1_HDR_M14B0_Wr01(pe1_fsw_ctrl_01, reg_drc_dc_bypass, pstParams->u8DCBypass ? 0x1:0x0);
				PE1_HDR_M14B0_WrFL(pe1_fsw_ctrl_01);
			}

			if (pstParams->u8DCsc != 0xFF)
			{
				PE1_HDR_M14B0_RdFL(pe1_fsw_ctrl_01);
				PE1_HDR_M14B0_Wr01(pe1_fsw_ctrl_01, reg_drc_dc_sc, pstParams->u8DCsc ? 0x1:0x0);
				PE1_HDR_M14B0_WrFL(pe1_fsw_ctrl_01);
			}

			/* return unexpected value */
			if ( stHwParams.uHDRMode == 0xFFFFFFFF )
			{
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_HDR_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_HDR_MODE_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_HDR_MODE_T, stHwParams);

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
				_beHDRHwM14Info.u8DECMode = (UINT8)stHwParams.u4DECMode;

			if (stHwParams.u4DCMode != 0xF)
				_beHDRHwM14Info.u8DCMode	= (UINT8)stHwParams.u4DCMode;

			if (stHwParams.bStore != 0xF)
				_beHDRHwM14Info.u8Store 	= (UINT8)stHwParams.bStore;

			if (stHwParams.bOSD != 0xF)
				_beHDRHwM14Info.u8OSD 	= (UINT8)stHwParams.bOSD;

			if (stHwParams.bDRC_shp != 0xF)
				_beHDRHwM14Info.u8DRC_shp = (UINT8)stHwParams.bDRC_shp;

			if (_beHDRHwM14Info.u8DCBypass)
			{
				if (stHwParams.bDCBypass == LX_BE_HDR_DCR_BYPASS_OFF)
				{
					_beHDRHwM14Info.u8DCBypass = FALSE;

					stHwParams.u4DECMode= _beHDRHwM14Info.u8DECMode;
					stHwParams.u4DCMode	= _beHDRHwM14Info.u8DCMode ;
					stHwParams.bStore 	= _beHDRHwM14Info.u8Store;
					stHwParams.bOSD 		= _beHDRHwM14Info.u8OSD;
					stHwParams.bDRC_shp 	= _beHDRHwM14Info.u8DRC_shp;
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
					_beHDRHwM14Info.u8DCBypass = TRUE;

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
		}
	} while (0);

	return ret;
}

int BE_HDR_HW_M14_Control(LX_BE_HDR_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_HDR_CONTROL_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uHDRControl = pstParams->u16DRCEnable == 0xFFFF ? 0xFFFFFFFF : pstParams->u16DRCEnable;


			/* return unexpected value */
			if ( stHwParams.uHDRControl == 0xFFFFFFFF )
			{
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_HDR_B0_CONTROL), &stHwParams, sizeof(BE_IPC_B0_HDR_CONTROL_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_HDR_CONTROL_T, stHwParams);

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
		}
	} while (0);

	return ret;
}

int BE_WCG_HW_M14_Get_Version(LX_BE_WCG_VERSION_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_WCG_VERSION_T, stHwParams);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_WCG_GET_VERSION, &stHwParams, sizeof(BE_IPC_WCG_VERSION_T));
			if (ret) BREAK_WRONG(ret);
			ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_WCG_VERSION_T));
			if (ret) BREAK_WRONG(ret);

			/* change value BE_IPC to BE_IPCLX_BE */
			pstParams->u32WCGLutVersion	= stHwParams.u32WCGLutVersion;
		}
	} while (0);

	return ret;
}

int BE_WCG_HW_M14_Set_Mode(LX_BE_WCG_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_WCG_MODE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uWCGMode = pstParams->u32PictureMode;

			if ( (stHwParams.uWCGMode != 0xFFFFFFFF) && (_beWCGHwM14Info.uWCGMode != stHwParams.uWCGMode) )
				_beWCGHwM14Info.uWCGMode = stHwParams.uWCGMode;
			else
				stHwParams.uWCGMode = 0xFFFFFFFF;

			if ( stHwParams.uWCGMode == 0xFFFFFFFF )
			{
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_WCG_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_WCG_MODE_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_WCG_MODE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u4PictureMode = pstParams->u32PictureMode;

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_WCG_SET_MODE, &stHwParams, sizeof(BE_IPC_WCG_MODE_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

int BE_WCG_HW_M14_Control(LX_BE_WCG_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	do {
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_WCG_CONTROL_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uWCGControl	= pstParams->u32Enable;

			if ((stHwParams.uWCGControl != 0xFFFFFFFF) && (_beWCGHwM14Info.uWCGControl != stHwParams.uWCGControl))
				_beWCGHwM14Info.uWCGControl = stHwParams.uWCGControl;
			else
				stHwParams.uWCGControl = 0xFFFFFFFF;

			if ( stHwParams.uWCGControl == 0xFFFFFFFF )
			{
				return ret;
			}

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_WCG_B0_CONTROL), &stHwParams, sizeof(BE_IPC_B0_WCG_CONTROL_T));
			if (ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_WCG_CONTROL_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bEnable	= pstParams->u32Enable;

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_WCG_CONTROL, &stHwParams, sizeof(BE_IPC_WCG_CONTROL_T));
			if (ret) BREAK_WRONG(ret);
		}
	} while (0);

	return ret;
}

/** @} */
