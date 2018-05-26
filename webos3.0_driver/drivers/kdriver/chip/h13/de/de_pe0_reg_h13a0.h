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

#ifndef _REG_PE0_H13A0_h
#define _REG_PE0_H13A0_h

/*-----------------------------------------------------------------------------
	0x0400 p0l_pe0_load ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                               : 1;   //     0
	UINT32 load_enable                             : 1;   //     1
	UINT32 load_type                               : 1;   //     2
} H13A0_P0L_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
	0x0404 p0l_pe0_intr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                             : 1;   //     0
	UINT32                                         : 3;   //  1: 3 reserved
	UINT32 intr_src                                : 3;   //  4: 6
	UINT32                                         : 9;   //  7:15 reserved
	UINT32 intr_line_pos                           : 11;  // 16:26
} H13A0_P0L_PE0_INTR_T;

/*-----------------------------------------------------------------------------
	0x0408 p0l_pe0_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                            : 4;   //  0: 3
	UINT32 ipc_frame_id                            : 4;   //  4: 7
	UINT32 tnrw_y_frame_id                         : 4;   //  8:11
	UINT32 tnrw_c_frame_id                         : 4;   // 12:15
	UINT32 dnr_frame_id                            : 8;   // 16:23
	UINT32 pic_init_frame_id                       : 4;   // 24:27
} H13A0_P0L_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x040c p0l_pe0_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                                : 11;  //  0:10
} H13A0_P0L_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x0410 p0l_pe0_opmode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                                : 1;   //     0
	UINT32 tnr_y_en                                : 1;   //     1
	UINT32 ipc_c_en                                : 1;   //     2
	UINT32 ipc_y_en                                : 1;   //     3
	UINT32 tnr_c_444_as_420                        : 1;   //     4
	UINT32 tnr_y2_en                               : 1;   //     5
	UINT32 force_422_to_420                        : 1;   //     6
	UINT32                                         : 1;   //     7 reserved
	UINT32 mif_last_mode                           : 3;   //  8:10
	UINT32                                         : 1;   //    11 reserved
	UINT32 dnr_out_force_44x                       : 1;   //    12
	UINT32 tnr_m_en                                : 1;   //    13
	UINT32                                         : 2;   // 14:15 reserved
	UINT32 detour_enable                           : 1;   //    16
	UINT32                                         : 7;   // 17:23 reserved
	UINT32 dnr_in_lsb_mode                         : 2;   // 24:25
	UINT32 dnr_out_lsb_mode                        : 2;   // 26:27
	UINT32 tnr_in_lsb_mode                         : 2;   // 28:29
} H13A0_P0L_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
	0x0414 p0l_pe0_src_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                                   : 11;  //  0:10
	UINT32                                         : 2;   // 11:12 reserved
	UINT32 cs_type                                 : 3;   // 13:15
	UINT32 vsize                                   : 12;  // 16:27
	UINT32                                         : 3;   // 28:30 reserved
	UINT32 scan_type                               : 1;   //    31
} H13A0_P0L_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
	0x0418 p0l_pe0_field ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                                : 1;   //     0
	UINT32                                         : 3;   //  1: 3 reserved
	UINT32 field_mode                              : 2;   //  4: 5
} H13A0_P0L_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
	0x041c p0l_pe0_param_by_ddr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 9;   //  0: 8 reserved
	UINT32 stride                                  : 2;   //  9:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 vsize                                   : 8;   // 16:23
	UINT32                                         : 6;   // 24:29 reserved
	UINT32 endian                                  : 1;   //    30
	UINT32 update_by_ddr                           : 1;   //    31
} H13A0_P0L_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
	0x0420 p0l_win_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y0                                      : 11;  // 16:26
} H13A0_P0L_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x0424 p0l_win_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y1                                      : 11;  // 16:26
} H13A0_P0L_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x0428 p0l_win_ctrl_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y0                                      : 11;  // 16:26
	UINT32                                         : 1;   //    27 reserved
	UINT32 ac_bnr_feature_cal_mode                 : 2;   // 28:29
} H13A0_P0L_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
	0x042c p0l_win_ctrl_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y1                                      : 11;  // 16:26
} H13A0_P0L_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
	0x0430 p0l_pe0_debug_mem_req ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_tnr_prv_y                         : 1;   //     0
	UINT32 en_rd_tnr_prv_c                         : 1;   //     1
	UINT32 en_rd_tnr_pr2_y                         : 1;   //     2
	UINT32 en_rd_ipc_cur_y                         : 1;   //     3
	UINT32 en_rd_ipc_cur_c                         : 1;   //     4
	UINT32 en_rd_ipc_pprv_y                        : 1;   //     5
	UINT32 en_rd_ipc_cur_m                         : 1;   //     6
	UINT32                                         : 1;   //     7 reserved
	UINT32 en_wr_tnrw_y                            : 1;   //     8
	UINT32 en_wr_tnrw_c                            : 1;   //     9
	UINT32 en_wr_tnrw_m                            : 1;   //    10
	UINT32                                         : 4;   // 11:14 reserved
	UINT32 manual_mem_en                           : 1;   //    15
} H13A0_P0L_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
	0x0440 p0l_tnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_enable                              : 1;   //     0
	UINT32 wa_enable                               : 1;   //     1
	UINT32 nt_lvl_hsub                             : 1;   //     2
	UINT32                                         : 4;   //  3: 6 reserved
	UINT32 frame_skip_enable_for_3d                : 1;   //     7
	UINT32 minfo_mode                              : 2;   //  8: 9
	UINT32 minfo_sw                                : 2;   // 10:11
	UINT32 hist_motion_scale                       : 2;   // 12:13
	UINT32 masf_flt_en                             : 1;   //    14
	UINT32 masf_flt_sel                            : 1;   //    15
	UINT32 calc_motion_max_yc                      : 1;   //    16
	UINT32 hist_motion_max_yc                      : 1;   //    17
	UINT32 motion_4f_en                            : 1;   //    18
	UINT32 mv_tflt_en                              : 1;   //    19
	UINT32 alpha_avg_en                            : 1;   //    20
	UINT32                                         : 1;   //    21 reserved
	UINT32 alpha_mapping_y_en                      : 1;   //    22
	UINT32 alpha_mapping_c_en                      : 1;   //    23
	UINT32 nt_lvl_adjust_gm_enable                 : 1;   //    24
	UINT32 nt_lvl_adjust_lpct_enable               : 1;   //    25
	UINT32 nt_lvl_adjust_xpeak_enable              : 1;   //    26
	UINT32 nt_lvl_adjust_avg_ts_enable             : 1;   //    27
	UINT32 nt_lvl_adjust_lpct_sel                  : 1;   //    28
} H13A0_P0L_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0444 p0l_tnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                                 : 8;   //  0: 7
	UINT32 y_th_hi                                 : 8;   //  8:15
	UINT32                                         : 4;   // 16:19 reserved
	UINT32 tnr_filter_lsb_ctrl                     : 2;   // 20:21
	UINT32 svar_mux_ctrl                           : 2;   // 22:23
	UINT32 nt_lvl_hist_n_pct                       : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0448 p0l_tnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_flat_svar_th                         : 8;   //  0: 7
	UINT32 alpha_lut_ind7                          : 8;   //  8:15
	UINT32 mv0_protect_th                          : 7;   // 16:22
	UINT32 mv0_protect_en                          : 1;   //    23
} H13A0_P0L_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x044c p0l_tnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                                   : 8;   //  0: 7
	UINT32 wa_y1                                   : 8;   //  8:15
	UINT32 wa_x0                                   : 8;   // 16:23
	UINT32 wa_x1                                   : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0450 p0l_tnr_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x0                       : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y1                       : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x1                       : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0454 p0l_tnr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                                  : 8;   //  0: 7
	UINT32 nt_max                                  : 8;   //  8:15
	UINT32 hist_svar_th                            : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th0                    : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0458 p0l_tnr_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                               : 8;   //  0: 7
	UINT32 nt_iir_alpha                            : 8;   //  8:15
} H13A0_P0L_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x045c p0l_tnr_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                          : 8;   //  0: 7
	UINT32 alpha_lut_ind1                          : 8;   //  8:15
	UINT32 alpha_lut_ind2                          : 8;   // 16:23
	UINT32 alpha_lut_ind3                          : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x0460 p0l_tnr_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                          : 8;   //  0: 7
	UINT32 alpha_lut_ind5                          : 8;   //  8:15
	UINT32 alpha_lut_ind6                          : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th1                    : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x0464 p0l_tnr_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 scene_change_enable                     : 1;   //     0
	UINT32 scene_change_max_flt_en                 : 1;   //     1
	UINT32 scene_change_measure_sel                : 1;   //     2
	UINT32 scene_change_prev_hist_calc_en          : 1;   //     3
	UINT32 scene_change_enforce                    : 1;   //     4
	UINT32                                         : 3;   //  5: 7 reserved
	UINT32 scene_change_max_flt_tap                : 2;   //  8: 9
	UINT32                                         : 2;   // 10:11 reserved
	UINT32 scene_change_scale                      : 3;   // 12:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 scene_change_th                         : 8;   // 16:23
} H13A0_P0L_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x0468 p0l_tnr_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_show_tnr_param                      : 4;   //  0: 3
	UINT32 dbg_status_sum_sel                      : 2;   //  4: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 dbg_force_wa                            : 1;   //     8
	UINT32 dbg_force_calc_motion                   : 1;   //     9
	UINT32 dbg_force_lut_alpha                     : 1;   //    10
	UINT32 dbg_force_adj_alpha                     : 1;   //    11
	UINT32 dbg_force_final_alpha                   : 1;   //    12
	UINT32                                         : 2;   // 13:14 reserved
	UINT32 ignore_active_window                    : 1;   //    15
	UINT32 dbg_tnrw_port_ctrl                      : 2;   // 16:17
	UINT32                                         : 6;   // 18:23 reserved
	UINT32 dbg_wa                                  : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x046c p0l_tnr_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                         : 8;   //  0: 7
	UINT32 dbg_alpha                               : 8;   //  8:15
} H13A0_P0L_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x0470 p0l_tnr_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x2                       : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y3                       : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x3                       : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x0474 p0l_tnr_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                            : 1;   //     0
	UINT32                                         : 7;   //  1: 7 reserved
	UINT32 force_nt_lvl_val                        : 8;   //  8:15
	UINT32 adjust_nt_lvl                           : 1;   //    16
	UINT32                                         : 7;   // 17:23 reserved
	UINT32 adjust_nt_lvl_val                       : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x0478 p0l_tnr_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y0                                      : 11;  // 16:26
	UINT32                                         : 4;   // 27:30 reserved
	UINT32 noise_measure_win_en                    : 1;   //    31
} H13A0_P0L_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x047c p0l_tnr_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y1                                      : 11;  // 16:26
} H13A0_P0L_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x0480 p0l_tnr_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_ctrl0                      : 8;   //  0: 7
	UINT32 show_dbg_bar_ctrl1                      : 8;   //  8:15
	UINT32 show_dbg_bar_ctrl2                      : 8;   // 16:23
	UINT32 show_dbg_bar_ctrl3                      : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x0484 p0l_tnr_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_ctrl4                      : 8;   //  0: 7
} H13A0_P0L_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x0488 p0l_tnr_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_enable                        : 1;   //     0
	UINT32 luma_gain_enable                        : 1;   //     1
	UINT32 saturation_gain_enable                  : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 input_for_luma                          : 2;   //  4: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 input_for_crgn_satr                     : 1;   //     8
	UINT32                                         : 3;   //  9:11 reserved
	UINT32 dbg_gain_sel                            : 3;   // 12:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 y_gain                                  : 8;   // 16:23
	UINT32 c_gain                                  : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x048c p0l_tnr_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                          : 8;   //  0: 7
	UINT32 luma_gain_p1_x                          : 8;   //  8:15
	UINT32 luma_gain_p2_x                          : 8;   // 16:23
	UINT32 luma_gain_p3_x                          : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x0490 p0l_tnr_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                          : 8;   //  0: 7
	UINT32 luma_gain_p1_y                          : 8;   //  8:15
	UINT32 luma_gain_p2_y                          : 8;   // 16:23
	UINT32 luma_gain_p3_y                          : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x0494 p0l_tnr_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 saturation_gain_p0_x                    : 8;   //  0: 7
	UINT32 saturation_gain_p1_x                    : 8;   //  8:15
	UINT32 saturation_gain_p2_x                    : 8;   // 16:23
	UINT32 saturation_gain_p3_x                    : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x0498 p0l_tnr_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 saturation_gain_p0_y                    : 8;   //  0: 7
	UINT32 saturation_gain_p1_y                    : 8;   //  8:15
	UINT32 saturation_gain_p2_y                    : 8;   // 16:23
	UINT32 saturation_gain_p3_y                    : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x049c p0l_tnr_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0                      : 8;   //  0: 7
	UINT32 alpha_mapping_y_x0                      : 8;   //  8:15
	UINT32 alpha_mapping_y_y1                      : 8;   // 16:23
	UINT32 alpha_mapping_y_x1                      : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x04a0 p0l_tnr_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2                      : 8;   //  0: 7
	UINT32 alpha_mapping_y_x2                      : 8;   //  8:15
	UINT32 alpha_mapping_y_y3                      : 8;   // 16:23
	UINT32 alpha_mapping_y_x3                      : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x04a4 p0l_tnr_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0                      : 8;   //  0: 7
	UINT32 alpha_mapping_c_x0                      : 8;   //  8:15
	UINT32 alpha_mapping_c_y1                      : 8;   // 16:23
	UINT32 alpha_mapping_c_x1                      : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x04a8 p0l_tnr_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2                      : 8;   //  0: 7
	UINT32 alpha_mapping_c_x2                      : 8;   //  8:15
	UINT32 alpha_mapping_c_y3                      : 8;   // 16:23
	UINT32 alpha_mapping_c_x3                      : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x04ac p0l_tnr_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                          : 8;   //  0: 7
	UINT32 crgn_gain_rgn1                          : 8;   //  8:15
	UINT32 crgn_gain_rgn2                          : 8;   // 16:23
	UINT32 crgn_gain_rgn3                          : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x04b0 p0l_tnr_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x0                       : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y1                       : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x1                       : 7;   // 24:30
} H13A0_P0L_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x04b4 p0l_tnr_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                                   : 8;   //  0: 7
	UINT32 wa_y3                                   : 8;   //  8:15
	UINT32 wa_x2                                   : 8;   // 16:23
	UINT32 wa_x3                                   : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x04b8 p0l_tnr_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x2                       : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y3                       : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x3                       : 7;   // 24:30
} H13A0_P0L_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x04bc p0l_tnr_ctrl_31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c                    : 1;   //     0
	UINT32 spatial_pre_flt_en_y                    : 1;   //     1
	UINT32                                         : 14;  //  2:15 reserved
	UINT32 spatial_pre_flt_th_c                    : 8;   // 16:23
	UINT32 spatial_pre_flt_th_y                    : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
	0x04c0 p0l_tnr_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                                 : 8;   //  0: 7
	UINT32 x_peak_t2                               : 8;   //  8:15
	UINT32 nt_lvl                                  : 8;   // 16:23
	UINT32 scene_change                            : 8;   // 24:31
} H13A0_P0L_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x04c4 p0l_tnr_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_s                                 : 8;   //  0: 7
	UINT32 valid_cnt_motion_histogram              : 24;  //  8:31
} H13A0_P0L_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x04c8 p0l_tnr_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 gm                                      : 8;   // 16:23
	UINT32 lp                                      : 8;   // 24:31
} H13A0_P0L_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x04cc p0l_tnr_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sum_of_temp_pel_diff                    : 29;  //  0:28
} H13A0_P0L_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x04d0 p0l_tnr_main_lut_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                     : 5;   //  0: 4
	UINT32                                         : 7;   //  5:11 reserved
	UINT32 main_lut_ai_enable                      : 1;   //    12
} H13A0_P0L_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
	0x04d4 p0l_tnr_main_lut_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0                    : 32;  //  0:31
} H13A0_P0L_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
	0x04d8 p0l_tnr_main_lut_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1                    : 32;  //  0:31
} H13A0_P0L_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
	0x04e0 p0l_tnr_ctrl_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0                     : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x0                     : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y1                     : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x1                     : 7;   // 24:30
} H13A0_P0L_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
	0x04e4 p0l_tnr_ctrl_33 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2                     : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x2                     : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y3                     : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x3                     : 7;   // 24:30
} H13A0_P0L_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
	0x04e8 p0l_tnr_ctrl_34 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_xpeak_y0                    : 8;   //  0: 7
	UINT32 nt_lvl_gain_xpeak_x0                    : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_xpeak_y1                    : 8;   // 16:23
	UINT32 nt_lvl_gain_xpeak_x1                    : 7;   // 24:30
} H13A0_P0L_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
	0x04ec p0l_tnr_ctrl_35 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_xpeak_y2                    : 8;   //  0: 7
	UINT32 nt_lvl_gain_xpeak_x2                    : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_xpeak_y3                    : 8;   // 16:23
	UINT32 nt_lvl_gain_xpeak_x3                    : 7;   // 24:30
} H13A0_P0L_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
	0x04f0 p0l_tnr_crg_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr                    : 8;   //  0: 7
	UINT32                                         : 4;   //  8:11 reserved
	UINT32 crgn_ctrl_ai_enable                     : 1;   //    12
} H13A0_P0L_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x04f4 p0l_tnr_crg_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data                    : 32;  //  0:31
} H13A0_P0L_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x04f8 p0l_tnr_ctrl_36 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_sad_sel_hist                       : 1;   //     0
	UINT32 mamc_sad_sel_pel                        : 1;   //     1
	UINT32 mamc_data_sel                           : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 me_srange                               : 2;   //  4: 5
	UINT32 m_tiir_scene_change_en                  : 1;   //     6
	UINT32 m_tiir_en                               : 1;   //     7
	UINT32 m_tiir_alpha                            : 8;   //  8:15
	UINT32 mc_avg_type                             : 2;   // 16:17
	UINT32                                         : 2;   // 18:19 reserved
	UINT32 mc_sad_avg_en                           : 1;   //    20
	UINT32                                         : 1;   //    21 reserved
	UINT32 mc_mv_flt_en                            : 1;   //    22
	UINT32 mc_mv_flt_type                          : 1;   //    23
} H13A0_P0L_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
	0x04fc p0l_tnr_ctrl_37 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_y0                              : 6;   //  0: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 mc_blur_x0                              : 8;   //  8:15
	UINT32 mc_blur_y1                              : 6;   // 16:21
	UINT32                                         : 2;   // 22:23 reserved
	UINT32 mc_blur_x1                              : 8;   // 24:31
} H13A0_P0L_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
	0x0500 p0l_ipc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_mode                               : 2;   //  0: 1
	UINT32                                         : 1;   //     2 reserved
	UINT32 luma_edi                                : 1;   //     3
	UINT32 chroma_mode                             : 2;   //  4: 5
	UINT32                                         : 1;   //     6 reserved
	UINT32 chroma_edi                              : 1;   //     7
	UINT32 chroma_adaptive_va_enable               : 1;   //     8
	UINT32                                         : 1;   //     9 reserved
	UINT32 st_filter_mode                          : 2;   // 10:11
	UINT32                                         : 3;   // 12:14 reserved
	UINT32 post_tnr_flicker_gain_setltection       : 1;   //    15
	UINT32                                         : 8;   // 16:23 reserved
	UINT32 ipc_fake_v_repeat_va_ta                 : 2;   // 24:25
	UINT32                                         : 3;   // 26:28 reserved
	UINT32 film_mode_apply_c                       : 1;   //    29
	UINT32 film_mode_apply_y                       : 1;   //    30
	UINT32 film_mode_enable                        : 1;   //    31
} H13A0_P0L_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0504 p0l_ipc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fuzzy_margin0                        : 8;   //  0: 7
	UINT32 cr_fuzzy_margin1                        : 8;   //  8:15
	UINT32 st_filter_enable                        : 1;   //    16
	UINT32 direction_v_filter_enable               : 1;   //    17
	UINT32                                         : 2;   // 18:19 reserved
	UINT32 direction_v_filtering_range             : 4;   // 20:23
	UINT32 cr_tnr_debug_mode                       : 1;   //    24
	UINT32 cr_black_mask_origline_when_tnr_debug   : 1;   //    25
	UINT32 debugmap_selection                      : 5;   // 26:30
	UINT32 cr_5f_mode                              : 1;   //    31
} H13A0_P0L_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0508 p0l_ipc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                     : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_x4                     : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x1                     : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y1                     : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x050c p0l_ipc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                     : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_y2                     : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x3                     : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y3                     : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0510 p0l_ipc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset_s7                            : 8;   //  0: 7
	UINT32 motion_x_tearing_gain                   : 8;   //  8:15
	UINT32 tearing_gain                            : 8;   // 16:23
	UINT32 motion_gain                             : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0514 p0l_ipc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha                   : 8;   //  0: 7
	UINT32 cr_t_grad_weight                        : 8;   //  8:15
	UINT32 cr_v_grad_weight                        : 8;   // 16:23
	UINT32 cr_h_grad_weight                        : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0518 p0l_ipc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                         : 1;   //     0
	UINT32 cr_flat_only                            : 1;   //     1
	UINT32 use_tnr_motion                          : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 cr_tearing_normalize_en                 : 1;   //     4
	UINT32                                         : 3;   //  5: 7 reserved
	UINT32 cr_normalize_base                       : 8;   //  8:15
	UINT32 cr_normalize_center                     : 8;   // 16:23
	UINT32 cr_motion_coring                        : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x051c p0l_ipc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 1;   //     0 reserved
	UINT32 cr_v_en                                 : 1;   //     1
	UINT32 cr_45_en                                : 1;   //     2
	UINT32 cr_15_en                                : 1;   //     3
	UINT32                                         : 11;  //  4:14 reserved
	UINT32 cr_postmedian_en                        : 1;   //    15
	UINT32                                         : 8;   // 16:23 reserved
	UINT32 tnr_motion_gain                         : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x0520 p0l_ipc_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top                   : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 fmd_caption_range_bot                   : 12;  // 16:27
	UINT32                                         : 2;   // 28:29 reserved
	UINT32 caption_flow_detected                   : 1;   //    30
	UINT32 hmc_when_cap_not_detected               : 1;   //    31
} H13A0_P0L_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x0524 p0l_ipc_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_vedge_th                            : 7;   //  0: 6
	UINT32 fmd_vedge_en                            : 1;   //     7
	UINT32 fmd_a_th                                : 6;   //  8:13
	UINT32 use_3field_err_t                        : 1;   //    14
	UINT32 two_field_delay_mode                    : 1;   //    15
	UINT32 fmd_margin_top                          : 8;   // 16:23
	UINT32 fmd_margin_bottom                       : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x0528 p0l_ipc_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 film_inter_with                         : 2;   //  0: 1
	UINT32 hw_film_enable                          : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 tearing_detect_strength                 : 1;   //     4
	UINT32 cross_check                             : 1;   //     5
	UINT32                                         : 1;   //     6 reserved
	UINT32 usediffpelcount                         : 1;   //     7
	UINT32 pn_diff_th                              : 8;   //  8:15
	UINT32                                         : 12;  // 16:27 reserved
	UINT32 scalemeasureforhd                       : 2;   // 28:29
	UINT32 loading_time_ctrl                       : 1;   //    30
	UINT32 fmd_sw_bad_edit_en                      : 1;   //    31
} H13A0_P0L_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x052c p0l_ipc_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_det_line_cnt_th                  : 8;   //  0: 7
	UINT32 repeat_det_diff_pel_cnt_th              : 8;   //  8:15
	UINT32 repeat_det_noise_th                     : 4;   // 16:19
} H13A0_P0L_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x0530 p0l_ipc_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_32_enable                           : 1;   //     0
	UINT32 fmd_22_enable                           : 1;   //     1
	UINT32 fmd_badediten                           : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 fmd_hw_bad_edit_mul_th                  : 3;   //  4: 6
	UINT32                                         : 1;   //     7 reserved
	UINT32 fmd_hw_bad_edit_th_min                  : 8;   //  8:15
	UINT32 pcn_diff_th                             : 7;   // 16:22
	UINT32 fmd_arbitrary_64only                    : 1;   //    23
	UINT32 fmd_stillconditition1                   : 1;   //    24
	UINT32 fmd_stillconditition2                   : 1;   //    25
	UINT32 fmd_arbitrary_still                     : 1;   //    26
	UINT32 fmd_arbitrary_32only                    : 1;   //    27
	UINT32 fmd_still_cntth                         : 3;   // 28:30
	UINT32 fmd_still_cntth_en                      : 1;   //    31
} H13A0_P0L_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x0534 p0l_ipc_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd32_stillth_pn                        : 8;   //  0: 7
	UINT32 fmd32_stillthpcn                        : 8;   //  8:15
	UINT32 fmd32_pn_maxvalue_minth0                : 8;   // 16:23
	UINT32 fmd32_pn_maxvalue_minth1                : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x0538 p0l_ipc_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd32_min_field_num                     : 4;   //  0: 3
	UINT32                                         : 4;   //  4: 7 reserved
	UINT32 fmd32_pn_minvalue_maxth                 : 8;   //  8:15
	UINT32 fmd32_pn_maxminratio0                   : 4;   // 16:19
	UINT32 fmd32_pn_maxminratio1                   : 4;   // 20:23
	UINT32 fmd_32_pcn_small_weight                 : 3;   // 24:26
} H13A0_P0L_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x053c p0l_ipc_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd22_stillth_pn                        : 8;   //  0: 7
	UINT32 fmd22_stillth_pcn                       : 8;   //  8:15
	UINT32 bad_edit_sm_min_th                      : 12;  // 16:27
} H13A0_P0L_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x0540 p0l_ipc_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd22_minfieldnum0                      : 4;   //  0: 3
	UINT32 fmd22_minfieldnum1                      : 4;   //  4: 7
	UINT32 fmd22_minvaluemaxth1                    : 8;   //  8:15
	UINT32 fmd22_maxvalueminth1                    : 8;   // 16:23
	UINT32 fmd22_maxminratio0                      : 4;   // 24:27
	UINT32 fmd22_maxminratio1                      : 4;   // 28:31
} H13A0_P0L_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x0544 p0l_ipc_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd22_maxvalueminth0                    : 8;   //  0: 7
	UINT32 fmd22_minvaluemaxth0                    : 8;   //  8:15
	UINT32 fmd32_minvaluemaxth0                    : 8;   // 16:23
	UINT32 fmd32_maxvalueminth0                    : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x0548 p0l_ipc_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                              : 8;   //  0: 7
	UINT32 cr_lr_gain                              : 8;   //  8:15
	UINT32 cr_h_grad_gain                          : 8;   // 16:23
	UINT32 cr_v_grad_gain                          : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x054c p0l_ipc_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 center_vld_gain                         : 4;   // 16:19
	UINT32 diff_vld_gain                           : 4;   // 20:23
} H13A0_P0L_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x0550 p0l_ipc_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pc_cnt                                  : 16;  //  0:15
	UINT32 nc_cnt                                  : 16;  // 16:31
} H13A0_P0L_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x0554 p0l_ipc_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_cnt                                  : 16;  //  0:15
	UINT32 same_cnt                                : 16;  // 16:31
} H13A0_P0L_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x0558 p0l_ipc_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd32_combwith                          : 2;   //  0: 1
	UINT32                                         : 1;   //     2 reserved
	UINT32 fmd32_detected                          : 1;   //     3
	UINT32 fmd32_lockingcnt                        : 8;   //  4:11
	UINT32 fmd22_combwith                          : 2;   // 12:13
	UINT32                                         : 1;   //    14 reserved
	UINT32 fmd22_detected                          : 1;   //    15
	UINT32 fmd22_lockingcnt                        : 8;   // 16:23
	UINT32 fmd_comb_predict                        : 2;   // 24:25
	UINT32 bad_edit_detected_hw                    : 1;   //    26
	UINT32 fmd22_still_detected                    : 1;   //    27
	UINT32 fmd32_still_detected                    : 1;   //    28
	UINT32 repeat_detected                         : 1;   //    29
} H13A0_P0L_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x055c p0l_ipc_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 fmd_badeditth_hw                        : 16;  // 16:31
} H13A0_P0L_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x0560 clc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 1;   //     0 reserved
	UINT32 cl_filter_enable                        : 1;   //     1
	UINT32 clc_detection_enable                    : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 motion_window                           : 2;   //  4: 5
	UINT32 cl_pattern_dist                         : 2;   //  6: 7
	UINT32 cl_var_protect_en                       : 1;   //     8
	UINT32 cl_var_protect_v_mode                   : 1;   //     9
	UINT32 cl_var_protect_h_dist                   : 2;   // 10:11
	UINT32 cl_ab_ratio                             : 4;   // 12:15
	UINT32 cl_y_hdistance                          : 2;   // 16:17
	UINT32 cl_c_hdistance                          : 2;   // 18:19
	UINT32 cl_y_protect_en                         : 1;   //    20
	UINT32                                         : 7;   // 21:27 reserved
	UINT32 cl_motion_a_filter                      : 1;   //    28
	UINT32 cl_motion_b_filter                      : 1;   //    29
} H13A0_CLC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0564 clc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_motion_a_th                          : 8;   //  0: 7
	UINT32 cl_motion_b_th                          : 8;   //  8:15
	UINT32 cl_y_protect_th0                        : 8;   // 16:23
	UINT32 cl_y_protect_th1                        : 8;   // 24:31
} H13A0_CLC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0568 clc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_y_bound_th                           : 7;   //  0: 6
	UINT32                                         : 1;   //     7 reserved
	UINT32 cl_c_bound_th                           : 7;   //  8:14
	UINT32 cl_c_bound_type                         : 1;   //    15
	UINT32 cl_y_bound_width_chroma                 : 2;   // 16:17
	UINT32 cl_y_bound_ignore_cc                    : 1;   //    18
	UINT32 cl_y_bound_ignore_p2                    : 1;   //    19
	UINT32 cl_y_bound_ignore_p4                    : 1;   //    20
} H13A0_CLC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x056c clc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_sat_min                              : 8;   //  0: 7
	UINT32 cl_var_h_gain                           : 8;   //  8:15
	UINT32 cl_var_v_gain                           : 8;   // 16:23
	UINT32 cl_var_protect_th                       : 8;   // 24:31
} H13A0_CLC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0570 clc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 12;  //  0:11 reserved
	UINT32 cl_frame_min                            : 12;  // 12:23
} H13A0_CLC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0574 clc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 clc_c_filter_when_cl_detected           : 1;   //     0
	UINT32                                         : 1;   //     1 reserved
	UINT32 clc_y_filter_when_cc_detected           : 2;   //  2: 3
	UINT32 clc_cc_line_mem_ctrl                    : 2;   //  4: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 clc_c_gain                              : 8;   //  8:15
	UINT32 clc_dbg_show_mask                       : 12;  // 16:27
} H13A0_CLC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0578 clc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_c_filter_gain                        : 8;   //  0: 7
	UINT32 cc_y_filter_when_cc_detected            : 1;   //     8
} H13A0_CLC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x057c clc_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 cl_detection_count                      : 16;  // 16:31
} H13A0_CLC_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x0580 p0l_vflt_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vfilterenable                           : 1;   //     0
	UINT32 vfilterlocalenable                      : 1;   //     1
	UINT32 vfilter_strength                        : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 cb_filter_y                             : 2;   //  4: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 vfilter_a_th                            : 8;   //  8:15
	UINT32 vfilter_in_field_var                    : 8;   // 16:23
	UINT32 vfilter_use_mc_mask                     : 1;   //    24
	UINT32 vfilter_expand                          : 1;   //    25
	UINT32 vfilter_center                          : 1;   //    26
	UINT32 vfilter_ul4                             : 1;   //    27
	UINT32 dbgshow_level                           : 2;   // 28:29
	UINT32                                         : 1;   //    30 reserved
	UINT32 dbgshow_enable                          : 1;   //    31
} H13A0_P0L_VFLT_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0588 p0l_mnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_enable                              : 1;   //     0
	UINT32 mnr_debug                               : 2;   //  1: 2
	UINT32                                         : 1;   //     3 reserved
	UINT32 h_expend                                : 1;   //     4
	UINT32 gain_flt_size                           : 1;   //     5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 mnr_master_gain                         : 8;   //  8:15
} H13A0_P0L_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x058c p0l_mnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_s1_mmd_min                          : 8;   //  0: 7
	UINT32 mnr_s2_ratio_min                        : 8;   //  8:15
	UINT32 mnr_s2_ratio_max                        : 8;   // 16:23
	UINT32 mnr_s2_mmd_min                          : 8;   // 24:31
} H13A0_P0L_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0590 p0l_mnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 mnr_h_gain                              : 8;   // 16:23
	UINT32 mnr_v_gain                              : 8;   // 24:31
} H13A0_P0L_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x0594 p0l_mnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 filter_x0                               : 8;   //  0: 7
	UINT32 filter_x1                               : 8;   //  8:15
	UINT32 filter_y0                               : 8;   // 16:23
	UINT32 filter_y1                               : 8;   // 24:31
} H13A0_P0L_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x05a8 p0l_vfilter_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en                    : 1;   //     0
	UINT32 cr_vfilter_force_en                     : 1;   //     1
	UINT32 cr_global_frame_num_th                  : 4;   //  2: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 cr_vfilter_global_th                    : 12;  //  8:19
	UINT32 tearing_count                           : 12;  // 20:31
} H13A0_P0L_VFILTER_01_T;

/*-----------------------------------------------------------------------------
	0x05ac p0l_bnr_dc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_enable                           : 1;   //     0
	UINT32 dc_bnr_var_en                           : 1;   //     1
	UINT32 dc_bnr_motion_en                        : 1;   //     2
	UINT32 dc_bnr_acadaptive                       : 1;   //     3
	UINT32 dc_bnr_sdhd_sel                         : 1;   //     4
	UINT32 dc_blur_sel                             : 2;   //  5: 6
	UINT32 dc_debug_mode                           : 1;   //     7
	UINT32 dc_bnr_mastergain                       : 8;   //  8:15
	UINT32 dc_bnr_chromagain                       : 9;   // 16:24
	UINT32 dc_protection_en                        : 1;   //    25
	UINT32                                         : 2;   // 26:27 reserved
	UINT32 dc_bnr_debug_mode                       : 4;   // 28:31
} H13A0_P0L_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x05b0 p0l_bnr_dc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_var_th3                          : 8;   //  0: 7
	UINT32 dc_bnr_var_th2                          : 8;   //  8:15
	UINT32 dc_bnr_var_th1                          : 8;   // 16:23
	UINT32 dc_bnr_var_th0                          : 8;   // 24:31
} H13A0_P0L_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x05b4 p0l_bnr_dc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_motion_th3                       : 8;   //  0: 7
	UINT32 dc_bnr_motion_th2                       : 8;   //  8:15
	UINT32 dc_bnr_motion_th1                       : 8;   // 16:23
	UINT32 dc_bnr_motion_th0                       : 8;   // 24:31
} H13A0_P0L_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x05b8 p0l_bnr_dc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_chroma_variance                      : 8;   //  0: 7
	UINT32 dc_var_h_gain                           : 8;   //  8:15
	UINT32 dc_protection_th                        : 8;   // 16:23
} H13A0_P0L_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x05bc p0l_bnr_ac_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 ac_bnr_protect_motion_max               : 8;   // 16:23
	UINT32 ac_bnr_protect_motion_min               : 8;   // 24:31
} H13A0_P0L_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x05c0 p0l_bnr_ac_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_h_en                                : 1;   //     0
	UINT32 bnr_v_en                                : 1;   //     1
	UINT32 motion_protect_enable                   : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 source_type                             : 1;   //     4
	UINT32 status_read_type                        : 1;   //     5
	UINT32 status_read_mode                        : 2;   //  6: 7
	UINT32 hbmax_gain                              : 4;   //  8:11
	UINT32 vbmax_gain                              : 4;   // 12:15
	UINT32 strength_resolution                     : 1;   //    16
	UINT32                                         : 3;   // 17:19 reserved
	UINT32 fiter_type                              : 1;   //    20
	UINT32                                         : 3;   // 21:23 reserved
	UINT32 output_mux                              : 2;   // 24:25
} H13A0_P0L_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x05c4 p0l_bnr_ac_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_h_x0                           : 8;   //  0: 7
	UINT32 strength_h_x1                           : 8;   //  8:15
	UINT32 strength_h_max                          : 8;   // 16:23
	UINT32 detect_min_th                           : 8;   // 24:31
} H13A0_P0L_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x05c8 p0l_bnr_ac_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_v_x0                           : 8;   //  0: 7
	UINT32 strength_v_x1                           : 8;   //  8:15
	UINT32 strength_v_max                          : 8;   // 16:23
} H13A0_P0L_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x05cc p0l_bnr_ac_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset_mode                           : 1;   //     0
	UINT32 manual_offset_h_value                   : 3;   //  1: 3
	UINT32 v_offset_mode                           : 1;   //     4
	UINT32 manual_offset_v_value                   : 3;   //  5: 7
	UINT32 use_of_hysterisis                       : 4;   //  8:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 t_filter_weight                         : 8;   // 16:23
} H13A0_P0L_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x05d0 p0l_bnr_ac_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_delta_th0                           : 8;   //  0: 7
	UINT32 max_delta_th1                           : 8;   //  8:15
	UINT32 h_blockness_th                          : 8;   // 16:23
	UINT32 h_weight_max                            : 8;   // 24:31
} H13A0_P0L_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x05d4 p0l_bnr_ac_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 use_of_hysterisis                       : 4;   //  0: 3
	UINT32 block_boundary_processing_type          : 1;   //     4
} H13A0_P0L_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x05d8 p0l_dnr_max_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_snr                              : 1;   //     0
	UINT32 enable_mnr                              : 1;   //     1
	UINT32 enable_der                              : 1;   //     2
	UINT32 enable_dc_bnr                           : 1;   //     3
	UINT32 enable_ac_bnr                           : 1;   //     4
	UINT32                                         : 3;   //  5: 7 reserved
	UINT32 debug_enable                            : 1;   //     8
	UINT32 debug_mode                              : 1;   //     9
	UINT32 debug_snr_enable                        : 1;   //    10
	UINT32 debug_mnr_enable                        : 1;   //    11
	UINT32 debug_der_enable                        : 1;   //    12
	UINT32 debug_dc_bnr_enable                     : 1;   //    13
	UINT32 debug_ac_bnr_enable                     : 1;   //    14
	UINT32                                         : 1;   //    15 reserved
	UINT32 win_control_enable                      : 1;   //    16
	UINT32 border_enable                           : 1;   //    17
} H13A0_P0L_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
	0x05dc p0l_dnr_dbar_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                            : 1;   //     0
	UINT32                                         : 1;   //     1 reserved
	UINT32 show_debug_bar_for_bnr                  : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 gain                                    : 3;   //  4: 6
	UINT32                                         : 1;   //     7 reserved
	UINT32 sd_hd_sel                               : 2;   //  8: 9
} H13A0_P0L_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
	0x05e0 p0l_bnr_stat_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                         : 8;   //  0: 7
	UINT32 ac_bnr_v_status                         : 8;   //  8:15
	UINT32 dc_bnr_th                               : 14;  // 16:29
} H13A0_P0L_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
	0x05e4 p0l_der_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 der_en                                  : 1;   //     0
	UINT32                                         : 1;   //     1 reserved
	UINT32 gain2weight_mapping                     : 1;   //     2
	UINT32 bif_en                                  : 1;   //     3
	UINT32 out_mux                                 : 4;   //  4: 7
	UINT32 bif_th                                  : 8;   //  8:15
} H13A0_P0L_DER_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x05e8 p0l_der_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain_th0                                : 8;   //  0: 7
	UINT32 gain_th1                                : 8;   //  8:15
} H13A0_P0L_DER_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x05f0 p0l_ipc_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_lock_dir_sel                         : 1;   //     0
	UINT32                                         : 7;   //  1: 7 reserved
	UINT32 cr_center_ratio                         : 4;   //  8:11
	UINT32 cr_lr_diff_ratio                        : 4;   // 12:15
	UINT32 cr_hmc_lock_filt_sizeh                  : 3;   // 16:18
} H13A0_P0L_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x05f4 p0l_ipc_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_lock_point_add                       : 8;   //  0: 7
	UINT32 cr_lock_point_penalty                   : 8;   //  8:15
	UINT32 cr_lock_point_valid                     : 8;   // 16:23
	UINT32                                         : 1;   //    24 reserved
	UINT32 cr_diff_limit                           : 4;   // 25:28
	UINT32 cr_count_check_en                       : 1;   //    29
	UINT32 cr_robustness                           : 2;   // 30:31
} H13A0_P0L_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x05f8 p0l_ipc_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0l_ipc_ctrl_22_f                       : 32;   // 31: 0
} H13A0_P0L_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x05fc p0l_ipc_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 global_tbm_en                           : 1;   //     0
	UINT32 flim_panelty_en                         : 1;   //     1
	UINT32 es_offset_mode                          : 1;   //     2
	UINT32 err_mode                                : 1;   //     3
	UINT32 hmc_motion_limit                        : 4;   //  4: 7
	UINT32 global_tbm_th                           : 8;   //  8:15
	UINT32 hmc_2d_gain                             : 8;   // 16:23
	UINT32 errs_limit                              : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x0600 p0l_ipc_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 errs_gain                               : 8;   //  0: 7
	UINT32 confidance_gain                         : 8;   //  8:15
	UINT32 pe0_2d_offset                           : 8;   // 16:23
	UINT32 pe0_2d_flicker_gain                     : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x0604 p0l_ipc_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cubic_coef0                             : 8;   //  0: 7
	UINT32 cubic_coef1                             : 8;   //  8:15
	UINT32 cubic_coef2                             : 8;   // 16:23
	UINT32 cubic_coef3                             : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x0608 p0l_ipc_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 va_cubic                                : 1;   //     0
	UINT32 boundary_window                         : 1;   //     1
	UINT32 force_va                                : 1;   //     2
	UINT32 half_pel_va                             : 1;   //     3
	UINT32 dir_med_tap                             : 2;   //  4: 5
	UINT32 diff_med_tap                            : 2;   //  6: 7
	UINT32 va_blending_ctrl1                       : 3;   //  8:10
	UINT32 wide_angle_protection_1                 : 1;   //    11
	UINT32 va_blending_ctrl2                       : 3;   // 12:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 errs_chroma_blend_gain                  : 8;   // 16:23
	UINT32 strong_global_th                        : 6;   // 24:29
	UINT32 narrow_angle_week                       : 1;   //    30
	UINT32 narrow_angle_protection                 : 1;   //    31
} H13A0_P0L_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x060c p0l_ipc_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0                   : 8;   //  0: 7
	UINT32 chroma_st_flt_ctrl_x1                   : 8;   //  8:15
	UINT32 luma_st_flt_ctrl_x0                     : 8;   // 16:23
	UINT32 luma_st_flt_ctrl_x1                     : 8;   // 24:31
} H13A0_P0L_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x0610 p0l_ipc_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_gain                               : 8;   //  0: 7
	UINT32 amp_gain                                : 8;   //  8:15
} H13A0_P0L_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x0614 p0l_ipc_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_gain                               : 8;   //  0: 7
	UINT32 amp_gain                                : 8;   //  8:15
} H13A0_P0L_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x0618 p0l_ipc_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vmc_motion_th                           : 8;   //  0: 7
	UINT32 vmc_debug                               : 1;   //     8
	UINT32 vmc_en                                  : 1;   //     9
	UINT32 vmc_still_check                         : 1;   //    10
	UINT32 vmc_mul                                 : 3;   // 11:13
	UINT32 vmc_out_mul                             : 2;   // 14:15
	UINT32 vmc_protect_th                          : 8;   // 16:23
	UINT32 protection_en                           : 1;   //    24
} H13A0_P0L_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x0640 p0l_tpd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_en                                  : 1;   //     0
	UINT32                                         : 1;   //     1 reserved
	UINT32 win_mode                                : 1;   //     2
	UINT32                                         : 12;  //  3:14 reserved
	UINT32 quick_out                               : 1;   //    15
	UINT32 tpd_th_s_vl                             : 8;   // 16:23
	UINT32 tpd_th_s_vh                             : 8;   // 24:31
} H13A0_P0L_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0644 p0l_tpd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 12;  //  0:11
	UINT32                                         : 3;   // 12:14 reserved
	UINT32 win_tpd_hen                             : 1;   //    15
	UINT32 y0                                      : 12;  // 16:27
	UINT32                                         : 3;   // 28:30 reserved
	UINT32 win_tpd_ven                             : 1;   //    31
} H13A0_P0L_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0648 p0l_tpd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y1                                      : 12;  // 16:27
} H13A0_P0L_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x064c p0l_tpd_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_th_vl                               : 8;   //  0: 7
	UINT32 tpd_th_vh                               : 8;   //  8:15
	UINT32 vh_valid_th                             : 8;   // 16:23
	UINT32 vh_max_th                               : 8;   // 24:31
} H13A0_P0L_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0650 p0l_tpd_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_th_s                                : 8;   //  0: 7
	UINT32 sv_valid_th                             : 8;   //  8:15
	UINT32 vl_valid_th                             : 8;   // 16:23
	UINT32 vl_max_th                               : 8;   // 24:31
} H13A0_P0L_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0654 p0l_tpd_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mid_level                               : 10;  //  0: 9
	UINT32                                         : 6;   // 10:15 reserved
	UINT32 tpd_th_sv_vl                            : 8;   // 16:23
	UINT32 tpd_th_sv_vh                            : 8;   // 24:31
} H13A0_P0L_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0658 p0l_tpd_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 4;   //  0: 3 reserved
	UINT32 mux_3                                   : 4;   //  4: 7
	UINT32                                         : 4;   //  8:11 reserved
	UINT32 mux_2                                   : 4;   // 12:15
	UINT32                                         : 4;   // 16:19 reserved
	UINT32 mux_1                                   : 4;   // 20:23
	UINT32 show_scale                              : 3;   // 24:26
	UINT32 bbd_show                                : 1;   //    27
	UINT32 show_bar_1                              : 1;   //    28
	UINT32 show_bar_2                              : 1;   //    29
	UINT32 show_bar_3                              : 1;   //    30
	UINT32 show_bar_4                              : 1;   //    31
} H13A0_P0L_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x065c p0l_tpd_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 4;   //  0: 3 reserved
	UINT32 mux_4                                   : 4;   //  4: 7
	UINT32 hys_mode                                : 6;   //  8:13
	UINT32                                         : 2;   // 14:15 reserved
	UINT32 tpd_th_s_sv                             : 5;   // 16:20
	UINT32                                         : 3;   // 21:23 reserved
	UINT32 tpd_th_sv                               : 5;   // 24:28
} H13A0_P0L_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x0660 tpd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_value                               : 1;   //     0
	UINT32 tpd_by_s                                : 1;   //     1
	UINT32 tpd_by_vl                               : 1;   //     2
	UINT32 tpd_by_vh                               : 1;   //     3
	UINT32 tpd_by_vl_max                           : 1;   //     4
	UINT32 tpd_by_vh_max                           : 1;   //     5
	UINT32 tpd_by_s_vl                             : 1;   //     6
	UINT32 tpd_by_s_vh                             : 1;   //     7
	UINT32 s_status                                : 8;   //  8:15
	UINT32 vl_status                               : 8;   // 16:23
	UINT32 vh_status                               : 8;   // 24:31
} H13A0_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x0664 tpd_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_by_sv                               : 1;   //     0
	UINT32 tpd_by_sv_vl                            : 1;   //     1
	UINT32 tpd_by_sv_vh                            : 1;   //     2
	UINT32 tpd_by_s_sv                             : 1;   //     3
	UINT32 hys_tpd_value                           : 1;   //     4
	UINT32                                         : 3;   //  5: 7 reserved
	UINT32 sv_status                               : 5;   //  8:12
} H13A0_TPD_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x0670 p0l_bbd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_en                                  : 1;   //     0
	UINT32 hys_mode                                : 5;   //  1: 5
	UINT32 op_mode                                 : 1;   //     6
	UINT32                                         : 1;   //     7 reserved
	UINT32 cnt_th                                  : 8;   //  8:15
	UINT32 diff_th                                 : 8;   // 16:23
	UINT32 bbd_mux                                 : 3;   // 24:26
	UINT32                                         : 1;   //    27 reserved
	UINT32 apl_mux                                 : 3;   // 28:30
} H13A0_P0L_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0674 p0l_bbd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y0                                      : 12;  // 16:27
} H13A0_P0L_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0678 p0l_bbd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y1                                      : 12;  // 16:27
} H13A0_P0L_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x067c p0l_bbd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y0                                      : 12;  // 16:27
} H13A0_P0L_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x0680 p0l_bbd_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y1                                      : 12;  // 16:27
} H13A0_P0L_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x0684 p0l_apl_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0l_apl_stat_00_f                       : 32;   // 31: 0
} H13A0_P0L_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x0690 p0l_cti_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_enable                              : 1;   //     0
	UINT32 cti_repeat_en                           : 1;   //     1
	UINT32 cti_clipping_en                         : 1;   //     2
	UINT32                                         : 13;  //  3:15 reserved
	UINT32 cti_gain                                : 8;   // 16:23
	UINT32 cb_filter_c                             : 2;   // 24:25
} H13A0_P0L_CTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x06a0 pe0_3dfd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_offset_x                             : 8;   //  0: 7
	UINT32 aw_offset_y                             : 8;   //  8:15
	UINT32 cb_th1                                  : 8;   // 16:23
	UINT32 cb_th2                                  : 8;   // 24:31
} H13A0_PE0_3DFD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x06a4 pe0_3dfd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_th1                                  : 8;   //  0: 7
	UINT32 cc_th2                                  : 8;   //  8:15
	UINT32 ll_th1                                  : 8;   // 16:23
	UINT32 ll_th2                                  : 8;   // 24:31
} H13A0_PE0_3DFD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x06a8 pe0_3dfd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pt_th                                   : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 sstb_th                                 : 8;   // 16:23
	UINT32 debug_mode                              : 3;   // 24:26
	UINT32                                         : 1;   //    27 reserved
	UINT32 div_mode                                : 1;   //    28
} H13A0_PE0_3DFD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x06ac pe0_3dfd_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 small_delta_th                          : 19;  //  0:18
	UINT32                                         : 5;   // 19:23 reserved
	UINT32 small_delta_en                          : 1;   //    24
} H13A0_PE0_3DFD_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x06b0 pe0_3dfd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_count                                : 16;  //  0:15
	UINT32 cc_count                                : 16;  // 16:31
} H13A0_PE0_3DFD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x06b4 pe0_3dfd_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ll_count                                : 16;  //  0:15
	UINT32 trid_format                             : 4;   // 16:19
} H13A0_PE0_3DFD_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x06b8 pe0_3dfd_stat_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_h                              : 23;  //  0:22
} H13A0_PE0_3DFD_STAT_02_T;

/*-----------------------------------------------------------------------------
	0x06bc pe0_3dfd_stat_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_v                              : 23;  //  0:22
} H13A0_PE0_3DFD_STAT_03_T;

/*-----------------------------------------------------------------------------
	0x06c0 pe0_3dfd_stat_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_v                              : 23;  //  0:22
} H13A0_PE0_3DFD_STAT_04_T;

/*-----------------------------------------------------------------------------
	0x06c4 pe0_3dfd_stat_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_h                              : 23;  //  0:22
} H13A0_PE0_3DFD_STAT_05_T;

/*-----------------------------------------------------------------------------
	0x06c8 pe0_3dfd_stat_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_ratio                                : 23;  //  0:22
} H13A0_PE0_3DFD_STAT_06_T;

/*-----------------------------------------------------------------------------
	0x06cc pe0_3dfd_stat_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_ratio                                : 23;  //  0:22
} H13A0_PE0_3DFD_STAT_07_T;

/*-----------------------------------------------------------------------------
	0x06d0 p0l_post_tnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_post_tnr_enable                      : 1;   //     0
	UINT32 cr_post_tnr_manual_mode                 : 1;   //     1
	UINT32 cr_post_tnr_p1_enable                   : 1;   //     2
	UINT32                                         : 2;   //  3: 4 reserved
	UINT32 cr_post_tnr_c_enable                    : 1;   //     5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 cr_p1_debug_mode                        : 1;   //     8
	UINT32 cr_p2_debug_mode                        : 1;   //     9
	UINT32 flicker_debug_mode                      : 1;   //    10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 cr_post_tnr_p2_th                       : 8;   // 16:23
	UINT32 cr_post_tnr_p1_th                       : 8;   // 24:31
} H13A0_P0L_POST_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x06d4 p0l_post_tnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_post_tnr_p1_x3                       : 8;   //  0: 7
	UINT32 cr_post_tnr_p1_x2                       : 8;   //  8:15
	UINT32 cr_post_tnr_p1_x1                       : 8;   // 16:23
	UINT32 cr_post_tnr_p1_x0                       : 8;   // 24:31
} H13A0_P0L_POST_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x06d8 p0l_post_tnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_post_tnr_p1_y3                       : 8;   //  0: 7
	UINT32 cr_post_tnr_p1_y2                       : 8;   //  8:15
	UINT32 cr_post_tnr_p1_y1                       : 8;   // 16:23
	UINT32 cr_post_tnr_p1_y0                       : 8;   // 24:31
} H13A0_P0L_POST_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x2400 p0r_pe0_load ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                               : 1;   //     0
	UINT32 load_enable                             : 1;   //     1
	UINT32 load_type                               : 1;   //     2
} H13A0_P0R_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
	0x2404 p0r_pe0_intr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                             : 1;   //     0
	UINT32                                         : 3;   //  1: 3 reserved
	UINT32 intr_src                                : 3;   //  4: 6
	UINT32                                         : 9;   //  7:15 reserved
	UINT32 intr_line_pos                           : 11;  // 16:26
} H13A0_P0R_PE0_INTR_T;

/*-----------------------------------------------------------------------------
	0x2408 p0r_pe0_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                            : 4;   //  0: 3
	UINT32 ipc_frame_id                            : 4;   //  4: 7
	UINT32 tnrw_y_frame_id                         : 4;   //  8:11
	UINT32 tnrw_c_frame_id                         : 4;   // 12:15
	UINT32 dnr_frame_id                            : 8;   // 16:23
	UINT32 pic_init_frame_id                       : 4;   // 24:27
} H13A0_P0R_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x240c p0r_pe0_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                                : 11;  //  0:10
} H13A0_P0R_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x2410 p0r_pe0_opmode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                                : 1;   //     0
	UINT32 tnr_y_en                                : 1;   //     1
	UINT32 ipc_c_en                                : 1;   //     2
	UINT32 ipc_y_en                                : 1;   //     3
	UINT32 tnr_c_422_as_420                        : 1;   //     4
	UINT32 tnr_y2_en                               : 1;   //     5
	UINT32 force_422_to_420                        : 1;   //     6
	UINT32                                         : 1;   //     7 reserved
	UINT32 mif_last_mode                           : 3;   //  8:10
	UINT32                                         : 1;   //    11 reserved
	UINT32 dnr_out_force_44x                       : 1;   //    12
	UINT32                                         : 3;   // 13:15 reserved
	UINT32 detour_enable                           : 1;   //    16
	UINT32                                         : 7;   // 17:23 reserved
	UINT32 dnr_in_lsb_mode                         : 2;   // 24:25
	UINT32                                         : 2;   // 26:27 reserved
	UINT32 tnr_in_lsb_mode                         : 2;   // 28:29
} H13A0_P0R_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
	0x2414 p0r_pe0_src_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                                   : 11;  //  0:10
	UINT32                                         : 2;   // 11:12 reserved
	UINT32 cs_type                                 : 3;   // 13:15
	UINT32 vsize                                   : 12;  // 16:27
	UINT32                                         : 3;   // 28:30 reserved
	UINT32 scan_type                               : 1;   //    31
} H13A0_P0R_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
	0x2418 p0r_pe0_field ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                                : 1;   //     0
	UINT32                                         : 3;   //  1: 3 reserved
	UINT32 field_mode                              : 2;   //  4: 5
} H13A0_P0R_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
	0x241c p0r_pe0_param_by_ddr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 9;   //  0: 8 reserved
	UINT32 stride                                  : 2;   //  9:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 vsize                                   : 8;   // 16:23
	UINT32                                         : 6;   // 24:29 reserved
	UINT32 endian                                  : 1;   //    30
	UINT32 update_by_ddr                           : 1;   //    31
} H13A0_P0R_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
	0x2420 p0r_win_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y0                                      : 11;  // 16:26
} H13A0_P0R_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x2424 p0r_win_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y1                                      : 11;  // 16:26
} H13A0_P0R_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x2428 p0r_win_ctrl_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y0                                      : 11;  // 16:26
	UINT32                                         : 1;   //    27 reserved
	UINT32 ac_bnr_feature_cal_mode                 : 2;   // 28:29
} H13A0_P0R_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
	0x242c p0r_win_ctrl_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y1                                      : 11;  // 16:26
} H13A0_P0R_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
	0x2430 p0r_pe0_debug_mem_req ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_tnr_prv_y                         : 1;   //     0
	UINT32 en_rd_tnr_prv_c                         : 1;   //     1
	UINT32 en_rd_tnr_pr2_y                         : 1;   //     2
	UINT32 en_rd_ipc_cur_y                         : 1;   //     3
	UINT32 en_rd_ipc_cur_c                         : 1;   //     4
	UINT32 en_rd_ipc_pprv_y                        : 1;   //     5
	UINT32 en_rd_ipc_cur_m                         : 1;   //     6
	UINT32                                         : 1;   //     7 reserved
	UINT32 en_wr_tnrw_y                            : 1;   //     8
	UINT32 en_wr_tnrw_c                            : 1;   //     9
	UINT32 en_wr_tnrw_m                            : 1;   //    10
	UINT32                                         : 4;   // 11:14 reserved
	UINT32 manual_mem_en                           : 1;   //    15
} H13A0_P0R_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
	0x2440 p0r_tnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_enable                              : 1;   //     0
	UINT32 wa_enable                               : 1;   //     1
	UINT32 nt_lvl_hsub                             : 1;   //     2
	UINT32                                         : 4;   //  3: 6 reserved
	UINT32 frame_skip_enable_for_3d                : 1;   //     7
	UINT32 minfo_mode                              : 2;   //  8: 9
	UINT32 minfo_sw                                : 2;   // 10:11
	UINT32 hist_motion_scale                       : 2;   // 12:13
	UINT32 masf_flt_en                             : 1;   //    14
	UINT32 masf_flt_sel                            : 1;   //    15
	UINT32 calc_motion_max_yc                      : 1;   //    16
	UINT32 hist_motion_max_yc                      : 1;   //    17
	UINT32 motion_4f_en                            : 1;   //    18
	UINT32 mv_tflt_en                              : 1;   //    19
	UINT32 alpha_avg_en                            : 1;   //    20
	UINT32                                         : 1;   //    21 reserved
	UINT32 alpha_mapping_y_en                      : 1;   //    22
	UINT32 alpha_mapping_c_en                      : 1;   //    23
	UINT32 nt_lvl_adjust_gm_enable                 : 1;   //    24
	UINT32 nt_lvl_adjust_lpct_enable               : 1;   //    25
	UINT32 nt_lvl_adjust_xpeak_enable              : 1;   //    26
	UINT32 nt_lvl_adjust_avg_ts_enable             : 1;   //    27
	UINT32 nt_lvl_adjust_lpct_sel                  : 1;   //    28
} H13A0_P0R_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2444 p0r_tnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                                 : 8;   //  0: 7
	UINT32 y_th_hi                                 : 8;   //  8:15
	UINT32                                         : 4;   // 16:19 reserved
	UINT32 tnr_filter_lsb_ctrl                     : 2;   // 20:21
	UINT32 svar_mux_ctrl                           : 2;   // 22:23
	UINT32 nt_lvl_hist_n_pct                       : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2448 p0r_tnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_flat_svar_th                         : 8;   //  0: 7
	UINT32 alpha_lut_ind7                          : 8;   //  8:15
	UINT32 mv0_protect_th                          : 7;   // 16:22
	UINT32 mv0_protect_en                          : 1;   //    23
} H13A0_P0R_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x244c p0r_tnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                                   : 8;   //  0: 7
	UINT32 wa_y1                                   : 8;   //  8:15
	UINT32 wa_x0                                   : 8;   // 16:23
	UINT32 wa_x1                                   : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2450 p0r_tnr_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x0                       : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y1                       : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x1                       : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x2454 p0r_tnr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                                  : 8;   //  0: 7
	UINT32 nt_max                                  : 8;   //  8:15
	UINT32 hist_svar_th                            : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th0                    : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x2458 p0r_tnr_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                               : 8;   //  0: 7
	UINT32 nt_iir_alpha                            : 8;   //  8:15
} H13A0_P0R_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x245c p0r_tnr_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                          : 8;   //  0: 7
	UINT32 alpha_lut_ind1                          : 8;   //  8:15
	UINT32 alpha_lut_ind2                          : 8;   // 16:23
	UINT32 alpha_lut_ind3                          : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x2460 p0r_tnr_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                          : 8;   //  0: 7
	UINT32 alpha_lut_ind5                          : 8;   //  8:15
	UINT32 alpha_lut_ind6                          : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th1                    : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x2464 p0r_tnr_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 scene_change_enable                     : 1;   //     0
	UINT32 scene_change_max_flt_en                 : 1;   //     1
	UINT32 scene_change_measure_sel                : 1;   //     2
	UINT32 scene_change_prev_hist_calc_en          : 1;   //     3
	UINT32 scene_change_enforce                    : 1;   //     4
	UINT32                                         : 3;   //  5: 7 reserved
	UINT32 scene_change_max_flt_tap                : 2;   //  8: 9
	UINT32                                         : 2;   // 10:11 reserved
	UINT32 scene_change_scale                      : 3;   // 12:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 scene_change_th                         : 8;   // 16:23
} H13A0_P0R_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x2468 p0r_tnr_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_show_tnr_param                      : 4;   //  0: 3
	UINT32 dbg_status_sum_sel                      : 2;   //  4: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 dbg_force_wa                            : 1;   //     8
	UINT32 dbg_force_calc_motion                   : 1;   //     9
	UINT32 dbg_force_lut_alpha                     : 1;   //    10
	UINT32 dbg_force_adj_alpha                     : 1;   //    11
	UINT32 dbg_force_final_alpha                   : 1;   //    12
	UINT32                                         : 2;   // 13:14 reserved
	UINT32 ignore_active_window                    : 1;   //    15
	UINT32 dbg_tnrw_port_ctrl                      : 2;   // 16:17
	UINT32                                         : 6;   // 18:23 reserved
	UINT32 dbg_wa                                  : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x246c p0r_tnr_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                         : 8;   //  0: 7
	UINT32 dbg_alpha                               : 8;   //  8:15
} H13A0_P0R_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x2470 p0r_tnr_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x2                       : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y3                       : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x3                       : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x2474 p0r_tnr_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                            : 1;   //     0
	UINT32                                         : 7;   //  1: 7 reserved
	UINT32 force_nt_lvl_val                        : 8;   //  8:15
	UINT32 adjust_nt_lvl                           : 1;   //    16
	UINT32                                         : 7;   // 17:23 reserved
	UINT32 adjust_nt_lvl_val                       : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x2478 p0r_tnr_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y0                                      : 11;  // 16:26
	UINT32                                         : 4;   // 27:30 reserved
	UINT32 noise_measure_win_en                    : 1;   //    31
} H13A0_P0R_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x247c p0r_tnr_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 11;  //  0:10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 y1                                      : 11;  // 16:26
} H13A0_P0R_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x2480 p0r_tnr_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_ctrl0                      : 8;   //  0: 7
	UINT32 show_dbg_bar_ctrl1                      : 8;   //  8:15
	UINT32 show_dbg_bar_ctrl2                      : 8;   // 16:23
	UINT32 show_dbg_bar_ctrl3                      : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x2484 p0r_tnr_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_ctrl4                      : 8;   //  0: 7
} H13A0_P0R_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x2488 p0r_tnr_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_enable                        : 1;   //     0
	UINT32 luma_gain_enable                        : 1;   //     1
	UINT32 saturation_gain_enable                  : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 input_for_luma                          : 2;   //  4: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 input_for_crgn_satr                     : 1;   //     8
	UINT32                                         : 3;   //  9:11 reserved
	UINT32 dbg_gain_sel                            : 3;   // 12:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 y_gain                                  : 8;   // 16:23
	UINT32 c_gain                                  : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x248c p0r_tnr_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                          : 8;   //  0: 7
	UINT32 luma_gain_p1_x                          : 8;   //  8:15
	UINT32 luma_gain_p2_x                          : 8;   // 16:23
	UINT32 luma_gain_p3_x                          : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x2490 p0r_tnr_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                          : 8;   //  0: 7
	UINT32 luma_gain_p1_y                          : 8;   //  8:15
	UINT32 luma_gain_p2_y                          : 8;   // 16:23
	UINT32 luma_gain_p3_y                          : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x2494 p0r_tnr_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 saturation_gain_p0_x                    : 8;   //  0: 7
	UINT32 saturation_gain_p1_x                    : 8;   //  8:15
	UINT32 saturation_gain_p2_x                    : 8;   // 16:23
	UINT32 saturation_gain_p3_x                    : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x2498 p0r_tnr_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 saturation_gain_p0_y                    : 8;   //  0: 7
	UINT32 saturation_gain_p1_y                    : 8;   //  8:15
	UINT32 saturation_gain_p2_y                    : 8;   // 16:23
	UINT32 saturation_gain_p3_y                    : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x249c p0r_tnr_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0                      : 8;   //  0: 7
	UINT32 alpha_mapping_y_x0                      : 8;   //  8:15
	UINT32 alpha_mapping_y_y1                      : 8;   // 16:23
	UINT32 alpha_mapping_y_x1                      : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x24a0 p0r_tnr_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2                      : 8;   //  0: 7
	UINT32 alpha_mapping_y_x2                      : 8;   //  8:15
	UINT32 alpha_mapping_y_y3                      : 8;   // 16:23
	UINT32 alpha_mapping_y_x3                      : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x24a4 p0r_tnr_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0                      : 8;   //  0: 7
	UINT32 alpha_mapping_c_x0                      : 8;   //  8:15
	UINT32 alpha_mapping_c_y1                      : 8;   // 16:23
	UINT32 alpha_mapping_c_x1                      : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x24a8 p0r_tnr_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2                      : 8;   //  0: 7
	UINT32 alpha_mapping_c_x2                      : 8;   //  8:15
	UINT32 alpha_mapping_c_y3                      : 8;   // 16:23
	UINT32 alpha_mapping_c_x3                      : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x24ac p0r_tnr_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                          : 8;   //  0: 7
	UINT32 crgn_gain_rgn1                          : 8;   //  8:15
	UINT32 crgn_gain_rgn2                          : 8;   // 16:23
	UINT32 crgn_gain_rgn3                          : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x24b0 p0r_tnr_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x0                       : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y1                       : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x1                       : 7;   // 24:30
} H13A0_P0R_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x24b4 p0r_tnr_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                                   : 8;   //  0: 7
	UINT32 wa_y3                                   : 8;   //  8:15
	UINT32 wa_x2                                   : 8;   // 16:23
	UINT32 wa_x3                                   : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x24b8 p0r_tnr_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2                       : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x2                       : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y3                       : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x3                       : 7;   // 24:30
} H13A0_P0R_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x24bc p0r_tnr_ctrl_31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c                    : 1;   //     0
	UINT32 spatial_pre_flt_en_y                    : 1;   //     1
	UINT32                                         : 14;  //  2:15 reserved
	UINT32 spatial_pre_flt_th_c                    : 8;   // 16:23
	UINT32 spatial_pre_flt_th_y                    : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
	0x24c0 p0r_tnr_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                                 : 8;   //  0: 7
	UINT32 x_peak_t2                               : 8;   //  8:15
	UINT32 nt_lvl                                  : 8;   // 16:23
	UINT32 scene_change                            : 8;   // 24:31
} H13A0_P0R_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x24c4 p0r_tnr_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_s                                 : 8;   //  0: 7
	UINT32 valid_cnt_motion_histogram              : 24;  //  8:31
} H13A0_P0R_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x24c8 p0r_tnr_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 gm                                      : 8;   // 16:23
	UINT32 lp                                      : 8;   // 24:31
} H13A0_P0R_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x24cc p0r_tnr_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sum_of_temp_pel_diff                    : 29;  //  0:28
} H13A0_P0R_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x24d0 p0r_tnr_main_lut_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                     : 5;   //  0: 4
	UINT32                                         : 7;   //  5:11 reserved
	UINT32 main_lut_ai_enable                      : 1;   //    12
} H13A0_P0R_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
	0x24d4 p0r_tnr_main_lut_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0                    : 32;  //  0:31
} H13A0_P0R_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
	0x24d8 p0r_tnr_main_lut_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1                    : 32;  //  0:31
} H13A0_P0R_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
	0x24e0 p0r_tnr_ctrl_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0                     : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x0                     : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y1                     : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x1                     : 7;   // 24:30
} H13A0_P0R_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
	0x24e4 p0r_tnr_ctrl_33 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2                     : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x2                     : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y3                     : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x3                     : 7;   // 24:30
} H13A0_P0R_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
	0x24e8 p0r_tnr_ctrl_34 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_xpeak_y0                    : 8;   //  0: 7
	UINT32 nt_lvl_gain_xpeak_x0                    : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_xpeak_y1                    : 8;   // 16:23
	UINT32 nt_lvl_gain_xpeak_x1                    : 7;   // 24:30
} H13A0_P0R_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
	0x24ec p0r_tnr_ctrl_35 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_xpeak_y2                    : 8;   //  0: 7
	UINT32 nt_lvl_gain_xpeak_x2                    : 7;   //  8:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 nt_lvl_gain_xpeak_y3                    : 8;   // 16:23
	UINT32 nt_lvl_gain_xpeak_x3                    : 7;   // 24:30
} H13A0_P0R_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
	0x24f0 p0r_tnr_crg_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr                    : 8;   //  0: 7
	UINT32                                         : 4;   //  8:11 reserved
	UINT32 crgn_ctrl_ai_enable                     : 1;   //    12
} H13A0_P0R_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x24f4 p0r_tnr_crg_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data                    : 32;  //  0:31
} H13A0_P0R_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x24f8 p0r_tnr_ctrl_36 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_sad_sel_hist                       : 1;   //     0
	UINT32 mamc_sad_sel_pel                        : 1;   //     1
	UINT32 mamc_data_sel                           : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 me_srange                               : 2;   //  4: 5
	UINT32 m_tiir_scene_change_en                  : 1;   //     6
	UINT32 m_tiir_en                               : 1;   //     7
	UINT32 m_tiir_alpha                            : 8;   //  8:15
	UINT32 mc_avg_type                             : 2;   // 16:17
	UINT32                                         : 2;   // 18:19 reserved
	UINT32 mc_sad_avg_en                           : 1;   //    20
	UINT32                                         : 1;   //    21 reserved
	UINT32 mc_mv_flt_en                            : 1;   //    22
	UINT32 mc_mv_flt_type                          : 1;   //    23
} H13A0_P0R_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
	0x24fc p0r_tnr_ctrl_37 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_y0                              : 6;   //  0: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 mc_blur_x0                              : 8;   //  8:15
	UINT32 mc_blur_y1                              : 6;   // 16:21
	UINT32                                         : 2;   // 22:23 reserved
	UINT32 mc_blur_x1                              : 8;   // 24:31
} H13A0_P0R_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
	0x2500 p0r_ipc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_mode                               : 2;   //  0: 1
	UINT32                                         : 1;   //     2 reserved
	UINT32 luma_edi                                : 1;   //     3
	UINT32 chroma_mode                             : 2;   //  4: 5
	UINT32                                         : 1;   //     6 reserved
	UINT32 chroma_edi                              : 1;   //     7
	UINT32 chroma_adaptive_va_enable               : 1;   //     8
	UINT32                                         : 1;   //     9 reserved
	UINT32 st_filter_mode                          : 2;   // 10:11
	UINT32                                         : 3;   // 12:14 reserved
	UINT32 post_tnr_flicker_gain_setltection       : 1;   //    15
	UINT32                                         : 8;   // 16:23 reserved
	UINT32 ipc_fake_v_repeat_va_ta                 : 2;   // 24:25
	UINT32                                         : 3;   // 26:28 reserved
	UINT32 film_mode_apply_c                       : 1;   //    29
	UINT32 film_mode_apply_y                       : 1;   //    30
	UINT32 film_mode_enable                        : 1;   //    31
} H13A0_P0R_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2504 p0r_ipc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fuzzy_margin0                        : 8;   //  0: 7
	UINT32 cr_fuzzy_margin1                        : 8;   //  8:15
	UINT32 st_filter_enable                        : 1;   //    16
	UINT32 direction_v_filter_enable               : 1;   //    17
	UINT32                                         : 2;   // 18:19 reserved
	UINT32 direction_v_filtering_range             : 4;   // 20:23
	UINT32 cr_tnr_debug_mode                       : 1;   //    24
	UINT32 cr_black_mask_origline_when_tnr_debug   : 1;   //    25
	UINT32 debugmap_selection                      : 5;   // 26:30
	UINT32 cr_5f_mode                              : 1;   //    31
} H13A0_P0R_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2508 p0r_ipc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                     : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_x4                     : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x1                     : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y1                     : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x250c p0r_ipc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                     : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_y2                     : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x3                     : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y3                     : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2510 p0r_ipc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset_s7                            : 8;   //  0: 7
	UINT32 motion_x_tearing_gain                   : 8;   //  8:15
	UINT32 tearing_gain                            : 8;   // 16:23
	UINT32 motion_gain                             : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x2514 p0r_ipc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha                   : 8;   //  0: 7
	UINT32 cr_t_grad_weight                        : 8;   //  8:15
	UINT32 cr_v_grad_weight                        : 8;   // 16:23
	UINT32 cr_h_grad_weight                        : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x2518 p0r_ipc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                         : 1;   //     0
	UINT32 cr_flat_only                            : 1;   //     1
	UINT32 use_tnr_motion                          : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 cr_tearing_normalize_en                 : 1;   //     4
	UINT32                                         : 3;   //  5: 7 reserved
	UINT32 cr_normalize_base                       : 8;   //  8:15
	UINT32 cr_normalize_center                     : 8;   // 16:23
	UINT32 cr_motion_coring                        : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x251c p0r_ipc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 1;   //     0 reserved
	UINT32 cr_v_en                                 : 1;   //     1
	UINT32 cr_45_en                                : 1;   //     2
	UINT32 cr_15_en                                : 1;   //     3
	UINT32                                         : 11;  //  4:14 reserved
	UINT32 cr_postmedian_en                        : 1;   //    15
	UINT32                                         : 8;   // 16:23 reserved
	UINT32 tnr_motion_gain                         : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x2520 p0r_ipc_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top                   : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 fmd_caption_range_bot                   : 12;  // 16:27
	UINT32                                         : 2;   // 28:29 reserved
	UINT32 caption_flow_detected                   : 1;   //    30
	UINT32 hmc_when_cap_not_detected               : 1;   //    31
} H13A0_P0R_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x2524 p0r_ipc_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_vedge_th                            : 7;   //  0: 6
	UINT32 fmd_vedge_en                            : 1;   //     7
	UINT32 fmd_a_th                                : 6;   //  8:13
	UINT32 use_3field_err_t                        : 1;   //    14
	UINT32 two_field_delay_mode                    : 1;   //    15
	UINT32 fmd_margin_top                          : 8;   // 16:23
	UINT32 fmd_margin_bottom                       : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x2528 p0r_ipc_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 film_inter_with                         : 2;   //  0: 1
	UINT32 hw_film_enable                          : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 tearing_detect_strength                 : 1;   //     4
	UINT32 cross_check                             : 1;   //     5
	UINT32                                         : 1;   //     6 reserved
	UINT32 usediffpelcount                         : 1;   //     7
	UINT32 pn_diff_th                              : 8;   //  8:15
	UINT32                                         : 12;  // 16:27 reserved
	UINT32 scalemeasureforhd                       : 2;   // 28:29
	UINT32 loading_time_ctrl                       : 1;   //    30
	UINT32 fmd_sw_bad_edit_en                      : 1;   //    31
} H13A0_P0R_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x252c p0r_ipc_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_det_line_cnt_th                  : 8;   //  0: 7
	UINT32 repeat_det_diff_pel_cnt_th              : 8;   //  8:15
	UINT32 repeat_det_noise_th                     : 4;   // 16:19
} H13A0_P0R_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x2530 p0r_ipc_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_32_enable                           : 1;   //     0
	UINT32 fmd_22_enable                           : 1;   //     1
	UINT32 fmd_badediten                           : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 fmd_hw_bad_edit_mul_th                  : 3;   //  4: 6
	UINT32                                         : 1;   //     7 reserved
	UINT32 fmd_hw_bad_edit_th_min                  : 8;   //  8:15
	UINT32 pcn_diff_th                             : 7;   // 16:22
	UINT32 fmd_arbitrary_64only                    : 1;   //    23
	UINT32 fmd_stillconditition1                   : 1;   //    24
	UINT32 fmd_stillconditition2                   : 1;   //    25
	UINT32 fmd_arbitrary_still                     : 1;   //    26
	UINT32 fmd_arbitrary_32only                    : 1;   //    27
	UINT32 fmd_still_cntth                         : 3;   // 28:30
	UINT32 fmd_still_cntth_en                      : 1;   //    31
} H13A0_P0R_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x2534 p0r_ipc_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd32_stillth_pn                        : 8;   //  0: 7
	UINT32 fmd32_stillthpcn                        : 8;   //  8:15
	UINT32 fmd32_pn_maxvalue_minth0                : 8;   // 16:23
	UINT32 fmd32_pn_maxvalue_minth1                : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x2538 p0r_ipc_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd32_min_field_num                     : 4;   //  0: 3
	UINT32                                         : 4;   //  4: 7 reserved
	UINT32 fmd32_pn_minvalue_maxth                 : 8;   //  8:15
	UINT32 fmd32_pn_maxminratio0                   : 4;   // 16:19
	UINT32 fmd32_pn_maxminratio1                   : 4;   // 20:23
	UINT32 fmd_32_pcn_small_weight                 : 3;   // 24:26
} H13A0_P0R_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x253c p0r_ipc_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd22_stillth_pn                        : 8;   //  0: 7
	UINT32 fmd22_stillth_pcn                       : 8;   //  8:15
	UINT32 bad_edit_sm_min_th                      : 12;  // 16:27
} H13A0_P0R_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x2540 p0r_ipc_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd22_minfieldnum0                      : 4;   //  0: 3
	UINT32 fmd22_minfieldnum1                      : 4;   //  4: 7
	UINT32 fmd22_minvaluemaxth1                    : 8;   //  8:15
	UINT32 fmd22_maxvalueminth1                    : 8;   // 16:23
	UINT32 fmd22_maxminratio0                      : 4;   // 24:27
	UINT32 fmd22_maxminratio1                      : 4;   // 28:31
} H13A0_P0R_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x2544 p0r_ipc_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd22_maxvalueminth0                    : 8;   //  0: 7
	UINT32 fmd22_minvaluemaxth0                    : 8;   //  8:15
	UINT32 fmd32_minvaluemaxth0                    : 8;   // 16:23
	UINT32 fmd32_maxvalueminth0                    : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x2548 p0r_ipc_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                              : 8;   //  0: 7
	UINT32 cr_lr_gain                              : 8;   //  8:15
	UINT32 cr_h_grad_gain                          : 8;   // 16:23
	UINT32 cr_v_grad_gain                          : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x254c p0r_ipc_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 center_vld_gain                         : 4;   // 16:19
	UINT32 diff_vld_gain                           : 4;   // 20:23
} H13A0_P0R_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x2550 p0r_ipc_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pc_cnt                                  : 16;  //  0:15
	UINT32 nc_cnt                                  : 16;  // 16:31
} H13A0_P0R_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x2554 p0r_ipc_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_cnt                                  : 16;  //  0:15
	UINT32 same_cnt                                : 16;  // 16:31
} H13A0_P0R_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x2558 p0r_ipc_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd32_combwith                          : 2;   //  0: 1
	UINT32                                         : 1;   //     2 reserved
	UINT32 fmd32_detected                          : 1;   //     3
	UINT32 fmd32_lockingcnt                        : 8;   //  4:11
	UINT32 fmd22_combwith                          : 2;   // 12:13
	UINT32                                         : 1;   //    14 reserved
	UINT32 fmd22_detected                          : 1;   //    15
	UINT32 fmd22_lockingcnt                        : 8;   // 16:23
	UINT32 fmd_comb_predict                        : 2;   // 24:25
	UINT32 bad_edit_detected_hw                    : 1;   //    26
	UINT32 fmd22_still_detected                    : 1;   //    27
	UINT32 fmd32_still_detected                    : 1;   //    28
	UINT32 repeat_detected                         : 1;   //    29
} H13A0_P0R_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x255c p0r_ipc_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 fmd_badeditth_hw                        : 16;  // 16:31
} H13A0_P0R_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x2580 p0r_vflt_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vfilterenable                           : 1;   //     0
	UINT32 vfilterlocalenable                      : 1;   //     1
	UINT32 vfilter_strength                        : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 cb_filter_y                             : 2;   //  4: 5
	UINT32 cb_filter_c                             : 2;   //  6: 7
	UINT32 vfilter_a_th                            : 8;   //  8:15
	UINT32 vfilter_in_field_var                    : 8;   // 16:23
	UINT32 vfilter_use_mc_mask                     : 1;   //    24
	UINT32 vfilter_expand                          : 1;   //    25
	UINT32 vfilter_center                          : 1;   //    26
	UINT32 vfilter_ul4                             : 1;   //    27
	UINT32 dbgshow_level                           : 2;   // 28:29
	UINT32                                         : 1;   //    30 reserved
	UINT32 dbgshow_enable                          : 1;   //    31
} H13A0_P0R_VFLT_CTRL_T;

/*-----------------------------------------------------------------------------
	0x2588 p0r_mnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_enable                              : 1;   //     0
	UINT32 mnr_debug                               : 2;   //  1: 2
	UINT32                                         : 1;   //     3 reserved
	UINT32 h_expend                                : 1;   //     4
	UINT32 gain_flt_size                           : 1;   //     5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 mnr_master_gain                         : 8;   //  8:15
} H13A0_P0R_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x258c p0r_mnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_s1_mmd_min                          : 8;   //  0: 7
	UINT32 mnr_s2_ratio_min                        : 8;   //  8:15
	UINT32 mnr_s2_ratio_max                        : 8;   // 16:23
	UINT32 mnr_s2_mmd_min                          : 8;   // 24:31
} H13A0_P0R_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2590 p0r_mnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 mnr_h_gain                              : 8;   // 16:23
	UINT32 mnr_v_gain                              : 8;   // 24:31
} H13A0_P0R_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x2594 p0r_mnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 filter_x0                               : 8;   //  0: 7
	UINT32 filter_x1                               : 8;   //  8:15
	UINT32 filter_y0                               : 8;   // 16:23
	UINT32 filter_y1                               : 8;   // 24:31
} H13A0_P0R_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x25a8 p0r_vfilter_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en                    : 1;   //     0
	UINT32 cr_vfilter_force_en                     : 1;   //     1
	UINT32 cr_global_frame_num_th                  : 4;   //  2: 5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 cr_vfilter_global_th                    : 12;  //  8:19
	UINT32 tearing_count                           : 12;  // 20:31
} H13A0_P0R_VFILTER_01_T;

/*-----------------------------------------------------------------------------
	0x25ac p0r_bnr_dc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_enable                           : 1;   //     0
	UINT32 dc_bnr_var_en                           : 1;   //     1
	UINT32 dc_bnr_motion_en                        : 1;   //     2
	UINT32 dc_bnr_acadaptive                       : 1;   //     3
	UINT32 dc_bnr_sdhd_sel                         : 1;   //     4
	UINT32 dc_blur_sel                             : 2;   //  5: 6
	UINT32 dc_debug_mode                           : 1;   //     7
	UINT32 dc_bnr_mastergain                       : 8;   //  8:15
	UINT32 dc_bnr_chromagain                       : 9;   // 16:24
	UINT32 dc_protection_en                        : 1;   //    25
	UINT32                                         : 2;   // 26:27 reserved
	UINT32 dc_bnr_debug_mode                       : 4;   // 28:31
} H13A0_P0R_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x25b0 p0r_bnr_dc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_var_th3                          : 8;   //  0: 7
	UINT32 dc_bnr_var_th2                          : 8;   //  8:15
	UINT32 dc_bnr_var_th1                          : 8;   // 16:23
	UINT32 dc_bnr_var_th0                          : 8;   // 24:31
} H13A0_P0R_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x25b4 p0r_bnr_dc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_motion_th3                       : 8;   //  0: 7
	UINT32 dc_bnr_motion_th2                       : 8;   //  8:15
	UINT32 dc_bnr_motion_th1                       : 8;   // 16:23
	UINT32 dc_bnr_motion_th0                       : 8;   // 24:31
} H13A0_P0R_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x25b8 p0r_bnr_dc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_chroma_variance                      : 8;   //  0: 7
	UINT32 dc_var_h_gain                           : 8;   //  8:15
	UINT32 dc_protection_th                        : 8;   // 16:23
} H13A0_P0R_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x25bc p0r_bnr_ac_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 16;  //  0:15 reserved
	UINT32 ac_bnr_protect_motion_max               : 8;   // 16:23
	UINT32 ac_bnr_protect_motion_min               : 8;   // 24:31
} H13A0_P0R_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x25c0 p0r_bnr_ac_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_h_en                                : 1;   //     0
	UINT32 bnr_v_en                                : 1;   //     1
	UINT32 motion_protect_enable                   : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 source_type                             : 1;   //     4
	UINT32 status_read_type                        : 1;   //     5
	UINT32 status_read_mode                        : 2;   //  6: 7
	UINT32 hbmax_gain                              : 4;   //  8:11
	UINT32 vbmax_gain                              : 4;   // 12:15
	UINT32 strength_resolution                     : 1;   //    16
	UINT32                                         : 3;   // 17:19 reserved
	UINT32 fiter_type                              : 1;   //    20
	UINT32                                         : 3;   // 21:23 reserved
	UINT32 output_mux                              : 2;   // 24:25
} H13A0_P0R_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x25c4 p0r_bnr_ac_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_h_x0                           : 8;   //  0: 7
	UINT32 strength_h_x1                           : 8;   //  8:15
	UINT32 strength_h_max                          : 8;   // 16:23
	UINT32 detect_min_th                           : 8;   // 24:31
} H13A0_P0R_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x25c8 p0r_bnr_ac_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_v_x0                           : 8;   //  0: 7
	UINT32 strength_v_x1                           : 8;   //  8:15
	UINT32 strength_v_max                          : 8;   // 16:23
} H13A0_P0R_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x25cc p0r_bnr_ac_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset_mode                           : 1;   //     0
	UINT32 manual_offset_h_value                   : 3;   //  1: 3
	UINT32 v_offset_mode                           : 1;   //     4
	UINT32 manual_offset_v_value                   : 3;   //  5: 7
	UINT32 use_of_hysterisis                       : 4;   //  8:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 t_filter_weight                         : 8;   // 16:23
} H13A0_P0R_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x25d0 p0r_bnr_ac_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_delta_th0                           : 8;   //  0: 7
	UINT32 max_delta_th1                           : 8;   //  8:15
	UINT32 h_blockness_th                          : 8;   // 16:23
	UINT32 h_weight_max                            : 8;   // 24:31
} H13A0_P0R_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x25d4 p0r_bnr_ac_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 use_of_hysterisis                       : 4;   //  0: 3
	UINT32 block_boundary_processing_type          : 1;   //     4
} H13A0_P0R_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x25d8 p0r_dnr_max_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_snr                              : 1;   //     0
	UINT32 enable_mnr                              : 1;   //     1
	UINT32 enable_der                              : 1;   //     2
	UINT32 enable_dc_bnr                           : 1;   //     3
	UINT32 enable_ac_bnr                           : 1;   //     4
	UINT32                                         : 3;   //  5: 7 reserved
	UINT32 debug_enable                            : 1;   //     8
	UINT32 debug_mode                              : 1;   //     9
	UINT32 debug_snr_enable                        : 1;   //    10
	UINT32 debug_mnr_enable                        : 1;   //    11
	UINT32 debug_der_enable                        : 1;   //    12
	UINT32 debug_dc_bnr_enable                     : 1;   //    13
	UINT32 debug_ac_bnr_enable                     : 1;   //    14
	UINT32                                         : 1;   //    15 reserved
	UINT32 win_control_enable                      : 1;   //    16
	UINT32 border_enable                           : 1;   //    17
} H13A0_P0R_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
	0x25dc p0r_dnr_dbar_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                            : 1;   //     0
	UINT32                                         : 1;   //     1 reserved
	UINT32 show_debug_bar_for_bnr                  : 1;   //     2
	UINT32                                         : 1;   //     3 reserved
	UINT32 gain                                    : 3;   //  4: 6
	UINT32                                         : 1;   //     7 reserved
	UINT32 sd_hd_sel                               : 2;   //  8: 9
} H13A0_P0R_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
	0x25e0 p0r_bnr_stat_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                         : 8;   //  0: 7
	UINT32 ac_bnr_v_status                         : 8;   //  8:15
	UINT32 dc_bnr_th                               : 14;  // 16:29
} H13A0_P0R_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
	0x25e4 p0r_der_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 der_en                                  : 1;   //     0
	UINT32                                         : 1;   //     1 reserved
	UINT32 gain2weight_mapping                     : 1;   //     2
	UINT32 bif_en                                  : 1;   //     3
	UINT32 out_mux                                 : 4;   //  4: 7
	UINT32 bif_th                                  : 8;   //  8:15
} H13A0_P0R_DER_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x25e8 p0r_der_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain_th0                                : 8;   //  0: 7
	UINT32 gain_th1                                : 8;   //  8:15
} H13A0_P0R_DER_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x25f0 p0r_ipc_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_lock_dir_sel                         : 1;   //     0
	UINT32                                         : 7;   //  1: 7 reserved
	UINT32 cr_center_ratio                         : 4;   //  8:11
	UINT32 cr_lr_diff_ratio                        : 4;   // 12:15
	UINT32 cr_hmc_lock_filt_sizeh                  : 3;   // 16:18
} H13A0_P0R_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x25f4 p0r_ipc_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_lock_point_add                       : 8;   //  0: 7
	UINT32 cr_lock_point_penalty                   : 8;   //  8:15
	UINT32 cr_lock_point_valid                     : 8;   // 16:23
	UINT32                                         : 1;   //    24 reserved
	UINT32 cr_diff_limit                           : 4;   // 25:28
	UINT32 cr_count_check_en                       : 1;   //    29
	UINT32 cr_robustness                           : 2;   // 30:31
} H13A0_P0R_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x25f8 p0r_ipc_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0r_ipc_ctrl_22_f                       : 32;   // 31: 0
} H13A0_P0R_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x25fc p0r_ipc_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 global_tbm_en                           : 1;   //     0
	UINT32 flim_panelty_en                         : 1;   //     1
	UINT32 es_offset_mode                          : 1;   //     2
	UINT32 err_mode                                : 1;   //     3
	UINT32 hmc_motion_limit                        : 4;   //  4: 7
	UINT32 global_tbm_th                           : 8;   //  8:15
	UINT32 hmc_2d_gain                             : 8;   // 16:23
	UINT32 errs_limit                              : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x2600 p0r_ipc_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 errs_gain                               : 8;   //  0: 7
	UINT32 confidance_gain                         : 8;   //  8:15
	UINT32 pe0_2d_offset                           : 8;   // 16:23
	UINT32 pe0_2d_flicker_gain                     : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x2604 p0r_ipc_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cubic_coef0                             : 8;   //  0: 7
	UINT32 cubic_coef1                             : 8;   //  8:15
	UINT32 cubic_coef2                             : 8;   // 16:23
	UINT32 cubic_coef3                             : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x2608 p0r_ipc_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 va_cubic                                : 1;   //     0
	UINT32 boundary_window                         : 1;   //     1
	UINT32 force_va                                : 1;   //     2
	UINT32 half_pel_va                             : 1;   //     3
	UINT32 dir_med_tap                             : 2;   //  4: 5
	UINT32 diff_med_tap                            : 2;   //  6: 7
	UINT32 va_blending_ctrl1                       : 3;   //  8:10
	UINT32 wide_angle_protection_1                 : 1;   //    11
	UINT32 va_blending_ctrl2                       : 3;   // 12:14
	UINT32                                         : 1;   //    15 reserved
	UINT32 errs_chroma_blend_gain                  : 8;   // 16:23
	UINT32 strong_global_th                        : 6;   // 24:29
	UINT32 narrow_angle_week                       : 1;   //    30
	UINT32 narrow_angle_protection                 : 1;   //    31
} H13A0_P0R_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x260c p0r_ipc_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0                   : 8;   //  0: 7
	UINT32 chroma_st_flt_ctrl_x1                   : 8;   //  8:15
	UINT32 luma_st_flt_ctrl_x0                     : 8;   // 16:23
	UINT32 luma_st_flt_ctrl_x1                     : 8;   // 24:31
} H13A0_P0R_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x2610 p0r_ipc_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_gain                               : 8;   //  0: 7
	UINT32 amp_gain                                : 8;   //  8:15
} H13A0_P0R_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x2614 p0r_ipc_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_gain                               : 8;   //  0: 7
	UINT32 amp_gain                                : 8;   //  8:15
} H13A0_P0R_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x2618 p0r_ipc_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vmc_motion_th                           : 8;   //  0: 7
	UINT32 vmc_debug                               : 1;   //     8
	UINT32 vmc_en                                  : 1;   //     9
	UINT32 vmc_still_check                         : 1;   //    10
	UINT32 vmc_mul                                 : 3;   // 11:13
	UINT32 vmc_out_mul                             : 2;   // 14:15
	UINT32 vmc_protect_th                          : 8;   // 16:23
	UINT32 protection_en                           : 1;   //    24
} H13A0_P0R_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x2640 p0r_tpd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_en                                  : 1;   //     0
	UINT32                                         : 1;   //     1 reserved
	UINT32 win_mode                                : 1;   //     2
	UINT32                                         : 12;  //  3:14 reserved
	UINT32 quick_out                               : 1;   //    15
	UINT32 tpd_th_s_vl                             : 8;   // 16:23
	UINT32 tpd_th_s_vh                             : 8;   // 24:31
} H13A0_P0R_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2644 p0r_tpd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 12;  //  0:11
	UINT32                                         : 3;   // 12:14 reserved
	UINT32 win_tpd_hen                             : 1;   //    15
	UINT32 y0                                      : 12;  // 16:27
	UINT32                                         : 3;   // 28:30 reserved
	UINT32 win_tpd_ven                             : 1;   //    31
} H13A0_P0R_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2648 p0r_tpd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y1                                      : 12;  // 16:27
} H13A0_P0R_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x264c p0r_tpd_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_th_vl                               : 8;   //  0: 7
	UINT32 tpd_th_vh                               : 8;   //  8:15
	UINT32 vh_valid_th                             : 8;   // 16:23
	UINT32 vh_max_th                               : 8;   // 24:31
} H13A0_P0R_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2650 p0r_tpd_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpd_th_s                                : 8;   //  0: 7
	UINT32 sv_valid_th                             : 8;   //  8:15
	UINT32 vl_valid_th                             : 8;   // 16:23
	UINT32 vl_max_th                               : 8;   // 24:31
} H13A0_P0R_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x2654 p0r_tpd_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mid_level                               : 10;  //  0: 9
	UINT32                                         : 6;   // 10:15 reserved
	UINT32 tpd_th_sv_vl                            : 8;   // 16:23
	UINT32 tpd_th_sv_vh                            : 8;   // 24:31
} H13A0_P0R_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x2658 p0r_tpd_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 4;   //  0: 3 reserved
	UINT32 mux_3                                   : 4;   //  4: 7
	UINT32                                         : 4;   //  8:11 reserved
	UINT32 mux_2                                   : 4;   // 12:15
	UINT32                                         : 4;   // 16:19 reserved
	UINT32 mux_1                                   : 4;   // 20:23
	UINT32 show_scale                              : 3;   // 24:26
	UINT32 bbd_show                                : 1;   //    27
	UINT32 show_bar_1                              : 1;   //    28
	UINT32 show_bar_2                              : 1;   //    29
	UINT32 show_bar_3                              : 1;   //    30
	UINT32 show_bar_4                              : 1;   //    31
} H13A0_P0R_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x265c p0r_tpd_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                         : 4;   //  0: 3 reserved
	UINT32 mux_4                                   : 4;   //  4: 7
	UINT32 hys_mode                                : 6;   //  8:13
	UINT32                                         : 2;   // 14:15 reserved
	UINT32 tpd_th_s_sv                             : 5;   // 16:20
	UINT32                                         : 3;   // 21:23 reserved
	UINT32 tpd_th_sv                               : 5;   // 24:28
} H13A0_P0R_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x2670 p0r_bbd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_en                                  : 1;   //     0
	UINT32 hys_mode                                : 5;   //  1: 5
	UINT32 op_mode                                 : 1;   //     6
	UINT32                                         : 1;   //     7 reserved
	UINT32 cnt_th                                  : 8;   //  8:15
	UINT32 diff_th                                 : 8;   // 16:23
	UINT32 bbd_mux                                 : 3;   // 24:26
	UINT32                                         : 1;   //    27 reserved
	UINT32 apl_mux                                 : 3;   // 28:30
} H13A0_P0R_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2674 p0r_bbd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y0                                      : 12;  // 16:27
} H13A0_P0R_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2678 p0r_bbd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y1                                      : 12;  // 16:27
} H13A0_P0R_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x267c p0r_bbd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y0                                      : 12;  // 16:27
} H13A0_P0R_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x2680 p0r_bbd_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                      : 12;  //  0:11
	UINT32                                         : 4;   // 12:15 reserved
	UINT32 y1                                      : 12;  // 16:27
} H13A0_P0R_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x2684 p0r_apl_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0r_apl_stat_00_f                       : 32;   // 31: 0
} H13A0_P0R_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x2690 p0r_cti_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_enable                              : 1;   //     0
	UINT32 cti_repeat_en                           : 1;   //     1
	UINT32 cti_clipping_en                         : 1;   //     2
	UINT32                                         : 13;  //  3:15 reserved
	UINT32 cti_gain                                : 8;   // 16:23
} H13A0_P0R_CTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x26d0 p0r_post_tnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_post_tnr_enable                      : 1;   //     0
	UINT32 cr_post_tnr_manual_mode                 : 1;   //     1
	UINT32 cr_post_tnr_p1_enable                   : 1;   //     2
	UINT32                                         : 2;   //  3: 4 reserved
	UINT32 cr_post_tnr_c_enable                    : 1;   //     5
	UINT32                                         : 2;   //  6: 7 reserved
	UINT32 cr_p1_debug_mode                        : 1;   //     8
	UINT32 cr_p2_debug_mode                        : 1;   //     9
	UINT32 flicker_debug_mode                      : 1;   //    10
	UINT32                                         : 5;   // 11:15 reserved
	UINT32 cr_post_tnr_p2_th                       : 8;   // 16:23
	UINT32 cr_post_tnr_p1_th                       : 8;   // 24:31
} H13A0_P0R_POST_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x26d4 p0r_post_tnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_post_tnr_p1_x3                       : 8;   //  0: 7
	UINT32 cr_post_tnr_p1_x2                       : 8;   //  8:15
	UINT32 cr_post_tnr_p1_x1                       : 8;   // 16:23
	UINT32 cr_post_tnr_p1_x0                       : 8;   // 24:31
} H13A0_P0R_POST_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x26d8 p0r_post_tnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_post_tnr_p1_y3                       : 8;   //  0: 7
	UINT32 cr_post_tnr_p1_y2                       : 8;   //  8:15
	UINT32 cr_post_tnr_p1_y1                       : 8;   // 16:23
	UINT32 cr_post_tnr_p1_y0                       : 8;   // 24:31
} H13A0_P0R_POST_TNR_CTRL_02_T;

typedef struct {
	H13A0_P0L_PE0_LOAD_T            p0l_pe0_load;                    // 0x0400
	H13A0_P0L_PE0_INTR_T            p0l_pe0_intr;                    // 0x0404
	H13A0_P0L_PE0_STATUS0_T         p0l_pe0_status0;                 // 0x0408
	H13A0_P0L_PE0_STATUS1_T         p0l_pe0_status1;                 // 0x040c
	H13A0_P0L_PE0_OPMODE_T          p0l_pe0_opmode;                  // 0x0410
	H13A0_P0L_PE0_SRC_SIZE_T        p0l_pe0_src_size;                // 0x0414
	H13A0_P0L_PE0_FIELD_T           p0l_pe0_field;                   // 0x0418
	H13A0_P0L_PE0_PARAM_BY_DDR_T    p0l_pe0_param_by_ddr;            // 0x041c
	H13A0_P0L_WIN_CTRL_0_T          p0l_win_ctrl_0;                  // 0x0420
	H13A0_P0L_WIN_CTRL_1_T          p0l_win_ctrl_1;                  // 0x0424
	H13A0_P0L_WIN_CTRL_2_T          p0l_win_ctrl_2;                  // 0x0428
	H13A0_P0L_WIN_CTRL_3_T          p0l_win_ctrl_3;                  // 0x042c
	H13A0_P0L_PE0_DEBUG_MEM_REQ_T   p0l_pe0_debug_mem_req;           // 0x0430
	UINT32                          reserved0[3];                    // 0x0434~0x043c
	H13A0_P0L_TNR_CTRL_00_T         p0l_tnr_ctrl_00;                 // 0x0440
	H13A0_P0L_TNR_CTRL_01_T         p0l_tnr_ctrl_01;                 // 0x0444
	H13A0_P0L_TNR_CTRL_02_T         p0l_tnr_ctrl_02;                 // 0x0448
	H13A0_P0L_TNR_CTRL_03_T         p0l_tnr_ctrl_03;                 // 0x044c
	H13A0_P0L_TNR_CTRL_04_T         p0l_tnr_ctrl_04;                 // 0x0450
	H13A0_P0L_TNR_CTRL_05_T         p0l_tnr_ctrl_05;                 // 0x0454
	H13A0_P0L_TNR_CTRL_06_T         p0l_tnr_ctrl_06;                 // 0x0458
	H13A0_P0L_TNR_CTRL_07_T         p0l_tnr_ctrl_07;                 // 0x045c
	H13A0_P0L_TNR_CTRL_08_T         p0l_tnr_ctrl_08;                 // 0x0460
	H13A0_P0L_TNR_CTRL_09_T         p0l_tnr_ctrl_09;                 // 0x0464
	H13A0_P0L_TNR_CTRL_10_T         p0l_tnr_ctrl_10;                 // 0x0468
	H13A0_P0L_TNR_CTRL_11_T         p0l_tnr_ctrl_11;                 // 0x046c
	H13A0_P0L_TNR_CTRL_12_T         p0l_tnr_ctrl_12;                 // 0x0470
	H13A0_P0L_TNR_CTRL_13_T         p0l_tnr_ctrl_13;                 // 0x0474
	H13A0_P0L_TNR_CTRL_14_T         p0l_tnr_ctrl_14;                 // 0x0478
	H13A0_P0L_TNR_CTRL_15_T         p0l_tnr_ctrl_15;                 // 0x047c
	H13A0_P0L_TNR_CTRL_16_T         p0l_tnr_ctrl_16;                 // 0x0480
	H13A0_P0L_TNR_CTRL_17_T         p0l_tnr_ctrl_17;                 // 0x0484
	H13A0_P0L_TNR_CTRL_18_T         p0l_tnr_ctrl_18;                 // 0x0488
	H13A0_P0L_TNR_CTRL_19_T         p0l_tnr_ctrl_19;                 // 0x048c
	H13A0_P0L_TNR_CTRL_20_T         p0l_tnr_ctrl_20;                 // 0x0490
	H13A0_P0L_TNR_CTRL_21_T         p0l_tnr_ctrl_21;                 // 0x0494
	H13A0_P0L_TNR_CTRL_22_T         p0l_tnr_ctrl_22;                 // 0x0498
	H13A0_P0L_TNR_CTRL_23_T         p0l_tnr_ctrl_23;                 // 0x049c
	H13A0_P0L_TNR_CTRL_24_T         p0l_tnr_ctrl_24;                 // 0x04a0
	H13A0_P0L_TNR_CTRL_25_T         p0l_tnr_ctrl_25;                 // 0x04a4
	H13A0_P0L_TNR_CTRL_26_T         p0l_tnr_ctrl_26;                 // 0x04a8
	H13A0_P0L_TNR_CTRL_27_T         p0l_tnr_ctrl_27;                 // 0x04ac
	H13A0_P0L_TNR_CTRL_28_T         p0l_tnr_ctrl_28;                 // 0x04b0
	H13A0_P0L_TNR_CTRL_29_T         p0l_tnr_ctrl_29;                 // 0x04b4
	H13A0_P0L_TNR_CTRL_30_T         p0l_tnr_ctrl_30;                 // 0x04b8
	H13A0_P0L_TNR_CTRL_31_T         p0l_tnr_ctrl_31;                 // 0x04bc
	H13A0_P0L_TNR_STATUS_00_T       p0l_tnr_status_00;               // 0x04c0
	H13A0_P0L_TNR_STATUS_01_T       p0l_tnr_status_01;               // 0x04c4
	H13A0_P0L_TNR_STATUS_02_T       p0l_tnr_status_02;               // 0x04c8
	H13A0_P0L_TNR_STATUS_03_T       p0l_tnr_status_03;               // 0x04cc
	H13A0_P0L_TNR_MAIN_LUT_00_T     p0l_tnr_main_lut_00;             // 0x04d0
	H13A0_P0L_TNR_MAIN_LUT_01_T     p0l_tnr_main_lut_01;             // 0x04d4
	H13A0_P0L_TNR_MAIN_LUT_02_T     p0l_tnr_main_lut_02;             // 0x04d8
	UINT32                          reserved1;                       // 0x04dc
	H13A0_P0L_TNR_CTRL_32_T         p0l_tnr_ctrl_32;                 // 0x04e0
	H13A0_P0L_TNR_CTRL_33_T         p0l_tnr_ctrl_33;                 // 0x04e4
	H13A0_P0L_TNR_CTRL_34_T         p0l_tnr_ctrl_34;                 // 0x04e8
	H13A0_P0L_TNR_CTRL_35_T         p0l_tnr_ctrl_35;                 // 0x04ec
	H13A0_P0L_TNR_CRG_CTRL_00_T     p0l_tnr_crg_ctrl_00;             // 0x04f0
	H13A0_P0L_TNR_CRG_CTRL_01_T     p0l_tnr_crg_ctrl_01;             // 0x04f4
	H13A0_P0L_TNR_CTRL_36_T         p0l_tnr_ctrl_36;                 // 0x04f8
	H13A0_P0L_TNR_CTRL_37_T         p0l_tnr_ctrl_37;                 // 0x04fc
	H13A0_P0L_IPC_CTRL_00_T         p0l_ipc_ctrl_00;                 // 0x0500
	H13A0_P0L_IPC_CTRL_01_T         p0l_ipc_ctrl_01;                 // 0x0504
	H13A0_P0L_IPC_CTRL_02_T         p0l_ipc_ctrl_02;                 // 0x0508
	H13A0_P0L_IPC_CTRL_03_T         p0l_ipc_ctrl_03;                 // 0x050c
	H13A0_P0L_IPC_CTRL_04_T         p0l_ipc_ctrl_04;                 // 0x0510
	H13A0_P0L_IPC_CTRL_05_T         p0l_ipc_ctrl_05;                 // 0x0514
	H13A0_P0L_IPC_CTRL_06_T         p0l_ipc_ctrl_06;                 // 0x0518
	H13A0_P0L_IPC_CTRL_07_T         p0l_ipc_ctrl_07;                 // 0x051c
	H13A0_P0L_IPC_CTRL_08_T         p0l_ipc_ctrl_08;                 // 0x0520
	H13A0_P0L_IPC_CTRL_09_T         p0l_ipc_ctrl_09;                 // 0x0524
	H13A0_P0L_IPC_CTRL_10_T         p0l_ipc_ctrl_10;                 // 0x0528
	H13A0_P0L_IPC_CTRL_11_T         p0l_ipc_ctrl_11;                 // 0x052c
	H13A0_P0L_IPC_CTRL_12_T         p0l_ipc_ctrl_12;                 // 0x0530
	H13A0_P0L_IPC_CTRL_13_T         p0l_ipc_ctrl_13;                 // 0x0534
	H13A0_P0L_IPC_CTRL_14_T         p0l_ipc_ctrl_14;                 // 0x0538
	H13A0_P0L_IPC_CTRL_15_T         p0l_ipc_ctrl_15;                 // 0x053c
	H13A0_P0L_IPC_CTRL_16_T         p0l_ipc_ctrl_16;                 // 0x0540
	H13A0_P0L_IPC_CTRL_17_T         p0l_ipc_ctrl_17;                 // 0x0544
	H13A0_P0L_IPC_CTRL_18_T         p0l_ipc_ctrl_18;                 // 0x0548
	H13A0_P0L_IPC_CTRL_19_T         p0l_ipc_ctrl_19;                 // 0x054c
	H13A0_P0L_IPC_STATUS_00_T       p0l_ipc_status_00;               // 0x0550
	H13A0_P0L_IPC_STATUS_01_T       p0l_ipc_status_01;               // 0x0554
	H13A0_P0L_IPC_STATUS_02_T       p0l_ipc_status_02;               // 0x0558
	H13A0_P0L_IPC_STATUS_03_T       p0l_ipc_status_03;               // 0x055c
	H13A0_CLC_CTRL_00_T             clc_ctrl_00;                     // 0x0560
	H13A0_CLC_CTRL_01_T             clc_ctrl_01;                     // 0x0564
	H13A0_CLC_CTRL_02_T             clc_ctrl_02;                     // 0x0568
	H13A0_CLC_CTRL_03_T             clc_ctrl_03;                     // 0x056c
	H13A0_CLC_CTRL_04_T             clc_ctrl_04;                     // 0x0570
	H13A0_CLC_CTRL_05_T             clc_ctrl_05;                     // 0x0574
	H13A0_CLC_CTRL_06_T             clc_ctrl_06;                     // 0x0578
	H13A0_CLC_STAT_00_T             clc_stat_00;                     // 0x057c
	H13A0_P0L_VFLT_CTRL_T           p0l_vflt_ctrl;                   // 0x0580
	UINT32                          reserved2;                       // 0x0584
	H13A0_P0L_MNR_CTRL_00_T         p0l_mnr_ctrl_00;                 // 0x0588
	H13A0_P0L_MNR_CTRL_01_T         p0l_mnr_ctrl_01;                 // 0x058c
	H13A0_P0L_MNR_CTRL_02_T         p0l_mnr_ctrl_02;                 // 0x0590
	H13A0_P0L_MNR_CTRL_03_T         p0l_mnr_ctrl_03;                 // 0x0594
	UINT32                          reserved3[4];                    // 0x0598~0x05a4
	H13A0_P0L_VFILTER_01_T          p0l_vfilter_01;                  // 0x05a8
	H13A0_P0L_BNR_DC_CTRL_00_T      p0l_bnr_dc_ctrl_00;              // 0x05ac
	H13A0_P0L_BNR_DC_CTRL_01_T      p0l_bnr_dc_ctrl_01;              // 0x05b0
	H13A0_P0L_BNR_DC_CTRL_02_T      p0l_bnr_dc_ctrl_02;              // 0x05b4
	H13A0_P0L_BNR_DC_CTRL_03_T      p0l_bnr_dc_ctrl_03;              // 0x05b8
	H13A0_P0L_BNR_AC_CTRL_06_T      p0l_bnr_ac_ctrl_06;              // 0x05bc
	H13A0_P0L_BNR_AC_CTRL_00_T      p0l_bnr_ac_ctrl_00;              // 0x05c0
	H13A0_P0L_BNR_AC_CTRL_01_T      p0l_bnr_ac_ctrl_01;              // 0x05c4
	H13A0_P0L_BNR_AC_CTRL_02_T      p0l_bnr_ac_ctrl_02;              // 0x05c8
	H13A0_P0L_BNR_AC_CTRL_03_T      p0l_bnr_ac_ctrl_03;              // 0x05cc
	H13A0_P0L_BNR_AC_CTRL_04_T      p0l_bnr_ac_ctrl_04;              // 0x05d0
	H13A0_P0L_BNR_AC_CTRL_05_T      p0l_bnr_ac_ctrl_05;              // 0x05d4
	H13A0_P0L_DNR_MAX_CTRL_T        p0l_dnr_max_ctrl;                // 0x05d8
	H13A0_P0L_DNR_DBAR_CTRL_T       p0l_dnr_dbar_ctrl;               // 0x05dc
	H13A0_P0L_BNR_STAT_0_T          p0l_bnr_stat_0;                  // 0x05e0
	H13A0_P0L_DER_CTRL_0_T          p0l_der_ctrl_0;                  // 0x05e4
	H13A0_P0L_DER_CTRL_1_T          p0l_der_ctrl_1;                  // 0x05e8
	UINT32                          reserved4;                       // 0x05ec
	H13A0_P0L_IPC_CTRL_20_T         p0l_ipc_ctrl_20;                 // 0x05f0
	H13A0_P0L_IPC_CTRL_21_T         p0l_ipc_ctrl_21;                 // 0x05f4
	H13A0_P0L_IPC_CTRL_22_T         p0l_ipc_ctrl_22;                 // 0x05f8
	H13A0_P0L_IPC_CTRL_23_T         p0l_ipc_ctrl_23;                 // 0x05fc
	H13A0_P0L_IPC_CTRL_24_T         p0l_ipc_ctrl_24;                 // 0x0600
	H13A0_P0L_IPC_CTRL_25_T         p0l_ipc_ctrl_25;                 // 0x0604
	H13A0_P0L_IPC_CTRL_26_T         p0l_ipc_ctrl_26;                 // 0x0608
	H13A0_P0L_IPC_CTRL_27_T         p0l_ipc_ctrl_27;                 // 0x060c
	H13A0_P0L_IPC_CTRL_28_T         p0l_ipc_ctrl_28;                 // 0x0610
	H13A0_P0L_IPC_CTRL_29_T         p0l_ipc_ctrl_29;                 // 0x0614
	H13A0_P0L_IPC_CTRL_30_T         p0l_ipc_ctrl_30;                 // 0x0618
	UINT32                          reserved5[9];                    // 0x061c~0x063c
	H13A0_P0L_TPD_CTRL_00_T         p0l_tpd_ctrl_00;                 // 0x0640
	H13A0_P0L_TPD_CTRL_01_T         p0l_tpd_ctrl_01;                 // 0x0644
	H13A0_P0L_TPD_CTRL_02_T         p0l_tpd_ctrl_02;                 // 0x0648
	H13A0_P0L_TPD_CTRL_03_T         p0l_tpd_ctrl_03;                 // 0x064c
	H13A0_P0L_TPD_CTRL_04_T         p0l_tpd_ctrl_04;                 // 0x0650
	H13A0_P0L_TPD_CTRL_05_T         p0l_tpd_ctrl_05;                 // 0x0654
	H13A0_P0L_TPD_CTRL_06_T         p0l_tpd_ctrl_06;                 // 0x0658
	H13A0_P0L_TPD_CTRL_07_T         p0l_tpd_ctrl_07;                 // 0x065c
	H13A0_TPD_STAT_00_T             tpd_stat_00;                     // 0x0660
	H13A0_TPD_STAT_01_T             tpd_stat_01;                     // 0x0664
	UINT32                          reserved6[2];                    // 0x0668~0x066c
	H13A0_P0L_BBD_CTRL_00_T         p0l_bbd_ctrl_00;                 // 0x0670
	H13A0_P0L_BBD_CTRL_01_T         p0l_bbd_ctrl_01;                 // 0x0674
	H13A0_P0L_BBD_CTRL_02_T         p0l_bbd_ctrl_02;                 // 0x0678
	H13A0_P0L_BBD_STAT_00_T         p0l_bbd_stat_00;                 // 0x067c
	H13A0_P0L_BBD_STAT_01_T         p0l_bbd_stat_01;                 // 0x0680
	H13A0_P0L_APL_STAT_00_T         p0l_apl_stat_00;                 // 0x0684
	UINT32                          reserved7[2];                    // 0x0688~0x068c
	H13A0_P0L_CTI_CTRL_00_T         p0l_cti_ctrl_00;                 // 0x0690
	UINT32                          reserved8[3];                    // 0x0694~0x069c
	H13A0_PE0_3DFD_CTRL_00_T        pe0_3dfd_ctrl_00;                // 0x06a0
	H13A0_PE0_3DFD_CTRL_01_T        pe0_3dfd_ctrl_01;                // 0x06a4
	H13A0_PE0_3DFD_CTRL_02_T        pe0_3dfd_ctrl_02;                // 0x06a8
	H13A0_PE0_3DFD_CTRL_03_T        pe0_3dfd_ctrl_03;                // 0x06ac
	H13A0_PE0_3DFD_STAT_00_T        pe0_3dfd_stat_00;                // 0x06b0
	H13A0_PE0_3DFD_STAT_01_T        pe0_3dfd_stat_01;                // 0x06b4
	H13A0_PE0_3DFD_STAT_02_T        pe0_3dfd_stat_02;                // 0x06b8
	H13A0_PE0_3DFD_STAT_03_T        pe0_3dfd_stat_03;                // 0x06bc
	H13A0_PE0_3DFD_STAT_04_T        pe0_3dfd_stat_04;                // 0x06c0
	H13A0_PE0_3DFD_STAT_05_T        pe0_3dfd_stat_05;                // 0x06c4
	H13A0_PE0_3DFD_STAT_06_T        pe0_3dfd_stat_06;                // 0x06c8
	H13A0_PE0_3DFD_STAT_07_T        pe0_3dfd_stat_07;                // 0x06cc
	H13A0_P0L_POST_TNR_CTRL_00_T    p0l_post_tnr_ctrl_00;            // 0x06d0
	H13A0_P0L_POST_TNR_CTRL_01_T    p0l_post_tnr_ctrl_01;            // 0x06d4
	H13A0_P0L_POST_TNR_CTRL_02_T    p0l_post_tnr_ctrl_02;            // 0x06d8
} DE_P0L_REG_H13A0_T;

typedef struct {
	H13A0_P0R_PE0_LOAD_T            p0r_pe0_load;                    // 0x2400
	H13A0_P0R_PE0_INTR_T            p0r_pe0_intr;                    // 0x2404
	H13A0_P0R_PE0_STATUS0_T         p0r_pe0_status0;                 // 0x2408
	H13A0_P0R_PE0_STATUS1_T         p0r_pe0_status1;                 // 0x240c
	H13A0_P0R_PE0_OPMODE_T          p0r_pe0_opmode;                  // 0x2410
	H13A0_P0R_PE0_SRC_SIZE_T        p0r_pe0_src_size;                // 0x2414
	H13A0_P0R_PE0_FIELD_T           p0r_pe0_field;                   // 0x2418
	H13A0_P0R_PE0_PARAM_BY_DDR_T    p0r_pe0_param_by_ddr;            // 0x241c
	H13A0_P0R_WIN_CTRL_0_T          p0r_win_ctrl_0;                  // 0x2420
	H13A0_P0R_WIN_CTRL_1_T          p0r_win_ctrl_1;                  // 0x2424
	H13A0_P0R_WIN_CTRL_2_T          p0r_win_ctrl_2;                  // 0x2428
	H13A0_P0R_WIN_CTRL_3_T          p0r_win_ctrl_3;                  // 0x242c
	H13A0_P0R_PE0_DEBUG_MEM_REQ_T   p0r_pe0_debug_mem_req;           // 0x2430
	UINT32                          reserved9[3];                    // 0x2434~0x243c
	H13A0_P0R_TNR_CTRL_00_T         p0r_tnr_ctrl_00;                 // 0x2440
	H13A0_P0R_TNR_CTRL_01_T         p0r_tnr_ctrl_01;                 // 0x2444
	H13A0_P0R_TNR_CTRL_02_T         p0r_tnr_ctrl_02;                 // 0x2448
	H13A0_P0R_TNR_CTRL_03_T         p0r_tnr_ctrl_03;                 // 0x244c
	H13A0_P0R_TNR_CTRL_04_T         p0r_tnr_ctrl_04;                 // 0x2450
	H13A0_P0R_TNR_CTRL_05_T         p0r_tnr_ctrl_05;                 // 0x2454
	H13A0_P0R_TNR_CTRL_06_T         p0r_tnr_ctrl_06;                 // 0x2458
	H13A0_P0R_TNR_CTRL_07_T         p0r_tnr_ctrl_07;                 // 0x245c
	H13A0_P0R_TNR_CTRL_08_T         p0r_tnr_ctrl_08;                 // 0x2460
	H13A0_P0R_TNR_CTRL_09_T         p0r_tnr_ctrl_09;                 // 0x2464
	H13A0_P0R_TNR_CTRL_10_T         p0r_tnr_ctrl_10;                 // 0x2468
	H13A0_P0R_TNR_CTRL_11_T         p0r_tnr_ctrl_11;                 // 0x246c
	H13A0_P0R_TNR_CTRL_12_T         p0r_tnr_ctrl_12;                 // 0x2470
	H13A0_P0R_TNR_CTRL_13_T         p0r_tnr_ctrl_13;                 // 0x2474
	H13A0_P0R_TNR_CTRL_14_T         p0r_tnr_ctrl_14;                 // 0x2478
	H13A0_P0R_TNR_CTRL_15_T         p0r_tnr_ctrl_15;                 // 0x247c
	H13A0_P0R_TNR_CTRL_16_T         p0r_tnr_ctrl_16;                 // 0x2480
	H13A0_P0R_TNR_CTRL_17_T         p0r_tnr_ctrl_17;                 // 0x2484
	H13A0_P0R_TNR_CTRL_18_T         p0r_tnr_ctrl_18;                 // 0x2488
	H13A0_P0R_TNR_CTRL_19_T         p0r_tnr_ctrl_19;                 // 0x248c
	H13A0_P0R_TNR_CTRL_20_T         p0r_tnr_ctrl_20;                 // 0x2490
	H13A0_P0R_TNR_CTRL_21_T         p0r_tnr_ctrl_21;                 // 0x2494
	H13A0_P0R_TNR_CTRL_22_T         p0r_tnr_ctrl_22;                 // 0x2498
	H13A0_P0R_TNR_CTRL_23_T         p0r_tnr_ctrl_23;                 // 0x249c
	H13A0_P0R_TNR_CTRL_24_T         p0r_tnr_ctrl_24;                 // 0x24a0
	H13A0_P0R_TNR_CTRL_25_T         p0r_tnr_ctrl_25;                 // 0x24a4
	H13A0_P0R_TNR_CTRL_26_T         p0r_tnr_ctrl_26;                 // 0x24a8
	H13A0_P0R_TNR_CTRL_27_T         p0r_tnr_ctrl_27;                 // 0x24ac
	H13A0_P0R_TNR_CTRL_28_T         p0r_tnr_ctrl_28;                 // 0x24b0
	H13A0_P0R_TNR_CTRL_29_T         p0r_tnr_ctrl_29;                 // 0x24b4
	H13A0_P0R_TNR_CTRL_30_T         p0r_tnr_ctrl_30;                 // 0x24b8
	H13A0_P0R_TNR_CTRL_31_T         p0r_tnr_ctrl_31;                 // 0x24bc
	H13A0_P0R_TNR_STATUS_00_T       p0r_tnr_status_00;               // 0x24c0
	H13A0_P0R_TNR_STATUS_01_T       p0r_tnr_status_01;               // 0x24c4
	H13A0_P0R_TNR_STATUS_02_T       p0r_tnr_status_02;               // 0x24c8
	H13A0_P0R_TNR_STATUS_03_T       p0r_tnr_status_03;               // 0x24cc
	H13A0_P0R_TNR_MAIN_LUT_00_T     p0r_tnr_main_lut_00;             // 0x24d0
	H13A0_P0R_TNR_MAIN_LUT_01_T     p0r_tnr_main_lut_01;             // 0x24d4
	H13A0_P0R_TNR_MAIN_LUT_02_T     p0r_tnr_main_lut_02;             // 0x24d8
	UINT32                          reserved10;                      // 0x24dc
	H13A0_P0R_TNR_CTRL_32_T         p0r_tnr_ctrl_32;                 // 0x24e0
	H13A0_P0R_TNR_CTRL_33_T         p0r_tnr_ctrl_33;                 // 0x24e4
	H13A0_P0R_TNR_CTRL_34_T         p0r_tnr_ctrl_34;                 // 0x24e8
	H13A0_P0R_TNR_CTRL_35_T         p0r_tnr_ctrl_35;                 // 0x24ec
	H13A0_P0R_TNR_CRG_CTRL_00_T     p0r_tnr_crg_ctrl_00;             // 0x24f0
	H13A0_P0R_TNR_CRG_CTRL_01_T     p0r_tnr_crg_ctrl_01;             // 0x24f4
	H13A0_P0R_TNR_CTRL_36_T         p0r_tnr_ctrl_36;                 // 0x24f8
	H13A0_P0R_TNR_CTRL_37_T         p0r_tnr_ctrl_37;                 // 0x24fc
	H13A0_P0R_IPC_CTRL_00_T         p0r_ipc_ctrl_00;                 // 0x2500
	H13A0_P0R_IPC_CTRL_01_T         p0r_ipc_ctrl_01;                 // 0x2504
	H13A0_P0R_IPC_CTRL_02_T         p0r_ipc_ctrl_02;                 // 0x2508
	H13A0_P0R_IPC_CTRL_03_T         p0r_ipc_ctrl_03;                 // 0x250c
	H13A0_P0R_IPC_CTRL_04_T         p0r_ipc_ctrl_04;                 // 0x2510
	H13A0_P0R_IPC_CTRL_05_T         p0r_ipc_ctrl_05;                 // 0x2514
	H13A0_P0R_IPC_CTRL_06_T         p0r_ipc_ctrl_06;                 // 0x2518
	H13A0_P0R_IPC_CTRL_07_T         p0r_ipc_ctrl_07;                 // 0x251c
	H13A0_P0R_IPC_CTRL_08_T         p0r_ipc_ctrl_08;                 // 0x2520
	H13A0_P0R_IPC_CTRL_09_T         p0r_ipc_ctrl_09;                 // 0x2524
	H13A0_P0R_IPC_CTRL_10_T         p0r_ipc_ctrl_10;                 // 0x2528
	H13A0_P0R_IPC_CTRL_11_T         p0r_ipc_ctrl_11;                 // 0x252c
	H13A0_P0R_IPC_CTRL_12_T         p0r_ipc_ctrl_12;                 // 0x2530
	H13A0_P0R_IPC_CTRL_13_T         p0r_ipc_ctrl_13;                 // 0x2534
	H13A0_P0R_IPC_CTRL_14_T         p0r_ipc_ctrl_14;                 // 0x2538
	H13A0_P0R_IPC_CTRL_15_T         p0r_ipc_ctrl_15;                 // 0x253c
	H13A0_P0R_IPC_CTRL_16_T         p0r_ipc_ctrl_16;                 // 0x2540
	H13A0_P0R_IPC_CTRL_17_T         p0r_ipc_ctrl_17;                 // 0x2544
	H13A0_P0R_IPC_CTRL_18_T         p0r_ipc_ctrl_18;                 // 0x2548
	H13A0_P0R_IPC_CTRL_19_T         p0r_ipc_ctrl_19;                 // 0x254c
	H13A0_P0R_IPC_STATUS_00_T       p0r_ipc_status_00;               // 0x2550
	H13A0_P0R_IPC_STATUS_01_T       p0r_ipc_status_01;               // 0x2554
	H13A0_P0R_IPC_STATUS_02_T       p0r_ipc_status_02;               // 0x2558
	H13A0_P0R_IPC_STATUS_03_T       p0r_ipc_status_03;               // 0x255c
	UINT32                          reserved11[8];                   // 0x2560~0x257c
	H13A0_P0R_VFLT_CTRL_T           p0r_vflt_ctrl;                   // 0x2580
	UINT32                          reserved12;                      // 0x2584
	H13A0_P0R_MNR_CTRL_00_T         p0r_mnr_ctrl_00;                 // 0x2588
	H13A0_P0R_MNR_CTRL_01_T         p0r_mnr_ctrl_01;                 // 0x258c
	H13A0_P0R_MNR_CTRL_02_T         p0r_mnr_ctrl_02;                 // 0x2590
	H13A0_P0R_MNR_CTRL_03_T         p0r_mnr_ctrl_03;                 // 0x2594
	UINT32                          reserved13[4];                   // 0x2598~0x25a4
	H13A0_P0R_VFILTER_01_T          p0r_vfilter_01;                  // 0x25a8
	H13A0_P0R_BNR_DC_CTRL_00_T      p0r_bnr_dc_ctrl_00;              // 0x25ac
	H13A0_P0R_BNR_DC_CTRL_01_T      p0r_bnr_dc_ctrl_01;              // 0x25b0
	H13A0_P0R_BNR_DC_CTRL_02_T      p0r_bnr_dc_ctrl_02;              // 0x25b4
	H13A0_P0R_BNR_DC_CTRL_03_T      p0r_bnr_dc_ctrl_03;              // 0x25b8
	H13A0_P0R_BNR_AC_CTRL_06_T      p0r_bnr_ac_ctrl_06;              // 0x25bc
	H13A0_P0R_BNR_AC_CTRL_00_T      p0r_bnr_ac_ctrl_00;              // 0x25c0
	H13A0_P0R_BNR_AC_CTRL_01_T      p0r_bnr_ac_ctrl_01;              // 0x25c4
	H13A0_P0R_BNR_AC_CTRL_02_T      p0r_bnr_ac_ctrl_02;              // 0x25c8
	H13A0_P0R_BNR_AC_CTRL_03_T      p0r_bnr_ac_ctrl_03;              // 0x25cc
	H13A0_P0R_BNR_AC_CTRL_04_T      p0r_bnr_ac_ctrl_04;              // 0x25d0
	H13A0_P0R_BNR_AC_CTRL_05_T      p0r_bnr_ac_ctrl_05;              // 0x25d4
	H13A0_P0R_DNR_MAX_CTRL_T        p0r_dnr_max_ctrl;                // 0x25d8
	H13A0_P0R_DNR_DBAR_CTRL_T       p0r_dnr_dbar_ctrl;               // 0x25dc
	H13A0_P0R_BNR_STAT_0_T          p0r_bnr_stat_0;                  // 0x25e0
	H13A0_P0R_DER_CTRL_0_T          p0r_der_ctrl_0;                  // 0x25e4
	H13A0_P0R_DER_CTRL_1_T          p0r_der_ctrl_1;                  // 0x25e8
	UINT32                          reserved14;                      // 0x25ec
	H13A0_P0R_IPC_CTRL_20_T         p0r_ipc_ctrl_20;                 // 0x25f0
	H13A0_P0R_IPC_CTRL_21_T         p0r_ipc_ctrl_21;                 // 0x25f4
	H13A0_P0R_IPC_CTRL_22_T         p0r_ipc_ctrl_22;                 // 0x25f8
	H13A0_P0R_IPC_CTRL_23_T         p0r_ipc_ctrl_23;                 // 0x25fc
	H13A0_P0R_IPC_CTRL_24_T         p0r_ipc_ctrl_24;                 // 0x2600
	H13A0_P0R_IPC_CTRL_25_T         p0r_ipc_ctrl_25;                 // 0x2604
	H13A0_P0R_IPC_CTRL_26_T         p0r_ipc_ctrl_26;                 // 0x2608
	H13A0_P0R_IPC_CTRL_27_T         p0r_ipc_ctrl_27;                 // 0x260c
	H13A0_P0R_IPC_CTRL_28_T         p0r_ipc_ctrl_28;                 // 0x2610
	H13A0_P0R_IPC_CTRL_29_T         p0r_ipc_ctrl_29;                 // 0x2614
	H13A0_P0R_IPC_CTRL_30_T         p0r_ipc_ctrl_30;                 // 0x2618
	UINT32                          reserved15[9];                   // 0x261c~0x263c
	H13A0_P0R_TPD_CTRL_00_T         p0r_tpd_ctrl_00;                 // 0x2640
	H13A0_P0R_TPD_CTRL_01_T         p0r_tpd_ctrl_01;                 // 0x2644
	H13A0_P0R_TPD_CTRL_02_T         p0r_tpd_ctrl_02;                 // 0x2648
	H13A0_P0R_TPD_CTRL_03_T         p0r_tpd_ctrl_03;                 // 0x264c
	H13A0_P0R_TPD_CTRL_04_T         p0r_tpd_ctrl_04;                 // 0x2650
	H13A0_P0R_TPD_CTRL_05_T         p0r_tpd_ctrl_05;                 // 0x2654
	H13A0_P0R_TPD_CTRL_06_T         p0r_tpd_ctrl_06;                 // 0x2658
	H13A0_P0R_TPD_CTRL_07_T         p0r_tpd_ctrl_07;                 // 0x265c
	UINT32                          reserved16[4];                   // 0x2660~0x266c
	H13A0_P0R_BBD_CTRL_00_T         p0r_bbd_ctrl_00;                 // 0x2670
	H13A0_P0R_BBD_CTRL_01_T         p0r_bbd_ctrl_01;                 // 0x2674
	H13A0_P0R_BBD_CTRL_02_T         p0r_bbd_ctrl_02;                 // 0x2678
	H13A0_P0R_BBD_STAT_00_T         p0r_bbd_stat_00;                 // 0x267c
	H13A0_P0R_BBD_STAT_01_T         p0r_bbd_stat_01;                 // 0x2680
	H13A0_P0R_APL_STAT_00_T         p0r_apl_stat_00;                 // 0x2684
	UINT32                          reserved17[2];                   // 0x2688~0x268c
	H13A0_P0R_CTI_CTRL_00_T         p0r_cti_ctrl_00;                 // 0x2690
	UINT32                          reserved18[15];                  // 0x2694~0x26cc
	H13A0_P0R_POST_TNR_CTRL_00_T    p0r_post_tnr_ctrl_00;            // 0x26d0
	H13A0_P0R_POST_TNR_CTRL_01_T    p0r_post_tnr_ctrl_01;            // 0x26d4
	H13A0_P0R_POST_TNR_CTRL_02_T    p0r_post_tnr_ctrl_02;            // 0x26d8
} DE_P0R_REG_H13A0_T;

#endif
