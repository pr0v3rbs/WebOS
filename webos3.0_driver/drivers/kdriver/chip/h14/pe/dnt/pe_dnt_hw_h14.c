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

/** @file pe_dnt_hw_h14.c
 *
 *  driver for picture enhance deinterlacer functions. ( used only within kdriver )
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
#include "pe_dnt_hw_h14.h"
#include "pe_dnt_hw_h14a0.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_DNT_HW_H14_ERROR	printk

#define PE_DNT_HW_H14_DBG_PRINT(fmt,args...)	\
	if(_g_dnt_hw_h14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_DNT_HW_H14_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_DNT_HW_H14_ERROR(fmt,##args);_action;}}
#define PE_DNT_HW_H14_DBG_PRINT_FMT_STATE(_pre,_cur)	\
	PE_DNT_HW_H14_DBG_PRINT("%s(%d),%s(%d) : %s.\n",\
	#_pre,(_pre),#_cur,(_cur),((_pre)!=(_cur))? "different":"same")

#define PE_DNT_HW_H14_SET_CLC_REG_TABLE(_mdl_,_tbl,_ofs)	\
	do{\
		PE_DNT_HW_H14_DBG_PRINT("write %s.\n",#_tbl);\
		ret = PE_DNT_HW_H14_SetClcDfltTbl((_tbl),(_ofs),sizeof(_tbl));\
		PE_DNT_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_DNT_HW_H14_SetClcDfltTbl() error.\n",__F__,__L__);\
	}while(0)

#define PE_DNT_HW_H14_SET_IPC_REG_TABLE(_tbl,_ofs)	\
	do{\
		PE_DNT_HW_H14_DBG_PRINT("write %s.\n",#_tbl);\
		ret = PE_DNT_HW_H14_SetIpcDfltTbl((_tbl),(_ofs),sizeof(_tbl));\
		PE_DNT_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_DNT_HW_H14_SetIpcDfltTbl() error.\n",__F__,__L__);\
	}while(0)

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
static PE_DNT_HW_H14_IPC_FMT PE_DNT_HW_H14_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_H14_CLC_FMT PE_DNT_HW_H14_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static PE_DNT_HW_H14_VFY_FMT PE_DNT_HW_H14_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf);
static int PE_DNT_HW_H14_SetIpcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size);
static int PE_DNT_HW_H14_SetClcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_dnt_hw_h14_trace=0x0;		//default should be off.
static PE_DNT_HW_H14_SETTINGS_T _g_pe_dnt_hw_h14_info;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init dnt
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H14)
		{
			_g_pe_dnt_hw_h14_info.pre_p0l_ipc_ctrl_01 = 0x0;
			_g_pe_dnt_hw_h14_info.pre_p0r_ipc_ctrl_01 = 0x0;
			_g_pe_dnt_hw_h14_info.cur_p0l_clc_ctrl_00 = 0x0;
			_g_pe_dnt_hw_h14_info.pre_p0l_clc_ctrl_00 = 0x0;
			PE_DNT_HW_H14_DBG_PRINT("init ipc, tpd.\n");
			/* init ipc */
			PE_SET_REG_TABLE(DNT_HW_H14,ipc_l_init_h14a0,PE_REG_H14_WRITE_BASE);
			PE_SET_REG_TABLE(DNT_HW_H14,ipc_l_init_h14a0,PE_PE0_REG_H14_LRDIFF);
			/* init clc */
			PE_SET_REG_TABLE(DNT_HW_H14,clc_l_init_h14a0,PE_REG_H14_WRITE_BASE);
			/* init vflt y */
			PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_init_h14a0,PE_REG_H14_WRITE_BASE);
			PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_init_h14a0,PE_PE0_REG_H14_LRDIFF);
		}
		else
		{
			PE_DNT_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);	
	return ret;
}
/**
 * debug setting
 *
 * @param   *pstParams [in]LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_DNT_HW_H14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(DNT)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_dnt_hw_h14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(DNT)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}

/**
 * set dnt default(for H14)
 *
 * @param   *pstParams [in] LX_PE_DEFAULT_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_INF_H14_SETTINGS_T inf_set;
	LX_PE_INF_DISPLAY_T *cur0_inf = NULL;
	LX_PE_INF_DISPLAY_T *cur1_inf = NULL;
	PE_DNT_HW_H14_IPC_FMT cur0_ipc_fmt,cur1_ipc_fmt;
	PE_DNT_HW_H14_CLC_FMT cur0_clc_fmt;
	PE_DNT_HW_H14_VFY_FMT cur0_vfy_fmt,cur1_vfy_fmt;
	static PE_DNT_HW_H14_IPC_FMT pre0_ipc_fmt = PE_DNT_H14_IPC_NUM;
	static PE_DNT_HW_H14_IPC_FMT pre1_ipc_fmt = PE_DNT_H14_IPC_NUM;
	static PE_DNT_HW_H14_CLC_FMT pre0_clc_fmt = PE_DNT_H14_CLC_NUM;
	static PE_DNT_HW_H14_VFY_FMT pre0_vfy_fmt = PE_DNT_H14_VFY_NUM;
	static PE_DNT_HW_H14_VFY_FMT pre1_vfy_fmt = PE_DNT_H14_VFY_NUM;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		ret = PE_INF_H14_GetCurInfSettings(&inf_set);
		PE_DNT_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_INF_H14_GetCurInfSettings() error.\n",__F__,__L__);
		cur0_inf=&inf_set.disp_info[LX_PE_WIN_0];
		cur1_inf=&inf_set.disp_info[LX_PE_WIN_1];
		CHECK_KNULL(cur0_inf);
		CHECK_KNULL(cur1_inf);
		/* L : ipc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_ipc_fmt = PE_DNT_HW_H14_ConvDispInfoToIpcFmt(cur0_inf);
			PE_DNT_HW_H14_DBG_PRINT_FMT_STATE(pre0_ipc_fmt,cur0_ipc_fmt);
			if(pre0_ipc_fmt!=cur0_ipc_fmt)
			{
				switch(cur0_ipc_fmt)
				{
					case PE_DNT_H14_IPC_ATV_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_atv_480_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_ATV_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_atv_576_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_DTV_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_dtv_480_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_DTV_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_dtv_576_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_DTV_HD:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_dtv_hd_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_AV_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_av_480_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_AV_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_av_576_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_SCARTRGB:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_scartrgb_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_SD_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_sd_480_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_SD_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_sd_576_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_HD:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_hd_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_IPC_PC:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_pc_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					default:
						break;
				}
				#if 0	// changed_to_fw_control
				PE_P1L_H14_RdFL(pe1_dse_ctrl_01);
				PE_P1L_H14_Wr01(pe1_dse_ctrl_01, reg_fmd_en_0,	0x1);
				PE_P1L_H14_WrFL(pe1_dse_ctrl_01);
				#endif
				pre0_ipc_fmt=cur0_ipc_fmt;
			}
		}
		/* R : ipc */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_ipc_fmt = PE_DNT_HW_H14_ConvDispInfoToIpcFmt(cur1_inf);
			PE_DNT_HW_H14_DBG_PRINT_FMT_STATE(pre1_ipc_fmt,cur1_ipc_fmt);
			if(pre1_ipc_fmt!=cur1_ipc_fmt)
			{
				switch(cur1_ipc_fmt)
				{
					case PE_DNT_H14_IPC_ATV_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_atv_480_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_ATV_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_atv_576_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_DTV_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_dtv_480_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_DTV_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_dtv_576_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_DTV_HD:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_dtv_hd_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_AV_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_av_480_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_AV_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_av_576_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_SCARTRGB:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_scartrgb_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_SD_480:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_sd_480_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_SD_576:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_sd_576_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_HD:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_hd_default_h14a0,		PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_IPC_PC:
						PE_DNT_HW_H14_SET_IPC_REG_TABLE(ipc_l_pc_default_h14a0,		PE_PE0_REG_H14_LRDIFF);
						break;
					default:
						break;
				}
				#if 0	// changed_to_fw_control
				PE_P1L_H14_RdFL(pe1_dse_ctrl_01);
				PE_P1L_H14_Wr01(pe1_dse_ctrl_01, reg_fmd_en_1,	0x1);
				PE_P1L_H14_WrFL(pe1_dse_ctrl_01);
				#endif
				pre1_ipc_fmt=cur1_ipc_fmt;
			}
		}
		/* L : clc */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_clc_fmt = PE_DNT_HW_H14_ConvDispInfoToClcFmt(cur0_inf);
			PE_DNT_HW_H14_DBG_PRINT_FMT_STATE(pre0_clc_fmt,cur0_clc_fmt);
			if(pre0_clc_fmt!=cur0_clc_fmt)
			{
				switch(cur0_clc_fmt)
				{
					case PE_DNT_H14_CLC_AV_480:
						PE_DNT_HW_H14_SET_CLC_REG_TABLE(DNT_HW_H14,clc_l_av_480_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_CLC_AV_576:
						PE_DNT_HW_H14_SET_CLC_REG_TABLE(DNT_HW_H14,clc_l_av_576_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_CLC_SD_50:
						PE_DNT_HW_H14_SET_CLC_REG_TABLE(DNT_HW_H14,clc_l_sd_50_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_CLC_SD_60:
						PE_DNT_HW_H14_SET_CLC_REG_TABLE(DNT_HW_H14,clc_l_sd_60_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_CLC_HD_50:
						PE_DNT_HW_H14_SET_CLC_REG_TABLE(DNT_HW_H14,clc_l_hd_50_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_CLC_HD_60:
						PE_DNT_HW_H14_SET_CLC_REG_TABLE(DNT_HW_H14,clc_l_hd_60_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					default:
						break;
				}
				pre0_clc_fmt=cur0_clc_fmt;
			}
		}
		/* L : vflt y */
		if(PE_CHECK_WIN0(pstParams->win_id))
		{
			cur0_vfy_fmt = PE_DNT_HW_H14_ConvDispInfoToVfyFmt(cur0_inf);
			PE_DNT_HW_H14_DBG_PRINT_FMT_STATE(pre0_vfy_fmt,cur0_vfy_fmt);
			if(pre0_vfy_fmt!=cur0_vfy_fmt)
			{
				switch(cur0_vfy_fmt)
				{
					case PE_DNT_H14_VFY_SD:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_sd_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_VFY_HD_I:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_hd_i_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_VFY_HD_P:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_hd_p_default_h14a0,	PE_REG_H14_WRITE_BASE);
						break;
					case PE_DNT_H14_VFY_PC:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_pc_default_h14a0,		PE_REG_H14_WRITE_BASE);
						break;
					default:
						break;
				}
				pre0_vfy_fmt=cur0_vfy_fmt;
			}
		}
		/* R : vflt y */
		if(PE_CHECK_WIN1(pstParams->win_id))
		{
			cur1_vfy_fmt = PE_DNT_HW_H14_ConvDispInfoToVfyFmt(cur1_inf);
			PE_DNT_HW_H14_DBG_PRINT_FMT_STATE(pre1_vfy_fmt,cur1_vfy_fmt);
			if(pre1_vfy_fmt!=cur1_vfy_fmt)
			{
				switch(cur1_vfy_fmt)
				{
					case PE_DNT_H14_VFY_SD:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_sd_default_h14a0,		PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_VFY_HD_I:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_hd_i_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_VFY_HD_P:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_hd_p_default_h14a0,	PE_PE0_REG_H14_LRDIFF);
						break;
					case PE_DNT_H14_VFY_PC:
						PE_SET_REG_TABLE(DNT_HW_H14,vflt_y_l_pc_default_h14a0,		PE_PE0_REG_H14_LRDIFF);
						break;
					default:
						break;
				}
				pre1_vfy_fmt=cur1_vfy_fmt;
			}
		}
	}while(0);	
	return ret;
}
/**
 * get ipc fmt using display info(for H14)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static PE_DNT_HW_H14_IPC_FMT PE_DNT_HW_H14_ConvDispInfoToIpcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_H14_IPC_FMT ipc_fmt = PE_DNT_H14_IPC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
			if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_H14_IPC_ATV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_H14_IPC_ATV_576;
			}
			break;
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			if(disp_inf->src_type==LX_PE_SRC_SCART && \
				disp_inf->scart_type==LX_PE_SCART_RGB)
			{
				ipc_fmt = PE_DNT_H14_IPC_SCARTRGB;
			}
			else if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
			{
				ipc_fmt = PE_DNT_H14_IPC_AV_480;
			}
			else
			{
				ipc_fmt = PE_DNT_H14_IPC_AV_576;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				ipc_fmt = PE_DNT_H14_IPC_PC;
			}
			break;
		case LX_PE_SRC_DTV:
			if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						ipc_fmt = PE_DNT_H14_IPC_DTV_480;
					}
					else
					{
						ipc_fmt = PE_DNT_H14_IPC_DTV_576;
					}
				}
				else
				{
					ipc_fmt = PE_DNT_H14_IPC_DTV_HD;
				}
			}
			else
			{
				ipc_fmt = PE_DNT_H14_IPC_PC;
			}
			break;
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI || \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				ipc_fmt = PE_DNT_H14_IPC_PC;
			}
			else if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
			{
				if(disp_inf->fmt_type==LX_PE_FMT_SD)
				{
					if(disp_inf->fr_type==LX_PE_FR_60HZ)
					{
						ipc_fmt = PE_DNT_H14_IPC_SD_480;
					}
					else
					{
						ipc_fmt = PE_DNT_H14_IPC_SD_576;
					}
				}
				else
				{
					ipc_fmt = PE_DNT_H14_IPC_HD;
				}
			}
			else
			{
				ipc_fmt = PE_DNT_H14_IPC_PC;
			}
			break;
	}
	return ipc_fmt;
}
/**
 * get clc fmt using display info(for H14)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_H14_CLC_FMT
 * @see
 * @author
 */
static PE_DNT_HW_H14_CLC_FMT PE_DNT_HW_H14_ConvDispInfoToClcFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_H14_CLC_FMT clc_fmt = PE_DNT_H14_CLC_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				if(disp_inf->cstd_type==LX_PE_CSTD_NTSC)
					clc_fmt = PE_DNT_H14_CLC_AV_480;
				else
					clc_fmt = PE_DNT_H14_CLC_AV_576;
			}
			break;
		case LX_PE_SRC_RGBPC:
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				if(disp_inf->fr_type== LX_PE_FR_50HZ)
					clc_fmt = PE_DNT_H14_CLC_SD_50;
				else
					clc_fmt = PE_DNT_H14_CLC_SD_60;
			}
			else
			{
				if(disp_inf->fr_type== LX_PE_FR_50HZ)
					clc_fmt = PE_DNT_H14_CLC_HD_50;
				else
					clc_fmt = PE_DNT_H14_CLC_HD_60;
			}
			break;
	}
	return clc_fmt;
}
/**
 * get vflt y fmt using display info(for H14)
 *
 * @param   *disp_inf [in] LX_PE_INF_DISPLAY_T
 * @return  PE_DNT_HW_H14_VFY_FMT
 * @see
 * @author
 */
static PE_DNT_HW_H14_VFY_FMT PE_DNT_HW_H14_ConvDispInfoToVfyFmt(LX_PE_INF_DISPLAY_T *disp_inf)
{
	PE_DNT_HW_H14_VFY_FMT vfy_fmt = PE_DNT_H14_VFY_NUM;
	switch(disp_inf->src_type)
	{
		case LX_PE_SRC_ATV:
		case LX_PE_SRC_CVBS:
		case LX_PE_SRC_SCART:
			{
				vfy_fmt = PE_DNT_H14_VFY_SD;
			}
			break;
		case LX_PE_SRC_RGBPC:
			{
				vfy_fmt = PE_DNT_H14_VFY_PC;
			}
			break;
		case LX_PE_SRC_DTV:
		case LX_PE_SRC_COMP:
		case LX_PE_SRC_HDMI:
		default:
			if(disp_inf->src_type==LX_PE_SRC_HDMI && \
				disp_inf->hdmi_type==LX_PE_HDMI_PC)
			{
				vfy_fmt = PE_DNT_H14_VFY_PC;
			}
			else if(disp_inf->fmt_type==LX_PE_FMT_SD)
			{
				vfy_fmt = PE_DNT_H14_VFY_SD;
			}
			else
			{
				if(disp_inf->scan_type==LX_PE_SCAN_INTERLACE)
				{
					vfy_fmt = PE_DNT_H14_VFY_HD_I;
				}
				else
				{
					vfy_fmt = PE_DNT_H14_VFY_HD_P;
				}
			}
			break;
	}
	return vfy_fmt;
}

/**
 * set ipc default settings(for H14)
 *	- keep 0x0500,0x2500[1:0]cr_luma_mode,[5:4]cr_chroma_mode,[31]cr_film_mode_enable
 *
 * @param   *table [in] PE_REG_PARAM_T
 * @param   offset [in] UINT32
 * @param   size [in] UINT32
 * @return  void
 * @see
 * @author
 */
static int PE_DNT_HW_H14_SetIpcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size)
{
	int ret = RET_OK;
	UINT32 table_size,i,r_data;
	PE_REG_PARAM_T *p_data = NULL;
	PE_H14A0_P0L_IPC_CTRL_00_T *p0l_rd_ipc_ctrl_00 = NULL;
	PE_H14A0_P0L_IPC_CTRL_00_T *p0l_wr_ipc_ctrl_00 = NULL;
	PE_H14A0_P0R_IPC_CTRL_00_T *p0r_rd_ipc_ctrl_00 = NULL;
	PE_H14A0_P0R_IPC_CTRL_00_T *p0r_wr_ipc_ctrl_00 = NULL;
	#ifdef PE_DNT_HW_H14_SET_GAME_MODE_BY_FW
	PE_H14A0_P0L_IPC_CTRL_01_T *p0l_rd_ipc_ctrl_01 = NULL;
	PE_H14A0_P0L_IPC_CTRL_01_T *p0l_wr_ipc_ctrl_01 = NULL;
	PE_H14A0_P0R_IPC_CTRL_01_T *p0r_rd_ipc_ctrl_01 = NULL;
	PE_H14A0_P0R_IPC_CTRL_01_T *p0r_wr_ipc_ctrl_01 = NULL;
	PE_DNT_HW_H14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h14_info;
	#endif
	do {
		PE_DNT_HW_H14_CHECK_CODE(!table,break,"[%s,%d] table is null.\n",__F__,__L__);
		PE_DNT_HW_H14_CHECK_CODE(!size,break,"[%s,%d] size is zero.\n",__F__,__L__);
		p_data = (PE_REG_PARAM_T *)OS_Malloc(size);
		PE_DNT_HW_H14_CHECK_CODE(!p_data,break,"[%s,%d] p_data is null.\n",__F__,__L__);
		table_size = size/sizeof(PE_REG_PARAM_T);
		for(i=0;i<table_size;i++)
		{
			p_data[i].addr = table[i].addr;
			if((table[i].addr+offset)==0x0500)
			{
				p_data[i].data = table[i].data;
				PE_P0L_H14_RdFL(p0l_ipc_ctrl_00);
				r_data = PE_P0L_H14_Rd(p0l_ipc_ctrl_00);
				p0l_rd_ipc_ctrl_00 = (PE_H14A0_P0L_IPC_CTRL_00_T *)&(r_data);
				p0l_wr_ipc_ctrl_00 = (PE_H14A0_P0L_IPC_CTRL_00_T *)&(p_data[i].data);
				p0l_wr_ipc_ctrl_00->cr_film_mode_enable = p0l_rd_ipc_ctrl_00->cr_film_mode_enable;
			}
			else if((table[i].addr+offset)==0x2500)
			{
				p_data[i].data = table[i].data;
				PE_P0R_H14_RdFL(p0r_ipc_ctrl_00);
				r_data = PE_P0R_H14_Rd(p0r_ipc_ctrl_00);
				p0r_rd_ipc_ctrl_00 = (PE_H14A0_P0R_IPC_CTRL_00_T *)&(r_data);
				p0r_wr_ipc_ctrl_00 = (PE_H14A0_P0R_IPC_CTRL_00_T *)&(p_data[i].data);
				p0r_wr_ipc_ctrl_00->cr_film_mode_enable = p0r_rd_ipc_ctrl_00->cr_film_mode_enable;
			}
			#ifdef PE_DNT_HW_H14_SET_GAME_MODE_BY_FW
			else if((table[i].addr+offset)==0x0504)
			{
				p_data[i].data = table[i].data;
				PE_P0L_H14_RdFL(p0l_ipc_ctrl_01);
				r_data = PE_P0L_H14_Rd(p0l_ipc_ctrl_01);
				p0l_rd_ipc_ctrl_01 = (PE_H14A0_P0L_IPC_CTRL_01_T *)&(r_data);
				p0l_wr_ipc_ctrl_01 = (PE_H14A0_P0L_IPC_CTRL_01_T *)&(p_data[i].data);
				p0l_wr_ipc_ctrl_01->cr_game_mode = p0l_rd_ipc_ctrl_01->cr_game_mode;
				/* store default */
				pInfo->pre_p0l_ipc_ctrl_01 = table[i].data;
			}
			else if((table[i].addr+offset)==0x2504)
			{
				p_data[i].data = table[i].data;
				PE_P0R_H14_RdFL(p0r_ipc_ctrl_01);
				r_data = PE_P0R_H14_Rd(p0r_ipc_ctrl_01);
				p0r_rd_ipc_ctrl_01 = (PE_H14A0_P0R_IPC_CTRL_01_T *)&(r_data);
				p0r_wr_ipc_ctrl_01 = (PE_H14A0_P0R_IPC_CTRL_01_T *)&(p_data[i].data);
				p0r_wr_ipc_ctrl_01->cr_game_mode = p0r_rd_ipc_ctrl_01->cr_game_mode;
				/* store default */
				pInfo->pre_p0r_ipc_ctrl_01 = table[i].data;
			}
			#endif
			else
			{
				p_data[i].data = table[i].data;
			}
		}
		ret = PE_REG_SetTable(p_data,offset,size);
		PE_DNT_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_REG_SetTable() error.\n",__F__,__L__);
	}while(0);
	if(p_data){OS_Free(p_data);}
	return ret;
}

/**
 * set clc default settings(for H14)
 *	- keep 0x0560 [1]cl_filter_enable
 *
 * @param   *table [in] PE_REG_PARAM_T
 * @param   offset [in] UINT32
 * @param   size [in] UINT32
 * @return  void
 * @see
 * @author
 */
static int PE_DNT_HW_H14_SetClcDfltTbl(const PE_REG_PARAM_T *table, UINT32 offset, UINT32 size)
{
	int ret = RET_OK;
	UINT32 table_size,i,r_data;
	PE_REG_PARAM_T *p_data = NULL;
	PE_H14A0_P0L_CLC_CTRL_00_T *p0l_rd_clc_ctrl_00 = NULL;
	PE_H14A0_P0L_CLC_CTRL_00_T *p0l_wr_clc_ctrl_00 = NULL;
	PE_DNT_HW_H14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h14_info;
	do {
		PE_DNT_HW_H14_CHECK_CODE(!table,break,"[%s,%d] table is null.\n",__F__,__L__);
		PE_DNT_HW_H14_CHECK_CODE(!size,break,"[%s,%d] size is zero.\n",__F__,__L__);
		p_data = (PE_REG_PARAM_T *)OS_Malloc(size);
		PE_DNT_HW_H14_CHECK_CODE(!p_data,break,"[%s,%d] p_data is null.\n",__F__,__L__);
		table_size = size/sizeof(PE_REG_PARAM_T);
		for(i=0;i<table_size;i++)
		{
			p_data[i].addr = table[i].addr;
			if((table[i].addr+offset)==0x0560)
			{
				p_data[i].data = table[i].data;
				PE_P0L_H14_RdFL(p0l_clc_ctrl_00);
				r_data = PE_P0L_H14_Rd(p0l_clc_ctrl_00);
				p0l_rd_clc_ctrl_00 = (PE_H14A0_P0L_CLC_CTRL_00_T *)&(r_data);
				p0l_wr_clc_ctrl_00 = (PE_H14A0_P0L_CLC_CTRL_00_T *)&(p_data[i].data);
				p0l_wr_clc_ctrl_00->cl_filter_enable = p0l_rd_clc_ctrl_00->cl_filter_enable;
				p0l_wr_clc_ctrl_00->clc_detection_enable = p0l_rd_clc_ctrl_00->clc_detection_enable;
				/* store default */
				pInfo->cur_p0l_clc_ctrl_00 = table[i].data;
			}
			else
			{
				p_data[i].data = table[i].data;
			}
		}
		ret = PE_REG_SetTable(p_data,offset,size);
		PE_DNT_HW_H14_CHECK_CODE(ret,break,"[%s,%d] PE_REG_SetTable() error.\n",__F__,__L__);
	}while(0);
	if(p_data){OS_Free(p_data);}
	return ret;
}

/**
 * set film mode
 *
 * @param   *pstParams [in]LX_PE_DNT_FILMMODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_SetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DNT_HW_H14_DBG_PRINT("set pstParams[%d] : en:%d\n",pstParams->win_id,pstParams->enable);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_H14_RdFL(pe1_dse_ctrl_01);
				PE_P1L_H14_Wr01(pe1_dse_ctrl_01, reg_fmd_en_0,	(pstParams->enable)? 0x1:0x0);
				PE_P1L_H14_WrFL(pe1_dse_ctrl_01);
				#else
				PE_P0L_H14_RdFL(p0l_ipc_ctrl_00);
				PE_P0L_H14_Wr01(p0l_ipc_ctrl_00, cr_film_mode_enable,	(pstParams->enable)? 0x1:0x0);
				PE_P0L_H14_WrFL(p0l_ipc_ctrl_00);
				#endif
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_H14_RdFL(pe1_dse_ctrl_01);
				PE_P1L_H14_Wr01(pe1_dse_ctrl_01, reg_fmd_en_1,	(pstParams->enable)? 0x1:0x0);
				PE_P1L_H14_WrFL(pe1_dse_ctrl_01);
				#else
				PE_P0R_H14_RdFL(p0r_ipc_ctrl_00);
				PE_P0R_H14_Wr01(p0r_ipc_ctrl_00, cr_film_mode_enable,	(pstParams->enable)? 0x1:0x0);
				PE_P0R_H14_WrFL(p0r_ipc_ctrl_00);
				#endif
			}
		}
		else
		{
			PE_DNT_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * get film mode
 *
 * @param   *pstParams [in/out]LX_PE_DNT_FILMMODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_GetFilmMode(LX_PE_DNT_FILMMODE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_H14_RdFL(pe1_dse_ctrl_01);
				PE_P1L_H14_Rd01(pe1_dse_ctrl_01, reg_fmd_en_0,	pstParams->enable);
				#else
				PE_P0L_H14_RdFL(p0l_ipc_ctrl_00);
				PE_P0L_H14_Rd01(p0l_ipc_ctrl_00, cr_film_mode_enable,	pstParams->enable);
				#endif
			}
			if(PE_CHECK_WIN1(win_id))
			{
				#if 1	// changed_to_fw_control
				PE_P1L_H14_RdFL(pe1_dse_ctrl_01);
				PE_P1L_H14_Rd01(pe1_dse_ctrl_01, reg_fmd_en_1,	pstParams->enable);
				#else
				PE_P0R_H14_RdFL(p0r_ipc_ctrl_00);
				PE_P0R_H14_Rd01(p0r_ipc_ctrl_00, cr_film_mode_enable,	pstParams->enable);
				#endif
			}
		}
		else
		{
			PE_DNT_HW_H14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_DNT_HW_H14_DBG_PRINT("get pstParams[%d] : en:%d\n",pstParams->win_id,pstParams->enable);
	} while (0);
	return ret;
}
/**
 * set low delay mode : control game_en,clc_en
 *
 * @param   *pstParams [in] LX_PE_DNT_LD_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_SetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	UINT32 cr_game_en;
	#ifdef PE_DNT_HW_H14_SET_GAME_MODE_BY_FW
	#else
	PE_H14A0_P0L_IPC_CTRL_01_T *p0l_rd_ipc_ctrl_01 = NULL;
	PE_DNT_HW_H14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h14_info;
	#endif
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_DNT_HW_H14_DBG_PRINT("set[%d]:en:%d\n",pstParams->win_id,pstParams->enable);
		if(PE_KDRV_VER_H14)
		{
			#ifdef PE_DNT_HW_H14_SET_GAME_MODE_BY_FW
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_H14_DBG_PRINT("set0: not support, game(%d) by fw\n", cr_game_en);
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_DNT_HW_H14_DBG_PRINT("set0: not support, game(%d) by fw\n", cr_game_en);
			}
			#else
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_P0L_H14_RdFL(p0l_ipc_ctrl_01);
				PE_P0L_H14_Wr01(p0l_ipc_ctrl_01, cr_game_mode,			cr_game_en);
				PE_P0L_H14_WrFL(p0l_ipc_ctrl_01);
				p0l_rd_ipc_ctrl_01 = (PE_H14A0_P0L_IPC_CTRL_01_T *)&(pInfo->pre_p0l_ipc_ctrl_01);
				p0l_rd_ipc_ctrl_01->cr_game_mode = cr_game_en;
				PE_DNT_HW_H14_DBG_PRINT("set0:game:0x%x\n",cr_game_en);
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				cr_game_en = (pstParams->enable)? 0x1:0x0;
				PE_P0R_H14_RdFL(p0r_ipc_ctrl_01);
				PE_P0R_H14_Wr01(p0r_ipc_ctrl_01, cr_game_mode,			cr_game_en);
				PE_P0R_H14_WrFL(p0r_ipc_ctrl_01);
				PE_DNT_HW_H14_DBG_PRINT("set1:game:0x%x\n",cr_game_en);
			}
			#endif
		}
		else
		{
			PE_DNT_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get low delay mode : game_en
 *
 * @param   *pstParams [in] LX_PE_DNT_LD_MODE_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_GetLowDelayMode(LX_PE_DNT_LD_MODE_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_DNT_HW_H14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h14_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H14)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				PE_P0L_H14_RdFL(p0l_ipc_ctrl_01);
				PE_P0L_H14_Rd01(p0l_ipc_ctrl_01, cr_game_mode,	pstParams->enable);
				PE_DNT_HW_H14_DBG_PRINT("inf0:clc_ctrl_00:0x%08x\n",pInfo->cur_p0l_clc_ctrl_00);
			}
			if(PE_CHECK_WIN1(win_id))
			{
				PE_P0R_H14_RdFL(p0r_ipc_ctrl_01);
				PE_P0R_H14_Rd01(p0r_ipc_ctrl_01, cr_game_mode,	pstParams->enable);
			}
		}
		else
		{
			PE_DNT_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		PE_DNT_HW_H14_DBG_PRINT("get[%d]:en:%d\n",pstParams->win_id,pstParams->enable);
	}while(0);
	return ret;
}

/**
 * set ipc and clc mode
 *	- ctrl clc only
 *
 * @param   *pCfg [in] PE_TSK_H14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_SetIpcClcMode(PE_TSK_H14_CFG_T *pCfg)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	UINT32 cur_clc_flt_en,cur_clc_det_en;
	PE_INF_H14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_DNT_HW_H14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h14_info;
	PE_H14A0_P0L_CLC_CTRL_00_T *cur_p0l_clc_ctrl_00 = NULL;
	PE_H14A0_P0L_CLC_CTRL_00_T *pre_p0l_clc_ctrl_00 = NULL;
	PE_H14A0_P0L_IPC_CTRL_01_T *pre_p0l_ipc_ctrl_01 = NULL;
	#ifdef PE_DNT_HW_H14_SET_GAME_MODE_BY_FW
	PE_H14A0_P0R_IPC_CTRL_01_T *pre_p0r_ipc_ctrl_01 = NULL;
	PE_H14A0_P0L_IPC_CTRL_01_T *cur_p0l_ipc_ctrl_01 = NULL;
	PE_H14A0_P0R_IPC_CTRL_01_T *cur_p0r_ipc_ctrl_01 = NULL;
	UINT32 l_data, r_data;
	#endif
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H14)
		{
			pre_p0l_ipc_ctrl_01 = (PE_H14A0_P0L_IPC_CTRL_01_T *)&(pInfo->pre_p0l_ipc_ctrl_01);
			cur_p0l_clc_ctrl_00 = (PE_H14A0_P0L_CLC_CTRL_00_T *)&(pInfo->cur_p0l_clc_ctrl_00);
			pre_p0l_clc_ctrl_00 = (PE_H14A0_P0L_CLC_CTRL_00_T *)&(pInfo->pre_p0l_clc_ctrl_00);
			#ifdef PE_DNT_HW_H14_SET_GAME_MODE_BY_FW
			pre_p0r_ipc_ctrl_01 = (PE_H14A0_P0R_IPC_CTRL_01_T *)&(pInfo->pre_p0r_ipc_ctrl_01);
			cur_p0l_ipc_ctrl_01 = (PE_H14A0_P0L_IPC_CTRL_01_T *)&(l_data);
			cur_p0r_ipc_ctrl_01 = (PE_H14A0_P0R_IPC_CTRL_01_T *)&(r_data);
			/* read current p0l reg. */
			PE_P0L_H14_RdFL(p0l_ipc_ctrl_01);
			l_data = PE_P0L_H14_Rd(p0l_ipc_ctrl_01);
			/* read current p0r reg. */
			PE_P0R_H14_RdFL(p0r_ipc_ctrl_01);
			r_data = PE_P0R_H14_Rd(p0r_ipc_ctrl_01);
			/* compare p0l reg. */
			if (cur_p0l_ipc_ctrl_01->cr_st_flt_enable != pre_p0l_ipc_ctrl_01->cr_st_flt_enable || \
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en != pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en || \
				cur_p0l_ipc_ctrl_01->cr_hmc_flt_enable != pre_p0l_ipc_ctrl_01->cr_hmc_flt_enable || \
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range != pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range || \
				cur_p0l_ipc_ctrl_01->cr_black_mask != pre_p0l_ipc_ctrl_01->cr_black_mask)
			{
				PE_DNT_HW_H14_DBG_PRINT("[set]p0l ipc01 reg:0x%08x->0x%08x, game:%d\n",\
					l_data, pInfo->pre_p0l_ipc_ctrl_01, cur_p0l_ipc_ctrl_01->cr_game_mode);
				/* not control cr_game_mode, cr_5f_mode */
				cur_p0l_ipc_ctrl_01->cr_st_flt_enable = pre_p0l_ipc_ctrl_01->cr_st_flt_enable;
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en = pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_en;
				cur_p0l_ipc_ctrl_01->cr_hmc_flt_enable = pre_p0l_ipc_ctrl_01->cr_hmc_flt_enable;
				cur_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range = pre_p0l_ipc_ctrl_01->cr_st_2d_v_flt_range;
				cur_p0l_ipc_ctrl_01->cr_black_mask = pre_p0l_ipc_ctrl_01->cr_black_mask;
				PE_P0L_H14_Wr(p0l_ipc_ctrl_01, l_data);
				PE_P0L_H14_WrFL(p0l_ipc_ctrl_01);
			}
			/* compare p0r reg. */
			if (cur_p0r_ipc_ctrl_01->cr_st_flt_enable != pre_p0r_ipc_ctrl_01->cr_st_flt_enable || \
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en != pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en || \
				cur_p0r_ipc_ctrl_01->cr_hmc_flt_enable != pre_p0r_ipc_ctrl_01->cr_hmc_flt_enable || \
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range != pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range || \
				cur_p0r_ipc_ctrl_01->cr_black_mask != pre_p0r_ipc_ctrl_01->cr_black_mask)
			{
				PE_DNT_HW_H14_DBG_PRINT("[set]p0r ipc01 reg:0x%08x->0x%08x, game:%d\n",\
					r_data, pInfo->pre_p0r_ipc_ctrl_01, cur_p0r_ipc_ctrl_01->cr_game_mode);
				/* not control cr_game_mode, cr_5f_mode */
				cur_p0r_ipc_ctrl_01->cr_st_flt_enable = pre_p0r_ipc_ctrl_01->cr_st_flt_enable;
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en = pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_en;
				cur_p0r_ipc_ctrl_01->cr_hmc_flt_enable = pre_p0r_ipc_ctrl_01->cr_hmc_flt_enable;
				cur_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range = pre_p0r_ipc_ctrl_01->cr_st_2d_v_flt_range;
				cur_p0r_ipc_ctrl_01->cr_black_mask = pre_p0r_ipc_ctrl_01->cr_black_mask;
				PE_P0R_H14_Wr(p0r_ipc_ctrl_01, r_data);
				PE_P0R_H14_WrFL(p0r_ipc_ctrl_01);
			}
			/* check ipc,clc off case */
			if (ctrl0_info->is_src_frz && !ctrl0_info->mute_on && \
				disp0_info->src_type==LX_PE_SRC_DTV && \
				disp0_info->dtv_type==LX_PE_DTV_FILEPLAY && \
				disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur_clc_flt_en = PE_DNT_HW_H14_CLC_OFF;		//[1]
				cur_clc_det_en = PE_DNT_HW_H14_CLC_OFF;		//[2]
			}
			else
			{
				/* clc off when current game mode on(set by fw) */
				if(cur_p0l_ipc_ctrl_01->cr_game_mode)
				{
					cur_clc_flt_en = PE_DNT_HW_H14_CLC_OFF;		//[1]
					cur_clc_det_en = PE_DNT_HW_H14_CLC_OFF;		//[2]
				}
				else
				{
					cur_clc_flt_en = cur_p0l_clc_ctrl_00->cl_filter_enable;
					cur_clc_det_en = cur_p0l_clc_ctrl_00->clc_detection_enable;
				}
			}
			if (pre_p0l_clc_ctrl_00->cl_filter_enable != cur_clc_flt_en || \
				pre_p0l_clc_ctrl_00->clc_detection_enable != cur_clc_det_en)
			{
				PE_DNT_HW_H14_DBG_PRINT("[inf][clc]frz:%d,mute:%d,src:%d,dtv:%d\n",\
					ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
					disp0_info->src_type, disp0_info->dtv_type);
				PE_DNT_HW_H14_DBG_PRINT("[inf][clc]fmt:%d,scan:%d,out:%d,game:%d\n",\
					disp0_info->fmt_type, disp0_info->scan_type,\
					disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_H14_DBG_PRINT("[set][clc]flt:%d->%d, det:%d->%d\n",\
					pre_p0l_clc_ctrl_00->cl_filter_enable, cur_clc_flt_en,\
					pre_p0l_clc_ctrl_00->clc_detection_enable, cur_clc_det_en);
				PE_P0L_H14_RdFL(p0l_clc_ctrl_00);
				PE_P0L_H14_Wr01(p0l_clc_ctrl_00, cl_filter_enable,		cur_clc_flt_en);
				PE_P0L_H14_Wr01(p0l_clc_ctrl_00, clc_detection_enable,	cur_clc_det_en);
				PE_P0L_H14_WrFL(p0l_clc_ctrl_00);
				pre_p0l_clc_ctrl_00->cl_filter_enable = cur_clc_flt_en;
				pre_p0l_clc_ctrl_00->clc_detection_enable = cur_clc_det_en;
			}
			if(_g_dnt_hw_h14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_H14_DBG_PRINT("[state][inf]frz:%d,mute:%d,src:%d,dtv:%d\n",\
						ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
						disp0_info->src_type, disp0_info->dtv_type);
					PE_DNT_HW_H14_DBG_PRINT("[state][inf]fmt:%d,scan:%d,out:%d,game:%d\n",\
						disp0_info->fmt_type, disp0_info->scan_type,\
						disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
					PE_DNT_HW_H14_DBG_PRINT("[state][clc]flt:%d,det:%d\n",\
						cur_clc_flt_en, cur_clc_det_en);
					count=0;
				}
				count++;
			}
			#else
			/* check ipc,clc off case */
			if(ctrl0_info->is_src_frz && !ctrl0_info->mute_on && \
				disp0_info->src_type==LX_PE_SRC_DTV && \
				disp0_info->dtv_type==LX_PE_DTV_FILEPLAY && \
				disp0_info->fmt_type==LX_PE_FMT_HD && \
				disp0_info->scan_type==LX_PE_SCAN_INTERLACE)
			{
				cur_clc_flt_en = PE_DNT_HW_H14_CLC_OFF;		//[1]
				cur_clc_det_en = PE_DNT_HW_H14_CLC_OFF;		//[2]
			}
			else
			{
				/* clc off when game mode on */
				if(pre_p0l_ipc_ctrl_01->cr_game_mode)
				{
					cur_clc_flt_en = PE_DNT_HW_H14_CLC_OFF;		//[1]
					cur_clc_det_en = PE_DNT_HW_H14_CLC_OFF;		//[2]
				}
				else
				{
					cur_clc_flt_en = cur_p0l_clc_ctrl_00->cl_filter_enable;
					cur_clc_det_en = cur_p0l_clc_ctrl_00->clc_detection_enable;
				}
			}
			if(pre_p0l_clc_ctrl_00->cl_filter_enable != cur_clc_flt_en || \
				pre_p0l_clc_ctrl_00->clc_detection_enable != cur_clc_det_en)
			{
				PE_DNT_HW_H14_DBG_PRINT("[inf][clc]frz:%d,mute:%d,src:%d,dtv:%d\n",\
					ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
					disp0_info->src_type, disp0_info->dtv_type);
				PE_DNT_HW_H14_DBG_PRINT("[inf][clc]fmt:%d,scan:%d,out:%d,game:%d\n",\
					disp0_info->fmt_type, disp0_info->scan_type,\
					disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
				PE_DNT_HW_H14_DBG_PRINT("[set][clc]flt:%d->%d, det:%d->%d\n",\
					pre_p0l_clc_ctrl_00->cl_filter_enable, cur_clc_flt_en,\
					pre_p0l_clc_ctrl_00->clc_detection_enable, cur_clc_det_en);
				PE_P0L_H14_RdFL(p0l_clc_ctrl_00);
				PE_P0L_H14_Wr01(p0l_clc_ctrl_00, cl_filter_enable,		cur_clc_flt_en);
				PE_P0L_H14_Wr01(p0l_clc_ctrl_00, clc_detection_enable,	cur_clc_det_en);
				PE_P0L_H14_WrFL(p0l_clc_ctrl_00);
				pre_p0l_clc_ctrl_00->cl_filter_enable = cur_clc_flt_en;
				pre_p0l_clc_ctrl_00->clc_detection_enable = cur_clc_det_en;
			}
			if(_g_dnt_hw_h14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_DNT_HW_H14_DBG_PRINT("[state][inf]frz:%d,mute:%d,src:%d,dtv:%d\n",\
						ctrl0_info->is_src_frz, ctrl0_info->mute_on,\
						disp0_info->src_type, disp0_info->dtv_type);
					PE_DNT_HW_H14_DBG_PRINT("[state][inf]fmt:%d,scan:%d,out:%d,game:%d\n",\
						disp0_info->fmt_type, disp0_info->scan_type,\
						disp0_info->out_type, pre_p0l_ipc_ctrl_01->cr_game_mode);
					PE_DNT_HW_H14_DBG_PRINT("[state][clc]flt:%d, det:%d\n",\
						cur_clc_flt_en, cur_clc_det_en);
					count=0;
				}
				count++;
			}
			#endif
		}
		else
		{
			PE_DNT_HW_H14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * get current dnt settings
 *
 * @param   *pstParams [in/out] PE_DNT_HW_H14_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_DNT_HW_H14_GetCurDntSettings(PE_DNT_HW_H14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_DNT_HW_H14_SETTINGS_T *pInfo = &_g_pe_dnt_hw_h14_info;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H14)
		{
			memcpy(pstParams,pInfo,sizeof(PE_DNT_HW_H14_SETTINGS_T));
			PE_DNT_HW_H14_DBG_PRINT("get ipc1(lr):0x%08x,0x%08x, "\
				"clc0(cp):0x%08x,0x%08x\n", \
				pInfo->pre_p0l_ipc_ctrl_01,	pInfo->pre_p0r_ipc_ctrl_01, \
				pInfo->cur_p0l_clc_ctrl_00, pInfo->pre_p0l_clc_ctrl_00);
		}
		else
		{
			PE_DNT_HW_H14_DBG_PRINT("nothing to do.\n");
		}
	}while(0);
	return ret;
}

/**
 * read default setting
 *
 * @param   *pstParams [in/out] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see     
 * @author
 */
int PE_DNT_HW_H14_RdDefault(LX_PE_ETC_TBL_T *pstParams)
{
	int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	UINT32 param_size=0, table_size=0, is_valid=0;
	static UINT32 rd_cnt=0;
	PE_REG_PARAM_T param;

#define PE_DNT_H14_PRINT_START		"START_OF_PRINT"
#define PE_DNT_H14_PRINT_END		"END_OF_PRINT"
#define PE_DNT_H14_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_DNT_H14_CASE_PRMENU(_cnt, _action, _buf, _item)	\
	case (_cnt):sprintf(_buf, "   [0x%02x] %s\n", (_item), #_item);_action;break
#define PE_DNT_H14_CASE_SPRINT(_cnt, _action, _buf, fmt, args...)	\
	case (_cnt):sprintf(_buf, fmt, ##args);_action;break
#define PE_DNT_H14_DFLT_SPRINT(_action, _buf, fmt, args...)		\
	default:sprintf(_buf, fmt, ##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if (PE_KDRV_VER_H14)
		{
			if (pstParams->data[0]>=PE_DNT_HW_H14_RD_DFLT_MAX)
			{
				sprintf(buffer, "%s", PE_DNT_H14_PRINT_EXIT);
				rd_cnt=0;
			}
			else if (pstParams->data[0]==PE_DNT_HW_H14_RD_DFLT_DISPLAY)
			{
				switch (rd_cnt)
				{
					PE_DNT_H14_CASE_SPRINT( 0, rd_cnt++, buffer, "%s", PE_DNT_H14_PRINT_START);
					PE_DNT_H14_CASE_PRMENU( 1, rd_cnt++, buffer, PE_DNT_HW_H14_RD_DFLT_IPC_L);
					PE_DNT_H14_CASE_PRMENU( 2, rd_cnt++, buffer, PE_DNT_HW_H14_RD_DFLT_IPC_R);
					PE_DNT_H14_CASE_PRMENU( 3, rd_cnt++, buffer, PE_DNT_HW_H14_RD_DFLT_CLC_L);
					PE_DNT_H14_CASE_PRMENU( 4, rd_cnt++, buffer, PE_DNT_HW_H14_RD_DFLT_VFY_L);
					PE_DNT_H14_CASE_PRMENU( 5, rd_cnt++, buffer, PE_DNT_HW_H14_RD_DFLT_VFY_R);
					PE_DNT_H14_DFLT_SPRINT(rd_cnt=0, buffer, "%s", PE_DNT_H14_PRINT_END);
				}
			}
			else
			{
				is_valid = 0;
				switch (pstParams->data[0])
				{
					case PE_DNT_HW_H14_RD_DFLT_IPC_L:
					case PE_DNT_HW_H14_RD_DFLT_IPC_R:
						table_size = sizeof(ipc_l_pc_default_h14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = ipc_l_pc_default_h14a0[rd_cnt].addr;
							if(pstParams->data[0]==PE_DNT_HW_H14_RD_DFLT_IPC_R)
								param.addr += PE_PE0_REG_H14_LRDIFF;
							is_valid = 1;
						}
						break;
					case PE_DNT_HW_H14_RD_DFLT_CLC_L:
						table_size = sizeof(clc_l_pc_default_h14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = clc_l_pc_default_h14a0[rd_cnt].addr;
							is_valid = 1;
						}
						break;
					case PE_DNT_HW_H14_RD_DFLT_VFY_L:
					case PE_DNT_HW_H14_RD_DFLT_VFY_R:
					default:
						table_size = sizeof(vflt_y_l_pc_default_h14a0);
						param_size = table_size/sizeof(PE_REG_PARAM_T);
						if (rd_cnt<param_size)
						{
							param.addr = vflt_y_l_pc_default_h14a0[rd_cnt].addr;
							if(pstParams->data[0]==PE_DNT_HW_H14_RD_DFLT_VFY_R)
								param.addr += PE_PE0_REG_H14_LRDIFF;
							is_valid = 1;
						}
						break;
				}
				if (is_valid)
				{
					param.data = PE_REG_H14_RD(param.addr);
					sprintf(buffer, "   {0x%04X, 0x%08X},\n", param.addr, param.data);
					rd_cnt++;
				}
				else
				{
					sprintf(buffer, "%s", PE_DNT_H14_PRINT_END);
					rd_cnt = 0;
				}
			}
			memcpy(pstParams->data, buffer, sizeof(char)*PE_TRACE_STR_SIZE);
		}
	}while (0);
	return ret;
}

