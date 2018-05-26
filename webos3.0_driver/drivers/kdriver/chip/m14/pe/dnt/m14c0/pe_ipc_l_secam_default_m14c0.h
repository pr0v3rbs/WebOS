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

const PE_REG_PARAM_T ipc_l_secam_default_m14c0[] =
{
	/* pe0m ipc, 0x2200~0x224C, 0x2280~0x22A0, 0x22F0~0x2324 */
	{0x2200, 0xF2D0ACAA},    // IPC_CTRL_00
 	                         // [ 1: 0]=0x2(2) : cr_luma_mode
	                         // [ 5: 4]=0x2(2) : cr_chroma_mode
	                         // [    7]=0x1(1) : cr_chroma_edi
	                         // [    8]=0x0(0) : cr_integer_edi
	                         // [   23]=0x1(1) : cr_tava_en
	                         // [   24]=0x0(0) : cr_hmc_force_fmd
	                         // [   26]=0x0(0) : cr_clc_fmd_off
	                         // [   27]=0x1(1) : cr_chroma_tava_only : diff. turn on for secam
	                         // [   28]=0x1(1) : cr_film_auto_gain
	                         // [   29]=0x1(1) : cr_film_apply_c
	                         // [   30]=0x1(1) : cr_film_apply_y
	                         // [   31]=0x1(1) : cr_film_mode_enable
	{0x2204, 0x80878004},    // IPC_CTRL_01
	                         // [ 1: 0]=0x0(0) : cr_hmc_en_sel1
	                         // [ 3: 2]=0x1(1) : cr_hmc_en_sel2
	                         // [    8]=0x0(0) : cr_fmd_vfilter_en
	                         // [   15]=0x1(1) : cr_fend_auto
	                         // [   16]=0x1(1) : cr_st_flt_enable
	                         // [   17]=0x1(1) : cr_st_2d_v_flt_en
	                         // [   18]=0x1(1) : cr_hmc_flt_enable
	                         // [   19]=0x0(0) : cr_game_mode
	                         // [23:20]=0x8(8) : cr_st_2d_v_flt_range
	                         // [   25]=0x0(0) : cr_black_mask
	                         // [30:26]=0x0(0) : cr_ipc_debug_show
	                         // [   31]=0x1(1) : cr_5f_mode
	{0x2208, 0x1828700F},    // IPC_CTRL_02
	                         // [ 7: 0]=0xf(15) : Fuzzy_Ctrl_Point_x0
	                         // [15: 8]=0x70(112) : Fuzzy_Ctrl_Point_x4
	                         // [23:16]=0x28(40) : Fuzzy_Ctrl_Point_x1
	                         // [31:24]=0x18(24) : Fuzzy_Ctrl_Point_y1
	{0x220C, 0xF5505038},    // IPC_CTRL_03
	                         // [ 7: 0]=0x38(56) : Fuzzy_Ctrl_Point_x2
	                         // [15: 8]=0x50(80) : Fuzzy_Ctrl_Point_y2
	                         // [23:16]=0x50(80) : Fuzzy_Ctrl_Point_x3
	                         // [31:24]=0xf5(245) : Fuzzy_Ctrl_Point_y3
	{0x2210, 0x780010FC},    // IPC_CTRL_04
	                         // [ 7: 0]=0xf8(248) : ET_Offset__s7	# 2014.09.29
	                         // [15: 8]=0x10(16) : Motion_x_Tearing_Gain
	                         // [23:16]=0x0(0) : Tearing_Gain
	                         // [31:24]=0x48(72) : Motion_Gain	# 2014.09.29
	{0x2214, 0x10D02840},    // IPC_CTRL_05
	                         // [ 7: 0]=0x40(64) : cr_ipc_c_motion_alpha
	                         // [15: 8]=0x28(40) : cr_t_grad_weight
	                         // [23:16]=0xd0(208) : cr_v_grad_weight
	                         // [31:24]=0x10(16) : cr_h_grad_weight
	{0x2218, 0x00800A77},    // IPC_CTRL_06
	                         // [    0]=0x1(1) : cr_normalize_en
	                         // [    1]=0x1(1) : cr_flat_only
	                         // [    2]=0x1(1) : use_TNR_motion
	                         // [    4]=0x1(1) : cr_tearing_normalize_en
	                         // [15: 8]=0xa(10) : cr_normalize_base
	                         // [23:16]=0x68(104) : cr_normalize_center # 2014.09.29
	                         // [31:24]=0x0(0) : cr_motion_coring
	{0x221C, 0xB0E002DF},    // IPC_CTRL_07
	                         // [    1]=0x1(1) : cr_v_en
	                         // [    2]=0x1(1) : cr_45_en
	                         // [    3]=0x1(1) : cr_15_en
	                         // [31:24]=0xe(14) : TNR_motion_gain
	{0x2220, 0xF0000000},    // IPC_CTRL_08
	                         // [11: 0]=0x320(800) : fmd_caption_range_top
	                         // [27:16]=0x440(1088) : fmd_caption_range_bot
	                         // [   30]=0x0(0) : Caption_Flow_Detected
	                         // [   31]=0x0(0) : hmc_when_cap_not_detected
	{0x2224, 0x00FF2814},    // IPC_CTRL_09
	                         // [ 7: 0]=0x14(20) : PN_fmd_a_th
	                         // [23:16]=0xff(255) : PN_variance_th
	                         // [31:24]=0x0(0) : PN_fmd_motion_th
	{0x2228, 0x25100408},    // IPC_CTRL_10
	                         // [ 7: 0]=0x8(8) : PCNC_fmd_a_th
	                         // [23:16]=0x10(16) : PCNC_variance_th
	                         // [31:24]=0x25(37) : PCNC_fmd_motion_th
	{0x222C, 0x16000540},    // IPC_CTRL_11
	                         // [ 7: 0]=0xa0(160) : cr_pndiff_th
	                         // [15: 8]=0x5(5) : cr_center_bound_ctrl
	                         // [23:16]=0x0(0) : cr_dn_bound_ctrl
	                         // [31:24]=0x16(22) : cr_up_bound_ctrl
	{0x2230, 0xFF2A1003},    // IPC_CTRL_12
	                         // [ 7: 0]=0x3(3) : reg_still_pn_th
	                         // [15: 8]=0x10(16) : reg_fmd_ratio_th
	                         // [23:16]=0x2a(42) : reg_fmd_max_th
	                         // [31:24]=0xff(255) : reg_fmd_min_th
	{0x2234, 0x30001003},    // IPC_CTRL_13
	                         // [12: 0]=0x1003(4099) : cr_hmc_debug_view
	                         // [   16]=0x0(0) : cr_hmc_debug_mode
	                         // [23:19]=0x0(0) : cr_hmc_debug_dir
	                         // [29:24]=0x30(48) : cr_hmc_debug_gain
	{0x2238, 0x0750C019},    // IPC_CTRL_14
	                         // [    0]=0x1(1) : reg_lockcnt_en
	                         // [    1]=0x0(0) : RESERVED
	                         // [ 7: 2]=0x6(6) : reg_lock_speed
	                         // [15: 8]=0xc0(192) : reg_lockcnt_th
	                         // [23:16]=0x50(80) : reg_same_th
	                         // [31:24]=0x7(7) : reg_still_frame_limit
	{0x223C, 0x6A380404},    // IPC_CTRL_15
	                         // [    0]=0x0(0) : reg_3_2_detect_en
	                         // [    1]=0x0(0) : reg_3_2_detect_only
	                         // [    2]=0x1(1) : reg_3_2_lockcnt_255_en
	                         // [    3]=0x0(0) : reg_3_2_pndiff_disable
	                         // [    4]=0x0(0) : reg_hd_mode
	                         // [15: 8]=0x4(4) : reg_3_2_pndiff_th
	                         // [23:16]=0x38(56) : reg_3_2_pn_ratio_max_th
	                         // [31:24]=0x6a(106) : reg_3_2_pn_ratio_min_th
	{0x2240, 0x53703804},    // IPC_CTRL_16
	                         // [ 7: 0]=0x4(4) : reg_still_pn_th2
	                         // [15: 8]=0x38(56) : reg_still_prev_ratio
	                         // [23:16]=0x70(112) : reg_lock_reducer
	                         // [   31]=0x0(0) : reg_badedit_protect
	{0x2244, 0x40505CE1},    // IPC_CTRL_17
	                         // [ 1: 0]=0x1(1) : cr_inv_resolution
	                         // [ 3: 2]=0x0(0) : cr_field_manual_mode
	                         // [    4]=0x0(0) : cr_field_inverse
	                         // [31:24]=0x40(64) : cr_tearing_a_th
	{0x2248, 0xFF10FF08},    // IPC_CTRL_18
	                         // [ 7: 0]=0x8(8) : cr_UD_gain
	                         // [15: 8]=0xff(255) : cr_LR_gain
	                         // [23:16]=0x10(16) : cr_h_grad_gain
	                         // [31:24]=0xff(255) : cr_v_grad_gain
	{0x224C, 0x00782038},    // IPC_CTRL_19
	                         // [ 7: 0]=0x38(56) : reg_errs_gain
	                         // [15: 8]=0x20(32) : reg_confidance_gain
	                         // [23:16]=0x78(120) : reg_2d_offset
	                         // [31:24]=0x0(0) : reg_flicker_gain
	{0x22F0, 0x41919818},    // IPC_CTRL_20
	                         // [ 7: 0]=0x18(24) : reg_cubic_coef0
	                         // [15: 8]=0x98(152) : reg_cubic_coef1
	                         // [18:16]=0x1(1) : reg_va_blending_ctrl_lv1
	                         // [22:20]=0x1(1) : reg_va_blending_ctrl_lv2
	                         // [26:24]=0x1(1) : reg_va_blending_ctrl_lv3
	                         // [30:28]=0x4(4) : reg_va_blending_ctrl_lv4
	{0x22F4, 0xCC409959},    // IPC_CTRL_21
	                         // [    0]=0x1(1) : reg_va_cubic
	                         // [    1]=0x0(0) : reg_boundary_window
	                         // [    2]=0x0(0) : reg_force_va
	                         // [    3]=0x1(1) : reg_half_pel_va
	                         // [ 5: 4]=0x1(1) : reg_dir_med_tap
	                         // [ 7: 6]=0x1(1) : reg_diff_med_tap
	                         // [10: 8]=0x1(1) : reg_va_blending_ctrl1
	                         // [   11]=0x1(1) : reg_wide_angle_protection_1
	                         // [14:12]=0x1(1) : reg_va_blending_ctrl2
	                         // [   15]=0x1(1) : reg_wide_angle_protection_2
	                         // [23:16]=0x40(64) : reg_errs_chroma_blend_coef
	                         // [29:24]=0xc(12) : reg_strong_global_th
	                         // [   30]=0x1(1) : narrow_angle_week
	                         // [   31]=0x1(1) : narrow_angle_protection
	{0x22F8, 0xFFC02018},    // IPC_CTRL_22
	                         // [ 7: 0]=0x18(24) : cr_flicker_diff_gain
	                         // [15: 8]=0x20(32) : cr_flicker_amp_gain
	{0x22FC, 0x9895FF29},    // IPC_CTRL_23
	                         // [    0]=0x1(1) : cr_hmc_enable
	                         // [ 7: 1]=0xa(10) : cr_lock_th
	                         // [15: 8]=0xff(255) : cr_g_motion_th
	                         // [17:16]=0x1(1) : cr_hmc_expend
	                         // [   18]=0x1(1) : cr_hmc_half_en
	                         // [23:20]=0x9(9) : cr_robustness
	                         // [31:24]=0x98(152) : cr_center_weight
	{0x2300, 0x507B0130},    // IPC_CTRL_24
	                         // [    0]=0x0(0) : cr_noise_en
	                         // [15: 8]=0x1(1) : cr_noise_th
	                         // [18:16]=0x3(3) : cr_gds_resolution
	{0x2304, 0x28501010},    // IPC_CTRL_25
	                         // [    0]=0x0(0) : reg_vdiff_en
	                         // [15: 8]=0x10(16) : reg_vdiff_th
	                         // [23:16]=0x20(32) : reg_hmc_err_max
	                         // [31:24]=0x14(20) : reg_hmc_err_min
	{0x2308, 0x0A081019},    // IPC_CTRL_26
	                         // [    0]=0x1(1) : cr_fmd_check_en
	                         // [15: 8]=0x10(16) : cr_fmd_ath
	                         // [31:24]=0xa(10) : cr_fmd_ratio_th
	{0x230C, 0xE010E010},    // IPC_CTRL_27
	                         // [ 7: 0]=0x10(16) : CHROMA_st_flt_ctrl_x0
	                         // [15: 8]=0xe0(224) : CHROMA_st_flt_ctrl_x1
	                         // [23:16]=0x10(16) : LUMA_st_flt_ctrl_x0
	                         // [31:24]=0xe0(224) : LUMA_st_flt_ctrl_x1
	{0x2310, 0x0900800D},    // IPC_CTRL_28
	                         // [    0]=0x1(1) : reg_hmc_bonus_es_en
	                         // [    1]=0x0(0) : reg_hmc_bonus_et_en
	                         // [15: 8]=0x80(128) : reg_hmc_bonus_gain
	                         // [23:16]=0x0(0) : reg_hmc_bonus_th
	{0x2314, 0xF0704018},    // IPC_CTRL_29
	                         // [    0]=0x0(0) : HMC_flt_ctrl_x0
	                         // [31:24]=0xf0(240) : HMC_st_flt_ctrl_x1
	{0x2318, 0x6180C118},    // IPC_CTRL_30
	                         // [ 7: 0]=0x18(24) : reg_vmc_motion_th
	                         // [    8]=0x1(1) : reg_vmc_debug
	                         // [    9]=0x0(0) : reg_vmc_en
	                         // [   10]=0x0(0) : reg_vmc_still_check
	                         // [    0]=0x0(0) : reg_vmc_mul
	                         // [    0]=0x0(0) : reg_vmc_out_mul
	                         // [    0]=0x0(0) : reg_vmc_protect_th
	                         // [   24]=0x1(1) : reg_protection_en
	{0x231C, 0x00000503},    // IPC_CTRL_31
	                         // [    0]=0x1(1) : reg_toosmall_en
	                         // [ 7: 1]=0x1(1) : reg_toosmall_min
	                         // [15: 8]=0x5(5) : reg_toosmall_max
	                         // [   16]=0x0(0) : reg_pndiff_disable
	{0x2320, 0x80004040},    // IPC_CTRL_32
	                         // [31:24]=0x80(128) : tava_ErrT_x3
	                         // [23:16]=0x0(0) : tava_ErrT_y3
	                         // [15: 8]=0x40(64) : tava_ErrT_x2
	                         // [ 7: 0]=0x40(64) : tava_ErrT_y2
	{0x2324, 0x308020FF},    // IPC_CTRL_33
	                         // [31:24]=0x30(48) : tava_ErrT_x1
	                         // [23:16]=0x80(128) : tava_ErrT_y1
	                         // [15: 8]=0x20(32) : tava_ErrT_x0
	                         // [ 7: 0]=0xff(255) : tava_ErrT_y0
	{0x2280, 0x180010FF},    // IPC_CTRL_34
	                         // [31:24]=0x30(48) : tava_v_diff_x1
	                         // [23:16]=0x0(0) : tava_v_diff_y1
	                         // [15: 8]=0x20(32) : tava_v_diff_x0
	                         // [ 7: 0]=0xff(255) : tava_v_diff_y0
	{0x2284, 0x00010002},    // IPC_CTRL_35
	                         // [31:24]=0x0(0) : reg_div_position
	                         // [23:22]=0x0(0) : reg_manual_mode1
	                         // [21:20]=0x0(0) : reg_manual_mode2
	                         // [   16]=0x1(1) : reg_manual_en
	                         // [    4]=0x0(0) : cr_sub_main_sel
	                         // [    1]=0x1(1) : cr_bound_exp_en
	                         // [    0]=0x0(0) : reg_div_en
	{0x2288, 0xFF2A1003},    // IPC_CTRL_36
	                         // [ 7: 0]=0x3(3) : reg_still_pn_th
	                         // [15: 8]=0x10(16) : reg_fmd_ratio_th
	                         // [23:16]=0x2a(42) : reg_fmd_max_th
	                         // [31:24]=0xff(255) : reg_fmd_min_th
	{0x228C, 0x0750C061},    // IPC_CTRL_37
	                         // [    0]=0x1(1) : reg_lockcnt_en
	                         // [    1]=0x0(0) : RESERVED
	                         // [ 7: 2]=0x18(24) : reg_lock_speed
	                         // [15: 8]=0xc0(192) : reg_lockcnt_th
	                         // [23:16]=0x50(80) : reg_same_th
	                         // [31:24]=0x7(7) : reg_still_frame_limit
	{0x2290, 0x6A380404},    // IPC_CTRL_38
	                         // [    0]=0x0(0) : reg_3_2_detect_en
	                         // [    1]=0x0(0) : reg_3_2_detect_only
	                         // [    2]=0x1(1) : reg_3_2_lockcnt_255_en
	                         // [    3]=0x0(0) : reg_3_2_pndiff_disable
	                         // [    4]=0x0(0) : reg_hd_mode
	                         // [15: 8]=0x4(4) : reg_3_2_pndiff_th
	                         // [23:16]=0x38(56) : reg_3_2_pn_ratio_max_th
	                         // [31:24]=0x6a(106) : reg_3_2_pn_ratio_min_th
	{0x2294, 0x53703804},    // IPC_CTRL_39
	                         // [ 7: 0]=0x4(4) : reg_still_pn_th2
	                         // [15: 8]=0x38(56) : reg_still_prev_ratio
	                         // [23:16]=0x70(112) : reg_lock_reducer
	                         // [   31]=0x0(0) : reg_badedit_protect
	{0x2298, 0x00000503},    // IPC_CTRL_40
	                         // [    0]=0x1(1) : reg_toosmall_en
	                         // [ 7: 1]=0x1(1) : reg_toosmall_min
	                         // [15: 8]=0x5(5) : reg_toosmall_max
	                         // [   16]=0x0(0) : reg_pndiff_disable
	{0x229C, 0x000001E0},    // IPC_CTRL_41
	                         // [10: 0]=0x1E0(480) : reg_fend_position
	{0x22A0, 0x0006F2FB}     // IPC_CTRL_42
	                         // [ 3: 0]=0xb(11) : reg_yy_diff_gain
	                         // [ 7: 4]=0xf(15) : reg_hv_add_gain
	                         // [11: 8]=0x2(2) : reg_v_motion_gain
	                         // [15:12]=0xf(15) : reg_h_motion_gain
	                         // [19:16]=0x6(6) : reg_cc_diff_gain
};