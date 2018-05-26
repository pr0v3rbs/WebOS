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

/** @file pe_win.c
 *
 *  driver for picture enhance window control functions. ( used only within kdriver )
 *	- control window of each pe module
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2011.06.11
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

#include "pe_hw_h15.h"
#include "pe_reg_h15.h"
#include "pe_tsk_h15.h"
#include "pe_win_hw_h15.h"
#include "pe_hst_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_WIN_HW_H15_UHD_H_MAX			3840
#define PE_WIN_HW_H15_UHD_V_MAX			2160
#define PE_WIN_HW_H15_FHD_H_MAX			1920
#define PE_WIN_HW_H15_FHD_V_MAX			1080
#define PE_WIN_HW_H15_SHP_WIN			0
#define PE_WIN_HW_H15_CEN_WIN			1
#define PE_WIN_HW_H15_WIN_EN_OFF		0
#define PE_WIN_HW_H15_WIN_EN__ON		1
#define PE_WIN_HW_H15_P1SHP_WIN_OFST	64

#define PE_WIN_HW_H15_WIN_OFFSET_MAX	(4)

/* 20140305, sj.youm, use win0 for lrcr win because of single path */
#define PE_WIN_HW_H15A_USE_WIN_0_ONLY

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_WIN_HW_H15_ERROR	printk

#define PE_WIN_HW_H15_DBG_PRINT(fmt,args...)	\
	if(_g_win_hw_h15_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_WIN_HW_H15_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_WIN_HW_H15_ERROR(fmt,##args);_action;}}

#define PE_WIN_HW_H15_DIVIDE(x,y)	(((((x)<<10)/y)+512)>>10)

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
static void PE_WIN_HW_H15_SetVerWinReverse(UINT32 *y0,UINT32 *y1,UINT32 v_max);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_win_hw_h15_trace=0x0;	//default should be off.
static PE_WIN_HW_H15_SETTINGS_T _g_pe_win_hw_h15_info;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * init pe window
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_WIN_HW_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			memset(&_g_pe_win_hw_h15_info,0xffff,sizeof(PE_WIN_HW_H15_SETTINGS_T));
			PE_WIN_HW_H15_DBG_PRINT("pe1 win ctrl init, inside.\n");
			PE_SHPLR_H15A0_QWr05(pe1_win1_ctrl_00,win0_en,0x1, \
                                                  win1_en,0x1, \
                                                  win01_en,0x1, \
                                                  win_en,0x1, \
                                                  win_outside,0x0);
			PE_CEO_PE1_H15A0_QWr05(pe1_win2_ctrl_00,win0_en,0x1, \
                                                    win1_en,0x1, \
                                                    win01_en,0x1, \
                                                    win_en,0x1, \
                                                    win_outside,0x0);
			PE_SHPLR_H15A0_QWr05(pe1_win3_ctrl_00,win0_en,0x1, \
                                                  win1_en,0x1, \
                                                  win01_en,0x1, \
                                                  win_en,0x1, \
                                                  win_outside,0x0);
		}
		else
		{
			PE_WIN_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set debug settings
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_WIN_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_WIN_HW_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(WIN)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_win_hw_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(WIN)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * set vertical window reverse
 *
 * @param   on_off [in] UINT8
 * @return  void
 * @see
 * @author
 */
static void PE_WIN_HW_H15_SetVerWinReverse(UINT32 *y0,UINT32 *y1,UINT32 v_max)
{
	UINT32 cur_y0 = *y0;
	UINT32 cur_y1 = *y1;
	*y0 = (v_max-1)-cur_y1;
	*y1 = (v_max-1)-cur_y0;
	return;
}
/**
 * set pe window periodically
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_WIN_HW_H15_SetEnhancingWindow(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 is_invalid_size[LX_PE_WIN_NUM];
	UINT32 x0_ofst[LX_PE_WIN_NUM],y0_ofst[LX_PE_WIN_NUM];
	UINT32 x1_ofst[LX_PE_WIN_NUM],y1_ofst[LX_PE_WIN_NUM];
	UINT32 x0[LX_PE_WIN_NUM],y0[LX_PE_WIN_NUM];
	UINT32 x1[LX_PE_WIN_NUM],y1[LX_PE_WIN_NUM];
	UINT32 act0_x0,act0_y0,act0_x1,act0_y1;
	UINT32 act1_x0,act1_y0,act1_x1,act1_y1;
	UINT32 win0_x0,win0_x1,win0_y0,win0_y1;
	UINT32 win1_x0,win1_x1,win1_y0,win1_y1;
	UINT32 h_max,v_max,is_resize_case;
	UINT32 shp_l_w0_x0, shp_l_w0_x1, shp_r_w0_x0, shp_r_w0_x1;
	UINT32 shp_l_w1_x0, shp_l_w1_x1, shp_r_w1_x0, shp_r_w1_x1;
	UINT32 win0_en[2], win1_en[2], win01_en[2];	//[0]:shp, [1]:cen
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &pCfg->inf.disp_info[LX_PE_WIN_1];
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_WIN_HW_H15_CTRL_T *win_info=&_g_pe_win_hw_h15_info.win_inf;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			h_max=PE_WIN_HW_H15_UHD_H_MAX;
			v_max=PE_WIN_HW_H15_UHD_V_MAX;
			x0_ofst[LX_PE_WIN_0] = x1_ofst[LX_PE_WIN_0] = \
				(ctrl0_info->u_o_win_h_size<PE_WIN_HW_H15_UHD_H_MAX)? 3:0;
			x0_ofst[LX_PE_WIN_1] = x1_ofst[LX_PE_WIN_1] = \
				(ctrl1_info->u_o_win_h_size<PE_WIN_HW_H15_UHD_H_MAX)? 3:0;
			y0_ofst[LX_PE_WIN_0] = y1_ofst[LX_PE_WIN_0] = \
				(ctrl0_info->u_o_win_v_size<PE_WIN_HW_H15_UHD_V_MAX)? 3:0;
			y0_ofst[LX_PE_WIN_1] = y1_ofst[LX_PE_WIN_1] = \
				(ctrl1_info->u_o_win_v_size<PE_WIN_HW_H15_UHD_V_MAX)? 3:0;
			/* 2012xxxx, sj.youm, inner pattern for 2d,3d and so on. */
			is_invalid_size[LX_PE_WIN_0] = \
				(ctrl0_info->u_o_win_h_size <= (PE_WIN_HW_H15_WIN_OFFSET_MAX<<1) || \
				ctrl0_info->u_o_win_v_size <= (PE_WIN_HW_H15_WIN_OFFSET_MAX<<1))? 1:0;
			is_invalid_size[LX_PE_WIN_1] = \
				(ctrl1_info->u_o_win_h_size <= (PE_WIN_HW_H15_WIN_OFFSET_MAX<<1) || \
				ctrl1_info->u_o_win_v_size <= (PE_WIN_HW_H15_WIN_OFFSET_MAX<<1))? 1:0;
			/* 20130121, sj.youm, add photo case because of slide show */
			if(is_invalid_size[LX_PE_WIN_0] || \
				(disp0_info->src_type==LX_PE_SRC_DTV && \
				disp0_info->dtv_type==LX_PE_DTV_PHOTOPLAY))
			{
				x0[LX_PE_WIN_0] = 0;
				y0[LX_PE_WIN_0] = 0;
				x1[LX_PE_WIN_0] = h_max-1;
				y1[LX_PE_WIN_0] = v_max-1;
			}
			else
			{
				x0[LX_PE_WIN_0] = ctrl0_info->u_o_win_h_ofst;
				y0[LX_PE_WIN_0] = ctrl0_info->u_o_win_v_ofst;
				x1[LX_PE_WIN_0] = x0[LX_PE_WIN_0]+ctrl0_info->u_o_win_h_size-1;
				y1[LX_PE_WIN_0] = y0[LX_PE_WIN_0]+ctrl0_info->u_o_win_v_size-1;
			}
			/* 20130121, sj.youm, add photo case because of slide show */
			if(is_invalid_size[LX_PE_WIN_1] || \
				(disp1_info->src_type==LX_PE_SRC_DTV && \
				disp1_info->dtv_type==LX_PE_DTV_PHOTOPLAY))
			{
				x0[LX_PE_WIN_1] = 0;
				y0[LX_PE_WIN_1] = 0;
				x1[LX_PE_WIN_1] = h_max-1;
				y1[LX_PE_WIN_1] = v_max-1;
			}
			else
			{
				x0[LX_PE_WIN_1] = ctrl1_info->u_o_win_h_ofst;
				y0[LX_PE_WIN_1] = ctrl1_info->u_o_win_v_ofst;
				x1[LX_PE_WIN_1] = x0[LX_PE_WIN_1]+ctrl1_info->u_o_win_h_size-1;
				y1[LX_PE_WIN_1] = y0[LX_PE_WIN_1]+ctrl1_info->u_o_win_v_size-1;
			}

			/* 20140614, sj.youm, ud is something like 2d */
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_TB:
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = (y0[LX_PE_WIN_0]>>1);
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = (y1[LX_PE_WIN_0]>>1);
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = (y0[LX_PE_WIN_0]>>1)+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = (y1[LX_PE_WIN_0]>>1)-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = x0[LX_PE_WIN_0];
							act1_y0 = (y0[LX_PE_WIN_0]>>1)+PE_WIN_HW_H15_FHD_V_MAX;
							act1_x1 = x1[LX_PE_WIN_0];
							act1_y1 = (y1[LX_PE_WIN_0]>>1)+PE_WIN_HW_H15_FHD_V_MAX;
							win1_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win1_y0 = (y0[LX_PE_WIN_0]>>1)+y0_ofst[LX_PE_WIN_0]+PE_WIN_HW_H15_FHD_V_MAX;
							win1_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win1_y1 = (y1[LX_PE_WIN_0]>>1)-y1_ofst[LX_PE_WIN_0]+PE_WIN_HW_H15_FHD_V_MAX;
						}
						break;
					case LX_PE_OUT_SS:
						{
							/* L : window 0 */
							act0_x0 = (x0[LX_PE_WIN_0]>>1);
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = (x1[LX_PE_WIN_0]>>1);
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = (x0[LX_PE_WIN_0]>>1)+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = (x1[LX_PE_WIN_0]>>1)-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = (x0[LX_PE_WIN_0]>>1)+PE_WIN_HW_H15_FHD_H_MAX;
							act1_y0 = y0[LX_PE_WIN_0];
							act1_x1 = (x1[LX_PE_WIN_0]>>1)+PE_WIN_HW_H15_FHD_H_MAX;
							act1_y1 = y1[LX_PE_WIN_0];
							win1_x0 = (x0[LX_PE_WIN_0]>>1)+x0_ofst[LX_PE_WIN_0]+PE_WIN_HW_H15_FHD_H_MAX;
							win1_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win1_x1 = (x1[LX_PE_WIN_0]>>1)-x1_ofst[LX_PE_WIN_0]+PE_WIN_HW_H15_FHD_H_MAX;
							win1_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
						}
						break;
					case LX_PE_OUT_PIP:
						if(ctrl0_info->hv_same)
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = x0[LX_PE_WIN_1];
							act1_y0 = y0[LX_PE_WIN_1];
							act1_x1 = x1[LX_PE_WIN_1];
							act1_y1 = y1[LX_PE_WIN_1];
							win1_x0 = x0[LX_PE_WIN_1]+x0_ofst[LX_PE_WIN_1];
							win1_y0 = y0[LX_PE_WIN_1]+y0_ofst[LX_PE_WIN_1];
							win1_x1 = x1[LX_PE_WIN_1]-x1_ofst[LX_PE_WIN_1];
							win1_y1 = y1[LX_PE_WIN_1]-y1_ofst[LX_PE_WIN_1];
						}
						else
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							/* 20130218, sj.youm, add inv size to avoid main cen artifact when turn on pip */
							/* 20130221, sj.youm, add x,y comp to avoid main cen artifact when turn on pip */
							if(is_invalid_size[LX_PE_WIN_1] || \
								x0[LX_PE_WIN_0]==x0[LX_PE_WIN_1] || \
								y0[LX_PE_WIN_0]==y0[LX_PE_WIN_1] || \
								x1[LX_PE_WIN_0]==x1[LX_PE_WIN_1] || \
								y1[LX_PE_WIN_0]==y1[LX_PE_WIN_1])
							{
								act1_x0 = 0;
								act1_y0 = 0;
								act1_x1 = 0;
								act1_y1 = 0;
								win1_x0 = 0;
								win1_y0 = 0;
								win1_x1 = 0;
								win1_y1 = 0;
							}
							else
							{
								act1_x0 = x0[LX_PE_WIN_1];
								act1_y0 = y0[LX_PE_WIN_1];
								act1_x1 = x1[LX_PE_WIN_1];
								act1_y1 = y1[LX_PE_WIN_1];
								win1_x0 = ((x0[LX_PE_WIN_1]-x0_ofst[LX_PE_WIN_1])>0)? \
									(x0[LX_PE_WIN_1]-x0_ofst[LX_PE_WIN_1]):0;
								win1_y0 = ((y0[LX_PE_WIN_1]-y0_ofst[LX_PE_WIN_1])>0)? \
									(y0[LX_PE_WIN_1]-y0_ofst[LX_PE_WIN_1]):0;
								win1_x1 = x1[LX_PE_WIN_1]+x1_ofst[LX_PE_WIN_1];
								win1_y1 = y1[LX_PE_WIN_1]+y1_ofst[LX_PE_WIN_1];
							}
						}
						break;
					case LX_PE_OUT_2D_3D:
						if (disp0_info->out_sub==LX_PE_OUT_TB)
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = (y0[LX_PE_WIN_0]>>1);
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = (y1[LX_PE_WIN_0]>>1);
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = (y0[LX_PE_WIN_0]>>1)+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = (y1[LX_PE_WIN_0]>>1)-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = x0[LX_PE_WIN_0];
							act1_y0 = (y0[LX_PE_WIN_0]>>1)+PE_WIN_HW_H15_FHD_V_MAX;
							act1_x1 = x1[LX_PE_WIN_0];
							act1_y1 = (y1[LX_PE_WIN_0]>>1)+PE_WIN_HW_H15_FHD_V_MAX;
							win1_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win1_y0 = (y0[LX_PE_WIN_0]>>1)+y0_ofst[LX_PE_WIN_0]+PE_WIN_HW_H15_FHD_V_MAX;
							win1_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win1_y1 = (y1[LX_PE_WIN_0]>>1)-y1_ofst[LX_PE_WIN_0]+PE_WIN_HW_H15_FHD_V_MAX;
						}
						else	//LX_PE_OUT_LA
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = 0;
							act1_y0 = 0;
							act1_x1 = 0;
							act1_y1 = 0;
							win1_x0 = 0;
							win1_y0 = 0;
							win1_x1 = 0;
							win1_y1 = 0;
						}
						break;
					case LX_PE_OUT_UD:
					case LX_PE_OUT_2D:
					case LX_PE_OUT_LA:
					case LX_PE_OUT_3D_2D:
					case LX_PE_OUT_FS:			/* N/A on external frc */
					case LX_PE_OUT_DUAL_TB:		/* N/A on external frc */
					case LX_PE_OUT_DUAL_SS:		/* N/A on external frc */
					case LX_PE_OUT_DUAL_FULL:
					default:
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = 0;
							act1_y0 = 0;
							act1_x1 = 0;
							act1_y1 = 0;
							win1_x0 = 0;
							win1_y0 = 0;
							win1_x1 = 0;
							win1_y1 = 0;
						}
						break;
				}
			}
			else	/* internal frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_PIP:
						if(ctrl0_info->hv_same)
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = x0[LX_PE_WIN_1];
							act1_y0 = y0[LX_PE_WIN_1];
							act1_x1 = x1[LX_PE_WIN_1];
							act1_y1 = y1[LX_PE_WIN_1];
							win1_x0 = x0[LX_PE_WIN_1]+x0_ofst[LX_PE_WIN_1];
							win1_y0 = y0[LX_PE_WIN_1]+y0_ofst[LX_PE_WIN_1];
							win1_x1 = x1[LX_PE_WIN_1]-x1_ofst[LX_PE_WIN_1];
							win1_y1 = y1[LX_PE_WIN_1]-y1_ofst[LX_PE_WIN_1];
						}
						else
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							/* 20130218, sj.youm, add inv size to avoid main cen artifact when turn on pip */
							/* 20130221, sj.youm, add x,y comp to avoid main cen artifact when turn on pip */
							if(is_invalid_size[LX_PE_WIN_1] || \
								x0[LX_PE_WIN_0]==x0[LX_PE_WIN_1] || \
								y0[LX_PE_WIN_0]==y0[LX_PE_WIN_1] || \
								x1[LX_PE_WIN_0]==x1[LX_PE_WIN_1] || \
								y1[LX_PE_WIN_0]==y1[LX_PE_WIN_1])
							{
								act1_x0 = 0;
								act1_y0 = 0;
								act1_x1 = 0;
								act1_y1 = 0;
								win1_x0 = 0;
								win1_y0 = 0;
								win1_x1 = 0;
								win1_y1 = 0;
							}
							else
							{
								act1_x0 = x0[LX_PE_WIN_1];
								act1_y0 = y0[LX_PE_WIN_1];
								act1_x1 = x1[LX_PE_WIN_1];
								act1_y1 = y1[LX_PE_WIN_1];
								win1_x0 = ((x0[LX_PE_WIN_1]-x0_ofst[LX_PE_WIN_1])>0)? \
									(x0[LX_PE_WIN_1]-x0_ofst[LX_PE_WIN_1]):0;
								win1_y0 = ((y0[LX_PE_WIN_1]-y0_ofst[LX_PE_WIN_1])>0)? \
									(y0[LX_PE_WIN_1]-y0_ofst[LX_PE_WIN_1]):0;
								win1_x1 = x1[LX_PE_WIN_1]+x1_ofst[LX_PE_WIN_1];
								win1_y1 = y1[LX_PE_WIN_1]+y1_ofst[LX_PE_WIN_1];
							}
						}
						break;
					case LX_PE_OUT_LA:
					case LX_PE_OUT_TB:		/* N/A on internal frc */
					case LX_PE_OUT_2D_3D:
					case LX_PE_OUT_SS:		/* N/A on internal frc */
					case LX_PE_OUT_UD:
					case LX_PE_OUT_DUAL_TB:
					case LX_PE_OUT_DUAL_SS:
					case LX_PE_OUT_DUAL_FULL:
					case LX_PE_OUT_2D:
					case LX_PE_OUT_3D_2D:
					case LX_PE_OUT_FS:		/* N/A on internal frc */
					default:
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = y1[LX_PE_WIN_0]-y1_ofst[LX_PE_WIN_0];
							/* L : window 1 */
							act1_x0 = 0;
							act1_y0 = 0;
							act1_x1 = 0;
							act1_y1 = 0;
							win1_x0 = 0;
							win1_y0 = 0;
							win1_x1 = 0;
							win1_y1 = 0;
						}
						break;
				}
			}
			/* reverse mode */
			if(disp0_info->mode.is_reverse)
			{
				PE_WIN_HW_H15_SetVerWinReverse(&act0_y0,&act0_y1,v_max);
				PE_WIN_HW_H15_SetVerWinReverse(&act1_y0,&act1_y1,v_max);
				PE_WIN_HW_H15_SetVerWinReverse(&win0_y0,&win0_y1,v_max);
				PE_WIN_HW_H15_SetVerWinReverse(&win1_y0,&win1_y1,v_max);
			}
			/* 20130206, sj.youm, check resize_case that is resize(ctrl1 hv_same) and not file/photo play */
			is_resize_case = ((ctrl1_info->hv_same) && \
				!(disp0_info->src_type==LX_PE_SRC_DTV&&(disp0_info->dtv_type==LX_PE_DTV_FILEPLAY || \
				disp0_info->dtv_type==LX_PE_DTV_PHOTOPLAY)))? 1:0;
			/* win enable [0]:shp win, [1]:cen win */
			/* 20130130, sj.youm, on shp,cen win for split window because of function enable */
			/* 20130206, sj.youm, off shp, on cen win for resize(non pip) because of shp artifact */
			/* 20140614, sj.youm, ud is something like 2d */
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_TB:
					case LX_PE_OUT_SS:
						win0_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
						win1_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
						win01_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
						win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
						win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
						win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
						break;
					case LX_PE_OUT_PIP:
						if(ctrl0_info->hv_same)	//split window case
						{
							win0_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_SHP_WIN]=(ctrl0_info->mute_on)? \
								PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
							win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						}
						else
						{
							win0_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						}
						break;
					case LX_PE_OUT_2D_3D:
						if (disp0_info->out_sub==LX_PE_OUT_TB)
						{
							win0_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
								PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
								PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
							win01_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
								PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
							win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
						}
						else	//LX_PE_OUT_LA
						{
							win0_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
								PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						}
						break;
					default:
						win0_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
						win1_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
						win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						break;
				}
			}
			else	/* internal frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_PIP:
						if(ctrl0_info->hv_same)	//split window case
						{
							win0_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_SHP_WIN]=(ctrl0_info->mute_on)? \
								PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
							win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						}
						else
						{
							if (ctrl0_info->pmux_pos)	//pip mux before shp : win protection on
							{
								win0_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
								win1_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
								win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							}
							else	//pip mux after shp : win protectio off(turn on shp win)
							{
								win0_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
								win1_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
								win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							}
							win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
							win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
							win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						}
						break;
					default:
						win0_en[PE_WIN_HW_H15_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_H15_WIN_EN_OFF:PE_WIN_HW_H15_WIN_EN__ON;
						win1_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						win01_en[PE_WIN_HW_H15_SHP_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						win0_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN__ON;
						win1_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						win01_en[PE_WIN_HW_H15_CEN_WIN]=PE_WIN_HW_H15_WIN_EN_OFF;
						break;
				}
			}
			/* 20130221, sj.youm, check valid window coordinate */
			/* check limit, x1, y1 */
			if(act0_x1>=h_max)	act0_x1=h_max-1;
			if(act0_y1>=v_max)	act0_y1=v_max-1;
			if(win0_x1>=h_max)	win0_x1=h_max-1;
			if(win0_y1>=v_max)	win0_y1=v_max-1;
			if(act1_x1>=h_max)	act1_x1=h_max-1;
			if(act1_y1>=v_max)	act1_y1=v_max-1;
			if(win1_x1>=h_max)	win1_x1=h_max-1;
			if(win1_y1>=v_max)	win1_y1=v_max-1;
			/* check inversion, x0 vs. x1, y0 vs. y1 */
			if(act0_x0>act0_x1)	act0_x0=act0_x1;
			if(act0_y0>act0_y1)	act0_y0=act0_y1;
			if(win0_x0>win0_x1)	win0_x0=win0_x1;
			if(win0_x0>win0_x1)	win0_x0=win0_x1;
			if(act1_x0>act1_x1)	act1_x0=act1_x1;
			if(act1_y0>act1_y1)	act1_y0=act1_y1;
			if(win1_x0>win1_x1)	win1_x0=win1_x1;
			if(win1_x0>win1_x1)	win1_x0=win1_x1;
			/* set act(cen) */
			if((win_info->act0_x0 != act0_x0)|| (win_info->act0_y0 != act0_y0)|| \
				(win_info->act0_x1 != act0_x1)|| (win_info->act0_y1 != act0_y1)|| \
				(win_info->act1_x0 != act1_x0)|| (win_info->act1_y0 != act1_y0)|| \
				(win_info->act1_x1 != act1_x1)|| (win_info->act1_y1 != act1_y1))
			{
				PE_WIN_HW_H15_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_H15_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,ctrl0_info->u_o_win_h_size,\
					ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,ctrl1_info->u_o_win_v_ofst,\
					ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
				PE_WIN_HW_H15_DBG_PRINT("[set]act0 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->act0_x0,win_info->act0_y0,win_info->act0_x1,\
					win_info->act0_y1,act0_x0,act0_y0,act0_x1,act0_y1);
				PE_WIN_HW_H15_DBG_PRINT("[set]act1 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->act1_x0,win_info->act1_y0,win_info->act1_x1,\
					win_info->act1_y1,act1_x0,act1_y0,act1_x1,act1_y1);
				if(PE_KDRV_VER_H15)
				{
		            PE_CEO_PE1_H15A0_QWr02(pe1_win2_ctrl_01,win_w0_x0,(act0_x0>>1),\
                                                            win_w0_y0,act0_y0);
		            PE_CEO_PE1_H15A0_QWr02(pe1_win2_ctrl_02,win_w0_x1,(act0_x1>>1),\
                                                            win_w0_y1,act0_y1);
		            PE_CEO_PE1_H15A0_QWr02(pe1_win2_ctrl_03,win_w1_x0,(act1_x0>>1),\
                                                            win_w1_y0,act1_y0);
		            PE_CEO_PE1_H15A0_QWr02(pe1_win2_ctrl_04,win_w1_x1,(act1_x1>>1),\
                                                            win_w1_y1,act1_y1);
				}
				win_info->act0_x0 = act0_x0;
				win_info->act0_y0 = act0_y0;
				win_info->act0_x1 = act0_x1;
				win_info->act0_y1 = act0_y1;
				win_info->act1_x0 = act1_x0;
				win_info->act1_y0 = act1_y0;
				win_info->act1_x1 = act1_x1;
				win_info->act1_y1 = act1_y1;
			}
			/* set win(shp) */
			if((win_info->win0_x0 != win0_x0)|| (win_info->win0_y0 != win0_y0)|| \
				(win_info->win0_x1 != win0_x1)|| (win_info->win0_y1 != win0_y1)|| \
				(win_info->win1_x0 != win1_x0)|| (win_info->win1_y0 != win1_y0)|| \
				(win_info->win1_x1 != win1_x1)|| (win_info->win1_y1 != win1_y1))
			{
				PE_WIN_HW_H15_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_H15_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,ctrl0_info->u_o_win_h_size,\
					ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,ctrl1_info->u_o_win_v_ofst,\
					ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
				PE_WIN_HW_H15_DBG_PRINT("[set]win0 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->win0_x0,win_info->win0_y0,win_info->win0_x1,\
					win_info->win0_y1,win0_x0,win0_y0,win0_x1,win0_y1);
				PE_WIN_HW_H15_DBG_PRINT("[set]win1 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->win1_x0,win_info->win1_y0,win_info->win1_x1,\
					win_info->win1_y1,win1_x0,win1_y0,win1_x1,win1_y1);
				if(PE_KDRV_VER_H15)
				{
					/* shp_l */
					shp_l_w0_x0 = (win0_x0 < PE_WIN_HW_H15_FHD_H_MAX)? \
						win0_x0:(PE_WIN_HW_H15_FHD_H_MAX-1);
					shp_l_w0_x1 = (win0_x1 < PE_WIN_HW_H15_FHD_H_MAX)? \
						win0_x1:(PE_WIN_HW_H15_FHD_H_MAX+PE_WIN_HW_H15_P1SHP_WIN_OFST-1);
					shp_l_w1_x0 = (win1_x0 < PE_WIN_HW_H15_FHD_H_MAX)? \
						win1_x0:(PE_WIN_HW_H15_FHD_H_MAX-1);
					shp_l_w1_x1 = (win1_x1 < PE_WIN_HW_H15_FHD_H_MAX)? \
						win1_x1:(PE_WIN_HW_H15_FHD_H_MAX+PE_WIN_HW_H15_P1SHP_WIN_OFST-1);
					/* shp_r */
					shp_r_w0_x0 = (win0_x0 > PE_WIN_HW_H15_FHD_H_MAX)? \
						(win0_x0-PE_WIN_HW_H15_FHD_H_MAX+PE_WIN_HW_H15_P1SHP_WIN_OFST):0;
					shp_r_w0_x1 = (win0_x1 > PE_WIN_HW_H15_FHD_H_MAX)? \
						(win0_x1-PE_WIN_HW_H15_FHD_H_MAX+PE_WIN_HW_H15_P1SHP_WIN_OFST):PE_WIN_HW_H15_P1SHP_WIN_OFST;
					shp_r_w1_x0 = (win1_x0 > PE_WIN_HW_H15_FHD_H_MAX)? \
						(win1_x0-PE_WIN_HW_H15_FHD_H_MAX+PE_WIN_HW_H15_P1SHP_WIN_OFST):0;
					shp_r_w1_x1 = (win1_x1 > PE_WIN_HW_H15_FHD_H_MAX)? \
						(win1_x1-PE_WIN_HW_H15_FHD_H_MAX+PE_WIN_HW_H15_P1SHP_WIN_OFST):PE_WIN_HW_H15_P1SHP_WIN_OFST;
					/* shp_l */
		            PE_SHPL_H15A0_QWr02(pe1_win1_ctrl_01,	win_w0_x0,	shp_l_w0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_SHPL_H15A0_QWr02(pe1_win1_ctrl_02,	win_w0_x1,	shp_l_w0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_SHPL_H15A0_QWr02(pe1_win1_ctrl_03,	win_w1_x0,	shp_l_w1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_SHPL_H15A0_QWr02(pe1_win1_ctrl_04,	win_w1_x1,	shp_l_w1_x1,\
		            										win_w1_y1,	win1_y1);
		            PE_SHPL_H15A0_QWr02(pe1_win3_ctrl_01,	win_w0_x0,	shp_l_w0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_SHPL_H15A0_QWr02(pe1_win3_ctrl_02,	win_w0_x1,	shp_l_w0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_SHPL_H15A0_QWr02(pe1_win3_ctrl_03,	win_w1_x0,	shp_l_w1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_SHPL_H15A0_QWr02(pe1_win3_ctrl_04,	win_w1_x1,	shp_l_w1_x1,\
		            										win_w1_y1,	win1_y1);
					/* shp_r */
		            PE_SHPR_H15A0_QWr02(pe1_win1_ctrl_01,	win_w0_x0,	shp_r_w0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_SHPR_H15A0_QWr02(pe1_win1_ctrl_02,	win_w0_x1,	shp_r_w0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_SHPR_H15A0_QWr02(pe1_win1_ctrl_03,	win_w1_x0,	shp_r_w1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_SHPR_H15A0_QWr02(pe1_win1_ctrl_04,	win_w1_x1,	shp_r_w1_x1,\
		            										win_w1_y1,	win1_y1);
		            PE_SHPR_H15A0_QWr02(pe1_win3_ctrl_01,	win_w0_x0,	shp_r_w0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_SHPR_H15A0_QWr02(pe1_win3_ctrl_02,	win_w0_x1,	shp_r_w0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_SHPR_H15A0_QWr02(pe1_win3_ctrl_03,	win_w1_x0,	shp_r_w1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_SHPR_H15A0_QWr02(pe1_win3_ctrl_04,	win_w1_x1,	shp_r_w1_x1,\
		            										win_w1_y1,	win1_y1);
				}
				win_info->win0_x0 = win0_x0;
				win_info->win0_y0 = win0_y0;
				win_info->win0_x1 = win0_x1;
				win_info->win0_y1 = win0_y1;
				win_info->win1_x0 = win1_x0;
				win_info->win1_y0 = win1_y0;
				win_info->win1_x1 = win1_x1;
				win_info->win1_y1 = win1_y1;
			}
			/* shp win en */
			if((win_info->win0_en[PE_WIN_HW_H15_SHP_WIN] != win0_en[PE_WIN_HW_H15_SHP_WIN]) || \
				(win_info->win1_en[PE_WIN_HW_H15_SHP_WIN] != win1_en[PE_WIN_HW_H15_SHP_WIN]) || \
				(win_info->win01_en[PE_WIN_HW_H15_SHP_WIN] != win01_en[PE_WIN_HW_H15_SHP_WIN]))
			{
				PE_WIN_HW_H15_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_H15_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,ctrl0_info->u_o_win_h_size,\
					ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,ctrl1_info->u_o_win_v_ofst,\
					ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
				PE_WIN_HW_H15_DBG_PRINT("[set]shp_win_en(0,1,01):%d,%d,%d \n",\
					win_info->win0_en[PE_WIN_HW_H15_SHP_WIN], win_info->win1_en[PE_WIN_HW_H15_SHP_WIN],\
					win_info->win01_en[PE_WIN_HW_H15_SHP_WIN]);
				if(PE_KDRV_VER_H15)
				{
					PE_SHPLR_H15A0_QWr03(pe1_win1_ctrl_00,win0_en,win0_en[PE_WIN_HW_H15_SHP_WIN], \
                                                          win1_en,win1_en[PE_WIN_HW_H15_SHP_WIN], \
                                                          win01_en,win01_en[PE_WIN_HW_H15_SHP_WIN]);
					PE_SHPLR_H15A0_QWr03(pe1_win3_ctrl_00,win0_en,win0_en[PE_WIN_HW_H15_SHP_WIN], \
                                                          win1_en,win1_en[PE_WIN_HW_H15_SHP_WIN], \
                                                          win01_en,win01_en[PE_WIN_HW_H15_SHP_WIN]);
				}
				win_info->win0_en[PE_WIN_HW_H15_SHP_WIN] = win0_en[PE_WIN_HW_H15_SHP_WIN];
				win_info->win1_en[PE_WIN_HW_H15_SHP_WIN] = win1_en[PE_WIN_HW_H15_SHP_WIN];
				win_info->win01_en[PE_WIN_HW_H15_SHP_WIN] = win01_en[PE_WIN_HW_H15_SHP_WIN];
			}
			/* cen win en */
			if((win_info->win0_en[PE_WIN_HW_H15_CEN_WIN] != win0_en[PE_WIN_HW_H15_CEN_WIN]) || \
				(win_info->win1_en[PE_WIN_HW_H15_CEN_WIN] != win1_en[PE_WIN_HW_H15_CEN_WIN]) || \
				(win_info->win01_en[PE_WIN_HW_H15_CEN_WIN] != win01_en[PE_WIN_HW_H15_CEN_WIN]))
			{
				PE_WIN_HW_H15_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_H15_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,ctrl0_info->u_o_win_h_size,\
					ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,ctrl1_info->u_o_win_v_ofst,\
					ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
				PE_WIN_HW_H15_DBG_PRINT("[set]cen_win_en(0,1,01):%d,%d,%d \n",\
					win_info->win0_en[PE_WIN_HW_H15_CEN_WIN], win_info->win1_en[PE_WIN_HW_H15_CEN_WIN],\
					win_info->win01_en[PE_WIN_HW_H15_CEN_WIN]);
				if(PE_KDRV_VER_H15)
				{
					PE_CEO_PE1_H15A0_QWr03(pe1_win2_ctrl_00,win0_en,win0_en[PE_WIN_HW_H15_CEN_WIN], \
                                                            win1_en,win1_en[PE_WIN_HW_H15_CEN_WIN], \
                                                            win01_en,win01_en[PE_WIN_HW_H15_CEN_WIN]);
				}
				win_info->win0_en[PE_WIN_HW_H15_CEN_WIN] = win0_en[PE_WIN_HW_H15_CEN_WIN];
				win_info->win1_en[PE_WIN_HW_H15_CEN_WIN] = win1_en[PE_WIN_HW_H15_CEN_WIN];
				win_info->win01_en[PE_WIN_HW_H15_CEN_WIN] = win01_en[PE_WIN_HW_H15_CEN_WIN];
			}
			/* etc */
			if((win_info->h_max != h_max)|| (win_info->v_max != v_max))
			{
				PE_WIN_HW_H15_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_H15_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,ctrl0_info->u_o_win_h_size,\
					ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,ctrl1_info->u_o_win_v_ofst,\
					ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
				PE_WIN_HW_H15_DBG_PRINT("[set]h,v_max : prev:%d,%d, cur:%d,%d\n",\
					win_info->h_max,win_info->v_max,h_max,v_max);
				if(PE_KDRV_VER_H15)
				{
					PE_SHPLR_H15A0_QWr02(pe1_top_ctrl_01,width,(h_max>>1),height,v_max);
					PE_CEO_PE1_H15A0_QWr02(pe1_top_ctrl_01,width,(h_max>>1),height,v_max);
		            PE_CEO_PE1_H15A0_QWr02(pe1_apl_ctrl_00,apl_win_ctrl_x0,0,apl_win_ctrl_y0,0);
		            PE_CEO_PE1_H15A0_QWr02(pe1_apl_ctrl_01,apl_win_ctrl_x1,(h_max>>1),apl_win_ctrl_y1,v_max);
				}
				win_info->h_max = h_max;
				win_info->v_max = v_max;
			}
		}
		else
		{
			PE_WIN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set lrcr window periodically
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_WIN_HW_H15_SetLrcrWindow(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 h_max,v_max,pxl_cnt;
	SINT32 temp_h,temp_v,act_out_h,act_out_v,src_h,src_v,in_h,in_v,out_h,out_v;
	UINT32 l_x0,l_y0,l_x1,l_y1,r_x0,r_y0,r_x1,r_y1;
	UINT32 x0[LX_PE_WIN_NUM],y0[LX_PE_WIN_NUM],x1[LX_PE_WIN_NUM],y1[LX_PE_WIN_NUM];
	UINT32 cur_histo_l_win[4],cur_histo_r_win[4],cur_apply_r_win[4];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_WIN_HW_H15_LRCR_CTRL_T *lrcr_info=&_g_pe_win_hw_h15_info.lrcr_inf;
	PE_TSK_H15_LRC_HST_CTRL_T *lrc_ctrl = &pCfg->lrc;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			h_max=PE_WIN_HW_H15_UHD_H_MAX;
			v_max=PE_WIN_HW_H15_UHD_V_MAX;
			/* lrcr win default : not 0,0,1919,1079, but 0,0,1920,1080 */
			if(ctrl0_info->u_o_win_h_size)
			{
				x0[LX_PE_WIN_0] = ctrl0_info->u_o_win_h_ofst;
				x1[LX_PE_WIN_0] = x0[LX_PE_WIN_0]+ctrl0_info->u_o_win_h_size;
			}
			else
			{
				x0[LX_PE_WIN_0] = 0;
				x1[LX_PE_WIN_0] = h_max;
			}
			if(ctrl0_info->u_o_win_v_size)
			{
				/* [h15a0] doesn't matter 2d or 2dto3d */
				y0[LX_PE_WIN_0] = ctrl0_info->u_o_win_v_ofst;
				y1[LX_PE_WIN_0] = y0[LX_PE_WIN_0]+ctrl0_info->u_o_win_v_size;
			}
			else
			{
				y0[LX_PE_WIN_0] = 0;
				y1[LX_PE_WIN_0] = v_max;
			}
			if(ctrl1_info->u_o_win_h_size)
			{
				x0[LX_PE_WIN_1] = ctrl1_info->u_o_win_h_ofst;
				x1[LX_PE_WIN_1] = x0[LX_PE_WIN_1]+ctrl1_info->u_o_win_h_size;
			}
			else
			{
				x0[LX_PE_WIN_1] = 0;
				x1[LX_PE_WIN_1] = h_max;
			}
			if(ctrl1_info->u_o_win_v_size)
			{
				y0[LX_PE_WIN_1] = ctrl1_info->u_o_win_v_ofst;
				y1[LX_PE_WIN_1] = y0[LX_PE_WIN_1]+ctrl1_info->u_o_win_v_size;
			}
			else
			{
				y0[LX_PE_WIN_1] = 0;
				y1[LX_PE_WIN_1] = v_max;
			}
			/* if 2D_3D&TB, half of y is available */
			if(disp0_info->out_type==LX_PE_OUT_2D_3D && disp0_info->out_sub==LX_PE_OUT_TB)
			{
				y0[LX_PE_WIN_0] = y0[LX_PE_WIN_0]>>1;
				y1[LX_PE_WIN_0] = y1[LX_PE_WIN_0]>>1;
				y0[LX_PE_WIN_1] = y0[LX_PE_WIN_1]>>1;
				y1[LX_PE_WIN_1] = y1[LX_PE_WIN_1]>>1;
			}
			/* [h15a0]
			* lrcr win setting for 3d : LA ll mode on
			* here consider external frc case for 3d
			*/
			switch(disp0_info->out_type)
			{
				case LX_PE_OUT_2D:
				case LX_PE_OUT_2D_3D:
				case LX_PE_OUT_UD:
					if(PE_HW_H15_CHK_UHD_FMT_N_3D_CASE(disp0_info))
					{
						if(disp0_info->user_o_type==LX_PE_OUT_TB && \
							disp0_info->user_i_type==LX_PE_3D_IN_TB)
						{
							/* user 3d(tb) case */
							cur_histo_l_win[0] = 0;
							cur_histo_l_win[1] = 0;
							cur_histo_l_win[2] = h_max;
							cur_histo_l_win[3] = (v_max>>1);
							cur_histo_r_win[0] = 0;
							cur_histo_r_win[1] = (v_max>>1);
							cur_histo_r_win[2] = h_max;
							cur_histo_r_win[3] = v_max;
							cur_apply_r_win[0] = 0;
							cur_apply_r_win[1] = (v_max>>1);
							cur_apply_r_win[2] = h_max;
							cur_apply_r_win[3] = v_max;
						}
						else	//user_o_type==LX_PE_OUT_SS && user_i_type==LX_PE_3D_IN_SS
						{
							/* user 3d(ss) case */
							cur_histo_l_win[0] = 0;
							cur_histo_l_win[1] = 0;
							cur_histo_l_win[2] = (h_max>>1);
							cur_histo_l_win[3] = v_max;
							cur_histo_r_win[0] = (h_max>>1);
							cur_histo_r_win[1] = 0;
							cur_histo_r_win[2] = h_max;
							cur_histo_r_win[3] = v_max;
							cur_apply_r_win[0] = (h_max>>1);
							cur_apply_r_win[1] = 0;
							cur_apply_r_win[2] = h_max;
							cur_apply_r_win[3] = v_max;
						}
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0 || \
						lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1 || \
						lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0 || \
						lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1)
					{
						if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0 || \
							lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1)
						{
							src_v = disp0_info->in_v_size;//ctrl0_info->pe0_src_v
							in_v = ctrl0_info->u_i_win_v_size;
							if(disp0_info->out_type==LX_PE_OUT_2D_3D && \
								disp0_info->out_sub==LX_PE_OUT_TB)
							{
								out_v = ctrl0_info->u_o_win_v_size>>1;	//0.5
							}
							else	//2D or 2D_3D&LA
							{
								out_v = ctrl0_info->u_o_win_v_size;
							}
							if(in_v)
							{
								temp_v = out_v-((src_v-in_v)*out_v/in_v);
								if(temp_v<=0)	act_out_v=0;
								else			act_out_v=(temp_v>>1);
							}
							else
							{
								act_out_v=0;
							}
							l_x0 = x0[LX_PE_WIN_0];
							l_y0 = y0[LX_PE_WIN_0];
							l_x1 = x1[LX_PE_WIN_0];
							l_y1 = y0[LX_PE_WIN_0] + act_out_v;
							r_x0 = x0[LX_PE_WIN_0];
							r_y0 = y1[LX_PE_WIN_0] - act_out_v;
							r_x1 = x1[LX_PE_WIN_0];
							r_y1 = y1[LX_PE_WIN_0];
						}
						else
						{
							src_h = disp0_info->in_h_size;//ctrl0_info->pe0_src_h;
							in_h = ctrl0_info->u_i_win_h_size;
							out_h = ctrl0_info->u_o_win_h_size;
							if(in_h)
							{
								temp_h = (out_h-((src_h-in_h)*out_h/in_h));
								if(temp_h<=0)	act_out_h=0;
								else			act_out_h=(temp_h>>1);
							}
							else
							{
								act_out_h=0;
							}
							l_x0 = x0[LX_PE_WIN_0];
							l_y0 = y0[LX_PE_WIN_0];
							l_x1 = x0[LX_PE_WIN_0] + act_out_h;
							l_y1 = y1[LX_PE_WIN_0];
							r_x0 = x1[LX_PE_WIN_0] - act_out_h;
							r_y0 = y0[LX_PE_WIN_0];
							r_x1 = x1[LX_PE_WIN_0];
							r_y1 = y1[LX_PE_WIN_0];
						}
						if(l_x1>PE_WIN_HW_H15_UHD_H_MAX)	l_x1=PE_WIN_HW_H15_UHD_H_MAX;
						if(l_y1>PE_WIN_HW_H15_UHD_V_MAX)	l_y1=PE_WIN_HW_H15_UHD_V_MAX;
						if(r_x1>PE_WIN_HW_H15_UHD_H_MAX)	r_x1=PE_WIN_HW_H15_UHD_H_MAX;
						if(r_y1>PE_WIN_HW_H15_UHD_V_MAX)	r_y1=PE_WIN_HW_H15_UHD_V_MAX;
						cur_histo_l_win[0] = l_x0;
						cur_histo_l_win[2] = l_x1;
						cur_histo_r_win[0] = r_x0;
						cur_histo_r_win[2] = r_x1;
						if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0|| \
							lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0)
						{
							cur_histo_l_win[1] = l_y0;
							cur_histo_l_win[3] = l_y0+((l_y1-l_y0)>>1);
							cur_histo_r_win[1] = r_y0;
							cur_histo_r_win[3] = r_y0+((r_y1-r_y0)>>1);
						}
						else
						{
							cur_histo_l_win[1] = l_y0+((l_y1-l_y0)>>1);
							cur_histo_l_win[3] = l_y1;
							cur_histo_r_win[1] = r_y0+((r_y1-r_y0)>>1);
							cur_histo_r_win[3] = r_y1;
						}
						cur_apply_r_win[0] = x0[LX_PE_WIN_0];
						cur_apply_r_win[1] = y0[LX_PE_WIN_0];
						cur_apply_r_win[2] = x1[LX_PE_WIN_0];
						cur_apply_r_win[3] = y1[LX_PE_WIN_0];
					}
					else
					{
						cur_histo_l_win[0] = x0[LX_PE_WIN_0];
						cur_histo_l_win[1] = y0[LX_PE_WIN_0];
						cur_histo_l_win[2] = x1[LX_PE_WIN_0];
						cur_histo_l_win[3] = y1[LX_PE_WIN_0];
						#ifdef PE_WIN_HW_H15A_USE_WIN_0_ONLY
						cur_histo_r_win[0] = x0[LX_PE_WIN_0];
						cur_histo_r_win[1] = y0[LX_PE_WIN_0];
						cur_histo_r_win[2] = x1[LX_PE_WIN_0];
						cur_histo_r_win[3] = y1[LX_PE_WIN_0];
						cur_apply_r_win[0] = x0[LX_PE_WIN_0];
						cur_apply_r_win[1] = y0[LX_PE_WIN_0];
						cur_apply_r_win[2] = x1[LX_PE_WIN_0];
						cur_apply_r_win[3] = y1[LX_PE_WIN_0];
						#else
						cur_histo_r_win[0] = x0[LX_PE_WIN_1];
						cur_histo_r_win[1] = y0[LX_PE_WIN_1];
						cur_histo_r_win[2] = x1[LX_PE_WIN_1];
						cur_histo_r_win[3] = y1[LX_PE_WIN_1];
						cur_apply_r_win[0] = x0[LX_PE_WIN_1];
						cur_apply_r_win[1] = y0[LX_PE_WIN_1];
						cur_apply_r_win[2] = x1[LX_PE_WIN_1];
						cur_apply_r_win[3] = y1[LX_PE_WIN_1];
						#endif
					}
					break;
				case LX_PE_OUT_TB:
					{
						cur_histo_l_win[0] = 0;
						cur_histo_l_win[1] = 0;
						cur_histo_l_win[2] = h_max;
						cur_histo_l_win[3] = (v_max>>1);
						cur_histo_r_win[0] = 0;
						cur_histo_r_win[1] = (v_max>>1);
						cur_histo_r_win[2] = h_max;
						cur_histo_r_win[3] = v_max;
						cur_apply_r_win[0] = 0;
						cur_apply_r_win[1] = (v_max>>1);
						cur_apply_r_win[2] = h_max;
						cur_apply_r_win[3] = v_max;
					}
					break;
				case LX_PE_OUT_SS:
					{
						cur_histo_l_win[0] = 0;
						cur_histo_l_win[1] = 0;
						cur_histo_l_win[2] = (h_max>>1);
						cur_histo_l_win[3] = v_max;
						cur_histo_r_win[0] = (h_max>>1);
						cur_histo_r_win[1] = 0;
						cur_histo_r_win[2] = h_max;
						cur_histo_r_win[3] = v_max;
						cur_apply_r_win[0] = (h_max>>1);
						cur_apply_r_win[1] = 0;
						cur_apply_r_win[2] = h_max;
						cur_apply_r_win[3] = v_max;
					}
					break;
				case LX_PE_OUT_LA:
				case LX_PE_OUT_FS:
				case LX_PE_OUT_3D_2D:
				case LX_PE_OUT_DUAL_TB:
				case LX_PE_OUT_DUAL_SS:
				case LX_PE_OUT_DUAL_FULL:
				case LX_PE_OUT_PIP:
				default:
					{
						cur_histo_l_win[0] = x0[LX_PE_WIN_0];
						cur_histo_l_win[1] = y0[LX_PE_WIN_0];
						cur_histo_l_win[2] = x1[LX_PE_WIN_0];
						cur_histo_l_win[3] = y1[LX_PE_WIN_0];
						#ifdef PE_WIN_HW_H15A_USE_WIN_0_ONLY
						cur_histo_r_win[0] = x0[LX_PE_WIN_0];
						cur_histo_r_win[1] = y0[LX_PE_WIN_0];
						cur_histo_r_win[2] = x1[LX_PE_WIN_0];
						cur_histo_r_win[3] = y1[LX_PE_WIN_0];
						cur_apply_r_win[0] = x0[LX_PE_WIN_0];
						cur_apply_r_win[1] = y0[LX_PE_WIN_0];
						cur_apply_r_win[2] = x1[LX_PE_WIN_0];
						cur_apply_r_win[3] = y1[LX_PE_WIN_0];
						#else
						cur_histo_r_win[0] = x0[LX_PE_WIN_1];
						cur_histo_r_win[1] = y0[LX_PE_WIN_1];
						cur_histo_r_win[2] = x1[LX_PE_WIN_1];
						cur_histo_r_win[3] = y1[LX_PE_WIN_1];
						cur_apply_r_win[0] = x0[LX_PE_WIN_1];
						cur_apply_r_win[1] = y0[LX_PE_WIN_1];
						cur_apply_r_win[2] = x1[LX_PE_WIN_1];
						cur_apply_r_win[3] = y1[LX_PE_WIN_1];
						#endif
					}
					break;
			}
			/* 20130221, sj.youm, check valid window coordinate */
			/* check limit */
			if(cur_histo_l_win[2]>h_max)	cur_histo_l_win[2]=h_max;
			if(cur_histo_l_win[3]>v_max)	cur_histo_l_win[3]=v_max;
			if(cur_histo_r_win[2]>h_max)	cur_histo_r_win[2]=h_max;
			if(cur_histo_r_win[3]>v_max)	cur_histo_r_win[3]=v_max;
			if(cur_apply_r_win[2]>h_max)	cur_apply_r_win[2]=h_max;
			if(cur_apply_r_win[3]>v_max)	cur_apply_r_win[3]=v_max;
			/* check inversion */
			if(cur_histo_l_win[0]>cur_histo_l_win[2])	{cur_histo_l_win[0]=0;cur_histo_l_win[2]=h_max;}
			if(cur_histo_l_win[1]>cur_histo_l_win[3])	{cur_histo_l_win[1]=0;cur_histo_l_win[3]=v_max;}
			if(cur_histo_r_win[0]>cur_histo_r_win[2])	{cur_histo_r_win[0]=0;cur_histo_r_win[2]=h_max;}
			if(cur_histo_r_win[1]>cur_histo_r_win[3])	{cur_histo_r_win[1]=0;cur_histo_r_win[3]=v_max;}
			if(cur_apply_r_win[0]>cur_apply_r_win[2])	{cur_apply_r_win[0]=0;cur_apply_r_win[2]=h_max;}
			if(cur_apply_r_win[1]>cur_apply_r_win[3])	{cur_apply_r_win[1]=0;cur_apply_r_win[3]=v_max;}
			if((lrcr_info->histo_l_x0!=cur_histo_l_win[0])||(lrcr_info->histo_l_y0!=cur_histo_l_win[1])|| \
				(lrcr_info->histo_l_x1!=cur_histo_l_win[2])||(lrcr_info->histo_l_y1!=cur_histo_l_win[3]))
			{
				/* too many prints */
				if(disp0_info->out_type!=LX_PE_OUT_2D && disp0_info->out_type!=LX_PE_OUT_2D_3D)
				{
					PE_WIN_HW_H15_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
						disp0_info->out_type,ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,\
						ctrl0_info->u_o_win_h_size,ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,\
						ctrl1_info->u_o_win_v_ofst,ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
					PE_WIN_HW_H15_DBG_PRINT("[set]histo_l_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
						lrcr_info->histo_l_x0,lrcr_info->histo_l_y0,lrcr_info->histo_l_x1,lrcr_info->histo_l_y1,\
						cur_histo_l_win[0],cur_histo_l_win[1],cur_histo_l_win[2],cur_histo_l_win[3]);
				}
				lrcr_info->histo_l_x0 = cur_histo_l_win[0];
				lrcr_info->histo_l_y0 = cur_histo_l_win[1];
				lrcr_info->histo_l_x1 = cur_histo_l_win[2];
				lrcr_info->histo_l_y1 = cur_histo_l_win[3];
				PE_CEO_PE1_H15A0_RdFL(pe1_lrcr_ctrl_02);
				PE_CEO_PE1_H15A0_RdFL(pe1_lrcr_ctrl_03);
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_02, x0,	(cur_histo_l_win[0]>>1));
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_02, y0,	cur_histo_l_win[1]);
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_03, x1,	(cur_histo_l_win[2]>>1));
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_03, y1,	cur_histo_l_win[3]);
				PE_CEO_PE1_H15A0_WrFL(pe1_lrcr_ctrl_02);
				PE_CEO_PE1_H15A0_WrFL(pe1_lrcr_ctrl_03);
				if((disp0_info->out_type==LX_PE_OUT_2D || \
					disp0_info->out_type==LX_PE_OUT_2D_3D) && \
					lrc_ctrl->win_mod>=PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0 && \
					lrc_ctrl->win_mod<=PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1)
				{
					if((cur_histo_l_win[2]>cur_histo_l_win[0]) && \
						(cur_histo_l_win[3]>cur_histo_l_win[1]))
					{
						temp_h = cur_histo_l_win[2]-cur_histo_l_win[0];
						temp_v = cur_histo_l_win[3]-cur_histo_l_win[1];
						pxl_cnt = (temp_h*temp_v)<<1;
					}
					else
					{
						pxl_cnt = 0;
					}
					if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0)
					{
						lrc_ctrl->tb0_t_pos[0] = (cur_histo_l_win[0]>>1);
						lrc_ctrl->tb0_t_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->tb0_t_pos[2] = (cur_histo_l_win[2]>>1);
						lrc_ctrl->tb0_t_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[0] = (pxl_cnt>>1);
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0)
					{
						lrc_ctrl->ss0_l_pos[0] = (cur_histo_l_win[0]>>1);
						lrc_ctrl->ss0_l_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->ss0_l_pos[2] = (cur_histo_l_win[2]>>1);
						lrc_ctrl->ss0_l_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[1] = (pxl_cnt>>1);
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1)
					{
						lrc_ctrl->tb1_t_pos[0] = (cur_histo_l_win[0]>>1);
						lrc_ctrl->tb1_t_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->tb1_t_pos[2] = (cur_histo_l_win[2]>>1);
						lrc_ctrl->tb1_t_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[2] = (pxl_cnt>>1);
					}
					else	//PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1
					{
						lrc_ctrl->ss1_l_pos[0] = (cur_histo_l_win[0]>>1);
						lrc_ctrl->ss1_l_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->ss1_l_pos[2] = (cur_histo_l_win[2]>>1);
						lrc_ctrl->ss1_l_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[3] = (pxl_cnt>>1);
					}
				}
			}
			if((lrcr_info->histo_r_x0!=cur_histo_r_win[0])||(lrcr_info->histo_r_y0!=cur_histo_r_win[1])|| \
				(lrcr_info->histo_r_x1!=cur_histo_r_win[2])||(lrcr_info->histo_r_y1!=cur_histo_r_win[3]))
			{
				/* too many prints */
				if(disp0_info->out_type!=LX_PE_OUT_2D && disp0_info->out_type!=LX_PE_OUT_2D_3D)
				{
					PE_WIN_HW_H15_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
						disp0_info->out_type,ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,\
						ctrl0_info->u_o_win_h_size,ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,\
						ctrl1_info->u_o_win_v_ofst,ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
					PE_WIN_HW_H15_DBG_PRINT("[set]histo_r_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
						lrcr_info->histo_r_x0,lrcr_info->histo_r_y0,lrcr_info->histo_r_x1,lrcr_info->histo_r_y1,\
						cur_histo_r_win[0],cur_histo_r_win[1],cur_histo_r_win[2],cur_histo_r_win[3]);
				}
				lrcr_info->histo_r_x0 = cur_histo_r_win[0];
				lrcr_info->histo_r_y0 = cur_histo_r_win[1];
				lrcr_info->histo_r_x1 = cur_histo_r_win[2];
				lrcr_info->histo_r_y1 = cur_histo_r_win[3];
				PE_CEO_PE1_H15A0_RdFL(pe1_lrcr_ctrl_04);
				PE_CEO_PE1_H15A0_RdFL(pe1_lrcr_ctrl_05);
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_04, x0,	(cur_histo_r_win[0]>>1));
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_04, y0,	cur_histo_r_win[1]);
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_05, x1,	(cur_histo_r_win[2]>>1));
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_05, y1,	cur_histo_r_win[3]);
				PE_CEO_PE1_H15A0_WrFL(pe1_lrcr_ctrl_04);
				PE_CEO_PE1_H15A0_WrFL(pe1_lrcr_ctrl_05);
				if((disp0_info->out_type==LX_PE_OUT_2D || \
					disp0_info->out_type==LX_PE_OUT_2D_3D) && \
					lrc_ctrl->win_mod>=PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0 && \
					lrc_ctrl->win_mod<=PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1)
				{
					if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0)
					{
						lrc_ctrl->tb0_b_pos[0] = (cur_histo_r_win[0]>>1);
						lrc_ctrl->tb0_b_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->tb0_b_pos[2] = (cur_histo_r_win[2]>>1);
						lrc_ctrl->tb0_b_pos[3] = cur_histo_r_win[3];
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0)
					{
						lrc_ctrl->ss0_r_pos[0] = (cur_histo_r_win[0]>>1);
						lrc_ctrl->ss0_r_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->ss0_r_pos[2] = (cur_histo_r_win[2]>>1);
						lrc_ctrl->ss0_r_pos[3] = cur_histo_r_win[3];
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1)
					{
						lrc_ctrl->tb1_b_pos[0] = (cur_histo_r_win[0]>>1);
						lrc_ctrl->tb1_b_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->tb1_b_pos[2] = (cur_histo_r_win[2]>>1);
						lrc_ctrl->tb1_b_pos[3] = cur_histo_r_win[3];
					}
					else	//PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1
					{
						lrc_ctrl->ss1_r_pos[0] = (cur_histo_r_win[0]>>1);
						lrc_ctrl->ss1_r_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->ss1_r_pos[2] = (cur_histo_r_win[2]>>1);
						lrc_ctrl->ss1_r_pos[3] = cur_histo_r_win[3];
					}
				}
			}
			if((lrcr_info->apply_r_x0!=cur_apply_r_win[0])||(lrcr_info->apply_r_y0!=cur_apply_r_win[1])|| \
				(lrcr_info->apply_r_x1!=cur_apply_r_win[2])||(lrcr_info->apply_r_y1!=cur_apply_r_win[3]))
			{
				PE_WIN_HW_H15_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					disp0_info->out_type,ctrl0_info->u_o_win_h_ofst,ctrl0_info->u_o_win_v_ofst,\
					ctrl0_info->u_o_win_h_size,ctrl0_info->u_o_win_v_size,ctrl1_info->u_o_win_h_ofst,\
					ctrl1_info->u_o_win_v_ofst,ctrl1_info->u_o_win_h_size,ctrl1_info->u_o_win_v_size);
				PE_WIN_HW_H15_DBG_PRINT("[set]apply_r_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					lrcr_info->apply_r_x0,lrcr_info->apply_r_y0,lrcr_info->apply_r_x1,lrcr_info->apply_r_y1,\
					cur_apply_r_win[0],cur_apply_r_win[1],cur_apply_r_win[2],cur_apply_r_win[3]);
				lrcr_info->apply_r_x0 = cur_apply_r_win[0];
				lrcr_info->apply_r_y0 = cur_apply_r_win[1];
				lrcr_info->apply_r_x1 = cur_apply_r_win[2];
				lrcr_info->apply_r_y1 = cur_apply_r_win[3];
				PE_CEO_PE1_H15A0_RdFL(pe1_lrcr_ctrl_06);
				PE_CEO_PE1_H15A0_RdFL(pe1_lrcr_ctrl_07);
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_06, x0,	(cur_apply_r_win[0]>>1));
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_06, y0,	cur_apply_r_win[1]);
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_07, x1,	(cur_apply_r_win[2]>>1));
				PE_CEO_PE1_H15A0_Wr01(pe1_lrcr_ctrl_07, y1,	cur_apply_r_win[3]);
				PE_CEO_PE1_H15A0_WrFL(pe1_lrcr_ctrl_06);
				PE_CEO_PE1_H15A0_WrFL(pe1_lrcr_ctrl_07);
			}
		}
		else
		{
			PE_WIN_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
int PE_WIN_HW_H15_GetCurWinSettings(PE_WIN_HW_H15_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_WIN_HW_H15_SETTINGS_T *pInfo = &_g_pe_win_hw_h15_info;
	PE_WIN_HW_H15_CTRL_T *win_info = &_g_pe_win_hw_h15_info.win_inf;
	PE_WIN_HW_H15_LRCR_CTRL_T *lrcr_info = &_g_pe_win_hw_h15_info.lrcr_inf;
	PE_WIN_HW_H15_TNR_CTRL_T *tnr_info = &_g_pe_win_hw_h15_info.tnr_inf;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			memcpy(pstParams,pInfo,sizeof(PE_WIN_HW_H15_SETTINGS_T));
			PE_WIN_HW_H15_DBG_PRINT("act0:%d,%d,%d,%d, act1:%d,%d,%d,%d\n",\
				win_info->act0_x0,win_info->act0_y0,win_info->act0_x1,win_info->act0_y1,\
				win_info->act1_x0,win_info->act1_y0,win_info->act1_x1,win_info->act1_y1);
			PE_WIN_HW_H15_DBG_PRINT("win0:%d,%d,%d,%d, win1:%d,%d,%d,%d\n",\
				win_info->win0_x0,win_info->win0_y0,win_info->win0_x1,win_info->win0_y1,\
				win_info->win1_x0,win_info->win1_y0,win_info->win1_x1,win_info->win1_y1);
			PE_WIN_HW_H15_DBG_PRINT("h,v_max:%d,%d, w0,w1,w01_en:[%d,%d],[%d,%d],[%d,%d]\n",\
				win_info->h_max,win_info->v_max,win_info->win0_en[0],win_info->win0_en[1],\
				win_info->win1_en[0],win_info->win1_en[1],win_info->win01_en[0],win_info->win01_en[1]);
			PE_WIN_HW_H15_DBG_PRINT("histo_l,r, apply_r : %d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d,%d\n",\
				lrcr_info->histo_l_x0,lrcr_info->histo_l_y0,lrcr_info->histo_l_x1,lrcr_info->histo_l_y1,\
				lrcr_info->histo_r_x0,lrcr_info->histo_r_y0,lrcr_info->histo_r_x1,lrcr_info->histo_r_y1,\
				lrcr_info->apply_r_x0,lrcr_info->apply_r_y0,lrcr_info->apply_r_x1,lrcr_info->apply_r_y1);
			PE_WIN_HW_H15_DBG_PRINT("tnr win h,v size0:%d,%d, size1:%d,%d\n",\
				tnr_info->h_size0,tnr_info->v_size0,tnr_info->h_size1,tnr_info->v_size1);
		}
		else
		{
			PE_WIN_HW_H15_DBG_PRINT("nothing to do.\n");
		}
	} while (0);
	return ret;
}

