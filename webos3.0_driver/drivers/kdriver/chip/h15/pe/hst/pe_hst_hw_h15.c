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

/** @file pe_hst_hw_h15.c
 *
 *  driver for picture enhance dynamic contrast module functions. ( used only within kdriver )
 *	- histogram information
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.07.04
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
#include "pe_fwi_h15.h"
#include "pe_hst_hw_h15.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define PE_HST_HW_H15_SRC_APL_BM	0x000003ff
#define PE_HST_HW_H15_HIST_OP_RUN	0
#define PE_HST_HW_H15_HIST_OP_STOP	1

#define PE_HST_HW_H15_HIST_INVALID	0
#define PE_HST_HW_H15_HIST_VALID	1

#define PE_HST_HW_H15_HIST_SET_CNT	255

/* bin count(>(3840x2160=8294400)) when the histo is not available. */
#define PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE	9999999

#define PE_HST0		0
#define PE_HST1		1
#define PE_HST2		2

#define PE_RD_DONE_OFF	0
#define PE_RD_DONE_ON	1
#define PE_RD_EN_OFF	0
#define PE_RD_EN_ON		1

#define PE_HST_HW_H15_UHD_H_MAX		3840
#define PE_HST_HW_H15_UHD_V_MAX		2160
#define PE_HST_HW_H15_FHD_H_MAX		1920
#define PE_HST_HW_H15_FHD_V_MAX		1080

/* h15a use fw src apl,histo info */
#undef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO

/* h15a use ysv src apl only */
#define PE_HST_HW_H15A_USE_SRC_APL_YSV_ONLY

/* h15a consider overlapped horizontal region */
#define PE_HST_HW_H15A_CONSIDER_OVERLAP_H_RGN

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define PE_HST_HW_H15_ERROR	printk

#define PE_HST_HW_H15_DBG_PRINT(fmt,args...)	\
	if(_g_hst_hw_h15_trace) printk("[%x,%x][%s,%d] "fmt,PE_CHIP_VER,g_pe_kdrv_ver_mask,__F__,__L__,##args)

#define PE_HST_HW_H15_CHECK_CODE(_checker,_action,fmt,args...) \
	{if(_checker){PE_HST_HW_H15_ERROR(fmt,##args);_action;}}

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct
{
	UINT32 apl_y                              : 10;  //  9: 0
	UINT32 apl_s                              : 10;  // 19:10
	UINT32 apl_v                              : 10;  // 29:20
}
PE_HST_HW_H15_FW_APL_DATA_T;

typedef struct
{
	UINT32 rd_buf_num                         : 4 ;  //  3: 0
	UINT32 wr_buf_num                         : 4 ;  //  7: 4
}
PE_HST_HW_H15_FW_APL_STATUS_T;

typedef struct
{
	UINT32 rd_buf_num                         : 4 ;  //  3: 0
	UINT32 wr_buf_num                         : 4 ;  //  7: 4
	UINT32                                    : 16;  // 23: 8     reserved
	UINT32 sat_status                         : 8 ;  // 31:24
}
PE_HST_HW_H15_FW_HST_STATUS_T;

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
static int PE_HST_HW_H15_ReadyToSetSrcApl(PE_HST_HW_H15_SRC_APL_CTRL_T *cur_ctrl);
static int PE_HST_HW_H15_UpdateSrcApl(PE_HST_HW_H15_SRC_APL_CTRL_T *cur_ctrl);
static int PE_HST_HW_H15_CheckSrcAplInfo(PE_TSK_H15_CFG_T *pCfg);
static void PE_HST_HW_H15_SetSrcHistoInit(void);
static void PE_HST_HW_H15_SetSrcHistoWin(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl);
static void PE_HST_HW_H15_SetSrcHistoMux(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl);
static void PE_HST_HW_H15_SetSrcHistoMode(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl);
static void PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_ID wId, UINT32 *data);
static void PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_ID wId, UINT32 sel, PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl);
static void PE_HST_HW_H15_UpdateSrcHisto(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl);
static void PE_HST_HW_H15_GetSrcHistoInfo(UINT32 sel, PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl);
static void PE_HST_HW_H15_SetSrcHistoRdEnable(UINT32 rd_en);
static void PE_HST_HW_H15_SetSrcHistoRdDone(UINT32 rd_done);
static void PE_HST_HW_H15_ReleaseToSetSrcHisto(UINT32 rd_done);
static void PE_HST_HW_H15_CheckSrcHistoInfoForYhsFullWin(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl);
static int PE_HST_HW_H15_CheckSrcHistoInfo(PE_TSK_H15_CFG_T *pCfg);
static void PE_HST_HW_H15_ReadyToSetLrcHisto(void);
static void PE_HST_HW_H15_GetLrcHistoFromReg(UINT32 hst_sel, UINT32 *l_data, UINT32 *r_data);
static void PE_HST_HW_H15_UpdateLrcHisto(PE_HST_HW_H15_LRC_HST_CTRL_T *cur_ctrl);
static void PE_HST_HW_H15_ReleaseToSetLrcHisto(void);
static void PE_HST_HW_H15_UpdateLrcCtrl(PE_TSK_H15_LRC_HST_CTRL_T *ctrl);
static void PE_HST_HW_H15_GetHistDiff(UINT32 *l_data, UINT32 *r_data, UINT32 *diff);
static void PE_HST_HW_H15_GetHistDiffFromFh(UINT32 *full, UINT32 *half, UINT32 *diff);
static void PE_HST_HW_H15_UpdateLrcInfo(PE_TSK_H15_LRC_HST_CTRL_T *ctrl);
static int PE_HST_HW_H15_CheckLrcrHistoInfo(PE_TSK_H15_CFG_T *pCfg);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static UINT32 _g_hst_hw_h15_trace=0x0;	//default should be off.
static PE_HST_HW_H15_SETTINGS_T _g_pe_hst_hw_h15_info;

/*========================================================================================
	Implementation Group
========================================================================================*/
/**
 * set debug print on off
 *
 * @param   on_off [in] UINT8
 * @return  void
 * @see
 * @author
 */
void PE_HST_HW_H15_SetDbgPrint(UINT32 on_off)
{
	_g_hst_hw_h15_trace = on_off? 1:0;
	return;
}
/**
 * init histogram module
 *
 * @param   *pstParams [in] PE_CFG_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_Init(PE_CFG_CTRL_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			PE_HST_HW_H15_DBG_PRINT("init general histo info.\n");
			memset0(_g_pe_hst_hw_h15_info);
			#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
			_g_pe_hst_hw_h15_info.cfg[LX_PE_WIN_0].opr.src_apl_op = PE_HST_HW_H15_HIST_OP_STOP;
			_g_pe_hst_hw_h15_info.cfg[LX_PE_WIN_0].opr.src_hist_op = PE_HST_HW_H15_HIST_OP_STOP;
			PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_01,reg_histo_fw_en,0x1);
			#else
			PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_01,reg_histo_fw_en,0x0);
			#endif
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * debug setting
 *
 * @param   *pstParams [in] LX_PE_DBG_SETTINGS_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_SetDebugSettings(LX_PE_DBG_SETTINGS_T *pstParams)
{
	int ret = RET_OK;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_HST_HW_H15_DBG_PRINT("wid:%d,type:0x%x,[0x%x]print:0x%x,bypass:0x%x\n",\
			pstParams->win_id,pstParams->type,(0x1<<PE_ITEM_PKTMODL(HST)),\
			pstParams->print_lvl,pstParams->bypass);
		/* set print level */
		if(pstParams->type&LX_PE_DBG_LV)
		{
			_g_hst_hw_h15_trace = (pstParams->print_lvl & (0x1<<PE_ITEM_PKTMODL(HST)))? 0x1:0x0;
		}
	}while(0);
	return ret;
}
/**
 * print 32bin histogram and valid
 *
 * @param   *str [in] char
 * @param   *data [in] UINT32
 * @return  void
 * @see
 * @author
 */
void PE_HST_HW_H15_PrintSrcHisto(char *str, UINT32 *data)
{
	if(!str)	return;
	if(!data)	return;
	PE_HST_HW_H15_DBG_PRINT("%s (valid:%d)\n"\
		"[00] %7d [01] %7d [02] %7d [03] %7d [04] %7d [05] %7d [06] %7d [07] %7d\n"\
		"[08] %7d [09] %7d [10] %7d [11] %7d [12] %7d [13] %7d [14] %7d [15] %7d\n"\
		"[16] %7d [17] %7d [18] %7d [19] %7d [20] %7d [21] %7d [22] %7d [23] %7d\n"\
		"[24] %7d [25] %7d [26] %7d [27] %7d [28] %7d [29] %7d [30] %7d [31] %7d\n",\
		str,data[32],data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],\
		data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15],\
		data[16],data[17],data[18],data[19],data[20],data[21],data[22],data[23],\
		data[24],data[25],data[26],data[27],data[28],data[29],data[30],data[31]);
	return;
}

/**
 * print 32bin histogram
 *
 * @param   *str [in] char
 * @param   *data [in] UINT32
 * @return  void
 * @see
 * @author
 */
void PE_HST_HW_H15_PrintHistogram(char *str, UINT32 *data)
{
	if(!str)	return;
	if(!data)	return;
	PE_HST_HW_H15_DBG_PRINT("%s\n"\
		"[00] %7d [01] %7d [02] %7d [03] %7d [04] %7d [05] %7d [06] %7d [07] %7d\n"\
		"[08] %7d [09] %7d [10] %7d [11] %7d [12] %7d [13] %7d [14] %7d [15] %7d\n"\
		"[16] %7d [17] %7d [18] %7d [19] %7d [20] %7d [21] %7d [22] %7d [23] %7d\n"\
		"[24] %7d [25] %7d [26] %7d [27] %7d [28] %7d [29] %7d [30] %7d [31] %7d\n",\
		str,data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],\
		data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15],\
		data[16],data[17],data[18],data[19],data[20],data[21],data[22],data[23],\
		data[24],data[25],data[26],data[27],data[28],data[29],data[30],data[31]);
	return;
}
/**
 * get histo diff
 *
 * @param   *l_data [in] UINT32
 * @param   *r_data [in] UINT32
 * @param   *diff [in/out] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_GetHistDiff(UINT32 *l_data, UINT32 *r_data, UINT32 *diff)
{
	int ret = RET_OK;
	UINT32 i,lr_diff,diff_sum;
	do{
		CHECK_KNULL(l_data);
		CHECK_KNULL(r_data);
		CHECK_KNULL(diff);
		/* protection in case of histo not available */
		if(l_data[0]==PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE || \
			r_data[0]==PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE)
		{
			*diff = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
		}
		else
		{
			diff_sum = 0;
			for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
			{
				lr_diff = (l_data[i]>r_data[i])? (l_data[i]-r_data[i]):(r_data[i]-l_data[i]);
				diff_sum += lr_diff;
			}
			*diff = diff_sum;
		}
	}while(0);
	return;
}
/**
 * get histo diff from full and half images
 *
 * @param   *full [in] UINT32
 * @param   *half [in] UINT32
 * @param   *diff [in/out] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_GetHistDiffFromFh(UINT32 *full, UINT32 *half, UINT32 *diff)
{
	int ret = RET_OK;
	UINT32 i,half_2x,lr_diff,diff_sum;
	do{
		CHECK_KNULL(full);
		CHECK_KNULL(half);
		CHECK_KNULL(diff);
		/* protection in case of histo not available */
		if(full[0]==PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE || \
			half[0]==PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE)
		{
			*diff = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
		}
		else
		{
			diff_sum = 0;
			for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
			{
				half_2x = half[i]<<1;
				lr_diff = (full[i]>half_2x)? (full[i]-half_2x):(half_2x-full[i]);
				diff_sum += lr_diff;
			}
			*diff = diff_sum;
		}
	}while(0);
	return;
}
/**
 * ready to set src apl
 *
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_APL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_HW_H15_ReadyToSetSrcApl(PE_HST_HW_H15_SRC_APL_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	UINT32 cur_mux_sel=0x0,op_md;
	UINT32 h_ofst,v_ofst,cur_x0,cur_y0,cur_x1,cur_y1;
	#ifdef PE_HST_HW_H15A_CONSIDER_OVERLAP_H_RGN
	UINT32 is_overlapped, overlap_margin;
	#endif
	PE_HST_HW_H15_SRC_APL_CTRL_T *pre_ctrl = &_g_pe_hst_hw_h15_info.src_apl.ctrl;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			/* nd1 set src apl window(use winc) */
			if(pre_ctrl->h_size[LX_PE_WIN_0] != cur_ctrl->h_size[LX_PE_WIN_0] || \
				pre_ctrl->v_size[LX_PE_WIN_0] != cur_ctrl->v_size[LX_PE_WIN_0])
			{
				#ifdef PE_HST_HW_H15A_CONSIDER_OVERLAP_H_RGN
				is_overlapped = (cur_ctrl->h_size[LX_PE_WIN_0]>PE_HST_HW_H15_FHD_H_MAX)? 1:0;
				h_ofst = is_overlapped? (PE_HST_HW_H15_FHD_H_MAX>>4):\
					(cur_ctrl->h_size[LX_PE_WIN_0]>>4);	// 1/16=6.25%
				cur_x0 = h_ofst;
				cur_x1 = is_overlapped? (PE_HST_HW_H15_FHD_H_MAX-1):\
					(cur_ctrl->h_size[LX_PE_WIN_0]-h_ofst);
				#else
				h_ofst = cur_ctrl->h_size[LX_PE_WIN_0]>>4;	// 1/16=6.25%
				cur_x0 = h_ofst;
				cur_x1 = cur_ctrl->h_size[LX_PE_WIN_0]-h_ofst;
				#endif
				v_ofst = cur_ctrl->v_size[LX_PE_WIN_0]>>4;	// 1/16=6.25%
				cur_y0 = v_ofst;
				cur_y1 = cur_ctrl->v_size[LX_PE_WIN_0]-v_ofst;
				PE_ND_ND1_H15A0_QWr02(tpd_ctrl_05,winc_x0,cur_x0,\
                                                  winc_y0,cur_y0);
				PE_ND_ND1_H15A0_QWr02(tpd_ctrl_06,winc_x1,cur_x1,\
                                                  winc_y1,cur_y1);
				pre_ctrl->h_size[LX_PE_WIN_0] = cur_ctrl->h_size[LX_PE_WIN_0];
				pre_ctrl->v_size[LX_PE_WIN_0] = cur_ctrl->v_size[LX_PE_WIN_0];
			}
			/* nd2 set src apl window(use winc) */
			if(pre_ctrl->h_size[LX_PE_WIN_1] != cur_ctrl->h_size[LX_PE_WIN_1] || \
				pre_ctrl->v_size[LX_PE_WIN_1] != cur_ctrl->v_size[LX_PE_WIN_1])
			{
				#ifdef PE_HST_HW_H15A_CONSIDER_OVERLAP_H_RGN
				is_overlapped = (cur_ctrl->h_size[LX_PE_WIN_1]>PE_HST_HW_H15_FHD_H_MAX)? 1:0;
				overlap_margin = is_overlapped? \
					(cur_ctrl->h_size[LX_PE_WIN_1]-PE_HST_HW_H15_FHD_H_MAX):0;
				h_ofst = is_overlapped? (PE_HST_HW_H15_FHD_H_MAX>>4):\
					(cur_ctrl->h_size[LX_PE_WIN_1]>>4);	// 1/16=6.25%
				cur_x0 = is_overlapped? overlap_margin:h_ofst;
				cur_x1 = cur_ctrl->h_size[LX_PE_WIN_1]-h_ofst;
				#else
				h_ofst = cur_ctrl->h_size[LX_PE_WIN_1]>>4;	// 1/16=6.25%
				cur_x0 = h_ofst;
				cur_x1 = cur_ctrl->h_size[LX_PE_WIN_1]-h_ofst;
				#endif
				v_ofst = cur_ctrl->v_size[LX_PE_WIN_1]>>4;	// 1/16=6.25%
				cur_y0 = v_ofst;
				cur_y1 = cur_ctrl->v_size[LX_PE_WIN_1]-v_ofst;
				PE_ND_ND2_H15A0_QWr02(tpd_ctrl_05,winc_x0,cur_x0,\
                                                  winc_y0,cur_y0);
				PE_ND_ND2_H15A0_QWr02(tpd_ctrl_06,winc_x1,cur_x1,\
                                                  winc_y1,cur_y1);
				pre_ctrl->h_size[LX_PE_WIN_1] = cur_ctrl->h_size[LX_PE_WIN_1];
				pre_ctrl->v_size[LX_PE_WIN_1] = cur_ctrl->v_size[LX_PE_WIN_1];
			}
			/* set src apl mux selection(pe0_s ycbcr only */
			switch(cur_ctrl->apl_sel)
			{
				case PE_HST_HW_H15_SRC_APL_SEL_YUV0:
					cur_mux_sel = 0x0;	//YCbCr
					break;
				case PE_HST_HW_H15_SRC_APL_SEL_GBR0:
					cur_mux_sel = 0x1;	//GBR
					break;
				case PE_HST_HW_H15_SRC_APL_SEL_HSV0:
					cur_mux_sel = 0x2;	//HSV
					break;
				case PE_HST_HW_H15_SRC_APL_SEL_YSV0:
					cur_mux_sel = 0x3;	//YSV
					break;
				case PE_HST_HW_H15_SRC_APL_SEL_INIT:
				case PE_HST_HW_H15_SRC_APL_SEL_BBD0:
				default:
					#ifdef PE_HST_HW_H15A_USE_SRC_APL_YSV_ONLY
					cur_mux_sel = 0x3;	//YSV
					#else
					cur_mux_sel = 0x0;	//YCbCr
					#endif
					op_md = 0x3;	// sel winC
					PE_ND12_H15A0_QWr01(bbd_ctrl_00,reg_apl_win_sel,op_md);
					PE_ND12_H15A0_QWr02(tpd_ctrl_05,reg_winc_hen,0x1,\
                                                    reg_winc_ven,0x1); //0:full, 1:manual
					break;
			}
			if(pre_ctrl->mux_sel != cur_mux_sel)
			{
				PE_ND12_H15A0_QWr01(bbd_ctrl_00,reg_apl_mux,cur_mux_sel);
				pre_ctrl->mux_sel = cur_mux_sel;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * update src apl
 *
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_APL_CTRL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_HW_H15_UpdateSrcApl(PE_HST_HW_H15_SRC_APL_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	UINT32 r_data[LX_PE_WIN_NUM][2];
	PE_HST_HW_H15_SRC_APL_DATA_T *apl_data = &_g_pe_hst_hw_h15_info.src_apl.data;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			PE_ND_ND1_H15A0_RdFL(apl_stat_00);
			r_data[LX_PE_WIN_0][0] = PE_ND_ND1_H15A0_Rd(apl_stat_00);
			PE_ND_ND2_H15A0_RdFL(apl_stat_00);
			r_data[LX_PE_WIN_1][0] = PE_ND_ND2_H15A0_Rd(apl_stat_00);
			switch(cur_ctrl->apl_sel)
			{
				case PE_HST_HW_H15_SRC_APL_SEL_GBR0:
					apl_data->gbr[LX_PE_WIN_0] = r_data[LX_PE_WIN_0][0];
					apl_data->gbr[LX_PE_WIN_1] = r_data[LX_PE_WIN_1][0];
					break;
				case PE_HST_HW_H15_SRC_APL_SEL_HSV0:
				case PE_HST_HW_H15_SRC_APL_SEL_YSV0:
					if (cur_ctrl->apl_sel == PE_HST_HW_H15_SRC_APL_SEL_HSV0)
					{
						apl_data->hsv[LX_PE_WIN_0] = r_data[LX_PE_WIN_0][0];
						apl_data->hsv[LX_PE_WIN_1] = r_data[LX_PE_WIN_1][0];
					}
					else
					{
						apl_data->ysv[LX_PE_WIN_0] = r_data[LX_PE_WIN_0][0];
						apl_data->ysv[LX_PE_WIN_1] = r_data[LX_PE_WIN_1][0];
					}
					PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_00,reg_s_apl_0,GET_BITS(r_data[LX_PE_WIN_0][0],12,8));
					break;
				case PE_HST_HW_H15_SRC_APL_SEL_YUV0:
					apl_data->yuv[LX_PE_WIN_0] = r_data[LX_PE_WIN_0][0];
					apl_data->yuv[LX_PE_WIN_1] = r_data[LX_PE_WIN_1][0];
					break;
				case PE_HST_HW_H15_SRC_APL_SEL_INIT:
				default:
					#ifdef PE_HST_HW_H15A_USE_SRC_APL_YSV_ONLY
					apl_data->ysv[LX_PE_WIN_0] = r_data[LX_PE_WIN_0][0];
					apl_data->ysv[LX_PE_WIN_1] = r_data[LX_PE_WIN_1][0];
					#else
					apl_data->yuv[LX_PE_WIN_0] = r_data[LX_PE_WIN_0][0];
					apl_data->yuv[LX_PE_WIN_1] = r_data[LX_PE_WIN_1][0];
					#endif
					break;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}

/**
 * check src apl info
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_HW_H15_CheckSrcAplInfo(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 under_30hz = 0;
	PE_HST_HW_H15_SRC_APL_CTRL_T cur_ctrl;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_HST_HW_H15_STATE_T *apl_state = &_g_pe_hst_hw_h15_info.src_apl.state;
	LX_PE_HST_HISTO_OPR_T *hist_opr = &_g_pe_hst_hw_h15_info.cfg[LX_PE_WIN_0].opr;
	PE_HST_HW_H15_SRC_APL_CTRL_T *pre_ctrl = &_g_pe_hst_hw_h15_info.src_apl.ctrl;
	do {
		CHECK_KNULL(pCfg);
		if(((ctrl0_info->pe0_src_h==0||ctrl0_info->pe0_src_v==0) && \
			(ctrl1_info->pe0_src_h==0||ctrl1_info->pe0_src_v==0)) || \
			hist_opr->src_apl_op==PE_HST_HW_H15_HIST_OP_STOP)
		{
			apl_state->next = PE_HST_HW_H15_SRC_APL_STM_ALL_INIT;
			break;
		}
		/* update the cur_ctrl because it is a local variable */
		cur_ctrl.h_size[LX_PE_WIN_0] = ctrl0_info->pe0_src_h;
		cur_ctrl.v_size[LX_PE_WIN_0] = ctrl0_info->pe0_src_v;
		cur_ctrl.h_size[LX_PE_WIN_1] = ctrl1_info->pe0_src_h;
		cur_ctrl.v_size[LX_PE_WIN_1] = ctrl1_info->pe0_src_v;
		cur_ctrl.apl_sel = pre_ctrl->apl_sel;
		apl_state->curr = apl_state->next;
		under_30hz = (disp0_info->in_f_rate < 305)? 1:0;
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_HST_HW_H15A_USE_SRC_APL_YSV_ONLY
			switch(apl_state->curr)
			{
				case PE_HST_HW_H15_SRC_APL_STM_ALL_INIT:
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_INIT;
					ret = PE_HST_HW_H15_ReadyToSetSrcApl(&cur_ctrl);
					if(ret)	break;
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_YSV0;
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_YSV_RDY0:\
						PE_HST_HW_H15_SRC_APL_STM_YSV_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YSV_RDY0:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_YSV_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YSV_RDY1:
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_YSV_RDY2:\
						PE_HST_HW_H15_SRC_APL_STM_YSV_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YSV_RDY2:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_YSV_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YSV_GET0:
					ret = PE_HST_HW_H15_UpdateSrcApl(&cur_ctrl);
					if(ret)	break;
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_YSV0;
					ret = PE_HST_HW_H15_ReadyToSetSrcApl(&cur_ctrl);
					if(ret)	break;
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_YSV_RDY0:\
						PE_HST_HW_H15_SRC_APL_STM_YSV_RDY1;
					break;
				default:
					PE_HST_HW_H15_DBG_PRINT("apl_state->curr:%d, wrong type.\n",apl_state->curr);
					break;
			}
			#else
			switch(apl_state->curr)
			{
				case PE_HST_HW_H15_SRC_APL_STM_ALL_INIT:
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_INIT;
					ret = PE_HST_HW_H15_ReadyToSetSrcApl(&cur_ctrl);
					if(ret)	break;
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_YUV0;
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_YUV_RDY0:\
						PE_HST_HW_H15_SRC_APL_STM_YUV_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YUV_RDY0:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_YUV_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YUV_RDY1:
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_YUV_RDY2:\
						PE_HST_HW_H15_SRC_APL_STM_YUV_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YUV_RDY2:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_YUV_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_YUV_GET0:
					ret = PE_HST_HW_H15_UpdateSrcApl(&cur_ctrl);
					if(ret)	break;
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_HSV0;
					ret = PE_HST_HW_H15_ReadyToSetSrcApl(&cur_ctrl);
					if(ret)	break;
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_HSV_RDY0:\
						PE_HST_HW_H15_SRC_APL_STM_HSV_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_HSV_RDY0:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_HSV_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_HSV_RDY1:
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_HSV_RDY2:\
						PE_HST_HW_H15_SRC_APL_STM_HSV_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_HSV_RDY2:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_HSV_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_HSV_GET0:
					ret = PE_HST_HW_H15_UpdateSrcApl(&cur_ctrl);
					if(ret)	break;
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_GBR0;
					ret = PE_HST_HW_H15_ReadyToSetSrcApl(&cur_ctrl);
					if(ret)	break;
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_GBR_RDY0:\
						PE_HST_HW_H15_SRC_APL_STM_GBR_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_GBR_RDY0:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_GBR_RDY1;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_GBR_RDY1:
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_GBR_RDY2:\
						PE_HST_HW_H15_SRC_APL_STM_GBR_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_GBR_RDY2:
					apl_state->next = PE_HST_HW_H15_SRC_APL_STM_GBR_GET0;
					break;
				case PE_HST_HW_H15_SRC_APL_STM_GBR_GET0:
					ret = PE_HST_HW_H15_UpdateSrcApl(&cur_ctrl);
					if(ret)	break;
					cur_ctrl.apl_sel = PE_HST_HW_H15_SRC_APL_SEL_YUV0;
					ret = PE_HST_HW_H15_ReadyToSetSrcApl(&cur_ctrl);
					if(ret)	break;
					apl_state->next = (pCfg->hdlr_opr.is_registered && under_30hz)? \
						PE_HST_HW_H15_SRC_APL_STM_YUV_RDY0:\
						PE_HST_HW_H15_SRC_APL_STM_YUV_RDY1;
					break;
				default:
					PE_HST_HW_H15_DBG_PRINT("apl_state->curr:%d, wrong type.\n",apl_state->curr);
					break;
			}
			#endif
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
		pre_ctrl->apl_sel = cur_ctrl.apl_sel;
	}while(0);
	return ret;
}
/**
 * set src histo init
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetSrcHistoInit(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		/* nd1,2 hist init */
		PE_ND12_H15A0_QWr02(tpd_ctrl_01,reg_wina_hen,0x1, \
                                        reg_wina_ven,0x1);	//0:full, 1:manual
		PE_ND12_H15A0_QWr02(tpd_ctrl_02,reg_hist_hsv_hsl_sel,0x1, \
                                        reg_hist_vscaling_en,0x1);	//0:HSL,1:HSV, 0:HSV,1:sHSV
		PE_ND12_H15A0_QWr02(tpd_ctrl_03,reg_winb_hen,0x1, \
                                        reg_winb_ven,0x1);	//0:full, 1:manual
		PE_ND12_H15A0_QWr03(tpd_hist_ai_ctrl,hif_hist_address,0x0, \
                                             hif_hist_ai,     0x0, \
                                             hif_hist_enable, 0x1);	//0:host,1:normal
	}
	else
	{
		PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	}
	return;
}
/**
 * set src histo window
 *
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetSrcHistoWin(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	UINT32 x0[LX_PE_WIN_NUM][2],y0[LX_PE_WIN_NUM][2];
	UINT32 x1[LX_PE_WIN_NUM][2],y1[LX_PE_WIN_NUM][2];
	UINT32 sel[LX_PE_WIN_NUM][3];
	#ifdef PE_HST_HW_H15A_CONSIDER_OVERLAP_H_RGN
	UINT32 is_overlapped, overlap_margin;
	#endif
	PE_HST_HW_H15_SRC_HST_CTRL_T *pre_ctrl = &_g_pe_hst_hw_h15_info.src_hist.ctrl;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			switch(cur_ctrl->mod_sel)
			{
				case PE_HST_HW_H15_SRC_HST_MOD_SEL_TB:
					/* nd1 win_a(full) */
					x0[LX_PE_WIN_0][0] = cur_ctrl->x_ofst[LX_PE_WIN_0];
					y0[LX_PE_WIN_0][0] = cur_ctrl->y_ofst[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][0] = x0[LX_PE_WIN_0][0]+cur_ctrl->h_size[LX_PE_WIN_0];
					y1[LX_PE_WIN_0][0] = y0[LX_PE_WIN_0][0]+cur_ctrl->v_size[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][0] = (x1[LX_PE_WIN_0][0]>0)? (x1[LX_PE_WIN_0][0]-1):0;
					y1[LX_PE_WIN_0][0] = (y1[LX_PE_WIN_0][0]>0)? (y1[LX_PE_WIN_0][0]-1):0;
					/* nd1 win_b(half) */
					x0[LX_PE_WIN_0][1] = cur_ctrl->x_ofst[LX_PE_WIN_0];
					y0[LX_PE_WIN_0][1] = cur_ctrl->y_ofst[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][1] = x0[LX_PE_WIN_0][1]+cur_ctrl->h_size[LX_PE_WIN_0];
					y1[LX_PE_WIN_0][1] = y0[LX_PE_WIN_0][1]+(cur_ctrl->v_size[LX_PE_WIN_0]>>1);
					x1[LX_PE_WIN_0][1] = (x1[LX_PE_WIN_0][1]>0)? (x1[LX_PE_WIN_0][1]-1):0;
					y1[LX_PE_WIN_0][1] = (y1[LX_PE_WIN_0][1]>0)? (y1[LX_PE_WIN_0][1]-1):0;
					/* nd2 win_a */
					x0[LX_PE_WIN_1][0] = y0[LX_PE_WIN_1][0] = 0;
					x1[LX_PE_WIN_1][0] = y1[LX_PE_WIN_1][0] = 0;
					/* nd2 win_b */
					x0[LX_PE_WIN_1][1] = y0[LX_PE_WIN_1][1] = 0;
					x1[LX_PE_WIN_1][1] = y1[LX_PE_WIN_1][1] = 0;
					/* nd1 win sel */
					sel[LX_PE_WIN_0][0] = 1;	//win b(half)
					sel[LX_PE_WIN_0][1] = 0;	//win a(full), a only(hw fix)
					sel[LX_PE_WIN_0][2] = 0;	//win a(full), a only(hw fix)
					/* nd2 win sel */
					sel[LX_PE_WIN_1][0] = 0;	//not use, dont care
					sel[LX_PE_WIN_1][1] = 0;	//not use, dont care, a only(hw fix)
					sel[LX_PE_WIN_1][2] = 0;	//not use, dont care, a only(hw fix)
					break;
				case PE_HST_HW_H15_SRC_HST_MOD_SEL_SS:
					/* nd1 win_a(full) */
					x0[LX_PE_WIN_0][0] = cur_ctrl->x_ofst[LX_PE_WIN_0];
					y0[LX_PE_WIN_0][0] = cur_ctrl->y_ofst[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][0] = x0[LX_PE_WIN_0][0]+cur_ctrl->h_size[LX_PE_WIN_0];
					y1[LX_PE_WIN_0][0] = y0[LX_PE_WIN_0][0]+cur_ctrl->v_size[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][0] = (x1[LX_PE_WIN_0][0]>0)? (x1[LX_PE_WIN_0][0]-1):0;
					y1[LX_PE_WIN_0][0] = (y1[LX_PE_WIN_0][0]>0)? (y1[LX_PE_WIN_0][0]-1):0;
					/* nd1 win_b(half) */
					x0[LX_PE_WIN_0][1] = cur_ctrl->x_ofst[LX_PE_WIN_0];
					y0[LX_PE_WIN_0][1] = cur_ctrl->y_ofst[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][1] = x0[LX_PE_WIN_0][1]+(cur_ctrl->h_size[LX_PE_WIN_0]>>1);
					y1[LX_PE_WIN_0][1] = y0[LX_PE_WIN_0][1]+cur_ctrl->v_size[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][1] = (x1[LX_PE_WIN_0][1]>0)? (x1[LX_PE_WIN_0][1]-1):0;
					y1[LX_PE_WIN_0][1] = (y1[LX_PE_WIN_0][1]>0)? (y1[LX_PE_WIN_0][1]-1):0;
					/* nd2 win_a */
					x0[LX_PE_WIN_1][0] = y0[LX_PE_WIN_1][0] = 0;
					x1[LX_PE_WIN_1][0] = y1[LX_PE_WIN_1][0] = 0;
					/* nd2 win_b */
					x0[LX_PE_WIN_1][1] = y0[LX_PE_WIN_1][1] = 0;
					x1[LX_PE_WIN_1][1] = y1[LX_PE_WIN_1][1] = 0;
					/* nd1 win sel */
					sel[LX_PE_WIN_0][0] = 1;	//win b(half)
					sel[LX_PE_WIN_0][1] = 0;	//win a(full), a only(hw fix)
					sel[LX_PE_WIN_0][2] = 0;	//win a(full), a only(hw fix)
					/* nd2 win sel */
					sel[LX_PE_WIN_1][0] = 0;	//not use, dont care
					sel[LX_PE_WIN_1][1] = 0;	//not use, dont care, a only(hw fix)
					sel[LX_PE_WIN_1][2] = 0;	//not use, dont care, a only(hw fix)
					break;
				case PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL:
				default:
					/* nd1 win_a(full) */
					#ifdef PE_HST_HW_H15A_CONSIDER_OVERLAP_H_RGN
					is_overlapped = (cur_ctrl->h_size[LX_PE_WIN_0]>PE_HST_HW_H15_FHD_H_MAX)? 1:0;
					x0[LX_PE_WIN_0][0] = is_overlapped? 0:cur_ctrl->x_ofst[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][0] = is_overlapped? \
						PE_HST_HW_H15_FHD_H_MAX:(x0[LX_PE_WIN_0][0]+cur_ctrl->h_size[LX_PE_WIN_0]);
					#else
					x0[LX_PE_WIN_0][0] = cur_ctrl->x_ofst[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][0] = x0[LX_PE_WIN_0][0]+cur_ctrl->h_size[LX_PE_WIN_0];
					#endif
					y0[LX_PE_WIN_0][0] = cur_ctrl->y_ofst[LX_PE_WIN_0];
					y1[LX_PE_WIN_0][0] = y0[LX_PE_WIN_0][0]+cur_ctrl->v_size[LX_PE_WIN_0];
					x1[LX_PE_WIN_0][0] = (x1[LX_PE_WIN_0][0]>0)? (x1[LX_PE_WIN_0][0]-1):0;
					y1[LX_PE_WIN_0][0] = (y1[LX_PE_WIN_0][0]>0)? (y1[LX_PE_WIN_0][0]-1):0;
					/* nd1 win_b */
					x0[LX_PE_WIN_0][1] = y0[LX_PE_WIN_0][1] = 0;
					x1[LX_PE_WIN_0][1] = y1[LX_PE_WIN_0][1] = 0;
					/* nd2 win_a(full) */
					#ifdef PE_HST_HW_H15A_CONSIDER_OVERLAP_H_RGN
					is_overlapped = (cur_ctrl->h_size[LX_PE_WIN_1]>PE_HST_HW_H15_FHD_H_MAX)? 1:0;
					overlap_margin = is_overlapped? \
						(cur_ctrl->h_size[LX_PE_WIN_1]-PE_HST_HW_H15_FHD_H_MAX):0;
					x0[LX_PE_WIN_1][0] = is_overlapped? overlap_margin:cur_ctrl->x_ofst[LX_PE_WIN_1];
					x1[LX_PE_WIN_1][0] = is_overlapped? \
						(cur_ctrl->h_size[LX_PE_WIN_1]):(x0[LX_PE_WIN_1][0]+cur_ctrl->h_size[LX_PE_WIN_1]);
					#else
					x0[LX_PE_WIN_1][0] = cur_ctrl->x_ofst[LX_PE_WIN_1];
					x1[LX_PE_WIN_1][0] = x0[LX_PE_WIN_1][0]+cur_ctrl->h_size[LX_PE_WIN_1];
					#endif
					y0[LX_PE_WIN_1][0] = cur_ctrl->y_ofst[LX_PE_WIN_1];
					y1[LX_PE_WIN_1][0] = y0[LX_PE_WIN_1][0]+cur_ctrl->v_size[LX_PE_WIN_1];
					x1[LX_PE_WIN_1][0] = (x1[LX_PE_WIN_1][0]>0)? (x1[LX_PE_WIN_1][0]-1):0;
					y1[LX_PE_WIN_1][0] = (y1[LX_PE_WIN_1][0]>0)? (y1[LX_PE_WIN_1][0]-1):0;
					/* nd2 win_b */
					x0[LX_PE_WIN_1][1] = y0[LX_PE_WIN_1][1] = 0;
					x1[LX_PE_WIN_1][1] = y1[LX_PE_WIN_1][1] = 0;
					/* nd1 win sel */
					sel[LX_PE_WIN_0][0] = 0;	//win a(full)
					sel[LX_PE_WIN_0][1] = 0;	//win a(full), a only(hw fix)
					sel[LX_PE_WIN_0][2] = 0;	//win a(full), a only(hw fix)
					/* nd2 win sel */
					sel[LX_PE_WIN_1][0] = 0;	//win a(full)
					sel[LX_PE_WIN_1][1] = 0;	//win a(full), a only(hw fix)
					sel[LX_PE_WIN_1][2] = 0;	//win a(full), a only(hw fix)
					break;
			}
			/* set src histo window : nd1 win_a */
			if(pre_ctrl->x0_pos[LX_PE_WIN_0][0] != x0[LX_PE_WIN_0][0] || \
				pre_ctrl->y0_pos[LX_PE_WIN_0][0] != y0[LX_PE_WIN_0][0] || \
				pre_ctrl->x1_pos[LX_PE_WIN_0][0] != x1[LX_PE_WIN_0][0] || \
				pre_ctrl->y1_pos[LX_PE_WIN_0][0] != y1[LX_PE_WIN_0][0])
			{
				PE_HST_HW_H15_DBG_PRINT("[W0A]inf:%d,%d,%d,%d, x,y:%d,%d,%d,%d to %d,%d,%d,%d\n",\
					cur_ctrl->x_ofst[LX_PE_WIN_0],cur_ctrl->y_ofst[LX_PE_WIN_0],\
					cur_ctrl->h_size[LX_PE_WIN_0],cur_ctrl->v_size[LX_PE_WIN_0],\
					pre_ctrl->x0_pos[LX_PE_WIN_0][0],pre_ctrl->y0_pos[LX_PE_WIN_0][0],\
					pre_ctrl->x1_pos[LX_PE_WIN_0][0],pre_ctrl->y1_pos[LX_PE_WIN_0][0],\
					x0[LX_PE_WIN_0][0],y0[LX_PE_WIN_0][0],x1[LX_PE_WIN_0][0],y1[LX_PE_WIN_0][0]);
				PE_ND_ND1_H15A0_QWr02(tpd_ctrl_01,wina_x0,x0[LX_PE_WIN_0][0], \
                                                  wina_y0,y0[LX_PE_WIN_0][0]);
				PE_ND_ND1_H15A0_QWr02(tpd_ctrl_02,wina_x1,x1[LX_PE_WIN_0][0], \
                                                  wina_y1,y1[LX_PE_WIN_0][0]);
				pre_ctrl->x0_pos[LX_PE_WIN_0][0] = x0[LX_PE_WIN_0][0];
				pre_ctrl->y0_pos[LX_PE_WIN_0][0] = y0[LX_PE_WIN_0][0];
				pre_ctrl->x1_pos[LX_PE_WIN_0][0] = x1[LX_PE_WIN_0][0];
				pre_ctrl->y1_pos[LX_PE_WIN_0][0] = y1[LX_PE_WIN_0][0];
			}
			/* set src histo window : nd1 win_b */
			if(pre_ctrl->x0_pos[LX_PE_WIN_0][1] != x0[LX_PE_WIN_0][1] || \
				pre_ctrl->y0_pos[LX_PE_WIN_0][1] != y0[LX_PE_WIN_0][1] || \
				pre_ctrl->x1_pos[LX_PE_WIN_0][1] != x1[LX_PE_WIN_0][1] || \
				pre_ctrl->y1_pos[LX_PE_WIN_0][1] != y1[LX_PE_WIN_0][1])
			{
				PE_HST_HW_H15_DBG_PRINT("[W0B]inf:%d,%d,%d,%d, x,y:%d,%d,%d,%d to %d,%d,%d,%d\n",\
					cur_ctrl->x_ofst[LX_PE_WIN_0],cur_ctrl->y_ofst[LX_PE_WIN_0],\
					cur_ctrl->h_size[LX_PE_WIN_0],cur_ctrl->v_size[LX_PE_WIN_0],\
					pre_ctrl->x0_pos[LX_PE_WIN_0][1],pre_ctrl->y0_pos[LX_PE_WIN_0][1],\
					pre_ctrl->x1_pos[LX_PE_WIN_0][1],pre_ctrl->y1_pos[LX_PE_WIN_0][1],\
					x0[LX_PE_WIN_0][1],y0[LX_PE_WIN_0][1],x1[LX_PE_WIN_0][1],y1[LX_PE_WIN_0][1]);
				PE_ND_ND1_H15A0_QWr02(tpd_ctrl_03,winb_x0,x0[LX_PE_WIN_0][1], \
                                                  winb_y0,y0[LX_PE_WIN_0][1]);
				PE_ND_ND1_H15A0_QWr02(tpd_ctrl_04,winb_x1,x1[LX_PE_WIN_0][1], \
                                                  winb_y1,y1[LX_PE_WIN_0][1]);
				pre_ctrl->x0_pos[LX_PE_WIN_0][1] = x0[LX_PE_WIN_0][1];
				pre_ctrl->y0_pos[LX_PE_WIN_0][1] = y0[LX_PE_WIN_0][1];
				pre_ctrl->x1_pos[LX_PE_WIN_0][1] = x1[LX_PE_WIN_0][1];
				pre_ctrl->y1_pos[LX_PE_WIN_0][1] = y1[LX_PE_WIN_0][1];
			}
			/* set src histo window : nd2 win_a */
			if(pre_ctrl->x0_pos[LX_PE_WIN_1][0] != x0[LX_PE_WIN_1][0] || \
				pre_ctrl->y0_pos[LX_PE_WIN_1][0] != y0[LX_PE_WIN_1][0] || \
				pre_ctrl->x1_pos[LX_PE_WIN_1][0] != x1[LX_PE_WIN_1][0] || \
				pre_ctrl->y1_pos[LX_PE_WIN_1][0] != y1[LX_PE_WIN_1][0])
			{
				PE_HST_HW_H15_DBG_PRINT("[W1A]inf:%d,%d,%d,%d, x,y:%d,%d,%d,%d to %d,%d,%d,%d\n",\
					cur_ctrl->x_ofst[LX_PE_WIN_1],cur_ctrl->y_ofst[LX_PE_WIN_1],\
					cur_ctrl->h_size[LX_PE_WIN_1],cur_ctrl->v_size[LX_PE_WIN_1],\
					pre_ctrl->x0_pos[LX_PE_WIN_1][0],pre_ctrl->y0_pos[LX_PE_WIN_1][0],\
					pre_ctrl->x1_pos[LX_PE_WIN_1][0],pre_ctrl->y1_pos[LX_PE_WIN_1][0],\
					x0[LX_PE_WIN_1][0],y0[LX_PE_WIN_1][0],x1[LX_PE_WIN_1][0],y1[LX_PE_WIN_1][0]);
				PE_ND_ND2_H15A0_QWr02(tpd_ctrl_01,wina_x0,x0[LX_PE_WIN_1][0], \
                                                  wina_y0,y0[LX_PE_WIN_1][0]);
				PE_ND_ND2_H15A0_QWr02(tpd_ctrl_02,wina_x1,x1[LX_PE_WIN_1][0], \
                                                  wina_y1,y1[LX_PE_WIN_1][0]);
				pre_ctrl->x0_pos[LX_PE_WIN_1][0] = x0[LX_PE_WIN_1][0];
				pre_ctrl->y0_pos[LX_PE_WIN_1][0] = y0[LX_PE_WIN_1][0];
				pre_ctrl->x1_pos[LX_PE_WIN_1][0] = x1[LX_PE_WIN_1][0];
				pre_ctrl->y1_pos[LX_PE_WIN_1][0] = y1[LX_PE_WIN_1][0];
			}
			/* set src histo window : nd2 win_b */
			if(pre_ctrl->x0_pos[LX_PE_WIN_1][1] != x0[LX_PE_WIN_1][1] || \
				pre_ctrl->y0_pos[LX_PE_WIN_1][1] != y0[LX_PE_WIN_1][1] || \
				pre_ctrl->x1_pos[LX_PE_WIN_1][1] != x1[LX_PE_WIN_1][1] || \
				pre_ctrl->y1_pos[LX_PE_WIN_1][1] != y1[LX_PE_WIN_1][1])
			{
				PE_HST_HW_H15_DBG_PRINT("[W1B]inf:%d,%d,%d,%d, x,y:%d,%d,%d,%d to %d,%d,%d,%d\n",\
					cur_ctrl->x_ofst[LX_PE_WIN_1],cur_ctrl->y_ofst[LX_PE_WIN_1],\
					cur_ctrl->h_size[LX_PE_WIN_1],cur_ctrl->v_size[LX_PE_WIN_1],\
					pre_ctrl->x0_pos[LX_PE_WIN_1][1],pre_ctrl->y0_pos[LX_PE_WIN_1][1],\
					pre_ctrl->x1_pos[LX_PE_WIN_1][1],pre_ctrl->y1_pos[LX_PE_WIN_1][1],\
					x0[LX_PE_WIN_1][1],y0[LX_PE_WIN_1][1],x1[LX_PE_WIN_1][1],y1[LX_PE_WIN_1][1]);
				PE_ND_ND2_H15A0_QWr02(tpd_ctrl_03,winb_x0,x0[LX_PE_WIN_1][1], \
                                                  winb_y0,y0[LX_PE_WIN_1][1]);
				PE_ND_ND2_H15A0_QWr02(tpd_ctrl_04,winb_x1,x1[LX_PE_WIN_1][1], \
                                                  winb_y1,y1[LX_PE_WIN_1][1]);
				pre_ctrl->x0_pos[LX_PE_WIN_1][1] = x0[LX_PE_WIN_1][1];
				pre_ctrl->y0_pos[LX_PE_WIN_1][1] = y0[LX_PE_WIN_1][1];
				pre_ctrl->x1_pos[LX_PE_WIN_1][1] = x1[LX_PE_WIN_1][1];
				pre_ctrl->y1_pos[LX_PE_WIN_1][1] = y1[LX_PE_WIN_1][1];
			}
			/* set win sel : nd1 */
			if(pre_ctrl->win_sel[LX_PE_WIN_0][0] != sel[LX_PE_WIN_0][0] || \
				pre_ctrl->win_sel[LX_PE_WIN_0][1] != sel[LX_PE_WIN_0][1] || \
				pre_ctrl->win_sel[LX_PE_WIN_0][2] != sel[LX_PE_WIN_0][2])
			{
				PE_HST_HW_H15_DBG_PRINT("[W0]win sel:%d,%d,%d to %d,%d,%d\n",\
					pre_ctrl->win_sel[LX_PE_WIN_0][0],pre_ctrl->win_sel[LX_PE_WIN_0][1],\
					pre_ctrl->win_sel[LX_PE_WIN_0][2],sel[LX_PE_WIN_0][0],\
					sel[LX_PE_WIN_0][1],sel[LX_PE_WIN_0][2]);
				PE_ND_ND1_H15A0_QWr03(tpd_ctrl_00,reg_hist0_win_sel,sel[LX_PE_WIN_0][0], \
                                                  reg_hist1_win_sel,sel[LX_PE_WIN_0][1], \
                                                  reg_hist2_win_sel,sel[LX_PE_WIN_0][2]);
				pre_ctrl->win_sel[LX_PE_WIN_0][0] = sel[LX_PE_WIN_0][0];
				pre_ctrl->win_sel[LX_PE_WIN_0][1] = sel[LX_PE_WIN_0][1];
				pre_ctrl->win_sel[LX_PE_WIN_0][2] = sel[LX_PE_WIN_0][2];
			}
			/* set win sel : nd2 */
			if(pre_ctrl->win_sel[LX_PE_WIN_1][0] != sel[LX_PE_WIN_1][0] || \
				pre_ctrl->win_sel[LX_PE_WIN_1][1] != sel[LX_PE_WIN_1][1] || \
				pre_ctrl->win_sel[LX_PE_WIN_1][2] != sel[LX_PE_WIN_1][2])
			{
				PE_HST_HW_H15_DBG_PRINT("[W1]win sel:%d,%d,%d to %d,%d,%d\n",\
					pre_ctrl->win_sel[LX_PE_WIN_1][0],pre_ctrl->win_sel[LX_PE_WIN_1][1],\
					pre_ctrl->win_sel[LX_PE_WIN_1][2],sel[LX_PE_WIN_1][0],\
					sel[LX_PE_WIN_1][1],sel[LX_PE_WIN_1][2]);
				PE_ND_ND2_H15A0_QWr03(tpd_ctrl_00,reg_hist0_win_sel,sel[LX_PE_WIN_1][0], \
                                                  reg_hist1_win_sel,sel[LX_PE_WIN_1][1], \
                                                  reg_hist2_win_sel,sel[LX_PE_WIN_1][2]);
				pre_ctrl->win_sel[LX_PE_WIN_1][0] = sel[LX_PE_WIN_1][0];
				pre_ctrl->win_sel[LX_PE_WIN_1][1] = sel[LX_PE_WIN_1][1];
				pre_ctrl->win_sel[LX_PE_WIN_1][2] = sel[LX_PE_WIN_1][2];
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * set src histo mux
 *
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetSrcHistoMux(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	PE_HST_HW_H15_SRC_HST_CTRL_T *pre_ctrl = &_g_pe_hst_hw_h15_info.src_hist.ctrl;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			if((pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0] != cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0]) || \
				(pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1] != cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1]) || \
				(pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2] != cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2]))
			{
				PE_ND_ND1_H15A0_QWr03(tpd_ctrl_00,reg_hist0_data_mux,cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0], \
                                                  reg_hist1_data_mux,cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1], \
                                                  reg_hist2_data_mux,cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2]);
				pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0] = cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0];
				pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1] = cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1];
				pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2] = cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2];
			}
			if((pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0] != cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0]) || \
				(pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1] != cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1]) || \
				(pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2] != cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2]))
			{
				PE_ND_ND2_H15A0_QWr03(tpd_ctrl_00,reg_hist0_data_mux,cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0], \
                                                  reg_hist1_data_mux,cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1], \
                                                  reg_hist2_data_mux,cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2]);
				pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0] = cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0];
				pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1] = cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1];
				pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2] = cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2];
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * set histo mode before ready to set src histo
 *
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetSrcHistoMode(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(cur_ctrl);
		/* set src histo window */
		PE_HST_HW_H15_SetSrcHistoWin(cur_ctrl);
		/* set src histo mux selection */
		PE_HST_HW_H15_SetSrcHistoMux(cur_ctrl);
	}while(0);
	return;
}
/**
 * get src histo from register
 *
 * @param   wId [in] LX_PE_WIN_ID
 * @param   *data [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_ID wId, UINT32 *data)
{
	int ret = RET_OK;
	UINT32 i=0, r_data=0;
	do {
		CHECK_KNULL(data);
		if(PE_KDRV_VER_H15)
		{
			if(wId==LX_PE_WIN_0)
			{
				PE_ND_ND1_H15A0_QWr03(tpd_hist_ai_ctrl,hif_hist_address,0x0, \
                                                       hif_hist_ai,     0x1, \
                                                       hif_hist_enable, 0x0);	//0:host,1:normal
				PE_ND_ND1_H15A0_QRd01(tpd_hist_ai_ctrl,hist_rd_available,r_data);
				if(r_data)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						PE_ND_ND1_H15A0_RdFL(tpd_hist_ai_data);
						r_data= PE_ND_ND1_H15A0_Rd(tpd_hist_ai_data);
						data[i] = GET_BITS(r_data,0,22);
					}
					data[PE_HST_HW_H15_HST_BIN_NUM]=PE_HST_HW_H15_HIST_VALID;
				}
				else
				{
					data[PE_HST_HW_H15_HST_BIN_NUM]=PE_HST_HW_H15_HIST_INVALID;
				}
				PE_ND_ND1_H15A0_QWr03(tpd_hist_ai_ctrl,hif_hist_address,0x0, \
                                                       hif_hist_ai,     0x0, \
                                                       hif_hist_enable, 0x1);	//0:host,1:normal
			}
			else
			{
				PE_ND_ND2_H15A0_QWr03(tpd_hist_ai_ctrl,hif_hist_address,0x0, \
                                                       hif_hist_ai,     0x1, \
                                                       hif_hist_enable, 0x0);	//0:host,1:normal
				PE_ND_ND2_H15A0_QRd01(tpd_hist_ai_ctrl,hist_rd_available,r_data);
				if(r_data)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						PE_ND_ND2_H15A0_RdFL(tpd_hist_ai_data);
						r_data= PE_ND_ND2_H15A0_Rd(tpd_hist_ai_data);
						data[i] = GET_BITS(r_data,0,22);
					}
					data[PE_HST_HW_H15_HST_BIN_NUM]=PE_HST_HW_H15_HIST_VALID;
				}
				else
				{
					data[PE_HST_HW_H15_HST_BIN_NUM]=PE_HST_HW_H15_HIST_INVALID;
				}
				PE_ND_ND2_H15A0_QWr03(tpd_hist_ai_ctrl,hif_hist_address,0x0, \
                                                       hif_hist_ai,     0x0, \
                                                       hif_hist_enable, 0x1);	//0:host,1:normal
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * get src histo
 *
 * @param   wId [in] LX_PE_WIN_ID
 * @param   sel [in] UINT32, histo selection, hist0~2
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_ID wId, UINT32 sel, PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	UINT32 pxl_cnt0=0, pxl_cnt1=0;
	PE_HST_HW_H15_SRC_HST_DATA_T *hist_data = &_g_pe_hst_hw_h15_info.src_hist.data;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			/* update histo */
			if(cur_ctrl->mod_sel==PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL)
			{
				if(wId==LX_PE_WIN_0)
				{
					PE_ND_ND1_H15A0_QWr01(tpd_hist_ai_ctrl,reg_read_hist_sel,sel);//0:hist0,1:hist1,etc:hist2
					switch(cur_ctrl->mux_sel[LX_PE_WIN_0][sel])
					{
						case PE_HST_HW_H15_SRC_HST_SEL_CB:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->cb0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_CR:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->cr0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_G:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->g0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_B:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->b0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_R:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->r0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_H:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->h0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_S:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->s0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_V:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->v0[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_Y:
						default:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->y0[0]));
							break;
					}
				}
				else	//LX_PE_WIN_0
				{
					PE_ND_ND2_H15A0_QWr01(tpd_hist_ai_ctrl,reg_read_hist_sel,sel);//0:hist0,1:hist1,etc:hist2
					switch(cur_ctrl->mux_sel[LX_PE_WIN_1][sel])
					{
						case PE_HST_HW_H15_SRC_HST_SEL_CB:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->cb1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_CR:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->cr1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_G:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->g1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_B:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->b1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_R:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->r1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_H:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->h1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_S:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->s1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_V:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->v1[0]));
							break;
						case PE_HST_HW_H15_SRC_HST_SEL_Y:
						default:
							PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_1, &(hist_data->y1[0]));
							break;
					}
				}
			}
			else	//PE_HST_HW_H15_SRC_HST_MOD_SEL_TB, PE_HST_HW_H15_SRC_HST_MOD_SEL_SS
			{
				if(wId==LX_PE_WIN_0)
				{
					PE_ND_ND1_H15A0_QWr01(tpd_hist_ai_ctrl,reg_read_hist_sel,sel);//0:hist0,1:hist1,etc:hist2
					/* store y0:full(hist1,2), y1:half(hist0), same for others */
					if(sel==PE_HST0)
					{
						switch(cur_ctrl->mux_sel[LX_PE_WIN_0][sel])
						{
							case PE_HST_HW_H15_SRC_HST_SEL_CB:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->cb1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_CR:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->cr1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_G:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->g1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_B:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->b1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_R:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->r1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_H:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->h1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_S:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->s1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_V:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->v1[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_Y:
							default:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->y1[0]));
								break;
						}
					}
					else
					{
						switch(cur_ctrl->mux_sel[LX_PE_WIN_0][sel])
						{
							case PE_HST_HW_H15_SRC_HST_SEL_CB:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->cb0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_CR:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->cr0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_G:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->g0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_B:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->b0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_R:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->r0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_H:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->h0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_S:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->s0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_V:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->v0[0]));
								break;
							case PE_HST_HW_H15_SRC_HST_SEL_Y:
							default:
								PE_HST_HW_H15_GetSrcHistoFromReg(LX_PE_WIN_0, &(hist_data->y0[0]));
								break;
						}
					}
				}
			}
			/* update pixel count */
			/* PE_HST0 is half, PE_HST1,2 is full for PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL */
			/* PE_HST0,1,2 is full for PE_HST_HW_H15_SRC_HST_MOD_SEL_SS,TB */
			if(sel==PE_HST1 && wId==LX_PE_WIN_0)
			{
				/* update win0 pxl cnt for HST1 only */
				PE_ND_ND1_H15A0_QRd01(tpd_stat_00,hist_pixel_count,pxl_cnt0);
				hist_data->pxl_cnt[LX_PE_WIN_0] = GET_BITS(pxl_cnt0,0,22);
			}
			else if(sel==PE_HST0 && wId==LX_PE_WIN_1 && \
				cur_ctrl->mod_sel==PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL)
			{
				/* update win1 cnt for HST0 only and PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL */
				PE_ND_ND2_H15A0_QRd01(tpd_stat_00,hist_pixel_count,pxl_cnt1);
				hist_data->pxl_cnt[LX_PE_WIN_1] = GET_BITS(pxl_cnt1,0,22);
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * update src histo
 *
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_UpdateSrcHisto(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	UINT32 bin6,bin7,bin8,bin9;
	UINT32 sum_total,sum_low,sum_high,s_state;
	PE_HST_HW_H15_SRC_HST_DATA_T *hist_data = &_g_pe_hst_hw_h15_info.src_hist.data;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_0,PE_HST0,cur_ctrl);	/* pe0 l hist0 */
			PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_0,PE_HST1,cur_ctrl);	/* pe0 l hist1 */
			PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_0,PE_HST2,cur_ctrl);	/* pe0 l hist2 */
			PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_1,PE_HST0,cur_ctrl);	/* pe0 r hist0 */
			PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_1,PE_HST1,cur_ctrl);	/* pe0 r hist1 */
			PE_HST_HW_H15_GetSrcHisto(LX_PE_WIN_1,PE_HST2,cur_ctrl);	/* pe0 r hist2 */
			/* get s_state */
			if(cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2]==PE_HST_HW_H15_SRC_HST_SEL_S)
			{
				/* protection in case of histo not available */
				if(hist_data->s0[0]!=PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE)
				{
					sum_low = hist_data->s0[0]+hist_data->s0[1]+hist_data->s0[2];
					sum_high = hist_data->s0[29]+hist_data->s0[30]+hist_data->s0[31];
					sum_total = (hist_data->pxl_cnt[LX_PE_WIN_0]>>8);
					s_state = (sum_total)? ((sum_low+sum_high)/sum_total):0;
					hist_data->s_state[LX_PE_WIN_0] = (s_state<256)? s_state:255;
				}
			}
			if(cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2]==PE_HST_HW_H15_SRC_HST_SEL_S)
			{
				/* protection in case of histo not available */
				if(hist_data->s1[0]!=PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE)
				{
					sum_low = hist_data->s1[0]+hist_data->s1[1]+hist_data->s1[2];
					sum_high = hist_data->s1[29]+hist_data->s1[30]+hist_data->s1[31];
					sum_total = (hist_data->pxl_cnt[LX_PE_WIN_1]>>8);
					s_state = (sum_total)? ((sum_low+sum_high)/sum_total):0;
					hist_data->s_state[LX_PE_WIN_1] = (s_state<256)? s_state:255;
				}
			}
			/* get hue_bin */
			if(cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1]==PE_HST_HW_H15_SRC_HST_SEL_H)
			{
				sum_total = (hist_data->pxl_cnt[LX_PE_WIN_0]>>8);
				bin6 = (sum_total)? ((hist_data->h0[6])/sum_total):0;
				bin7 = (sum_total)? ((hist_data->h0[7])/sum_total):0;
				bin8 = (sum_total)? ((hist_data->h0[8])/sum_total):0;
				bin9 = (sum_total)? ((hist_data->h0[9])/sum_total):0;
				if (bin6>255)	bin6=255;
				if (bin7>255)	bin7=255;
				if (bin8>255)	bin8=255;
				if (bin9>255)	bin9=255;
				hist_data->cur_h_bin[LX_PE_WIN_0] = (bin9<<24)|(bin8<<16)|(bin7<<8)|bin6;
			}
			if(cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1]==PE_HST_HW_H15_SRC_HST_SEL_H)
			{
				sum_total = (hist_data->pxl_cnt[LX_PE_WIN_1]>>8);
				bin6 = (sum_total)? ((hist_data->h0[6])/sum_total):0;
				bin7 = (sum_total)? ((hist_data->h0[7])/sum_total):0;
				bin8 = (sum_total)? ((hist_data->h0[8])/sum_total):0;
				bin9 = (sum_total)? ((hist_data->h0[9])/sum_total):0;
				if (bin6>255)	bin6=255;
				if (bin7>255)	bin7=255;
				if (bin8>255)	bin8=255;
				if (bin9>255)	bin9=255;
				hist_data->cur_h_bin[LX_PE_WIN_1] = (bin9<<24)|(bin8<<16)|(bin7<<8)|bin6;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * update src histo difference
 *
 * @param   sel [in] UINT32
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_GetSrcHistoInfo(UINT32 sel, PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	UINT32 diff,idx;
	PE_HST_HW_H15_SRC_HST_DATA_T *hist_data = &_g_pe_hst_hw_h15_info.src_hist.data;
	PE_HST_HW_H15_SRC_INFO_T *hist_info = &_g_pe_hst_hw_h15_info.hist_inf.src_inf;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			if(sel>PE_HST2)	sel=PE_HST0;
			if(cur_ctrl->mod_sel==PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL)
			{
				diff = 0;	// diff is not supported for full win case
				/* [0,1]y tb,ss,[2,3]cb,[4,5]cr, [6,7]g,[8,9]b,[10,11]r, [12,13]h,[14,15]s,[16,17]v */
				idx = cur_ctrl->mux_sel[LX_PE_WIN_0][sel]<<1;
				if((idx+1)>=PE_HST_HW_H15_SRC_INF_NUM)	idx=0;
				hist_info->dif_cnt[idx] = hist_info->dif_cnt[idx+1] = diff;
				hist_info->pxl_cnt[idx] = hist_data->pxl_cnt[LX_PE_WIN_0];
				hist_info->pxl_cnt[idx+1] = 0;	// pxl_cnt is not supported for full win case
				if(hist_info->set_cnt[idx]<PE_HST_HW_H15_HIST_SET_CNT)
					hist_info->set_cnt[idx]++;
				else
					hist_info->set_cnt[idx]=0;
				if(hist_info->set_cnt[idx+1]<PE_HST_HW_H15_HIST_SET_CNT)
					hist_info->set_cnt[idx+1]++;
				else
					hist_info->set_cnt[idx+1]=0;
			}
			else	//PE_HST_HW_H15_SRC_HST_MOD_SEL_TB, PE_HST_HW_H15_SRC_HST_MOD_SEL_SS
			{
				/* we stored y0:full(hist1,2), y1:half(hist0), same for others */
				switch(cur_ctrl->mux_sel[LX_PE_WIN_0][sel])
				{
					case PE_HST_HW_H15_SRC_HST_SEL_CB:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->cb0[0]),&(hist_data->cb1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_CR:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->cr0[0]),&(hist_data->cr1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_G:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->g0[0]),&(hist_data->g1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_B:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->b0[0]),&(hist_data->b1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_R:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->r0[0]),&(hist_data->r1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_H:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->h0[0]),&(hist_data->h1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_S:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->s0[0]),&(hist_data->s1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_V:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->v0[0]),&(hist_data->v1[0]),&diff);
						break;
					case PE_HST_HW_H15_SRC_HST_SEL_Y:
					default:
						PE_HST_HW_H15_GetHistDiffFromFh(&(hist_data->y0[0]),&(hist_data->y1[0]),&diff);
						break;
				}
				/* [0,1]y tb,ss,[2,3]cb,[4,5]cr, [6,7]g,[8,9]b,[10,11]r, [12,13]h,[14,15]s,[16,17]v */
				idx = cur_ctrl->mux_sel[LX_PE_WIN_0][sel]<<1;
				if((idx+1)>=PE_HST_HW_H15_SRC_INF_NUM)	idx=0;
				if(cur_ctrl->mod_sel==PE_HST_HW_H15_SRC_HST_MOD_SEL_TB)
				{
					/* protection in case of histo not available */
					if(diff!=PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE)
						hist_info->dif_cnt[idx] = diff;
					/********************************************/
					hist_info->pxl_cnt[idx] = hist_data->pxl_cnt[LX_PE_WIN_0];
					if(hist_info->set_cnt[idx]<PE_HST_HW_H15_HIST_SET_CNT)
						hist_info->set_cnt[idx]++;
					else
						hist_info->set_cnt[idx]=0;
				}
				else	//PE_HST_HW_H15_SRC_HST_MOD_SEL_SS
				{
					/* protection in case of histo not available */
					if(diff!=PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE)
						hist_info->dif_cnt[idx+1] = diff;
					/********************************************/
					hist_info->pxl_cnt[idx+1] = hist_data->pxl_cnt[LX_PE_WIN_0];
					if(hist_info->set_cnt[idx+1]<PE_HST_HW_H15_HIST_SET_CNT)
						hist_info->set_cnt[idx+1]++;
					else
						hist_info->set_cnt[idx+1]=0;
				}
			}
			if(sel==PE_HST0)	// for update once
			{
				/* set s_state */
				if(cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2]==PE_HST_HW_H15_SRC_HST_SEL_S)
				{
					if(hist_info->s_state[LX_PE_WIN_0]!=hist_data->s_state[LX_PE_WIN_0])
					{
						PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_00,reg_s_status_0,GET_BITS(hist_data->s_state[LX_PE_WIN_0],0,8));
						hist_info->s_state[LX_PE_WIN_0] = hist_data->s_state[LX_PE_WIN_0];
					}
				}
				if(cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2]==PE_HST_HW_H15_SRC_HST_SEL_S)
				{
					if(hist_info->s_state[LX_PE_WIN_1]!=hist_data->s_state[LX_PE_WIN_1])
					{
						PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_00,reg_s_status_1,GET_BITS(hist_data->s_state[LX_PE_WIN_0],0,8));
						hist_info->s_state[LX_PE_WIN_1] = hist_data->s_state[LX_PE_WIN_1];
					}
				}
				/* set hue_bin */
				if(cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1]==PE_HST_HW_H15_SRC_HST_SEL_H)
				{
					if(hist_data->pre_h_bin[LX_PE_WIN_0]!=hist_data->cur_h_bin[LX_PE_WIN_0])
					{
						PE_SHPR_H15A0_Wr(pe1_fsw_ctrl_00,hist_data->cur_h_bin[LX_PE_WIN_0]);
						PE_SHPR_H15A0_WrFL(pe1_fsw_ctrl_00);
						hist_data->pre_h_bin[LX_PE_WIN_0]=hist_data->cur_h_bin[LX_PE_WIN_0];
					}
				}
				if(cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1]==PE_HST_HW_H15_SRC_HST_SEL_H)
				{
					if(hist_data->pre_h_bin[LX_PE_WIN_1]!=hist_data->cur_h_bin[LX_PE_WIN_1])
					{
						PE_SHPR_H15A0_Wr(pe1_fsw_ctrl_01,hist_data->cur_h_bin[LX_PE_WIN_1]);
						PE_SHPR_H15A0_WrFL(pe1_fsw_ctrl_01);
						hist_data->pre_h_bin[LX_PE_WIN_1]=hist_data->cur_h_bin[LX_PE_WIN_1];
					}
				}
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * update src histo info
 *
 * @param   *ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_UpdateSrcHistoInfo(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			if(cur_ctrl->mod_sel==PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL)
			{
				PE_HST_HW_H15_GetSrcHistoInfo(PE_HST0, cur_ctrl);
				PE_HST_HW_H15_GetSrcHistoInfo(PE_HST1, cur_ctrl);
				PE_HST_HW_H15_GetSrcHistoInfo(PE_HST2, cur_ctrl);
			}
			else	//PE_HST_HW_H15_SRC_HST_MOD_SEL_TB, PE_HST_HW_H15_SRC_HST_MOD_SEL_SS
			{
				PE_HST_HW_H15_GetSrcHistoInfo(PE_HST0, cur_ctrl);
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * set rd enable
 *
 * @param   rd_en [in] UINT32.
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetSrcHistoRdEnable(UINT32 rd_en)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		PE_ND12_H15A0_QWr01(tpd_ctrl_00,hist_rd_en,(rd_en==PE_RD_EN_ON)? 0x1:0x0);
	}
	else
	{
		PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	}
	return;
}
/**
 * set rd done
 *
 * @param   rd_done [in] UINT32.
 * @return  void.
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetSrcHistoRdDone(UINT32 rd_done)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		PE_ND12_H15A0_QWr01(tpd_hist_ai_ctrl,hist_read_done,(rd_done==PE_RD_DONE_ON)? 0x1:0x0);
	}
	else
	{
		PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	}
	return;
}
/**
 * release to set src histo
 *
 * @param   rd_done [in] UINT32.
 * @return  void.
 * @see
 * @author
 */
static void PE_HST_HW_H15_ReleaseToSetSrcHisto(UINT32 rd_done)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		if(rd_done==PE_RD_DONE_OFF)
		{
			PE_HST_HW_H15_SetSrcHistoRdDone(PE_RD_DONE_OFF);
			PE_HST_HW_H15_SetSrcHistoRdEnable(PE_RD_EN_OFF);	// rd off
		}
		else	//PE_RD_DONE_ON
		{
			PE_HST_HW_H15_SetSrcHistoRdDone(PE_RD_DONE_ON);
		}
	}
	else
	{
		PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	}
	return;
}
/**
 * check src histo info of Y,H,S domain for full window 
 *	- get Y full,H full and S full
 *
 * @param   *cur_ctrl [in] PE_HST_HW_H15_SRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_CheckSrcHistoInfoForYhsFullWin(PE_HST_HW_H15_SRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	UINT32 under_30hz = 0;
	PE_HST_HW_H15_STATE_T *hist_state = &_g_pe_hst_hw_h15_info.src_hist.state;
	PE_HST_HW_H15_SRC_HST_CTRL_T *pre_ctrl = &_g_pe_hst_hw_h15_info.src_hist.ctrl;
	do {
		CHECK_KNULL(cur_ctrl);
		hist_state->curr = hist_state->next;
		/* update the cur_ctrl because it is a local variable */
		cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0] = pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0];
		cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1] = pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1];
		cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2] = pre_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2];
		cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0] = pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0];
		cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1] = pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1];
		cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2] = pre_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2];
		cur_ctrl->mod_sel = pre_ctrl->mod_sel;
		under_30hz = (cur_ctrl->in_f_rate < 305)? 1:0;
		switch(hist_state->curr)
		{
			/* 0. start */
			case PE_HST_HW_H15_SRC_HST_STM_START:
				PE_HST_HW_H15_SetSrcHistoInit();
				/* set next histo mode */
				cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST0] = PE_HST_HW_H15_SRC_HST_SEL_Y;
				cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST1] = PE_HST_HW_H15_SRC_HST_SEL_H;
				cur_ctrl->mux_sel[LX_PE_WIN_0][PE_HST2] = PE_HST_HW_H15_SRC_HST_SEL_S;
				cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST0] = PE_HST_HW_H15_SRC_HST_SEL_Y;
				cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST1] = PE_HST_HW_H15_SRC_HST_SEL_H;
				cur_ctrl->mux_sel[LX_PE_WIN_1][PE_HST2] = PE_HST_HW_H15_SRC_HST_SEL_S;
				cur_ctrl->mod_sel = PE_HST_HW_H15_SRC_HST_MOD_SEL_FULL;
				PE_HST_HW_H15_SetSrcHistoMode(cur_ctrl);
				hist_state->next = under_30hz? PE_HST_HW_H15_SRC_HST_STM_RDY_01ST_0:\
					PE_HST_HW_H15_SRC_HST_STM_RDY_02ND_0;
				break;
			/* 1. y,h,s full */
			case PE_HST_HW_H15_SRC_HST_STM_RDY_01ST_0:
				hist_state->next = PE_HST_HW_H15_SRC_HST_STM_RDY_02ND_0;
				break;
			case PE_HST_HW_H15_SRC_HST_STM_RDY_02ND_0:
				PE_HST_HW_H15_SetSrcHistoRdDone(PE_RD_DONE_OFF);
				PE_HST_HW_H15_SetSrcHistoRdEnable(PE_RD_EN_ON);
				hist_state->next = under_30hz? PE_HST_HW_H15_SRC_HST_STM_RDY_01ST_1:\
					PE_HST_HW_H15_SRC_HST_STM_RDY_02ND_1;
				break;
			case PE_HST_HW_H15_SRC_HST_STM_RDY_01ST_1:
				hist_state->next = PE_HST_HW_H15_SRC_HST_STM_RDY_02ND_1;
				break;
			case PE_HST_HW_H15_SRC_HST_STM_RDY_02ND_1:
				hist_state->next = under_30hz? PE_HST_HW_H15_SRC_HST_STM_GET_01ST:\
					PE_HST_HW_H15_SRC_HST_STM_GET_02ND;
				break;
			case PE_HST_HW_H15_SRC_HST_STM_GET_01ST:
				hist_state->next = PE_HST_HW_H15_SRC_HST_STM_GET_02ND;
				break;
			case PE_HST_HW_H15_SRC_HST_STM_GET_02ND:
				PE_HST_HW_H15_UpdateSrcHisto(cur_ctrl);
				PE_HST_HW_H15_SetSrcHistoRdDone(PE_RD_DONE_ON);
				PE_HST_HW_H15_UpdateSrcHistoInfo(cur_ctrl);
				PE_HST_HW_H15_SetSrcHistoRdEnable(PE_RD_EN_OFF);
				hist_state->next = under_30hz? PE_HST_HW_H15_SRC_HST_STM_RDY_01ST_0:\
					PE_HST_HW_H15_SRC_HST_STM_RDY_02ND_0;
				break;
			default:
				PE_HST_HW_H15_DBG_PRINT("hist_state(%d) wrong type.\n",hist_state->curr);
				break;
		}
		pre_ctrl->mod_sel = cur_ctrl->mod_sel;
	}while(0);
	return;
}
/**
 * check src histo info
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_HW_H15_CheckSrcHistoInfo(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	PE_HST_HW_H15_SRC_HST_CTRL_T cur_ctrl;
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl1_info = &pCfg->inf.ctrl_info[LX_PE_WIN_1];
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_HST_HW_H15_STATE_T *hist_state = &_g_pe_hst_hw_h15_info.src_hist.state;
	LX_PE_HST_HISTO_OPR_T *hist_opr = &_g_pe_hst_hw_h15_info.cfg[LX_PE_WIN_0].opr;
	PE_HST_HW_H15_SRC_INFO_T *hist_info = &_g_pe_hst_hw_h15_info.hist_inf.src_inf;
	PE_HST_HW_H15_SRC_HST_CTRL_T *pre_ctrl = &_g_pe_hst_hw_h15_info.src_hist.ctrl;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			cur_ctrl.out_type = disp0_info->out_type;
			cur_ctrl.in_f_rate = disp0_info->in_f_rate;
			if(pre_ctrl->out_type!=cur_ctrl.out_type)
			{
				hist_state->next = PE_HST_HW_H15_SRC_HST_STM_START;
				/* reset set cnt */
				memset(hist_info->set_cnt,0,sizeof(UINT32)*PE_HST_HW_H15_SRC_INF_NUM);
				PE_HST_HW_H15_ReleaseToSetSrcHisto(PE_RD_DONE_ON);
				PE_HST_HW_H15_ReleaseToSetSrcHisto(PE_RD_DONE_OFF);
				PE_HST_HW_H15_DBG_PRINT("Release src hist, outtype change(%d to %d).\n",\
					pre_ctrl->out_type,cur_ctrl.out_type);
				pre_ctrl->out_type=cur_ctrl.out_type;
			}
			if(((ctrl0_info->pe0_src_h==0||ctrl0_info->pe0_src_v==0)&& \
				(ctrl1_info->pe0_src_h==0||ctrl1_info->pe0_src_v==0)) || \
				hist_opr->src_hist_op==PE_HST_HW_H15_HIST_OP_STOP)
			{
				hist_state->next = PE_HST_HW_H15_SRC_HST_STM_START;
				break;
			}
			cur_ctrl.x_ofst[LX_PE_WIN_0] = 0;
			cur_ctrl.y_ofst[LX_PE_WIN_0] = 0;
			cur_ctrl.h_size[LX_PE_WIN_0] = ctrl0_info->pe0_src_h;
			cur_ctrl.v_size[LX_PE_WIN_0] = ctrl0_info->pe0_src_v;
			cur_ctrl.x_ofst[LX_PE_WIN_1] = 0;
			cur_ctrl.y_ofst[LX_PE_WIN_1] = 0;
			cur_ctrl.h_size[LX_PE_WIN_1] = ctrl1_info->pe0_src_h;
			cur_ctrl.v_size[LX_PE_WIN_1] = ctrl1_info->pe0_src_v;
			if(pre_ctrl->h_size[LX_PE_WIN_0]!=cur_ctrl.h_size[LX_PE_WIN_0] || \
				pre_ctrl->v_size[LX_PE_WIN_0]!=cur_ctrl.v_size[LX_PE_WIN_0] || \
				pre_ctrl->h_size[LX_PE_WIN_1]!=cur_ctrl.h_size[LX_PE_WIN_1] || \
				pre_ctrl->v_size[LX_PE_WIN_1]!=cur_ctrl.v_size[LX_PE_WIN_1])
			{
				PE_HST_HW_H15_DBG_PRINT("size change([w0]%d,%d to %d,%d, [w1]%d,%d to %d,%d)\n",\
					pre_ctrl->h_size[LX_PE_WIN_0],pre_ctrl->v_size[LX_PE_WIN_0],\
					cur_ctrl.h_size[LX_PE_WIN_0],cur_ctrl.v_size[LX_PE_WIN_0],\
					pre_ctrl->h_size[LX_PE_WIN_1],pre_ctrl->v_size[LX_PE_WIN_1],\
					cur_ctrl.h_size[LX_PE_WIN_1],cur_ctrl.v_size[LX_PE_WIN_1]);
				hist_state->next = PE_HST_HW_H15_SRC_HST_STM_START;
			}
			PE_HST_HW_H15_CheckSrcHistoInfoForYhsFullWin(&cur_ctrl);
			pre_ctrl->in_f_rate=cur_ctrl.in_f_rate;
			pre_ctrl->x_ofst[LX_PE_WIN_0]=cur_ctrl.x_ofst[LX_PE_WIN_0];
			pre_ctrl->y_ofst[LX_PE_WIN_0]=cur_ctrl.y_ofst[LX_PE_WIN_0];
			pre_ctrl->h_size[LX_PE_WIN_0]=cur_ctrl.h_size[LX_PE_WIN_0];
			pre_ctrl->v_size[LX_PE_WIN_0]=cur_ctrl.v_size[LX_PE_WIN_0];
			pre_ctrl->x_ofst[LX_PE_WIN_1]=cur_ctrl.x_ofst[LX_PE_WIN_1];
			pre_ctrl->y_ofst[LX_PE_WIN_1]=cur_ctrl.y_ofst[LX_PE_WIN_1];
			pre_ctrl->h_size[LX_PE_WIN_1]=cur_ctrl.h_size[LX_PE_WIN_1];
			pre_ctrl->v_size[LX_PE_WIN_1]=cur_ctrl.v_size[LX_PE_WIN_1];
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * ready to set lrcr histo
 *
 * @param   void
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_ReadyToSetLrcHisto(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		PE_CE_PE1_H15A0_QWr01(pe1_lrcr_ctrl_00, hist_rd_en, 0x1);
	}
	else
	{
		PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	}
	return;
}
/**
 * get lrcr histo from register
 *
 * @param   hst_sel [in] UINT32
 * @param   *l_data [in] UINT32
 * @param   *r_data [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_GetLrcHistoFromReg(UINT32 hst_sel, UINT32 *l_data, UINT32 *r_data)
{
	int ret = RET_OK;
	UINT32 i=0, rd_data=0;
	do {
		CHECK_KNULL(l_data);
		CHECK_KNULL(r_data);
		if(PE_KDRV_VER_H15)
		{
			switch(hst_sel)
			{
				case PE_HST_HW_H15_LRC_HST_SEL_TB0_GRN:
				case PE_HST_HW_H15_LRC_HST_SEL_TB1_GRN:
				case PE_HST_HW_H15_LRC_HST_SEL_SS0_GRN:
				case PE_HST_HW_H15_LRC_HST_SEL_SS1_GRN:
					PE_CE_PE1_H15A0_RdFL(pe1_l_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Rd01(pe1_l_g_hist_ia_ctrl,	hist_rd_g_available,	rd_data);
					if(rd_data)
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							PE_CE_PE1_H15A0_RdFL(pe1_l_g_hist_ia_data);
							rd_data= PE_CE_PE1_H15A0_Rd(pe1_l_g_hist_ia_data);
							l_data[i] = GET_BITS(rd_data,0,22);
						}
					}
					else
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							l_data[i] = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
						}
					}
					PE_CE_PE1_H15A0_RdFL(pe1_r_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Rd01(pe1_r_g_hist_ia_ctrl,	hist_rd_g_available,	rd_data);
					if(rd_data)
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							PE_CE_PE1_H15A0_RdFL(pe1_r_g_hist_ia_data);
							rd_data= PE_CE_PE1_H15A0_Rd(pe1_r_g_hist_ia_data);
							r_data[i] = GET_BITS(rd_data,0,22);
						}
					}
					else
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							r_data[i] = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
						}
					}
					break;
				case PE_HST_HW_H15_LRC_HST_SEL_TB0_BLU:
				case PE_HST_HW_H15_LRC_HST_SEL_TB1_BLU:
				case PE_HST_HW_H15_LRC_HST_SEL_SS0_BLU:
				case PE_HST_HW_H15_LRC_HST_SEL_SS1_BLU:
					PE_CE_PE1_H15A0_RdFL(pe1_l_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Rd01(pe1_l_b_hist_ia_ctrl,	hist_rd_b_available,	rd_data);
					if(rd_data)
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							PE_CE_PE1_H15A0_RdFL(pe1_l_b_hist_ia_data);
							rd_data= PE_CE_PE1_H15A0_Rd(pe1_l_b_hist_ia_data);
							l_data[i] = GET_BITS(rd_data,0,22);
						}
					}
					else
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							l_data[i] = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
						}
					}
					PE_CE_PE1_H15A0_RdFL(pe1_r_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Rd01(pe1_r_b_hist_ia_ctrl,	hist_rd_b_available,	rd_data);
					if(rd_data)
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							PE_CE_PE1_H15A0_RdFL(pe1_r_b_hist_ia_data);
							rd_data= PE_CE_PE1_H15A0_Rd(pe1_r_b_hist_ia_data);
							r_data[i] = GET_BITS(rd_data,0,22);
						}
					}
					else
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							r_data[i] = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
						}
					}
					break;
				case PE_HST_HW_H15_LRC_HST_SEL_TB0_RED:
				case PE_HST_HW_H15_LRC_HST_SEL_TB1_RED:
				case PE_HST_HW_H15_LRC_HST_SEL_SS0_RED:
				case PE_HST_HW_H15_LRC_HST_SEL_SS1_RED:
				default:
					PE_CE_PE1_H15A0_RdFL(pe1_l_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Rd01(pe1_l_r_hist_ia_ctrl,	hist_rd_r_available,	rd_data);
					if(rd_data)
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							PE_CE_PE1_H15A0_RdFL(pe1_l_r_hist_ia_data);
							rd_data= PE_CE_PE1_H15A0_Rd(pe1_l_r_hist_ia_data);
							l_data[i] = GET_BITS(rd_data,0,22);
						}
					}
					else
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							l_data[i] = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
						}
					}
					PE_CE_PE1_H15A0_RdFL(pe1_r_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Rd01(pe1_r_r_hist_ia_ctrl,	hist_rd_r_available,	rd_data);
					if(rd_data)
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							PE_CE_PE1_H15A0_RdFL(pe1_r_r_hist_ia_data);
							rd_data= PE_CE_PE1_H15A0_Rd(pe1_r_r_hist_ia_data);
							r_data[i] = GET_BITS(rd_data,0,22);
						}
					}
					else
					{
						for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
						{
							r_data[i] = PE_HST_HW_H15_HIST_BIN_NOTAVAILABLE;
						}
					}
					break;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * update lrcr histo
 *
 * @param   *ctrl [in] PE_HST_HW_H15_LRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_UpdateLrcHisto(PE_HST_HW_H15_LRC_HST_CTRL_T *cur_ctrl)
{
	int ret = RET_OK;
	PE_HST_HW_H15_LRC_HST_DATA_T *hist_data = &_g_pe_hst_hw_h15_info.lrc_hist.data;
	do {
		CHECK_KNULL(cur_ctrl);
		if(PE_KDRV_VER_H15)
		{
			switch(cur_ctrl->hst_sel)
			{
				case PE_HST_HW_H15_LRC_HST_SEL_TB0_GRN:
				case PE_HST_HW_H15_LRC_HST_SEL_TB1_GRN:
				case PE_HST_HW_H15_LRC_HST_SEL_SS0_GRN:
				case PE_HST_HW_H15_LRC_HST_SEL_SS1_GRN:
					PE_CE_PE1_H15A0_RdFL(pe1_l_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_l_g_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_g_hist_ia_ctrl,	hif_hist_ai,		0x1);
					PE_CE_PE1_H15A0_Wr01(pe1_l_g_hist_ia_ctrl,	hif_hist_enable,	0x0);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_l_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_RdFL(pe1_r_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_r_g_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_g_hist_ia_ctrl,	hif_hist_ai,		0x1);
					PE_CE_PE1_H15A0_Wr01(pe1_r_g_hist_ia_ctrl,	hif_hist_enable,	0x0);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_r_g_hist_ia_ctrl);
					if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_TB0_GRN)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->tb0_t_grn[0]),&(hist_data->tb0_b_grn[0]));
					}
					else if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_SS0_GRN)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->ss0_l_grn[0]),&(hist_data->ss0_r_grn[0]));
					}
					else if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_TB1_GRN)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->tb1_t_grn[0]),&(hist_data->tb1_b_grn[0]));
					}
					else	//PE_HST_HW_H15_LRC_HST_SEL_SS1_GRN
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->ss1_l_grn[0]),&(hist_data->ss1_r_grn[0]));
					}
					PE_CE_PE1_H15A0_RdFL(pe1_l_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_l_g_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_g_hist_ia_ctrl,	hif_hist_ai,		0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_g_hist_ia_ctrl,	hif_hist_enable,	0x1);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_l_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_RdFL(pe1_r_g_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_r_g_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_g_hist_ia_ctrl,	hif_hist_ai,		0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_g_hist_ia_ctrl,	hif_hist_enable,	0x1);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_r_g_hist_ia_ctrl);
					break;
				case PE_HST_HW_H15_LRC_HST_SEL_TB0_BLU:
				case PE_HST_HW_H15_LRC_HST_SEL_TB1_BLU:
				case PE_HST_HW_H15_LRC_HST_SEL_SS0_BLU:
				case PE_HST_HW_H15_LRC_HST_SEL_SS1_BLU:
					PE_CE_PE1_H15A0_RdFL(pe1_l_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_l_b_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_b_hist_ia_ctrl,	hif_hist_ai,		0x1);
					PE_CE_PE1_H15A0_Wr01(pe1_l_b_hist_ia_ctrl,	hif_hist_enable,	0x0);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_l_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_RdFL(pe1_r_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_r_b_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_b_hist_ia_ctrl,	hif_hist_ai,		0x1);
					PE_CE_PE1_H15A0_Wr01(pe1_r_b_hist_ia_ctrl,	hif_hist_enable,	0x0);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_r_b_hist_ia_ctrl);
					if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_TB0_BLU)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->tb0_t_blu[0]),&(hist_data->tb0_b_blu[0]));
					}
					else if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_SS0_BLU)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->ss0_l_blu[0]),&(hist_data->ss0_r_blu[0]));
					}
					else if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_TB1_BLU)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->tb1_t_blu[0]),&(hist_data->tb1_b_blu[0]));
					}
					else	//PE_HST_HW_H15_LRC_HST_SEL_SS1_BLU
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->ss1_l_blu[0]),&(hist_data->ss1_r_blu[0]));
					}
					PE_CE_PE1_H15A0_RdFL(pe1_l_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_l_b_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_b_hist_ia_ctrl,	hif_hist_ai,		0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_b_hist_ia_ctrl,	hif_hist_enable,	0x1);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_l_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_RdFL(pe1_r_b_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_r_b_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_b_hist_ia_ctrl,	hif_hist_ai,		0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_b_hist_ia_ctrl,	hif_hist_enable,	0x1);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_r_b_hist_ia_ctrl);
					break;
				case PE_HST_HW_H15_LRC_HST_SEL_TB0_RED:
				case PE_HST_HW_H15_LRC_HST_SEL_TB1_RED:
				case PE_HST_HW_H15_LRC_HST_SEL_SS0_RED:
				case PE_HST_HW_H15_LRC_HST_SEL_SS1_RED:
					PE_CE_PE1_H15A0_RdFL(pe1_l_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_l_r_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_r_hist_ia_ctrl,	hif_hist_ai,		0x1);
					PE_CE_PE1_H15A0_Wr01(pe1_l_r_hist_ia_ctrl,	hif_hist_enable,	0x0);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_l_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_RdFL(pe1_r_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_r_r_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_r_hist_ia_ctrl,	hif_hist_ai,		0x1);
					PE_CE_PE1_H15A0_Wr01(pe1_r_r_hist_ia_ctrl,	hif_hist_enable,	0x0);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_r_r_hist_ia_ctrl);
					if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_TB0_RED)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->tb0_t_red[0]),&(hist_data->tb0_b_red[0]));
					}
					else if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_SS0_RED)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->ss0_l_red[0]),&(hist_data->ss0_r_red[0]));
					}
					else if(cur_ctrl->hst_sel==PE_HST_HW_H15_LRC_HST_SEL_TB1_RED)
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->tb1_t_red[0]),&(hist_data->tb1_b_red[0]));
					}
					else	//PE_HST_HW_H15_LRC_HST_SEL_SS1_RED
					{
						PE_HST_HW_H15_GetLrcHistoFromReg(cur_ctrl->hst_sel,\
							&(hist_data->ss1_l_red[0]),&(hist_data->ss1_r_red[0]));
					}
					PE_CE_PE1_H15A0_RdFL(pe1_l_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_l_r_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_r_hist_ia_ctrl,	hif_hist_ai,		0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_l_r_hist_ia_ctrl,	hif_hist_enable,	0x1);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_l_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_RdFL(pe1_r_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_r_r_hist_ia_ctrl,	hif_hist_address,	0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_r_hist_ia_ctrl,	hif_hist_ai,		0x0);
					PE_CE_PE1_H15A0_Wr01(pe1_r_r_hist_ia_ctrl,	hif_hist_enable,	0x1);	//0:host,1:normal
					PE_CE_PE1_H15A0_WrFL(pe1_r_r_hist_ia_ctrl);
					break;
				case PE_HST_HW_H15_LRC_HST_SEL_RD_DONE:
				default:
					PE_CE_PE1_H15A0_RdFL(pe1_r_r_hist_ia_ctrl);
					PE_CE_PE1_H15A0_Wr01(pe1_r_r_hist_ia_ctrl,	hist_rd_done,	0x1);
					PE_CE_PE1_H15A0_WrFL(pe1_r_r_hist_ia_ctrl);
					break;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * release to set lrcr histo
 *
 * @param   void
 * @return  void.
 * @see
 * @author
 */
static void PE_HST_HW_H15_ReleaseToSetLrcHisto(void)
{
	int ret = RET_OK;
	if(PE_KDRV_VER_H15)
	{
		PE_CE_PE1_H15A0_QWr01(pe1_r_r_hist_ia_ctrl,hist_rd_done,0x0);
		PE_CE_PE1_H15A0_QWr01(pe1_lrcr_ctrl_00,    hist_rd_en,  0x0);
	}
	else
	{
		PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
	}
	return;
}
/**
 * update lrcr ctrl
 *
 * @param   *ctrl [in] PE_TSK_H15_LRC_HST_CTRL_T
 * @return  void.
 * @see
 * @author
 */
static void PE_HST_HW_H15_UpdateLrcCtrl(PE_TSK_H15_LRC_HST_CTRL_T *ctrl)
{
	int ret = RET_OK;
	UINT32 i;
	PE_HST_HW_H15_LRC_HST_CTRL_T *hist_ctrl = &_g_pe_hst_hw_h15_info.lrc_hist.ctrl;
	do {
		CHECK_KNULL(ctrl);
		if(PE_KDRV_VER_H15)
		{
			switch(ctrl->win_mod)
			{
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0:
					hist_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0;
					for(i=0;i<4;i++)
					{
						hist_ctrl->tb0_t_pos[i] = ctrl->tb0_t_pos[i];
						hist_ctrl->tb0_b_pos[i] = ctrl->tb0_b_pos[i];
					}
					break;
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0:
					hist_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0;
					for(i=0;i<4;i++)
					{
						hist_ctrl->ss0_l_pos[i] = ctrl->ss0_l_pos[i];
						hist_ctrl->ss0_r_pos[i] = ctrl->ss0_r_pos[i];
					}
					break;
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1:
					hist_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1;
					for(i=0;i<4;i++)
					{
						hist_ctrl->tb1_t_pos[i] = ctrl->tb1_t_pos[i];
						hist_ctrl->tb1_b_pos[i] = ctrl->tb1_b_pos[i];
					}
					break;
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1:
					hist_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1;
					for(i=0;i<4;i++)
					{
						hist_ctrl->ss1_l_pos[i] = ctrl->ss1_l_pos[i];
						hist_ctrl->ss1_r_pos[i] = ctrl->ss1_r_pos[i];
					}
					break;
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_2D:
				default:
					break;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * update lrcr info
 *
 * @param   *ctrl [in] PE_TSK_H15_LRC_HST_CTRL_T
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_UpdateLrcInfo(PE_TSK_H15_LRC_HST_CTRL_T *ctrl)
{
	int ret = RET_OK;
	UINT32 g_diff,b_diff,r_diff,idx;
	PE_HST_HW_H15_LRC_HST_DATA_T *hist_data = &_g_pe_hst_hw_h15_info.lrc_hist.data;
	PE_HST_HW_H15_LRC_INFO_T *hist_info = &_g_pe_hst_hw_h15_info.hist_inf.lrc_inf;
	do {
		CHECK_KNULL(ctrl);
		if(PE_KDRV_VER_H15)
		{
			if(ctrl->win_mod==PE_HST_HW_H15_LRC_HST_WIN_MOD_2D)
			{
				PE_HST_HW_H15_DBG_PRINT("nothing to do for win_mod 2D.\n");
				break;
			}
			switch(ctrl->win_mod)
			{
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0:
					idx = 0;
					PE_HST_HW_H15_GetHistDiff(&(hist_data->tb0_t_grn[0]),&(hist_data->tb0_b_grn[0]),&g_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->tb0_t_blu[0]),&(hist_data->tb0_b_blu[0]),&b_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->tb0_t_red[0]),&(hist_data->tb0_b_red[0]),&r_diff);
					break;
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0:
					idx = 1;
					PE_HST_HW_H15_GetHistDiff(&(hist_data->ss0_l_grn[0]),&(hist_data->ss0_r_grn[0]),&g_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->ss0_l_blu[0]),&(hist_data->ss0_r_blu[0]),&b_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->ss0_l_red[0]),&(hist_data->ss0_r_red[0]),&r_diff);
					break;
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1:
					idx = 2;
					PE_HST_HW_H15_GetHistDiff(&(hist_data->tb1_t_grn[0]),&(hist_data->tb1_b_grn[0]),&g_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->tb1_t_blu[0]),&(hist_data->tb1_b_blu[0]),&b_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->tb1_t_red[0]),&(hist_data->tb1_b_red[0]),&r_diff);
					break;
				case PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1:
				default:
					idx = 3;
					PE_HST_HW_H15_GetHistDiff(&(hist_data->ss1_l_grn[0]),&(hist_data->ss1_r_grn[0]),&g_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->ss1_l_blu[0]),&(hist_data->ss1_r_blu[0]),&b_diff);
					PE_HST_HW_H15_GetHistDiff(&(hist_data->ss1_l_red[0]),&(hist_data->ss1_r_red[0]),&r_diff);
					break;
			}
			hist_info->dif_max[idx] = PE_GET_MAX(g_diff,b_diff,r_diff);
			hist_info->pxl_cnt[idx] = ctrl->pxl_cnt[idx];
			if(hist_info->set_cnt[idx]<PE_HST_HW_H15_HIST_SET_CNT)
				hist_info->set_cnt[idx]++;
			else
				hist_info->set_cnt[idx]=0;
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return;
}
/**
 * check lrcr histo info
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_HW_H15_CheckLrcrHistoInfo(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	UINT32 is_2d_case,wId = 0;
	PE_HST_HW_H15_LRC_HST_CTRL_T cur_ctrl;
	PE_TSK_H15_LRC_HST_CTRL_T *lrc_ctrl = &pCfg->lrc;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_INF_H15_CTRL_T *ctrl0_info = &pCfg->inf.ctrl_info[LX_PE_WIN_0];
	PE_HST_HW_H15_STATE_T *hist_state = &_g_pe_hst_hw_h15_info.lrc_hist.state;
	LX_PE_HST_HISTO_OPR_T *hist_opr = &_g_pe_hst_hw_h15_info.cfg[wId].opr;
	PE_HST_HW_H15_LRC_INFO_T *hist_info = &_g_pe_hst_hw_h15_info.hist_inf.lrc_inf;
	PE_HST_HW_H15_LRC_HST_CTRL_T *pre_ctrl = &_g_pe_hst_hw_h15_info.lrc_hist.ctrl;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			cur_ctrl.out_type = disp0_info->out_type;
			if(pre_ctrl->out_type!=cur_ctrl.out_type)
			{
				lrc_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_2D;	/* to set lrcr win */
				hist_state->next = PE_HST_HW_H15_LRC_HST_STM_INIT;
				/* reset set cnt */
				hist_info->set_cnt[0]=hist_info->set_cnt[1]=0;
				hist_info->set_cnt[2]=hist_info->set_cnt[3]=0;
				/**********************************************************/
				/* caution : need to turn rd done on before release histo */
				cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_RD_DONE;
				PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
				/**********************************************************/
				PE_HST_HW_H15_ReleaseToSetLrcHisto();
				pre_ctrl->out_type=cur_ctrl.out_type;
			}
			if(disp0_info->mode.is_ext_frc)	/* external frc */
			{
				is_2d_case = (cur_ctrl.out_type==LX_PE_OUT_2D || \
					cur_ctrl.out_type==LX_PE_OUT_2D_3D)? 1:0;
			}
			else	/* internal frc */
			{
				is_2d_case = (cur_ctrl.out_type==LX_PE_OUT_2D || \
					cur_ctrl.out_type==LX_PE_OUT_2D_3D)? 1:0;
			}
			if((is_2d_case && (ctrl0_info->pe0_src_h==0||ctrl0_info->pe0_src_v==0)) || \
				(!is_2d_case) || (hist_opr->lrc_hist_op==PE_HST_HW_H15_HIST_OP_STOP))
			{
				break;
			}
			hist_state->curr = hist_state->next;
			switch(hist_state->curr)
			{
				/* init */
				case PE_HST_HW_H15_LRC_HST_STM_INIT:
					lrc_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0;	/* to set lrcr win tb0 */
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY0_0;
					break;
				/* tb0 */
				case PE_HST_HW_H15_LRC_HST_STM_RDY0_0:
					PE_HST_HW_H15_ReadyToSetLrcHisto();
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY0_1;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RDY0_1:
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_GET0;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_GET0:
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_TB0_GRN;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_TB0_BLU;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_TB0_RED;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_RD_DONE;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RLS0;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RLS0:
					PE_HST_HW_H15_ReleaseToSetLrcHisto();
					PE_HST_HW_H15_UpdateLrcCtrl(lrc_ctrl);
					PE_HST_HW_H15_UpdateLrcInfo(lrc_ctrl);
					lrc_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_SS0;	/* to set lrcr win ss0 */
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY1_0;
					break;
				/* ss0 */
				case PE_HST_HW_H15_LRC_HST_STM_RDY1_0:
					PE_HST_HW_H15_ReadyToSetLrcHisto();
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY1_1;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RDY1_1:
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_GET1;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_GET1:
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_SS0_GRN;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_SS0_BLU;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_SS0_RED;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_RD_DONE;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RLS1;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RLS1:
					PE_HST_HW_H15_ReleaseToSetLrcHisto();
					PE_HST_HW_H15_UpdateLrcCtrl(lrc_ctrl);
					PE_HST_HW_H15_UpdateLrcInfo(lrc_ctrl);
					lrc_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_TB1;	/* to set lrcr win tb1 */
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY2_0;
					break;
				/* tb1 */
				case PE_HST_HW_H15_LRC_HST_STM_RDY2_0:
					PE_HST_HW_H15_ReadyToSetLrcHisto();
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY2_1;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RDY2_1:
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_GET2;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_GET2:
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_TB1_GRN;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_TB1_BLU;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_TB1_RED;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_RD_DONE;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RLS2;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RLS2:
					PE_HST_HW_H15_ReleaseToSetLrcHisto();
					PE_HST_HW_H15_UpdateLrcCtrl(lrc_ctrl);
					PE_HST_HW_H15_UpdateLrcInfo(lrc_ctrl);
					lrc_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_SS1;	/* to set lrcr win ss1 */
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY3_0;
					break;
				/* ss1 */
				case PE_HST_HW_H15_LRC_HST_STM_RDY3_0:
					PE_HST_HW_H15_ReadyToSetLrcHisto();
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY3_1;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RDY3_1:
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_GET3;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_GET3:
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_SS1_GRN;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_SS1_BLU;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_SS1_RED;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					cur_ctrl.hst_sel = PE_HST_HW_H15_LRC_HST_SEL_RD_DONE;
					PE_HST_HW_H15_UpdateLrcHisto(&cur_ctrl);
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RLS3;
					break;
				case PE_HST_HW_H15_LRC_HST_STM_RLS3:
					PE_HST_HW_H15_ReleaseToSetLrcHisto();
					PE_HST_HW_H15_UpdateLrcCtrl(lrc_ctrl);
					PE_HST_HW_H15_UpdateLrcInfo(lrc_ctrl);
					lrc_ctrl->win_mod = PE_HST_HW_H15_LRC_HST_WIN_MOD_TB0;	/* to set lrcr win tb0 */
					hist_state->next = PE_HST_HW_H15_LRC_HST_STM_RDY0_0;
					break;
				default:
					PE_HST_HW_H15_DBG_PRINT("hist_state->curr:%d, wrong type.\n",hist_state->curr);
					break;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set src histo print buffer
 *
 * @param   *str [in] char
 * @param   *rd_cnt [in] UINT32
 * @param   *buf [in] char
 * @param   *data [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetSrcHstPrintBuf(char *str, UINT32 *rd_cnt, char *buf, UINT32 *data)
{
	int ret = RET_OK;

#define PE_HST_H15_PRINT_START		"START_OF_PRINT"
#define PE_HST_H15_PRINT_END		"END_OF_PRINT"
#define PE_HST_H15_CASE_SPRINT(_cnt,_action,_buf,fmt,args...)	\
	case (_cnt):sprintf(_buf,fmt,##args);_action;break
#define PE_HST_H15_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(str);
		CHECK_KNULL(rd_cnt);
		CHECK_KNULL(buf);
		CHECK_KNULL(data);
		switch(*rd_cnt)
		{
			PE_HST_H15_CASE_SPRINT( 0,(*rd_cnt)++,buf,"%s",PE_HST_H15_PRINT_START);
			PE_HST_H15_CASE_SPRINT( 1,(*rd_cnt)++,buf,"src hst %s (valid:%d)\n",str,data[32]);
			PE_HST_H15_CASE_SPRINT( 2,(*rd_cnt)++,buf,\
				"[00]%7d [01]%7d [02]%7d [03]%7d [04]%7d [05]%7d [06]%7d [07]%7d\n",\
				data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
			PE_HST_H15_CASE_SPRINT( 3,(*rd_cnt)++,buf,\
				"[08]%7d [09]%7d [10]%7d [11]%7d [12]%7d [13]%7d [14]%7d [15]%7d\n",\
				data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15]);
			PE_HST_H15_CASE_SPRINT( 4,(*rd_cnt)++,buf,\
				"[16]%7d [17]%7d [18]%7d [19]%7d [20]%7d [21]%7d [22]%7d [23]%7d\n",\
				data[16],data[17],data[18],data[19],data[20],data[21],data[22],data[23]);
			PE_HST_H15_CASE_SPRINT( 5,(*rd_cnt)++,buf,\
				"[24]%7d [25]%7d [26]%7d [27]%7d [28]%7d [29]%7d [30]%7d [31]%7d\n",\
				data[24],data[25],data[26],data[27],data[28],data[29],data[30],data[31]);
			PE_HST_H15_DFLT_SPRINT((*rd_cnt)=0,buf,"%s",PE_HST_H15_PRINT_END);
		}
	}while(0);
	return;
}
/**
 * set lrcr histo print buffer
 *
 * @param   *str [in] char
 * @param   *rd_cnt [in] UINT32
 * @param   *buf [in] char
 * @param   *data [in] UINT32
 * @return  void
 * @see
 * @author
 */
static void PE_HST_HW_H15_SetLrcHstPrintBuf(char *str, UINT32 *rd_cnt, char *buf, UINT32 *data)
{
	int ret = RET_OK;

#define PE_HST_H15_PRINT_START		"START_OF_PRINT"
#define PE_HST_H15_PRINT_END		"END_OF_PRINT"
#define PE_HST_H15_CASE_SPRINT(_cnt,_action,_buf,fmt,args...)	\
	case (_cnt):sprintf(_buf,fmt,##args);_action;break
#define PE_HST_H15_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(str);
		CHECK_KNULL(rd_cnt);
		CHECK_KNULL(buf);
		CHECK_KNULL(data);
		switch(*rd_cnt)
		{
			PE_HST_H15_CASE_SPRINT( 0,(*rd_cnt)++,buf,"%s",PE_HST_H15_PRINT_START);
			PE_HST_H15_CASE_SPRINT( 1,(*rd_cnt)++,buf,"lrc hst %s\n",str);
			PE_HST_H15_CASE_SPRINT( 2,(*rd_cnt)++,buf,\
				"[00]%7d [01]%7d [02]%7d [03]%7d [04]%7d [05]%7d [06]%7d [07]%7d\n",\
				data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7]);
			PE_HST_H15_CASE_SPRINT( 3,(*rd_cnt)++,buf,\
				"[08]%7d [09]%7d [10]%7d [11]%7d [12]%7d [13]%7d [14]%7d [15]%7d\n",\
				data[8],data[9],data[10],data[11],data[12],data[13],data[14],data[15]);
			PE_HST_H15_CASE_SPRINT( 4,(*rd_cnt)++,buf,\
				"[16]%7d [17]%7d [18]%7d [19]%7d [20]%7d [21]%7d [22]%7d [23]%7d\n",\
				data[16],data[17],data[18],data[19],data[20],data[21],data[22],data[23]);
			PE_HST_H15_CASE_SPRINT( 5,(*rd_cnt)++,buf,\
				"[24]%7d [25]%7d [26]%7d [27]%7d [28]%7d [29]%7d [30]%7d [31]%7d\n",\
				data[24],data[25],data[26],data[27],data[28],data[29],data[30],data[31]);
			PE_HST_H15_DFLT_SPRINT((*rd_cnt)=0,buf,"%s",PE_HST_H15_PRINT_END);
		}
	}while(0);
	return;
}
/**
 * get global histogram info state
 *
 * @param   *pstParams [in] LX_PE_ETC_TBL_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_GetGlobalHistInfo(LX_PE_ETC_TBL_T *pstParams)
{
    int ret = RET_OK;
	char buffer[PE_TRACE_STR_SIZE];
	static UINT32 rd_cnt=0;
	PE_HST_HW_H15_SRC_APL_T *src_apl = &_g_pe_hst_hw_h15_info.src_apl;
	PE_HST_HW_H15_SRC_HST_T *src_hst = &_g_pe_hst_hw_h15_info.src_hist;
	PE_HST_HW_H15_LRC_HST_T *lrc_hst = &_g_pe_hst_hw_h15_info.lrc_hist;
	PE_HST_HW_H15_INFO_T *hst_inf = &_g_pe_hst_hw_h15_info.hist_inf;
	#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
	UINT32 histo_bin[PE_HST_HW_H15_HST_BIN_NUM];
	#endif

#define WIN0	LX_PE_WIN_0
#define WIN1	LX_PE_WIN_1
#define PE_HST_H15_PRINT_START		"START_OF_PRINT"
#define PE_HST_H15_PRINT_RESERVED	"RESERVED"
#define PE_HST_H15_PRINT_END		"END_OF_PRINT"
#define PE_HST_H15_PRINT_EXIT		"EXIT_OF_PRINT"
#define PE_HST_H15_CASE_PRMENU(_cnt,_action,_buf,_item)	\
	case (_cnt):sprintf(_buf,"   [0x%02x] %s\n",(_item),#_item);_action;break
#define PE_HST_H15_CASE_SPRINT(_cnt,_action,_buf,fmt,args...)	\
	case (_cnt):sprintf(_buf,fmt,##args);_action;break
#define PE_HST_H15_DFLT_SPRINT(_action,_buf,fmt,args...)		\
	default:sprintf(_buf,fmt,##args);_action;break

	do{
		CHECK_KNULL(pstParams);
		CHECK_KNULL(pstParams->data);
		if(PE_KDRV_VER_H15)
		{
			if(pstParams->data[0]>=PE_HST_HW_H15_MENU_MAX)
			{
				sprintf(buffer,"%s",PE_HST_H15_PRINT_EXIT);rd_cnt=0;
			}
			else if(pstParams->data[0]==PE_HST_HW_H15_MENU_DISPLAY)
			{
				switch(rd_cnt)
				{
					PE_HST_H15_CASE_SPRINT( 0,rd_cnt++,buffer,"%s",PE_HST_H15_PRINT_START);
					PE_HST_H15_CASE_PRMENU( 1,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_APL_INF);
					PE_HST_H15_CASE_PRMENU( 2,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_INF);
					PE_HST_H15_CASE_PRMENU( 3,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_HST_INF);
					PE_HST_H15_CASE_PRMENU( 4,rd_cnt++,buffer,PE_HST_HW_H15_MENU_CMN_HST_INF);
					PE_HST_H15_CASE_PRMENU( 5,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_Y_0);
					PE_HST_H15_CASE_PRMENU( 6,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_Y_1);
					PE_HST_H15_CASE_PRMENU( 7,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_CB0);
					PE_HST_H15_CASE_PRMENU( 8,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_CB1);
					PE_HST_H15_CASE_PRMENU( 9,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_CR0);
					PE_HST_H15_CASE_PRMENU(10,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_CR1);
					PE_HST_H15_CASE_PRMENU(11,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_G_0);
					PE_HST_H15_CASE_PRMENU(12,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_G_1);
					PE_HST_H15_CASE_PRMENU(13,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_B_0);
					PE_HST_H15_CASE_PRMENU(14,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_B_1);
					PE_HST_H15_CASE_PRMENU(15,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_R_0);
					PE_HST_H15_CASE_PRMENU(16,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_R_1);
					PE_HST_H15_CASE_PRMENU(17,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_H_0);
					PE_HST_H15_CASE_PRMENU(18,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_H_1);
					PE_HST_H15_CASE_PRMENU(19,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_S_0);
					PE_HST_H15_CASE_PRMENU(20,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_S_1);
					PE_HST_H15_CASE_PRMENU(21,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_V_0);
					PE_HST_H15_CASE_PRMENU(22,rd_cnt++,buffer,PE_HST_HW_H15_MENU_SRC_HST_V_1);
					PE_HST_H15_CASE_PRMENU(23,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB0_T_G);
					PE_HST_H15_CASE_PRMENU(24,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB0_B_G);
					PE_HST_H15_CASE_PRMENU(25,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB0_T_B);
					PE_HST_H15_CASE_PRMENU(26,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB0_B_B);
					PE_HST_H15_CASE_PRMENU(27,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB0_T_R);
					PE_HST_H15_CASE_PRMENU(28,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB0_B_R);
					PE_HST_H15_CASE_PRMENU(29,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS0_L_G);
					PE_HST_H15_CASE_PRMENU(30,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS0_R_G);
					PE_HST_H15_CASE_PRMENU(31,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS0_L_B);
					PE_HST_H15_CASE_PRMENU(32,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS0_R_B);
					PE_HST_H15_CASE_PRMENU(33,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS0_L_R);
					PE_HST_H15_CASE_PRMENU(34,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS0_R_R);
					PE_HST_H15_CASE_PRMENU(35,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB1_T_G);
					PE_HST_H15_CASE_PRMENU(36,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB1_B_G);
					PE_HST_H15_CASE_PRMENU(37,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB1_T_B);
					PE_HST_H15_CASE_PRMENU(38,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB1_B_B);
					PE_HST_H15_CASE_PRMENU(39,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB1_T_R);
					PE_HST_H15_CASE_PRMENU(40,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_TB1_B_R);
					PE_HST_H15_CASE_PRMENU(41,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS1_L_G);
					PE_HST_H15_CASE_PRMENU(42,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS1_R_G);
					PE_HST_H15_CASE_PRMENU(43,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS1_L_B);
					PE_HST_H15_CASE_PRMENU(44,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS1_R_B);
					PE_HST_H15_CASE_PRMENU(45,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS1_L_R);
					PE_HST_H15_CASE_PRMENU(46,rd_cnt++,buffer,PE_HST_HW_H15_MENU_LRC_SS1_R_R);
					PE_HST_H15_DFLT_SPRINT(rd_cnt=0,buffer,"%s",PE_HST_H15_PRINT_END);
				}
			}
			else if(pstParams->data[0]==PE_HST_HW_H15_MENU_SRC_APL_INF)
			{
				switch(rd_cnt)
				{
					PE_HST_H15_CASE_SPRINT( 0,rd_cnt++,buffer,"%s",PE_HST_H15_PRINT_START);
					PE_HST_H15_CASE_SPRINT( 1,rd_cnt++,buffer,\
						"src_apl : state curr[%2d], next[%2d]\n",\
						src_apl->state.curr,src_apl->state.next);
					PE_HST_H15_CASE_SPRINT( 2,rd_cnt++,buffer,\
						"src_apl : sel[%2d], h[%4d][%4d], v[%4d][%4d], mux[%2d]\n",\
						src_apl->ctrl.apl_sel,src_apl->ctrl.h_size[WIN0],src_apl->ctrl.h_size[WIN1],\
						src_apl->ctrl.v_size[WIN0],src_apl->ctrl.v_size[WIN1],src_apl->ctrl.mux_sel);
					PE_HST_H15_CASE_SPRINT( 3,rd_cnt++,buffer,\
						"src_apl : yuv w0[%4d,%4d,%4d] w1[%4d,%4d,%4d]\n",\
						GET_BITS(src_apl->data.yuv[WIN0],0,10),GET_BITS(src_apl->data.yuv[WIN0],10,10),\
						GET_BITS(src_apl->data.yuv[WIN0],20,10),GET_BITS(src_apl->data.yuv[WIN1],0,10),\
						GET_BITS(src_apl->data.yuv[WIN1],10,10),GET_BITS(src_apl->data.yuv[WIN1],20,10));
					PE_HST_H15_CASE_SPRINT( 4,rd_cnt++,buffer,\
						"src_apl : hsv w0[%4d,%4d,%4d] w1[%4d,%4d,%4d]\n",\
						GET_BITS(src_apl->data.hsv[WIN0],0,10),GET_BITS(src_apl->data.hsv[WIN0],10,10),\
						GET_BITS(src_apl->data.hsv[WIN0],20,10),GET_BITS(src_apl->data.hsv[WIN1],0,10),\
						GET_BITS(src_apl->data.hsv[WIN1],10,10),GET_BITS(src_apl->data.hsv[WIN1],20,10));
					PE_HST_H15_CASE_SPRINT( 5,rd_cnt++,buffer,\
						"src_apl : gbr w0[%4d,%4d,%4d] w1[%4d,%4d,%4d]\n",\
						GET_BITS(src_apl->data.gbr[WIN0],0,10),GET_BITS(src_apl->data.gbr[WIN0],10,10),\
						GET_BITS(src_apl->data.gbr[WIN0],20,10),GET_BITS(src_apl->data.gbr[WIN1],0,10),\
						GET_BITS(src_apl->data.gbr[WIN1],10,10),GET_BITS(src_apl->data.gbr[WIN1],20,10));
					PE_HST_H15_CASE_SPRINT( 6,rd_cnt++,buffer,\
						"src_apl : ysv w0[%4d,%4d,%4d] w1[%4d,%4d,%4d]\n",\
						GET_BITS(src_apl->data.ysv[WIN0],0,10),GET_BITS(src_apl->data.ysv[WIN0],10,10),\
						GET_BITS(src_apl->data.ysv[WIN0],20,10),GET_BITS(src_apl->data.ysv[WIN1],0,10),\
						GET_BITS(src_apl->data.ysv[WIN1],10,10),GET_BITS(src_apl->data.ysv[WIN1],20,10));
					PE_HST_H15_CASE_SPRINT( 7,rd_cnt++,buffer,\
						"src_bbd : xy0 w0[%4d,%4d] w1[%4d,%4d]\n",\
						GET_BITS(src_apl->data.bb0[WIN0],0,12),GET_BITS(src_apl->data.bb0[WIN0],16,12),\
						GET_BITS(src_apl->data.bb0[WIN1],0,12),GET_BITS(src_apl->data.bb0[WIN1],16,12));
					PE_HST_H15_CASE_SPRINT( 8,rd_cnt++,buffer,\
						"src_bbd : xy1 w0[%4d,%4d] w1[%4d,%4d]\n",\
						GET_BITS(src_apl->data.bb1[WIN0],0,12),GET_BITS(src_apl->data.bb1[WIN0],16,12),\
						GET_BITS(src_apl->data.bb1[WIN1],0,12),GET_BITS(src_apl->data.bb1[WIN1],16,12));
					PE_HST_H15_DFLT_SPRINT(rd_cnt=0,buffer,"%s",PE_HST_H15_PRINT_END);
				}
			}
			else if(pstParams->data[0]==PE_HST_HW_H15_MENU_SRC_HST_INF)
			{
				switch(rd_cnt)
				{
					PE_HST_H15_CASE_SPRINT( 0,rd_cnt++,buffer,"%s",PE_HST_H15_PRINT_START);
					PE_HST_H15_CASE_SPRINT( 1,rd_cnt++,buffer,\
						"src_hst : state cur[%2d],next[%2d], out[%2d],in_fr[%3d],mod[%2d]\n",\
						src_hst->state.curr,src_hst->state.next,src_hst->ctrl.out_type,\
						src_hst->ctrl.in_f_rate,src_hst->ctrl.mod_sel);
					PE_HST_H15_CASE_SPRINT( 2,rd_cnt++,buffer,\
						"src_hst : mux_sel[%2d,%2d,%2d][%2d,%2d,%2d]\n",\
						src_hst->ctrl.mux_sel[WIN0][0],src_hst->ctrl.mux_sel[WIN0][1],\
						src_hst->ctrl.mux_sel[WIN0][2],src_hst->ctrl.mux_sel[WIN1][0],\
						src_hst->ctrl.mux_sel[WIN1][1],src_hst->ctrl.mux_sel[WIN1][2]);
					PE_HST_H15_CASE_SPRINT( 3,rd_cnt++,buffer,\
						"src_hst : win_sel[%2d,%2d,%2d][%2d,%2d,%2d]\n",\
						src_hst->ctrl.win_sel[WIN0][0],src_hst->ctrl.win_sel[WIN0][1],\
						src_hst->ctrl.win_sel[WIN0][2],src_hst->ctrl.win_sel[WIN1][0],\
						src_hst->ctrl.win_sel[WIN1][1],src_hst->ctrl.win_sel[WIN1][2]);
					PE_HST_H15_CASE_SPRINT( 4,rd_cnt++,buffer,\
						"src_hst : x[%4d][%4d],y[%4d][%4d],h[%4d][%4d],v[%4d][%4d]\n",\
						src_hst->ctrl.x_ofst[WIN0],src_hst->ctrl.x_ofst[WIN1],\
						src_hst->ctrl.y_ofst[WIN0],src_hst->ctrl.y_ofst[WIN1],\
						src_hst->ctrl.h_size[WIN0],src_hst->ctrl.h_size[WIN1],\
						src_hst->ctrl.v_size[WIN0],src_hst->ctrl.v_size[WIN1]);
					PE_HST_H15_CASE_SPRINT( 5,rd_cnt++,buffer,\
						"src_hst : x0[%4d,%4d][%4d,%4d],y0[%4d,%4d][%4d,%4d]\n",\
						src_hst->ctrl.x0_pos[WIN0][0],src_hst->ctrl.x0_pos[WIN0][1],\
						src_hst->ctrl.x0_pos[WIN1][0],src_hst->ctrl.x0_pos[WIN1][1],\
						src_hst->ctrl.y0_pos[WIN0][0],src_hst->ctrl.y0_pos[WIN0][1],\
						src_hst->ctrl.y0_pos[WIN1][0],src_hst->ctrl.y0_pos[WIN1][1]);
					PE_HST_H15_CASE_SPRINT( 6,rd_cnt++,buffer,\
						"src_hst : x1[%4d,%4d][%4d,%4d],y1[%4d,%4d][%4d,%4d]\n",\
						src_hst->ctrl.x1_pos[WIN0][0],src_hst->ctrl.x1_pos[WIN0][1],\
						src_hst->ctrl.x1_pos[WIN1][0],src_hst->ctrl.x1_pos[WIN1][1],\
						src_hst->ctrl.y1_pos[WIN0][0],src_hst->ctrl.y1_pos[WIN0][1],\
						src_hst->ctrl.y1_pos[WIN1][0],src_hst->ctrl.y1_pos[WIN1][1]);
					PE_HST_H15_CASE_SPRINT( 7,rd_cnt++,buffer,\
						"src_hst : pxl_cnt[%07d, %07d], s_state[%d,%d]\n",\
						src_hst->data.pxl_cnt[WIN0],src_hst->data.pxl_cnt[WIN1],\
						src_hst->data.s_state[WIN0],src_hst->data.s_state[WIN1]);
					PE_HST_H15_CASE_SPRINT( 8,rd_cnt++,buffer,\
						"src_hst : pre_h_bin[0x%08x,0x%08x], cur_h_bin[0x%08x,0x%08x]\n",\
						src_hst->data.pre_h_bin[WIN0], src_hst->data.pre_h_bin[WIN1], \
						src_hst->data.cur_h_bin[WIN0], src_hst->data.cur_h_bin[WIN1]);
					PE_HST_H15_DFLT_SPRINT(rd_cnt=0,buffer,"%s",PE_HST_H15_PRINT_END);
				}
			}
			else if(pstParams->data[0]==PE_HST_HW_H15_MENU_LRC_HST_INF)
			{
				switch(rd_cnt)
				{
					PE_HST_H15_CASE_SPRINT( 0,rd_cnt++,buffer,"%s",PE_HST_H15_PRINT_START);
					PE_HST_H15_CASE_SPRINT( 1,rd_cnt++,buffer,\
						"lrc hst : state cur[%2d], next[%2d]\n",\
						lrc_hst->state.curr,lrc_hst->state.next);
					PE_HST_H15_CASE_SPRINT( 2,rd_cnt++,buffer,\
						"lrc hst : out[%d],hst_sel[%d],win_mod[%d]\n",\
						lrc_hst->ctrl.out_type,lrc_hst->ctrl.hst_sel,lrc_hst->ctrl.win_mod);
					PE_HST_H15_CASE_SPRINT( 3,rd_cnt++,buffer,\
						"lrc hst : tb0_t pos[%4d,%4d,%4d,%4d],b[%4d,%4d,%4d,%4d]\n",\
						lrc_hst->ctrl.tb0_t_pos[0],lrc_hst->ctrl.tb0_t_pos[1],\
						lrc_hst->ctrl.tb0_t_pos[2],lrc_hst->ctrl.tb0_t_pos[3],\
						lrc_hst->ctrl.tb0_b_pos[0],lrc_hst->ctrl.tb0_b_pos[1],\
						lrc_hst->ctrl.tb0_b_pos[2],lrc_hst->ctrl.tb0_b_pos[3]);
					PE_HST_H15_CASE_SPRINT( 4,rd_cnt++,buffer,\
						"lrc hst : ss0_l pos[%4d,%4d,%4d,%4d],r[%4d,%4d,%4d,%4d]\n",\
						lrc_hst->ctrl.ss0_l_pos[0],lrc_hst->ctrl.ss0_l_pos[1],\
						lrc_hst->ctrl.ss0_l_pos[2],lrc_hst->ctrl.ss0_l_pos[3],\
						lrc_hst->ctrl.ss0_r_pos[0],lrc_hst->ctrl.ss0_r_pos[1],\
						lrc_hst->ctrl.ss0_r_pos[2],lrc_hst->ctrl.ss0_r_pos[3]);
					PE_HST_H15_CASE_SPRINT( 5,rd_cnt++,buffer,\
						"lrc hst : tb1_t pos[%4d,%4d,%4d,%4d],b[%4d,%4d,%4d,%4d]\n",\
						lrc_hst->ctrl.tb1_t_pos[0],lrc_hst->ctrl.tb1_t_pos[1],\
						lrc_hst->ctrl.tb1_t_pos[2],lrc_hst->ctrl.tb1_t_pos[3],\
						lrc_hst->ctrl.tb1_b_pos[0],lrc_hst->ctrl.tb1_b_pos[1],\
						lrc_hst->ctrl.tb1_b_pos[2],lrc_hst->ctrl.tb1_b_pos[3]);
					PE_HST_H15_CASE_SPRINT( 6,rd_cnt++,buffer,\
						"lrc hst : ss1_l pos[%4d,%4d,%4d,%4d],r[%4d,%4d,%4d,%4d]\n",\
						lrc_hst->ctrl.ss1_l_pos[0],lrc_hst->ctrl.ss1_l_pos[1],\
						lrc_hst->ctrl.ss1_l_pos[2],lrc_hst->ctrl.ss1_l_pos[3],\
						lrc_hst->ctrl.ss1_r_pos[0],lrc_hst->ctrl.ss1_r_pos[1],\
						lrc_hst->ctrl.ss1_r_pos[2],lrc_hst->ctrl.ss1_r_pos[3]);
					PE_HST_H15_DFLT_SPRINT(rd_cnt=0,buffer,"%s",PE_HST_H15_PRINT_END);
				}
			}
			else if(pstParams->data[0]==PE_HST_HW_H15_MENU_CMN_HST_INF)
			{
				switch(rd_cnt)
				{
					PE_HST_H15_CASE_SPRINT( 0,rd_cnt++,buffer,"%s",PE_HST_H15_PRINT_START);
					PE_HST_H15_CASE_SPRINT( 1,rd_cnt++,buffer,\
						"hst inf : det 3df:(0~6:2d,cb,cc,ll,ss,tb,unk, 7:max)\n");
					PE_HST_H15_CASE_SPRINT( 2,rd_cnt++,buffer,\
						"hst inf : det 3df[%d],src_type[%d],out_type[%d]\n",hst_inf->det_3df,\
						hst_inf->dis_inf.src_type,hst_inf->dis_inf.out_type);
					PE_HST_H15_CASE_SPRINT( 3,rd_cnt++,buffer,\
						"hst inf : lrc dif_max[%7d,%7d,%7d,%7d]\n",\
						hst_inf->lrc_inf.dif_max[0],hst_inf->lrc_inf.dif_max[1],\
						hst_inf->lrc_inf.dif_max[2],hst_inf->lrc_inf.dif_max[3]);
					PE_HST_H15_CASE_SPRINT( 4,rd_cnt++,buffer,\
						"hst inf : lrc pxl_cnt[%7d,%7d,%7d,%7d]\n",\
						hst_inf->lrc_inf.pxl_cnt[0],hst_inf->lrc_inf.pxl_cnt[1],\
						hst_inf->lrc_inf.pxl_cnt[2],hst_inf->lrc_inf.pxl_cnt[3]);
					PE_HST_H15_CASE_SPRINT( 5,rd_cnt++,buffer,\
						"hst inf : lrc set_cnt[%7d,%7d,%7d,%7d]\n",\
						hst_inf->lrc_inf.set_cnt[0],hst_inf->lrc_inf.set_cnt[1],\
						hst_inf->lrc_inf.set_cnt[2],hst_inf->lrc_inf.set_cnt[3]);
					PE_HST_H15_CASE_SPRINT( 6,rd_cnt++,buffer,\
						"hst inf : src s_state[%3d,%3d]\n",\
						hst_inf->src_inf.s_state[WIN0],hst_inf->src_inf.s_state[WIN1]);
					PE_HST_H15_CASE_SPRINT( 7,rd_cnt++,buffer,\
						"hst inf : src dif_cnt y[%7d,%7d],cb[%7d,%7d],cr[%7d,%7d]\n",\
						hst_inf->src_inf.dif_cnt[0],hst_inf->src_inf.dif_cnt[1],\
						hst_inf->src_inf.dif_cnt[2],hst_inf->src_inf.dif_cnt[3],\
						hst_inf->src_inf.dif_cnt[4],hst_inf->src_inf.dif_cnt[5]);
					PE_HST_H15_CASE_SPRINT( 8,rd_cnt++,buffer,\
						"hst inf : src dif_cnt g[%7d,%7d], b[%7d,%7d], r[%7d,%7d]\n",\
						hst_inf->src_inf.dif_cnt[6],hst_inf->src_inf.dif_cnt[7],\
						hst_inf->src_inf.dif_cnt[8],hst_inf->src_inf.dif_cnt[9],\
						hst_inf->src_inf.dif_cnt[10],hst_inf->src_inf.dif_cnt[11]);
					PE_HST_H15_CASE_SPRINT( 9,rd_cnt++,buffer,\
						"hst inf : src dif_cnt h[%7d,%7d], s[%7d,%7d], v[%7d,%7d]\n",\
						hst_inf->src_inf.dif_cnt[12],hst_inf->src_inf.dif_cnt[13],\
						hst_inf->src_inf.dif_cnt[14],hst_inf->src_inf.dif_cnt[15],\
						hst_inf->src_inf.dif_cnt[16],hst_inf->src_inf.dif_cnt[17]);
					PE_HST_H15_CASE_SPRINT(10,rd_cnt++,buffer,\
						"hst inf : src pxl_cnt y[%7d,%7d],cb[%7d,%7d],cr[%7d,%7d]\n",\
						hst_inf->src_inf.pxl_cnt[0],hst_inf->src_inf.pxl_cnt[1],\
						hst_inf->src_inf.pxl_cnt[2],hst_inf->src_inf.pxl_cnt[3],\
						hst_inf->src_inf.pxl_cnt[4],hst_inf->src_inf.pxl_cnt[5]);
					PE_HST_H15_CASE_SPRINT(11,rd_cnt++,buffer,\
						"hst inf : src pxl_cnt g[%7d,%7d], b[%7d,%7d], r[%7d,%7d]\n",\
						hst_inf->src_inf.pxl_cnt[6],hst_inf->src_inf.pxl_cnt[7],\
						hst_inf->src_inf.pxl_cnt[8],hst_inf->src_inf.pxl_cnt[9],\
						hst_inf->src_inf.pxl_cnt[10],hst_inf->src_inf.pxl_cnt[11]);
					PE_HST_H15_CASE_SPRINT(12,rd_cnt++,buffer,\
						"hst inf : src pxl_cnt h[%7d,%7d], s[%7d,%7d], v[%7d,%7d]\n",\
						hst_inf->src_inf.pxl_cnt[12],hst_inf->src_inf.pxl_cnt[13],\
						hst_inf->src_inf.pxl_cnt[14],hst_inf->src_inf.pxl_cnt[15],\
						hst_inf->src_inf.pxl_cnt[16],hst_inf->src_inf.pxl_cnt[17]);
					PE_HST_H15_CASE_SPRINT(13,rd_cnt++,buffer,\
						"hst inf : src set_cnt y[%7d,%7d],cb[%7d,%7d],cr[%7d,%7d]\n",\
						hst_inf->src_inf.set_cnt[0],hst_inf->src_inf.set_cnt[1],\
						hst_inf->src_inf.set_cnt[2],hst_inf->src_inf.set_cnt[3],\
						hst_inf->src_inf.set_cnt[4],hst_inf->src_inf.set_cnt[5]);
					PE_HST_H15_CASE_SPRINT(14,rd_cnt++,buffer,\
						"hst inf : src set_cnt g[%7d,%7d], b[%7d,%7d], r[%7d,%7d]\n",\
						hst_inf->src_inf.set_cnt[6],hst_inf->src_inf.set_cnt[7],\
						hst_inf->src_inf.set_cnt[8],hst_inf->src_inf.set_cnt[9],\
						hst_inf->src_inf.set_cnt[10],hst_inf->src_inf.set_cnt[11]);
					PE_HST_H15_CASE_SPRINT(15,rd_cnt++,buffer,\
						"hst inf : src set_cnt y[%7d,%7d],cb[%7d,%7d],cr[%7d,%7d]\n",\
						hst_inf->src_inf.set_cnt[12],hst_inf->src_inf.set_cnt[13],\
						hst_inf->src_inf.set_cnt[14],hst_inf->src_inf.set_cnt[15],\
						hst_inf->src_inf.set_cnt[16],hst_inf->src_inf.set_cnt[17]);
					PE_HST_H15_DFLT_SPRINT(rd_cnt=0,buffer,"%s",PE_HST_H15_PRINT_END);
				}
			}
			else
			{
				switch(pstParams->data[0])
				{
					case PE_HST_HW_H15_MENU_SRC_HST_Y_0:
						#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
						PE_HST_HW_H15_GetSrcHistoBin(0, histo_bin, PE_HST_HW_H15_SRC_HST_SEL_Y);
						#endif
						PE_HST_HW_H15_SetSrcHstPrintBuf("y0",&rd_cnt,buffer,&(src_hst->data.y0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_Y_1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("y1",&rd_cnt,buffer,&(src_hst->data.y1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_CB0:
						PE_HST_HW_H15_SetSrcHstPrintBuf("cb0",&rd_cnt,buffer,&(src_hst->data.cb0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_CB1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("cb1",&rd_cnt,buffer,&(src_hst->data.cb1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_CR0:
						PE_HST_HW_H15_SetSrcHstPrintBuf("cr0",&rd_cnt,buffer,&(src_hst->data.cr0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_CR1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("cr1",&rd_cnt,buffer,&(src_hst->data.cr1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_G_0:
						PE_HST_HW_H15_SetSrcHstPrintBuf("g0",&rd_cnt,buffer,&(src_hst->data.g0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_G_1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("g1",&rd_cnt,buffer,&(src_hst->data.g1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_B_0:
						PE_HST_HW_H15_SetSrcHstPrintBuf("b0",&rd_cnt,buffer,&(src_hst->data.b0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_B_1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("b1",&rd_cnt,buffer,&(src_hst->data.b1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_R_0:
						PE_HST_HW_H15_SetSrcHstPrintBuf("r0",&rd_cnt,buffer,&(src_hst->data.r0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_R_1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("r1",&rd_cnt,buffer,&(src_hst->data.r1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_H_0:
						#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
						PE_HST_HW_H15_GetSrcHistoBin(0, histo_bin, PE_HST_HW_H15_SRC_HST_SEL_H);
						#endif
						PE_HST_HW_H15_SetSrcHstPrintBuf("h0",&rd_cnt,buffer,&(src_hst->data.h0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_H_1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("h1",&rd_cnt,buffer,&(src_hst->data.h1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_S_0:
						#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
						PE_HST_HW_H15_GetSrcHistoBin(0, histo_bin, PE_HST_HW_H15_SRC_HST_SEL_S);
						#endif
						PE_HST_HW_H15_SetSrcHstPrintBuf("s0",&rd_cnt,buffer,&(src_hst->data.s0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_S_1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("s1",&rd_cnt,buffer,&(src_hst->data.s1[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_V_0:
						PE_HST_HW_H15_SetSrcHstPrintBuf("v0",&rd_cnt,buffer,&(src_hst->data.v0[0]));
						break;
					case PE_HST_HW_H15_MENU_SRC_HST_V_1:
						PE_HST_HW_H15_SetSrcHstPrintBuf("v1",&rd_cnt,buffer,&(src_hst->data.v1[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB0_T_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb0_t_grn",&rd_cnt,buffer,&(lrc_hst->data.tb0_t_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB0_B_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb0_b_grn",&rd_cnt,buffer,&(lrc_hst->data.tb0_b_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB0_T_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb0_t_blu",&rd_cnt,buffer,&(lrc_hst->data.tb0_t_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB0_B_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb0_b_blu",&rd_cnt,buffer,&(lrc_hst->data.tb0_b_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB0_T_R:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb0_t_red",&rd_cnt,buffer,&(lrc_hst->data.tb0_t_red[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB0_B_R:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb0_b_red",&rd_cnt,buffer,&(lrc_hst->data.tb0_b_red[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS0_L_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss0_l_grn",&rd_cnt,buffer,&(lrc_hst->data.ss0_l_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS0_R_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss0_r_grn",&rd_cnt,buffer,&(lrc_hst->data.ss0_r_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS0_L_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss0_l_blu",&rd_cnt,buffer,&(lrc_hst->data.ss0_l_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS0_R_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss0_r_blu",&rd_cnt,buffer,&(lrc_hst->data.ss0_r_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS0_L_R:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss0_l_red",&rd_cnt,buffer,&(lrc_hst->data.ss0_l_red[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS0_R_R:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss0_r_red",&rd_cnt,buffer,&(lrc_hst->data.ss0_r_red[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB1_T_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb1_t_grn",&rd_cnt,buffer,&(lrc_hst->data.tb1_t_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB1_B_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb1_b_grn",&rd_cnt,buffer,&(lrc_hst->data.tb1_b_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB1_T_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb1_t_blu",&rd_cnt,buffer,&(lrc_hst->data.tb1_t_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB1_B_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb1_b_blu",&rd_cnt,buffer,&(lrc_hst->data.tb1_b_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB1_T_R:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb1_t_red",&rd_cnt,buffer,&(lrc_hst->data.tb1_t_red[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_TB1_B_R:
						PE_HST_HW_H15_SetLrcHstPrintBuf("tb1_b_red",&rd_cnt,buffer,&(lrc_hst->data.tb1_b_red[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS1_L_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss1_l_grn",&rd_cnt,buffer,&(lrc_hst->data.ss1_l_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS1_R_G:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss1_r_grn",&rd_cnt,buffer,&(lrc_hst->data.ss1_r_grn[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS1_L_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss1_l_blu",&rd_cnt,buffer,&(lrc_hst->data.ss1_l_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS1_R_B:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss1_r_blu",&rd_cnt,buffer,&(lrc_hst->data.ss1_r_blu[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS1_L_R:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss1_l_red",&rd_cnt,buffer,&(lrc_hst->data.ss1_l_red[0]));
						break;
					case PE_HST_HW_H15_MENU_LRC_SS1_R_R:
					default:
						PE_HST_HW_H15_SetLrcHstPrintBuf("ss1_r_red",&rd_cnt,buffer,&(lrc_hst->data.ss1_r_red[0]));
						break;
				}
			}
			memcpy(pstParams->data,buffer,sizeof(char)*PE_TRACE_STR_SIZE);
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * update histo display info
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
static int PE_HST_HW_H15_UpdateHistoDispInfo(PE_TSK_H15_CFG_T *pCfg)
{
	int ret = RET_OK;
	LX_PE_INF_DISPLAY_T *disp0_info = &pCfg->inf.disp_info[LX_PE_WIN_0];
	PE_HST_HW_H15_DIS_INFO_T *hist_info = &_g_pe_hst_hw_h15_info.hist_inf.dis_inf;
	do {
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			hist_info->src_type = disp0_info->src_type;
			hist_info->out_type = disp0_info->out_type;
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * check global histo info
 *	- src apl, src histo, lrcr histo
 *
 * @param   *pCfg [in] PE_TSK_H15_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_CheckGlobalHistoInfo(PE_TSK_H15_CFG_T *pCfg)
{
    int ret = RET_OK;
	do{
		CHECK_KNULL(pCfg);
		if(PE_KDRV_VER_H15)
		{
			ret = PE_HST_HW_H15_UpdateHistoDispInfo(pCfg);
			PE_HST_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_HST_HW_H15_UpdateHistoDispInfo() error.\n",__F__,__L__);
			ret = PE_HST_HW_H15_CheckSrcAplInfo(pCfg);
			PE_HST_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_HST_HW_H15_CheckSrcAplInfo() error.\n",__F__,__L__);
			ret = PE_HST_HW_H15_CheckSrcHistoInfo(pCfg);
			PE_HST_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_HST_HW_H15_CheckSrcHistoInfo() error.\n",__F__,__L__);
			ret = PE_HST_HW_H15_CheckLrcrHistoInfo(pCfg);
			PE_HST_HW_H15_CHECK_CODE(ret,break,"[%s,%d] PE_HST_HW_H15_CheckLrcrHistoInfo() error.\n",__F__,__L__);
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do.\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * set histo config
 *
 * @param   *pstParams [in] LX_PE_HST_HISTO_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_SetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams)
{
	int ret = RET_OK;
	PE_HST_HW_H15_SETTINGS_T *pInfo=&_g_pe_hst_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		PE_HST_HW_H15_DBG_PRINT("set pstParams[%d] : op:src_apl:%d,src_hist:%d,lrc_hist:%d\n",\
			pstParams->win_id,pstParams->opr.src_apl_op,pstParams->opr.src_hist_op,pstParams->opr.lrc_hist_op);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(pstParams->win_id))
			{
				memcpy(&(pInfo->cfg[LX_PE_WIN_0]),pstParams,sizeof(LX_PE_HST_HISTO_CFG_T));
			}
			if(PE_CHECK_WIN1(pstParams->win_id))
			{
				memcpy(&(pInfo->cfg[LX_PE_WIN_1]),pstParams,sizeof(LX_PE_HST_HISTO_CFG_T));
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get histo config
 *
 * @param   *pstParams [in] LX_PE_HST_HISTO_CFG_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_GetHistoConfig(LX_PE_HST_HISTO_CFG_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	PE_HST_HW_H15_SETTINGS_T *pInfo=&_g_pe_hst_hw_h15_info;
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				pInfo->cfg[LX_PE_WIN_0].win_id=pstParams->win_id;
				memcpy(pstParams,&(pInfo->cfg[LX_PE_WIN_0]),sizeof(LX_PE_HST_HISTO_CFG_T));
			}
			if(PE_CHECK_WIN1(win_id))
			{
				pInfo->cfg[LX_PE_WIN_1].win_id=pstParams->win_id;
				memcpy(pstParams,&(pInfo->cfg[LX_PE_WIN_1]),sizeof(LX_PE_HST_HISTO_CFG_T));
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
		PE_HST_HW_H15_DBG_PRINT("get pstParams[%d] : op:src_apl:%d,src_hist:%d,lrc_hist:%d\n",\
			pstParams->win_id,pstParams->opr.src_apl_op,pstParams->opr.src_hist_op,pstParams->opr.lrc_hist_op);
	}while(0);
	return ret;
}
/**
 * update detected 3d format and get histo info
 *
 * @param   *pstParams [in/out] PE_HST_HW_H15_INFO_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_GetHistoInfo(PE_HST_HW_H15_INFO_T *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_HST_HW_H15_INFO_T *pInfo = &_g_pe_hst_hw_h15_info.hist_inf;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			pInfo->det_3df = pstParams->det_3df;
			memcpy(pstParams,pInfo,sizeof(PE_HST_HW_H15_INFO_T));
			if(_g_hst_hw_h15_trace)
			{
				if(count>(PE_PRINT_COUNT_NUM>>3))
				{
					PE_HST_HW_H15_DBG_PRINT("det_3df[%d],src_type[%d],out_type[%d]\n",\
						pstParams->det_3df,pstParams->dis_inf.src_type,pstParams->dis_inf.out_type);
					PE_HST_HW_H15_DBG_PRINT("lrc : dif_max[%7d,%7d,%7d,%7d]\n",pstParams->lrc_inf.dif_max[0],\
						pstParams->lrc_inf.dif_max[1],pstParams->lrc_inf.dif_max[2],pstParams->lrc_inf.dif_max[3]);
					PE_HST_HW_H15_DBG_PRINT("lrc : pxl_cnt[%7d,%7d,%7d,%7d]\n",pstParams->lrc_inf.pxl_cnt[0],\
						pstParams->lrc_inf.pxl_cnt[1],pstParams->lrc_inf.pxl_cnt[2],pstParams->lrc_inf.pxl_cnt[3]);
					PE_HST_HW_H15_DBG_PRINT("lrc : set_cnt[%7d,%7d,%7d,%7d]\n",pstParams->lrc_inf.set_cnt[0],\
						pstParams->lrc_inf.set_cnt[1],pstParams->lrc_inf.set_cnt[2],pstParams->lrc_inf.set_cnt[3]);
					PE_HST_HW_H15_DBG_PRINT("src : s_state[%3d,%3d]\n",\
						pstParams->src_inf.s_state[0],pstParams->src_inf.s_state[1]);
					PE_HST_HW_H15_DBG_PRINT("src : dif_cnt y[%7d,%7d],cb[%7d,%7d],cr[%7d,%7d]\n",
						pstParams->src_inf.dif_cnt[0],pstParams->src_inf.dif_cnt[1],pstParams->src_inf.dif_cnt[2],\
						pstParams->src_inf.dif_cnt[3],pstParams->src_inf.dif_cnt[4],pstParams->src_inf.dif_cnt[5]);
					PE_HST_HW_H15_DBG_PRINT("src : dif_cnt g[%7d,%7d], b[%7d,%7d], r[%7d,%7d]\n",
						pstParams->src_inf.dif_cnt[6],pstParams->src_inf.dif_cnt[7],pstParams->src_inf.dif_cnt[8],\
						pstParams->src_inf.dif_cnt[9],pstParams->src_inf.dif_cnt[10],pstParams->src_inf.dif_cnt[11]);
					PE_HST_HW_H15_DBG_PRINT("src : dif_cnt h[%7d,%7d], s[%7d,%7d], v[%7d,%7d]\n",
						pstParams->src_inf.dif_cnt[12],pstParams->src_inf.dif_cnt[13],pstParams->src_inf.dif_cnt[14],\
						pstParams->src_inf.dif_cnt[15],pstParams->src_inf.dif_cnt[16],pstParams->src_inf.dif_cnt[17]);
					PE_HST_HW_H15_DBG_PRINT("src : pxl_cnt y[%7d,%7d],cb[%7d,%7d],cr[%7d,%7d]\n",
						pstParams->src_inf.pxl_cnt[0],pstParams->src_inf.pxl_cnt[1],pstParams->src_inf.pxl_cnt[2],\
						pstParams->src_inf.pxl_cnt[3],pstParams->src_inf.pxl_cnt[4],pstParams->src_inf.pxl_cnt[5]);
					PE_HST_HW_H15_DBG_PRINT("src : pxl_cnt g[%7d,%7d], b[%7d,%7d], r[%7d,%7d]\n",
						pstParams->src_inf.pxl_cnt[6],pstParams->src_inf.pxl_cnt[7],pstParams->src_inf.pxl_cnt[8],\
						pstParams->src_inf.pxl_cnt[9],pstParams->src_inf.pxl_cnt[10],pstParams->src_inf.pxl_cnt[11]);
					PE_HST_HW_H15_DBG_PRINT("src : pxl_cnt h[%7d,%7d], s[%7d,%7d], v[%7d,%7d]\n",
						pstParams->src_inf.pxl_cnt[12],pstParams->src_inf.pxl_cnt[13],pstParams->src_inf.pxl_cnt[14],\
						pstParams->src_inf.pxl_cnt[15],pstParams->src_inf.pxl_cnt[16],pstParams->src_inf.pxl_cnt[17]);
					PE_HST_HW_H15_DBG_PRINT("src : set_cnt y[%7d,%7d],cb[%7d,%7d],cr[%7d,%7d]\n",
						pstParams->src_inf.set_cnt[0],pstParams->src_inf.set_cnt[1],pstParams->src_inf.set_cnt[2],\
						pstParams->src_inf.set_cnt[3],pstParams->src_inf.set_cnt[4],pstParams->src_inf.set_cnt[5]);
					PE_HST_HW_H15_DBG_PRINT("src : set_cnt g[%7d,%7d], b[%7d,%7d], r[%7d,%7d]\n",
						pstParams->src_inf.set_cnt[6],pstParams->src_inf.set_cnt[7],pstParams->src_inf.set_cnt[8],\
						pstParams->src_inf.set_cnt[9],pstParams->src_inf.set_cnt[10],pstParams->src_inf.set_cnt[11]);
					PE_HST_HW_H15_DBG_PRINT("src : set_cnt h[%7d,%7d], s[%7d,%7d], v[%7d,%7d]\n",
						pstParams->src_inf.set_cnt[12],pstParams->src_inf.set_cnt[13],pstParams->src_inf.set_cnt[14],\
						pstParams->src_inf.set_cnt[15],pstParams->src_inf.set_cnt[16],pstParams->src_inf.set_cnt[17]);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get detected 3d format
 *
 * @param   *pstParams [in/out] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see		PE_HST_HW_H15_DET_3D_FMT
 * @author
 */
int PE_HST_HW_H15_GetDetected3dFmt(UINT32 *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	PE_HST_HW_H15_INFO_T *pInfo = &_g_pe_hst_hw_h15_info.hist_inf;
	do{
		CHECK_KNULL(pstParams);
		if(PE_KDRV_VER_H15)
		{
			*pstParams = (UINT32)pInfo->det_3df;
			if(_g_hst_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_HST_HW_H15_DBG_PRINT("det_3df[%d]\n",pInfo->det_3df);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get saturation state
 *	- if PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO defined,
 *		s_state and s_apl will be updated.
 *
 * @param   win_id [in] UINT32
 * @param   *pstParams [in/out] UINT32
 * @return  OK if success, ERROR otherwise.
 * @see		
 * @author
 */
int PE_HST_HW_H15_GetSaturationState(UINT32 win_id, UINT32 *pstParams)
{
	int ret = RET_OK;
	static UINT32 count = 0;
	LX_PE_WIN_ID wId;
	PE_HST_HW_H15_SRC_HST_DATA_T *hist_data = &_g_pe_hst_hw_h15_info.src_hist.data;
	#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
	UINT32 rd_data0 = 0, rd_data1 = 0, rd_data2 = 0;
	PE_HST_HW_H15_SRC_APL_DATA_T *apl_data = &_g_pe_hst_hw_h15_info.src_apl.data;
	PE_HST_HW_H15_SRC_INFO_T *hist_info = &_g_pe_hst_hw_h15_info.hist_inf.src_inf;
	PE_HST_HW_H15_FW_APL_STATUS_T *reg_apl_status = (PE_HST_HW_H15_FW_APL_STATUS_T *)&rd_data0;
	PE_HST_HW_H15_FW_HST_STATUS_T *reg_hst_status = (PE_HST_HW_H15_FW_HST_STATUS_T *)&rd_data1;
	PE_HST_HW_H15_FW_APL_DATA_T *reg_apl_data = (PE_HST_HW_H15_FW_APL_DATA_T *)&rd_data2;
	#endif
	do{
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(win_id);
		wId = PE_GET_CHECKED_WINID(win_id);
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
			if(PE_CHECK_WIN0(wId))
			{
				rd_data0 = gPE_BFW_INFO_H15A.phys.data->histoInfo.apl_status;
				rd_data1 = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_status;
				hist_data->s_state[wId] = reg_hst_status->sat_status;
				if(hist_info->s_state[wId] != hist_data->s_state[wId])
				{
					PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_00, reg_s_status_0,	GET_BITS(hist_data->s_state[wId],0,8));
					hist_info->s_state[wId] = hist_data->s_state[wId];
				}
				if (reg_apl_status->rd_buf_num == 0)
				{
					rd_data2 = gPE_BFW_INFO_H15A.phys.data->histoInfo.apl_buf0_ysv;
				}
				else if (reg_apl_status->rd_buf_num == 1)
				{
					rd_data2 = gPE_BFW_INFO_H15A.phys.data->histoInfo.apl_buf1_ysv;
				}
				else
				{
					rd_data2 = gPE_BFW_INFO_H15A.phys.data->histoInfo.apl_buf2_ysv;
				}
				apl_data->yuv[LX_PE_WIN_0] = (rd_data2&0x000003ff);
				apl_data->hsv[LX_PE_WIN_0] = (rd_data2&0x3ffffc00);
				PE_CE_PE1_H15A0_QWr01(pe1_fsw_ctrl_00,reg_s_apl_0,GET_BITS(reg_apl_data->apl_s,2,8));
			}
			#endif
			*pstParams = hist_data->s_state[wId];
			if(_g_hst_hw_h15_trace)
			{
				if(count>PE_PRINT_COUNT_NUM)
				{
					PE_HST_HW_H15_DBG_PRINT("get[%d] s_state[%d][%d]\n",wId,\
						hist_data->s_state[LX_PE_WIN_0],hist_data->s_state[LX_PE_WIN_1]);
					count=0;
				}
				count++;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get src(pe0) histo bin(32bin)
 *
 * @param   win_id [in/out] UINT32
 * @param   *pstParams [in/out] UINT32
 * @param   hst_sel [in] PE_HST_HW_H15_SRC_HST_SEL
 * @return  OK if success, ERROR otherwise.
 * @see		
 * @author
 */
int PE_HST_HW_H15_GetSrcHistoBin(UINT32 win_id, UINT32 *pstParams, PE_HST_HW_H15_SRC_HST_SEL hst_sel)
{
	int ret = RET_OK;
	PE_HST_HW_H15_SRC_HST_DATA_T *hist_data = &_g_pe_hst_hw_h15_info.src_hist.data;
	#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
	UINT32 i;
	UINT32 rd_data0 = 0;
	PE_HST_HW_H15_FW_HST_STATUS_T *reg_hst_status = (PE_HST_HW_H15_FW_HST_STATUS_T *)&rd_data0;
	#endif
	do{
		CHECK_KNULL(pstParams);
		PE_HST_HW_H15_CHECK_CODE((hst_sel!=PE_HST_HW_H15_SRC_HST_SEL_S)&&\
			(hst_sel!=PE_HST_HW_H15_SRC_HST_SEL_H)&&(hst_sel!=PE_HST_HW_H15_SRC_HST_SEL_Y),\
			break,"[%s,%d] hst_sel(%d) is wrong.\n",__F__,__L__,hst_sel);
		PE_HST_HW_H15_CHECK_CODE((win_id>=LX_PE_WIN_NUM),\
			break,"[%s,%d] win_id(%d) is wrong.\n",__F__,__L__,win_id);
		if(PE_KDRV_VER_H15)
		{
			#ifdef PE_HST_HW_H15A_USE_SRC_FW_APL_HISTO
			rd_data0 = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_status;
			if(hst_sel==PE_HST_HW_H15_SRC_HST_SEL_S)
			{
				if (reg_hst_status->rd_buf_num == 0)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->s0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf0_s[i];
						pstParams[i] = hist_data->s0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->s0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf0_s[i];
				}
				else if (reg_hst_status->rd_buf_num == 1)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->s0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf1_s[i];
						pstParams[i] = hist_data->s0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->s0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf1_s[i];
				}
				else
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->s0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf2_s[i];
						pstParams[i] = hist_data->s0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->s0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf2_s[i];
				}
			}
			else if(hst_sel==PE_HST_HW_H15_SRC_HST_SEL_H)
			{
				if (reg_hst_status->rd_buf_num == 0)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->h0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf0_h[i];
						pstParams[i] = hist_data->h0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->h0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf0_h[i];
				}
				else if (reg_hst_status->rd_buf_num == 1)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->h0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf1_h[i];
						pstParams[i] = hist_data->h0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->h0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf1_h[i];
				}
				else
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->h0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf2_h[i];
						pstParams[i] = hist_data->h0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->h0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf2_h[i];
				}
			}
			else	//PE_HST_HW_H15_SRC_HST_SEL_Y
			{
				if (reg_hst_status->rd_buf_num == 0)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->y0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf0_y[i];
						pstParams[i] = hist_data->y0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->y0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf0_y[i];
				}
				else if (reg_hst_status->rd_buf_num == 1)
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->y0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf1_y[i];
						pstParams[i] = hist_data->y0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->y0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf1_y[i];
				}
				else
				{
					for(i=0;i<PE_HST_HW_H15_HST_BIN_NUM;i++)
					{
						hist_data->y0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf2_y[i];
						pstParams[i] = hist_data->y0[i];
					}
					i = PE_HST_HW_H15_HST_BIN_NUM;
					hist_data->y0[i] = gPE_BFW_INFO_H15A.phys.data->histoInfo.histo_buf2_y[i];
				}
			}
			#else
			if(hst_sel==PE_HST_HW_H15_SRC_HST_SEL_S)
			{
				if (win_id==LX_PE_WIN_0)
					memcpy(pstParams,&(hist_data->s0[0]),sizeof(UINT32)*PE_HST_HW_H15_HST_BIN_NUM);
				else
					memcpy(pstParams,&(hist_data->s1[0]),sizeof(UINT32)*PE_HST_HW_H15_HST_BIN_NUM);
			}
			else if(hst_sel==PE_HST_HW_H15_SRC_HST_SEL_H)
			{
				if (win_id==LX_PE_WIN_0)
					memcpy(pstParams,&(hist_data->h0[0]),sizeof(UINT32)*PE_HST_HW_H15_HST_BIN_NUM);
				else
					memcpy(pstParams,&(hist_data->h1[0]),sizeof(UINT32)*PE_HST_HW_H15_HST_BIN_NUM);
			}
			else	//PE_HST_HW_H15_SRC_HST_SEL_Y
			{
				if (win_id==LX_PE_WIN_0)
					memcpy(pstParams,&(hist_data->y0[0]),sizeof(UINT32)*PE_HST_HW_H15_HST_BIN_NUM);
				else
					memcpy(pstParams,&(hist_data->y1[0]),sizeof(UINT32)*PE_HST_HW_H15_HST_BIN_NUM);
			}
			#endif
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	}while(0);
	return ret;
}
/**
 * get dce histo info
 *
 * @param   *pstParams [in/out] LX_PE_HST_HISTO_INFO_T
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int PE_HST_HW_H15_GetHistogramInfo(LX_PE_HST_HISTO_INFO_T *pstParams)
{
	int ret = RET_OK;
	LX_PE_WIN_ID win_id;
	UINT32 count=0, histoUpdateMaskEven=0, histoUpdateMaskOdd=0;
	UINT32 r_data[2]={0,0};
	PE_INF_H15_SETTINGS_T inf_set;
	UINT32 avrg0[3]={0,0,0}, avrg1[3]={0,0,0}, histo_start=0x0;
	UINT32 tm[2]={0,0};
	UINT32 hist0[PE_HST_HW_H15_HST_BIN_NUM];
	UINT32 hist1[PE_HST_HW_H15_HST_BIN_NUM];
	do {
		CHECK_KNULL(pstParams);
		PE_CHECK_WINID(pstParams->win_id);
		win_id = PE_GET_CHECKED_WINID(pstParams->win_id);
		if(PE_KDRV_VER_H15)
		{
			if(PE_CHECK_WIN0(win_id))
			{
				if(pstParams->domain==LX_PE_YC_DOMAIN)
				{
					PE_CE_PE1_H15A0_RdFL(pe1_apl_stat_00);
					PE_CO_PE1_H15A0_RdFL(pe1_apl_stat_00);
					PE_CE_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cr,avrg0[2]);
					PE_CE_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_yg,avrg0[0]);
					PE_CE_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cb,avrg0[1]);
					PE_CO_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cr,avrg1[2]);
					PE_CO_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_yg,avrg1[0]);
					PE_CO_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cb,avrg1[1]);
				}
				else
				{
					PE_CE_PE1_H15A0_RdFL(pe1_apl_stat_00);
					PE_CO_PE1_H15A0_RdFL(pe1_apl_stat_00);
					PE_CE_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cr,avrg0[0]);
					PE_CE_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_yg,avrg0[1]);
					PE_CE_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cb,avrg0[2]);
					PE_CO_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cr,avrg1[0]);
					PE_CO_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_yg,avrg1[1]);
					PE_CO_PE1_H15A0_Rd01(pe1_apl_stat_00,apl_cb,avrg1[2]);
				}
				pstParams->average[0] = ((avrg0[0]+avrg1[0])>>1);
				pstParams->average[1] = ((avrg0[1]+avrg1[1])>>1);
				pstParams->average[2] = ((avrg0[2]+avrg1[2])>>1);
				PE_CEO_PE1_H15A0_RdFL(pe1_dce_hist_ia_ctrl);
				PE_CEO_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_hist_enable, 0x0);	//0:read mode,  1:normal mode
				PE_CEO_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_hist_ai,     0x1);	//ai 0:disable,  1:enable
				PE_CEO_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_hist_address,0x0);	//bin address
				PE_CEO_PE1_H15A0_WrFL(pe1_dce_hist_ia_ctrl);
				PE_CE_PE1_H15A0_QRd01(pe1_dce_hist_ia_ctrl,hif_mask,histoUpdateMaskEven);
				PE_CO_PE1_H15A0_QRd01(pe1_dce_hist_ia_ctrl,hif_mask,histoUpdateMaskOdd);
				if(histoUpdateMaskEven==0 && histoUpdateMaskOdd==0)
				{
					for(count=0;count<PE_HST_HW_H15_HST_BIN_NUM;count++)
					{
						PE_CEO_PE1_H15A0_RdFL(pe1_dce_hist_ia_data1);
						r_data[0] = PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data1);
						r_data[1] = PE_CO_PE1_H15A0_Rd(pe1_dce_hist_ia_data1);
						pstParams->histogram[count]= r_data[0] + r_data[1];
					}
				}
				else
				{
					PE_HST_HW_H15_DBG_PRINT("cant read histogram\n");
				}
				//get cur info setting
				ret = PE_INF_H15_GetCurInfSettings(&inf_set);
				//src sat histo
				//src hue histo
				if (PE_HW_H15_CHK_MULTI_PATH_FMT(inf_set.disp_info[LX_PE_WIN_0].fmt_type) || \
					PE_HW_H15_CHK_MULTI_PATH_OUT(inf_set.disp_info[LX_PE_WIN_0].out_type))
				{
					histo_start = PE_HST_HW_H15_HST_BIN_NUM;
					ret = PE_HST_HW_H15_GetSrcHistoBin(LX_PE_WIN_0, hist0, PE_HST_HW_H15_SRC_HST_SEL_S);
					ret = PE_HST_HW_H15_GetSrcHistoBin(LX_PE_WIN_1, hist1, PE_HST_HW_H15_SRC_HST_SEL_S);
					for(count=0;count<PE_HST_HW_H15_HST_BIN_NUM;count++)
					{
						pstParams->histogram[histo_start+count] = hist0[count]+hist1[count];
					}
					histo_start = (PE_HST_HW_H15_HST_BIN_NUM<<1);
					ret = PE_HST_HW_H15_GetSrcHistoBin(LX_PE_WIN_0, hist0, PE_HST_HW_H15_SRC_HST_SEL_H);
					ret = PE_HST_HW_H15_GetSrcHistoBin(LX_PE_WIN_1, hist1, PE_HST_HW_H15_SRC_HST_SEL_H);
					for(count=0;count<PE_HST_HW_H15_HST_BIN_NUM;count++)
					{
						pstParams->histogram[histo_start+count] = hist0[count]+hist1[count];
					}
				}
				else
				{
					histo_start = PE_HST_HW_H15_HST_BIN_NUM;
					ret = PE_HST_HW_H15_GetSrcHistoBin(LX_PE_WIN_0, \
						&(pstParams->histogram[histo_start]), PE_HST_HW_H15_SRC_HST_SEL_S);
					histo_start = (PE_HST_HW_H15_HST_BIN_NUM<<1);
					ret = PE_HST_HW_H15_GetSrcHistoBin(LX_PE_WIN_0, \
						&(pstParams->histogram[histo_start]), PE_HST_HW_H15_SRC_HST_SEL_H);
				}
				/* 0:read mode,  1:normal mode */
				PE_CEO_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_hist_enable,0x1);
				PE_CEO_PE1_H15A0_WrFL(pe1_dce_hist_ia_ctrl);
				PE_CEO_PE1_H15A0_RdFL(pe1_dce_hist_ia_ctrl);
				//status[0] : min, max (ce only)
				PE_CEO_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_status_address,0x0);
				PE_CEO_PE1_H15A0_WrFL(pe1_dce_hist_ia_ctrl);
				PE_CEO_PE1_H15A0_RdFL(pe1_dce_hist_ia_data2);
				pstParams->min = GET_BITS(PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data2), 0, 10);
				pstParams->max = GET_BITS(PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data2), 16, 10);
				pstParams->status[0] = PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data2);
				//status[1] : V-min max (ce only)
				PE_CE_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_status_address,0x1);
				PE_CE_PE1_H15A0_WrFL(pe1_dce_hist_ia_ctrl);
				PE_CE_PE1_H15A0_RdFL(pe1_dce_hist_ia_data2);
				pstParams->status[1] = PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data2);
				//status[2] : bin val max, min (ce only)
				PE_CE_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_status_address,0x2);
				PE_CE_PE1_H15A0_WrFL(pe1_dce_hist_ia_ctrl);
				PE_CE_PE1_H15A0_RdFL(pe1_dce_hist_ia_data2);
				pstParams->status[2] = PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data2);
				//status[3] : diff sum (ce,co sum)
				PE_CEO_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_status_address,0x3);
				PE_CEO_PE1_H15A0_WrFL(pe1_dce_hist_ia_ctrl);
				PE_CEO_PE1_H15A0_RdFL(pe1_dce_hist_ia_data2);
				r_data[0] = PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data2);
				r_data[1] = PE_CO_PE1_H15A0_Rd(pe1_dce_hist_ia_data2);
				pstParams->status[3] = (r_data[0]+r_data[1]);
				//status[4] : detected region num (ce,co sum)
				PE_CEO_PE1_H15A0_Wr01(pe1_dce_hist_ia_ctrl,hif_status_address,0x4);
				PE_CEO_PE1_H15A0_WrFL(pe1_dce_hist_ia_ctrl);
				PE_CEO_PE1_H15A0_RdFL(pe1_dce_hist_ia_data2);
				r_data[0] = PE_CE_PE1_H15A0_Rd(pe1_dce_hist_ia_data2);
				r_data[1] = PE_CO_PE1_H15A0_Rd(pe1_dce_hist_ia_data2);
				pstParams->status[4] = (r_data[0]+r_data[1]);
				//status[5] : saturation status
				//status[6] : motion status
				if (PE_HW_H15_CHK_MULTI_PATH_FMT(inf_set.disp_info[LX_PE_WIN_0].fmt_type) || \
					PE_HW_H15_CHK_MULTI_PATH_OUT(inf_set.disp_info[LX_PE_WIN_0].out_type))
				{
					r_data[0] = inf_set.ctrl_info[LX_PE_WIN_0].sat_status;
					r_data[1] = inf_set.ctrl_info[LX_PE_WIN_1].sat_status;
					pstParams->status[5] = ((r_data[0]+r_data[1])>>1);
					PE_ND_ND1_H15A0_QRd01(tnr_status_01,w_apl_motion,r_data[0]);
					PE_ND_ND2_H15A0_QRd01(tnr_status_01,w_apl_motion,r_data[1]);
					pstParams->status[6] = ((r_data[0]+r_data[1])>>1);
				}
				else
				{
					pstParams->status[5] = inf_set.ctrl_info[LX_PE_WIN_0].sat_status;
					PE_ND_ND1_H15A0_QRd01(tnr_status_01,w_apl_motion,pstParams->status[6]);
				}
				//status[7] : texture measure
				PE_SHPL_H15A0_RdFL(pe1_coring_stat_01);
				PE_SHPR_H15A0_RdFL(pe1_coring_stat_01);
				PE_SHPL_H15A0_Rd01(pe1_coring_stat_01,t_map_frame_sum,tm[0]);
				PE_SHPR_H15A0_Rd01(pe1_coring_stat_01,t_map_frame_sum,tm[1]);
				pstParams->status[7] = tm[0]+tm[1];
				pstParams->histo_ready = 1;
			}
		}
		else
		{
			PE_HST_HW_H15_DBG_PRINT("nothing to do\n");	ret = RET_OK;
		}
	} while(0);
	return ret;
}

