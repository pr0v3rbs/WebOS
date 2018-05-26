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
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     juhee.park
 *  version    1.0
 *  date       2011.04.01
 *
 */

#ifndef _VPORT_I2C_L9A0_H_
#define _VPORT_I2C_L9A0_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	UINT32
	l9a_version:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_VERSION_0_T;

typedef struct
{
	UINT32
	reg_sel_afe3ch_ext_clock:2,		//1,0
	reg_sel_hostif_clock:1,		//2,2
	ro_hostif_is_162mhz:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HOSTIF_CONTROL_0_T;

typedef struct
{
	UINT32
	intr_hostif_is_162mhz:1,		//0,0
	intr_hostif_is_24mhz:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HOSTIF_INTR_0_T;

typedef struct
{
	UINT32
	enable_intr_hostif_is_162mhz:1,		//0,0
	enable_intr_hostif_is_24mhz:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HOSTIF_INTR_EN_0_T;

typedef struct
{
	UINT32
	ro_f324m_not_osc:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} TOP_CONTROL_0_T;

typedef struct
{
	UINT32
	ro_f324m_135_xtal_cnt:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} TOP_CONTROL_1_T;

typedef struct
{
	UINT32
	sro_force_sync:1,		//0,0
	sro_rd_mode:1,		//1,1
	srw_wr_force_sync:1,		//2,2
	srw_wr_sync_mode:1,		//3,3
	srw_wr_mode:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} TOP_CONTROL_2_T;

typedef struct
{
	UINT32
	swrst_pix:1,		//0,0
	swrst_f24m:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_0_T;

typedef struct
{
	UINT32
	swrst_link_prt1_tmds:1,		//0,0
	swrst_link_prt0_tmds:1,		//1,1
	swrst_hdcp1:1,		//2,2
	swrst_hdcp0:1,		//3,3
	swrst_f3m:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_1_T;

typedef struct
{
	UINT32
	swrst_link_pip0_pix:1,		//0,0
	swrst_aw1:1,		//1,1
	swrst_vw1:1,		//2,2
	swrst_link_pip1_tmds:1,		//3,3
	swrst_aw0:1,		//4,4
	swrst_vw0:1,		//5,5
	swrst_link_pip0_tmds:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_2_T;

typedef struct
{
	UINT32
	swrst_ar1:1,		//0,0
	swrst_link_pip1_aud:1,		//1,1
	swrst_ar0:1,		//2,2
	swrst_link_pip0_aud:1,		//3,3
	swrst_vr1:1,		//4,4
	swrst_link_pip1_pix:1,		//5,5
	swrst_vr0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_3_T;

typedef struct
{
	UINT32
	swrst_hp_drv_ctrl:1,		//0,0
	swrst_hp_drv:1,		//1,1
	swrst_mic:1,		//2,2
	swrst_auad:1,		//3,3
	swrst_mclk1:1,		//4,4
	swrst_mclk0:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_4_T;

typedef struct
{
	UINT32
	swrst_fs00:1,		//0,0
	swrst_afe3ch_cvbs:1,		//1,1
	swrst_cvbs:1,		//2,2
	swrst_f54m:1,		//3,3
	swrst_chb:1,		//4,4
	swrst_vdac:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_5_T;

typedef struct
{
	UINT32
	swrst_half_ultra:1,		//0,0
	swrst_ultra:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_6_T;

typedef struct
{
	UINT32
	swrst_hs1_lvds:1,		//0,0
	swrst_hs0_lvds:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_7_T;

typedef struct
{
	UINT32
	swrst_aclk_mux2:1,		//0,0
	swrst_hp_drv_32x:1,		//1,1
	swrst_hdmi_dto1:1,		//2,2
	swrst_hdmi_dto0:1,		//3,3
	swrst_adto:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_8_T;

typedef struct
{
	UINT32
	swrst_aud_fs26clk:1,		//0,0
	swrst_aud_fs25clk:1,		//1,1
	swrst_aud_fs24clk:1,		//2,2
	swrst_aud_fs23clk:1,		//3,3
	swrst_aud_fs21clk:1,		//4,4
	swrst_aud_fs20clk:1,		//5,5
	swrst_audclk_out:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_9_T;

typedef struct
{
	UINT32
	aclk_mux1_src:3,		//2,0
	swrst_f256fs_src:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_10_T;

typedef struct
{
	UINT32
	aclk_mux2_src:3,		//2,0
	aclk_mux1_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_11_T;

typedef struct
{
	UINT32
	aclk_mux3_src:3,		//2,0
	aclk_mux2_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_12_T;

typedef struct
{
	UINT32
	aclk_mux4_src:3,		//2,0
	aclk_mux3_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_13_T;

typedef struct
{
	UINT32
	aclk_mux5_src:3,		//2,0
	aclk_mux4_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_14_T;

typedef struct
{
	UINT32
	aclk_mux6_src:3,		//2,0
	aclk_mux5_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_15_T;

typedef struct
{
	UINT32
	aclk_mux7_src:3,		//2,0
	aclk_mux6_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_16_T;

typedef struct
{
	UINT32
	aclk_mux8_div:2,		//1,0
	aclk_mux8_src:3,		//4,2
	aclk_mux7_div:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_17_T;

typedef struct
{
	UINT32
	sel_inv_vdac_clock:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} CLOCK_CONTROL_0_T;

typedef struct
{
	UINT32
	sel_inv_chb_clock:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} CLOCK_CONTROL_1_T;

typedef struct
{
	UINT32
	sel_inv_f54m_clock:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} CLOCK_CONTROL_2_T;

typedef struct
{
	UINT32
	reg_adto_loop_value_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_0_T;

typedef struct
{
	UINT32
	reg_adto_loop_value_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_1_T;

typedef struct
{
	UINT32
	reg_adto_loop_value_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_2_T;

typedef struct
{
	UINT32
	reg_adto_add_value_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_3_T;

typedef struct
{
	UINT32
	reg_adto_add_value_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_4_T;

typedef struct
{
	UINT32
	reg_adto_add_value_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_5_T;

typedef struct
{
	UINT32
	reg_adto_add_value_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_6_T;

typedef struct
{
	UINT32
	reg_adto_err_value_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_7_T;

typedef struct
{
	UINT32
	reg_adto_err_value_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_8_T;

typedef struct
{
	UINT32
	reg_cvbs_clock_rate:1,		//0,0
	reg_cvbs_out_disable:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_0_T;

typedef struct
{
	UINT32
	reg_cvbs_c_delay:3,		//2,0
	reg_cvbs_y_delay:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_1_T;

typedef struct
{
	UINT32
	reg_ud_autoset:1,		//0,0
	reg_3dfr_autoset:1,		//1,1
	reg_link_ultra_channel_sel:1,		//2,2
	reg_3dfr_bridge_mode:1,		//3,3
	reg_half_bridge_mode_en:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_2_T;

typedef struct
{
	UINT32
	reg_ultra_h_fp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_3_T;

typedef struct
{
	UINT32
	reg_ultra_h_fp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_4_T;

typedef struct
{
	UINT32
	reg_ultra_h_sync_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_5_T;

typedef struct
{
	UINT32
	reg_ultra_h_sync_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_6_T;

typedef struct
{
	UINT32
	reg_ultra_h_bp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_7_T;

typedef struct
{
	UINT32
	reg_ultra_h_bp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_8_T;

typedef struct
{
	UINT32
	reg_ultra_h_active_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_9_T;

typedef struct
{
	UINT32
	reg_ultra_h_active_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_10_T;

typedef struct
{
	UINT32
	reg_half_v_fp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_11_T;

typedef struct
{
	UINT32
	reg_half_v_fp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_12_T;

typedef struct
{
	UINT32
	reg_half_v_sync_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_13_T;

typedef struct
{
	UINT32
	reg_half_v_sync_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_14_T;

typedef struct
{
	UINT32
	reg_half_v_bp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_15_T;

typedef struct
{
	UINT32
	reg_half_v_bp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_16_T;

typedef struct
{
	UINT32
	reg_ultra_v_active_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_17_T;

typedef struct
{
	UINT32
	reg_ultra_v_active_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_18_T;

typedef struct
{
	UINT32
	reg_ultra_3dfr_v_aspace_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_19_T;

typedef struct
{
	UINT32
	reg_ultra_3dfr_v_aspace_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_20_T;

typedef struct
{
	UINT32
	pdb1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_0_T;

typedef struct
{
	UINT32
	ch_en1:6,		//5,0
	flip_en1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_1_T;

typedef struct
{
	UINT32
	rs1:6,		//5,0
	rf1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_2_T;

typedef struct
{
	UINT32
	enpemp1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_3_T;

typedef struct
{
	UINT32
	swc_t1b1:3,		//2,0
	swc_t1a1:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_4_T;

typedef struct
{
	UINT32
	swc_t1clk1:3,		//2,0
	swc_t1c1:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_5_T;

typedef struct
{
	UINT32
	swc_t1e1:3,		//2,0
	swc_t1d1:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_6_T;

typedef struct
{
	UINT32
	pdb2:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_0_T;

typedef struct
{
	UINT32
	ch_en2:6,		//5,0
	flip_en2:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_1_T;

typedef struct
{
	UINT32
	rs2:6,		//5,0
	rf2:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_2_T;

typedef struct
{
	UINT32
	enpemp2:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_3_T;

typedef struct
{
	UINT32
	swc_t2b2:3,		//2,0
	swc_t2a2:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_4_T;

typedef struct
{
	UINT32
	swc_t2clk2:3,		//2,0
	swc_t2c2:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_5_T;

typedef struct
{
	UINT32
	swc_t2e2:3,		//2,0
	swc_t2d2:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_6_T;

typedef struct
{
	UINT32
	pdb3:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX2_0_T;

typedef struct
{
	UINT32
	ch_en3:6,		//5,0
	flip_en3:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX2_1_T;

typedef struct
{
	UINT32
	rs3:6,		//5,0
	rf3:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX2_2_T;

typedef struct
{
	UINT32
	enpemp3:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX2_3_T;

typedef struct
{
	UINT32
	swc_t3b3:3,		//2,0
	swc_t3a3:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX2_4_T;

typedef struct
{
	UINT32
	swc_t3clk3:3,		//2,0
	swc_t3c3:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX2_5_T;

typedef struct
{
	UINT32
	swc_t3e3:3,		//2,0
	swc_t3d3:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX2_6_T;

typedef struct
{
	UINT32
	inck_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} GBB_AFE_0_T;

typedef struct
{
	UINT32
	tun_i2c_sel:1,		//0,0
	bbmode:2,		//2,1
	i2csel:1,		//3,3
	reset:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} GBB_0_T;

typedef struct
{
	UINT32
	cvbs_vref:2,		//1,0
	cvbs_sdel:2,		//3,2
	cvbs_sck:1,		//4,4
	cvbs_endy:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_CVBS_0_T;

typedef struct
{
	UINT32
	cvbs_lpf:1,		//0,0
	cvbs_cns:1,		//1,1
	cvbs_cnf:1,		//2,2
	cvbs_pdbm:1,		//3,3
	cvbs_cnt:1,		//4,4
	cvbs_vdc:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_CVBS_1_T;

typedef struct
{
	UINT32
	cvbs_ibuf:3,		//2,0
	cvbs_bw:1,		//3,3
	cvbs_byp:1,		//4,4
	cvbs_icon:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_CVBS_2_T;

typedef struct
{
	UINT32
	cvbs_insel:3,		//2,0
	cvbs_cp:1,		//3,3
	cvbs_pdb:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_CVBS_3_T;

typedef struct
{
	UINT32
	buf_pdb2:1,		//0,0
	buf_ycm:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_VBUF_0_T;

typedef struct
{
	UINT32
	buf_pdb1:1,		//0,0
	buf_sel1:3,		//3,1
	buf_sel2:3,		//6,4
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_VBUF_1_T;

typedef struct
{
	UINT32
	vdac_pdb:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_VDAC_0_T;

typedef struct
{
	UINT32
	cvbsafe_selp:2,		//1,0
	cvbsafe_seldac:1,		//2,2
	vdicnt:4,		//6,3
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_VDAC_1_T;

typedef struct
{
	UINT32
	dr3p_nsc:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_0_T;

typedef struct
{
	UINT32
	dr3p_npc:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_1_T;

typedef struct
{
	UINT32
	dr3p_m:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_2_T;

typedef struct
{
	UINT32
	dr3p_od_cvdclk:3,		//2,0
	dr3p_od_recclk:4,		//6,3
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_3_T;

typedef struct
{
	UINT32
	dr3p_ps23c:1,		//0,0
	dr3p_lf:1,		//1,1
	dr3p_pdb:1,		//2,2
	dr3p_cih:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_4_T;

typedef struct
{
	UINT32
	cb_cp:1,		//0,0
	cb_gc:3,		//3,1
	cb_pdb:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_CB_0_T;

typedef struct
{
	UINT32
	cb_selref:2,		//1,0
	cb_dcsel:2,		//3,2
	cb_lpf:1,		//4,4
	cb_icon:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_CB_1_T;

typedef struct
{
	UINT32
	hpd1_oen:1,		//0,0
	hpd1_out:1,		//1,1
	hpd0_oen:1,		//2,2
	hpd0_out:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HPD_OUT_CONTROL_0_T;

typedef struct
{
	UINT32
	phy1_phy_pdb:1,		//0,0
	phy0_phy_arc_pdb:1,		//1,1
	phy0_phy_rstn:1,		//2,2
	phy0_phy_pdb:1,		//3,3
	phy_enable:1,		//4,4
	ddc_write_mode:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HPD_OUT_CONTROL_1_T;

typedef struct
{
	UINT32
	phy0_bert_run_ch2:1,		//0,0
	phy0_bert_run_ch1:1,		//1,1
	phy0_bert_run_ch0:1,		//2,2
	phy0_bert_ch_sel:2,		//4,3
	phy1_phy_arc_pdb:1,		//5,5
	phy1_phy_rstn:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HPD_OUT_CONTROL_2_T;

typedef struct
{
	UINT32
	phy0_reg_bert_num_err_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY0_0_T;

typedef struct
{
	UINT32
	phy0_reg_bert_num_err_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY0_1_T;

typedef struct
{
	UINT32
	phy0_reg_bert_test_res:2,		//1,0
	phy0_reg_bert_sync_done:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY0_2_T;

typedef struct
{
	UINT32
	phy1_bert_run_ch2:1,		//0,0
	phy1_bert_run_ch1:1,		//1,1
	phy1_bert_run_ch0:1,		//2,2
	phy1_bert_ch_sel:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY1_0_T;

typedef struct
{
	UINT32
	phy1_reg_bert_num_err_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY1_1_T;

typedef struct
{
	UINT32
	phy1_reg_bert_num_err_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY1_2_T;

typedef struct
{
	UINT32
	phy1_reg_bert_test_res:2,		//1,0
	phy1_reg_bert_sync_done:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY1_3_T;

typedef struct
{
	UINT32
	intr_clr_mute_pip0:1,		//0,0
	intr_set_mute_pip0:1,		//1,1
	intr_hdmi_mode_prt0:1,		//2,2
	intr_vs_det_prt0:1,		//3,3
	intr_scdt_prt0:1,		//4,4
	intr_plug_prt0:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_0_T;

typedef struct
{
	UINT32
	intr_asp_err_pip0:1,		//0,0
	intr_acr_err_pip0:1,		//1,1
	intr_terc4_err_prt0:1,		//2,2
	intr_ecc_err_pip0:1,		//3,3
	intr_hdcp_err_pip0:1,		//4,4
	intr_auth_done_prt0:1,		//5,5
	intr_auth_init_prt0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_1_T;

typedef struct
{
	UINT32
	intr_new_acr_pip0:1,		//0,0
	intr_new_asp_pip0:1,		//1,1
	intr_new_gcp_pip0:1,		//2,2
	intr_new_vsi_pip0:1,		//3,3
	intr_new_avi_pip0:1,		//4,4
	intr_no_gcp_pip0:1,		//5,5
	intr_no_avi_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_2_T;

typedef struct
{
	UINT32
	intr_new_spd_pip0:1,		//0,0
	intr_new_aud_pip0:1,		//1,1
	intr_new_mpg_pip0:1,		//2,2
	intr_new_isrc2_pip0:1,		//3,3
	intr_new_isrc1_pip0:1,		//4,4
	intr_new_acp_pip0:1,		//5,5
	intr_new_gbd_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_3_T;

typedef struct
{
	UINT32
	intr_burst_info_pip0:1,		//0,0
	intr_afifo_ovrr_pip0:1,		//1,1
	intr_afifo_undr_pip0:1,		//2,2
	intr_spdif_err_pip0:1,		//3,3
	intr_hwn_chg_pip0:1,		//4,4
	intr_hwcts_chg_pip0:1,		//5,5
	intr_new_unr_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_4_T;

typedef struct
{
	UINT32
	intr_vfifo_ovrr_pip0:1,		//0,0
	intr_vfifo_undr_pip0:1,		//1,1
	intr_il_chg_pip0:1,		//2,2
	intr_po_chg_pip0:1,		//3,3
	intr_hr_chg_pip0:1,		//4,4
	intr_vr_chg_pip0:1,		//5,5
	intr_vid_chg_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_5_T;

typedef struct
{
	UINT32
	enable_intr_clr_mute_pip0:1,		//0,0
	enable_intr_set_mute_pip0:1,		//1,1
	enable_intr_hdmi_mode_prt0:1,		//2,2
	enable_intr_vs_det_prt0:1,		//3,3
	enable_intr_scdt_prt0:1,		//4,4
	enable_intr_plug_prt0:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_0_T;

typedef struct
{
	UINT32
	enable_intr_asp_err_pip0:1,		//0,0
	enable_intr_acr_err_pip0:1,		//1,1
	enable_intr_terc4_err_prt0:1,		//2,2
	enable_intr_ecc_err_pip0:1,		//3,3
	enable_intr_hdcp_err_pip0:1,		//4,4
	enable_intr_auth_done_prt0:1,		//5,5
	enable_intr_auth_init_prt0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_1_T;

typedef struct
{
	UINT32
	enable_intr_new_acr_pip0:1,		//0,0
	enable_intr_new_asp_pip0:1,		//1,1
	enable_intr_new_gcp_pip0:1,		//2,2
	enable_intr_new_vsi_pip0:1,		//3,3
	enable_intr_new_avi_pip0:1,		//4,4
	enable_intr_no_gcp_pip0:1,		//5,5
	enable_intr_no_avi_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_2_T;

typedef struct
{
	UINT32
	enable_intr_new_spd_pip0:1,		//0,0
	enable_intr_new_aud_pip0:1,		//1,1
	enable_intr_new_mpg_pip0:1,		//2,2
	enable_intr_new_isrc2_pip0:1,		//3,3
	enable_intr_new_isrc1_pip0:1,		//4,4
	enable_intr_new_acp_pip0:1,		//5,5
	enable_intr_new_gbd_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_3_T;

typedef struct
{
	UINT32
	enable_intr_burst_info_pip0:1,		//0,0
	enable_intr_afifo_ovrr_pip0:1,		//1,1
	enable_intr_afifo_undr_pip0:1,		//2,2
	enable_intr_spdif_err_pip0:1,		//3,3
	enable_intr_hwn_chg_pip0:1,		//4,4
	enable_intr_hwcts_chg_pip0:1,		//5,5
	enable_intr_new_unr_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_4_T;

typedef struct
{
	UINT32
	enable_intr_vfifo_ovrr_pip0:1,		//0,0
	enable_intr_vfifo_undr_pip0:1,		//1,1
	enable_intr_il_chg_pip0:1,		//2,2
	enable_intr_po_chg_pip0:1,		//3,3
	enable_intr_hr_chg_pip0:1,		//4,4
	enable_intr_vr_chg_pip0:1,		//5,5
	enable_intr_vid_chg_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_5_T;

typedef struct
{
	UINT32
	intr_clr_mute_pip1:1,		//0,0
	intr_set_mute_pip1:1,		//1,1
	intr_hdmi_mode_prt1:1,		//2,2
	intr_vs_det_prt1:1,		//3,3
	intr_scdt_prt1:1,		//4,4
	intr_plug_prt1:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_0_T;

typedef struct
{
	UINT32
	intr_asp_err_pip1:1,		//0,0
	intr_acr_err_pip1:1,		//1,1
	intr_terc4_err_prt1:1,		//2,2
	intr_ecc_err_pip1:1,		//3,3
	intr_hdcp_err_pip1:1,		//4,4
	intr_auth_done_prt1:1,		//5,5
	intr_auth_init_prt1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_1_T;

typedef struct
{
	UINT32
	intr_new_acr_pip1:1,		//0,0
	intr_new_asp_pip1:1,		//1,1
	intr_new_gcp_pip1:1,		//2,2
	intr_new_vsi_pip1:1,		//3,3
	intr_new_avi_pip1:1,		//4,4
	intr_no_gcp_pip1:1,		//5,5
	intr_no_avi_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_2_T;

typedef struct
{
	UINT32
	intr_new_spd_pip1:1,		//0,0
	intr_new_aud_pip1:1,		//1,1
	intr_new_mpg_pip1:1,		//2,2
	intr_new_isrc2_pip1:1,		//3,3
	intr_new_isrc1_pip1:1,		//4,4
	intr_new_acp_pip1:1,		//5,5
	intr_new_gbd_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_3_T;

typedef struct
{
	UINT32
	intr_burst_info_pip1:1,		//0,0
	intr_afifo_ovrr_pip1:1,		//1,1
	intr_afifo_undr_pip1:1,		//2,2
	intr_spdif_err_pip1:1,		//3,3
	intr_hwn_chg_pip1:1,		//4,4
	intr_hwcts_chg_pip1:1,		//5,5
	intr_new_unr_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_4_T;

typedef struct
{
	UINT32
	intr_vfifo_ovrr_pip1:1,		//0,0
	intr_vfifo_undr_pip1:1,		//1,1
	intr_il_chg_pip1:1,		//2,2
	intr_po_chg_pip1:1,		//3,3
	intr_hr_chg_pip1:1,		//4,4
	intr_vr_chg_pip1:1,		//5,5
	intr_vid_chg_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_5_T;

typedef struct
{
	UINT32
	enable_intr_clr_mute_pip1:1,		//0,0
	enable_intr_set_mute_pip1:1,		//1,1
	enable_intr_hdmi_mode_prt1:1,		//2,2
	enable_intr_vs_det_prt1:1,		//3,3
	enable_intr_scdt_prt1:1,		//4,4
	enable_intr_plug_prt1:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_EN_0_T;

typedef struct
{
	UINT32
	enable_intr_asp_err_pip1:1,		//0,0
	enable_intr_acr_err_pip1:1,		//1,1
	enable_intr_terc4_err_prt1:1,		//2,2
	enable_intr_ecc_err_pip1:1,		//3,3
	enable_intr_hdcp_err_pip1:1,		//4,4
	enable_intr_auth_done_prt1:1,		//5,5
	enable_intr_auth_init_prt1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_EN_1_T;

typedef struct
{
	UINT32
	enable_intr_new_acr_pip1:1,		//0,0
	enable_intr_new_asp_pip1:1,		//1,1
	enable_intr_new_gcp_pip1:1,		//2,2
	enable_intr_new_vsi_pip1:1,		//3,3
	enable_intr_new_avi_pip1:1,		//4,4
	enable_intr_no_gcp_pip1:1,		//5,5
	enable_intr_no_avi_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_EN_2_T;

typedef struct
{
	UINT32
	enable_intr_new_spd_pip1:1,		//0,0
	enable_intr_new_aud_pip1:1,		//1,1
	enable_intr_new_mpg_pip1:1,		//2,2
	enable_intr_new_isrc2_pip1:1,		//3,3
	enable_intr_new_isrc1_pip1:1,		//4,4
	enable_intr_new_acp_pip1:1,		//5,5
	enable_intr_new_gbd_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_EN_3_T;

typedef struct
{
	UINT32
	enable_intr_burst_info_pip1:1,		//0,0
	enable_intr_afifo_ovrr_pip1:1,		//1,1
	enable_intr_afifo_undr_pip1:1,		//2,2
	enable_intr_spdif_err_pip1:1,		//3,3
	enable_intr_hwn_chg_pip1:1,		//4,4
	enable_intr_hwcts_chg_pip1:1,		//5,5
	enable_intr_new_unr_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_EN_4_T;

typedef struct
{
	UINT32
	enable_intr_vfifo_ovrr_pip1:1,		//0,0
	enable_intr_vfifo_undr_pip1:1,		//1,1
	enable_intr_il_chg_pip1:1,		//2,2
	enable_intr_po_chg_pip1:1,		//3,3
	enable_intr_hr_chg_pip1:1,		//4,4
	enable_intr_vr_chg_pip1:1,		//5,5
	enable_intr_vid_chg_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI1_INTR_EN_5_T;

typedef struct
{
	UINT32
	reg_arc_src_prt1:2,		//1,0
	reg_arc_src_prt0:2,		//3,2
	reg_prt_sel_pip0:1,		//4,4
	reg_conn_prt0:1,		//5,5
	reg_scdt_prt0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_0_T;

typedef struct
{
	UINT32
	reg_aac_en_pip0:1,		//0,0
	reg_mute_stat_pip0:2,		//2,1
	reg_mute_vid_pip0:1,		//3,3
	reg_mute_aud_pip0:1,		//4,4
	reg_mute_pol_pip0:1,		//5,5
	reg_hdmi_mode_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1_T;

typedef struct
{
	UINT32
	reg_vfifo_pdiff_pip0:6,		//5,0
	reg_avc_en_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_2_T;

typedef struct
{
	UINT32
	reg_afifo_pdiff_pip0:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_3_T;

typedef struct
{
	UINT32
	reg_vfifo_ctrl_thr_pip0:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_4_T;

typedef struct
{
	UINT32
	reg_afifo_ctrl_thr_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_5_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_6_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_7_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_8_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_9_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_10_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_11_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_12_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_13_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_14_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_15_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_16_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_17_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_18_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_19_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_20_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_21_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_22_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_23_T;

typedef struct
{
	UINT32
	reg_hdcp_ri_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_24_T;

typedef struct
{
	UINT32
	reg_hdcp_ri_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_25_T;

typedef struct
{
	UINT32
	reg_hdcp_enc_en_prt0:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_26_T;

typedef struct
{
	UINT32
	reg_hdcp_bcaps_prt0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_27_T;

typedef struct
{
	UINT32
	reg_hdcp_authed_prt0:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_28_T;

typedef struct
{
	UINT32
	reg_pkt_acp_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_29_T;

typedef struct
{
	UINT32
	reg_pkt_acp_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_30_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_31_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_32_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_33_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_34_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_35_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_36_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_37_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_38_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_39_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_40_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_41_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_42_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_43_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_44_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_45_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_46_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_47_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_48_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_49_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_50_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_51_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_52_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_53_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_54_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_55_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_56_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_57_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_58_T;

typedef struct
{
	UINT32
	reg_pkt_aif_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_59_T;

typedef struct
{
	UINT32
	reg_pkt_aif_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_60_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_61_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_62_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_63_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_64_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_65_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_66_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_67_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_68_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_69_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_70_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_71_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_72_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_73_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_74_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_75_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_76_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_77_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_78_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_79_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_80_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_81_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_82_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_83_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_84_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_85_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_86_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_87_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_88_T;

typedef struct
{
	UINT32
	reg_pkt_asp_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_89_T;

typedef struct
{
	UINT32
	reg_pkt_asp_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_90_T;

typedef struct
{
	UINT32
	reg_pkt_avi_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_91_T;

typedef struct
{
	UINT32
	reg_pkt_avi_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_92_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_93_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_94_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_95_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_96_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_97_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_98_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_99_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_100_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_101_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_102_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_103_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_104_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_105_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_106_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_107_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_108_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_109_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_110_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_111_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_112_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_113_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_114_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_115_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_116_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_117_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_118_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_119_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_120_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_121_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_122_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_123_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_124_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_125_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_126_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_127_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_128_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_129_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_130_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_131_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_132_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_133_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_134_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_135_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_136_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_137_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_138_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_139_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_140_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_141_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_142_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_143_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_144_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_145_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_146_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_147_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_148_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_149_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_150_T;

typedef struct
{
	UINT32
	reg_pkt_gcp_cmute_pip0:1,		//0,0
	reg_pkt_gcp_smute_pip0:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_151_T;

typedef struct
{
	UINT32
	reg_pkt_hbr_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_152_T;

typedef struct
{
	UINT32
	reg_pkt_hbr_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_153_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_154_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_155_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_156_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_157_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_158_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_159_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_160_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_161_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_162_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_163_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_164_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_165_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_166_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_167_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_168_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_169_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_170_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_171_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_172_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_173_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_174_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_175_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_176_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_177_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_178_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_179_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_180_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_181_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_182_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_183_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_184_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_185_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_186_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_187_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_188_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_189_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_190_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_191_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_192_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_193_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_194_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_195_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_196_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_197_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_198_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_199_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_200_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_201_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_202_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_203_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_204_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_205_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_206_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_207_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_208_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_209_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_210_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_211_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_212_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_213_T;

typedef struct
{
	UINT32
	reg_pkt_mif_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_214_T;

typedef struct
{
	UINT32
	reg_pkt_mif_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_215_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_216_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_217_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_218_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_219_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_220_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_221_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_222_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_223_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_224_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_225_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_226_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_227_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_228_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_229_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_230_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_231_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_232_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_233_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_234_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_235_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_236_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_237_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_238_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_239_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_240_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_241_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_242_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_243_T;

typedef struct
{
	UINT32
	reg_pkt_spd_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_244_T;

typedef struct
{
	UINT32
	reg_pkt_spd_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_245_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_246_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_247_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_248_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_249_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_250_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_251_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_252_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_253_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_254_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_255_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_256_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_257_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_258_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_259_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_260_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_261_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_262_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_263_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_264_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_265_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_266_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_267_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_268_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_269_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_270_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_271_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_272_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_273_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_30:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_274_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_29:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_275_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_28:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_276_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_277_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_278_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_279_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_280_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_281_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_282_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_283_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_284_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_285_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_286_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_287_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_288_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_289_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_290_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_291_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_292_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_293_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_294_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_295_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_296_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_297_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_298_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_299_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_300_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_301_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_302_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_303_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_304_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_305_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_306_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_307_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_308_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_309_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_310_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_311_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_312_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_313_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_314_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_315_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_316_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_317_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_318_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_319_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_320_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_321_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_322_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_323_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_324_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_325_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_326_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_327_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_328_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_329_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_330_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_331_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_332_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_333_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_334_T;

typedef struct
{
	UINT32
	info_vsi_3d_pip0:1,		//0,0
	info_vsi_4k2k_pip0:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_335_T;

typedef struct
{
	UINT32
	info_vsi_hdmi_vic_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_336_T;

typedef struct
{
	UINT32
	info_vsi_3d_struct_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_337_T;

typedef struct
{
	UINT32
	reg_pr_rx_pip0:4,		//3,0
	info_vsi_3d_meta_en_pip0:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_338_T;

typedef struct
{
	UINT32
	reg_pr_tx_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_339_T;

typedef struct
{
	UINT32
	reg_pr_stat_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_340_T;

typedef struct
{
	UINT32
	reg_align422_swap_rb_pip0:1,		//0,0
	reg_align422_stat_pip0:1,		//1,1
	reg_align422_tx_pip0:1,		//2,2
	reg_align422_rx_pip0:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_341_T;

typedef struct
{
	UINT32
	reg_cmode_rx_pip0:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_342_T;

typedef struct
{
	UINT32
	reg_cmode_tx_pip0:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_343_T;

typedef struct
{
	UINT32
	reg_packing_phase_pip0:4,	//3,0
	reg_default_phase_pip0:1,	//4,4
	reg_cmode_stat_pip0:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_344_T;

typedef struct
{
	UINT32
	reg_lbp_f2_pip0:1,		//0,0
	reg_inv_vs_pip0:1,		//1,1
	reg_inv_hs_pip0:1,		//2,2
	reg_neg_pol_en_pip0:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_345_T;

typedef struct
{
	UINT32
	reg_h_tot_pip0_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_346_T;

typedef struct
{
	UINT32
	reg_h_tot_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_347_T;

typedef struct
{
	UINT32
	reg_v_tot_pip0_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_348_T;

typedef struct
{
	UINT32
	reg_v_tot_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_349_T;

typedef struct
{
	UINT32
	reg_h_av_pip0_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_350_T;

typedef struct
{
	UINT32
	reg_h_av_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_351_T;

typedef struct
{
	UINT32
	reg_v_av_pip0_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_352_T;

typedef struct
{
	UINT32
	reg_v_av_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_353_T;

typedef struct
{
	UINT32
	reg_h_fp_pip0_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_354_T;

typedef struct
{
	UINT32
	reg_h_fp_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_355_T;

typedef struct
{
	UINT32
	reg_v_fp_pip0:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_356_T;

typedef struct
{
	UINT32
	reg_h_bp_pip0_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_357_T;

typedef struct
{
	UINT32
	reg_h_bp_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_358_T;

typedef struct
{
	UINT32
	reg_v_bp_pip0:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_359_T;

typedef struct
{
	UINT32
	reg_h_sync_pip0_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_360_T;

typedef struct
{
	UINT32
	reg_h_sync_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_361_T;

typedef struct
{
	UINT32
	reg_v_sync_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_362_T;

typedef struct
{
	UINT32
	reg_intrl_pip0:1,		//0,0
	reg_v_pol_pip0:1,		//1,1
	reg_h_pol_pip0:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_363_T;

typedef struct
{
	UINT32
	reg_vid_blank_r_prt0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_364_T;

typedef struct
{
	UINT32
	reg_vid_blank_g_prt0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_365_T;

typedef struct
{
	UINT32
	reg_vid_blank_b_prt0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_366_T;

typedef struct
{
	UINT32
	reg_acr_clk_aud_div_pip0:2,		//1,0
	reg_acr_n_fs_pip0:2,		//3,2
	reg_acr_en_pip0:1,		//4,4
	reg_asp_layout_pip0:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_367_T;

typedef struct
{
	UINT32
	reg_acr_ncts_sel_pip0:1,		//0,0
	reg_acr_adj_thr_pip0:4,		//4,1
	reg_acr_adj_en_pip0:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_368_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_369_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_370_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_371_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_372_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_373_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_374_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_375_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_376_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_377_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_378_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_379_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_380_T;

typedef struct
{
	UINT32
	reg_i2s_rj_pip0:1,		//0,0
	reg_i2s_lsb_first_pip0:1,		//1,1
	reg_i2s_shift1_pip0:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_381_T;

typedef struct
{
	UINT32
	reg_i2s_vucp_pip0:1,		//0,0
	reg_i2s_pcm_only_pip0:1,		//1,1
	reg_i2s_pass_inval_pip0:1,		//2,2
	reg_i2s_sck_edge_pip0:1,		//3,3
	reg_i2s_16bit_pip0:1,		//4,4
	reg_i2s_sign_ext_pip0:1,		//5,5
	reg_i2s_ws_pol_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_382_T;

typedef struct
{
	UINT32
	reg_i2s_sd0_map_pip0:2,		//1,0
	reg_i2s_sd_en_pip0:4,		//5,2
	reg_i2s_mclk_en_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_383_T;

typedef struct
{
	UINT32
	reg_i2s_sd3_map_pip0:2,		//1,0
	reg_i2s_sd2_map_pip0:2,		//3,2
	reg_i2s_sd1_map_pip0:2,		//5,4
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_384_T;

typedef struct
{
	UINT32
	reg_achst_ow_bit2_pip0:1,		//0,0
	reg_achst_ow_en_pip0:1,		//1,1
	reg_sp_paerr_pip0:1,		//2,2
	reg_sp_pserr_pip0:1,		//3,3
	reg_sp_gnd_mode_pip0:1,		//4,4
	reg_sp_en_pip0:1,		//5,5
	reg_i2s_out_en_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_385_T;

typedef struct
{
	UINT32
	reg_achst_ow_byte1_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_386_T;

typedef struct
{
	UINT32
	reg_achst_byte0_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_387_T;

typedef struct
{
	UINT32
	reg_achst_byte1_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_388_T;

typedef struct
{
	UINT32
	reg_achst_byte2_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_389_T;

typedef struct
{
	UINT32
	reg_achst_byte3_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_390_T;

typedef struct
{
	UINT32
	reg_achst_byte4_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_391_T;

typedef struct
{
	UINT32
	reg_achst_fs_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_392_T;

typedef struct
{
	UINT32
	reg_burst_pc_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_393_T;

typedef struct
{
	UINT32
	reg_burst_pc_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_394_T;

typedef struct
{
	UINT32
	reg_burst_pd_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_395_T;

typedef struct
{
	UINT32
	reg_burst_pd_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_396_T;

typedef struct
{
	UINT32
	reg_i2s_len_ovrr_pip0:4,		//3,0
	reg_i2s_len_ovrr_en_pip0:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_397_T;

typedef struct
{
	UINT32
	int_scdt_prt0:1,		//0,0
	int_plug_prt0:1,		//1,1
	reg_i2s_ch_mute_pip0:4,		//5,2
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_398_T;

typedef struct
{
	UINT32
	int_hdcp_err_pip0:1,		//0,0
	int_auth_done_prt0:1,		//1,1
	int_auth_init_prt0:1,		//2,2
	int_clr_mute_pip0:1,		//3,3
	int_set_mute_pip0:1,		//4,4
	int_hdmi_mode_prt0:1,		//5,5
	int_vs_det_prt0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_399_T;

typedef struct
{
	UINT32
	int_new_avi_pip0:1,		//0,0
	int_no_gcp_pip0:1,		//1,1
	int_no_avi_pip0:1,		//2,2
	int_asp_err_pip0:1,		//3,3
	int_acr_err_pip0:1,		//4,4
	int_terc4_err_prt0:1,		//5,5
	int_ecc_err_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_400_T;

typedef struct
{
	UINT32
	int_new_isrc1_pip0:1,		//0,0
	int_new_acp_pip0:1,		//1,1
	int_new_gbd_pip0:1,		//2,2
	int_new_acr_pip0:1,		//3,3
	int_new_asp_pip0:1,		//4,4
	int_new_gcp_pip0:1,		//5,5
	int_new_vsi_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_401_T;

typedef struct
{
	UINT32
	int_hwn_chg_pip0:1,		//0,0
	int_hwcts_chg_pip0:1,		//1,1
	int_new_unr_pip0:1,		//2,2
	int_new_spd_pip0:1,		//3,3
	int_new_aud_pip0:1,		//4,4
	int_new_mpg_pip0:1,		//5,5
	int_new_isrc2_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_402_T;

typedef struct
{
	UINT32
	int_hr_chg_pip0:1,		//0,0
	int_vr_chg_pip0:1,		//1,1
	int_vid_chg_pip0:1,		//2,2
	int_burst_info_pip0:1,		//3,3
	int_afifo_ovrr_pip0:1,		//4,4
	int_afifo_undr_pip0:1,		//5,5
	int_spdif_err_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_403_T;

typedef struct
{
	UINT32
	reg_prt_sel_pip1:1,		//0,0
	reg_conn_prt1:1,		//1,1
	reg_scdt_prt1:1,		//2,2
	int_vfifo_ovrr_pip0:1,		//3,3
	int_vfifo_undr_pip0:1,		//4,4
	int_il_chg_pip0:1,		//5,5
	int_po_chg_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_404_T;

typedef struct
{
	UINT32
	reg_aac_en_pip1:1,		//0,0
	reg_mute_stat_pip1:2,		//2,1
	reg_mute_vid_pip1:1,		//3,3
	reg_mute_aud_pip1:1,		//4,4
	reg_mute_pol_pip1:1,		//5,5
	reg_hdmi_mode_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_405_T;

typedef struct
{
	UINT32
	reg_vfifo_pdiff_pip1:6,		//5,0
	reg_avc_en_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_406_T;

typedef struct
{
	UINT32
	reg_afifo_pdiff_pip1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_407_T;

typedef struct
{
	UINT32
	reg_vfifo_ctrl_thr_pip1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_408_T;

typedef struct
{
	UINT32
	reg_afifo_ctrl_thr_pip1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_409_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_410_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_411_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_412_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_413_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_414_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_415_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_416_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_417_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_418_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_419_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_420_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_421_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_422_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_423_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_424_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_425_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_426_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_427_T;

typedef struct
{
	UINT32
	reg_hdcp_ri_prt1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_428_T;

typedef struct
{
	UINT32
	reg_hdcp_ri_prt1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_429_T;

typedef struct
{
	UINT32
	reg_hdcp_enc_en_prt1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_430_T;

typedef struct
{
	UINT32
	reg_hdcp_bcaps_prt1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_431_T;

typedef struct
{
	UINT32
	reg_hdcp_authed_prt1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_432_T;

typedef struct
{
	UINT32
	reg_pkt_acp_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_433_T;

typedef struct
{
	UINT32
	reg_pkt_acp_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_434_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_435_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_436_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_437_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_438_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_439_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_440_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_441_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_442_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_443_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_444_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_445_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_446_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_447_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_448_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_449_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_450_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_451_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_452_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_453_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_454_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_455_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_456_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_457_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_458_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_459_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_460_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_461_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_462_T;

typedef struct
{
	UINT32
	reg_pkt_aif_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_463_T;

typedef struct
{
	UINT32
	reg_pkt_aif_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_464_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_465_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_466_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_467_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_468_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_469_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_470_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_471_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_472_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_473_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_474_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_475_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_476_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_477_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_478_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_479_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_480_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_481_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_482_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_483_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_484_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_485_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_486_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_487_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_488_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_489_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_490_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_491_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_492_T;

typedef struct
{
	UINT32
	reg_pkt_asp_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_493_T;

typedef struct
{
	UINT32
	reg_pkt_asp_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_494_T;

typedef struct
{
	UINT32
	reg_pkt_avi_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_495_T;

typedef struct
{
	UINT32
	reg_pkt_avi_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_496_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_497_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_498_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_499_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_500_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_501_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_502_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_503_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_504_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_505_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_506_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_507_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_508_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_509_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_510_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_511_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_512_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_513_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_514_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_515_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_516_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_517_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_518_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_519_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_520_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_521_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_522_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_523_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_524_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_525_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_526_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_527_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_528_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_529_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_530_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_531_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_532_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_533_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_534_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_535_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_536_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_537_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_538_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_539_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_540_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_541_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_542_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_543_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_544_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_545_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_546_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_547_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_548_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_549_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_550_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_551_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_552_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_553_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_554_T;

typedef struct
{
	UINT32
	reg_pkt_gcp_cmute_pip1:1,		//0,0
	reg_pkt_gcp_smute_pip1:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_555_T;

typedef struct
{
	UINT32
	reg_pkt_hbr_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_556_T;

typedef struct
{
	UINT32
	reg_pkt_hbr_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_557_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_558_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_559_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_560_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_561_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_562_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_563_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_564_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_565_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_566_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_567_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_568_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_569_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_570_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_571_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_572_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_573_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_574_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_575_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_576_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_577_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_578_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_579_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_580_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_581_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_582_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_583_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_584_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_585_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_586_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_587_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_588_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_589_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_590_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_591_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_592_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_593_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_594_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_595_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_596_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_597_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_598_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_599_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_600_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_601_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_602_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_603_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_604_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_605_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_606_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_607_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_608_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_609_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_610_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_611_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_612_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_613_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_614_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_615_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_616_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_617_T;

typedef struct
{
	UINT32
	reg_pkt_mif_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_618_T;

typedef struct
{
	UINT32
	reg_pkt_mif_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_619_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_620_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_621_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_622_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_623_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_624_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_625_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_626_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_627_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_628_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_629_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_630_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_631_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_632_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_633_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_634_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_635_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_636_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_637_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_638_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_639_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_640_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_641_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_642_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_643_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_644_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_645_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_646_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_647_T;

typedef struct
{
	UINT32
	reg_pkt_spd_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_648_T;

typedef struct
{
	UINT32
	reg_pkt_spd_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_649_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_650_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_651_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_652_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_653_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_654_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_655_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_656_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_657_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_658_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_659_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_660_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_661_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_662_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_663_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_664_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_665_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_666_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_667_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_668_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_669_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_670_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_671_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_672_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_673_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_674_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_675_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_676_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_677_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_30:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_678_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_29:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_679_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_28:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_680_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_681_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_682_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_683_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_684_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_685_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_686_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_687_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_688_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_689_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_690_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_691_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_692_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_693_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_694_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_695_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_696_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_697_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_698_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_699_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_700_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_701_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_702_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_703_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_704_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_705_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_706_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_707_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_708_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_hdr_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_709_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_hdr_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_710_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_711_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_712_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_713_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_714_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_715_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_716_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_717_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_718_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_719_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_720_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_721_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_722_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_723_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_724_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_725_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_726_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_727_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_728_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_729_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_730_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_731_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_732_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_733_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_734_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_735_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_736_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_737_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_738_T;

typedef struct
{
	UINT32
	info_vsi_3d_pip1:1,		//0,0
	info_vsi_4k2k_pip1:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_739_T;

typedef struct
{
	UINT32
	info_vsi_hdmi_vic_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_740_T;

typedef struct
{
	UINT32
	info_vsi_3d_struct_pip1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_741_T;

typedef struct
{
	UINT32
	reg_pr_rx_pip1:4,		//3,0
	info_vsi_3d_meta_en_pi1:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_742_T;

typedef struct
{
	UINT32
	reg_pr_tx_pip1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_743_T;

typedef struct
{
	UINT32
	reg_pr_stat_pip1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_744_T;

typedef struct
{
	UINT32
	reg_align422_swap_rb_pip1:1,		//0,0
	reg_align422_stat_pip1:1,		//1,1
	reg_align422_tx_pip1:1,		//2,2
	reg_align422_rx_pip1:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_745_T;

typedef struct
{
	UINT32
	reg_cmode_rx_pip1:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_746_T;

typedef struct
{
	UINT32
	reg_cmode_tx_pip1:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_747_T;

typedef struct
{
	UINT32
	reg_packing_phase_pip1:4,	//3,0
	reg_default_phase_pip1:1,	//4,4
	reg_cmode_stat_pip1:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_748_T;

typedef struct
{
	UINT32
	reg_lbp_f2_pip1:1,		//0,0
	reg_inv_vs_pip1:1,		//1,1
	reg_inv_hs_pip1:1,		//2,2
	reg_neg_pol_en_pip1:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_749_T;

typedef struct
{
	UINT32
	reg_h_tot_pip1_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_750_T;

typedef struct
{
	UINT32
	reg_h_tot_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_751_T;

typedef struct
{
	UINT32
	reg_v_tot_pip1_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_752_T;

typedef struct
{
	UINT32
	reg_v_tot_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_753_T;

typedef struct
{
	UINT32
	reg_h_av_pip1_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_754_T;

typedef struct
{
	UINT32
	reg_h_av_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_755_T;

typedef struct
{
	UINT32
	reg_v_av_pip1_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_756_T;

typedef struct
{
	UINT32
	reg_v_av_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_757_T;

typedef struct
{
	UINT32
	reg_h_fp_pip1_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_758_T;

typedef struct
{
	UINT32
	reg_h_fp_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_759_T;

typedef struct
{
	UINT32
	reg_v_fp_pip1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_760_T;

typedef struct
{
	UINT32
	reg_h_bp_pip1_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_761_T;

typedef struct
{
	UINT32
	reg_h_bp_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_762_T;

typedef struct
{
	UINT32
	reg_v_bp_pip1:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_763_T;

typedef struct
{
	UINT32
	reg_h_sync_pip1_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_764_T;

typedef struct
{
	UINT32
	reg_h_sync_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_765_T;

typedef struct
{
	UINT32
	reg_v_sync_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_766_T;

typedef struct
{
	UINT32
	reg_intrl_pip1:1,		//0,0
	reg_v_pol_pip1:1,		//1,1
	reg_h_pol_pip1:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_767_T;

typedef struct
{
	UINT32
	reg_vid_blank_r_prt1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_768_T;

typedef struct
{
	UINT32
	reg_vid_blank_g_prt1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_769_T;

typedef struct
{
	UINT32
	reg_vid_blank_b_prt1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_770_T;

typedef struct
{
	UINT32
	reg_acr_clk_aud_div_pip1:2,		//1,0
	reg_acr_n_fs_pip1:2,		//3,2
	reg_acr_en_pip1:1,		//4,4
	reg_asp_layout_pip1:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_771_T;

typedef struct
{
	UINT32
	reg_acr_ncts_sel_pip1:1,		//0,0
	reg_acr_adj_thr_pip1:4,		//4,1
	reg_acr_adj_en_pip1:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_772_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip1_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_773_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_774_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_775_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip1_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_776_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_777_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_778_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip1_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_779_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_780_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_781_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip1_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_782_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_783_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_784_T;

typedef struct
{
	UINT32
	reg_i2s_rj_pip1:1,		//0,0
	reg_i2s_lsb_first_pip1:1,		//1,1
	reg_i2s_shift1_pip1:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_785_T;

typedef struct
{
	UINT32
	reg_i2s_vucp_pip1:1,		//0,0
	reg_i2s_pcm_only_pip1:1,		//1,1
	reg_i2s_pass_inval_pip1:1,		//2,2
	reg_i2s_sck_edge_pip1:1,		//3,3
	reg_i2s_16bit_pip1:1,		//4,4
	reg_i2s_sign_ext_pip1:1,		//5,5
	reg_i2s_ws_pol_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_786_T;

typedef struct
{
	UINT32
	reg_i2s_sd0_map_pip1:2,		//1,0
	reg_i2s_sd_en_pip1:4,		//5,2
	reg_i2s_mclk_en_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_787_T;

typedef struct
{
	UINT32
	reg_i2s_sd3_map_pip1:2,		//1,0
	reg_i2s_sd2_map_pip1:2,		//3,2
	reg_i2s_sd1_map_pip1:2,		//5,4
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_788_T;

typedef struct
{
	UINT32
	reg_achst_ow_bit2_pip1:1,		//0,0
	reg_achst_ow_en_pip1:1,		//1,1
	reg_sp_paerr_pip1:1,		//2,2
	reg_sp_pserr_pip1:1,		//3,3
	reg_sp_gnd_mode_pip1:1,		//4,4
	reg_sp_en_pip1:1,		//5,5
	reg_i2s_out_en_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_789_T;

typedef struct
{
	UINT32
	reg_achst_ow_byte1_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_790_T;

typedef struct
{
	UINT32
	reg_achst_byte0_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_791_T;

typedef struct
{
	UINT32
	reg_achst_byte1_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_792_T;

typedef struct
{
	UINT32
	reg_achst_byte2_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_793_T;

typedef struct
{
	UINT32
	reg_achst_byte3_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_794_T;

typedef struct
{
	UINT32
	reg_achst_byte4_pip1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_795_T;

typedef struct
{
	UINT32
	reg_achst_fs_pip1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_796_T;

typedef struct
{
	UINT32
	reg_burst_pc_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_797_T;

typedef struct
{
	UINT32
	reg_burst_pc_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_798_T;

typedef struct
{
	UINT32
	reg_burst_pd_pip1_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_799_T;

typedef struct
{
	UINT32
	reg_burst_pd_pip1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_800_T;

typedef struct
{
	UINT32
	reg_i2s_len_ovrr_pip1:4,		//3,0
	reg_i2s_len_ovrr_en_pip1:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_801_T;

typedef struct
{
	UINT32
	int_vs_det_prt1:1,		//0,0
	int_scdt_prt1:1,		//1,1
	int_plug_prt1:1,		//2,2
	reg_i2s_ch_mute_pip1:4,		//6,3
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_802_T;

typedef struct
{
	UINT32
	int_ecc_err_pip1:1,		//0,0
	int_hdcp_err_pip1:1,		//1,1
	int_auth_done_prt1:1,		//2,2
	int_auth_init_prt1:1,		//3,3
	int_clr_mute_pip1:1,		//4,4
	int_set_mute_pip1:1,		//5,5
	int_hdmi_mode_prt1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_803_T;

typedef struct
{
	UINT32
	int_new_vsi_pip1:1,		//0,0
	int_new_avi_pip1:1,		//1,1
	int_no_gcp_pip1:1,		//2,2
	int_no_avi_pip1:1,		//3,3
	int_asp_err_pip1:1,		//4,4
	int_acr_err_pip1:1,		//5,5
	int_terc4_err_prt1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_804_T;

typedef struct
{
	UINT32
	int_new_isrc2_pip1:1,		//0,0
	int_new_isrc1_pip1:1,		//1,1
	int_new_acp_pip1:1,		//2,2
	int_new_gbd_pip1:1,		//3,3
	int_new_acr_pip1:1,		//4,4
	int_new_asp_pip1:1,		//5,5
	int_new_gcp_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_805_T;

typedef struct
{
	UINT32
	int_spdif_err_pip1:1,		//0,0
	int_hwn_chg_pip1:1,		//1,1
	int_hwcts_chg_pip1:1,		//2,2
	int_new_unr_pip1:1,		//3,3
	int_new_spd_pip1:1,		//4,4
	int_new_aud_pip1:1,		//5,5
	int_new_mpg_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_806_T;

typedef struct
{
	UINT32
	int_po_chg_pip1:1,		//0,0
	int_hr_chg_pip1:1,		//1,1
	int_vr_chg_pip1:1,		//2,2
	int_vid_chg_pip1:1,		//3,3
	int_burst_info_pip1:1,		//4,4
	int_afifo_ovrr_pip1:1,		//5,5
	int_afifo_undr_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_807_T;

typedef struct
{
	UINT32
	reg_pre_crit:4,		//3,0
	int_vfifo_ovrr_pip1:1,		//4,4
	int_vfifo_undr_pip1:1,		//5,5
	int_il_chg_pip1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_808_T;

typedef struct
{
	UINT32
	reg_pgmr_en:1,		//0,0
	reg_bist_err:1,		//1,1
	reg_bist_done:1,		//2,2
	reg_bist_init:1,		//3,3
	reg_mute_aud_on_nosync:1,		//4,4
	reg_mute_aud_on_dvi:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_809_T;

typedef struct
{
	UINT32
	reg_pgmr_addr_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_810_T;

typedef struct
{
	UINT32
	reg_pgmr_addr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_811_T;

typedef struct
{
	UINT32
	reg_pgmr_wdata:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_812_T;

typedef struct
{
	UINT32
	reg_pgmr_rdata:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_813_T;

typedef struct
{
	UINT32
	reg_pgmr_wr:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_814_T;

typedef struct
{
	UINT32
	reg_unauth_on_mode_chg:1,		//0,0
	reg_unauth_on_nosync:1,		//1,1
	reg_hdcp_bksv_ld:1,		//2,2
	reg_hdcp_bypass_xor:1,		//3,3
	reg_hdcp_disable:1,		//4,4
	reg_pgmr_done:1,		//5,5
	reg_pgmr_rd:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_815_T;

typedef struct
{
	UINT32
	reg_mif_dec:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_816_T;

typedef struct
{
	UINT32
	reg_spd_dec:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_817_T;

typedef struct
{
	UINT32
	reg_dbg_hdcp_key_bak:1,		//0,0
	reg_vf_clr_on_nosync:1,		//1,1
	reg_pkt_clr_on_nosync:1,		//2,2
	reg_pkt_clr_on_dvi:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_818_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_287:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_819_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_286:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_820_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_285:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_821_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_284:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_822_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_283:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_823_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_282:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_824_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_281:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_825_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_280:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_826_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_279:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_827_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_278:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_828_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_277:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_829_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_276:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_830_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_275:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_831_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_274:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_832_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_273:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_833_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_272:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_834_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_271:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_835_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_270:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_836_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_269:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_837_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_268:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_838_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_267:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_839_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_266:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_840_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_265:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_841_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_264:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_842_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_263:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_843_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_262:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_844_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_261:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_845_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_260:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_846_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_259:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_847_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_258:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_848_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_257:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_849_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_256:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_850_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_255:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_851_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_254:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_852_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_253:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_853_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_252:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_854_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_251:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_855_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_250:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_856_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_249:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_857_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_248:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_858_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_247:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_859_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_246:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_860_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_245:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_861_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_244:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_862_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_243:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_863_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_242:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_864_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_241:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_865_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_240:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_866_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_239:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_867_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_238:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_868_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_237:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_869_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_236:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_870_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_235:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_871_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_234:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_872_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_233:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_873_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_232:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_874_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_231:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_875_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_230:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_876_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_229:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_877_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_228:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_878_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_227:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_879_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_226:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_880_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_225:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_881_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_224:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_882_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_223:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_883_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_222:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_884_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_221:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_885_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_220:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_886_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_219:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_887_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_218:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_888_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_217:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_889_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_216:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_890_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_215:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_891_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_214:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_892_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_213:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_893_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_212:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_894_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_211:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_895_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_210:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_896_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_209:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_897_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_208:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_898_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_207:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_899_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_206:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_900_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_205:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_901_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_204:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_902_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_203:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_903_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_202:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_904_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_201:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_905_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_200:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_906_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_199:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_907_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_198:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_908_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_197:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_909_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_196:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_910_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_195:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_911_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_194:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_912_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_193:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_913_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_192:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_914_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_191:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_915_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_190:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_916_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_189:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_917_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_188:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_918_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_187:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_919_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_186:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_920_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_185:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_921_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_184:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_922_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_183:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_923_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_182:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_924_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_181:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_925_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_180:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_926_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_179:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_927_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_178:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_928_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_177:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_929_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_176:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_930_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_175:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_931_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_174:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_932_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_173:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_933_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_172:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_934_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_171:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_935_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_170:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_936_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_169:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_937_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_168:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_938_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_167:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_939_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_166:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_940_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_165:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_941_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_164:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_942_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_163:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_943_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_162:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_944_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_161:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_945_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_160:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_946_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_159:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_947_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_158:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_948_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_157:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_949_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_156:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_950_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_155:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_951_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_154:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_952_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_153:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_953_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_152:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_954_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_151:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_955_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_150:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_956_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_149:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_957_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_148:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_958_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_147:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_959_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_146:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_960_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_145:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_961_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_144:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_962_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_143:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_963_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_142:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_964_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_141:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_965_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_140:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_966_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_139:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_967_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_138:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_968_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_137:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_969_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_136:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_970_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_135:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_971_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_134:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_972_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_133:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_973_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_132:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_974_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_131:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_975_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_130:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_976_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_129:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_977_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_128:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_978_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_127:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_979_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_126:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_980_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_125:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_981_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_124:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_982_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_123:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_983_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_122:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_984_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_121:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_985_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_120:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_986_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_119:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_987_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_118:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_988_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_117:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_989_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_116:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_990_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_115:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_991_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_114:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_992_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_113:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_993_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_112:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_994_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_111:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_995_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_110:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_996_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_109:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_997_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_108:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_998_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_107:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_999_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_106:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1000_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_105:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1001_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_104:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1002_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_103:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1003_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_102:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1004_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_101:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1005_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_100:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1006_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_99:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1007_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_98:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1008_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_97:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1009_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_96:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1010_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_95:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1011_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_94:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1012_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_93:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1013_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_92:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1014_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_91:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1015_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_90:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1016_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_89:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1017_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_88:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1018_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_87:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1019_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_86:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1020_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_85:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1021_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_84:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1022_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_83:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1023_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_82:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1024_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_81:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1025_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_80:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1026_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_79:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1027_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_78:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1028_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_77:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1029_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_76:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1030_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_75:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1031_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_74:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1032_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_73:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1033_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_72:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1034_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_71:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1035_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_70:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1036_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_69:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1037_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_68:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1038_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_67:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1039_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_66:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1040_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_65:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1041_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_64:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1042_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_63:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1043_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_62:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1044_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_61:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1045_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_60:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1046_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_59:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1047_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_58:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1048_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_57:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1049_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_56:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1050_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_55:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1051_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_54:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1052_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_53:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1053_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_52:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1054_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_51:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1055_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_50:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1056_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_49:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1057_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_48:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1058_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_47:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1059_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_46:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1060_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_45:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1061_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_44:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1062_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_43:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1063_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_42:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1064_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_41:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1065_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_40:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1066_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_39:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1067_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_38:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1068_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_37:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1069_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_36:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1070_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_35:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1071_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_34:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1072_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_33:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1073_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_32:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1074_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_31:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1075_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_30:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1076_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_29:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1077_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_28:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1078_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1079_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1080_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1081_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1082_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1083_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1084_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1085_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1086_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1087_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1088_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1089_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1090_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1091_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1092_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1093_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1094_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1095_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1096_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1097_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1098_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1099_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1100_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1101_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1102_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1103_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1104_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1105_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1106_T;

typedef struct
{
	UINT32
	intr_adc3ch_det_hsync:1,		//0,0
	intr_adc3ch_resolution_change:1,		//1,1
	intr_adc3ch_in_signal:1,		//2,2
	intr_adc3ch_no_signal:1,		//3,3
	intr_adc3ch_dpms_statechange:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_0_T;

typedef struct
{
	UINT32
	intr_adc3ch_aogc_err:1,		//0,0
	intr_adc3ch_aogc_done:1,		//1,1
	intr_adc3ch_nonstablehs:1,		//2,2
	intr_adc3ch_nonstablevs:1,		//3,3
	intr_adc3ch_loss_vsync:1,		//4,4
	intr_adc3ch_det_vsync:1,		//5,5
	intr_adc3ch_loss_hsync:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_1_T;

typedef struct
{
	UINT32
	enable_intr_adc3ch_det_hsync:1,		//0,0
	enable_intr_adc3ch_resolution_change:1,		//1,1
	enable_intr_adc3ch_in_signal:1,		//2,2
	enable_intr_adc3ch_no_signal:1,		//3,3
	enable_intr_adc3ch_dpms_statechange:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_EN_0_T;

typedef struct
{
	UINT32
	enable_intr_adc3ch_aogc_err:1,		//0,0
	enable_intr_adc3ch_aogc_done:1,		//1,1
	enable_intr_adc3ch_nonstablehs:1,		//2,2
	enable_intr_adc3ch_nonstablevs:1,		//3,3
	enable_intr_adc3ch_loss_vsync:1,		//4,4
	enable_intr_adc3ch_det_vsync:1,		//5,5
	enable_intr_adc3ch_loss_hsync:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_EN_1_T;

typedef struct
{
	UINT32
	llpll_disable_fm:1,		//0,0
	llpll_ref_div_sel:1,		//1,1
	llpll_ref_input_sel:1,		//2,2
	llpll_pdb:1,		//3,3
	reset_n:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_0_T;

typedef struct
{
	UINT32
	llpll_div_max:5,		//4,0
	llpll_dco_test_mode:1,		//5,5
	llpll_disable_sfm:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_1_T;

typedef struct
{
	UINT32
	llpll_counter_max_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_2_T;

typedef struct
{
	UINT32
	llpll_counter_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_3_T;

typedef struct
{
	UINT32
	llpll_shift_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_4_T;

typedef struct
{
	UINT32
	llpll_shift_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_5_T;

typedef struct
{
	UINT32
	llpll_sf_mode_control:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_6_T;

typedef struct
{
	UINT32
	llpll_dco_max:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_7_T;

typedef struct
{
	UINT32
	llpll_dco_min:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_8_T;

typedef struct
{
	UINT32
	llpll_coarse_scale:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_9_T;

typedef struct
{
	UINT32
	llpll_g1_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_10_T;

typedef struct
{
	UINT32
	llpll_g2_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_11_T;

typedef struct
{
	UINT32
	llpll_g3_p_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_12_T;

typedef struct
{
	UINT32
	llpll_g3_n_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_13_T;

typedef struct
{
	UINT32
	llpll_g1_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_14_T;

typedef struct
{
	UINT32
	llpll_g2_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_15_T;

typedef struct
{
	UINT32
	llpll_g3_p_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_16_T;

typedef struct
{
	UINT32
	llpll_g3_n_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_17_T;

typedef struct
{
	UINT32
	llpll_os_sw1:1,		//0,0
	llpll_os_sw2:1,		//1,1
	llpll_os_sw3:1,		//2,2
	llpll_ref_vcon:1,		//3,3
	llpll_lpf_clk_sel:1,		//4,4
	llpll_lpf_clk_div:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_18_T;

typedef struct
{
	UINT32
	llpll_diff_monitor_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_19_T;

typedef struct
{
	UINT32
	llpll_diff_monitor_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_20_T;

typedef struct
{
	UINT32
	llpll_counter_monitor_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_21_T;

typedef struct
{
	UINT32
	llpll_counter_monitor_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_22_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_3:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_23_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_24_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_25_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_26_T;

typedef struct
{
	UINT32
	llpll_filter_status:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_27_T;

typedef struct
{
	UINT32
	afe3ch_selmux:2,		//1,0
	afe3ch_bpdb:1,		//2,2
	afe3ch_gpdb:1,		//3,3
	afe3ch_rpdb:1,		//4,4
	afe3ch_biaspdb:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_0_T;

typedef struct
{
	UINT32
	afe3ch_icon_adc:3,		//2,0
	afe3ch_ire_selrefck:1,		//3,3
	afe3ch_ire_sel:2,		//5,4
	adc_ire_test:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_1_T;

typedef struct
{
	UINT32
	afe3ch_decm:2,		//1,0
	afe3ch_ref_sel:2,		//3,2
	afe3ch_icon_buf:3,		//6,4
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_2_T;

typedef struct
{
	UINT32
	afe3ch_gvclpsel:1,		//0,0
	afe3ch_rvclpsel:1,		//1,1
	afe3ch_rcp_clp:1,		//2,2
	afe3ch_clpcur:2,		//4,3
	afe3ch_sel_ck:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_3_T;

typedef struct
{
	UINT32
	afe3ch_fblvl:2,		//1,0
	afe3ch_sid2lvl:2,		//3,2
	afe3ch_sid1lvl:2,		//5,4
	afe3ch_bvclpsel:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_4_T;

typedef struct
{
	UINT32
	afe3ch_soglvl:5,		//4,0
	afe3ch_sog_hys:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_5_T;

typedef struct
{
	UINT32
	afe3ch_sc1_sid1:1,		//0,0
	afe3ch_sc1_sid2:1,		//1,1
	afe3ch_sog_outmux:1,		//2,2
	afe3ch_sog_inbw:2,		//4,3
	afe3ch_sog_bw:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_6_T;

typedef struct
{
	UINT32
	afe3ch_ctl_add:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_7_T;

typedef struct
{
	UINT32
	r_pre_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_0_T;

typedef struct
{
	UINT32
	r_pre_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_1_T;

typedef struct
{
	UINT32
	g_pre_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_2_T;

typedef struct
{
	UINT32
	g_pre_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_3_T;

typedef struct
{
	UINT32
	b_pre_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_4_T;

typedef struct
{
	UINT32
	b_pre_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_5_T;

typedef struct
{
	UINT32
	r_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_6_T;

typedef struct
{
	UINT32
	r_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_7_T;

typedef struct
{
	UINT32
	g_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_8_T;

typedef struct
{
	UINT32
	g_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_9_T;

typedef struct
{
	UINT32
	b_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_10_T;

typedef struct
{
	UINT32
	b_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_11_T;

typedef struct
{
	UINT32
	common_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_12_T;

typedef struct
{
	UINT32
	common_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_13_T;

typedef struct
{
	UINT32
	gain_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_14_T;

typedef struct
{
	UINT32
	r_dig_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_15_T;

typedef struct
{
	UINT32
	r_dig_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_16_T;

typedef struct
{
	UINT32
	g_dig_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_17_T;

typedef struct
{
	UINT32
	g_dig_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_18_T;

typedef struct
{
	UINT32
	b_dig_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_19_T;

typedef struct
{
	UINT32
	b_dig_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_20_T;

typedef struct
{
	UINT32
	vspoluser:1,		//0,0
	vspolover:1,		//1,1
	hspoluser:1,		//2,2
	hspolover:1,		//3,3
	offset_mode:2,		//5,4
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_21_T;

typedef struct
{
	UINT32
	precoast:5,		//4,0
	compuser:1,		//5,5
	compover:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_22_T;

typedef struct
{
	UINT32
	coastpolover:1,		//0,0
	extcoastsel:1,		//1,1
	postcoast:5,		//6,2
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_23_T;

typedef struct
{
	UINT32
	hsref_thr:4,		//3,0
	vsdet_thr:2,		//5,4
	coastpoluser:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_24_T;

typedef struct
{
	UINT32
	cst_eq_thr:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_25_T;

typedef struct
{
	UINT32
	cst_diff_thr_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_26_T;

typedef struct
{
	UINT32
	cst_diff_thr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_27_T;

typedef struct
{
	UINT32
	ro_vsdet:1,		//0,0
	ro_hsdet:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_28_T;

typedef struct
{
	UINT32
	ro_equalpulse:1,		//0,0
	ro_interlaced:1,		//1,1
	ro_composite:1,		//2,2
	ro_coastpol:1,		//3,3
	ro_vspol:1,		//4,4
	ro_hspol:1,		//5,5
	ro_coastdet:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_29_T;

typedef struct
{
	UINT32
	ro_hsprdref_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_30_T;

typedef struct
{
	UINT32
	ro_hsprdref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_31_T;

typedef struct
{
	UINT32
	ro_hslowref_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_32_T;

typedef struct
{
	UINT32
	ro_hslowref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_33_T;

typedef struct
{
	UINT32
	ro_vsprdref_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_34_T;

typedef struct
{
	UINT32
	ro_vsprdref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_35_T;

typedef struct
{
	UINT32
	ro_vslowref_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_36_T;

typedef struct
{
	UINT32
	ro_vslowref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_37_T;

typedef struct
{
	UINT32
	adc_ro_rovf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_38_T;

typedef struct
{
	UINT32
	adc_ro_rovf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_39_T;

typedef struct
{
	UINT32
	adc_ro_rudf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_40_T;

typedef struct
{
	UINT32
	adc_ro_rudf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_41_T;

typedef struct
{
	UINT32
	adc_ro_govf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_42_T;

typedef struct
{
	UINT32
	adc_ro_govf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_43_T;

typedef struct
{
	UINT32
	adc_ro_gudf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_44_T;

typedef struct
{
	UINT32
	adc_ro_gudf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_45_T;

typedef struct
{
	UINT32
	adc_ro_bovf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_46_T;

typedef struct
{
	UINT32
	adc_ro_bovf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_47_T;

typedef struct
{
	UINT32
	adc_ro_budf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_48_T;

typedef struct
{
	UINT32
	adc_ro_budf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_49_T;

typedef struct
{
	UINT32
	clamp_base:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_50_T;

typedef struct
{
	UINT32
	clamp_place:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_51_T;

typedef struct
{
	UINT32
	hsdiff_thr_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_52_T;

typedef struct
{
	UINT32
	hsdiff_thr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_53_T;

typedef struct
{
	UINT32
	hsprdeq_thr:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_54_T;

typedef struct
{
	UINT32
	hsoutover:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_55_T;

typedef struct
{
	UINT32
	hsoutcnt_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_56_T;

typedef struct
{
	UINT32
	hsoutcnt_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_57_T;

typedef struct
{
	UINT32
	hsout_width_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_58_T;

typedef struct
{
	UINT32
	hsout_width_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_59_T;

typedef struct
{
	UINT32
	hsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_60_T;

typedef struct
{
	UINT32
	vsout_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_61_T;

typedef struct
{
	UINT32
	vsdiff_thr_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_62_T;

typedef struct
{
	UINT32
	vsdiff_thr_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_63_T;

typedef struct
{
	UINT32
	vsdiff_thr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_64_T;

typedef struct
{
	UINT32
	vsprdeq_thr:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_65_T;

typedef struct
{
	UINT32
	vsoutover:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_66_T;

typedef struct
{
	UINT32
	vsoutcnt_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_67_T;

typedef struct
{
	UINT32
	vsoutcnt_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_68_T;

typedef struct
{
	UINT32
	vsoutcnt_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_69_T;

typedef struct
{
	UINT32
	vsout_width_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_70_T;

typedef struct
{
	UINT32
	vsout_width_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_71_T;

typedef struct
{
	UINT32
	vsout_width_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_72_T;

typedef struct
{
	UINT32
	vsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_73_T;

typedef struct
{
	UINT32
	vs_auto_dly_enable:1,		//0,0
	fld_inv:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_74_T;

typedef struct
{
	UINT32
	ro_hsout_width_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_75_T;

typedef struct
{
	UINT32
	ro_hsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_76_T;

typedef struct
{
	UINT32
	ro_vsout_width_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_77_T;

typedef struct
{
	UINT32
	ro_vsout_width_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_78_T;

typedef struct
{
	UINT32
	ro_vsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_79_T;

typedef struct
{
	UINT32
	ro_nogen_clp:1,		//0,0
	ro_nogen_vs:1,		//1,1
	ro_nogen_hs:1,		//2,2
	ro_nonstablevs:1,		//3,3
	ro_nonstablehs:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_80_T;

typedef struct
{
	UINT32
	ro_hsoutprd_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_81_T;

typedef struct
{
	UINT32
	ro_hsoutprd_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_82_T;

typedef struct
{
	UINT32
	ro_vsoutprd_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_83_T;

typedef struct
{
	UINT32
	ro_vsoutprd_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_84_T;

typedef struct
{
	UINT32
	ro_vsoutprd_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_85_T;

typedef struct
{
	UINT32
	ro_linecnt_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_86_T;

typedef struct
{
	UINT32
	ro_linecnt_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_87_T;

typedef struct
{
	UINT32
	ctrl_sequence:1,		//0,0
	hsync_sel:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_88_T;

typedef struct
{
	UINT32
	aogc_src_sel:1,		//0,0
	aogc_enable_sel:2,		//2,1
	aogc_enable:1,		//3,3
	signedout:3,		//6,4
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_89_T;

typedef struct
{
	UINT32
	r_compare_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_90_T;

typedef struct
{
	UINT32
	r_compare_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_91_T;

typedef struct
{
	UINT32
	g_compare_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_92_T;

typedef struct
{
	UINT32
	g_compare_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_93_T;

typedef struct
{
	UINT32
	b_compare_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_94_T;

typedef struct
{
	UINT32
	b_compare_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_95_T;

typedef struct
{
	UINT32
	aogc_th_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_96_T;

typedef struct
{
	UINT32
	aogc_th_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_97_T;

typedef struct
{
	UINT32
	aogc_sample_accum:1,		//0,0
	aogc_sample_count:4,		//4,1
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_98_T;

typedef struct
{
	UINT32
	aogc_counter_max_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_99_T;

typedef struct
{
	UINT32
	aogc_counter_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_100_T;

typedef struct
{
	UINT32
	ro_aogc_state:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_101_T;

typedef struct
{
	UINT32
	ro_avg_r_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_102_T;

typedef struct
{
	UINT32
	ro_avg_r_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_103_T;

typedef struct
{
	UINT32
	ro_avg_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_104_T;

typedef struct
{
	UINT32
	ro_avg_g_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_105_T;

typedef struct
{
	UINT32
	ro_avg_g_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_106_T;

typedef struct
{
	UINT32
	ro_avg_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_107_T;

typedef struct
{
	UINT32
	ro_avg_b_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_108_T;

typedef struct
{
	UINT32
	ro_avg_b_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_109_T;

typedef struct
{
	UINT32
	ro_avg_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_110_T;

typedef struct
{
	UINT32
	intr_ctrl:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_111_T;

typedef struct
{
	UINT32
	size_detect_ctrl_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_112_T;

typedef struct
{
	UINT32
	size_detect_ctrl_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_113_T;

typedef struct
{
	UINT32
	sd_ext_vsize_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_114_T;

typedef struct
{
	UINT32
	sd_ext_vsize_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_115_T;

typedef struct
{
	UINT32
	sd_vd_vsize_2:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_116_T;

typedef struct
{
	UINT32
	sd_vd_vsize_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_117_T;

typedef struct
{
	UINT32
	sd_vd_vsize_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_118_T;

typedef struct
{
	UINT32
	sd_vd_hsize_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_119_T;

typedef struct
{
	UINT32
	sd_vd_hsize_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_120_T;

typedef struct
{
	UINT32
	ap_ctrl:2,		//1,0
	no_signal:1,		//2,2
	dpms_state:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_121_T;

typedef struct
{
	UINT32
	ap_xpos_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_122_T;

typedef struct
{
	UINT32
	ap_xpos_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_123_T;

typedef struct
{
	UINT32
	ap_xsize_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_124_T;

typedef struct
{
	UINT32
	ap_xsize_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_125_T;

typedef struct
{
	UINT32
	ap_ypos_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_126_T;

typedef struct
{
	UINT32
	ap_ypos_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_127_T;

typedef struct
{
	UINT32
	ap_ysize_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_128_T;

typedef struct
{
	UINT32
	ap_ysize_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_129_T;

typedef struct
{
	UINT32
	ap_xpos_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_130_T;

typedef struct
{
	UINT32
	ap_xpos_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_131_T;

typedef struct
{
	UINT32
	ap_xsize_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_132_T;

typedef struct
{
	UINT32
	ap_xsize_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_133_T;

typedef struct
{
	UINT32
	ap_ypos_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_134_T;

typedef struct
{
	UINT32
	ap_ypos_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_135_T;

typedef struct
{
	UINT32
	ap_ysize_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_136_T;

typedef struct
{
	UINT32
	ap_ysize_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_137_T;

typedef struct
{
	UINT32
	ap_xpos_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_138_T;

typedef struct
{
	UINT32
	ap_xpos_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_139_T;

typedef struct
{
	UINT32
	ap_xsize_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_140_T;

typedef struct
{
	UINT32
	ap_xsize_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_141_T;

typedef struct
{
	UINT32
	ap_ypos_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_142_T;

typedef struct
{
	UINT32
	ap_ypos_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_143_T;

typedef struct
{
	UINT32
	ap_ysize_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_144_T;

typedef struct
{
	UINT32
	ap_ysize_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_145_T;

typedef struct
{
	UINT32
	ap_out_b_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_146_T;

typedef struct
{
	UINT32
	ap_out_b_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_147_T;

typedef struct
{
	UINT32
	ap_out_b_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_148_T;

typedef struct
{
	UINT32
	ap_out_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_149_T;

typedef struct
{
	UINT32
	ap_out_g_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_150_T;

typedef struct
{
	UINT32
	ap_out_g_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_151_T;

typedef struct
{
	UINT32
	ap_out_g_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_152_T;

typedef struct
{
	UINT32
	ap_out_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_153_T;

typedef struct
{
	UINT32
	ap_out_r_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_154_T;

typedef struct
{
	UINT32
	ap_out_r_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_155_T;

typedef struct
{
	UINT32
	ap_out_r_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_156_T;

typedef struct
{
	UINT32
	ap_out_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_157_T;

typedef struct
{
	UINT32
	ap_sum_b_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_158_T;

typedef struct
{
	UINT32
	ap_sum_b_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_159_T;

typedef struct
{
	UINT32
	ap_sum_b_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_160_T;

typedef struct
{
	UINT32
	ap_sum_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_161_T;

typedef struct
{
	UINT32
	ap_sum_g_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_162_T;

typedef struct
{
	UINT32
	ap_sum_g_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_163_T;

typedef struct
{
	UINT32
	ap_sum_g_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_164_T;

typedef struct
{
	UINT32
	ap_sum_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_165_T;

typedef struct
{
	UINT32
	ap_sum_r_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_166_T;

typedef struct
{
	UINT32
	ap_sum_r_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_167_T;

typedef struct
{
	UINT32
	ap_sum_r_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_168_T;

typedef struct
{
	UINT32
	ap_sum_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_169_T;

typedef struct
{
	UINT32
	extau_ctrl_1:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_170_T;

typedef struct
{
	UINT32
	extau_ctrl_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_171_T;

typedef struct
{
	UINT32
	extau_level_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_172_T;

typedef struct
{
	UINT32
	extau_level_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_173_T;

typedef struct
{
	UINT32
	extau_up_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_174_T;

typedef struct
{
	UINT32
	extau_up_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_175_T;

typedef struct
{
	UINT32
	extau_down_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_176_T;

typedef struct
{
	UINT32
	extau_down_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_177_T;

typedef struct
{
	UINT32
	extau_left_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_178_T;

typedef struct
{
	UINT32
	extau_left_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_179_T;

typedef struct
{
	UINT32
	extau_right_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_180_T;

typedef struct
{
	UINT32
	extau_right_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_181_T;

typedef struct
{
	UINT32
	extau_up_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_182_T;

typedef struct
{
	UINT32
	extau_up_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_183_T;

typedef struct
{
	UINT32
	extau_down_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_184_T;

typedef struct
{
	UINT32
	extau_down_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_185_T;

typedef struct
{
	UINT32
	extau_left_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_186_T;

typedef struct
{
	UINT32
	extau_left_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_187_T;

typedef struct
{
	UINT32
	extau_right_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_188_T;

typedef struct
{
	UINT32
	extau_right_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_189_T;

typedef struct
{
	UINT32
	r_min_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_190_T;

typedef struct
{
	UINT32
	r_min_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_191_T;

typedef struct
{
	UINT32
	r_max_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_192_T;

typedef struct
{
	UINT32
	r_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_193_T;

typedef struct
{
	UINT32
	g_min_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_194_T;

typedef struct
{
	UINT32
	g_min_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_195_T;

typedef struct
{
	UINT32
	g_max_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_196_T;

typedef struct
{
	UINT32
	g_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_197_T;

typedef struct
{
	UINT32
	b_min_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_198_T;

typedef struct
{
	UINT32
	b_min_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_199_T;

typedef struct
{
	UINT32
	b_max_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_200_T;

typedef struct
{
	UINT32
	b_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_201_T;

typedef struct
{
	UINT32
	blank_sp_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_202_T;

typedef struct
{
	UINT32
	blank_sp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_203_T;

typedef struct
{
	UINT32
	r_blank_mode:2,		//1,0
	r_blank_num_clear:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_204_T;

typedef struct
{
	UINT32
	r_blank_high_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_205_T;

typedef struct
{
	UINT32
	r_blank_high_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_206_T;

typedef struct
{
	UINT32
	r_blank_low_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_207_T;

typedef struct
{
	UINT32
	r_blank_low_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_208_T;

typedef struct
{
	UINT32
	g_blank_mode:2,		//1,0
	g_blank_num_clear:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_209_T;

typedef struct
{
	UINT32
	g_blank_high_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_210_T;

typedef struct
{
	UINT32
	g_blank_high_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_211_T;

typedef struct
{
	UINT32
	g_blank_low_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_212_T;

typedef struct
{
	UINT32
	g_blank_low_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_213_T;

typedef struct
{
	UINT32
	b_blank_num_clear:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_214_T;

typedef struct
{
	UINT32
	b_blank_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_215_T;

typedef struct
{
	UINT32
	b_blank_high_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_216_T;

typedef struct
{
	UINT32
	b_blank_high_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_217_T;

typedef struct
{
	UINT32
	b_blank_low_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_218_T;

typedef struct
{
	UINT32
	b_blank_low_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_219_T;

typedef struct
{
	UINT32
	clamp_width:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_220_T;

typedef struct
{
	UINT32
	clamp_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_221_T;

typedef struct
{
	UINT32
	clamp_init:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_222_T;

typedef struct
{
	UINT32
	sub_sampling_ctrl:3,		//2,0
	sub_sampling_en:1,		//3,3
	clamp_vsync_only:1,		//4,4
	clamp_hsync_only:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_223_T;

typedef struct
{
	UINT32
	r_pre_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_224_T;

typedef struct
{
	UINT32
	r_pre_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_225_T;

typedef struct
{
	UINT32
	g_pre_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_226_T;

typedef struct
{
	UINT32
	g_pre_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_227_T;

typedef struct
{
	UINT32
	b_pre_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_228_T;

typedef struct
{
	UINT32
	b_pre_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_229_T;

typedef struct
{
	UINT32
	apll_mode:4,		//3,0
	apll_en_edge:1,		//4,4
	apll_mode_en:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_0_T;

typedef struct
{
	UINT32
	rsvd8:16,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_1_T;

typedef struct
{
	UINT32
	fs00clk_inv:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_2_T;

typedef struct
{
	UINT32
	rsvd8:16,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_3_T;

typedef struct
{
	UINT32
	b_rsvd1:1,			// 0,0
	aud_fs20clk_inv:1,		//1,1
	aud_fs21clk_inv:1,		//2,2
	aud_fs23clk_inv:1,		//3,3
	aud_fs24clk_inv:1,		//4,4
	aud_fs25clk_inv:1,		//5,5
	aud_fs26clk_inv:1,		//6,6
	audclk_out_inv:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_4_T;

typedef struct
{
	UINT32
	apll_sel:1,		//0,0
	hdmiphy0_sdaout_enable:1,		//1,1
	hdmiphy1_sdaout_enable:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_5_T;

typedef struct
{
	UINT32
	apll_cvs:2,		//1,0
	apll_cvl:2,		//3,2
	apll_ci:2,		//5,4
	apll_pdb:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_6_T;

typedef struct
{
	UINT32
	aad_dco_resetb:1,		//0,0
	aad_dco_rtest:1,		//1,1
	aad_adc_pdb:1,		//2,2
	aad_adc_selref:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_0_T;

typedef struct
{
	UINT32
	aad_dco_fcw_2:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_1_T;

typedef struct
{
	UINT32
	aad_dco_fcw_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_2_T;

typedef struct
{
	UINT32
	aad_dco_fcw_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_3_T;

typedef struct
{
	UINT32
	aad_spll_m:2,		//1,0
	aad_dco_lpfon:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_4_T;

typedef struct
{
	UINT32
	aad_spll_n:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_5_T;

typedef struct
{
	UINT32
	aad_spll_ci:2,		//1,0
	aad_spll_od:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_6_T;

typedef struct
{
	UINT32
	aad_spllclk_test:1,		//0,0
	aad_spll_pdb:1,		//1,1
	aad_spll_sel:1,		//2,2
	aad_spll_cvs:2,		//4,3
	aad_spll_cvl:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_7_T;

typedef struct
{
	UINT32
	aad_btscclk_sel:1,		//0,0
	aad_fs02clk_sel:2,		//2,1
	aad_fs01clk_sel:2,		//4,3
	aad_fs00clk_sel:1,		//5,5
	aad_adcclk_test:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_8_T;

typedef struct
{
	UINT32
	auad_ccon:3,		//2,0
	auda_pdb:2,		//4,3
	auda_pdb_dse:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_0_T;

typedef struct
{
	UINT32
	auad_ch_sel:3,		//2,0
	auad_gcon:4,		//6,3
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_1_T;

typedef struct
{
	UINT32
	aud_lindrv1_ch_sel:3,		//2,0
	aud_lindrv0_ch_sel:3,		//5,3
	auad_dc_sel:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_2_T;

typedef struct
{
	UINT32
	auda_bci:3,		//2,0
	auad_lin_drv_ch_muteb:1,		//3,3
	auad_pdb_ldrv_in:1,		//4,4
	auad_rec_ch_muteb:1,		//5,5
	auad_pdb_in:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_3_T;

typedef struct
{
	UINT32
	aumi_cnt:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_4_T;

typedef struct
{
	UINT32
	aumi_pop:1,		//0,0
	aumi_pdb_in:1,		//1,1
	aumi_cont:4,		//5,2
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_5_T;

typedef struct
{
	UINT32
	hp_ssp_cv:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_HP_DRV_0_T;

typedef struct
{
	UINT32
	hp_ssp_sr:4,		//3,0
	hp_ssp_cih:3,		//6,4
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_HP_DRV_1_T;

typedef struct
{
	UINT32
	hp_ssp_m:2,		//1,0
	hp_ssp_od:2,		//3,2
	hp_ssp_ms:3,		//6,4
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_HP_DRV_2_T;

typedef struct
{
	UINT32
	hp_ssp_n:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_HP_DRV_3_T;

typedef struct
{
	UINT32
	hp_ana_pdb:1,		//0,0
	hp_pll_pdb:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_HP_DRV_4_T;

typedef struct
{
	UINT32
	dem0:2,		//1,0
	mutec:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_0_T;

typedef struct
{
	UINT32
	test0:2,		//1,0
	dif1:2,		//3,2
	dif0:2,		//5,4
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_1_T;

typedef struct
{
	UINT32
	out_sel:2,		//1,0
	sgm_zero_sel:2,		//3,2
	test1:2,		//5,4
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_2_T;

typedef struct
{
	UINT32
	dac0_vol_con_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_3_T;

typedef struct
{
	UINT32
	dac0_vol_con_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_4_T;

typedef struct
{
	UINT32
	dac0_rate_cont_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_5_T;

typedef struct
{
	UINT32
	dac0_rate_cont_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_6_T;

typedef struct
{
	UINT32
	dac0_rate_cont_mute_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_7_T;

typedef struct
{
	UINT32
	dac0_rate_cont_mute_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_8_T;

typedef struct
{
	UINT32
	dac0_dc_setup_byps:1,		//0,0
	dac0_pop_dc_on_control:1,		//1,1
	dac0_mute_ena:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_9_T;

typedef struct
{
	UINT32
	dac0_pop_con_rate_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_10_T;

typedef struct
{
	UINT32
	dac0_pop_con_rate_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_11_T;

typedef struct
{
	UINT32
	dac1_vol_con_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_12_T;

typedef struct
{
	UINT32
	dac1_vol_con_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_13_T;

typedef struct
{
	UINT32
	dac1_rate_cont_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_14_T;

typedef struct
{
	UINT32
	dac1_rate_cont_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_15_T;

typedef struct
{
	UINT32
	dac1_rate_cont_mute_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_16_T;

typedef struct
{
	UINT32
	dac1_rate_cont_mute_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_17_T;

typedef struct
{
	UINT32
	dac1_dc_setup_byps:1,		//0,0
	dac1_pop_dc_on_control:1,		//1,1
	dac1_mute_ena:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_18_T;

typedef struct
{
	UINT32
	dac1_pop_con_rate_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_19_T;

typedef struct
{
	UINT32
	dac1_pop_con_rate_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_20_T;

typedef struct
{
	UINT32
	lrn_invert:1,		//0,0
	sclk_invert:1,		//1,1
	dif:2,		//3,2
	dac_lrn_invert:1,		//4,4
	dac_sclk_invert:1,		//5,5
	dac_pop_invert:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_21_T;

typedef struct
{
	UINT32
	hp_vol_con_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_22_T;

typedef struct
{
	UINT32
	hp_vol_con_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_23_T;

typedef struct
{
	UINT32
	hp_rate_cont_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_24_T;

typedef struct
{
	UINT32
	hp_rate_cont_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_25_T;

typedef struct
{
	UINT32
	hp_rate_cont_mute_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_26_T;

typedef struct
{
	UINT32
	hp_rate_cont_mute_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_27_T;

typedef struct
{
	UINT32
	hp_dc_setup_byps:1,		//0,0
	hp_pop_dc_on_control:1,		//1,1
	hp_mute_ena:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_28_T;

typedef struct
{
	UINT32
	hp_pop_con_rate_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_29_T;

typedef struct
{
	UINT32
	hp_pop_con_rate_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_30_T;

typedef struct
{
	UINT32
	pol_con:2,		//1,0
	cont:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_31_T;

typedef struct
{
	UINT32
	testinr:1,		//0,0
	testinl:1,		//1,1
	testr:1,		//2,2
	testl:1,		//3,3
	dig_sel:2,		//5,4
	twos_com:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_32_T;

typedef struct
{
	UINT32
	ladc_vol_con_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_33_T;

typedef struct
{
	UINT32
	ladc_vol_con_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_34_T;

typedef struct
{
	UINT32
	ladc_rate_cont_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_35_T;

typedef struct
{
	UINT32
	ladc_rate_cont_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_36_T;

typedef struct
{
	UINT32
	ladc_rate_cont_mute_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_37_T;

typedef struct
{
	UINT32
	ladc_rate_cont_mute_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_38_T;

typedef struct
{
	UINT32
	cont_mic:2,				//1,0
	ladc_mute_ena:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_39_T;

typedef struct
{
	UINT32
	testin_mic:1,		//0,0
	test_mic:1,		//1,1
	dig_sel_mic:2,		//3,2
	twos_com_mic:1,		//4,4
	pol_con_mic:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_40_T;

typedef struct
{
	UINT32
	vol_con_mic_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_41_T;

typedef struct
{
	UINT32
	vol_con_mic_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_42_T;

typedef struct
{
	UINT32
	rate_cont_mic_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_43_T;

typedef struct
{
	UINT32
	rate_cont_mic_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_44_T;

typedef struct
{
	UINT32
	rate_cont_mute_mic_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_45_T;

typedef struct
{
	UINT32
	rate_cont_mute_mic_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_46_T;

typedef struct
{
	UINT32
	force_mic:1,		//0,0
	mute_ena_mic:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_47_T;

typedef struct
{
	UINT32
	pdb_all:1,		//0,0
	b_rsvd1:3,
	resetb_all:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_01_T;

typedef struct
{
	UINT32
	pdb_sel:1,	//0,0
	b_rsvd1:3,
	reset_sel:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_02_T;

typedef struct
{
	UINT32
	cr_pll_pdb:1,	//0,0
	b_rsvd1:3,
	cr_pll_resetb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_03_T;

typedef struct
{
	UINT32
	cr_dig_ldo_pdb:1,	//0,0
	b_rsvd1:3,
	cr_ldo_pdb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_04_T;

typedef struct
{
	UINT32
	cr_mode_sel_resetb:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_05_T;

typedef struct
{
	UINT32
	eq_pdb:1,	//0,0
	b_rsvd1:3,
	eq_resetb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_06_T;

typedef struct
{
	UINT32
	dr_pdb:1,	//0,0
	b_rsvd1:3,
	dr_resetb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_07_T;

typedef struct
{
	UINT32
	dtb_resetb:1,	//0,0
	b_rsvd1:3,
	bert_resetb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_08_T;

typedef struct
{
	UINT32
	aud_pdb:1,	//0,0
	b_rsvd1:3,
	aud_resetb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_09_T;

typedef struct
{
	UINT32
	pix_pdb:1,	//0,0
	b_rsvd1:3,
	pix_resetb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_0A_T;

typedef struct
{
	UINT32
	cr_tmr_scale:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_10_T;

typedef struct
{
	UINT32
	cr_icp_adj:3,	//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_11_T;

typedef struct
{
	UINT32
	cr_test:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_12_T;

typedef struct
{
	UINT32
	cr_kvco_offset:4,	//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_13_T;

typedef struct
{
	UINT32
	cr_pll_man_mode:2,	//1,0
	b_rsvd1:2,
	cr_i2c_pll_mode:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_14_T;

typedef struct
{
	UINT32
	cr_i2c_offset:3,	//2,0
	b_rsvd1:1,
	cr_i2c_offset_mode:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_15_T;

typedef struct
{
	UINT32
	cr_man_run:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_16_T;

typedef struct
{
	UINT32
	cr_ref_clk_mode:6,	//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_17_T;

typedef struct
{
	UINT32
	cr_ref_clk_mode:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_18_T;

typedef struct
{
	UINT32
	cr_vbgr_ctrl:3,	//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_19_T;

typedef struct
{
	UINT32
	cr_dig_vbgr_ctrl:3,	//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1A_T;

typedef struct
{
	UINT32
	cr_test_clk_en:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1B_T;

typedef struct
{
	UINT32
	cr_pll_offset:3,	//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1C_T;

typedef struct
{
	UINT32
	cr_pll_mode:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1D_T;

typedef struct
{
	UINT32
	cr_lt:1,	//0,0
	b_rsvd1:3,
	cr_ht:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1E_T;

typedef struct
{
	UINT32
	cr_done:1,	//0,0
	b_rsvd1:3,
	cr_lock:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1F_T;

typedef struct
{
	UINT32
	tmr_done:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_20_T;

typedef struct
{
	UINT32
	measure_done_s4:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_21_T;

typedef struct
{
	UINT32
	mode_sel_st:3,	//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_22_T;

typedef struct
{
	UINT32
	tmds_freq:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_23_T;

typedef struct
{
	UINT32
	tmds_freq:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_24_T;

typedef struct
{
	UINT32
	idr_adj:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_25_T;

typedef struct
{
	UINT32
	odt_sel:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_26_T;

typedef struct
{
	UINT32
	pix_pdb:1,	//0,0
	b_rsvd1:3,
	pix_resetb:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_30_T;

typedef struct
{
	UINT32
	pix_cp_i:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_31_T;

typedef struct
{
	UINT32
	pix_vco_offset:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_32_T;

typedef struct
{
	UINT32
	pix_pclk_div2:1,	//0,0
	b_rsvd1:3,
	pix_pclk_div4:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_33_T;

typedef struct
{
	UINT32
	pix_div:2,	//1,0
	b_rsvd1:2,
	pix_pll_dat_sel:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_34_T;

typedef struct
{
	UINT32
	pix_test_en:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_35_T;

typedef struct
{
	UINT32
	pix_link_div:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_36_T;

typedef struct
{
	UINT32
	eq_testen_i2c:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_40_T;

typedef struct
{
	UINT32
	eq_rs_sel:1,	//0,0
	b_rsvd1:3,
	eq_cs_sel:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_41_T;

typedef struct
{
	UINT32
	eq_freeze_l:1,	//0,0
	b_rsvd1:3,
	eq_freeze_h:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_42_T;

typedef struct
{
	UINT32
	eq_rs:5,	//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_43_T;

typedef struct
{
	UINT32
	eq_cs:5,	//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_44_T;

typedef struct
{
	UINT32
	eq_thr_h:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_45_T;

typedef struct
{
	UINT32
	eq_thr_l:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_46_T;

typedef struct
{
	UINT32
	eq_tot_h:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_47_T;

typedef struct
{
	UINT32
	eq_tot_l:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_48_T;

typedef struct
{
	UINT32
	eq_ictl:3,	//2,0
	b_rsvd1:1,
	eq_igmc:3,	//6,4
	b_rsvd2:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_49_T;

typedef struct
{
	UINT32
	eq_cidiv:2,	//1,0
	b_rsvd1:2,
	eq_hldiv:2,	//5,4
	b_rsvd2:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4A_T;

typedef struct
{
	UINT32
	eq_start_frz:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4B_T;

typedef struct
{
	UINT32
	eq_avg_width:2,	//1,0
	b_rsvd1:2,
	eq_af_en:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4C_T;

typedef struct
{
	UINT32
	eq_offset_h:3,	//2,0
	b_rsvd1:1,
	eq_offset_l:3,	//6,4
	b_rsvd2:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4D_T;

typedef struct
{
	UINT32
	eq_cmpe:3,	//2,0
	b_rsvd1:1,
	eq_cmpc:3,	//6,4
	b_rsvd2:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4E_T;

typedef struct
{
	UINT32
	eq_m_cs:5,	//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4F_T;

typedef struct
{
	UINT32
	eq_m_rs:5,	//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_50_T;

typedef struct
{
	UINT32
	eq_freeze_fh:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_51_T;

typedef struct
{
	UINT32
	eq_freeze_fl:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_52_T;

typedef struct
{
	UINT32
	eq_freeze_flag:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_53_T;

typedef struct
{
	UINT32
	odt_ctrl:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_54_T;

typedef struct
{
	UINT32
	dr_clk_inv_ch0:1,	//0,0
	dr_clk_inv_ch1:1,	//1,1
	dr_clk_inv_ch2:1,	//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_80_T;

typedef struct
{
	UINT32
	dr_man_mode:2,	//1,0
	b_rsvd1:2,
	dr_man_mod_sel:1,	//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_81_T;

typedef struct
{
	UINT32
	dr_mode:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_82_T;

typedef struct
{
	UINT32
	dr_n1:3,	//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_83_T;

typedef struct
{
	UINT32
	dr_filter_ch0:6,	//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_84_T;

typedef struct
{
	UINT32
	dr_filter_ch1:6,	//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_85_T;

typedef struct
{
	UINT32
	dr_filter_ch2:6,	//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_86_T;

typedef struct
{
	UINT32
	dr_phsel_f_ch0:4,	//3,0
	dr_phsel_c_ch0:4,	//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_87_T;

typedef struct
{
	UINT32
	dr_phsel_f_ch1:4,	//3,0
	dr_phsel_c_ch1:4,	//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_88_T;

typedef struct
{
	UINT32
	dr_phsel_f_ch2:4,	//3,0
	dr_phsel_c_ch2:4,	//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_89_T;

typedef struct
{
	UINT32
	dr_phsel_en_ch0:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8A_T;

typedef struct
{
	UINT32
	dr_phsel_en_ch1:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8B_T;

typedef struct
{
	UINT32
	dr_phsel_en_ch2:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8C_T;

typedef struct
{
	UINT32
	dr_monitor_ui_en_ch0:1,	//0,0
	dr_monitor_ui_en_ch1:1,	//1,1
	dr_monitor_ui_en_ch2:1,	//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8D_T;

typedef struct
{
	UINT32
	dr_ph_f_ch0:4,	//3,0
	dr_ph_c_ch0:4,	//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8E_T;

typedef struct
{
	UINT32
	dr_ph_f_ch1:4,	//3,0
	dr_ph_c_ch1:4,	//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8F_T;

typedef struct
{
	UINT32
	dr_ph_f_ch2:4,	//3,0
	dr_ph_c_ch2:4,	//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_90_T;

typedef struct
{
	UINT32
	dr_monitor_ui_ch2:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_91_T;

typedef struct
{
	UINT32
	dr_monitor_ui_ch1:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_92_T;

typedef struct
{
	UINT32
	dr_monitor_ui_ch0:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_93_T;

typedef struct
{
	UINT32
	bert_run_ch0:1,	//0,0
	bert_run_ch1:1,	//1,1
	bert_run_ch2:1,	//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A0_T;

typedef struct
{
	UINT32
	bert_ch_sel:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A1_T;

typedef struct
{
	UINT32
	bert_ex_run:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A2_T;

typedef struct
{
	UINT32
	tmds_de_recover:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A3_T;

typedef struct
{
	UINT32
	tmds_clk_inv:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A4_T;

typedef struct
{
	UINT32
	tmds_bypass:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A5_T;

typedef struct
{
	UINT32
	err_thres:2,	//1,0
	b_rsvd1:2,
	bit_thres:2,	//5,4
	b_rsvd2:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A6_T;

typedef struct
{
	UINT32
	bert_sync_done:1,	//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A7_T;

typedef struct
{
	UINT32
	bert_test_res:2,	//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A8_T;

typedef struct
{
	UINT32
	bert_num_err:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A9_T;

typedef struct
{
	UINT32
	bert_num_err:8,	//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_AA_T;

extern L9A_VERSION_0_T gl9a_version_0;
extern HOSTIF_CONTROL_0_T ghostif_control_0;
extern L9A_HOSTIF_INTR_0_T gl9a_hostif_intr_0;
extern L9A_HOSTIF_INTR_EN_0_T gl9a_hostif_intr_en_0;
extern TOP_CONTROL_0_T gtop_control_0;
extern TOP_CONTROL_1_T gtop_control_1;
extern TOP_CONTROL_2_T gtop_control_2;
extern SOFT_RESET_0_T gsoft_reset_0;
extern SOFT_RESET_1_T gsoft_reset_1;
extern SOFT_RESET_2_T gsoft_reset_2;
extern SOFT_RESET_3_T gsoft_reset_3;
extern SOFT_RESET_4_T gsoft_reset_4;
extern SOFT_RESET_5_T gsoft_reset_5;
extern SOFT_RESET_6_T gsoft_reset_6;
extern SOFT_RESET_7_T gsoft_reset_7;
extern SOFT_RESET_8_T gsoft_reset_8;
extern SOFT_RESET_9_T gsoft_reset_9;
extern SOFT_RESET_10_T gsoft_reset_10;
extern SOFT_RESET_11_T gsoft_reset_11;
extern SOFT_RESET_12_T gsoft_reset_12;
extern SOFT_RESET_13_T gsoft_reset_13;
extern SOFT_RESET_14_T gsoft_reset_14;
extern SOFT_RESET_15_T gsoft_reset_15;
extern SOFT_RESET_16_T gsoft_reset_16;
extern SOFT_RESET_17_T gsoft_reset_17;
extern CLOCK_CONTROL_0_T gclock_control_0;
extern CLOCK_CONTROL_1_T gclock_control_1;
extern CLOCK_CONTROL_2_T gclock_control_2;
extern AUDIO_DTO_0_T gaudio_dto_0;
extern AUDIO_DTO_1_T gaudio_dto_1;
extern AUDIO_DTO_2_T gaudio_dto_2;
extern AUDIO_DTO_3_T gaudio_dto_3;
extern AUDIO_DTO_4_T gaudio_dto_4;
extern AUDIO_DTO_5_T gaudio_dto_5;
extern AUDIO_DTO_6_T gaudio_dto_6;
extern AUDIO_DTO_7_T gaudio_dto_7;
extern AUDIO_DTO_8_T gaudio_dto_8;
extern DATA_BRIDGE_0_T gdata_bridge_0;
extern DATA_BRIDGE_1_T gdata_bridge_1;
extern DATA_BRIDGE_2_T gdata_bridge_2;
extern DATA_BRIDGE_3_T gdata_bridge_3;
extern DATA_BRIDGE_4_T gdata_bridge_4;
extern DATA_BRIDGE_5_T gdata_bridge_5;
extern DATA_BRIDGE_6_T gdata_bridge_6;
extern DATA_BRIDGE_7_T gdata_bridge_7;
extern DATA_BRIDGE_8_T gdata_bridge_8;
extern DATA_BRIDGE_9_T gdata_bridge_9;
extern DATA_BRIDGE_10_T gdata_bridge_10;
extern DATA_BRIDGE_11_T gdata_bridge_11;
extern DATA_BRIDGE_12_T gdata_bridge_12;
extern DATA_BRIDGE_13_T gdata_bridge_13;
extern DATA_BRIDGE_14_T gdata_bridge_14;
extern DATA_BRIDGE_15_T gdata_bridge_15;
extern DATA_BRIDGE_16_T gdata_bridge_16;
extern DATA_BRIDGE_17_T gdata_bridge_17;
extern DATA_BRIDGE_18_T gdata_bridge_18;
extern DATA_BRIDGE_19_T gdata_bridge_19;
extern DATA_BRIDGE_20_T gdata_bridge_20;
extern HSLVDSTX0_0_T ghslvdstx0_0;
extern HSLVDSTX0_1_T ghslvdstx0_1;
extern HSLVDSTX0_2_T ghslvdstx0_2;
extern HSLVDSTX0_3_T ghslvdstx0_3;
extern HSLVDSTX0_4_T ghslvdstx0_4;
extern HSLVDSTX0_5_T ghslvdstx0_5;
extern HSLVDSTX0_6_T ghslvdstx0_6;
extern HSLVDSTX1_0_T ghslvdstx1_0;
extern HSLVDSTX1_1_T ghslvdstx1_1;
extern HSLVDSTX1_2_T ghslvdstx1_2;
extern HSLVDSTX1_3_T ghslvdstx1_3;
extern HSLVDSTX1_4_T ghslvdstx1_4;
extern HSLVDSTX1_5_T ghslvdstx1_5;
extern HSLVDSTX1_6_T ghslvdstx1_6;
extern HSLVDSTX2_0_T ghslvdstx2_0;
extern HSLVDSTX2_1_T ghslvdstx2_1;
extern HSLVDSTX2_2_T ghslvdstx2_2;
extern HSLVDSTX2_3_T ghslvdstx2_3;
extern HSLVDSTX2_4_T ghslvdstx2_4;
extern HSLVDSTX2_5_T ghslvdstx2_5;
extern HSLVDSTX2_6_T ghslvdstx2_6;
extern GBB_AFE_0_T ggbb_afe_0;
extern GBB_0_T ggbb_0;
extern AFE_CVBS_0_T gafe_cvbs_0;
extern AFE_CVBS_1_T gafe_cvbs_1;
extern AFE_CVBS_2_T gafe_cvbs_2;
extern AFE_CVBS_3_T gafe_cvbs_3;
extern AFE_VBUF_0_T gafe_vbuf_0;
extern AFE_VBUF_1_T gafe_vbuf_1;
extern AFE_VDAC_0_T gafe_vdac_0;
extern AFE_VDAC_1_T gafe_vdac_1;
extern MAIN_PLL_0_T gmain_pll_0;
extern MAIN_PLL_1_T gmain_pll_1;
extern MAIN_PLL_2_T gmain_pll_2;
extern MAIN_PLL_3_T gmain_pll_3;
extern MAIN_PLL_4_T gmain_pll_4;
extern AFE_CB_0_T gafe_cb_0;
extern AFE_CB_1_T gafe_cb_1;
extern HPD_OUT_CONTROL_0_T ghpd_out_control_0;
extern HPD_OUT_CONTROL_1_T ghpd_out_control_1;
extern HPD_OUT_CONTROL_2_T ghpd_out_control_2;
extern HDMI_PHY0_0_T ghdmi_phy0_0;
extern HDMI_PHY0_1_T ghdmi_phy0_1;
extern HDMI_PHY0_2_T ghdmi_phy0_2;
extern HDMI_PHY1_0_T ghdmi_phy1_0;
extern HDMI_PHY1_1_T ghdmi_phy1_1;
extern HDMI_PHY1_2_T ghdmi_phy1_2;
extern HDMI_PHY1_3_T ghdmi_phy1_3;
extern L9A_HDMI0_INTR_0_T gl9a_hdmi0_intr_0;
extern L9A_HDMI0_INTR_1_T gl9a_hdmi0_intr_1;
extern L9A_HDMI0_INTR_2_T gl9a_hdmi0_intr_2;
extern L9A_HDMI0_INTR_3_T gl9a_hdmi0_intr_3;
extern L9A_HDMI0_INTR_4_T gl9a_hdmi0_intr_4;
extern L9A_HDMI0_INTR_5_T gl9a_hdmi0_intr_5;
extern L9A_HDMI0_INTR_EN_0_T gl9a_hdmi0_intr_en_0;
extern L9A_HDMI0_INTR_EN_1_T gl9a_hdmi0_intr_en_1;
extern L9A_HDMI0_INTR_EN_2_T gl9a_hdmi0_intr_en_2;
extern L9A_HDMI0_INTR_EN_3_T gl9a_hdmi0_intr_en_3;
extern L9A_HDMI0_INTR_EN_4_T gl9a_hdmi0_intr_en_4;
extern L9A_HDMI0_INTR_EN_5_T gl9a_hdmi0_intr_en_5;
extern L9A_HDMI1_INTR_0_T gl9a_hdmi1_intr_0;
extern L9A_HDMI1_INTR_1_T gl9a_hdmi1_intr_1;
extern L9A_HDMI1_INTR_2_T gl9a_hdmi1_intr_2;
extern L9A_HDMI1_INTR_3_T gl9a_hdmi1_intr_3;
extern L9A_HDMI1_INTR_4_T gl9a_hdmi1_intr_4;
extern L9A_HDMI1_INTR_5_T gl9a_hdmi1_intr_5;
extern L9A_HDMI1_INTR_EN_0_T gl9a_hdmi1_intr_en_0;
extern L9A_HDMI1_INTR_EN_1_T gl9a_hdmi1_intr_en_1;
extern L9A_HDMI1_INTR_EN_2_T gl9a_hdmi1_intr_en_2;
extern L9A_HDMI1_INTR_EN_3_T gl9a_hdmi1_intr_en_3;
extern L9A_HDMI1_INTR_EN_4_T gl9a_hdmi1_intr_en_4;
extern L9A_HDMI1_INTR_EN_5_T gl9a_hdmi1_intr_en_5;
extern LINK_0_T glink_0;
extern LINK_1_T glink_1;
extern LINK_2_T glink_2;
extern LINK_3_T glink_3;
extern LINK_4_T glink_4;
extern LINK_5_T glink_5;
extern LINK_6_T glink_6;
extern LINK_7_T glink_7;
extern LINK_8_T glink_8;
extern LINK_9_T glink_9;
extern LINK_10_T glink_10;
extern LINK_11_T glink_11;
extern LINK_12_T glink_12;
extern LINK_13_T glink_13;
extern LINK_14_T glink_14;
extern LINK_15_T glink_15;
extern LINK_16_T glink_16;
extern LINK_17_T glink_17;
extern LINK_18_T glink_18;
extern LINK_19_T glink_19;
extern LINK_20_T glink_20;
extern LINK_21_T glink_21;
extern LINK_22_T glink_22;
extern LINK_23_T glink_23;
extern LINK_24_T glink_24;
extern LINK_25_T glink_25;
extern LINK_26_T glink_26;
extern LINK_27_T glink_27;
extern LINK_28_T glink_28;
extern LINK_29_T glink_29;
extern LINK_30_T glink_30;
extern LINK_31_T glink_31;
extern LINK_32_T glink_32;
extern LINK_33_T glink_33;
extern LINK_34_T glink_34;
extern LINK_35_T glink_35;
extern LINK_36_T glink_36;
extern LINK_37_T glink_37;
extern LINK_38_T glink_38;
extern LINK_39_T glink_39;
extern LINK_40_T glink_40;
extern LINK_41_T glink_41;
extern LINK_42_T glink_42;
extern LINK_43_T glink_43;
extern LINK_44_T glink_44;
extern LINK_45_T glink_45;
extern LINK_46_T glink_46;
extern LINK_47_T glink_47;
extern LINK_48_T glink_48;
extern LINK_49_T glink_49;
extern LINK_50_T glink_50;
extern LINK_51_T glink_51;
extern LINK_52_T glink_52;
extern LINK_53_T glink_53;
extern LINK_54_T glink_54;
extern LINK_55_T glink_55;
extern LINK_56_T glink_56;
extern LINK_57_T glink_57;
extern LINK_58_T glink_58;
extern LINK_59_T glink_59;
extern LINK_60_T glink_60;
extern LINK_61_T glink_61;
extern LINK_62_T glink_62;
extern LINK_63_T glink_63;
extern LINK_64_T glink_64;
extern LINK_65_T glink_65;
extern LINK_66_T glink_66;
extern LINK_67_T glink_67;
extern LINK_68_T glink_68;
extern LINK_69_T glink_69;
extern LINK_70_T glink_70;
extern LINK_71_T glink_71;
extern LINK_72_T glink_72;
extern LINK_73_T glink_73;
extern LINK_74_T glink_74;
extern LINK_75_T glink_75;
extern LINK_76_T glink_76;
extern LINK_77_T glink_77;
extern LINK_78_T glink_78;
extern LINK_79_T glink_79;
extern LINK_80_T glink_80;
extern LINK_81_T glink_81;
extern LINK_82_T glink_82;
extern LINK_83_T glink_83;
extern LINK_84_T glink_84;
extern LINK_85_T glink_85;
extern LINK_86_T glink_86;
extern LINK_87_T glink_87;
extern LINK_88_T glink_88;
extern LINK_89_T glink_89;
extern LINK_90_T glink_90;
extern LINK_91_T glink_91;
extern LINK_92_T glink_92;
extern LINK_93_T glink_93;
extern LINK_94_T glink_94;
extern LINK_95_T glink_95;
extern LINK_96_T glink_96;
extern LINK_97_T glink_97;
extern LINK_98_T glink_98;
extern LINK_99_T glink_99;
extern LINK_100_T glink_100;
extern LINK_101_T glink_101;
extern LINK_102_T glink_102;
extern LINK_103_T glink_103;
extern LINK_104_T glink_104;
extern LINK_105_T glink_105;
extern LINK_106_T glink_106;
extern LINK_107_T glink_107;
extern LINK_108_T glink_108;
extern LINK_109_T glink_109;
extern LINK_110_T glink_110;
extern LINK_111_T glink_111;
extern LINK_112_T glink_112;
extern LINK_113_T glink_113;
extern LINK_114_T glink_114;
extern LINK_115_T glink_115;
extern LINK_116_T glink_116;
extern LINK_117_T glink_117;
extern LINK_118_T glink_118;
extern LINK_119_T glink_119;
extern LINK_120_T glink_120;
extern LINK_121_T glink_121;
extern LINK_122_T glink_122;
extern LINK_123_T glink_123;
extern LINK_124_T glink_124;
extern LINK_125_T glink_125;
extern LINK_126_T glink_126;
extern LINK_127_T glink_127;
extern LINK_128_T glink_128;
extern LINK_129_T glink_129;
extern LINK_130_T glink_130;
extern LINK_131_T glink_131;
extern LINK_132_T glink_132;
extern LINK_133_T glink_133;
extern LINK_134_T glink_134;
extern LINK_135_T glink_135;
extern LINK_136_T glink_136;
extern LINK_137_T glink_137;
extern LINK_138_T glink_138;
extern LINK_139_T glink_139;
extern LINK_140_T glink_140;
extern LINK_141_T glink_141;
extern LINK_142_T glink_142;
extern LINK_143_T glink_143;
extern LINK_144_T glink_144;
extern LINK_145_T glink_145;
extern LINK_146_T glink_146;
extern LINK_147_T glink_147;
extern LINK_148_T glink_148;
extern LINK_149_T glink_149;
extern LINK_150_T glink_150;
extern LINK_151_T glink_151;
extern LINK_152_T glink_152;
extern LINK_153_T glink_153;
extern LINK_154_T glink_154;
extern LINK_155_T glink_155;
extern LINK_156_T glink_156;
extern LINK_157_T glink_157;
extern LINK_158_T glink_158;
extern LINK_159_T glink_159;
extern LINK_160_T glink_160;
extern LINK_161_T glink_161;
extern LINK_162_T glink_162;
extern LINK_163_T glink_163;
extern LINK_164_T glink_164;
extern LINK_165_T glink_165;
extern LINK_166_T glink_166;
extern LINK_167_T glink_167;
extern LINK_168_T glink_168;
extern LINK_169_T glink_169;
extern LINK_170_T glink_170;
extern LINK_171_T glink_171;
extern LINK_172_T glink_172;
extern LINK_173_T glink_173;
extern LINK_174_T glink_174;
extern LINK_175_T glink_175;
extern LINK_176_T glink_176;
extern LINK_177_T glink_177;
extern LINK_178_T glink_178;
extern LINK_179_T glink_179;
extern LINK_180_T glink_180;
extern LINK_181_T glink_181;
extern LINK_182_T glink_182;
extern LINK_183_T glink_183;
extern LINK_184_T glink_184;
extern LINK_185_T glink_185;
extern LINK_186_T glink_186;
extern LINK_187_T glink_187;
extern LINK_188_T glink_188;
extern LINK_189_T glink_189;
extern LINK_190_T glink_190;
extern LINK_191_T glink_191;
extern LINK_192_T glink_192;
extern LINK_193_T glink_193;
extern LINK_194_T glink_194;
extern LINK_195_T glink_195;
extern LINK_196_T glink_196;
extern LINK_197_T glink_197;
extern LINK_198_T glink_198;
extern LINK_199_T glink_199;
extern LINK_200_T glink_200;
extern LINK_201_T glink_201;
extern LINK_202_T glink_202;
extern LINK_203_T glink_203;
extern LINK_204_T glink_204;
extern LINK_205_T glink_205;
extern LINK_206_T glink_206;
extern LINK_207_T glink_207;
extern LINK_208_T glink_208;
extern LINK_209_T glink_209;
extern LINK_210_T glink_210;
extern LINK_211_T glink_211;
extern LINK_212_T glink_212;
extern LINK_213_T glink_213;
extern LINK_214_T glink_214;
extern LINK_215_T glink_215;
extern LINK_216_T glink_216;
extern LINK_217_T glink_217;
extern LINK_218_T glink_218;
extern LINK_219_T glink_219;
extern LINK_220_T glink_220;
extern LINK_221_T glink_221;
extern LINK_222_T glink_222;
extern LINK_223_T glink_223;
extern LINK_224_T glink_224;
extern LINK_225_T glink_225;
extern LINK_226_T glink_226;
extern LINK_227_T glink_227;
extern LINK_228_T glink_228;
extern LINK_229_T glink_229;
extern LINK_230_T glink_230;
extern LINK_231_T glink_231;
extern LINK_232_T glink_232;
extern LINK_233_T glink_233;
extern LINK_234_T glink_234;
extern LINK_235_T glink_235;
extern LINK_236_T glink_236;
extern LINK_237_T glink_237;
extern LINK_238_T glink_238;
extern LINK_239_T glink_239;
extern LINK_240_T glink_240;
extern LINK_241_T glink_241;
extern LINK_242_T glink_242;
extern LINK_243_T glink_243;
extern LINK_244_T glink_244;
extern LINK_245_T glink_245;
extern LINK_246_T glink_246;
extern LINK_247_T glink_247;
extern LINK_248_T glink_248;
extern LINK_249_T glink_249;
extern LINK_250_T glink_250;
extern LINK_251_T glink_251;
extern LINK_252_T glink_252;
extern LINK_253_T glink_253;
extern LINK_254_T glink_254;
extern LINK_255_T glink_255;
extern LINK_256_T glink_256;
extern LINK_257_T glink_257;
extern LINK_258_T glink_258;
extern LINK_259_T glink_259;
extern LINK_260_T glink_260;
extern LINK_261_T glink_261;
extern LINK_262_T glink_262;
extern LINK_263_T glink_263;
extern LINK_264_T glink_264;
extern LINK_265_T glink_265;
extern LINK_266_T glink_266;
extern LINK_267_T glink_267;
extern LINK_268_T glink_268;
extern LINK_269_T glink_269;
extern LINK_270_T glink_270;
extern LINK_271_T glink_271;
extern LINK_272_T glink_272;
extern LINK_273_T glink_273;
extern LINK_274_T glink_274;
extern LINK_275_T glink_275;
extern LINK_276_T glink_276;
extern LINK_277_T glink_277;
extern LINK_278_T glink_278;
extern LINK_279_T glink_279;
extern LINK_280_T glink_280;
extern LINK_281_T glink_281;
extern LINK_282_T glink_282;
extern LINK_283_T glink_283;
extern LINK_284_T glink_284;
extern LINK_285_T glink_285;
extern LINK_286_T glink_286;
extern LINK_287_T glink_287;
extern LINK_288_T glink_288;
extern LINK_289_T glink_289;
extern LINK_290_T glink_290;
extern LINK_291_T glink_291;
extern LINK_292_T glink_292;
extern LINK_293_T glink_293;
extern LINK_294_T glink_294;
extern LINK_295_T glink_295;
extern LINK_296_T glink_296;
extern LINK_297_T glink_297;
extern LINK_298_T glink_298;
extern LINK_299_T glink_299;
extern LINK_300_T glink_300;
extern LINK_301_T glink_301;
extern LINK_302_T glink_302;
extern LINK_303_T glink_303;
extern LINK_304_T glink_304;
extern LINK_305_T glink_305;
extern LINK_306_T glink_306;
extern LINK_307_T glink_307;
extern LINK_308_T glink_308;
extern LINK_309_T glink_309;
extern LINK_310_T glink_310;
extern LINK_311_T glink_311;
extern LINK_312_T glink_312;
extern LINK_313_T glink_313;
extern LINK_314_T glink_314;
extern LINK_315_T glink_315;
extern LINK_316_T glink_316;
extern LINK_317_T glink_317;
extern LINK_318_T glink_318;
extern LINK_319_T glink_319;
extern LINK_320_T glink_320;
extern LINK_321_T glink_321;
extern LINK_322_T glink_322;
extern LINK_323_T glink_323;
extern LINK_324_T glink_324;
extern LINK_325_T glink_325;
extern LINK_326_T glink_326;
extern LINK_327_T glink_327;
extern LINK_328_T glink_328;
extern LINK_329_T glink_329;
extern LINK_330_T glink_330;
extern LINK_331_T glink_331;
extern LINK_332_T glink_332;
extern LINK_333_T glink_333;
extern LINK_334_T glink_334;
extern LINK_335_T glink_335;
extern LINK_336_T glink_336;
extern LINK_337_T glink_337;
extern LINK_338_T glink_338;
extern LINK_339_T glink_339;
extern LINK_340_T glink_340;
extern LINK_341_T glink_341;
extern LINK_342_T glink_342;
extern LINK_343_T glink_343;
extern LINK_344_T glink_344;
extern LINK_345_T glink_345;
extern LINK_346_T glink_346;
extern LINK_347_T glink_347;
extern LINK_348_T glink_348;
extern LINK_349_T glink_349;
extern LINK_350_T glink_350;
extern LINK_351_T glink_351;
extern LINK_352_T glink_352;
extern LINK_353_T glink_353;
extern LINK_354_T glink_354;
extern LINK_355_T glink_355;
extern LINK_356_T glink_356;
extern LINK_357_T glink_357;
extern LINK_358_T glink_358;
extern LINK_359_T glink_359;
extern LINK_360_T glink_360;
extern LINK_361_T glink_361;
extern LINK_362_T glink_362;
extern LINK_363_T glink_363;
extern LINK_364_T glink_364;
extern LINK_365_T glink_365;
extern LINK_366_T glink_366;
extern LINK_367_T glink_367;
extern LINK_368_T glink_368;
extern LINK_369_T glink_369;
extern LINK_370_T glink_370;
extern LINK_371_T glink_371;
extern LINK_372_T glink_372;
extern LINK_373_T glink_373;
extern LINK_374_T glink_374;
extern LINK_375_T glink_375;
extern LINK_376_T glink_376;
extern LINK_377_T glink_377;
extern LINK_378_T glink_378;
extern LINK_379_T glink_379;
extern LINK_380_T glink_380;
extern LINK_381_T glink_381;
extern LINK_382_T glink_382;
extern LINK_383_T glink_383;
extern LINK_384_T glink_384;
extern LINK_385_T glink_385;
extern LINK_386_T glink_386;
extern LINK_387_T glink_387;
extern LINK_388_T glink_388;
extern LINK_389_T glink_389;
extern LINK_390_T glink_390;
extern LINK_391_T glink_391;
extern LINK_392_T glink_392;
extern LINK_393_T glink_393;
extern LINK_394_T glink_394;
extern LINK_395_T glink_395;
extern LINK_396_T glink_396;
extern LINK_397_T glink_397;
extern LINK_398_T glink_398;
extern LINK_399_T glink_399;
extern LINK_400_T glink_400;
extern LINK_401_T glink_401;
extern LINK_402_T glink_402;
extern LINK_403_T glink_403;
extern LINK_404_T glink_404;
extern LINK_405_T glink_405;
extern LINK_406_T glink_406;
extern LINK_407_T glink_407;
extern LINK_408_T glink_408;
extern LINK_409_T glink_409;
extern LINK_410_T glink_410;
extern LINK_411_T glink_411;
extern LINK_412_T glink_412;
extern LINK_413_T glink_413;
extern LINK_414_T glink_414;
extern LINK_415_T glink_415;
extern LINK_416_T glink_416;
extern LINK_417_T glink_417;
extern LINK_418_T glink_418;
extern LINK_419_T glink_419;
extern LINK_420_T glink_420;
extern LINK_421_T glink_421;
extern LINK_422_T glink_422;
extern LINK_423_T glink_423;
extern LINK_424_T glink_424;
extern LINK_425_T glink_425;
extern LINK_426_T glink_426;
extern LINK_427_T glink_427;
extern LINK_428_T glink_428;
extern LINK_429_T glink_429;
extern LINK_430_T glink_430;
extern LINK_431_T glink_431;
extern LINK_432_T glink_432;
extern LINK_433_T glink_433;
extern LINK_434_T glink_434;
extern LINK_435_T glink_435;
extern LINK_436_T glink_436;
extern LINK_437_T glink_437;
extern LINK_438_T glink_438;
extern LINK_439_T glink_439;
extern LINK_440_T glink_440;
extern LINK_441_T glink_441;
extern LINK_442_T glink_442;
extern LINK_443_T glink_443;
extern LINK_444_T glink_444;
extern LINK_445_T glink_445;
extern LINK_446_T glink_446;
extern LINK_447_T glink_447;
extern LINK_448_T glink_448;
extern LINK_449_T glink_449;
extern LINK_450_T glink_450;
extern LINK_451_T glink_451;
extern LINK_452_T glink_452;
extern LINK_453_T glink_453;
extern LINK_454_T glink_454;
extern LINK_455_T glink_455;
extern LINK_456_T glink_456;
extern LINK_457_T glink_457;
extern LINK_458_T glink_458;
extern LINK_459_T glink_459;
extern LINK_460_T glink_460;
extern LINK_461_T glink_461;
extern LINK_462_T glink_462;
extern LINK_463_T glink_463;
extern LINK_464_T glink_464;
extern LINK_465_T glink_465;
extern LINK_466_T glink_466;
extern LINK_467_T glink_467;
extern LINK_468_T glink_468;
extern LINK_469_T glink_469;
extern LINK_470_T glink_470;
extern LINK_471_T glink_471;
extern LINK_472_T glink_472;
extern LINK_473_T glink_473;
extern LINK_474_T glink_474;
extern LINK_475_T glink_475;
extern LINK_476_T glink_476;
extern LINK_477_T glink_477;
extern LINK_478_T glink_478;
extern LINK_479_T glink_479;
extern LINK_480_T glink_480;
extern LINK_481_T glink_481;
extern LINK_482_T glink_482;
extern LINK_483_T glink_483;
extern LINK_484_T glink_484;
extern LINK_485_T glink_485;
extern LINK_486_T glink_486;
extern LINK_487_T glink_487;
extern LINK_488_T glink_488;
extern LINK_489_T glink_489;
extern LINK_490_T glink_490;
extern LINK_491_T glink_491;
extern LINK_492_T glink_492;
extern LINK_493_T glink_493;
extern LINK_494_T glink_494;
extern LINK_495_T glink_495;
extern LINK_496_T glink_496;
extern LINK_497_T glink_497;
extern LINK_498_T glink_498;
extern LINK_499_T glink_499;
extern LINK_500_T glink_500;
extern LINK_501_T glink_501;
extern LINK_502_T glink_502;
extern LINK_503_T glink_503;
extern LINK_504_T glink_504;
extern LINK_505_T glink_505;
extern LINK_506_T glink_506;
extern LINK_507_T glink_507;
extern LINK_508_T glink_508;
extern LINK_509_T glink_509;
extern LINK_510_T glink_510;
extern LINK_511_T glink_511;
extern LINK_512_T glink_512;
extern LINK_513_T glink_513;
extern LINK_514_T glink_514;
extern LINK_515_T glink_515;
extern LINK_516_T glink_516;
extern LINK_517_T glink_517;
extern LINK_518_T glink_518;
extern LINK_519_T glink_519;
extern LINK_520_T glink_520;
extern LINK_521_T glink_521;
extern LINK_522_T glink_522;
extern LINK_523_T glink_523;
extern LINK_524_T glink_524;
extern LINK_525_T glink_525;
extern LINK_526_T glink_526;
extern LINK_527_T glink_527;
extern LINK_528_T glink_528;
extern LINK_529_T glink_529;
extern LINK_530_T glink_530;
extern LINK_531_T glink_531;
extern LINK_532_T glink_532;
extern LINK_533_T glink_533;
extern LINK_534_T glink_534;
extern LINK_535_T glink_535;
extern LINK_536_T glink_536;
extern LINK_537_T glink_537;
extern LINK_538_T glink_538;
extern LINK_539_T glink_539;
extern LINK_540_T glink_540;
extern LINK_541_T glink_541;
extern LINK_542_T glink_542;
extern LINK_543_T glink_543;
extern LINK_544_T glink_544;
extern LINK_545_T glink_545;
extern LINK_546_T glink_546;
extern LINK_547_T glink_547;
extern LINK_548_T glink_548;
extern LINK_549_T glink_549;
extern LINK_550_T glink_550;
extern LINK_551_T glink_551;
extern LINK_552_T glink_552;
extern LINK_553_T glink_553;
extern LINK_554_T glink_554;
extern LINK_555_T glink_555;
extern LINK_556_T glink_556;
extern LINK_557_T glink_557;
extern LINK_558_T glink_558;
extern LINK_559_T glink_559;
extern LINK_560_T glink_560;
extern LINK_561_T glink_561;
extern LINK_562_T glink_562;
extern LINK_563_T glink_563;
extern LINK_564_T glink_564;
extern LINK_565_T glink_565;
extern LINK_566_T glink_566;
extern LINK_567_T glink_567;
extern LINK_568_T glink_568;
extern LINK_569_T glink_569;
extern LINK_570_T glink_570;
extern LINK_571_T glink_571;
extern LINK_572_T glink_572;
extern LINK_573_T glink_573;
extern LINK_574_T glink_574;
extern LINK_575_T glink_575;
extern LINK_576_T glink_576;
extern LINK_577_T glink_577;
extern LINK_578_T glink_578;
extern LINK_579_T glink_579;
extern LINK_580_T glink_580;
extern LINK_581_T glink_581;
extern LINK_582_T glink_582;
extern LINK_583_T glink_583;
extern LINK_584_T glink_584;
extern LINK_585_T glink_585;
extern LINK_586_T glink_586;
extern LINK_587_T glink_587;
extern LINK_588_T glink_588;
extern LINK_589_T glink_589;
extern LINK_590_T glink_590;
extern LINK_591_T glink_591;
extern LINK_592_T glink_592;
extern LINK_593_T glink_593;
extern LINK_594_T glink_594;
extern LINK_595_T glink_595;
extern LINK_596_T glink_596;
extern LINK_597_T glink_597;
extern LINK_598_T glink_598;
extern LINK_599_T glink_599;
extern LINK_600_T glink_600;
extern LINK_601_T glink_601;
extern LINK_602_T glink_602;
extern LINK_603_T glink_603;
extern LINK_604_T glink_604;
extern LINK_605_T glink_605;
extern LINK_606_T glink_606;
extern LINK_607_T glink_607;
extern LINK_608_T glink_608;
extern LINK_609_T glink_609;
extern LINK_610_T glink_610;
extern LINK_611_T glink_611;
extern LINK_612_T glink_612;
extern LINK_613_T glink_613;
extern LINK_614_T glink_614;
extern LINK_615_T glink_615;
extern LINK_616_T glink_616;
extern LINK_617_T glink_617;
extern LINK_618_T glink_618;
extern LINK_619_T glink_619;
extern LINK_620_T glink_620;
extern LINK_621_T glink_621;
extern LINK_622_T glink_622;
extern LINK_623_T glink_623;
extern LINK_624_T glink_624;
extern LINK_625_T glink_625;
extern LINK_626_T glink_626;
extern LINK_627_T glink_627;
extern LINK_628_T glink_628;
extern LINK_629_T glink_629;
extern LINK_630_T glink_630;
extern LINK_631_T glink_631;
extern LINK_632_T glink_632;
extern LINK_633_T glink_633;
extern LINK_634_T glink_634;
extern LINK_635_T glink_635;
extern LINK_636_T glink_636;
extern LINK_637_T glink_637;
extern LINK_638_T glink_638;
extern LINK_639_T glink_639;
extern LINK_640_T glink_640;
extern LINK_641_T glink_641;
extern LINK_642_T glink_642;
extern LINK_643_T glink_643;
extern LINK_644_T glink_644;
extern LINK_645_T glink_645;
extern LINK_646_T glink_646;
extern LINK_647_T glink_647;
extern LINK_648_T glink_648;
extern LINK_649_T glink_649;
extern LINK_650_T glink_650;
extern LINK_651_T glink_651;
extern LINK_652_T glink_652;
extern LINK_653_T glink_653;
extern LINK_654_T glink_654;
extern LINK_655_T glink_655;
extern LINK_656_T glink_656;
extern LINK_657_T glink_657;
extern LINK_658_T glink_658;
extern LINK_659_T glink_659;
extern LINK_660_T glink_660;
extern LINK_661_T glink_661;
extern LINK_662_T glink_662;
extern LINK_663_T glink_663;
extern LINK_664_T glink_664;
extern LINK_665_T glink_665;
extern LINK_666_T glink_666;
extern LINK_667_T glink_667;
extern LINK_668_T glink_668;
extern LINK_669_T glink_669;
extern LINK_670_T glink_670;
extern LINK_671_T glink_671;
extern LINK_672_T glink_672;
extern LINK_673_T glink_673;
extern LINK_674_T glink_674;
extern LINK_675_T glink_675;
extern LINK_676_T glink_676;
extern LINK_677_T glink_677;
extern LINK_678_T glink_678;
extern LINK_679_T glink_679;
extern LINK_680_T glink_680;
extern LINK_681_T glink_681;
extern LINK_682_T glink_682;
extern LINK_683_T glink_683;
extern LINK_684_T glink_684;
extern LINK_685_T glink_685;
extern LINK_686_T glink_686;
extern LINK_687_T glink_687;
extern LINK_688_T glink_688;
extern LINK_689_T glink_689;
extern LINK_690_T glink_690;
extern LINK_691_T glink_691;
extern LINK_692_T glink_692;
extern LINK_693_T glink_693;
extern LINK_694_T glink_694;
extern LINK_695_T glink_695;
extern LINK_696_T glink_696;
extern LINK_697_T glink_697;
extern LINK_698_T glink_698;
extern LINK_699_T glink_699;
extern LINK_700_T glink_700;
extern LINK_701_T glink_701;
extern LINK_702_T glink_702;
extern LINK_703_T glink_703;
extern LINK_704_T glink_704;
extern LINK_705_T glink_705;
extern LINK_706_T glink_706;
extern LINK_707_T glink_707;
extern LINK_708_T glink_708;
extern LINK_709_T glink_709;
extern LINK_710_T glink_710;
extern LINK_711_T glink_711;
extern LINK_712_T glink_712;
extern LINK_713_T glink_713;
extern LINK_714_T glink_714;
extern LINK_715_T glink_715;
extern LINK_716_T glink_716;
extern LINK_717_T glink_717;
extern LINK_718_T glink_718;
extern LINK_719_T glink_719;
extern LINK_720_T glink_720;
extern LINK_721_T glink_721;
extern LINK_722_T glink_722;
extern LINK_723_T glink_723;
extern LINK_724_T glink_724;
extern LINK_725_T glink_725;
extern LINK_726_T glink_726;
extern LINK_727_T glink_727;
extern LINK_728_T glink_728;
extern LINK_729_T glink_729;
extern LINK_730_T glink_730;
extern LINK_731_T glink_731;
extern LINK_732_T glink_732;
extern LINK_733_T glink_733;
extern LINK_734_T glink_734;
extern LINK_735_T glink_735;
extern LINK_736_T glink_736;
extern LINK_737_T glink_737;
extern LINK_738_T glink_738;
extern LINK_739_T glink_739;
extern LINK_740_T glink_740;
extern LINK_741_T glink_741;
extern LINK_742_T glink_742;
extern LINK_743_T glink_743;
extern LINK_744_T glink_744;
extern LINK_745_T glink_745;
extern LINK_746_T glink_746;
extern LINK_747_T glink_747;
extern LINK_748_T glink_748;
extern LINK_749_T glink_749;
extern LINK_750_T glink_750;
extern LINK_751_T glink_751;
extern LINK_752_T glink_752;
extern LINK_753_T glink_753;
extern LINK_754_T glink_754;
extern LINK_755_T glink_755;
extern LINK_756_T glink_756;
extern LINK_757_T glink_757;
extern LINK_758_T glink_758;
extern LINK_759_T glink_759;
extern LINK_760_T glink_760;
extern LINK_761_T glink_761;
extern LINK_762_T glink_762;
extern LINK_763_T glink_763;
extern LINK_764_T glink_764;
extern LINK_765_T glink_765;
extern LINK_766_T glink_766;
extern LINK_767_T glink_767;
extern LINK_768_T glink_768;
extern LINK_769_T glink_769;
extern LINK_770_T glink_770;
extern LINK_771_T glink_771;
extern LINK_772_T glink_772;
extern LINK_773_T glink_773;
extern LINK_774_T glink_774;
extern LINK_775_T glink_775;
extern LINK_776_T glink_776;
extern LINK_777_T glink_777;
extern LINK_778_T glink_778;
extern LINK_779_T glink_779;
extern LINK_780_T glink_780;
extern LINK_781_T glink_781;
extern LINK_782_T glink_782;
extern LINK_783_T glink_783;
extern LINK_784_T glink_784;
extern LINK_785_T glink_785;
extern LINK_786_T glink_786;
extern LINK_787_T glink_787;
extern LINK_788_T glink_788;
extern LINK_789_T glink_789;
extern LINK_790_T glink_790;
extern LINK_791_T glink_791;
extern LINK_792_T glink_792;
extern LINK_793_T glink_793;
extern LINK_794_T glink_794;
extern LINK_795_T glink_795;
extern LINK_796_T glink_796;
extern LINK_797_T glink_797;
extern LINK_798_T glink_798;
extern LINK_799_T glink_799;
extern LINK_800_T glink_800;
extern LINK_801_T glink_801;
extern LINK_802_T glink_802;
extern LINK_803_T glink_803;
extern LINK_804_T glink_804;
extern LINK_805_T glink_805;
extern LINK_806_T glink_806;
extern LINK_807_T glink_807;
extern LINK_808_T glink_808;
extern LINK_809_T glink_809;
extern LINK_810_T glink_810;
extern LINK_811_T glink_811;
extern LINK_812_T glink_812;
extern LINK_813_T glink_813;
extern LINK_814_T glink_814;
extern LINK_815_T glink_815;
extern LINK_816_T glink_816;
extern LINK_817_T glink_817;
extern LINK_818_T glink_818;
extern LINK_819_T glink_819;
extern LINK_820_T glink_820;
extern LINK_821_T glink_821;
extern LINK_822_T glink_822;
extern LINK_823_T glink_823;
extern LINK_824_T glink_824;
extern LINK_825_T glink_825;
extern LINK_826_T glink_826;
extern LINK_827_T glink_827;
extern LINK_828_T glink_828;
extern LINK_829_T glink_829;
extern LINK_830_T glink_830;
extern LINK_831_T glink_831;
extern LINK_832_T glink_832;
extern LINK_833_T glink_833;
extern LINK_834_T glink_834;
extern LINK_835_T glink_835;
extern LINK_836_T glink_836;
extern LINK_837_T glink_837;
extern LINK_838_T glink_838;
extern LINK_839_T glink_839;
extern LINK_840_T glink_840;
extern LINK_841_T glink_841;
extern LINK_842_T glink_842;
extern LINK_843_T glink_843;
extern LINK_844_T glink_844;
extern LINK_845_T glink_845;
extern LINK_846_T glink_846;
extern LINK_847_T glink_847;
extern LINK_848_T glink_848;
extern LINK_849_T glink_849;
extern LINK_850_T glink_850;
extern LINK_851_T glink_851;
extern LINK_852_T glink_852;
extern LINK_853_T glink_853;
extern LINK_854_T glink_854;
extern LINK_855_T glink_855;
extern LINK_856_T glink_856;
extern LINK_857_T glink_857;
extern LINK_858_T glink_858;
extern LINK_859_T glink_859;
extern LINK_860_T glink_860;
extern LINK_861_T glink_861;
extern LINK_862_T glink_862;
extern LINK_863_T glink_863;
extern LINK_864_T glink_864;
extern LINK_865_T glink_865;
extern LINK_866_T glink_866;
extern LINK_867_T glink_867;
extern LINK_868_T glink_868;
extern LINK_869_T glink_869;
extern LINK_870_T glink_870;
extern LINK_871_T glink_871;
extern LINK_872_T glink_872;
extern LINK_873_T glink_873;
extern LINK_874_T glink_874;
extern LINK_875_T glink_875;
extern LINK_876_T glink_876;
extern LINK_877_T glink_877;
extern LINK_878_T glink_878;
extern LINK_879_T glink_879;
extern LINK_880_T glink_880;
extern LINK_881_T glink_881;
extern LINK_882_T glink_882;
extern LINK_883_T glink_883;
extern LINK_884_T glink_884;
extern LINK_885_T glink_885;
extern LINK_886_T glink_886;
extern LINK_887_T glink_887;
extern LINK_888_T glink_888;
extern LINK_889_T glink_889;
extern LINK_890_T glink_890;
extern LINK_891_T glink_891;
extern LINK_892_T glink_892;
extern LINK_893_T glink_893;
extern LINK_894_T glink_894;
extern LINK_895_T glink_895;
extern LINK_896_T glink_896;
extern LINK_897_T glink_897;
extern LINK_898_T glink_898;
extern LINK_899_T glink_899;
extern LINK_900_T glink_900;
extern LINK_901_T glink_901;
extern LINK_902_T glink_902;
extern LINK_903_T glink_903;
extern LINK_904_T glink_904;
extern LINK_905_T glink_905;
extern LINK_906_T glink_906;
extern LINK_907_T glink_907;
extern LINK_908_T glink_908;
extern LINK_909_T glink_909;
extern LINK_910_T glink_910;
extern LINK_911_T glink_911;
extern LINK_912_T glink_912;
extern LINK_913_T glink_913;
extern LINK_914_T glink_914;
extern LINK_915_T glink_915;
extern LINK_916_T glink_916;
extern LINK_917_T glink_917;
extern LINK_918_T glink_918;
extern LINK_919_T glink_919;
extern LINK_920_T glink_920;
extern LINK_921_T glink_921;
extern LINK_922_T glink_922;
extern LINK_923_T glink_923;
extern LINK_924_T glink_924;
extern LINK_925_T glink_925;
extern LINK_926_T glink_926;
extern LINK_927_T glink_927;
extern LINK_928_T glink_928;
extern LINK_929_T glink_929;
extern LINK_930_T glink_930;
extern LINK_931_T glink_931;
extern LINK_932_T glink_932;
extern LINK_933_T glink_933;
extern LINK_934_T glink_934;
extern LINK_935_T glink_935;
extern LINK_936_T glink_936;
extern LINK_937_T glink_937;
extern LINK_938_T glink_938;
extern LINK_939_T glink_939;
extern LINK_940_T glink_940;
extern LINK_941_T glink_941;
extern LINK_942_T glink_942;
extern LINK_943_T glink_943;
extern LINK_944_T glink_944;
extern LINK_945_T glink_945;
extern LINK_946_T glink_946;
extern LINK_947_T glink_947;
extern LINK_948_T glink_948;
extern LINK_949_T glink_949;
extern LINK_950_T glink_950;
extern LINK_951_T glink_951;
extern LINK_952_T glink_952;
extern LINK_953_T glink_953;
extern LINK_954_T glink_954;
extern LINK_955_T glink_955;
extern LINK_956_T glink_956;
extern LINK_957_T glink_957;
extern LINK_958_T glink_958;
extern LINK_959_T glink_959;
extern LINK_960_T glink_960;
extern LINK_961_T glink_961;
extern LINK_962_T glink_962;
extern LINK_963_T glink_963;
extern LINK_964_T glink_964;
extern LINK_965_T glink_965;
extern LINK_966_T glink_966;
extern LINK_967_T glink_967;
extern LINK_968_T glink_968;
extern LINK_969_T glink_969;
extern LINK_970_T glink_970;
extern LINK_971_T glink_971;
extern LINK_972_T glink_972;
extern LINK_973_T glink_973;
extern LINK_974_T glink_974;
extern LINK_975_T glink_975;
extern LINK_976_T glink_976;
extern LINK_977_T glink_977;
extern LINK_978_T glink_978;
extern LINK_979_T glink_979;
extern LINK_980_T glink_980;
extern LINK_981_T glink_981;
extern LINK_982_T glink_982;
extern LINK_983_T glink_983;
extern LINK_984_T glink_984;
extern LINK_985_T glink_985;
extern LINK_986_T glink_986;
extern LINK_987_T glink_987;
extern LINK_988_T glink_988;
extern LINK_989_T glink_989;
extern LINK_990_T glink_990;
extern LINK_991_T glink_991;
extern LINK_992_T glink_992;
extern LINK_993_T glink_993;
extern LINK_994_T glink_994;
extern LINK_995_T glink_995;
extern LINK_996_T glink_996;
extern LINK_997_T glink_997;
extern LINK_998_T glink_998;
extern LINK_999_T glink_999;
extern LINK_1000_T glink_1000;
extern LINK_1001_T glink_1001;
extern LINK_1002_T glink_1002;
extern LINK_1003_T glink_1003;
extern LINK_1004_T glink_1004;
extern LINK_1005_T glink_1005;
extern LINK_1006_T glink_1006;
extern LINK_1007_T glink_1007;
extern LINK_1008_T glink_1008;
extern LINK_1009_T glink_1009;
extern LINK_1010_T glink_1010;
extern LINK_1011_T glink_1011;
extern LINK_1012_T glink_1012;
extern LINK_1013_T glink_1013;
extern LINK_1014_T glink_1014;
extern LINK_1015_T glink_1015;
extern LINK_1016_T glink_1016;
extern LINK_1017_T glink_1017;
extern LINK_1018_T glink_1018;
extern LINK_1019_T glink_1019;
extern LINK_1020_T glink_1020;
extern LINK_1021_T glink_1021;
extern LINK_1022_T glink_1022;
extern LINK_1023_T glink_1023;
extern LINK_1024_T glink_1024;
extern LINK_1025_T glink_1025;
extern LINK_1026_T glink_1026;
extern LINK_1027_T glink_1027;
extern LINK_1028_T glink_1028;
extern LINK_1029_T glink_1029;
extern LINK_1030_T glink_1030;
extern LINK_1031_T glink_1031;
extern LINK_1032_T glink_1032;
extern LINK_1033_T glink_1033;
extern LINK_1034_T glink_1034;
extern LINK_1035_T glink_1035;
extern LINK_1036_T glink_1036;
extern LINK_1037_T glink_1037;
extern LINK_1038_T glink_1038;
extern LINK_1039_T glink_1039;
extern LINK_1040_T glink_1040;
extern LINK_1041_T glink_1041;
extern LINK_1042_T glink_1042;
extern LINK_1043_T glink_1043;
extern LINK_1044_T glink_1044;
extern LINK_1045_T glink_1045;
extern LINK_1046_T glink_1046;
extern LINK_1047_T glink_1047;
extern LINK_1048_T glink_1048;
extern LINK_1049_T glink_1049;
extern LINK_1050_T glink_1050;
extern LINK_1051_T glink_1051;
extern LINK_1052_T glink_1052;
extern LINK_1053_T glink_1053;
extern LINK_1054_T glink_1054;
extern LINK_1055_T glink_1055;
extern LINK_1056_T glink_1056;
extern LINK_1057_T glink_1057;
extern LINK_1058_T glink_1058;
extern LINK_1059_T glink_1059;
extern LINK_1060_T glink_1060;
extern LINK_1061_T glink_1061;
extern LINK_1062_T glink_1062;
extern LINK_1063_T glink_1063;
extern LINK_1064_T glink_1064;
extern LINK_1065_T glink_1065;
extern LINK_1066_T glink_1066;
extern LINK_1067_T glink_1067;
extern LINK_1068_T glink_1068;
extern LINK_1069_T glink_1069;
extern LINK_1070_T glink_1070;
extern LINK_1071_T glink_1071;
extern LINK_1072_T glink_1072;
extern LINK_1073_T glink_1073;
extern LINK_1074_T glink_1074;
extern LINK_1075_T glink_1075;
extern LINK_1076_T glink_1076;
extern LINK_1077_T glink_1077;
extern LINK_1078_T glink_1078;
extern LINK_1079_T glink_1079;
extern LINK_1080_T glink_1080;
extern LINK_1081_T glink_1081;
extern LINK_1082_T glink_1082;
extern LINK_1083_T glink_1083;
extern LINK_1084_T glink_1084;
extern LINK_1085_T glink_1085;
extern LINK_1086_T glink_1086;
extern LINK_1087_T glink_1087;
extern LINK_1088_T glink_1088;
extern LINK_1089_T glink_1089;
extern LINK_1090_T glink_1090;
extern LINK_1091_T glink_1091;
extern LINK_1092_T glink_1092;
extern LINK_1093_T glink_1093;
extern LINK_1094_T glink_1094;
extern LINK_1095_T glink_1095;
extern LINK_1096_T glink_1096;
extern LINK_1097_T glink_1097;
extern LINK_1098_T glink_1098;
extern LINK_1099_T glink_1099;
extern LINK_1100_T glink_1100;
extern LINK_1101_T glink_1101;
extern LINK_1102_T glink_1102;
extern LINK_1103_T glink_1103;
extern LINK_1104_T glink_1104;
extern LINK_1105_T glink_1105;
extern LINK_1106_T glink_1106;
extern L9A_3CH_INTR_0_T gl9a_3ch_intr_0;
extern L9A_3CH_INTR_1_T gl9a_3ch_intr_1;
extern L9A_3CH_INTR_EN_0_T gl9a_3ch_intr_en_0;
extern L9A_3CH_INTR_EN_1_T gl9a_3ch_intr_en_1;
extern LLPLL_0_T gllpll_0;
extern LLPLL_1_T gllpll_1;
extern LLPLL_2_T gllpll_2;
extern LLPLL_3_T gllpll_3;
extern LLPLL_4_T gllpll_4;
extern LLPLL_5_T gllpll_5;
extern LLPLL_6_T gllpll_6;
extern LLPLL_7_T gllpll_7;
extern LLPLL_8_T gllpll_8;
extern LLPLL_9_T gllpll_9;
extern LLPLL_10_T gllpll_10;
extern LLPLL_11_T gllpll_11;
extern LLPLL_12_T gllpll_12;
extern LLPLL_13_T gllpll_13;
extern LLPLL_14_T gllpll_14;
extern LLPLL_15_T gllpll_15;
extern LLPLL_16_T gllpll_16;
extern LLPLL_17_T gllpll_17;
extern LLPLL_18_T gllpll_18;
extern LLPLL_19_T gllpll_19;
extern LLPLL_20_T gllpll_20;
extern LLPLL_21_T gllpll_21;
extern LLPLL_22_T gllpll_22;
extern LLPLL_23_T gllpll_23;
extern LLPLL_24_T gllpll_24;
extern LLPLL_25_T gllpll_25;
extern LLPLL_26_T gllpll_26;
extern LLPLL_27_T gllpll_27;
extern AFE_3CH_0_T gafe_3ch_0;
extern AFE_3CH_1_T gafe_3ch_1;
extern AFE_3CH_2_T gafe_3ch_2;
extern AFE_3CH_3_T gafe_3ch_3;
extern AFE_3CH_4_T gafe_3ch_4;
extern AFE_3CH_5_T gafe_3ch_5;
extern AFE_3CH_6_T gafe_3ch_6;
extern AFE_3CH_7_T gafe_3ch_7;
extern ADC_DIG_0_T gadc_dig_0;
extern ADC_DIG_1_T gadc_dig_1;
extern ADC_DIG_2_T gadc_dig_2;
extern ADC_DIG_3_T gadc_dig_3;
extern ADC_DIG_4_T gadc_dig_4;
extern ADC_DIG_5_T gadc_dig_5;
extern ADC_DIG_6_T gadc_dig_6;
extern ADC_DIG_7_T gadc_dig_7;
extern ADC_DIG_8_T gadc_dig_8;
extern ADC_DIG_9_T gadc_dig_9;
extern ADC_DIG_10_T gadc_dig_10;
extern ADC_DIG_11_T gadc_dig_11;
extern ADC_DIG_12_T gadc_dig_12;
extern ADC_DIG_13_T gadc_dig_13;
extern ADC_DIG_14_T gadc_dig_14;
extern ADC_DIG_15_T gadc_dig_15;
extern ADC_DIG_16_T gadc_dig_16;
extern ADC_DIG_17_T gadc_dig_17;
extern ADC_DIG_18_T gadc_dig_18;
extern ADC_DIG_19_T gadc_dig_19;
extern ADC_DIG_20_T gadc_dig_20;
extern ADC_DIG_21_T gadc_dig_21;
extern ADC_DIG_22_T gadc_dig_22;
extern ADC_DIG_23_T gadc_dig_23;
extern ADC_DIG_24_T gadc_dig_24;
extern ADC_DIG_25_T gadc_dig_25;
extern ADC_DIG_26_T gadc_dig_26;
extern ADC_DIG_27_T gadc_dig_27;
extern ADC_DIG_28_T gadc_dig_28;
extern ADC_DIG_29_T gadc_dig_29;
extern ADC_DIG_30_T gadc_dig_30;
extern ADC_DIG_31_T gadc_dig_31;
extern ADC_DIG_32_T gadc_dig_32;
extern ADC_DIG_33_T gadc_dig_33;
extern ADC_DIG_34_T gadc_dig_34;
extern ADC_DIG_35_T gadc_dig_35;
extern ADC_DIG_36_T gadc_dig_36;
extern ADC_DIG_37_T gadc_dig_37;
extern ADC_DIG_38_T gadc_dig_38;
extern ADC_DIG_39_T gadc_dig_39;
extern ADC_DIG_40_T gadc_dig_40;
extern ADC_DIG_41_T gadc_dig_41;
extern ADC_DIG_42_T gadc_dig_42;
extern ADC_DIG_43_T gadc_dig_43;
extern ADC_DIG_44_T gadc_dig_44;
extern ADC_DIG_45_T gadc_dig_45;
extern ADC_DIG_46_T gadc_dig_46;
extern ADC_DIG_47_T gadc_dig_47;
extern ADC_DIG_48_T gadc_dig_48;
extern ADC_DIG_49_T gadc_dig_49;
extern ADC_DIG_50_T gadc_dig_50;
extern ADC_DIG_51_T gadc_dig_51;
extern ADC_DIG_52_T gadc_dig_52;
extern ADC_DIG_53_T gadc_dig_53;
extern ADC_DIG_54_T gadc_dig_54;
extern ADC_DIG_55_T gadc_dig_55;
extern ADC_DIG_56_T gadc_dig_56;
extern ADC_DIG_57_T gadc_dig_57;
extern ADC_DIG_58_T gadc_dig_58;
extern ADC_DIG_59_T gadc_dig_59;
extern ADC_DIG_60_T gadc_dig_60;
extern ADC_DIG_61_T gadc_dig_61;
extern ADC_DIG_62_T gadc_dig_62;
extern ADC_DIG_63_T gadc_dig_63;
extern ADC_DIG_64_T gadc_dig_64;
extern ADC_DIG_65_T gadc_dig_65;
extern ADC_DIG_66_T gadc_dig_66;
extern ADC_DIG_67_T gadc_dig_67;
extern ADC_DIG_68_T gadc_dig_68;
extern ADC_DIG_69_T gadc_dig_69;
extern ADC_DIG_70_T gadc_dig_70;
extern ADC_DIG_71_T gadc_dig_71;
extern ADC_DIG_72_T gadc_dig_72;
extern ADC_DIG_73_T gadc_dig_73;
extern ADC_DIG_74_T gadc_dig_74;
extern ADC_DIG_75_T gadc_dig_75;
extern ADC_DIG_76_T gadc_dig_76;
extern ADC_DIG_77_T gadc_dig_77;
extern ADC_DIG_78_T gadc_dig_78;
extern ADC_DIG_79_T gadc_dig_79;
extern ADC_DIG_80_T gadc_dig_80;
extern ADC_DIG_81_T gadc_dig_81;
extern ADC_DIG_82_T gadc_dig_82;
extern ADC_DIG_83_T gadc_dig_83;
extern ADC_DIG_84_T gadc_dig_84;
extern ADC_DIG_85_T gadc_dig_85;
extern ADC_DIG_86_T gadc_dig_86;
extern ADC_DIG_87_T gadc_dig_87;
extern ADC_DIG_88_T gadc_dig_88;
extern ADC_DIG_89_T gadc_dig_89;
extern ADC_DIG_90_T gadc_dig_90;
extern ADC_DIG_91_T gadc_dig_91;
extern ADC_DIG_92_T gadc_dig_92;
extern ADC_DIG_93_T gadc_dig_93;
extern ADC_DIG_94_T gadc_dig_94;
extern ADC_DIG_95_T gadc_dig_95;
extern ADC_DIG_96_T gadc_dig_96;
extern ADC_DIG_97_T gadc_dig_97;
extern ADC_DIG_98_T gadc_dig_98;
extern ADC_DIG_99_T gadc_dig_99;
extern ADC_DIG_100_T gadc_dig_100;
extern ADC_DIG_101_T gadc_dig_101;
extern ADC_DIG_102_T gadc_dig_102;
extern ADC_DIG_103_T gadc_dig_103;
extern ADC_DIG_104_T gadc_dig_104;
extern ADC_DIG_105_T gadc_dig_105;
extern ADC_DIG_106_T gadc_dig_106;
extern ADC_DIG_107_T gadc_dig_107;
extern ADC_DIG_108_T gadc_dig_108;
extern ADC_DIG_109_T gadc_dig_109;
extern ADC_DIG_110_T gadc_dig_110;
extern ADC_DIG_111_T gadc_dig_111;
extern ADC_DIG_112_T gadc_dig_112;
extern ADC_DIG_113_T gadc_dig_113;
extern ADC_DIG_114_T gadc_dig_114;
extern ADC_DIG_115_T gadc_dig_115;
extern ADC_DIG_116_T gadc_dig_116;
extern ADC_DIG_117_T gadc_dig_117;
extern ADC_DIG_118_T gadc_dig_118;
extern ADC_DIG_119_T gadc_dig_119;
extern ADC_DIG_120_T gadc_dig_120;
extern ADC_DIG_121_T gadc_dig_121;
extern ADC_DIG_122_T gadc_dig_122;
extern ADC_DIG_123_T gadc_dig_123;
extern ADC_DIG_124_T gadc_dig_124;
extern ADC_DIG_125_T gadc_dig_125;
extern ADC_DIG_126_T gadc_dig_126;
extern ADC_DIG_127_T gadc_dig_127;
extern ADC_DIG_128_T gadc_dig_128;
extern ADC_DIG_129_T gadc_dig_129;
extern ADC_DIG_130_T gadc_dig_130;
extern ADC_DIG_131_T gadc_dig_131;
extern ADC_DIG_132_T gadc_dig_132;
extern ADC_DIG_133_T gadc_dig_133;
extern ADC_DIG_134_T gadc_dig_134;
extern ADC_DIG_135_T gadc_dig_135;
extern ADC_DIG_136_T gadc_dig_136;
extern ADC_DIG_137_T gadc_dig_137;
extern ADC_DIG_138_T gadc_dig_138;
extern ADC_DIG_139_T gadc_dig_139;
extern ADC_DIG_140_T gadc_dig_140;
extern ADC_DIG_141_T gadc_dig_141;
extern ADC_DIG_142_T gadc_dig_142;
extern ADC_DIG_143_T gadc_dig_143;
extern ADC_DIG_144_T gadc_dig_144;
extern ADC_DIG_145_T gadc_dig_145;
extern ADC_DIG_146_T gadc_dig_146;
extern ADC_DIG_147_T gadc_dig_147;
extern ADC_DIG_148_T gadc_dig_148;
extern ADC_DIG_149_T gadc_dig_149;
extern ADC_DIG_150_T gadc_dig_150;
extern ADC_DIG_151_T gadc_dig_151;
extern ADC_DIG_152_T gadc_dig_152;
extern ADC_DIG_153_T gadc_dig_153;
extern ADC_DIG_154_T gadc_dig_154;
extern ADC_DIG_155_T gadc_dig_155;
extern ADC_DIG_156_T gadc_dig_156;
extern ADC_DIG_157_T gadc_dig_157;
extern ADC_DIG_158_T gadc_dig_158;
extern ADC_DIG_159_T gadc_dig_159;
extern ADC_DIG_160_T gadc_dig_160;
extern ADC_DIG_161_T gadc_dig_161;
extern ADC_DIG_162_T gadc_dig_162;
extern ADC_DIG_163_T gadc_dig_163;
extern ADC_DIG_164_T gadc_dig_164;
extern ADC_DIG_165_T gadc_dig_165;
extern ADC_DIG_166_T gadc_dig_166;
extern ADC_DIG_167_T gadc_dig_167;
extern ADC_DIG_168_T gadc_dig_168;
extern ADC_DIG_169_T gadc_dig_169;
extern ADC_DIG_170_T gadc_dig_170;
extern ADC_DIG_171_T gadc_dig_171;
extern ADC_DIG_172_T gadc_dig_172;
extern ADC_DIG_173_T gadc_dig_173;
extern ADC_DIG_174_T gadc_dig_174;
extern ADC_DIG_175_T gadc_dig_175;
extern ADC_DIG_176_T gadc_dig_176;
extern ADC_DIG_177_T gadc_dig_177;
extern ADC_DIG_178_T gadc_dig_178;
extern ADC_DIG_179_T gadc_dig_179;
extern ADC_DIG_180_T gadc_dig_180;
extern ADC_DIG_181_T gadc_dig_181;
extern ADC_DIG_182_T gadc_dig_182;
extern ADC_DIG_183_T gadc_dig_183;
extern ADC_DIG_184_T gadc_dig_184;
extern ADC_DIG_185_T gadc_dig_185;
extern ADC_DIG_186_T gadc_dig_186;
extern ADC_DIG_187_T gadc_dig_187;
extern ADC_DIG_188_T gadc_dig_188;
extern ADC_DIG_189_T gadc_dig_189;
extern ADC_DIG_190_T gadc_dig_190;
extern ADC_DIG_191_T gadc_dig_191;
extern ADC_DIG_192_T gadc_dig_192;
extern ADC_DIG_193_T gadc_dig_193;
extern ADC_DIG_194_T gadc_dig_194;
extern ADC_DIG_195_T gadc_dig_195;
extern ADC_DIG_196_T gadc_dig_196;
extern ADC_DIG_197_T gadc_dig_197;
extern ADC_DIG_198_T gadc_dig_198;
extern ADC_DIG_199_T gadc_dig_199;
extern ADC_DIG_200_T gadc_dig_200;
extern ADC_DIG_201_T gadc_dig_201;
extern ADC_DIG_202_T gadc_dig_202;
extern ADC_DIG_203_T gadc_dig_203;
extern ADC_DIG_204_T gadc_dig_204;
extern ADC_DIG_205_T gadc_dig_205;
extern ADC_DIG_206_T gadc_dig_206;
extern ADC_DIG_207_T gadc_dig_207;
extern ADC_DIG_208_T gadc_dig_208;
extern ADC_DIG_209_T gadc_dig_209;
extern ADC_DIG_210_T gadc_dig_210;
extern ADC_DIG_211_T gadc_dig_211;
extern ADC_DIG_212_T gadc_dig_212;
extern ADC_DIG_213_T gadc_dig_213;
extern ADC_DIG_214_T gadc_dig_214;
extern ADC_DIG_215_T gadc_dig_215;
extern ADC_DIG_216_T gadc_dig_216;
extern ADC_DIG_217_T gadc_dig_217;
extern ADC_DIG_218_T gadc_dig_218;
extern ADC_DIG_219_T gadc_dig_219;
extern ADC_DIG_220_T gadc_dig_220;
extern ADC_DIG_221_T gadc_dig_221;
extern ADC_DIG_222_T gadc_dig_222;
extern ADC_DIG_223_T gadc_dig_223;
extern ADC_DIG_224_T gadc_dig_224;
extern ADC_DIG_225_T gadc_dig_225;
extern ADC_DIG_226_T gadc_dig_226;
extern ADC_DIG_227_T gadc_dig_227;
extern ADC_DIG_228_T gadc_dig_228;
extern ADC_DIG_229_T gadc_dig_229;
extern AUDIO_PLL_0_T gaudio_pll_0;
extern AUDIO_PLL_1_T gaudio_pll_1;
extern AUDIO_PLL_2_T gaudio_pll_2;
extern AUDIO_PLL_3_T gaudio_pll_3;
extern AUDIO_PLL_4_T gaudio_pll_4;
extern AUDIO_PLL_5_T gaudio_pll_5;
extern AUDIO_PLL_6_T gaudio_pll_6;
extern AFE_AAD_0_T gafe_aad_0;
extern AFE_AAD_1_T gafe_aad_1;
extern AFE_AAD_2_T gafe_aad_2;
extern AFE_AAD_3_T gafe_aad_3;
extern AFE_AAD_4_T gafe_aad_4;
extern AFE_AAD_5_T gafe_aad_5;
extern AFE_AAD_6_T gafe_aad_6;
extern AFE_AAD_7_T gafe_aad_7;
extern AFE_AAD_8_T gafe_aad_8;
extern AFE_ACODEC_0_T gafe_acodec_0;
extern AFE_ACODEC_1_T gafe_acodec_1;
extern AFE_ACODEC_2_T gafe_acodec_2;
extern AFE_ACODEC_3_T gafe_acodec_3;
extern AFE_ACODEC_4_T gafe_acodec_4;
extern AFE_ACODEC_5_T gafe_acodec_5;
extern AFE_HP_DRV_0_T gafe_hp_drv_0;
extern AFE_HP_DRV_1_T gafe_hp_drv_1;
extern AFE_HP_DRV_2_T gafe_hp_drv_2;
extern AFE_HP_DRV_3_T gafe_hp_drv_3;
extern AFE_HP_DRV_4_T gafe_hp_drv_4;
extern ACODEC_0_T gacodec_0;
extern ACODEC_1_T gacodec_1;
extern ACODEC_2_T gacodec_2;
extern ACODEC_3_T gacodec_3;
extern ACODEC_4_T gacodec_4;
extern ACODEC_5_T gacodec_5;
extern ACODEC_6_T gacodec_6;
extern ACODEC_7_T gacodec_7;
extern ACODEC_8_T gacodec_8;
extern ACODEC_9_T gacodec_9;
extern ACODEC_10_T gacodec_10;
extern ACODEC_11_T gacodec_11;
extern ACODEC_12_T gacodec_12;
extern ACODEC_13_T gacodec_13;
extern ACODEC_14_T gacodec_14;
extern ACODEC_15_T gacodec_15;
extern ACODEC_16_T gacodec_16;
extern ACODEC_17_T gacodec_17;
extern ACODEC_18_T gacodec_18;
extern ACODEC_19_T gacodec_19;
extern ACODEC_20_T gacodec_20;
extern ACODEC_21_T gacodec_21;
extern ACODEC_22_T gacodec_22;
extern ACODEC_23_T gacodec_23;
extern ACODEC_24_T gacodec_24;
extern ACODEC_25_T gacodec_25;
extern ACODEC_26_T gacodec_26;
extern ACODEC_27_T gacodec_27;
extern ACODEC_28_T gacodec_28;
extern ACODEC_29_T gacodec_29;
extern ACODEC_30_T gacodec_30;
extern ACODEC_31_T gacodec_31;
extern ACODEC_32_T gacodec_32;
extern ACODEC_33_T gacodec_33;
extern ACODEC_34_T gacodec_34;
extern ACODEC_35_T gacodec_35;
extern ACODEC_36_T gacodec_36;
extern ACODEC_37_T gacodec_37;
extern ACODEC_38_T gacodec_38;
extern ACODEC_39_T gacodec_39;
extern ACODEC_40_T gacodec_40;
extern ACODEC_41_T gacodec_41;
extern ACODEC_42_T gacodec_42;
extern ACODEC_43_T gacodec_43;
extern ACODEC_44_T gacodec_44;
extern ACODEC_45_T gacodec_45;
extern ACODEC_46_T gacodec_46;
extern ACODEC_47_T gacodec_47;
extern HDMI_PHY_N_01_T ghdmi_phy_n_01;
extern HDMI_PHY_N_02_T ghdmi_phy_n_02;
extern HDMI_PHY_N_03_T ghdmi_phy_n_03;
extern HDMI_PHY_N_04_T ghdmi_phy_n_04;
extern HDMI_PHY_N_05_T ghdmi_phy_n_05;
extern HDMI_PHY_N_06_T ghdmi_phy_n_06;
extern HDMI_PHY_N_07_T ghdmi_phy_n_07;
extern HDMI_PHY_N_08_T ghdmi_phy_n_08;
extern HDMI_PHY_N_09_T ghdmi_phy_n_09;
extern HDMI_PHY_N_0A_T ghdmi_phy_n_0a;
extern HDMI_PHY_N_10_T ghdmi_phy_n_10;
extern HDMI_PHY_N_11_T ghdmi_phy_n_11;
extern HDMI_PHY_N_12_T ghdmi_phy_n_12;
extern HDMI_PHY_N_13_T ghdmi_phy_n_13;
extern HDMI_PHY_N_14_T ghdmi_phy_n_14;
extern HDMI_PHY_N_15_T ghdmi_phy_n_15;
extern HDMI_PHY_N_16_T ghdmi_phy_n_16;
extern HDMI_PHY_N_17_T ghdmi_phy_n_17;
extern HDMI_PHY_N_18_T ghdmi_phy_n_18;
extern HDMI_PHY_N_19_T ghdmi_phy_n_19;
extern HDMI_PHY_N_1A_T ghdmi_phy_n_1a;
extern HDMI_PHY_N_1B_T ghdmi_phy_n_1b;
extern HDMI_PHY_N_1C_T ghdmi_phy_n_1c;
extern HDMI_PHY_N_1D_T ghdmi_phy_n_1d;
extern HDMI_PHY_N_1E_T ghdmi_phy_n_1e;
extern HDMI_PHY_N_1F_T ghdmi_phy_n_1f;
extern HDMI_PHY_N_20_T ghdmi_phy_n_20;
extern HDMI_PHY_N_21_T ghdmi_phy_n_21;
extern HDMI_PHY_N_22_T ghdmi_phy_n_22;
extern HDMI_PHY_N_23_T ghdmi_phy_n_23;
extern HDMI_PHY_N_24_T ghdmi_phy_n_24;
extern HDMI_PHY_N_25_T ghdmi_phy_n_25;
extern HDMI_PHY_N_26_T ghdmi_phy_n_26;
extern HDMI_PHY_N_30_T ghdmi_phy_n_30;
extern HDMI_PHY_N_31_T ghdmi_phy_n_31;
extern HDMI_PHY_N_32_T ghdmi_phy_n_32;
extern HDMI_PHY_N_33_T ghdmi_phy_n_33;
extern HDMI_PHY_N_34_T ghdmi_phy_n_34;
extern HDMI_PHY_N_35_T ghdmi_phy_n_35;
extern HDMI_PHY_N_36_T ghdmi_phy_n_36;
extern HDMI_PHY_N_40_T ghdmi_phy_n_40;
extern HDMI_PHY_N_41_T ghdmi_phy_n_41;
extern HDMI_PHY_N_42_T ghdmi_phy_n_42;
extern HDMI_PHY_N_43_T ghdmi_phy_n_43;
extern HDMI_PHY_N_44_T ghdmi_phy_n_44;
extern HDMI_PHY_N_45_T ghdmi_phy_n_45;
extern HDMI_PHY_N_46_T ghdmi_phy_n_46;
extern HDMI_PHY_N_47_T ghdmi_phy_n_47;
extern HDMI_PHY_N_48_T ghdmi_phy_n_48;
extern HDMI_PHY_N_49_T ghdmi_phy_n_49;
extern HDMI_PHY_N_4A_T ghdmi_phy_n_4a;
extern HDMI_PHY_N_4B_T ghdmi_phy_n_4b;
extern HDMI_PHY_N_4C_T ghdmi_phy_n_4c;
extern HDMI_PHY_N_4D_T ghdmi_phy_n_4d;
extern HDMI_PHY_N_4E_T ghdmi_phy_n_4e;
extern HDMI_PHY_N_4F_T ghdmi_phy_n_4f;
extern HDMI_PHY_N_50_T ghdmi_phy_n_50;
extern HDMI_PHY_N_51_T ghdmi_phy_n_51;
extern HDMI_PHY_N_52_T ghdmi_phy_n_52;
extern HDMI_PHY_N_53_T ghdmi_phy_n_53;
extern HDMI_PHY_N_54_T ghdmi_phy_n_54;
extern HDMI_PHY_N_80_T ghdmi_phy_n_80;
extern HDMI_PHY_N_81_T ghdmi_phy_n_81;
extern HDMI_PHY_N_82_T ghdmi_phy_n_82;
extern HDMI_PHY_N_83_T ghdmi_phy_n_83;
extern HDMI_PHY_N_84_T ghdmi_phy_n_84;
extern HDMI_PHY_N_85_T ghdmi_phy_n_85;
extern HDMI_PHY_N_86_T ghdmi_phy_n_86;
extern HDMI_PHY_N_87_T ghdmi_phy_n_87;
extern HDMI_PHY_N_88_T ghdmi_phy_n_88;
extern HDMI_PHY_N_89_T ghdmi_phy_n_89;
extern HDMI_PHY_N_8A_T ghdmi_phy_n_8a;
extern HDMI_PHY_N_8B_T ghdmi_phy_n_8b;
extern HDMI_PHY_N_8C_T ghdmi_phy_n_8c;
extern HDMI_PHY_N_8D_T ghdmi_phy_n_8d;
extern HDMI_PHY_N_8E_T ghdmi_phy_n_8e;
extern HDMI_PHY_N_8F_T ghdmi_phy_n_8f;
extern HDMI_PHY_N_90_T ghdmi_phy_n_90;
extern HDMI_PHY_N_91_T ghdmi_phy_n_91;
extern HDMI_PHY_N_92_T ghdmi_phy_n_92;
extern HDMI_PHY_N_93_T ghdmi_phy_n_93;
extern HDMI_PHY_N_A0_T ghdmi_phy_n_a0;
extern HDMI_PHY_N_A1_T ghdmi_phy_n_a1;
extern HDMI_PHY_N_A2_T ghdmi_phy_n_a2;
extern HDMI_PHY_N_A3_T ghdmi_phy_n_a3;
extern HDMI_PHY_N_A4_T ghdmi_phy_n_a4;
extern HDMI_PHY_N_A5_T ghdmi_phy_n_a5;
extern HDMI_PHY_N_A6_T ghdmi_phy_n_a6;
extern HDMI_PHY_N_A7_T ghdmi_phy_n_a7;
extern HDMI_PHY_N_A8_T ghdmi_phy_n_a8;
extern HDMI_PHY_N_A9_T ghdmi_phy_n_a9;
extern HDMI_PHY_N_AA_T ghdmi_phy_n_aa;

//JUHEE :
//1. Move the following structure to Header file
//2. Change param of the VPORT_I2C_R/W to the following structure.
typedef struct
{
	UINT32
	data:8,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} VPORT_I2C_DATA_T;

UINT32 VPORT_I2C_Read(UINT32* reg);
UINT32 VPORT_I2C_Write(UINT32* reg);

#ifdef __cplusplus
}
#endif

#endif	/* _VPORT_I2C_L9A0_H_ */

/** @} *//* end of macro documentation */

