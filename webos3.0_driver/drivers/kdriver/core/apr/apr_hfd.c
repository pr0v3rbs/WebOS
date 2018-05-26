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
 *  measure hardware format detection 
 *	
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.07.10
 *  note		Additional information.
 *
 *  @addtogroup apr
 *	@{
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/uaccess.h>

#include "base_types.h"
#include "apr_def.h"
#include "apr_core.h"
#include "apr_hal.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define APR_DIFF(a,b) (a>=b)?abs(a-b):abs(b-a)
#define FD3MSG	if(pCfg->fd3.enb == LX_APR_FD3_FMT)printk
#define FD3DBG	if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_FD3 && g_msg_cnt & 1)printk
#define FD3ALG  if(pCfg->fd3.enb == LX_APR_FD3_LOG)printk
#define FD3PRT  if(pCfg->hidden_cmd == LX_APR_HCMD_DUMP_FD3 && g_msg_cnt & 1)printk

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern APR_INFO_T apr_info;

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
UINT32 g_msg_cnt = 0;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/
static int APR_FD3_ChangeThreshold(UINT32 param)
{
	APR_DTA_T *pData = &apr_info.dta;
	APR_CFG_T *pCfg  = &apr_info.cfg;

	int seg_idx = pData->fd3.hsv_picture.pic_seg_hsv_max;
	int margin_th_p[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int margin_th_m[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

	if(seg_idx < 12)
	{
		if (seg_idx == 10)
		{
			margin_th_p[11] = 4;
			margin_th_m[0]       = 4;
		}
		if (seg_idx == 11)
		{
			margin_th_p[0] = 4;
			margin_th_m[1]       = 4;
		}
		else if (seg_idx < 10)
		{
			margin_th_p[seg_idx+1]   = 4;
			margin_th_m[seg_idx+2] = 4;
		}
	}

	// 0 : default, 1 : 
	if (param == 0)
	{
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
	}
	else
	{
		pCfg->fd3.seg_thr.seg_red         = (173 + margin_th_p[0]  -  margin_th_m[0] ); 
		pCfg->fd3.seg_thr.seg_re_ye       = (  8 + margin_th_p[1]  -  margin_th_m[1] );
		pCfg->fd3.seg_thr.seg_yellow      = ( 23 + margin_th_p[2]  -  margin_th_m[2] );
		pCfg->fd3.seg_thr.seg_ye_gr       = ( 38 + margin_th_p[3]  -  margin_th_m[3] );
		pCfg->fd3.seg_thr.seg_green       = ( 53 + margin_th_p[4]  -  margin_th_m[4] );
		pCfg->fd3.seg_thr.seg_gr_cy       = ( 68 + margin_th_p[5]  -  margin_th_m[5] );
		pCfg->fd3.seg_thr.seg_cyan        = ( 83 + margin_th_p[6]  -  margin_th_m[6] );
		pCfg->fd3.seg_thr.seg_cy_bl       = ( 98 + margin_th_p[7]  -  margin_th_m[7] );
		pCfg->fd3.seg_thr.seg_blue        = (113 + margin_th_p[8]  -  margin_th_m[8] );
		pCfg->fd3.seg_thr.seg_bl_ma       = (128 + margin_th_p[9]  -  margin_th_m[9] );
		pCfg->fd3.seg_thr.seg_magenta     = (143 + margin_th_p[10] -  margin_th_m[10]);
		pCfg->fd3.seg_thr.seg_ma_re       = (158 + margin_th_p[11] -  margin_th_m[11]);

		pCfg->fd3.seg_thr.seg_value_black =  32;     
		pCfg->fd3.seg_thr.seg_saturate    =  61;
		pCfg->fd3.seg_thr.seg_sdelta      =  30;
	}                                        

	APR_HAL_SetFd3Config(pCfg);

	return RET_OK;
}

static int APR_FD3_SubtitleMode(LX_APR_FORMAT *result)
{
	APR_DTA_T *pData = &apr_info.dta;
	APR_THR_T *pThr  = &apr_info.thr;
	APR_CFG_T *pCfg  = &apr_info.cfg;

	int i;

	int final_score_ss = 0; // score (SS) - sum of valid corr 
	int final_score_tb = 0; // score (TB) - sum of valid corr 

	int final_cnt_ss = 0;   // number of valid blk (SS) 
	int final_cnt_tb = 0;   // number of valid blk (TB)

	int final_th_ss;        // decision threshold (SS)
	int final_th_tb;        // decision threshold (TB)

	for (i=0; i<4; i++)
	{
		if (!pData->fd3.corr.ss_wnd_info[i])
		{
			final_cnt_ss += 1;
			final_score_ss += pData->fd3.corr.ss[i];
		}

		if (!pData->fd3.corr.tb_wnd_info[i])
		{
			final_cnt_tb += 1;
			final_score_tb += pData->fd3.corr.tb[i];
		}		
	}

	final_th_ss = pThr->fd3.subtitle * final_cnt_ss;
	final_th_tb = pThr->fd3.subtitle * final_cnt_tb;

	// No valid blk
	if (!final_cnt_ss) final_score_ss = -1;
	if (!final_cnt_tb) final_score_tb = -1;

	// format decision
	if ((!final_cnt_ss) && (!final_cnt_tb))
	{
		*result = LX_APR_FMT_UNKNOWN;
	}
	else
	{
		if      ( (final_score_ss >= final_th_ss) && (final_score_tb >= final_th_tb) )
			*result = LX_APR_FMT_UNKNOWN;
		else if (final_score_ss >= final_th_ss)
			*result = LX_APR_FMT_3D_SS;
		else if (final_score_tb >= final_th_tb)
			*result = LX_APR_FMT_3D_TB;
		else
			*result = LX_APR_FMT_2D_2D;

		// Exception for CG case
		if((*result == LX_APR_FMT_3D_SS) && (pData->tbr > 20))
		{
			FD3PRT("### m1+hfd  tbr[%d] subt exception ###\n", pData->tbr);
			*result = LX_APR_FMT_2D_2D;
			//*result = LX_APR_FMT_UNKNOWN;
		}
		else if((*result == LX_APR_FMT_3D_TB) && (pData->ssr > 20))
		{
			FD3PRT("### m1+hfd  ssr[%d] subt exception ###\n", pData->ssr);
			*result = LX_APR_FMT_2D_2D;
			//*result = LX_APR_FMT_UNKNOWN;
		}
	}

	FD3DBG("SubtitleMode\n");
	FD3DBG("final_cnt ss-tb[%d-%d] | final_score/th ss/th[%d/%d] tb/th[%d/%d] | result[%d]\n", \
	   final_cnt_ss, final_cnt_tb, final_score_ss, final_th_ss, final_score_tb, final_th_tb, *result);
	FD3MSG("SubtitleMode : result[%d]\n", *result);

	return RET_OK;
}

static int APR_FD3_VideoMode(LX_APR_FORMAT *result)
{
	APR_DTA_T *pData = &apr_info.dta;
	APR_THR_T *pThr  = &apr_info.thr;
	APR_CFG_T *pCfg  = &apr_info.cfg;

	LX_APR_FORMAT detect_format;
	LX_APR_FORMAT current_format;
	APR_IMG_CLASS_T m1_img_class;

	int i,iter;

	int bMask = pData->fd3.bMask; 

	int var_th            = pThr->fd3.variation;
	int corr_th           = pThr->fd3.correlation;
	int third_cor_th      = pThr->fd3.corr_3rd;
	int final_corr_th     = pThr->fd3.final_corr;
	int mean_tolerance_th = pThr->fd3.mean_tolerance;

	int cross_corr_th0;
	int cross_corr_th1;
	int cross_corr_en;

	int dom_seg_chk_en;
	int dom_seg_chk_th[2];
	int dom_seg_chk_th0;
	int dom_seg_chk_th1;

	int blk_mean_chk_en;

	int var_valid_blk;
	int candi_blk_thaw;

	int cnt_ss = 0, cnt1_ss = 0, cnt2_ss = 0;
	int cnt_tb = 0, cnt1_tb = 0, cnt2_tb = 0;
	int cnt_idx = 0;

	int sum_ss = 0, sum_tb = 0;
	int final_corr = 0;

	int scan_3d[4] = {0, 1, 2, 3};
	int corr_3d[4];

	int swi_val;
	int swi_idx;

	int mean_d[4], mean_d1[4];
	int tmp_mean_a, tmp_mean_b;
	int match_cor_mean = 1;
	int match_max_d    = 0;
	int di, di_a;

	int tmp_idx;
	int case_flag = 0;

	int tpd_flag = 0;

	int change_segment = 0;


	// temporally image width, height
	int img_width = pCfg->in_win_x;
	int img_height = pCfg->in_win_y;

	// seg[0] : current, seg[1] : current
	static int seg_flag = 0;
	static int seg_hsv_max_p = 0;
	static int seg_hsv_cnt_p = 0;
	static int seg_count = 0;
	int seg_di;
		
	int seg_hsv_max = pData->fd3.hsv_picture.pic_seg_hsv_max;
	int seg_hsv_cnt = pData->fd3.hsv_picture.pic_seg_hsv_count;

	if(pData->tpd >= 235)	tpd_flag = 1;

	if(pCfg->cap.enb)
		current_format = pCfg->fmt_before_vote[APR_METHOD_2];
	else
		current_format = pCfg->fmt_before_vote[APR_METHOD_1];

	m1_img_class = pCfg->img_class;

	if (m1_img_class == APR_IMG_CLASS_2D_HARD)
	{
		corr_th            = 248;
		third_cor_th       = 200;
		final_corr_th      = 220; 
		mean_tolerance_th  = 20;
		var_th             = 64;
		var_valid_blk      = 1;
		candi_blk_thaw     = 0;
		cross_corr_en      = 1;
		cross_corr_th0     = 30;
		cross_corr_th1     = 50;
		dom_seg_chk_en     = 1;
		dom_seg_chk_th0    = (img_width * img_height * 3)>>9; // 3/32 * blk_size
		dom_seg_chk_th1    = (img_width * img_height * 3)>>8; // 3/32 * blk_size
		blk_mean_chk_en    = 1;
	}
	else if (m1_img_class == APR_IMG_CLASS_TB_HARD)
	{
		corr_th            = 248;
		third_cor_th       = 160;
		final_corr_th      = 200; 
		mean_tolerance_th  = 20;
		var_th             = 0;
		var_valid_blk      = 2;
		candi_blk_thaw     = 1;
		cross_corr_en      = 0;
		cross_corr_th0     = 0;
		cross_corr_th1     = 0;
		dom_seg_chk_en     = 0;
		dom_seg_chk_th0    = (img_width * img_height * 3)>>9; // 3/32 * blk_size
		dom_seg_chk_th1    = (img_width * img_height * 3)>>8; // 3/32 * blk_size
		blk_mean_chk_en    = 0;
	}
	else if (m1_img_class == APR_IMG_CLASS_SS_HARD)
	{
		corr_th            = 248;
		third_cor_th       = 160;
		final_corr_th      = 200; 
		mean_tolerance_th  = 20;
		var_th             = 64;
		var_valid_blk      = 2;
		candi_blk_thaw     = 1;
		cross_corr_en      = 0;
		cross_corr_th0     = 0;
		cross_corr_th1     = 0;
		dom_seg_chk_en     = 0;
		dom_seg_chk_th0    = (img_width * img_height * 3)>>9; // 3/32 * blk_size
		dom_seg_chk_th1    = (img_width * img_height * 3)>>8; // 3/32 * blk_size
		blk_mean_chk_en    = 0;
	}
	else if ((m1_img_class == APR_IMG_CLASS_TB_WEAK) || (m1_img_class == APR_IMG_CLASS_2D_WEAK_TB))
	{
		corr_th            = 248;
		third_cor_th       = 179;
		final_corr_th      = 220; 
		mean_tolerance_th  = 20;
		var_th             = 0;
		var_valid_blk      = 1;
		candi_blk_thaw     = 0;
		cross_corr_en      = 1;
		cross_corr_th0     = 30;
		cross_corr_th1     = 50;
		dom_seg_chk_en     = 1;
		dom_seg_chk_th0    = (img_width * img_height * 3)>>9; // 3/32 * blk_size
		dom_seg_chk_th1    = (img_width * img_height * 3)>>8; // 3/32 * blk_size
		blk_mean_chk_en    = 1;
	}
	else if ((m1_img_class == APR_IMG_CLASS_SS_WEAK) || (m1_img_class == APR_IMG_CLASS_2D_WEAK_SS))
	{
		corr_th            = 248;
		third_cor_th       = 179;
		final_corr_th      = 220; 
		mean_tolerance_th  = 20;
		var_th             = 0;
		var_valid_blk      = 1;
		candi_blk_thaw     = 0;
		cross_corr_en      = 1;
		cross_corr_th0     = 30;
		cross_corr_th1     = 50;
		dom_seg_chk_en     = 1;
		dom_seg_chk_th0    = (img_width * img_height * 3)>>9; // 3/32 * blk_size
		dom_seg_chk_th1    = (img_width * img_height * 3)>>8; // 3/32 * blk_size
		blk_mean_chk_en    = 1;
	}
	else
	{
		corr_th            = 248;
		third_cor_th       = 179;
		final_corr_th      = 220; 
		mean_tolerance_th  = 20;
		var_th             = 0;
		var_valid_blk      = 1;
		candi_blk_thaw     = 0;
		cross_corr_en      = 0;
		cross_corr_th0     = 0;
		cross_corr_th1     = 0;
		dom_seg_chk_en     = 1;
		dom_seg_chk_th0    = (img_width * img_height * 3)>>9; // 3/32 * blk_size
		dom_seg_chk_th1    = (img_width * img_height * 3)>>8; // 3/32 * blk_size
		blk_mean_chk_en    = 1;
	}

	cross_corr_en = 0; // set to '0'

	FD3DBG("VideoMode\n");
	FD3DBG("var_th/corr_th/third_cor_th  %d/%d/%d\n", var_th,corr_th,third_cor_th);
	for(i=0;i<4;i++) {
		FD3DBG("Corr[%d] : ss/ss_wnd tb/tb_wnd - 0x%02X/0x%02X  0x%02X/0x%02X\n",\
							i, pData->fd3.corr.ss[i], pData->fd3.corr.ss_wnd_info[i],\
							pData->fd3.corr.tb[i], pData->fd3.corr.tb_wnd_info[i]); 
	}

	///////////////////////////////////////////////////////////////////////////
	// 1st decision - basic 
	///////////////////////////////////////////////////////////////////////////

	// condition check (SS)
	for (i=0; i<4; i++)
	{
		tmp_idx = (i<2) ? i : i+2;

		if (pData->fd3.corr.ss_wnd_info[i] == 0)
		{
			cnt1_ss++;
			if ( ((int)pData->fd3.blk_stat.s_dev[tmp_idx]   >= var_th) && \
				 ((int)pData->fd3.blk_stat.s_dev[tmp_idx+2] >= var_th) )
			{
				cnt_ss++;			
				sum_ss += (int)pData->fd3.corr.ss[i];
			}
		}

		if (pData->fd3.corr.ss_wnd_info[i] == 2) cnt2_ss++;
	}

	// condition check (TB)
	for (i=0; i<4; i++)
	{
		tmp_idx = i;

		if (pData->fd3.corr.tb_wnd_info[i] == 0)
		{
			cnt1_tb++;

#if 0
			if ( ((int)pData->fd3.blk_stat.s_dev[tmp_idx]   >= var_th) && \
				 ((int)pData->fd3.blk_stat.s_dev[tmp_idx+4] >= var_th) )
			{
				cnt_tb++;
				sum_tb += (int)pData->fd3.corr.tb[i];
			}
#else

			cnt_tb++;
			sum_tb += (int)pData->fd3.corr.tb[i];
#endif
			
		}

		if (pData->fd3.corr.tb_wnd_info[i] == 2) cnt2_tb++;
	}

	sum_ss = (cnt_ss == 0) ? 0 : sum_ss;
	sum_tb = (cnt_tb == 0) ? 0 : sum_tb;


	FD3DBG("sum_ss : %d sum_tb : %d\n",sum_ss,sum_tb );

	// format decision based on correlation
	if      (                 (cnt1_tb < 2) && (cnt1_ss < 2)) {
		detect_format = LX_APR_FMT_2D_2D;
		if (candi_blk_thaw) change_segment = 1;
		FD3ALG("SEG_ALT[0] : cnt1_tb, cnt1_ss = %d,%d\n",cnt1_tb,cnt1_ss);
	}
	else if ((bMask == 31) && (cnt_tb  < 2) && (cnt_ss  < 2)) detect_format = LX_APR_FMT_UNKNOWN;
	else if (                 (cnt_tb == 0) && (cnt_ss == 0)) detect_format = LX_APR_FMT_2D_2D;
	else
	{
		if      ((cnt_tb == 0) && (cnt_ss ==0)) case_flag = 0;
		else if ((cnt_tb == 0)                ) case_flag = 1;
		else if ((cnt_ss == 0)                ) case_flag = 2;		
		else 					case_flag = 3;	

		switch (case_flag)
		{
			case 0:
				if (cnt2_ss > var_valid_blk)           detect_format = LX_APR_FMT_2D_2D;
				else if (sum_tb > corr_th * cnt_tb) detect_format = LX_APR_FMT_2D_2D;
				else                       detect_format = LX_APR_FMT_2D_2D;

				cnt_idx    = cnt_ss;
				final_corr = sum_ss;
				break;
			case 1:
				if (cnt2_ss > var_valid_blk)           {
					detect_format = LX_APR_FMT_2D_2D;
					change_segment = 1;
				}
				else if (sum_tb > corr_th * cnt_tb) detect_format = LX_APR_FMT_2D_2D;
				else                       detect_format = LX_APR_FMT_3D_SS;

				cnt_idx    = cnt_ss;
				final_corr = sum_ss;
				break;
			case 2:
				if (cnt2_tb > var_valid_blk) {
					detect_format = LX_APR_FMT_2D_2D;
					change_segment = 1;
				}
				else if (sum_ss > corr_th * cnt_ss) detect_format = LX_APR_FMT_2D_2D;
				else                       detect_format = LX_APR_FMT_3D_TB;

				cnt_idx    = cnt_tb;
				final_corr = sum_tb;
				break;
			default :
				if (sum_tb*cnt_ss > sum_ss*cnt_tb)
				{
					if (cnt2_tb > var_valid_blk) {
						detect_format = LX_APR_FMT_2D_2D;
						change_segment = 1;
					}
					else if (sum_ss > corr_th * cnt_ss) detect_format = LX_APR_FMT_2D_2D;
					else                       detect_format = LX_APR_FMT_3D_TB;

					cnt_idx    = cnt_tb;
					final_corr = sum_tb;
				}
				else
				{
					if (cnt2_ss > var_valid_blk) {
						detect_format = LX_APR_FMT_2D_2D;
						change_segment = 1;
					}
					else if (sum_tb > corr_th * cnt_tb) detect_format = LX_APR_FMT_2D_2D;
					else                       detect_format = LX_APR_FMT_3D_SS;

					cnt_idx    = cnt_ss;
					final_corr = sum_ss;
				}
				break;
		}

		{
			int diff_corr_tb[2];
			int diff_corr_ss[2];

			int cross_corr_th[2];

			int large_corr_tb[2];
			int large_corr_ss[2];

			diff_corr_tb[0] = ( (int)pData->fd3.corr.tb[0] - (int)pData->fd3.corr.tb[2]);
			diff_corr_tb[1] = ( (int)pData->fd3.corr.tb[1] - (int)pData->fd3.corr.tb[3]);

			diff_corr_ss[0] = ( (int)pData->fd3.corr.ss[0] - (int)pData->fd3.corr.ss[2]);
			diff_corr_ss[1] = ( (int)pData->fd3.corr.ss[1] - (int)pData->fd3.corr.ss[3]);

			diff_corr_tb[0] = (diff_corr_tb[0] < 0) ? -diff_corr_tb[0] : diff_corr_tb[0];
			diff_corr_tb[1] = (diff_corr_tb[1] < 0) ? -diff_corr_tb[1] : diff_corr_tb[1];
			diff_corr_ss[0] = (diff_corr_ss[0] < 0) ? -diff_corr_ss[0] : diff_corr_ss[0];
			diff_corr_ss[1] = (diff_corr_ss[1] < 0) ? -diff_corr_ss[1] : diff_corr_ss[1];

			large_corr_tb[0] = (pData->fd3.corr.tb[0] >  pData->fd3.corr.tb[2]) ? pData->fd3.corr.tb[0] : pData->fd3.corr.tb[1];
			large_corr_ss[0] = (pData->fd3.corr.ss[0] >  pData->fd3.corr.ss[2]) ? pData->fd3.corr.ss[0] : pData->fd3.corr.ss[1];

			FD3DBG("diff_tb[0..1] : %d   %d\n",diff_corr_tb[0],diff_corr_tb[1]);
			FD3DBG("diff_ss[0..1] : %d   %d\n",diff_corr_ss[0],diff_corr_ss[1]);


			if (cross_corr_en == 1)
			{
				if (detect_format == LX_APR_FMT_3D_SS) 
				{
					cross_corr_th[0] = large_corr_tb[0] < 100 ? cross_corr_th0 : cross_corr_th1;
					cross_corr_th[1] = large_corr_tb[1] < 100 ? cross_corr_th0 : cross_corr_th1;

					if (diff_corr_tb[0] > cross_corr_th[0] || diff_corr_tb[1] > cross_corr_th[1])
					{
						detect_format = LX_APR_FMT_2D_2D;
					}
				}
				else if (detect_format == LX_APR_FMT_3D_TB)
				{
					cross_corr_th[0] = large_corr_ss[0] < 100 ? cross_corr_th0 : cross_corr_th1;
					cross_corr_th[1] = large_corr_ss[1] < 100 ? cross_corr_th0 : cross_corr_th1;

					if (diff_corr_ss[0] > cross_corr_th[0] || diff_corr_ss[1] > cross_corr_th[1])
					{
						detect_format = LX_APR_FMT_2D_2D;
					}
				}

			}
		}
	}

	pCfg->hfd_1_fmt = detect_format;

	FD3DBG("1st : cnt_ss/cnt1_ss/cnt2_ss - cnt_tb/cnt1_tb/cnt2_tb  %d/%d/%d - %d/%d/%d , result %d\n",\
			cnt_ss,cnt1_ss,cnt2_ss,cnt_tb,cnt1_tb,cnt2_tb,detect_format);
	if (final_corr != sum_tb) FD3DBG("Error : 1st Step\n");

	///////////////////////////////////////////////////////////////////////////
	// 2nd decision - 3rd largest correlation value
	///////////////////////////////////////////////////////////////////////////

	if ( (detect_format == LX_APR_FMT_3D_TB) || (detect_format == LX_APR_FMT_3D_SS) )
	{
		if (detect_format == LX_APR_FMT_3D_TB)
		{
			for (i=0; i<4; i++) corr_3d[i] = (pData->fd3.corr.tb_wnd_info[i]==0) ? pData->fd3.corr.tb[i] : -1;				 
		}
		else
		{
			for (i=0; i<4; i++) corr_3d[i] = (pData->fd3.corr.ss_wnd_info[i]==0) ? pData->fd3.corr.ss[i] : -1;				 
		}

		FD3DBG("2nd : corr_3d[0..3] = %d %d %d %d \n",corr_3d[0],corr_3d[1],corr_3d[2],corr_3d[3]);

		for (iter=0; iter<3; iter++)
		{
			for (i=0; i<3; i++)
			{
				if (corr_3d[i] < corr_3d[i+1])
				{
					swi_val      = corr_3d[i];
					corr_3d[i]   = corr_3d[i+1];
					corr_3d[i+1] = swi_val;

					swi_idx      = scan_3d[i];
					scan_3d[i]   = scan_3d[i+1];
					scan_3d[i+1] = swi_idx;
				}
			}
		}
		FD3DBG("2nd(A) : corr_3d[0..3] = %d %d %d %d \n",corr_3d[0],corr_3d[1],corr_3d[2],corr_3d[3]);

		if (corr_3d[2] < 0) 
		{
			corr_3d[2] = (corr_3d[1] < 0) ? corr_3d[0] : corr_3d[1];
		}

		if (corr_3d[2] < third_cor_th)
		{
			detect_format = LX_APR_FMT_2D_2D;
		}
	}
	
	pCfg->hfd_2_fmt = detect_format;

	FD3DBG("2nd : corr_3d[2]/third_cor_th  %d/%d , result %d\n", corr_3d[2],third_cor_th,detect_format);
	if (detect_format != LX_APR_FMT_3D_TB) FD3DBG("Error : 2nd step!\n");

	///////////////////////////////////////////////////////////////////////////
	// 3rd decision - mean-diff order and corr order
	///////////////////////////////////////////////////////////////////////////

	if (detect_format == LX_APR_FMT_3D_TB)
	{
		for (i=0; i<4; i++)
		{
			tmp_idx = i;
			tmp_mean_a = pData->fd3.blk_stat.mean[tmp_idx  ];
			tmp_mean_b = pData->fd3.blk_stat.mean[tmp_idx+4];

			mean_d[i] = (tmp_mean_a > tmp_mean_b) ? (tmp_mean_a - tmp_mean_b) : (tmp_mean_b - tmp_mean_a);
		}

		for (i=0; i<4; i++) mean_d1[i] = mean_d[scan_3d[i]];
	}
	else if (detect_format == LX_APR_FMT_3D_SS)
	{
		for (i=0; i<4; i++)
		{
			tmp_idx = (i<2) ? i : i+2;
			tmp_mean_a = pData->fd3.blk_stat.mean[tmp_idx  ];
			tmp_mean_b = pData->fd3.blk_stat.mean[tmp_idx+2];

			mean_d[i] = (tmp_mean_a > tmp_mean_b) ? (tmp_mean_a - tmp_mean_b) : (tmp_mean_b - tmp_mean_a);
		}

		for (i=0; i<4; i++) mean_d1[i] = mean_d[scan_3d[i]];
	}
	else
	{
		for (i=0; i<4; i++) mean_d1[i] = 0;
	}

	match_cor_mean = 1;
	match_max_d    = 0;

	FD3DBG("\nmean_d1[0..3] : %d/%d/%d/%d \n",mean_d1[0],mean_d1[1],mean_d1[2],mean_d1[3]);


	for (i=3; i>0; i--)
	{
		di   = mean_d1[i] - mean_d1[i-1];
		di_a = (di<0) ? -di : di;

		if (di_a > match_max_d) match_max_d = di_a;
		if (di < -20*16) match_cor_mean = 0;
	}

	if ( (detect_format != LX_APR_FMT_2D_2D) && (final_corr < corr_th*cnt_idx) && (bMask == 31) )
	{
		if (!match_cor_mean)
			detect_format = LX_APR_FMT_2D_2D;
		else if ( (final_corr > final_corr_th*cnt_idx) && (match_max_d<mean_tolerance_th*16) ) 
			detect_format = detect_format;
		else
			detect_format += 16;
		FD3DBG("3rd : match_cor_mean/final_corr/match_max_d  %d/%d/%d ,  result %d\n",\
				match_cor_mean, final_corr, match_max_d, detect_format);
	}

	pCfg->hfd_3_1_fmt = detect_format;

	if (dom_seg_chk_en==1)
	{
		int seg_idx[4], seg_max[4];
		int seg_max_di[2];
		int seg_idx_flag[2] = {0,0}; // 0 : HSV, 1 : MONO

		int i;

		for (i=0; i<4; i++) 
		{
			seg_idx[i] = pData->fd3.hsv_blk.seg_hsv_max[i];
			seg_max[i] = pData->fd3.hsv_blk.seg_hsv_count[i];
		}

		FD3DBG("seg_idx[0...3] : %d/%d/%d/%d\n",seg_idx[0],seg_idx[1],seg_idx[2],seg_idx[3]);

		if (detect_format == LX_APR_FMT_3D_SS) 
		{
			seg_idx_flag[0] = (seg_idx[0] < 12) ? 0 : 1;
			seg_idx_flag[1] = (seg_idx[2] < 12) ? 0 : 1;

			if (seg_idx[0]==seg_idx[1]) 
			{	
				seg_max_di[0] = (seg_max[0]>seg_max[1]) ? (seg_max[0] - seg_max[1]) : (seg_max[1] - seg_max[0]);
			}
			else if (( seg_idx[0]>=12) || (seg_idx[1] >= 12))
				seg_max_di[0] = -2;
			else
				seg_max_di[0] = -1;
			
			if (seg_idx[2]==seg_idx[3]) 
				seg_max_di[1] = (seg_max[2]>seg_max[3]) ? (seg_max[2] - seg_max[3]) : (seg_max[3] - seg_max[2]);
			else if (( seg_idx[2]>=12) || (seg_idx[3] >= 12))
				seg_max_di[1] = -2;
			else
				seg_max_di[1] = -1;
		}
		else if (detect_format == LX_APR_FMT_3D_TB)
		{
			seg_idx_flag[0] = (seg_idx[0] < 12) ? 0 : 1;
			seg_idx_flag[1] = (seg_idx[1] < 12) ? 0 : 1;

			if (seg_idx[0]==seg_idx[2]) 
				seg_max_di[0] = (seg_max[0]>seg_max[2]) ? (seg_max[0] - seg_max[2]) : (seg_max[2] - seg_max[0]);
			else if (( seg_idx[0]>=12) || (seg_idx[2] >= 12))
				seg_max_di[0] = -2;
			else
				seg_max_di[0] = -1;
			
			if (seg_idx[1]==seg_idx[3]) 
				seg_max_di[1] = (seg_max[1]>seg_max[3]) ? (seg_max[1] - seg_max[3]) : (seg_max[3] - seg_max[1]);
			else if (( seg_idx[1]>=12) || (seg_idx[3] >= 12))
				seg_max_di[1] = -2;
			else
				seg_max_di[1] = -1;
		}
		else
			seg_max_di[0] = seg_max_di[1] = -100;

		FD3DBG("seg_di[0...1] : %d/%d\n",seg_max_di[0],seg_max_di[1]);

		dom_seg_chk_th[0] = (seg_idx_flag[0] == 0) ? dom_seg_chk_th1 : dom_seg_chk_th0;
		dom_seg_chk_th[1] = (seg_idx_flag[1] == 0) ? dom_seg_chk_th1 : dom_seg_chk_th0;

		if ( (seg_idx[0] != 0) && (seg_max_di[0] > dom_seg_chk_th[0]))
		{	
			detect_format = LX_APR_FMT_2D_2D;
		}
		else if ( (seg_idx[1] != 0) && (seg_max_di[1] > dom_seg_chk_th[1]))
		{
			detect_format = LX_APR_FMT_2D_2D;
		}

#if 1
		if(detect_format == LX_APR_FMT_3D_SS) {
			int seg_max_sum[2] = {0, 0};
			int seg_max_cnt2 = 0;
			int seg_max_cnt3 = 0;

			int tmp_b[2] = {0, 0};
			int tmp_b_sum[2] = {0,0};

			//int tmp_b_sum_th0, tmp_b_sum_th1;
			int tmp_b_th0 = 0, tmp_b_th1 = 0;
			int tmp_b_sum_th0 = 0, tmp_b_sum_th1 = 0;

			for (i=0; i<4; i++) 
			{
				if (seg_idx[i] == 18) seg_max_cnt2++;
				else if (seg_idx[i] == 0) seg_max_cnt3++;
			}

			if (seg_idx[0] == 15) seg_max_cnt3++;
			if (seg_idx[1] == 15) seg_max_cnt3++;

			if (seg_max_cnt2 == 4 || seg_max_cnt3 == 4 )
			{
				seg_max_sum[0] = seg_max[0] + seg_max[1];
				seg_max_sum[1] = seg_max[2] + seg_max[3];

				if (seg_max_cnt2 ==4)
				{
					tmp_b_sum_th0 = 133;
					tmp_b_sum_th1 = 141;
				}
				else if (seg_max_cnt3 ==4)
				{
					tmp_b_sum_th0 = 90;
					tmp_b_sum_th1 = 145;
				}

				tmp_b_sum[0] = (tmp_b_sum_th0*img_width*img_height)>>11;
				tmp_b_sum[1] = (tmp_b_sum_th1*img_width*img_height)>>11;

				if (seg_max_cnt2 ==4)
				{
					tmp_b_th0 = 220;
					tmp_b_th1 = 250;
				}
				else if (seg_max_cnt3 ==4)
				{
					tmp_b_th0 = 208;
					tmp_b_th1 = 300;
				}

				tmp_b[0] = (tmp_b_th0*seg_max_sum[1]) >> 8;
				tmp_b[1] = (tmp_b_th1*seg_max_sum[1]) >> 8;

				FD3DBG("seg_max_sum[0] : %d  seg_max_sum[1] : %d\n",seg_max_sum[0],seg_max_sum[1]);
				FD3DBG("tmp_b[0]       : %d  tmp_b[1]       : %d\n",tmp_b[0],tmp_b[1]);
				FD3DBG("tmp_b_sum[0]   : %d  tmp_b_sum[1]   : %d\n",tmp_b_sum[0],tmp_b_sum[1]);

				if ( (seg_max_sum[1] > tmp_b_sum[0]) && (seg_max_sum[1] < tmp_b_sum[1]) )
				{
					FD3DBG("1st pass\n");
					if ( (seg_max_sum[0] > tmp_b[0]) && (seg_max_sum[0] < tmp_b[1]) )
					{
						FD3DBG("1st :  scene detect!!!\n");
						detect_format = LX_APR_FMT_2D_2D;
					}
				}
			}
		}
#endif

	}
	
	if (blk_mean_chk_en == 1)
	{
		int blk_mean[8];
		int L_census[4];
		int R_census[4];

		//int ref_min[2], ref_max[2];
		//int ref_min_pos[2], ref_max_pos[2];
		int ref_mean;

		int blk_mean0[4], blk_mean1[4];
		int blk_idx0[4], blk_idx1[4];

		int Hamming_Distance = 0;

		int m;
		
		for (m=0; m<8; m++)
		{
			blk_mean[m] = pData->fd3.blk_stat.mean[m];
		}
		memset(L_census, 0, sizeof(int)*4);
		memset(R_census, 0, sizeof(int)*4);
		memset(blk_mean0, 0, sizeof(int)*4);
		memset(blk_mean1, 0, sizeof(int)*4);
		memset(blk_idx0, 0, sizeof(int)*4);
		memset(blk_idx1, 0, sizeof(int)*4);

		if (bMask == 31)  
		{
			if (detect_format == LX_APR_FMT_3D_SS && cnt2_ss == 0)
			{				
				// copy 
				for (m=0; m<4; m++)
				{
					blk_mean0[m] = (m>1) ? blk_mean[m+2] : blk_mean[m];
					blk_mean1[m] = (m>1) ? blk_mean[m+4] : blk_mean[m+2];
					blk_idx0[m] = blk_idx1[m] = m;
				}

				// sorting 
				for (iter=0; iter<3; iter++)
				{
					for (i=0; i<3; i++)
					{
						if (blk_mean0[i] < blk_mean0[i+1])
						{
							swi_val      	= blk_mean0[i];
							blk_mean0[i]   	= blk_mean0[i+1];
							blk_mean0[i+1] 	= swi_val;

							swi_idx      = blk_idx0[i];
							blk_idx0[i]   = blk_idx0[i+1];
							blk_idx0[i+1] = swi_idx;
						}

						if (blk_mean1[i] < blk_mean1[i+1])
						{
							swi_val      	= blk_mean1[i];
							blk_mean1[i]   	= blk_mean1[i+1];
							blk_mean1[i+1] 	= swi_val;

							swi_idx      = blk_idx1[i];
							blk_idx1[i]   = blk_idx1[i+1];
							blk_idx1[i+1] = swi_idx;
						}
					}
				}
				FD3DBG("blk_idx = %d%d%d%d / %d%d%d%d \n",
						blk_idx0[0], blk_idx0[1], blk_idx0[2], blk_idx0[3],
						blk_idx1[0], blk_idx1[1], blk_idx1[2], blk_idx1[3]);

				// if ((blk_idx0[0] == blk_idx1[0]) && (blk_idx0[3] == blk_idx1[3])) break;
	
#if 1
				ref_mean = blk_mean0[3];
				ref_mean += 320; // 20(integer)
				
				L_census[0] = (blk_mean[0] > ref_mean ) ? 1 : 0;
				L_census[1] = (blk_mean[1] > ref_mean ) ? 1 : 0;
				L_census[2] = (blk_mean[4] > ref_mean ) ? 1 : 0;
				L_census[3] = (blk_mean[5] > ref_mean ) ? 1 : 0;


				ref_mean = blk_mean1[3];
				ref_mean += 320; // 20(integer)

				R_census[0] = (blk_mean[2] > ref_mean ) ? 1 : 0;
				R_census[1] = (blk_mean[3] > ref_mean ) ? 1 : 0;
				R_census[2] = (blk_mean[6] > ref_mean ) ? 1 : 0;
				R_census[3] = (blk_mean[7] > ref_mean ) ? 1 : 0;
#endif
				FD3DBG("Hamming_Distance = ");
				for (m=0; m<4; m++)
				{
					if (L_census[m] == R_census[m]) Hamming_Distance += 1;
					FD3DBG("%d ",(L_census[m] == R_census[m])?1:0);
				}
				FD3DBG("\n");

				if (Hamming_Distance < 3) detect_format = LX_APR_FMT_2D_2D;
			}
			else if (detect_format == LX_APR_FMT_3D_TB && cnt2_tb == 0)
			{ 
				// copy 
				for (m=0; m<4; m++)
				{
					blk_mean0[m] = blk_mean[m];
					blk_mean1[m] = blk_mean[m+4];
					blk_idx0[m] = blk_idx1[m] = m;
				}

				// sorting 
				for (iter=0; iter<3; iter++)
				{
					for (i=0; i<3; i++)
					{
						if (blk_mean0[i] < blk_mean0[i+1])
						{
							swi_val      	= blk_mean0[i];
							blk_mean0[i]   	= blk_mean0[i+1];
							blk_mean0[i+1] 	= swi_val;

							swi_idx      = blk_idx0[i];
							blk_idx0[i]   = blk_idx0[i+1];
							blk_idx0[i+1] = swi_idx;
						}

						if (blk_mean1[i] < blk_mean1[i+1])
						{
							swi_val      	= blk_mean1[i];
							blk_mean1[i]   	= blk_mean1[i+1];
							blk_mean1[i+1] 	= swi_val;

							swi_idx      = blk_idx1[i];
							blk_idx1[i]   = blk_idx1[i+1];
							blk_idx1[i+1] = swi_idx;
						}
					}
				}

				FD3DBG("blk_idx = %d%d%d%d / %d%d%d%d \n",
						blk_idx0[0], blk_idx0[1], blk_idx0[2], blk_idx0[3],
						blk_idx1[0], blk_idx1[1], blk_idx1[2], blk_idx1[3]);

				// if ((blk_idx0[0] == blk_idx1[0]) && (blk_idx0[3] == blk_idx1[3])) break;
#if 1
				ref_mean = blk_mean0[3];
				ref_mean += 320; // 20(integer)

				L_census[0] = (blk_mean[0] > ref_mean ) ? 1 : 0;
				L_census[1] = (blk_mean[1] > ref_mean ) ? 1 : 0;
				L_census[2] = (blk_mean[2] > ref_mean ) ? 1 : 0;
				L_census[3] = (blk_mean[3] > ref_mean ) ? 1 : 0;


				ref_mean = blk_mean1[3];
				ref_mean += 320; // 20(integer)

				R_census[0] = (blk_mean[4] > ref_mean ) ? 1 : 0;
				R_census[1] = (blk_mean[5] > ref_mean ) ? 1 : 0;
				R_census[2] = (blk_mean[6] > ref_mean ) ? 1 : 0;
				R_census[3] = (blk_mean[7] > ref_mean ) ? 1 : 0;
#endif
				FD3DBG("Hamming_Distance = ");
				for (m=0; m<4; m++)
				{
					if (L_census[m] == R_census[m]) Hamming_Distance += 1;
					FD3DBG("%d ",(L_census[m] == R_census[m])?1:0);
				}
				FD3DBG("\n");

				if (Hamming_Distance < 3) detect_format = LX_APR_FMT_2D_2D;
			}
		}
	}

	pCfg->hfd_3_fmt = detect_format;

	// Exception for CG case
	if(detect_format == LX_APR_FMT_3D_SS)
	{
		if((pData->tbr > 20) || (pData->tbr > pData->ssr))
		{
			//detect_format = LX_APR_FMT_UNKNOWN;
			detect_format = LX_APR_FMT_2D_2D;
			FD3PRT("### m1+hfd exception 1 ###\n");
		}
	}
	else if(detect_format == LX_APR_FMT_3D_TB)
	{
		if((pData->ssr > 20) || (pData->ssr > pData->tbr))
		{
			//detect_format = LX_APR_FMT_UNKNOWN;
			detect_format = LX_APR_FMT_2D_2D;
			FD3PRT("### m1+hfd exception 2 ###\n");
		}
	}
	if(detect_format == LX_APR_FMT_3D_TB)
	{
		if( (m1_img_class == APR_IMG_CLASS_2D_WEAK_SS) || \
				(m1_img_class == APR_IMG_CLASS_SS_HARD   ) || \
				(m1_img_class == APR_IMG_CLASS_SS_WEAK   )    )
		{
			//detect_format = LX_APR_FMT_UNKNOWN;
			detect_format = LX_APR_FMT_2D_2D;
			FD3PRT("### m1+hfd exception 3 ###\n");
		}
		else if(m1_img_class == APR_IMG_CLASS_2D_HARD)
		{
			detect_format = LX_APR_FMT_UNKNOWN;
			FD3PRT("### m1+hfd exception 3 -- 1  ###\n");
		}
	}
	else if(detect_format == LX_APR_FMT_3D_SS)
	{
		if( (m1_img_class == APR_IMG_CLASS_2D_WEAK_TB) || \
				(m1_img_class == APR_IMG_CLASS_TB_HARD   ) || \
				(m1_img_class == APR_IMG_CLASS_TB_WEAK   )    )
		{
			//detect_format = LX_APR_FMT_UNKNOWN;
			detect_format = LX_APR_FMT_2D_2D;
			FD3PRT("### m1+hfd exception 4 ###\n");
		}
		else if(m1_img_class == APR_IMG_CLASS_2D_HARD)
		{
			detect_format = LX_APR_FMT_UNKNOWN;
			FD3PRT("### m1+hfd exception 4 -- 1  ###\n");
		}
	}
	// Exception end

	if(tpd_flag)
		*result = LX_APR_FMT_2D_2D;
	else
		*result = (detect_format >= 16) ? LX_APR_FMT_UNKNOWN : detect_format;

	//FD3MSG("VideoMode : result[%d]\n", *result);
	FD3DBG("VideoMode : result[%d] tpd_flag[%d]\n", *result, tpd_flag);
	FD3ALG("seg_flag, seg_hsv_max = %d / %d\n",seg_flag,seg_hsv_max);


	// seg[0] : current, seg[1] : current
	if (seg_flag == 0) seg_count = 0;
	else seg_count ++;

	seg_di = seg_hsv_max - seg_hsv_max_p;
	seg_di = (seg_di < 0) ? -seg_di : seg_di;
	seg_di = (seg_di == 11) ? 1 : seg_di;

	if (( seg_hsv_max < 12 ) && (seg_hsv_max_p < 12))
	{
		if (seg_flag == 0)
		{
			if (change_segment)
			{
				APR_FD3_ChangeThreshold(0);
				APR_FD3_ChangeThreshold(1);
				seg_flag = 1;
				seg_hsv_max_p = seg_hsv_max;
				seg_hsv_cnt_p = seg_hsv_cnt;
				FD3ALG("change_segment, seg_hsv_max = %d\n",seg_hsv_max);
			}
		}
		else if (seg_di > 1) 
		{
			APR_FD3_ChangeThreshold(0);
			seg_flag = 0;
			FD3ALG("thaw0, seg_hsv_max = %d\n",seg_hsv_max);
		}
		else if ( (seg_di==1) && (change_segment==1))
		{
			seg_flag = 0;
			APR_FD3_ChangeThreshold(0);
			FD3ALG("thaw1, seg_hsv_max = %d\n",seg_hsv_max);
		}
	}
	else if ( (seg_flag==1) && (( seg_hsv_max > 12 ) || (seg_hsv_max_p > 12)) )
	{
		APR_FD3_ChangeThreshold(0);
		seg_flag = 0;
		FD3ALG("thaw2, seg_hsv_max = %d\n",seg_hsv_max);
	}
	
	if ( (seg_flag==1) && (bMask ==31) )
	{
		APR_FD3_ChangeThreshold(0);
		seg_flag = 0;
		FD3ALG("tha32, seg_hsv_max = %d\n",seg_hsv_max);
	}
	

	return RET_OK;
}

int APR_FD3_Detect3D(void)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = &apr_info.cfg;
	APR_DTA_T *pData = &apr_info.dta;
	LX_APR_FORMAT m2_fmt = LX_APR_FMT_2D_2D;

	FD3DBG("\n\nStart]]] FD3 cfg : sampling[%d](0:all,1:2line) , scan[%d](0:P,1:I) , color[%d](0:420,1:422:2:444)\n",\
			pCfg->fd3.sampling, pCfg->fd3.scan, pCfg->fd3.color);
	FD3DBG("status  : init_count[0x%X] suc_fail_history[0x%X]\n", \
			pData->fd3.status.init_count, pData->fd3.status.suc_fail_history);
	FD3DBG("Image Class by method 1 :  %d\n", pCfg->img_class);
	FD3DBG("  (0:TP,1:2DHARD,2:2DWSS,3:2DWTB,4:SSHARD,5:SSWEAK,6:TBHARD,7:TBWEAK)\n");
	do {
		UINT32 alt_seg = 0;
		if ( pData->tpd < 210 )
	   	{
			alt_seg = 3;
			APR_HAL_SetFd3SubtConfig(pCfg, alt_seg);
		}

		if(pData->fd3.scene_type) 
		{				
			ret = APR_FD3_SubtitleMode(&m2_fmt);
		}
		else
		{
			if ( (pData->tpd > 210) && (pData->fd3.bMask != 31) ) 
			{
				alt_seg = pData->fd3.bMask;
				APR_HAL_SetFd3SubtConfig(pCfg, alt_seg);
			}

			ret = APR_FD3_VideoMode(&m2_fmt);
		}
		pCfg->fd3_fmt = m2_fmt;
		FD3DBG("SubtConfig : alt_seg [%d]\n", alt_seg);
	} while(0);

	FD3DBG("\nAlgorithm end] FD3 result : %d (0:2D,4:SS,5:TB)\n\n", m2_fmt);
	FD3DBG("###########################################################################\n");
	if(++g_msg_cnt==9) g_msg_cnt = 0;
	return ret;
}

// vim:fdm=marker
