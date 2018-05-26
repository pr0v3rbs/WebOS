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
#include "../fwi/ipc_def_h15.h"
#include "ipc_drv.h"
#include "be_led_hw_h15.h"
#include "../reg/be_reg_def_h15.h"
#include "../reg/be_reg_dpe_h15.h"

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
} BE_LED_DRC_HW_H15_INFO_T;

typedef struct {
	UINT32	uWCGMode;
	UINT32	uWCGEn;
} BE_LED_WCG_HW_H15_INFO_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern DPE_LED_REG_H15_T gDPE_LED_H15;
extern PE1_DRC_REG_H15_T gPE1_DRC_H15;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static BE_LED_DRC_HW_H15_INFO_T _beDRCHwH15Info = {0,};
static BE_LED_WCG_HW_H15_INFO_T _beWCGHwH15Info = {0xFF,0xFF};

/*========================================================================================
	Implementation Group
========================================================================================*/

int BE_LED_HW_H15_Initialize(void)
{
	int ret = RET_OK;

	do {
		/* Init for smart black */
		DPE_LED_H15_RdFL(led_lut_offset_en);
		DPE_LED_H15_Wr01(led_lut_offset_en, bv_offset_en, 0x3);
		DPE_LED_H15_WrFL(led_lut_offset_en);

		DPE_LED_H15_RdFL(led_ld_bv_ext_max);
		DPE_LED_H15_Wr01(led_ld_bv_ext_max, bv_ext_max, 0x0);
		DPE_LED_H15_WrFL(led_ld_bv_ext_max);

		DPE_LED_H15_RdFL(led_lut_offset_x1_x2);
		DPE_LED_H15_Wr01(led_lut_offset_x1_x2, bv_offset_x1, 200);
		DPE_LED_H15_Wr01(led_lut_offset_x1_x2, bv_offset_x2, 400);
		DPE_LED_H15_WrFL(led_lut_offset_x1_x2);

		DPE_LED_H15_RdFL(led_lut_offset_x3_x4);
		DPE_LED_H15_Wr01(led_lut_offset_x3_x4, bv_offset_x3, 600);
		DPE_LED_H15_Wr01(led_lut_offset_x3_x4, bv_offset_x4, 800);
		DPE_LED_H15_WrFL(led_lut_offset_x3_x4);

		DPE_LED_H15_RdFL(led_lut_offset_y0_y1);
		DPE_LED_H15_Wr01(led_lut_offset_y0_y1, bv_offset_y0, 0);
		DPE_LED_H15_Wr01(led_lut_offset_y0_y1, bv_offset_y1, 200);
		DPE_LED_H15_WrFL(led_lut_offset_y0_y1);

		DPE_LED_H15_RdFL(led_lut_offset_y2_y3);
		DPE_LED_H15_Wr01(led_lut_offset_y2_y3, bv_offset_y2, 400);
		DPE_LED_H15_Wr01(led_lut_offset_y2_y3, bv_offset_y3, 600);
		DPE_LED_H15_WrFL(led_lut_offset_y2_y3);

		DPE_LED_H15_RdFL(led_lut_offset_y4_y5);
		DPE_LED_H15_Wr01(led_lut_offset_y4_y5, bv_offset_y4, 800);
		DPE_LED_H15_Wr01(led_lut_offset_y4_y5, bv_offset_y5, 1023);
		DPE_LED_H15_WrFL(led_lut_offset_y4_y5);
	} while (0);

	return ret;
}

int BE_LED_HW_H15_Close(void)
{
	int ret = RET_OK;

	return ret;
}

int BE_LED_HW_H15_Resume(void)
{
	int ret = RET_OK;

	/* Init for smart black */
	DPE_LED_H15_RdFL(led_lut_offset_en);
	DPE_LED_H15_Wr01(led_lut_offset_en, bv_offset_en, 0x3);
	DPE_LED_H15_WrFL(led_lut_offset_en);

	DPE_LED_H15_RdFL(led_ld_bv_ext_max);
	DPE_LED_H15_Wr01(led_ld_bv_ext_max, bv_ext_max, 0x0);
	DPE_LED_H15_WrFL(led_ld_bv_ext_max);

	DPE_LED_H15_RdFL(led_lut_offset_x1_x2);
	DPE_LED_H15_Wr01(led_lut_offset_x1_x2, bv_offset_x1, 200);
	DPE_LED_H15_Wr01(led_lut_offset_x1_x2, bv_offset_x2, 400);
	DPE_LED_H15_WrFL(led_lut_offset_x1_x2);

	DPE_LED_H15_RdFL(led_lut_offset_x3_x4);
	DPE_LED_H15_Wr01(led_lut_offset_x3_x4, bv_offset_x3, 600);
	DPE_LED_H15_Wr01(led_lut_offset_x3_x4, bv_offset_x4, 800);
	DPE_LED_H15_WrFL(led_lut_offset_x3_x4);

	DPE_LED_H15_RdFL(led_lut_offset_y0_y1);
	DPE_LED_H15_Wr01(led_lut_offset_y0_y1, bv_offset_y0, 0);
	DPE_LED_H15_Wr01(led_lut_offset_y0_y1, bv_offset_y1, 200);
	DPE_LED_H15_WrFL(led_lut_offset_y0_y1);

	DPE_LED_H15_RdFL(led_lut_offset_y2_y3);
	DPE_LED_H15_Wr01(led_lut_offset_y2_y3, bv_offset_y2, 400);
	DPE_LED_H15_Wr01(led_lut_offset_y2_y3, bv_offset_y3, 600);
	DPE_LED_H15_WrFL(led_lut_offset_y2_y3);

	DPE_LED_H15_RdFL(led_lut_offset_y4_y5);
	DPE_LED_H15_Wr01(led_lut_offset_y4_y5, bv_offset_y4, 800);
	DPE_LED_H15_Wr01(led_lut_offset_y4_y5, bv_offset_y5, 1023);
	DPE_LED_H15_WrFL(led_lut_offset_y4_y5);

	return ret;
}

int BE_LED_HW_H15_Set_Mode(LX_BE_LED_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_LED_MODE_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uLEDMode	= pstParams->u32PictureMode;

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_LED_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_LED_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_HW_H15_Control(LX_BE_LED_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	do {
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_LED_MODE_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uLEDEn	= pstParams->u16DimminOn == 0xFFFF ? 0xFFFFFFFF : pstParams->u16DimminOn;

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_LED_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_LED_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_LED_HW_H15_Get_Apl(LX_BE_LED_APL_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_NULL(pstParams);

		DPE_LED_H15_RdFL(led_bv_gen_bv_max);
		DPE_LED_H15_Rd01(led_bv_gen_bv_max, bvgen_min, pstParams->u16Min);

		DPE_LED_H15_RdFL(led_bv_min_max);
		DPE_LED_H15_Rd01(led_bv_min_max, max_bv, pstParams->u16Max);
	} while (0);

	return ret;
}

int BE_LED_HW_H15_Set_ClippingValue(UINT32 u32Param)
{
	int ret = RET_OK;

	do {
		DPE_LED_H15_RdFL(led_ld_bv_ext_max);
		DPE_LED_H15_Wr01(led_ld_bv_ext_max, bv_ext_max, (u32Param & 0x3FF));
		DPE_LED_H15_WrFL(led_ld_bv_ext_max);
	} while (0);

	return ret;
}

int BE_LED_HW_H15_Get_RegInfo(LX_BE_LED_REG_INFO_T *pstParams)
{
	int ret = RET_OK;

	do {
		CHECK_NULL(pstParams);

		pstParams->u32CpuBaseAddr = DPE_LED_REG_H15A0_BASE;
		pstParams->u32McuBaseAddr = DPE_LED_REG_H15A0_BASE;
		pstParams->regOffset.AD_MEM_ADDR_CLR = offsetof(DPE_LED_REG_H15A0_T, led_mem_addr_clr);
		pstParams->regOffset.AD_MEM_DL_SBI_HCOEFF = offsetof(DPE_LED_REG_H15A0_T, led_mem_dl_sbi_hcoeff);
		pstParams->regOffset.AD_MEM_DL_SBI_VCOEFF = offsetof(DPE_LED_REG_H15A0_T, led_mem_dl_sbi_vcoeff);
		pstParams->regOffset.AD_FC_TAB1 = offsetof(DPE_LED_REG_H15A0_T, led_fc_tab1);
		pstParams->regOffset.AD_FC_TAB5 = offsetof(DPE_LED_REG_H15A0_T, led_fc_tab5);
		pstParams->regOffset.AD_UPPER_DC_LUT = offsetof(DPE_LED_REG_H15A0_T, led_upper_dc_lut);
		pstParams->regOffset.AD_LOWER_DC_LUT = offsetof(DPE_LED_REG_H15A0_T, led_lower_dc_lut);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_A = offsetof(DPE_LED_REG_H15A0_T, led_mem_dl_pixel_comp_a);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_B = offsetof(DPE_LED_REG_H15A0_T, led_mem_dl_pixel_comp_b);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_C = offsetof(DPE_LED_REG_H15A0_T, led_mem_dl_pixel_comp_c);
		pstParams->regOffset.AD_MEM_DL_PIXEL_COMP_D = offsetof(DPE_LED_REG_H15A0_T, led_mem_dl_pixel_comp_d);
		pstParams->regOffset.AD_UMASK_WR_COEFF = offsetof(DPE_LED_REG_H15A0_T, led_umask_wr_coeff);
	} while (0);

	return ret;
}

int BE_DRC_HW_H15_Set_Mode(LX_BE_HDR_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_DRC_MODE_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uDRCMode	= pstParams->u8DCMode	== 0xFF ? 0xFFFFFFFF : pstParams->u8DCMode;

		/* Set value for argument */
		if ((stHwParams.uDRCMode != 0xFFFFFFFF) && (_beDRCHwH15Info.u8DCMode != stHwParams.uDRCMode))
			_beDRCHwH15Info.u8DCMode	= (UINT8)stHwParams.uDRCMode;
		else
			stHwParams.uDRCMode = 0xFFFFFFFF;

		if ((pstParams->u8Store != 0xFF) && (_beDRCHwH15Info.u8Store !=pstParams->u8Store ))
			_beDRCHwH15Info.u8Store 	= pstParams->u8Store;
		else
			pstParams->u8Store = 0xFF;

		if ((pstParams->u8DRC_shp != 0xFF) && (_beDRCHwH15Info.u8DRC_shp != pstParams->u8DRC_shp))
			_beDRCHwH15Info.u8DRC_shp = pstParams->u8DRC_shp;
		else
			pstParams->u8DRC_shp = 0xFF;

		/* HDR bypass control */
		if (_beDRCHwH15Info.u8DCBypass)
		{
			if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_OFF)
			{
				_beDRCHwH15Info.u8DCBypass = FALSE;

				stHwParams.uDRCMode = (UINT32)_beDRCHwH15Info.u8DCMode;
				pstParams->u8Store 		= _beDRCHwH15Info.u8Store;
				pstParams->u8DRC_shp 	= _beDRCHwH15Info.u8DRC_shp;
			}
			else
			{
				return RET_OK;
			}
		}
		else
		{
			if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_ON)
				_beDRCHwH15Info.u8DCBypass = TRUE;

			if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_INTERNAL_OFF)
				pstParams->u8DCBypass = FALSE;
			else if (pstParams->u8DCBypass == LX_BE_HDR_DCR_BYPASS_INTERNAL_ON)
				pstParams->u8DCBypass = TRUE;
		}

		if (pstParams->u8Store != 0xFF)
		{
			PE1_DRC_H15_RdFL(pe1_fsw_ctrl_01);
			PE1_DRC_H15_Wr01(pe1_fsw_ctrl_01, reg_drc_store_mode, pstParams->u8Store ? 0x1:0x0);
			PE1_DRC_H15_WrFL(pe1_fsw_ctrl_01);
		}

		if (pstParams->u8DRC_shp != 0xFF)
		{
			PE1_DRC_H15_RdFL(pe1_fsw_ctrl_00);
			PE1_DRC_H15_Wr01(pe1_fsw_ctrl_00, reg_drc_use_shp, pstParams->u8DRC_shp ? 0x1:0x0);
			PE1_DRC_H15_WrFL(pe1_fsw_ctrl_00);
		}

		if (pstParams->u8DCBypass != 0xFF)
		{
			PE1_DRC_H15_RdFL(pe1_fsw_ctrl_01);
			PE1_DRC_H15_Wr01(pe1_fsw_ctrl_01, reg_drc_dc_bypass, pstParams->u8DCBypass ? 0x1:0x0);
			PE1_DRC_H15_WrFL(pe1_fsw_ctrl_01);
		}

		if (pstParams->u8DCsc != 0xFF)
		{
			PE1_DRC_H15_RdFL(pe1_fsw_ctrl_01);
			PE1_DRC_H15_Wr01(pe1_fsw_ctrl_01, reg_drc_dc_sc, pstParams->u8DCsc ? 0x1:0x0);
			PE1_DRC_H15_WrFL(pe1_fsw_ctrl_01);
		}

		/* return unexpected value */
		if ( stHwParams.uDRCMode == 0xFFFFFFFF )
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_DRC_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_DRC_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_DRC_HW_H15_Control(LX_BE_HDR_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	do {
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_DRC_MODE_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uDRCEn = pstParams->u16DRCEnable == 0xFFFF ? 0xFFFFFFFF : pstParams->u16DRCEnable;


		/* return unexpected value */
		if ( stHwParams.uDRCEn == 0xFFFFFFFF )
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_DRC_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_DRC_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_WCG_HW_H15_Set_Mode(LX_BE_WCG_MODE_T *pstParams)
{
	int ret = RET_OK;

	do {
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_WCG_MODE_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uWCGMode = pstParams->u32PictureMode;

		if ( (stHwParams.uWCGMode != 0xFFFFFFFF) && (_beWCGHwH15Info.uWCGMode != stHwParams.uWCGMode) )
			_beWCGHwH15Info.uWCGMode = stHwParams.uWCGMode;
		else
			stHwParams.uWCGMode = 0xFFFFFFFF;

		if ( stHwParams.uWCGMode == 0xFFFFFFFF )
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_WCG_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_WCG_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

int BE_WCG_HW_H15_Control(LX_BE_WCG_CONTROL_T *pstParams)
{
	int ret = RET_OK;

	do {
		IPC_SET_DEFAULT_PARAM(BE_IPC_B0_WCG_MODE_T, stHwParams);

		/* change value LX_BE to BE_IPC */
		stHwParams.uWCGEn	= pstParams->u32Enable;

		if ((stHwParams.uWCGEn != 0xFFFFFFFF) && (_beWCGHwH15Info.uWCGEn != stHwParams.uWCGEn))
			_beWCGHwH15Info.uWCGEn = stHwParams.uWCGEn;
		else
			stHwParams.uWCGEn = 0xFFFFFFFF;

		if ( stHwParams.uWCGEn == 0xFFFFFFFF )
		{
			return ret;
		}

		/* Send command  to MCU via IPC & Check result */
		ret = BE_KIPC_SetData(DPPB_PACK_CMD(BE_WCG_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_WCG_MODE_T));
		if (ret) BREAK_WRONG(ret);
	} while (0);

	return ret;
}

/** @} */
