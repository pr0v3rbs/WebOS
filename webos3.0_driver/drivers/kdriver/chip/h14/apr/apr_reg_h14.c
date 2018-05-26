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


/** @file apr_reg_h14a0.c
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
#include <asm/io.h>

#include "base_types.h"
#include "os_util.h"

#include "apr_def.h"
#include "apr_core.h"
#include "apr_reg_h14.h"

/* external de module */
#include "../de/de_cfg_h14.h"
#include "../de/de_ctr_reg_h14.h"
#include "../de/de_mif_reg_h14.h"
#include "../de/de_wcp_reg_h14.h"
#include "../de/de_msc_reg_h14.h"
#include "../de/de_ssc_reg_h14.h"

/* external pe module */
#include "pe_cfg.h"
#include "pe_reg_h14.h"
#include "pe_hst_hw_h14.h"

/* external venc module */
#include "venc_cfg.h"

/* h14-a0 fd3 */
#include "apr_fd3_reg_h14.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define APR_FD3_REG_H14_BASE (H14_DE_BASE + 0x0900)
#define APR_H3D_REG_H14_BASE (H14_BVE_BASE + 0x4000)

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/
#define ROW2PHY(a) (a << 15)

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
extern DE_DEA_REG_H14_T gDE_DEA_H14;
extern DE_DEB_REG_H14_T gDE_DEB_H14;
extern DE_DEC_REG_H14_T gDE_DEC_H14;
extern DE_MIA_REG_H14_T gDE_MIA_H14;
extern DE_MIB_REG_H14_T gDE_MIB_H14;
extern DE_MID_REG_H14_T gDE_MID_H14;
extern DE_OVL_REG_H14_T gDE_OVL_H14;
extern DE_OVR_REG_H14_T gDE_OVR_H14;
extern DE_MSL_REG_H14_T gDE_MSL_H14;
extern DE_MSR_REG_H14_T gDE_MSR_H14;
extern DE_SSC_REG_H14_T gDE_SSC_H14;
extern LX_VENC_MEM_CFG_S_T	*gpstVencMemConfig;

/*----------------------------------------------------------------------------------------
	global Functions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
volatile UINT8 *gpMBase_ssc_h14  = NULL;
volatile UINT8 *gpMBase_venc_h14 = NULL;
volatile UINT32 *gpTimerBase_h14 = NULL;
volatile UINT32 *gpH3DInfoBase_h14 = NULL;
APR_FD3_REG_H14_T gAPR_FD3_H14;

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Static Variables
----------------------------------------------------------------------------------------*/

/*========================================================================================
	Implementation Group
========================================================================================*/

int APR_H14_Init(void)
{
	int ret = RET_OK;

	if(gpMBase_ssc_h14 == NULL)
	{
		gpMBase_ssc_h14 = (volatile UINT8 *)ioremap(VIDEO_H14_FRAME_DDR_BASE, VIDEO_H14_MEM_SIZE_OF_FRAME_MEMORY);
	}

	if(gpMBase_venc_h14 == NULL)
	{
		if(gpstVencMemConfig->uiInBufBase != 0)
		{
			gpMBase_venc_h14 = (volatile UINT8 *)ioremap(gpstVencMemConfig->uiInBufBase, gpstVencMemConfig->uiInBufSize);
		}
	}

	if(gpTimerBase_h14 == NULL)
	{
		gpTimerBase_h14 = (volatile UINT32 *)ioremap(H14_TIMER_BASE + 0x24, 4);
	}

	if(lx_chip_rev() >= LX_CHIP_REV(H14,A0))
	{
		gAPR_FD3_H14.shdw.addr = (UINT32 *)OS_KMalloc(sizeof(APR_FD3_REG_H14A0_T));
		gAPR_FD3_H14.phys.addr = (volatile UINT32 *)ioremap(APR_FD3_REG_H14_BASE, sizeof(APR_FD3_REG_H14A0_T));
		gpH3DInfoBase_h14      = (volatile UINT32 *)ioremap(APR_H3D_REG_H14_BASE + 0xB4, sizeof(UINT32) * 8);
	}

	return ret;
}

int APR_H14_Uninit(void)
{
	int ret = RET_OK;
	
	if (gpMBase_ssc_h14) iounmap((void *)gpMBase_ssc_h14);
	if (gpMBase_venc_h14) iounmap((void *)gpMBase_venc_h14);
	if (gpTimerBase_h14) iounmap((void *)gpTimerBase_h14);
	if(lx_chip_rev() >= LX_CHIP_REV(H14,A0))
	{
		if(gAPR_FD3_H14.shdw.addr) OS_Free((void *)gAPR_FD3_H14.shdw.addr);
		if(gAPR_FD3_H14.phys.addr) iounmap((void *)gAPR_FD3_H14.phys.addr);
		if(gpH3DInfoBase_h14)      iounmap((void *)gpH3DInfoBase_h14);
	}

	return ret;
}

int APR_H14_InitHardware(void)
{
	int ret = RET_OK;

	do {
		PE_P0L_H14_RdFL(p0l_3dfd_ctrl_00);
		PE_P0L_H14_RdFL(p0l_3dfd_ctrl_01);
		PE_P0L_H14_RdFL(p0l_3dfd_ctrl_02);
		PE_P0L_H14_RdFL(p0l_3dfd_ctrl_03);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_00, aw_offset_x, 0);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_00, aw_offset_y, 0);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_00, cb_th1, 0x3C);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_00, cb_th2, 0x14);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_01, cc_th1, 0x3C);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_01, cc_th2, 0x14);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_01, ll_th1, 0x3C);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_01, ll_th2, 0x14);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_02, pt_th, 0x80);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_02, sstb_th, 0x07);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_02, div_mode, 0);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_03, small_delta_en, 0);
		PE_P0L_H14_Wr01(p0l_3dfd_ctrl_03, small_delta_th, 0xFF);
		PE_P0L_H14_WrFL(p0l_3dfd_ctrl_00);
		PE_P0L_H14_WrFL(p0l_3dfd_ctrl_01);
		PE_P0L_H14_WrFL(p0l_3dfd_ctrl_02);
		PE_P0L_H14_WrFL(p0l_3dfd_ctrl_03);
	} while(0);

	return ret;
}

int APR_H14_GetMaxFromBuffer(UINT32 *pBuf, UINT32 count, UINT32 *pElect)
{
    int ret = RET_OK;
	UINT32 i=0, j=0;
	UINT32 temp=0;

	do {
		for(i=0;i<count;i++)
		{
			for(j=0;j<count;j++)
			{
				if(pBuf[j] >= temp)
				{
					temp = pBuf[j];
				}
			}
		}
		*pElect = temp;
	} while(0);
	return ret;
}

int APR_H14_CollectBasicData(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data)
{
    int ret = RET_OK;
	APR_CFG_T *pCfg  = apr_cfg;
	APR_DTA_T *pData = apr_data;
	static UINT32 motion_l[4] = { 0 };
	static UINT32 motion_r[4] = { 0 };
	UINT32 scan_info  = 0;
	UINT32 color_info = 0;

	do {
#ifndef _DISABLE_ACCESS_TO_PE_
		PE_P0L_H14_RdFL(p0l_tpd_stat_00);
// rm		PE_P0L_H14_Rd01(p0l_tpd_stat_00, reg_s_status, pData->tpd);
		PE_P0L_H14_RdFL(p0l_3dfd_stat_00);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_00, cb_count, pData->cb);
		PE_P0L_H14_RdFL(p0l_3dfd_stat_00);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_00, cc_count, pData->cc);
		PE_P0L_H14_RdFL(p0l_3dfd_stat_01);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_01, ll_count, pData->ll);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_01, trid_format, pData->hw_fmt);

		PE_P0L_H14_RdFL(p0l_3dfd_stat_02);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_02, delta_ss_h, pData->ssh);
		PE_P0L_H14_RdFL(p0l_3dfd_stat_03);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_03, delta_ss_v, pData->ssv);
		PE_P0L_H14_RdFL(p0l_3dfd_stat_04);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_04, delta_tb_v, pData->tbv);
		PE_P0L_H14_RdFL(p0l_3dfd_stat_05);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_05, delta_tb_h, pData->tbh);

		PE_P0L_H14_RdFL(p0l_3dfd_stat_06);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_06, ss_ratio, pData->ssr);
		PE_P0L_H14_RdFL(p0l_3dfd_stat_07);
		PE_P0L_H14_Rd01(p0l_3dfd_stat_07, tb_ratio, pData->tbr);

		PE_P0L_H14_RdFL(p0l_tnr_status_00);
		PE_P0L_H14_RdFL(p0l_tnr_status_01);
		PE_P0L_H14_RdFL(p0l_apl_stat_00);
		motion_l[3] = motion_l[2];
		motion_l[2] = motion_l[1];
		motion_l[1] = motion_l[0];
// rm		PE_P0L_H14_Rd01(p0l_tnr_status_00, x_avg_t, motion_l[0]);
		ret = APR_H14_GetMaxFromBuffer(motion_l, 4, &pData->motion_l);
// rm		PE_P0L_H14_Rd01(p0l_tnr_status_00, nt_lvl, pData->nt_l);
// rm		PE_P0L_H14_Rd01(p0l_tnr_status_01, x_avg_s, pData->spatial_l);
// rm		PE_P0L_H14_Rd01(p0l_apl_stat_00, p0l_apl_stat_00_f, pData->apl_l);

		PE_P0R_H14_RdFL(p0r_tnr_status_00);
		PE_P0R_H14_RdFL(p0r_tnr_status_01);
		PE_P0R_H14_RdFL(p0r_apl_stat_00);
		motion_r[3] = motion_r[2];
		motion_r[2] = motion_r[1];
		motion_r[1] = motion_r[0];
// rm		PE_P0R_H14_Rd01(p0r_tnr_status_00, x_avg_t, motion_r[0]);
		ret = APR_H14_GetMaxFromBuffer(motion_r, 4, &pData->motion_r);
// rm		PE_P0R_H14_Rd01(p0r_tnr_status_00, nt_lvl, pData->nt_r);
// rm		PE_P0R_H14_Rd01(p0r_tnr_status_01, x_avg_s, pData->spatial_r);
// rm		PE_P0R_H14_Rd01(p0r_apl_stat_00, p0r_apl_stat_00_f, pData->apl_r);
#endif //#ifndef _DISABLE_ACCESS_TO_PE_
		DE_OVL_H14_RdFL(ovl_ov_msize);
		DE_OVL_H14_Rd01(ovl_ov_msize, m_hsize, pCfg->out_win_x);
		DE_OVL_H14_Rd01(ovl_ov_msize, m_vsize, pCfg->out_win_y);

		DE_DEA_H14_RdFL(dea_cha_info);
		DE_DEA_H14_Rd01(dea_cha_info, cha_hsize, pCfg->in_win_x);
		DE_DEA_H14_Rd01(dea_cha_info, cha_vsize, pCfg->in_win_y);
		
		PE_P0L_H14_RdFL(p0l_pe0_src_size);
		PE_P0L_H14_Rd01(p0l_pe0_src_size, scan_type, scan_info);
		PE_P0L_H14_Rd01(p0l_pe0_src_size, cs_type, color_info);

		pCfg->scan      = scan_info;
		pCfg->fd3.scan  = (scan_info==0)?1:0;
		switch(color_info)
		{
			case 6: // 444
				pCfg->fd3.color = 2;
				break;
			case 5: // 422
				pCfg->fd3.color = 1;
				break;
			case 4: // 420
			default:
				pCfg->fd3.color = 0;
				break;
		}

#ifndef _DISABLE_ACCESS_TO_PE_
		PE_P1L_H14_RdFL(pe1_conv_ctrl_03);
		PE_P1L_H14_Rd01(pe1_conv_ctrl_03, pxl_rep_in, pCfg->mute_status);
#endif
		if(pCfg->fd3.enb == LX_APR_FD3_LOG)
		{
			APR_PRINT("in x/y [%d/%d] , scan %d , color %d\n", \
					pCfg->in_win_x, pCfg->in_win_y, scan_info, color_info);
		}
	}while(0);

	return ret;
}

int APR_H14_GetOVInfo(APR_CFG_T *apr_cfg, APR_CAP_SRC_T ov_src, APR_OV_INFO_T *ov_info)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg = apr_cfg;
	UINT32 stride=0;
	UINT32 intr_mask;
	BOOLEAN sel_scaler_l = FALSE;
	BOOLEAN sel_scaler_r = FALSE;
	BOOLEAN sel_scaler_s = FALSE;

	do {
		switch (ov_src)
		{
			case APR_CAP_SRC_L_OVR :
			{
				DE_DEA_H14_RdFL(dea_intr_mask);
/*chg*/				DE_DEA_H14_Rd01(dea_intr_mask, mask_src_intr, intr_mask);
/*chg*/				//ov_info->src_valid = ((intr_mask & (1<<23)) == 0)?TRUE:FALSE;
/*chg*/				ov_info->src_valid = (intr_mask==0)?TRUE:FALSE;

				DE_MIB_H14_RdFL(mifb_saddr_rd_ext0);
				DE_MIB_H14_Rd01(mifb_saddr_rd_ext0, req_rd_saddr_8_ov_y_msb4b, ov_info->y_msb);
				DE_MIB_H14_Rd01(mifb_saddr_rd_ext0, req_rd_saddr_8_ov_c_msb4b, ov_info->c_msb);

				DE_MIB_H14_RdFL(mifb_saddr_rd_ov_y);
				DE_MIB_H14_Rd01(mifb_saddr_rd_ov_y, req_rd_saddr_8_ov_y, ov_info->y_base);
				DE_MIB_H14_RdFL(mifb_saddr_rd_ov_c);
				DE_MIB_H14_Rd01(mifb_saddr_rd_ov_c, req_rd_saddr_8_ov_c, ov_info->c_base);

				DE_MIB_H14_RdFL(mifb_rd_set_10b);
				DE_MIB_H14_Rd01(mifb_rd_set_10b, set_10b_req_rd_ov_y, ov_info->bpp);

				ov_info->stride = 2048;

				DE_OVL_H14_RdFL(ovl_ov_msize);
				DE_OVL_H14_Rd01(ovl_ov_msize, m_hsize, ov_info->h);
				DE_OVL_H14_Rd01(ovl_ov_msize, m_vsize, ov_info->v);

				DE_MSL_H14_RdFL(msc_c_format_l);
				DE_MSL_H14_Rd01(msc_c_format_l, out_c_format, ov_info->sub_sample);
	
				DE_MSL_H14_RdFL(msc_win_in_offset_l);
				DE_MSL_H14_Rd01(msc_win_in_offset_l, in_win_h_offset, ov_info->input.h_offset);
				DE_MSL_H14_Rd01(msc_win_in_offset_l, in_win_v_offset, ov_info->input.v_offset);
				DE_MSL_H14_RdFL(msc_win_in_size_l);
				DE_MSL_H14_Rd01(msc_win_in_size_l, in_win_h_size, ov_info->input.h_size);
				DE_MSL_H14_Rd01(msc_win_in_size_l, in_win_v_size, ov_info->input.v_size);

				ov_info->frame_base_phys[0] = ROW2PHY(ov_info->y_base);
				sel_scaler_l = TRUE;
			}
			break;
			case APR_CAP_SRC_R_OVR :
			{
				DE_DEA_H14_RdFL(dea_intr_mask);
/*chg*/				DE_DEA_H14_Rd01(dea_intr_mask, mask_src_intr, intr_mask);
/*chg*/				//ov_info->src_valid = ((intr_mask & (1<<23)) == 0)?TRUE:FALSE;
/*chg*/				ov_info->src_valid = (intr_mask==0)?TRUE:FALSE;

				DE_MID_H14_RdFL(mifd_saddr_rd_ext0);
				DE_MID_H14_Rd01(mifd_saddr_rd_ext0, req_rd_saddr_ov_y_msb4b, ov_info->y_msb);
				DE_MID_H14_Rd01(mifd_saddr_rd_ext0, req_rd_saddr_ov_c_msb4b, ov_info->c_msb);

				DE_MID_H14_RdFL(mifd_saddr_rd_ov_y_ov_c);
				DE_MID_H14_Rd01(mifd_saddr_rd_ov_y_ov_c, req_rd_saddr_ov_y, ov_info->y_base);
				DE_MID_H14_Rd01(mifd_saddr_rd_ov_y_ov_c, req_rd_saddr_ov_c, ov_info->c_base);

				//DE_MID_H14_RdFL(mifd_rd_set_10b);
				//DE_MID_H14_Rd01(mifd_rd_set_10b, set_10b_req_rd_ov_y, ov_info->bpp);

				ov_info->stride = 2048;

				DE_OVR_H14_RdFL(ovr_ov_msize);
				DE_OVR_H14_Rd01(ovr_ov_msize, m_hsize, ov_info->h);
				DE_OVR_H14_Rd01(ovr_ov_msize, m_vsize, ov_info->v);

				DE_MSR_H14_RdFL(msc_c_format_r);
				DE_MSR_H14_Rd01(msc_c_format_r, out_c_format, ov_info->sub_sample);
	
				DE_MSR_H14_RdFL(msc_win_in_offset_r);
				DE_MSR_H14_Rd01(msc_win_in_offset_r, in_win_h_offset, ov_info->input.h_offset);
				DE_MSR_H14_Rd01(msc_win_in_offset_r, in_win_v_offset, ov_info->input.v_offset);
				DE_MSR_H14_RdFL(msc_win_in_size_r);
				DE_MSR_H14_Rd01(msc_win_in_size_r, in_win_h_size, ov_info->input.h_size);
				DE_MSR_H14_Rd01(msc_win_in_size_r, in_win_v_size, ov_info->input.v_size);
			
				ov_info->frame_base_phys[1] = ROW2PHY(ov_info->y_base);
				sel_scaler_r = TRUE;
			}
			break;
			case APR_CAP_SRC_S_CVR :
			{
				DE_DEC_H14_RdFL(dec_intr_mask);
/*chg*/				DE_DEC_H14_Rd01(dec_intr_mask, mask_ssrc_intr, intr_mask);
/*chg*/				//ov_info->src_valid = ((intr_mask & (1<<16)) == 0)?TRUE:FALSE;
/*chg*/				ov_info->src_valid = (intr_mask==0)?TRUE:FALSE;

				DE_MID_H14_RdFL(mifd_saddr_rd_ext0);
				DE_MID_H14_Rd01(mifd_saddr_rd_ext0, req_rd_saddr_cve_y_msb4b, ov_info->y_msb);
				DE_MID_H14_Rd01(mifd_saddr_rd_ext0, req_rd_saddr_cve_c_msb4b, ov_info->c_msb);

				DE_MID_H14_RdFL(mifd_saddr_rd_cve_y_cve_c);
				DE_MID_H14_Rd01(mifd_saddr_rd_cve_y_cve_c, req_rd_saddr_cve_y, ov_info->y_base);
				DE_MID_H14_Rd01(mifd_saddr_rd_cve_y_cve_c, req_rd_saddr_cve_c, ov_info->c_base);

				DE_SSC_H14_RdFL(ssc_win_out_size);
				DE_SSC_H14_Rd01(ssc_win_out_size, out_win_h_size, ov_info->h);
				DE_SSC_H14_Rd01(ssc_win_out_size, out_win_v_size, ov_info->v);

				DE_SSC_H14_RdFL(ssc_ctrl);
				DE_SSC_H14_Rd01(ssc_ctrl, dvr_write_mode, stride);
				if(stride)
					ov_info->stride = ((ov_info->h + 15) / 16) * 16;
				else
					ov_info->stride = 1024;

				DE_SSC_H14_RdFL(ssc_c_format);
				DE_SSC_H14_Rd01(ssc_c_format, out_c_format, ov_info->sub_sample);

				DE_SSC_H14_RdFL(ssc_win_in_offset);
				DE_SSC_H14_Rd01(ssc_win_in_offset, in_win_h_offset, ov_info->input.h_offset);
				DE_SSC_H14_Rd01(ssc_win_in_offset, in_win_v_offset, ov_info->input.v_offset);
				DE_SSC_H14_RdFL(ssc_win_in_size);
				DE_SSC_H14_Rd01(ssc_win_in_size, in_win_h_size, ov_info->input.h_size);
				DE_SSC_H14_Rd01(ssc_win_in_size, in_win_v_size, ov_info->input.v_size);

				ov_info->frame_base_phys[0] = ROW2PHY(ov_info->y_base);
				sel_scaler_s = TRUE;
			}
			break;
			case APR_CAP_SRC_L_TNR :
			{
				UINT32 scan = 0;
/*chg*/				DE_DEA_H14_RdFL(dea_intr_mask);
/*chg*/				DE_DEA_H14_Rd01(dea_intr_mask, mask_src_intr, intr_mask);
/*chg*/				//ov_info->src_valid = ((intr_mask & (1<<23)) == 0)?TRUE:FALSE;
/*chg*/				ov_info->src_valid = (intr_mask == 0)?TRUE:FALSE;

				DE_MIA_H14_RdFL(mifa_saddr_rd_ext0);
				DE_MIA_H14_RdFL(mifa_saddr_rd_ext1);
				DE_MIA_H14_Rd01(mifa_saddr_rd_ext0, req_rd_saddr_8_ipc0_y_msb4b, ov_info->y_msb);
				DE_MIA_H14_Rd01(mifa_saddr_rd_ext1, req_rd_saddr_8_ipc0_c_msb4b, ov_info->c_msb);

				DE_MIA_H14_RdFL(mifa_saddr_rd_ipc0_y);
				DE_MIA_H14_Rd01(mifa_saddr_rd_ipc0_y, req_rd_saddr_8_ipc0_y, ov_info->y_base);
				DE_MIA_H14_RdFL(mifa_saddr_rd_ipc0_c);
				DE_MIA_H14_Rd01(mifa_saddr_rd_ipc0_c, req_rd_saddr_8_ipc0_c, ov_info->c_base);

				PE_P0L_H14_RdFL(p0l_pe0_src_size);
				PE_P0L_H14_Rd01(p0l_pe0_src_size, hsize, ov_info->h);
				PE_P0L_H14_Rd01(p0l_pe0_src_size, vsize, ov_info->v);
				PE_P0L_H14_Rd01(p0l_pe0_src_size, scan_type, scan);
				ov_info->v            = (scan==0)?ov_info->v>>1:ov_info->v;
				ov_info->input.h_size = ov_info->h;
				ov_info->input.v_size = ov_info->v;
				ov_info->stride = 2048;

				DE_MSL_H14_RdFL(msc_c_format_l);
				DE_MSL_H14_Rd01(msc_c_format_l, out_c_format, ov_info->sub_sample);

				ov_info->frame_base_phys[0] = ROW2PHY(ov_info->y_base);
				sel_scaler_l = TRUE;
			}
			break;
			case APR_CAP_SRC_L_PRW :
			case APR_CAP_SRC_R_PRW :
			case APR_CAP_SRC_R_TNR :
			default :
				ov_info->src_valid = FALSE;
				break;
		}

		ov_info->pFrameBase = NULL;
		
		if(	(ov_info->y_base == 0)                || \
			(ov_info->h == 0)                     || \
			(ov_info->v == 0)                     || \
			(ov_info->stride == 0)                || \
			(ov_info->h > APR_MAX_HSIZE)          || \
			(ov_info->v > APR_MAX_VSIZE)          || \
			(ov_info->stride > 2048) )              
		{
			if(pCfg->cap.enb >= LX_APR_CAP_FMT) APR_PRINT("h_size[%d]/v_size[%d]/y_base[%d]\n", \
					ov_info->h, ov_info->v, ov_info->y_base);
			ov_info->src_valid = FALSE;
			break;
		}

		if(ROW2PHY(ov_info->y_base) > 0x10000000)
		{
			if(pCfg->cap.enb >= LX_APR_CAP_FMT) APR_PRINT("y_base  0x%08x\n", ROW2PHY(ov_info->y_base));
			ov_info->src_valid = FALSE;
			break;
		}

		if(ov_info->y_msb == 0x7)
		{
			if((gpMBase_venc_h14 == NULL) || (gpstVencMemConfig->uiInBufBase == 0))
			{
				ov_info->src_valid = FALSE;
			}
			ov_info->pFrameBase = (volatile UINT8*)((UINT32)gpMBase_venc_h14 + (ROW2PHY(ov_info->y_base) - (gpstVencMemConfig->uiInBufBase - 0x70000000)));
			if(sel_scaler_l || sel_scaler_s)
				ov_info->frame_base_phys[0] += 0x70000000;
			if(sel_scaler_r)
				ov_info->frame_base_phys[1] += 0x70000000;
		}
		else if(ov_info->y_msb == 0x9)
		{
			ov_info->pFrameBase = (volatile UINT8*)((UINT32)gpMBase_ssc_h14 + ROW2PHY(ov_info->y_base));
			if(sel_scaler_l || sel_scaler_s)
				ov_info->frame_base_phys[0] += 0x90000000;
			if(sel_scaler_r)
				ov_info->frame_base_phys[1] += 0x90000000;
		}
		else
		{
			if(pCfg->cap.enb >= LX_APR_CAP_FMT) APR_PRINT("%s : un-defined msb [ 0x%X000 0000 ]\n", \
					__F__, ov_info->y_msb & 0xF);
			if(sel_scaler_l || sel_scaler_s)
				ov_info->frame_base_phys[0] = 0;
			if(sel_scaler_r)
				ov_info->frame_base_phys[1] = 0;
			ov_info->src_valid = FALSE;
		}

		if(ov_info->src_valid)
		{
			if(pCfg->cap.enb == LX_APR_CAP_LOG) APR_PRINT("%s : ov src[%d] frame_base_phys[0/1] 0x%08X 0x%08X\n",\
					__F__, ov_src, ov_info->frame_base_phys[0],ov_info->frame_base_phys[1]);
		}
	} while(0);

	return ret;
}

int APR_H14_GetHistInfo(APR_HST_INFO_T *hist_info)
{
	int ret = RET_OK;
	PE_HST_HW_H14_INFO_T histo_info;

	do {
		switch(hist_info->cur_fmt)
		{
			case LX_APR_FMT_2D_2D :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_2D_2D;
				break;
			case LX_APR_FMT_3D_CB :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_3D_CB;
				break;
			case LX_APR_FMT_3D_CC :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_3D_CC;
				break;
			case LX_APR_FMT_3D_LL :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_3D_LL;
				break;
			case LX_APR_FMT_3D_SS :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_3D_SS;
				break;
			case LX_APR_FMT_3D_TB :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_3D_TB;
				break;
			case LX_APR_FMT_UNKNOWN :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_UNKNOWN;
				break;
			default :
				histo_info.det_3df = PE_HST_HW_H14_DET_3D_FMT_2D_2D;
				break;
		}
		ret = PE_HST_HW_H14_GetHistoInfo(&histo_info);
		if(ret != RET_OK) break;
		hist_info->src_type = histo_info.dis_inf.src_type;
		hist_info->out_type = histo_info.dis_inf.out_type;
		memcpy(hist_info->lrc.dif_max, histo_info.lrc_inf.dif_max, sizeof(UINT32)*4);
		memcpy(hist_info->lrc.pxl_cnt, histo_info.lrc_inf.pxl_cnt, sizeof(UINT32)*4);
		memcpy(hist_info->lrc.set_cnt, histo_info.lrc_inf.set_cnt, sizeof(UINT32)*4);

		if(lx_chip_rev() >= LX_CHIP_REV(H14,A0))
		{
			memcpy(hist_info->src.s_state, histo_info.src_inf.s_state, sizeof(UINT32)*2);
			memcpy(hist_info->src.dif_max, histo_info.src_inf.dif_cnt, sizeof(UINT32)*18);
			memcpy(hist_info->src.pxl_cnt, histo_info.src_inf.pxl_cnt, sizeof(UINT32)*18);
			memcpy(hist_info->src.set_cnt, histo_info.src_inf.set_cnt, sizeof(UINT32)*18);
		}
	} while(0);

	return ret;
}

int APR_H14_GetUSecTimerVal(UINT32 *tv)
{
	int ret = RET_OK;
	*tv = (UINT32)((0xFFFFFFFF - *gpTimerBase_h14)/198);
	return ret;
}

int APR_H14_SetFd3SubtConfig(APR_CFG_T *apr_cfg, int alt_seg)
{
	int ret = RET_OK;

	do {
		if ( lx_chip_rev() >= LX_CHIP_REV(H14,A0) ) {
			APR_FD3_H14_RdFL(subtitle_threshold);
			APR_FD3_H14_Wr01(subtitle_threshold, subtitle_alt_seg, alt_seg);
			APR_FD3_H14_WrFL(subtitle_threshold);
		}		
	} while(0);

	return ret;
}

int APR_H14_SetFd3Config(APR_CFG_T *apr_cfg)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = apr_cfg;
	UINT32 v_sampling = 0;
	UINT32 vcnt_ratio = 0;

	do {
		if ( lx_chip_rev() >= LX_CHIP_REV(H14,A0) ) {
			if(pCfg->in_win_y < 720)
			{
				v_sampling = 0;
				vcnt_ratio = 1;

			}
			else if((pCfg->in_win_y >= 720) && (pCfg->in_win_y < 1080))
			{
				v_sampling = (pCfg->fd3.scan==0) ? 1 : 0;
				vcnt_ratio = 2;
			}
			else
			{
				v_sampling = (pCfg->fd3.scan==0) ? 1 : 0;
				vcnt_ratio = 1;
			}

			if((pCfg->fd3.color == 0/*420:color fmt*/) && (pCfg->fd3.scan == 0/*progressive*/))
			{
				v_sampling = 0;
			}

			APR_FD3_H14_RdFL(fd3_mode);
			APR_FD3_H14_RdFL(segmentation_window);
			APR_FD3_H14_RdFL(projection_window);
			APR_FD3_H14_RdFL(projection_parameter_1);
			APR_FD3_H14_Wr01(fd3_mode, fd3_color_format,  pCfg->fd3.color);
			APR_FD3_H14_Wr01(fd3_mode, interlaced,        pCfg->fd3.scan);// 0:progressive, 1:interlaced
			APR_FD3_H14_Wr01(fd3_mode, vertical_sampling, v_sampling); // 0:all line, 1:every two line
			APR_FD3_H14_Wr01(segmentation_window, seg_wnd_loffset, pCfg->fd3.win.seg_l);
			APR_FD3_H14_Wr01(segmentation_window, seg_wnd_roffset, pCfg->fd3.win.seg_r);
			APR_FD3_H14_Wr01(segmentation_window, seg_wnd_toffset, pCfg->fd3.win.seg_t);
			APR_FD3_H14_Wr01(segmentation_window, seg_wnd_boffset, pCfg->fd3.win.seg_b);
			APR_FD3_H14_Wr01(projection_window, proj_wnd_lroffset, pCfg->fd3.win.prj_l);
			APR_FD3_H14_Wr01(projection_window, proj_wnd_toffset, pCfg->fd3.win.prj_t);
			APR_FD3_H14_Wr01(projection_window, proj_wnd_boffset, pCfg->fd3.win.prj_b);
			APR_FD3_H14_Wr01(projection_parameter_1, vcnt_ratio,   vcnt_ratio);
			APR_FD3_H14_WrFL(fd3_mode);
			APR_FD3_H14_WrFL(segmentation_window);
			APR_FD3_H14_WrFL(projection_window);
			APR_FD3_H14_WrFL(projection_parameter_1);

			APR_FD3_H14_RdFL(subtitle_threshold);
			APR_FD3_H14_Wr01(subtitle_threshold, subtitle_th,0xD1/*192*/);
			APR_FD3_H14_WrFL(subtitle_threshold);

			APR_FD3_H14_RdFL(segmentation_threshold_0);
			APR_FD3_H14_RdFL(segmentation_threshold_1);
			APR_FD3_H14_RdFL(segmentation_threshold_2);
			APR_FD3_H14_RdFL(segmentation_threshold_3);
			APR_FD3_H14_Wr01(segmentation_threshold_0, seg_red,         pCfg->fd3.seg_thr.seg_red);
			APR_FD3_H14_Wr01(segmentation_threshold_0, seg_re_ye,       pCfg->fd3.seg_thr.seg_re_ye);
			APR_FD3_H14_Wr01(segmentation_threshold_0, seg_yellow,      pCfg->fd3.seg_thr.seg_yellow);
			APR_FD3_H14_Wr01(segmentation_threshold_0, seg_ye_gr,       pCfg->fd3.seg_thr.seg_ye_gr);
			APR_FD3_H14_Wr01(segmentation_threshold_1, seg_green,       pCfg->fd3.seg_thr.seg_green);
			APR_FD3_H14_Wr01(segmentation_threshold_1, seg_gr_cy,       pCfg->fd3.seg_thr.seg_gr_cy);
			APR_FD3_H14_Wr01(segmentation_threshold_1, seg_cyan,        pCfg->fd3.seg_thr.seg_cyan);
			APR_FD3_H14_Wr01(segmentation_threshold_1, seg_cy_bl,       pCfg->fd3.seg_thr.seg_cy_bl);
			APR_FD3_H14_Wr01(segmentation_threshold_2, seg_blue,        pCfg->fd3.seg_thr.seg_blue);
			APR_FD3_H14_Wr01(segmentation_threshold_2, seg_bl_ma,       pCfg->fd3.seg_thr.seg_bl_ma);
			APR_FD3_H14_Wr01(segmentation_threshold_2, seg_magenta,     pCfg->fd3.seg_thr.seg_magenta);
			APR_FD3_H14_Wr01(segmentation_threshold_2, seg_ma_re,       pCfg->fd3.seg_thr.seg_ma_re);
			APR_FD3_H14_Wr01(segmentation_threshold_3, seg_th_sdelta,   pCfg->fd3.seg_thr.seg_value_black);
			APR_FD3_H14_Wr01(segmentation_threshold_3, seg_th_saturate, pCfg->fd3.seg_thr.seg_saturate);
			APR_FD3_H14_Wr01(segmentation_threshold_3, seg_th_sdelta,   pCfg->fd3.seg_thr.seg_sdelta);
			APR_FD3_H14_WrFL(segmentation_threshold_0);
			APR_FD3_H14_WrFL(segmentation_threshold_1);
			APR_FD3_H14_WrFL(segmentation_threshold_2);
			APR_FD3_H14_WrFL(segmentation_threshold_3);
		}		
	} while(0);

	return ret;
}

int APR_H14_CollectFd3Data(APR_DTA_T *apr_data)
{
	int ret = RET_OK;
	APR_DTA_T *pData  = apr_data;

	do {
		if ( lx_chip_rev() >= LX_CHIP_REV(H14,A0) ) {
			// scene
			APR_FD3_H14_RdFL(status_dominant_1);
			APR_FD3_H14_Rd01(status_dominant_1, scene_type, pData->fd3.scene_type);
			// bMask
			APR_FD3_H14_Rd01(status_dominant_1, dominant_hsv_idx0, pData->fd3.bMask);
			// correlation
			APR_FD3_H14_RdFL(corr_ss_blk_01);
			APR_FD3_H14_RdFL(corr_ss_blk_23);
			APR_FD3_H14_RdFL(corr_tb_blk_01);
			APR_FD3_H14_RdFL(corr_tb_blk_23);
			APR_FD3_H14_Rd01(corr_ss_blk_01, ss_corr0, pData->fd3.corr.ss[0]);
			APR_FD3_H14_Rd01(corr_ss_blk_01, ss_corr1, pData->fd3.corr.ss[1]);
			APR_FD3_H14_Rd01(corr_ss_blk_23, ss_corr2, pData->fd3.corr.ss[2]);
			APR_FD3_H14_Rd01(corr_ss_blk_23, ss_corr3, pData->fd3.corr.ss[3]);
			APR_FD3_H14_Rd01(corr_tb_blk_01, tb_corr0, pData->fd3.corr.tb[0]);
			APR_FD3_H14_Rd01(corr_tb_blk_01, tb_corr1, pData->fd3.corr.tb[1]);
			APR_FD3_H14_Rd01(corr_tb_blk_23, tb_corr2, pData->fd3.corr.tb[2]);
			APR_FD3_H14_Rd01(corr_tb_blk_23, tb_corr3, pData->fd3.corr.tb[3]);
			APR_FD3_H14_Rd01(corr_ss_blk_01, ss_wnd_info0, pData->fd3.corr.ss_wnd_info[0]);
			APR_FD3_H14_Rd01(corr_ss_blk_01, ss_wnd_info1, pData->fd3.corr.ss_wnd_info[1]);
			APR_FD3_H14_Rd01(corr_ss_blk_23, ss_wnd_info2, pData->fd3.corr.ss_wnd_info[2]);
			APR_FD3_H14_Rd01(corr_ss_blk_23, ss_wnd_info3, pData->fd3.corr.ss_wnd_info[3]);
			APR_FD3_H14_Rd01(corr_tb_blk_01, tb_wnd_info0, pData->fd3.corr.tb_wnd_info[0]);
			APR_FD3_H14_Rd01(corr_tb_blk_01, tb_wnd_info1, pData->fd3.corr.tb_wnd_info[1]);
			APR_FD3_H14_Rd01(corr_tb_blk_23, tb_wnd_info2, pData->fd3.corr.tb_wnd_info[2]);
			APR_FD3_H14_Rd01(corr_tb_blk_23, tb_wnd_info3, pData->fd3.corr.tb_wnd_info[3]);
			// mean & standard deviation
			APR_FD3_H14_RdFL(blk0_stat);
			APR_FD3_H14_RdFL(blk1_stat);
			APR_FD3_H14_RdFL(blk2_stat);
			APR_FD3_H14_RdFL(blk3_stat);
			APR_FD3_H14_RdFL(blk4_stat);
			APR_FD3_H14_RdFL(blk5_stat);
			APR_FD3_H14_RdFL(blk6_stat);
			APR_FD3_H14_RdFL(blk7_stat);
			APR_FD3_H14_Rd01(blk0_stat, mean, pData->fd3.blk_stat.mean[0]);
			APR_FD3_H14_Rd01(blk1_stat, mean, pData->fd3.blk_stat.mean[1]);
			APR_FD3_H14_Rd01(blk2_stat, mean, pData->fd3.blk_stat.mean[2]);
			APR_FD3_H14_Rd01(blk3_stat, mean, pData->fd3.blk_stat.mean[3]);
			APR_FD3_H14_Rd01(blk4_stat, mean, pData->fd3.blk_stat.mean[4]);
			APR_FD3_H14_Rd01(blk5_stat, mean, pData->fd3.blk_stat.mean[5]);
			APR_FD3_H14_Rd01(blk6_stat, mean, pData->fd3.blk_stat.mean[6]);
			APR_FD3_H14_Rd01(blk7_stat, mean, pData->fd3.blk_stat.mean[7]);
			APR_FD3_H14_Rd01(blk0_stat, standard_dev, pData->fd3.blk_stat.s_dev[0]);
			APR_FD3_H14_Rd01(blk1_stat, standard_dev, pData->fd3.blk_stat.s_dev[1]);
			APR_FD3_H14_Rd01(blk2_stat, standard_dev, pData->fd3.blk_stat.s_dev[2]);
			APR_FD3_H14_Rd01(blk3_stat, standard_dev, pData->fd3.blk_stat.s_dev[3]);
			APR_FD3_H14_Rd01(blk4_stat, standard_dev, pData->fd3.blk_stat.s_dev[4]);
			APR_FD3_H14_Rd01(blk5_stat, standard_dev, pData->fd3.blk_stat.s_dev[5]);
			APR_FD3_H14_Rd01(blk6_stat, standard_dev, pData->fd3.blk_stat.s_dev[6]);
			APR_FD3_H14_Rd01(blk7_stat, standard_dev, pData->fd3.blk_stat.s_dev[7]);

			APR_FD3_H14_RdFL(hsv_picture);
			APR_FD3_H14_Rd01(hsv_picture, pic_seg_hsv_max, pData->fd3.hsv_picture.pic_seg_hsv_max);
			APR_FD3_H14_Rd01(hsv_picture, pic_seg_hsv_count, pData->fd3.hsv_picture.pic_seg_hsv_count);

			APR_FD3_H14_RdFL(hsv_blk0);
			APR_FD3_H14_RdFL(hsv_blk1);
			APR_FD3_H14_RdFL(hsv_blk2);
			APR_FD3_H14_RdFL(hsv_blk3);
			APR_FD3_H14_Rd01(hsv_blk0, blk0_seg_hsv_count, pData->fd3.hsv_blk.seg_hsv_count[0]);
			APR_FD3_H14_Rd01(hsv_blk0, blk0_seg_hsv_max,   pData->fd3.hsv_blk.seg_hsv_max[0]);
			APR_FD3_H14_Rd01(hsv_blk1, blk1_seg_hsv_count, pData->fd3.hsv_blk.seg_hsv_count[1]);
			APR_FD3_H14_Rd01(hsv_blk1, blk1_seg_hsv_max,   pData->fd3.hsv_blk.seg_hsv_max[1]);
			APR_FD3_H14_Rd01(hsv_blk2, blk2_seg_hsv_count, pData->fd3.hsv_blk.seg_hsv_count[2]);
			APR_FD3_H14_Rd01(hsv_blk2, blk2_seg_hsv_max,   pData->fd3.hsv_blk.seg_hsv_max[2]);
			APR_FD3_H14_Rd01(hsv_blk3, blk3_seg_hsv_count, pData->fd3.hsv_blk.seg_hsv_count[3]);
			APR_FD3_H14_Rd01(hsv_blk3, blk3_seg_hsv_max,   pData->fd3.hsv_blk.seg_hsv_max[3]);

			APR_FD3_H14_RdFL(fd3_status_1);
			APR_FD3_H14_Rd01(fd3_status_1, init_count, pData->fd3.status.init_count);
			APR_FD3_H14_Rd01(fd3_status_1, suc_fail_history, pData->fd3.status.suc_fail_history);
		}
	} while(0);

	return ret;
}

int APR_H14_DetectBlackBar(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = apr_cfg;
	APR_DTA_T *pData = apr_data;

	int film_bar_th = ((pCfg->in_win_y/2)*3)>>4;
	int real_dom_seg = pData->fd3.bMask;
	int bbd_stat_x0, bbd_stat_y0;

#if 1
	do {
		if ( lx_chip_rev() >= LX_CHIP_REV(H14,A0) ) {
			// check black bar state
			PE_P0L_H14_RdFL(p0l_bbd_stat_00);
			PE_P0L_H14_Rd01(p0l_bbd_stat_00, x0, bbd_stat_x0);
			PE_P0L_H14_Rd01(p0l_bbd_stat_00, y0, bbd_stat_y0);
			
			if((bbd_stat_y0 > film_bar_th) && (pData->tpd < 200) )
			{
				// remove black color
				APR_FD3_H14_RdFL(force_mode);
				APR_FD3_H14_Wr01(force_mode, force_mode, 2);
//				APR_FD3_H14_Wr01(force_mode, force_scn_type, 0);
				APR_FD3_H14_Wr01(force_mode, force_hsv_idx0, real_dom_seg);
				APR_FD3_H14_Wr01(force_mode, force_hsv_idx1, 12);
				APR_FD3_H14_Wr01(force_mode, force_val_idx,  31);
				APR_FD3_H14_WrFL(force_mode);

				APR_FD3_H14_RdFL(dominant_threshold_0);
				APR_FD3_H14_Wr01(dominant_threshold_0, dom_pic_seg_th0, 0x30);
				APR_FD3_H14_Wr01(dominant_threshold_0, dom_pic_seg_th1, 0x86);
				APR_FD3_H14_WrFL(dominant_threshold_0);

				APR_FD3_H14_RdFL(dominant_threshold_1);
				APR_FD3_H14_Wr01(dominant_threshold_1, dom_blk_seg_hsv_th, 0x60);
				APR_FD3_H14_Wr01(dominant_threshold_1, dom_blk_seg_hsv_th_h, 0x90);
				APR_FD3_H14_WrFL(dominant_threshold_1);

				APR_FD3_H14_RdFL(projection_parameter_1);
				APR_FD3_H14_Wr01(projection_parameter_1, vdcnt_ratio, 0x3);
				APR_FD3_H14_WrFL(projection_parameter_1);
			}
			else // off force mode
			{
				APR_FD3_H14_RdFL(force_mode);
				APR_FD3_H14_Wr01(force_mode, force_mode, 0);
				APR_FD3_H14_WrFL(force_mode);

				APR_FD3_H14_RdFL(dominant_threshold_0);
				APR_FD3_H14_Wr01(dominant_threshold_0, dom_pic_seg_th0, 0x40);
				APR_FD3_H14_Wr01(dominant_threshold_0, dom_pic_seg_th1, 0xB3);
				APR_FD3_H14_WrFL(dominant_threshold_0);

				APR_FD3_H14_RdFL(dominant_threshold_1);
				APR_FD3_H14_Wr01(dominant_threshold_1, dom_blk_seg_hsv_th,   0x80);
				APR_FD3_H14_Wr01(dominant_threshold_1, dom_blk_seg_hsv_th_h, 0xC0);
				APR_FD3_H14_WrFL(dominant_threshold_1);

				APR_FD3_H14_RdFL(projection_parameter_1);
				APR_FD3_H14_Wr01(projection_parameter_1, vdcnt_ratio, 0x4);
				APR_FD3_H14_WrFL(projection_parameter_1);
			}
			
			if(pCfg->fd3.enb == LX_APR_FD3_BBD) APR_PRINT("y0 : %d,  th : %d\n",bbd_stat_y0,film_bar_th);
		}
	} while(0);
#endif

	return ret;
}

int APR_H14_SetCurrentInfo(APR_CFG_T *apr_cfg, APR_DTA_T *apr_data)
{
	int ret = RET_OK;
	APR_CFG_T *pCfg  = apr_cfg;
	APR_DTA_T *pData = apr_data;
	UINT32 reg = 0;

	do {
		if(!pCfg->fd3.enb) break;

		// [ 1: 0] detection info
		// [ 2   ] scene type
		// [30: 3] reserved
		// [31   ] 0:invalid, 1:valid
		switch(pCfg->apr_fmt)
		{
			case LX_APR_FMT_2D_2D :
				reg |= (0 << 0);
				break;
			case LX_APR_FMT_3D_SS :
				reg |= (1 << 0);
				break;
			case LX_APR_FMT_3D_TB :
				reg |= (2 << 0);
				break;
			default :
				reg |= (0 << 0);
				break;
		}
		reg |= (pData->fd3.scene_type << 2);
		reg |= (1 << 31);
	} while(0);
	
	if(gpH3DInfoBase_h14)
	{
		gpH3DInfoBase_h14[0] = reg;
		gpH3DInfoBase_h14[3] = pData->fd3.hsv_blk.seg_hsv_count[0] | (pData->fd3.hsv_blk.seg_hsv_max[0] << 24);
		gpH3DInfoBase_h14[4] = pData->fd3.hsv_blk.seg_hsv_count[1] | (pData->fd3.hsv_blk.seg_hsv_max[1] << 24);
		gpH3DInfoBase_h14[5] = pData->fd3.hsv_blk.seg_hsv_count[2] | (pData->fd3.hsv_blk.seg_hsv_max[2] << 24);
		gpH3DInfoBase_h14[6] = pData->fd3.hsv_blk.seg_hsv_count[3] | (pData->fd3.hsv_blk.seg_hsv_max[3] << 24);
	}

	return ret;
}

