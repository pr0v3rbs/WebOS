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

#include "pe_hw_m14.h"
#include "pe_reg_m14.h"
#include "pe_tsk_m14.h"
#include "pe_win_hw_m14.h"
#include "pe_hst_hw_m14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_WIN_HW_M14_FHD_H_MAX		1920
#define PE_WIN_HW_M14_FHD_V_MAX		1080
#define PE_WIN_HW_M14_SHP_WIN		0
#define PE_WIN_HW_M14_CEN_WIN		1
#define PE_WIN_HW_M14_WIN_EN_OFF	0
#define PE_WIN_HW_M14_WIN_EN__ON	1

/* 20131124, sj.youm, workaround for right side shp boundary error, use offset 4 */
/* 20140312, sj.youm, undef becuase of right side vertical blur issue on 16:9 */
#undef PE_WIN_HW_M14B_USE_RIGHT_SIDE_OFFSET

/* 20140316, sj.youm, workaround for right side shp boundary error for 3d(ss) mode */
#define PE_WIN_HW_M14B_USE_RIGHT_SIDE_OFFSET_FOR_3D

#define PE_WIN_HW_M14_WIN_OFFSET_MAX	(4)

/* 20140305, sj.youm, use win0 for lrcr win because of single path */
#define PE_WIN_HW_M14B_USE_WIN_0_ONLY

/* 20140306, sj.youm, workaround for pic win setting error(4:3 on OQA)  */
#define PE_WIN_HW_M14_OVERWRITE_PIC_WIN

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_WIN_HW_M14_ERROR	printk

#define PE_WIN_HW_M14_DBG_PRINT(fmt,args...)	\
	if(_g_win_hw_m14_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)
#define PE_WIN_HW_M14_CHECK_CODE(_checker,_action,fmt,args...)	\
	{if(_checker){PE_WIN_HW_M14_ERROR(fmt,##args);_action;}}

#define PE_WIN_HW_M14_DIVIDE(x,y)	(((((x)<<10)/y)+512)>>10)

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
static void PE_WIN_HW_M14_SetVerWinReverse(UINT32 *y0,UINT32 *y1,UINT32 v_max);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_win_hw_m14_trace=0x0;	//default should be off.
static PE_WIN_HW_M14_SETTINGS_T _g_pe_win_hw_m14_info;

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
int PE_WIN_HW_M14_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14BX)
		{
			memset(&_g_pe_win_hw_m14_info,0xffff,sizeof(PE_WIN_HW_M14_SETTINGS_T));
			PE_WIN_HW_M14_DBG_PRINT("pe1 win ctrl init, inside.\n");
	        PE_PE1_M14B0_QWr05(pe1_win1_ctrl_00,	win0_en,		0x1,\
													win1_en,		0x1,\
													win01_en,		0x1,\
													win_en,			0x1,\
            										win_outside,	0x0);
	        PE_PE1_M14B0_QWr05(pe1_win2_ctrl_00,	win0_en,		0x1,\
													win1_en,		0x1,\
													win01_en,		0x1,\
													win_en,			0x1,\
            										win_outside,	0x0);
	        PE_PE1_M14B0_QWr05(pe1_win3_ctrl_00,	win0_en,		0x1,\
													win1_en,		0x1,\
													win01_en,		0x1,\
													win_en,			0x1,\
            										win_outside,	0x0);
		}
		else if(PE_KDRV_VER_M14AX)
		{
			memset(&_g_pe_win_hw_m14_info,0xffff,sizeof(PE_WIN_HW_M14_SETTINGS_T));
			PE_WIN_HW_M14_DBG_PRINT("pe1 win ctrl init, inside.\n");
	        PE_P1L_M14A0_QWr05(pe1_win1_ctrl_00,	win0_en,		0x1,\
													win1_en,		0x1,\
													win01_en,		0x1,\
													win_en,			0x1,\
            										win_outside,	0x0);
	        PE_P1L_M14A0_QWr05(pe1_win2_ctrl_00,	win0_en,		0x1,\
													win1_en,		0x1,\
													win01_en,		0x1,\
													win_en,			0x1,\
            										win_outside,	0x0);
	        PE_P1L_M14A0_QWr05(pe1_win3_ctrl_00,	win0_en,		0x1,\
													win1_en,		0x1,\
													win01_en,		0x1,\
													win_en,			0x1,\
            										win_outside,	0x0);
		}
		else
		{
			PE_WIN_HW_M14_DBG_PRINT("nothing to do\n");	ret = RET_OK;
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
int PE_WIN_HW_M14_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_WIN_HW_M14_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(WIN)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_win_hw_m14_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(WIN)))? 0x1:0x0;
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
static void PE_WIN_HW_M14_SetVerWinReverse(UINT32 *y0,UINT32 *y1,UINT32 v_max)
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
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_WIN_HW_M14_SetEnhancingWindow(PE_TSK_M14_CFG_T *pCfg)
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
	UINT32 set_act_flag, set_win_flag;
	UINT32 win0_en[2], win1_en[2], win01_en[2];	//[0]:shp, [1]:cen
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	LX_PE_INF_DISPLAY_T *disp1_info = &pCfg->inf.disp_info[LX_PE_WIN_1];
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_WIN_HW_M14_CTRL_T *win_info=&_g_pe_win_hw_m14_info.win_inf;
	#ifdef PE_WIN_HW_M14B_USE_RIGHT_SIDE_OFFSET_FOR_3D
	UINT32 is_pe1_3d_mode;
	#endif
	#ifdef PE_WIN_HW_M14_OVERWRITE_PIC_WIN
	PE_TSK_M14_OPR_T *p_opr = &(pCfg->opr);
	PE_M14B0_PE1_WIN1_CTRL_01_T *reg_pe1_win1_ctrl_01 = NULL;
	PE_M14B0_PE1_WIN1_CTRL_02_T *reg_pe1_win1_ctrl_02 = NULL;
	PE_M14B0_PE1_WIN1_CTRL_03_T *reg_pe1_win1_ctrl_03 = NULL;
	PE_M14B0_PE1_WIN1_CTRL_04_T *reg_pe1_win1_ctrl_04 = NULL;
	PE_M14B0_PE1_WIN2_CTRL_01_T *reg_pe1_win2_ctrl_01 = NULL;
	PE_M14B0_PE1_WIN2_CTRL_02_T *reg_pe1_win2_ctrl_02 = NULL;
	PE_M14B0_PE1_WIN2_CTRL_03_T *reg_pe1_win2_ctrl_03 = NULL;
	PE_M14B0_PE1_WIN2_CTRL_04_T *reg_pe1_win2_ctrl_04 = NULL;
	PE_M14B0_PE1_WIN3_CTRL_01_T *reg_pe1_win3_ctrl_01 = NULL;
	PE_M14B0_PE1_WIN3_CTRL_02_T *reg_pe1_win3_ctrl_02 = NULL;
	PE_M14B0_PE1_WIN3_CTRL_03_T *reg_pe1_win3_ctrl_03 = NULL;
	PE_M14B0_PE1_WIN3_CTRL_04_T *reg_pe1_win3_ctrl_04 = NULL;
	UINT32 reg_pe1_win1_ctrl_01_data;
	UINT32 reg_pe1_win1_ctrl_02_data;
	UINT32 reg_pe1_win1_ctrl_03_data;
	UINT32 reg_pe1_win1_ctrl_04_data;
	UINT32 reg_pe1_win2_ctrl_01_data;
	UINT32 reg_pe1_win2_ctrl_02_data;
	UINT32 reg_pe1_win2_ctrl_03_data;
	UINT32 reg_pe1_win2_ctrl_04_data;
	UINT32 reg_pe1_win3_ctrl_01_data;
	UINT32 reg_pe1_win3_ctrl_02_data;
	UINT32 reg_pe1_win3_ctrl_03_data;
	UINT32 reg_pe1_win3_ctrl_04_data;
	reg_pe1_win1_ctrl_01 = (PE_M14B0_PE1_WIN1_CTRL_01_T *)&(reg_pe1_win1_ctrl_01_data);
	reg_pe1_win1_ctrl_02 = (PE_M14B0_PE1_WIN1_CTRL_02_T *)&(reg_pe1_win1_ctrl_02_data);
	reg_pe1_win1_ctrl_03 = (PE_M14B0_PE1_WIN1_CTRL_03_T *)&(reg_pe1_win1_ctrl_03_data);
	reg_pe1_win1_ctrl_04 = (PE_M14B0_PE1_WIN1_CTRL_04_T *)&(reg_pe1_win1_ctrl_04_data);
	reg_pe1_win2_ctrl_01 = (PE_M14B0_PE1_WIN2_CTRL_01_T *)&(reg_pe1_win2_ctrl_01_data);
	reg_pe1_win2_ctrl_02 = (PE_M14B0_PE1_WIN2_CTRL_02_T *)&(reg_pe1_win2_ctrl_02_data);
	reg_pe1_win2_ctrl_03 = (PE_M14B0_PE1_WIN2_CTRL_03_T *)&(reg_pe1_win2_ctrl_03_data);
	reg_pe1_win2_ctrl_04 = (PE_M14B0_PE1_WIN2_CTRL_04_T *)&(reg_pe1_win2_ctrl_04_data);
	reg_pe1_win3_ctrl_01 = (PE_M14B0_PE1_WIN3_CTRL_01_T *)&(reg_pe1_win3_ctrl_01_data);
	reg_pe1_win3_ctrl_02 = (PE_M14B0_PE1_WIN3_CTRL_02_T *)&(reg_pe1_win3_ctrl_02_data);
	reg_pe1_win3_ctrl_03 = (PE_M14B0_PE1_WIN3_CTRL_03_T *)&(reg_pe1_win3_ctrl_03_data);
	reg_pe1_win3_ctrl_04 = (PE_M14B0_PE1_WIN3_CTRL_04_T *)&(reg_pe1_win3_ctrl_04_data);
	#endif
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14)
		{
			if(disp0_info->out_type==LX_PE_OUT_UD)
			{
				h_max=PE_WIN_HW_M14_FHD_H_MAX<<1;
				v_max=PE_WIN_HW_M14_FHD_V_MAX<<1;
				x0_ofst[LX_PE_WIN_0] = x0_ofst[LX_PE_WIN_1] = 0;
				x1_ofst[LX_PE_WIN_0] = x1_ofst[LX_PE_WIN_1] = 0;
				y0_ofst[LX_PE_WIN_0] = y0_ofst[LX_PE_WIN_1] = 0;
				y1_ofst[LX_PE_WIN_0] = y1_ofst[LX_PE_WIN_1] = 0;
			}
			else
			{
				h_max=PE_WIN_HW_M14_FHD_H_MAX;
				v_max=PE_WIN_HW_M14_FHD_V_MAX;
				#ifdef PE_WIN_HW_M14B_USE_RIGHT_SIDE_OFFSET
				if(PE_KDRV_VER_M14BX)
				{
					x0_ofst[LX_PE_WIN_0] = (ctrl0_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX)? 3:0;
					x0_ofst[LX_PE_WIN_1] = (ctrl1_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX)? 3:0;
					x1_ofst[LX_PE_WIN_0] = x1_ofst[LX_PE_WIN_1] = PE_WIN_HW_M14_WIN_OFFSET_MAX;
				}
				else
				#endif
				{
					#ifdef PE_WIN_HW_M14B_USE_RIGHT_SIDE_OFFSET_FOR_3D
					/* sync with PE_CMN_HW_M14_SetOperationMode() */
					if(PE_KDRV_VER_M14BX)
					{
						switch(disp0_info->out_type)
						{
							case LX_PE_OUT_DUAL_SS:
								is_pe1_3d_mode = 1;
								break;
							case LX_PE_OUT_DUAL_TB:
							case LX_PE_OUT_DUAL_FULL:
							case LX_PE_OUT_2D_3D:
							case LX_PE_OUT_2D:
							case LX_PE_OUT_3D_2D:
							case LX_PE_OUT_UD:
							case LX_PE_OUT_TB:
							case LX_PE_OUT_SS:
							case LX_PE_OUT_FS:
							case LX_PE_OUT_PIP:
							default:
								is_pe1_3d_mode = 0;
								break;
						}
					}
					else
					{
						is_pe1_3d_mode = 0;
					}
					x0_ofst[LX_PE_WIN_0] = (ctrl0_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX)? 3:0;
					x0_ofst[LX_PE_WIN_1] = (ctrl1_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX)? 3:0;
					/* add appling x1 offset for 3d */
					x1_ofst[LX_PE_WIN_0] = \
						(ctrl0_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX || is_pe1_3d_mode)? 3:0;
					x1_ofst[LX_PE_WIN_1] = \
						(ctrl1_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX || is_pe1_3d_mode)? 3:0;
					#else
					x0_ofst[LX_PE_WIN_0] = x1_ofst[LX_PE_WIN_0] = \
						(ctrl0_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX)? 3:0;
					x0_ofst[LX_PE_WIN_1] = x1_ofst[LX_PE_WIN_1] = \
						(ctrl1_info->m_win_h_size<PE_WIN_HW_M14_FHD_H_MAX)? 3:0;
					#endif
				}
				y0_ofst[LX_PE_WIN_0] = y1_ofst[LX_PE_WIN_0] = \
					(ctrl0_info->m_win_v_size<PE_WIN_HW_M14_FHD_V_MAX)? 3:0;
				y0_ofst[LX_PE_WIN_1] = y1_ofst[LX_PE_WIN_1] = \
					(ctrl1_info->m_win_v_size<PE_WIN_HW_M14_FHD_V_MAX)? 3:0;
			}
			/* 2012xxxx, sj.youm, inner pattern for 2d,3d and so on. */
			is_invalid_size[LX_PE_WIN_0] = \
				(ctrl0_info->m_win_h_size <= (PE_WIN_HW_M14_WIN_OFFSET_MAX<<1) || \
				ctrl0_info->m_win_v_size <= (PE_WIN_HW_M14_WIN_OFFSET_MAX<<1))? 1:0;
			is_invalid_size[LX_PE_WIN_1] = \
				(ctrl1_info->m_win_h_size <= (PE_WIN_HW_M14_WIN_OFFSET_MAX<<1) || \
				ctrl1_info->m_win_v_size <= (PE_WIN_HW_M14_WIN_OFFSET_MAX<<1))? 1:0;
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
				x0[LX_PE_WIN_0] = ctrl0_info->m_win_h_ofst;
				y0[LX_PE_WIN_0] = ctrl0_info->m_win_v_ofst;
				x1[LX_PE_WIN_0] = x0[LX_PE_WIN_0]+ctrl0_info->m_win_h_size-1;
				y1[LX_PE_WIN_0] = y0[LX_PE_WIN_0]+ctrl0_info->m_win_v_size-1;
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
				x0[LX_PE_WIN_1] = ctrl1_info->m_win_h_ofst;
				y0[LX_PE_WIN_1] = ctrl1_info->m_win_v_ofst;
				x1[LX_PE_WIN_1] = x0[LX_PE_WIN_1]+ctrl1_info->m_win_h_size-1;
				y1[LX_PE_WIN_1] = y0[LX_PE_WIN_1]+ctrl1_info->m_win_v_size-1;
			}

			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_TB:
					case LX_PE_OUT_SS:
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
						break;
					case LX_PE_OUT_UD:
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0]<<1;
							act0_y0 = y0[LX_PE_WIN_0];
							act0_x1 = (x1[LX_PE_WIN_0]<<1)+1;
							act0_y1 = y1[LX_PE_WIN_0];
							win0_x0 = (x0[LX_PE_WIN_0]<<1)+x0_ofst[LX_PE_WIN_0];
							win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
							win0_x1 = (x1[LX_PE_WIN_0]<<1)+1-x1_ofst[LX_PE_WIN_0];
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
						{
							/* L : window 0 */
							act0_x0 = x0[LX_PE_WIN_0];
							act0_y0 = y0[LX_PE_WIN_0]<<1;
							act0_x1 = x1[LX_PE_WIN_0];
							act0_y1 = (y1[LX_PE_WIN_0]<<1)+1;
							win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
							win0_y0 = (y0[LX_PE_WIN_0]<<1)+y0_ofst[LX_PE_WIN_0];
							win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
							win0_y1 = (y1[LX_PE_WIN_0]<<1)+1-y1_ofst[LX_PE_WIN_0];
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
					case LX_PE_OUT_2D:
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
				if(PE_KDRV_VER_M14BX)
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
				else	//PE_KDRV_VER_M14AX
				{
					switch(disp0_info->out_type)
					{
						case LX_PE_OUT_TB:		/* N/A on internal frc */
						case LX_PE_OUT_2D_3D:
							{
								/* L : window 0 */
								act0_x0 = x0[LX_PE_WIN_0];
								act0_y0 = y0[LX_PE_WIN_0]<<1;
								act0_x1 = x1[LX_PE_WIN_0];
								act0_y1 = (y1[LX_PE_WIN_0]<<1)+1;
								win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
								win0_y0 = (y0[LX_PE_WIN_0]<<1)+y0_ofst[LX_PE_WIN_0];
								win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
								win0_y1 = (y1[LX_PE_WIN_0]<<1)+1-y1_ofst[LX_PE_WIN_0];
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
						case LX_PE_OUT_SS:		/* N/A on internal frc */
						case LX_PE_OUT_UD:
							{
								/* L : window 0 */
								act0_x0 = x0[LX_PE_WIN_0]<<1;
								act0_y0 = y0[LX_PE_WIN_0];
								act0_x1 = (x1[LX_PE_WIN_0]<<1)+1;
								act0_y1 = y1[LX_PE_WIN_0];
								win0_x0 = (x0[LX_PE_WIN_0]<<1)+x0_ofst[LX_PE_WIN_0];
								win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
								win0_x1 = (x1[LX_PE_WIN_0]<<1)+1-x1_ofst[LX_PE_WIN_0];
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
						case LX_PE_OUT_DUAL_TB:
							{
								/* L : window 0 */
								act0_x0 = x0[LX_PE_WIN_0];
								act0_y0 = y0[LX_PE_WIN_0]<<1;
								act0_x1 = x1[LX_PE_WIN_0];
								act0_y1 = (y1[LX_PE_WIN_0]<<1)+1;
								win0_x0 = x0[LX_PE_WIN_0]+x0_ofst[LX_PE_WIN_0];
								win0_y0 = (y0[LX_PE_WIN_0]<<1)+y0_ofst[LX_PE_WIN_0];
								win0_x1 = x1[LX_PE_WIN_0]-x1_ofst[LX_PE_WIN_0];
								win0_y1 = (y1[LX_PE_WIN_0]<<1)+1-y1_ofst[LX_PE_WIN_0];
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
						case LX_PE_OUT_DUAL_SS:
							{
								/* L : window 0 */
								act0_x0 = x0[LX_PE_WIN_0]<<1;
								act0_y0 = y0[LX_PE_WIN_0];
								act0_x1 = (x1[LX_PE_WIN_0]<<1)+1;
								act0_y1 = y1[LX_PE_WIN_0];
								win0_x0 = (x0[LX_PE_WIN_0]<<1)+x0_ofst[LX_PE_WIN_0];
								win0_y0 = y0[LX_PE_WIN_0]+y0_ofst[LX_PE_WIN_0];
								win0_x1 = (x1[LX_PE_WIN_0]<<1)+1-x1_ofst[LX_PE_WIN_0];
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
			}
			/* reverse mode */
			if(disp0_info->mode.is_reverse)
			{
				PE_WIN_HW_M14_SetVerWinReverse(&act0_y0,&act0_y1,v_max);
				PE_WIN_HW_M14_SetVerWinReverse(&act1_y0,&act1_y1,v_max);
				PE_WIN_HW_M14_SetVerWinReverse(&win0_y0,&win0_y1,v_max);
				PE_WIN_HW_M14_SetVerWinReverse(&win1_y0,&win1_y1,v_max);
			}
			/* 20130206, sj.youm, check resize_case that is scl_resize and not file/photo play */
			is_resize_case = (ctrl0_info->scl_resize && \
				!(disp0_info->src_type==LX_PE_SRC_DTV&&(disp0_info->dtv_type==LX_PE_DTV_FILEPLAY || \
				disp0_info->dtv_type==LX_PE_DTV_PHOTOPLAY)))? 1:0;
			/* win enable [0]:shp win, [1]:cen win */
			/* 20130130, sj.youm, on shp,cen win for split window because of function enable */
			/* 20130206, sj.youm, off shp, on cen win for resize(non pip) because of shp artifact */
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				switch(disp0_info->out_type)
				{
					case LX_PE_OUT_TB:
					case LX_PE_OUT_SS:
					case LX_PE_OUT_UD:
						win0_en[PE_WIN_HW_M14_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_M14_WIN_EN_OFF:PE_WIN_HW_M14_WIN_EN__ON;
						win1_en[PE_WIN_HW_M14_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_M14_WIN_EN_OFF:PE_WIN_HW_M14_WIN_EN__ON;
						win01_en[PE_WIN_HW_M14_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_M14_WIN_EN_OFF:PE_WIN_HW_M14_WIN_EN__ON;
						win0_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
						win1_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
						win01_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
						break;
					case LX_PE_OUT_PIP:
						if(ctrl0_info->hv_same)	//split window case
						{
							win0_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win1_en[PE_WIN_HW_M14_SHP_WIN]=(ctrl0_info->mute_on)? \
								PE_WIN_HW_M14_WIN_EN_OFF:PE_WIN_HW_M14_WIN_EN__ON;
							win01_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
							win0_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win1_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win01_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						}
						else
						{
							win0_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win1_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
							win01_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
							win0_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win1_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
							win01_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						}
						break;
					default:
						win0_en[PE_WIN_HW_M14_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_M14_WIN_EN_OFF:PE_WIN_HW_M14_WIN_EN__ON;
						win1_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						win01_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						win0_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
						win1_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						win01_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
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
							win0_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win1_en[PE_WIN_HW_M14_SHP_WIN]=(ctrl0_info->mute_on)? \
								PE_WIN_HW_M14_WIN_EN_OFF:PE_WIN_HW_M14_WIN_EN__ON;
							win01_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
							win0_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win1_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win01_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						}
						else
						{
							if (ctrl0_info->pmux_pos)	//pip mux before shp : win protection on
							{
								win0_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
								win1_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
								win01_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
							}
							else	//pip mux after shp : win protectio off(turn on shp win)
							{
								win0_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
								win1_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
								win01_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							}
							win0_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
							win1_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
							win01_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						}
						break;
					default:
						win0_en[PE_WIN_HW_M14_SHP_WIN]=(is_resize_case)? \
							PE_WIN_HW_M14_WIN_EN_OFF:PE_WIN_HW_M14_WIN_EN__ON;
						win1_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						win01_en[PE_WIN_HW_M14_SHP_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						win0_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN__ON;
						win1_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
						win01_en[PE_WIN_HW_M14_CEN_WIN]=PE_WIN_HW_M14_WIN_EN_OFF;
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
			#ifdef PE_WIN_HW_M14_OVERWRITE_PIC_WIN
			/* for debug, see PE_TSK_M14_RUN_STOP_OVWR */
			if (p_opr->pause_task == 3)
			{
				set_act_flag = set_win_flag = 0;
			}
			else
			{
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_RdFL(pe1_win1_ctrl_01);
					PE_PE1_M14B0_RdFL(pe1_win1_ctrl_02);
					PE_PE1_M14B0_RdFL(pe1_win1_ctrl_03);
					PE_PE1_M14B0_RdFL(pe1_win1_ctrl_04);
					PE_PE1_M14B0_RdFL(pe1_win2_ctrl_01);
					PE_PE1_M14B0_RdFL(pe1_win2_ctrl_02);
					PE_PE1_M14B0_RdFL(pe1_win2_ctrl_03);
					PE_PE1_M14B0_RdFL(pe1_win2_ctrl_04);
					PE_PE1_M14B0_RdFL(pe1_win3_ctrl_01);
					PE_PE1_M14B0_RdFL(pe1_win3_ctrl_02);
					PE_PE1_M14B0_RdFL(pe1_win3_ctrl_03);
					PE_PE1_M14B0_RdFL(pe1_win3_ctrl_04);
					reg_pe1_win1_ctrl_01_data = PE_PE1_M14B0_Rd(pe1_win1_ctrl_01);
					reg_pe1_win1_ctrl_02_data = PE_PE1_M14B0_Rd(pe1_win1_ctrl_02);
					reg_pe1_win1_ctrl_03_data = PE_PE1_M14B0_Rd(pe1_win1_ctrl_03);
					reg_pe1_win1_ctrl_04_data = PE_PE1_M14B0_Rd(pe1_win1_ctrl_04);
					reg_pe1_win2_ctrl_01_data = PE_PE1_M14B0_Rd(pe1_win2_ctrl_01);
					reg_pe1_win2_ctrl_02_data = PE_PE1_M14B0_Rd(pe1_win2_ctrl_02);
					reg_pe1_win2_ctrl_03_data = PE_PE1_M14B0_Rd(pe1_win2_ctrl_03);
					reg_pe1_win2_ctrl_04_data = PE_PE1_M14B0_Rd(pe1_win2_ctrl_04);
					reg_pe1_win3_ctrl_01_data = PE_PE1_M14B0_Rd(pe1_win3_ctrl_01);
					reg_pe1_win3_ctrl_02_data = PE_PE1_M14B0_Rd(pe1_win3_ctrl_02);
					reg_pe1_win3_ctrl_03_data = PE_PE1_M14B0_Rd(pe1_win3_ctrl_03);
					reg_pe1_win3_ctrl_04_data = PE_PE1_M14B0_Rd(pe1_win3_ctrl_04);
					/* pe1_win2 */
					set_act_flag = ((reg_pe1_win2_ctrl_01->win_w0_x0 != act0_x0) || \
						(reg_pe1_win2_ctrl_01->win_w0_y0 != act0_y0) || \
						(reg_pe1_win2_ctrl_02->win_w0_x1 != act0_x1) || \
						(reg_pe1_win2_ctrl_02->win_w0_y1 != act0_y1) || \
						(reg_pe1_win2_ctrl_03->win_w1_x0 != act1_x0) || \
						(reg_pe1_win2_ctrl_03->win_w1_y0 != act1_y0) || \
						(reg_pe1_win2_ctrl_04->win_w1_x1 != act1_x1) || \
						(reg_pe1_win2_ctrl_04->win_w1_y1 != act1_y1))? 1:0;
					/* pe1_win1, pe1_win3 */
					set_win_flag = ((reg_pe1_win1_ctrl_01->win_w0_x0 != win0_x0) || \
						(reg_pe1_win1_ctrl_01->win_w0_y0 != win0_y0) || \
						(reg_pe1_win1_ctrl_02->win_w0_x1 != win0_x1) || \
						(reg_pe1_win1_ctrl_02->win_w0_y1 != win0_y1) || \
						(reg_pe1_win1_ctrl_03->win_w1_x0 != win1_x0) || \
						(reg_pe1_win1_ctrl_03->win_w1_y0 != win1_y0) || \
						(reg_pe1_win1_ctrl_04->win_w1_x1 != win1_x1) || \
						(reg_pe1_win1_ctrl_04->win_w1_y1 != win1_y1) || \
						(reg_pe1_win3_ctrl_01->win_w0_x0 != win0_x0) || \
						(reg_pe1_win3_ctrl_01->win_w0_y0 != win0_y0) || \
						(reg_pe1_win3_ctrl_02->win_w0_x1 != win0_x1) || \
						(reg_pe1_win3_ctrl_02->win_w0_y1 != win0_y1) || \
						(reg_pe1_win3_ctrl_03->win_w1_x0 != win1_x0) || \
						(reg_pe1_win3_ctrl_03->win_w1_y0 != win1_y0) || \
						(reg_pe1_win3_ctrl_04->win_w1_x1 != win1_x1) || \
						(reg_pe1_win3_ctrl_04->win_w1_y1 != win1_y1))? 1:0;
				}
				else
				{
					set_act_flag = set_win_flag = 0;
				}
			}
			#else
			set_act_flag = set_win_flag = 0;
			#endif
			/* L : set act */
			if((win_info->act0_x0 != act0_x0)|| (win_info->act0_y0 != act0_y0)|| \
				(win_info->act0_x1 != act0_x1)|| (win_info->act0_y1 != act0_y1)|| \
				(win_info->act1_x0 != act1_x0)|| (win_info->act1_y0 != act1_y0)|| \
				(win_info->act1_x1 != act1_x1)|| (win_info->act1_y1 != act1_y1)|| \
				(set_act_flag == 1))
			{
				PE_WIN_HW_M14_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_M14_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,ctrl0_info->m_win_h_size,\
					ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,ctrl1_info->m_win_v_ofst,\
					ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
				PE_WIN_HW_M14_DBG_PRINT("[set]act0 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->act0_x0,win_info->act0_y0,win_info->act0_x1,\
					win_info->act0_y1,act0_x0,act0_y0,act0_x1,act0_y1);
				PE_WIN_HW_M14_DBG_PRINT("[set]act1 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->act1_x0,win_info->act1_y0,win_info->act1_x1,\
					win_info->act1_y1,act1_x0,act1_y0,act1_x1,act1_y1);
				PE_WIN_HW_M14_DBG_PRINT("[set]set_act_flag:%d\n", set_act_flag);
				if(PE_KDRV_VER_M14BX)
				{
		            PE_PE1_M14B0_QWr02(pe1_win2_ctrl_01,	win_w0_x0,	act0_x0,\
		            										win_w0_y0,	act0_y0);
		            PE_PE1_M14B0_QWr02(pe1_win2_ctrl_02,	win_w0_x1,	act0_x1,\
		            										win_w0_y1,	act0_y1);
		            PE_PE1_M14B0_QWr02(pe1_win2_ctrl_03,	win_w1_x0,	act1_x0,\
		            										win_w1_y0,	act1_y0);
		            PE_PE1_M14B0_QWr02(pe1_win2_ctrl_04,	win_w1_x1,	act1_x1,\
		            										win_w1_y1,	act1_y1);
					/* for 3d(tb,ss) apl window setting with fhd input for ext frc,3dfmt case */
					if(disp0_info->mode.is_ext_frc && disp0_info->out_type==LX_PE_OUT_TB)
					{
			            PE_PE1_M14B0_QWr02(pe1_apl_ctrl_00,	apl_win_ctrl_x0, act0_x0,\
			            									apl_win_ctrl_y0, (act0_y0<<1));
			            PE_PE1_M14B0_QWr02(pe1_apl_ctrl_01,	apl_win_ctrl_x1, act0_x1,\
			            									apl_win_ctrl_y1, ((act0_y1<<1)+1));
					}
					else if(disp0_info->mode.is_ext_frc && disp0_info->out_type==LX_PE_OUT_SS)
					{
			            PE_PE1_M14B0_QWr02(pe1_apl_ctrl_00,	apl_win_ctrl_x0, (act0_x0<<1),\
			            									apl_win_ctrl_y0, act0_y0);
			            PE_PE1_M14B0_QWr02(pe1_apl_ctrl_01,	apl_win_ctrl_x1, ((act0_x1<<1)+1),\
			            									apl_win_ctrl_y1, act0_y1);
					}
					else
					{
			            PE_PE1_M14B0_QWr02(pe1_apl_ctrl_00,	apl_win_ctrl_x0, act0_x0,\
			            									apl_win_ctrl_y0, act0_y0);
			            PE_PE1_M14B0_QWr02(pe1_apl_ctrl_01,	apl_win_ctrl_x1, act0_x1,\
			            									apl_win_ctrl_y1, act0_y1);
					}
				}
				else if(PE_KDRV_VER_M14AX)
				{
		            PE_P1L_M14A0_QWr02(pe1_win2_ctrl_01,	win_w0_x0,	act0_x0,\
		            										win_w0_y0,	act0_y0);
		            PE_P1L_M14A0_QWr02(pe1_win2_ctrl_02,	win_w0_x1,	act0_x1,\
		            										win_w0_y1,	act0_y1);
		            PE_P1L_M14A0_QWr02(pe1_win2_ctrl_03,	win_w1_x0,	act1_x0,\
		            										win_w1_y0,	act1_y0);
		            PE_P1L_M14A0_QWr02(pe1_win2_ctrl_04,	win_w1_x1,	act1_x1,\
		            										win_w1_y1,	act1_y1);
					/* for 3d(tb,ss) apl window setting with fhd input for ext frc,3dfmt case */
					if(disp0_info->mode.is_ext_frc && disp0_info->out_type==LX_PE_OUT_TB)
					{
			            PE_P1L_M14A0_QWr02(pe1_apl_ctrl_00,	apl_win_ctrl_x0, act0_x0,\
			            									apl_win_ctrl_y0, (act0_y0<<1));
			            PE_P1L_M14A0_QWr02(pe1_apl_ctrl_01,	apl_win_ctrl_x1, act0_x1,\
			            									apl_win_ctrl_y1, ((act0_y1<<1)+1));
					}
					else if(disp0_info->mode.is_ext_frc && disp0_info->out_type==LX_PE_OUT_SS)
					{
			            PE_P1L_M14A0_QWr02(pe1_apl_ctrl_00,	apl_win_ctrl_x0, (act0_x0<<1),\
			            									apl_win_ctrl_y0, act0_y0);
			            PE_P1L_M14A0_QWr02(pe1_apl_ctrl_01,	apl_win_ctrl_x1, ((act0_x1<<1)+1),\
			            									apl_win_ctrl_y1, act0_y1);
					}
					else
					{
			            PE_P1L_M14A0_QWr02(pe1_apl_ctrl_00,	apl_win_ctrl_x0, act0_x0,\
			            									apl_win_ctrl_y0, act0_y0);
			            PE_P1L_M14A0_QWr02(pe1_apl_ctrl_01,	apl_win_ctrl_x1, act0_x1,\
			            									apl_win_ctrl_y1, act0_y1);
					}
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
			/* L : set win */
			if((win_info->win0_x0 != win0_x0)|| (win_info->win0_y0 != win0_y0)|| \
				(win_info->win0_x1 != win0_x1)|| (win_info->win0_y1 != win0_y1)|| \
				(win_info->win1_x0 != win1_x0)|| (win_info->win1_y0 != win1_y0)|| \
				(win_info->win1_x1 != win1_x1)|| (win_info->win1_y1 != win1_y1)|| \
				(set_win_flag == 1))
			{
				PE_WIN_HW_M14_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_M14_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,ctrl0_info->m_win_h_size,\
					ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,ctrl1_info->m_win_v_ofst,\
					ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
				PE_WIN_HW_M14_DBG_PRINT("[set]win0 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->win0_x0,win_info->win0_y0,win_info->win0_x1,\
					win_info->win0_y1,win0_x0,win0_y0,win0_x1,win0_y1);
				PE_WIN_HW_M14_DBG_PRINT("[set]win1 : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					win_info->win1_x0,win_info->win1_y0,win_info->win1_x1,\
					win_info->win1_y1,win1_x0,win1_y0,win1_x1,win1_y1);
				PE_WIN_HW_M14_DBG_PRINT("[set]set_win_flag:%d\n", set_win_flag);
				if(PE_KDRV_VER_M14BX)
				{
		            PE_PE1_M14B0_QWr02(pe1_win1_ctrl_01,	win_w0_x0,	win0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_PE1_M14B0_QWr02(pe1_win1_ctrl_02,	win_w0_x1,	win0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_PE1_M14B0_QWr02(pe1_win1_ctrl_03,	win_w1_x0,	win1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_PE1_M14B0_QWr02(pe1_win1_ctrl_04,	win_w1_x1,	win1_x1,\
		            										win_w1_y1,	win1_y1);
		            PE_PE1_M14B0_QWr02(pe1_win3_ctrl_01,	win_w0_x0,	win0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_PE1_M14B0_QWr02(pe1_win3_ctrl_02,	win_w0_x1,	win0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_PE1_M14B0_QWr02(pe1_win3_ctrl_03,	win_w1_x0,	win1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_PE1_M14B0_QWr02(pe1_win3_ctrl_04,	win_w1_x1,	win1_x1,\
		            										win_w1_y1,	win1_y1);
				}
				else if(PE_KDRV_VER_M14AX)
				{
		            PE_P1L_M14A0_QWr02(pe1_win1_ctrl_01,	win_w0_x0,	win0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_P1L_M14A0_QWr02(pe1_win1_ctrl_02,	win_w0_x1,	win0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_P1L_M14A0_QWr02(pe1_win1_ctrl_03,	win_w1_x0,	win1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_P1L_M14A0_QWr02(pe1_win1_ctrl_04,	win_w1_x1,	win1_x1,\
		            										win_w1_y1,	win1_y1);
		            PE_P1L_M14A0_QWr02(pe1_win3_ctrl_01,	win_w0_x0,	win0_x0,\
		            										win_w0_y0,	win0_y0);
		            PE_P1L_M14A0_QWr02(pe1_win3_ctrl_02,	win_w0_x1,	win0_x1,\
		            										win_w0_y1,	win0_y1);
		            PE_P1L_M14A0_QWr02(pe1_win3_ctrl_03,	win_w1_x0,	win1_x0,\
		            										win_w1_y0,	win1_y0);
		            PE_P1L_M14A0_QWr02(pe1_win3_ctrl_04,	win_w1_x1,	win1_x1,\
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
			if((win_info->win0_en[PE_WIN_HW_M14_SHP_WIN] != win0_en[PE_WIN_HW_M14_SHP_WIN]) || \
				(win_info->win1_en[PE_WIN_HW_M14_SHP_WIN] != win1_en[PE_WIN_HW_M14_SHP_WIN]) || \
				(win_info->win01_en[PE_WIN_HW_M14_SHP_WIN] != win01_en[PE_WIN_HW_M14_SHP_WIN]))
			{
				PE_WIN_HW_M14_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_M14_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,ctrl0_info->m_win_h_size,\
					ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,ctrl1_info->m_win_v_ofst,\
					ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
				PE_WIN_HW_M14_DBG_PRINT("[set]shp_win_en(0,1,01):%d,%d,%d \n",\
					win_info->win0_en[PE_WIN_HW_M14_SHP_WIN], win_info->win1_en[PE_WIN_HW_M14_SHP_WIN],\
					win_info->win01_en[PE_WIN_HW_M14_SHP_WIN]);
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QWr03(pe1_win1_ctrl_00,	win0_en,	win0_en[PE_WIN_HW_M14_SHP_WIN],\
															win1_en,	win1_en[PE_WIN_HW_M14_SHP_WIN],\
															win01_en,	win01_en[PE_WIN_HW_M14_SHP_WIN]);
					PE_PE1_M14B0_QWr03(pe1_win3_ctrl_00,	win0_en,	win0_en[PE_WIN_HW_M14_SHP_WIN],\
															win1_en,	win1_en[PE_WIN_HW_M14_SHP_WIN],\
															win01_en,	win01_en[PE_WIN_HW_M14_SHP_WIN]);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QWr03(pe1_win1_ctrl_00,	win0_en,	win0_en[PE_WIN_HW_M14_SHP_WIN],\
															win1_en,	win1_en[PE_WIN_HW_M14_SHP_WIN],\
															win01_en,	win01_en[PE_WIN_HW_M14_SHP_WIN]);
					PE_P1L_M14A0_QWr03(pe1_win3_ctrl_00,	win0_en,	win0_en[PE_WIN_HW_M14_SHP_WIN],\
															win1_en,	win1_en[PE_WIN_HW_M14_SHP_WIN],\
															win01_en,	win01_en[PE_WIN_HW_M14_SHP_WIN]);
				}
				win_info->win0_en[PE_WIN_HW_M14_SHP_WIN] = win0_en[PE_WIN_HW_M14_SHP_WIN];
				win_info->win1_en[PE_WIN_HW_M14_SHP_WIN] = win1_en[PE_WIN_HW_M14_SHP_WIN];
				win_info->win01_en[PE_WIN_HW_M14_SHP_WIN] = win01_en[PE_WIN_HW_M14_SHP_WIN];
			}
			/* cen win en */
			if((win_info->win0_en[PE_WIN_HW_M14_CEN_WIN] != win0_en[PE_WIN_HW_M14_CEN_WIN]) || \
				(win_info->win1_en[PE_WIN_HW_M14_CEN_WIN] != win1_en[PE_WIN_HW_M14_CEN_WIN]) || \
				(win_info->win01_en[PE_WIN_HW_M14_CEN_WIN] != win01_en[PE_WIN_HW_M14_CEN_WIN]))
			{
				PE_WIN_HW_M14_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_M14_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,ctrl0_info->m_win_h_size,\
					ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,ctrl1_info->m_win_v_ofst,\
					ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
				PE_WIN_HW_M14_DBG_PRINT("[set]cen_win_en(0,1,01):%d,%d,%d \n",\
					win_info->win0_en[PE_WIN_HW_M14_CEN_WIN], win_info->win1_en[PE_WIN_HW_M14_CEN_WIN],\
					win_info->win01_en[PE_WIN_HW_M14_CEN_WIN]);
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QWr03(pe1_win2_ctrl_00,	win0_en,	win0_en[PE_WIN_HW_M14_CEN_WIN],\
															win1_en,	win1_en[PE_WIN_HW_M14_CEN_WIN],\
															win01_en,	win01_en[PE_WIN_HW_M14_CEN_WIN]);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QWr03(pe1_win2_ctrl_00,	win0_en,	win0_en[PE_WIN_HW_M14_CEN_WIN],\
															win1_en,	win1_en[PE_WIN_HW_M14_CEN_WIN],\
															win01_en,	win01_en[PE_WIN_HW_M14_CEN_WIN]);
				}
				win_info->win0_en[PE_WIN_HW_M14_CEN_WIN] = win0_en[PE_WIN_HW_M14_CEN_WIN];
				win_info->win1_en[PE_WIN_HW_M14_CEN_WIN] = win1_en[PE_WIN_HW_M14_CEN_WIN];
				win_info->win01_en[PE_WIN_HW_M14_CEN_WIN] = win01_en[PE_WIN_HW_M14_CEN_WIN];
			}
			/* etc */
			if((win_info->h_max != h_max)|| (win_info->v_max != v_max))
			{
				PE_WIN_HW_M14_DBG_PRINT("[inf]ex_f:%d,rev:%d,out:%d,ofs0,1:%d,%d,%d,%d, %d,%d,%d,%d\n",\
					disp0_info->mode.is_ext_frc,disp0_info->mode.is_reverse,disp0_info->out_type,\
					x0_ofst[LX_PE_WIN_0],y0_ofst[LX_PE_WIN_0],x1_ofst[LX_PE_WIN_0],y1_ofst[LX_PE_WIN_0],\
					x0_ofst[LX_PE_WIN_1],y0_ofst[LX_PE_WIN_1],x1_ofst[LX_PE_WIN_1],y1_ofst[LX_PE_WIN_1]);
				PE_WIN_HW_M14_DBG_PRINT("[inf]o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,ctrl0_info->m_win_h_size,\
					ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,ctrl1_info->m_win_v_ofst,\
					ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
				PE_WIN_HW_M14_DBG_PRINT("[set]h,v_max : prev:%d,%d, cur:%d,%d\n",\
					win_info->h_max,win_info->v_max,h_max,v_max);
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_QWr02(pe1_top_ctrl_01,		width,	h_max,\
															height,	v_max);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_QWr02(pe1_top_ctrl_01,		width,	h_max,\
															height,	v_max);
				}
				win_info->h_max = h_max;
				win_info->v_max = v_max;
			}
		}
		else
		{
			PE_WIN_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set lrcr window periodically
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_WIN_HW_M14_SetLrcrWindow(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 h_max,v_max,pxl_cnt;
	SINT32 temp_h,temp_v,act_out_h,act_out_v,src_h,src_v,in_h,in_v,out_h,out_v;
	UINT32 l_x0,l_y0,l_x1,l_y1,r_x0,r_y0,r_x1,r_y1;
	UINT32 x0[LX_PE_WIN_NUM],y0[LX_PE_WIN_NUM],x1[LX_PE_WIN_NUM],y1[LX_PE_WIN_NUM];
	UINT32 cur_histo_l_win[4],cur_histo_r_win[4],cur_apply_r_win[4];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_WIN_HW_M14_LRCR_CTRL_T *lrcr_info=&_g_pe_win_hw_m14_info.lrcr_inf;
	PE_TSK_M14_LRC_HST_CTRL_T *lrc_ctrl = &pCfg->lrc;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14BX)
		{
			if(disp0_info->out_type==LX_PE_OUT_UD)
			{
				h_max=PE_WIN_HW_M14_FHD_H_MAX<<1;
				v_max=PE_WIN_HW_M14_FHD_V_MAX<<1;
			}
			else
			{
				h_max=PE_WIN_HW_M14_FHD_H_MAX;
				v_max=PE_WIN_HW_M14_FHD_V_MAX;
			}
			/* lrcr win default : not 0,0,1919,1079, but 0,0,1920,1080 */
			if(ctrl0_info->m_win_h_size)
			{
				x0[LX_PE_WIN_0] = ctrl0_info->m_win_h_ofst;
				x1[LX_PE_WIN_0] = x0[LX_PE_WIN_0]+ctrl0_info->m_win_h_size;
			}
			else
			{
				x0[LX_PE_WIN_0] = 0;
				x1[LX_PE_WIN_0] = h_max;
			}
			if(ctrl0_info->m_win_v_size)
			{
				/* [m14b0] doesn't matter 2d or 2dto3d */
				y0[LX_PE_WIN_0] = ctrl0_info->m_win_v_ofst;
				y1[LX_PE_WIN_0] = y0[LX_PE_WIN_0]+ctrl0_info->m_win_v_size;
			}
			else
			{
				y0[LX_PE_WIN_0] = 0;
				y1[LX_PE_WIN_0] = v_max;
			}
			if(ctrl1_info->m_win_h_size)
			{
				x0[LX_PE_WIN_1] = ctrl1_info->m_win_h_ofst;
				x1[LX_PE_WIN_1] = x0[LX_PE_WIN_1]+ctrl1_info->m_win_h_size;
			}
			else
			{
				x0[LX_PE_WIN_1] = 0;
				x1[LX_PE_WIN_1] = h_max;
			}
			if(ctrl1_info->m_win_v_size)
			{
				y0[LX_PE_WIN_1] = ctrl1_info->m_win_v_ofst;
				y1[LX_PE_WIN_1] = y0[LX_PE_WIN_1]+ctrl1_info->m_win_v_size;
			}
			else
			{
				y0[LX_PE_WIN_1] = 0;
				y1[LX_PE_WIN_1] = v_max;
			}
			/* [m14b0]
			* lrcr win setting for 3d : don't care for internal frc, because of ll mode on
			* here consider external frc case for 3d
			*/
			switch(disp0_info->out_type)
			{
				case LX_PE_OUT_2D:
				case LX_PE_OUT_2D_3D:
					if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 || \
						lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1 || \
						lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0 || \
						lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1)
					{
						if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 || \
							lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1)
						{
							src_v = ctrl0_info->pe0_src_v;
							in_v = ctrl0_info->i_win_v_size;
							/* [m14b0] doesn't matter 2d or 2dto3d */
							out_v = ctrl0_info->m_win_v_size;
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
							src_h = ctrl0_info->pe0_src_h;
							in_h = ctrl0_info->i_win_h_size;
							out_h = ctrl0_info->m_win_h_size;
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
						if(l_x1>PE_WIN_HW_M14_FHD_H_MAX)	l_x1=PE_WIN_HW_M14_FHD_H_MAX;
						if(l_y1>PE_WIN_HW_M14_FHD_V_MAX)	l_y1=PE_WIN_HW_M14_FHD_V_MAX;
						if(r_x1>PE_WIN_HW_M14_FHD_H_MAX)	r_x1=PE_WIN_HW_M14_FHD_H_MAX;
						if(r_y1>PE_WIN_HW_M14_FHD_V_MAX)	r_y1=PE_WIN_HW_M14_FHD_V_MAX;
						cur_histo_l_win[0] = l_x0;
						cur_histo_l_win[2] = l_x1;
						cur_histo_r_win[0] = r_x0;
						cur_histo_r_win[2] = r_x1;
						if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0|| \
							lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0)
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
						cur_histo_r_win[0] = x0[LX_PE_WIN_0];
						cur_histo_r_win[1] = y0[LX_PE_WIN_0];
						cur_histo_r_win[2] = x1[LX_PE_WIN_0];
						cur_histo_r_win[3] = y1[LX_PE_WIN_0];
						cur_apply_r_win[0] = x0[LX_PE_WIN_0];
						cur_apply_r_win[1] = y0[LX_PE_WIN_0];
						cur_apply_r_win[2] = x1[LX_PE_WIN_0];
						cur_apply_r_win[3] = y1[LX_PE_WIN_0];
					}
					break;
				case LX_PE_OUT_UD:
					if(disp0_info->user_o_type==LX_PE_OUT_TB && \
						disp0_info->user_i_type==LX_PE_3D_IN_TB)
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
					else if(disp0_info->user_o_type==LX_PE_OUT_SS && \
						disp0_info->user_i_type==LX_PE_3D_IN_SS)
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
					else
					{
						cur_histo_l_win[0] = x0[LX_PE_WIN_0];
						cur_histo_l_win[1] = y0[LX_PE_WIN_0];
						cur_histo_l_win[2] = x1[LX_PE_WIN_0];
						cur_histo_l_win[3] = y1[LX_PE_WIN_0];
						cur_histo_r_win[0] = x0[LX_PE_WIN_1];
						cur_histo_r_win[1] = y0[LX_PE_WIN_1];
						cur_histo_r_win[2] = x1[LX_PE_WIN_1];
						cur_histo_r_win[3] = y1[LX_PE_WIN_1];
						cur_apply_r_win[0] = x0[LX_PE_WIN_1];
						cur_apply_r_win[1] = y0[LX_PE_WIN_1];
						cur_apply_r_win[2] = x1[LX_PE_WIN_1];
						cur_apply_r_win[3] = y1[LX_PE_WIN_1];
					}
					break;
				case LX_PE_OUT_3D_2D:
				case LX_PE_OUT_TB:
				case LX_PE_OUT_SS:
				case LX_PE_OUT_FS:
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
						#ifdef PE_WIN_HW_M14B_USE_WIN_0_ONLY
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
					PE_WIN_HW_M14_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
						disp0_info->out_type,ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
						ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,\
						ctrl1_info->m_win_v_ofst,ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
					PE_WIN_HW_M14_DBG_PRINT("[set]histo_l_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
						lrcr_info->histo_l_x0,lrcr_info->histo_l_y0,lrcr_info->histo_l_x1,lrcr_info->histo_l_y1,\
						cur_histo_l_win[0],cur_histo_l_win[1],cur_histo_l_win[2],cur_histo_l_win[3]);
				}
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_02);
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_03);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, x0,		cur_histo_l_win[0]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, y0,		cur_histo_l_win[1]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_03, x1,		cur_histo_l_win[2]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_03, y1,		cur_histo_l_win[3]);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_02);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_03);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_02);
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_03);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, x0,		cur_histo_l_win[0]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, y0,		cur_histo_l_win[1]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_03, x1,		cur_histo_l_win[2]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_03, y1,		cur_histo_l_win[3]);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_02);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_03);
				}
				lrcr_info->histo_l_x0 = cur_histo_l_win[0];
				lrcr_info->histo_l_y0 = cur_histo_l_win[1];
				lrcr_info->histo_l_x1 = cur_histo_l_win[2];
				lrcr_info->histo_l_y1 = cur_histo_l_win[3];
				if((disp0_info->out_type==LX_PE_OUT_2D || \
					disp0_info->out_type==LX_PE_OUT_2D_3D) && \
					lrc_ctrl->win_mod>=PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 && \
					lrc_ctrl->win_mod<=PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1)
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
					if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0)
					{
						lrc_ctrl->tb0_t_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->tb0_t_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->tb0_t_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->tb0_t_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[0] = pxl_cnt;
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0)
					{
						lrc_ctrl->ss0_l_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->ss0_l_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->ss0_l_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->ss0_l_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[1] = pxl_cnt;
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1)
					{
						lrc_ctrl->tb1_t_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->tb1_t_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->tb1_t_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->tb1_t_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[2] = pxl_cnt;
					}
					else	//PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1
					{
						lrc_ctrl->ss1_l_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->ss1_l_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->ss1_l_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->ss1_l_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[3] = pxl_cnt;
					}
				}
			}
			if((lrcr_info->histo_r_x0!=cur_histo_r_win[0])||(lrcr_info->histo_r_y0!=cur_histo_r_win[1])|| \
				(lrcr_info->histo_r_x1!=cur_histo_r_win[2])||(lrcr_info->histo_r_y1!=cur_histo_r_win[3]))
			{
				/* too many prints */
				if(disp0_info->out_type!=LX_PE_OUT_2D && disp0_info->out_type!=LX_PE_OUT_2D_3D)
				{
					PE_WIN_HW_M14_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
						disp0_info->out_type,ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
						ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,\
						ctrl1_info->m_win_v_ofst,ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
					PE_WIN_HW_M14_DBG_PRINT("[set]histo_r_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
						lrcr_info->histo_r_x0,lrcr_info->histo_r_y0,lrcr_info->histo_r_x1,lrcr_info->histo_r_y1,\
						cur_histo_r_win[0],cur_histo_r_win[1],cur_histo_r_win[2],cur_histo_r_win[3]);
				}
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_04);
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_05);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, x0,		cur_histo_r_win[0]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, y0,		cur_histo_r_win[1]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_05, x1,		cur_histo_r_win[2]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_05, y1,		cur_histo_r_win[3]);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_04);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_05);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_04);
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_05);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, x0,		cur_histo_r_win[0]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, y0,		cur_histo_r_win[1]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_05, x1,		cur_histo_r_win[2]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_05, y1,		cur_histo_r_win[3]);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_04);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_05);
				}
				lrcr_info->histo_r_x0 = cur_histo_r_win[0];
				lrcr_info->histo_r_y0 = cur_histo_r_win[1];
				lrcr_info->histo_r_x1 = cur_histo_r_win[2];
				lrcr_info->histo_r_y1 = cur_histo_r_win[3];
				if((disp0_info->out_type==LX_PE_OUT_2D || \
					disp0_info->out_type==LX_PE_OUT_2D_3D) && \
					lrc_ctrl->win_mod>=PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 && \
					lrc_ctrl->win_mod<=PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1)
				{
					if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0)
					{
						lrc_ctrl->tb0_b_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->tb0_b_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->tb0_b_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->tb0_b_pos[3] = cur_histo_r_win[3];
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0)
					{
						lrc_ctrl->ss0_r_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->ss0_r_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->ss0_r_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->ss0_r_pos[3] = cur_histo_r_win[3];
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1)
					{
						lrc_ctrl->tb1_b_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->tb1_b_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->tb1_b_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->tb1_b_pos[3] = cur_histo_r_win[3];
					}
					else	//PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1
					{
						lrc_ctrl->ss1_r_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->ss1_r_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->ss1_r_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->ss1_r_pos[3] = cur_histo_r_win[3];
					}
				}
			}
			if((lrcr_info->apply_r_x0!=cur_apply_r_win[0])||(lrcr_info->apply_r_y0!=cur_apply_r_win[1])|| \
				(lrcr_info->apply_r_x1!=cur_apply_r_win[2])||(lrcr_info->apply_r_y1!=cur_apply_r_win[3]))
			{
				PE_WIN_HW_M14_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					disp0_info->out_type,ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
					ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,\
					ctrl1_info->m_win_v_ofst,ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
				PE_WIN_HW_M14_DBG_PRINT("[set]apply_r_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					lrcr_info->apply_r_x0,lrcr_info->apply_r_y0,lrcr_info->apply_r_x1,lrcr_info->apply_r_y1,\
					cur_apply_r_win[0],cur_apply_r_win[1],cur_apply_r_win[2],cur_apply_r_win[3]);
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_06);
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_07);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, x0,		cur_apply_r_win[0]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, y0,		cur_apply_r_win[1]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_07, x1,		cur_apply_r_win[2]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_07, y1,		cur_apply_r_win[3]);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_06);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_07);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_06);
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_07);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, x0,		cur_apply_r_win[0]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, y0,		cur_apply_r_win[1]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_07, x1,		cur_apply_r_win[2]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_07, y1,		cur_apply_r_win[3]);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_06);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_07);
				}
				lrcr_info->apply_r_x0 = cur_apply_r_win[0];
				lrcr_info->apply_r_y0 = cur_apply_r_win[1];
				lrcr_info->apply_r_x1 = cur_apply_r_win[2];
				lrcr_info->apply_r_y1 = cur_apply_r_win[3];
			}
		}
		else if(PE_KDRV_VER_M14AX)
		{
			if(disp0_info->out_type==LX_PE_OUT_UD)
			{
				h_max=PE_WIN_HW_M14_FHD_H_MAX<<1;
				v_max=PE_WIN_HW_M14_FHD_V_MAX<<1;
			}
			else
			{
				h_max=PE_WIN_HW_M14_FHD_H_MAX;
				v_max=PE_WIN_HW_M14_FHD_V_MAX;
			}
			/* lrcr win default : not 0,0,1919,1079, but 0,0,1920,1080 */
			if(ctrl0_info->m_win_h_size)
			{
				x0[LX_PE_WIN_0] = ctrl0_info->m_win_h_ofst;
				x1[LX_PE_WIN_0] = x0[LX_PE_WIN_0]+ctrl0_info->m_win_h_size;
			}
			else
			{
				x0[LX_PE_WIN_0] = 0;
				x1[LX_PE_WIN_0] = h_max;
			}
			if(ctrl0_info->m_win_v_size)
			{
				/* [m14a0] 2x because of something like dual_tb in case of 2dto3d */
				if(disp0_info->out_type==LX_PE_OUT_2D_3D)
				{
					y0[LX_PE_WIN_0] = ctrl0_info->m_win_v_ofst<<1;
					y1[LX_PE_WIN_0] = y0[LX_PE_WIN_0]+(ctrl0_info->m_win_v_size<<1);
				}
				else
				{
					y0[LX_PE_WIN_0] = ctrl0_info->m_win_v_ofst;
					y1[LX_PE_WIN_0] = y0[LX_PE_WIN_0]+ctrl0_info->m_win_v_size;
				}
			}
			else
			{
				y0[LX_PE_WIN_0] = 0;
				y1[LX_PE_WIN_0] = v_max;
			}
			if(ctrl1_info->m_win_h_size)
			{
				x0[LX_PE_WIN_1] = ctrl1_info->m_win_h_ofst;
				x1[LX_PE_WIN_1] = x0[LX_PE_WIN_1]+ctrl1_info->m_win_h_size;
			}
			else
			{
				x0[LX_PE_WIN_1] = 0;
				x1[LX_PE_WIN_1] = h_max;
			}
			if(ctrl1_info->m_win_v_size)
			{
				y0[LX_PE_WIN_1] = ctrl1_info->m_win_v_ofst;
				y1[LX_PE_WIN_1] = y0[LX_PE_WIN_1]+ctrl1_info->m_win_v_size;
			}
			else
			{
				y0[LX_PE_WIN_1] = 0;
				y1[LX_PE_WIN_1] = v_max;
			}
			/* [m14a0]
			* lrcr win setting for 3d : don't care for internal frc, because of ll mode on
			* here consider external frc case for 3d
			*/
			switch(disp0_info->out_type)
			{
				case LX_PE_OUT_2D:
				case LX_PE_OUT_2D_3D:
					if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 || \
						lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1 || \
						lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0 || \
						lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1)
					{
						if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 || \
							lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1)
						{
							src_v = ctrl0_info->pe0_src_v;
							in_v = ctrl0_info->i_win_v_size;
							/* [m14a0] 2x because of something like dual_tb in case of 2dto3d */
							if(disp0_info->out_type==LX_PE_OUT_2D_3D)
							{
								out_v = ctrl0_info->m_win_v_size<<1;
							}
							else
							{
								out_v = ctrl0_info->m_win_v_size;
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
							src_h = ctrl0_info->pe0_src_h;
							in_h = ctrl0_info->i_win_h_size;
							out_h = ctrl0_info->m_win_h_size;
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
						if(l_x1>PE_WIN_HW_M14_FHD_H_MAX)	l_x1=PE_WIN_HW_M14_FHD_H_MAX;
						if(l_y1>PE_WIN_HW_M14_FHD_V_MAX)	l_y1=PE_WIN_HW_M14_FHD_V_MAX;
						if(r_x1>PE_WIN_HW_M14_FHD_H_MAX)	r_x1=PE_WIN_HW_M14_FHD_H_MAX;
						if(r_y1>PE_WIN_HW_M14_FHD_V_MAX)	r_y1=PE_WIN_HW_M14_FHD_V_MAX;
						cur_histo_l_win[0] = l_x0;
						cur_histo_l_win[2] = l_x1;
						cur_histo_r_win[0] = r_x0;
						cur_histo_r_win[2] = r_x1;
						if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0|| \
							lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0)
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
						cur_histo_r_win[0] = x0[LX_PE_WIN_0];
						cur_histo_r_win[1] = y0[LX_PE_WIN_0];
						cur_histo_r_win[2] = x1[LX_PE_WIN_0];
						cur_histo_r_win[3] = y1[LX_PE_WIN_0];
						cur_apply_r_win[0] = x0[LX_PE_WIN_0];
						cur_apply_r_win[1] = y0[LX_PE_WIN_0];
						cur_apply_r_win[2] = x1[LX_PE_WIN_0];
						cur_apply_r_win[3] = y1[LX_PE_WIN_0];
					}
					break;
				case LX_PE_OUT_3D_2D:
					{
						cur_histo_l_win[0] = x0[LX_PE_WIN_0];
						cur_histo_l_win[1] = y0[LX_PE_WIN_0];
						cur_histo_l_win[2] = x1[LX_PE_WIN_0];
						cur_histo_l_win[3] = y1[LX_PE_WIN_0];
						cur_histo_r_win[0] = x0[LX_PE_WIN_0];
						cur_histo_r_win[1] = y0[LX_PE_WIN_0];
						cur_histo_r_win[2] = x1[LX_PE_WIN_0];
						cur_histo_r_win[3] = y1[LX_PE_WIN_0];
						cur_apply_r_win[0] = x0[LX_PE_WIN_0];
						cur_apply_r_win[1] = y0[LX_PE_WIN_0];
						cur_apply_r_win[2] = x1[LX_PE_WIN_0];
						cur_apply_r_win[3] = y1[LX_PE_WIN_0];
					}
					break;
				case LX_PE_OUT_TB:
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
				case LX_PE_OUT_UD:
					if(disp0_info->user_o_type==LX_PE_OUT_TB && \
						disp0_info->user_i_type==LX_PE_3D_IN_TB)
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
					else if(disp0_info->user_o_type==LX_PE_OUT_SS && \
						disp0_info->user_i_type==LX_PE_3D_IN_SS)
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
					else
					{
						cur_histo_l_win[0] = x0[LX_PE_WIN_0];
						cur_histo_l_win[1] = y0[LX_PE_WIN_0];
						cur_histo_l_win[2] = x1[LX_PE_WIN_0];
						cur_histo_l_win[3] = y1[LX_PE_WIN_0];
						cur_histo_r_win[0] = x0[LX_PE_WIN_1];
						cur_histo_r_win[1] = y0[LX_PE_WIN_1];
						cur_histo_r_win[2] = x1[LX_PE_WIN_1];
						cur_histo_r_win[3] = y1[LX_PE_WIN_1];
						cur_apply_r_win[0] = x0[LX_PE_WIN_1];
						cur_apply_r_win[1] = y0[LX_PE_WIN_1];
						cur_apply_r_win[2] = x1[LX_PE_WIN_1];
						cur_apply_r_win[3] = y1[LX_PE_WIN_1];
					}
					break;
				case LX_PE_OUT_FS:
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
						cur_histo_r_win[0] = x0[LX_PE_WIN_1];
						cur_histo_r_win[1] = y0[LX_PE_WIN_1];
						cur_histo_r_win[2] = x1[LX_PE_WIN_1];
						cur_histo_r_win[3] = y1[LX_PE_WIN_1];
						cur_apply_r_win[0] = x0[LX_PE_WIN_1];
						cur_apply_r_win[1] = y0[LX_PE_WIN_1];
						cur_apply_r_win[2] = x1[LX_PE_WIN_1];
						cur_apply_r_win[3] = y1[LX_PE_WIN_1];
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
					PE_WIN_HW_M14_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
						disp0_info->out_type,ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
						ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,\
						ctrl1_info->m_win_v_ofst,ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
					PE_WIN_HW_M14_DBG_PRINT("[set]histo_l_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
						lrcr_info->histo_l_x0,lrcr_info->histo_l_y0,lrcr_info->histo_l_x1,lrcr_info->histo_l_y1,\
						cur_histo_l_win[0],cur_histo_l_win[1],cur_histo_l_win[2],cur_histo_l_win[3]);
				}
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_02);
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_03);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, x0,		cur_histo_l_win[0]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_02, y0,		cur_histo_l_win[1]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_03, x1,		cur_histo_l_win[2]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_03, y1,		cur_histo_l_win[3]);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_02);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_03);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_02);
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_03);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, x0,		cur_histo_l_win[0]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_02, y0,		cur_histo_l_win[1]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_03, x1,		cur_histo_l_win[2]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_03, y1,		cur_histo_l_win[3]);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_02);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_03);
				}
				lrcr_info->histo_l_x0 = cur_histo_l_win[0];
				lrcr_info->histo_l_y0 = cur_histo_l_win[1];
				lrcr_info->histo_l_x1 = cur_histo_l_win[2];
				lrcr_info->histo_l_y1 = cur_histo_l_win[3];
				if((disp0_info->out_type==LX_PE_OUT_2D || \
					disp0_info->out_type==LX_PE_OUT_2D_3D) && \
					lrc_ctrl->win_mod>=PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 && \
					lrc_ctrl->win_mod<=PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1)
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
					if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0)
					{
						lrc_ctrl->tb0_t_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->tb0_t_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->tb0_t_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->tb0_t_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[0] = pxl_cnt;
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0)
					{
						lrc_ctrl->ss0_l_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->ss0_l_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->ss0_l_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->ss0_l_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[1] = pxl_cnt;
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1)
					{
						lrc_ctrl->tb1_t_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->tb1_t_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->tb1_t_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->tb1_t_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[2] = pxl_cnt;
					}
					else	//PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1
					{
						lrc_ctrl->ss1_l_pos[0] = cur_histo_l_win[0];
						lrc_ctrl->ss1_l_pos[1] = cur_histo_l_win[1];
						lrc_ctrl->ss1_l_pos[2] = cur_histo_l_win[2];
						lrc_ctrl->ss1_l_pos[3] = cur_histo_l_win[3];
						lrc_ctrl->pxl_cnt[3] = pxl_cnt;
					}
				}
			}
			if((lrcr_info->histo_r_x0!=cur_histo_r_win[0])||(lrcr_info->histo_r_y0!=cur_histo_r_win[1])|| \
				(lrcr_info->histo_r_x1!=cur_histo_r_win[2])||(lrcr_info->histo_r_y1!=cur_histo_r_win[3]))
			{
				/* too many prints */
				if(disp0_info->out_type!=LX_PE_OUT_2D && disp0_info->out_type!=LX_PE_OUT_2D_3D)
				{
					PE_WIN_HW_M14_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
						disp0_info->out_type,ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
						ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,\
						ctrl1_info->m_win_v_ofst,ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
					PE_WIN_HW_M14_DBG_PRINT("[set]histo_r_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
						lrcr_info->histo_r_x0,lrcr_info->histo_r_y0,lrcr_info->histo_r_x1,lrcr_info->histo_r_y1,\
						cur_histo_r_win[0],cur_histo_r_win[1],cur_histo_r_win[2],cur_histo_r_win[3]);
				}
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_04);
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_05);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, x0,		cur_histo_r_win[0]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_04, y0,		cur_histo_r_win[1]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_05, x1,		cur_histo_r_win[2]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_05, y1,		cur_histo_r_win[3]);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_04);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_05);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_04);
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_05);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, x0,		cur_histo_r_win[0]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_04, y0,		cur_histo_r_win[1]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_05, x1,		cur_histo_r_win[2]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_05, y1,		cur_histo_r_win[3]);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_04);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_05);
				}
				lrcr_info->histo_r_x0 = cur_histo_r_win[0];
				lrcr_info->histo_r_y0 = cur_histo_r_win[1];
				lrcr_info->histo_r_x1 = cur_histo_r_win[2];
				lrcr_info->histo_r_y1 = cur_histo_r_win[3];
				if((disp0_info->out_type==LX_PE_OUT_2D || \
					disp0_info->out_type==LX_PE_OUT_2D_3D) && \
					lrc_ctrl->win_mod>=PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0 && \
					lrc_ctrl->win_mod<=PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1)
				{
					if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB0)
					{
						lrc_ctrl->tb0_b_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->tb0_b_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->tb0_b_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->tb0_b_pos[3] = cur_histo_r_win[3];
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_SS0)
					{
						lrc_ctrl->ss0_r_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->ss0_r_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->ss0_r_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->ss0_r_pos[3] = cur_histo_r_win[3];
					}
					else if(lrc_ctrl->win_mod==PE_HST_HW_M14_LRC_HST_WIN_MOD_TB1)
					{
						lrc_ctrl->tb1_b_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->tb1_b_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->tb1_b_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->tb1_b_pos[3] = cur_histo_r_win[3];
					}
					else	//PE_HST_HW_M14_LRC_HST_WIN_MOD_SS1
					{
						lrc_ctrl->ss1_r_pos[0] = cur_histo_r_win[0];
						lrc_ctrl->ss1_r_pos[1] = cur_histo_r_win[1];
						lrc_ctrl->ss1_r_pos[2] = cur_histo_r_win[2];
						lrc_ctrl->ss1_r_pos[3] = cur_histo_r_win[3];
					}
				}
			}
			if((lrcr_info->apply_r_x0!=cur_apply_r_win[0])||(lrcr_info->apply_r_y0!=cur_apply_r_win[1])|| \
				(lrcr_info->apply_r_x1!=cur_apply_r_win[2])||(lrcr_info->apply_r_y1!=cur_apply_r_win[3]))
			{
				PE_WIN_HW_M14_DBG_PRINT("[inf]out:%d,o_win0:%d,%d,%d,%d,o_win1:%d,%d,%d,%d\n",\
					disp0_info->out_type,ctrl0_info->m_win_h_ofst,ctrl0_info->m_win_v_ofst,\
					ctrl0_info->m_win_h_size,ctrl0_info->m_win_v_size,ctrl1_info->m_win_h_ofst,\
					ctrl1_info->m_win_v_ofst,ctrl1_info->m_win_h_size,ctrl1_info->m_win_v_size);
				PE_WIN_HW_M14_DBG_PRINT("[set]apply_r_win : prev:%d,%d,%d,%d, cur:%d,%d,%d,%d\n",\
					lrcr_info->apply_r_x0,lrcr_info->apply_r_y0,lrcr_info->apply_r_x1,lrcr_info->apply_r_y1,\
					cur_apply_r_win[0],cur_apply_r_win[1],cur_apply_r_win[2],cur_apply_r_win[3]);
				if(PE_KDRV_VER_M14BX)
				{
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_06);
					PE_PE1_M14B0_RdFL(pe1_lrcr_ctrl_07);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, x0,		cur_apply_r_win[0]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_06, y0,		cur_apply_r_win[1]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_07, x1,		cur_apply_r_win[2]);
					PE_PE1_M14B0_Wr01(pe1_lrcr_ctrl_07, y1,		cur_apply_r_win[3]);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_06);
					PE_PE1_M14B0_WrFL(pe1_lrcr_ctrl_07);
				}
				else if(PE_KDRV_VER_M14AX)
				{
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_06);
					PE_P1L_M14A0_RdFL(pe1_lrcr_ctrl_07);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, x0,		cur_apply_r_win[0]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_06, y0,		cur_apply_r_win[1]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_07, x1,		cur_apply_r_win[2]);
					PE_P1L_M14A0_Wr01(pe1_lrcr_ctrl_07, y1,		cur_apply_r_win[3]);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_06);
					PE_P1L_M14A0_WrFL(pe1_lrcr_ctrl_07);
				}
				lrcr_info->apply_r_x0 = cur_apply_r_win[0];
				lrcr_info->apply_r_y0 = cur_apply_r_win[1];
				lrcr_info->apply_r_x1 = cur_apply_r_win[2];
				lrcr_info->apply_r_y1 = cur_apply_r_win[3];
			}
		}
		else
		{
			PE_WIN_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	} while (0);
	return ret;
}
/**
 * set tnr measure window
 *
 * @param   *pCfg [in] PE_TSK_M14_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_WIN_HW_M14A_SetTnrMeasureWin(PE_TSK_M14_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 h_size[2],v_size[2];
	UINT32 h_ofst[2]={0,0};
	UINT32 v_ofst[2]={0,0};
	UINT32 cur_x0[2]={0,0};
	UINT32 cur_y0[2]={0,0};
	UINT32 cur_x1[2]={0,0};
	UINT32 cur_y1[2]={0,0};
	static UINT32 count = 0;
	PE_INF_M14_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_M14_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	PE_WIN_HW_M14_TNR_CTRL_T *tnr_info=&_g_pe_win_hw_m14_info.tnr_inf;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_M14AX)
		{
			h_size[0] = ctrl0_info->pe0_src_h;
			v_size[0] = ctrl0_info->pe0_src_v;
			h_size[1] = ctrl1_info->pe0_src_h;
			v_size[1] = ctrl1_info->pe0_src_v;
			if(tnr_info->h_size0!=h_size[0] || tnr_info->v_size0!=v_size[0])
			{
				h_ofst[0]=h_size[0]>>4;	// 1/16=6.25%
				v_ofst[0]=v_size[0]>>4;	// 1/16=6.25%
				cur_x0[0]=h_ofst[0];
				cur_y0[0]=v_ofst[0];
				cur_x1[0]=h_size[0]-h_ofst[0];
				cur_y1[0]=v_size[0]-v_ofst[0];
				PE_P0L_M14A0_RdFL(p0l_tnr_ctrl_14);
				PE_P0L_M14A0_RdFL(p0l_tnr_ctrl_15);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_14, x0,	cur_x0[0]);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_14, y0,	cur_y0[0]);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_15, x1,	cur_x1[0]);
				PE_P0L_M14A0_Wr01(p0l_tnr_ctrl_15, y1,	cur_y1[0]);
				PE_P0L_M14A0_WrFL(p0l_tnr_ctrl_14);
				PE_P0L_M14A0_WrFL(p0l_tnr_ctrl_15);
				PE_WIN_HW_M14_DBG_PRINT("[L][size:%4d,%4d -> %4d,%4d][ofst:%4d,%4d][x,y:%4d,%4d,%4d,%4d]\n\n",\
					tnr_info->h_size0,tnr_info->v_size0,h_size[0],v_size[0],h_ofst[0],v_ofst[0],\
					cur_x0[0],cur_y0[0],cur_x1[0],cur_y1[0]);
				tnr_info->h_size0=h_size[0];
				tnr_info->v_size0=v_size[0];
			}
			if(tnr_info->h_size1!=h_size[1] || tnr_info->v_size1!=v_size[1])
			{
				h_ofst[1]=h_size[1]>>4;	// 1/16=6.25%
				v_ofst[1]=v_size[1]>>4;	// 1/16=6.25%
				cur_x0[1]=h_ofst[1];
				cur_y0[1]=v_ofst[1];
				cur_x1[1]=h_size[1]-h_ofst[1];
				cur_y1[1]=v_size[1]-v_ofst[1];
				PE_P0R_M14A0_RdFL(p0r_tnr_ctrl_14);
				PE_P0R_M14A0_RdFL(p0r_tnr_ctrl_15);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_14, x0,	cur_x0[1]);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_14, y0,	cur_y0[1]);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_15, x1,	cur_x1[1]);
				PE_P0R_M14A0_Wr01(p0r_tnr_ctrl_15, y1,	cur_y1[1]);
				PE_P0R_M14A0_WrFL(p0r_tnr_ctrl_14);
				PE_P0R_M14A0_WrFL(p0r_tnr_ctrl_15);
				PE_WIN_HW_M14_DBG_PRINT("[R][size:%4d,%4d -> %4d,%4d][ofst:%4d,%4d][x,y:%4d,%4d,%4d,%4d]\n\n",\
					tnr_info->h_size1,tnr_info->v_size1,h_size[1],v_size[1],h_ofst[1],v_ofst[1],\
					cur_x0[1],cur_y0[1],cur_x1[1],cur_y1[1]);
				tnr_info->h_size1=h_size[1];
				tnr_info->v_size1=v_size[1];
			}
			if(_g_win_hw_m14_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_WIN_HW_M14_DBG_PRINT("[state][L:%4d,%4d -> %4d,%4d][R:%4d,%4d -> %4d,%4d]\n\n",\
						tnr_info->h_size0,tnr_info->v_size0,h_size[0],v_size[0],\
						tnr_info->h_size1,tnr_info->v_size1,h_size[1],v_size[1]);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_WIN_HW_M14_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
int PE_WIN_HW_M14_GetCurWinSettings(PE_WIN_HW_M14_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	PE_WIN_HW_M14_SETTINGS_T *pInfo = &_g_pe_win_hw_m14_info;
	PE_WIN_HW_M14_CTRL_T *win_info = &_g_pe_win_hw_m14_info.win_inf;
	PE_WIN_HW_M14_LRCR_CTRL_T *lrcr_info = &_g_pe_win_hw_m14_info.lrcr_inf;
	PE_WIN_HW_M14_TNR_CTRL_T *tnr_info = &_g_pe_win_hw_m14_info.tnr_inf;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_M14)
		{
			memcpy(pstParams,pInfo,sizeof(PE_WIN_HW_M14_SETTINGS_T));
			PE_WIN_HW_M14_DBG_PRINT("act0:%d,%d,%d,%d, act1:%d,%d,%d,%d\n",\
				win_info->act0_x0,win_info->act0_y0,win_info->act0_x1,win_info->act0_y1,\
				win_info->act1_x0,win_info->act1_y0,win_info->act1_x1,win_info->act1_y1);
			PE_WIN_HW_M14_DBG_PRINT("win0:%d,%d,%d,%d, win1:%d,%d,%d,%d\n",\
				win_info->win0_x0,win_info->win0_y0,win_info->win0_x1,win_info->win0_y1,\
				win_info->win1_x0,win_info->win1_y0,win_info->win1_x1,win_info->win1_y1);
			PE_WIN_HW_M14_DBG_PRINT("h,v_max:%d,%d, w0,w1,w01_en:[%d,%d],[%d,%d],[%d,%d]\n",\
				win_info->h_max,win_info->v_max,win_info->win0_en[0],win_info->win0_en[1],\
				win_info->win1_en[0],win_info->win1_en[1],win_info->win01_en[0],win_info->win01_en[1]);
			PE_WIN_HW_M14_DBG_PRINT("histo_l,r, apply_r : %d,%d,%d,%d, %d,%d,%d,%d, %d,%d,%d,%d\n",\
				lrcr_info->histo_l_x0,lrcr_info->histo_l_y0,lrcr_info->histo_l_x1,lrcr_info->histo_l_y1,\
				lrcr_info->histo_r_x0,lrcr_info->histo_r_y0,lrcr_info->histo_r_x1,lrcr_info->histo_r_y1,\
				lrcr_info->apply_r_x0,lrcr_info->apply_r_y0,lrcr_info->apply_r_x1,lrcr_info->apply_r_y1);
			PE_WIN_HW_M14_DBG_PRINT("tnr win h,v size0:%d,%d, size1:%d,%d\n",\
				tnr_info->h_size0,tnr_info->v_size0,tnr_info->h_size1,tnr_info->v_size1);
		}
		else
		{
			PE_WIN_HW_M14_DBG_PRINT("nothing to do.\n");
		}
	} while (0);
	return ret;
}

