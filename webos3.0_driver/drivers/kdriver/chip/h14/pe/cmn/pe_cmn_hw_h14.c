/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file pe_cmn_hw_h14.c
 *
 *  driver for picture enhance common functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.12
 *	@see		
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"

#include "pe_hw_h14.h"
#include "pe_reg_h14.h"
#include "pe_fwi_h14.h"
#include "pe_cmn_hw_h14.h"



/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_CMN_HW_H14_ERROR	printk
#define PE_CMN_HW_H14_DBG_PRINT(fmt,args...)	\
	if(_g_cmn_hw_h14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_CMN_HW_H14_CHECK_CODE(_checker,_action,fmt,args...)	\
	if(_checker){PE_CMN_HW_H14_ERROR(fmt,##args);_action;}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
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
static UINT32 _g_cmn_hw_h14_trace=0x0;		//default should be off.
static PE_CMN_HW_H14_SETTINGS_T _g_pe_cmn_hw_h14_info;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init cmn
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H14)
		{
			memset_0xff(_g_pe_cmn_hw_h14_info);
			_g_pe_cmn_hw_h14_info.dbg_print_lvl = 0x0;
			_g_pe_cmn_hw_h14_info.dbg_bypass[LX_PE_WIN_0] = 0x0;
			_g_pe_cmn_hw_h14_info.dbg_bypass[LX_PE_WIN_1] = 0x0;
			PE_P1L_H14_QWr02(pe1_top_ctrl_00,			load_type,			0x0,\
														load_time,			0x0);
			PE_CMN_HW_H14_DBG_PRINT("vsp-yc,rgb : enable.\n");
			PE_P0L_H14_QWr04(p0l_lvcrtl_ctrl_00,		enable,				0x1,\
														y_offset,			0x200,\
														center_position,	0x80,\
														y_gain,				0x80);
			PE_P0R_H14_QWr04(p0r_lvcrtl_ctrl_00,		enable,				0x1,\
														y_offset,			0x200,\
														center_position,	0x80,\
														y_gain,				0x80);
			/* 20130104, sj.youm, vspyc center_position 0x0->0x10(related with 16~235 range) */
			PE_P1L_H14_QWr04(pe1_vspyc_ctrl_00,			enable,				0x1,\
														graycolor_enable,	0x0,\
														center_position,	0x10,\
														contrast,			0x200);
			PE_P1L_H14_QWr02(pe1_vspyc_ctrl_01,			saturation,			0x80,\
														brightness,			0x200);
		}	
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set cmn debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_CMN_HW_H14_SETTINGS_T *pInfo = &_g_pe_cmn_hw_h14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H14_DBG_PRINT("set[%d] type:0x%x, print:0x%x, bypass:0x%x, fwc_en,dbg:%d,%d\n",\
			pstParams->win_id,pstParams->type,pstParams->print_lvl,pstParams->bypass,\
			pstParams->fwc.ctrl_en,pstParams->fwc.dbg_en);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_cmn_hw_h14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(CMN)))? 0x1:0x0;
			pInfo->dbg_print_lvl = pstParams->print_lvl;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_H14)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(CMN)))
				{
					PE_CMN_HW_H14_DBG_PRINT("cont,bri,sat disable, hue bypass.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_H14_QWr01(p0l_lvcrtl_ctrl_00,	enable,			0x0);
						PE_P1L_H14_QWr01(pe1_vspyc_ctrl_00, 	enable,			0x0);
						PE_P1L_H14_QWr01(pe1_cen_ctrl_03, 		ihsv_hoffset,	0x80);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_H14_QWr01(p0r_lvcrtl_ctrl_00, enable,		0x0);
					}
					
				}
				else
				{
					PE_CMN_HW_H14_DBG_PRINT("cont,bri,sat enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_P0L_H14_QWr01(p0l_lvcrtl_ctrl_00,	enable,		0x1);
						PE_P1L_H14_QWr01(pe1_vspyc_ctrl_00,		enable,		0x1);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_P0R_H14_QWr01(p0r_lvcrtl_ctrl_00,	enable,		0x1);
					}
				}
				if(PE_CHECK_WIN0(pstParams->win_id))
				{
					pInfo->dbg_bypass[LX_PE_WIN_0] = pstParams->bypass;
				}
			}
			else
			{
				PE_CMN_HW_H14_DBG_PRINT("do nothing.\n");	ret = RET_OK;
			}
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				pInfo->dbg_bypass[LX_PE_WIN_0] = pstParams->bypass;
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				pInfo->dbg_bypass[LX_PE_WIN_1] = pstParams->bypass;
			}
		}
		/* set fwc */
		if(pstParams->type&LX_PE_DBG_FW)
		{
			if(PE_KDRV_VER_H14)
			{
				PE_FWI_H14_FW_CTRL fw_ctrl;
				PE_FWI_H14_SetDbgPrint(pstParams->fwc.dbg_en);
				fw_ctrl.enable = (pstParams->fwc.ctrl_en)? 0x1:0x0;
				ret = PE_FWI_H14_SetFwCtrl(&fw_ctrl);
				PE_CMN_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H14_SetFwCtrl() error.\n",__F__,__L__);
			}
			else
			{
				PE_CMN_HW_H14_DBG_PRINT("do nothing.\n");	ret = RET_OK;
			}
		}
	}while(0);
	return ret;
}

/**
 * get cmn debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_GetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_CMN_HW_H14_SETTINGS_T *pInfo = &_g_pe_cmn_hw_h14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_CHECK_WIN0(win_id))
		{
			pstParams->bypass = pInfo->dbg_bypass[LX_PE_WIN_0];
		}
		if(PE_CHECK_WIN1(win_id))
		{
			pstParams->bypass = pInfo->dbg_bypass[LX_PE_WIN_1];
		}
		pstParams->print_lvl = pInfo->dbg_print_lvl;
		if(PE_KDRV_VER_H14)
		{
			PE_FWI_H14_SETTINGS_T param;
			pstParams->fwc.dbg_en = PE_FWI_H14_GetDbgPrint();
			ret = PE_FWI_H14_GetCurFwiSettings(&param);
			PE_CMN_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H14_GetCurFwiSettings() error.\n",__F__,__L__);
			pstParams->fwc.ctrl_en = param.fw_ctrl.enable;
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("not use fwc.\n");
			pstParams->fwc.dbg_en = 0;
			pstParams->fwc.ctrl_en = 0;
		}
		PE_CMN_HW_H14_DBG_PRINT("get[%d] type:0x%x, print:0x%x, bypass:0x%x, fwc_en,dbg:%d,%d\n",\
			pstParams->win_id,pstParams->type,pstParams->print_lvl,pstParams->bypass,\
			pstParams->fwc.ctrl_en,pstParams->fwc.dbg_en);
	}while(0);
	return ret;
}
/**
 * set pe cmn operation periodically
 *	- ud,3d mode on pe1 operation
 *
 * @param   *pCfg [in] PE_TSK_H14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_SetOperationMode(PE_TSK_H14_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_CMN_HW_H14_OPR_MODE_T cur_param;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_CMN_HW_H14_OPR_MODE_T *opr_info=&_g_pe_cmn_hw_h14_info.opr_inf;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H14)
		{
			/* pe1 ud mode */
			switch(disp0_info->out_type)
			{
				case LX_PE_OUT_UD:
					cur_param.pe1_mode_ud = 1;
					break;
				case LX_PE_OUT_2D:
				case LX_PE_OUT_3D_2D:
				case LX_PE_OUT_2D_3D:
				case LX_PE_OUT_TB:
				case LX_PE_OUT_SS:
				case LX_PE_OUT_PIP:
				case LX_PE_OUT_FS:
				case LX_PE_OUT_DUAL_TB:
				case LX_PE_OUT_DUAL_SS:
				case LX_PE_OUT_DUAL_FULL:
				default:
					cur_param.pe1_mode_ud = 0;
					break;
			}
			/* pe1 3d mode(interleaving) */
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				cur_param.pe1_mode_3d = (disp0_info->out_type==LX_PE_OUT_2D_3D)? 1:0;
			}
			else	/* internal frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_DUAL_TB:
					case LX_PE_OUT_DUAL_SS:
					case LX_PE_OUT_DUAL_FULL:
					case LX_PE_OUT_2D_3D:
						cur_param.pe1_mode_3d = 1;
						break;
					case LX_PE_OUT_2D:
					case LX_PE_OUT_3D_2D:
					case LX_PE_OUT_UD:
					case LX_PE_OUT_TB:
					case LX_PE_OUT_SS:
					case LX_PE_OUT_FS:
					case LX_PE_OUT_PIP:
					default:
						cur_param.pe1_mode_3d = 0;
						break;
				}
			}
			/* set pe1 top ctrl : ud,3d mode */
			if((opr_info->pe1_mode_ud != cur_param.pe1_mode_ud)|| \
				(opr_info->pe1_mode_3d != cur_param.pe1_mode_3d))
			{
				PE_CMN_HW_H14_DBG_PRINT("[inf]is_ext_frc:%d,out:%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->out_type);
				PE_CMN_HW_H14_DBG_PRINT("[set]ud,3d mode : prev:%d,%d, cur:%d,%d\n",\
					opr_info->pe1_mode_ud,opr_info->pe1_mode_3d,\
					cur_param.pe1_mode_ud,cur_param.pe1_mode_3d);
	            PE_P1L_H14_RdFL(pe1_top_ctrl_02);
	            PE_P1L_H14_Wr01(pe1_top_ctrl_02,	ud_mode,	cur_param.pe1_mode_ud);
	            PE_P1L_H14_Wr01(pe1_top_ctrl_02,	mode_3d,	cur_param.pe1_mode_3d);
				PE_P1L_H14_WrFL(pe1_top_ctrl_02);
				opr_info->pe1_mode_ud = cur_param.pe1_mode_ud;
				opr_info->pe1_mode_3d = cur_param.pe1_mode_3d;
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set contrast
 *
 * @param   *pstParams [in] LX_PE_CMN_CONTRAST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_SetContrast(LX_PE_CMN_CONTRAST_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H14_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->contrast);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_QWr01(pe1_vspyc_ctrl_00, contrast,		GET_BITS(pstParams->contrast,0,10));
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * get contrast
 *
 * @param   *pstParams [in/out] LX_PE_CMN_CONTRAST_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_GetContrast(LX_PE_CMN_CONTRAST_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd01(pe1_vspyc_ctrl_00, contrast,		pstParams->contrast);
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H14_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->contrast);
	}while(0);
	return ret;
}
/**
 * set brightness
 *
 * @param   *pstParams [in] LX_PE_CMN_BRIGHTNESS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_SetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H14_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->brightness);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_QWr01(pe1_vspyc_ctrl_01, brightness,		GET_BITS(pstParams->brightness,0,10));
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get brightness
 *
 * @param   *pstParams [in/out] LX_PE_CMN_BRIGHTNESS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_GetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd01(pe1_vspyc_ctrl_01, brightness,		pstParams->brightness);
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H14_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->brightness);
	}while(0);
	return ret;
}
/**
 * set saturation
 *
 * @param   *pstParams [in] LX_PE_CMN_SATURATION_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_SetSaturation(LX_PE_CMN_SATURATION_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H14_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->saturation);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_QWr01(pe1_vspyc_ctrl_01, saturation,		GET_BITS(pstParams->saturation,0,8));
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get saturation
 *
 * @param   *pstParams [in/out] LX_PE_CMN_SATURATION_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_GetSaturation(LX_PE_CMN_SATURATION_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd01(pe1_vspyc_ctrl_01, saturation,		pstParams->saturation);
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H14_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->saturation);
	}while(0);
	return ret;
}
/**
 * set hue
 *
 * @param   *pstParams [in] LX_PE_CMN_HUE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_SetHue(LX_PE_CMN_HUE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H14_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->hue);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P1L_H14_QWr01(pe1_cen_ctrl_03, ihsv_hoffset,	GET_BITS(pstParams->hue,0,8));
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get hue
 *
 * @param   *pstParams [in/out] LX_PE_CMN_HUE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_GetHue(LX_PE_CMN_HUE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P1L_H14_QRd01(pe1_cen_ctrl_03, ihsv_hoffset,	pstParams->hue);
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H14_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->hue);
	}while(0);
	return ret;
}
/**
 * set level control
 *
 * @param   *pstParams [in] LX_PE_CMN_LEVEL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_SetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H14_DBG_PRINT("set pstParams[%d] : ofs:%d, center:%d, gain:%d, tp:%d\n",\
			pstParams->win_id,pstParams->offset,pstParams->center,\
			pstParams->gain,pstParams->tp_on);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_P0L_H14_QWr03(p0l_lvcrtl_ctrl_00,	y_offset,			GET_BITS(pstParams->offset,0,10),\
														center_position,	GET_BITS(pstParams->center,0,8),\
														y_gain,				GET_BITS(pstParams->gain,0,8));
				PE_P1L_H14_QWr01(pe1_vspyc_ctrl_00, 	graycolor_enable,		(pstParams->tp_on)? 0x1:0x0);
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_P0R_H14_QWr03(p0r_lvcrtl_ctrl_00,	y_offset,			GET_BITS(pstParams->offset,0,10),\
														center_position,	GET_BITS(pstParams->center,0,8),\
														y_gain,				GET_BITS(pstParams->gain,0,8));
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get level control
 *
 * @param   *pstParams [in/out] LX_PE_CMN_LEVEL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H14_GetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_H14_QRd03(p0l_lvcrtl_ctrl_00,	y_offset,			pstParams->offset,\
														center_position,	pstParams->center,\
														y_gain,				pstParams->gain);
				PE_P1L_H14_QRd01(pe1_vspyc_ctrl_00,		graycolor_enable,	pstParams->tp_on);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_H14_QRd03(p0r_lvcrtl_ctrl_00,	y_offset,			pstParams->offset,\
														center_position,	pstParams->center,\
														y_gain,				pstParams->gain);
			}
		}
		else
		{
			PE_CMN_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CMN_HW_H14_DBG_PRINT("get pstParams[%d] : ofs:%d, center:%d, gain:%d, tp:%d\n",\
			pstParams->win_id,pstParams->offset,pstParams->center,\
			pstParams->gain,pstParams->tp_on);
	}while(0);
	return ret;
}

