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

/** @file pe_cmn_hw_h15.c
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
#include "debug_util.h"

#include "be_cfg.h"
#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_fwi_h15.h"
#include "pe_cmn_hw_h15.h"
#include "pe_dcm_hw_h15.h"
#include "pe_shp_hw_h15.h"
#include "pe_nrd_hw_h15.h"
#include "cvd_control_h15a0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
/* use vgain, voffset instead of lvctrl, when tnr,sc mem share mode */
#define PE_CMN_HW_H15_USE_VGAIN_FOR_LVCTRL

/* db mode for resume init, to avoid db mode clear on resume init */
#define PE_CMN_HW_H15_DB_MODE_FOR_RESUME

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_CMN_HW_H15_ERROR	printk
#define PE_CMN_HW_H15_DBG_PRINT(fmt,args...)	\
	if(_g_cmn_hw_h15_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_CMN_HW_H15_CHECK_CODE(_checker,_action,fmt,args...)	\
	if(_checker){PE_CMN_HW_H15_ERROR(fmt,##args);_action;}

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
static int PE_CMN_HW_H15_DownloadModeDb(void);
static int PE_CMN_HW_H15_DownloadCvdDb(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_cmn_hw_h15_trace=0x0;		//default should be off.
static PE_CMN_HW_H15_SETTINGS_T _g_pe_cmn_hw_h15_info;

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
int PE_CMN_HW_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		memset(&_g_pe_cmn_hw_h15_info, 0, sizeof(PE_CMN_HW_H15_SETTINGS_T));
		if(PE_KDRV_VER_H15)
		{
			PE_SHPLR_H15A0_QWr01(pe1_top_ctrl_02,mode_3d,0x0);
			PE_CEO_PE1_H15A0_QWr01(pe1_top_ctrl_02,mode_3d,0x0);
			PE_CMN_HW_H15_DBG_PRINT("vsp-yc,rgb, lvctrl : enable.\n");
			PE_ND12_H15A0_QWr04(lvcrtl_ctrl_00,     enable,             0x1, \
                                                    y_offset,           0x200, \
                                                    center_position,    0x80, \
                                                    y_gain,             0x80);
			/* nds lvctrl y_offset : 8bit */
			PE_ND_NDS_H15A0_QWr04(lvcrtl_ctrl_00,   enable,             0x1, \
                                                    y_offset,           0x80, \
                                                    center_position,    0x80, \
                                                    y_gain,             0x80);
			/* vspyc center_position 0x0->0x10(related with 16~235 range) */
			PE_CEO_PE1_H15A0_QWr05(pe1_vspyc_ctrl_00,enable,            0x1, \
                                                     graycolor_enable,  0x0, \
                                                     color_only_enable, 0x0, \
                                                     center_position,   0x10, \
                                                     contrast,          0x200);
			PE_CEO_PE1_H15A0_QWr02(pe1_vspyc_ctrl_01,saturation,        0x80,\
                                                     brightness,        0x200);
			PE_CEO_PE1_H15A0_QWr01(pe1_cen_ctrl_02,  ihsv_vgain,        0x80);
			PE_CEO_PE1_H15A0_QWr02(pe1_cen_ctrl_03,  ihsv_voffset,      0x80, \
                                                     ihsv_hoffset,      0x80);
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_CMN_HW_H15_SETTINGS_T *pInfo = &_g_pe_cmn_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H15_DBG_PRINT("set[%d] type:0x%x, print:0x%x, bypass:0x%x, fwc_en,dbg:%d,%d\n",\
			pstParams->win_id,pstParams->type,pstParams->print_lvl,pstParams->bypass,\
			pstParams->fwc.ctrl_en,pstParams->fwc.dbg_en);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_cmn_hw_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(CMN)))? 0x1:0x0;
			pInfo->dbg_print_lvl = pstParams->print_lvl;
		}
		/* set bypass */
		if(pstParams->type&LX_PE_DBG_BY)
		{
			if(PE_KDRV_VER_H15)
			{
				if(pstParams->bypass & (0x1<<PE_ITEM_PKTMODL(CMN)))
				{
					PE_CMN_HW_H15_DBG_PRINT("cont,bri,sat disable, hue bypass.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_ND12_H15A0_QWr01(lvcrtl_ctrl_00,enable,0x0);
						PE_CEO_PE1_H15A0_QWr01(pe1_vspyc_ctrl_00,enable,0x0);
						PE_CEO_PE1_H15A0_QWr01(pe1_cen_ctrl_03,ihsv_hoffset,0x80);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_ND_NDS_H15A0_QWr01(lvcrtl_ctrl_00,enable,0x0);
					}
				}
				else
				{
					PE_CMN_HW_H15_DBG_PRINT("cont,bri,sat enable.\n");
					if(PE_CHECK_WIN0(pstParams->win_id))
					{
						PE_ND12_H15A0_QWr01(lvcrtl_ctrl_00,enable,0x1);
						PE_CEO_PE1_H15A0_QWr01(pe1_vspyc_ctrl_00,enable,0x1);
					}
					if(PE_CHECK_WIN1(pstParams->win_id))
					{
						PE_ND_NDS_H15A0_QWr01(lvcrtl_ctrl_00,enable,0x1);
					}
				}
			}
			else
			{
				PE_CMN_HW_H15_DBG_PRINT("do nothing.\n");	ret = RET_OK;
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
			if(PE_KDRV_VER_H15)
			{
				PE_FWI_H15_FW_CTRL fw_ctrl;
				PE_FWI_H15_SetDbgPrint(pstParams->fwc.dbg_en);
				fw_ctrl.enable = (pstParams->fwc.ctrl_en)? 0x1:0x0;
				ret = PE_FWI_H15_SetFwCtrl(&fw_ctrl, FALSE);
				PE_CMN_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H15_SetFwCtrl() error.\n",__F__,__L__);
			}
			else
			{
				PE_CMN_HW_H15_DBG_PRINT("do nothing.\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_GetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_CMN_HW_H15_SETTINGS_T *pInfo = &_g_pe_cmn_hw_h15_info;
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
		if(PE_KDRV_VER_H15)
		{
			PE_FWI_H15_SETTINGS_T param;
			pstParams->fwc.dbg_en = PE_FWI_H15_GetDbgPrint();
			ret = PE_FWI_H15_GetCurFwiSettings(&param);
			PE_CMN_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_FWI_H15_GetCurFwiSettings() error.\n",__F__,__L__);
			pstParams->fwc.ctrl_en = param.fw_ctrl.enable;
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("not use fwc.\n");
			pstParams->fwc.dbg_en = 0;
			pstParams->fwc.ctrl_en = 0;
		}
		PE_CMN_HW_H15_DBG_PRINT("get[%d] type:0x%x, print:0x%x, bypass:0x%x, fwc_en,dbg:%d,%d\n",\
			pstParams->win_id,pstParams->type,pstParams->print_lvl,pstParams->bypass,\
			pstParams->fwc.ctrl_en,pstParams->fwc.dbg_en);
	}while(0);
	return ret;
}
/**
 * set pe cmn operation periodically
 *	- ud,3d mode on pe1 operation
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H15_SetOperationMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_CMN_HW_H15_OPR_MODE_T cur_param;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_CMN_HW_H15_OPR_MODE_T *opr_info=&_g_pe_cmn_hw_h15_info.opr_inf;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			/* pe1 ud mode */
			cur_param.pe1_mode_ud = 0;
			/* pe1 3d mode(interleaving) */
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				cur_param.pe1_mode_3d = (disp0_info->out_type==LX_PE_OUT_LA)? 1:0;
			}
			else	/* internal frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_DUAL_TB:
					case LX_PE_OUT_DUAL_SS:
					case LX_PE_OUT_DUAL_FULL:
					case LX_PE_OUT_LA:
						cur_param.pe1_mode_3d = 1;
						break;
					case LX_PE_OUT_2D_3D:
						if(disp0_info->out_sub==LX_PE_OUT_LA)
						{
							cur_param.pe1_mode_3d = 1;
						}
						else
						{
							cur_param.pe1_mode_3d = 0;
						}
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
			/* set pe1 top ctrl : 3d mode(ud mode is NA for H15) */
			if(opr_info->pe1_mode_3d != cur_param.pe1_mode_3d)
			{
				PE_CMN_HW_H15_DBG_PRINT("[inf]is_ext_frc:%d,out:%d,fmt:%d\n",\
					disp0_info->mode.is_ext_frc,\
					disp0_info->out_type,disp0_info->fmt_type);
				PE_CMN_HW_H15_DBG_PRINT("[set]ud,3d mode : prev:%d,%d, cur:%d,%d\n",\
					opr_info->pe1_mode_ud,opr_info->pe1_mode_3d,\
					cur_param.pe1_mode_ud,cur_param.pe1_mode_3d);
				PE_SHPLR_H15A0_QWr01(pe1_top_ctrl_02,mode_3d,cur_param.pe1_mode_3d);
				PE_CEO_PE1_H15A0_QWr01(pe1_top_ctrl_02,mode_3d,cur_param.pe1_mode_3d);
				opr_info->pe1_mode_ud = cur_param.pe1_mode_ud;
				opr_info->pe1_mode_3d = cur_param.pe1_mode_3d;
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_SetContrast(LX_PE_CMN_CONTRAST_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H15_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->contrast);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CEO_PE1_H15A0_QWr01(pe1_vspyc_ctrl_00, contrast, GET_BITS(pstParams->contrast,0,10));
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_GetContrast(LX_PE_CMN_CONTRAST_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CE_PE1_H15A0_QRd01(pe1_vspyc_ctrl_00, contrast, pstParams->contrast);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CO_PE1_H15A0_QRd01(pe1_vspyc_ctrl_00, contrast, pstParams->contrast);
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H15_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->contrast);
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
int PE_CMN_HW_H15_SetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H15_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->brightness);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CEO_PE1_H15A0_QWr01(pe1_vspyc_ctrl_01, brightness, GET_BITS(pstParams->brightness,0,10));
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_GetBrightness(LX_PE_CMN_BRIGHTNESS_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CE_PE1_H15A0_QRd01(pe1_vspyc_ctrl_01, brightness, pstParams->brightness);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CO_PE1_H15A0_QRd01(pe1_vspyc_ctrl_01, brightness, pstParams->brightness);
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H15_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->brightness);
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
int PE_CMN_HW_H15_SetSaturation(LX_PE_CMN_SATURATION_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H15_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->saturation);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CEO_PE1_H15A0_QWr01(pe1_vspyc_ctrl_01, saturation, GET_BITS(pstParams->saturation,0,8));
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_GetSaturation(LX_PE_CMN_SATURATION_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CE_PE1_H15A0_QRd01(pe1_vspyc_ctrl_01, saturation, pstParams->saturation);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CO_PE1_H15A0_QRd01(pe1_vspyc_ctrl_01, saturation, pstParams->saturation);
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H15_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->saturation);
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
int PE_CMN_HW_H15_SetHue(LX_PE_CMN_HUE_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H15_DBG_PRINT("set pstParams[%d] : %d\n",pstParams->win_id,pstParams->hue);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				PE_CEO_PE1_H15A0_QWr01(pe1_cen_ctrl_03, ihsv_hoffset, GET_BITS(pstParams->hue,0,8));
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_GetHue(LX_PE_CMN_HUE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_CE_PE1_H15A0_QRd01(pe1_cen_ctrl_03, ihsv_hoffset, pstParams->hue);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_CO_PE1_H15A0_QRd01(pe1_cen_ctrl_03, ihsv_hoffset, pstParams->hue);
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_CMN_HW_H15_DBG_PRINT("get pstParams[%d] : %d\n",pstParams->win_id,pstParams->hue);
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
int PE_CMN_HW_H15_SetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_CMN_HW_H15_USE_VGAIN_FOR_LVCTRL
	PE_CMN_HW_H15_SETTINGS_T *pInfo = &_g_pe_cmn_hw_h15_info;
	#endif
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_CMN_HW_H15_DBG_PRINT("set pstParams[%d] : ofs:%d, center:%d, gain:%d, tp:%d\n",\
			pstParams->win_id,pstParams->offset,pstParams->center,\
			pstParams->gain,pstParams->tp_on);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				#ifdef PE_CMN_HW_H15_USE_VGAIN_FOR_LVCTRL
				pInfo->cur_lvctrl_gain = GET_BITS(pstParams->gain,0,8);
				pInfo->cur_lvctrl_ofst = GET_BITS(pstParams->offset,0,10);
				PE_ND12_H15A0_QWr01(lvcrtl_ctrl_00, center_position, GET_BITS(pstParams->center,0,8));
				#else
				PE_ND12_H15A0_QWr03(lvcrtl_ctrl_00, y_offset,        GET_BITS(pstParams->offset,0,10), \
                                                    center_position, GET_BITS(pstParams->center,0,8), \
                                                    y_gain,          GET_BITS(pstParams->gain,0,8));
				#endif
				PE_CEO_PE1_H15A0_QWr01(pe1_vspyc_ctrl_00,graycolor_enable,(pstParams->tp_on)? 0x1:0x0);
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				PE_ND_NDS_H15A0_QWr03(lvcrtl_ctrl_00, y_offset,        GET_BITS(pstParams->offset,0,10), \
                                                      center_position, GET_BITS(pstParams->center,0,8), \
                                                      y_gain,          GET_BITS(pstParams->gain,0,8));
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_CMN_HW_H15_GetLevelCtrl(LX_PE_CMN_LEVEL_CTRL_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_ND_ND1_H15A0_QRd03(lvcrtl_ctrl_00, y_offset,        pstParams->offset,\
                                                      center_position, pstParams->center,\
                                                      y_gain,          pstParams->gain);
				PE_CE_PE1_H15A0_QRd01(pe1_vspyc_ctrl_00,graycolor_enable,pstParams->tp_on);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_ND_ND2_H15A0_QRd03(lvcrtl_ctrl_00, y_offset,        pstParams->offset,\
                                                      center_position, pstParams->center,\
                                                      y_gain,          pstParams->gain);
				PE_CO_PE1_H15A0_QRd01(pe1_vspyc_ctrl_00,graycolor_enable,pstParams->tp_on);
			}
			if(pstParams->win_id == LX_PE_WIN_ALL)
			{
				PE_ND_NDS_H15A0_QRd03(lvcrtl_ctrl_00, y_offset,        pstParams->offset,\
                                                      center_position, pstParams->center,\
                                                      y_gain,          pstParams->gain);
				pstParams->tp_on = 0;
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_CMN_HW_H15_DBG_PRINT("get pstParams[%d] : ofs:%d, center:%d, gain:%d, tp:%d\n",\
			pstParams->win_id,pstParams->offset,pstParams->center,\
			pstParams->gain,pstParams->tp_on);
	}while(0);
	return ret;
}
/**
 * set level mode
 *	- set cen vgain instead of lvctrl, when tnr,sc mem share mode
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H15_SetLevelCtrlMode(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	#ifdef PE_CMN_HW_H15_USE_VGAIN_FOR_LVCTRL
	static UINT32 count = 0;
	UINT32 tnrw_off, delta;
	UINT32 cur_ihsv_v_gain,cur_ihsv_v_ofst,cur_lvctrl_gain,cur_lvctrl_ofst;
	PE_CMN_HW_H15_SETTINGS_T *pInfo = &_g_pe_cmn_hw_h15_info;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			PE_ND_ND1_H15A0_QRd01(pe0_opmode,tnrw_off,tnrw_off);
			if (tnrw_off)	//mem share mode
			{
				cur_lvctrl_gain = 128;//default
				cur_lvctrl_ofst = 512;//default
				cur_ihsv_v_gain = pInfo->cur_lvctrl_gain;
				if (pInfo->cur_lvctrl_ofst>512)
				{
					delta = pInfo->cur_lvctrl_ofst-512;
					if (delta>127)	delta=127;
					cur_ihsv_v_ofst = 128+delta;
				}
				else
				{
					delta = 512-pInfo->cur_lvctrl_ofst;
					if (delta>128)	delta=128;
					cur_ihsv_v_ofst = 128-delta;
				}
			}
			else	//normal mode
			{
				cur_lvctrl_gain = pInfo->cur_lvctrl_gain;
				cur_lvctrl_ofst = pInfo->cur_lvctrl_ofst;
				cur_ihsv_v_gain = 128;//default
				cur_ihsv_v_ofst = 128;//default
			}
			/* set lvctrl gain,ofst */
			if (pInfo->pre_lvctrl_gain!=cur_lvctrl_gain || pInfo->pre_lvctrl_ofst!=cur_lvctrl_ofst)
			{
				PE_CMN_HW_H15_DBG_PRINT("[inf] tnrw_off:%d, cur_lvctrl gain:%d,ofst:%d\n",\
					tnrw_off, pInfo->cur_lvctrl_gain, pInfo->cur_lvctrl_ofst);
				PE_CMN_HW_H15_DBG_PRINT("[set] lvctrl gain:%d->%d,ofst:%d->%d\n",\
					pInfo->pre_lvctrl_gain, cur_lvctrl_gain, pInfo->pre_lvctrl_ofst, cur_lvctrl_ofst);
				PE_ND12_H15A0_QWr02(lvcrtl_ctrl_00,y_offset,cur_lvctrl_ofst,y_gain,cur_lvctrl_gain);
				pInfo->pre_lvctrl_gain=cur_lvctrl_gain;
				pInfo->pre_lvctrl_ofst=cur_lvctrl_ofst;
			}
			/* set ihsv_v gain,ofst */
			if (pInfo->pre_ihsv_v_gain!=cur_ihsv_v_gain || pInfo->pre_ihsv_v_ofst!=cur_ihsv_v_ofst)
			{
				PE_CMN_HW_H15_DBG_PRINT("[inf] tnrw_off:%d, cur_ihsv_v gain:%d,ofst:%d\n",\
					tnrw_off, pInfo->cur_lvctrl_gain, pInfo->cur_lvctrl_ofst);
				PE_CMN_HW_H15_DBG_PRINT("[set] ihsv_v gain:%d->%d,ofst:%d->%d\n",\
					pInfo->pre_ihsv_v_gain, cur_ihsv_v_gain, pInfo->pre_ihsv_v_ofst, cur_ihsv_v_ofst);
				PE_CEO_PE1_H15A0_QWr01(pe1_cen_ctrl_02, ihsv_vgain, cur_ihsv_v_gain);
				PE_CEO_PE1_H15A0_QWr01(pe1_cen_ctrl_03, ihsv_voffset, cur_ihsv_v_ofst);
				pInfo->pre_ihsv_v_gain=cur_ihsv_v_gain;
				pInfo->pre_ihsv_v_ofst=cur_ihsv_v_ofst;
			}
			if(_g_cmn_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					printk("[stat] tnrw_off:%d, cur_lvctrl gain:%d,ofst:%d\n",\
						tnrw_off, pInfo->cur_lvctrl_gain, pInfo->cur_lvctrl_ofst);
					printk("[stat] lvctrl gain:%d,ofst:%d, ihsv_v gain:%d,ofst:%d\n", \
						cur_lvctrl_gain, cur_lvctrl_ofst, cur_ihsv_v_gain, cur_ihsv_v_ofst);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	#endif
	return ret;
}
/**
 * download db
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_CMN_HW_H15_DownloadDb(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	#ifdef PE_CMN_HW_H15_DB_MODE_FOR_RESUME
	PE_DDR_H15_MD0_T *p_md0;
	#endif
	PE_CMN_HW_H15_SETTINGS_T *p_inf = &_g_pe_cmn_hw_h15_info;
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = NULL;
	volatile PE_DDR_DB_PHYS_REG_H15_T *p_phys_db = NULL;
	gPE_DDR_DB_H15.phys.addr = NULL;
	p_shdw_db = gPE_DDR_DB_H15.shdw.data;
	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(p_shdw_db);
		if (PE_KDRV_VER_H15)
		{
			PE_PRINT_NOTI("DL db start(resume:%d,init:%d).\n", \
				pstParams->resume_mode, p_inf->db_initialized);
			if (!p_inf->db_initialized)
			{
				#ifdef PE_CMN_HW_H15_DB_MODE_FOR_RESUME
				/* move db mode setting here to avoid db mode clear on resume init */
				p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
				p_md0->shp_fw_en = 0;
				p_md0->hst_fw_en = 0;
				p_md0->scl_fw_en = 1;
				p_md0->tnr_fw_en = 1;
				#ifdef PE_FWI_H15_ADD_FW_DNR_DATA
				p_md0->dnr_fw_en = 1;
				#else
				p_md0->dnr_fw_en = 0;
				#endif
				#endif
				if (!pstParams->resume_mode)
				{
					gPE_DDR_DB_H15.phys.addr = (volatile UINT32 *)ioremap(PE_H15_DDR_DB_BASE, \
						sizeof(PE_DDR_DB_PHYS_REG_H15_T));
					p_phys_db = gPE_DDR_DB_H15.phys.data;
					CHECK_KNULL(p_phys_db);
					p_phys_db->top_h.version = p_shdw_db->top_h.version = PE_DDR_DB_VERSION;
					p_phys_db->top_h.db_base = p_shdw_db->top_h.db_base = PE_H15_DDR_DB_BASE;
					p_phys_db->top_h.sec_num = p_shdw_db->top_h.sec_num = PE_DDR_H15_SEC_NUM;
					/* *********************************************************************/
					/* Causion : Should be arranged in enum order **************************/
					/* *********************************************************************/
					/* mode : PE_DDR_H15_SEC_MODE */
					ret = PE_CMN_HW_H15_DownloadModeDb();
					PE_CMN_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_CMN_HW_H15_DownloadModeDb() error.\n",__F__,__L__);
					/* drc : PE_DDR_H15_SEC_DRC_LUT */
					ret = PE_DCM_HW_H15_DownloadDrcDb();
					PE_CMN_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_DCM_HW_H15_DownloadDrcDb() error.\n",__F__,__L__);
					/* scl : PE_DDR_H15_SEC_SCL_H12T_M ~ PE_DDR_H15_SEC_SCL_V06T_M */
					ret = PE_SHP_HW_H15_DownloadSclDb();
					PE_CMN_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_SHP_HW_H15_DownloadSclDb() error.\n",__F__,__L__);
					/* tnr : PE_DDR_H15_SEC_TNR_LUT ~ PE_DDR_H15_SEC_TNR_USER */
					ret = PE_NRD_HW_H15_DownloadTnrDb();
					PE_CMN_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_NRD_HW_H15_DownloadTnrDb() error.\n",__F__,__L__);
					/* cvd : PE_DDR_H15_SEC_CVD_DFLT */
					ret = PE_CMN_HW_H15_DownloadCvdDb();
					PE_CMN_HW_H15_CHECK_CODE(ret, break, \
						"[%s,%d] PE_CMN_HW_H15_DownloadCvdDb() error.\n",__F__,__L__);
				}
				/* inform load db data */
				ret = PE_FWI_H15_LoadDbData(p_shdw_db->top_h.db_base);
				PE_CMN_HW_H15_CHECK_CODE(ret, break, \
					"[%s,%d] PE_FWI_H15_LoadDbData() error.\n",__F__,__L__);
				/* *********************************************************************/
				PE_PRINT_NOTI("DL db done.(md:0x%08x)(base:0x%08x)\n", p_inf->db_mode_data[0], PE_H15_DDR_DB_BASE);
				p_inf->db_initialized = 1;
			}
			else
			{
				PE_PRINT_NOTI("DL db done already.(md:0x%08x)(base:0x%08x)\n", p_inf->db_mode_data[0], PE_H15_DDR_DB_BASE);
			}
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	if (gPE_DDR_DB_H15.phys.addr) iounmap((void *)gPE_DDR_DB_H15.phys.addr);
	return ret;
}
/**
 * download mode db
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_HW_H15_DownloadModeDb(void)
{
	int ret = RET_OK;
	UINT32 size = 0, sec_typ = 0;
	#ifdef PE_CMN_HW_H15_DB_MODE_FOR_RESUME
	#else
	PE_DDR_H15_MD0_T *p_md0;
	#endif
	PE_CMN_HW_H15_SETTINGS_T *p_inf = &_g_pe_cmn_hw_h15_info;
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;
	volatile PE_DDR_DB_PHYS_REG_H15_T *p_phys_db = gPE_DDR_DB_H15.phys.data;
	do{
		CHECK_KNULL(p_shdw_db);
		CHECK_KNULL(p_phys_db);
		if (PE_KDRV_VER_H15)
		{
			/* header */
			sec_typ = PE_DDR_H15_SEC_MODE;
			p_shdw_db->sec_d[sec_typ].header.version   = 0x1;
			p_shdw_db->sec_d[sec_typ].header.sec_type  = sec_typ;
			p_shdw_db->sec_d[sec_typ].header.item_num  = PE_DDR_H15_MODE_ITEM_NUM;
			p_shdw_db->sec_d[sec_typ].header.item_size = PE_DDR_H15_MODE_SIZE;
			p_phys_db->mode.header.version   = p_shdw_db->sec_d[sec_typ].header.version;
			p_phys_db->mode.header.sec_type  = p_shdw_db->sec_d[sec_typ].header.sec_type;
			p_phys_db->mode.header.item_num  = p_shdw_db->sec_d[sec_typ].header.item_num;
			p_phys_db->mode.header.item_size = p_shdw_db->sec_d[sec_typ].header.item_size;
			/* data */
			size = (PE_DDR_H15_MODE_ITEM_NUM * PE_DDR_H15_MODE_SIZE * sizeof(UINT32));
			p_shdw_db->top_h.sec_base[sec_typ] = \
				p_shdw_db->top_h.db_base+sizeof(PE_DDR_H15_TOP_HEADER_T);
			p_shdw_db->top_h.sec_size[sec_typ] = size + sizeof(PE_DDR_H15_HEADER_T);
			p_phys_db->top_h.sec_base[sec_typ] = p_shdw_db->top_h.sec_base[sec_typ];
			p_phys_db->top_h.sec_size[sec_typ] = p_shdw_db->top_h.sec_size[sec_typ];
			p_shdw_db->sec_d[sec_typ].data_base = \
				p_shdw_db->top_h.sec_base[sec_typ] + sizeof(PE_DDR_H15_HEADER_T);
			/* wr ddr */
			#ifdef PE_CMN_HW_H15_DB_MODE_FOR_RESUME
			#else
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			p_md0->shp_fw_en = 0;
			p_md0->hst_fw_en = 0;
			p_md0->scl_fw_en = 1;
			p_md0->tnr_fw_en = 1;
			#ifdef PE_FWI_H15_ADD_FW_DNR_DATA
			p_md0->dnr_fw_en = 1;
			#else
			p_md0->dnr_fw_en = 0;
			#endif
			#endif
			p_phys_db->mode.data[0] = p_inf->db_mode_data[0];
			/* noti */
			PE_PRINT_NOTI("%s: DL done.(base:0x%08x)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), p_shdw_db->top_h.sec_base[sec_typ]);
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}
/**
 * download cvd db
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_CMN_HW_H15_DownloadCvdDb(void)
{
	int ret = RET_OK;
	UINT32 size, sec_typ;
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;
	volatile PE_DDR_DB_PHYS_REG_H15_T *p_phys_db = gPE_DDR_DB_H15.phys.data;
	volatile UINT32 *p_cvd_dflt_data  = p_phys_db->cvd_dflt.data;
	do{
		CHECK_KNULL(p_shdw_db);
		CHECK_KNULL(p_phys_db);
		CHECK_KNULL(p_cvd_dflt_data);
		if (PE_KDRV_VER_H15)
		{
			/* cvd_dflt **************************************************************/
			sec_typ = PE_DDR_H15_SEC_CVD_DFLT;
			/* header */
			p_shdw_db->sec_d[sec_typ].header.version   = 0x1;
			p_shdw_db->sec_d[sec_typ].header.sec_type  = sec_typ;
			p_shdw_db->sec_d[sec_typ].header.item_num  = PE_DDR_H15_CVD_DFLT_ITEM_NUM;
			p_shdw_db->sec_d[sec_typ].header.item_size = PE_DDR_H15_CVD_DFLT_SIZE;
			p_phys_db->cvd_dflt.header.version   = p_shdw_db->sec_d[sec_typ].header.version;
			p_phys_db->cvd_dflt.header.sec_type  = p_shdw_db->sec_d[sec_typ].header.sec_type;
			p_phys_db->cvd_dflt.header.item_num  = p_shdw_db->sec_d[sec_typ].header.item_num;
			p_phys_db->cvd_dflt.header.item_size = p_shdw_db->sec_d[sec_typ].header.item_size;
			/* data */
			size = (PE_DDR_H15_CVD_DFLT_ITEM_NUM * PE_DDR_H15_CVD_DFLT_SIZE * sizeof(UINT32));
			p_shdw_db->top_h.sec_base[sec_typ] = \
				p_shdw_db->top_h.sec_base[sec_typ-1]+p_shdw_db->top_h.sec_size[sec_typ-1];
			p_shdw_db->top_h.sec_size[sec_typ] = size + sizeof(PE_DDR_H15_HEADER_T);
			p_phys_db->top_h.sec_base[sec_typ] = p_shdw_db->top_h.sec_base[sec_typ];
			p_phys_db->top_h.sec_size[sec_typ] = p_shdw_db->top_h.sec_size[sec_typ];
			p_shdw_db->sec_d[sec_typ].data_base = \
				p_shdw_db->top_h.sec_base[sec_typ] + sizeof(PE_DDR_H15_HEADER_T);
			/* wr ddr */
			ret = CVD_H15Ax_SetPqModeToDdr(p_cvd_dflt_data);
			PE_CMN_HW_H15_CHECK_CODE(ret, break, \
				"[%s,%d] CVD_H15Ax_SetPqModeToDdr() error.\n",__F__,__L__);
			/* noti */
			PE_PRINT_NOTI("%s: DL done.(base:0x%08x)\n", \
				PE_DDR_REG_H15_SEC_TYPE_TO_STR(sec_typ), p_shdw_db->top_h.sec_base[sec_typ]);
		}
		else
		{
			PE_CMN_HW_H15_DBG_PRINT("nothing to do.\n");
			ret = RET_OK;
		}
	}while (0);
	return ret;
}

/**
 * get db info.
 *
 * @param   inf_type [in] UINT32, see PE_CMN_HW_H15_DB_INF_TYPE
 * @param   sec_type [in] UINT32, see PE_DDR_H15_SEC_TYPE
 * @return  UINT32.
 * @see
 * @author
 */
UINT32 PE_CMN_HW_H15_GetDbInfo(UINT32 inf_type, UINT32 sec_type)
{
	UINT32 ret = 0;
	PE_DDR_H15_MD0_T *p_md0;
	PE_CMN_HW_H15_SETTINGS_T *p_inf = &_g_pe_cmn_hw_h15_info;
	PE_DDR_DB_SHDW_REG_H15_T *p_shdw_db = gPE_DDR_DB_H15.shdw.data;

	if (sec_type>=PE_DDR_H15_SEC_NUM)
		return ret;

	switch (inf_type)
	{
		case PE_CMN_HW_H15_DB_INF_IS_DB_INIT:
			ret = p_inf->db_initialized;
			break;
		case PE_CMN_HW_H15_DB_INF_TOP_VERSION:
			ret = p_shdw_db->top_h.version;
			break;
		case PE_CMN_HW_H15_DB_INF_TOP_BASE:
			ret = p_shdw_db->top_h.db_base;
			break;
		case PE_CMN_HW_H15_DB_INF_TOP_SEC_NUM:
			ret = p_shdw_db->top_h.sec_num;
			break;
		case PE_CMN_HW_H15_DB_INF_SEC_BASE:
			ret = p_shdw_db->top_h.sec_base[sec_type];
			break;
		case PE_CMN_HW_H15_DB_INF_SEC_SIZE:
			ret = p_shdw_db->top_h.sec_size[sec_type];
			break;
		case PE_CMN_HW_H15_DB_INF_SEC_VERSION:
			ret = p_shdw_db->sec_d[sec_type].header.version;
			break;
		case PE_CMN_HW_H15_DB_INF_SEC_TYPE:
			ret = p_shdw_db->sec_d[sec_type].header.sec_type;
			break;
		case PE_CMN_HW_H15_DB_INF_SEC_ITEM_NUM:
			ret = p_shdw_db->sec_d[sec_type].header.item_num;
			break;
		case PE_CMN_HW_H15_DB_INF_SEC_ITEM_SIZE:
			ret = p_shdw_db->sec_d[sec_type].header.item_size;
			break;
		case PE_CMN_HW_H15_DB_INF_SEC_DATA_BASE:
			ret = p_shdw_db->sec_d[sec_type].data_base;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_SHP_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			ret = p_md0->shp_fw_en;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_HST_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			ret = p_md0->hst_fw_en;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			ret = p_md0->scl_fw_en;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_TNR_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			ret = p_md0->tnr_fw_en;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_DNR_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			ret = p_md0->dnr_fw_en;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

/**
 * set db info.
 * - I think that shdw db modification is not good.
 *
 * @param   inf_type [in] UINT32, see PE_CMN_HW_H15_DB_INF_TYPE
 * @param   sec_type [in] UINT32, see PE_DDR_H15_SEC_TYPE
 * @param   value [in] UINT32
 * @return  UINT32.
 * @see
 * @author
 */
UINT32 PE_CMN_HW_H15_SetDbInfo(UINT32 inf_type, UINT32 sec_type, UINT32 value)
{
	UINT32 ret = 0;
	PE_DDR_H15_MD0_T *p_md0;
	PE_CMN_HW_H15_SETTINGS_T *p_inf = &_g_pe_cmn_hw_h15_info;

	if (sec_type>=PE_DDR_H15_SEC_NUM)
		return ret;

	switch (inf_type)
	{
		case PE_CMN_HW_H15_DB_INF_MD0_SHP_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			p_md0->shp_fw_en = value? 1:0;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_HST_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			p_md0->hst_fw_en = value? 1:0;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_SCL_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			p_md0->scl_fw_en = value? 1:0;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_TNR_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			p_md0->tnr_fw_en = value? 1:0;
			break;
		case PE_CMN_HW_H15_DB_INF_MD0_DNR_FW_EN:
			p_md0 = (PE_DDR_H15_MD0_T *)p_inf->db_mode_data;
			p_md0->dnr_fw_en = value? 1:0;
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}

