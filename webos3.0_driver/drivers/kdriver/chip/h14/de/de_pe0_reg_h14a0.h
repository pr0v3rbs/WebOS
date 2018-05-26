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

#ifndef _REG_PE0_H14A0_h
#define _REG_PE0_H14A0_h

/*-----------------------------------------------------------------------------
	0x0400 p0l_pe0_load ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                       : 1;   //     0
	UINT32 load_enable                     : 1;   //     1
	UINT32 load_type                       : 1;   //     2
} H14A0_P0L_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
	0x0404 p0l_pe0_intr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                     : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 intr_src                        : 3;   //  4: 6
	UINT32                                 : 9;   //  7:15 reserved
	UINT32 intr_line_pos                   : 11;  // 16:26
} H14A0_P0L_PE0_INTR_T;

/*-----------------------------------------------------------------------------
	0x0408 p0l_pe0_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                    : 4;   //  0: 3
	UINT32 ipc_frame_id                    : 4;   //  4: 7
	UINT32 tnrw_y_frame_id                 : 4;   //  8:11
	UINT32 tnrw_c_frame_id                 : 4;   // 12:15
	UINT32 dnr_frame_id                    : 8;   // 16:23
	UINT32 pic_init_frame_id               : 4;   // 24:27
} H14A0_P0L_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x040c p0l_pe0_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                        : 11;  //  0:10
} H14A0_P0L_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x0410 p0l_pe0_opmode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                        : 1;   //     0
	UINT32 tnr_y_en                        : 1;   //     1
	UINT32 ipc_c_en                        : 1;   //     2
	UINT32 ipc_y_en                        : 1;   //     3
	UINT32 tnr_c_444_as_420                : 1;   //     4
	UINT32 tnr_y2_en                       : 1;   //     5
	UINT32 force_422_to_420                : 1;   //     6
	UINT32 ipc0_y_en_prog                  : 1;   //     7
	UINT32 mif_last_mode                   : 3;   //  8:10
	UINT32                                 : 1;   //    11 reserved
	UINT32 dnr_out_force_44x               : 1;   //    12
	UINT32 tnr_m_en                        : 1;   //    13
	UINT32 force_444_to_422                : 1;   //    14
	UINT32                                 : 1;   //    15 reserved
	UINT32 detour_enable                   : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 ibuf_en                         : 2;   // 20:21
	UINT32                                 : 2;   // 22:23 reserved
	UINT32 dnr_in_lsb_mode                 : 2;   // 24:25
	UINT32 dnr_out_lsb_mode                : 2;   // 26:27
	UINT32 tnr_in_lsb_mode                 : 2;   // 28:29
} H14A0_P0L_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
	0x0414 p0l_pe0_src_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                           : 11;  //  0:10
	UINT32                                 : 2;   // 11:12 reserved
	UINT32 cs_type                         : 3;   // 13:15
	UINT32 vsize                           : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 scan_type                       : 1;   //    31
} H14A0_P0L_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
	0x0418 p0l_pe0_field ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                        : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 field_mode                      : 2;   //  4: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 secam_line                      : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 secam_line_mode                 : 2;   // 12:13
} H14A0_P0L_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
	0x041c p0l_pe0_param_by_ddr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 9;   //  0: 8 reserved
	UINT32 stride                          : 2;   //  9:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 vsize                           : 8;   // 16:23
	UINT32                                 : 6;   // 24:29 reserved
	UINT32 endian                          : 1;   //    30
	UINT32 update_by_ddr                   : 1;   //    31
} H14A0_P0L_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
	0x0420 p0l_win_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y0                              : 12;  // 16:27
} H14A0_P0L_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x0424 p0l_win_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0L_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x0428 p0l_win_ctrl_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y0                              : 12;  // 16:27
	UINT32 ac_bnr_feature_cal_mode         : 2;   // 28:29
} H14A0_P0L_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
	0x042c p0l_win_ctrl_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0L_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
	0x0430 p0l_pe0_debug_mem_req ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_t1_y                      : 1;   //     0
	UINT32 en_rd_t1_c                      : 1;   //     1
	UINT32 en_rd_t2_y                      : 1;   //     2
	UINT32 en_rd_t2_c                      : 1;   //     3
	UINT32 en_rd_t3_y                      : 1;   //     4
	UINT32 en_rd_t3_c                      : 1;   //     5
	UINT32 en_rd_t4_y                      : 1;   //     6
	UINT32 en_rd_t1_m                      : 1;   //     7
	UINT32 en_wr_tnrw_y                    : 1;   //     8
	UINT32 en_wr_tnrw_c                    : 1;   //     9
	UINT32 en_wr_tnrw_m                    : 1;   //    10
	UINT32                                 : 4;   // 11:14 reserved
	UINT32 manual_mem_en                   : 1;   //    15
} H14A0_P0L_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
	0x0434 p0l_pe0_debug_lreq_dly ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                      : 11;  //  0:10
	UINT32 lreq_dly_c_en                   : 1;   //    11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 lreq_dly_y                      : 11;  // 16:26
	UINT32 lreq_dly_y_en                   : 1;   //    27
} H14A0_P0L_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
	0x0440 p0l_tnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_enable                      : 1;   //     0
	UINT32                                 : 1;   //     1 reserved
	UINT32 nt_lvl_hsub                     : 1;   //     2
	UINT32 secam_cmode_en                  : 1;   //     3
	UINT32 secam_alpha_scale               : 2;   //  4: 5
	UINT32 prev_sel_prog                   : 1;   //     6
	UINT32 frame_skip_enable_for_3d        : 1;   //     7
	UINT32 minfo_mode                      : 2;   //  8: 9
	UINT32 minfo_sw                        : 2;   // 10:11
	UINT32 hist_motion_scale               : 2;   // 12:13
	UINT32 minfo_flt_en_tnr                : 1;   //    14
	UINT32 minfo_flt_en_ipc                : 1;   //    15
	UINT32 calc_motion_max_yc              : 1;   //    16
	UINT32 motion_4f_en                    : 1;   //    17
	UINT32 mv_tflt_en                      : 1;   //    18
	UINT32 mv_sflt_en                      : 1;   //    19
	UINT32 alpha_avg_en                    : 1;   //    20
	UINT32                                 : 1;   //    21 reserved
	UINT32 alpha_mapping_y_en              : 1;   //    22
	UINT32 alpha_mapping_c_en              : 1;   //    23
	UINT32 nt_lvl_adjust_gm_enable         : 1;   //    24
	UINT32 nt_lvl_adjust_lpct_enable       : 1;   //    25
	UINT32                                 : 1;   //    26 reserved
	UINT32 nt_lvl_adjust_avg_ts_enable     : 1;   //    27
	UINT32 nt_lvl_adjust_lpct_sel          : 1;   //    28
	UINT32 mamc_blend_sel                  : 1;   //    29
} H14A0_P0L_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0444 p0l_tnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                         : 8;   //  0: 7
	UINT32 y_th_hi                         : 8;   //  8:15
	UINT32 dnr_motion_scale                : 2;   // 16:17
	UINT32                                 : 2;   // 18:19 reserved
	UINT32 tnr_filter_lsb_ctrl             : 2;   // 20:21
	UINT32 svar_mux_ctrl                   : 2;   // 22:23
	UINT32 nt_lvl_hist_n_pct               : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0448 p0l_tnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_flat_svar_th                 : 8;   //  0: 7
	UINT32 alpha_lut_ind7                  : 8;   //  8:15
	UINT32 secam_cmode_flt_en              : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 secam_cmode_flt_th              : 4;   // 20:23
	UINT32 m_gain_en_var                   : 1;   //    24
	UINT32 a_gain_en_var                   : 1;   //    25
	UINT32 m_gain_en_lsc                   : 1;   //    26
	UINT32 a_gain_en_lsc                   : 1;   //    27
	UINT32 m_gain_en_var2                  : 1;   //    28
	UINT32 m_offset_mode_en                : 1;   //    29
} H14A0_P0L_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x044c p0l_tnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                           : 8;   //  0: 7
	UINT32 wa_y1                           : 8;   //  8:15
	UINT32 wa_x0                           : 8;   // 16:23
	UINT32 wa_x1                           : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0450 p0l_tnr_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0               : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x0               : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y1               : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x1               : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0454 p0l_tnr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                          : 8;   //  0: 7
	UINT32 nt_max                          : 8;   //  8:15
	UINT32 hist_svar_th                    : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th0            : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0458 p0l_tnr_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                       : 8;   //  0: 7
	UINT32 nt_iir_alpha                    : 8;   //  8:15
	UINT32 y_th_lo2                        : 8;   // 16:23
	UINT32 y_th_hi2                        : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x045c p0l_tnr_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                  : 8;   //  0: 7
	UINT32 alpha_lut_ind1                  : 8;   //  8:15
	UINT32 alpha_lut_ind2                  : 8;   // 16:23
	UINT32 alpha_lut_ind3                  : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x0460 p0l_tnr_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                  : 8;   //  0: 7
	UINT32 alpha_lut_ind5                  : 8;   //  8:15
	UINT32 alpha_lut_ind6                  : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th1            : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x0464 p0l_tnr_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 scene_change_enable             : 1;   //     0
	UINT32 scene_change_max_flt_en         : 1;   //     1
	UINT32 scene_change_measure_sel        : 1;   //     2
	UINT32 scene_change_prev_hist_calc_en  : 1;   //     3
	UINT32 scene_change_enforce            : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 scene_change_max_flt_tap        : 2;   //  8: 9
	UINT32                                 : 2;   // 10:11 reserved
	UINT32 scene_change_scale              : 3;   // 12:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 scene_change_th                 : 8;   // 16:23
	UINT32 gmv_svar_th                     : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x0468 p0l_tnr_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_show_tnr_param              : 4;   //  0: 3
	UINT32 dbg_status_sum_sel              : 2;   //  4: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 dbg_force_wa                    : 1;   //     8
	UINT32 dbg_force_calc_motion           : 1;   //     9
	UINT32 dbg_force_lut_alpha             : 1;   //    10
	UINT32 dbg_force_adj_alpha             : 1;   //    11
	UINT32 dbg_force_final_alpha           : 1;   //    12
	UINT32                                 : 2;   // 13:14 reserved
	UINT32 ignore_active_window            : 1;   //    15
	UINT32 dbg_tnrw_port_ctrl              : 2;   // 16:17
	UINT32                                 : 2;   // 18:19 reserved
	UINT32 tnr_black_line_en_for_i         : 1;   //    20
	UINT32 tnr_debug_map_en_for_i          : 1;   //    21
	UINT32                                 : 2;   // 22:23 reserved
	UINT32 dbg_wa                          : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x046c p0l_tnr_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                 : 8;   //  0: 7
	UINT32 dbg_alpha                       : 8;   //  8:15
	UINT32 peak0_x_th                      : 4;   // 16:19
	UINT32                                 : 4;   // 20:23 reserved
	UINT32 scene_change_th_mv              : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x0470 p0l_tnr_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2               : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x2               : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y3               : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x3               : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x0474 p0l_tnr_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                    : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 force_nt_lvl_val                : 8;   //  8:15
	UINT32 adjust_nt_lvl                   : 1;   //    16
	UINT32                                 : 7;   // 17:23 reserved
	UINT32 adjust_nt_lvl_val               : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x0478 p0l_tnr_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y0                              : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 noise_measure_win_en            : 1;   //    31
} H14A0_P0L_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x047c p0l_tnr_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0L_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x0480 p0l_tnr_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_scale              : 3;   //  0: 2
	UINT32                                 : 1;   //     3 reserved
	UINT32 show_dbg_bar_en                 : 4;   //  4: 7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 show_dbg_bar0_mux               : 4;   // 16:19
	UINT32 show_dbg_bar1_mux               : 4;   // 20:23
	UINT32 show_dbg_bar2_mux               : 4;   // 24:27
	UINT32 show_dbg_bar3_mux               : 4;   // 28:31
} H14A0_P0L_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x0484 p0l_tnr_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y0                  : 8;   //  0: 7
	UINT32 m_tiir_ctrl_x0                  : 8;   //  8:15
	UINT32 m_tiir_ctrl_y1                  : 8;   // 16:23
	UINT32 m_tiir_ctrl_x1                  : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x0488 p0l_tnr_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_enable                : 1;   //     0
	UINT32 luma_gain_enable                : 1;   //     1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 input_for_luma                  : 2;   //  4: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 input_for_crgn_satr             : 1;   //     8
	UINT32                                 : 7;   //  9:15 reserved
	UINT32 y_gain                          : 8;   // 16:23
	UINT32 c_gain                          : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x048c p0l_tnr_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                  : 8;   //  0: 7
	UINT32 luma_gain_p1_x                  : 8;   //  8:15
	UINT32 luma_gain_p2_x                  : 8;   // 16:23
	UINT32 luma_gain_p3_x                  : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x0490 p0l_tnr_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                  : 8;   //  0: 7
	UINT32 luma_gain_p1_y                  : 8;   //  8:15
	UINT32 luma_gain_p2_y                  : 8;   // 16:23
	UINT32 luma_gain_p3_y                  : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x0494 p0l_tnr_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_mamc_blend                  : 8;   //  0: 7
} H14A0_P0L_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x0498 p0l_tnr_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_protect_control_y0           : 8;   //  0: 7
	UINT32 mv_protect_control_x0           : 8;   //  8:15
	UINT32 mv_protect_control_y1           : 8;   // 16:23
	UINT32 mv_protect_control_x1           : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x049c p0l_tnr_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0              : 8;   //  0: 7
	UINT32 alpha_mapping_y_x0              : 8;   //  8:15
	UINT32 alpha_mapping_y_y1              : 8;   // 16:23
	UINT32 alpha_mapping_y_x1              : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x04a0 p0l_tnr_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2              : 8;   //  0: 7
	UINT32 alpha_mapping_y_x2              : 8;   //  8:15
	UINT32 alpha_mapping_y_y3              : 8;   // 16:23
	UINT32 alpha_mapping_y_x3              : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x04a4 p0l_tnr_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0              : 8;   //  0: 7
	UINT32 alpha_mapping_c_x0              : 8;   //  8:15
	UINT32 alpha_mapping_c_y1              : 8;   // 16:23
	UINT32 alpha_mapping_c_x1              : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x04a8 p0l_tnr_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2              : 8;   //  0: 7
	UINT32 alpha_mapping_c_x2              : 8;   //  8:15
	UINT32 alpha_mapping_c_y3              : 8;   // 16:23
	UINT32 alpha_mapping_c_x3              : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x04ac p0l_tnr_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                  : 8;   //  0: 7
	UINT32 crgn_gain_rgn1                  : 8;   //  8:15
	UINT32 crgn_gain_rgn2                  : 8;   // 16:23
	UINT32 crgn_gain_rgn3                  : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x04b0 p0l_tnr_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0               : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x0               : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y1               : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x1               : 7;   // 24:30
} H14A0_P0L_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x04b4 p0l_tnr_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                           : 8;   //  0: 7
	UINT32 wa_y3                           : 8;   //  8:15
	UINT32 wa_x2                           : 8;   // 16:23
	UINT32 wa_x3                           : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x04b8 p0l_tnr_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2               : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x2               : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y3               : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x3               : 7;   // 24:30
} H14A0_P0L_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x04bc p0l_tnr_ctrl_31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c            : 1;   //     0
	UINT32 spatial_pre_flt_en_y            : 1;   //     1
	UINT32                                 : 14;  //  2:15 reserved
	UINT32 spatial_pre_flt_th_c            : 8;   // 16:23
	UINT32 spatial_pre_flt_th_y            : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
	0x04c0 p0l_tnr_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                         : 8;   //  0: 7
	UINT32 x_peak_t2                       : 8;   //  8:15
	UINT32 nt_lvl                          : 8;   // 16:23
	UINT32 scene_change                    : 8;   // 24:31
} H14A0_P0L_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x04c4 p0l_tnr_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_s                         : 8;   //  0: 7
	UINT32 valid_cnt_motion_histogram      : 16;  //  8:23
} H14A0_P0L_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x04c8 p0l_tnr_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_l                            : 4;   //  0: 3
	UINT32 mv_r                            : 4;   //  4: 7
	UINT32 dfd                             : 8;   //  8:15
	UINT32 gm                              : 8;   // 16:23
	UINT32 lp                              : 8;   // 24:31
} H14A0_P0L_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x04cc p0l_tnr_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_valid_cnt                   : 16;  //  0:15
	UINT32 fd                              : 8;   // 16:23
	UINT32 gmv_lock_cnt                    : 6;   // 24:29
} H14A0_P0L_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x04d0 p0l_tnr_main_lut_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr             : 5;   //  0: 4
	UINT32                                 : 7;   //  5:11 reserved
	UINT32 main_lut_ai_enable              : 1;   //    12
} H14A0_P0L_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
	0x04d4 p0l_tnr_main_lut_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0            : 32;  //  0:31
} H14A0_P0L_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
	0x04d8 p0l_tnr_main_lut_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1            : 32;  //  0:31
} H14A0_P0L_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
	0x04e0 p0l_tnr_ctrl_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0             : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x0             : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y1             : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x1             : 7;   // 24:30
} H14A0_P0L_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
	0x04e4 p0l_tnr_ctrl_33 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2             : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x2             : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y3             : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x3             : 7;   // 24:30
} H14A0_P0L_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
	0x04e8 p0l_tnr_ctrl_34 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_ctrl_y0                 : 8;   //  0: 7
	UINT32 mc_blur_ctrl_x0                 : 8;   //  8:15
	UINT32 mc_blur_ctrl_y1                 : 8;   // 16:23
	UINT32 mc_blur_ctrl_x1                 : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
	0x04ec p0l_tnr_ctrl_35 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y2                  : 8;   //  0: 7
	UINT32 m_tiir_ctrl_x2                  : 8;   //  8:15
	UINT32 m_tiir_ctrl_y3                  : 8;   // 16:23
	UINT32 m_tiir_ctrl_x3                  : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
	0x04f0 p0l_tnr_crg_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr            : 8;   //  0: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 crgn_ctrl_ai_enable             : 1;   //    12
} H14A0_P0L_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x04f4 p0l_tnr_crg_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data            : 32;  //  0:31
} H14A0_P0L_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x04f8 p0l_tnr_ctrl_36 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_sel_hist                    : 1;   //     0
	UINT32 sad_sel_pels                    : 1;   //     1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 hme_half_pel_en                 : 1;   //     4
	UINT32 m_tiir_x_sel                    : 1;   //     5
	UINT32 m_tiir_scene_change_en          : 1;   //     6
	UINT32 m_tiir_en                       : 1;   //     7
	UINT32 sf_blend_en_y                   : 1;   //     8
	UINT32 sf_blend_en_c                   : 1;   //     9
	UINT32                                 : 2;   // 10:11 reserved
	UINT32 sf_blend_motion_exp_mode        : 2;   // 12:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 mv0_protect_th                  : 7;   // 16:22
	UINT32 mv0_protect_en                  : 1;   //    23
	UINT32 mv_4f_en                        : 1;   //    24
	UINT32 mv_pfilter_en                   : 1;   //    25
	UINT32 mv_cost_smooth_gain             : 5;   // 26:30
	UINT32 mv_cost_smooth_en               : 1;   //    31
} H14A0_P0L_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
	0x04fc p0l_tnr_ctrl_37 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_flt_en_c                     : 1;   //     0
	UINT32 mc_flt_mv_inv_c                 : 1;   //     1
	UINT32 mc_flt_mv_debug_en_c            : 1;   //     2
	UINT32 mc_flt_manual_blend_en_c        : 1;   //     3
	UINT32 mc_flt_mv_debug_c               : 4;   //  4: 7
	UINT32 mc_flt_manual_blend_c           : 6;   //  8:13
	UINT32                                 : 1;   //    14 reserved
	UINT32 mc_flt_blur_en_c                : 1;   //    15
	UINT32 mc_flt_en_y                     : 1;   //    16
	UINT32 mc_flt_mv_inv_y                 : 1;   //    17
	UINT32 mc_flt_mv_debug_en_y            : 1;   //    18
	UINT32 mc_flt_manual_blend_en_y        : 1;   //    19
	UINT32 mc_flt_mv_debug_y               : 4;   // 20:23
	UINT32 mc_flt_manual_blend_y           : 6;   // 24:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 mc_flt_blur_en_y                : 1;   //    31
} H14A0_P0L_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
	0x0500 p0l_ipc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                    : 2;   //  0: 1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 cr_chroma_mode                  : 2;   //  4: 5
	UINT32                                 : 1;   //     6 reserved
	UINT32 cr_chroma_edi                   : 1;   //     7
	UINT32 cr_integer_edi                  : 1;   //     8
	UINT32                                 : 15;  //  9:23 reserved
	UINT32 cr_hmc_force_fmd                : 1;   //    24
	UINT32                                 : 1;   //    25 reserved
	UINT32 cr_clc_fmd_off                  : 1;   //    26
	UINT32 cr_st_flt_original_en           : 1;   //    27
	UINT32 cr_film_auto_gain               : 1;   //    28
	UINT32 cr_film_apply_c                 : 1;   //    29
	UINT32 cr_film_apply_y                 : 1;   //    30
	UINT32 cr_film_mode_enable             : 1;   //    31
} H14A0_P0L_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0504 p0l_ipc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 cr_st_flt_enable                : 1;   //    16
	UINT32 cr_st_2d_v_flt_en               : 1;   //    17
	UINT32 cr_hmc_flt_enable               : 1;   //    18
	UINT32 cr_game_mode                    : 1;   //    19
	UINT32 cr_st_2d_v_flt_range            : 4;   // 20:23
	UINT32                                 : 1;   //    24 reserved
	UINT32 cr_black_mask                   : 1;   //    25
	UINT32 cr_ipc_debug_show               : 5;   // 26:30
	UINT32 cr_5f_mode                      : 1;   //    31
} H14A0_P0L_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0508 p0l_ipc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0             : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_x4             : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x1             : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y1             : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x050c p0l_ipc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2             : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_y2             : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x3             : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y3             : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0510 p0l_ipc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset_s7                    : 8;   //  0: 7
	UINT32 motion_x_tearing_gain           : 8;   //  8:15
	UINT32 tearing_gain                    : 8;   // 16:23
	UINT32 motion_gain                     : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0514 p0l_ipc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha           : 8;   //  0: 7
	UINT32 cr_t_grad_weight                : 8;   //  8:15
	UINT32 cr_v_grad_weight                : 8;   // 16:23
	UINT32 cr_h_grad_weight                : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0518 p0l_ipc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                 : 1;   //     0
	UINT32 cr_flat_only                    : 1;   //     1
	UINT32 use_tnr_motion                  : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 cr_tearing_normalize_en         : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 cr_normalize_base               : 8;   //  8:15
	UINT32 cr_normalize_center             : 8;   // 16:23
	UINT32 cr_motion_coring                : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x051c p0l_ipc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 1;   //     0 reserved
	UINT32 cr_v_en                         : 1;   //     1
	UINT32 cr_45_en                        : 1;   //     2
	UINT32 cr_15_en                        : 1;   //     3
	UINT32                                 : 20;  //  4:23 reserved
	UINT32 tnr_motion_gain                 : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x0520 p0l_ipc_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top           : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 fmd_caption_range_bot           : 12;  // 16:27
	UINT32                                 : 2;   // 28:29 reserved
	UINT32 caption_flow_detected           : 1;   //    30
	UINT32 hmc_when_cap_not_detected       : 1;   //    31
} H14A0_P0L_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x0524 p0l_ipc_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                     : 8;   //  0: 7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 pn_variance_th                  : 8;   // 16:23
	UINT32 pn_fmd_motion_th                : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x0528 p0l_ipc_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                   : 8;   //  0: 7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 pcnc_variance_th                : 8;   // 16:23
	UINT32 pcnc_fmd_motion_th              : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x052c p0l_ipc_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                    : 8;   //  0: 7
	UINT32 cr_center_bound_ctrl            : 8;   //  8:15
	UINT32 cr_dn_bound_ctrl                : 8;   // 16:23
	UINT32 cr_up_bound_ctrl                : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x0530 p0l_ipc_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 still_pn_th                     : 8;   //  0: 7
	UINT32 fmd_ratio_th                    : 8;   //  8:15
	UINT32 fmd_max_th                      : 8;   // 16:23
	UINT32 fmd_min_th                      : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x0534 p0l_ipc_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view               : 13;  //  0:12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 cr_hmc_debug_mode               : 1;   //    16
	UINT32                                 : 2;   // 17:18 reserved
	UINT32 cr_hmc_debug_dir                : 5;   // 19:23
	UINT32 cr_hmc_debug_gain               : 6;   // 24:29
} H14A0_P0L_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x0538 p0l_ipc_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 lockcnt_en                      : 1;   //     0
	UINT32 manual_en                       : 1;   //     1
	UINT32 manual_mode                     : 2;   //  2: 3
	UINT32 lock_speed                      : 4;   //  4: 7
	UINT32 lockcnt_th                      : 8;   //  8:15
	UINT32 same_th                         : 8;   // 16:23
	UINT32 still_frame_limit               : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x053c p0l_ipc_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 protection_en                   : 1;   //     0
	UINT32 protection_s1en                 : 1;   //     1
	UINT32 protection_s2en                 : 1;   //     2
	UINT32 protection_s3en                 : 1;   //     3
	UINT32 protection_s4en                 : 1;   //     4
	UINT32 badedit_protect                 : 1;   //     5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 modify_ratio_th                 : 8;   //  8:15
	UINT32 fmd_tower_spangain              : 4;   // 16:19
	UINT32 fmd_bleacherratio               : 4;   // 20:23
	UINT32 fmd_vlrratiomin                 : 4;   // 24:27
	UINT32 fmd_spxl2ratio                  : 4;   // 28:31
} H14A0_P0L_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x0540 p0l_ipc_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 still_pn_th2                    : 8;   //  0: 7
	UINT32 still_prev_ratio                : 8;   //  8:15
	UINT32 lock_reducer                    : 8;   // 16:23
	UINT32 same_min_ratio                  : 4;   // 24:27
	UINT32 same_adder_gain                 : 4;   // 28:31
} H14A0_P0L_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x0544 p0l_ipc_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution               : 2;   //  0: 1
	UINT32 cr_field_manual_mode            : 2;   //  2: 3
	UINT32 cr_field_inverse                : 1;   //     4
	UINT32                                 : 19;  //  5:23 reserved
	UINT32 cr_tearing_a_th                 : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x0548 p0l_ipc_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                      : 8;   //  0: 7
	UINT32 cr_lr_gain                      : 8;   //  8:15
	UINT32 cr_h_grad_gain                  : 8;   // 16:23
	UINT32 cr_v_grad_gain                  : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x054c p0l_ipc_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 errs_gain                       : 8;   //  0: 7
	UINT32 confidance_gain                 : 8;   //  8:15
	UINT32 pe0_2d_offset                   : 8;   // 16:23
	UINT32 flicker_gain                    : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x0550 p0l_ipc_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                    : 16;  //  0:15
	UINT32 r_nc_v_count                    : 16;  // 16:31
} H14A0_P0L_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x0554 p0l_ipc_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                    : 16;  //  0:15
	UINT32 r_pn_v_count                    : 16;  // 16:31
} H14A0_P0L_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x0558 p0l_ipc_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_same                    : 16;  //  0:15
	UINT32 r_pc_pn_same                    : 16;  // 16:31
} H14A0_P0L_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x055c p0l_ipc_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pn_lr_count                   : 16;  //  0:15
	UINT32 same_ng_flag                    : 1;   //    16
	UINT32 s3_protect_en_flag              : 1;   //    17
	UINT32 s2_protect_en_flag              : 1;   //    18
	UINT32 s1_protect_en_flag              : 1;   //    19
	UINT32 modify_pn_ratio_flag            : 1;   //    20
	UINT32 base_pn_ratio_flag              : 1;   //    21
	UINT32 r_fmd_comb_mode                 : 2;   // 22:23
	UINT32 r_fmd_lock_count                : 8;   // 24:31
} H14A0_P0L_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x0560 clc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 1;   //     0 reserved
	UINT32 cl_filter_enable                : 1;   //     1
	UINT32 clc_detection_enable            : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 motion_window                   : 2;   //  4: 5
	UINT32 cl_pattern_dist                 : 2;   //  6: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 cl_ab_ratio                     : 4;   // 12:15
	UINT32 cl_y_hdistance                  : 2;   // 16:17
	UINT32 cl_c_hdistance                  : 2;   // 18:19
	UINT32 cl_y_protect_en                 : 1;   //    20
	UINT32                                 : 7;   // 21:27 reserved
	UINT32 cl_motion_a_filter              : 1;   //    28
	UINT32 cl_motion_b_filter              : 1;   //    29
} H14A0_CLC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0564 clc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_motion_a_th                  : 8;   //  0: 7
	UINT32 cl_motion_b_th                  : 8;   //  8:15
	UINT32 cl_y_protect_th0                : 8;   // 16:23
	UINT32 cl_y_protect_th1                : 8;   // 24:31
} H14A0_CLC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0568 clc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_y_bound_th                   : 7;   //  0: 6
	UINT32 cl_c_bound_use_4line            : 1;   //     7
	UINT32 cl_c_bound_th                   : 7;   //  8:14
	UINT32 cl_c_bound_type                 : 1;   //    15
	UINT32 cl_y_bound_width_chroma         : 2;   // 16:17
	UINT32 cl_y_bound_ignore_cc            : 1;   //    18
	UINT32 cl_y_bound_ignore_p2            : 1;   //    19
	UINT32 cl_y_bound_ignore_p4            : 1;   //    20
	UINT32 bin3x1_flt_en                   : 1;   //    21
	UINT32                                 : 2;   // 22:23 reserved
	UINT32 bin5x1_flt_en                   : 1;   //    24
	UINT32 bin5x1_flt_th                   : 3;   // 25:27
	UINT32 vm_original_mode                : 1;   //    28
	UINT32 vm_other_mode                   : 1;   //    29
} H14A0_CLC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x056c clc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_sat_min                      : 8;   //  0: 7
	UINT32                                 : 2;   //  8: 9 reserved
	UINT32 cl_var_measure_th1              : 10;  // 10:19
	UINT32 cl_var_measure_th2              : 10;  // 20:29
	UINT32 cl_pt_enable                    : 2;   // 30:31
} H14A0_CLC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0570 clc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_tearing_pt_th1               : 12;  //  0:11
	UINT32                                 : 8;   // 12:19 reserved
	UINT32 cl_frame_min                    : 12;  // 20:31
} H14A0_CLC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0574 clc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_data_line_switching_sel      : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 clc_bin_flt_th                  : 3;   //  8:10
	UINT32                                 : 4;   // 11:14 reserved
	UINT32 clc_bin_flt_en                  : 1;   //    15
	UINT32 clc_dbg_show_mask               : 13;  // 16:28
} H14A0_CLC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0578 clc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_c_filter_gain                : 8;   //  0: 7
	UINT32 cc_y_filter_when_cc_detected    : 1;   //     8
	UINT32                                 : 1;   //     9 reserved
	UINT32 cl_cc_original_en               : 1;   //    10
	UINT32 clc_force_non_fmd               : 1;   //    11
	UINT32 cl_c_filter_when_cl_detected    : 1;   //    12
	UINT32 cl_y_filter_when_cl_detected    : 1;   //    13
	UINT32 clc_filter_mode                 : 2;   // 14:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 cl_cc_protection_th             : 8;   // 24:31
} H14A0_CLC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x057c clc_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 cl_detection_count              : 16;  // 16:31
} H14A0_CLC_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x0580 vflt_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vfilterenable                   : 1;   //     0
	UINT32 vfilterlocalenable              : 1;   //     1
	UINT32 vfilter_strength                : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 cb_filter_y                     : 2;   //  4: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 vfilter_a_th                    : 8;   //  8:15
	UINT32 vfilter_in_field_var            : 8;   // 16:23
	UINT32 vfilter_use_mc_mask             : 1;   //    24
	UINT32 vfilter_expand                  : 1;   //    25
	UINT32 vfilter_center                  : 1;   //    26
	UINT32 vfilter_ul4                     : 1;   //    27
	UINT32 dbgshow_level                   : 2;   // 28:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 dbgshow_enable                  : 1;   //    31
} H14A0_VFLT_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0584 p0l_cti_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                  : 8;   //  0: 7
	UINT32 cti_coring_th1                  : 8;   //  8:15
	UINT32 cti_coring_smooth               : 3;   // 16:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 cti_coring_map_size             : 2;   // 20:21
} H14A0_P0L_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x0588 p0l_mnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_enable                      : 1;   //     0
	UINT32 mnr_debug                       : 2;   //  1: 2
	UINT32 mnr_motion_view                 : 1;   //     3
	UINT32 h_expend                        : 1;   //     4
	UINT32 gain_flt_size                   : 1;   //     5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 mnr_master_gain                 : 8;   //  8:15
	UINT32 chroma_master_gain              : 8;   // 16:23
} H14A0_P0L_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x058c p0l_mnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_s1_mmd_min                  : 8;   //  0: 7
	UINT32 mnr_s2_ratio_min                : 8;   //  8:15
	UINT32 mnr_s2_ratio_max                : 8;   // 16:23
	UINT32 mnr_s2_mmd_min                  : 8;   // 24:31
} H14A0_P0L_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0590 p0l_mnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 mnr_h_gain                      : 8;   // 16:23
	UINT32 mnr_v_gain                      : 8;   // 24:31
} H14A0_P0L_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x0594 p0l_mnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 filter_x0                       : 8;   //  0: 7
	UINT32 filter_x1                       : 8;   //  8:15
	UINT32 filter_y0                       : 8;   // 16:23
	UINT32 filter_y1                       : 8;   // 24:31
} H14A0_P0L_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0598 p0l_mnr_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 motion_mnr_en                   : 1;   //     0
	UINT32 motion_mnr_filter               : 1;   //     1
	UINT32                                 : 14;  //  2:15 reserved
	UINT32 mnr_motion_min                  : 8;   // 16:23
	UINT32 mnr_motion_max                  : 8;   // 24:31
} H14A0_P0L_MNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x059c p0l_mnr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 motion_mnr_x0                   : 8;   //  0: 7
	UINT32 motion_mnr_x1                   : 8;   //  8:15
	UINT32 motion_mnr_y0                   : 8;   // 16:23
	UINT32 motion_mnr_y1                   : 8;   // 24:31
} H14A0_P0L_MNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x05a0 p0l_bnr_detail_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_detail_max               : 8;   //  0: 7
	UINT32 bnr_ac_detail_min               : 8;   //  8:15
	UINT32 bnr_diff_l                      : 8;   // 16:23
	UINT32 bnr_diff_p                      : 8;   // 24:31
} H14A0_P0L_BNR_DETAIL_CTRL_T;

/*-----------------------------------------------------------------------------
	0x05a4 p0l_vfilter_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable           : 1;   //     0
	UINT32                                 : 15;  //  1:15 reserved
	UINT32 cr_hmc_protect_th_max           : 8;   // 16:23
	UINT32 cr_hmc_protect_th_min           : 8;   // 24:31
} H14A0_P0L_VFILTER_02_T;

/*-----------------------------------------------------------------------------
	0x05a8 p0l_vfilter_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en            : 1;   //     0
	UINT32 cr_vfilter_force_en             : 1;   //     1
	UINT32 cr_global_frame_num_th          : 4;   //  2: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 cr_vfilter_global_th            : 12;  //  8:19
	UINT32 tearing_count                   : 12;  // 20:31
} H14A0_P0L_VFILTER_01_T;

/*-----------------------------------------------------------------------------
	0x05ac p0l_bnr_dc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_enable                   : 1;   //     0
	UINT32 dc_blur_debug_mode              : 1;   //     1
	UINT32 dc_blur_sel                     : 2;   //  2: 3
	UINT32 dc_output_debug_mode            : 4;   //  4: 7
	UINT32 bnr_ac_manual_init_h            : 4;   //  8:11
	UINT32 bnr_ac_manual_init_v            : 4;   // 12:15
	UINT32 dc_motion_max                   : 8;   // 16:23
	UINT32 dc_motion_min                   : 8;   // 24:31
} H14A0_P0L_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x05b0 p0l_bnr_dc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_detail_max                   : 8;   //  0: 7
	UINT32 dc_detail_min                   : 8;   //  8:15
	UINT32 var_chroma_en                   : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 var_v_gain                      : 4;   // 20:23
	UINT32 var_h_gain                      : 4;   // 24:27
	UINT32 var_cut_resolution              : 4;   // 28:31
} H14A0_P0L_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x05b4 p0l_bnr_ac_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_pos_gain_l2              : 8;   //  0: 7
	UINT32 bnr_ac_pos_gain_l1              : 8;   //  8:15
	UINT32 bnr_ac_pos_gain_l0              : 8;   // 16:23
	UINT32 bnr_ac_pos_gain_h3              : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x05b8 p0l_bnr_ac_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_filter_coef5_2           : 4;   //  0: 3
	UINT32 bnr_ac_filter_coef5_1           : 4;   //  4: 7
	UINT32 bnr_ac_filter_coef5_0           : 4;   //  8:11
	UINT32 bnr_ac_filter_coef3_2           : 4;   // 12:15
	UINT32 bnr_ac_filter_coef3_1           : 4;   // 16:19
	UINT32 bnr_ac_filter_coef3_0           : 4;   // 20:23
	UINT32 bnr_ac_pos_gain_l3              : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x05bc p0l_bnr_ac_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_pos_gain_h2              : 8;   //  0: 7
	UINT32 bnr_ac_pos_gain_h1              : 8;   //  8:15
	UINT32 bnr_ac_pos_gain_h0              : 8;   // 16:23
	UINT32 bnr_ac_detail_gain_th           : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x05c0 p0l_bnr_ac_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_h_en                     : 1;   //     0
	UINT32 bnr_ac_v_en                     : 1;   //     1
	UINT32 bnr_ac_h_chroma_en              : 1;   //     2
	UINT32 bnr_ac_v_chroma_en              : 1;   //     3
	UINT32 bnr_ac_acness_resol_h           : 2;   //  4: 5
	UINT32 bnr_ac_sdhd_sel                 : 1;   //     6
	UINT32                                 : 1;   //     7 reserved
	UINT32 bnr_ac_diff_min_v_th            : 8;   //  8:15
	UINT32 bnr_ac_diff_min_h_th            : 8;   // 16:23
	UINT32 bnr_ac_alpha_gain               : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x05c4 p0l_bnr_ac_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_global_motion_th         : 8;   //  0: 7
	UINT32 bnr_ac_acness_max               : 8;   //  8:15
	UINT32 bnr_ac_acness_min               : 8;   // 16:23
	UINT32 bnr_ac_bin_th                   : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x05c8 p0l_bnr_ac_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_motion_max_offset_h2     : 8;   //  0: 7
	UINT32 bnr_ac_motion_min_offset_h2     : 8;   //  8:15
	UINT32 bnr_ac_motion_max_offset_h1     : 8;   // 16:23
	UINT32 bnr_ac_motion_min_offset_h1     : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x05cc p0l_bnr_ac_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_motion_acness_th1        : 8;   //  0: 7
	UINT32 bnr_ac_motion_acness_th0        : 8;   //  8:15
	UINT32 bnr_ac_motion_max_offset_h3     : 8;   // 16:23
	UINT32 bnr_ac_motion_min_offset_h3     : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x05d0 p0l_bnr_ac_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_v_init_offset            : 3;   //  0: 2
	UINT32 bnr_ac_h_init_offset            : 3;   //  3: 5
	UINT32 bnr_ac_acness_resol_v           : 2;   //  6: 7
	UINT32 bnr_ac_debug_mode               : 4;   //  8:11
	UINT32 bnr_ac_hsy_mode                 : 4;   // 12:15
	UINT32 bnr_ac_motion_max               : 8;   // 16:23
	UINT32 bnr_ac_motion_min               : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x05d4 p0l_bnr_ac_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_detail_th4               : 8;   //  0: 7
	UINT32 bnr_ac_detail_th3               : 8;   //  8:15
	UINT32 bnr_ac_detail_th2               : 8;   // 16:23
	UINT32 bnr_ac_detail_th1               : 8;   // 24:31
} H14A0_P0L_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x05d8 p0l_dnr_max_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_ac_bnr                   : 1;   //     0
	UINT32 enable_mnr                      : 1;   //     1
	UINT32 enable_dc_bnr                   : 1;   //     2
	UINT32                                 : 5;   //  3: 7 reserved
	UINT32 debug_enable                    : 1;   //     8
	UINT32 debug_mode                      : 1;   //     9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 win_control_enable              : 1;   //    16
	UINT32 border_enable                   : 1;   //    17
	UINT32 win_inout                       : 1;   //    18
	UINT32 motion_blur_sel                 : 1;   //    19
	UINT32 motion_bit_ctrl                 : 3;   // 20:22
} H14A0_P0L_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
	0x05dc p0l_dnr_dbar_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                    : 1;   //     0
	UINT32                                 : 1;   //     1 reserved
	UINT32 show_debug_bar_for_bnr          : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 gain                            : 3;   //  4: 6
	UINT32                                 : 1;   //     7 reserved
	UINT32 sd_hd_sel                       : 2;   //  8: 9
} H14A0_P0L_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
	0x05e0 p0l_bnr_stat_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                 : 8;   //  0: 7
	UINT32 ac_bnr_v_status                 : 8;   //  8:15
	UINT32 dc_bnr_th                       : 14;  // 16:29
} H14A0_P0L_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
	0x05e4 p0l_cti_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                          : 1;   //     0
	UINT32 cti_clipping_mode               : 1;   //     1
	UINT32 cti_pre_flt_mode                : 2;   //  2: 3
	UINT32 cti_tap                         : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 cti_gain                        : 8;   //  8:15
	UINT32 cti_debug_mode                  : 3;   // 16:18
} H14A0_P0L_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x05e8 c420_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c420_ctrl_f                     : 32;   // 31: 0
} H14A0_C420_CTRL_T;

/*-----------------------------------------------------------------------------
	0x05f0 p0l_ipc_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cubic_coef0                     : 8;   //  0: 7
	UINT32 cubic_coef1                     : 8;   //  8:15
	UINT32 va_blending_ctrl_lv1            : 3;   // 16:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 va_blending_ctrl_lv2            : 3;   // 20:22
	UINT32                                 : 1;   //    23 reserved
	UINT32 va_blending_ctrl_lv3            : 3;   // 24:26
	UINT32                                 : 1;   //    27 reserved
	UINT32 va_blending_ctrl_lv4            : 3;   // 28:30
} H14A0_P0L_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x05f4 p0l_ipc_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 va_cubic                        : 1;   //     0
	UINT32 boundary_window                 : 1;   //     1
	UINT32 force_va                        : 1;   //     2
	UINT32 half_pel_va                     : 1;   //     3
	UINT32 dir_med_tap                     : 2;   //  4: 5
	UINT32 diff_med_tap                    : 2;   //  6: 7
	UINT32 va_blending_ctrl1               : 3;   //  8:10
	UINT32 wide_angle_protection_1         : 1;   //    11
	UINT32 va_blending_ctrl2               : 3;   // 12:14
	UINT32 wide_angle_protection_2         : 1;   //    15
	UINT32 errs_chroma_blend_coef          : 8;   // 16:23
	UINT32 strong_global_th                : 6;   // 24:29
	UINT32 narrow_angle_week               : 1;   //    30
	UINT32 narrow_angle_protection         : 1;   //    31
} H14A0_P0L_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x05f8 p0l_ipc_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain            : 8;   //  0: 7
	UINT32 cr_flicker_amp_gain             : 8;   //  8:15
} H14A0_P0L_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x05fc p0l_ipc_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                   : 1;   //     0
	UINT32 cr_lock_th                      : 7;   //  1: 7
	UINT32 cr_g_motion_th                  : 8;   //  8:15
	UINT32 cr_hmc_expend                   : 2;   // 16:17
	UINT32 cr_hmc_half_en                  : 1;   //    18
	UINT32                                 : 1;   //    19 reserved
	UINT32 cr_robustness                   : 4;   // 20:23
	UINT32 cr_center_weight                : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x0600 p0l_ipc_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                     : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 cr_noise_th                     : 8;   //  8:15
	UINT32 cr_gds_resolution               : 3;   // 16:18
} H14A0_P0L_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x0604 p0l_ipc_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vdiff_en                        : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 vdiff_th                        : 8;   //  8:15
	UINT32 hmc_err_max                     : 8;   // 16:23
	UINT32 hmc_err_min                     : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x0608 p0l_ipc_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                 : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 cr_fmd_ath                      : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 cr_fmd_ratio_th                 : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x060c p0l_ipc_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0           : 8;   //  0: 7
	UINT32 chroma_st_flt_ctrl_x1           : 8;   //  8:15
	UINT32 luma_st_flt_ctrl_x0             : 8;   // 16:23
	UINT32 luma_st_flt_ctrl_x1             : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x0610 p0l_ipc_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0l_ipc_ctrl_28_f               : 32;   // 31: 0
} H14A0_P0L_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x0614 p0l_ipc_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 hmc_flt_ctrl_x0                 : 8;   // 16:23
	UINT32 hmc_st_flt_ctrl_x1              : 8;   // 24:31
} H14A0_P0L_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x0618 p0l_ipc_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vmc_motion_th                   : 8;   //  0: 7
	UINT32 vmc_debug                       : 1;   //     8
	UINT32 vmc_en                          : 1;   //     9
	UINT32 vmc_still_check                 : 1;   //    10
	UINT32 vmc_mul                         : 3;   // 11:13
	UINT32 vmc_out_mul                     : 2;   // 14:15
	UINT32 vmc_protect_th                  : 8;   // 16:23
	UINT32 protection_en                   : 1;   //    24
} H14A0_P0L_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x061c p0l_ipc_ctrl_31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 toosmall_en                     : 1;   //     0
	UINT32 toosmall_min                    : 7;   //  1: 7
	UINT32 toosmall_max                    : 8;   //  8:15
	UINT32 pndiff_disable                  : 1;   //    16
} H14A0_P0L_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
	0x0628 p0l_ipc_status_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_lr_count                   : 16;  //  0:15
	UINT32 r_pc_lr_count                   : 16;  // 16:31
} H14A0_P0L_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
	0x062c p0l_ipc_status_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_lr_same                 : 16;  //  0:15
	UINT32 r_pndiff_count                  : 16;  // 16:31
} H14A0_P0L_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
	0x0630 p0l_ipc_status_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_lr_same                 : 16;  //  0:15
	UINT32 r_pc_pn_lr_same                 : 16;  // 16:31
} H14A0_P0L_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
	0x0634 p0l_ipc_status_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt              : 16;  //  0:15
	UINT32 r_inv_original_cnt              : 16;  // 16:31
} H14A0_P0L_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
	0x0638 p0l_ipc_status_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0l_ipc_status_08_f             : 32;   // 31: 0
} H14A0_P0L_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
	0x063c p0l_ipc_status_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0l_ipc_status_09_f             : 32;   // 31: 0
} H14A0_P0L_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
	0x0640 p0l_tpd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_scale                      : 3;   //  0: 2
	UINT32 bbd_show                        : 1;   //     3
	UINT32 show_bar_1                      : 1;   //     4
	UINT32 show_bar_2                      : 1;   //     5
	UINT32 show_bar_3                      : 1;   //     6
	UINT32 bar1_data_mux                   : 2;   //  7: 8
	UINT32 bar2_data_mux                   : 2;   //  9:10
	UINT32 bar3_data_mux                   : 2;   // 11:12
	UINT32 hist0_data_mux                  : 4;   // 13:16
	UINT32 hist1_data_mux                  : 4;   // 17:20
	UINT32 hist2_data_mux                  : 4;   // 21:24
	UINT32 hist0_win_sel                   : 2;   // 25:26
	UINT32 hist1_win_sel                   : 2;   // 27:28
	UINT32 hist2_win_sel                   : 2;   // 29:30
	UINT32 hist_rd_en                      : 1;   //    31
} H14A0_P0L_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0644 p0l_tpd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                         : 12;  //  0:11
	UINT32                                 : 3;   // 12:14 reserved
	UINT32 wina_hen                        : 1;   //    15
	UINT32 wina_y0                         : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 wina_ven                        : 1;   //    31
} H14A0_P0L_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0648 p0l_tpd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                         : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 wina_y1                         : 12;  // 16:27
	UINT32                                 : 2;   // 28:29 reserved
	UINT32 hist_hsv_hsl_sel                : 1;   //    30
	UINT32 hist_vscaling_en                : 1;   //    31
} H14A0_P0L_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x064c p0l_tpd_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                         : 12;  //  0:11
	UINT32                                 : 3;   // 12:14 reserved
	UINT32 winb_hen                        : 1;   //    15
	UINT32 winb_y0                         : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 winb_ven                        : 1;   //    31
} H14A0_P0L_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0650 p0l_tpd_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                         : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 winb_y1                         : 12;  // 16:27
} H14A0_P0L_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x0654 p0l_tpd_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x0                         : 12;  //  0:11
	UINT32                                 : 3;   // 12:14 reserved
	UINT32 winc_hen                        : 1;   //    15
	UINT32 winc_y0                         : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 winc_ven                        : 1;   //    31
} H14A0_P0L_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x0658 p0l_tpd_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x1                         : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 winc_y1                         : 12;  // 16:27
} H14A0_P0L_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x065c p0l_tpd_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_y                         : 10;  //  0: 9
	UINT32 force_y_max_th                  : 10;  // 10:19
	UINT32 force_y_min_th                  : 10;  // 20:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 force_y_enable                  : 1;   //    31
} H14A0_P0L_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x0660 p0l_tpd_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_cb                        : 10;  //  0: 9
	UINT32 force_cb_max_th                 : 10;  // 10:19
	UINT32 force_cb_min_th                 : 10;  // 20:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 force_cb_enable                 : 1;   //    31
} H14A0_P0L_TPD_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x0664 p0l_tpd_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_cr                        : 10;  //  0: 9
	UINT32 force_cr_max_th                 : 10;  // 10:19
	UINT32 force_cr_min_th                 : 10;  // 20:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 force_cr_enable                 : 1;   //    31
} H14A0_P0L_TPD_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x0668 p0l_tpd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                : 22;  //  0:21
} H14A0_P0L_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x066c clc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_lcount_enable                : 1;   //     0
	UINT32 cl_lcnt_mode                    : 2;   //  1: 2
	UINT32 cl_scene_change_enable          : 1;   //     3
	UINT32 cl_scene_change_mode            : 1;   //     4
	UINT32 cl_scene_change_manual          : 1;   //     5
	UINT32 cl_lcnt_th                      : 4;   //  6: 9
	UINT32 cl_plus_lcount                  : 3;   // 10:12
	UINT32 cl_minus_lcount                 : 2;   // 13:14
	UINT32 cl_scene_change_protect         : 1;   //    15
	UINT32                                 : 4;   // 16:19 reserved
	UINT32 cl_tearing_pt_th2               : 12;  // 20:31
} H14A0_CLC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x0670 p0l_bbd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_en                          : 1;   //     0
	UINT32 hys_mode                        : 5;   //  1: 5
	UINT32 op_mode                         : 1;   //     6
	UINT32                                 : 1;   //     7 reserved
	UINT32 cnt_th                          : 8;   //  8:15
	UINT32 diff_th                         : 8;   // 16:23
	UINT32 bbd_mux                         : 3;   // 24:26
	UINT32 apl_mux                         : 3;   // 27:29
	UINT32 apl_win_sel                     : 2;   // 30:31
} H14A0_P0L_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x0674 p0l_bbd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y0                              : 12;  // 16:27
} H14A0_P0L_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x0678 p0l_bbd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0L_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x067c p0l_bbd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y0                              : 12;  // 16:27
} H14A0_P0L_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x0680 p0l_bbd_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0L_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x0684 p0l_apl_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                       : 10;  //  0: 9
	UINT32 cb_b_s_apl                      : 10;  // 10:19
	UINT32 cr_r_s_apl                      : 10;  // 20:29
} H14A0_P0L_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x0688 p0l_bnr_dc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_var_en                       : 1;   //     0
	UINT32 dc_motion_en                    : 1;   //     1
	UINT32 dc_protection_en                : 1;   //     2
	UINT32 dc_detail_en                    : 1;   //     3
	UINT32 dc_global_motion_th             : 4;   //  4: 7
	UINT32 dc_bnr_mastergain               : 8;   //  8:15
	UINT32 dc_bnr_chromagain               : 8;   // 16:23
	UINT32 dc_protection_th                : 8;   // 24:31
} H14A0_P0L_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x068c p0l_bnr_dc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_var_th3                  : 8;   //  0: 7
	UINT32 dc_bnr_var_th2                  : 8;   //  8:15
	UINT32 dc_bnr_var_th1                  : 8;   // 16:23
	UINT32 dc_bnr_var_th0                  : 8;   // 24:31
} H14A0_P0L_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x0690 tpd_hist_ai_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                : 5;   //  0: 4
	UINT32                                 : 7;   //  5:11 reserved
	UINT32 hif_hist_ai                     : 1;   //    12
	UINT32 read_hist_sel                   : 2;   // 13:14
	UINT32 hif_hist_enable                 : 1;   //    15
	UINT32                                 : 12;  // 16:27 reserved
	UINT32 hist_rd_available               : 1;   //    28
	UINT32                                 : 2;   // 29:30 reserved
	UINT32 hist_read_done                  : 1;   //    31
} H14A0_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0694 tpd_hist_ai_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_status                     : 22;  //  0:21
} H14A0_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
	0x0698 lvcrtl_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                          : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 y_offset                        : 10;  //  4:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 center_position                 : 8;   // 16:23
	UINT32 y_gain                          : 8;   // 24:31
} H14A0_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x069c p0l_bnr_ac_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_scale                      : 3;   //  0: 2
	UINT32                                 : 1;   //     3 reserved
	UINT32 show_bar                        : 4;   //  4: 7
	UINT32 acness_or_gain_sel              : 1;   //     8
	UINT32 final_ac_bnr_gain_debug_h_en    : 1;   //     9
	UINT32 final_ac_bnr_gain_debug_v_en    : 1;   //    10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 mux_1                           : 4;   // 16:19
	UINT32 mux_2                           : 4;   // 20:23
	UINT32 mux_3                           : 4;   // 24:27
	UINT32 mux_4                           : 4;   // 28:31
} H14A0_P0L_BNR_AC_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x06a0 pe0_3dfd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_offset_x                     : 8;   //  0: 7
	UINT32 aw_offset_y                     : 8;   //  8:15
	UINT32 cb_th1                          : 8;   // 16:23
	UINT32 cb_th2                          : 8;   // 24:31
} H14A0_PE0_3DFD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x06a4 pe0_3dfd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_th1                          : 8;   //  0: 7
	UINT32 cc_th2                          : 8;   //  8:15
	UINT32 ll_th1                          : 8;   // 16:23
	UINT32 ll_th2                          : 8;   // 24:31
} H14A0_PE0_3DFD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x06a8 pe0_3dfd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pt_th                           : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 sstb_th                         : 8;   // 16:23
	UINT32 debug_mode                      : 3;   // 24:26
	UINT32                                 : 1;   //    27 reserved
	UINT32 div_mode                        : 1;   //    28
} H14A0_PE0_3DFD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x06ac pe0_3dfd_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 small_delta_th                  : 19;  //  0:18
	UINT32                                 : 5;   // 19:23 reserved
	UINT32 small_delta_en                  : 1;   //    24
} H14A0_PE0_3DFD_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x06b0 pe0_3dfd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_count                        : 16;  //  0:15
	UINT32 cc_count                        : 16;  // 16:31
} H14A0_PE0_3DFD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x06b4 pe0_3dfd_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ll_count                        : 16;  //  0:15
	UINT32 trid_format                     : 4;   // 16:19
} H14A0_PE0_3DFD_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x06b8 pe0_3dfd_stat_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_h                      : 23;  //  0:22
} H14A0_PE0_3DFD_STAT_02_T;

/*-----------------------------------------------------------------------------
	0x06bc pe0_3dfd_stat_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_v                      : 23;  //  0:22
} H14A0_PE0_3DFD_STAT_03_T;

/*-----------------------------------------------------------------------------
	0x06c0 pe0_3dfd_stat_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_v                      : 23;  //  0:22
} H14A0_PE0_3DFD_STAT_04_T;

/*-----------------------------------------------------------------------------
	0x06c4 pe0_3dfd_stat_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_h                      : 23;  //  0:22
} H14A0_PE0_3DFD_STAT_05_T;

/*-----------------------------------------------------------------------------
	0x06c8 pe0_3dfd_stat_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_ratio                        : 23;  //  0:22
} H14A0_PE0_3DFD_STAT_06_T;

/*-----------------------------------------------------------------------------
	0x06cc pe0_3dfd_stat_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_ratio                        : 23;  //  0:22
} H14A0_PE0_3DFD_STAT_07_T;

/*-----------------------------------------------------------------------------
	0x06d0 p0l_tnr_ctrl_38 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_coring_en_c                   : 1;   //     0
	UINT32 m_coring_en_y                   : 1;   //     1
	UINT32 mc_sad_sel                      : 1;   //     2
	UINT32 sad_protect_en                  : 1;   //     3
	UINT32 m_debug_mode                    : 3;   //  4: 6
	UINT32 m_debug_mode_en                 : 1;   //     7
	UINT32 m_gain_c                        : 4;   //  8:11
	UINT32 m_gain_y                        : 4;   // 12:15
	UINT32 m_coring_th                     : 6;   // 16:21
	UINT32 sad_protect_gm_en               : 1;   //    22
	UINT32                                 : 1;   //    23 reserved
	UINT32 mv_cost_gmv_smooth_gain         : 5;   // 24:28
	UINT32                                 : 2;   // 29:30 reserved
	UINT32 mv_cost_gmv_smooth_en           : 1;   //    31
} H14A0_P0L_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
	0x06d4 p0l_tnr_ctrl_39 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 secam_mot_c_th                  : 8;   //  0: 7
	UINT32 secam_mot_y_th                  : 8;   //  8:15
	UINT32 secam_c_v_th                    : 8;   // 16:23
	UINT32                                 : 6;   // 24:29 reserved
	UINT32 secam_c_v_calc_mode             : 2;   // 30:31
} H14A0_P0L_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
	0x06d8 p0l_tnr_ctrl_40 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_en                           : 1;   //     0
	UINT32 sf_map_flt_en                   : 1;   //     1
	UINT32 sf_debug_mode                   : 3;   //  2: 4
	UINT32 sf_map_tap                      : 2;   //  5: 6
	UINT32                                 : 1;   //     7 reserved
	UINT32 sf_map_gain                     : 6;   //  8:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 sf_th0                          : 8;   // 16:23
	UINT32 sf_th1                          : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
	0x06dc p0l_tnr_ctrl_41 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_th2                          : 8;   //  0: 7
	UINT32 sf_th3                          : 8;   //  8:15
	UINT32 sf_th4                          : 8;   // 16:23
	UINT32 sf_f0_sel                       : 2;   // 24:25
	UINT32 sf_f1_sel                       : 2;   // 26:27
	UINT32 sf_f2_sel                       : 2;   // 28:29
	UINT32 sf_f3_sel                       : 2;   // 30:31
} H14A0_P0L_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
	0x06e0 p0l_tnr_ctrl_42 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y0                        : 8;   //  0: 7
	UINT32 sf_mb_x0                        : 8;   //  8:15
	UINT32 sf_mb_y1                        : 8;   // 16:23
	UINT32 sf_mb_x1                        : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
	0x06e4 p0l_tnr_ctrl_43 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0l_tnr_ctrl_43_f               : 32;   // 31: 0
} H14A0_P0L_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
	0x06e8 p0l_tnr_ctrl_44 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y2                        : 8;   //  0: 7
	UINT32 sf_mb_x2                        : 8;   //  8:15
	UINT32 sf_mb_y3                        : 8;   // 16:23
	UINT32 sf_mb_x3                        : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
	0x06ec p0l_tnr_ctrl_45 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 sf_debug_map_sel                : 2;   // 16:17
	UINT32 sf_gain_mode                    : 1;   //    18
	UINT32                                 : 1;   //    19 reserved
	UINT32 sf_gain                         : 4;   // 20:23
} H14A0_P0L_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
	0x06f0 p0l_tnr_ctrl_46 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_blend_y0                   : 8;   //  0: 7
	UINT32 mamc_blend_x0                   : 8;   //  8:15
	UINT32 mamc_blend_y1                   : 8;   // 16:23
	UINT32 mamc_blend_x1                   : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
	0x06f4 p0l_tnr_ctrl_47 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_lock_th0                    : 6;   //  0: 5
	UINT32 gmv_max_cnt_th                  : 2;   //  6: 7
	UINT32 gmv_lock_th1                    : 6;   //  8:13
	UINT32 gmv_val_cnt_th                  : 2;   // 14:15
	UINT32 gmv_manual_mv_en                : 1;   //    16
	UINT32 gmv_manual_alpha_en             : 1;   //    17
	UINT32                                 : 1;   //    18 reserved
	UINT32 gmv_lock_en                     : 1;   //    19
	UINT32 gmv_lock_max                    : 4;   // 20:23
	UINT32 gmv_manual_mv                   : 4;   // 24:27
	UINT32 gmv_manual_alpha                : 4;   // 28:31
} H14A0_P0L_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
	0x06f8 p0l_tnr_ctrl_48 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_gain_ctrl2_y0                 : 8;   //  0: 7
	UINT32 m_gain_ctrl2_x0                 : 8;   //  8:15
	UINT32 m_gain_ctrl2_y1                 : 8;   // 16:23
	UINT32 m_gain_ctrl2_x1                 : 8;   // 24:31
} H14A0_P0L_TNR_CTRL_48_T;

/*-----------------------------------------------------------------------------
	0x2400 p0r_pe0_load ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                       : 1;   //     0
	UINT32 load_enable                     : 1;   //     1
	UINT32 load_type                       : 1;   //     2
} H14A0_P0R_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
	0x2404 p0r_pe0_intr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                     : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 intr_src                        : 3;   //  4: 6
	UINT32                                 : 9;   //  7:15 reserved
	UINT32 intr_line_pos                   : 11;  // 16:26
} H14A0_P0R_PE0_INTR_T;

/*-----------------------------------------------------------------------------
	0x2408 p0r_pe0_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                    : 4;   //  0: 3
	UINT32 ipc_frame_id                    : 4;   //  4: 7
	UINT32 tnrw_y_frame_id                 : 4;   //  8:11
	UINT32 tnrw_c_frame_id                 : 4;   // 12:15
	UINT32 dnr_frame_id                    : 8;   // 16:23
	UINT32 pic_init_frame_id               : 4;   // 24:27
} H14A0_P0R_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x240c p0r_pe0_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                        : 11;  //  0:10
} H14A0_P0R_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x2410 p0r_pe0_opmode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                        : 1;   //     0
	UINT32 tnr_y_en                        : 1;   //     1
	UINT32 ipc_c_en                        : 1;   //     2
	UINT32 ipc_y_en                        : 1;   //     3
	UINT32 tnr_c_422_as_420                : 1;   //     4
	UINT32 tnr_y2_en                       : 1;   //     5
	UINT32 force_422_to_420                : 1;   //     6
	UINT32 ipc0_y_en_prog                  : 1;   //     7
	UINT32 mif_last_mode                   : 3;   //  8:10
	UINT32                                 : 1;   //    11 reserved
	UINT32 dnr_out_force_44x               : 1;   //    12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 detour_enable                   : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 ibuf_en                         : 2;   // 20:21
	UINT32                                 : 2;   // 22:23 reserved
	UINT32 dnr_in_lsb_mode                 : 2;   // 24:25
	UINT32                                 : 2;   // 26:27 reserved
	UINT32 tnr_in_lsb_mode                 : 2;   // 28:29
} H14A0_P0R_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
	0x2414 p0r_pe0_src_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                           : 11;  //  0:10
	UINT32                                 : 2;   // 11:12 reserved
	UINT32 cs_type                         : 3;   // 13:15
	UINT32 vsize                           : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 scan_type                       : 1;   //    31
} H14A0_P0R_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
	0x2418 p0r_pe0_field ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                        : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 field_mode                      : 2;   //  4: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 secam_line                      : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 secam_line_mode                 : 2;   // 12:13
} H14A0_P0R_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
	0x241c p0r_pe0_param_by_ddr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 9;   //  0: 8 reserved
	UINT32 stride                          : 2;   //  9:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 vsize                           : 8;   // 16:23
	UINT32                                 : 6;   // 24:29 reserved
	UINT32 endian                          : 1;   //    30
	UINT32 update_by_ddr                   : 1;   //    31
} H14A0_P0R_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
	0x2420 p0r_win_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y0                              : 12;  // 16:27
} H14A0_P0R_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x2424 p0r_win_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0R_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x2428 p0r_win_ctrl_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y0                              : 12;  // 16:27
	UINT32 ac_bnr_feature_cal_mode         : 2;   // 28:29
} H14A0_P0R_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
	0x242c p0r_win_ctrl_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0R_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
	0x2430 p0r_pe0_debug_mem_req ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_t1_y                      : 1;   //     0
	UINT32 en_rd_t1_c                      : 1;   //     1
	UINT32 en_rd_t2_y                      : 1;   //     2
	UINT32 en_rd_t2_c                      : 1;   //     3
	UINT32 en_rd_t3_y                      : 1;   //     4
	UINT32 en_rd_t3_c                      : 1;   //     5
	UINT32 en_rd_t4_y                      : 1;   //     6
	UINT32 en_rd_t1_m                      : 1;   //     7
	UINT32 en_wr_tnrw_y                    : 1;   //     8
	UINT32 en_wr_tnrw_c                    : 1;   //     9
	UINT32 en_wr_tnrw_m                    : 1;   //    10
	UINT32                                 : 4;   // 11:14 reserved
	UINT32 manual_mem_en                   : 1;   //    15
} H14A0_P0R_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
	0x2434 p0r_pe0_debug_lreq_dly ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                      : 11;  //  0:10
	UINT32 lreq_dly_c_en                   : 1;   //    11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 lreq_dly_y                      : 11;  // 16:26
	UINT32 lreq_dly_y_en                   : 1;   //    27
} H14A0_P0R_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
	0x2440 p0r_tnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_enable                      : 1;   //     0
	UINT32                                 : 1;   //     1 reserved
	UINT32 nt_lvl_hsub                     : 1;   //     2
	UINT32 secam_cmode_en                  : 1;   //     3
	UINT32 secam_alpha_scale               : 2;   //  4: 5
	UINT32 prev_sel_prog                   : 1;   //     6
	UINT32 frame_skip_enable_for_3d        : 1;   //     7
	UINT32 minfo_mode                      : 2;   //  8: 9
	UINT32 minfo_sw                        : 2;   // 10:11
	UINT32 hist_motion_scale               : 2;   // 12:13
	UINT32 minfo_flt_en_tnr                : 1;   //    14
	UINT32 minfo_flt_en_ipc                : 1;   //    15
	UINT32 calc_motion_max_yc              : 1;   //    16
	UINT32 motion_4f_en                    : 1;   //    17
	UINT32 mv_tflt_en                      : 1;   //    18
	UINT32 mv_sflt_en                      : 1;   //    19
	UINT32 alpha_avg_en                    : 1;   //    20
	UINT32                                 : 1;   //    21 reserved
	UINT32 alpha_mapping_y_en              : 1;   //    22
	UINT32 alpha_mapping_c_en              : 1;   //    23
	UINT32 nt_lvl_adjust_gm_enable         : 1;   //    24
	UINT32 nt_lvl_adjust_lpct_enable       : 1;   //    25
	UINT32                                 : 1;   //    26 reserved
	UINT32 nt_lvl_adjust_avg_ts_enable     : 1;   //    27
	UINT32 nt_lvl_adjust_lpct_sel          : 1;   //    28
	UINT32 mamc_blend_sel                  : 1;   //    29
} H14A0_P0R_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2444 p0r_tnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                         : 8;   //  0: 7
	UINT32 y_th_hi                         : 8;   //  8:15
	UINT32 dnr_motion_scale                : 2;   // 16:17
	UINT32                                 : 2;   // 18:19 reserved
	UINT32 tnr_filter_lsb_ctrl             : 2;   // 20:21
	UINT32 svar_mux_ctrl                   : 2;   // 22:23
	UINT32 nt_lvl_hist_n_pct               : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2448 p0r_tnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_flat_svar_th                 : 8;   //  0: 7
	UINT32 alpha_lut_ind7                  : 8;   //  8:15
	UINT32 secam_cmode_flt_en              : 1;   //    16
	UINT32                                 : 3;   // 17:19 reserved
	UINT32 secam_cmode_flt_th              : 4;   // 20:23
	UINT32 m_gain_en_var                   : 1;   //    24
	UINT32 a_gain_en_var                   : 1;   //    25
	UINT32 m_gain_en_lsc                   : 1;   //    26
	UINT32 a_gain_en_lsc                   : 1;   //    27
	UINT32 m_gain_en_var2                  : 1;   //    28
	UINT32 m_offset_mode_en                : 1;   //    29
} H14A0_P0R_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x244c p0r_tnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                           : 8;   //  0: 7
	UINT32 wa_y1                           : 8;   //  8:15
	UINT32 wa_x0                           : 8;   // 16:23
	UINT32 wa_x1                           : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2450 p0r_tnr_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0               : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x0               : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y1               : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x1               : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x2454 p0r_tnr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                          : 8;   //  0: 7
	UINT32 nt_max                          : 8;   //  8:15
	UINT32 hist_svar_th                    : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th0            : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x2458 p0r_tnr_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                       : 8;   //  0: 7
	UINT32 nt_iir_alpha                    : 8;   //  8:15
	UINT32 y_th_lo2                        : 8;   // 16:23
	UINT32 y_th_hi2                        : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x245c p0r_tnr_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                  : 8;   //  0: 7
	UINT32 alpha_lut_ind1                  : 8;   //  8:15
	UINT32 alpha_lut_ind2                  : 8;   // 16:23
	UINT32 alpha_lut_ind3                  : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x2460 p0r_tnr_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                  : 8;   //  0: 7
	UINT32 alpha_lut_ind5                  : 8;   //  8:15
	UINT32 alpha_lut_ind6                  : 8;   // 16:23
	UINT32 adj_alpha_nt_lvl_th1            : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x2464 p0r_tnr_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 scene_change_enable             : 1;   //     0
	UINT32 scene_change_max_flt_en         : 1;   //     1
	UINT32 scene_change_measure_sel        : 1;   //     2
	UINT32 scene_change_prev_hist_calc_en  : 1;   //     3
	UINT32 scene_change_enforce            : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 scene_change_max_flt_tap        : 2;   //  8: 9
	UINT32                                 : 2;   // 10:11 reserved
	UINT32 scene_change_scale              : 3;   // 12:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 scene_change_th                 : 8;   // 16:23
	UINT32 gmv_svar_th                     : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x2468 p0r_tnr_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_show_tnr_param              : 4;   //  0: 3
	UINT32 dbg_status_sum_sel              : 2;   //  4: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 dbg_force_wa                    : 1;   //     8
	UINT32 dbg_force_calc_motion           : 1;   //     9
	UINT32 dbg_force_lut_alpha             : 1;   //    10
	UINT32 dbg_force_adj_alpha             : 1;   //    11
	UINT32 dbg_force_final_alpha           : 1;   //    12
	UINT32                                 : 2;   // 13:14 reserved
	UINT32 ignore_active_window            : 1;   //    15
	UINT32 dbg_tnrw_port_ctrl              : 2;   // 16:17
	UINT32                                 : 2;   // 18:19 reserved
	UINT32 tnr_black_line_en_for_i         : 1;   //    20
	UINT32 tnr_debug_map_en_for_i          : 1;   //    21
	UINT32                                 : 2;   // 22:23 reserved
	UINT32 dbg_wa                          : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x246c p0r_tnr_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                 : 8;   //  0: 7
	UINT32 dbg_alpha                       : 8;   //  8:15
	UINT32 peak0_x_th                      : 4;   // 16:19
	UINT32                                 : 4;   // 20:23 reserved
	UINT32 scene_change_th_mv              : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x2470 p0r_tnr_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2               : 8;   //  0: 7
	UINT32 nt_lvl_gain_gm_x2               : 8;   //  8:15
	UINT32 nt_lvl_gain_gm_y3               : 8;   // 16:23
	UINT32 nt_lvl_gain_gm_x3               : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x2474 p0r_tnr_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                    : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 force_nt_lvl_val                : 8;   //  8:15
	UINT32 adjust_nt_lvl                   : 1;   //    16
	UINT32                                 : 7;   // 17:23 reserved
	UINT32 adjust_nt_lvl_val               : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x2478 p0r_tnr_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y0                              : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 noise_measure_win_en            : 1;   //    31
} H14A0_P0R_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x247c p0r_tnr_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0R_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x2480 p0r_tnr_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_scale              : 3;   //  0: 2
	UINT32                                 : 1;   //     3 reserved
	UINT32 show_dbg_bar_en                 : 4;   //  4: 7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 show_dbg_bar0_mux               : 4;   // 16:19
	UINT32 show_dbg_bar1_mux               : 4;   // 20:23
	UINT32 show_dbg_bar2_mux               : 4;   // 24:27
	UINT32 show_dbg_bar3_mux               : 4;   // 28:31
} H14A0_P0R_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x2484 p0r_tnr_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y0                  : 8;   //  0: 7
	UINT32 m_tiir_ctrl_x0                  : 8;   //  8:15
	UINT32 m_tiir_ctrl_y1                  : 8;   // 16:23
	UINT32 m_tiir_ctrl_x1                  : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x2488 p0r_tnr_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_enable                : 1;   //     0
	UINT32 luma_gain_enable                : 1;   //     1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 input_for_luma                  : 2;   //  4: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 input_for_crgn_satr             : 1;   //     8
	UINT32                                 : 7;   //  9:15 reserved
	UINT32 y_gain                          : 8;   // 16:23
	UINT32 c_gain                          : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x248c p0r_tnr_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                  : 8;   //  0: 7
	UINT32 luma_gain_p1_x                  : 8;   //  8:15
	UINT32 luma_gain_p2_x                  : 8;   // 16:23
	UINT32 luma_gain_p3_x                  : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x2490 p0r_tnr_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                  : 8;   //  0: 7
	UINT32 luma_gain_p1_y                  : 8;   //  8:15
	UINT32 luma_gain_p2_y                  : 8;   // 16:23
	UINT32 luma_gain_p3_y                  : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x2494 p0r_tnr_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_mamc_blend                  : 8;   //  0: 7
} H14A0_P0R_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x2498 p0r_tnr_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_protect_control_y0           : 8;   //  0: 7
	UINT32 mv_protect_control_x0           : 8;   //  8:15
	UINT32 mv_protect_control_y1           : 8;   // 16:23
	UINT32 mv_protect_control_x1           : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x249c p0r_tnr_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0              : 8;   //  0: 7
	UINT32 alpha_mapping_y_x0              : 8;   //  8:15
	UINT32 alpha_mapping_y_y1              : 8;   // 16:23
	UINT32 alpha_mapping_y_x1              : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x24a0 p0r_tnr_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2              : 8;   //  0: 7
	UINT32 alpha_mapping_y_x2              : 8;   //  8:15
	UINT32 alpha_mapping_y_y3              : 8;   // 16:23
	UINT32 alpha_mapping_y_x3              : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x24a4 p0r_tnr_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0              : 8;   //  0: 7
	UINT32 alpha_mapping_c_x0              : 8;   //  8:15
	UINT32 alpha_mapping_c_y1              : 8;   // 16:23
	UINT32 alpha_mapping_c_x1              : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x24a8 p0r_tnr_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2              : 8;   //  0: 7
	UINT32 alpha_mapping_c_x2              : 8;   //  8:15
	UINT32 alpha_mapping_c_y3              : 8;   // 16:23
	UINT32 alpha_mapping_c_x3              : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x24ac p0r_tnr_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                  : 8;   //  0: 7
	UINT32 crgn_gain_rgn1                  : 8;   //  8:15
	UINT32 crgn_gain_rgn2                  : 8;   // 16:23
	UINT32 crgn_gain_rgn3                  : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x24b0 p0r_tnr_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0               : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x0               : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y1               : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x1               : 7;   // 24:30
} H14A0_P0R_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x24b4 p0r_tnr_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                           : 8;   //  0: 7
	UINT32 wa_y3                           : 8;   //  8:15
	UINT32 wa_x2                           : 8;   // 16:23
	UINT32 wa_x3                           : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x24b8 p0r_tnr_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2               : 8;   //  0: 7
	UINT32 nt_lvl_gain_st_x2               : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_st_y3               : 8;   // 16:23
	UINT32 nt_lvl_gain_st_x3               : 7;   // 24:30
} H14A0_P0R_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x24bc p0r_tnr_ctrl_31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c            : 1;   //     0
	UINT32 spatial_pre_flt_en_y            : 1;   //     1
	UINT32                                 : 14;  //  2:15 reserved
	UINT32 spatial_pre_flt_th_c            : 8;   // 16:23
	UINT32 spatial_pre_flt_th_y            : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
	0x24c0 p0r_tnr_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                         : 8;   //  0: 7
	UINT32 x_peak_t2                       : 8;   //  8:15
	UINT32 nt_lvl                          : 8;   // 16:23
	UINT32 scene_change                    : 8;   // 24:31
} H14A0_P0R_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x24c4 p0r_tnr_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_s                         : 8;   //  0: 7
	UINT32 valid_cnt_motion_histogram      : 16;  //  8:23
} H14A0_P0R_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x24c8 p0r_tnr_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_l                            : 4;   //  0: 3
	UINT32 mv_r                            : 4;   //  4: 7
	UINT32 dfd                             : 8;   //  8:15
	UINT32 gm                              : 8;   // 16:23
	UINT32 lp                              : 8;   // 24:31
} H14A0_P0R_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x24cc p0r_tnr_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_valid_cnt                   : 16;  //  0:15
	UINT32 fd                              : 8;   // 16:23
	UINT32 gmv_lock_cnt                    : 6;   // 24:29
} H14A0_P0R_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x24d0 p0r_tnr_main_lut_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr             : 5;   //  0: 4
	UINT32                                 : 7;   //  5:11 reserved
	UINT32 main_lut_ai_enable              : 1;   //    12
} H14A0_P0R_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
	0x24d4 p0r_tnr_main_lut_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0            : 32;  //  0:31
} H14A0_P0R_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
	0x24d8 p0r_tnr_main_lut_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1            : 32;  //  0:31
} H14A0_P0R_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
	0x24e0 p0r_tnr_ctrl_32 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0             : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x0             : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y1             : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x1             : 7;   // 24:30
} H14A0_P0R_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
	0x24e4 p0r_tnr_ctrl_33 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2             : 8;   //  0: 7
	UINT32 nt_lvl_gain_lpct_x2             : 7;   //  8:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 nt_lvl_gain_lpct_y3             : 8;   // 16:23
	UINT32 nt_lvl_gain_lpct_x3             : 7;   // 24:30
} H14A0_P0R_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
	0x24e8 p0r_tnr_ctrl_34 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_ctrl_y0                 : 8;   //  0: 7
	UINT32 mc_blur_ctrl_x0                 : 8;   //  8:15
	UINT32 mc_blur_ctrl_y1                 : 8;   // 16:23
	UINT32 mc_blur_ctrl_x1                 : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
	0x24ec p0r_tnr_ctrl_35 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y2                  : 8;   //  0: 7
	UINT32 m_tiir_ctrl_x2                  : 8;   //  8:15
	UINT32 m_tiir_ctrl_y3                  : 8;   // 16:23
	UINT32 m_tiir_ctrl_x3                  : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
	0x24f0 p0r_tnr_crg_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr            : 8;   //  0: 7
	UINT32                                 : 4;   //  8:11 reserved
	UINT32 crgn_ctrl_ai_enable             : 1;   //    12
} H14A0_P0R_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x24f4 p0r_tnr_crg_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data            : 32;  //  0:31
} H14A0_P0R_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x24f8 p0r_tnr_ctrl_36 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_sel_hist                    : 1;   //     0
	UINT32 sad_sel_pels                    : 1;   //     1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 hme_half_pel_en                 : 1;   //     4
	UINT32 m_tiir_x_sel                    : 1;   //     5
	UINT32 m_tiir_scene_change_en          : 1;   //     6
	UINT32 m_tiir_en                       : 1;   //     7
	UINT32 sf_blend_en_y                   : 1;   //     8
	UINT32 sf_blend_en_c                   : 1;   //     9
	UINT32                                 : 2;   // 10:11 reserved
	UINT32 sf_blend_motion_exp_mode        : 2;   // 12:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 mv0_protect_th                  : 7;   // 16:22
	UINT32 mv0_protect_en                  : 1;   //    23
	UINT32 mv_4f_en                        : 1;   //    24
	UINT32 mv_pfilter_en                   : 1;   //    25
	UINT32 mv_cost_smooth_gain             : 5;   // 26:30
	UINT32 mv_cost_smooth_en               : 1;   //    31
} H14A0_P0R_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
	0x24fc p0r_tnr_ctrl_37 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_flt_en_c                     : 1;   //     0
	UINT32 mc_flt_mv_inv_c                 : 1;   //     1
	UINT32 mc_flt_mv_debug_en_c            : 1;   //     2
	UINT32 mc_flt_manual_blend_en_c        : 1;   //     3
	UINT32 mc_flt_mv_debug_c               : 4;   //  4: 7
	UINT32 mc_flt_manual_blend_c           : 6;   //  8:13
	UINT32                                 : 1;   //    14 reserved
	UINT32 mc_flt_blur_en_c                : 1;   //    15
	UINT32 mc_flt_en_y                     : 1;   //    16
	UINT32 mc_flt_mv_inv_y                 : 1;   //    17
	UINT32 mc_flt_mv_debug_en_y            : 1;   //    18
	UINT32 mc_flt_manual_blend_en_y        : 1;   //    19
	UINT32 mc_flt_mv_debug_y               : 4;   // 20:23
	UINT32 mc_flt_manual_blend_y           : 6;   // 24:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 mc_flt_blur_en_y                : 1;   //    31
} H14A0_P0R_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
	0x2500 p0r_ipc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                    : 2;   //  0: 1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 cr_chroma_mode                  : 2;   //  4: 5
	UINT32                                 : 1;   //     6 reserved
	UINT32 cr_chroma_edi                   : 1;   //     7
	UINT32 cr_integer_edi                  : 1;   //     8
	UINT32                                 : 15;  //  9:23 reserved
	UINT32 cr_hmc_force_fmd                : 1;   //    24
	UINT32                                 : 1;   //    25 reserved
	UINT32 cr_clc_fmd_off                  : 1;   //    26
	UINT32 cr_st_flt_original_en           : 1;   //    27
	UINT32 cr_film_auto_gain               : 1;   //    28
	UINT32 cr_film_apply_c                 : 1;   //    29
	UINT32 cr_film_apply_y                 : 1;   //    30
	UINT32 cr_film_mode_enable             : 1;   //    31
} H14A0_P0R_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2504 p0r_ipc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 cr_st_flt_enable                : 1;   //    16
	UINT32 cr_st_2d_v_flt_en               : 1;   //    17
	UINT32 cr_hmc_flt_enable               : 1;   //    18
	UINT32 cr_game_mode                    : 1;   //    19
	UINT32 cr_st_2d_v_flt_range            : 4;   // 20:23
	UINT32                                 : 1;   //    24 reserved
	UINT32 cr_black_mask                   : 1;   //    25
	UINT32 cr_ipc_debug_show               : 5;   // 26:30
	UINT32 cr_5f_mode                      : 1;   //    31
} H14A0_P0R_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2508 p0r_ipc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0             : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_x4             : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x1             : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y1             : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x250c p0r_ipc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2             : 8;   //  0: 7
	UINT32 fuzzy_ctrl_point_y2             : 8;   //  8:15
	UINT32 fuzzy_ctrl_point_x3             : 8;   // 16:23
	UINT32 fuzzy_ctrl_point_y3             : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2510 p0r_ipc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset_s7                    : 8;   //  0: 7
	UINT32 motion_x_tearing_gain           : 8;   //  8:15
	UINT32 tearing_gain                    : 8;   // 16:23
	UINT32 motion_gain                     : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x2514 p0r_ipc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha           : 8;   //  0: 7
	UINT32 cr_t_grad_weight                : 8;   //  8:15
	UINT32 cr_v_grad_weight                : 8;   // 16:23
	UINT32 cr_h_grad_weight                : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x2518 p0r_ipc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                 : 1;   //     0
	UINT32 cr_flat_only                    : 1;   //     1
	UINT32 use_tnr_motion                  : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 cr_tearing_normalize_en         : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 cr_normalize_base               : 8;   //  8:15
	UINT32 cr_normalize_center             : 8;   // 16:23
	UINT32 cr_motion_coring                : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x251c p0r_ipc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 1;   //     0 reserved
	UINT32 cr_v_en                         : 1;   //     1
	UINT32 cr_45_en                        : 1;   //     2
	UINT32 cr_15_en                        : 1;   //     3
	UINT32                                 : 20;  //  4:23 reserved
	UINT32 tnr_motion_gain                 : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x2520 p0r_ipc_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top           : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 fmd_caption_range_bot           : 12;  // 16:27
	UINT32                                 : 2;   // 28:29 reserved
	UINT32 caption_flow_detected           : 1;   //    30
	UINT32 hmc_when_cap_not_detected       : 1;   //    31
} H14A0_P0R_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x2524 p0r_ipc_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                     : 8;   //  0: 7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 pn_variance_th                  : 8;   // 16:23
	UINT32 pn_fmd_motion_th                : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x2528 p0r_ipc_ctrl_10 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                   : 8;   //  0: 7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 pcnc_variance_th                : 8;   // 16:23
	UINT32 pcnc_fmd_motion_th              : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
	0x252c p0r_ipc_ctrl_11 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                    : 8;   //  0: 7
	UINT32 cr_center_bound_ctrl            : 8;   //  8:15
	UINT32 cr_dn_bound_ctrl                : 8;   // 16:23
	UINT32 cr_up_bound_ctrl                : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
	0x2530 p0r_ipc_ctrl_12 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 still_pn_th                     : 8;   //  0: 7
	UINT32 fmd_ratio_th                    : 8;   //  8:15
	UINT32 fmd_max_th                      : 8;   // 16:23
	UINT32 fmd_min_th                      : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
	0x2534 p0r_ipc_ctrl_13 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view               : 13;  //  0:12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 cr_hmc_debug_mode               : 1;   //    16
	UINT32                                 : 2;   // 17:18 reserved
	UINT32 cr_hmc_debug_dir                : 5;   // 19:23
	UINT32 cr_hmc_debug_gain               : 6;   // 24:29
} H14A0_P0R_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
	0x2538 p0r_ipc_ctrl_14 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 lockcnt_en                      : 1;   //     0
	UINT32 manual_en                       : 1;   //     1
	UINT32 manual_mode                     : 2;   //  2: 3
	UINT32 lock_speed                      : 4;   //  4: 7
	UINT32 lockcnt_th                      : 8;   //  8:15
	UINT32 same_th                         : 8;   // 16:23
	UINT32 still_frame_limit               : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
	0x253c p0r_ipc_ctrl_15 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 protection_en                   : 1;   //     0
	UINT32 protection_s1en                 : 1;   //     1
	UINT32 protection_s2en                 : 1;   //     2
	UINT32 protection_s3en                 : 1;   //     3
	UINT32 protection_s4en                 : 1;   //     4
	UINT32 badedit_protect                 : 1;   //     5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 modify_ratio_th                 : 8;   //  8:15
	UINT32 fmd_tower_spangain              : 4;   // 16:19
	UINT32 fmd_bleacherratio               : 4;   // 20:23
	UINT32 fmd_vlrratiomin                 : 4;   // 24:27
	UINT32 fmd_spxl2ratio                  : 4;   // 28:31
} H14A0_P0R_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
	0x2540 p0r_ipc_ctrl_16 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 still_pn_th2                    : 8;   //  0: 7
	UINT32 still_prev_ratio                : 8;   //  8:15
	UINT32 lock_reducer                    : 8;   // 16:23
	UINT32 same_min_ratio                  : 4;   // 24:27
	UINT32 same_adder_gain                 : 4;   // 28:31
} H14A0_P0R_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
	0x2544 p0r_ipc_ctrl_17 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution               : 2;   //  0: 1
	UINT32 cr_field_manual_mode            : 2;   //  2: 3
	UINT32 cr_field_inverse                : 1;   //     4
	UINT32                                 : 19;  //  5:23 reserved
	UINT32 cr_tearing_a_th                 : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
	0x2548 p0r_ipc_ctrl_18 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                      : 8;   //  0: 7
	UINT32 cr_lr_gain                      : 8;   //  8:15
	UINT32 cr_h_grad_gain                  : 8;   // 16:23
	UINT32 cr_v_grad_gain                  : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
	0x254c p0r_ipc_ctrl_19 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 errs_gain                       : 8;   //  0: 7
	UINT32 confidance_gain                 : 8;   //  8:15
	UINT32 pe0_2d_offset                   : 8;   // 16:23
	UINT32 flicker_gain                    : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
	0x2550 p0r_ipc_status_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                    : 16;  //  0:15
	UINT32 r_nc_v_count                    : 16;  // 16:31
} H14A0_P0R_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
	0x2554 p0r_ipc_status_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                    : 16;  //  0:15
	UINT32 r_pn_v_count                    : 16;  // 16:31
} H14A0_P0R_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
	0x2558 p0r_ipc_status_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_same                    : 16;  //  0:15
	UINT32 r_pc_pn_same                    : 16;  // 16:31
} H14A0_P0R_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
	0x255c p0r_ipc_status_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pn_lr_count                   : 16;  //  0:15
	UINT32 same_ng_flag                    : 1;   //    16
	UINT32 s3_protect_en_flag              : 1;   //    17
	UINT32 s2_protect_en_flag              : 1;   //    18
	UINT32 s1_protect_en_flag              : 1;   //    19
	UINT32 modify_pn_ratio_flag            : 1;   //    20
	UINT32 base_pn_ratio_flag              : 1;   //    21
	UINT32 r_fmd_comb_mode                 : 2;   // 22:23
	UINT32 r_fmd_lock_count                : 8;   // 24:31
} H14A0_P0R_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
	0x2580 vfilter_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vfilterenable                   : 1;   //     0
	UINT32 vfilterlocalenable              : 1;   //     1
	UINT32 vfilter_strength                : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 cb_filter_y                     : 2;   //  4: 5
	UINT32 cb_filter_c                     : 2;   //  6: 7
	UINT32 vfilter_a_th                    : 8;   //  8:15
	UINT32 vfilter_in_field_var            : 8;   // 16:23
	UINT32 vfilter_use_mc_mask             : 1;   //    24
	UINT32 vfilter_expand                  : 1;   //    25
	UINT32 vfilter_center                  : 1;   //    26
	UINT32 vfilter_ul4                     : 1;   //    27
	UINT32 dbgshow_level                   : 2;   // 28:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 dbgshow_enable                  : 1;   //    31
} H14A0_VFILTER_00_T;

/*-----------------------------------------------------------------------------
	0x2584 p0r_cti_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                  : 8;   //  0: 7
	UINT32 cti_coring_th1                  : 8;   //  8:15
	UINT32 cti_coring_smooth               : 3;   // 16:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 cti_coring_map_size             : 2;   // 20:21
} H14A0_P0R_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x2588 p0r_mnr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_enable                      : 1;   //     0
	UINT32 mnr_debug                       : 2;   //  1: 2
	UINT32 mnr_motion_view                 : 1;   //     3
	UINT32 h_expend                        : 1;   //     4
	UINT32 gain_flt_size                   : 1;   //     5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 mnr_master_gain                 : 8;   //  8:15
	UINT32 chroma_master_gain              : 8;   // 16:23
} H14A0_P0R_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x258c p0r_mnr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mnr_s1_mmd_min                  : 8;   //  0: 7
	UINT32 mnr_s2_ratio_min                : 8;   //  8:15
	UINT32 mnr_s2_ratio_max                : 8;   // 16:23
	UINT32 mnr_s2_mmd_min                  : 8;   // 24:31
} H14A0_P0R_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2590 p0r_mnr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 mnr_h_gain                      : 8;   // 16:23
	UINT32 mnr_v_gain                      : 8;   // 24:31
} H14A0_P0R_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x2594 p0r_mnr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 filter_x0                       : 8;   //  0: 7
	UINT32 filter_x1                       : 8;   //  8:15
	UINT32 filter_y0                       : 8;   // 16:23
	UINT32 filter_y1                       : 8;   // 24:31
} H14A0_P0R_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2598 p0r_mnr_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 motion_mnr_en                   : 1;   //     0
	UINT32 motion_mnr_filter               : 1;   //     1
	UINT32                                 : 14;  //  2:15 reserved
	UINT32 mnr_motion_min                  : 8;   // 16:23
	UINT32 mnr_motion_max                  : 8;   // 24:31
} H14A0_P0R_MNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x259c p0r_mnr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 motion_mnr_x0                   : 8;   //  0: 7
	UINT32 motion_mnr_x1                   : 8;   //  8:15
	UINT32 motion_mnr_y0                   : 8;   // 16:23
	UINT32 motion_mnr_y1                   : 8;   // 24:31
} H14A0_P0R_MNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x25a0 p0r_bnr_detail_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_detail_max               : 8;   //  0: 7
	UINT32 bnr_ac_detail_min               : 8;   //  8:15
	UINT32 bnr_diff_l                      : 8;   // 16:23
	UINT32 bnr_diff_p                      : 8;   // 24:31
} H14A0_P0R_BNR_DETAIL_CTRL_T;

/*-----------------------------------------------------------------------------
	0x25a4 p0r_vfilter_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable           : 1;   //     0
	UINT32                                 : 15;  //  1:15 reserved
	UINT32 cr_hmc_protect_th_max           : 8;   // 16:23
	UINT32 cr_hmc_protect_th_min           : 8;   // 24:31
} H14A0_P0R_VFILTER_02_T;

/*-----------------------------------------------------------------------------
	0x25a8 p0r_vfilter_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en            : 1;   //     0
	UINT32 cr_vfilter_force_en             : 1;   //     1
	UINT32 cr_global_frame_num_th          : 4;   //  2: 5
	UINT32                                 : 2;   //  6: 7 reserved
	UINT32 cr_vfilter_global_th            : 12;  //  8:19
	UINT32 tearing_count                   : 12;  // 20:31
} H14A0_P0R_VFILTER_01_T;

/*-----------------------------------------------------------------------------
	0x25ac p0r_bnr_dc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_enable                   : 1;   //     0
	UINT32 dc_blur_debug_mode              : 1;   //     1
	UINT32 dc_blur_sel                     : 2;   //  2: 3
	UINT32 dc_output_debug_mode            : 4;   //  4: 7
	UINT32 bnr_ac_manual_init_h            : 4;   //  8:11
	UINT32 bnr_ac_manual_init_v            : 4;   // 12:15
	UINT32 dc_motion_max                   : 8;   // 16:23
	UINT32 dc_motion_min                   : 8;   // 24:31
} H14A0_P0R_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x25b0 p0r_bnr_dc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_detail_max                   : 8;   //  0: 7
	UINT32 dc_detail_min                   : 8;   //  8:15
	UINT32 var_chroma_en                   : 1;   //    16
	UINT32 dc_variance_y_axis_inv          : 1;   //    17
	UINT32 dc_detail_y_axis_inv            : 1;   //    18
	UINT32 dc_motion_y_axis_inv            : 1;   //    19
	UINT32 var_v_gain                      : 4;   // 20:23
	UINT32 var_h_gain                      : 4;   // 24:27
	UINT32 var_cut_resolution              : 4;   // 28:31
} H14A0_P0R_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x25b4 p0r_bnr_ac_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_pos_gain_l2              : 8;   //  0: 7
	UINT32 bnr_ac_pos_gain_l1              : 8;   //  8:15
	UINT32 bnr_ac_pos_gain_l0              : 8;   // 16:23
	UINT32 bnr_ac_pos_gain_h3              : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x25b8 p0r_bnr_ac_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_filter_coef5_2           : 4;   //  0: 3
	UINT32 bnr_ac_filter_coef5_1           : 4;   //  4: 7
	UINT32 bnr_ac_filter_coef5_0           : 4;   //  8:11
	UINT32 bnr_ac_filter_coef3_2           : 4;   // 12:15
	UINT32 bnr_ac_filter_coef3_1           : 4;   // 16:19
	UINT32 bnr_ac_filter_coef3_0           : 4;   // 20:23
	UINT32 bnr_ac_pos_gain_l3              : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x25bc p0r_bnr_ac_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_pos_gain_h2              : 8;   //  0: 7
	UINT32 bnr_ac_pos_gain_h1              : 8;   //  8:15
	UINT32 bnr_ac_pos_gain_h0              : 8;   // 16:23
	UINT32 bnr_ac_detail_gain_th           : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x25c0 p0r_bnr_ac_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_h_en                     : 1;   //     0
	UINT32 bnr_ac_v_en                     : 1;   //     1
	UINT32 bnr_ac_h_chroma_en              : 1;   //     2
	UINT32 bnr_ac_v_chroma_en              : 1;   //     3
	UINT32 bnr_ac_acness_resol_h           : 2;   //  4: 5
	UINT32 bnr_ac_sdhd_sel                 : 1;   //     6
	UINT32                                 : 1;   //     7 reserved
	UINT32 bnr_ac_diff_min_v_th            : 8;   //  8:15
	UINT32 bnr_ac_diff_min_h_th            : 8;   // 16:23
	UINT32 bnr_ac_alpha_gain               : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x25c4 p0r_bnr_ac_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_global_motion_th         : 8;   //  0: 7
	UINT32 bnr_ac_acness_max               : 8;   //  8:15
	UINT32 bnr_ac_acness_min               : 8;   // 16:23
	UINT32 bnr_ac_bin_th                   : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x25c8 p0r_bnr_ac_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_motion_max_offset_h2     : 8;   //  0: 7
	UINT32 bnr_ac_motion_min_offset_h2     : 8;   //  8:15
	UINT32 bnr_ac_motion_max_offset_h1     : 8;   // 16:23
	UINT32 bnr_ac_motion_min_offset_h1     : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x25cc p0r_bnr_ac_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_motion_acness_th1        : 8;   //  0: 7
	UINT32 bnr_ac_motion_acness_th0        : 8;   //  8:15
	UINT32 bnr_ac_motion_max_offset_h3     : 8;   // 16:23
	UINT32 bnr_ac_motion_min_offset_h3     : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x25d0 p0r_bnr_ac_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_v_init_offset            : 3;   //  0: 2
	UINT32 bnr_ac_h_init_offset            : 3;   //  3: 5
	UINT32 bnr_ac_acness_resol_v           : 2;   //  6: 7
	UINT32 bnr_ac_debug_mode               : 4;   //  8:11
	UINT32 bnr_ac_hsy_mode                 : 4;   // 12:15
	UINT32 bnr_ac_motion_max               : 8;   // 16:23
	UINT32 bnr_ac_motion_min               : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x25d4 p0r_bnr_ac_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnr_ac_detail_th4               : 8;   //  0: 7
	UINT32 bnr_ac_detail_th3               : 8;   //  8:15
	UINT32 bnr_ac_detail_th2               : 8;   // 16:23
	UINT32 bnr_ac_detail_th1               : 8;   // 24:31
} H14A0_P0R_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x25d8 p0r_dnr_max_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_ac_bnr                   : 1;   //     0
	UINT32 enable_mnr                      : 1;   //     1
	UINT32 enable_dc_bnr                   : 1;   //     2
	UINT32                                 : 5;   //  3: 7 reserved
	UINT32 debug_enable                    : 1;   //     8
	UINT32 debug_mode                      : 1;   //     9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 win_control_enable              : 1;   //    16
	UINT32 border_enable                   : 1;   //    17
	UINT32 win_inout                       : 1;   //    18
	UINT32 motion_blur_sel                 : 1;   //    19
	UINT32 motion_bit_ctrl                 : 3;   // 20:22
} H14A0_P0R_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
	0x25dc p0r_dnr_dbar_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                    : 1;   //     0
	UINT32                                 : 1;   //     1 reserved
	UINT32 show_debug_bar_for_bnr          : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 gain                            : 3;   //  4: 6
	UINT32                                 : 1;   //     7 reserved
	UINT32 sd_hd_sel                       : 2;   //  8: 9
} H14A0_P0R_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
	0x25e0 p0r_bnr_stat_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                 : 8;   //  0: 7
	UINT32 ac_bnr_v_status                 : 8;   //  8:15
	UINT32 dc_bnr_th                       : 14;  // 16:29
} H14A0_P0R_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
	0x25e4 p0r_cti_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                          : 1;   //     0
	UINT32 cti_clipping_mode               : 1;   //     1
	UINT32 cti_pre_flt_mode                : 2;   //  2: 3
	UINT32 cti_tap                         : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 cti_gain                        : 8;   //  8:15
	UINT32 cti_debug_mode                  : 3;   // 16:18
} H14A0_P0R_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x25f0 p0r_ipc_ctrl_20 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cubic_coef0                     : 8;   //  0: 7
	UINT32 cubic_coef1                     : 8;   //  8:15
	UINT32 va_blending_ctrl_lv1            : 3;   // 16:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 va_blending_ctrl_lv2            : 3;   // 20:22
	UINT32                                 : 1;   //    23 reserved
	UINT32 va_blending_ctrl_lv3            : 3;   // 24:26
	UINT32                                 : 1;   //    27 reserved
	UINT32 va_blending_ctrl_lv4            : 3;   // 28:30
} H14A0_P0R_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
	0x25f4 p0r_ipc_ctrl_21 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 va_cubic                        : 1;   //     0
	UINT32 boundary_window                 : 1;   //     1
	UINT32 force_va                        : 1;   //     2
	UINT32 half_pel_va                     : 1;   //     3
	UINT32 dir_med_tap                     : 2;   //  4: 5
	UINT32 diff_med_tap                    : 2;   //  6: 7
	UINT32 va_blending_ctrl1               : 3;   //  8:10
	UINT32 wide_angle_protection_1         : 1;   //    11
	UINT32 va_blending_ctrl2               : 3;   // 12:14
	UINT32 wide_angle_protection_2         : 1;   //    15
	UINT32 errs_chroma_blend_coef          : 8;   // 16:23
	UINT32 strong_global_th                : 6;   // 24:29
	UINT32 narrow_angle_week               : 1;   //    30
	UINT32 narrow_angle_protection         : 1;   //    31
} H14A0_P0R_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
	0x25f8 p0r_ipc_ctrl_22 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain            : 8;   //  0: 7
	UINT32 cr_flicker_amp_gain             : 8;   //  8:15
} H14A0_P0R_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
	0x25fc p0r_ipc_ctrl_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                   : 1;   //     0
	UINT32 cr_lock_th                      : 7;   //  1: 7
	UINT32 cr_g_motion_th                  : 8;   //  8:15
	UINT32 cr_hmc_expend                   : 2;   // 16:17
	UINT32 cr_hmc_half_en                  : 1;   //    18
	UINT32                                 : 1;   //    19 reserved
	UINT32 cr_robustness                   : 4;   // 20:23
	UINT32 cr_center_weight                : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
	0x2600 p0r_ipc_ctrl_24 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                     : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 cr_noise_th                     : 8;   //  8:15
	UINT32 cr_gds_resolution               : 3;   // 16:18
} H14A0_P0R_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
	0x2604 p0r_ipc_ctrl_25 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vdiff_en                        : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 vdiff_th                        : 8;   //  8:15
	UINT32 hmc_err_max                     : 8;   // 16:23
	UINT32 hmc_err_min                     : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
	0x2608 p0r_ipc_ctrl_26 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                 : 1;   //     0
	UINT32                                 : 7;   //  1: 7 reserved
	UINT32 cr_fmd_ath                      : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 cr_fmd_ratio_th                 : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
	0x260c p0r_ipc_ctrl_27 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0           : 8;   //  0: 7
	UINT32 chroma_st_flt_ctrl_x1           : 8;   //  8:15
	UINT32 luma_st_flt_ctrl_x0             : 8;   // 16:23
	UINT32 luma_st_flt_ctrl_x1             : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
	0x2610 p0r_ipc_ctrl_28 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0r_ipc_ctrl_28_f               : 32;   // 31: 0
} H14A0_P0R_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
	0x2614 p0r_ipc_ctrl_29 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 hmc_flt_ctrl_x0                 : 8;   // 16:23
	UINT32 hmc_st_flt_ctrl_x1              : 8;   // 24:31
} H14A0_P0R_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
	0x2618 p0r_ipc_ctrl_30 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vmc_motion_th                   : 8;   //  0: 7
	UINT32 vmc_debug                       : 1;   //     8
	UINT32 vmc_en                          : 1;   //     9
	UINT32 vmc_still_check                 : 1;   //    10
	UINT32 vmc_mul                         : 3;   // 11:13
	UINT32 vmc_out_mul                     : 2;   // 14:15
	UINT32 vmc_protect_th                  : 8;   // 16:23
	UINT32 protection_en                   : 1;   //    24
} H14A0_P0R_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
	0x261c p0r_ipc_ctrl_31 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 toosmall_en                     : 1;   //     0
	UINT32 toosmall_min                    : 7;   //  1: 7
	UINT32 toosmall_max                    : 8;   //  8:15
	UINT32 pndiff_disable                  : 1;   //    16
} H14A0_P0R_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
	0x2628 p0r_ipc_status_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_lr_count                   : 16;  //  0:15
	UINT32 r_pc_lr_count                   : 16;  // 16:31
} H14A0_P0R_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
	0x262c p0r_ipc_status_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_lr_same                 : 16;  //  0:15
	UINT32 r_pndiff_count                  : 16;  // 16:31
} H14A0_P0R_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
	0x2630 p0r_ipc_status_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_lr_same                 : 16;  //  0:15
	UINT32 r_pc_pn_lr_same                 : 16;  // 16:31
} H14A0_P0R_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
	0x2634 p0r_ipc_status_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt              : 16;  //  0:15
	UINT32 r_inv_original_cnt              : 16;  // 16:31
} H14A0_P0R_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
	0x2638 p0r_ipc_status_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0r_ipc_status_08_f             : 32;   // 31: 0
} H14A0_P0R_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
	0x263c p0r_ipc_status_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0r_ipc_status_09_f             : 32;   // 31: 0
} H14A0_P0R_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
	0x2640 p0r_tpd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_scale                      : 3;   //  0: 2
	UINT32 bbd_show                        : 1;   //     3
	UINT32 show_bar_1                      : 1;   //     4
	UINT32 show_bar_2                      : 1;   //     5
	UINT32 show_bar_3                      : 1;   //     6
	UINT32 bar1_data_mux                   : 2;   //  7: 8
	UINT32 bar2_data_mux                   : 2;   //  9:10
	UINT32 bar3_data_mux                   : 2;   // 11:12
	UINT32 hist0_data_mux                  : 4;   // 13:16
	UINT32 hist1_data_mux                  : 4;   // 17:20
	UINT32 hist2_data_mux                  : 4;   // 21:24
	UINT32 hist0_win_sel                   : 2;   // 25:26
	UINT32 hist1_win_sel                   : 2;   // 27:28
	UINT32 hist2_win_sel                   : 2;   // 29:30
	UINT32 hist_rd_en                      : 1;   //    31
} H14A0_P0R_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2644 p0r_tpd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                         : 12;  //  0:11
	UINT32                                 : 3;   // 12:14 reserved
	UINT32 wina_hen                        : 1;   //    15
	UINT32 wina_y0                         : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 wina_ven                        : 1;   //    31
} H14A0_P0R_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2648 p0r_tpd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                         : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 wina_y1                         : 12;  // 16:27
	UINT32                                 : 2;   // 28:29 reserved
	UINT32 hist_hsv_hsl_sel                : 1;   //    30
	UINT32 hist_vscaling_en                : 1;   //    31
} H14A0_P0R_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x264c p0r_tpd_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                         : 12;  //  0:11
	UINT32                                 : 3;   // 12:14 reserved
	UINT32 winb_hen                        : 1;   //    15
	UINT32 winb_y0                         : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 winb_ven                        : 1;   //    31
} H14A0_P0R_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2650 p0r_tpd_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                         : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 winb_y1                         : 12;  // 16:27
} H14A0_P0R_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x2654 p0r_tpd_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x0                         : 12;  //  0:11
	UINT32                                 : 3;   // 12:14 reserved
	UINT32 winc_hen                        : 1;   //    15
	UINT32 winc_y0                         : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 winc_ven                        : 1;   //    31
} H14A0_P0R_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x2658 p0r_tpd_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x1                         : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 winc_y1                         : 12;  // 16:27
} H14A0_P0R_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x265c p0r_tpd_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_y                         : 10;  //  0: 9
	UINT32 force_y_max_th                  : 10;  // 10:19
	UINT32 force_y_min_th                  : 10;  // 20:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 force_y_enable                  : 1;   //    31
} H14A0_P0R_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x2660 p0r_tpd_ctrl_08 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_cb                        : 10;  //  0: 9
	UINT32 force_cb_max_th                 : 10;  // 10:19
	UINT32 force_cb_min_th                 : 10;  // 20:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 force_cb_enable                 : 1;   //    31
} H14A0_P0R_TPD_CTRL_08_T;

/*-----------------------------------------------------------------------------
	0x2664 p0r_tpd_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_cr                        : 10;  //  0: 9
	UINT32 force_cr_max_th                 : 10;  // 10:19
	UINT32 force_cr_min_th                 : 10;  // 20:29
	UINT32                                 : 1;   //    30 reserved
	UINT32 force_cr_enable                 : 1;   //    31
} H14A0_P0R_TPD_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x2668 p0r_tpd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                : 22;  //  0:21
} H14A0_P0R_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x2670 p0r_bbd_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_en                          : 1;   //     0
	UINT32 hys_mode                        : 5;   //  1: 5
	UINT32 op_mode                         : 1;   //     6
	UINT32                                 : 1;   //     7 reserved
	UINT32 cnt_th                          : 8;   //  8:15
	UINT32 diff_th                         : 8;   // 16:23
	UINT32 bbd_mux                         : 3;   // 24:26
	UINT32 apl_mux                         : 3;   // 27:29
	UINT32 apl_win_sel                     : 2;   // 30:31
} H14A0_P0R_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x2674 p0r_bbd_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y0                              : 12;  // 16:27
} H14A0_P0R_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x2678 p0r_bbd_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0R_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x267c p0r_bbd_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y0                              : 12;  // 16:27
} H14A0_P0R_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x2680 p0r_bbd_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                              : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y1                              : 12;  // 16:27
} H14A0_P0R_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x2684 p0r_apl_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                       : 10;  //  0: 9
	UINT32 cb_b_s_apl                      : 10;  // 10:19
	UINT32 cr_r_s_apl                      : 10;  // 20:29
} H14A0_P0R_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x2688 p0r_bnr_dc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_var_en                       : 1;   //     0
	UINT32 dc_motion_en                    : 1;   //     1
	UINT32 dc_protection_en                : 1;   //     2
	UINT32 dc_detail_en                    : 1;   //     3
	UINT32 dc_global_motion_th             : 4;   //  4: 7
	UINT32                                 : 2;   //  8: 9 reserved
	UINT32 dc_bnr_mastergain               : 6;   // 10:15
	UINT32 dc_bnr_chromagain               : 8;   // 16:23
	UINT32 dc_protection_th                : 8;   // 24:31
} H14A0_P0R_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x268c p0r_bnr_dc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dc_bnr_var_th3                  : 8;   //  0: 7
	UINT32 dc_bnr_var_th2                  : 8;   //  8:15
	UINT32 dc_bnr_var_th1                  : 8;   // 16:23
	UINT32 dc_bnr_var_th0                  : 8;   // 24:31
} H14A0_P0R_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x2690 cti_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_enable                      : 1;   //     0
	UINT32 cti_repeat_en                   : 1;   //     1
	UINT32 cti_clipping_en                 : 1;   //     2
	UINT32                                 : 13;  //  3:15 reserved
	UINT32 cti_gain                        : 8;   // 16:23
} H14A0_CTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x269c p0r_bnr_ac_ctrl_09 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_scale                      : 3;   //  0: 2
	UINT32 acness_or_gain_sel              : 1;   //     3
	UINT32 show_bar                        : 4;   //  4: 7
	UINT32 final_ac_bnr_gain_debug_h_en    : 1;   //     8
	UINT32 final_ac_bnr_gain_debug_v_en    : 1;   //     9
	UINT32 ac_manual_gain                  : 6;   // 10:15
	UINT32 mux_1                           : 4;   // 16:19
	UINT32 mux_2                           : 4;   // 20:23
	UINT32 mux_3                           : 4;   // 24:27
	UINT32 mux_4                           : 4;   // 28:31
} H14A0_P0R_BNR_AC_CTRL_09_T;

/*-----------------------------------------------------------------------------
	0x26d0 p0r_tnr_ctrl_38 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_coring_en_c                   : 1;   //     0
	UINT32 m_coring_en_y                   : 1;   //     1
	UINT32 mc_sad_sel                      : 1;   //     2
	UINT32 sad_protect_en                  : 1;   //     3
	UINT32 m_debug_mode                    : 3;   //  4: 6
	UINT32 m_debug_mode_en                 : 1;   //     7
	UINT32 m_gain_c                        : 4;   //  8:11
	UINT32 m_gain_y                        : 4;   // 12:15
	UINT32 m_coring_th                     : 6;   // 16:21
	UINT32 sad_protect_gm_en               : 1;   //    22
	UINT32                                 : 1;   //    23 reserved
	UINT32 mv_cost_gmv_smooth_gain         : 5;   // 24:28
	UINT32                                 : 2;   // 29:30 reserved
	UINT32 mv_cost_gmv_smooth_en           : 1;   //    31
} H14A0_P0R_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
	0x26d4 p0r_tnr_ctrl_39 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 secam_mot_c_th                  : 8;   //  0: 7
	UINT32 secam_mot_y_th                  : 8;   //  8:15
	UINT32 secam_c_v_th                    : 8;   // 16:23
	UINT32                                 : 6;   // 24:29 reserved
	UINT32 secam_c_v_calc_mode             : 2;   // 30:31
} H14A0_P0R_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
	0x26d8 p0r_tnr_ctrl_40 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_en                           : 1;   //     0
	UINT32 sf_map_flt_en                   : 1;   //     1
	UINT32 sf_debug_mode                   : 3;   //  2: 4
	UINT32 sf_map_tap                      : 2;   //  5: 6
	UINT32                                 : 1;   //     7 reserved
	UINT32 sf_map_gain                     : 6;   //  8:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 sf_th0                          : 8;   // 16:23
	UINT32 sf_th1                          : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
	0x26dc p0r_tnr_ctrl_41 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_th2                          : 8;   //  0: 7
	UINT32 sf_th3                          : 8;   //  8:15
	UINT32 sf_th4                          : 8;   // 16:23
	UINT32 sf_f0_sel                       : 2;   // 24:25
	UINT32 sf_f1_sel                       : 2;   // 26:27
	UINT32 sf_f2_sel                       : 2;   // 28:29
	UINT32 sf_f3_sel                       : 2;   // 30:31
} H14A0_P0R_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
	0x26e0 p0r_tnr_ctrl_42 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y0                        : 8;   //  0: 7
	UINT32 sf_mb_x0                        : 8;   //  8:15
	UINT32 sf_mb_y1                        : 8;   // 16:23
	UINT32 sf_mb_x1                        : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
	0x26e4 p0r_tnr_ctrl_43 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 p0r_tnr_ctrl_43_f               : 32;   // 31: 0
} H14A0_P0R_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
	0x26e8 p0r_tnr_ctrl_44 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y2                        : 8;   //  0: 7
	UINT32 sf_mb_x2                        : 8;   //  8:15
	UINT32 sf_mb_y3                        : 8;   // 16:23
	UINT32 sf_mb_x3                        : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
	0x26ec p0r_tnr_ctrl_45 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 16;  //  0:15 reserved
	UINT32 sf_debug_map_sel                : 2;   // 16:17
	UINT32 sf_gain_mode                    : 1;   //    18
	UINT32                                 : 1;   //    19 reserved
	UINT32 sf_gain                         : 4;   // 20:23
} H14A0_P0R_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
	0x26f0 p0r_tnr_ctrl_46 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_blend_y0                   : 8;   //  0: 7
	UINT32 mamc_blend_x0                   : 8;   //  8:15
	UINT32 mamc_blend_y1                   : 8;   // 16:23
	UINT32 mamc_blend_x1                   : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
	0x26f4 p0r_tnr_ctrl_47 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_lock_th0                    : 6;   //  0: 5
	UINT32 gmv_max_cnt_th                  : 2;   //  6: 7
	UINT32 gmv_lock_th1                    : 6;   //  8:13
	UINT32 gmv_val_cnt_th                  : 2;   // 14:15
	UINT32 gmv_manual_mv_en                : 1;   //    16
	UINT32 gmv_manual_alpha_en             : 1;   //    17
	UINT32                                 : 1;   //    18 reserved
	UINT32 gmv_lock_en                     : 1;   //    19
	UINT32 gmv_lock_max                    : 4;   // 20:23
	UINT32 gmv_manual_mv                   : 4;   // 24:27
	UINT32 gmv_manual_alpha                : 4;   // 28:31
} H14A0_P0R_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
	0x26f8 p0r_tnr_ctrl_48 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_gain_ctrl2_y0                 : 8;   //  0: 7
	UINT32 m_gain_ctrl2_x0                 : 8;   //  8:15
	UINT32 m_gain_ctrl2_y1                 : 8;   // 16:23
	UINT32 m_gain_ctrl2_x1                 : 8;   // 24:31
} H14A0_P0R_TNR_CTRL_48_T;

typedef struct {
	H14A0_P0L_PE0_LOAD_T            p0l_pe0_load;                    // 0x0400
	H14A0_P0L_PE0_INTR_T            p0l_pe0_intr;                    // 0x0404
	H14A0_P0L_PE0_STATUS0_T         p0l_pe0_status0;                 // 0x0408
	H14A0_P0L_PE0_STATUS1_T         p0l_pe0_status1;                 // 0x040c
	H14A0_P0L_PE0_OPMODE_T          p0l_pe0_opmode;                  // 0x0410
	H14A0_P0L_PE0_SRC_SIZE_T        p0l_pe0_src_size;                // 0x0414
	H14A0_P0L_PE0_FIELD_T           p0l_pe0_field;                   // 0x0418
	H14A0_P0L_PE0_PARAM_BY_DDR_T    p0l_pe0_param_by_ddr;            // 0x041c
	H14A0_P0L_WIN_CTRL_0_T          p0l_win_ctrl_0;                  // 0x0420
	H14A0_P0L_WIN_CTRL_1_T          p0l_win_ctrl_1;                  // 0x0424
	H14A0_P0L_WIN_CTRL_2_T          p0l_win_ctrl_2;                  // 0x0428
	H14A0_P0L_WIN_CTRL_3_T          p0l_win_ctrl_3;                  // 0x042c
	H14A0_P0L_PE0_DEBUG_MEM_REQ_T   p0l_pe0_debug_mem_req;           // 0x0430
	H14A0_P0L_PE0_DEBUG_LREQ_DLY_T  p0l_pe0_debug_lreq_dly;          // 0x0434
	UINT32                          reserved0[2];                    // 0x0438~0x043c
	H14A0_P0L_TNR_CTRL_00_T         p0l_tnr_ctrl_00;                 // 0x0440
	H14A0_P0L_TNR_CTRL_01_T         p0l_tnr_ctrl_01;                 // 0x0444
	H14A0_P0L_TNR_CTRL_02_T         p0l_tnr_ctrl_02;                 // 0x0448
	H14A0_P0L_TNR_CTRL_03_T         p0l_tnr_ctrl_03;                 // 0x044c
	H14A0_P0L_TNR_CTRL_04_T         p0l_tnr_ctrl_04;                 // 0x0450
	H14A0_P0L_TNR_CTRL_05_T         p0l_tnr_ctrl_05;                 // 0x0454
	H14A0_P0L_TNR_CTRL_06_T         p0l_tnr_ctrl_06;                 // 0x0458
	H14A0_P0L_TNR_CTRL_07_T         p0l_tnr_ctrl_07;                 // 0x045c
	H14A0_P0L_TNR_CTRL_08_T         p0l_tnr_ctrl_08;                 // 0x0460
	H14A0_P0L_TNR_CTRL_09_T         p0l_tnr_ctrl_09;                 // 0x0464
	H14A0_P0L_TNR_CTRL_10_T         p0l_tnr_ctrl_10;                 // 0x0468
	H14A0_P0L_TNR_CTRL_11_T         p0l_tnr_ctrl_11;                 // 0x046c
	H14A0_P0L_TNR_CTRL_12_T         p0l_tnr_ctrl_12;                 // 0x0470
	H14A0_P0L_TNR_CTRL_13_T         p0l_tnr_ctrl_13;                 // 0x0474
	H14A0_P0L_TNR_CTRL_14_T         p0l_tnr_ctrl_14;                 // 0x0478
	H14A0_P0L_TNR_CTRL_15_T         p0l_tnr_ctrl_15;                 // 0x047c
	H14A0_P0L_TNR_CTRL_16_T         p0l_tnr_ctrl_16;                 // 0x0480
	H14A0_P0L_TNR_CTRL_17_T         p0l_tnr_ctrl_17;                 // 0x0484
	H14A0_P0L_TNR_CTRL_18_T         p0l_tnr_ctrl_18;                 // 0x0488
	H14A0_P0L_TNR_CTRL_19_T         p0l_tnr_ctrl_19;                 // 0x048c
	H14A0_P0L_TNR_CTRL_20_T         p0l_tnr_ctrl_20;                 // 0x0490
	H14A0_P0L_TNR_CTRL_21_T         p0l_tnr_ctrl_21;                 // 0x0494
	H14A0_P0L_TNR_CTRL_22_T         p0l_tnr_ctrl_22;                 // 0x0498
	H14A0_P0L_TNR_CTRL_23_T         p0l_tnr_ctrl_23;                 // 0x049c
	H14A0_P0L_TNR_CTRL_24_T         p0l_tnr_ctrl_24;                 // 0x04a0
	H14A0_P0L_TNR_CTRL_25_T         p0l_tnr_ctrl_25;                 // 0x04a4
	H14A0_P0L_TNR_CTRL_26_T         p0l_tnr_ctrl_26;                 // 0x04a8
	H14A0_P0L_TNR_CTRL_27_T         p0l_tnr_ctrl_27;                 // 0x04ac
	H14A0_P0L_TNR_CTRL_28_T         p0l_tnr_ctrl_28;                 // 0x04b0
	H14A0_P0L_TNR_CTRL_29_T         p0l_tnr_ctrl_29;                 // 0x04b4
	H14A0_P0L_TNR_CTRL_30_T         p0l_tnr_ctrl_30;                 // 0x04b8
	H14A0_P0L_TNR_CTRL_31_T         p0l_tnr_ctrl_31;                 // 0x04bc
	H14A0_P0L_TNR_STATUS_00_T       p0l_tnr_status_00;               // 0x04c0
	H14A0_P0L_TNR_STATUS_01_T       p0l_tnr_status_01;               // 0x04c4
	H14A0_P0L_TNR_STATUS_02_T       p0l_tnr_status_02;               // 0x04c8
	H14A0_P0L_TNR_STATUS_03_T       p0l_tnr_status_03;               // 0x04cc
	H14A0_P0L_TNR_MAIN_LUT_00_T     p0l_tnr_main_lut_00;             // 0x04d0
	H14A0_P0L_TNR_MAIN_LUT_01_T     p0l_tnr_main_lut_01;             // 0x04d4
	H14A0_P0L_TNR_MAIN_LUT_02_T     p0l_tnr_main_lut_02;             // 0x04d8
	UINT32                          reserved1;                       // 0x04dc
	H14A0_P0L_TNR_CTRL_32_T         p0l_tnr_ctrl_32;                 // 0x04e0
	H14A0_P0L_TNR_CTRL_33_T         p0l_tnr_ctrl_33;                 // 0x04e4
	H14A0_P0L_TNR_CTRL_34_T         p0l_tnr_ctrl_34;                 // 0x04e8
	H14A0_P0L_TNR_CTRL_35_T         p0l_tnr_ctrl_35;                 // 0x04ec
	H14A0_P0L_TNR_CRG_CTRL_00_T     p0l_tnr_crg_ctrl_00;             // 0x04f0
	H14A0_P0L_TNR_CRG_CTRL_01_T     p0l_tnr_crg_ctrl_01;             // 0x04f4
	H14A0_P0L_TNR_CTRL_36_T         p0l_tnr_ctrl_36;                 // 0x04f8
	H14A0_P0L_TNR_CTRL_37_T         p0l_tnr_ctrl_37;                 // 0x04fc
	H14A0_P0L_IPC_CTRL_00_T         p0l_ipc_ctrl_00;                 // 0x0500
	H14A0_P0L_IPC_CTRL_01_T         p0l_ipc_ctrl_01;                 // 0x0504
	H14A0_P0L_IPC_CTRL_02_T         p0l_ipc_ctrl_02;                 // 0x0508
	H14A0_P0L_IPC_CTRL_03_T         p0l_ipc_ctrl_03;                 // 0x050c
	H14A0_P0L_IPC_CTRL_04_T         p0l_ipc_ctrl_04;                 // 0x0510
	H14A0_P0L_IPC_CTRL_05_T         p0l_ipc_ctrl_05;                 // 0x0514
	H14A0_P0L_IPC_CTRL_06_T         p0l_ipc_ctrl_06;                 // 0x0518
	H14A0_P0L_IPC_CTRL_07_T         p0l_ipc_ctrl_07;                 // 0x051c
	H14A0_P0L_IPC_CTRL_08_T         p0l_ipc_ctrl_08;                 // 0x0520
	H14A0_P0L_IPC_CTRL_09_T         p0l_ipc_ctrl_09;                 // 0x0524
	H14A0_P0L_IPC_CTRL_10_T         p0l_ipc_ctrl_10;                 // 0x0528
	H14A0_P0L_IPC_CTRL_11_T         p0l_ipc_ctrl_11;                 // 0x052c
	H14A0_P0L_IPC_CTRL_12_T         p0l_ipc_ctrl_12;                 // 0x0530
	H14A0_P0L_IPC_CTRL_13_T         p0l_ipc_ctrl_13;                 // 0x0534
	H14A0_P0L_IPC_CTRL_14_T         p0l_ipc_ctrl_14;                 // 0x0538
	H14A0_P0L_IPC_CTRL_15_T         p0l_ipc_ctrl_15;                 // 0x053c
	H14A0_P0L_IPC_CTRL_16_T         p0l_ipc_ctrl_16;                 // 0x0540
	H14A0_P0L_IPC_CTRL_17_T         p0l_ipc_ctrl_17;                 // 0x0544
	H14A0_P0L_IPC_CTRL_18_T         p0l_ipc_ctrl_18;                 // 0x0548
	H14A0_P0L_IPC_CTRL_19_T         p0l_ipc_ctrl_19;                 // 0x054c
	H14A0_P0L_IPC_STATUS_00_T       p0l_ipc_status_00;               // 0x0550
	H14A0_P0L_IPC_STATUS_01_T       p0l_ipc_status_01;               // 0x0554
	H14A0_P0L_IPC_STATUS_02_T       p0l_ipc_status_02;               // 0x0558
	H14A0_P0L_IPC_STATUS_03_T       p0l_ipc_status_03;               // 0x055c
	H14A0_CLC_CTRL_00_T             clc_ctrl_00;                     // 0x0560
	H14A0_CLC_CTRL_01_T             clc_ctrl_01;                     // 0x0564
	H14A0_CLC_CTRL_02_T             clc_ctrl_02;                     // 0x0568
	H14A0_CLC_CTRL_03_T             clc_ctrl_03;                     // 0x056c
	H14A0_CLC_CTRL_04_T             clc_ctrl_04;                     // 0x0570
	H14A0_CLC_CTRL_05_T             clc_ctrl_05;                     // 0x0574
	H14A0_CLC_CTRL_06_T             clc_ctrl_06;                     // 0x0578
	H14A0_CLC_STAT_00_T             clc_stat_00;                     // 0x057c
	H14A0_VFLT_CTRL_T               vflt_ctrl;                       // 0x0580
	H14A0_P0L_CTI_CTRL_1_T          p0l_cti_ctrl_1;                  // 0x0584
	H14A0_P0L_MNR_CTRL_00_T         p0l_mnr_ctrl_00;                 // 0x0588
	H14A0_P0L_MNR_CTRL_01_T         p0l_mnr_ctrl_01;                 // 0x058c
	H14A0_P0L_MNR_CTRL_02_T         p0l_mnr_ctrl_02;                 // 0x0590
	H14A0_P0L_MNR_CTRL_03_T         p0l_mnr_ctrl_03;                 // 0x0594
	H14A0_P0L_MNR_CTRL_04_T         p0l_mnr_ctrl_04;                 // 0x0598
	H14A0_P0L_MNR_CTRL_05_T         p0l_mnr_ctrl_05;                 // 0x059c
	H14A0_P0L_BNR_DETAIL_CTRL_T     p0l_bnr_detail_ctrl;             // 0x05a0
	H14A0_P0L_VFILTER_02_T          p0l_vfilter_02;                  // 0x05a4
	H14A0_P0L_VFILTER_01_T          p0l_vfilter_01;                  // 0x05a8
	H14A0_P0L_BNR_DC_CTRL_00_T      p0l_bnr_dc_ctrl_00;              // 0x05ac
	H14A0_P0L_BNR_DC_CTRL_01_T      p0l_bnr_dc_ctrl_01;              // 0x05b0
	H14A0_P0L_BNR_AC_CTRL_07_T      p0l_bnr_ac_ctrl_07;              // 0x05b4
	H14A0_P0L_BNR_AC_CTRL_08_T      p0l_bnr_ac_ctrl_08;              // 0x05b8
	H14A0_P0L_BNR_AC_CTRL_06_T      p0l_bnr_ac_ctrl_06;              // 0x05bc
	H14A0_P0L_BNR_AC_CTRL_00_T      p0l_bnr_ac_ctrl_00;              // 0x05c0
	H14A0_P0L_BNR_AC_CTRL_01_T      p0l_bnr_ac_ctrl_01;              // 0x05c4
	H14A0_P0L_BNR_AC_CTRL_02_T      p0l_bnr_ac_ctrl_02;              // 0x05c8
	H14A0_P0L_BNR_AC_CTRL_03_T      p0l_bnr_ac_ctrl_03;              // 0x05cc
	H14A0_P0L_BNR_AC_CTRL_04_T      p0l_bnr_ac_ctrl_04;              // 0x05d0
	H14A0_P0L_BNR_AC_CTRL_05_T      p0l_bnr_ac_ctrl_05;              // 0x05d4
	H14A0_P0L_DNR_MAX_CTRL_T        p0l_dnr_max_ctrl;                // 0x05d8
	H14A0_P0L_DNR_DBAR_CTRL_T       p0l_dnr_dbar_ctrl;               // 0x05dc
	H14A0_P0L_BNR_STAT_0_T          p0l_bnr_stat_0;                  // 0x05e0
	H14A0_P0L_CTI_CTRL_0_T          p0l_cti_ctrl_0;                  // 0x05e4
	H14A0_C420_CTRL_T               c420_ctrl;                       // 0x05e8
	UINT32                          reserved2;                       // 0x05ec
	H14A0_P0L_IPC_CTRL_20_T         p0l_ipc_ctrl_20;                 // 0x05f0
	H14A0_P0L_IPC_CTRL_21_T         p0l_ipc_ctrl_21;                 // 0x05f4
	H14A0_P0L_IPC_CTRL_22_T         p0l_ipc_ctrl_22;                 // 0x05f8
	H14A0_P0L_IPC_CTRL_23_T         p0l_ipc_ctrl_23;                 // 0x05fc
	H14A0_P0L_IPC_CTRL_24_T         p0l_ipc_ctrl_24;                 // 0x0600
	H14A0_P0L_IPC_CTRL_25_T         p0l_ipc_ctrl_25;                 // 0x0604
	H14A0_P0L_IPC_CTRL_26_T         p0l_ipc_ctrl_26;                 // 0x0608
	H14A0_P0L_IPC_CTRL_27_T         p0l_ipc_ctrl_27;                 // 0x060c
	H14A0_P0L_IPC_CTRL_28_T         p0l_ipc_ctrl_28;                 // 0x0610
	H14A0_P0L_IPC_CTRL_29_T         p0l_ipc_ctrl_29;                 // 0x0614
	H14A0_P0L_IPC_CTRL_30_T         p0l_ipc_ctrl_30;                 // 0x0618
	H14A0_P0L_IPC_CTRL_31_T         p0l_ipc_ctrl_31;                 // 0x061c
	UINT32                          reserved3[2];                    // 0x0620~0x0624
	H14A0_P0L_IPC_STATUS_04_T       p0l_ipc_status_04;               // 0x0628
	H14A0_P0L_IPC_STATUS_05_T       p0l_ipc_status_05;               // 0x062c
	H14A0_P0L_IPC_STATUS_06_T       p0l_ipc_status_06;               // 0x0630
	H14A0_P0L_IPC_STATUS_07_T       p0l_ipc_status_07;               // 0x0634
	H14A0_P0L_IPC_STATUS_08_T       p0l_ipc_status_08;               // 0x0638
	H14A0_P0L_IPC_STATUS_09_T       p0l_ipc_status_09;               // 0x063c
	H14A0_P0L_TPD_CTRL_00_T         p0l_tpd_ctrl_00;                 // 0x0640
	H14A0_P0L_TPD_CTRL_01_T         p0l_tpd_ctrl_01;                 // 0x0644
	H14A0_P0L_TPD_CTRL_02_T         p0l_tpd_ctrl_02;                 // 0x0648
	H14A0_P0L_TPD_CTRL_03_T         p0l_tpd_ctrl_03;                 // 0x064c
	H14A0_P0L_TPD_CTRL_04_T         p0l_tpd_ctrl_04;                 // 0x0650
	H14A0_P0L_TPD_CTRL_05_T         p0l_tpd_ctrl_05;                 // 0x0654
	H14A0_P0L_TPD_CTRL_06_T         p0l_tpd_ctrl_06;                 // 0x0658
	H14A0_P0L_TPD_CTRL_07_T         p0l_tpd_ctrl_07;                 // 0x065c
	H14A0_P0L_TPD_CTRL_08_T         p0l_tpd_ctrl_08;                 // 0x0660
	H14A0_P0L_TPD_CTRL_09_T         p0l_tpd_ctrl_09;                 // 0x0664
	H14A0_P0L_TPD_STAT_00_T         p0l_tpd_stat_00;                 // 0x0668
	H14A0_CLC_CTRL_07_T             clc_ctrl_07;                     // 0x066c
	H14A0_P0L_BBD_CTRL_00_T         p0l_bbd_ctrl_00;                 // 0x0670
	H14A0_P0L_BBD_CTRL_01_T         p0l_bbd_ctrl_01;                 // 0x0674
	H14A0_P0L_BBD_CTRL_02_T         p0l_bbd_ctrl_02;                 // 0x0678
	H14A0_P0L_BBD_STAT_00_T         p0l_bbd_stat_00;                 // 0x067c
	H14A0_P0L_BBD_STAT_01_T         p0l_bbd_stat_01;                 // 0x0680
	H14A0_P0L_APL_STAT_00_T         p0l_apl_stat_00;                 // 0x0684
	H14A0_P0L_BNR_DC_CTRL_02_T      p0l_bnr_dc_ctrl_02;              // 0x0688
	H14A0_P0L_BNR_DC_CTRL_03_T      p0l_bnr_dc_ctrl_03;              // 0x068c
	H14A0_TPD_HIST_AI_CTRL_T        tpd_hist_ai_ctrl;                // 0x0690
	H14A0_TPD_HIST_AI_DATA_T        tpd_hist_ai_data;                // 0x0694
	H14A0_LVCRTL_CTRL_00_T          lvcrtl_ctrl_00;                  // 0x0698
	H14A0_P0L_BNR_AC_CTRL_09_T      p0l_bnr_ac_ctrl_09;              // 0x069c
	H14A0_PE0_3DFD_CTRL_00_T        pe0_3dfd_ctrl_00;                // 0x06a0
	H14A0_PE0_3DFD_CTRL_01_T        pe0_3dfd_ctrl_01;                // 0x06a4
	H14A0_PE0_3DFD_CTRL_02_T        pe0_3dfd_ctrl_02;                // 0x06a8
	H14A0_PE0_3DFD_CTRL_03_T        pe0_3dfd_ctrl_03;                // 0x06ac
	H14A0_PE0_3DFD_STAT_00_T        pe0_3dfd_stat_00;                // 0x06b0
	H14A0_PE0_3DFD_STAT_01_T        pe0_3dfd_stat_01;                // 0x06b4
	H14A0_PE0_3DFD_STAT_02_T        pe0_3dfd_stat_02;                // 0x06b8
	H14A0_PE0_3DFD_STAT_03_T        pe0_3dfd_stat_03;                // 0x06bc
	H14A0_PE0_3DFD_STAT_04_T        pe0_3dfd_stat_04;                // 0x06c0
	H14A0_PE0_3DFD_STAT_05_T        pe0_3dfd_stat_05;                // 0x06c4
	H14A0_PE0_3DFD_STAT_06_T        pe0_3dfd_stat_06;                // 0x06c8
	H14A0_PE0_3DFD_STAT_07_T        pe0_3dfd_stat_07;                // 0x06cc
	H14A0_P0L_TNR_CTRL_38_T         p0l_tnr_ctrl_38;                 // 0x06d0
	H14A0_P0L_TNR_CTRL_39_T         p0l_tnr_ctrl_39;                 // 0x06d4
	H14A0_P0L_TNR_CTRL_40_T         p0l_tnr_ctrl_40;                 // 0x06d8
	H14A0_P0L_TNR_CTRL_41_T         p0l_tnr_ctrl_41;                 // 0x06dc
	H14A0_P0L_TNR_CTRL_42_T         p0l_tnr_ctrl_42;                 // 0x06e0
	H14A0_P0L_TNR_CTRL_43_T         p0l_tnr_ctrl_43;                 // 0x06e4
	H14A0_P0L_TNR_CTRL_44_T         p0l_tnr_ctrl_44;                 // 0x06e8
	H14A0_P0L_TNR_CTRL_45_T         p0l_tnr_ctrl_45;                 // 0x06ec
	H14A0_P0L_TNR_CTRL_46_T         p0l_tnr_ctrl_46;                 // 0x06f0
	H14A0_P0L_TNR_CTRL_47_T         p0l_tnr_ctrl_47;                 // 0x06f4
	H14A0_P0L_TNR_CTRL_48_T         p0l_tnr_ctrl_48;                 // 0x06f8
} DE_P0L_REG_H14A0_T;

typedef struct {
	H14A0_P0R_PE0_LOAD_T            p0r_pe0_load;                    // 0x2400
	H14A0_P0R_PE0_INTR_T            p0r_pe0_intr;                    // 0x2404
	H14A0_P0R_PE0_STATUS0_T         p0r_pe0_status0;                 // 0x2408
	H14A0_P0R_PE0_STATUS1_T         p0r_pe0_status1;                 // 0x240c
	H14A0_P0R_PE0_OPMODE_T          p0r_pe0_opmode;                  // 0x2410
	H14A0_P0R_PE0_SRC_SIZE_T        p0r_pe0_src_size;                // 0x2414
	H14A0_P0R_PE0_FIELD_T           p0r_pe0_field;                   // 0x2418
	H14A0_P0R_PE0_PARAM_BY_DDR_T    p0r_pe0_param_by_ddr;            // 0x241c
	H14A0_P0R_WIN_CTRL_0_T          p0r_win_ctrl_0;                  // 0x2420
	H14A0_P0R_WIN_CTRL_1_T          p0r_win_ctrl_1;                  // 0x2424
	H14A0_P0R_WIN_CTRL_2_T          p0r_win_ctrl_2;                  // 0x2428
	H14A0_P0R_WIN_CTRL_3_T          p0r_win_ctrl_3;                  // 0x242c
	H14A0_P0R_PE0_DEBUG_MEM_REQ_T   p0r_pe0_debug_mem_req;           // 0x2430
	H14A0_P0R_PE0_DEBUG_LREQ_DLY_T  p0r_pe0_debug_lreq_dly;          // 0x2434
	UINT32                          reserved4[2];                    // 0x2438~0x243c
	H14A0_P0R_TNR_CTRL_00_T         p0r_tnr_ctrl_00;                 // 0x2440
	H14A0_P0R_TNR_CTRL_01_T         p0r_tnr_ctrl_01;                 // 0x2444
	H14A0_P0R_TNR_CTRL_02_T         p0r_tnr_ctrl_02;                 // 0x2448
	H14A0_P0R_TNR_CTRL_03_T         p0r_tnr_ctrl_03;                 // 0x244c
	H14A0_P0R_TNR_CTRL_04_T         p0r_tnr_ctrl_04;                 // 0x2450
	H14A0_P0R_TNR_CTRL_05_T         p0r_tnr_ctrl_05;                 // 0x2454
	H14A0_P0R_TNR_CTRL_06_T         p0r_tnr_ctrl_06;                 // 0x2458
	H14A0_P0R_TNR_CTRL_07_T         p0r_tnr_ctrl_07;                 // 0x245c
	H14A0_P0R_TNR_CTRL_08_T         p0r_tnr_ctrl_08;                 // 0x2460
	H14A0_P0R_TNR_CTRL_09_T         p0r_tnr_ctrl_09;                 // 0x2464
	H14A0_P0R_TNR_CTRL_10_T         p0r_tnr_ctrl_10;                 // 0x2468
	H14A0_P0R_TNR_CTRL_11_T         p0r_tnr_ctrl_11;                 // 0x246c
	H14A0_P0R_TNR_CTRL_12_T         p0r_tnr_ctrl_12;                 // 0x2470
	H14A0_P0R_TNR_CTRL_13_T         p0r_tnr_ctrl_13;                 // 0x2474
	H14A0_P0R_TNR_CTRL_14_T         p0r_tnr_ctrl_14;                 // 0x2478
	H14A0_P0R_TNR_CTRL_15_T         p0r_tnr_ctrl_15;                 // 0x247c
	H14A0_P0R_TNR_CTRL_16_T         p0r_tnr_ctrl_16;                 // 0x2480
	H14A0_P0R_TNR_CTRL_17_T         p0r_tnr_ctrl_17;                 // 0x2484
	H14A0_P0R_TNR_CTRL_18_T         p0r_tnr_ctrl_18;                 // 0x2488
	H14A0_P0R_TNR_CTRL_19_T         p0r_tnr_ctrl_19;                 // 0x248c
	H14A0_P0R_TNR_CTRL_20_T         p0r_tnr_ctrl_20;                 // 0x2490
	H14A0_P0R_TNR_CTRL_21_T         p0r_tnr_ctrl_21;                 // 0x2494
	H14A0_P0R_TNR_CTRL_22_T         p0r_tnr_ctrl_22;                 // 0x2498
	H14A0_P0R_TNR_CTRL_23_T         p0r_tnr_ctrl_23;                 // 0x249c
	H14A0_P0R_TNR_CTRL_24_T         p0r_tnr_ctrl_24;                 // 0x24a0
	H14A0_P0R_TNR_CTRL_25_T         p0r_tnr_ctrl_25;                 // 0x24a4
	H14A0_P0R_TNR_CTRL_26_T         p0r_tnr_ctrl_26;                 // 0x24a8
	H14A0_P0R_TNR_CTRL_27_T         p0r_tnr_ctrl_27;                 // 0x24ac
	H14A0_P0R_TNR_CTRL_28_T         p0r_tnr_ctrl_28;                 // 0x24b0
	H14A0_P0R_TNR_CTRL_29_T         p0r_tnr_ctrl_29;                 // 0x24b4
	H14A0_P0R_TNR_CTRL_30_T         p0r_tnr_ctrl_30;                 // 0x24b8
	H14A0_P0R_TNR_CTRL_31_T         p0r_tnr_ctrl_31;                 // 0x24bc
	H14A0_P0R_TNR_STATUS_00_T       p0r_tnr_status_00;               // 0x24c0
	H14A0_P0R_TNR_STATUS_01_T       p0r_tnr_status_01;               // 0x24c4
	H14A0_P0R_TNR_STATUS_02_T       p0r_tnr_status_02;               // 0x24c8
	H14A0_P0R_TNR_STATUS_03_T       p0r_tnr_status_03;               // 0x24cc
	H14A0_P0R_TNR_MAIN_LUT_00_T     p0r_tnr_main_lut_00;             // 0x24d0
	H14A0_P0R_TNR_MAIN_LUT_01_T     p0r_tnr_main_lut_01;             // 0x24d4
	H14A0_P0R_TNR_MAIN_LUT_02_T     p0r_tnr_main_lut_02;             // 0x24d8
	UINT32                          reserved5;                       // 0x24dc
	H14A0_P0R_TNR_CTRL_32_T         p0r_tnr_ctrl_32;                 // 0x24e0
	H14A0_P0R_TNR_CTRL_33_T         p0r_tnr_ctrl_33;                 // 0x24e4
	H14A0_P0R_TNR_CTRL_34_T         p0r_tnr_ctrl_34;                 // 0x24e8
	H14A0_P0R_TNR_CTRL_35_T         p0r_tnr_ctrl_35;                 // 0x24ec
	H14A0_P0R_TNR_CRG_CTRL_00_T     p0r_tnr_crg_ctrl_00;             // 0x24f0
	H14A0_P0R_TNR_CRG_CTRL_01_T     p0r_tnr_crg_ctrl_01;             // 0x24f4
	H14A0_P0R_TNR_CTRL_36_T         p0r_tnr_ctrl_36;                 // 0x24f8
	H14A0_P0R_TNR_CTRL_37_T         p0r_tnr_ctrl_37;                 // 0x24fc
	H14A0_P0R_IPC_CTRL_00_T         p0r_ipc_ctrl_00;                 // 0x2500
	H14A0_P0R_IPC_CTRL_01_T         p0r_ipc_ctrl_01;                 // 0x2504
	H14A0_P0R_IPC_CTRL_02_T         p0r_ipc_ctrl_02;                 // 0x2508
	H14A0_P0R_IPC_CTRL_03_T         p0r_ipc_ctrl_03;                 // 0x250c
	H14A0_P0R_IPC_CTRL_04_T         p0r_ipc_ctrl_04;                 // 0x2510
	H14A0_P0R_IPC_CTRL_05_T         p0r_ipc_ctrl_05;                 // 0x2514
	H14A0_P0R_IPC_CTRL_06_T         p0r_ipc_ctrl_06;                 // 0x2518
	H14A0_P0R_IPC_CTRL_07_T         p0r_ipc_ctrl_07;                 // 0x251c
	H14A0_P0R_IPC_CTRL_08_T         p0r_ipc_ctrl_08;                 // 0x2520
	H14A0_P0R_IPC_CTRL_09_T         p0r_ipc_ctrl_09;                 // 0x2524
	H14A0_P0R_IPC_CTRL_10_T         p0r_ipc_ctrl_10;                 // 0x2528
	H14A0_P0R_IPC_CTRL_11_T         p0r_ipc_ctrl_11;                 // 0x252c
	H14A0_P0R_IPC_CTRL_12_T         p0r_ipc_ctrl_12;                 // 0x2530
	H14A0_P0R_IPC_CTRL_13_T         p0r_ipc_ctrl_13;                 // 0x2534
	H14A0_P0R_IPC_CTRL_14_T         p0r_ipc_ctrl_14;                 // 0x2538
	H14A0_P0R_IPC_CTRL_15_T         p0r_ipc_ctrl_15;                 // 0x253c
	H14A0_P0R_IPC_CTRL_16_T         p0r_ipc_ctrl_16;                 // 0x2540
	H14A0_P0R_IPC_CTRL_17_T         p0r_ipc_ctrl_17;                 // 0x2544
	H14A0_P0R_IPC_CTRL_18_T         p0r_ipc_ctrl_18;                 // 0x2548
	H14A0_P0R_IPC_CTRL_19_T         p0r_ipc_ctrl_19;                 // 0x254c
	H14A0_P0R_IPC_STATUS_00_T       p0r_ipc_status_00;               // 0x2550
	H14A0_P0R_IPC_STATUS_01_T       p0r_ipc_status_01;               // 0x2554
	H14A0_P0R_IPC_STATUS_02_T       p0r_ipc_status_02;               // 0x2558
	H14A0_P0R_IPC_STATUS_03_T       p0r_ipc_status_03;               // 0x255c
	UINT32                          reserved6[8];                    // 0x2560~0x257c
	H14A0_VFILTER_00_T              vfilter_00;                      // 0x2580
	H14A0_P0R_CTI_CTRL_1_T          p0r_cti_ctrl_1;                  // 0x2584
	H14A0_P0R_MNR_CTRL_00_T         p0r_mnr_ctrl_00;                 // 0x2588
	H14A0_P0R_MNR_CTRL_01_T         p0r_mnr_ctrl_01;                 // 0x258c
	H14A0_P0R_MNR_CTRL_02_T         p0r_mnr_ctrl_02;                 // 0x2590
	H14A0_P0R_MNR_CTRL_03_T         p0r_mnr_ctrl_03;                 // 0x2594
	H14A0_P0R_MNR_CTRL_04_T         p0r_mnr_ctrl_04;                 // 0x2598
	H14A0_P0R_MNR_CTRL_05_T         p0r_mnr_ctrl_05;                 // 0x259c
	H14A0_P0R_BNR_DETAIL_CTRL_T     p0r_bnr_detail_ctrl;             // 0x25a0
	H14A0_P0R_VFILTER_02_T          p0r_vfilter_02;                  // 0x25a4
	H14A0_P0R_VFILTER_01_T          p0r_vfilter_01;                  // 0x25a8
	H14A0_P0R_BNR_DC_CTRL_00_T      p0r_bnr_dc_ctrl_00;              // 0x25ac
	H14A0_P0R_BNR_DC_CTRL_01_T      p0r_bnr_dc_ctrl_01;              // 0x25b0
	H14A0_P0R_BNR_AC_CTRL_07_T      p0r_bnr_ac_ctrl_07;              // 0x25b4
	H14A0_P0R_BNR_AC_CTRL_08_T      p0r_bnr_ac_ctrl_08;              // 0x25b8
	H14A0_P0R_BNR_AC_CTRL_06_T      p0r_bnr_ac_ctrl_06;              // 0x25bc
	H14A0_P0R_BNR_AC_CTRL_00_T      p0r_bnr_ac_ctrl_00;              // 0x25c0
	H14A0_P0R_BNR_AC_CTRL_01_T      p0r_bnr_ac_ctrl_01;              // 0x25c4
	H14A0_P0R_BNR_AC_CTRL_02_T      p0r_bnr_ac_ctrl_02;              // 0x25c8
	H14A0_P0R_BNR_AC_CTRL_03_T      p0r_bnr_ac_ctrl_03;              // 0x25cc
	H14A0_P0R_BNR_AC_CTRL_04_T      p0r_bnr_ac_ctrl_04;              // 0x25d0
	H14A0_P0R_BNR_AC_CTRL_05_T      p0r_bnr_ac_ctrl_05;              // 0x25d4
	H14A0_P0R_DNR_MAX_CTRL_T        p0r_dnr_max_ctrl;                // 0x25d8
	H14A0_P0R_DNR_DBAR_CTRL_T       p0r_dnr_dbar_ctrl;               // 0x25dc
	H14A0_P0R_BNR_STAT_0_T          p0r_bnr_stat_0;                  // 0x25e0
	H14A0_P0R_CTI_CTRL_0_T          p0r_cti_ctrl_0;                  // 0x25e4
	UINT32                          reserved7[2];                    // 0x25e8~0x25ec
	H14A0_P0R_IPC_CTRL_20_T         p0r_ipc_ctrl_20;                 // 0x25f0
	H14A0_P0R_IPC_CTRL_21_T         p0r_ipc_ctrl_21;                 // 0x25f4
	H14A0_P0R_IPC_CTRL_22_T         p0r_ipc_ctrl_22;                 // 0x25f8
	H14A0_P0R_IPC_CTRL_23_T         p0r_ipc_ctrl_23;                 // 0x25fc
	H14A0_P0R_IPC_CTRL_24_T         p0r_ipc_ctrl_24;                 // 0x2600
	H14A0_P0R_IPC_CTRL_25_T         p0r_ipc_ctrl_25;                 // 0x2604
	H14A0_P0R_IPC_CTRL_26_T         p0r_ipc_ctrl_26;                 // 0x2608
	H14A0_P0R_IPC_CTRL_27_T         p0r_ipc_ctrl_27;                 // 0x260c
	H14A0_P0R_IPC_CTRL_28_T         p0r_ipc_ctrl_28;                 // 0x2610
	H14A0_P0R_IPC_CTRL_29_T         p0r_ipc_ctrl_29;                 // 0x2614
	H14A0_P0R_IPC_CTRL_30_T         p0r_ipc_ctrl_30;                 // 0x2618
	H14A0_P0R_IPC_CTRL_31_T         p0r_ipc_ctrl_31;                 // 0x261c
	UINT32                          reserved8[2];                    // 0x2620~0x2624
	H14A0_P0R_IPC_STATUS_04_T       p0r_ipc_status_04;               // 0x2628
	H14A0_P0R_IPC_STATUS_05_T       p0r_ipc_status_05;               // 0x262c
	H14A0_P0R_IPC_STATUS_06_T       p0r_ipc_status_06;               // 0x2630
	H14A0_P0R_IPC_STATUS_07_T       p0r_ipc_status_07;               // 0x2634
	H14A0_P0R_IPC_STATUS_08_T       p0r_ipc_status_08;               // 0x2638
	H14A0_P0R_IPC_STATUS_09_T       p0r_ipc_status_09;               // 0x263c
	H14A0_P0R_TPD_CTRL_00_T         p0r_tpd_ctrl_00;                 // 0x2640
	H14A0_P0R_TPD_CTRL_01_T         p0r_tpd_ctrl_01;                 // 0x2644
	H14A0_P0R_TPD_CTRL_02_T         p0r_tpd_ctrl_02;                 // 0x2648
	H14A0_P0R_TPD_CTRL_03_T         p0r_tpd_ctrl_03;                 // 0x264c
	H14A0_P0R_TPD_CTRL_04_T         p0r_tpd_ctrl_04;                 // 0x2650
	H14A0_P0R_TPD_CTRL_05_T         p0r_tpd_ctrl_05;                 // 0x2654
	H14A0_P0R_TPD_CTRL_06_T         p0r_tpd_ctrl_06;                 // 0x2658
	H14A0_P0R_TPD_CTRL_07_T         p0r_tpd_ctrl_07;                 // 0x265c
	H14A0_P0R_TPD_CTRL_08_T         p0r_tpd_ctrl_08;                 // 0x2660
	H14A0_P0R_TPD_CTRL_09_T         p0r_tpd_ctrl_09;                 // 0x2664
	H14A0_P0R_TPD_STAT_00_T         p0r_tpd_stat_00;                 // 0x2668
	UINT32                          reserved9;                       // 0x266c
	H14A0_P0R_BBD_CTRL_00_T         p0r_bbd_ctrl_00;                 // 0x2670
	H14A0_P0R_BBD_CTRL_01_T         p0r_bbd_ctrl_01;                 // 0x2674
	H14A0_P0R_BBD_CTRL_02_T         p0r_bbd_ctrl_02;                 // 0x2678
	H14A0_P0R_BBD_STAT_00_T         p0r_bbd_stat_00;                 // 0x267c
	H14A0_P0R_BBD_STAT_01_T         p0r_bbd_stat_01;                 // 0x2680
	H14A0_P0R_APL_STAT_00_T         p0r_apl_stat_00;                 // 0x2684
	H14A0_P0R_BNR_DC_CTRL_02_T      p0r_bnr_dc_ctrl_02;              // 0x2688
	H14A0_P0R_BNR_DC_CTRL_03_T      p0r_bnr_dc_ctrl_03;              // 0x268c
	H14A0_CTI_CTRL_00_T             cti_ctrl_00;                     // 0x2690
	UINT32                          reserved10[2];                   // 0x2694~0x2698
	H14A0_P0R_BNR_AC_CTRL_09_T      p0r_bnr_ac_ctrl_09;              // 0x269c
	UINT32                          reserved11[12];                  // 0x26a0~0x26cc
	H14A0_P0R_TNR_CTRL_38_T         p0r_tnr_ctrl_38;                 // 0x26d0
	H14A0_P0R_TNR_CTRL_39_T         p0r_tnr_ctrl_39;                 // 0x26d4
	H14A0_P0R_TNR_CTRL_40_T         p0r_tnr_ctrl_40;                 // 0x26d8
	H14A0_P0R_TNR_CTRL_41_T         p0r_tnr_ctrl_41;                 // 0x26dc
	H14A0_P0R_TNR_CTRL_42_T         p0r_tnr_ctrl_42;                 // 0x26e0
	H14A0_P0R_TNR_CTRL_43_T         p0r_tnr_ctrl_43;                 // 0x26e4
	H14A0_P0R_TNR_CTRL_44_T         p0r_tnr_ctrl_44;                 // 0x26e8
	H14A0_P0R_TNR_CTRL_45_T         p0r_tnr_ctrl_45;                 // 0x26ec
	H14A0_P0R_TNR_CTRL_46_T         p0r_tnr_ctrl_46;                 // 0x26f0
	H14A0_P0R_TNR_CTRL_47_T         p0r_tnr_ctrl_47;                 // 0x26f4
	H14A0_P0R_TNR_CTRL_48_T         p0r_tnr_ctrl_48;                 // 0x26f8
} DE_P0R_REG_H14A0_T;

#endif
