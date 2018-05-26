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
 *  author	   won.hur
 *  version    1.0
 *  date       2011.08.16
 *
 */

#ifndef _VPORT_I2C_L9B0_H_
#define _VPORT_I2C_L9B0_H_

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
} L9A_VERSION_0_Bx_T;

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
} HOSTIF_CONTROL_0_Bx_T;

typedef struct
{
	UINT32
	intr_hostif_is_162mhz:1,		//0,0
	intr_hostif_is_24mhz:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HOSTIF_INTR_0_Bx_T;

typedef struct
{
	UINT32
	enable_intr_hostif_is_162mhz:1,		//0,0
	enable_intr_hostif_is_24mhz:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HOSTIF_INTR_EN_0_Bx_T;

typedef struct
{
	UINT32
	ro_f324m_not_osc:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} TOP_CONTROL_0_Bx_T;

typedef struct
{
	UINT32
	ro_f324m_135_xtal_cnt:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} TOP_CONTROL_1_Bx_T;

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
} TOP_CONTROL_2_Bx_T;

typedef struct
{
	UINT32
	swrst_pix:1,		//0,0
	swrst_f24m:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_0_Bx_T;

typedef struct
{
	UINT32
	swrst_link_prt0_tmds:1,		//0,0
	swrst_hdcp0:1,		//1,1
	swrst_f3m:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_1_Bx_T;

typedef struct
{
	UINT32
	swrst_link_pip0_pix:1,		//0,0
	swrst_aw0:1,		//1,1
	swrst_vw0:1,		//2,2
	swrst_link_pip0_tmds:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_2_Bx_T;

typedef struct
{
	UINT32
	swrst_ar0:1,		//0,0
	swrst_link_pip0_aud:1,		//1,1
	swrst_vr0:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_3_Bx_T;

typedef struct
{
	UINT32
	swrst_mic:1,		//0,0
	swrst_auad:1,		//1,1
	swrst_auda_mclk1:1,		//2,2
	swrst_auda_mclk0:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_4_Bx_T;

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
} SOFT_RESET_5_Bx_T;

typedef struct
{
	UINT32
	swrst_half_ultra:1,		//0,0
	swrst_ultra:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_6_Bx_T;

typedef struct
{
	UINT32
	swrst_hs1_lvds:1,		//0,0
	swrst_hs0_lvds:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_7_Bx_T;

typedef struct
{
	UINT32
	swrst_aclk_mux2:1,		//0,0
	swrst_hdmi_dto0:1,		//1,1
	swrst_adto:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_8_Bx_T;

typedef struct
{
	UINT32
	swrst_audclk_out_sub:1,		//0,0
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
} SOFT_RESET_9_Bx_T;

typedef struct
{
	UINT32
	aclk_mux1_src:3,		//2,0
	swrst_f256fs_src:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_10_Bx_T;

typedef struct
{
	UINT32
	aclk_mux2_src:3,		//2,0
	aclk_mux1_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_11_Bx_T;

typedef struct
{
	UINT32
	aclk_mux3_src:3,		//2,0
	aclk_mux2_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_12_Bx_T;

typedef struct
{
	UINT32
	aclk_mux4_src:3,		//2,0
	aclk_mux3_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_13_Bx_T;

typedef struct
{
	UINT32
	aclk_mux5_src:3,		//2,0
	aclk_mux4_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_14_Bx_T;

typedef struct
{
	UINT32
	aclk_mux6_src:3,		//2,0
	aclk_mux5_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_15_Bx_T;

typedef struct
{
	UINT32
	aclk_mux7_src:3,		//2,0
	aclk_mux6_div:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_16_Bx_T;

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
} SOFT_RESET_17_Bx_T;

typedef struct
{
	UINT32
	sel_inv_vdac_clock:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} CLOCK_CONTROL_0_Bx_T;

typedef struct
{
	UINT32
	sel_inv_chb_clock:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} CLOCK_CONTROL_1_Bx_T;

typedef struct
{
	UINT32
	sel_inv_f54m_clock:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} CLOCK_CONTROL_2_Bx_T;

typedef struct
{
	UINT32
	reg_adto_loop_value_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_0_Bx_T;

typedef struct
{
	UINT32
	reg_adto_loop_value_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_1_Bx_T;

typedef struct
{
	UINT32
	reg_adto_loop_value_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_2_Bx_T;

typedef struct
{
	UINT32
	reg_adto_add_value_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_3_Bx_T;

typedef struct
{
	UINT32
	reg_adto_add_value_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_4_Bx_T;

typedef struct
{
	UINT32
	reg_adto_add_value_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_5_Bx_T;

typedef struct
{
	UINT32
	reg_adto_add_value_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_6_Bx_T;

typedef struct
{
	UINT32
	reg_adto_err_value_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_7_Bx_T;

typedef struct
{
	UINT32
	reg_adto_err_value_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_DTO_8_Bx_T;

typedef struct
{
	UINT32
	reg_cvbs_clock_rate:1,		//0,0
	reg_cvbs_out_disable:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_0_Bx_T;

typedef struct
{
	UINT32
	reg_cvbs_c_delay:3,		//2,0
	reg_cvbs_y_delay:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_1_Bx_T;

typedef struct
{
	UINT32
	reg_ud_autoset:1,		//0,0
	reg_3dfr_autoset:1,		//1,1
	reg_3dfr_bridge_mode:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_2_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_fp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_3_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_fp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_4_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_sync_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_5_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_sync_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_6_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_bp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_7_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_bp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_8_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_active_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_9_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_h_active_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_10_Bx_T;

typedef struct
{
	UINT32
	reg_half_v_fp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_11_Bx_T;

typedef struct
{
	UINT32
	reg_half_v_fp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_12_Bx_T;

typedef struct
{
	UINT32
	reg_half_v_sync_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_13_Bx_T;

typedef struct
{
	UINT32
	reg_half_v_sync_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_14_Bx_T;

typedef struct
{
	UINT32
	reg_half_v_bp_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_15_Bx_T;

typedef struct
{
	UINT32
	reg_half_v_bp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_16_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_v_active_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_17_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_v_active_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_18_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_3dfr_v_aspace_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_19_Bx_T;

typedef struct
{
	UINT32
	reg_ultra_3dfr_v_aspace_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_20_Bx_T;

typedef struct
{
	UINT32
	aclk_mux9_div:2,		//1,0
	aclk_mux9_src:3,		//4,2
	swrst_auda_f256fs_clk1:1,		//5,5
	swrst_auda_f256fs_clk0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_18_Bx_T;

typedef struct
{
	UINT32
	aclk_mux10_div:2,		//1,0
	aclk_mux10_src:3,		//4,2
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} SOFT_RESET_19_Bx_T;

typedef struct
{
	UINT32
//	reg_lvds0_sel:4,		//3,0
	reg_lvds0_src:3,			//2,0
	reg_lvds0_type:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_21_Bx_T;

typedef struct
{
	UINT32
	//reg_lvds1_sel:4,		//3,0
	reg_lvds1_src:3,			//2,0
	reg_lvds1_type:1,		//3,3
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_22_Bx_T;

typedef struct
{
	UINT32
	reg_scart_mix_mode:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_23_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef0_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_24_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_25_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef1_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_26_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_27_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef2_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_28_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef2_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_29_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef3_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_30_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef3_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_31_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef4_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_32_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef4_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_33_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef5_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_34_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef5_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_35_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef6_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_36_Bx_T;

typedef struct
{
	UINT32
	reg_fir_y_coef6_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_37_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef0_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_38_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_39_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef1_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_40_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef1_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_41_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef2_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_42_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef2_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_43_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef3_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_44_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef3_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_45_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef4_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_46_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef4_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_47_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef5_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_48_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef5_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_49_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef6_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_50_Bx_T;

typedef struct
{
	UINT32
	reg_fir_c_coef6_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} DATA_BRIDGE_51_Bx_T;

typedef struct
{
	UINT32
	fs00_clk_out:1,		//0,0
	aud_fs20clk:1,		//1,1
	aud_fs21clk:1,		//2,2
	aud_fs23clk:1,		//3,3
	aud_fs24clk:1,		//4,4
	aud_fs25clk:1,		//5,5
	audclk_out_sub:1,	//6,6
	audclk_out:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUD_CLOCK_INV_0_Bx_T;

typedef struct
{
	UINT32
	pdb1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_0_Bx_T;

typedef struct
{
	UINT32
	ch_en1:6,		//5,0
	flip_en1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_1_Bx_T;

typedef struct
{
	UINT32
	rs1:6,		//5,0
	rf1:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_2_Bx_T;

typedef struct
{
	UINT32
	enpemp1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_3_Bx_T;

typedef struct
{
	UINT32
	swc_t1b1:3,		//2,0
	swc_t1a1:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_4_Bx_T;

typedef struct
{
	UINT32
	swc_t1clk1:3,		//2,0
	swc_t1c1:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_5_Bx_T;

typedef struct
{
	UINT32
	swc_t1e1:3,		//2,0
	swc_t1d1:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX0_6_Bx_T;

typedef struct
{
	UINT32
	pdb2:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_0_Bx_T;

typedef struct
{
	UINT32
	ch_en2:6,		//5,0
	flip_en2:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_1_Bx_T;

typedef struct
{
	UINT32
	rs2:6,		//5,0
	rf2:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_2_Bx_T;

typedef struct
{
	UINT32
	enpemp2:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_3_Bx_T;

typedef struct
{
	UINT32
	swc_t2b2:3,		//2,0
	swc_t2a2:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_4_Bx_T;

typedef struct
{
	UINT32
	swc_t2clk2:3,		//2,0
	swc_t2c2:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_5_Bx_T;

typedef struct
{
	UINT32
	swc_t2e2:3,		//2,0
	swc_t2d2:3,		//5,3
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HSLVDSTX1_6_Bx_T;

typedef struct
{
	UINT32
	inck_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} GBB_AFE_0_Bx_T;

typedef struct
{
	UINT32
	i2csel:1,		//0,0
	reset:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} GBB_0_Bx_T;

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
} AFE_CVBS_0_Bx_T;

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
} AFE_CVBS_1_Bx_T;

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
} AFE_CVBS_2_Bx_T;

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
} AFE_CVBS_3_Bx_T;

typedef struct
{
	UINT32
	buf_pdb2:1,		//0,0
	buf_ycm:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_VBUF_0_Bx_T;

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
} AFE_VBUF_1_Bx_T;

typedef struct
{
	UINT32
	vdac_pdb:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_VDAC_0_Bx_T;

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
} AFE_VDAC_1_Bx_T;

typedef struct
{
	UINT32
	dr3p_nsc:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_0_Bx_T;

typedef struct
{
	UINT32
	dr3p_npc:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_1_Bx_T;

typedef struct
{
	UINT32
	dr3p_m:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_2_Bx_T;

typedef struct
{
	UINT32
	dr3p_od_cvdclk:3,		//2,0
	dr3p_od_recclk:4,		//6,3
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} MAIN_PLL_3_Bx_T;

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
} MAIN_PLL_4_Bx_T;

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
} AFE_CB_0_Bx_T;

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
} AFE_CB_1_Bx_T;

typedef struct
{
	UINT32
	hpd0_oen:1,		//0,0
	hpd0_out:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HPD_OUT_CONTROL_0_Bx_T;

typedef struct
{
	UINT32
	phy0_phy_arc_pdb:1,		//0,0
	phy0_phy_rstn:1,		//1,1
	phy0_phy_pdb:1,		//2,2
	phy_enable:1,		//3,3
	ddc_write_mode:2,		//5,4
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HPD_OUT_CONTROL_1_Bx_T;

typedef struct
{
	UINT32
	phy0_bert_run_ch2:1,		//0,0
	phy0_bert_run_ch1:1,		//1,1
	phy0_bert_run_ch0:1,		//2,2
	phy0_bert_ch_sel:2,		//4,3
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HPD_OUT_CONTROL_2_Bx_T;

typedef struct
{
	UINT32
	phy0_reg_bert_num_err_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY0_0_Bx_T;

typedef struct
{
	UINT32
	phy0_reg_bert_num_err_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY0_1_Bx_T;

typedef struct
{
	UINT32
	phy0_reg_bert_test_res:2,		//1,0
	phy0_reg_bert_sync_done:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY0_2_Bx_T;

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
} L9A_HDMI0_INTR_0_Bx_T;

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
} L9A_HDMI0_INTR_1_Bx_T;

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
} L9A_HDMI0_INTR_2_Bx_T;

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
} L9A_HDMI0_INTR_3_Bx_T;

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
} L9A_HDMI0_INTR_4_Bx_T;

typedef struct
{
	UINT32
	intr_chg_avi_b12345:1,		//0,0
	intr_vfifo_ovrr_pip0:1,		//1,1
	intr_vfifo_undr_pip0:1,		//2,2
	intr_il_chg_pip0:1,		//3,3
	intr_po_chg_pip0:1,		//4,4
	intr_hr_chg_pip0:1,		//5,5
	intr_vr_chg_pip0:1,		//6,6
	intr_vid_chg_pip0:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_5_Bx_T;

typedef struct
{
	UINT32
	intr_2npcm_chg_pip0:1,		//0,0
	intr_2pcm_chg_pip0:1,		//1,1
	intr_fs_chg_pip0:1,		//2,2
	intr_vf_stable:1,		//3,3
	intr_chg_vsi_vformat:1,		//4,4
	intr_chg_avi_to_ycc444:1,		//5,5
	intr_chg_avi_to_ycc422:1,		//6,6
	intr_chg_avi_to_rgb:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_6_Bx_T;

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
} L9A_HDMI0_INTR_EN_0_Bx_T;

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
} L9A_HDMI0_INTR_EN_1_Bx_T;

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
} L9A_HDMI0_INTR_EN_2_Bx_T;

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
} L9A_HDMI0_INTR_EN_3_Bx_T;

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
} L9A_HDMI0_INTR_EN_4_Bx_T;

typedef struct
{
	UINT32
	enable_intr_chg_avi_b12345:1,		//0,0
	enable_intr_vfifo_ovrr_pip0:1,		//1,1
	enable_intr_vfifo_undr_pip0:1,		//2,2
	enable_intr_il_chg_pip0:1,		//3,3
	enable_intr_po_chg_pip0:1,		//4,4
	enable_intr_hr_chg_pip0:1,		//5,5
	enable_intr_vr_chg_pip0:1,		//6,6
	enable_intr_vid_chg_pip0:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_5_Bx_T;

typedef struct
{
	UINT32
	enable_intr_2npcm_chg_pip0:1,		//0,0
	enable_intr_2pcm_chg_pip0:1,		//1,1
	enable_intr_fs_chg_pip0:1,		//2,2
	enable_intr_vf_stable:1,		//3,3
	enable_intr_chg_vsi_vformat:1,		//4,4
	enable_intr_chg_avi_to_ycc444:1,		//5,5
	enable_intr_chg_avi_to_ycc422:1,		//6,6
	enable_intr_chg_avi_to_rgb:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_HDMI0_INTR_EN_6_Bx_T;

typedef struct
{
	UINT32
	reg_arc_src_prt0:1,		//0,0
	reg_conn_prt0:1,		//1,1
	reg_scdt_prt0:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_0_Bx_T;

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
} LINK_1_Bx_T;

typedef struct
{
	UINT32
	reg_vfifo_pdiff_pip0:6,		//5,0
	reg_avc_en_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_2_Bx_T;

typedef struct
{
	UINT32
	reg_afifo_pdiff_pip0:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_3_Bx_T;

typedef struct
{
	UINT32
	reg_vfifo_ctrl_thr_pip0:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_4_Bx_T;

typedef struct
{
	UINT32
	reg_afifo_ctrl_thr_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_5_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_6_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_7_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_8_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_9_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_10_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_11_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_12_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_an_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_13_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_14_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_15_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_16_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_17_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_aksv_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_18_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_19_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_20_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_21_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_22_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_bksv_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_23_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_ri_prt0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_24_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_ri_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_25_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_enc_en_prt0:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_26_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_bcaps_prt0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_27_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_authed_prt0:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_28_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_29_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_30_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_31_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_32_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_33_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_34_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_35_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_36_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_37_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_38_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_39_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_40_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_41_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_42_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_43_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_44_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_45_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_46_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_47_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_48_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_49_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_50_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_51_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_52_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_53_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_54_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_55_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_56_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_57_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_acp_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_58_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_59_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_60_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_61_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_62_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_63_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_64_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_65_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_66_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_67_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_68_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_69_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_70_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_71_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_72_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_73_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_74_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_75_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_76_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_77_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_78_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_79_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_80_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_81_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_82_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_83_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_84_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_85_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_86_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_87_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_aif_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_88_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_asp_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_89_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_asp_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_90_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_91_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_92_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_93_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_94_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_95_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_96_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_97_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_98_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_99_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_100_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_101_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_102_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_103_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_104_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_105_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_106_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_107_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_108_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_109_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_110_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_111_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_112_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_113_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_114_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_115_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_116_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_117_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_118_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_119_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_avi_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_120_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_121_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_122_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_123_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_124_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_125_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_126_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_127_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_128_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_129_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_130_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_131_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_132_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_133_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_134_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_135_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_136_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_137_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_138_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_139_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_140_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_141_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_142_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_143_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_144_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_145_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_146_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_147_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_148_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_149_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gbd_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_150_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_gcp_cmute_pip0:1,		//0,0
	reg_pkt_gcp_smute_pip0:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_151_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_hbr_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_152_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_hbr_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_153_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_154_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_155_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_156_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_157_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_158_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_159_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_160_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_161_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_162_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_163_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_164_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_165_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_166_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_167_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_168_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_169_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_170_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_171_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_172_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_173_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_174_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_175_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_176_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_177_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_178_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_179_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_180_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_181_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_182_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc1_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_183_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_184_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_185_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_186_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_187_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_188_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_189_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_190_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_191_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_192_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_193_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_194_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_195_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_196_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_197_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_198_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_199_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_200_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_201_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_202_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_203_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_204_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_205_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_206_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_207_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_208_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_209_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_210_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_211_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_212_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_isrc2_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_213_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_214_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_215_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_216_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_217_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_218_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_219_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_220_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_221_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_222_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_223_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_224_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_225_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_226_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_227_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_228_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_229_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_230_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_231_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_232_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_233_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_234_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_235_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_236_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_237_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_238_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_239_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_240_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_241_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_242_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_mif_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_243_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_244_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_245_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_246_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_247_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_248_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_249_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_250_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_251_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_252_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_253_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_254_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_255_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_256_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_257_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_258_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_259_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_260_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_261_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_262_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_263_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_264_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_265_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_266_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_267_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_268_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_269_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_270_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_271_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_272_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_spd_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_273_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_30:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_274_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_29:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_275_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_28:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_276_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_277_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_278_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_279_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_280_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_281_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_282_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_283_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_284_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_285_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_286_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_287_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_288_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_289_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_290_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_291_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_292_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_293_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_294_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_295_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_296_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_297_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_298_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_299_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_300_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_301_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_302_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_303_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_unr_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_304_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_hdr_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_305_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_hdr_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_306_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_307_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_308_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_309_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_310_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_311_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_312_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_313_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_314_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_315_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_316_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_317_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_318_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_319_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_320_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_321_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_322_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_323_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_324_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_325_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_326_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_327_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_328_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_329_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_330_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_331_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_332_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_333_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_vsi_dat_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_334_Bx_T;

typedef struct
{
	UINT32
	info_vsi_3d_pip0:1,		//0,0
	info_vsi_4k2k_pip0:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_335_Bx_T;

typedef struct
{
	UINT32
	info_vsi_hdmi_vic_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_336_Bx_T;

typedef struct
{
	UINT32
	info_vsi_3d_struct_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_337_Bx_T;

typedef struct
{
	UINT32
	reg_pr_auto_pip0:1,		//0,0
	reg_pr_rx_pip0:4,		//4,1
	reg_pr_sel:1,		//5,5
	info_vsi_3d_meta_en_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_338_Bx_T;

typedef struct
{
	UINT32
	reg_pr_tx_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_339_Bx_T;

typedef struct
{
	UINT32
	reg_pr_stat_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_340_Bx_T;

typedef struct
{
	UINT32
	reg_cmode_ppll_sync_pip0:1,		//0,0
	reg_cmode_sel:1,		//1,1
	reg_align422_sel:1,		//2,2
	reg_align422_swap_rb_pip0:1,		//3,3
	reg_align422_stat_pip0:1,		//4,4
	reg_align422_tx_pip0:1,		//5,5
	reg_align422_rx_pip0:1,		//6,6
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_341_Bx_T;

typedef struct
{
	UINT32
	reg_cmode_rx_pip0:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_342_Bx_T;

typedef struct
{
	UINT32
	reg_cmode_tx_pip0:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_343_Bx_T;

typedef struct
{
	UINT32
	reg_packing_phase_pip0:4,	//3,0
	reg_default_phase_pip0:1,	//4,4
	reg_cmode_stat_pip0:2,		//6,5
	//reg_cmode_stat_pip0:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_344_Bx_T;

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
} LINK_345_Bx_T;

typedef struct
{
	UINT32
	reg_h_tot_pip0_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_346_Bx_T;

typedef struct
{
	UINT32
	reg_h_tot_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_347_Bx_T;

typedef struct
{
	UINT32
	reg_v_tot_pip0_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_348_Bx_T;

typedef struct
{
	UINT32
	reg_v_tot_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_349_Bx_T;

typedef struct
{
	UINT32
	reg_h_av_pip0_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_350_Bx_T;

typedef struct
{
	UINT32
	reg_h_av_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_351_Bx_T;

typedef struct
{
	UINT32
	reg_v_av_pip0_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_352_Bx_T;

typedef struct
{
	UINT32
	reg_v_av_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_353_Bx_T;

typedef struct
{
	UINT32
	reg_h_fp_pip0_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_354_Bx_T;

typedef struct
{
	UINT32
	reg_h_fp_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_355_Bx_T;

typedef struct
{
	UINT32
	reg_v_fp_pip0:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_356_Bx_T;

typedef struct
{
	UINT32
	reg_h_bp_pip0_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_357_Bx_T;

typedef struct
{
	UINT32
	reg_h_bp_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_358_Bx_T;

typedef struct
{
	UINT32
	reg_v_bp_pip0:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_359_Bx_T;

typedef struct
{
	UINT32
	reg_h_sync_pip0_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_360_Bx_T;

typedef struct
{
	UINT32
	reg_h_sync_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_361_Bx_T;

typedef struct
{
	UINT32
	reg_v_sync_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_362_Bx_T;

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
} LINK_363_Bx_T;

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
} LINK_367_Bx_T;

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
} LINK_368_Bx_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_369_Bx_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_370_Bx_T;

typedef struct
{
	UINT32
	reg_acr_n_rx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_371_Bx_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_372_Bx_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_373_Bx_T;

typedef struct
{
	UINT32
	reg_acr_n_tx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_374_Bx_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_375_Bx_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_376_Bx_T;

typedef struct
{
	UINT32
	reg_acr_cts_rx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_377_Bx_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip0_2:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_378_Bx_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_379_Bx_T;

typedef struct
{
	UINT32
	reg_acr_cts_tx_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_380_Bx_T;

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
} LINK_381_Bx_T;

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
} LINK_382_Bx_T;

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
} LINK_383_Bx_T;

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
} LINK_384_Bx_T;

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
} LINK_385_Bx_T;

typedef struct
{
	UINT32
	reg_achst_ow_byte1_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_386_Bx_T;

typedef struct
{
	UINT32
	reg_achst_byte0_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_387_Bx_T;

typedef struct
{
	UINT32
	reg_achst_byte1_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_388_Bx_T;

typedef struct
{
	UINT32
	reg_achst_byte2_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_389_Bx_T;

typedef struct
{
	UINT32
	reg_achst_byte3_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_390_Bx_T;

typedef struct
{
	UINT32
	reg_achst_byte4_pip0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_391_Bx_T;

typedef struct
{
	UINT32
	reg_achst_fs_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_392_Bx_T;

typedef struct
{
	UINT32
	reg_burst_pc_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_393_Bx_T;

typedef struct
{
	UINT32
	reg_burst_pc_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_394_Bx_T;

typedef struct
{
	UINT32
	reg_burst_pd_pip0_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_395_Bx_T;

typedef struct
{
	UINT32
	reg_burst_pd_pip0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_396_Bx_T;

typedef struct
{
	UINT32
	reg_i2s_len_ovrr_pip0:4,		//3,0
	reg_i2s_len_ovrr_en_pip0:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_397_Bx_T;

typedef struct
{
	UINT32
	reg_i2s_ch_mute_pip0:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_398_Bx_T;

typedef struct
{
	UINT32
	reg_vid_blank_r_prt0_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_399_Bx_T;

typedef struct
{
	UINT32
	reg_vid_blank_r_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_400_Bx_T;

typedef struct
{
	UINT32
	reg_vid_blank_g_prt0_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_401_Bx_T;

typedef struct
{
	UINT32
	reg_vid_blank_g_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_402_Bx_T;

typedef struct
{
	UINT32
	reg_vid_blank_b_prt0_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_403_Bx_T;

typedef struct
{
	UINT32
	reg_vid_blank_b_prt0_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_404_Bx_T;

typedef struct
{
	UINT32
	reg_pre_crit:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_808_Bx_T;

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
} LINK_809_Bx_T;

typedef struct
{
	UINT32
	reg_pgmr_addr_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_810_Bx_T;

typedef struct
{
	UINT32
	reg_pgmr_addr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_811_Bx_T;

typedef struct
{
	UINT32
	reg_pgmr_wdata:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_812_Bx_T;

typedef struct
{
	UINT32
	reg_pgmr_rdata:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_813_Bx_T;

typedef struct
{
	UINT32
	reg_pgmr_wr:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_814_Bx_T;

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
} LINK_815_Bx_T;

typedef struct
{
	UINT32
	reg_mif_dec:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_816_Bx_T;

typedef struct
{
	UINT32
	reg_spd_dec:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_817_Bx_T;

typedef struct
{
	UINT32
	reg_pkt_clr:1,		//0,0
	reg_vf_clr:1,		//1,1
	reg_dbg_hdcp_key_bak:1,		//2,2
	reg_vf_clr_on_nosync:1,		//3,3
	reg_pkt_clr_on_nosync:1,		//4,4
	reg_pkt_clr_on_dvi:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_818_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_287:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_819_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_286:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_820_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_285:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_821_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_284:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_822_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_283:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_823_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_282:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_824_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_281:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_825_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_280:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_826_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_279:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_827_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_278:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_828_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_277:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_829_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_276:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_830_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_275:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_831_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_274:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_832_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_273:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_833_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_272:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_834_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_271:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_835_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_270:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_836_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_269:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_837_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_268:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_838_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_267:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_839_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_266:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_840_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_265:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_841_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_264:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_842_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_263:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_843_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_262:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_844_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_261:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_845_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_260:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_846_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_259:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_847_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_258:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_848_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_257:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_849_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_256:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_850_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_255:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_851_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_254:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_852_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_253:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_853_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_252:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_854_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_251:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_855_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_250:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_856_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_249:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_857_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_248:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_858_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_247:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_859_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_246:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_860_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_245:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_861_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_244:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_862_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_243:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_863_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_242:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_864_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_241:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_865_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_240:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_866_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_239:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_867_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_238:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_868_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_237:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_869_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_236:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_870_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_235:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_871_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_234:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_872_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_233:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_873_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_232:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_874_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_231:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_875_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_230:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_876_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_229:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_877_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_228:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_878_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_227:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_879_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_226:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_880_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_225:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_881_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_224:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_882_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_223:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_883_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_222:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_884_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_221:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_885_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_220:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_886_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_219:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_887_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_218:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_888_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_217:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_889_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_216:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_890_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_215:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_891_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_214:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_892_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_213:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_893_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_212:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_894_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_211:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_895_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_210:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_896_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_209:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_897_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_208:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_898_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_207:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_899_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_206:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_900_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_205:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_901_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_204:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_902_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_203:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_903_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_202:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_904_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_201:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_905_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_200:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_906_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_199:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_907_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_198:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_908_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_197:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_909_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_196:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_910_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_195:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_911_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_194:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_912_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_193:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_913_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_192:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_914_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_191:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_915_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_190:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_916_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_189:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_917_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_188:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_918_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_187:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_919_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_186:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_920_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_185:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_921_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_184:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_922_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_183:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_923_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_182:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_924_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_181:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_925_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_180:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_926_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_179:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_927_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_178:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_928_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_177:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_929_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_176:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_930_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_175:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_931_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_174:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_932_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_173:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_933_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_172:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_934_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_171:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_935_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_170:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_936_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_169:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_937_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_168:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_938_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_167:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_939_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_166:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_940_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_165:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_941_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_164:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_942_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_163:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_943_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_162:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_944_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_161:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_945_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_160:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_946_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_159:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_947_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_158:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_948_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_157:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_949_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_156:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_950_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_155:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_951_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_154:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_952_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_153:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_953_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_152:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_954_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_151:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_955_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_150:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_956_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_149:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_957_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_148:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_958_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_147:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_959_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_146:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_960_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_145:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_961_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_144:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_962_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_143:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_963_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_142:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_964_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_141:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_965_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_140:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_966_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_139:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_967_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_138:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_968_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_137:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_969_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_136:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_970_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_135:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_971_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_134:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_972_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_133:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_973_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_132:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_974_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_131:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_975_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_130:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_976_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_129:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_977_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_128:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_978_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_127:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_979_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_126:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_980_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_125:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_981_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_124:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_982_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_123:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_983_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_122:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_984_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_121:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_985_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_120:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_986_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_119:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_987_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_118:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_988_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_117:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_989_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_116:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_990_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_115:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_991_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_114:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_992_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_113:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_993_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_112:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_994_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_111:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_995_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_110:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_996_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_109:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_997_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_108:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_998_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_107:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_999_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_106:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1000_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_105:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1001_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_104:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1002_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_103:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1003_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_102:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1004_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_101:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1005_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_100:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1006_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_99:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1007_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_98:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1008_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_97:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1009_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_96:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1010_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_95:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1011_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_94:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1012_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_93:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1013_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_92:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1014_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_91:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1015_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_90:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1016_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_89:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1017_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_88:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1018_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_87:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1019_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_86:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1020_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_85:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1021_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_84:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1022_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_83:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1023_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_82:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1024_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_81:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1025_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_80:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1026_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_79:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1027_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_78:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1028_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_77:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1029_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_76:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1030_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_75:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1031_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_74:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1032_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_73:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1033_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_72:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1034_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_71:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1035_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_70:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1036_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_69:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1037_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_68:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1038_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_67:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1039_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_66:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1040_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_65:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1041_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_64:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1042_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_63:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1043_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_62:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1044_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_61:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1045_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_60:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1046_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_59:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1047_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_58:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1048_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_57:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1049_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_56:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1050_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_55:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1051_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_54:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1052_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_53:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1053_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_52:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1054_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_51:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1055_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_50:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1056_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_49:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1057_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_48:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1058_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_47:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1059_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_46:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1060_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_45:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1061_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_44:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1062_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_43:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1063_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_42:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1064_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_41:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1065_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_40:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1066_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_39:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1067_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_38:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1068_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_37:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1069_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_36:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1070_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_35:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1071_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_34:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1072_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_33:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1073_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_32:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1074_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_31:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1075_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_30:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1076_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_29:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1077_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_28:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1078_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_27:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1079_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_26:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1080_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_25:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1081_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_24:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1082_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_23:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1083_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_22:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1084_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_21:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1085_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_20:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1086_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_19:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1087_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_18:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1088_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_17:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1089_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_16:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1090_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_15:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1091_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_14:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1092_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_13:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1093_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_12:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1094_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_11:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1095_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_10:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1096_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_9:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1097_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_8:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1098_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_7:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1099_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_6:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1100_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_5:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1101_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_4:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1102_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1103_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1104_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1105_Bx_T;

typedef struct
{
	UINT32
	reg_hdcp_key_bak_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LINK_1106_Bx_T;

typedef struct
{
	UINT32
	intr_adc3ch_vsfreq_change:1,		//0,0
	intr_adc3ch_hsfreq_change:1,		//1,1
	intr_adc3ch_llpll_status_change:1,		//2,2
	intr_adc3ch_det_hsync:1,		//3,3
	intr_adc3ch_resolution_change:1,		//4,4
	intr_adc3ch_in_signal:1,		//5,5
	intr_adc3ch_no_signal:1,		//6,6
	intr_adc3ch_dpms_statechange:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_0_Bx_T;

typedef struct
{
	UINT32
	intr_adc3ch_apa_complete:1,		//0,0
	intr_adc3ch_aogc_err:1,		//1,1
	intr_adc3ch_aogc_done:1,		//2,2
	intr_adc3ch_nonstablehs:1,		//3,3
	intr_adc3ch_nonstablevs:1,		//4,4
	intr_adc3ch_loss_vsync:1,		//5,5
	intr_adc3ch_det_vsync:1,		//6,6
	intr_adc3ch_loss_hsync:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_1_Bx_T;

typedef struct
{
	UINT32
	enable_intr_adc3ch_vsfreq_change:1,		//0,0
	enable_intr_adc3ch_hsfreq_change:1,		//1,1
	enable_intr_adc3ch_llpll_status_change:1,		//2,2
	enable_intr_adc3ch_det_hsync:1,		//3,3
	enable_intr_adc3ch_resolution_change:1,		//4,4
	enable_intr_adc3ch_in_signal:1,		//5,5
	enable_intr_adc3ch_no_signal:1,		//6,6
	enable_intr_adc3ch_dpms_statechange:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_EN_0_Bx_T;

typedef struct
{
	UINT32
	enable_intr_adc3ch_apa_complete:1,		//0,0
	enable_intr_adc3ch_aogc_err:1,		//1,1
	enable_intr_adc3ch_aogc_done:1,		//2,2
	enable_intr_adc3ch_nonstablehs:1,		//3,3
	enable_intr_adc3ch_nonstablevs:1,		//4,4
	enable_intr_adc3ch_loss_vsync:1,		//5,5
	enable_intr_adc3ch_det_vsync:1,		//6,6
	enable_intr_adc3ch_loss_hsync:1,		//7,7
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} L9A_3CH_INTR_EN_1_Bx_T;

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
} LLPLL_0_Bx_T;

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
} LLPLL_1_Bx_T;

typedef struct
{
	UINT32
	llpll_counter_max_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_2_Bx_T;

typedef struct
{
	UINT32
	llpll_counter_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_3_Bx_T;

typedef struct
{
	UINT32
	llpll_shift_man_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_4_Bx_T;

typedef struct
{
	UINT32
	llpll_shift_man_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_5_Bx_T;

typedef struct
{
	UINT32
	llpll_sf_mode_control:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_6_Bx_T;

typedef struct
{
	UINT32
	llpll_dco_max:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_7_Bx_T;

typedef struct
{
	UINT32
	llpll_dco_min:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_8_Bx_T;

typedef struct
{
	UINT32
	llpll_coarse_scale:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_9_Bx_T;

typedef struct
{
	UINT32
	llpll_g1_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_10_Bx_T;

typedef struct
{
	UINT32
	llpll_g2_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_11_Bx_T;

typedef struct
{
	UINT32
	llpll_g3_p_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_12_Bx_T;

typedef struct
{
	UINT32
	llpll_g3_n_nom:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_13_Bx_T;

typedef struct
{
	UINT32
	llpll_g1_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_14_Bx_T;

typedef struct
{
	UINT32
	llpll_g2_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_15_Bx_T;

typedef struct
{
	UINT32
	llpll_g3_p_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_16_Bx_T;

typedef struct
{
	UINT32
	llpll_g3_n_fine:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_17_Bx_T;

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
} LLPLL_18_Bx_T;

typedef struct
{
	UINT32
	llpll_diff_monitor_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_19_Bx_T;

typedef struct
{
	UINT32
	llpll_diff_monitor_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_20_Bx_T;

typedef struct
{
	UINT32
	llpll_counter_monitor_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_21_Bx_T;

typedef struct
{
	UINT32
	llpll_counter_monitor_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_22_Bx_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_3:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_23_Bx_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_24_Bx_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_25_Bx_T;

typedef struct
{
	UINT32
	llpll_dco_control_monitor_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_26_Bx_T;

typedef struct
{
	UINT32
	llpll_filter_status:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} LLPLL_27_Bx_T;

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
} AFE_3CH_0_Bx_T;

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
} AFE_3CH_1_Bx_T;

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
} AFE_3CH_2_Bx_T;

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
} AFE_3CH_3_Bx_T;

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
} AFE_3CH_4_Bx_T;

typedef struct
{
	UINT32
	afe3ch_soglvl:5,		//4,0
	afe3ch_sog_hys:2,		//6,5
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_5_Bx_T;

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
} AFE_3CH_6_Bx_T;

typedef struct
{
	UINT32
	afe3ch_ctl_add:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_3CH_7_Bx_T;

typedef struct
{
	UINT32
	r_pre_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_0_Bx_T;

typedef struct
{
	UINT32
	r_pre_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_1_Bx_T;

typedef struct
{
	UINT32
	g_pre_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_2_Bx_T;

typedef struct
{
	UINT32
	g_pre_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_3_Bx_T;

typedef struct
{
	UINT32
	b_pre_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_4_Bx_T;

typedef struct
{
	UINT32
	b_pre_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_5_Bx_T;

typedef struct
{
	UINT32
	r_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_6_Bx_T;

typedef struct
{
	UINT32
	r_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_7_Bx_T;

typedef struct
{
	UINT32
	g_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_8_Bx_T;

typedef struct
{
	UINT32
	g_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_9_Bx_T;

typedef struct
{
	UINT32
	b_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_10_Bx_T;

typedef struct
{
	UINT32
	b_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_11_Bx_T;

typedef struct
{
	UINT32
	common_gain_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_12_Bx_T;

typedef struct
{
	UINT32
	common_gain_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_13_Bx_T;

typedef struct
{
	UINT32
	gain_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_14_Bx_T;

typedef struct
{
	UINT32
	r_dig_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_15_Bx_T;

typedef struct
{
	UINT32
	r_dig_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_16_Bx_T;

typedef struct
{
	UINT32
	g_dig_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_17_Bx_T;

typedef struct
{
	UINT32
	g_dig_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_18_Bx_T;

typedef struct
{
	UINT32
	b_dig_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_19_Bx_T;

typedef struct
{
	UINT32
	b_dig_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_20_Bx_T;

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
} ADC_DIG_21_Bx_T;

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
} ADC_DIG_22_Bx_T;

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
} ADC_DIG_23_Bx_T;

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
} ADC_DIG_24_Bx_T;

typedef struct
{
	UINT32
	cst_eq_thr:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_25_Bx_T;

typedef struct
{
	UINT32
	cst_diff_thr_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_26_Bx_T;

typedef struct
{
	UINT32
	cst_diff_thr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_27_Bx_T;

typedef struct
{
	UINT32
	ro_vsdet:1,		//0,0
	ro_hsdet:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_28_Bx_T;

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
} ADC_DIG_29_Bx_T;

typedef struct
{
	UINT32
	ro_hsprdref_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_30_Bx_T;

typedef struct
{
	UINT32
	ro_hsprdref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_31_Bx_T;

typedef struct
{
	UINT32
	ro_hslowref_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_32_Bx_T;

typedef struct
{
	UINT32
	ro_hslowref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_33_Bx_T;

typedef struct
{
	UINT32
	ro_vsprdref_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_34_Bx_T;

typedef struct
{
	UINT32
	ro_vsprdref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_35_Bx_T;

typedef struct
{
	UINT32
	ro_vslowref_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_36_Bx_T;

typedef struct
{
	UINT32
	ro_vslowref_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_37_Bx_T;

typedef struct
{
	UINT32
	adc_ro_rovf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_38_Bx_T;

typedef struct
{
	UINT32
	adc_ro_rovf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_39_Bx_T;

typedef struct
{
	UINT32
	adc_ro_rudf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_40_Bx_T;

typedef struct
{
	UINT32
	adc_ro_rudf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_41_Bx_T;

typedef struct
{
	UINT32
	adc_ro_govf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_42_Bx_T;

typedef struct
{
	UINT32
	adc_ro_govf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_43_Bx_T;

typedef struct
{
	UINT32
	adc_ro_gudf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_44_Bx_T;

typedef struct
{
	UINT32
	adc_ro_gudf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_45_Bx_T;

typedef struct
{
	UINT32
	adc_ro_bovf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_46_Bx_T;

typedef struct
{
	UINT32
	adc_ro_bovf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_47_Bx_T;

typedef struct
{
	UINT32
	adc_ro_budf_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_48_Bx_T;

typedef struct
{
	UINT32
	adc_ro_budf_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_49_Bx_T;

typedef struct
{
	UINT32
	clamp_base:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_50_Bx_T;

typedef struct
{
	UINT32
	clamp_place:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_51_Bx_T;

typedef struct
{
	UINT32
	hsdiff_thr_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_52_Bx_T;

typedef struct
{
	UINT32
	hsdiff_thr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_53_Bx_T;

typedef struct
{
	UINT32
	hsprdeq_thr:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_54_Bx_T;

typedef struct
{
	UINT32
	hsoutover:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_55_Bx_T;

typedef struct
{
	UINT32
	hsoutcnt_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_56_Bx_T;

typedef struct
{
	UINT32
	hsoutcnt_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_57_Bx_T;

typedef struct
{
	UINT32
	hsout_width_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_58_Bx_T;

typedef struct
{
	UINT32
	hsout_width_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_59_Bx_T;

typedef struct
{
	UINT32
	hsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_60_Bx_T;

typedef struct
{
	UINT32
	vsout_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_61_Bx_T;

typedef struct
{
	UINT32
	vsdiff_thr_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_62_Bx_T;

typedef struct
{
	UINT32
	vsdiff_thr_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_63_Bx_T;

typedef struct
{
	UINT32
	vsdiff_thr_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_64_Bx_T;

typedef struct
{
	UINT32
	vsprdeq_thr:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_65_Bx_T;

typedef struct
{
	UINT32
	vsoutover:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_66_Bx_T;

typedef struct
{
	UINT32
	vsoutcnt_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_67_Bx_T;

typedef struct
{
	UINT32
	vsoutcnt_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_68_Bx_T;

typedef struct
{
	UINT32
	vsoutcnt_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_69_Bx_T;

typedef struct
{
	UINT32
	vsout_width_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_70_Bx_T;

typedef struct
{
	UINT32
	vsout_width_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_71_Bx_T;

typedef struct
{
	UINT32
	vsout_width_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_72_Bx_T;

typedef struct
{
	UINT32
	vsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_73_Bx_T;

typedef struct
{
	UINT32
	vs_auto_dly_enable:1,		//0,0
	fld_inv:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_74_Bx_T;

typedef struct
{
	UINT32
	ro_hsout_width_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_75_Bx_T;

typedef struct
{
	UINT32
	ro_hsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_76_Bx_T;

typedef struct
{
	UINT32
	ro_vsout_width_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_77_Bx_T;

typedef struct
{
	UINT32
	ro_vsout_width_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_78_Bx_T;

typedef struct
{
	UINT32
	ro_vsout_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_79_Bx_T;

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
} ADC_DIG_80_Bx_T;

typedef struct
{
	UINT32
	ro_hsoutprd_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_81_Bx_T;

typedef struct
{
	UINT32
	ro_hsoutprd_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_82_Bx_T;

typedef struct
{
	UINT32
	ro_vsoutprd_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_83_Bx_T;

typedef struct
{
	UINT32
	ro_vsoutprd_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_84_Bx_T;

typedef struct
{
	UINT32
	ro_vsoutprd_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_85_Bx_T;

typedef struct
{
	UINT32
	ro_linecnt_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_86_Bx_T;

typedef struct
{
	UINT32
	ro_linecnt_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_87_Bx_T;

typedef struct
{
	UINT32
	ctrl_sequence:1,		//0,0
	hsync_sel:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_88_Bx_T;

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
} ADC_DIG_89_Bx_T;

typedef struct
{
	UINT32
	r_compare_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_90_Bx_T;

typedef struct
{
	UINT32
	r_compare_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_91_Bx_T;

typedef struct
{
	UINT32
	g_compare_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_92_Bx_T;

typedef struct
{
	UINT32
	g_compare_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_93_Bx_T;

typedef struct
{
	UINT32
	b_compare_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_94_Bx_T;

typedef struct
{
	UINT32
	b_compare_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_95_Bx_T;

typedef struct
{
	UINT32
	aogc_th_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_96_Bx_T;

typedef struct
{
	UINT32
	aogc_th_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_97_Bx_T;

typedef struct
{
	UINT32
	aogc_sample_accum:1,		//0,0
	aogc_sample_count:4,		//4,1
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_98_Bx_T;

typedef struct
{
	UINT32
	aogc_counter_max_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_99_Bx_T;

typedef struct
{
	UINT32
	aogc_counter_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_100_Bx_T;

typedef struct
{
	UINT32
	ro_aogc_state:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_101_Bx_T;

typedef struct
{
	UINT32
	ro_avg_r_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_102_Bx_T;

typedef struct
{
	UINT32
	ro_avg_r_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_103_Bx_T;

typedef struct
{
	UINT32
	ro_avg_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_104_Bx_T;

typedef struct
{
	UINT32
	ro_avg_g_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_105_Bx_T;

typedef struct
{
	UINT32
	ro_avg_g_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_106_Bx_T;

typedef struct
{
	UINT32
	ro_avg_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_107_Bx_T;

typedef struct
{
	UINT32
	ro_avg_b_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_108_Bx_T;

typedef struct
{
	UINT32
	ro_avg_b_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_109_Bx_T;

typedef struct
{
	UINT32
	ro_avg_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_110_Bx_T;

typedef struct
{
	UINT32
	intr_ctrl:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_111_Bx_T;

typedef struct
{
	UINT32
	size_detect_ctrl_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_112_Bx_T;

typedef struct
{
	UINT32
	size_detect_ctrl_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_113_Bx_T;

typedef struct
{
	UINT32
	sd_ext_vsize_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_114_Bx_T;

typedef struct
{
	UINT32
	sd_ext_vsize_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_115_Bx_T;

typedef struct
{
	UINT32
	sd_vd_vsize_2:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_116_Bx_T;

typedef struct
{
	UINT32
	sd_vd_vsize_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_117_Bx_T;

typedef struct
{
	UINT32
	sd_vd_vsize_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_118_Bx_T;

typedef struct
{
	UINT32
	sd_vd_hsize_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_119_Bx_T;

typedef struct
{
	UINT32
	sd_vd_hsize_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_120_Bx_T;

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
} ADC_DIG_121_Bx_T;

typedef struct
{
	UINT32
	ap_xpos_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_122_Bx_T;

typedef struct
{
	UINT32
	ap_xpos_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_123_Bx_T;

typedef struct
{
	UINT32
	ap_xsize_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_124_Bx_T;

typedef struct
{
	UINT32
	ap_xsize_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_125_Bx_T;

typedef struct
{
	UINT32
	ap_ypos_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_126_Bx_T;

typedef struct
{
	UINT32
	ap_ypos_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_127_Bx_T;

typedef struct
{
	UINT32
	ap_ysize_g_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_128_Bx_T;

typedef struct
{
	UINT32
	ap_ysize_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_129_Bx_T;

typedef struct
{
	UINT32
	ap_xpos_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_130_Bx_T;

typedef struct
{
	UINT32
	ap_xpos_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_131_Bx_T;

typedef struct
{
	UINT32
	ap_xsize_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_132_Bx_T;

typedef struct
{
	UINT32
	ap_xsize_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_133_Bx_T;

typedef struct
{
	UINT32
	ap_ypos_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_134_Bx_T;

typedef struct
{
	UINT32
	ap_ypos_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_135_Bx_T;

typedef struct
{
	UINT32
	ap_ysize_b_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_136_Bx_T;

typedef struct
{
	UINT32
	ap_ysize_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_137_Bx_T;

typedef struct
{
	UINT32
	ap_xpos_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_138_Bx_T;

typedef struct
{
	UINT32
	ap_xpos_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_139_Bx_T;

typedef struct
{
	UINT32
	ap_xsize_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_140_Bx_T;

typedef struct
{
	UINT32
	ap_xsize_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_141_Bx_T;

typedef struct
{
	UINT32
	ap_ypos_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_142_Bx_T;

typedef struct
{
	UINT32
	ap_ypos_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_143_Bx_T;

typedef struct
{
	UINT32
	ap_ysize_r_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_144_Bx_T;

typedef struct
{
	UINT32
	ap_ysize_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_145_Bx_T;

typedef struct
{
	UINT32
	ap_out_b_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_146_Bx_T;

typedef struct
{
	UINT32
	ap_out_b_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_147_Bx_T;

typedef struct
{
	UINT32
	ap_out_b_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_148_Bx_T;

typedef struct
{
	UINT32
	ap_out_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_149_Bx_T;

typedef struct
{
	UINT32
	ap_out_g_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_150_Bx_T;

typedef struct
{
	UINT32
	ap_out_g_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_151_Bx_T;

typedef struct
{
	UINT32
	ap_out_g_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_152_Bx_T;

typedef struct
{
	UINT32
	ap_out_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_153_Bx_T;

typedef struct
{
	UINT32
	ap_out_r_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_154_Bx_T;

typedef struct
{
	UINT32
	ap_out_r_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_155_Bx_T;

typedef struct
{
	UINT32
	ap_out_r_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_156_Bx_T;

typedef struct
{
	UINT32
	ap_out_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_157_Bx_T;

typedef struct
{
	UINT32
	ap_sum_b_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_158_Bx_T;

typedef struct
{
	UINT32
	ap_sum_b_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_159_Bx_T;

typedef struct
{
	UINT32
	ap_sum_b_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_160_Bx_T;

typedef struct
{
	UINT32
	ap_sum_b_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_161_Bx_T;

typedef struct
{
	UINT32
	ap_sum_g_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_162_Bx_T;

typedef struct
{
	UINT32
	ap_sum_g_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_163_Bx_T;

typedef struct
{
	UINT32
	ap_sum_g_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_164_Bx_T;

typedef struct
{
	UINT32
	ap_sum_g_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_165_Bx_T;

typedef struct
{
	UINT32
	ap_sum_r_3:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_166_Bx_T;

typedef struct
{
	UINT32
	ap_sum_r_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_167_Bx_T;

typedef struct
{
	UINT32
	ap_sum_r_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_168_Bx_T;

typedef struct
{
	UINT32
	ap_sum_r_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_169_Bx_T;

typedef struct
{
	UINT32
	extau_ctrl_1:7,		//6,0
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_170_Bx_T;

typedef struct
{
	UINT32
	extau_ctrl_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_171_Bx_T;

typedef struct
{
	UINT32
	extau_level_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_172_Bx_T;

typedef struct
{
	UINT32
	extau_level_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_173_Bx_T;

typedef struct
{
	UINT32
	extau_up_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_174_Bx_T;

typedef struct
{
	UINT32
	extau_up_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_175_Bx_T;

typedef struct
{
	UINT32
	extau_down_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_176_Bx_T;

typedef struct
{
	UINT32
	extau_down_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_177_Bx_T;

typedef struct
{
	UINT32
	extau_left_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_178_Bx_T;

typedef struct
{
	UINT32
	extau_left_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_179_Bx_T;

typedef struct
{
	UINT32
	extau_right_offset_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_180_Bx_T;

typedef struct
{
	UINT32
	extau_right_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_181_Bx_T;

typedef struct
{
	UINT32
	extau_up_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_182_Bx_T;

typedef struct
{
	UINT32
	extau_up_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_183_Bx_T;

typedef struct
{
	UINT32
	extau_down_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_184_Bx_T;

typedef struct
{
	UINT32
	extau_down_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_185_Bx_T;

typedef struct
{
	UINT32
	extau_left_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_186_Bx_T;

typedef struct
{
	UINT32
	extau_left_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_187_Bx_T;

typedef struct
{
	UINT32
	extau_right_result_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_188_Bx_T;

typedef struct
{
	UINT32
	extau_right_result_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_189_Bx_T;

typedef struct
{
	UINT32
	r_min_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_190_Bx_T;

typedef struct
{
	UINT32
	r_min_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_191_Bx_T;

typedef struct
{
	UINT32
	r_max_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_192_Bx_T;

typedef struct
{
	UINT32
	r_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_193_Bx_T;

typedef struct
{
	UINT32
	g_min_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_194_Bx_T;

typedef struct
{
	UINT32
	g_min_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_195_Bx_T;

typedef struct
{
	UINT32
	g_max_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_196_Bx_T;

typedef struct
{
	UINT32
	g_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_197_Bx_T;

typedef struct
{
	UINT32
	b_min_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_198_Bx_T;

typedef struct
{
	UINT32
	b_min_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_199_Bx_T;

typedef struct
{
	UINT32
	b_max_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_200_Bx_T;

typedef struct
{
	UINT32
	b_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_201_Bx_T;

typedef struct
{
	UINT32
	blank_sp_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_202_Bx_T;

typedef struct
{
	UINT32
	blank_sp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_203_Bx_T;

typedef struct
{
	UINT32
	r_blank_mode:2,		//1,0
	r_blank_num_clear:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_204_Bx_T;

typedef struct
{
	UINT32
	r_blank_high_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_205_Bx_T;

typedef struct
{
	UINT32
	r_blank_high_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_206_Bx_T;

typedef struct
{
	UINT32
	r_blank_low_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_207_Bx_T;

typedef struct
{
	UINT32
	r_blank_low_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_208_Bx_T;

typedef struct
{
	UINT32
	g_blank_mode:2,		//1,0
	g_blank_num_clear:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_209_Bx_T;

typedef struct
{
	UINT32
	g_blank_high_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_210_Bx_T;

typedef struct
{
	UINT32
	g_blank_high_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_211_Bx_T;

typedef struct
{
	UINT32
	g_blank_low_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_212_Bx_T;

typedef struct
{
	UINT32
	g_blank_low_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_213_Bx_T;

typedef struct
{
	UINT32
	b_blank_num_clear:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_214_Bx_T;

typedef struct
{
	UINT32
	b_blank_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_215_Bx_T;

typedef struct
{
	UINT32
	b_blank_high_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_216_Bx_T;

typedef struct
{
	UINT32
	b_blank_high_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_217_Bx_T;

typedef struct
{
	UINT32
	b_blank_low_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_218_Bx_T;

typedef struct
{
	UINT32
	b_blank_low_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_219_Bx_T;

typedef struct
{
	UINT32
	clamp_width:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_220_Bx_T;

typedef struct
{
	UINT32
	clamp_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_221_Bx_T;

typedef struct
{
	UINT32
	clamp_init:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_222_Bx_T;

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
} ADC_DIG_223_Bx_T;

typedef struct
{
	UINT32
	r_pre_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_224_Bx_T;

typedef struct
{
	UINT32
	r_pre_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_225_Bx_T;

typedef struct
{
	UINT32
	g_pre_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_226_Bx_T;

typedef struct
{
	UINT32
	g_pre_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_227_Bx_T;

typedef struct
{
	UINT32
	b_pre_offset_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_228_Bx_T;

typedef struct
{
	UINT32
	b_pre_offset_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_229_Bx_T;

typedef struct
{
	UINT32
	de_h_base:1,		//0,0
	mute_ctrl:3,		//3,1
	cst_free:1,		//4,4
	cst_bypass:1,		//5,5
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_230_Bx_T;

typedef struct
{
	UINT32
	de_h_place_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_231_Bx_T;

typedef struct
{
	UINT32
	de_h_place_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_232_Bx_T;

typedef struct
{
	UINT32
	de_h_width_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_233_Bx_T;

typedef struct
{
	UINT32
	de_h_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_234_Bx_T;

typedef struct
{
	UINT32
	de_v_base:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_235_Bx_T;

typedef struct
{
	UINT32
	de_v_place_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_236_Bx_T;

typedef struct
{
	UINT32
	de_v_place_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_237_Bx_T;

typedef struct
{
	UINT32
	de_v_width_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_238_Bx_T;

typedef struct
{
	UINT32
	de_v_width_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_239_Bx_T;

typedef struct
{
	UINT32
	apa_mode:2,		//1,0
	apa_enable:1,		//2,2
	de_auto_set:1,		//3,3
	de_auto_en:1,		//4,4
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_240_Bx_T;

/*
typedef struct
{
	UINT32
	apa_enable:1,		//2,2
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_241_Bx_T;

typedef struct
{
	UINT32
	apa_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_242_Bx_T;
*/

typedef struct
{
	UINT32
	apa_wait_counter_max_2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_243_Bx_T;

typedef struct
{
	UINT32
	apa_wait_counter_max_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_244_Bx_T;

typedef struct
{
	UINT32
	apa_wait_counter_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_245_Bx_T;

typedef struct
{
	UINT32
	apa_blank_target_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_246_Bx_T;

typedef struct
{
	UINT32
	apa_blank_target_man_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_247_Bx_T;

typedef struct
{
	UINT32
	apa_blank_target_man_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_248_Bx_T;

typedef struct
{
	UINT32
	apa_blank_counter_max:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_249_Bx_T;

typedef struct
{
	UINT32
	apa_blank_avg_ro_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_250_Bx_T;

typedef struct
{
	UINT32
	apa_blank_avg_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_251_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_thr:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_252_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_pos_ro_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_253_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_pos_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_254_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_offset:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_255_Bx_T;

typedef struct
{
	UINT32
	apa_pos_offset:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_256_Bx_T;

typedef struct
{
	UINT32
	apa_sampling_counter_max:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_257_Bx_T;

typedef struct
{
	UINT32
	apa_phase_max_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_258_Bx_T;

typedef struct
{
	UINT32
	apa_phase_max_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_259_Bx_T;

typedef struct
{
	UINT32
	apa_phase_step:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_260_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_phase_thr:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_261_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_pos_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_262_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_pos_man_1:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_263_Bx_T;

typedef struct
{
	UINT32
	apa_hsfal_pos_man_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_264_Bx_T;

typedef struct
{
	UINT32
	apa_data1_ro_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_265_Bx_T;

typedef struct
{
	UINT32
	apa_data1_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_266_Bx_T;

typedef struct
{
	UINT32
	apa_phase1_ro_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_267_Bx_T;

typedef struct
{
	UINT32
	apa_phase1_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_268_Bx_T;

typedef struct
{
	UINT32
	apa_pos1_ro:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_269_Bx_T;

typedef struct
{
	UINT32
	apa_data2_ro_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_270_Bx_T;

typedef struct
{
	UINT32
	apa_data2_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_271_Bx_T;

typedef struct
{
	UINT32
	apa_phase2_ro_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_272_Bx_T;

typedef struct
{
	UINT32
	apa_phase2_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_273_Bx_T;

typedef struct
{
	UINT32
	apa_pos2_ro:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_274_Bx_T;

typedef struct
{
	UINT32
	apa_data3_ro_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_275_Bx_T;

typedef struct
{
	UINT32
	apa_data3_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_276_Bx_T;

typedef struct
{
	UINT32
	apa_phase3_ro_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_277_Bx_T;

typedef struct
{
	UINT32
	apa_phase3_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_278_Bx_T;

typedef struct
{
	UINT32
	agc_clear:1,		//0,0
	agc_enable:1,		//1,1
	apa_pos3_ro:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_279_Bx_T;

/*
typedef struct
{
	UINT32
	agc_enable:1,		//1,1
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_280_Bx_T;

typedef struct
{
	UINT32
	agc_clear:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_281_Bx_T;
*/

typedef struct
{
	UINT32
	agc_step:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_282_Bx_T;

typedef struct
{
	UINT32
	agc_blank_sp_1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_283_Bx_T;

typedef struct
{
	UINT32
	agc_blank_sp_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_284_Bx_T;

typedef struct
{
	UINT32
	agc_blank_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_285_Bx_T;

typedef struct
{
	UINT32
	agc_blank_high_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_286_Bx_T;

typedef struct
{
	UINT32
	agc_blank_high_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_287_Bx_T;

typedef struct
{
	UINT32
	agc_blank_low_1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_288_Bx_T;

typedef struct
{
	UINT32
	agc_blank_low_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_289_Bx_T;

typedef struct
{
	UINT32
	agc_clamp_width:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_290_Bx_T;

typedef struct
{
	UINT32
	agc_vsync_only:1,		//0,0
	agc_hsync_only:1,		//1,1
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_291_Bx_T;

typedef struct
{
	UINT32
	r_pre_gain_fin_ro_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_292_Bx_T;

typedef struct
{
	UINT32
	r_pre_gain_fin_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_293_Bx_T;

typedef struct
{
	UINT32
	g_pre_gain_fin_ro_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_294_Bx_T;

typedef struct
{
	UINT32
	g_pre_gain_fin_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_295_Bx_T;

typedef struct
{
	UINT32
	b_pre_gain_fin_ro_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_296_Bx_T;

typedef struct
{
	UINT32
	b_pre_gain_fin_ro_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_297_Bx_T;

typedef struct
{
	UINT32
	llpll_shift_sel:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ADC_DIG_298_Bx_T;

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
} AUDIO_PLL_0_Bx_T;

typedef struct
{
	UINT32
	apll_m_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_1_Bx_T;

typedef struct
{
	UINT32
	apll_m_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_2_Bx_T;

typedef struct
{
	UINT32
	apll_n_1:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_3_Bx_T;

typedef struct
{
	UINT32
	apll_n_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_4_Bx_T;

typedef struct
{
	UINT32
	apll_sel:1,		//0,0
	phy_i2c_sel:1,	//1,1
	apll_od:5,		//6,2
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AUDIO_PLL_5_Bx_T;

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
} AUDIO_PLL_6_Bx_T;

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
} AFE_AAD_0_Bx_T;

typedef struct
{
	UINT32
	aad_dco_fcw_2:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_1_Bx_T;

typedef struct
{
	UINT32
	aad_dco_fcw_1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_2_Bx_T;

typedef struct
{
	UINT32
	aad_dco_fcw_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_3_Bx_T;

typedef struct
{
	UINT32
	aad_spll_m:2,		//1,0
	aad_dco_lpfon:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_4_Bx_T;

typedef struct
{
	UINT32
	aad_spll_n:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_5_Bx_T;

typedef struct
{
	UINT32
	aad_spll_ci:2,		//1,0
	aad_spll_od:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_AAD_6_Bx_T;

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
} AFE_AAD_7_Bx_T;

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
} AFE_AAD_8_Bx_T;

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
} AFE_ACODEC_0_Bx_T;

typedef struct
{
	UINT32
	auad_ch_sel:3,		//2,0
	auad_gcon:4,		//6,3
	b_rsvd1:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_1_Bx_T;

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
} AFE_ACODEC_2_Bx_T;

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
} AFE_ACODEC_3_Bx_T;

typedef struct
{
	UINT32
	aumi_cnt:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} AFE_ACODEC_4_Bx_T;

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
} AFE_ACODEC_5_Bx_T;

typedef struct
{
	UINT32
	dem0:2,		//1,0
	mutec:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_0_Bx_T;

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
} ACODEC_1_Bx_T;

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
} ACODEC_2_Bx_T;

typedef struct
{
	UINT32
	dac0_vol_con_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_3_Bx_T;

typedef struct
{
	UINT32
	dac0_vol_con_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_4_Bx_T;

typedef struct
{
	UINT32
	dac0_rate_cont_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_5_Bx_T;

typedef struct
{
	UINT32
	dac0_rate_cont_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_6_Bx_T;

typedef struct
{
	UINT32
	dac0_rate_cont_mute_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_7_Bx_T;

typedef struct
{
	UINT32
	dac0_rate_cont_mute_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_8_Bx_T;

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
} ACODEC_9_Bx_T;

typedef struct
{
	UINT32
	dac0_pop_con_rate_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_10_Bx_T;

typedef struct
{
	UINT32
	dac0_pop_con_rate_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_11_Bx_T;

typedef struct
{
	UINT32
	dac1_vol_con_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_12_Bx_T;

typedef struct
{
	UINT32
	dac1_vol_con_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_13_Bx_T;

typedef struct
{
	UINT32
	dac1_rate_cont_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_14_Bx_T;

typedef struct
{
	UINT32
	dac1_rate_cont_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_15_Bx_T;

typedef struct
{
	UINT32
	dac1_rate_cont_mute_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_16_Bx_T;

typedef struct
{
	UINT32
	dac1_rate_cont_mute_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_17_Bx_T;

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
} ACODEC_18_Bx_T;

typedef struct
{
	UINT32
	dac1_pop_con_rate_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_19_Bx_T;

typedef struct
{
	UINT32
	dac1_pop_con_rate_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_20_Bx_T;

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
} ACODEC_21_Bx_T;

typedef struct
{
	UINT32
	pol_con:2,		//1,0
	cont:2,		//3,2
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_31_Bx_T;

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
} ACODEC_32_Bx_T;

typedef struct
{
	UINT32
	ladc_vol_con_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_33_Bx_T;

typedef struct
{
	UINT32
	ladc_vol_con_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_34_Bx_T;

typedef struct
{
	UINT32
	ladc_rate_cont_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_35_Bx_T;

typedef struct
{
	UINT32
	ladc_rate_cont_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_36_Bx_T;

typedef struct
{
	UINT32
	ladc_rate_cont_mute_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_37_Bx_T;

typedef struct
{
	UINT32
	ladc_rate_cont_mute_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_38_Bx_T;

typedef struct
{
	UINT32
	cont_mic:2,		//1,0
	ladc_mute_ena:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_39_Bx_T;

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
} ACODEC_40_Bx_T;

typedef struct
{
	UINT32
	vol_con_mic_1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_41_Bx_T;

typedef struct
{
	UINT32
	vol_con_mic_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_42_Bx_T;

typedef struct
{
	UINT32
	rate_cont_mic_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_43_Bx_T;

typedef struct
{
	UINT32
	rate_cont_mic_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_44_Bx_T;

typedef struct
{
	UINT32
	rate_cont_mute_mic_1:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_45_Bx_T;

typedef struct
{
	UINT32
	rate_cont_mute_mic_0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_46_Bx_T;

typedef struct
{
	UINT32
	codec_loop_back:1,		//0,0
	force_mic:1,		//1,1
	mute_ena_mic:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} ACODEC_47_Bx_T;

typedef struct
{
	UINT32
	pdb_all:1,		//0,0
	b_rsvd1:3,
	resetb_all:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_01_Bx_T;

typedef struct
{
	UINT32
	pdb_sel:1,		//0,0
	b_rsvd1:3,
	reset_sel:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_02_Bx_T;

typedef struct
{
	UINT32
	cr_pll_pdb:1,		//0,0
	b_rsvd1:3,
	cr_pll_resetb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_03_Bx_T;

typedef struct
{
	UINT32
	cr_dig_ldo_pdb:1,		//0,0
	b_rsvd1:3,
	cr_ldo_pdb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_04_Bx_T;

typedef struct
{
	UINT32
	cr_mode_sel_resetb:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_05_Bx_T;

typedef struct
{
	UINT32
	eq_pdb:1,		//0,0
	b_rsvd1:3,
	eq_resetb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_06_Bx_T;

typedef struct
{
	UINT32
	dr_pdb:1,		//0,0
	b_rsvd1:3,
	dr_resetb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_07_Bx_T;

typedef struct
{
	UINT32
	dtb_resetb:1,		//0,0
	b_rsvd1:3,
	bert_resetb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_08_Bx_T;

typedef struct
{
	UINT32
	aud_pdb:1,		//0,0
	b_rsvd1:3,
	aud_resetb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_09_Bx_T;

typedef struct
{
	UINT32
	pix_pdb:1,		//0,0
	b_rsvd1:3,
	pix_resetb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_0A_Bx_T;

typedef struct
{
	UINT32
	cr_tmr_scale:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_10_Bx_T;

typedef struct
{
	UINT32
	cr_icp_adj:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_11_Bx_T;

typedef struct
{
	UINT32
	cr_test:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_12_Bx_T;

typedef struct
{
	UINT32
	cr_kvco_offset:4,		//3,0
	b_rsvd1:4,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_13_Bx_T;

typedef struct
{
	UINT32
	cr_pll_man_mode:2,		//1,0
	b_rsvd1:2,
	cr_i2c_pll_mode:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_14_Bx_T;

typedef struct
{
	UINT32
	cr_i2c_offset:3,		//2,0
	b_rsvd1:1,
	cr_i2c_offset_mode:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_15_Bx_T;

typedef struct
{
	UINT32
	cr_man_run:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_16_Bx_T;

typedef struct
{
	UINT32
	cr_ref_clk_mode:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_17_Bx_T;

typedef struct
{
	UINT32
	cr_ref_clk_mode:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_18_Bx_T;

typedef struct
{
	UINT32
	cr_vbgr_ctrl:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_19_Bx_T;

typedef struct
{
	UINT32
	cr_dig_vbgr_ctrl:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1A_Bx_T;

typedef struct
{
	UINT32
	cr_test_clk_en:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1B_Bx_T;

typedef struct
{
	UINT32
	cr_pll_offset:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1C_Bx_T;

typedef struct
{
	UINT32
	cr_pll_mode:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1D_Bx_T;

typedef struct
{
	UINT32
	cr_lt:1,		//0,0
	b_rsvd1:3,
	cr_ht:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1E_Bx_T;

typedef struct
{
	UINT32
	cr_done:1,		//0,0
	b_rsvd1:3,
	cr_lock:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_1F_Bx_T;

typedef struct
{
	UINT32
	tmr_done:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_20_Bx_T;

typedef struct
{
	UINT32
	measure_done_s4:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_21_Bx_T;

typedef struct
{
	UINT32
	mode_sel_st:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_22_Bx_T;

typedef struct
{
	UINT32
	tmds_freq:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_23_Bx_T;

typedef struct
{
	UINT32
	tmds_freq:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_24_Bx_T;

typedef struct
{
	UINT32
	idr_adj:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_25_Bx_T;

typedef struct
{
	UINT32
	odt_sel:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_26_Bx_T;

typedef struct
{
	UINT32
	pix_pdb:1,		//0,0
	b_rsvd1:3,
	pix_resetb:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_30_Bx_T;

typedef struct
{
	UINT32
	pix_cp_i:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_31_Bx_T;

typedef struct
{
	UINT32
	pix_vco_offset:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_32_Bx_T;

typedef struct
{
	UINT32
	pix_pclk_div2:1,		//0,0
	b_rsvd1:3,
	pix_pclk_div4:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_33_Bx_T;

typedef struct
{
	UINT32
	pix_div:2,		//1,0
	b_rsvd1:2,
	pix_pll_dat_sel:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_34_Bx_T;

typedef struct
{
	UINT32
	pix_test_en:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_35_Bx_T;

typedef struct
{
	UINT32
	pix_link_div:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_36_Bx_T;

typedef struct
{
	UINT32
	eq_testen_i2c:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_40_Bx_T;

typedef struct
{
	UINT32
	eq_rs_sel:1,		//0,0
	b_rsvd1:3,
	eq_cs_sel:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_41_Bx_T;

typedef struct
{
	UINT32
	eq_freeze_l:1,		//0,0
	b_rsvd1:3,
	eq_freeze_h:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_42_Bx_T;

typedef struct
{
	UINT32
	eq_rs:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_43_Bx_T;

typedef struct
{
	UINT32
	eq_cs:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_44_Bx_T;

typedef struct
{
	UINT32
	eq_thr_h:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_45_Bx_T;

typedef struct
{
	UINT32
	eq_thr_l:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_46_Bx_T;

typedef struct
{
	UINT32
	eq_tot_h:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_47_Bx_T;

typedef struct
{
	UINT32
	eq_tot_l:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_48_Bx_T;

typedef struct
{
	UINT32
	eq_ictl:3,		//2,0
	b_rsvd1:1,
	eq_igmc:3,		//6,4
	b_rsvd2:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_49_Bx_T;

typedef struct
{
	UINT32
	eq_cidiv:2,		//1,0
	b_rsvd1:2,
	eq_hldiv:2,		//5,4
	b_rsvd2:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4A_Bx_T;

typedef struct
{
	UINT32
	eq_start_frz:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4B_Bx_T;

typedef struct
{
	UINT32
	eq_avg_width:2,		//1,0
	b_rsvd1:2,
	eq_af_en:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4C_Bx_T;

typedef struct
{
	UINT32
	eq_offset_h:3,		//2,0
	b_rsvd1:1,
	eq_offset_l:3,		//6,4
	b_rsvd2:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4D_Bx_T;

typedef struct
{
	UINT32
	eq_cmpe:3,		//2,0
	b_rsvd1:1,
	eq_cmpc:3,		//6,4
	b_rsvd2:1,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4E_Bx_T;

typedef struct
{
	UINT32
	eq_m_cs:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_4F_Bx_T;

typedef struct
{
	UINT32
	eq_m_rs:5,		//4,0
	b_rsvd1:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_50_Bx_T;

typedef struct
{
	UINT32
	eq_freeze_fh:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_51_Bx_T;

typedef struct
{
	UINT32
	eq_freeze_fl:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_52_Bx_T;

typedef struct
{
	UINT32
	eq_freeze_flag:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_53_Bx_T;

typedef struct
{
	UINT32
	odt_ctrl:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_54_Bx_T;

typedef struct
{
	UINT32
	dr_clk_inv_ch0:1,		//0,0
	dr_clk_inv_ch1:1,		//1,1
	dr_clk_inv_ch2:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_80_Bx_T;

typedef struct
{
	UINT32
	dr_man_mode:2,		//1,0
	b_rsvd1:2,
	dr_man_mod_sel:1,		//4,4
	b_rsvd2:3,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_81_Bx_T;

typedef struct
{
	UINT32
	dr_mode:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_82_Bx_T;

typedef struct
{
	UINT32
	dr_n1:3,		//2,0
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_83_Bx_T;

typedef struct
{
	UINT32
	dr_filter_ch0:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_84_Bx_T;

typedef struct
{
	UINT32
	dr_filter_ch1:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_85_Bx_T;

typedef struct
{
	UINT32
	dr_filter_ch2:6,		//5,0
	b_rsvd1:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_86_Bx_T;

typedef struct
{
	UINT32
	dr_phsel_f_ch0:4,		//3,0
	dr_phsel_c_ch0:4,		//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_87_Bx_T;

typedef struct
{
	UINT32
	dr_phsel_f_ch1:4,		//3,0
	dr_phsel_c_ch1:4,		//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_88_Bx_T;

typedef struct
{
	UINT32
	dr_phsel_f_ch2:4,		//3,0
	dr_phsel_c_ch2:4,		//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_89_Bx_T;

typedef struct
{
	UINT32
	dr_phsel_en_ch0:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8A_Bx_T;

typedef struct
{
	UINT32
	dr_phsel_en_ch1:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8B_Bx_T;

typedef struct
{
	UINT32
	dr_phsel_en_ch2:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8C_Bx_T;

typedef struct
{
	UINT32
	dr_monitor_ui_en_ch0:1,		//0,0
	dr_monitor_ui_en_ch1:1,		//1,1
	dr_monitor_ui_en_ch2:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8D_Bx_T;

typedef struct
{
	UINT32
	dr_ph_f_ch0:4,		//3,0
	dr_ph_c_ch0:4,		//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8E_Bx_T;

typedef struct
{
	UINT32
	dr_ph_f_ch1:4,		//3,0
	dr_ph_c_ch1:4,		//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_8F_Bx_T;

typedef struct
{
	UINT32
	dr_ph_f_ch2:4,		//3,0
	dr_ph_c_ch2:4,		//7,4
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_90_Bx_T;

typedef struct
{
	UINT32
	dr_monitor_ui_ch2:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_91_Bx_T;

typedef struct
{
	UINT32
	dr_monitor_ui_ch1:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_92_Bx_T;

typedef struct
{
	UINT32
	dr_monitor_ui_ch0:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_93_Bx_T;

typedef struct
{
	UINT32
	bert_run_ch0:1,		//0,0
	bert_run_ch1:1,		//1,1
	bert_run_ch2:1,		//2,2
	b_rsvd1:5,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A0_Bx_T;

typedef struct
{
	UINT32
	bert_ch_sel:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A1_Bx_T;

typedef struct
{
	UINT32
	bert_ex_run:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A2_Bx_T;

typedef struct
{
	UINT32
	tmds_de_recover:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A3_Bx_T;

typedef struct
{
	UINT32
	tmds_clk_inv:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A4_Bx_T;

typedef struct
{
	UINT32
	tmds_bypass:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A5_Bx_T;

typedef struct
{
	UINT32
	err_thres:2,		//1,0
	b_rsvd1:2,
	bit_thres:2,		//5,4
	b_rsvd2:2,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A6_Bx_T;

typedef struct
{
	UINT32
	bert_sync_done:1,		//0,0
	b_rsvd1:7,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A7_Bx_T;

typedef struct
{
	UINT32
	bert_test_res:2,		//1,0
	b_rsvd1:6,
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A8_Bx_T;

typedef struct
{
	UINT32
	bert_num_err:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_A9_Bx_T;

typedef struct
{
	UINT32
	bert_num_err:8,		//7,0
	rsvd8:8,
	slaveAddr:8,
	regAddr:8;
} HDMI_PHY_N_AA_Bx_T;

extern L9A_VERSION_0_Bx_T gl9a_version_0_bx;
extern HOSTIF_CONTROL_0_Bx_T ghostif_control_0_bx;
extern L9A_HOSTIF_INTR_0_Bx_T gl9a_hostif_intr_0_bx;
extern L9A_HOSTIF_INTR_EN_0_Bx_T gl9a_hostif_intr_en_0_bx;
extern TOP_CONTROL_0_Bx_T gtop_control_0_bx;
extern TOP_CONTROL_1_Bx_T gtop_control_1_bx;
extern TOP_CONTROL_2_Bx_T gtop_control_2_bx;
extern SOFT_RESET_0_Bx_T gsoft_reset_0_bx;
extern SOFT_RESET_1_Bx_T gsoft_reset_1_bx;
extern SOFT_RESET_2_Bx_T gsoft_reset_2_bx;
extern SOFT_RESET_3_Bx_T gsoft_reset_3_bx;
extern SOFT_RESET_4_Bx_T gsoft_reset_4_bx;
extern SOFT_RESET_5_Bx_T gsoft_reset_5_bx;
extern SOFT_RESET_6_Bx_T gsoft_reset_6_bx;
extern SOFT_RESET_7_Bx_T gsoft_reset_7_bx;
extern SOFT_RESET_8_Bx_T gsoft_reset_8_bx;
extern SOFT_RESET_9_Bx_T gsoft_reset_9_bx;
extern SOFT_RESET_10_Bx_T gsoft_reset_10_bx;
extern SOFT_RESET_11_Bx_T gsoft_reset_11_bx;
extern SOFT_RESET_12_Bx_T gsoft_reset_12_bx;
extern SOFT_RESET_13_Bx_T gsoft_reset_13_bx;
extern SOFT_RESET_14_Bx_T gsoft_reset_14_bx;
extern SOFT_RESET_15_Bx_T gsoft_reset_15_bx;
extern SOFT_RESET_16_Bx_T gsoft_reset_16_bx;
extern SOFT_RESET_17_Bx_T gsoft_reset_17_bx;
extern CLOCK_CONTROL_0_Bx_T gclock_control_0_bx;
extern CLOCK_CONTROL_1_Bx_T gclock_control_1_bx;
extern CLOCK_CONTROL_2_Bx_T gclock_control_2_bx;
extern AUDIO_DTO_0_Bx_T gaudio_dto_0_bx;
extern AUDIO_DTO_1_Bx_T gaudio_dto_1_bx;
extern AUDIO_DTO_2_Bx_T gaudio_dto_2_bx;
extern AUDIO_DTO_3_Bx_T gaudio_dto_3_bx;
extern AUDIO_DTO_4_Bx_T gaudio_dto_4_bx;
extern AUDIO_DTO_5_Bx_T gaudio_dto_5_bx;
extern AUDIO_DTO_6_Bx_T gaudio_dto_6_bx;
extern AUDIO_DTO_7_Bx_T gaudio_dto_7_bx;
extern AUDIO_DTO_8_Bx_T gaudio_dto_8_bx;
extern DATA_BRIDGE_0_Bx_T gdata_bridge_0_bx;
extern DATA_BRIDGE_1_Bx_T gdata_bridge_1_bx;
extern DATA_BRIDGE_2_Bx_T gdata_bridge_2_bx;
extern DATA_BRIDGE_3_Bx_T gdata_bridge_3_bx;
extern DATA_BRIDGE_4_Bx_T gdata_bridge_4_bx;
extern DATA_BRIDGE_5_Bx_T gdata_bridge_5_bx;
extern DATA_BRIDGE_6_Bx_T gdata_bridge_6_bx;
extern DATA_BRIDGE_7_Bx_T gdata_bridge_7_bx;
extern DATA_BRIDGE_8_Bx_T gdata_bridge_8_bx;
extern DATA_BRIDGE_9_Bx_T gdata_bridge_9_bx;
extern DATA_BRIDGE_10_Bx_T gdata_bridge_10_bx;
extern DATA_BRIDGE_11_Bx_T gdata_bridge_11_bx;
extern DATA_BRIDGE_12_Bx_T gdata_bridge_12_bx;
extern DATA_BRIDGE_13_Bx_T gdata_bridge_13_bx;
extern DATA_BRIDGE_14_Bx_T gdata_bridge_14_bx;
extern DATA_BRIDGE_15_Bx_T gdata_bridge_15_bx;
extern DATA_BRIDGE_16_Bx_T gdata_bridge_16_bx;
extern DATA_BRIDGE_17_Bx_T gdata_bridge_17_bx;
extern DATA_BRIDGE_18_Bx_T gdata_bridge_18_bx;
extern DATA_BRIDGE_19_Bx_T gdata_bridge_19_bx;
extern DATA_BRIDGE_20_Bx_T gdata_bridge_20_bx;
extern SOFT_RESET_18_Bx_T gsoft_reset_18_bx;
extern SOFT_RESET_19_Bx_T gsoft_reset_19_bx;
extern DATA_BRIDGE_21_Bx_T gdata_bridge_21_bx;
extern DATA_BRIDGE_22_Bx_T gdata_bridge_22_bx;
extern DATA_BRIDGE_23_Bx_T gdata_bridge_23_bx;
extern DATA_BRIDGE_24_Bx_T gdata_bridge_24_bx;
extern DATA_BRIDGE_25_Bx_T gdata_bridge_25_bx;
extern DATA_BRIDGE_26_Bx_T gdata_bridge_26_bx;
extern DATA_BRIDGE_27_Bx_T gdata_bridge_27_bx;
extern DATA_BRIDGE_28_Bx_T gdata_bridge_28_bx;
extern DATA_BRIDGE_29_Bx_T gdata_bridge_29_bx;
extern DATA_BRIDGE_30_Bx_T gdata_bridge_30_bx;
extern DATA_BRIDGE_31_Bx_T gdata_bridge_31_bx;
extern DATA_BRIDGE_32_Bx_T gdata_bridge_32_bx;
extern DATA_BRIDGE_33_Bx_T gdata_bridge_33_bx;
extern DATA_BRIDGE_34_Bx_T gdata_bridge_34_bx;
extern DATA_BRIDGE_35_Bx_T gdata_bridge_35_bx;
extern DATA_BRIDGE_36_Bx_T gdata_bridge_36_bx;
extern DATA_BRIDGE_37_Bx_T gdata_bridge_37_bx;
extern DATA_BRIDGE_38_Bx_T gdata_bridge_38_bx;
extern DATA_BRIDGE_39_Bx_T gdata_bridge_39_bx;
extern DATA_BRIDGE_40_Bx_T gdata_bridge_40_bx;
extern DATA_BRIDGE_41_Bx_T gdata_bridge_41_bx;
extern DATA_BRIDGE_42_Bx_T gdata_bridge_42_bx;
extern DATA_BRIDGE_43_Bx_T gdata_bridge_43_bx;
extern DATA_BRIDGE_44_Bx_T gdata_bridge_44_bx;
extern DATA_BRIDGE_45_Bx_T gdata_bridge_45_bx;
extern DATA_BRIDGE_46_Bx_T gdata_bridge_46_bx;
extern DATA_BRIDGE_47_Bx_T gdata_bridge_47_bx;
extern DATA_BRIDGE_48_Bx_T gdata_bridge_48_bx;
extern DATA_BRIDGE_49_Bx_T gdata_bridge_49_bx;
extern DATA_BRIDGE_50_Bx_T gdata_bridge_50_bx;
extern DATA_BRIDGE_51_Bx_T gdata_bridge_51_bx;
extern AUD_CLOCK_INV_0_Bx_T gaud_clock_inv_0_bx;
extern HSLVDSTX0_0_Bx_T ghslvdstx0_0_bx;
extern HSLVDSTX0_1_Bx_T ghslvdstx0_1_bx;
extern HSLVDSTX0_2_Bx_T ghslvdstx0_2_bx;
extern HSLVDSTX0_3_Bx_T ghslvdstx0_3_bx;
extern HSLVDSTX0_4_Bx_T ghslvdstx0_4_bx;
extern HSLVDSTX0_5_Bx_T ghslvdstx0_5_bx;
extern HSLVDSTX0_6_Bx_T ghslvdstx0_6_bx;
extern HSLVDSTX1_0_Bx_T ghslvdstx1_0_bx;
extern HSLVDSTX1_1_Bx_T ghslvdstx1_1_bx;
extern HSLVDSTX1_2_Bx_T ghslvdstx1_2_bx;
extern HSLVDSTX1_3_Bx_T ghslvdstx1_3_bx;
extern HSLVDSTX1_4_Bx_T ghslvdstx1_4_bx;
extern HSLVDSTX1_5_Bx_T ghslvdstx1_5_bx;
extern HSLVDSTX1_6_Bx_T ghslvdstx1_6_bx;
extern GBB_AFE_0_Bx_T ggbb_afe_0_bx;
extern GBB_0_Bx_T ggbb_0_bx;
extern AFE_CVBS_0_Bx_T gafe_cvbs_0_bx;
extern AFE_CVBS_1_Bx_T gafe_cvbs_1_bx;
extern AFE_CVBS_2_Bx_T gafe_cvbs_2_bx;
extern AFE_CVBS_3_Bx_T gafe_cvbs_3_bx;
extern AFE_VBUF_0_Bx_T gafe_vbuf_0_bx;
extern AFE_VBUF_1_Bx_T gafe_vbuf_1_bx;
extern AFE_VDAC_0_Bx_T gafe_vdac_0_bx;
extern AFE_VDAC_1_Bx_T gafe_vdac_1_bx;
extern MAIN_PLL_0_Bx_T gmain_pll_0_bx;
extern MAIN_PLL_1_Bx_T gmain_pll_1_bx;
extern MAIN_PLL_2_Bx_T gmain_pll_2_bx;
extern MAIN_PLL_3_Bx_T gmain_pll_3_bx;
extern MAIN_PLL_4_Bx_T gmain_pll_4_bx;
extern AFE_CB_0_Bx_T gafe_cb_0_bx;
extern AFE_CB_1_Bx_T gafe_cb_1_bx;
extern HPD_OUT_CONTROL_0_Bx_T ghpd_out_control_0_bx;
extern HPD_OUT_CONTROL_1_Bx_T ghpd_out_control_1_bx;
extern HPD_OUT_CONTROL_2_Bx_T ghpd_out_control_2_bx;
extern HDMI_PHY0_0_Bx_T ghdmi_phy0_0_bx;
extern HDMI_PHY0_1_Bx_T ghdmi_phy0_1_bx;
extern HDMI_PHY0_2_Bx_T ghdmi_phy0_2_bx;
extern L9A_HDMI0_INTR_0_Bx_T gl9a_hdmi0_intr_0_bx;
extern L9A_HDMI0_INTR_1_Bx_T gl9a_hdmi0_intr_1_bx;
extern L9A_HDMI0_INTR_2_Bx_T gl9a_hdmi0_intr_2_bx;
extern L9A_HDMI0_INTR_3_Bx_T gl9a_hdmi0_intr_3_bx;
extern L9A_HDMI0_INTR_4_Bx_T gl9a_hdmi0_intr_4_bx;
extern L9A_HDMI0_INTR_5_Bx_T gl9a_hdmi0_intr_5_bx;
extern L9A_HDMI0_INTR_6_Bx_T gl9a_hdmi0_intr_6_bx;
extern L9A_HDMI0_INTR_EN_0_Bx_T gl9a_hdmi0_intr_en_0_bx;
extern L9A_HDMI0_INTR_EN_1_Bx_T gl9a_hdmi0_intr_en_1_bx;
extern L9A_HDMI0_INTR_EN_2_Bx_T gl9a_hdmi0_intr_en_2_bx;
extern L9A_HDMI0_INTR_EN_3_Bx_T gl9a_hdmi0_intr_en_3_bx;
extern L9A_HDMI0_INTR_EN_4_Bx_T gl9a_hdmi0_intr_en_4_bx;
extern L9A_HDMI0_INTR_EN_5_Bx_T gl9a_hdmi0_intr_en_5_bx;
extern L9A_HDMI0_INTR_EN_6_Bx_T gl9a_hdmi0_intr_en_6_bx;
extern LINK_0_Bx_T glink_0_bx;
extern LINK_1_Bx_T glink_1_bx;
extern LINK_2_Bx_T glink_2_bx;
extern LINK_3_Bx_T glink_3_bx;
extern LINK_4_Bx_T glink_4_bx;
extern LINK_5_Bx_T glink_5_bx;
extern LINK_6_Bx_T glink_6_bx;
extern LINK_7_Bx_T glink_7_bx;
extern LINK_8_Bx_T glink_8_bx;
extern LINK_9_Bx_T glink_9_bx;
extern LINK_10_Bx_T glink_10_bx;
extern LINK_11_Bx_T glink_11_bx;
extern LINK_12_Bx_T glink_12_bx;
extern LINK_13_Bx_T glink_13_bx;
extern LINK_14_Bx_T glink_14_bx;
extern LINK_15_Bx_T glink_15_bx;
extern LINK_16_Bx_T glink_16_bx;
extern LINK_17_Bx_T glink_17_bx;
extern LINK_18_Bx_T glink_18_bx;
extern LINK_19_Bx_T glink_19_bx;
extern LINK_20_Bx_T glink_20_bx;
extern LINK_21_Bx_T glink_21_bx;
extern LINK_22_Bx_T glink_22_bx;
extern LINK_23_Bx_T glink_23_bx;
extern LINK_24_Bx_T glink_24_bx;
extern LINK_25_Bx_T glink_25_bx;
extern LINK_26_Bx_T glink_26_bx;
extern LINK_27_Bx_T glink_27_bx;
extern LINK_28_Bx_T glink_28_bx;
extern LINK_29_Bx_T glink_29_bx;
extern LINK_30_Bx_T glink_30_bx;
extern LINK_31_Bx_T glink_31_bx;
extern LINK_32_Bx_T glink_32_bx;
extern LINK_33_Bx_T glink_33_bx;
extern LINK_34_Bx_T glink_34_bx;
extern LINK_35_Bx_T glink_35_bx;
extern LINK_36_Bx_T glink_36_bx;
extern LINK_37_Bx_T glink_37_bx;
extern LINK_38_Bx_T glink_38_bx;
extern LINK_39_Bx_T glink_39_bx;
extern LINK_40_Bx_T glink_40_bx;
extern LINK_41_Bx_T glink_41_bx;
extern LINK_42_Bx_T glink_42_bx;
extern LINK_43_Bx_T glink_43_bx;
extern LINK_44_Bx_T glink_44_bx;
extern LINK_45_Bx_T glink_45_bx;
extern LINK_46_Bx_T glink_46_bx;
extern LINK_47_Bx_T glink_47_bx;
extern LINK_48_Bx_T glink_48_bx;
extern LINK_49_Bx_T glink_49_bx;
extern LINK_50_Bx_T glink_50_bx;
extern LINK_51_Bx_T glink_51_bx;
extern LINK_52_Bx_T glink_52_bx;
extern LINK_53_Bx_T glink_53_bx;
extern LINK_54_Bx_T glink_54_bx;
extern LINK_55_Bx_T glink_55_bx;
extern LINK_56_Bx_T glink_56_bx;
extern LINK_57_Bx_T glink_57_bx;
extern LINK_58_Bx_T glink_58_bx;
extern LINK_59_Bx_T glink_59_bx;
extern LINK_60_Bx_T glink_60_bx;
extern LINK_61_Bx_T glink_61_bx;
extern LINK_62_Bx_T glink_62_bx;
extern LINK_63_Bx_T glink_63_bx;
extern LINK_64_Bx_T glink_64_bx;
extern LINK_65_Bx_T glink_65_bx;
extern LINK_66_Bx_T glink_66_bx;
extern LINK_67_Bx_T glink_67_bx;
extern LINK_68_Bx_T glink_68_bx;
extern LINK_69_Bx_T glink_69_bx;
extern LINK_70_Bx_T glink_70_bx;
extern LINK_71_Bx_T glink_71_bx;
extern LINK_72_Bx_T glink_72_bx;
extern LINK_73_Bx_T glink_73_bx;
extern LINK_74_Bx_T glink_74_bx;
extern LINK_75_Bx_T glink_75_bx;
extern LINK_76_Bx_T glink_76_bx;
extern LINK_77_Bx_T glink_77_bx;
extern LINK_78_Bx_T glink_78_bx;
extern LINK_79_Bx_T glink_79_bx;
extern LINK_80_Bx_T glink_80_bx;
extern LINK_81_Bx_T glink_81_bx;
extern LINK_82_Bx_T glink_82_bx;
extern LINK_83_Bx_T glink_83_bx;
extern LINK_84_Bx_T glink_84_bx;
extern LINK_85_Bx_T glink_85_bx;
extern LINK_86_Bx_T glink_86_bx;
extern LINK_87_Bx_T glink_87_bx;
extern LINK_88_Bx_T glink_88_bx;
extern LINK_89_Bx_T glink_89_bx;
extern LINK_90_Bx_T glink_90_bx;
extern LINK_91_Bx_T glink_91_bx;
extern LINK_92_Bx_T glink_92_bx;
extern LINK_93_Bx_T glink_93_bx;
extern LINK_94_Bx_T glink_94_bx;
extern LINK_95_Bx_T glink_95_bx;
extern LINK_96_Bx_T glink_96_bx;
extern LINK_97_Bx_T glink_97_bx;
extern LINK_98_Bx_T glink_98_bx;
extern LINK_99_Bx_T glink_99_bx;
extern LINK_100_Bx_T glink_100_bx;
extern LINK_101_Bx_T glink_101_bx;
extern LINK_102_Bx_T glink_102_bx;
extern LINK_103_Bx_T glink_103_bx;
extern LINK_104_Bx_T glink_104_bx;
extern LINK_105_Bx_T glink_105_bx;
extern LINK_106_Bx_T glink_106_bx;
extern LINK_107_Bx_T glink_107_bx;
extern LINK_108_Bx_T glink_108_bx;
extern LINK_109_Bx_T glink_109_bx;
extern LINK_110_Bx_T glink_110_bx;
extern LINK_111_Bx_T glink_111_bx;
extern LINK_112_Bx_T glink_112_bx;
extern LINK_113_Bx_T glink_113_bx;
extern LINK_114_Bx_T glink_114_bx;
extern LINK_115_Bx_T glink_115_bx;
extern LINK_116_Bx_T glink_116_bx;
extern LINK_117_Bx_T glink_117_bx;
extern LINK_118_Bx_T glink_118_bx;
extern LINK_119_Bx_T glink_119_bx;
extern LINK_120_Bx_T glink_120_bx;
extern LINK_121_Bx_T glink_121_bx;
extern LINK_122_Bx_T glink_122_bx;
extern LINK_123_Bx_T glink_123_bx;
extern LINK_124_Bx_T glink_124_bx;
extern LINK_125_Bx_T glink_125_bx;
extern LINK_126_Bx_T glink_126_bx;
extern LINK_127_Bx_T glink_127_bx;
extern LINK_128_Bx_T glink_128_bx;
extern LINK_129_Bx_T glink_129_bx;
extern LINK_130_Bx_T glink_130_bx;
extern LINK_131_Bx_T glink_131_bx;
extern LINK_132_Bx_T glink_132_bx;
extern LINK_133_Bx_T glink_133_bx;
extern LINK_134_Bx_T glink_134_bx;
extern LINK_135_Bx_T glink_135_bx;
extern LINK_136_Bx_T glink_136_bx;
extern LINK_137_Bx_T glink_137_bx;
extern LINK_138_Bx_T glink_138_bx;
extern LINK_139_Bx_T glink_139_bx;
extern LINK_140_Bx_T glink_140_bx;
extern LINK_141_Bx_T glink_141_bx;
extern LINK_142_Bx_T glink_142_bx;
extern LINK_143_Bx_T glink_143_bx;
extern LINK_144_Bx_T glink_144_bx;
extern LINK_145_Bx_T glink_145_bx;
extern LINK_146_Bx_T glink_146_bx;
extern LINK_147_Bx_T glink_147_bx;
extern LINK_148_Bx_T glink_148_bx;
extern LINK_149_Bx_T glink_149_bx;
extern LINK_150_Bx_T glink_150_bx;
extern LINK_151_Bx_T glink_151_bx;
extern LINK_152_Bx_T glink_152_bx;
extern LINK_153_Bx_T glink_153_bx;
extern LINK_154_Bx_T glink_154_bx;
extern LINK_155_Bx_T glink_155_bx;
extern LINK_156_Bx_T glink_156_bx;
extern LINK_157_Bx_T glink_157_bx;
extern LINK_158_Bx_T glink_158_bx;
extern LINK_159_Bx_T glink_159_bx;
extern LINK_160_Bx_T glink_160_bx;
extern LINK_161_Bx_T glink_161_bx;
extern LINK_162_Bx_T glink_162_bx;
extern LINK_163_Bx_T glink_163_bx;
extern LINK_164_Bx_T glink_164_bx;
extern LINK_165_Bx_T glink_165_bx;
extern LINK_166_Bx_T glink_166_bx;
extern LINK_167_Bx_T glink_167_bx;
extern LINK_168_Bx_T glink_168_bx;
extern LINK_169_Bx_T glink_169_bx;
extern LINK_170_Bx_T glink_170_bx;
extern LINK_171_Bx_T glink_171_bx;
extern LINK_172_Bx_T glink_172_bx;
extern LINK_173_Bx_T glink_173_bx;
extern LINK_174_Bx_T glink_174_bx;
extern LINK_175_Bx_T glink_175_bx;
extern LINK_176_Bx_T glink_176_bx;
extern LINK_177_Bx_T glink_177_bx;
extern LINK_178_Bx_T glink_178_bx;
extern LINK_179_Bx_T glink_179_bx;
extern LINK_180_Bx_T glink_180_bx;
extern LINK_181_Bx_T glink_181_bx;
extern LINK_182_Bx_T glink_182_bx;
extern LINK_183_Bx_T glink_183_bx;
extern LINK_184_Bx_T glink_184_bx;
extern LINK_185_Bx_T glink_185_bx;
extern LINK_186_Bx_T glink_186_bx;
extern LINK_187_Bx_T glink_187_bx;
extern LINK_188_Bx_T glink_188_bx;
extern LINK_189_Bx_T glink_189_bx;
extern LINK_190_Bx_T glink_190_bx;
extern LINK_191_Bx_T glink_191_bx;
extern LINK_192_Bx_T glink_192_bx;
extern LINK_193_Bx_T glink_193_bx;
extern LINK_194_Bx_T glink_194_bx;
extern LINK_195_Bx_T glink_195_bx;
extern LINK_196_Bx_T glink_196_bx;
extern LINK_197_Bx_T glink_197_bx;
extern LINK_198_Bx_T glink_198_bx;
extern LINK_199_Bx_T glink_199_bx;
extern LINK_200_Bx_T glink_200_bx;
extern LINK_201_Bx_T glink_201_bx;
extern LINK_202_Bx_T glink_202_bx;
extern LINK_203_Bx_T glink_203_bx;
extern LINK_204_Bx_T glink_204_bx;
extern LINK_205_Bx_T glink_205_bx;
extern LINK_206_Bx_T glink_206_bx;
extern LINK_207_Bx_T glink_207_bx;
extern LINK_208_Bx_T glink_208_bx;
extern LINK_209_Bx_T glink_209_bx;
extern LINK_210_Bx_T glink_210_bx;
extern LINK_211_Bx_T glink_211_bx;
extern LINK_212_Bx_T glink_212_bx;
extern LINK_213_Bx_T glink_213_bx;
extern LINK_214_Bx_T glink_214_bx;
extern LINK_215_Bx_T glink_215_bx;
extern LINK_216_Bx_T glink_216_bx;
extern LINK_217_Bx_T glink_217_bx;
extern LINK_218_Bx_T glink_218_bx;
extern LINK_219_Bx_T glink_219_bx;
extern LINK_220_Bx_T glink_220_bx;
extern LINK_221_Bx_T glink_221_bx;
extern LINK_222_Bx_T glink_222_bx;
extern LINK_223_Bx_T glink_223_bx;
extern LINK_224_Bx_T glink_224_bx;
extern LINK_225_Bx_T glink_225_bx;
extern LINK_226_Bx_T glink_226_bx;
extern LINK_227_Bx_T glink_227_bx;
extern LINK_228_Bx_T glink_228_bx;
extern LINK_229_Bx_T glink_229_bx;
extern LINK_230_Bx_T glink_230_bx;
extern LINK_231_Bx_T glink_231_bx;
extern LINK_232_Bx_T glink_232_bx;
extern LINK_233_Bx_T glink_233_bx;
extern LINK_234_Bx_T glink_234_bx;
extern LINK_235_Bx_T glink_235_bx;
extern LINK_236_Bx_T glink_236_bx;
extern LINK_237_Bx_T glink_237_bx;
extern LINK_238_Bx_T glink_238_bx;
extern LINK_239_Bx_T glink_239_bx;
extern LINK_240_Bx_T glink_240_bx;
extern LINK_241_Bx_T glink_241_bx;
extern LINK_242_Bx_T glink_242_bx;
extern LINK_243_Bx_T glink_243_bx;
extern LINK_244_Bx_T glink_244_bx;
extern LINK_245_Bx_T glink_245_bx;
extern LINK_246_Bx_T glink_246_bx;
extern LINK_247_Bx_T glink_247_bx;
extern LINK_248_Bx_T glink_248_bx;
extern LINK_249_Bx_T glink_249_bx;
extern LINK_250_Bx_T glink_250_bx;
extern LINK_251_Bx_T glink_251_bx;
extern LINK_252_Bx_T glink_252_bx;
extern LINK_253_Bx_T glink_253_bx;
extern LINK_254_Bx_T glink_254_bx;
extern LINK_255_Bx_T glink_255_bx;
extern LINK_256_Bx_T glink_256_bx;
extern LINK_257_Bx_T glink_257_bx;
extern LINK_258_Bx_T glink_258_bx;
extern LINK_259_Bx_T glink_259_bx;
extern LINK_260_Bx_T glink_260_bx;
extern LINK_261_Bx_T glink_261_bx;
extern LINK_262_Bx_T glink_262_bx;
extern LINK_263_Bx_T glink_263_bx;
extern LINK_264_Bx_T glink_264_bx;
extern LINK_265_Bx_T glink_265_bx;
extern LINK_266_Bx_T glink_266_bx;
extern LINK_267_Bx_T glink_267_bx;
extern LINK_268_Bx_T glink_268_bx;
extern LINK_269_Bx_T glink_269_bx;
extern LINK_270_Bx_T glink_270_bx;
extern LINK_271_Bx_T glink_271_bx;
extern LINK_272_Bx_T glink_272_bx;
extern LINK_273_Bx_T glink_273_bx;
extern LINK_274_Bx_T glink_274_bx;
extern LINK_275_Bx_T glink_275_bx;
extern LINK_276_Bx_T glink_276_bx;
extern LINK_277_Bx_T glink_277_bx;
extern LINK_278_Bx_T glink_278_bx;
extern LINK_279_Bx_T glink_279_bx;
extern LINK_280_Bx_T glink_280_bx;
extern LINK_281_Bx_T glink_281_bx;
extern LINK_282_Bx_T glink_282_bx;
extern LINK_283_Bx_T glink_283_bx;
extern LINK_284_Bx_T glink_284_bx;
extern LINK_285_Bx_T glink_285_bx;
extern LINK_286_Bx_T glink_286_bx;
extern LINK_287_Bx_T glink_287_bx;
extern LINK_288_Bx_T glink_288_bx;
extern LINK_289_Bx_T glink_289_bx;
extern LINK_290_Bx_T glink_290_bx;
extern LINK_291_Bx_T glink_291_bx;
extern LINK_292_Bx_T glink_292_bx;
extern LINK_293_Bx_T glink_293_bx;
extern LINK_294_Bx_T glink_294_bx;
extern LINK_295_Bx_T glink_295_bx;
extern LINK_296_Bx_T glink_296_bx;
extern LINK_297_Bx_T glink_297_bx;
extern LINK_298_Bx_T glink_298_bx;
extern LINK_299_Bx_T glink_299_bx;
extern LINK_300_Bx_T glink_300_bx;
extern LINK_301_Bx_T glink_301_bx;
extern LINK_302_Bx_T glink_302_bx;
extern LINK_303_Bx_T glink_303_bx;
extern LINK_304_Bx_T glink_304_bx;
extern LINK_305_Bx_T glink_305_bx;
extern LINK_306_Bx_T glink_306_bx;
extern LINK_307_Bx_T glink_307_bx;
extern LINK_308_Bx_T glink_308_bx;
extern LINK_309_Bx_T glink_309_bx;
extern LINK_310_Bx_T glink_310_bx;
extern LINK_311_Bx_T glink_311_bx;
extern LINK_312_Bx_T glink_312_bx;
extern LINK_313_Bx_T glink_313_bx;
extern LINK_314_Bx_T glink_314_bx;
extern LINK_315_Bx_T glink_315_bx;
extern LINK_316_Bx_T glink_316_bx;
extern LINK_317_Bx_T glink_317_bx;
extern LINK_318_Bx_T glink_318_bx;
extern LINK_319_Bx_T glink_319_bx;
extern LINK_320_Bx_T glink_320_bx;
extern LINK_321_Bx_T glink_321_bx;
extern LINK_322_Bx_T glink_322_bx;
extern LINK_323_Bx_T glink_323_bx;
extern LINK_324_Bx_T glink_324_bx;
extern LINK_325_Bx_T glink_325_bx;
extern LINK_326_Bx_T glink_326_bx;
extern LINK_327_Bx_T glink_327_bx;
extern LINK_328_Bx_T glink_328_bx;
extern LINK_329_Bx_T glink_329_bx;
extern LINK_330_Bx_T glink_330_bx;
extern LINK_331_Bx_T glink_331_bx;
extern LINK_332_Bx_T glink_332_bx;
extern LINK_333_Bx_T glink_333_bx;
extern LINK_334_Bx_T glink_334_bx;
extern LINK_335_Bx_T glink_335_bx;
extern LINK_336_Bx_T glink_336_bx;
extern LINK_337_Bx_T glink_337_bx;
extern LINK_338_Bx_T glink_338_bx;
extern LINK_339_Bx_T glink_339_bx;
extern LINK_340_Bx_T glink_340_bx;
extern LINK_341_Bx_T glink_341_bx;
extern LINK_342_Bx_T glink_342_bx;
extern LINK_343_Bx_T glink_343_bx;
extern LINK_344_Bx_T glink_344_bx;
extern LINK_345_Bx_T glink_345_bx;
extern LINK_346_Bx_T glink_346_bx;
extern LINK_347_Bx_T glink_347_bx;
extern LINK_348_Bx_T glink_348_bx;
extern LINK_349_Bx_T glink_349_bx;
extern LINK_350_Bx_T glink_350_bx;
extern LINK_351_Bx_T glink_351_bx;
extern LINK_352_Bx_T glink_352_bx;
extern LINK_353_Bx_T glink_353_bx;
extern LINK_354_Bx_T glink_354_bx;
extern LINK_355_Bx_T glink_355_bx;
extern LINK_356_Bx_T glink_356_bx;
extern LINK_357_Bx_T glink_357_bx;
extern LINK_358_Bx_T glink_358_bx;
extern LINK_359_Bx_T glink_359_bx;
extern LINK_360_Bx_T glink_360_bx;
extern LINK_361_Bx_T glink_361_bx;
extern LINK_362_Bx_T glink_362_bx;
extern LINK_363_Bx_T glink_363_bx;
extern LINK_367_Bx_T glink_367_bx;
extern LINK_368_Bx_T glink_368_bx;
extern LINK_369_Bx_T glink_369_bx;
extern LINK_370_Bx_T glink_370_bx;
extern LINK_371_Bx_T glink_371_bx;
extern LINK_372_Bx_T glink_372_bx;
extern LINK_373_Bx_T glink_373_bx;
extern LINK_374_Bx_T glink_374_bx;
extern LINK_375_Bx_T glink_375_bx;
extern LINK_376_Bx_T glink_376_bx;
extern LINK_377_Bx_T glink_377_bx;
extern LINK_378_Bx_T glink_378_bx;
extern LINK_379_Bx_T glink_379_bx;
extern LINK_380_Bx_T glink_380_bx;
extern LINK_381_Bx_T glink_381_bx;
extern LINK_382_Bx_T glink_382_bx;
extern LINK_383_Bx_T glink_383_bx;
extern LINK_384_Bx_T glink_384_bx;
extern LINK_385_Bx_T glink_385_bx;
extern LINK_386_Bx_T glink_386_bx;
extern LINK_387_Bx_T glink_387_bx;
extern LINK_388_Bx_T glink_388_bx;
extern LINK_389_Bx_T glink_389_bx;
extern LINK_390_Bx_T glink_390_bx;
extern LINK_391_Bx_T glink_391_bx;
extern LINK_392_Bx_T glink_392_bx;
extern LINK_393_Bx_T glink_393_bx;
extern LINK_394_Bx_T glink_394_bx;
extern LINK_395_Bx_T glink_395_bx;
extern LINK_396_Bx_T glink_396_bx;
extern LINK_397_Bx_T glink_397_bx;
extern LINK_398_Bx_T glink_398_bx;
extern LINK_399_Bx_T glink_399_bx;
extern LINK_400_Bx_T glink_400_bx;
extern LINK_401_Bx_T glink_401_bx;
extern LINK_402_Bx_T glink_402_bx;
extern LINK_403_Bx_T glink_403_bx;
extern LINK_404_Bx_T glink_404_bx;
extern LINK_808_Bx_T glink_808_bx;
extern LINK_809_Bx_T glink_809_bx;
extern LINK_810_Bx_T glink_810_bx;
extern LINK_811_Bx_T glink_811_bx;
extern LINK_812_Bx_T glink_812_bx;
extern LINK_813_Bx_T glink_813_bx;
extern LINK_814_Bx_T glink_814_bx;
extern LINK_815_Bx_T glink_815_bx;
extern LINK_816_Bx_T glink_816_bx;
extern LINK_817_Bx_T glink_817_bx;
extern LINK_818_Bx_T glink_818_bx;
extern LINK_819_Bx_T glink_819_bx;
extern LINK_820_Bx_T glink_820_bx;
extern LINK_821_Bx_T glink_821_bx;
extern LINK_822_Bx_T glink_822_bx;
extern LINK_823_Bx_T glink_823_bx;
extern LINK_824_Bx_T glink_824_bx;
extern LINK_825_Bx_T glink_825_bx;
extern LINK_826_Bx_T glink_826_bx;
extern LINK_827_Bx_T glink_827_bx;
extern LINK_828_Bx_T glink_828_bx;
extern LINK_829_Bx_T glink_829_bx;
extern LINK_830_Bx_T glink_830_bx;
extern LINK_831_Bx_T glink_831_bx;
extern LINK_832_Bx_T glink_832_bx;
extern LINK_833_Bx_T glink_833_bx;
extern LINK_834_Bx_T glink_834_bx;
extern LINK_835_Bx_T glink_835_bx;
extern LINK_836_Bx_T glink_836_bx;
extern LINK_837_Bx_T glink_837_bx;
extern LINK_838_Bx_T glink_838_bx;
extern LINK_839_Bx_T glink_839_bx;
extern LINK_840_Bx_T glink_840_bx;
extern LINK_841_Bx_T glink_841_bx;
extern LINK_842_Bx_T glink_842_bx;
extern LINK_843_Bx_T glink_843_bx;
extern LINK_844_Bx_T glink_844_bx;
extern LINK_845_Bx_T glink_845_bx;
extern LINK_846_Bx_T glink_846_bx;
extern LINK_847_Bx_T glink_847_bx;
extern LINK_848_Bx_T glink_848_bx;
extern LINK_849_Bx_T glink_849_bx;
extern LINK_850_Bx_T glink_850_bx;
extern LINK_851_Bx_T glink_851_bx;
extern LINK_852_Bx_T glink_852_bx;
extern LINK_853_Bx_T glink_853_bx;
extern LINK_854_Bx_T glink_854_bx;
extern LINK_855_Bx_T glink_855_bx;
extern LINK_856_Bx_T glink_856_bx;
extern LINK_857_Bx_T glink_857_bx;
extern LINK_858_Bx_T glink_858_bx;
extern LINK_859_Bx_T glink_859_bx;
extern LINK_860_Bx_T glink_860_bx;
extern LINK_861_Bx_T glink_861_bx;
extern LINK_862_Bx_T glink_862_bx;
extern LINK_863_Bx_T glink_863_bx;
extern LINK_864_Bx_T glink_864_bx;
extern LINK_865_Bx_T glink_865_bx;
extern LINK_866_Bx_T glink_866_bx;
extern LINK_867_Bx_T glink_867_bx;
extern LINK_868_Bx_T glink_868_bx;
extern LINK_869_Bx_T glink_869_bx;
extern LINK_870_Bx_T glink_870_bx;
extern LINK_871_Bx_T glink_871_bx;
extern LINK_872_Bx_T glink_872_bx;
extern LINK_873_Bx_T glink_873_bx;
extern LINK_874_Bx_T glink_874_bx;
extern LINK_875_Bx_T glink_875_bx;
extern LINK_876_Bx_T glink_876_bx;
extern LINK_877_Bx_T glink_877_bx;
extern LINK_878_Bx_T glink_878_bx;
extern LINK_879_Bx_T glink_879_bx;
extern LINK_880_Bx_T glink_880_bx;
extern LINK_881_Bx_T glink_881_bx;
extern LINK_882_Bx_T glink_882_bx;
extern LINK_883_Bx_T glink_883_bx;
extern LINK_884_Bx_T glink_884_bx;
extern LINK_885_Bx_T glink_885_bx;
extern LINK_886_Bx_T glink_886_bx;
extern LINK_887_Bx_T glink_887_bx;
extern LINK_888_Bx_T glink_888_bx;
extern LINK_889_Bx_T glink_889_bx;
extern LINK_890_Bx_T glink_890_bx;
extern LINK_891_Bx_T glink_891_bx;
extern LINK_892_Bx_T glink_892_bx;
extern LINK_893_Bx_T glink_893_bx;
extern LINK_894_Bx_T glink_894_bx;
extern LINK_895_Bx_T glink_895_bx;
extern LINK_896_Bx_T glink_896_bx;
extern LINK_897_Bx_T glink_897_bx;
extern LINK_898_Bx_T glink_898_bx;
extern LINK_899_Bx_T glink_899_bx;
extern LINK_900_Bx_T glink_900_bx;
extern LINK_901_Bx_T glink_901_bx;
extern LINK_902_Bx_T glink_902_bx;
extern LINK_903_Bx_T glink_903_bx;
extern LINK_904_Bx_T glink_904_bx;
extern LINK_905_Bx_T glink_905_bx;
extern LINK_906_Bx_T glink_906_bx;
extern LINK_907_Bx_T glink_907_bx;
extern LINK_908_Bx_T glink_908_bx;
extern LINK_909_Bx_T glink_909_bx;
extern LINK_910_Bx_T glink_910_bx;
extern LINK_911_Bx_T glink_911_bx;
extern LINK_912_Bx_T glink_912_bx;
extern LINK_913_Bx_T glink_913_bx;
extern LINK_914_Bx_T glink_914_bx;
extern LINK_915_Bx_T glink_915_bx;
extern LINK_916_Bx_T glink_916_bx;
extern LINK_917_Bx_T glink_917_bx;
extern LINK_918_Bx_T glink_918_bx;
extern LINK_919_Bx_T glink_919_bx;
extern LINK_920_Bx_T glink_920_bx;
extern LINK_921_Bx_T glink_921_bx;
extern LINK_922_Bx_T glink_922_bx;
extern LINK_923_Bx_T glink_923_bx;
extern LINK_924_Bx_T glink_924_bx;
extern LINK_925_Bx_T glink_925_bx;
extern LINK_926_Bx_T glink_926_bx;
extern LINK_927_Bx_T glink_927_bx;
extern LINK_928_Bx_T glink_928_bx;
extern LINK_929_Bx_T glink_929_bx;
extern LINK_930_Bx_T glink_930_bx;
extern LINK_931_Bx_T glink_931_bx;
extern LINK_932_Bx_T glink_932_bx;
extern LINK_933_Bx_T glink_933_bx;
extern LINK_934_Bx_T glink_934_bx;
extern LINK_935_Bx_T glink_935_bx;
extern LINK_936_Bx_T glink_936_bx;
extern LINK_937_Bx_T glink_937_bx;
extern LINK_938_Bx_T glink_938_bx;
extern LINK_939_Bx_T glink_939_bx;
extern LINK_940_Bx_T glink_940_bx;
extern LINK_941_Bx_T glink_941_bx;
extern LINK_942_Bx_T glink_942_bx;
extern LINK_943_Bx_T glink_943_bx;
extern LINK_944_Bx_T glink_944_bx;
extern LINK_945_Bx_T glink_945_bx;
extern LINK_946_Bx_T glink_946_bx;
extern LINK_947_Bx_T glink_947_bx;
extern LINK_948_Bx_T glink_948_bx;
extern LINK_949_Bx_T glink_949_bx;
extern LINK_950_Bx_T glink_950_bx;
extern LINK_951_Bx_T glink_951_bx;
extern LINK_952_Bx_T glink_952_bx;
extern LINK_953_Bx_T glink_953_bx;
extern LINK_954_Bx_T glink_954_bx;
extern LINK_955_Bx_T glink_955_bx;
extern LINK_956_Bx_T glink_956_bx;
extern LINK_957_Bx_T glink_957_bx;
extern LINK_958_Bx_T glink_958_bx;
extern LINK_959_Bx_T glink_959_bx;
extern LINK_960_Bx_T glink_960_bx;
extern LINK_961_Bx_T glink_961_bx;
extern LINK_962_Bx_T glink_962_bx;
extern LINK_963_Bx_T glink_963_bx;
extern LINK_964_Bx_T glink_964_bx;
extern LINK_965_Bx_T glink_965_bx;
extern LINK_966_Bx_T glink_966_bx;
extern LINK_967_Bx_T glink_967_bx;
extern LINK_968_Bx_T glink_968_bx;
extern LINK_969_Bx_T glink_969_bx;
extern LINK_970_Bx_T glink_970_bx;
extern LINK_971_Bx_T glink_971_bx;
extern LINK_972_Bx_T glink_972_bx;
extern LINK_973_Bx_T glink_973_bx;
extern LINK_974_Bx_T glink_974_bx;
extern LINK_975_Bx_T glink_975_bx;
extern LINK_976_Bx_T glink_976_bx;
extern LINK_977_Bx_T glink_977_bx;
extern LINK_978_Bx_T glink_978_bx;
extern LINK_979_Bx_T glink_979_bx;
extern LINK_980_Bx_T glink_980_bx;
extern LINK_981_Bx_T glink_981_bx;
extern LINK_982_Bx_T glink_982_bx;
extern LINK_983_Bx_T glink_983_bx;
extern LINK_984_Bx_T glink_984_bx;
extern LINK_985_Bx_T glink_985_bx;
extern LINK_986_Bx_T glink_986_bx;
extern LINK_987_Bx_T glink_987_bx;
extern LINK_988_Bx_T glink_988_bx;
extern LINK_989_Bx_T glink_989_bx;
extern LINK_990_Bx_T glink_990_bx;
extern LINK_991_Bx_T glink_991_bx;
extern LINK_992_Bx_T glink_992_bx;
extern LINK_993_Bx_T glink_993_bx;
extern LINK_994_Bx_T glink_994_bx;
extern LINK_995_Bx_T glink_995_bx;
extern LINK_996_Bx_T glink_996_bx;
extern LINK_997_Bx_T glink_997_bx;
extern LINK_998_Bx_T glink_998_bx;
extern LINK_999_Bx_T glink_999_bx;
extern LINK_1000_Bx_T glink_1000_bx;
extern LINK_1001_Bx_T glink_1001_bx;
extern LINK_1002_Bx_T glink_1002_bx;
extern LINK_1003_Bx_T glink_1003_bx;
extern LINK_1004_Bx_T glink_1004_bx;
extern LINK_1005_Bx_T glink_1005_bx;
extern LINK_1006_Bx_T glink_1006_bx;
extern LINK_1007_Bx_T glink_1007_bx;
extern LINK_1008_Bx_T glink_1008_bx;
extern LINK_1009_Bx_T glink_1009_bx;
extern LINK_1010_Bx_T glink_1010_bx;
extern LINK_1011_Bx_T glink_1011_bx;
extern LINK_1012_Bx_T glink_1012_bx;
extern LINK_1013_Bx_T glink_1013_bx;
extern LINK_1014_Bx_T glink_1014_bx;
extern LINK_1015_Bx_T glink_1015_bx;
extern LINK_1016_Bx_T glink_1016_bx;
extern LINK_1017_Bx_T glink_1017_bx;
extern LINK_1018_Bx_T glink_1018_bx;
extern LINK_1019_Bx_T glink_1019_bx;
extern LINK_1020_Bx_T glink_1020_bx;
extern LINK_1021_Bx_T glink_1021_bx;
extern LINK_1022_Bx_T glink_1022_bx;
extern LINK_1023_Bx_T glink_1023_bx;
extern LINK_1024_Bx_T glink_1024_bx;
extern LINK_1025_Bx_T glink_1025_bx;
extern LINK_1026_Bx_T glink_1026_bx;
extern LINK_1027_Bx_T glink_1027_bx;
extern LINK_1028_Bx_T glink_1028_bx;
extern LINK_1029_Bx_T glink_1029_bx;
extern LINK_1030_Bx_T glink_1030_bx;
extern LINK_1031_Bx_T glink_1031_bx;
extern LINK_1032_Bx_T glink_1032_bx;
extern LINK_1033_Bx_T glink_1033_bx;
extern LINK_1034_Bx_T glink_1034_bx;
extern LINK_1035_Bx_T glink_1035_bx;
extern LINK_1036_Bx_T glink_1036_bx;
extern LINK_1037_Bx_T glink_1037_bx;
extern LINK_1038_Bx_T glink_1038_bx;
extern LINK_1039_Bx_T glink_1039_bx;
extern LINK_1040_Bx_T glink_1040_bx;
extern LINK_1041_Bx_T glink_1041_bx;
extern LINK_1042_Bx_T glink_1042_bx;
extern LINK_1043_Bx_T glink_1043_bx;
extern LINK_1044_Bx_T glink_1044_bx;
extern LINK_1045_Bx_T glink_1045_bx;
extern LINK_1046_Bx_T glink_1046_bx;
extern LINK_1047_Bx_T glink_1047_bx;
extern LINK_1048_Bx_T glink_1048_bx;
extern LINK_1049_Bx_T glink_1049_bx;
extern LINK_1050_Bx_T glink_1050_bx;
extern LINK_1051_Bx_T glink_1051_bx;
extern LINK_1052_Bx_T glink_1052_bx;
extern LINK_1053_Bx_T glink_1053_bx;
extern LINK_1054_Bx_T glink_1054_bx;
extern LINK_1055_Bx_T glink_1055_bx;
extern LINK_1056_Bx_T glink_1056_bx;
extern LINK_1057_Bx_T glink_1057_bx;
extern LINK_1058_Bx_T glink_1058_bx;
extern LINK_1059_Bx_T glink_1059_bx;
extern LINK_1060_Bx_T glink_1060_bx;
extern LINK_1061_Bx_T glink_1061_bx;
extern LINK_1062_Bx_T glink_1062_bx;
extern LINK_1063_Bx_T glink_1063_bx;
extern LINK_1064_Bx_T glink_1064_bx;
extern LINK_1065_Bx_T glink_1065_bx;
extern LINK_1066_Bx_T glink_1066_bx;
extern LINK_1067_Bx_T glink_1067_bx;
extern LINK_1068_Bx_T glink_1068_bx;
extern LINK_1069_Bx_T glink_1069_bx;
extern LINK_1070_Bx_T glink_1070_bx;
extern LINK_1071_Bx_T glink_1071_bx;
extern LINK_1072_Bx_T glink_1072_bx;
extern LINK_1073_Bx_T glink_1073_bx;
extern LINK_1074_Bx_T glink_1074_bx;
extern LINK_1075_Bx_T glink_1075_bx;
extern LINK_1076_Bx_T glink_1076_bx;
extern LINK_1077_Bx_T glink_1077_bx;
extern LINK_1078_Bx_T glink_1078_bx;
extern LINK_1079_Bx_T glink_1079_bx;
extern LINK_1080_Bx_T glink_1080_bx;
extern LINK_1081_Bx_T glink_1081_bx;
extern LINK_1082_Bx_T glink_1082_bx;
extern LINK_1083_Bx_T glink_1083_bx;
extern LINK_1084_Bx_T glink_1084_bx;
extern LINK_1085_Bx_T glink_1085_bx;
extern LINK_1086_Bx_T glink_1086_bx;
extern LINK_1087_Bx_T glink_1087_bx;
extern LINK_1088_Bx_T glink_1088_bx;
extern LINK_1089_Bx_T glink_1089_bx;
extern LINK_1090_Bx_T glink_1090_bx;
extern LINK_1091_Bx_T glink_1091_bx;
extern LINK_1092_Bx_T glink_1092_bx;
extern LINK_1093_Bx_T glink_1093_bx;
extern LINK_1094_Bx_T glink_1094_bx;
extern LINK_1095_Bx_T glink_1095_bx;
extern LINK_1096_Bx_T glink_1096_bx;
extern LINK_1097_Bx_T glink_1097_bx;
extern LINK_1098_Bx_T glink_1098_bx;
extern LINK_1099_Bx_T glink_1099_bx;
extern LINK_1100_Bx_T glink_1100_bx;
extern LINK_1101_Bx_T glink_1101_bx;
extern LINK_1102_Bx_T glink_1102_bx;
extern LINK_1103_Bx_T glink_1103_bx;
extern LINK_1104_Bx_T glink_1104_bx;
extern LINK_1105_Bx_T glink_1105_bx;
extern LINK_1106_Bx_T glink_1106_bx;
extern L9A_3CH_INTR_0_Bx_T gl9a_3ch_intr_0_bx;
extern L9A_3CH_INTR_1_Bx_T gl9a_3ch_intr_1_bx;
extern L9A_3CH_INTR_EN_0_Bx_T gl9a_3ch_intr_en_0_bx;
extern L9A_3CH_INTR_EN_1_Bx_T gl9a_3ch_intr_en_1_bx;
extern LLPLL_0_Bx_T gllpll_0_bx;
extern LLPLL_1_Bx_T gllpll_1_bx;
extern LLPLL_2_Bx_T gllpll_2_bx;
extern LLPLL_3_Bx_T gllpll_3_bx;
extern LLPLL_4_Bx_T gllpll_4_bx;
extern LLPLL_5_Bx_T gllpll_5_bx;
extern LLPLL_6_Bx_T gllpll_6_bx;
extern LLPLL_7_Bx_T gllpll_7_bx;
extern LLPLL_8_Bx_T gllpll_8_bx;
extern LLPLL_9_Bx_T gllpll_9_bx;
extern LLPLL_10_Bx_T gllpll_10_bx;
extern LLPLL_11_Bx_T gllpll_11_bx;
extern LLPLL_12_Bx_T gllpll_12_bx;
extern LLPLL_13_Bx_T gllpll_13_bx;
extern LLPLL_14_Bx_T gllpll_14_bx;
extern LLPLL_15_Bx_T gllpll_15_bx;
extern LLPLL_16_Bx_T gllpll_16_bx;
extern LLPLL_17_Bx_T gllpll_17_bx;
extern LLPLL_18_Bx_T gllpll_18_bx;
extern LLPLL_19_Bx_T gllpll_19_bx;
extern LLPLL_20_Bx_T gllpll_20_bx;
extern LLPLL_21_Bx_T gllpll_21_bx;
extern LLPLL_22_Bx_T gllpll_22_bx;
extern LLPLL_23_Bx_T gllpll_23_bx;
extern LLPLL_24_Bx_T gllpll_24_bx;
extern LLPLL_25_Bx_T gllpll_25_bx;
extern LLPLL_26_Bx_T gllpll_26_bx;
extern LLPLL_27_Bx_T gllpll_27_bx;
extern AFE_3CH_0_Bx_T gafe_3ch_0_bx;
extern AFE_3CH_1_Bx_T gafe_3ch_1_bx;
extern AFE_3CH_2_Bx_T gafe_3ch_2_bx;
extern AFE_3CH_3_Bx_T gafe_3ch_3_bx;
extern AFE_3CH_4_Bx_T gafe_3ch_4_bx;
extern AFE_3CH_5_Bx_T gafe_3ch_5_bx;
extern AFE_3CH_6_Bx_T gafe_3ch_6_bx;
extern AFE_3CH_7_Bx_T gafe_3ch_7_bx;
extern ADC_DIG_0_Bx_T gadc_dig_0_bx;
extern ADC_DIG_1_Bx_T gadc_dig_1_bx;
extern ADC_DIG_2_Bx_T gadc_dig_2_bx;
extern ADC_DIG_3_Bx_T gadc_dig_3_bx;
extern ADC_DIG_4_Bx_T gadc_dig_4_bx;
extern ADC_DIG_5_Bx_T gadc_dig_5_bx;
extern ADC_DIG_6_Bx_T gadc_dig_6_bx;
extern ADC_DIG_7_Bx_T gadc_dig_7_bx;
extern ADC_DIG_8_Bx_T gadc_dig_8_bx;
extern ADC_DIG_9_Bx_T gadc_dig_9_bx;
extern ADC_DIG_10_Bx_T gadc_dig_10_bx;
extern ADC_DIG_11_Bx_T gadc_dig_11_bx;
extern ADC_DIG_12_Bx_T gadc_dig_12_bx;
extern ADC_DIG_13_Bx_T gadc_dig_13_bx;
extern ADC_DIG_14_Bx_T gadc_dig_14_bx;
extern ADC_DIG_15_Bx_T gadc_dig_15_bx;
extern ADC_DIG_16_Bx_T gadc_dig_16_bx;
extern ADC_DIG_17_Bx_T gadc_dig_17_bx;
extern ADC_DIG_18_Bx_T gadc_dig_18_bx;
extern ADC_DIG_19_Bx_T gadc_dig_19_bx;
extern ADC_DIG_20_Bx_T gadc_dig_20_bx;
extern ADC_DIG_21_Bx_T gadc_dig_21_bx;
extern ADC_DIG_22_Bx_T gadc_dig_22_bx;
extern ADC_DIG_23_Bx_T gadc_dig_23_bx;
extern ADC_DIG_24_Bx_T gadc_dig_24_bx;
extern ADC_DIG_25_Bx_T gadc_dig_25_bx;
extern ADC_DIG_26_Bx_T gadc_dig_26_bx;
extern ADC_DIG_27_Bx_T gadc_dig_27_bx;
extern ADC_DIG_28_Bx_T gadc_dig_28_bx;
extern ADC_DIG_29_Bx_T gadc_dig_29_bx;
extern ADC_DIG_30_Bx_T gadc_dig_30_bx;
extern ADC_DIG_31_Bx_T gadc_dig_31_bx;
extern ADC_DIG_32_Bx_T gadc_dig_32_bx;
extern ADC_DIG_33_Bx_T gadc_dig_33_bx;
extern ADC_DIG_34_Bx_T gadc_dig_34_bx;
extern ADC_DIG_35_Bx_T gadc_dig_35_bx;
extern ADC_DIG_36_Bx_T gadc_dig_36_bx;
extern ADC_DIG_37_Bx_T gadc_dig_37_bx;
extern ADC_DIG_38_Bx_T gadc_dig_38_bx;
extern ADC_DIG_39_Bx_T gadc_dig_39_bx;
extern ADC_DIG_40_Bx_T gadc_dig_40_bx;
extern ADC_DIG_41_Bx_T gadc_dig_41_bx;
extern ADC_DIG_42_Bx_T gadc_dig_42_bx;
extern ADC_DIG_43_Bx_T gadc_dig_43_bx;
extern ADC_DIG_44_Bx_T gadc_dig_44_bx;
extern ADC_DIG_45_Bx_T gadc_dig_45_bx;
extern ADC_DIG_46_Bx_T gadc_dig_46_bx;
extern ADC_DIG_47_Bx_T gadc_dig_47_bx;
extern ADC_DIG_48_Bx_T gadc_dig_48_bx;
extern ADC_DIG_49_Bx_T gadc_dig_49_bx;
extern ADC_DIG_50_Bx_T gadc_dig_50_bx;
extern ADC_DIG_51_Bx_T gadc_dig_51_bx;
extern ADC_DIG_52_Bx_T gadc_dig_52_bx;
extern ADC_DIG_53_Bx_T gadc_dig_53_bx;
extern ADC_DIG_54_Bx_T gadc_dig_54_bx;
extern ADC_DIG_55_Bx_T gadc_dig_55_bx;
extern ADC_DIG_56_Bx_T gadc_dig_56_bx;
extern ADC_DIG_57_Bx_T gadc_dig_57_bx;
extern ADC_DIG_58_Bx_T gadc_dig_58_bx;
extern ADC_DIG_59_Bx_T gadc_dig_59_bx;
extern ADC_DIG_60_Bx_T gadc_dig_60_bx;
extern ADC_DIG_61_Bx_T gadc_dig_61_bx;
extern ADC_DIG_62_Bx_T gadc_dig_62_bx;
extern ADC_DIG_63_Bx_T gadc_dig_63_bx;
extern ADC_DIG_64_Bx_T gadc_dig_64_bx;
extern ADC_DIG_65_Bx_T gadc_dig_65_bx;
extern ADC_DIG_66_Bx_T gadc_dig_66_bx;
extern ADC_DIG_67_Bx_T gadc_dig_67_bx;
extern ADC_DIG_68_Bx_T gadc_dig_68_bx;
extern ADC_DIG_69_Bx_T gadc_dig_69_bx;
extern ADC_DIG_70_Bx_T gadc_dig_70_bx;
extern ADC_DIG_71_Bx_T gadc_dig_71_bx;
extern ADC_DIG_72_Bx_T gadc_dig_72_bx;
extern ADC_DIG_73_Bx_T gadc_dig_73_bx;
extern ADC_DIG_74_Bx_T gadc_dig_74_bx;
extern ADC_DIG_75_Bx_T gadc_dig_75_bx;
extern ADC_DIG_76_Bx_T gadc_dig_76_bx;
extern ADC_DIG_77_Bx_T gadc_dig_77_bx;
extern ADC_DIG_78_Bx_T gadc_dig_78_bx;
extern ADC_DIG_79_Bx_T gadc_dig_79_bx;
extern ADC_DIG_80_Bx_T gadc_dig_80_bx;
extern ADC_DIG_81_Bx_T gadc_dig_81_bx;
extern ADC_DIG_82_Bx_T gadc_dig_82_bx;
extern ADC_DIG_83_Bx_T gadc_dig_83_bx;
extern ADC_DIG_84_Bx_T gadc_dig_84_bx;
extern ADC_DIG_85_Bx_T gadc_dig_85_bx;
extern ADC_DIG_86_Bx_T gadc_dig_86_bx;
extern ADC_DIG_87_Bx_T gadc_dig_87_bx;
extern ADC_DIG_88_Bx_T gadc_dig_88_bx;
extern ADC_DIG_89_Bx_T gadc_dig_89_bx;
extern ADC_DIG_90_Bx_T gadc_dig_90_bx;
extern ADC_DIG_91_Bx_T gadc_dig_91_bx;
extern ADC_DIG_92_Bx_T gadc_dig_92_bx;
extern ADC_DIG_93_Bx_T gadc_dig_93_bx;
extern ADC_DIG_94_Bx_T gadc_dig_94_bx;
extern ADC_DIG_95_Bx_T gadc_dig_95_bx;
extern ADC_DIG_96_Bx_T gadc_dig_96_bx;
extern ADC_DIG_97_Bx_T gadc_dig_97_bx;
extern ADC_DIG_98_Bx_T gadc_dig_98_bx;
extern ADC_DIG_99_Bx_T gadc_dig_99_bx;
extern ADC_DIG_100_Bx_T gadc_dig_100_bx;
extern ADC_DIG_101_Bx_T gadc_dig_101_bx;
extern ADC_DIG_102_Bx_T gadc_dig_102_bx;
extern ADC_DIG_103_Bx_T gadc_dig_103_bx;
extern ADC_DIG_104_Bx_T gadc_dig_104_bx;
extern ADC_DIG_105_Bx_T gadc_dig_105_bx;
extern ADC_DIG_106_Bx_T gadc_dig_106_bx;
extern ADC_DIG_107_Bx_T gadc_dig_107_bx;
extern ADC_DIG_108_Bx_T gadc_dig_108_bx;
extern ADC_DIG_109_Bx_T gadc_dig_109_bx;
extern ADC_DIG_110_Bx_T gadc_dig_110_bx;
extern ADC_DIG_111_Bx_T gadc_dig_111_bx;
extern ADC_DIG_112_Bx_T gadc_dig_112_bx;
extern ADC_DIG_113_Bx_T gadc_dig_113_bx;
extern ADC_DIG_114_Bx_T gadc_dig_114_bx;
extern ADC_DIG_115_Bx_T gadc_dig_115_bx;
extern ADC_DIG_116_Bx_T gadc_dig_116_bx;
extern ADC_DIG_117_Bx_T gadc_dig_117_bx;
extern ADC_DIG_118_Bx_T gadc_dig_118_bx;
extern ADC_DIG_119_Bx_T gadc_dig_119_bx;
extern ADC_DIG_120_Bx_T gadc_dig_120_bx;
extern ADC_DIG_121_Bx_T gadc_dig_121_bx;
extern ADC_DIG_122_Bx_T gadc_dig_122_bx;
extern ADC_DIG_123_Bx_T gadc_dig_123_bx;
extern ADC_DIG_124_Bx_T gadc_dig_124_bx;
extern ADC_DIG_125_Bx_T gadc_dig_125_bx;
extern ADC_DIG_126_Bx_T gadc_dig_126_bx;
extern ADC_DIG_127_Bx_T gadc_dig_127_bx;
extern ADC_DIG_128_Bx_T gadc_dig_128_bx;
extern ADC_DIG_129_Bx_T gadc_dig_129_bx;
extern ADC_DIG_130_Bx_T gadc_dig_130_bx;
extern ADC_DIG_131_Bx_T gadc_dig_131_bx;
extern ADC_DIG_132_Bx_T gadc_dig_132_bx;
extern ADC_DIG_133_Bx_T gadc_dig_133_bx;
extern ADC_DIG_134_Bx_T gadc_dig_134_bx;
extern ADC_DIG_135_Bx_T gadc_dig_135_bx;
extern ADC_DIG_136_Bx_T gadc_dig_136_bx;
extern ADC_DIG_137_Bx_T gadc_dig_137_bx;
extern ADC_DIG_138_Bx_T gadc_dig_138_bx;
extern ADC_DIG_139_Bx_T gadc_dig_139_bx;
extern ADC_DIG_140_Bx_T gadc_dig_140_bx;
extern ADC_DIG_141_Bx_T gadc_dig_141_bx;
extern ADC_DIG_142_Bx_T gadc_dig_142_bx;
extern ADC_DIG_143_Bx_T gadc_dig_143_bx;
extern ADC_DIG_144_Bx_T gadc_dig_144_bx;
extern ADC_DIG_145_Bx_T gadc_dig_145_bx;
extern ADC_DIG_146_Bx_T gadc_dig_146_bx;
extern ADC_DIG_147_Bx_T gadc_dig_147_bx;
extern ADC_DIG_148_Bx_T gadc_dig_148_bx;
extern ADC_DIG_149_Bx_T gadc_dig_149_bx;
extern ADC_DIG_150_Bx_T gadc_dig_150_bx;
extern ADC_DIG_151_Bx_T gadc_dig_151_bx;
extern ADC_DIG_152_Bx_T gadc_dig_152_bx;
extern ADC_DIG_153_Bx_T gadc_dig_153_bx;
extern ADC_DIG_154_Bx_T gadc_dig_154_bx;
extern ADC_DIG_155_Bx_T gadc_dig_155_bx;
extern ADC_DIG_156_Bx_T gadc_dig_156_bx;
extern ADC_DIG_157_Bx_T gadc_dig_157_bx;
extern ADC_DIG_158_Bx_T gadc_dig_158_bx;
extern ADC_DIG_159_Bx_T gadc_dig_159_bx;
extern ADC_DIG_160_Bx_T gadc_dig_160_bx;
extern ADC_DIG_161_Bx_T gadc_dig_161_bx;
extern ADC_DIG_162_Bx_T gadc_dig_162_bx;
extern ADC_DIG_163_Bx_T gadc_dig_163_bx;
extern ADC_DIG_164_Bx_T gadc_dig_164_bx;
extern ADC_DIG_165_Bx_T gadc_dig_165_bx;
extern ADC_DIG_166_Bx_T gadc_dig_166_bx;
extern ADC_DIG_167_Bx_T gadc_dig_167_bx;
extern ADC_DIG_168_Bx_T gadc_dig_168_bx;
extern ADC_DIG_169_Bx_T gadc_dig_169_bx;
extern ADC_DIG_170_Bx_T gadc_dig_170_bx;
extern ADC_DIG_171_Bx_T gadc_dig_171_bx;
extern ADC_DIG_172_Bx_T gadc_dig_172_bx;
extern ADC_DIG_173_Bx_T gadc_dig_173_bx;
extern ADC_DIG_174_Bx_T gadc_dig_174_bx;
extern ADC_DIG_175_Bx_T gadc_dig_175_bx;
extern ADC_DIG_176_Bx_T gadc_dig_176_bx;
extern ADC_DIG_177_Bx_T gadc_dig_177_bx;
extern ADC_DIG_178_Bx_T gadc_dig_178_bx;
extern ADC_DIG_179_Bx_T gadc_dig_179_bx;
extern ADC_DIG_180_Bx_T gadc_dig_180_bx;
extern ADC_DIG_181_Bx_T gadc_dig_181_bx;
extern ADC_DIG_182_Bx_T gadc_dig_182_bx;
extern ADC_DIG_183_Bx_T gadc_dig_183_bx;
extern ADC_DIG_184_Bx_T gadc_dig_184_bx;
extern ADC_DIG_185_Bx_T gadc_dig_185_bx;
extern ADC_DIG_186_Bx_T gadc_dig_186_bx;
extern ADC_DIG_187_Bx_T gadc_dig_187_bx;
extern ADC_DIG_188_Bx_T gadc_dig_188_bx;
extern ADC_DIG_189_Bx_T gadc_dig_189_bx;
extern ADC_DIG_190_Bx_T gadc_dig_190_bx;
extern ADC_DIG_191_Bx_T gadc_dig_191_bx;
extern ADC_DIG_192_Bx_T gadc_dig_192_bx;
extern ADC_DIG_193_Bx_T gadc_dig_193_bx;
extern ADC_DIG_194_Bx_T gadc_dig_194_bx;
extern ADC_DIG_195_Bx_T gadc_dig_195_bx;
extern ADC_DIG_196_Bx_T gadc_dig_196_bx;
extern ADC_DIG_197_Bx_T gadc_dig_197_bx;
extern ADC_DIG_198_Bx_T gadc_dig_198_bx;
extern ADC_DIG_199_Bx_T gadc_dig_199_bx;
extern ADC_DIG_200_Bx_T gadc_dig_200_bx;
extern ADC_DIG_201_Bx_T gadc_dig_201_bx;
extern ADC_DIG_202_Bx_T gadc_dig_202_bx;
extern ADC_DIG_203_Bx_T gadc_dig_203_bx;
extern ADC_DIG_204_Bx_T gadc_dig_204_bx;
extern ADC_DIG_205_Bx_T gadc_dig_205_bx;
extern ADC_DIG_206_Bx_T gadc_dig_206_bx;
extern ADC_DIG_207_Bx_T gadc_dig_207_bx;
extern ADC_DIG_208_Bx_T gadc_dig_208_bx;
extern ADC_DIG_209_Bx_T gadc_dig_209_bx;
extern ADC_DIG_210_Bx_T gadc_dig_210_bx;
extern ADC_DIG_211_Bx_T gadc_dig_211_bx;
extern ADC_DIG_212_Bx_T gadc_dig_212_bx;
extern ADC_DIG_213_Bx_T gadc_dig_213_bx;
extern ADC_DIG_214_Bx_T gadc_dig_214_bx;
extern ADC_DIG_215_Bx_T gadc_dig_215_bx;
extern ADC_DIG_216_Bx_T gadc_dig_216_bx;
extern ADC_DIG_217_Bx_T gadc_dig_217_bx;
extern ADC_DIG_218_Bx_T gadc_dig_218_bx;
extern ADC_DIG_219_Bx_T gadc_dig_219_bx;
extern ADC_DIG_220_Bx_T gadc_dig_220_bx;
extern ADC_DIG_221_Bx_T gadc_dig_221_bx;
extern ADC_DIG_222_Bx_T gadc_dig_222_bx;
extern ADC_DIG_223_Bx_T gadc_dig_223_bx;
extern ADC_DIG_224_Bx_T gadc_dig_224_bx;
extern ADC_DIG_225_Bx_T gadc_dig_225_bx;
extern ADC_DIG_226_Bx_T gadc_dig_226_bx;
extern ADC_DIG_227_Bx_T gadc_dig_227_bx;
extern ADC_DIG_228_Bx_T gadc_dig_228_bx;
extern ADC_DIG_229_Bx_T gadc_dig_229_bx;
extern ADC_DIG_230_Bx_T gadc_dig_230_bx;
extern ADC_DIG_231_Bx_T gadc_dig_231_bx;
extern ADC_DIG_232_Bx_T gadc_dig_232_bx;
extern ADC_DIG_233_Bx_T gadc_dig_233_bx;
extern ADC_DIG_234_Bx_T gadc_dig_234_bx;
extern ADC_DIG_235_Bx_T gadc_dig_235_bx;
extern ADC_DIG_236_Bx_T gadc_dig_236_bx;
extern ADC_DIG_237_Bx_T gadc_dig_237_bx;
extern ADC_DIG_238_Bx_T gadc_dig_238_bx;
extern ADC_DIG_239_Bx_T gadc_dig_239_bx;
extern ADC_DIG_240_Bx_T gadc_dig_240_bx;
/*
extern ADC_DIG_241_Bx_T gadc_dig_241_bx;
extern ADC_DIG_242_Bx_T gadc_dig_242_bx;
*/
extern ADC_DIG_243_Bx_T gadc_dig_243_bx;
extern ADC_DIG_244_Bx_T gadc_dig_244_bx;
extern ADC_DIG_245_Bx_T gadc_dig_245_bx;
extern ADC_DIG_246_Bx_T gadc_dig_246_bx;
extern ADC_DIG_247_Bx_T gadc_dig_247_bx;
extern ADC_DIG_248_Bx_T gadc_dig_248_bx;
extern ADC_DIG_249_Bx_T gadc_dig_249_bx;
extern ADC_DIG_250_Bx_T gadc_dig_250_bx;
extern ADC_DIG_251_Bx_T gadc_dig_251_bx;
extern ADC_DIG_252_Bx_T gadc_dig_252_bx;
extern ADC_DIG_253_Bx_T gadc_dig_253_bx;
extern ADC_DIG_254_Bx_T gadc_dig_254_bx;
extern ADC_DIG_255_Bx_T gadc_dig_255_bx;
extern ADC_DIG_256_Bx_T gadc_dig_256_bx;
extern ADC_DIG_257_Bx_T gadc_dig_257_bx;
extern ADC_DIG_258_Bx_T gadc_dig_258_bx;
extern ADC_DIG_259_Bx_T gadc_dig_259_bx;
extern ADC_DIG_260_Bx_T gadc_dig_260_bx;
extern ADC_DIG_261_Bx_T gadc_dig_261_bx;
extern ADC_DIG_262_Bx_T gadc_dig_262_bx;
extern ADC_DIG_263_Bx_T gadc_dig_263_bx;
extern ADC_DIG_264_Bx_T gadc_dig_264_bx;
extern ADC_DIG_265_Bx_T gadc_dig_265_bx;
extern ADC_DIG_266_Bx_T gadc_dig_266_bx;
extern ADC_DIG_267_Bx_T gadc_dig_267_bx;
extern ADC_DIG_268_Bx_T gadc_dig_268_bx;
extern ADC_DIG_269_Bx_T gadc_dig_269_bx;
extern ADC_DIG_270_Bx_T gadc_dig_270_bx;
extern ADC_DIG_271_Bx_T gadc_dig_271_bx;
extern ADC_DIG_272_Bx_T gadc_dig_272_bx;
extern ADC_DIG_273_Bx_T gadc_dig_273_bx;
extern ADC_DIG_274_Bx_T gadc_dig_274_bx;
extern ADC_DIG_275_Bx_T gadc_dig_275_bx;
extern ADC_DIG_276_Bx_T gadc_dig_276_bx;
extern ADC_DIG_277_Bx_T gadc_dig_277_bx;
extern ADC_DIG_278_Bx_T gadc_dig_278_bx;
extern ADC_DIG_279_Bx_T gadc_dig_279_bx;
//extern ADC_DIG_280_Bx_T gadc_dig_280_bx;
//extern ADC_DIG_281_Bx_T gadc_dig_281_bx;
extern ADC_DIG_282_Bx_T gadc_dig_282_bx;
extern ADC_DIG_283_Bx_T gadc_dig_283_bx;
extern ADC_DIG_284_Bx_T gadc_dig_284_bx;
extern ADC_DIG_285_Bx_T gadc_dig_285_bx;
extern ADC_DIG_286_Bx_T gadc_dig_286_bx;
extern ADC_DIG_287_Bx_T gadc_dig_287_bx;
extern ADC_DIG_288_Bx_T gadc_dig_288_bx;
extern ADC_DIG_289_Bx_T gadc_dig_289_bx;
extern ADC_DIG_290_Bx_T gadc_dig_290_bx;
extern ADC_DIG_291_Bx_T gadc_dig_291_bx;
extern ADC_DIG_292_Bx_T gadc_dig_292_bx;
extern ADC_DIG_293_Bx_T gadc_dig_293_bx;
extern ADC_DIG_294_Bx_T gadc_dig_294_bx;
extern ADC_DIG_295_Bx_T gadc_dig_295_bx;
extern ADC_DIG_296_Bx_T gadc_dig_296_bx;
extern ADC_DIG_297_Bx_T gadc_dig_297_bx;
extern ADC_DIG_298_Bx_T gadc_dig_298_bx;
extern AUDIO_PLL_0_Bx_T gaudio_pll_0_bx;
extern AUDIO_PLL_1_Bx_T gaudio_pll_1_bx;
extern AUDIO_PLL_2_Bx_T gaudio_pll_2_bx;
extern AUDIO_PLL_3_Bx_T gaudio_pll_3_bx;
extern AUDIO_PLL_4_Bx_T gaudio_pll_4_bx;
extern AUDIO_PLL_5_Bx_T gaudio_pll_5_bx;
extern AUDIO_PLL_6_Bx_T gaudio_pll_6_bx;
extern AFE_AAD_0_Bx_T gafe_aad_0_bx;
extern AFE_AAD_1_Bx_T gafe_aad_1_bx;
extern AFE_AAD_2_Bx_T gafe_aad_2_bx;
extern AFE_AAD_3_Bx_T gafe_aad_3_bx;
extern AFE_AAD_4_Bx_T gafe_aad_4_bx;
extern AFE_AAD_5_Bx_T gafe_aad_5_bx;
extern AFE_AAD_6_Bx_T gafe_aad_6_bx;
extern AFE_AAD_7_Bx_T gafe_aad_7_bx;
extern AFE_AAD_8_Bx_T gafe_aad_8_bx;
extern AFE_ACODEC_0_Bx_T gafe_acodec_0_bx;
extern AFE_ACODEC_1_Bx_T gafe_acodec_1_bx;
extern AFE_ACODEC_2_Bx_T gafe_acodec_2_bx;
extern AFE_ACODEC_3_Bx_T gafe_acodec_3_bx;
extern AFE_ACODEC_4_Bx_T gafe_acodec_4_bx;
extern AFE_ACODEC_5_Bx_T gafe_acodec_5_bx;
extern ACODEC_0_Bx_T gacodec_0_bx;
extern ACODEC_1_Bx_T gacodec_1_bx;
extern ACODEC_2_Bx_T gacodec_2_bx;
extern ACODEC_3_Bx_T gacodec_3_bx;
extern ACODEC_4_Bx_T gacodec_4_bx;
extern ACODEC_5_Bx_T gacodec_5_bx;
extern ACODEC_6_Bx_T gacodec_6_bx;
extern ACODEC_7_Bx_T gacodec_7_bx;
extern ACODEC_8_Bx_T gacodec_8_bx;
extern ACODEC_9_Bx_T gacodec_9_bx;
extern ACODEC_10_Bx_T gacodec_10_bx;
extern ACODEC_11_Bx_T gacodec_11_bx;
extern ACODEC_12_Bx_T gacodec_12_bx;
extern ACODEC_13_Bx_T gacodec_13_bx;
extern ACODEC_14_Bx_T gacodec_14_bx;
extern ACODEC_15_Bx_T gacodec_15_bx;
extern ACODEC_16_Bx_T gacodec_16_bx;
extern ACODEC_17_Bx_T gacodec_17_bx;
extern ACODEC_18_Bx_T gacodec_18_bx;
extern ACODEC_19_Bx_T gacodec_19_bx;
extern ACODEC_20_Bx_T gacodec_20_bx;
extern ACODEC_21_Bx_T gacodec_21_bx;
extern ACODEC_31_Bx_T gacodec_31_bx;
extern ACODEC_32_Bx_T gacodec_32_bx;
extern ACODEC_33_Bx_T gacodec_33_bx;
extern ACODEC_34_Bx_T gacodec_34_bx;
extern ACODEC_35_Bx_T gacodec_35_bx;
extern ACODEC_36_Bx_T gacodec_36_bx;
extern ACODEC_37_Bx_T gacodec_37_bx;
extern ACODEC_38_Bx_T gacodec_38_bx;
extern ACODEC_39_Bx_T gacodec_39_bx;
extern ACODEC_40_Bx_T gacodec_40_bx;
extern ACODEC_41_Bx_T gacodec_41_bx;
extern ACODEC_42_Bx_T gacodec_42_bx;
extern ACODEC_43_Bx_T gacodec_43_bx;
extern ACODEC_44_Bx_T gacodec_44_bx;
extern ACODEC_45_Bx_T gacodec_45_bx;
extern ACODEC_46_Bx_T gacodec_46_bx;
extern ACODEC_47_Bx_T gacodec_47_bx;
extern HDMI_PHY_N_01_Bx_T ghdmi_phy_n_01_bx;
extern HDMI_PHY_N_02_Bx_T ghdmi_phy_n_02_bx;
extern HDMI_PHY_N_03_Bx_T ghdmi_phy_n_03_bx;
extern HDMI_PHY_N_04_Bx_T ghdmi_phy_n_04_bx;
extern HDMI_PHY_N_05_Bx_T ghdmi_phy_n_05_bx;
extern HDMI_PHY_N_06_Bx_T ghdmi_phy_n_06_bx;
extern HDMI_PHY_N_07_Bx_T ghdmi_phy_n_07_bx;
extern HDMI_PHY_N_08_Bx_T ghdmi_phy_n_08_bx;
extern HDMI_PHY_N_09_Bx_T ghdmi_phy_n_09_bx;
extern HDMI_PHY_N_0A_Bx_T ghdmi_phy_n_0a_bx;
extern HDMI_PHY_N_10_Bx_T ghdmi_phy_n_10_bx;
extern HDMI_PHY_N_11_Bx_T ghdmi_phy_n_11_bx;
extern HDMI_PHY_N_12_Bx_T ghdmi_phy_n_12_bx;
extern HDMI_PHY_N_13_Bx_T ghdmi_phy_n_13_bx;
extern HDMI_PHY_N_14_Bx_T ghdmi_phy_n_14_bx;
extern HDMI_PHY_N_15_Bx_T ghdmi_phy_n_15_bx;
extern HDMI_PHY_N_16_Bx_T ghdmi_phy_n_16_bx;
extern HDMI_PHY_N_17_Bx_T ghdmi_phy_n_17_bx;
extern HDMI_PHY_N_18_Bx_T ghdmi_phy_n_18_bx;
extern HDMI_PHY_N_19_Bx_T ghdmi_phy_n_19_bx;
extern HDMI_PHY_N_1A_Bx_T ghdmi_phy_n_1a_bx;
extern HDMI_PHY_N_1B_Bx_T ghdmi_phy_n_1b_bx;
extern HDMI_PHY_N_1C_Bx_T ghdmi_phy_n_1c_bx;
extern HDMI_PHY_N_1D_Bx_T ghdmi_phy_n_1d_bx;
extern HDMI_PHY_N_1E_Bx_T ghdmi_phy_n_1e_bx;
extern HDMI_PHY_N_1F_Bx_T ghdmi_phy_n_1f_bx;
extern HDMI_PHY_N_20_Bx_T ghdmi_phy_n_20_bx;
extern HDMI_PHY_N_21_Bx_T ghdmi_phy_n_21_bx;
extern HDMI_PHY_N_22_Bx_T ghdmi_phy_n_22_bx;
extern HDMI_PHY_N_23_Bx_T ghdmi_phy_n_23_bx;
extern HDMI_PHY_N_24_Bx_T ghdmi_phy_n_24_bx;
extern HDMI_PHY_N_25_Bx_T ghdmi_phy_n_25_bx;
extern HDMI_PHY_N_26_Bx_T ghdmi_phy_n_26_bx;
extern HDMI_PHY_N_30_Bx_T ghdmi_phy_n_30_bx;
extern HDMI_PHY_N_31_Bx_T ghdmi_phy_n_31_bx;
extern HDMI_PHY_N_32_Bx_T ghdmi_phy_n_32_bx;
extern HDMI_PHY_N_33_Bx_T ghdmi_phy_n_33_bx;
extern HDMI_PHY_N_34_Bx_T ghdmi_phy_n_34_bx;
extern HDMI_PHY_N_35_Bx_T ghdmi_phy_n_35_bx;
extern HDMI_PHY_N_36_Bx_T ghdmi_phy_n_36_bx;
extern HDMI_PHY_N_40_Bx_T ghdmi_phy_n_40_bx;
extern HDMI_PHY_N_41_Bx_T ghdmi_phy_n_41_bx;
extern HDMI_PHY_N_42_Bx_T ghdmi_phy_n_42_bx;
extern HDMI_PHY_N_43_Bx_T ghdmi_phy_n_43_bx;
extern HDMI_PHY_N_44_Bx_T ghdmi_phy_n_44_bx;
extern HDMI_PHY_N_45_Bx_T ghdmi_phy_n_45_bx;
extern HDMI_PHY_N_46_Bx_T ghdmi_phy_n_46_bx;
extern HDMI_PHY_N_47_Bx_T ghdmi_phy_n_47_bx;
extern HDMI_PHY_N_48_Bx_T ghdmi_phy_n_48_bx;
extern HDMI_PHY_N_49_Bx_T ghdmi_phy_n_49_bx;
extern HDMI_PHY_N_4A_Bx_T ghdmi_phy_n_4a_bx;
extern HDMI_PHY_N_4B_Bx_T ghdmi_phy_n_4b_bx;
extern HDMI_PHY_N_4C_Bx_T ghdmi_phy_n_4c_bx;
extern HDMI_PHY_N_4D_Bx_T ghdmi_phy_n_4d_bx;
extern HDMI_PHY_N_4E_Bx_T ghdmi_phy_n_4e_bx;
extern HDMI_PHY_N_4F_Bx_T ghdmi_phy_n_4f_bx;
extern HDMI_PHY_N_50_Bx_T ghdmi_phy_n_50_bx;
extern HDMI_PHY_N_51_Bx_T ghdmi_phy_n_51_bx;
extern HDMI_PHY_N_52_Bx_T ghdmi_phy_n_52_bx;
extern HDMI_PHY_N_53_Bx_T ghdmi_phy_n_53_bx;
extern HDMI_PHY_N_54_Bx_T ghdmi_phy_n_54_bx;
extern HDMI_PHY_N_80_Bx_T ghdmi_phy_n_80_bx;
extern HDMI_PHY_N_81_Bx_T ghdmi_phy_n_81_bx;
extern HDMI_PHY_N_82_Bx_T ghdmi_phy_n_82_bx;
extern HDMI_PHY_N_83_Bx_T ghdmi_phy_n_83_bx;
extern HDMI_PHY_N_84_Bx_T ghdmi_phy_n_84_bx;
extern HDMI_PHY_N_85_Bx_T ghdmi_phy_n_85_bx;
extern HDMI_PHY_N_86_Bx_T ghdmi_phy_n_86_bx;
extern HDMI_PHY_N_87_Bx_T ghdmi_phy_n_87_bx;
extern HDMI_PHY_N_88_Bx_T ghdmi_phy_n_88_bx;
extern HDMI_PHY_N_89_Bx_T ghdmi_phy_n_89_bx;
extern HDMI_PHY_N_8A_Bx_T ghdmi_phy_n_8a_bx;
extern HDMI_PHY_N_8B_Bx_T ghdmi_phy_n_8b_bx;
extern HDMI_PHY_N_8C_Bx_T ghdmi_phy_n_8c_bx;
extern HDMI_PHY_N_8D_Bx_T ghdmi_phy_n_8d_bx;
extern HDMI_PHY_N_8E_Bx_T ghdmi_phy_n_8e_bx;
extern HDMI_PHY_N_8F_Bx_T ghdmi_phy_n_8f_bx;
extern HDMI_PHY_N_90_Bx_T ghdmi_phy_n_90_bx;
extern HDMI_PHY_N_91_Bx_T ghdmi_phy_n_91_bx;
extern HDMI_PHY_N_92_Bx_T ghdmi_phy_n_92_bx;
extern HDMI_PHY_N_93_Bx_T ghdmi_phy_n_93_bx;
extern HDMI_PHY_N_A0_Bx_T ghdmi_phy_n_a0_bx;
extern HDMI_PHY_N_A1_Bx_T ghdmi_phy_n_a1_bx;
extern HDMI_PHY_N_A2_Bx_T ghdmi_phy_n_a2_bx;
extern HDMI_PHY_N_A3_Bx_T ghdmi_phy_n_a3_bx;
extern HDMI_PHY_N_A4_Bx_T ghdmi_phy_n_a4_bx;
extern HDMI_PHY_N_A5_Bx_T ghdmi_phy_n_a5_bx;
extern HDMI_PHY_N_A6_Bx_T ghdmi_phy_n_a6_bx;
extern HDMI_PHY_N_A7_Bx_T ghdmi_phy_n_a7_bx;
extern HDMI_PHY_N_A8_Bx_T ghdmi_phy_n_a8_bx;
extern HDMI_PHY_N_A9_Bx_T ghdmi_phy_n_a9_bx;
extern HDMI_PHY_N_AA_Bx_T ghdmi_phy_n_aa_bx;

#ifdef __cplusplus
}
#endif

#endif	/* _VPORT_I2C_L9B0_H_ */


