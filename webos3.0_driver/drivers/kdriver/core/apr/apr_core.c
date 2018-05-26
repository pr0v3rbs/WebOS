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


/** @file apr_core.c
 *
 *  driver for 3d format auto detection. ( used only within kdriver )
 *	- functional functions regardless of modules
 *
 *	@author		justine.jeong
 *	@version	0.1
 *	@note
 *	@date		2012.07.10
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
#include <linux/vmalloc.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"

#include "apr_def.h"
#include "apr_core.h"
#include "apr_hal.h"
#include "apr_osd.h"
#include "apr_ldf.h"
#include "apr_hfd.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define DEFAULT_D_TPD_THR    182
#define DEFAULT_D_PTN_THR    64
#define DEFAULT_D_MTN2_THR   120      // 40
#define DEFAULT_D_MTN3_THR   3
#define DEFAULT_D_FMT_TH1    7
#define DEFAULT_D_FMT_TH2    8        // 13 -> 5 -> 8 changed by won.hur
#define DEFAULT_D_FMT_TH3    100000   // 150k->100K changed by won.hur
#define DEFAULT_D_FMT_TH4    2000
#define DEFAULT_D_FMT_TH5    300000
#define DEFAULT_D_FMT_TH6    30000    // 60k-> 30K changed by won.hur
#define DEFAULT_D_FMT_TH7    1500000
#define DEFAULT_D_FMT_TH8    250000
#define DEFAULT_D_FMT_TH9    12
#define DEFAULT_D_FMT_TH10   30000
#define DEFAULT_D_VLD_THR    5
#define DEFAULT_D_SUBTITLE   192      // 192
// #define DEFAULT_D_VARIATION  4        // 4.0 // 2012.10.28 by jungeun.lim
#define DEFAULT_D_VARIATION  0       // 4.0
#define DEFAULT_D_CORR       248      // 0.97 * 256
#define DEFAULT_D_CORR3D     179      // 0.70 * 256
#define DEFAULT_D_FINAL_CORR 220      // larger -> 2D , smaller -> 3D
#define DEFAULT_D_MEAN_TOLER 10       // larger -> 3D , smaller -> 2D

#define DEFAULT_A_TPD_THR    170
#define DEFAULT_A_PTN_THR    64
#define DEFAULT_A_MTN2_THR   150
#define DEFAULT_A_MTN3_THR   4
#define DEFAULT_A_FMT_TH1    5
#define DEFAULT_A_FMT_TH2    6
#define DEFAULT_A_FMT_TH3    450000
#define DEFAULT_A_FMT_TH4    2000
#define DEFAULT_A_FMT_TH5    300000
#define DEFAULT_A_FMT_TH6    30000
#define DEFAULT_A_FMT_TH7    1600000
#define DEFAULT_A_FMT_TH8    250000
#define DEFAULT_A_FMT_TH9    12
#define DEFAULT_A_FMT_TH10   30000
#define DEFAULT_A_VLD_THR    5
#define DEFAULT_A_LRD_THR    1600
#define DEFAULT_A_SUBTITLE   192      // 192
// #define DEFAULT_A_VARIATION  4        // 4.0 // 2012.10.28 by jungeun.lim
#define DEFAULT_A_VARIATION  64       // 4.0 * 16
#define DEFAULT_A_CORR       248      // 0.97 * 256
#define DEFAULT_A_CORR3D     179      // 0.70 * 256
#define DEFAULT_A_FINAL_CORR 220      // larger -> 2D , smaller -> 3D
#define DEFAULT_A_MEAN_TOLER 10       // larger -> 3D , smaller -> 2D

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
//#define APR_AUTO_START
#define APR_USE_VLINE
//#define USE_VSYNC_INTERRUPT
//#define _APR_USE_METHOD2_
#define APR_USE_FD3 

#define APR_WIN_RATIO     2
#define APR_POLLING_COUNT 6    // skip count for thread or vsync handler

#define APR_MSG			if(_g_apr_trace==LX_APR_CMD_LOG)APR_PRINT

#define APR_CHECK_CODE(_checker,_action,fmt,args...)	{if(_checker){APR_ERROR(fmt,##args);_action;}}

#define APR_MAX2(a,b) (a>=b)? a:b
#define APR_MIN2(a,b) (a<=b)? a:b
#define APR_MIN3(a,b,c) (a<=b)? ((a<=c)?a:c): ((b<=c)?b:c)
#define APR_DIFF(a,b) (a>=b)?abs(a-b):abs(b-a)

#define APR_THREAD_Register(f) (apr_thread_cb.func = f)
#define APR_VSYNC_Register de_sync_register
#define TO_STR(a) (#a)

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	int (*func)(void);
} APR_THREAD_CB_T;
APR_THREAD_CB_T apr_thread_cb;

typedef struct {
	int (*d3d)(APR_CAP_INFO_T *capture_info);
	int (*d2d)(APR_CAP_INFO_T *capture_info);
	int (*fd3)(void);
} APR_CB_MEASURE_T;

typedef struct {
	int (*d3d)( APR_CAP_SRC_T   ov_src,      \
				APR_OV_INFO_T  *ov_info,     \
				APR_CAP_INFO_T *capture_info,\
				UINT32 h_line_idx,           \
				UINT32 h_line_width,         \
				UINT32 v_line_idx,           \
				UINT32 v_line_width,         \
				UINT32 top_offset,           \
				UINT32 bot_offset,           \
				UINT32 lr_crop);
	int (*d2d)( APR_CAP_SRC_T   ov_src,      \
				APR_OV_INFO_T  *ov_info,     \
				APR_CAP_INFO_T *capture_info,\
				UINT32 h_line_idx,           \
				UINT32 h_line_width,         \
				UINT32 v_line_idx,           \
				UINT32 v_line_width,         \
				UINT32 top_offset,           \
				UINT32 bot_offset,           \
				UINT32 lr_crop);
	int (*fd3)( APR_OV_INFO_T  *ov_info,     \
				APR_CAP_INFO_T *capture_info);
} APR_CB_CAPTURE_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern int de_sync_register(BOOLEAN flag, int (*func)(void), char *str);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
APR_INFO_T    apr_info;
APR_THR_T     default_threshold_digital = {0, DEFAULT_D_TPD_THR, DEFAULT_D_PTN_THR,      \
                                              DEFAULT_D_MTN2_THR,DEFAULT_D_MTN3_THR,     \
                                              DEFAULT_D_FMT_TH1, DEFAULT_D_FMT_TH2,      \
                                              DEFAULT_D_FMT_TH3, DEFAULT_D_FMT_TH4,      \
                                              DEFAULT_D_FMT_TH5, DEFAULT_D_FMT_TH6,      \
                                              DEFAULT_D_FMT_TH7, DEFAULT_D_FMT_TH8,      \
                                              DEFAULT_D_FMT_TH9, DEFAULT_D_FMT_TH10,     \
                                              DEFAULT_D_VLD_THR, \
                                              {0,0,0,0,0,0,0},                           \
                                              {DEFAULT_D_SUBTITLE,DEFAULT_D_VARIATION,   \
                                              DEFAULT_D_CORR,     DEFAULT_D_CORR3D,      \
                                              DEFAULT_D_FINAL_CORR,DEFAULT_D_MEAN_TOLER}};
APR_THR_T     default_threshold_analog  = {0, DEFAULT_A_TPD_THR, DEFAULT_A_PTN_THR,      \
                                              DEFAULT_A_MTN2_THR,DEFAULT_A_MTN3_THR,     \
                                              DEFAULT_A_FMT_TH1, DEFAULT_A_FMT_TH2,      \
                                              DEFAULT_A_FMT_TH3, DEFAULT_A_FMT_TH4,      \
                                              DEFAULT_A_FMT_TH5, DEFAULT_A_FMT_TH6,      \
                                              DEFAULT_A_FMT_TH7, DEFAULT_A_FMT_TH8,      \
                                              DEFAULT_A_FMT_TH9, DEFAULT_D_FMT_TH10,     \
                                              DEFAULT_A_VLD_THR, \
                                              {0,0,0,0,0,0,0},                           \
                                              {DEFAULT_A_SUBTITLE,DEFAULT_A_VARIATION,   \
                                              DEFAULT_A_CORR,     DEFAULT_A_CORR3D,      \
                                              DEFAULT_A_FINAL_CORR,DEFAULT_A_MEAN_TOLER}};

LX_APR_FORMAT  initial_format;
APR_CAP_INFO_T capture_info;

UINT8	gfInputSourceChanged = 1;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static int APR_GetMax3(UINT32 *g_diff, UINT32 *b_diff, UINT32 *r_diff, UINT32 *diff_max);
static int APR_CheckConfig(void);
static int APR_CollectBasicData(void);
static int APR_GetLrcrHistogram(void);
static int APR_GetDceHistogram(void);
#ifdef APR_USE_LINE_DIFF_BY_SW
static int APR_GetOVInfo(			APR_CAP_SRC_T  ov_src,       \
									APR_OV_INFO_T *ov_info);
static int APR_GetStripFromFrame(	APR_CAP_SRC_T   ov_src,      \
									APR_OV_INFO_T  *ov_info,     \
									APR_CAP_INFO_T *capture_info,\
									UINT32 h_line_idx,           \
									UINT32 h_line_width,         \
									UINT32 v_line_idx,           \
									UINT32 v_line_width,         \
									UINT32 top_offset,           \
									UINT32 bot_offset,           \
									UINT32 lr_crop);
static int APR_GetStripForFd3(APR_OV_INFO_T  *ov_info, APR_CAP_INFO_T *capture_info);
static int APR_InitFrameMemory(UINT32 h_max_line, UINT32 v_max_line);
static int APR_UnInitFrameMemory(void);
static int APR_GetUSecTimerVal(UINT32 *tv);
static int APR_CheckTime(char *fn_str, APR_GET_TIME_T location, APR_TIME_T pos);
#endif
static int APR_DumpCurrentConfig(void); 
static int APR_DumpHistogram(void);
static int APR_DumpLrcHistData(void);
static int APR_DecideFormatBySRCHST(void);
static int APR_DecideFormatByLRCHST(void);
static int APR_DecideFormatByLDF(void);
static int APR_DecideFormatByFD3(void);
static int APR_VotingFormat(APR_METHOD_T method_idx, LX_APR_FORMAT fmt, BOOLEAN vote_enable);
static int APR_NegotiateFormat(BOOLEAN nego_enble);
static int APR_AccumulateFormat(void);
static int APR_DisplayResult(void);
static int APR_SetThreshold(APR_THR_T *pstParams);
static int APR_VsyncHandler(void);
static int APR_DetectHandler(void);

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/
static LX_APR_CMD _g_apr_trace = LX_APR_CMD_OFF;	//default should be off.

static	UINT32  thr_adj = 0;
static  UINT32  tmp_thr3 = 0, tmp_thr4 = 0, tmp_thr6 = 0, tmp_thr7 = 0;
static  UINT32  tmp_thr_const0 = 0, tmp_thr_const1 = 0, tmp_thr_const2 = 0, tmp_thr_const3 = 0;

static APR_CB_MEASURE_T fn_measure = {
#ifdef APR_USE_LINE_DIFF_BY_SW
		.d3d = APR_LDF_Detect3D,
		.d2d = APR_LDF_Detect2D,
#else
		.d3d = NULL,
		.d2d = NULL,
#endif
		.fd3 = APR_FD3_Detect3D
};
static APR_CB_CAPTURE_T fn_capture = {
#ifdef APR_USE_LINE_DIFF_BY_SW
		.d3d = APR_GetStripFromFrame,
		.d2d = APR_GetStripFromFrame,
		.fd3 = APR_GetStripForFd3
#else
		.d3d = NULL,
		.d2d = NULL,
		.fd3 = NULL 
#endif
};

/*========================================================================================
	Implementation Group
========================================================================================*/

/**
 * init apr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Core_Init(LX_APR_INIT_T *pstParams)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	static BOOLEAN init_flag = FALSE;
	
	do {
		if(init_flag) break;
		pCfg->enable           = LX_APR_CMD_OFF;
		apr_thread_cb.func     = NULL;
#ifdef APR_AUTO_START
		pCfg->enable           = LX_APR_CMD_ON;
		pCfg->sel_alg          = 1;
		pCfg->tmp_fmt          = LX_APR_FMT_2D_2D;
		pCfg->apr_fmt          = LX_APR_FMT_2D_2D;
		pCfg->cap.enb          = LX_APR_CAP_ON;
		pCfg->cap.h_line_num   = APR_MAX_LINE;
		pCfg->cap.v_line_num   = 0;
		pCfg->cap.h_line_width = 1;
		pCfg->cap.v_line_width = 0;
		pCfg->cap.top_offset   = APR_CAP_TOP_OFFSET;
		pCfg->cap.bot_offset   = APR_CAP_BOT_OFFSET;
		pCfg->cap.lr_crop      = APR_CAP_LR_CROP;
		pCfg->cnt_start        = TRUE;
		ret = APR_SetThreshold(&default_threshold_digital);
#endif
		ret = APR_HAL_Init();
		if (ret) break;
		ret = APR_HAL_InitHardware();
		if (ret) break;
#ifdef APR_USE_LINE_DIFF_BY_SW
		ret = APR_InitFrameMemory(/*h_max_line*/APR_MAX_LINE, /*v_max_line*/1);
		if (ret) break;
#endif
#ifdef USE_VSYNC_INTERRUPT
		APR_VSYNC_Register(TRUE, APR_DetectHandler, TO_STR(APR_DetectHandler));
#else
		APR_THREAD_Register(APR_DetectHandler);
		APR_VSYNC_Register(TRUE, APR_VsyncHandler, TO_STR(APR_VsyncHandler));
#endif
		init_flag = TRUE;
		APR_MSG("%s : done\n", __F__);
	}while(0);

	return ret;
}

/**
 * cleanup apr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Core_Cleanup(void)
{
	int ret = RET_OK;
	
	do {
#ifdef APR_USE_LINE_DIFF_BY_SW
		ret = APR_UnInitFrameMemory();
		if (ret) break;
#endif
		ret = APR_HAL_InitHardware();
		if (ret) break;
		ret = APR_HAL_Uninit();
	} while(0);

	return ret;
}

/**
 * resume apr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Core_Resume(void)
{
	int ret = RET_OK;
	LX_APR_CONTROL_T stParams;
	
	do {
		ret = APR_HAL_InitHardware();
		if (ret) break;
		stParams.cmd     = 1;
		stParams.sel_alg = 1;
		stParams.cap_enb = 0;
		stParams.fd3_enb = 1;
		stParams.fmt     = 0;
		ret = APR_Core_SetControl(&stParams);
	} while(0);

	return ret;
}

/**
 * thread handler for apr
 *
 * @param   void
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Core_ThreadHandle(void)
{
	if(apr_thread_cb.func == NULL)
		return RET_ERROR;
	return apr_thread_cb.func();
}

/**
 * base control for apr module
 *
 * @param   pstParams
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Core_SetControl(LX_APR_CONTROL_T *pstParams)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_HST_INFO_T hist_info;
	APR_THR_T stParams;
	UINT32 i, j;

	do {
		CHECK_KNULL(pstParams);

		_g_apr_trace = pstParams->cmd;
		initial_format = pstParams->fmt;
		if(pCfg->enable != LX_APR_CMD_STRING)
		{
			pCfg->hst_enb           = pstParams->sel_alg;
#ifdef _APR_USE_METHOD2_
			pCfg->cap.enb           = (pstParams->cap_enb>=LX_APR_CAP_MAX)?0:pstParams->cap_enb;
#else
			pCfg->cap.enb           = 0;
#endif
			pCfg->fd3.enb           = (pstParams->fd3_enb>=LX_APR_FD3_MAX)?0:pstParams->fd3_enb;
		}
		//pCfg->hidden_cmd = 0;
		pCfg->tmp_fmt    = LX_APR_FMT_2D_2D;
		pCfg->apr_fmt    = LX_APR_FMT_2D_2D;
		pCfg->cnt_start  = TRUE;
		pCfg->mute_status= 1; // initially, internal mute status is on		

		pCfg->cap.v_line_num    = 0;
		pCfg->cap.h_line_width  = 1;
		pCfg->cap.v_line_width  = 0;

		pCfg->fd3.sampling      = 0; // 0:all line,    1:every two line
		pCfg->fd3.color         = 0; // 0:YUV420,      1:YUV422,          2:YUV444
		pCfg->fd3.scan          = 0; // 0:progressive, 1:interlaced
		pCfg->fd3.win.seg_l     = 2;
		pCfg->fd3.win.seg_r     = 2;
		pCfg->fd3.win.seg_t     = 1;
		pCfg->fd3.win.seg_b     = 1;
		pCfg->fd3.win.prj_l     = 0;
		pCfg->fd3.win.prj_r     = 0;
		pCfg->fd3.win.prj_t     = 3;
		pCfg->fd3.win.prj_b     = 3;

		pCfg->fd3.seg_thr.seg_red         = 173;
		pCfg->fd3.seg_thr.seg_re_ye       =   8;
		pCfg->fd3.seg_thr.seg_yellow      =  23;
		pCfg->fd3.seg_thr.seg_ye_gr       =  38;
		pCfg->fd3.seg_thr.seg_green       =  53;
		pCfg->fd3.seg_thr.seg_gr_cy       =  68;
		pCfg->fd3.seg_thr.seg_cyan        =  83;
		pCfg->fd3.seg_thr.seg_cy_bl       =  98;
		pCfg->fd3.seg_thr.seg_blue        =  113;
		pCfg->fd3.seg_thr.seg_bl_ma       =  128;
		pCfg->fd3.seg_thr.seg_magenta     =  143;
		pCfg->fd3.seg_thr.seg_ma_re       =  158;

		pCfg->fd3.seg_thr.seg_value_black =  32;
		pCfg->fd3.seg_thr.seg_saturate    =  61;
		pCfg->fd3.seg_thr.seg_sdelta      =  30;
		
		for(i = 0;i < APR_METHOD_MAX;i++)
		{
			for(j = 0;j < APR_MAX_BAK_FMT;j++)
				pCfg->voting_fmt[i][j] = LX_APR_FMT_2D_2D;
		}
		memcpy(&stParams, &default_threshold_digital, sizeof(APR_THR_T));
		ret = APR_SetThreshold(&stParams);

		switch(pCfg->enable)
		{
			case LX_APR_CMD_OFF:
				hist_info.cur_fmt = LX_APR_FMT_2D_2D; 
				ret = APR_HAL_GetHistInfo(&hist_info);
				if(ret) break;
			case LX_APR_CMD_ON :
				ret = APR_OSD_Init();
				if(ret) break;
				ret = APR_OSD_ClearAllLayer();
				break;
			case LX_APR_CMD_STRING  :
				APR_OSD_DrawString(pstParams->str.str_buf, pstParams->str.x,  pstParams->str.y);
			case LX_APR_CMD_PATTERN :
			case LX_APR_CMD_OSD_LOG :
			case LX_APR_CMD_OSD_GRF :
			case LX_APR_CMD_LOG     :
			case LX_APR_CMD_FIX_2D  :
			case LX_APR_CMD_FIX_3D  :
				ret = APR_OSD_Init();
				if(ret) break;
				ret = APR_OSD_SetControl(TRUE);
				break;
			default :
				break;
		}
		
		pCfg->enable     = pstParams->cmd;
		APR_MSG("###     pstParams->cap_enb     %d    #\n", pCfg->cap.enb);
	} while (0);

	return ret;
}

/**
 * get detected info.
 *
 * @param   pstParams
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Core_GetInfo(LX_APR_INFO_T *pstParams)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;

	do{
		CHECK_KNULL(pstParams);
		pstParams->valid   = pCfg->valid;
		pstParams->apr_inf = pCfg->apr_fmt;
		pCfg->cnt_start = FALSE;
		
		if(_g_apr_trace==LX_APR_CMD_FIX_3D)
		{
			pstParams->apr_inf = initial_format;
		}
		else if(_g_apr_trace==LX_APR_CMD_FIX_2D)
		{
			pstParams->apr_inf = LX_APR_FMT_2D_2D;
		}

		switch(pData->hw_fmt)
		{
			case 0 :
				pstParams->dsc_type = LX_APR_DSC_TYPE_2D;
				break;
			case 8 :
				pstParams->dsc_type = LX_APR_DSC_TYPE_SS;
				break;
			case 9 :
				pstParams->dsc_type = LX_APR_DSC_TYPE_TB;
				break;
			default :
				pstParams->dsc_type = LX_APR_DSC_TYPE_2D;
				break;
		}

		if (lx_chip_rev( ) >= LX_CHIP_REV(H15,A0) ) {
			if(pstParams->apr_inf == LX_APR_FMT_2D_2D || pstParams->apr_inf == LX_APR_FMT_UNKNOWN)
			{
				ret = APR_HAL_InitHardware();
			}
		}
		//if(pCfg->fd3.enb == LX_APR_FD3_LOG)
		{
			 APR_TRACE("%s : valid[%d] result[%d] (2D:0/SS:4/TB:5) hw[%d]\n", \
				 __F__, pstParams->valid, pstParams->apr_inf, pstParams->dsc_type); 
		}
	}while(0);

	return ret;
}

/**
 * set hidden command for debug
 *
 * @param   pstParams
 * @return  OK if success, ERROR otherwise.
 * @see
 * @author
 */
int APR_Core_SetCommand(LX_APR_COMMAND_T *pstParams)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;

	do {
		pCfg->hidden_cmd = pstParams->cmd;
		switch(pstParams->cmd)
		{
			case LX_APR_HCMD_INIT :
				break;
			case LX_APR_HCMD_GET_INTERNAL_STATE :
				APR_DumpCurrentConfig();
				break;
			case LX_APR_HCMD_DUMP_SCRIPT_DATA :
			case LX_APR_HCMD_DUMP_EXCEL_DATA :
				break;
			default:
				break;
		}
	} while(0);

	return ret;
}

static int APR_SetThreshold(APR_THR_T *pstParams)
{
    int ret = RET_OK;
	APR_THR_T *pThr = &apr_info.thr;

	do {
		CHECK_KNULL(pstParams);
		if(pstParams->isDefault)
			memcpy(pThr, &default_threshold_digital, sizeof(APR_THR_T));
		else
			memcpy(pThr, pstParams, sizeof(APR_THR_T));
	} while(0);
	return ret;
}

static int APR_CheckConfig(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	static LX_PE_SRC_TYPE cur_src_type = LX_PE_SRC_NUM;
	static LX_PE_OUT_TYPE cur_out_type = LX_PE_OUT_NUM;
	static UINT32 cur_scan_info    = 0xFFFF;
	static UINT32 cur_color_sample = 0xFFFF;
	static UINT32 cur_input_height = 0;
	APR_THR_T    *p_default_threshold;
	UINT32 i=0,j=0;

	do {
		if(cur_src_type != pCfg->src_type)
		{
			switch(pCfg->src_type)
			{
				case LX_PE_SRC_ATV :
				case LX_PE_SRC_CVBS :
				case LX_PE_SRC_COMP :
				case LX_PE_SRC_RGBPC :
				case LX_PE_SRC_SCART :
					p_default_threshold = &default_threshold_analog;
					break;
				case LX_PE_SRC_DTV :
				case LX_PE_SRC_HDMI :
				default :
					p_default_threshold = &default_threshold_digital;
					break;
			}
			ret = APR_SetThreshold(p_default_threshold);
			if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_INFO)
				APR_MSG("%s : src_type  %s\n", __F__, ((pCfg->src_type==LX_PE_SRC_DTV)||(pCfg->src_type==LX_PE_SRC_HDMI))?"DIGITAL":"ANALOG");
			if(!ret) ret = RET_TRY_AGAIN;
		}
		cur_src_type = pCfg->src_type;
		
		if(cur_out_type != pCfg->out_type)
		{
			/* TODO:add LX_PE_OUT_LA */
			switch(pCfg->out_type)
			{
				case LX_PE_OUT_UD :
				case LX_PE_OUT_PIP :
				case LX_PE_OUT_2D_3D:
				case LX_PE_OUT_2D :
					pCfg->cap.h_line_num    = APR_MAX_LINE;
					pCfg->cap.top_offset    = APR_CAP_TOP_OFFSET;
					pCfg->cap.bot_offset    = APR_CAP_BOT_OFFSET;
					pCfg->cap.lr_crop       = APR_CAP_LR_CROP;
					break;
				case LX_PE_OUT_SS :
				case LX_PE_OUT_DUAL_SS:
					pCfg->cap.h_line_num    = 40;
					pCfg->cap.top_offset    = 64;
					pCfg->cap.bot_offset    = 0;
					pCfg->cap.lr_crop       = 32;
					break;
				case LX_PE_OUT_TB :
				case LX_PE_OUT_DUAL_TB:
				case LX_PE_OUT_3D_2D:
				case LX_PE_OUT_FS :
				case LX_PE_OUT_DUAL_FULL:
					pCfg->cap.h_line_num    = 40;
					pCfg->cap.top_offset    = 64;
					pCfg->cap.bot_offset    = 0;
					pCfg->cap.lr_crop       = 80;
					break;
				default:
					break;
			}
			if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_INFO)
				APR_MSG("%s : out_type  %d\n", __F__, pCfg->out_type);
			if(!ret) ret = RET_TRY_AGAIN;
		}
		cur_out_type = pCfg->out_type;

		if((cur_scan_info != pCfg->fd3.scan) || (cur_color_sample != pCfg->fd3.color) || (cur_input_height != pCfg->in_win_y) || (cur_out_type != pCfg->out_type))
		{
			APR_HAL_SetFd3Config(pCfg);
			if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_INFO)
				APR_MSG("%s : scan_info  %d , color_sample  %d , in_win_y  %d\n", __F__, pCfg->fd3.scan, pCfg->fd3.color, pCfg->in_win_y);
			if(!ret) ret = RET_TRY_AGAIN;
		}

		cur_scan_info    = pCfg->fd3.scan;
		cur_color_sample = pCfg->fd3.color;
		cur_input_height = pCfg->in_win_y;

#ifndef _DISABLE_ACCESS_TO_PE_
		if(pCfg->mute_status == 1)
#else
		if(0)
#endif
		{
			pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
			pCfg->apr_fmt = LX_APR_FMT_2D_2D;

			for(i = 0;i < APR_METHOD_MAX;i++)
			{
				for(j = 0;j < APR_MAX_BAK_FMT;j++)
					pCfg->voting_fmt[i][j] = LX_APR_FMT_2D_2D;
			}
			if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_INFO)
				APR_MSG("mute on -  initialize voting buffer to all 2D\n");
		}
	} while(0);

	return ret;
}

static int APR_CollectBasicData(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;

	do {
		ret = APR_HAL_CollectBasicData(pCfg, pData);
		if(ret) break;
		ret = APR_HAL_CollectFd3Data(pData);
	} while(0);

	return ret;
}

static int APR_GetMax3(UINT32 *g_diff, UINT32 *b_diff, UINT32 *r_diff, UINT32 *diff_max)
{
    int ret = RET_OK;

	do {
	    if(*g_diff>=*b_diff)
	    {
	    	if(*g_diff>=*r_diff)
	        	*diff_max = *g_diff;
	        else
				*diff_max = *r_diff;
	    }
	    else
	    {
	    	if(*b_diff>=*r_diff)
	        	*diff_max = *b_diff;
	        else
				*diff_max = *r_diff;
	    }
	} while(0);

    return ret;
}

static int APR_GetLrcrHistogram(void)
{
    int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	APR_STM_T *pStm  = &apr_info.stm;
	APR_HST_INFO_T hist_info;
	static UINT32 prev_lrc_count[4] = {0,0,0,0};
	static UINT32 prev_src_count[18] = {0};
	static LX_PE_OUT_TYPE prev_out_type = LX_PE_OUT_NUM;

	do {
		if(pCfg->hst_enb < LX_APR_HST_ON) break;
		hist_info.cur_fmt = pCfg->apr_fmt; 
		ret = APR_HAL_GetHistInfo(&hist_info);

		pCfg->out_type = hist_info.out_type;
		pCfg->src_type = hist_info.src_type;
		pCfg->in_f_rate = hist_info.in_f_rate;
		if(pCfg->out_type != prev_out_type) 
		{
			//prev_lrc_count[0]=prev_lrc_count[1]=prev_lrc_count[2]=prev_lrc_count[3]=0;
			memset(prev_lrc_count, 0, sizeof(UINT32)*4);
			memset(prev_src_count, 0, sizeof(UINT32)*18);
		}
		prev_out_type = pCfg->out_type;
		
		if( pCfg->out_type == LX_PE_OUT_3D_2D     || \
			pCfg->out_type == LX_PE_OUT_FS        || \
			pCfg->out_type == LX_PE_OUT_DUAL_FULL || \
			pCfg->out_type == LX_PE_OUT_TB        || \
			pCfg->out_type == LX_PE_OUT_SS        || \
			pCfg->out_type == LX_PE_OUT_LA        || \
			pCfg->out_type == LX_PE_OUT_DUAL_TB   || \
			pCfg->out_type == LX_PE_OUT_DUAL_SS )
			break;
			
		if((hist_info.lrc.set_cnt[0] == (hist_info.lrc.set_cnt[1])) \
				&& (prev_lrc_count[1] < hist_info.lrc.set_cnt[1]))
		{
			if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_HIST)
			{
				printk("!!! LRC Histogram difference win 0\n");
				printk("tb0 [%08d] ss0 [%08d]\n", hist_info.lrc.dif_max[0], hist_info.lrc.dif_max[1]);
				printk("pxl0[%08d] pxl1[%08d]\n", hist_info.lrc.pxl_cnt[0], hist_info.lrc.pxl_cnt[1]);
				printk("set0[%08d] set1[%08d]\n", hist_info.lrc.set_cnt[0], hist_info.lrc.set_cnt[1]);
			}
			prev_lrc_count[0] = hist_info.lrc.set_cnt[0];
			prev_lrc_count[1] = hist_info.lrc.set_cnt[1];
			if(hist_info.lrc.set_cnt[0] == 255)
			{
				prev_lrc_count[0] = 0;
				prev_lrc_count[1] = 0;
			}

			/* [0]tb0,[1]ss0,[2]tb1,[3]ss1 */
			pData->hist_lrc.tb_diff = hist_info.lrc.dif_max[0];
			pData->hist_lrc.lr_diff = hist_info.lrc.dif_max[1];
			pData->hist_lrc.tb_ratio = hist_info.lrc.pxl_cnt[0];
			pData->hist_lrc.ss_ratio = hist_info.lrc.pxl_cnt[1];

			pStm->win_pos  = APR_WIN_LRTB_HAF1;
			pData->lrc_complete_flag = TRUE; 
		}
		else if((hist_info.lrc.set_cnt[2] == (hist_info.lrc.set_cnt[3])) \
				&& (prev_lrc_count[3] < hist_info.lrc.set_cnt[3]))
		{
			if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_HIST)
			{
				printk("!!! LRC Histogram difference win 1\n");
				printk("tb1 [%08d] ss1 [%08d]\n", hist_info.lrc.dif_max[2], hist_info.lrc.dif_max[3]);
				printk("pxl2[%08d] pxl3[%08d]\n", hist_info.lrc.pxl_cnt[2], hist_info.lrc.pxl_cnt[3]);
				printk("set2[%08d] set3[%08d]\n", hist_info.lrc.set_cnt[2], hist_info.lrc.set_cnt[3]);
			}
			prev_lrc_count[2] = hist_info.lrc.set_cnt[2];
			prev_lrc_count[3] = hist_info.lrc.set_cnt[3];
			if(hist_info.lrc.set_cnt[2] == 255)
			{
				prev_lrc_count[2] = 0;
				prev_lrc_count[3] = 0;
			}
			pData->hist_lrc.tb_diff = hist_info.lrc.dif_max[2];
			pData->hist_lrc.lr_diff = hist_info.lrc.dif_max[3];
			pData->hist_lrc.tb_ratio = hist_info.lrc.pxl_cnt[2];
			pData->hist_lrc.ss_ratio = hist_info.lrc.pxl_cnt[3];
			
			pStm->win_pos  = APR_WIN_LRTB_HAF2;
			pData->lrc_complete_flag = TRUE; 
		}
		else
		{
			if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_HIST)
			{
				printk("!!! set_cnt[0/1/2/3] [%d/%d/%d/%d]\n",\
						hist_info.lrc.set_cnt[0],hist_info.lrc.set_cnt[1],\
						hist_info.lrc.set_cnt[2],hist_info.lrc.set_cnt[3]);
			}
			pData->lrc_complete_flag = FALSE; 
		}

		if (lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) ) {
			pData->tpd = hist_info.src.s_state[0];
		}
	} while(0);
	
	return ret;
}

static int APR_GetDceHistogram(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;

	do {
		if(pCfg->hst_enb < LX_APR_HST_ON) break;
		if(pCfg->out_type == LX_PE_OUT_2D)    break;
		if(pCfg->out_type == LX_PE_OUT_2D_3D) break;
		if(pCfg->out_type == LX_PE_OUT_UD)    break;
		if(pCfg->out_type == LX_PE_OUT_PIP)   break;
	} while(0);
	return ret;
}

static int APR_GetOVInfo(			APR_CAP_SRC_T  ov_src,       \
									APR_OV_INFO_T *ov_info)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;

	ret = APR_HAL_GetOVInfo(pCfg, ov_src, ov_info);

	return ret;
}

#ifdef APR_USE_LINE_DIFF_BY_SW
static int APR_GetStripFromFrame(	APR_CAP_SRC_T   ov_src,      \
									APR_OV_INFO_T  *ov_info,     \
									APR_CAP_INFO_T *capture_info,\
									UINT32 h_line_num,           \
									UINT32 h_line_width,         \
									UINT32 v_line_num,           \
									UINT32 v_line_width,         \
									UINT32 top_offset,           \
									UINT32 bot_offset,           \
									UINT32 lr_crop)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	UINT32 i,j;
#ifdef APR_USE_VLINE
	UINT32 h_mem_offset = 0;
#endif
	UINT32 v_mem_offset = 0;
	UINT32 sampling_size = 0;
	UINT32 buf_index = 0;
	UINT32 buf_index_max = 0;

	do {
		if(ov_info->pFrameBase == NULL) 
		{
			if(pCfg->cap.enb >= LX_APR_CAP_LOG) APR_MSG("%s : pFrameBase = 0x%08x\n", __F__, (UINT32)ov_info->pFrameBase);
			break;
		}
		if(ov_info->src_valid == FALSE)
		{
			if(pCfg->cap.enb >= LX_APR_CAP_LOG) APR_MSG("%s : src_valid [ %d ]\n", __F__, ov_info->src_valid);
			ret = RET_ERROR;
			break;
		}
		if((h_line_num > APR_MAX_LINE) || (h_line_width > APR_MAX_STRIP_WIDTH) || (ov_info->v < (top_offset<<1)))
		{
			if(pCfg->cap.enb >= LX_APR_CAP_LOG) APR_MSG("h_size[%d]/v_size[%d]/top_offset[%d]/bot_offset[%d]/lr_crop[%d]/h_line_num[%d]/h_line_width[%d]\n", \
															ov_info->h, ov_info->v, top_offset, bot_offset, lr_crop, h_line_num, h_line_width);
			ret = RET_ERROR;
			break;
		}

		capture_info->h_size = ov_info->h;
		capture_info->v_size = ov_info->v;
		capture_info->stride = ov_info->stride;
		capture_info->h_line_num = h_line_num;
		capture_info->top_offset = top_offset;
		capture_info->bot_offset = bot_offset;
		capture_info->lr_crop    = lr_crop;
		capture_info->frame_base_phys[0] = ov_info->frame_base_phys[0];
		capture_info->frame_base_phys[1] = ov_info->frame_base_phys[1];

		if((capture_info->v_size>>1) <= (top_offset + bot_offset)) break;

		switch(ov_src)
		{
			case APR_CAP_SRC_R_PRW :
			case APR_CAP_SRC_R_TNR :
			case APR_CAP_SRC_R_OVR :
				buf_index     = h_line_num;
				buf_index_max = (h_line_num << 1);
				sampling_size = ((capture_info->v_size>>0) - top_offset - bot_offset)/(h_line_num>>0);
				break;
			case APR_CAP_SRC_L_PRW :
			case APR_CAP_SRC_L_TNR :
			case APR_CAP_SRC_L_OVR : 
				buf_index     = 0;
				buf_index_max = h_line_num;
				sampling_size = ((capture_info->v_size>>0) - top_offset - bot_offset)/(h_line_num>>0);
				break;
			case APR_CAP_SRC_S_CVR :
			default:
				buf_index     = 0;
				buf_index_max = (h_line_num >> 1);
				sampling_size = ((capture_info->v_size>>1) - top_offset - bot_offset)/(h_line_num>>1);
				break;
		}

		if(sampling_size > (APR_MAX_VSIZE>>1))
		{
			if(pCfg->cap.enb >= LX_APR_CAP_FMT) APR_MSG("sampling_size  %d\n", sampling_size);
			ret = RET_ERROR;
			break;
		}

		if(pCfg->cap.enb == LX_APR_CAP_LOG) APR_MSG("pFrameBase : 0x%08X, h/v/s[%d/%d/%d]  h_line/h_width/v_line/v_width [%d/%d/%d/%d] top/bot/lr_crop [%d/%d/%d] -> sampling_size[%d]\n",\
										(UINT32)ov_info->frame_base_phys[0], ov_info->h, ov_info->v, ov_info->stride, h_line_num, h_line_width, v_line_num, v_line_width, \
										top_offset, bot_offset, lr_crop, sampling_size);
	
		// h line capture
		v_mem_offset = top_offset * ov_info->stride;
		for(i=buf_index;i<buf_index_max;i++)
		{
			for(j=0;j<h_line_width;j++)
			{
				/*debug*/if(pCfg->cap.enb >= LX_APR_CAP_DRAW) memset((UINT8*)(ov_info->pFrameBase + v_mem_offset + ov_info->stride * j + lr_crop), 0, ov_info->h - (lr_crop<<1));
				else memcpy((UINT8*)(capture_info->pHLine[i] + ov_info->h * j), (UINT8*)(ov_info->pFrameBase + v_mem_offset + ov_info->stride * j + lr_crop), ov_info->h - (lr_crop<<1));
			}
			v_mem_offset += sampling_size * ov_info->stride;
		}

		if(pCfg->out_type != LX_PE_OUT_2D) break;
		/*debug*/if(pCfg->cap.enb == LX_APR_CAP_DRW3) break;

		// h line capture for center difference (Delta)
		v_mem_offset = ((capture_info->v_size>>1) - (APR_MAX_CENTER_LINE>>1)) * ov_info->stride;
		for(i=0;i<APR_MAX_CENTER_LINE;i++)
		{
			/*debug*/if(pCfg->cap.enb == LX_APR_CAP_DRAW) memset((UINT8*)(ov_info->pFrameBase + v_mem_offset + ov_info->stride * i + lr_crop), 0x80, ov_info->h - (lr_crop<<1));
			else memcpy((UINT8*)(capture_info->pHLineCenter[i]), (UINT8*)(ov_info->pFrameBase + v_mem_offset + ov_info->stride * i + lr_crop), ov_info->h - (lr_crop<<1));
		}

		// center line drawing for debug
		v_mem_offset = ((capture_info->v_size>>1) - 1) * ov_info->stride;
		/*debug*/if(pCfg->cap.enb == LX_APR_CAP_DRAW) memset((UINT8*)(ov_info->pFrameBase + v_mem_offset + ov_info->stride * 1 + lr_crop), 0, ov_info->h - (lr_crop<<1));

		// h line capture
		v_mem_offset = (top_offset + (capture_info->v_size>>1)) * ov_info->stride;
		for(i=(h_line_num>>1);i<h_line_num;i++)
		{
			for(j=0;j<h_line_width;j++)
			{
				/*debug*/if(pCfg->cap.enb >= LX_APR_CAP_DRAW) memset((UINT8*)(ov_info->pFrameBase + v_mem_offset + ov_info->stride * j + lr_crop), 0, ov_info->h - (lr_crop<<1));
				else memcpy((UINT8*)(capture_info->pHLine[i] + ov_info->h * j), (UINT8*)(ov_info->pFrameBase + v_mem_offset + ov_info->stride * j + lr_crop), ov_info->h - (lr_crop<<1));
			}
			v_mem_offset += sampling_size * ov_info->stride;
		}

#ifdef APR_USE_VLINE
		// v line capture
		capture_info->v_pos_offset = ov_info->h / (v_line_num + 1);
		capture_info->v_line_num = v_line_num;
		h_mem_offset = capture_info->v_pos_offset;
		for(i=0;i<v_line_num;i++)
		{
			for(j=0;j<ov_info->v;j++)
			{
				/*debug*/if(pCfg->cap.enb == LX_APR_CAP_DRAW) memset((UINT8*)(ov_info->pFrameBase + h_mem_offset + ov_info->stride * j), 16, v_line_width);
				else memcpy((UINT8*)(capture_info->pVLine[i] + v_line_width * j), (UINT8*)(ov_info->pFrameBase + h_mem_offset + ov_info->stride * j), v_line_width);
			}
			h_mem_offset += capture_info->v_pos_offset;
		}
#endif
	} while(0);

	return ret;
}

				
static int APR_GetStripForFd3(APR_OV_INFO_T  *ov_info, APR_CAP_INFO_T *capture_info)
{
#define LEFT_WIN_X_OFFSET   30 // x offset of w0, w2
#define RIGHT_WIN_X_OFFSET  23 // x offset of w1, w3
#define BOTTOM_WIN_Y_OFFSET 0 // y offset of w2, w3
#define BOTTOM_WIN_X_OFFSET (0)

	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	int i;
	int	h_line_num      = 80;
	int top_offset      = 32;
	int width_in_win    = 370;
	int x_pos_in_win    = 0;
	int sampling_size   = (int)(((ov_info->v>>1) - (top_offset<<1))/(h_line_num>>1));
	int x_w0,x_w1,x_w2,x_w3;
	int y_w0,y_w1,y_w2,y_w3;
	int x_pos_offset[2] = {0, 0}; 
	static int x_pos_idx = 0;

	do {
		if(ov_info->pFrameBase == NULL) 
		{
			if(pCfg->cap.enb >= LX_APR_CAP_LOG) APR_MSG("%s : pFrameBase = 0x%08x\n", __F__, (UINT32)ov_info->pFrameBase);
			break;
		}
		if(ov_info->src_valid == FALSE)
		{
			if(pCfg->cap.enb >= LX_APR_CAP_LOG) APR_MSG("%s : src_valid [ %d ]\n", __F__, ov_info->src_valid);
			ret = RET_ERROR;
			break;
		}

		capture_info->h_size     = width_in_win<<1; // 740
		capture_info->h_line_num = h_line_num; // 80 lines

		if(pCfg->cap.enb == LX_APR_CAP_LOG)
		{
			APR_MSG("In Out info : in_offset h/v[ %d / %d ] in_size h/v[ %d / %d ]  out_size[ %d / %d ]\n",\
					ov_info->input.h_offset, ov_info->input.v_offset, ov_info->input.h_size, ov_info->input.v_size,\
					ov_info->h, ov_info->v);
			APR_MSG("pFrameBase : 0x%08X, line_num[%d] x_pos[%d] y_pos[%d] width[%d] v_sampling_size[%d]\n",\
				(UINT32)ov_info->frame_base_phys[0], h_line_num, x_pos_in_win, top_offset, width_in_win, sampling_size);
		}
		
		y_w0 = top_offset * ov_info->stride;
		y_w1 = top_offset * ov_info->stride;
		y_w2 = (top_offset + BOTTOM_WIN_Y_OFFSET + (ov_info->v>>1)) * ov_info->stride;
		y_w3 = (top_offset + BOTTOM_WIN_Y_OFFSET + (ov_info->v>>1)) * ov_info->stride;

		x_pos_offset[0] = (ov_info->h >> 1) >> 3; // half width / 8
		x_pos_offset[1] = (ov_info->h >> 2) - 50; // spread capture position 

		for(i=0;i<(h_line_num>>1);i++)
		{
			x_pos_in_win = x_pos_offset[x_pos_idx++];
			if(x_pos_idx==2) x_pos_idx = 0;

			x_w0 = LEFT_WIN_X_OFFSET + x_pos_in_win;
			x_w1 = x_pos_in_win + (ov_info->h>>1) + RIGHT_WIN_X_OFFSET;
			x_w2 = LEFT_WIN_X_OFFSET + x_pos_in_win + BOTTOM_WIN_X_OFFSET;
			x_w3 = x_pos_in_win + (ov_info->h>>1) + RIGHT_WIN_X_OFFSET + BOTTOM_WIN_X_OFFSET;

			if(pCfg->cap.enb == LX_APR_CAP_DRAW)
			{
				// w0
				memset((UINT8*)(ov_info->pFrameBase + x_w0 + y_w0), ((i%2)?0:0x80), width_in_win);
				// w1
				memset((UINT8*)(ov_info->pFrameBase + x_w1 + y_w1),	((i%2)?0:0x80), width_in_win);
				// w2
				memset((UINT8*)(ov_info->pFrameBase + x_w2 + y_w2), ((i%2)?0:0x80), width_in_win);
				// w3
				memset((UINT8*)(ov_info->pFrameBase + x_w3 + y_w3),	((i%2)?0:0x80), width_in_win);
			}
			else
			{
				// w0
				memcpy((UINT8*)(capture_info->pHLine[i]),\
						(UINT8*)(ov_info->pFrameBase + x_w0 + y_w0), width_in_win);
				// w1
				memcpy((UINT8*)(capture_info->pHLine[i] + width_in_win),\
						(UINT8*)(ov_info->pFrameBase + x_w1 + y_w1), width_in_win);
				// w2
				memcpy((UINT8*)(capture_info->pHLine[i + (h_line_num>>1)]),\
						(UINT8*)(ov_info->pFrameBase + x_w2 + y_w2), width_in_win);
				// w3
				memcpy((UINT8*)(capture_info->pHLine[i + (h_line_num>>1)] + width_in_win),\
						(UINT8*)(ov_info->pFrameBase + x_w3 + y_w3), width_in_win);
			}
			y_w0 += sampling_size * ov_info->stride;
			y_w1 += sampling_size * ov_info->stride;
			y_w2 += sampling_size * ov_info->stride;
			y_w3 += sampling_size * ov_info->stride;
		}

		// h line capture for center difference (Delta)
		x_w0 = (ov_info->h>>1) - 720/2;
		y_w0 = ((ov_info->v>>1) - 4) * ov_info->stride;
		for(i=0;i<APR_MAX_CENTER_LINE;i++)
		{
			if(pCfg->cap.enb == LX_APR_CAP_DRAW) 
				memset((UINT8*)(ov_info->pFrameBase + x_w0 + y_w0 + ov_info->stride * i), 0x80, 720);
			else
				memcpy((UINT8*)(capture_info->pHLineCenter[i]), \
						(UINT8*)(ov_info->pFrameBase + x_w0 + y_w0 + ov_info->stride * i), 720);
		}
 
	  	// v line capture for center difference (Delta)
		x_w0 = (ov_info->h>>1) - 4;
		y_w0 = (top_offset<<0) * ov_info->stride;
		sampling_size = (int)((ov_info->v - (top_offset<<2))/h_line_num);
		for(i=0;i<h_line_num;i++)
		{
			if(pCfg->cap.enb == LX_APR_CAP_DRAW) 
				memset((UINT8*)(ov_info->pFrameBase + x_w0 + y_w0 + ov_info->stride * i), 0x80, 9);
			else
				memcpy((UINT8*)(capture_info->pVLine[0] + i*9), \
						(UINT8*)(ov_info->pFrameBase + x_w0 + y_w0 + ov_info->stride * i), 9);
			y_w0 += sampling_size * ov_info->stride;
		}

		// center grid for debug
		if(pCfg->cap.enb == LX_APR_CAP_DRAW)
		{
			x_w0 = 0;
			y_w0 = ov_info->v/2 * ov_info->stride;
			memset((UINT8*)(ov_info->pFrameBase + x_w0 + y_w0), 0, ov_info->h);
			for(i=0;i<(ov_info->v);i=i+2)memset((UINT8*)(ov_info->pFrameBase + (ov_info->h>>1) + ov_info->stride*i), 0, 1);
		}
	} while(0);

	return ret;
}

static int APR_InitFrameMemory(UINT32 h_max_line, UINT32 v_max_line)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	UINT32 line_size  = 0;
	UINT32 total_size = 0;
	UINT32 i;

	do {
		if(h_max_line > APR_MAX_LINE)
			break;
		if(v_max_line > APR_MAX_LINE)
			break;

		for(i=0;i<h_max_line;i++) capture_info.pHLine[i] = NULL;
#ifdef APR_USE_VLINE
		for(i=0;i<v_max_line;i++) capture_info.pVLine[i] = NULL;
#endif
		for(i=0;i<APR_MAX_CENTER_LINE;i++) capture_info.pHLineCenter[i] = NULL;

		line_size = APR_MAX_HSIZE * APR_MAX_STRIP_WIDTH;
		for(i=0;i<h_max_line;i++)
		{
			capture_info.pHLine[i] = (UINT8*)vmalloc(line_size);
			total_size += line_size;
		}
#ifdef APR_USE_VLINE
		line_size = APR_MAX_VSIZE * APR_MAX_STRIP_WIDTH;
		for(i=0;i<v_max_line;i++)
		{
			capture_info.pVLine[i] = (UINT8*)vmalloc(line_size);
			total_size += line_size;
		}
#endif
		line_size = APR_MAX_HSIZE * APR_MAX_CENTER_LINE;
		for(i=0;i<APR_MAX_CENTER_LINE;i++)
		{
			capture_info.pHLineCenter[i] = (UINT8*)vmalloc(line_size);
			total_size += line_size;
		}

		pCfg->mem_usage = total_size;
	} while(0);

	return ret;
}

static int APR_UnInitFrameMemory(void)
{
	int ret = RET_OK;
	UINT32 i;

	do {
		for(i=0;i<APR_MAX_LINE;i++)	if(capture_info.pHLine[i] != NULL) OS_Free(capture_info.pHLine[i]);
#ifdef APR_USE_VLINE
		for(i=0;i<APR_MAX_LINE   ;i++) 	if(capture_info.pVLine[i] != NULL) OS_Free(capture_info.pVLine[i]);
#endif
		for(i=0;i<APR_MAX_CENTER_LINE;i++)	if(capture_info.pHLineCenter[i] != NULL) OS_Free(capture_info.pHLineCenter[i]);

	} while(0);

	return ret;
}
#endif //#ifdef APR_USE_LINE_DIFF_BY_SW

static int APR_GetUSecTimerVal(UINT32 *tv)
{
	int ret = RET_OK;
	ret = APR_HAL_GetUSecTimerVal(tv);
	return ret;
}

static int APR_CheckTime(char *fn_str, APR_GET_TIME_T location, APR_TIME_T pos)
{
	int ret = RET_OK;
	static APR_TIME_INFO_T time_info;
	APR_CFG_T *pCfg = &apr_info.cfg;
	UINT32 time_diff;
	UINT32 usec;

	do {
		switch(pos)
		{
			case APR_GET_START :
				APR_GetUSecTimerVal(&usec);
				time_info.tick_start_time[location] = usec;
				break;
			default :
			case APR_GET_END :
				APR_GetUSecTimerVal(&usec);
				time_info.tick_end_time[location] = usec;
				if(time_info.tick_end_time[location] >= time_info.tick_start_time[location])
					time_diff = time_info.tick_end_time[location] - time_info.tick_start_time[location];
				else
					time_diff = time_info.tick_start_time[location] + (0xFFFFFFFF - time_info.tick_end_time[location]);
				time_diff = (time_diff==0)?1:time_diff;
				if(location == APR_GET_TIME_CAP)
					pCfg->time.capture = time_diff;
				else if(location == APR_GET_TIME_MAT)
					pCfg->time.measure = time_diff;
				else if(location == APR_GET_TIME_DRW)
					pCfg->time.draw    = time_diff;
				else if(location == APR_GET_TIME_FD3)
					pCfg->time.fd3     = time_diff;
				if(pCfg->cap.enb == LX_APR_CAP_TIME) APR_MSG("%s :	%d usec  elapsed\n", fn_str, time_diff);
				break;
		}
	} while(0);

	return ret;
}

static int APR_CaptureFrame(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_OV_INFO_T ov_info;
	APR_CAP_INFO_T *pCapture;
	APR_CB_MEASURE_T *pCb_ldf = &fn_measure;
	APR_CB_CAPTURE_T *pCb_cap = &fn_capture;

	do {
		pCapture = &capture_info;
		memset(&ov_info, 0, sizeof(APR_OV_INFO_T));
		
		/*debug*/if(pCfg->cap.enb == LX_APR_CAP_DRW3) // for 3D image capure
		{
			ret = APR_GetOVInfo(APR_CAP_SRC_L_OVR, &ov_info);
			ret = pCb_cap->d2d(APR_CAP_SRC_L_OVR,&ov_info,pCapture,\
					40/*pCfg->cap.h_line_num*/, \
					1/*pCfg->cap.h_line_width*/,\
					0/*pCfg->cap.v_line_num*/,  \
					0/*pCfg->cap.v_line_width*/,\
					64/*pCfg->cap.top_offset*/, \
					0/*pCfg->cap.bot_offset*/,  \
					128/*pCfg->cap.lr_crop*/);
			ret = APR_GetOVInfo(APR_CAP_SRC_R_OVR, &ov_info);
			ret = pCb_cap->d2d(APR_CAP_SRC_R_OVR,&ov_info,pCapture,\
					40/*pCfg->cap.h_line_num*/, \
					1/*pCfg->cap.h_line_width*/,\
					0/*pCfg->cap.v_line_num*/,  \
					0/*pCfg->cap.v_line_width*/,\
					64/*pCfg->cap.top_offset*/, \
					0/*pCfg->cap.bot_offset*/,  \
					128/*pCfg->cap.lr_crop*/);
			break;
		}

		switch(pCfg->out_type)
		{
			case LX_PE_OUT_2D :
			case LX_PE_OUT_2D_3D :
				if(pCfg->fd3.enb)
				{
					APR_CheckTime("Capture line", APR_GET_TIME_CAP, APR_GET_START);
					ret = APR_GetOVInfo(APR_CAP_SRC_L_TNR, &ov_info);
					ret = pCb_cap->fd3(&ov_info,pCapture);
					//ret = pCb_ldf->fd3(pCapture);
					ret = pCb_ldf->d3d(pCapture);
					APR_CheckTime("Capture line", APR_GET_TIME_CAP, APR_GET_END);
				}
				else
				{
					APR_CheckTime("Capture line", APR_GET_TIME_CAP, APR_GET_START);
					if(pCfg->cap.enb >= LX_APR_CAP_DRAW) /*debug*/
						ret = APR_GetOVInfo(APR_CAP_SRC_L_OVR, &ov_info);
					else 
						ret = APR_GetOVInfo(APR_CAP_SRC_S_CVR, &ov_info);
					if (ret) break;
					ret = pCb_cap->d3d(APR_CAP_SRC_S_CVR,               \
							&ov_info,              \
							pCapture,              \
							pCfg->cap.h_line_num,  \
							pCfg->cap.h_line_width,\
							pCfg->cap.v_line_num,  \
							pCfg->cap.v_line_width,\
							32,                    \
							0,                     \
							0);
					if (ret) break;

					APR_CheckTime("Capture line", APR_GET_TIME_CAP, APR_GET_END);
					APR_CheckTime("Measure difference", APR_GET_TIME_MAT, APR_GET_START);
					ret = pCb_ldf->d3d(pCapture);
					APR_CheckTime("Measure difference", APR_GET_TIME_MAT, APR_GET_END);
				}
				break;
			case LX_PE_OUT_3D_2D :
			case LX_PE_OUT_FS :
			case LX_PE_OUT_DUAL_FULL :

			case LX_PE_OUT_TB :
			case LX_PE_OUT_SS :
			case LX_PE_OUT_LA :
			case LX_PE_OUT_DUAL_TB :
			case LX_PE_OUT_DUAL_SS :
				APR_CheckTime("Capture line", APR_GET_TIME_CAP, APR_GET_START);
				ret = APR_GetOVInfo(APR_CAP_SRC_L_OVR, &ov_info);
				if (ret) break;
				ret = pCb_cap->d2d(APR_CAP_SRC_L_OVR,              \
											&ov_info,              \
											pCapture,              \
											pCfg->cap.h_line_num,  \
											pCfg->cap.h_line_width,\
											pCfg->cap.v_line_num,  \
											pCfg->cap.v_line_width,\
											pCfg->cap.top_offset,  \
											pCfg->cap.bot_offset,  \
											pCfg->cap.lr_crop);
				if (ret) break;
				ret = APR_GetOVInfo(APR_CAP_SRC_R_OVR, &ov_info);
				if (ret) break;
				ret = pCb_cap->d2d(APR_CAP_SRC_R_OVR,               \
											&ov_info,              \
											pCapture,              \
											pCfg->cap.h_line_num,  \
											pCfg->cap.h_line_width,\
											pCfg->cap.v_line_num,  \
											pCfg->cap.v_line_width,\
											pCfg->cap.top_offset,  \
											pCfg->cap.bot_offset,  \
											pCfg->cap.lr_crop);
				if (ret) break;
				APR_CheckTime("Capture line", APR_GET_TIME_CAP, APR_GET_END);
				APR_CheckTime("Measure difference", APR_GET_TIME_MAT, APR_GET_START);
				ret = pCb_ldf->d2d(pCapture);
				APR_CheckTime("Measure difference", APR_GET_TIME_MAT, APR_GET_END);

				break;
			default:
				break;
		}
	} while(0);

	return ret;
}

static int APR_ProcessFD3(void)
{
	int ret = RET_OK;
#ifdef APR_USE_FD3 
	APR_CFG_T *pCfg  = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	APR_CB_MEASURE_T *pCb_fd3 = &fn_measure;

	do {
		switch(pCfg->out_type)
		{
			case LX_PE_OUT_3D_2D :
			case LX_PE_OUT_FS :
			case LX_PE_OUT_DUAL_FULL :
			case LX_PE_OUT_TB :
			case LX_PE_OUT_SS :
			case LX_PE_OUT_LA :
			case LX_PE_OUT_DUAL_TB :
			case LX_PE_OUT_DUAL_SS :
				// when 3D case, ignore the result of  m1, m2 
				pCfg->fmt_before_vote[APR_METHOD_1] = LX_APR_FMT_2D_2D;
				pCfg->fmt_before_vote[APR_METHOD_2] = LX_APR_FMT_2D_2D;
				pCfg->img_class = APR_IMG_CLASS_2D_HARD;
				pData->tpd      = 0;
			case LX_PE_OUT_2D :
			case LX_PE_OUT_2D_3D :
			case LX_PE_OUT_PIP :
			default :
				APR_CheckTime("Measure fd3", APR_GET_TIME_FD3, APR_GET_START);
				ret = pCb_fd3->fd3();			
				APR_CheckTime("Measure fd3", APR_GET_TIME_FD3, APR_GET_END);
				break;
		}
	} while(0);
#endif
	return ret;
}

static int APR_AccumulateFormat(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	LX_APR_FORMAT cur_fmt = pCfg->tmp_fmt;
	UINT32 sum = 0;
	UINT32 ratio_2d;
	UINT32 ratio_ss;
	UINT32 ratio_tb;
	UINT32 max;

	do {
		if(!pCfg->cnt_start)
		{
			pCfg->cnt_2d = 0;
			pCfg->cnt_ss = 0;
			pCfg->cnt_tb = 0;
			break;
		}
		else
		{
			if((cur_fmt == LX_APR_FMT_2D_2D) || (cur_fmt == LX_APR_FMT_UNKNOWN))
				pCfg->cnt_2d++;
			else if(cur_fmt == LX_APR_FMT_3D_SS)
				pCfg->cnt_ss++;
			else if(cur_fmt == LX_APR_FMT_3D_TB)
				pCfg->cnt_tb++;
			else
				pCfg->cnt_2d++;

			sum = pCfg->cnt_2d + pCfg->cnt_ss + pCfg->cnt_tb;
			ratio_2d = (UINT32)((pCfg->cnt_2d * 100) / sum);
			ratio_ss = (UINT32)((pCfg->cnt_ss * 100) / sum);
			ratio_tb = (UINT32)((pCfg->cnt_tb * 100) / sum);

			APR_GetMax3(&ratio_2d, &ratio_ss, &ratio_tb, &max);
			if(max < 50)
			{
				pCfg->acc_fmt = LX_APR_FMT_2D_2D;
				break;
			}

			if(ratio_2d >= ratio_ss)
			{
				if(ratio_tb > ratio_2d)
					pCfg->acc_fmt = LX_APR_FMT_3D_TB;
				else
					pCfg->acc_fmt = LX_APR_FMT_2D_2D;
			}
			else
			{
				if(ratio_tb > ratio_ss)
					pCfg->acc_fmt = LX_APR_FMT_3D_TB;
				else
					pCfg->acc_fmt = LX_APR_FMT_3D_SS;
			}
		}
	} while(0);

	return ret;
}

static int APR_VotingFormat(APR_METHOD_T method_idx, LX_APR_FORMAT fmt, BOOLEAN vote_enable)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_THR_T *pThr = &apr_info.thr;
	UINT32 i = 0;
	UINT32 vld_thr = 0;
	LX_APR_FORMAT cur_fmt = LX_APR_FMT_2D_2D;
	UINT32 vote_nominee[LX_APR_FMT_MAX];
	BOOLEAN	valid_flag = FALSE;
	UINT32 temp =0;
	
	do {
		pCfg->fmt_before_vote[method_idx] = fmt;
		if(!vote_enable)
		{
			pCfg->fmt_after_vote[method_idx] = fmt;
			break;
		}

		cur_fmt = fmt;
		vld_thr = pThr->vld_thr;

		for(i = 0;i< LX_APR_FMT_MAX; i++) vote_nominee[i] = 0;
		for(i = 0;i < vld_thr- 1;i++)
		{
			pCfg->voting_fmt[method_idx][vld_thr - 1 - i] = pCfg->voting_fmt[method_idx][vld_thr - 2 - i];
			temp = pCfg->voting_fmt[method_idx][vld_thr - 1 - i];	 // To prevent latchup from dependency errors... (incase of error from out-of-order execution processor)
			if(gfInputSourceChanged)
			{
				if((i<(APR_VOTE_STACK_SIZE_ON_FORMAT_CHANGE-1))&&(temp < LX_APR_FMT_MAX)) vote_nominee[temp] += 1;
			}
			else
			{
				if(temp < LX_APR_FMT_MAX) vote_nominee[temp] += 1;
			}
		}
		pCfg->voting_fmt[method_idx][0] = cur_fmt;
		temp = cur_fmt;					// To prevent latchup from dependency errors... (incase of error from out-of-order execution processor)
		vote_nominee[temp] += 1;

		for(i = 0;i< LX_APR_FMT_MAX; i++)
		{
			if(gfInputSourceChanged)
			{
				if((i != LX_APR_FMT_UNKNOWN)&&(vote_nominee[i] >= (APR_VOTE_STACK_SIZE_ON_FORMAT_CHANGE - APR_MAX_MINORITY_VOTE_VAL_CHANGED)))
				{
					valid_flag = TRUE;
					cur_fmt = i;
				}
			}
			else
			{
				if((i != LX_APR_FMT_UNKNOWN)&&(vote_nominee[i] >= (vld_thr - APR_MAX_MINORITY_VOTE_VAL_NORMAL)))
				{
					valid_flag = TRUE;
					cur_fmt = i;
				}
			}
		}
		
		if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_INFO) {
			printk("Voting -  method [ %d ] \n", method_idx + 1);
			for(i=0;i<vld_thr;i++) {
				printk("%d  ", pCfg->voting_fmt[method_idx][i]);
			}
			if(valid_flag) printk("   final => %d\n", cur_fmt);
			else printk("\n");
		}

		if(valid_flag)
		{
			pCfg->fmt_after_vote[method_idx] = cur_fmt;
		}
		pCfg->valid = valid_flag;

	} while(0);
	return ret;
}

static int APR_NegotiateFormat(BOOLEAN nego_enble)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	APR_THR_T *pThr = &apr_info.thr;

	do {
		if(!nego_enble)
		{
			pCfg->apr_fmt = pCfg->fmt_after_vote[APR_METHOD_3];
			break;
		}
		if(!pCfg->cap.enb)
		{
			pCfg->apr_fmt = pCfg->fmt_after_vote[APR_METHOD_3];
			break;
		}

		if((pCfg->out_type == LX_PE_OUT_2D) || (pCfg->out_type == LX_PE_OUT_2D_3D) || (pCfg->out_type == LX_PE_OUT_PIP))
		{
			if(pCfg->cap.enb == LX_APR_CAP_LOG) {
				APR_MSG("Nego : %d   %d\n",pCfg->fmt_before_vote[APR_METHOD_2],  pCfg->fmt_after_vote[APR_METHOD_3]);
			}

			if((pCfg->fmt_after_vote[APR_METHOD_3] == LX_APR_FMT_3D_SS) || (pCfg->fmt_after_vote[APR_METHOD_3] == LX_APR_FMT_3D_TB))
			{
				int i;
				int ss_count = 0, tb_count = 0, unk_count = 0;
				for(i=0;i<pThr->vld_thr;i++)
				{
					if(pCfg->voting_fmt[APR_METHOD_2][i] == LX_APR_FMT_UNKNOWN) unk_count++;
					if(pCfg->voting_fmt[APR_METHOD_2][i] == LX_APR_FMT_3D_SS  ) ss_count++;
					if(pCfg->voting_fmt[APR_METHOD_2][i] == LX_APR_FMT_3D_TB  ) tb_count++;
				}
				if(pCfg->cap.enb == LX_APR_CAP_LOG){APR_MSG("ss[%d] tb[%d] unk[%d] tpd[%d]\n", ss_count, tb_count, unk_count, pData->tpd);}

				if( ((pCfg->fmt_after_vote[APR_METHOD_3] == LX_APR_FMT_3D_SS) && (ss_count <= 2) && (ss_count >= tb_count)) ||\
						((pCfg->fmt_after_vote[APR_METHOD_3] == LX_APR_FMT_3D_TB) && (tb_count <= 2) && (ss_count <= tb_count))  )
				{
					if(pCfg->fmt_after_vote[APR_METHOD_1] != pCfg->fmt_after_vote[APR_METHOD_3] && (pCfg->fmt_before_vote[APR_METHOD_2] != LX_APR_FMT_UNKNOWN))
					{
						pCfg->apr_fmt = LX_APR_FMT_2D_2D;
						if(pCfg->cap.enb == LX_APR_CAP_LOG){APR_MSG("case 1 \n");}
						break;
					}
				}
				if( ((pCfg->fmt_after_vote[APR_METHOD_2] == LX_APR_FMT_3D_SS) && ( pCfg->fmt_after_vote[APR_METHOD_3] == LX_APR_FMT_3D_TB)) || \
						((pCfg->fmt_after_vote[APR_METHOD_2] == LX_APR_FMT_3D_TB) && ( pCfg->fmt_after_vote[APR_METHOD_3] == LX_APR_FMT_3D_SS))    )
				{
					pCfg->apr_fmt = LX_APR_FMT_2D_2D;
					if(pCfg->cap.enb == LX_APR_CAP_LOG){APR_MSG("case 2 -  tpd[%d]\n", pData->tpd);}
					break;
				}
			}		
			pCfg->apr_fmt = pCfg->fmt_after_vote[APR_METHOD_3];
		}
		else
		{
			pCfg->apr_fmt = pCfg->fmt_after_vote[APR_METHOD_3];
		}
	} while(0);

	return ret;
}

static int APR_DumpHistogram(void)
{
	int ret = RET_OK;
	UINT32 itr = 0;
	UINT32 sum = 0;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	UINT32 lr_g_diff, lr_b_diff, lr_r_diff, tb_g_diff, tb_b_diff, tb_r_diff;
	UINT32 lr_g_sum = 0, lr_b_sum = 0, lr_r_sum = 0, tb_g_sum = 0, tb_b_sum = 0, tb_r_sum = 0;
	UINT32 rb_bin_apr = 32;

	do {
		switch(pCfg->out_type)
		{
			case LX_PE_OUT_2D :
			case LX_PE_OUT_UD :
			case LX_PE_OUT_PIP :
				APR_MSG("\nAPR : LR Correction HISTOGRAM\n");
				APR_MSG("     G [L][R]           B [L][R]           R [L][R]         :  G [T][B]           B [T][B]           R [T][B]\n");
				for(itr=0;itr<rb_bin_apr;itr++)
				{

					lr_g_diff = APR_DIFF(pData->hist_l_g[itr], pData->hist_r_g[itr]);
					lr_b_diff = APR_DIFF(pData->hist_l_b[itr], pData->hist_r_b[itr]);
					lr_r_diff = APR_DIFF(pData->hist_l_r[itr], pData->hist_r_r[itr]);
					lr_g_sum += lr_g_diff;
					lr_b_sum += lr_b_diff;
					lr_r_sum += lr_r_diff;
					APR_MSG("%02d) [%07d][%07d] [%07d][%07d] [%07d][%07d]",itr,\
						pData->hist_l_g[itr], pData->hist_r_g[itr], pData->hist_l_b[itr], pData->hist_r_b[itr], pData->hist_l_r[itr], pData->hist_r_r[itr]);
					tb_g_diff = APR_DIFF(pData->hist_t_g[itr], pData->hist_b_g[itr]);
					tb_b_diff = APR_DIFF(pData->hist_t_b[itr], pData->hist_b_b[itr]);
					tb_r_diff = APR_DIFF(pData->hist_t_r[itr], pData->hist_b_r[itr]);
					tb_g_sum += tb_g_diff;
					tb_b_sum += tb_b_diff;
					tb_r_sum += tb_r_diff;
					APR_MSG(": [%07d][%07d] [%07d][%07d] [%07d][%07d]\n",\
						pData->hist_t_g[itr], pData->hist_b_g[itr], pData->hist_t_b[itr], pData->hist_b_b[itr], pData->hist_t_r[itr], pData->hist_b_r[itr]);
				}
				APR_MSG("    Total diff\n");
				APR_MSG("     %07d            %07d            %07d          :  %07d            %07d            %07d\n", lr_g_sum, lr_b_sum, lr_r_sum, tb_g_sum, tb_b_sum, tb_r_sum);
				break;
			case LX_PE_OUT_3D_2D :
			case LX_PE_OUT_TB :
			case LX_PE_OUT_SS :
			case LX_PE_OUT_FS :
			case LX_PE_OUT_LA :
			case LX_PE_OUT_DUAL_TB :
			case LX_PE_OUT_DUAL_SS :
			case LX_PE_OUT_DUAL_FULL :
				APR_MSG("\nAPR : DCE HISTOGRAM\n");
				APR_MSG("L		: R\n");
				for(itr=0;itr<rb_bin_apr;itr++)
				{
					APR_MSG("%02d) [%07d]	: [%07d]", itr, pData->dce_l[itr], pData->dce_r[itr]);
					sum += APR_DIFF(pData->dce_l[itr], pData->dce_r[itr]);
				}
				APR_MSG("    Total diff %d\n", sum);
				break;
			default :
				break;
		}
	} while(0);
	return ret;
}

static int APR_DumpLrcHistData(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	APR_THR_T *pThr = &apr_info.thr;
	APR_STM_T *pStm = &apr_info.stm;
	UINT32 max_pd = 0;
	UINT32 win_half_size = 0;
	
	do {
		if(pStm->win_pos != APR_WIN_LRTB_FULL)
			win_half_size = APR_WIN_RATIO;
		else
			win_half_size = 1;

		APR_GetMax3(&pData->cb, &pData->cc, &pData->ll, &max_pd);

		printk("LRC : == value	/  threshold  ====	[outtype %d  %dx%d] \n", pCfg->out_type, pCfg->out_win_x, pCfg->out_win_y);
		printk("tpd[%d] / %d	 ,	 motion_l[%d] / 2d-%d 3d-%d , motion_r[%d]\n", pData->tpd, pThr->tpd_thr, pData->motion_l, pThr->mtn_thr_2d, pThr->mtn_thr_3d, pData->motion_r);
		printk("cb[%d] cc[%d] ll[%d] -> max_pd[%d] / %d\n", pData->cb, pData->cc, pData->ll, max_pd, pThr->ptn_thr);
		printk("ssr[%d] tbr[%d] / 2D->3D[%d] , 3D->2D[%d]\n", pData->ssr, pData->tbr, pThr->fmt_th2, pThr->fmt_th1);
		printk("ssh[%d] ssv[%d] tbh[%d] tbv[%d] \n", pData->ssh, pData->ssv, pData->tbh, pData->tbv);
		printk("LRCR win_pos[%d] (0:LRTB_FULL,1:LRTB_HAF1,2:LRTB_HAF2) \n", pStm->win_pos);

		printk("thr_adj[%d] lr_diff[%d] tb_diff[%d] / fmt_th3[%d] ,  lr-tb[%d] / fmt_th4[%d] \n", \
		thr_adj, pData->hist_lrc.lr_diff, pData->hist_lrc.tb_diff, tmp_thr3, (int)abs(pData->hist_lrc.lr_diff - pData->hist_lrc.tb_diff), tmp_thr4);
		printk("fmt_th6[%d] fmt_th7[%d] fmt_th8[%d] fmt_th9[%d] fmt_th10[%d]\n", tmp_thr6, tmp_thr7, pThr->fmt_th8, pThr->fmt_th9, pThr->fmt_th10);
		printk("vld_thr[%d] \n", pThr->vld_thr);
		printk("motion[%d/%d], spatial[%d/%d], apl[%d/%d], noise[%d/%d],  dual_screen_fmt[%d](0:2D,8:SS,9:TB)\n", pData->motion_l, pData->motion_r, pData->spatial_l, pData->spatial_r, pData->apl_l, pData->apl_r, pData->nt_l, pData->nt_r, pData->hw_fmt);
		printk("APR : ==>> Format tmp[%d] final[%d]	(0:2D,1:CB,2:LL,3:CC,4:SS,5:TB,6:UNKNOWN)  Test pattern(%d)\n", pCfg->tmp_fmt, pCfg->apr_fmt, pCfg->tpd_flag);
	} while(0);
	return ret;
}

static int APR_DumpData4Simulation(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_DTA_T *pData= &apr_info.dta;
	APR_THR_T *pThr = &apr_info.thr;

	if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_SCRIPT_DATA) {
		printk("MESSAGE_B= %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",\
				pCfg->cur_lrc_fmt,pCfg->fmt_before_vote[APR_METHOD_3],pCfg->apr_fmt,pData->tpd,pData->ssr,pData->tbr,pThr->fd3.subtitle,pThr->fd3.variation,pThr->fd3.correlation,pThr->fd3.corr_3rd,pCfg->cur_src_fmt,\
				pData->fd3.blk_stat.mean[0],pData->fd3.blk_stat.mean[1],pData->fd3.blk_stat.mean[2],pData->fd3.blk_stat.mean[3],\
				pData->fd3.blk_stat.mean[4],pData->fd3.blk_stat.mean[5],pData->fd3.blk_stat.mean[6],pData->fd3.blk_stat.mean[7],\
				pData->fd3.blk_stat.s_dev[0],pData->fd3.blk_stat.s_dev[1],pData->fd3.blk_stat.s_dev[2],pData->fd3.blk_stat.s_dev[3],\
				pData->fd3.blk_stat.s_dev[4],pData->fd3.blk_stat.s_dev[5],pData->fd3.blk_stat.s_dev[6],pData->fd3.blk_stat.s_dev[7],\
				pData->fd3.scene_type,pData->fd3.bMask,\
				pData->fd3.corr.ss[0],pData->fd3.corr.ss[1],pData->fd3.corr.ss[2],pData->fd3.corr.ss[3],\
				pData->fd3.corr.tb[0],pData->fd3.corr.tb[1],pData->fd3.corr.tb[2],pData->fd3.corr.tb[3],\
				pData->fd3.corr.ss_wnd_info[0],pData->fd3.corr.ss_wnd_info[1],pData->fd3.corr.ss_wnd_info[2],pData->fd3.corr.ss_wnd_info[3],\
				pData->fd3.corr.tb_wnd_info[0],pData->fd3.corr.tb_wnd_info[1],pData->fd3.corr.tb_wnd_info[2],pData->fd3.corr.tb_wnd_info[3]);
#if 0
		printk("\n");
#else
		printk(" %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n",\
				pData->ssr,pData->tbr,pData->hist_lrc.lr_diff,pData->hist_lrc.tb_diff,\
				pData->hist_src.tb_diff[0],pData->hist_src.tb_diff[1],pData->hist_src.tb_diff[2],pData->hist_src.tb_diff[3],\
				pData->hist_src.tb_diff[4],pData->hist_src.tb_diff[5],pData->hist_src.tb_diff[6],pData->hist_src.tb_diff[7],\
				pData->hist_src.tb_diff[8],\
				pData->hist_src.lr_diff[0],pData->hist_src.lr_diff[1],pData->hist_src.lr_diff[2],pData->hist_src.lr_diff[3],\
				pData->hist_src.lr_diff[4],pData->hist_src.lr_diff[5],pData->hist_src.lr_diff[6],pData->hist_src.lr_diff[7],\
				pData->hist_src.lr_diff[8],\
				pData->hist_src.tb_ratio[0],pData->hist_src.ss_ratio[0],\
				pCfg->img_class, pData->fd3.hsv_picture.pic_seg_hsv_max,	pData->fd3.hsv_picture.pic_seg_hsv_count,\
				pData->hist_lrc.tb_ratio, pData->hist_lrc.ss_ratio,\
				pData->fd3.hsv_blk.seg_hsv_count[0], pData->fd3.hsv_blk.seg_hsv_count[1],\
				pData->fd3.hsv_blk.seg_hsv_count[2], pData->fd3.hsv_blk.seg_hsv_count[3],\
				pData->fd3.hsv_blk.seg_hsv_max[0], pData->fd3.hsv_blk.seg_hsv_max[1],\
				pData->fd3.hsv_blk.seg_hsv_max[2], pData->fd3.hsv_blk.seg_hsv_max[3],\
				pData->motion_l, pCfg->hfd_1_fmt, pCfg->hfd_2_fmt, pCfg->hfd_3_fmt, pCfg->hfd_3_1_fmt, pCfg->in_win_x, pCfg->in_win_y);
#endif
	}
	if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_EXCEL_DATA) {
		printk("MESSAGE_A= %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",\
							pCfg->out_type,pCfg->out_win_x,pCfg->out_win_y,pData->tpd,pThr->tpd_thr,pData->motion_l,pThr->mtn_thr_2d,pThr->mtn_thr_3d,pData->motion_r,pData->cb,pData->cc,pData->ll,pThr->ptn_thr,pData->ssr,pData->tbr,\
  							pThr->fmt_th2,pThr->fmt_th1,pData->ssh,pData->ssv,pData->tbh,pData->tbv,pData->hist_lrc.lr_diff,pData->hist_lrc.tb_diff,pThr->fmt_th3,pThr->fmt_th4,pThr->fmt_th6,pThr->fmt_th7,pThr->fmt_th8, pThr->fmt_th9, pThr->fmt_th10,\
				pData->fd3.blk_stat.mean[0],pData->fd3.blk_stat.mean[1],pData->fd3.blk_stat.mean[2],pData->fd3.blk_stat.mean[3],\
				pData->fd3.blk_stat.mean[4],pData->fd3.blk_stat.mean[5],pData->fd3.blk_stat.mean[6],pData->fd3.blk_stat.mean[7]);
		printk("MESSAGE_B= %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",\
				pThr->fmt_th5,pThr->vld_thr,pData->motion_l,pData->motion_r,pData->spatial_l,pData->spatial_r,pData->apl_l,pData->apl_r,pData->nt_l,\
				pData->nt_r,pData->hw_fmt,\
				pCfg->fd3.sampling,pCfg->fd3.scan,pCfg->fd3.color,\
				pCfg->fd3.win.seg_l,pCfg->fd3.win.seg_r,pCfg->fd3.win.seg_t,pCfg->fd3.win.seg_b,\
				pCfg->fd3.win.prj_l,pCfg->fd3.win.prj_r,pCfg->fd3.win.prj_t,pCfg->fd3.win.prj_b,\
				pThr->fd3.subtitle,pThr->fd3.variation,pThr->fd3.correlation,pThr->fd3.corr_3rd,pCfg->cur_src_fmt,\
				pData->fd3.blk_stat.s_dev[0],pData->fd3.blk_stat.s_dev[1],pData->fd3.blk_stat.s_dev[2],pData->fd3.blk_stat.s_dev[3],\
				pData->fd3.blk_stat.s_dev[4],pData->fd3.blk_stat.s_dev[5],pData->fd3.blk_stat.s_dev[6],pData->fd3.blk_stat.s_dev[7],\
				pCfg->fmt_before_vote[APR_METHOD_3]);
		printk("MESSAGE_C= %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n\n",\
				pCfg->cur_lrc_fmt, pCfg->apr_fmt,
				pData->hist_src.tb_diff[0],pData->hist_src.tb_diff[1],pData->hist_src.tb_diff[2],pData->hist_src.tb_diff[3],\
				pData->hist_src.tb_diff[4],pData->hist_src.tb_diff[5],pData->hist_src.tb_diff[6],pData->hist_src.tb_diff[7],\
				pData->hist_src.tb_diff[8],\
				pData->hist_src.lr_diff[0],pData->hist_src.lr_diff[1],pData->hist_src.lr_diff[2],pData->hist_src.lr_diff[3],\
				pData->hist_src.lr_diff[4],pData->hist_src.lr_diff[5],pData->hist_src.lr_diff[6],pData->hist_src.lr_diff[7],\
				pData->hist_src.lr_diff[8],\
				pData->hist_src.tb_ratio[0],pData->hist_src.ss_ratio[0],\
				pData->fd3.scene_type,pData->fd3.bMask,\
				pData->fd3.corr.ss[0],pData->fd3.corr.ss[1],pData->fd3.corr.ss[2],pData->fd3.corr.ss[3],\
				pData->fd3.corr.tb[0],pData->fd3.corr.tb[1],pData->fd3.corr.tb[2],pData->fd3.corr.tb[3],\
				pData->fd3.corr.ss_wnd_info[0],pData->fd3.corr.ss_wnd_info[1],pData->fd3.corr.ss_wnd_info[2],pData->fd3.corr.ss_wnd_info[3],\
				pData->fd3.corr.tb_wnd_info[0],pData->fd3.corr.tb_wnd_info[1],pData->fd3.corr.tb_wnd_info[2],pData->fd3.corr.tb_wnd_info[3]\
				);
	}

	return ret;
}

static int APR_DumpCurrentConfig(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;

	do {
		APR_PRINT("\n");
		APR_PRINT("src_type    [%d] (0:DTV,1:ATV,2:CVBS,3:SCART,4:COMP,5:PC,6:HDMI)\n", pCfg->src_type);
		APR_PRINT("out_type    [%d] (0:2D,1:3D-2D,2:2D-3D,3:UD,4:TB,5:SS,6:FS,7DUAL_TB,8:DUAL_SS,9:DUAL_FULL,10:PIP)\n", pCfg->out_type);
		APR_PRINT("in_type     [%d] (0:TB,1:SS,2:QC,3:CB,4:FP,5:FPI,6:FA,7:FS,8:LA,9:SSF,10:DUAL,11:CA,12:LAH)\n", pCfg->in_type);
		APR_PRINT("Image class [%d] (0:TP,1:2D_HARD,2:2D_WEAK_SS,3:2D_WEAK_TB,4:SS_HARD,5:SS_WEAK,6:TB_HARD,7:TB_WEAK)\n", pCfg->img_class);
		APR_PRINT("Enable     : apr[%d] hst[%d] cap[%d] fd3[%d]\n", pCfg->enable, pCfg->hst_enb, pCfg->cap.enb, pCfg->fd3.enb);
		APR_PRINT("Detect fmt : src_hst[%d],lrc_hst[%d],ldf[%d],fd3[%d],apr[%d] (0:2D,4:SS,5:TB,7:UN)\n", pCfg->cur_src_fmt,pCfg->cur_lrc_fmt,pCfg->ldf_fmt,pCfg->fd3_fmt,pCfg->apr_fmt);
		APR_PRINT("input  win : w[%d] h[%d]\n", pCfg->in_win_x, pCfg->in_win_y);
		APR_PRINT("output win : w[%d] h[%d]\n", pCfg->out_win_x, pCfg->out_win_y);
		APR_PRINT("scan       : [%d] (0:I,1:P)\n", pCfg->scan);
		APR_PRINT("\n");
		APR_PRINT("\n");
	} while(0);

	return ret;
}

static int APR_DecideFormatBySRCHST(void)
{
	int ret = RET_OK;

	return ret;
}

static int APR_DecideFormatByLRCHST(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_DTA_T *pData= &apr_info.dta;
	APR_THR_T *pThr = &apr_info.thr;
	APR_STM_T *pStm = &apr_info.stm;

	UINT32 max_pd = 0;

	static UINT32 tb_prev_dif = 0;
	static UINT32 lr_prev_dif = 0;
	UINT32 lr_dif = 0;
	UINT32 tb_dif = 0;
	UINT32 thr3, thr4, thr6, thr7;
	UINT32 thr_const0, thr_const1, thr_const2, thr_const3;
	APR_CAPA_INFO_T capacity;

	do {
		if(pCfg->hst_enb < LX_APR_HST_ON) break;
		if(pData->lrc_complete_flag == FALSE) goto LABEL_DECIDE_FMT;

		/* out window Å©±â°¡ 4:3 È­¸é º¸´Ù ?Û?» °æ¿ì ¹× PIP °æ¿ì, auto detection ÇÏÁö ¾Ê?½. */
		if(pCfg->out_type==LX_PE_OUT_PIP)
		{
			pCfg->tmp_fmt = LX_APR_FMT_2D_2D;

			goto LABEL_DECIDE_FMT;
		}

		thr_adj = (pData->hist_lrc.tb_ratio << 10)/(1036800);	//1920x540=1036800

		if(pStm->win_pos == APR_WIN_LRTB_FULL)
		{
			tmp_thr3   = (pThr->fmt_th3 * thr_adj)>>10;
			tmp_thr4   = (pThr->fmt_th4 * thr_adj)>>10;
			tmp_thr6   = (pThr->fmt_th6 * thr_adj)>>10;
			tmp_thr7   = (pThr->fmt_th7 * thr_adj)>>10;
			tmp_thr_const0 = (2000 * thr_adj)>>10;
			tmp_thr_const1 = (5000 * thr_adj)>>10;
			tmp_thr_const2 = (200 * thr_adj)>>10;
			tmp_thr_const3 = (70 * thr_adj)>>10;
		}
		else if(pStm->win_pos == APR_WIN_LRTB_HAF1 || \
				pStm->win_pos == APR_WIN_LRTB_HAF2 || \
				pStm->win_pos == APR_WIN_LRTB_MAX)
		{
			tmp_thr3   = ( (pThr->fmt_th3 * thr_adj) >>1)>>10;
			tmp_thr4   = ( (pThr->fmt_th4 * thr_adj) >>1)>>10;
			tmp_thr6   = ( (pThr->fmt_th6 * thr_adj) >>1)>>10;
			tmp_thr7   = ( (pThr->fmt_th7 * thr_adj) >>1)>>10;
			tmp_thr_const0 = ( (2000 * thr_adj) >>1)>>10;
			tmp_thr_const1 = ( (5000 * thr_adj) >>1)>>10;
			tmp_thr_const2 = ( (200 * thr_adj) >>1)>>10;
			tmp_thr_const3 = ( (70 * thr_adj) >>1)>>10;
		}

		if((pCfg->out_type == LX_PE_OUT_2D) || (pCfg->out_type == LX_PE_OUT_2D_3D))
		{
			if(pData->lrc_complete_flag == FALSE) break;

			if(pData->tpd > pThr->tpd_thr)
			{
#if 0 // not ready for B0
#endif
			}
			else // ?Ï¹Ý?û?Î ?Ú¿¬ È­¸é
			{
				// go to next step
			}
			pCfg->tpd_flag = FALSE;

			APR_GetMax3(&pData->cb, &pData->cc, &pData->ll, &max_pd);

			if(max_pd >= pThr->ptn_thr)
			{
				if(max_pd == pData->cb)
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				else if(max_pd == pData->cc)
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				else if(max_pd == pData->ll)
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				goto LABEL_DECIDE_FMT;
			}
			else
			{
				pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
			}

			ret = APR_HAL_GetCapacity(&capacity);

			if(capacity.max_src_width > 1920)
			{
				lr_dif = pData->hist_lrc.lr_diff >> 1;
				tb_dif = pData->hist_lrc.tb_diff >> 1;
			}
			else
			{
				lr_dif = pData->hist_lrc.lr_diff;
				tb_dif = pData->hist_lrc.tb_diff;
			}
			thr3   = tmp_thr3;
			thr4   = tmp_thr4;
			thr6   = tmp_thr6;
			thr7   = tmp_thr7;
			thr_const0 = tmp_thr_const0;
			thr_const1 = tmp_thr_const1;
			thr_const2 = tmp_thr_const2;
			thr_const3 = tmp_thr_const3;
			
			// Histogram diff measure
			if((lr_dif >= thr3 && tb_dif >= thr3) \
					|| (lr_dif < thr3 && tb_dif < thr3))
			{
				pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
			}
			else
			{
				if(abs(lr_dif - tb_dif) < thr4)
				{
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				}
				else
				{
					if(lr_dif >= tb_dif)
						pCfg->tmp_fmt = LX_APR_FMT_3D_TB;
					else
						pCfg->tmp_fmt = LX_APR_FMT_3D_SS;
				}
			}

			if (pCfg->tmp_fmt == LX_APR_FMT_2D_2D)
			{
				if(lr_dif > tb_dif)
				{
					if(tb_dif > thr7)
					{
						pCfg->img_class = APR_IMG_CLASS_2D_HARD;
						goto LABEL_DECIDE_FMT;
					}
					else if(tb_dif < thr_const0)
					{
						pCfg->tmp_fmt   = LX_APR_FMT_3D_TB;
						pCfg->img_class = APR_IMG_CLASS_TB_HARD;
						goto LABEL_DECIDE_FMT;
					}
				}
				else
				{
					if(lr_dif > thr7)
					{
						pCfg->img_class = APR_IMG_CLASS_2D_HARD;
						goto LABEL_DECIDE_FMT;
					}
					else if(lr_dif < thr_const0)
					{
						pCfg->tmp_fmt   = LX_APR_FMT_3D_SS;
						pCfg->img_class = APR_IMG_CLASS_SS_HARD;
						goto LABEL_DECIDE_FMT;
					}
				}
			}
			else if(pCfg->tmp_fmt == LX_APR_FMT_3D_SS && lr_dif < thr6)
			{
				if(lr_prev_dif >= thr6)
				{
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				}
				if(tb_dif > APR_MAX_HISTOGRAM_ON_CASE3)
				{
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				}

				if(pCfg->tmp_fmt == LX_APR_FMT_2D_2D)
					pCfg->img_class = APR_IMG_CLASS_2D_WEAK_SS;
				else
					pCfg->img_class = APR_IMG_CLASS_SS_WEAK;
				goto LABEL_DECIDE_FMT;
			}
			else if(pCfg->tmp_fmt == LX_APR_FMT_3D_TB && tb_dif < thr6)
			{
				if(tb_prev_dif >= thr6)
				{
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				}
				if(lr_dif > APR_MAX_HISTOGRAM_ON_CASE3)
				{
					pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
				}

				if(pCfg->tmp_fmt == LX_APR_FMT_2D_2D)
					pCfg->img_class = APR_IMG_CLASS_2D_WEAK_TB;
				else
					pCfg->img_class = APR_IMG_CLASS_TB_WEAK;
				goto LABEL_DECIDE_FMT;
			}
			// Delta measure
			if(pCfg->tmp_fmt == LX_APR_FMT_2D_2D)
			{
				if(pData->ssr > pData->tbr)
				{
					if(pData->ssr > pThr->fmt_th2 && (lr_dif + thr_const1) < tb_dif) // µÎ°¡Áö Á¶°Ç?» µ¿½Ã¿¡ ¸¸Á·ÇÒ °æ¿ì  - 2011/10/21
					{
						pCfg->tmp_fmt   = LX_APR_FMT_3D_SS;
						pCfg->img_class = APR_IMG_CLASS_SS_HARD;
					}
					else
					{
						pCfg->img_class = APR_IMG_CLASS_2D_WEAK_SS;
					}
				}
				else if(pData->ssr < pData->tbr)
				{
					if(pData->tbr > pThr->fmt_th2 && (tb_dif + thr_const1) < lr_dif) // µÎ°¡Áö Á¶°Ç?» µ¿½Ã¿¡ ¸¸Á·ÇÒ °æ¿ì - 2011/10/21
					{
						pCfg->tmp_fmt   = LX_APR_FMT_3D_TB;
						pCfg->img_class = APR_IMG_CLASS_TB_HARD;
					}
					else
					{
						pCfg->img_class = APR_IMG_CLASS_2D_WEAK_TB;
					}
				}
				else
				{
					pCfg->img_class = APR_IMG_CLASS_2D_HARD;
				}
			}
			else
			{
				if(pData->ssr > pData->tbr && lr_dif < tb_dif)
				{
					if(pData->ssr > pThr->fmt_th1)
					{
						if(pCfg->tmp_fmt != LX_APR_FMT_3D_SS)
						{
							pCfg->tmp_fmt   = LX_APR_FMT_2D_2D;
							pCfg->img_class = APR_IMG_CLASS_2D_WEAK_SS;
						}
						else
						{
							pCfg->img_class = APR_IMG_CLASS_SS_HARD;
						}
					}
					else if(pData->ssr - pData->tbr <= APR_MIN_TB_SS_RATIO_THRESHOLD)
					{
						pCfg->tmp_fmt   = LX_APR_FMT_2D_2D;
						pCfg->img_class = APR_IMG_CLASS_2D_WEAK_SS;
					}
				}
				else if(pData->tbr > pData->ssr && tb_dif < lr_dif)
				{
					if(pData->tbr > pThr->fmt_th1)
					{
						if(pCfg->tmp_fmt != LX_APR_FMT_3D_TB)
						{
							pCfg->tmp_fmt   = LX_APR_FMT_2D_2D;
							pCfg->img_class = APR_IMG_CLASS_2D_WEAK_TB;
						}
						else
						{
							pCfg->img_class = APR_IMG_CLASS_TB_HARD;
						}
					}
					else if(pData->tbr - pData->ssr <= APR_MIN_TB_SS_RATIO_THRESHOLD)
					{
						pCfg->tmp_fmt   = LX_APR_FMT_2D_2D;
						pCfg->img_class = APR_IMG_CLASS_2D_WEAK_TB;
					}
				}
				else
				{
					pCfg->tmp_fmt   = LX_APR_FMT_2D_2D;
					pCfg->img_class = APR_IMG_CLASS_2D_HARD;
				}
			}
		}
		else if(pCfg->out_type == LX_PE_OUT_UD        || \
				pCfg->out_type == LX_PE_OUT_TB        || \
				pCfg->out_type == LX_PE_OUT_SS        || \
				pCfg->out_type == LX_PE_OUT_FS        || \
				pCfg->out_type == LX_PE_OUT_LA        || \
				pCfg->out_type == LX_PE_OUT_DUAL_TB   || \
				pCfg->out_type == LX_PE_OUT_DUAL_SS   || \
				pCfg->out_type == LX_PE_OUT_DUAL_FULL || \
				pCfg->out_type == LX_PE_OUT_3D_2D )
		{
			//	 not yet
		}
		else
		{
			pCfg->tmp_fmt = LX_APR_FMT_2D_2D;
			//APR_MSG("APR : not supported out_type [%d]\n", pCfg->out_type);
			break;
		}

LABEL_DECIDE_FMT :

		pCfg->cur_lrc_fmt = pCfg->tmp_fmt;

		lr_prev_dif = lr_dif;
		tb_prev_dif = tb_dif;
	} while(0);

	return ret;
}

static int APR_DecideFormatByLDF(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;

	do {
		if(pCfg->cap.enb)
		{
			ret = APR_CaptureFrame();
		}
	} while(0);

	return ret;
}

static int APR_DecideFormatByFD3(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;

	do {
		if(pCfg->fd3.enb)
		{
			if (lx_chip_rev( ) >= LX_CHIP_REV(H13,B0) ) {
				ret = APR_ProcessFD3();
			}
		}

	} while(0);

	return ret;
}

static int APR_DetectBlackBar(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = &apr_info.cfg;
	APR_DTA_T *pData= &apr_info.dta;

	ret = APR_HAL_DetectBlackBar(pCfg, pData);

	return ret;
}

static int DrawGraph(UINT32 val, UINT32 thr, UINT32 norm, UINT32 max, int *line)
{
	char str[STRING_MAX_SIZE];
	UINT32 pos1, pos2;
	
	pos1 = (UINT32)(val*norm/max);
	pos2 = (UINT32)(thr*norm/max);
	pos1 = pos1 & 0x3F;
	pos2 = pos2 & 0x3F;
	pos1++;
	pos2++;
	
	memset(str, 0, STRING_MAX_SIZE);
	if(val > thr)
	{
		memset(str, 0x3E/*>*/, pos1);
		memset(str, 0x2E/*.*/, pos2);
	}
	else
	{
		memset(str, 0x2E/*.*/, pos1);
	}

	return APR_OSD_DrawString(str, 0, (*line)++);
}

static int APR_DisplayGraph(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	APR_THR_T *pThr  = &apr_info.thr;
	APR_OSD_PATTERN pattern_rect = APR_OSD_MAX;
	char *tmp_str = NULL;
	char *pattern_str = NULL;
	char str[STRING_MAX_SIZE];
	static LX_APR_FORMAT prev_fmt = LX_APR_FMT_MAX;
	static int display_skip_count = 0;
	int line_idx = 0;
	UINT32 pos;
	UINT32 normalize = 60;
	UINT32 i = 0;

	do {
		if(display_skip_count++ != 1) break;
		display_skip_count = 0;
//		if(pCfg->ldf_fmt == prev_fmt) break;
		prev_fmt = pCfg->ldf_fmt;
		
		APR_CheckTime("Draw  result", APR_GET_TIME_DRW, APR_GET_START);

		APR_OSD_SetFlip(APR_OSD_SET_BASE);

		FMT_PTN_STR(pCfg->ldf_fmt, pattern_rect, pattern_str, tmp_str); // result of method 2
		ret = APR_OSD_DrawString(pattern_str, 0, line_idx++);
		memset(str, 0, STRING_MAX_SIZE);
		for(i=0;i<32;i++)
		{
			char ch;
			sprintf(&ch, "%X", pData->d2d.state_num[i]);
			//strcat(str, &ch);
			str[i] = ch;
			if(pData->d2d.state_num[i] == 0) break;
			sprintf(&ch, ">");
			//strcat(str, &ch);
			str[i] = ch;
		}
		ret = APR_OSD_DrawString(	str, 0, line_idx++);

		line_idx++;	
		OUT_TYPE_STR(pCfg->out_type, tmp_str);
		memset(str, 0, STRING_MAX_SIZE);
		strcat(str, "Output type : ");
		strcat(str, tmp_str);
		ret = APR_OSD_DrawString(	str, 0, line_idx++);
		ret = APR_OSD_PrintParam(	"cap/mes/drw (usec) ", pCfg->time.capture,\
									"/", pCfg->time.measure,            \
									"/", pCfg->time.draw,               \
									NULL, 0,                            \
									0, line_idx++);
		line_idx++;
		ret = APR_OSD_DrawString(	"Differential info of 2 windows(L/R)", 0, line_idx++);
		ret = APR_OSD_PrintParam(	"tpd    : ", pData->tpd,            \
									" / ", pThr->tpd_thr,               \
									NULL, 0,                            \
									NULL, 0,                            \
								   	0, line_idx++);
#if 1
		ret = APR_OSD_PrintParam(	"APL    : ", pData->d2d.apl_diff_2d,\
									"  L/R ", pData->d2d.apl_l,         \
									"/",     pData->d2d.apl_r,          \
									NULL, 0,                            \
								   	0, line_idx++);
#endif
		ret = APR_OSD_PrintParam(	"Pixel  : ", pData->d2d.pixel_diff, \
									" THR3 ", pThr->d2d.thr_3,          \
									NULL, 0,                            \
									NULL, 0,                            \
								   	0, line_idx++);
		ret = DrawGraph(pData->d2d.pixel_diff, pThr->d2d.thr_3, normalize, 40*1760*127, &line_idx);

		memset(str, 0, STRING_MAX_SIZE);
		pos = pData->d2d.pixel_dinv*normalize/(40*1760*127);
		pos = pos & 0x3F;
		pos++;
		ret = APR_OSD_PrintParam(	"Pixinv : ", pData->d2d.pixel_dinv, \
									NULL, 0,                            \
									NULL, 0,                            \
									NULL, 0,                            \
								   	0, line_idx++);
		memset(str, 0x2E, pos);
		ret = APR_OSD_DrawString(	str, 0, line_idx++);

		ret = APR_OSD_PrintParam(	"Histo  : ", pData->d2d.histo_diff, \
									" THR1/2 ", pThr->d2d.thr_1,        \
									" / ", pThr->d2d.thr_2,             \
									NULL, 0,                            \
								   	0, line_idx++);
		ret = DrawGraph(pData->d2d.histo_diff, pThr->d2d.thr_1, normalize, 40*(1920-160)*32, &line_idx);
		ret = DrawGraph(pData->d2d.histo_diff, pThr->d2d.thr_2, normalize, 40*(1920-160)*32, &line_idx);
		
		ret = APR_OSD_PrintParam(	"Adjct  : ", pData->d2d.adjct_diff, \
									" THR4/5/6 ", pThr->d2d.thr_4,      \
									" / ", pThr->d2d.thr_5,             \
									" / ", pThr->d2d.thr_6,             \
								   	0, line_idx++);
		ret = DrawGraph(pData->d2d.adjct_diff, pThr->d2d.thr_4, normalize, 40*(1920-160)*16, &line_idx);
		ret = DrawGraph(pData->d2d.adjct_diff, pThr->d2d.thr_5, normalize, 40*(1920-160)*16, &line_idx);
		ret = DrawGraph(pData->d2d.adjct_diff, pThr->d2d.thr_6, normalize, 40*(1920-160)*16, &line_idx);
		
		APR_CheckTime("Draw  result", APR_GET_TIME_DRW, APR_GET_END);
		
		APR_OSD_SetFlip(APR_OSD_SET_FLIP);
	} while(0);

	return ret;
}

static int APR_DisplayInfo(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	APR_OSD_PATTERN m1_pattern_rect = APR_OSD_MAX;
	APR_OSD_PATTERN m2_pattern_rect = APR_OSD_MAX;
	APR_OSD_PATTERN m3_pattern_rect = APR_OSD_MAX;
	APR_OSD_PATTERN final_pattern_rect = APR_OSD_MAX;
	char *tmp_str = NULL;
	char *m1_pattern_str = NULL, *m2_pattern_str = NULL, *m3_pattern_str = NULL, *final_pattern_str = NULL;
	char *m1_tmp_str = NULL, *m2_tmp_str = NULL, *m3_tmp_str = NULL, *final_tmp_str = NULL;
	char str[STRING_MAX_SIZE];
	static LX_APR_FORMAT prev_fmt = LX_APR_FMT_MAX;
	static int display_skip_count = 0;
	int line_idx = 0;

	do {
		if(pCfg->enable < LX_APR_CMD_PATTERN) break;
//		if(pCfg->apr_fmt == LX_APR_FMT_UNKNOWN) break;
//		if(display_skip_count++ != 1) break;
		display_skip_count = 0;
//		if(pCfg->apr_fmt == prev_fmt) break;
		prev_fmt = pCfg->apr_fmt;
		memset(str, 0, STRING_MAX_SIZE);
		
		FMT_PTN_STR(pCfg->fmt_after_vote[APR_METHOD_1], m1_pattern_rect, m1_pattern_str, m1_tmp_str); // result of method 1
		FMT_PTN_STR(pCfg->fmt_after_vote[APR_METHOD_2], m2_pattern_rect, m2_pattern_str, m2_tmp_str); // result of method 2
		FMT_PTN_STR(pCfg->fmt_after_vote[APR_METHOD_3], m3_pattern_rect, m3_pattern_str, m3_tmp_str); // result of method 3(final)
		FMT_PTN_STR(pCfg->apr_fmt, final_pattern_rect, final_pattern_str, final_tmp_str); // result of final negotiation

		OUT_TYPE_STR(pCfg->out_type, tmp_str);
	
		APR_CheckTime("Draw  result", APR_GET_TIME_DRW, APR_GET_START);

		APR_OSD_SetFlip(APR_OSD_SET_BASE);
	
		if(pCfg->out_type==LX_PE_OUT_2D)
		{
			if(pCfg->fd3.enb)      ret = APR_OSD_DrawPattern(final_pattern_rect/*m3_pattern_rect*/);
			else if(pCfg->cap.enb) ret = APR_OSD_DrawPattern(m2_pattern_rect);
			else                   ret = APR_OSD_DrawPattern(m1_pattern_rect);
		}

		if(pCfg->enable < LX_APR_CMD_OSD_LOG)
		{
			APR_OSD_SetFlip(APR_OSD_SET_FLIP);
			APR_CheckTime("Draw  result", APR_GET_TIME_DRW, APR_GET_END);
			break;
		}
		
		ret = APR_OSD_DrawString(final_pattern_str, 0, line_idx++);

		strcat(str, "  M1:"); 
		strcat(str, m1_tmp_str);
		strcat(str, "  M2:");
		strcat(str, (pCfg->cap.enb)?m2_tmp_str:"?");
		strcat(str, "  M3:"); 
		strcat(str, (pCfg->fd3.enb)?m3_tmp_str:"?");
		ret = APR_OSD_DrawString(   str, 0, line_idx++);
		memset(str, 0, STRING_MAX_SIZE);
		strcat(str, "Operation mode : ");
		strcat(str, (apr_thread_cb.func==NULL)?"INTERRUPT":"THREAD");
		ret = APR_OSD_DrawString(	str, 0, line_idx++);
		memset(str, 0, STRING_MAX_SIZE);
		strcat(str, "Source    type : ");
		strcat(str, ((pCfg->src_type >= LX_PE_SRC_ATV) && (pCfg->src_type <= LX_PE_SRC_RGBPC))?"ANALOG":"DIGITAL");
		ret = APR_OSD_DrawString(	str, 0, line_idx++);
		memset(str, 0, STRING_MAX_SIZE);
		strcat(str, "Scan      type : ");
		strcat(str, (pCfg->scan == 0)?"INTERLACED":"PROGRESSIVE");
		ret = APR_OSD_DrawString(	str, 0, line_idx++);
		memset(str, 0, STRING_MAX_SIZE);
		strcat(str, "Output    type : ");
		strcat(str, tmp_str);
		ret = APR_OSD_DrawString(	str, 0, line_idx++);
		ret = APR_OSD_PrintParam(	"Mem usage : ", (pCfg->mem_usage>>10),\
									" KB  ", 0,                    \
									NULL, 0,                       \
									NULL, 0,                       \
									0, line_idx++);
		ret = APR_OSD_DrawString(	"Time info (usec)", 0, line_idx++);
		ret = APR_OSD_PrintParam(	" M2 : ", pCfg->time.capture,  \
									"  M3 : ", pCfg->time.fd3,      \
									"  drw : ", pCfg->time.draw,    \
									NULL, 0,                       \
									0, line_idx++);
		ret = APR_OSD_DrawString(	"Window info", 0, line_idx++);
		ret = APR_OSD_PrintParam(	" in_w  : ", pCfg->in_win_x,       \
									" in_h  : ", pCfg->in_win_y,       \
									NULL, 0,                           \
									NULL, 0,                           \
								   	0, line_idx++);
		ret = APR_OSD_PrintParam(	" out_w : ", pCfg->out_win_x,      \
									" out_h : ", pCfg->out_win_y,      \
									NULL, 0,                           \
									NULL, 0,                           \
								   	0, line_idx++);

		ret = APR_OSD_DrawString(	"Data info", 0, line_idx++);

		ret = APR_OSD_PrintParam(	" tpd : ", pData->tpd,\
									" ssr : ", pData->ssr,\
									" tbr : ", pData->tbr,\
									NULL, 0,              \
								   	0, line_idx++);
		ret = APR_OSD_PrintParam(	" scene_type : ", pData->fd3.scene_type,\
									" bMask : ", pData->fd3.bMask,      \
									NULL, 0,                            \
									NULL, 0,                            \
								   	0, line_idx++);
		ret = APR_OSD_PrintParamHex(" init_count : ", pData->fd3.status.init_count,\
									NULL, 0,                            \
									NULL, 0,                            \
									NULL, 0,                            \
								   	0, line_idx++);
		ret = APR_OSD_PrintParamHex(" suc_fail_history : ", pData->fd3.status.suc_fail_history,\
									NULL, 0,                            \
									NULL, 0,                            \
									NULL, 0,                            \
								   	0, line_idx++);
		ret = APR_OSD_DrawString(	" Correlation", 0, line_idx++);
		ret = APR_OSD_DrawString(	" SS", 0, line_idx++);
		ret = APR_OSD_PrintParam(	" ",   pData->fd3.corr.ss[0],       \
									" / ", pData->fd3.corr.ss[1],       \
									" / ", pData->fd3.corr.ss[2],       \
									" / ", pData->fd3.corr.ss[3],       \
								   	0, line_idx++);
		ret = APR_OSD_DrawString(	" TB", 0, line_idx++);
		ret = APR_OSD_PrintParam(	" ",   pData->fd3.corr.tb[0],       \
									" / ", pData->fd3.corr.tb[1],       \
									" / ", pData->fd3.corr.tb[2],       \
									" / ", pData->fd3.corr.tb[3],       \
								   	0, line_idx++);
		ret = APR_OSD_DrawString(	" SS_WND", 0, line_idx++);
		ret = APR_OSD_PrintParam(	" ",   pData->fd3.corr.ss_wnd_info[0],\
									" / ", pData->fd3.corr.ss_wnd_info[1],\
									" / ", pData->fd3.corr.ss_wnd_info[2],\
									" / ", pData->fd3.corr.ss_wnd_info[3],\
								   	0, line_idx++);
		ret = APR_OSD_DrawString(	" TB_WND", 0, line_idx++);
		ret = APR_OSD_PrintParam(	" ",   pData->fd3.corr.tb_wnd_info[0],\
									" / ", pData->fd3.corr.tb_wnd_info[1],\
									" / ", pData->fd3.corr.tb_wnd_info[2],\
									" / ", pData->fd3.corr.tb_wnd_info[3],\
								   	0, line_idx++);

		APR_CheckTime("Draw  result", APR_GET_TIME_DRW, APR_GET_END);

		APR_OSD_SetFlip(APR_OSD_SET_FLIP);
	} while(0);
	return ret;
}

static int APR_DisplayResult(void)
{
	APR_CFG_T *pCfg  = &apr_info.cfg;

	switch(pCfg->enable)
	{
		case LX_APR_CMD_OSD_LOG :
			APR_DisplayInfo();
			break;
		case LX_APR_CMD_OSD_GRF :
			APR_DisplayGraph();
			break;
		default :
			break;
	}

	if(1)
	{
		static UINT32 count = 0;
		if(pCfg->hst_enb == LX_APR_HST_FMT) {
			if(++count%8== 0 && pCfg->out_type == LX_PE_OUT_2D)
			{
				//APR_DumpHistogram();
				APR_DumpLrcHistData();
				count = 0;
			}
		}
		if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_INFO) {
			printk("Format [%d] (0:2D,4:SS,5:TB,6:UNKNOWN)  Test pattern(%d)\n", pCfg->apr_fmt, pCfg->tpd_flag);
		}
		if(count==100)APR_DumpHistogram();
		APR_DumpData4Simulation();
	}

	return RET_OK;
}

static int APR_SetCurrentInfo(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;

	do {
		ret = APR_HAL_SetCurrentInfo(pCfg, pData);
	} while(0);
	
	return ret;
}

static int APR_VsyncHandler(void)
{
	int ret = RET_OK;

	do {
		ret = APR_DetectBlackBar();
		if(ret) break;
		ret = APR_OSD_VsyncHandler();
	}while(0);

	return ret;
}

static int APR_DetectHandler(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;
	static int intr_skip_count = 0;

	do {
#ifdef USE_VSYNC_INTERRUPT
		ret = APR_VsyncHandler();	// for OSD flip
		if (ret) break;
		 // vsync   period = 16.6 msec
		 // polling period = 16.6 * APR_POLLING_COUNT = 100 msec
		if(intr_skip_count++ != APR_POLLING_COUNT) break;
#endif
		intr_skip_count = 0;
		ret = APR_SetCurrentInfo();
		if (ret) break;
		if(!pCfg->enable) break;
		ret = APR_CollectBasicData();
		if (ret) break;
		ret = APR_GetLrcrHistogram();
		if (ret) break;
		ret = APR_GetDceHistogram();
		if (ret) break;
		ret = APR_CheckConfig();
		if (ret) break;
		ret = APR_DecideFormatBySRCHST();
		if (ret) break;
		ret = APR_DecideFormatByLRCHST(); // method 1
		if (ret) break;
		ret = APR_VotingFormat(APR_METHOD_1, pCfg->cur_lrc_fmt, FALSE);
		if (ret) break;
		ret = APR_DecideFormatByLDF();    // method 2
		if (ret) break;
		ret = APR_VotingFormat(APR_METHOD_2, pCfg->ldf_fmt, FALSE);
		if (ret) break;
		ret = APR_DecideFormatByFD3();    // method 3
		if (ret) break;
		ret = APR_VotingFormat(APR_METHOD_3, pCfg->fd3_fmt, TRUE);
		if (ret) break;
		ret = APR_NegotiateFormat(TRUE);
		if (ret) break;
		ret = APR_AccumulateFormat();
		if (ret) break;
		ret = APR_DisplayResult();
	} while(0);

	return ret;
}

