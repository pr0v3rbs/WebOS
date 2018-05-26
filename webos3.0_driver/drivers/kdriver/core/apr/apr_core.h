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


/** @file apr_core.h
 *
 *  driver header for 3d format auto detection. ( used only within kdriver )
 *	
 *	@author		justine.jeong
 *	@version	0.1
 *	@note		
 *	@date		2012.07.10
 *	@see		
 */

#ifndef	_APR_CORE_H_
#define	_APR_CORE_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "apr_kapi.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define APR_USE_NEW_ALGO
//#define APR_USE_LINE_DIFF_BY_SW

#define APR_MAX_BIN     32
#define APR_MAX_BAK_FMT	16
#define APR_MAX_DCE_DLT	4


#define APR_MAX_HISTOGRAM_ON_CASE3	1000000

#ifdef APR_USE_NEW_ALGO	// Added by won.hur 2011.12.13
#define APR_MAX_MINORITY_VOTE_VAL_NORMAL	2
#define APR_MAX_MINORITY_VOTE_VAL_CHANGED	1
#define APR_VOTE_STACK_SIZE_ON_FORMAT_CHANGE	5
#define APR_MIN_TB_SS_RATIO_THRESHOLD		5
#define APR_ADJUSTABLE_VOTING_MSEC			5000	// 5000 msec = 5 sec
#endif
/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef enum {
	APR_METHOD_1,
	APR_METHOD_2,
	APR_METHOD_3,
	APR_METHOD_MAX
} APR_METHOD_T;

typedef enum {
	APR_IMG_CLASS_TPD,
	APR_IMG_CLASS_2D_HARD,
	APR_IMG_CLASS_2D_WEAK_SS,
	APR_IMG_CLASS_2D_WEAK_TB,
	APR_IMG_CLASS_SS_HARD,
	APR_IMG_CLASS_SS_WEAK,
	APR_IMG_CLASS_TB_HARD,
	APR_IMG_CLASS_TB_WEAK,
	APR_IMG_CLASS_MAX
} APR_IMG_CLASS_T;

typedef enum {
	APR_CAP_SRC_S_CVR,
	APR_CAP_SRC_L_PRW,
	APR_CAP_SRC_L_TNR,
	APR_CAP_SRC_L_OVR,
	APR_CAP_SRC_R_PRW,
	APR_CAP_SRC_R_TNR,
	APR_CAP_SRC_R_OVR,
	APR_CAP_SRC_MAX
} APR_CAP_SRC_T;

typedef enum {
	APR_GET_START,
	APR_GET_END
} APR_TIME_T;

typedef enum {
	APR_GET_TIME_HIS,
	APR_GET_TIME_DEC,
	APR_GET_TIME_CAP,
	APR_GET_TIME_MAT,
	APR_GET_TIME_DRW,
	APR_GET_TIME_FD3,
	APR_GET_TIME_MAX
} APR_GET_TIME_T;

typedef enum {
	APR_WIN_LFT,
	APR_WIN_RGT,
	APR_WIN_TOP,
	APR_WIN_BOT,
	APR_WIN_MAX
} APR_WIN_ID_T;

typedef enum {
	APR_WIN_HIS,
	APR_WIN_DCE
} APR_WIN_TYPE_T;

typedef enum {
	APR_HIS_GRE,
	APR_HIS_BLU,
	APR_HIS_RED,
	APR_HIS_MAX
} APR_HISTOGRAM_TYPE_T;

typedef enum {
	APR_STM_HIS_RDY__LR,
	APR_STM_HIS_RDY_LR1,
	APR_STM_HIS_RDY_LR2,
	APR_STM_HIS_RDY_LR3,

	APR_STM_HIS_SET__LR,
	APR_STM_HIS_REQ__LR,
	APR_STM_HIS_GET_LRG,
	APR_STM_HIS_GET_LRB,
	APR_STM_HIS_GET_LRR,

	APR_STM_HIS_TMP,
	APR_STM_HIS_RDY__TB,
	APR_STM_HIS_SET__TB,
	APR_STM_HIS_REQ__TB,
	APR_STM_HIS_GET_TBG,// = 0xA,
	APR_STM_HIS_GET_TBB,
	APR_STM_HIS_GET_TBR,

	APR_STM_MAX
} APR_STM_MOD_T;

typedef enum {
	APR_WIN_LRTB_FULL,
	APR_WIN_LRTB_HAF1,
	APR_WIN_LRTB_HAF2,
	APR_WIN_LRTB_MAX
} APR_WIN_POS_T;

typedef struct {
	APR_STM_MOD_T cur_stm;
	APR_STM_MOD_T nxt_stm;
	APR_WIN_POS_T win_pos;
} APR_STM_T;

typedef struct {
	UINT32 seg_l;
	UINT32 seg_r;
	UINT32 seg_t;
	UINT32 seg_b;
	UINT32 prj_l;
	UINT32 prj_r;
	UINT32 prj_t;
	UINT32 prj_b;
} APR_FD3_WIN_INFO_T;

typedef struct {
	UINT32 seg_red;
	UINT32 seg_re_ye;
	UINT32 seg_yellow;
	UINT32 seg_ye_gr;
	UINT32 seg_green;
	UINT32 seg_gr_cy;
	UINT32 seg_cyan;
	UINT32 seg_cy_bl;
	UINT32 seg_blue;
	UINT32 seg_bl_ma;
	UINT32 seg_magenta;
	UINT32 seg_ma_re;
	UINT32 seg_value_black;
	UINT32 seg_saturate;
	UINT32 seg_sdelta;
} APR_FD3_SEG_THR_T;

typedef struct {
	UINT32 src_complete_flag;
	UINT32 lrc_complete_flag;

	/* pattern */
	UINT32 tpd;
	UINT32 cb;
	UINT32 cc;
	UINT32 ll;
	UINT32 ssr;
	UINT32 tbr;
	UINT32 ssh;
	UINT32 ssv;	
	UINT32 tbh;
	UINT32 tbv;	

	/* LRCR Histogram */
	UINT32 hist_l_g[APR_MAX_BIN];
	UINT32 hist_r_g[APR_MAX_BIN];
	UINT32 hist_l_b[APR_MAX_BIN];
	UINT32 hist_r_b[APR_MAX_BIN];
	UINT32 hist_l_r[APR_MAX_BIN];
	UINT32 hist_r_r[APR_MAX_BIN];
	UINT32 hist_t_g[APR_MAX_BIN];
	UINT32 hist_b_g[APR_MAX_BIN];
	UINT32 hist_t_b[APR_MAX_BIN];
	UINT32 hist_b_b[APR_MAX_BIN];
	UINT32 hist_t_r[APR_MAX_BIN];
	UINT32 hist_b_r[APR_MAX_BIN];
	UINT32 g_diff;
	UINT32 b_diff;
	UINT32 r_diff;

	/* DCE Histogram */	
	UINT32 dce_l[APR_MAX_BIN];
	UINT32 dce_r[APR_MAX_BIN];

	/* tnr */
	UINT32 motion_l;
	UINT32 motion_r;	
	UINT32 spatial_l;
	UINT32 spatial_r;
	UINT32 apl_l;
	UINT32 apl_r;
	UINT32 nt_l;
	UINT32 nt_r;

	/* detection of dual play screen */
	UINT32 hw_fmt;
	
	/* detection of 2d */
	struct {
		UINT32 apl_l;
		UINT32 apl_r;
		UINT32 apl_diff_2d;
		UINT32 pixel_diff;
		UINT32 pixel_dinv;
		UINT32 histo_diff;
		UINT32 adjct_diff;
		UINT32 state_num[32];
	} d2d;

	/* L9 & H13 A0/A1 L/R correction histogram */
	struct {
		UINT32 lr_diff;
		UINT32 tb_diff;
		UINT32 tb_ratio;	//detect ratio for tb, 1024=100%
		UINT32 ss_ratio;	//detect ratio for ss, 1024=100%
	} hist_lrc;

	/* B0 source histogram  */
	struct {
		UINT32 tb_diff[9];
		UINT32 lr_diff[9];
		UINT32 tb_ratio[9];
		UINT32 ss_ratio[9];
	} hist_src;

	/* B0 fd3 */
	struct {
		UINT32 scene_type;
		UINT32 bMask;
		struct {
			UINT32 ss[4];
			UINT32 tb[4];
			UINT32 ss_wnd_info[4];
			UINT32 tb_wnd_info[4];
		} corr;
		struct {
			UINT32 mean[16];
			UINT32 s_dev[16];
		} blk_stat;
		struct {
			UINT32 pic_seg_hsv_max;
			UINT32 pic_seg_hsv_count;
		} hsv_picture;
		struct {
			UINT32 seg_hsv_max[4];
			UINT32 seg_hsv_count[4];
		} hsv_blk;
		struct {
			UINT32 init_count;
			UINT32 suc_fail_history;
		} status;
	} fd3;
} APR_DTA_T;

typedef struct {
	LX_PE_SRC_TYPE     src_type;
	LX_PE_OUT_TYPE     out_type;
	LX_PE_3D_IN_TYPE   in_type;
	UINT32             in_f_rate;
	
	LX_APR_FORMAT      cur_src_fmt;
	LX_APR_FORMAT      cur_lrc_fmt;
	LX_APR_FORMAT      tmp_fmt;
	LX_APR_FORMAT      apr_fmt;
	LX_APR_FORMAT      acc_fmt;	
	LX_APR_FORMAT      src_fmt;	
	LX_APR_FORMAT      ldf_fmt;	
	LX_APR_FORMAT      fd3_fmt;	
	LX_APR_FORMAT	   fmt_before_vote[APR_METHOD_MAX];
	LX_APR_FORMAT	   fmt_after_vote[APR_METHOD_MAX];
	LX_APR_FORMAT      voting_fmt[APR_METHOD_MAX][APR_MAX_BAK_FMT];
	
	APR_IMG_CLASS_T    img_class;

	LX_APR_FORMAT      hfd_1_fmt;	
	LX_APR_FORMAT      hfd_2_fmt;	
	LX_APR_FORMAT      hfd_3_fmt;	
	LX_APR_FORMAT      hfd_3_1_fmt;	
	LX_APR_FORMAT      hfd_3_2_fmt;	

	LX_APR_CMD         enable;
	LX_APR_HIDDEN_CMD  hidden_cmd;
	BOOLEAN            valid;
	UINT32             tpd_flag;
	UINT32             rb_bin_apr;
	UINT32             scan;
	UINT32             in_win_x;
	UINT32             in_win_y;
	UINT32             out_win_x;
	UINT32             out_win_y;
	UINT32             act_x0;
	UINT32             act_y0;
	UINT32             act_x1;
	UINT32             act_y1;
	UINT32             tp_mode;		//inner pattern mode
	UINT32             mute_en;		//mute on,off state
	UINT32             apr_chk;		// 0:normal,1:not tb,2:not ss
	UINT32             cnt_start;
	UINT32             cnt_2d;
	UINT32             cnt_ss;
	UINT32             cnt_tb;
	UINT32             mute_status;
	
	UINT32             hst_enb;
	UINT32             mem_usage;

	struct {
		LX_APR_CAP     enb;
		UINT32         h_line_num;
		UINT32         v_line_num;
		UINT32         h_line_width;
		UINT32         v_line_width;
		UINT32         top_offset;
		UINT32         bot_offset;
		UINT32         lr_crop;
	} cap;
	
	struct {
		UINT32         measure;
		UINT32         capture;
		UINT32         draw;
		UINT32         fd3;
	} time;
	
	struct {
		LX_APR_FD3     enb;	
		UINT32         sampling; // vertical sampling : 0 - all line   , 1 - every two line
		UINT32         scan;     // input type        : 0 - progressive, 1 - interlaced
		UINT32         color;    // color format      : 0 - YUV420     , 1 - YUV422        , 2 - YUV444
		APR_FD3_WIN_INFO_T win;  // window info for color segmentation & projection
		APR_FD3_SEG_THR_T  seg_thr;
	} fd3;
} APR_CFG_T;

/**
 *	pe apr threshold control parameter type
 */
typedef struct {
	UINT32 isDefault;        ///< Is default setting ?
	UINT32 tpd_thr;          ///< for test pattern
	UINT32 ptn_thr;          ///< for cb/cc/ll 
	UINT32 mtn_thr_2d;       ///< for motion check (2D)
	UINT32 mtn_thr_3d;       ///< for motion check	(3D)
	UINT32 fmt_th1;          ///< th1 in algorothm
	UINT32 fmt_th2;          ///< th2 in algorothm 
	UINT32 fmt_th3;	         ///< th3 in algorothm
	UINT32 fmt_th4;          ///< th4 in algorothm
	UINT32 fmt_th5;          ///< th5 in algorothm
	UINT32 fmt_th6;          ///< th6 in algorothm
	UINT32 fmt_th7;          ///< th7 in algorothm
	UINT32 fmt_th8;          ///< th8 in algorothm
	UINT32 fmt_th9;          ///< th9 in algorothm
	UINT32 fmt_th10;         ///< th10 in algorothm
	UINT32 vld_thr;          ///< size of buffer for valid cahek (2D->3D, 3D->2D)

	struct {
		UINT32 thr_0;
		UINT32 thr_1;
		UINT32 thr_2;
		UINT32 thr_3;
		UINT32 thr_4;
		UINT32 thr_5;
		UINT32 thr_6;
	} d2d;

	struct {
		SINT32 subtitle;
		SINT32 variation;
		SINT32 correlation;
		SINT32 corr_3rd;
		SINT32 final_corr;
		SINT32 mean_tolerance;
	} fd3;
} APR_THR_T;

typedef struct {
	APR_CFG_T	cfg;
	APR_STM_T	stm;
	APR_DTA_T	dta;
	APR_THR_T	thr;
} APR_INFO_T;

#define APR_MAX_LINE		80//128
#define APR_MAX_CENTER_LINE	8
#if 1 // for capture test, ->  Left main scaler
#define APR_MAX_HSIZE	1920
#define APR_MAX_VSIZE	1080
#else
#define APR_MAX_HSIZE	1280
#define APR_MAX_VSIZE	720
#endif
#define APR_MAX_STRIP_WIDTH	1
#define APR_CAP_TOP_OFFSET  32
#define APR_CAP_BOT_OFFSET  0
#define APR_CAP_LR_CROP     0

//#define VIDEO_H13_CODEC_DDR_OFFSET 0x075c4000


typedef struct {
	UINT32 y_base;
	UINT32 c_base;
	UINT32 y_msb;
	UINT32 c_msb;
	UINT32 h;
	UINT32 v;
	UINT32 bpp;
	UINT32 stride;
	UINT32 sub_sample;	// 0:420, 1:422, 2:444
	UINT32 src_valid;
	volatile UINT8* pFrameBase;
	UINT32 frame_base_phys[2];
	struct {
		UINT32 h_size;
		UINT32 v_size;
		UINT32 h_offset;
		UINT32 v_offset;
	} input;
} APR_OV_INFO_T;

typedef struct {
	APR_CAP_SRC_T cap_src;
	UINT8 *pHLine[APR_MAX_LINE];
	UINT32 h_size;
	UINT32 h_pos_offset;
	UINT32 h_line_num;

	UINT8 *pHLineCenter[APR_MAX_CENTER_LINE];
	
	UINT8 *pVLine[APR_MAX_LINE];
	UINT32 v_size;
	UINT32 v_pos_offset;
	UINT32 v_line_num;

	UINT32 frame_base_phys[2];
	UINT32 stride;
	UINT32 top_offset;
	UINT32 bot_offset;
	UINT32 lr_crop;
} APR_CAP_INFO_T;

typedef struct {
	UINT32 tick_start_time[APR_GET_TIME_MAX];
	UINT32 tick_end_time[APR_GET_TIME_MAX];
} APR_TIME_INFO_T;

typedef struct {
	LX_PE_SRC_TYPE src_type;	///< pe source type
	LX_PE_OUT_TYPE out_type;	///< pe out type
	LX_APR_FORMAT  cur_fmt;     ///< current 3df info 
	UINT32         in_f_rate;   ///< input frame rate(x10 hz) 

	struct {
		/* [0]tb0,[1]ss0,[2]tb1,[3]ss1 */
		UINT32 dif_max[4];		///< diff. max, [0]tb0,[1]ss0,[2]tb1,[3]ss1
		UINT32 pxl_cnt[4];		///< pixel count, [0]tb0,[1]ss0,[2]tb1,[3]ss1
		UINT32 set_cnt[4];		///< setting count, [0]tb0,[1]ss0,[2]tb1,[3]ss1
	} lrc;

	struct {
		UINT32 s_state[18];
		/* [0,1]y tb,ss,[2,3]cb,[4,5]cr, [6,7]g,[8,9]b,[10,11]r, [12,13]h,[14,15]s,[16,17]v */
		UINT32 dif_max[18];		///< diff. max 
		UINT32 pxl_cnt[18];		///< pixel count
		UINT32 set_cnt[18];		///< setting count
	} src;
} APR_HST_INFO_T;

typedef struct {
	UINT32 max_src_width;
} APR_CAPA_INFO_T;


/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int APR_Core_Init(LX_APR_INIT_T *pstParams);
int APR_Core_Cleanup(void);
int APR_Core_Resume(void);
int APR_Core_ThreadHandle(void);
int APR_Core_SetDefault(LX_PE_DEFAULT_SETTINGS_T *pstParams);
int APR_Core_SetControl(LX_APR_CONTROL_T *pstParams);
int APR_Core_GetInfo(LX_APR_INFO_T *pstParams);
int APR_Core_SetCommand(LX_APR_COMMAND_T *pstParams);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _APR_CORE_H_ */



