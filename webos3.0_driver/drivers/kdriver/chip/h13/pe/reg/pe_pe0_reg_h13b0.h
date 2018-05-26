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

#ifndef _PE_PE0_REG_H13B0_H_
#define _PE_PE0_REG_H13B0_H_

/*-----------------------------------------------------------------------------
		0x400 p0l_pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} PE_H13B0_P0L_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0x404 p0l_pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16     
} PE_H13B0_P0L_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0x408 p0l_pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                       : 4 ;  //  3: 0     
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4     
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8     
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12     
	UINT32 dnr_frame_id                       : 8 ;  // 23:16     
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24     
} PE_H13B0_P0L_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0x40c p0l_pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0     
} PE_H13B0_P0L_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0x410 p0l_pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                           : 1 ;  //     0     
	UINT32 tnr_y_en                           : 1 ;  //     1     
	UINT32 ipc_c_en                           : 1 ;  //     2     
	UINT32 ipc_y_en                           : 1 ;  //     3     
	UINT32 tnr_c_444_as_420                   : 1 ;  //     4     
	UINT32 tnr_y2_en                          : 1 ;  //     5     
	UINT32 force_422_to_420                   : 1 ;  //     6     
	UINT32 ipc0_y_en_prog                     : 1 ;  //     7     
	UINT32 mif_last_mode                      : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 dnr_out_force_44x                  : 1 ;  //    12     
	UINT32 tnr_m_en                           : 1 ;  //    13     
	UINT32 force_444_to_422                   : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 detour_enable                      : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ibuf_en                            : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 dnr_in_lsb_mode                    : 2 ;  // 25:24     
	UINT32 dnr_out_lsb_mode                   : 2 ;  // 27:26     
	UINT32 tnr_in_lsb_mode                    : 2 ;  // 29:28     
} PE_H13B0_P0L_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0x414 p0l_pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0     
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 cs_type                            : 3 ;  // 15:13     
	UINT32 vsize                              : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31     
} PE_H13B0_P0L_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0x418 p0l_pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 secam_line                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12     
} PE_H13B0_P0L_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
		0x41c p0l_pe0_param_by_ddr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 9 ;  //  8: 0     reserved
	UINT32 stride                             : 2 ;  // 10: 9     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 vsize                              : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 endian                             : 1 ;  //    30     
	UINT32 update_by_ddr                      : 1 ;  //    31     
} PE_H13B0_P0L_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
		0x420 p0l_win_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13B0_P0L_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0x424 p0l_win_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0L_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0x428 p0l_win_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32 ac_bnr_feature_cal_mode            : 2 ;  // 29:28     
} PE_H13B0_P0L_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0x42c p0l_win_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0L_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0x430 p0l_pe0_debug_mem_req ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_t1_y                         : 1 ;  //     0     
	UINT32 en_rd_t1_c                         : 1 ;  //     1     
	UINT32 en_rd_t2_y                         : 1 ;  //     2     
	UINT32 en_rd_t2_c                         : 1 ;  //     3     
	UINT32 en_rd_t3_y                         : 1 ;  //     4     
	UINT32 en_rd_t3_c                         : 1 ;  //     5     
	UINT32 en_rd_t4_y                         : 1 ;  //     6     
	UINT32 en_rd_t1_m                         : 1 ;  //     7     
	UINT32 en_wr_tnrw_y                       : 1 ;  //     8     
	UINT32 en_wr_tnrw_c                       : 1 ;  //     9     
	UINT32 en_wr_tnrw_m                       : 1 ;  //    10     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 manual_mem_en                      : 1 ;  //    15     
} PE_H13B0_P0L_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0x434 p0l_pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0     
	UINT32 lreq_dly_c_en                      : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16     
	UINT32 lreq_dly_y_en                      : 1 ;  //    27     
} PE_H13B0_P0L_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0x440 p0l_tnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_enable                         : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 nt_lvl_hsub                        : 1 ;  //     2     
	UINT32 secam_cmode_en                     : 1 ;  //     3     
	UINT32 secam_alpha_scale                  : 2 ;  //  5: 4     
	UINT32 prev_sel_prog                      : 1 ;  //     6     
	UINT32 frame_skip_enable_for_3d           : 1 ;  //     7     
	UINT32 minfo_mode                         : 2 ;  //  9: 8     
	UINT32 minfo_sw                           : 2 ;  // 11:10     
	UINT32 hist_motion_scale                  : 2 ;  // 13:12     
	UINT32 minfo_flt_en_tnr                   : 1 ;  //    14     
	UINT32 minfo_flt_en_ipc                   : 1 ;  //    15     
	UINT32 calc_motion_max_yc                 : 1 ;  //    16     
	UINT32 motion_4f_en                       : 1 ;  //    17     
	UINT32 mv_tflt_en                         : 1 ;  //    18     
	UINT32 mv_sflt_en                         : 1 ;  //    19     
	UINT32 alpha_avg_en                       : 1 ;  //    20     
	UINT32                                    : 1 ;  //    21     reserved
	UINT32 alpha_mapping_y_en                 : 1 ;  //    22     
	UINT32 alpha_mapping_c_en                 : 1 ;  //    23     
	UINT32 nt_lvl_adjust_gm_enable            : 1 ;  //    24     
	UINT32 nt_lvl_adjust_lpct_enable          : 1 ;  //    25     
	UINT32                                    : 1 ;  //    26     reserved
	UINT32 nt_lvl_adjust_avg_ts_enable        : 1 ;  //    27     
	UINT32 nt_lvl_adjust_lpct_sel             : 1 ;  //    28     
	UINT32 mamc_blend_sel                     : 1 ;  //    29     
} PE_H13B0_P0L_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x444 p0l_tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                            : 8 ;  //  7: 0     
	UINT32 y_th_hi                            : 8 ;  // 15: 8     
	UINT32 dnr_motion_scale                   : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 tnr_filter_lsb_ctrl                : 2 ;  // 21:20     
	UINT32 svar_mux_ctrl                      : 2 ;  // 23:22     
	UINT32 nt_lvl_hist_n_pct                  : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x448 p0l_tnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_flat_svar_th                    : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind7                     : 8 ;  // 15: 8     
	UINT32 secam_cmode_flt_en                 : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 secam_cmode_flt_th                 : 4 ;  // 23:20     
	UINT32 m_gain_en_var                      : 1 ;  //    24     
	UINT32 a_gain_en_var                      : 1 ;  //    25     
	UINT32 m_gain_en_lsc                      : 1 ;  //    26     
	UINT32 a_gain_en_lsc                      : 1 ;  //    27     
	UINT32 m_gain_en_var2                     : 1 ;  //    28     
	UINT32 m_offset_mode_en                   : 1 ;  //    29     
} PE_H13B0_P0L_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x44c p0l_tnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                              : 8 ;  //  7: 0     
	UINT32 wa_y1                              : 8 ;  // 15: 8     
	UINT32 wa_x0                              : 8 ;  // 23:16     
	UINT32 wa_x1                              : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x450 p0l_tnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x0                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x1                  : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x454 p0l_tnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                             : 8 ;  //  7: 0     
	UINT32 nt_max                             : 8 ;  // 15: 8     
	UINT32 hist_svar_th                       : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th0               : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x458 p0l_tnr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                          : 8 ;  //  7: 0     
	UINT32 nt_iir_alpha                       : 8 ;  // 15: 8     
	UINT32 y_th_lo2                           : 8 ;  // 23:16     
	UINT32 y_th_hi2                           : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x45c p0l_tnr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind1                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind2                     : 8 ;  // 23:16     
	UINT32 alpha_lut_ind3                     : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x460 p0l_tnr_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind5                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind6                     : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th1               : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0x464 p0l_tnr_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scene_change_enable                : 1 ;  //     0     
	UINT32 scene_change_max_flt_en            : 1 ;  //     1     
	UINT32 scene_change_measure_sel           : 1 ;  //     2     
	UINT32 scene_change_prev_hist_calc_en     : 1 ;  //     3     
	UINT32 scene_change_enforce               : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 scene_change_max_flt_tap           : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 scene_change_scale                 : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 scene_change_th                    : 8 ;  // 23:16     
	UINT32 gmv_svar_th                        : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0x468 p0l_tnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_show_tnr_param                 : 4 ;  //  3: 0     
	UINT32 dbg_status_sum_sel                 : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 dbg_force_wa                       : 1 ;  //     8     
	UINT32 dbg_force_calc_motion              : 1 ;  //     9     
	UINT32 dbg_force_lut_alpha                : 1 ;  //    10     
	UINT32 dbg_force_adj_alpha                : 1 ;  //    11     
	UINT32 dbg_force_final_alpha              : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 ignore_active_window               : 1 ;  //    15     
	UINT32 dbg_tnrw_port_ctrl                 : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 tnr_black_line_en_for_i            : 1 ;  //    20     
	UINT32 tnr_debug_map_en_for_i             : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 dbg_wa                             : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0x46c p0l_tnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                    : 8 ;  //  7: 0     
	UINT32 dbg_alpha                          : 8 ;  // 15: 8     
	UINT32 peak0_x_th                         : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 scene_change_th_mv                 : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0x470 p0l_tnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x2                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x3                  : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0x474 p0l_tnr_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 force_nt_lvl_val                   : 8 ;  // 15: 8     
	UINT32 adjust_nt_lvl                      : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 adjust_nt_lvl_val                  : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0x478 p0l_tnr_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 noise_measure_win_en               : 1 ;  //    31     
} PE_H13B0_P0L_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0x47c p0l_tnr_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0L_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0x480 p0l_tnr_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_scale                 : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 show_dbg_bar_en                    : 4 ;  //  7: 4     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 show_dbg_bar0_mux                  : 4 ;  // 19:16     
	UINT32 show_dbg_bar1_mux                  : 4 ;  // 23:20     
	UINT32 show_dbg_bar2_mux                  : 4 ;  // 27:24     
	UINT32 show_dbg_bar3_mux                  : 4 ;  // 31:28     
} PE_H13B0_P0L_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0x484 p0l_tnr_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y0                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x0                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y1                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x1                     : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0x488 p0l_tnr_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_enable                   : 1 ;  //     0     
	UINT32 luma_gain_enable                   : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 input_for_luma                     : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 input_for_crgn_satr                : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 y_gain                             : 8 ;  // 23:16     
	UINT32 c_gain                             : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0x48c p0l_tnr_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_x                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_x                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_x                     : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0x490 p0l_tnr_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_y                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_y                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_y                     : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0x494 p0l_tnr_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_mamc_blend                     : 8 ;  //  7: 0     
} PE_H13B0_P0L_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0x498 p0l_tnr_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_protect_control_y0              : 8 ;  //  7: 0     
	UINT32 mv_protect_control_x0              : 8 ;  // 15: 8     
	UINT32 mv_protect_control_y1              : 8 ;  // 23:16     
	UINT32 mv_protect_control_x1              : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0x49c p0l_tnr_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x1                 : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0x4a0 p0l_tnr_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x3                 : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0x4a4 p0l_tnr_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x1                 : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0x4a8 p0l_tnr_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x3                 : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0x4ac p0l_tnr_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                     : 8 ;  //  7: 0     
	UINT32 crgn_gain_rgn1                     : 8 ;  // 15: 8     
	UINT32 crgn_gain_rgn2                     : 8 ;  // 23:16     
	UINT32 crgn_gain_rgn3                     : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0x4b0 p0l_tnr_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x0                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x1                  : 7 ;  // 30:24     
} PE_H13B0_P0L_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0x4b4 p0l_tnr_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                              : 8 ;  //  7: 0     
	UINT32 wa_y3                              : 8 ;  // 15: 8     
	UINT32 wa_x2                              : 8 ;  // 23:16     
	UINT32 wa_x3                              : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0x4b8 p0l_tnr_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x2                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x3                  : 7 ;  // 30:24     
} PE_H13B0_P0L_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0x4bc p0l_tnr_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c               : 1 ;  //     0     
	UINT32 spatial_pre_flt_en_y               : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 spatial_pre_flt_th_c               : 8 ;  // 23:16     
	UINT32 spatial_pre_flt_th_y               : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0x4c0 p0l_tnr_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                            : 8 ;  //  7: 0     
	UINT32 x_peak_t2                          : 8 ;  // 15: 8     
	UINT32 nt_lvl                             : 8 ;  // 23:16     
	UINT32 x_avg_s                            : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0x4c4 p0l_tnr_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sum_valid                          : 16;  // 15: 0     
	UINT32 scene_diff                         : 16;  // 31:16     
} PE_H13B0_P0L_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0x4c8 p0l_tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_l                               : 4 ;  //  3: 0     
	UINT32 mv_r                               : 4 ;  //  7: 4     
	UINT32 dfd                                : 8 ;  // 15: 8     
	UINT32 apl_y                              : 8 ;  // 23:16     
	UINT32 lp                                 : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0x4cc p0l_tnr_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_valid_cnt                      : 16;  // 15: 0     
	UINT32 fd                                 : 8 ;  // 23:16     
	UINT32 gmv_lock_cnt                       : 6 ;  // 29:24     
} PE_H13B0_P0L_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0x4d0 p0l_tnr_main_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 main_lut_ai_enable                 : 1 ;  //    12     
} PE_H13B0_P0L_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
		0x4d4 p0l_tnr_main_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0               : 32;  // 31: 0     
} PE_H13B0_P0L_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
		0x4d8 p0l_tnr_main_lut_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1               : 32;  // 31: 0     
} PE_H13B0_P0L_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
		0x4dc p0l_tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_r_cnt                           : 16;  // 15: 0     
	UINT32 mv_l_cnt                           : 16;  // 31:16     
} PE_H13B0_P0L_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0x4e0 p0l_tnr_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x0                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y1                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x1                : 7 ;  // 30:24     
} PE_H13B0_P0L_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0x4e4 p0l_tnr_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x2                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y3                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x3                : 7 ;  // 30:24     
} PE_H13B0_P0L_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0x4e8 p0l_tnr_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_ctrl_y0                    : 8 ;  //  7: 0     
	UINT32 mc_blur_ctrl_x0                    : 8 ;  // 15: 8     
	UINT32 mc_blur_ctrl_y1                    : 8 ;  // 23:16     
	UINT32 mc_blur_ctrl_x1                    : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0x4ec p0l_tnr_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y2                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x2                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y3                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x3                     : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0x4f0 p0l_tnr_crg_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr               : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 crgn_ctrl_ai_enable                : 1 ;  //    12     
} PE_H13B0_P0L_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x4f4 p0l_tnr_crg_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data               : 32;  // 31: 0     
} PE_H13B0_P0L_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x4f8 p0l_tnr_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_sel_hist                       : 1 ;  //     0     
	UINT32 sad_sel_pels                       : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 hme_half_pel_en                    : 1 ;  //     4     
	UINT32 m_tiir_x_sel                       : 1 ;  //     5     
	UINT32 m_tiir_scene_change_en             : 1 ;  //     6     
	UINT32 m_tiir_en                          : 1 ;  //     7     
	UINT32 sf_blend_en_y                      : 1 ;  //     8     
	UINT32 sf_blend_en_c                      : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 sf_blend_motion_exp_mode           : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 mv0_protect_th                     : 7 ;  // 22:16     
	UINT32 mv0_protect_en                     : 1 ;  //    23     
	UINT32 mv_4f_en                           : 1 ;  //    24     
	UINT32 mv_pfilter_en                      : 1 ;  //    25     
	UINT32 mv_cost_smooth_gain                : 5 ;  // 30:26     
	UINT32 mv_cost_smooth_en                  : 1 ;  //    31     
} PE_H13B0_P0L_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0x4fc p0l_tnr_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_flt_en_c                        : 1 ;  //     0     
	UINT32 mc_flt_mv_inv_c                    : 1 ;  //     1     
	UINT32 mc_flt_mv_debug_en_c               : 1 ;  //     2     
	UINT32 mc_flt_manual_blend_en_c           : 1 ;  //     3     
	UINT32 mc_flt_mv_debug_c                  : 4 ;  //  7: 4     
	UINT32 mc_flt_manual_blend_c              : 6 ;  // 13: 8     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 mc_flt_blur_en_c                   : 1 ;  //    15     
	UINT32 mc_flt_en_y                        : 1 ;  //    16     
	UINT32 mc_flt_mv_inv_y                    : 1 ;  //    17     
	UINT32 mc_flt_mv_debug_en_y               : 1 ;  //    18     
	UINT32 mc_flt_manual_blend_en_y           : 1 ;  //    19     
	UINT32 mc_flt_mv_debug_y                  : 4 ;  // 23:20     
	UINT32 mc_flt_manual_blend_y              : 6 ;  // 29:24     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 mc_flt_blur_en_y                   : 1 ;  //    31     
} PE_H13B0_P0L_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0x500 p0l_ipc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_chroma_mode                     : 2 ;  //  5: 4     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 cr_chroma_edi                      : 1 ;  //     7     
	UINT32 cr_integer_edi                     : 1 ;  //     8     
	UINT32                                    : 15;  // 23: 9     reserved
	UINT32 cr_hmc_force_fmd                   : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 cr_clc_fmd_off                     : 1 ;  //    26     
	UINT32 cr_st_flt_original_en              : 1 ;  //    27     
	UINT32 cr_film_auto_gain                  : 1 ;  //    28     
	UINT32 cr_film_apply_c                    : 1 ;  //    29     
	UINT32 cr_film_apply_y                    : 1 ;  //    30     
	UINT32 cr_film_mode_enable                : 1 ;  //    31     
} PE_H13B0_P0L_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x504 p0l_ipc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 cr_st_flt_enable                   : 1 ;  //    16     
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    17     
	UINT32 cr_hmc_flt_enable                  : 1 ;  //    18     
	UINT32 cr_game_mode                       : 1 ;  //    19     
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 23:20     
	UINT32                                    : 1 ;  //    24     reserved
	UINT32 cr_black_mask                      : 1 ;  //    25     
	UINT32 cr_ipc_debug_show                  : 5 ;  // 30:26     
	UINT32 cr_5f_mode                         : 1 ;  //    31     
} PE_H13B0_P0L_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x508 p0l_ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x50c p0l_ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x510 p0l_ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0     
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8     
	UINT32 tearing_gain                       : 8 ;  // 23:16     
	UINT32 motion_gain                        : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x514 p0l_ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha              : 8 ;  //  7: 0     
	UINT32 cr_t_grad_weight                   : 8 ;  // 15: 8     
	UINT32 cr_v_grad_weight                   : 8 ;  // 23:16     
	UINT32 cr_h_grad_weight                   : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x518 p0l_ipc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                    : 1 ;  //     0     
	UINT32 cr_flat_only                       : 1 ;  //     1     
	UINT32 use_tnr_motion                     : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 cr_tearing_normalize_en            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cr_normalize_base                  : 8 ;  // 15: 8     
	UINT32 cr_normalize_center                : 8 ;  // 23:16     
	UINT32 cr_motion_coring                   : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x51c p0l_ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1     
	UINT32 cr_45_en                           : 1 ;  //     2     
	UINT32 cr_15_en                           : 1 ;  //     3     
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 tnr_motion_gain                    : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x520 p0l_ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30     
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31     
} PE_H13B0_P0L_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0x524 p0l_ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16     
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0x528 p0l_ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16     
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0x52c p0l_ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0     
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8     
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16     
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0x530 p0l_ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0x534 p0l_ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    16     
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 23:19     
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 29:24     
} PE_H13B0_P0L_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0x538 p0l_ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reg_manual_en                      : 1 ;  //     1     
	UINT32 reg_manual_mode                    : 2 ;  //  3: 2     
	UINT32 reg_lock_speed                     : 4 ;  //  7: 4     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0x53c p0l_ipc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_protection_en                  : 1 ;  //     0     
	UINT32 reg_protection_s1en                : 1 ;  //     1     
	UINT32 reg_protection_s2en                : 1 ;  //     2     
	UINT32 reg_protection_s3en                : 1 ;  //     3     
	UINT32 reg_protection_s4en                : 1 ;  //     4     
	UINT32 reg_badedit_protect                : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_modify_ratio_th                : 8 ;  // 15: 8     
	UINT32 fmd_tower_spangain                 : 4 ;  // 19:16     
	UINT32 fmd_bleacherratio                  : 4 ;  // 23:20     
	UINT32 fmd_vlrratiomin                    : 4 ;  // 27:24     
	UINT32 fmd_spxl2ratio                     : 4 ;  // 31:28     
} PE_H13B0_P0L_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0x540 p0l_ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32 reg_same_min_ratio                 : 4 ;  // 27:24     
	UINT32 reg_same_adder_gain                : 4 ;  // 31:28     
} PE_H13B0_P0L_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0x544 p0l_ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0     
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2     
	UINT32 cr_field_inverse                   : 1 ;  //     4     
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0x548 p0l_ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0     
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8     
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16     
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0x54c p0l_ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0     
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8     
	UINT32 reg_2d_offset                      : 8 ;  // 23:16     
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0x550 p0l_ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                       : 16;  // 15: 0     
	UINT32 r_nc_v_count                       : 16;  // 31:16     
} PE_H13B0_P0L_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0x554 p0l_ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                       : 16;  // 15: 0     
	UINT32 r_pn_v_count                       : 16;  // 31:16     
} PE_H13B0_P0L_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0x558 p0l_ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_same                       : 16;  // 15: 0     
	UINT32 r_pc_pn_same                       : 16;  // 31:16     
} PE_H13B0_P0L_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0x55c p0l_ipc_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pn_lr_count                      : 16;  // 15: 0     
	UINT32 same_ng_flag                       : 1 ;  //    16     
	UINT32 s3_protect_en_flag                 : 1 ;  //    17     
	UINT32 s2_protect_en_flag                 : 1 ;  //    18     
	UINT32 s1_protect_en_flag                 : 1 ;  //    19     
	UINT32 modify_pn_ratio_flag               : 1 ;  //    20     
	UINT32 base_pn_ratio_flag                 : 1 ;  //    21     
	UINT32 r_fmd_comb_mode                    : 2 ;  // 23:22     
	UINT32 r_fmd_lock_count                   : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0x560 p0l_clc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cl_filter_enable                   : 1 ;  //     1     
	UINT32 clc_detection_enable               : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 motion_window                      : 2 ;  //  5: 4     
	UINT32 cl_pattern_dist                    : 2 ;  //  7: 6     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 cl_ab_ratio                        : 4 ;  // 15:12     
	UINT32 cl_y_hdistance                     : 2 ;  // 17:16     
	UINT32 cl_c_hdistance                     : 2 ;  // 19:18     
	UINT32 cl_y_protect_en                    : 1 ;  //    20     
	UINT32                                    : 7 ;  // 27:21     reserved
	UINT32 cl_motion_a_filter                 : 1 ;  //    28     
	UINT32 cl_motion_b_filter                 : 1 ;  //    29     
} PE_H13B0_P0L_CLC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x564 p0l_clc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_motion_a_th                     : 8 ;  //  7: 0     
	UINT32 cl_motion_b_th                     : 8 ;  // 15: 8     
	UINT32 cl_y_protect_th0                   : 8 ;  // 23:16     
	UINT32 cl_y_protect_th1                   : 8 ;  // 31:24     
} PE_H13B0_P0L_CLC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x568 p0l_clc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_y_bound_th                      : 7 ;  //  6: 0     
	UINT32 cl_c_bound_use_4line               : 1 ;  //     7     
	UINT32 cl_c_bound_th                      : 7 ;  // 14: 8     
	UINT32 cl_c_bound_type                    : 1 ;  //    15     
	UINT32 cl_y_bound_width_chroma            : 2 ;  // 17:16     
	UINT32 cl_y_bound_ignore_cc               : 1 ;  //    18     
	UINT32 cl_y_bound_ignore_p2               : 1 ;  //    19     
	UINT32 cl_y_bound_ignore_p4               : 1 ;  //    20     
	UINT32 reg_bin3x1_flt_en                  : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_bin5x1_flt_en                  : 1 ;  //    24     
	UINT32 reg_bin5x1_flt_th                  : 3 ;  // 27:25     
	UINT32 reg_vm_original_mode               : 1 ;  //    28     
	UINT32 reg_vm_other_mode                  : 1 ;  //    29     
} PE_H13B0_P0L_CLC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x56c p0l_clc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_sat_min                         : 8 ;  //  7: 0     
	UINT32                                    : 2 ;  //  9: 8     reserved
	UINT32 cl_var_measure_th1                 : 10;  // 19:10     
	UINT32 cl_var_measure_th2                 : 10;  // 29:20     
	UINT32 cl_pt_enable                       : 2 ;  // 31:30     
} PE_H13B0_P0L_CLC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x570 p0l_clc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_tearing_pt_th1                  : 12;  // 11: 0     
	UINT32                                    : 8 ;  // 19:12     reserved
	UINT32 cl_frame_min                       : 12;  // 31:20     
} PE_H13B0_P0L_CLC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x574 p0l_clc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_data_line_switching_sel         : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 clc_bin_flt_th                     : 3 ;  // 10: 8     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 clc_bin_flt_en                     : 1 ;  //    15     
	UINT32 clc_dbg_show_mask                  : 13;  // 28:16     
} PE_H13B0_P0L_CLC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x578 p0l_clc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_c_filter_gain                   : 8 ;  //  7: 0     
	UINT32 cc_y_filter_when_cc_detected       : 1 ;  //     8     
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 cl_cc_original_en                  : 1 ;  //    10     
	UINT32 clc_force_non_fmd                  : 1 ;  //    11     
	UINT32 cl_c_filter_when_cl_detected       : 1 ;  //    12     
	UINT32 cl_y_filter_when_cl_detected       : 1 ;  //    13     
	UINT32 clc_filter_mode                    : 2 ;  // 15:14     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cl_cc_protection_th                : 8 ;  // 31:24     
} PE_H13B0_P0L_CLC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x57c p0l_clc_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 cl_detection_count                 : 16;  // 31:16     
} PE_H13B0_P0L_CLC_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x580 p0l_vflt_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vfilterenable                      : 1 ;  //     0     
	UINT32 vfilterlocalenable                 : 1 ;  //     1     
	UINT32 vfilter_strength                   : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 cb_filter_y                        : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 vfilter_a_th                       : 8 ;  // 15: 8     
	UINT32 vfilter_in_field_var               : 8 ;  // 23:16     
	UINT32 vfilter_use_mc_mask                : 1 ;  //    24     
	UINT32 vfilter_expand                     : 1 ;  //    25     
	UINT32 vfilter_center                     : 1 ;  //    26     
	UINT32 vfilter_ul4                        : 1 ;  //    27     
	UINT32 dbgshow_level                      : 2 ;  // 29:28     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 dbgshow_enable                     : 1 ;  //    31     
} PE_H13B0_P0L_VFLT_CTRL_T;

/*-----------------------------------------------------------------------------
		0x584 p0l_cti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                     : 8 ;  //  7: 0     
	UINT32 cti_coring_th1                     : 8 ;  // 15: 8     
	UINT32 cti_coring_smooth                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 cti_coring_map_size                : 2 ;  // 21:20     
} PE_H13B0_P0L_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0x588 p0l_mnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_enable                     : 1 ;  //     0     
	UINT32 reg_mnr_debug                      : 2 ;  //  2: 1     
	UINT32 reg_mnr_motion_view                : 1 ;  //     3     
	UINT32 reg_h_expend                       : 1 ;  //     4     
	UINT32 reg_gain_flt_size                  : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_mnr_master_gain                : 8 ;  // 15: 8     
	UINT32 reg_chroma_master_gain             : 8 ;  // 23:16     
} PE_H13B0_P0L_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x58c p0l_mnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_s1_mmd_min                 : 8 ;  //  7: 0     
	UINT32 reg_mnr_s2_ratio_min               : 8 ;  // 15: 8     
	UINT32 reg_mnr_s2_ratio_max               : 8 ;  // 23:16     
	UINT32 reg_mnr_s2_mmd_min                 : 8 ;  // 31:24     
} PE_H13B0_P0L_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x590 p0l_mnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_mnr_h_gain                     : 8 ;  // 23:16     
	UINT32 reg_mnr_v_gain                     : 8 ;  // 31:24     
} PE_H13B0_P0L_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x594 p0l_mnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_filter_x0                      : 8 ;  //  7: 0     
	UINT32 reg_filter_x1                      : 8 ;  // 15: 8     
	UINT32 reg_filter_y0                      : 8 ;  // 23:16     
	UINT32 reg_filter_y1                      : 8 ;  // 31:24     
} PE_H13B0_P0L_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x598 p0l_mnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_en                  : 1 ;  //     0     
	UINT32 reg_motion_mnr_filter              : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 reg_mnr_motion_min                 : 8 ;  // 23:16     
	UINT32 reg_mnr_motion_max                 : 8 ;  // 31:24     
} PE_H13B0_P0L_MNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x59c p0l_mnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_x0                  : 8 ;  //  7: 0     
	UINT32 reg_motion_mnr_x1                  : 8 ;  // 15: 8     
	UINT32 reg_motion_mnr_y0                  : 8 ;  // 23:16     
	UINT32 reg_motion_mnr_y1                  : 8 ;  // 31:24     
} PE_H13B0_P0L_MNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x5a4 p0l_vfilter_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable              : 1 ;  //     0     
	UINT32 cr_hmc_protect_only_hmc_line       : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved     
	UINT32 cr_hmc_protect_motion_th           : 8 ;  // 15: 8     
	UINT32 cr_hmc_protect_th_max              : 8 ;  // 23:16     
	UINT32 cr_hmc_protect_th_min              : 8 ;  // 31:24     
} PE_H13B0_P0L_VFILTER_02_T;

/*-----------------------------------------------------------------------------
		0x5a8 p0l_vfilter_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en               : 1 ;  //     0     
	UINT32 cr_vfilter_force_en                : 1 ;  //     1     
	UINT32 cr_global_frame_num_th             : 4 ;  //  5: 2     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cr_vfilter_global_th               : 12;  // 19: 8     
	UINT32 tearing_count                      : 12;  // 31:20     
} PE_H13B0_P0L_VFILTER_01_T;

/*-----------------------------------------------------------------------------
		0x5ac p0l_bnr_dc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_enable                  : 1 ;  //     0     
	UINT32 reg_dc_bnr_var_en                  : 1 ;  //     1     
	UINT32 reg_dc_bnr_motion_en               : 1 ;  //     2     
	UINT32 reg_dc_bnr_acadaptive              : 1 ;  //     3     
	UINT32 reg_dc_bnr_sdhd_sel                : 1 ;  //     4     
	UINT32 reg_dc_blur_sel                    : 2 ;  //  6: 5     
	UINT32 reg_dc_debug_mode                  : 1 ;  //     7     
	UINT32 reg_dc_bnr_mastergain              : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_chromagain              : 9 ;  // 24:16     
	UINT32 reg_dc_protection_en               : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_dc_bnr_debug_mode              : 4 ;  // 31:28     
} PE_H13B0_P0L_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x5b0 p0l_bnr_dc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_var_th3                 : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_var_th2                 : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_var_th1                 : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_var_th0                 : 8 ;  // 31:24     
} PE_H13B0_P0L_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x5b4 p0l_bnr_dc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_motion_th3              : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_motion_th2              : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_motion_th1              : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_motion_th0              : 8 ;  // 31:24     
} PE_H13B0_P0L_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x5b8 p0l_bnr_dc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_chroma_variance             : 8 ;  //  7: 0     
	UINT32 reg_dc_var_h_gain                  : 8 ;  // 15: 8     
	UINT32 reg_dc_protection_th               : 8 ;  // 23:16     
} PE_H13B0_P0L_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x5bc p0l_bnr_ac_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_hys_mode                    : 4 ;  //  3: 0     
	UINT32 reg_sc_bdr_type                    : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 ac_bnr_protect_motion_max          : 8 ;  // 23:16     
	UINT32 ac_bnr_protect_motion_min          : 8 ;  // 31:24     
} PE_H13B0_P0L_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x5c0 p0l_bnr_ac_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_h_en                    : 1 ;  //     0     
	UINT32 reg_bnr_ac_v_en                    : 1 ;  //     1     
	UINT32 reg_bnr_ac_protect_en              : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_bnr_ac_sdhd_sel                : 1 ;  //     4     
	UINT32 reg_bnr_ac_h_chroma_en             : 1 ;  //     5     
	UINT32 reg_bnr_ac_v_chroma_en             : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_bnr_ac_gain_h_max              : 4 ;  // 11: 8     
	UINT32 reg_bnr_ac_gain_v_max              : 4 ;  // 15:12     
	UINT32 reg_bnr_ac_str_resolution          : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_bnr_ac_flt_type                : 1 ;  //    20     
	UINT32                                    : 1 ;  //    21     reserved
	UINT32 reg_bnr_ac_dbg_sel                 : 2 ;  // 23:22     
	UINT32 ac_chroma_master_gain              : 8 ;  // 31:24     
} PE_H13B0_P0L_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x5c4 p0l_bnr_ac_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_h_x0                      : 8 ;  //  7: 0     
	UINT32 strength_h_x1                      : 8 ;  // 15: 8     
	UINT32 strength_h_max                     : 8 ;  // 23:16     
	UINT32 detect_min_th                      : 8 ;  // 31:24     
} PE_H13B0_P0L_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x5c8 p0l_bnr_ac_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_v_x0                      : 8 ;  //  7: 0     
	UINT32 strength_v_x1                      : 8 ;  // 15: 8     
	UINT32 strength_v_max                     : 8 ;  // 23:16     
} PE_H13B0_P0L_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x5cc p0l_bnr_ac_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset_mode                      : 1 ;  //     0     
	UINT32 manual_offset_h_value              : 3 ;  //  3: 1     
	UINT32 v_offset_mode                      : 1 ;  //     4     
	UINT32 manual_offset_v_value              : 3 ;  //  7: 5     
	UINT32 use_of_hysterisis                  : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 t_filter_weight                    : 8 ;  // 23:16     
} PE_H13B0_P0L_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x5d0 p0l_bnr_ac_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_delta_th0                      : 8 ;  //  7: 0     
	UINT32 max_delta_th1                      : 8 ;  // 15: 8     
	UINT32 h_blockness_th                     : 8 ;  // 23:16     
	UINT32 h_weight_max                       : 8 ;  // 31:24     
} PE_H13B0_P0L_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x5d4 p0l_bnr_ac_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_gain3                   : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_gain2                   : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_gain1                   : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_gain0                   : 8 ;  // 31:24     
} PE_H13B0_P0L_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x5d8 p0l_dnr_max_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_ac_bnr                      : 1 ;  //     0     
	UINT32 enable_mnr                         : 1 ;  //     1     
	UINT32 enable_dc_bnr                      : 1 ;  //     2     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 debug_enable                       : 1 ;  //     8     
	UINT32 debug_mode                         : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 win_control_enable                 : 1 ;  //    16     
	UINT32 border_enable                      : 1 ;  //    17     
	UINT32 reg_win_inout                      : 1 ;  //    18     
} PE_H13B0_P0L_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
		0x5dc p0l_dnr_dbar_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                       : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 show_debug_bar_for_bnr             : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 gain                               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sd_hd_sel                          : 2 ;  //  9: 8     
} PE_H13B0_P0L_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0x5e0 p0l_bnr_stat_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                    : 8 ;  //  7: 0     
	UINT32 ac_bnr_v_status                    : 8 ;  // 15: 8     
	UINT32 dc_bnr_th                          : 14;  // 29:16     
} PE_H13B0_P0L_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
		0x5e4 p0l_cti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32 cti_clipping_mode                  : 1 ;  //     1     
	UINT32 cti_pre_flt_mode                   : 2 ;  //  3: 2     
	UINT32 cti_tap                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
	UINT32 cti_debug_mode                     : 3 ;  // 18:16     
} PE_H13B0_P0L_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0x5e8 p0l_reg_c420_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0L_REG_C420_CTRL_T;

/*-----------------------------------------------------------------------------
		0x5f0 p0l_ipc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cubic_coef0                    : 8 ;  //  7: 0     
	UINT32 reg_cubic_coef1                    : 8 ;  // 15: 8     
	UINT32 reg_va_blending_ctrl_lv1           : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_va_blending_ctrl_lv2           : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_va_blending_ctrl_lv3           : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_va_blending_ctrl_lv4           : 3 ;  // 30:28     
} PE_H13B0_P0L_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0x5f4 p0l_ipc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_cubic                       : 1 ;  //     0     
	UINT32 reg_boundary_window                : 1 ;  //     1     
	UINT32 reg_force_va                       : 1 ;  //     2     
	UINT32 reg_half_pel_va                    : 1 ;  //     3     
	UINT32 reg_dir_med_tap                    : 2 ;  //  5: 4     
	UINT32 reg_diff_med_tap                   : 2 ;  //  7: 6     
	UINT32 reg_va_blending_ctrl1              : 3 ;  // 10: 8     
	UINT32 reg_wide_angle_protection_1        : 1 ;  //    11     
	UINT32 reg_va_blending_ctrl2              : 3 ;  // 14:12     
	UINT32 reg_wide_angle_protection_2        : 1 ;  //    15     
	UINT32 reg_errs_chroma_blend_coef         : 8 ;  // 23:16     
	UINT32 reg_strong_global_th               : 6 ;  // 29:24     
	UINT32 narrow_angle_week                  : 1 ;  //    30     
	UINT32 narrow_angle_protection            : 1 ;  //    31     
} PE_H13B0_P0L_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0x5f8 p0l_ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0     
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8     
} PE_H13B0_P0L_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0x5fc p0l_ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0     
	UINT32 cr_lock_th                         : 7 ;  //  7: 1     
	UINT32 cr_g_motion_th                     : 8 ;  // 15: 8     
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16     
	UINT32 cr_hmc_half_en                     : 1 ;  //    19
	UINT32 cr_robustness                      : 4 ;  // 23:20     
	UINT32 cr_center_weight                   : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0x600 p0l_ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8     
	UINT32 cr_gds_resolution                  : 3 ;  // 18:16     
} PE_H13B0_P0L_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0x604 p0l_ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8     
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16     
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0x608 p0l_ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0x60c p0l_ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0     
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8     
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16     
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0x610 p0l_ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0L_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0x614 p0l_ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16     
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24     
} PE_H13B0_P0L_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0x618 p0l_ipc_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmc_motion_th                  : 8 ;  //  7: 0     
	UINT32 reg_vmc_debug                      : 1 ;  //     8     
	UINT32 reg_vmc_en                         : 1 ;  //     9     
	UINT32 reg_vmc_still_check                : 1 ;  //    10     
	UINT32 reg_vmc_mul                        : 3 ;  // 13:11     
	UINT32 reg_vmc_out_mul                    : 2 ;  // 15:14     
	UINT32 reg_vmc_protect_th                 : 8 ;  // 23:16     
	UINT32 reg_protection_en                  : 1 ;  //    24     
} PE_H13B0_P0L_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0x61c p0l_ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} PE_H13B0_P0L_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0x628 p0l_ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_lr_count                      : 16;  // 15: 0     
	UINT32 r_pc_lr_count                      : 16;  // 31:16     
} PE_H13B0_P0L_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0x62c p0l_ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_lr_same                    : 16;  // 15: 0     
	UINT32 r_pndiff_count                     : 16;  // 31:16     
} PE_H13B0_P0L_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0x630 p0l_ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_lr_same                    : 16;  // 15: 0     
	UINT32 r_pc_pn_lr_same                    : 16;  // 31:16     
} PE_H13B0_P0L_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0x634 p0l_ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0     
	UINT32 r_inv_original_cnt                 : 16;  // 31:16     
} PE_H13B0_P0L_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0x638 p0l_ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0L_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0x63c p0l_ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0L_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0x640 p0l_tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_show_scale                     : 3 ;  //  2: 0     
	UINT32 reg_bbd_show                       : 1 ;  //     3     
	UINT32 reg_show_bar_1                     : 1 ;  //     4     
	UINT32 reg_show_bar_2                     : 1 ;  //     5     
	UINT32 reg_show_bar_3                     : 1 ;  //     6     
	UINT32 reg_show_bar_4                     : 1 ;  //     7     
	UINT32 reg_bar1_data_mux                  : 2 ;  //  9: 8     
	UINT32 reg_bar2_data_mux                  : 2 ;  // 11:10     
	UINT32 reg_bar3_data_mux                  : 2 ;  // 13:12     
	UINT32 reg_bar4_data_mux                  : 2 ;  // 15:14     
	UINT32 reg_hist0_data_mux                 : 4 ;  // 19:16     
	UINT32 reg_hist1_data_mux                 : 4 ;  // 23:20     
	UINT32 reg_hist2_data_mux                 : 4 ;  // 27:24     
	UINT32 reg_hist0_win_sel                  : 1 ;  //    28     
	UINT32 reg_hist1_win_sel                  : 1 ;  //    29     
	UINT32 reg_hist2_win_sel                  : 1 ;  //    30     
	UINT32 hist_rd_en                         : 1 ;  //    31     
} PE_H13B0_P0L_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x644 p0l_tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15     
	UINT32 wina_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31     
} PE_H13B0_P0L_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x648 p0l_tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30     
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31     
} PE_H13B0_P0L_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x64c p0l_tpd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winb_hen                       : 1 ;  //    15     
	UINT32 winb_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winb_ven                       : 1 ;  //    31     
} PE_H13B0_P0L_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x650 p0l_tpd_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winb_y1                            : 12;  // 27:16     
} PE_H13B0_P0L_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x654 p0l_tpd_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_y                        : 10;  //  9: 0     
	UINT32 reg_force_y_max_th                 : 10;  // 19:10     
	UINT32 reg_force_y_min_th                 : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_y_enable                 : 1 ;  //    31     
} PE_H13B0_P0L_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x658 p0l_tpd_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cb                       : 10;  //  9: 0     
	UINT32 reg_force_cb_max_th                : 10;  // 19:10     
	UINT32 reg_force_cb_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cb_enable                : 1 ;  //    31     
} PE_H13B0_P0L_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x65c p0l_tpd_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cr                       : 10;  //  9: 0     
	UINT32 reg_force_cr_max_th                : 10;  // 19:10     
	UINT32 reg_force_cr_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cr_enable                : 1 ;  //    31     
} PE_H13B0_P0L_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x660 p0l_tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                   : 22;  // 21: 0
} PE_H13B0_P0L_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x66c p0l_clc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_lcount_enable                   : 1 ;  //     0     
	UINT32 cl_lcnt_mode                       : 2 ;  //  2: 1     
	UINT32 cl_scene_change_enable             : 1 ;  //     3     
	UINT32 cl_scene_change_mode               : 1 ;  //     4     
	UINT32 cl_scene_change_manual             : 1 ;  //     5     
	UINT32 cl_lcnt_th                         : 4 ;  //  9: 6     
	UINT32 cl_plus_lcount                     : 3 ;  // 12:10     
	UINT32 cl_minus_lcount                    : 2 ;  // 14:13     
	UINT32 cl_scene_change_protect            : 1 ;  //    15     
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 cl_tearing_pt_th2                  : 12;  // 31:20     
} PE_H13B0_P0L_CLC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x670 p0l_bbd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0     
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1     
	UINT32 reg_op_mode                        : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8     
	UINT32 reg_diff_th                        : 8 ;  // 23:16     
	UINT32 reg_bbd_mux                        : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_apl_mux                        : 3 ;  // 30:28     
} PE_H13B0_P0L_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x674 p0l_bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13B0_P0L_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x678 p0l_bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0L_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x67c p0l_bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13B0_P0L_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x680 p0l_bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0L_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0x684 p0l_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0     
	UINT32 cb_b_s_apl                         : 10;  // 19:10     
	UINT32 cr_r_s_apl                         : 10;  // 29:20     
} PE_H13B0_P0L_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x690 p0l_tpd_hist_ai_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32 reg_read_hist_sel                  : 2 ;  // 14:13     
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_available                  : 1 ;  //    28     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 hist_read_done                     : 1 ;  //    31     
} PE_H13B0_P0L_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
		0x694 p0l_tpd_hist_ai_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_status                        : 22;  // 21: 0     
} PE_H13B0_P0L_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
		0x698 p0l_lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16     
	UINT32 y_gain                             : 8 ;  // 31:24     
} PE_H13B0_P0L_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x6a0 p0l_3dfd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_offset_x                        : 8 ;  //  7: 0     
	UINT32 aw_offset_y                        : 8 ;  // 15: 8     
	UINT32 cb_th1                             : 8 ;  // 23:16     
	UINT32 cb_th2                             : 8 ;  // 31:24     
} PE_H13B0_P0L_3DFD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x6a4 p0l_3dfd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_th1                             : 8 ;  //  7: 0     
	UINT32 cc_th2                             : 8 ;  // 15: 8     
	UINT32 ll_th1                             : 8 ;  // 23:16     
	UINT32 ll_th2                             : 8 ;  // 31:24     
} PE_H13B0_P0L_3DFD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x6a8 p0l_3dfd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pt_th                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 sstb_th                            : 8 ;  // 23:16     
	UINT32 debug_mode                         : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 div_mode                           : 1 ;  //    28     
} PE_H13B0_P0L_3DFD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x6ac p0l_3dfd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 small_delta_th                     : 19;  // 18: 0     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 small_delta_en                     : 1 ;  //    24     
} PE_H13B0_P0L_3DFD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x6b0 p0l_3dfd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_count                           : 16;  // 15: 0     
	UINT32 cc_count                           : 16;  // 31:16     
} PE_H13B0_P0L_3DFD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x6b4 p0l_3dfd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ll_count                           : 16;  // 15: 0     
	UINT32 trid_format                        : 4 ;  // 19:16     
} PE_H13B0_P0L_3DFD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0x6b8 p0l_3dfd_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_h                         : 23;  // 22: 0     
} PE_H13B0_P0L_3DFD_STAT_02_T;

/*-----------------------------------------------------------------------------
		0x6bc p0l_3dfd_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_v                         : 23;  // 22: 0     
} PE_H13B0_P0L_3DFD_STAT_03_T;

/*-----------------------------------------------------------------------------
		0x6c0 p0l_3dfd_stat_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_v                         : 23;  // 22: 0     
} PE_H13B0_P0L_3DFD_STAT_04_T;

/*-----------------------------------------------------------------------------
		0x6c4 p0l_3dfd_stat_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_h                         : 23;  // 22: 0     
} PE_H13B0_P0L_3DFD_STAT_05_T;

/*-----------------------------------------------------------------------------
		0x6c8 p0l_3dfd_stat_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_ratio                           : 23;  // 22: 0     
} PE_H13B0_P0L_3DFD_STAT_06_T;

/*-----------------------------------------------------------------------------
		0x6cc p0l_3dfd_stat_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_ratio                           : 23;  // 22: 0     
} PE_H13B0_P0L_3DFD_STAT_07_T;

/*-----------------------------------------------------------------------------
		0x6d0 p0l_tnr_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_coring_en_c                      : 1 ;  //     0     
	UINT32 m_coring_en_y                      : 1 ;  //     1     
	UINT32 mc_sad_sel                         : 1 ;  //     2     
	UINT32 sad_protect_en                     : 1 ;  //     3     
	UINT32 m_debug_mode                       : 3 ;  //  6: 4     
	UINT32 m_debug_mode_en                    : 1 ;  //     7     
	UINT32 m_gain_c                           : 4 ;  // 11: 8     
	UINT32 m_gain_y                           : 4 ;  // 15:12     
	UINT32 m_coring_th                        : 6 ;  // 21:16     
	UINT32 sad_protect_gm_en                  : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 mv_cost_gmv_smooth_gain            : 5 ;  // 28:24     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 mv_cost_gmv_smooth_en              : 1 ;  //    31     
} PE_H13B0_P0L_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0x6d4 p0l_tnr_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 secam_mot_c_th                     : 8 ;  //  7: 0     
	UINT32 secam_mot_y_th                     : 8 ;  // 15: 8     
	UINT32 secam_c_v_th                       : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 secam_c_v_calc_mode                : 2 ;  // 31:30     
} PE_H13B0_P0L_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0x6d8 p0l_tnr_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_en                              : 1 ;  //     0     
	UINT32 sf_map_flt_en                      : 1 ;  //     1     
	UINT32 sf_debug_mode                      : 3 ;  //  4: 2     
	UINT32 sf_map_tap                         : 2 ;  //  6: 5     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sf_map_gain                        : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 sf_th0                             : 8 ;  // 23:16     
	UINT32 sf_th1                             : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0x6dc p0l_tnr_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_th2                             : 8 ;  //  7: 0     
	UINT32 sf_th3                             : 8 ;  // 15: 8     
	UINT32 sf_th4                             : 8 ;  // 23:16     
	UINT32 sf_f0_sel                          : 2 ;  // 25:24     
	UINT32 sf_f1_sel                          : 2 ;  // 27:26     
	UINT32 sf_f2_sel                          : 2 ;  // 29:28     
	UINT32 sf_f3_sel                          : 2 ;  // 31:30     
} PE_H13B0_P0L_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0x6e0 p0l_tnr_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y0                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x0                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y1                           : 8 ;  // 23:16     
	UINT32 sf_mb_x1                           : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0x6e4 p0l_tnr_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0L_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0x6e8 p0l_tnr_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y2                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x2                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y3                           : 8 ;  // 23:16     
	UINT32 sf_mb_x3                           : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0x6ec p0l_tnr_ctrl_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 sf_debug_map_sel                   : 2 ;  // 17:16     
	UINT32 sf_gain_mode                       : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sf_gain                            : 4 ;  // 23:20     
} PE_H13B0_P0L_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
		0x6f0 p0l_tnr_ctrl_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_blend_y0                      : 8 ;  //  7: 0     
	UINT32 mamc_blend_x0                      : 8 ;  // 15: 8     
	UINT32 mamc_blend_y1                      : 8 ;  // 23:16     
	UINT32 mamc_blend_x1                      : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
		0x6f4 p0l_tnr_ctrl_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_lock_th0                       : 6 ;  //  5: 0     
	UINT32 gmv_max_cnt_th                     : 2 ;  //  7: 6     
	UINT32 gmv_lock_th1                       : 6 ;  // 13: 8     
	UINT32 gmv_val_cnt_th                     : 2 ;  // 15:14     
	UINT32 gmv_manual_mv_en                   : 1 ;  //    16     
	UINT32 gmv_manual_alpha_en                : 1 ;  //    17     
	UINT32                                    : 1 ;  //    18     reserved
	UINT32 gmv_lock_en                        : 1 ;  //    19     
	UINT32 gmv_lock_max                       : 4 ;  // 23:20     
	UINT32 gmv_manual_mv                      : 4 ;  // 27:24     
	UINT32 gmv_manual_alpha                   : 4 ;  // 31:28     
} PE_H13B0_P0L_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
		0x6f8 p0l_tnr_ctrl_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_gain_ctrl2_y0                    : 8 ;  //  7: 0     
	UINT32 m_gain_ctrl2_x0                    : 8 ;  // 15: 8     
	UINT32 m_gain_ctrl2_y1                    : 8 ;  // 23:16     
	UINT32 m_gain_ctrl2_x1                    : 8 ;  // 31:24     
} PE_H13B0_P0L_TNR_CTRL_48_T;


/*-----------------------------------------------------------------------------
		0x2400 p0r_pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} PE_H13B0_P0R_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0x2404 p0r_pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16     
} PE_H13B0_P0R_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0x2408 p0r_pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                       : 4 ;  //  3: 0     
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4     
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8     
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12     
	UINT32 dnr_frame_id                       : 8 ;  // 23:16     
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24     
} PE_H13B0_P0R_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0x240c p0r_pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0     
} PE_H13B0_P0R_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0x2410 p0r_pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_c_en                           : 1 ;  //     0     
	UINT32 tnr_y_en                           : 1 ;  //     1     
	UINT32 ipc_c_en                           : 1 ;  //     2     
	UINT32 ipc_y_en                           : 1 ;  //     3     
	UINT32 tnr_c_422_as_420                   : 1 ;  //     4     
	UINT32 tnr_y2_en                          : 1 ;  //     5     
	UINT32 force_422_to_420                   : 1 ;  //     6     
	UINT32 ipc0_y_en_prog                     : 1 ;  //     7     
	UINT32 mif_last_mode                      : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 dnr_out_force_44x                  : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 detour_enable                      : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ibuf_en                            : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 dnr_in_lsb_mode                    : 2 ;  // 25:24     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 tnr_in_lsb_mode                    : 2 ;  // 29:28     
} PE_H13B0_P0R_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0x2414 p0r_pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0     
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 cs_type                            : 3 ;  // 15:13     
	UINT32 vsize                              : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31     
} PE_H13B0_P0R_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0x2418 p0r_pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 secam_line                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12     
} PE_H13B0_P0R_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
		0x241c p0r_pe0_param_by_ddr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 9 ;  //  8: 0     reserved
	UINT32 stride                             : 2 ;  // 10: 9     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 vsize                              : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 endian                             : 1 ;  //    30     
	UINT32 update_by_ddr                      : 1 ;  //    31     
} PE_H13B0_P0R_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
		0x2420 p0r_win_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13B0_P0R_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0x2424 p0r_win_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0R_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0x2428 p0r_win_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32 ac_bnr_feature_cal_mode            : 2 ;  // 29:28     
} PE_H13B0_P0R_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0x242c p0r_win_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0R_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0x2430 p0r_pe0_debug_mem_req ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 en_rd_t1_y                         : 1 ;  //     0     
	UINT32 en_rd_t1_c                         : 1 ;  //     1     
	UINT32 en_rd_t2_y                         : 1 ;  //     2     
	UINT32 en_rd_t2_c                         : 1 ;  //     3     
	UINT32 en_rd_t3_y                         : 1 ;  //     4     
	UINT32 en_rd_t3_c                         : 1 ;  //     5     
	UINT32 en_rd_t4_y                         : 1 ;  //     6     
	UINT32 en_rd_t1_m                         : 1 ;  //     7     
	UINT32 en_wr_tnrw_y                       : 1 ;  //     8     
	UINT32 en_wr_tnrw_c                       : 1 ;  //     9     
	UINT32 en_wr_tnrw_m                       : 1 ;  //    10     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 manual_mem_en                      : 1 ;  //    15     
} PE_H13B0_P0R_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0x2434 p0r_pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0     
	UINT32 lreq_dly_c_en                      : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16     
	UINT32 lreq_dly_y_en                      : 1 ;  //    27     
} PE_H13B0_P0R_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0x2440 p0r_tnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_enable                         : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 nt_lvl_hsub                        : 1 ;  //     2     
	UINT32 secam_cmode_en                     : 1 ;  //     3     
	UINT32 secam_alpha_scale                  : 2 ;  //  5: 4     
	UINT32 prev_sel_prog                      : 1 ;  //     6     
	UINT32 frame_skip_enable_for_3d           : 1 ;  //     7     
	UINT32 minfo_mode                         : 2 ;  //  9: 8     
	UINT32 minfo_sw                           : 2 ;  // 11:10     
	UINT32 hist_motion_scale                  : 2 ;  // 13:12     
	UINT32 minfo_flt_en_tnr                   : 1 ;  //    14     
	UINT32 minfo_flt_en_ipc                   : 1 ;  //    15     
	UINT32 calc_motion_max_yc                 : 1 ;  //    16     
	UINT32 motion_4f_en                       : 1 ;  //    17     
	UINT32 mv_tflt_en                         : 1 ;  //    18     
	UINT32 mv_sflt_en                         : 1 ;  //    19     
	UINT32 alpha_avg_en                       : 1 ;  //    20     
	UINT32                                    : 1 ;  //    21     reserved
	UINT32 alpha_mapping_y_en                 : 1 ;  //    22     
	UINT32 alpha_mapping_c_en                 : 1 ;  //    23     
	UINT32 nt_lvl_adjust_gm_enable            : 1 ;  //    24     
	UINT32 nt_lvl_adjust_lpct_enable          : 1 ;  //    25     
	UINT32                                    : 1 ;  //    26     reserved
	UINT32 nt_lvl_adjust_avg_ts_enable        : 1 ;  //    27     
	UINT32 nt_lvl_adjust_lpct_sel             : 1 ;  //    28     
	UINT32 mamc_blend_sel                     : 1 ;  //    29     
} PE_H13B0_P0R_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x2444 p0r_tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                            : 8 ;  //  7: 0     
	UINT32 y_th_hi                            : 8 ;  // 15: 8     
	UINT32 dnr_motion_scale                   : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 tnr_filter_lsb_ctrl                : 2 ;  // 21:20     
	UINT32 svar_mux_ctrl                      : 2 ;  // 23:22     
	UINT32 nt_lvl_hist_n_pct                  : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x2448 p0r_tnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_flat_svar_th                    : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind7                     : 8 ;  // 15: 8     
	UINT32 secam_cmode_flt_en                 : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 secam_cmode_flt_th                 : 4 ;  // 23:20     
	UINT32 m_gain_en_var                      : 1 ;  //    24     
	UINT32 a_gain_en_var                      : 1 ;  //    25     
	UINT32 m_gain_en_lsc                      : 1 ;  //    26     
	UINT32 a_gain_en_lsc                      : 1 ;  //    27     
	UINT32 m_gain_en_var2                     : 1 ;  //    28     
	UINT32 m_offset_mode_en                   : 1 ;  //    29     
} PE_H13B0_P0R_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x244c p0r_tnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                              : 8 ;  //  7: 0     
	UINT32 wa_y1                              : 8 ;  // 15: 8     
	UINT32 wa_x0                              : 8 ;  // 23:16     
	UINT32 wa_x1                              : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x2450 p0r_tnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x0                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x1                  : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x2454 p0r_tnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                             : 8 ;  //  7: 0     
	UINT32 nt_max                             : 8 ;  // 15: 8     
	UINT32 hist_svar_th                       : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th0               : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x2458 p0r_tnr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                          : 8 ;  //  7: 0     
	UINT32 nt_iir_alpha                       : 8 ;  // 15: 8     
	UINT32 y_th_lo2                           : 8 ;  // 23:16     
	UINT32 y_th_hi2                           : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x245c p0r_tnr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind1                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind2                     : 8 ;  // 23:16     
	UINT32 alpha_lut_ind3                     : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x2460 p0r_tnr_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind5                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind6                     : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th1               : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0x2464 p0r_tnr_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scene_change_enable                : 1 ;  //     0     
	UINT32 scene_change_max_flt_en            : 1 ;  //     1     
	UINT32 scene_change_measure_sel           : 1 ;  //     2     
	UINT32 scene_change_prev_hist_calc_en     : 1 ;  //     3     
	UINT32 scene_change_enforce               : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 scene_change_max_flt_tap           : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 scene_change_scale                 : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 scene_change_th                    : 8 ;  // 23:16     
	UINT32 gmv_svar_th                        : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0x2468 p0r_tnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_show_tnr_param                 : 4 ;  //  3: 0     
	UINT32 dbg_status_sum_sel                 : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 dbg_force_wa                       : 1 ;  //     8     
	UINT32 dbg_force_calc_motion              : 1 ;  //     9     
	UINT32 dbg_force_lut_alpha                : 1 ;  //    10     
	UINT32 dbg_force_adj_alpha                : 1 ;  //    11     
	UINT32 dbg_force_final_alpha              : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 ignore_active_window               : 1 ;  //    15     
	UINT32 dbg_tnrw_port_ctrl                 : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 tnr_black_line_en_for_i            : 1 ;  //    20     
	UINT32 tnr_debug_map_en_for_i             : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 dbg_wa                             : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0x246c p0r_tnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                    : 8 ;  //  7: 0     
	UINT32 dbg_alpha                          : 8 ;  // 15: 8     
	UINT32 peak0_x_th                         : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 scene_change_th_mv                 : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0x2470 p0r_tnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x2                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x3                  : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0x2474 p0r_tnr_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 force_nt_lvl_val                   : 8 ;  // 15: 8     
	UINT32 adjust_nt_lvl                      : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 adjust_nt_lvl_val                  : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0x2478 p0r_tnr_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 noise_measure_win_en               : 1 ;  //    31     
} PE_H13B0_P0R_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0x247c p0r_tnr_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0R_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0x2480 p0r_tnr_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_dbg_bar_scale                 : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 show_dbg_bar_en                    : 4 ;  //  7: 4     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 show_dbg_bar0_mux                  : 4 ;  // 19:16     
	UINT32 show_dbg_bar1_mux                  : 4 ;  // 23:20     
	UINT32 show_dbg_bar2_mux                  : 4 ;  // 27:24     
	UINT32 show_dbg_bar3_mux                  : 4 ;  // 31:28     
} PE_H13B0_P0R_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0x2484 p0r_tnr_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y0                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x0                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y1                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x1                     : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0x2488 p0r_tnr_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_enable                   : 1 ;  //     0     
	UINT32 luma_gain_enable                   : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 input_for_luma                     : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 input_for_crgn_satr                : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 y_gain                             : 8 ;  // 23:16     
	UINT32 c_gain                             : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0x248c p0r_tnr_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_x                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_x                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_x                     : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0x2490 p0r_tnr_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_y                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_y                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_y                     : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0x2494 p0r_tnr_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_mamc_blend                     : 8 ;  //  7: 0     
} PE_H13B0_P0R_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0x2498 p0r_tnr_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_protect_control_y0              : 8 ;  //  7: 0     
	UINT32 mv_protect_control_x0              : 8 ;  // 15: 8     
	UINT32 mv_protect_control_y1              : 8 ;  // 23:16     
	UINT32 mv_protect_control_x1              : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0x249c p0r_tnr_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x1                 : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0x24a0 p0r_tnr_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x3                 : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0x24a4 p0r_tnr_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x1                 : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0x24a8 p0r_tnr_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x3                 : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0x24ac p0r_tnr_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                     : 8 ;  //  7: 0     
	UINT32 crgn_gain_rgn1                     : 8 ;  // 15: 8     
	UINT32 crgn_gain_rgn2                     : 8 ;  // 23:16     
	UINT32 crgn_gain_rgn3                     : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0x24b0 p0r_tnr_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x0                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x1                  : 7 ;  // 30:24     
} PE_H13B0_P0R_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0x24b4 p0r_tnr_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                              : 8 ;  //  7: 0     
	UINT32 wa_y3                              : 8 ;  // 15: 8     
	UINT32 wa_x2                              : 8 ;  // 23:16     
	UINT32 wa_x3                              : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0x24b8 p0r_tnr_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x2                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x3                  : 7 ;  // 30:24     
} PE_H13B0_P0R_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0x24bc p0r_tnr_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c               : 1 ;  //     0     
	UINT32 spatial_pre_flt_en_y               : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 spatial_pre_flt_th_c               : 8 ;  // 23:16     
	UINT32 spatial_pre_flt_th_y               : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0x24c0 p0r_tnr_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                            : 8 ;  //  7: 0     
	UINT32 x_peak_t2                          : 8 ;  // 15: 8     
	UINT32 nt_lvl                             : 8 ;  // 23:16     
	UINT32 x_avg_s                            : 8 ;  // 31:24      
} PE_H13B0_P0R_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0x24c4 p0r_tnr_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sum_valid                          : 16;  // 15: 0     
	UINT32 scene_diff                         : 16;  // 31:16      
} PE_H13B0_P0R_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0x24c8 p0r_tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_l                               : 4 ;  //  3: 0     
	UINT32 mv_r                               : 4 ;  //  7: 4     
	UINT32 dfd                                : 8 ;  // 15: 8     
	UINT32 apl_y                              : 8 ;  // 23:16     
	UINT32 lp                                 : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0x24cc p0r_tnr_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_valid_cnt                      : 16;  // 15: 0     
	UINT32 fd                                 : 8 ;  // 23:16     
	UINT32 gmv_lock_cnt                       : 6 ;  // 29:24     
} PE_H13B0_P0R_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0x24d0 p0r_tnr_main_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 main_lut_ai_enable                 : 1 ;  //    12     
} PE_H13B0_P0R_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
		0x24d4 p0r_tnr_main_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0               : 32;  // 31: 0     
} PE_H13B0_P0R_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
		0x24d8 p0r_tnr_main_lut_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1               : 32;  // 31: 0     
} PE_H13B0_P0R_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
		0x24dc p0r_tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_r_cnt                           : 16;  // 15: 0     
	UINT32 mv_l_cnt                           : 16;  // 31:16     
} PE_H13B0_P0R_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0x24e0 p0r_tnr_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x0                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y1                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x1                : 7 ;  // 30:24     
} PE_H13B0_P0R_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0x24e4 p0r_tnr_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x2                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y3                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x3                : 7 ;  // 30:24     
} PE_H13B0_P0R_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0x24e8 p0r_tnr_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_ctrl_y0                    : 8 ;  //  7: 0     
	UINT32 mc_blur_ctrl_x0                    : 8 ;  // 15: 8     
	UINT32 mc_blur_ctrl_y1                    : 8 ;  // 23:16     
	UINT32 mc_blur_ctrl_x1                    : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0x24ec p0r_tnr_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y2                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x2                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y3                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x3                     : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0x24f0 p0r_tnr_crg_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr               : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 crgn_ctrl_ai_enable                : 1 ;  //    12     
} PE_H13B0_P0R_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x24f4 p0r_tnr_crg_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data               : 32;  // 31: 0     
} PE_H13B0_P0R_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x24f8 p0r_tnr_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_sel_hist                       : 1 ;  //     0     
	UINT32 sad_sel_pels                       : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 hme_half_pel_en                    : 1 ;  //     4     
	UINT32 m_tiir_x_sel                       : 1 ;  //     5     
	UINT32 m_tiir_scene_change_en             : 1 ;  //     6     
	UINT32 m_tiir_en                          : 1 ;  //     7     
	UINT32 sf_blend_en_y                      : 1 ;  //     8     
	UINT32 sf_blend_en_c                      : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 sf_blend_motion_exp_mode           : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 mv0_protect_th                     : 7 ;  // 22:16     
	UINT32 mv0_protect_en                     : 1 ;  //    23     
	UINT32 mv_4f_en                           : 1 ;  //    24     
	UINT32 mv_pfilter_en                      : 1 ;  //    25     
	UINT32 mv_cost_smooth_gain                : 5 ;  // 30:26     
	UINT32 mv_cost_smooth_en                  : 1 ;  //    31     
} PE_H13B0_P0R_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0x24fc p0r_tnr_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_flt_en_c                        : 1 ;  //     0     
	UINT32 mc_flt_mv_inv_c                    : 1 ;  //     1     
	UINT32 mc_flt_mv_debug_en_c               : 1 ;  //     2     
	UINT32 mc_flt_manual_blend_en_c           : 1 ;  //     3     
	UINT32 mc_flt_mv_debug_c                  : 4 ;  //  7: 4     
	UINT32 mc_flt_manual_blend_c              : 6 ;  // 13: 8     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 mc_flt_blur_en_c                   : 1 ;  //    15     
	UINT32 mc_flt_en_y                        : 1 ;  //    16     
	UINT32 mc_flt_mv_inv_y                    : 1 ;  //    17     
	UINT32 mc_flt_mv_debug_en_y               : 1 ;  //    18     
	UINT32 mc_flt_manual_blend_en_y           : 1 ;  //    19     
	UINT32 mc_flt_mv_debug_y                  : 4 ;  // 23:20     
	UINT32 mc_flt_manual_blend_y              : 6 ;  // 29:24     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 mc_flt_blur_en_y                   : 1 ;  //    31     
} PE_H13B0_P0R_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0x2500 p0r_ipc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_chroma_mode                     : 2 ;  //  5: 4     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 cr_chroma_edi                      : 1 ;  //     7     
	UINT32 cr_integer_edi                     : 1 ;  //     8     
	UINT32                                    : 15;  // 23: 9     reserved
	UINT32 cr_hmc_force_fmd                   : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 cr_clc_fmd_off                     : 1 ;  //    26     
	UINT32 cr_st_flt_original_en              : 1 ;  //    27     
	UINT32 cr_film_auto_gain                  : 1 ;  //    28     
	UINT32 cr_film_apply_c                    : 1 ;  //    29     
	UINT32 cr_film_apply_y                    : 1 ;  //    30     
	UINT32 cr_film_mode_enable                : 1 ;  //    31     
} PE_H13B0_P0R_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x2504 p0r_ipc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 cr_st_flt_enable                   : 1 ;  //    16     
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    17     
	UINT32 cr_hmc_flt_enable                  : 1 ;  //    18     
	UINT32 cr_game_mode                       : 1 ;  //    19     
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 23:20     
	UINT32                                    : 1 ;  //    24     reserved
	UINT32 cr_black_mask                      : 1 ;  //    25     
	UINT32 cr_ipc_debug_show                  : 5 ;  // 30:26     
	UINT32 cr_5f_mode                         : 1 ;  //    31     
} PE_H13B0_P0R_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x2508 p0r_ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x250c p0r_ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x2510 p0r_ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0     
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8     
	UINT32 tearing_gain                       : 8 ;  // 23:16     
	UINT32 motion_gain                        : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x2514 p0r_ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha              : 8 ;  //  7: 0     
	UINT32 cr_t_grad_weight                   : 8 ;  // 15: 8     
	UINT32 cr_v_grad_weight                   : 8 ;  // 23:16     
	UINT32 cr_h_grad_weight                   : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x2518 p0r_ipc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_normalize_en                    : 1 ;  //     0     
	UINT32 cr_flat_only                       : 1 ;  //     1     
	UINT32 use_tnr_motion                     : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 cr_tearing_normalize_en            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cr_normalize_base                  : 8 ;  // 15: 8     
	UINT32 cr_normalize_center                : 8 ;  // 23:16     
	UINT32 cr_motion_coring                   : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x251c p0r_ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1     
	UINT32 cr_45_en                           : 1 ;  //     2     
	UINT32 cr_15_en                           : 1 ;  //     3     
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 tnr_motion_gain                    : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x2520 p0r_ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30     
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31     
} PE_H13B0_P0R_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0x2524 p0r_ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16     
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0x2528 p0r_ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16     
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0x252c p0r_ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0     
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8     
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16     
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0x2530 p0r_ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0x2534 p0r_ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    16     
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 23:19     
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 29:24     
} PE_H13B0_P0R_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0x2538 p0r_ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reg_manual_en                      : 1 ;  //     1     
	UINT32 reg_manual_mode                    : 2 ;  //  3: 2     
	UINT32 reg_lock_speed                     : 4 ;  //  7: 4     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0x253c p0r_ipc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_protection_en                  : 1 ;  //     0     
	UINT32 reg_protection_s1en                : 1 ;  //     1     
	UINT32 reg_protection_s2en                : 1 ;  //     2     
	UINT32 reg_protection_s3en                : 1 ;  //     3     
	UINT32 reg_protection_s4en                : 1 ;  //     4     
	UINT32 reg_badedit_protect                : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_modify_ratio_th                : 8 ;  // 15: 8     
	UINT32 fmd_tower_spangain                 : 4 ;  // 19:16     
	UINT32 fmd_bleacherratio                  : 4 ;  // 23:20     
	UINT32 fmd_vlrratiomin                    : 4 ;  // 27:24     
	UINT32 fmd_spxl2ratio                     : 4 ;  // 31:28     
} PE_H13B0_P0R_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0x2540 p0r_ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32 reg_same_min_ratio                 : 4 ;  // 27:24     
	UINT32 reg_same_adder_gain                : 4 ;  // 31:28     
} PE_H13B0_P0R_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0x2544 p0r_ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0     
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2     
	UINT32 cr_field_inverse                   : 1 ;  //     4     
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0x2548 p0r_ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0     
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8     
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16     
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0x254c p0r_ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0     
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8     
	UINT32 reg_2d_offset                      : 8 ;  // 23:16     
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0x2550 p0r_ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                       : 16;  // 15: 0     
	UINT32 r_nc_v_count                       : 16;  // 31:16     
} PE_H13B0_P0R_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0x2554 p0r_ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                       : 16;  // 15: 0     
	UINT32 r_pn_v_count                       : 16;  // 31:16     
} PE_H13B0_P0R_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0x2558 p0r_ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_same                       : 16;  // 15: 0     
	UINT32 r_pc_pn_same                       : 16;  // 31:16     
} PE_H13B0_P0R_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0x255c p0r_ipc_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pn_lr_count                      : 16;  // 15: 0     
	UINT32 same_ng_flag                       : 1 ;  //    16     
	UINT32 s3_protect_en_flag                 : 1 ;  //    17     
	UINT32 s2_protect_en_flag                 : 1 ;  //    18     
	UINT32 s1_protect_en_flag                 : 1 ;  //    19     
	UINT32 modify_pn_ratio_flag               : 1 ;  //    20     
	UINT32 base_pn_ratio_flag                 : 1 ;  //    21     
	UINT32 r_fmd_comb_mode                    : 2 ;  // 23:22     
	UINT32 r_fmd_lock_count                   : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0x2580 p0r_vfilter_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vfilterenable                      : 1 ;  //     0     
	UINT32 vfilterlocalenable                 : 1 ;  //     1     
	UINT32 vfilter_strength                   : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 cb_filter_y                        : 2 ;  //  5: 4     
	UINT32 cb_filter_c                        : 2 ;  //  7: 6     
	UINT32 vfilter_a_th                       : 8 ;  // 15: 8     
	UINT32 vfilter_in_field_var               : 8 ;  // 23:16     
	UINT32 vfilter_use_mc_mask                : 1 ;  //    24     
	UINT32 vfilter_expand                     : 1 ;  //    25     
	UINT32 vfilter_center                     : 1 ;  //    26     
	UINT32 vfilter_ul4                        : 1 ;  //    27     
	UINT32 dbgshow_level                      : 2 ;  // 29:28     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 dbgshow_enable                     : 1 ;  //    31     
} PE_H13B0_P0R_VFILTER_00_T;

/*-----------------------------------------------------------------------------
		0x2584 p0r_cti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                     : 8 ;  //  7: 0     
	UINT32 cti_coring_th1                     : 8 ;  // 15: 8     
	UINT32 cti_coring_smooth                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 cti_coring_map_size                : 2 ;  // 21:20     
} PE_H13B0_P0R_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0x2588 p0r_mnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_enable                     : 1 ;  //     0     
	UINT32 reg_mnr_debug                      : 2 ;  //  2: 1     
	UINT32 reg_mnr_motion_view                : 1 ;  //     3     
	UINT32 reg_h_expend                       : 1 ;  //     4     
	UINT32 reg_gain_flt_size                  : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_mnr_master_gain                : 8 ;  // 15: 8     
	UINT32 reg_chroma_master_gain             : 8 ;  // 23:16     
} PE_H13B0_P0R_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x258c p0r_mnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_s1_mmd_min                 : 8 ;  //  7: 0     
	UINT32 reg_mnr_s2_ratio_min               : 8 ;  // 15: 8     
	UINT32 reg_mnr_s2_ratio_max               : 8 ;  // 23:16     
	UINT32 reg_mnr_s2_mmd_min                 : 8 ;  // 31:24     
} PE_H13B0_P0R_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x2590 p0r_mnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_mnr_h_gain                     : 8 ;  // 23:16     
	UINT32 reg_mnr_v_gain                     : 8 ;  // 31:24     
} PE_H13B0_P0R_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x2594 p0r_mnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_filter_x0                      : 8 ;  //  7: 0     
	UINT32 reg_filter_x1                      : 8 ;  // 15: 8     
	UINT32 reg_filter_y0                      : 8 ;  // 23:16     
	UINT32 reg_filter_y1                      : 8 ;  // 31:24     
} PE_H13B0_P0R_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x2598 p0r_mnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_en                  : 1 ;  //     0     
	UINT32 reg_motion_mnr_filter              : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 reg_mnr_motion_min                 : 8 ;  // 23:16     
	UINT32 reg_mnr_motion_max                 : 8 ;  // 31:24     
} PE_H13B0_P0R_MNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x259c p0r_mnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_x0                  : 8 ;  //  7: 0     
	UINT32 reg_motion_mnr_x1                  : 8 ;  // 15: 8     
	UINT32 reg_motion_mnr_y0                  : 8 ;  // 23:16     
	UINT32 reg_motion_mnr_y1                  : 8 ;  // 31:24     
} PE_H13B0_P0R_MNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x25a4 p0r_vfilter_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable              : 1 ;  //     0     
	UINT32 cr_hmc_protect_only_hmc_line       : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved     
	UINT32 cr_hmc_protect_motion_th           : 8 ;  // 15: 8     
	UINT32 cr_hmc_protect_th_max              : 8 ;  // 23:16     
	UINT32 cr_hmc_protect_th_min              : 8 ;  // 31:24     
} PE_H13B0_P0R_VFILTER_02_T;

/*-----------------------------------------------------------------------------
		0x25a8 p0r_vfilter_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en               : 1 ;  //     0     
	UINT32 cr_vfilter_force_en                : 1 ;  //     1     
	UINT32 cr_global_frame_num_th             : 4 ;  //  5: 2     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cr_vfilter_global_th               : 12;  // 19: 8     
	UINT32 tearing_count                      : 12;  // 31:20     
} PE_H13B0_P0R_VFILTER_01_T;

/*-----------------------------------------------------------------------------
		0x25ac p0r_bnr_dc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_enable                  : 1 ;  //     0     
	UINT32 reg_dc_bnr_var_en                  : 1 ;  //     1     
	UINT32 reg_dc_bnr_motion_en               : 1 ;  //     2     
	UINT32 reg_dc_bnr_acadaptive              : 1 ;  //     3     
	UINT32 reg_dc_bnr_sdhd_sel                : 1 ;  //     4     
	UINT32 reg_dc_blur_sel                    : 2 ;  //  6: 5     
	UINT32 reg_dc_debug_mode                  : 1 ;  //     7     
	UINT32 reg_dc_bnr_mastergain              : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_chromagain              : 9 ;  // 24:16     
	UINT32 reg_dc_protection_en               : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_dc_bnr_debug_mode              : 4 ;  // 31:28     
} PE_H13B0_P0R_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x25b0 p0r_bnr_dc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_var_th3                 : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_var_th2                 : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_var_th1                 : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_var_th0                 : 8 ;  // 31:24     
} PE_H13B0_P0R_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x25b4 p0r_bnr_dc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_motion_th3              : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_motion_th2              : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_motion_th1              : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_motion_th0              : 8 ;  // 31:24     
} PE_H13B0_P0R_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x25b8 p0r_bnr_dc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_chroma_variance             : 8 ;  //  7: 0     
	UINT32 reg_dc_var_h_gain                  : 8 ;  // 15: 8     
	UINT32 reg_dc_protection_th               : 8 ;  // 23:16     
} PE_H13B0_P0R_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x25bc p0r_bnr_ac_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_hys_mode                    : 4 ;  //  3: 0     
	UINT32 reg_sc_bdr_type                    : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 ac_bnr_protect_motion_max          : 8 ;  // 23:16     
	UINT32 ac_bnr_protect_motion_min          : 8 ;  // 31:24     
} PE_H13B0_P0R_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x25c0 p0r_bnr_ac_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_h_en                    : 1 ;  //     0     
	UINT32 reg_bnr_ac_v_en                    : 1 ;  //     1     
	UINT32 reg_bnr_ac_protect_en              : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_bnr_ac_sdhd_sel                : 1 ;  //     4     
	UINT32 reg_bnr_ac_h_chroma_en             : 1 ;  //     5     
	UINT32 reg_bnr_ac_v_chroma_en             : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_bnr_ac_gain_h_max              : 4 ;  // 11: 8     
	UINT32 reg_bnr_ac_gain_v_max              : 4 ;  // 15:12     
	UINT32 reg_bnr_ac_str_resolution          : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_bnr_ac_flt_type                : 1 ;  //    20     
	UINT32                                    : 1 ;  //    21     reserved
	UINT32 reg_bnr_ac_dbg_sel                 : 2 ;  // 23:22     
	UINT32 ac_chroma_master_gain              : 8 ;  // 31:24     
} PE_H13B0_P0R_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x25c4 p0r_bnr_ac_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_h_x0                      : 8 ;  //  7: 0     
	UINT32 strength_h_x1                      : 8 ;  // 15: 8     
	UINT32 strength_h_max                     : 8 ;  // 23:16     
	UINT32 detect_min_th                      : 8 ;  // 31:24     
} PE_H13B0_P0R_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x25c8 p0r_bnr_ac_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_v_x0                      : 8 ;  //  7: 0     
	UINT32 strength_v_x1                      : 8 ;  // 15: 8     
	UINT32 strength_v_max                     : 8 ;  // 23:16     
} PE_H13B0_P0R_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x25cc p0r_bnr_ac_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset_mode                      : 1 ;  //     0     
	UINT32 manual_offset_h_value              : 3 ;  //  3: 1     
	UINT32 v_offset_mode                      : 1 ;  //     4     
	UINT32 manual_offset_v_value              : 3 ;  //  7: 5     
	UINT32 use_of_hysterisis                  : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 t_filter_weight                    : 8 ;  // 23:16     
} PE_H13B0_P0R_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x25d0 p0r_bnr_ac_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_delta_th0                      : 8 ;  //  7: 0     
	UINT32 max_delta_th1                      : 8 ;  // 15: 8     
	UINT32 h_blockness_th                     : 8 ;  // 23:16     
	UINT32 h_weight_max                       : 8 ;  // 31:24     
} PE_H13B0_P0R_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x25d4 p0r_bnr_ac_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_gain3                   : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_gain2                   : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_gain1                   : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_gain0                   : 8 ;  // 31:24     
} PE_H13B0_P0R_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x25d8 p0r_dnr_max_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_ac_bnr                      : 1 ;  //     0     
	UINT32 enable_mnr                         : 1 ;  //     1     
	UINT32 enable_dc_bnr                      : 1 ;  //     2     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 debug_enable                       : 1 ;  //     8     
	UINT32 debug_mode                         : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 win_control_enable                 : 1 ;  //    16     
	UINT32 border_enable                      : 1 ;  //    17     
	UINT32 reg_win_inout                      : 1 ;  //    18     
} PE_H13B0_P0R_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
		0x25dc p0r_dnr_dbar_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                       : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 show_debug_bar_for_bnr             : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 gain                               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sd_hd_sel                          : 2 ;  //  9: 8     
} PE_H13B0_P0R_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0x25e0 p0r_bnr_stat_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                    : 8 ;  //  7: 0     
	UINT32 ac_bnr_v_status                    : 8 ;  // 15: 8     
	UINT32 dc_bnr_th                          : 14;  // 29:16     
} PE_H13B0_P0R_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
		0x25e4 p0r_cti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32 cti_clipping_mode                  : 1 ;  //     1     
	UINT32 cti_pre_flt_mode                   : 2 ;  //  3: 2     
	UINT32 cti_tap                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
	UINT32 cti_debug_mode                     : 3 ;  // 18:16     
} PE_H13B0_P0R_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0x25f0 p0r_ipc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cubic_coef0                    : 8 ;  //  7: 0     
	UINT32 reg_cubic_coef1                    : 8 ;  // 15: 8     
	UINT32 reg_va_blending_ctrl_lv1           : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_va_blending_ctrl_lv2           : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_va_blending_ctrl_lv3           : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_va_blending_ctrl_lv4           : 3 ;  // 30:28     
} PE_H13B0_P0R_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0x25f4 p0r_ipc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_cubic                       : 1 ;  //     0     
	UINT32 reg_boundary_window                : 1 ;  //     1     
	UINT32 reg_force_va                       : 1 ;  //     2     
	UINT32 reg_half_pel_va                    : 1 ;  //     3     
	UINT32 reg_dir_med_tap                    : 2 ;  //  5: 4     
	UINT32 reg_diff_med_tap                   : 2 ;  //  7: 6     
	UINT32 reg_va_blending_ctrl1              : 3 ;  // 10: 8     
	UINT32 reg_wide_angle_protection_1        : 1 ;  //    11     
	UINT32 reg_va_blending_ctrl2              : 3 ;  // 14:12     
	UINT32 reg_wide_angle_protection_2        : 1 ;  //    15     
	UINT32 reg_errs_chroma_blend_coef         : 8 ;  // 23:16     
	UINT32 reg_strong_global_th               : 6 ;  // 29:24     
	UINT32 narrow_angle_week                  : 1 ;  //    30     
	UINT32 narrow_angle_protection            : 1 ;  //    31     
} PE_H13B0_P0R_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0x25f8 p0r_ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0     
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8     
} PE_H13B0_P0R_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0x25fc p0r_ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0     
	UINT32 cr_lock_th                         : 7 ;  //  7: 1     
	UINT32 cr_g_motion_th                     : 8 ;  // 15: 8     
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16     
	UINT32 cr_hmc_half_en                     : 1 ;  //    19
	UINT32 cr_robustness                      : 4 ;  // 23:20     
	UINT32 cr_center_weight                   : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0x2600 p0r_ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8     
	UINT32 cr_gds_resolution                  : 3 ;  // 18:16     
} PE_H13B0_P0R_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0x2604 p0r_ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8     
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16     
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0x2608 p0r_ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0x260c p0r_ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0     
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8     
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16     
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0x2610 p0r_ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0R_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0x2614 p0r_ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16     
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24     
} PE_H13B0_P0R_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0x2618 p0r_ipc_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmc_motion_th                  : 8 ;  //  7: 0     
	UINT32 reg_vmc_debug                      : 1 ;  //     8     
	UINT32 reg_vmc_en                         : 1 ;  //     9     
	UINT32 reg_vmc_still_check                : 1 ;  //    10     
	UINT32 reg_vmc_mul                        : 3 ;  // 13:11     
	UINT32 reg_vmc_out_mul                    : 2 ;  // 15:14     
	UINT32 reg_vmc_protect_th                 : 8 ;  // 23:16     
	UINT32 reg_protection_en                  : 1 ;  //    24     
} PE_H13B0_P0R_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0x261c p0r_ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} PE_H13B0_P0R_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0x2628 p0r_ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_lr_count                      : 16;  // 15: 0     
	UINT32 r_pc_lr_count                      : 16;  // 31:16     
} PE_H13B0_P0R_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0x262c p0r_ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_lr_same                    : 16;  // 15: 0     
	UINT32 r_pndiff_count                     : 16;  // 31:16     
} PE_H13B0_P0R_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0x2630 p0r_ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_lr_same                    : 16;  // 15: 0     
	UINT32 r_pc_pn_lr_same                    : 16;  // 31:16     
} PE_H13B0_P0R_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0x2634 p0r_ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0     
	UINT32 r_inv_original_cnt                 : 16;  // 31:16     
} PE_H13B0_P0R_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0x2638 p0r_ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0R_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0x263c p0r_ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0R_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0x2640 p0r_tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_show_scale                     : 3 ;  //  2: 0     
	UINT32 reg_bbd_show                       : 1 ;  //     3     
	UINT32 reg_show_bar_1                     : 1 ;  //     4     
	UINT32 reg_show_bar_2                     : 1 ;  //     5     
	UINT32 reg_show_bar_3                     : 1 ;  //     6     
	UINT32 reg_show_bar_4                     : 1 ;  //     7     
	UINT32 reg_bar1_data_mux                  : 2 ;  //  9: 8     
	UINT32 reg_bar2_data_mux                  : 2 ;  // 11:10     
	UINT32 reg_bar3_data_mux                  : 2 ;  // 13:12     
	UINT32 reg_bar4_data_mux                  : 2 ;  // 15:14     
	UINT32 reg_hist0_data_mux                 : 4 ;  // 19:16     
	UINT32 reg_hist1_data_mux                 : 4 ;  // 23:20     
	UINT32 reg_hist2_data_mux                 : 4 ;  // 27:24     
	UINT32 reg_hist0_win_sel                  : 1 ;  //    28     
	UINT32 reg_hist1_win_sel                  : 1 ;  //    29     
	UINT32 reg_hist2_win_sel                  : 1 ;  //    30     
	UINT32 hist_rd_en                         : 1 ;  //    31     
} PE_H13B0_P0R_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x2644 p0r_tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15     
	UINT32 wina_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31     
} PE_H13B0_P0R_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x2648 p0r_tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30     
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31     
} PE_H13B0_P0R_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x264c p0r_tpd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winb_hen                       : 1 ;  //    15     
	UINT32 winb_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winb_ven                       : 1 ;  //    31     
} PE_H13B0_P0R_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x2650 p0r_tpd_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winb_y1                            : 12;  // 27:16     
} PE_H13B0_P0R_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x2654 p0r_tpd_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_y                        : 10;  //  9: 0     
	UINT32 reg_force_y_max_th                 : 10;  // 19:10     
	UINT32 reg_force_y_min_th                 : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_y_enable                 : 1 ;  //    31     
} PE_H13B0_P0R_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x2658 p0r_tpd_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cb                       : 10;  //  9: 0     
	UINT32 reg_force_cb_max_th                : 10;  // 19:10     
	UINT32 reg_force_cb_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cb_enable                : 1 ;  //    31     
} PE_H13B0_P0R_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x265c p0r_tpd_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cr                       : 10;  //  9: 0     
	UINT32 reg_force_cr_max_th                : 10;  // 19:10     
	UINT32 reg_force_cr_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cr_enable                : 1 ;  //    31     
} PE_H13B0_P0R_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x2660 p0r_tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                   : 22;  // 21: 0
} PE_H13B0_P0R_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x2670 p0r_bbd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0     
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1     
	UINT32 reg_op_mode                        : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8     
	UINT32 reg_diff_th                        : 8 ;  // 23:16     
	UINT32 reg_bbd_mux                        : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_apl_mux                        : 3 ;  // 30:28     
} PE_H13B0_P0R_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x2674 p0r_bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13B0_P0R_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x2678 p0r_bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0R_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x267c p0r_bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13B0_P0R_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x2680 p0r_bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13B0_P0R_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0x2684 p0r_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0     
	UINT32 cb_b_s_apl                         : 10;  // 19:10     
	UINT32 cr_r_s_apl                         : 10;  // 29:20     
} PE_H13B0_P0R_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x2690 p0r_tpd_hist_ai_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32 reg_read_hist_sel                  : 2 ;  // 14:13     
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_available                  : 1 ;  //    28     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 hist_read_done                     : 1 ;  //    31     
} PE_H13B0_P0R_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
		0x2694 p0r_tpd_hist_ai_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0R_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
		0x2698 p0r_lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16     
	UINT32 y_gain                             : 8 ;  // 31:24     
} PE_H13B0_P0R_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x26d0 p0r_tnr_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_coring_en_c                      : 1 ;  //     0     
	UINT32 m_coring_en_y                      : 1 ;  //     1     
	UINT32 mc_sad_sel                         : 1 ;  //     2     
	UINT32 sad_protect_en                     : 1 ;  //     3     
	UINT32 m_debug_mode                       : 3 ;  //  6: 4     
	UINT32 m_debug_mode_en                    : 1 ;  //     7     
	UINT32 m_gain_c                           : 4 ;  // 11: 8     
	UINT32 m_gain_y                           : 4 ;  // 15:12     
	UINT32 m_coring_th                        : 6 ;  // 21:16     
	UINT32 sad_protect_gm_en                  : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 mv_cost_gmv_smooth_gain            : 5 ;  // 28:24     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 mv_cost_gmv_smooth_en              : 1 ;  //    31     
} PE_H13B0_P0R_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0x26d4 p0r_tnr_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 secam_mot_c_th                     : 8 ;  //  7: 0     
	UINT32 secam_mot_y_th                     : 8 ;  // 15: 8     
	UINT32 secam_c_v_th                       : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 secam_c_v_calc_mode                : 2 ;  // 31:30     
} PE_H13B0_P0R_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0x26d8 p0r_tnr_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_en                              : 1 ;  //     0     
	UINT32 sf_map_flt_en                      : 1 ;  //     1     
	UINT32 sf_debug_mode                      : 3 ;  //  4: 2     
	UINT32 sf_map_tap                         : 2 ;  //  6: 5     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sf_map_gain                        : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 sf_th0                             : 8 ;  // 23:16     
	UINT32 sf_th1                             : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0x26dc p0r_tnr_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_th2                             : 8 ;  //  7: 0     
	UINT32 sf_th3                             : 8 ;  // 15: 8     
	UINT32 sf_th4                             : 8 ;  // 23:16     
	UINT32 sf_f0_sel                          : 2 ;  // 25:24     
	UINT32 sf_f1_sel                          : 2 ;  // 27:26     
	UINT32 sf_f2_sel                          : 2 ;  // 29:28     
	UINT32 sf_f3_sel                          : 2 ;  // 31:30     
} PE_H13B0_P0R_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0x26e0 p0r_tnr_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y0                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x0                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y1                           : 8 ;  // 23:16     
	UINT32 sf_mb_x1                           : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0x26e4 p0r_tnr_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13B0_P0R_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0x26e8 p0r_tnr_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y2                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x2                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y3                           : 8 ;  // 23:16     
	UINT32 sf_mb_x3                           : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0x26ec p0r_tnr_ctrl_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 sf_debug_map_sel                   : 2 ;  // 17:16     
	UINT32 sf_gain_mode                       : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sf_gain                            : 4 ;  // 23:20     
} PE_H13B0_P0R_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
		0x26f0 p0r_tnr_ctrl_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_blend_y0                      : 8 ;  //  7: 0     
	UINT32 mamc_blend_x0                      : 8 ;  // 15: 8     
	UINT32 mamc_blend_y1                      : 8 ;  // 23:16     
	UINT32 mamc_blend_x1                      : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
		0x26f4 p0r_tnr_ctrl_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_lock_th0                       : 6 ;  //  5: 0     
	UINT32 gmv_max_cnt_th                     : 2 ;  //  7: 6     
	UINT32 gmv_lock_th1                       : 6 ;  // 13: 8     
	UINT32 gmv_val_cnt_th                     : 2 ;  // 15:14     
	UINT32 gmv_manual_mv_en                   : 1 ;  //    16     
	UINT32 gmv_manual_alpha_en                : 1 ;  //    17     
	UINT32                                    : 1 ;  //    18     reserved
	UINT32 gmv_lock_en                        : 1 ;  //    19     
	UINT32 gmv_lock_max                       : 4 ;  // 23:20     
	UINT32 gmv_manual_mv                      : 4 ;  // 27:24     
	UINT32 gmv_manual_alpha                   : 4 ;  // 31:28     
} PE_H13B0_P0R_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
		0x26f8 p0r_tnr_ctrl_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_gain_ctrl2_y0                    : 8 ;  //  7: 0     
	UINT32 m_gain_ctrl2_x0                    : 8 ;  // 15: 8     
	UINT32 m_gain_ctrl2_y1                    : 8 ;  // 23:16     
	UINT32 m_gain_ctrl2_x1                    : 8 ;  // 31:24     
} PE_H13B0_P0R_TNR_CTRL_48_T;

typedef struct {
	PE_H13B0_P0L_PE0_LOAD_T            	p0l_pe0_load;                 //0x400
	PE_H13B0_P0L_PE0_INTR_T            	p0l_pe0_intr;                 //0x404
	PE_H13B0_P0L_PE0_STATUS0_T         	p0l_pe0_status0;              //0x408
	PE_H13B0_P0L_PE0_STATUS1_T         	p0l_pe0_status1;              //0x40c
	PE_H13B0_P0L_PE0_OPMODE_T          	p0l_pe0_opmode;               //0x410
	PE_H13B0_P0L_PE0_SRC_SIZE_T        	p0l_pe0_src_size;             //0x414
	PE_H13B0_P0L_PE0_FIELD_T           	p0l_pe0_field;                //0x418
	PE_H13B0_P0L_PE0_PARAM_BY_DDR_T    	p0l_pe0_param_by_ddr;         //0x41c
	PE_H13B0_P0L_WIN_CTRL_0_T          	p0l_win_ctrl_0;               //0x420
	PE_H13B0_P0L_WIN_CTRL_1_T          	p0l_win_ctrl_1;               //0x424
	PE_H13B0_P0L_WIN_CTRL_2_T          	p0l_win_ctrl_2;               //0x428
	PE_H13B0_P0L_WIN_CTRL_3_T          	p0l_win_ctrl_3;               //0x42c
	PE_H13B0_P0L_PE0_DEBUG_MEM_REQ_T   	p0l_pe0_debug_mem_req;        //0x430
	PE_H13B0_P0L_PE0_DEBUG_LREQ_DLY_T  	p0l_pe0_debug_lreq_dly;       //0x434
	UINT32                             	reserved00[2];                //0x438~0x43c
	PE_H13B0_P0L_TNR_CTRL_00_T         	p0l_tnr_ctrl_00;              //0x440
	PE_H13B0_P0L_TNR_CTRL_01_T         	p0l_tnr_ctrl_01;              //0x444
	PE_H13B0_P0L_TNR_CTRL_02_T         	p0l_tnr_ctrl_02;              //0x448
	PE_H13B0_P0L_TNR_CTRL_03_T         	p0l_tnr_ctrl_03;              //0x44c
	PE_H13B0_P0L_TNR_CTRL_04_T         	p0l_tnr_ctrl_04;              //0x450
	PE_H13B0_P0L_TNR_CTRL_05_T         	p0l_tnr_ctrl_05;              //0x454
	PE_H13B0_P0L_TNR_CTRL_06_T         	p0l_tnr_ctrl_06;              //0x458
	PE_H13B0_P0L_TNR_CTRL_07_T         	p0l_tnr_ctrl_07;              //0x45c
	PE_H13B0_P0L_TNR_CTRL_08_T         	p0l_tnr_ctrl_08;              //0x460
	PE_H13B0_P0L_TNR_CTRL_09_T         	p0l_tnr_ctrl_09;              //0x464
	PE_H13B0_P0L_TNR_CTRL_10_T         	p0l_tnr_ctrl_10;              //0x468
	PE_H13B0_P0L_TNR_CTRL_11_T         	p0l_tnr_ctrl_11;              //0x46c
	PE_H13B0_P0L_TNR_CTRL_12_T         	p0l_tnr_ctrl_12;              //0x470
	PE_H13B0_P0L_TNR_CTRL_13_T         	p0l_tnr_ctrl_13;              //0x474
	PE_H13B0_P0L_TNR_CTRL_14_T         	p0l_tnr_ctrl_14;              //0x478
	PE_H13B0_P0L_TNR_CTRL_15_T         	p0l_tnr_ctrl_15;              //0x47c
	PE_H13B0_P0L_TNR_CTRL_16_T         	p0l_tnr_ctrl_16;              //0x480
	PE_H13B0_P0L_TNR_CTRL_17_T         	p0l_tnr_ctrl_17;              //0x484
	PE_H13B0_P0L_TNR_CTRL_18_T         	p0l_tnr_ctrl_18;              //0x488
	PE_H13B0_P0L_TNR_CTRL_19_T         	p0l_tnr_ctrl_19;              //0x48c
	PE_H13B0_P0L_TNR_CTRL_20_T         	p0l_tnr_ctrl_20;              //0x490
	PE_H13B0_P0L_TNR_CTRL_21_T         	p0l_tnr_ctrl_21;              //0x494
	PE_H13B0_P0L_TNR_CTRL_22_T         	p0l_tnr_ctrl_22;              //0x498
	PE_H13B0_P0L_TNR_CTRL_23_T         	p0l_tnr_ctrl_23;              //0x49c
	PE_H13B0_P0L_TNR_CTRL_24_T         	p0l_tnr_ctrl_24;              //0x4a0
	PE_H13B0_P0L_TNR_CTRL_25_T         	p0l_tnr_ctrl_25;              //0x4a4
	PE_H13B0_P0L_TNR_CTRL_26_T         	p0l_tnr_ctrl_26;              //0x4a8
	PE_H13B0_P0L_TNR_CTRL_27_T         	p0l_tnr_ctrl_27;              //0x4ac
	PE_H13B0_P0L_TNR_CTRL_28_T         	p0l_tnr_ctrl_28;              //0x4b0
	PE_H13B0_P0L_TNR_CTRL_29_T         	p0l_tnr_ctrl_29;              //0x4b4
	PE_H13B0_P0L_TNR_CTRL_30_T         	p0l_tnr_ctrl_30;              //0x4b8
	PE_H13B0_P0L_TNR_CTRL_31_T         	p0l_tnr_ctrl_31;              //0x4bc
	PE_H13B0_P0L_TNR_STATUS_00_T       	p0l_tnr_status_00;            //0x4c0
	PE_H13B0_P0L_TNR_STATUS_01_T       	p0l_tnr_status_01;            //0x4c4
	PE_H13B0_P0L_TNR_STATUS_02_T       	p0l_tnr_status_02;            //0x4c8
	PE_H13B0_P0L_TNR_STATUS_03_T       	p0l_tnr_status_03;            //0x4cc
	PE_H13B0_P0L_TNR_MAIN_LUT_00_T     	p0l_tnr_main_lut_00;          //0x4d0
	PE_H13B0_P0L_TNR_MAIN_LUT_01_T     	p0l_tnr_main_lut_01;          //0x4d4
	PE_H13B0_P0L_TNR_MAIN_LUT_02_T     	p0l_tnr_main_lut_02;          //0x4d8
	PE_H13B0_P0L_TNR_STATUS_04_T        p0l_tnr_status_04;            //0x4dc
	PE_H13B0_P0L_TNR_CTRL_32_T         	p0l_tnr_ctrl_32;              //0x4e0
	PE_H13B0_P0L_TNR_CTRL_33_T         	p0l_tnr_ctrl_33;              //0x4e4
	PE_H13B0_P0L_TNR_CTRL_34_T         	p0l_tnr_ctrl_34;              //0x4e8
	PE_H13B0_P0L_TNR_CTRL_35_T         	p0l_tnr_ctrl_35;              //0x4ec
	PE_H13B0_P0L_TNR_CRG_CTRL_00_T     	p0l_tnr_crg_ctrl_00;          //0x4f0
	PE_H13B0_P0L_TNR_CRG_CTRL_01_T     	p0l_tnr_crg_ctrl_01;          //0x4f4
	PE_H13B0_P0L_TNR_CTRL_36_T         	p0l_tnr_ctrl_36;              //0x4f8
	PE_H13B0_P0L_TNR_CTRL_37_T         	p0l_tnr_ctrl_37;              //0x4fc
	PE_H13B0_P0L_IPC_CTRL_00_T         	p0l_ipc_ctrl_00;              //0x500
	PE_H13B0_P0L_IPC_CTRL_01_T         	p0l_ipc_ctrl_01;              //0x504
	PE_H13B0_P0L_IPC_CTRL_02_T         	p0l_ipc_ctrl_02;              //0x508
	PE_H13B0_P0L_IPC_CTRL_03_T         	p0l_ipc_ctrl_03;              //0x50c
	PE_H13B0_P0L_IPC_CTRL_04_T         	p0l_ipc_ctrl_04;              //0x510
	PE_H13B0_P0L_IPC_CTRL_05_T         	p0l_ipc_ctrl_05;              //0x514
	PE_H13B0_P0L_IPC_CTRL_06_T         	p0l_ipc_ctrl_06;              //0x518
	PE_H13B0_P0L_IPC_CTRL_07_T         	p0l_ipc_ctrl_07;              //0x51c
	PE_H13B0_P0L_IPC_CTRL_08_T         	p0l_ipc_ctrl_08;              //0x520
	PE_H13B0_P0L_IPC_CTRL_09_T         	p0l_ipc_ctrl_09;              //0x524
	PE_H13B0_P0L_IPC_CTRL_10_T         	p0l_ipc_ctrl_10;              //0x528
	PE_H13B0_P0L_IPC_CTRL_11_T         	p0l_ipc_ctrl_11;              //0x52c
	PE_H13B0_P0L_IPC_CTRL_12_T         	p0l_ipc_ctrl_12;              //0x530
	PE_H13B0_P0L_IPC_CTRL_13_T         	p0l_ipc_ctrl_13;              //0x534
	PE_H13B0_P0L_IPC_CTRL_14_T         	p0l_ipc_ctrl_14;              //0x538
	PE_H13B0_P0L_IPC_CTRL_15_T         	p0l_ipc_ctrl_15;              //0x53c
	PE_H13B0_P0L_IPC_CTRL_16_T         	p0l_ipc_ctrl_16;              //0x540
	PE_H13B0_P0L_IPC_CTRL_17_T         	p0l_ipc_ctrl_17;              //0x544
	PE_H13B0_P0L_IPC_CTRL_18_T         	p0l_ipc_ctrl_18;              //0x548
	PE_H13B0_P0L_IPC_CTRL_19_T         	p0l_ipc_ctrl_19;              //0x54c
	PE_H13B0_P0L_IPC_STATUS_00_T       	p0l_ipc_status_00;            //0x550
	PE_H13B0_P0L_IPC_STATUS_01_T       	p0l_ipc_status_01;            //0x554
	PE_H13B0_P0L_IPC_STATUS_02_T       	p0l_ipc_status_02;            //0x558
	PE_H13B0_P0L_IPC_STATUS_03_T       	p0l_ipc_status_03;            //0x55c
	PE_H13B0_P0L_CLC_CTRL_00_T         	p0l_clc_ctrl_00;              //0x560
	PE_H13B0_P0L_CLC_CTRL_01_T         	p0l_clc_ctrl_01;              //0x564
	PE_H13B0_P0L_CLC_CTRL_02_T         	p0l_clc_ctrl_02;              //0x568
	PE_H13B0_P0L_CLC_CTRL_03_T         	p0l_clc_ctrl_03;              //0x56c
	PE_H13B0_P0L_CLC_CTRL_04_T         	p0l_clc_ctrl_04;              //0x570
	PE_H13B0_P0L_CLC_CTRL_05_T         	p0l_clc_ctrl_05;              //0x574
	PE_H13B0_P0L_CLC_CTRL_06_T         	p0l_clc_ctrl_06;              //0x578
	PE_H13B0_P0L_CLC_STAT_00_T         	p0l_clc_stat_00;              //0x57c
	PE_H13B0_P0L_VFLT_CTRL_T           	p0l_vflt_ctrl;                //0x580
	PE_H13B0_P0L_CTI_CTRL_1_T          	p0l_cti_ctrl_1;               //0x584
	PE_H13B0_P0L_MNR_CTRL_00_T         	p0l_mnr_ctrl_00;              //0x588
	PE_H13B0_P0L_MNR_CTRL_01_T         	p0l_mnr_ctrl_01;              //0x58c
	PE_H13B0_P0L_MNR_CTRL_02_T         	p0l_mnr_ctrl_02;              //0x590
	PE_H13B0_P0L_MNR_CTRL_03_T         	p0l_mnr_ctrl_03;              //0x594
	PE_H13B0_P0L_MNR_CTRL_04_T         	p0l_mnr_ctrl_04;              //0x598
	PE_H13B0_P0L_MNR_CTRL_05_T         	p0l_mnr_ctrl_05;              //0x59c
	UINT32                             	reserved01;                   //0x5a0
	PE_H13B0_P0L_VFILTER_02_T          	p0l_vfilter_02;               //0x5a4
	PE_H13B0_P0L_VFILTER_01_T          	p0l_vfilter_01;               //0x5a8
	PE_H13B0_P0L_BNR_DC_CTRL_00_T      	p0l_bnr_dc_ctrl_00;           //0x5ac
	PE_H13B0_P0L_BNR_DC_CTRL_01_T      	p0l_bnr_dc_ctrl_01;           //0x5b0
	PE_H13B0_P0L_BNR_DC_CTRL_02_T      	p0l_bnr_dc_ctrl_02;           //0x5b4
	PE_H13B0_P0L_BNR_DC_CTRL_03_T      	p0l_bnr_dc_ctrl_03;           //0x5b8
	PE_H13B0_P0L_BNR_AC_CTRL_06_T      	p0l_bnr_ac_ctrl_06;           //0x5bc
	PE_H13B0_P0L_BNR_AC_CTRL_00_T      	p0l_bnr_ac_ctrl_00;           //0x5c0
	PE_H13B0_P0L_BNR_AC_CTRL_01_T      	p0l_bnr_ac_ctrl_01;           //0x5c4
	PE_H13B0_P0L_BNR_AC_CTRL_02_T      	p0l_bnr_ac_ctrl_02;           //0x5c8
	PE_H13B0_P0L_BNR_AC_CTRL_03_T      	p0l_bnr_ac_ctrl_03;           //0x5cc
	PE_H13B0_P0L_BNR_AC_CTRL_04_T      	p0l_bnr_ac_ctrl_04;           //0x5d0
	PE_H13B0_P0L_BNR_AC_CTRL_05_T      	p0l_bnr_ac_ctrl_05;           //0x5d4
	PE_H13B0_P0L_DNR_MAX_CTRL_T        	p0l_dnr_max_ctrl;             //0x5d8
	PE_H13B0_P0L_DNR_DBAR_CTRL_T       	p0l_dnr_dbar_ctrl;            //0x5dc
	PE_H13B0_P0L_BNR_STAT_0_T          	p0l_bnr_stat_0;               //0x5e0
	PE_H13B0_P0L_CTI_CTRL_0_T          	p0l_cti_ctrl_0;               //0x5e4
	PE_H13B0_P0L_REG_C420_CTRL_T       	p0l_reg_c420_ctrl;            //0x5e8
	UINT32                             	reserved02;                   //0x5ec
	PE_H13B0_P0L_IPC_CTRL_20_T         	p0l_ipc_ctrl_20;              //0x5f0
	PE_H13B0_P0L_IPC_CTRL_21_T         	p0l_ipc_ctrl_21;              //0x5f4
	PE_H13B0_P0L_IPC_CTRL_22_T         	p0l_ipc_ctrl_22;              //0x5f8
	PE_H13B0_P0L_IPC_CTRL_23_T         	p0l_ipc_ctrl_23;              //0x5fc
	PE_H13B0_P0L_IPC_CTRL_24_T         	p0l_ipc_ctrl_24;              //0x600
	PE_H13B0_P0L_IPC_CTRL_25_T         	p0l_ipc_ctrl_25;              //0x604
	PE_H13B0_P0L_IPC_CTRL_26_T         	p0l_ipc_ctrl_26;              //0x608
	PE_H13B0_P0L_IPC_CTRL_27_T         	p0l_ipc_ctrl_27;              //0x60c
	PE_H13B0_P0L_IPC_CTRL_28_T         	p0l_ipc_ctrl_28;              //0x610
	PE_H13B0_P0L_IPC_CTRL_29_T         	p0l_ipc_ctrl_29;              //0x614
	PE_H13B0_P0L_IPC_CTRL_30_T         	p0l_ipc_ctrl_30;              //0x618
	PE_H13B0_P0L_IPC_CTRL_31_T         	p0l_ipc_ctrl_31;              //0x61c
	UINT32                             	reserved03[2];                //0x620~0x624
	PE_H13B0_P0L_IPC_STATUS_04_T       	p0l_ipc_status_04;            //0x628
	PE_H13B0_P0L_IPC_STATUS_05_T       	p0l_ipc_status_05;            //0x62c
	PE_H13B0_P0L_IPC_STATUS_06_T       	p0l_ipc_status_06;            //0x630
	PE_H13B0_P0L_IPC_STATUS_07_T       	p0l_ipc_status_07;            //0x634
	PE_H13B0_P0L_IPC_STATUS_08_T       	p0l_ipc_status_08;            //0x638
	PE_H13B0_P0L_IPC_STATUS_09_T       	p0l_ipc_status_09;            //0x63c
	PE_H13B0_P0L_TPD_CTRL_00_T         	p0l_tpd_ctrl_00;              //0x640
	PE_H13B0_P0L_TPD_CTRL_01_T         	p0l_tpd_ctrl_01;              //0x644
	PE_H13B0_P0L_TPD_CTRL_02_T         	p0l_tpd_ctrl_02;              //0x648
	PE_H13B0_P0L_TPD_CTRL_03_T         	p0l_tpd_ctrl_03;              //0x64c
	PE_H13B0_P0L_TPD_CTRL_04_T         	p0l_tpd_ctrl_04;              //0x650
	PE_H13B0_P0L_TPD_CTRL_05_T         	p0l_tpd_ctrl_05;              //0x654
	PE_H13B0_P0L_TPD_CTRL_06_T         	p0l_tpd_ctrl_06;              //0x658
	PE_H13B0_P0L_TPD_CTRL_07_T         	p0l_tpd_ctrl_07;              //0x65c
	PE_H13B0_P0L_TPD_STAT_00_T         	p0l_tpd_stat_00;              //0x660
	UINT32                             	reserved04[2];                //0x664~0x668
	PE_H13B0_P0L_CLC_CTRL_07_T         	p0l_clc_ctrl_07;              //0x66c
	PE_H13B0_P0L_BBD_CTRL_00_T         	p0l_bbd_ctrl_00;              //0x670
	PE_H13B0_P0L_BBD_CTRL_01_T         	p0l_bbd_ctrl_01;              //0x674
	PE_H13B0_P0L_BBD_CTRL_02_T         	p0l_bbd_ctrl_02;              //0x678
	PE_H13B0_P0L_BBD_STAT_00_T         	p0l_bbd_stat_00;              //0x67c
	PE_H13B0_P0L_BBD_STAT_01_T         	p0l_bbd_stat_01;              //0x680
	PE_H13B0_P0L_APL_STAT_00_T         	p0l_apl_stat_00;              //0x684
	UINT32                             	reserved05[2];                //0x688~0x68c
	PE_H13B0_P0L_TPD_HIST_AI_CTRL_T    	p0l_tpd_hist_ai_ctrl;         //0x690
	PE_H13B0_P0L_TPD_HIST_AI_DATA_T    	p0l_tpd_hist_ai_data;         //0x694
	PE_H13B0_P0L_LVCRTL_CTRL_00_T      	p0l_lvcrtl_ctrl_00;           //0x698
	UINT32                             	reserved06;                   //0x69c
	PE_H13B0_P0L_3DFD_CTRL_00_T        	p0l_3dfd_ctrl_00;             //0x6a0
	PE_H13B0_P0L_3DFD_CTRL_01_T        	p0l_3dfd_ctrl_01;             //0x6a4
	PE_H13B0_P0L_3DFD_CTRL_02_T        	p0l_3dfd_ctrl_02;             //0x6a8
	PE_H13B0_P0L_3DFD_CTRL_03_T        	p0l_3dfd_ctrl_03;             //0x6ac
	PE_H13B0_P0L_3DFD_STAT_00_T        	p0l_3dfd_stat_00;             //0x6b0
	PE_H13B0_P0L_3DFD_STAT_01_T        	p0l_3dfd_stat_01;             //0x6b4
	PE_H13B0_P0L_3DFD_STAT_02_T        	p0l_3dfd_stat_02;             //0x6b8
	PE_H13B0_P0L_3DFD_STAT_03_T        	p0l_3dfd_stat_03;             //0x6bc
	PE_H13B0_P0L_3DFD_STAT_04_T        	p0l_3dfd_stat_04;             //0x6c0
	PE_H13B0_P0L_3DFD_STAT_05_T        	p0l_3dfd_stat_05;             //0x6c4
	PE_H13B0_P0L_3DFD_STAT_06_T        	p0l_3dfd_stat_06;             //0x6c8
	PE_H13B0_P0L_3DFD_STAT_07_T        	p0l_3dfd_stat_07;             //0x6cc
	PE_H13B0_P0L_TNR_CTRL_38_T         	p0l_tnr_ctrl_38;              //0x6d0
	PE_H13B0_P0L_TNR_CTRL_39_T         	p0l_tnr_ctrl_39;              //0x6d4
	PE_H13B0_P0L_TNR_CTRL_40_T         	p0l_tnr_ctrl_40;              //0x6d8
	PE_H13B0_P0L_TNR_CTRL_41_T         	p0l_tnr_ctrl_41;              //0x6dc
	PE_H13B0_P0L_TNR_CTRL_42_T         	p0l_tnr_ctrl_42;              //0x6e0
	PE_H13B0_P0L_TNR_CTRL_43_T         	p0l_tnr_ctrl_43;              //0x6e4
	PE_H13B0_P0L_TNR_CTRL_44_T         	p0l_tnr_ctrl_44;              //0x6e8
	PE_H13B0_P0L_TNR_CTRL_45_T         	p0l_tnr_ctrl_45;              //0x6ec
	PE_H13B0_P0L_TNR_CTRL_46_T         	p0l_tnr_ctrl_46;              //0x6f0
	PE_H13B0_P0L_TNR_CTRL_47_T         	p0l_tnr_ctrl_47;              //0x6f4
	PE_H13B0_P0L_TNR_CTRL_48_T         	p0l_tnr_ctrl_48;              //0x6f8
}PE_P0L_REG_H13B0_T;

typedef struct {
	PE_H13B0_P0R_PE0_LOAD_T            	p0r_pe0_load;                 //0x2400
	PE_H13B0_P0R_PE0_INTR_T            	p0r_pe0_intr;                 //0x2404
	PE_H13B0_P0R_PE0_STATUS0_T         	p0r_pe0_status0;              //0x2408
	PE_H13B0_P0R_PE0_STATUS1_T         	p0r_pe0_status1;              //0x240c
	PE_H13B0_P0R_PE0_OPMODE_T          	p0r_pe0_opmode;               //0x2410
	PE_H13B0_P0R_PE0_SRC_SIZE_T        	p0r_pe0_src_size;             //0x2414
	PE_H13B0_P0R_PE0_FIELD_T           	p0r_pe0_field;                //0x2418
	PE_H13B0_P0R_PE0_PARAM_BY_DDR_T    	p0r_pe0_param_by_ddr;         //0x241c
	PE_H13B0_P0R_WIN_CTRL_0_T          	p0r_win_ctrl_0;               //0x2420
	PE_H13B0_P0R_WIN_CTRL_1_T          	p0r_win_ctrl_1;               //0x2424
	PE_H13B0_P0R_WIN_CTRL_2_T          	p0r_win_ctrl_2;               //0x2428
	PE_H13B0_P0R_WIN_CTRL_3_T          	p0r_win_ctrl_3;               //0x242c
	PE_H13B0_P0R_PE0_DEBUG_MEM_REQ_T   	p0r_pe0_debug_mem_req;        //0x2430
	PE_H13B0_P0R_PE0_DEBUG_LREQ_DLY_T  	p0r_pe0_debug_lreq_dly;       //0x2434
	UINT32                             	reserved00[2];                //0x2438~0x243c
	PE_H13B0_P0R_TNR_CTRL_00_T         	p0r_tnr_ctrl_00;              //0x2440
	PE_H13B0_P0R_TNR_CTRL_01_T         	p0r_tnr_ctrl_01;              //0x2444
	PE_H13B0_P0R_TNR_CTRL_02_T         	p0r_tnr_ctrl_02;              //0x2448
	PE_H13B0_P0R_TNR_CTRL_03_T         	p0r_tnr_ctrl_03;              //0x244c
	PE_H13B0_P0R_TNR_CTRL_04_T         	p0r_tnr_ctrl_04;              //0x2450
	PE_H13B0_P0R_TNR_CTRL_05_T         	p0r_tnr_ctrl_05;              //0x2454
	PE_H13B0_P0R_TNR_CTRL_06_T         	p0r_tnr_ctrl_06;              //0x2458
	PE_H13B0_P0R_TNR_CTRL_07_T         	p0r_tnr_ctrl_07;              //0x245c
	PE_H13B0_P0R_TNR_CTRL_08_T         	p0r_tnr_ctrl_08;              //0x2460
	PE_H13B0_P0R_TNR_CTRL_09_T         	p0r_tnr_ctrl_09;              //0x2464
	PE_H13B0_P0R_TNR_CTRL_10_T         	p0r_tnr_ctrl_10;              //0x2468
	PE_H13B0_P0R_TNR_CTRL_11_T         	p0r_tnr_ctrl_11;              //0x246c
	PE_H13B0_P0R_TNR_CTRL_12_T         	p0r_tnr_ctrl_12;              //0x2470
	PE_H13B0_P0R_TNR_CTRL_13_T         	p0r_tnr_ctrl_13;              //0x2474
	PE_H13B0_P0R_TNR_CTRL_14_T         	p0r_tnr_ctrl_14;              //0x2478
	PE_H13B0_P0R_TNR_CTRL_15_T         	p0r_tnr_ctrl_15;              //0x247c
	PE_H13B0_P0R_TNR_CTRL_16_T         	p0r_tnr_ctrl_16;              //0x2480
	PE_H13B0_P0R_TNR_CTRL_17_T         	p0r_tnr_ctrl_17;              //0x2484
	PE_H13B0_P0R_TNR_CTRL_18_T         	p0r_tnr_ctrl_18;              //0x2488
	PE_H13B0_P0R_TNR_CTRL_19_T         	p0r_tnr_ctrl_19;              //0x248c
	PE_H13B0_P0R_TNR_CTRL_20_T         	p0r_tnr_ctrl_20;              //0x2490
	PE_H13B0_P0R_TNR_CTRL_21_T         	p0r_tnr_ctrl_21;              //0x2494
	PE_H13B0_P0R_TNR_CTRL_22_T         	p0r_tnr_ctrl_22;              //0x2498
	PE_H13B0_P0R_TNR_CTRL_23_T         	p0r_tnr_ctrl_23;              //0x249c
	PE_H13B0_P0R_TNR_CTRL_24_T         	p0r_tnr_ctrl_24;              //0x24a0
	PE_H13B0_P0R_TNR_CTRL_25_T         	p0r_tnr_ctrl_25;              //0x24a4
	PE_H13B0_P0R_TNR_CTRL_26_T         	p0r_tnr_ctrl_26;              //0x24a8
	PE_H13B0_P0R_TNR_CTRL_27_T         	p0r_tnr_ctrl_27;              //0x24ac
	PE_H13B0_P0R_TNR_CTRL_28_T         	p0r_tnr_ctrl_28;              //0x24b0
	PE_H13B0_P0R_TNR_CTRL_29_T         	p0r_tnr_ctrl_29;              //0x24b4
	PE_H13B0_P0R_TNR_CTRL_30_T         	p0r_tnr_ctrl_30;              //0x24b8
	PE_H13B0_P0R_TNR_CTRL_31_T         	p0r_tnr_ctrl_31;              //0x24bc
	PE_H13B0_P0R_TNR_STATUS_00_T       	p0r_tnr_status_00;            //0x24c0
	PE_H13B0_P0R_TNR_STATUS_01_T       	p0r_tnr_status_01;            //0x24c4
	PE_H13B0_P0R_TNR_STATUS_02_T       	p0r_tnr_status_02;            //0x24c8
	PE_H13B0_P0R_TNR_STATUS_03_T       	p0r_tnr_status_03;            //0x24cc
	PE_H13B0_P0R_TNR_MAIN_LUT_00_T     	p0r_tnr_main_lut_00;          //0x24d0
	PE_H13B0_P0R_TNR_MAIN_LUT_01_T     	p0r_tnr_main_lut_01;          //0x24d4
	PE_H13B0_P0R_TNR_MAIN_LUT_02_T     	p0r_tnr_main_lut_02;          //0x24d8
	PE_H13B0_P0R_TNR_STATUS_04_T        p0r_tnr_status_04;            //0x24dc
	PE_H13B0_P0R_TNR_CTRL_32_T         	p0r_tnr_ctrl_32;              //0x24e0
	PE_H13B0_P0R_TNR_CTRL_33_T         	p0r_tnr_ctrl_33;              //0x24e4
	PE_H13B0_P0R_TNR_CTRL_34_T         	p0r_tnr_ctrl_34;              //0x24e8
	PE_H13B0_P0R_TNR_CTRL_35_T         	p0r_tnr_ctrl_35;              //0x24ec
	PE_H13B0_P0R_TNR_CRG_CTRL_00_T     	p0r_tnr_crg_ctrl_00;          //0x24f0
	PE_H13B0_P0R_TNR_CRG_CTRL_01_T     	p0r_tnr_crg_ctrl_01;          //0x24f4
	PE_H13B0_P0R_TNR_CTRL_36_T         	p0r_tnr_ctrl_36;              //0x24f8
	PE_H13B0_P0R_TNR_CTRL_37_T         	p0r_tnr_ctrl_37;              //0x24fc
	PE_H13B0_P0R_IPC_CTRL_00_T         	p0r_ipc_ctrl_00;              //0x2500
	PE_H13B0_P0R_IPC_CTRL_01_T         	p0r_ipc_ctrl_01;              //0x2504
	PE_H13B0_P0R_IPC_CTRL_02_T         	p0r_ipc_ctrl_02;              //0x2508
	PE_H13B0_P0R_IPC_CTRL_03_T         	p0r_ipc_ctrl_03;              //0x250c
	PE_H13B0_P0R_IPC_CTRL_04_T         	p0r_ipc_ctrl_04;              //0x2510
	PE_H13B0_P0R_IPC_CTRL_05_T         	p0r_ipc_ctrl_05;              //0x2514
	PE_H13B0_P0R_IPC_CTRL_06_T         	p0r_ipc_ctrl_06;              //0x2518
	PE_H13B0_P0R_IPC_CTRL_07_T         	p0r_ipc_ctrl_07;              //0x251c
	PE_H13B0_P0R_IPC_CTRL_08_T         	p0r_ipc_ctrl_08;              //0x2520
	PE_H13B0_P0R_IPC_CTRL_09_T         	p0r_ipc_ctrl_09;              //0x2524
	PE_H13B0_P0R_IPC_CTRL_10_T         	p0r_ipc_ctrl_10;              //0x2528
	PE_H13B0_P0R_IPC_CTRL_11_T         	p0r_ipc_ctrl_11;              //0x252c
	PE_H13B0_P0R_IPC_CTRL_12_T         	p0r_ipc_ctrl_12;              //0x2530
	PE_H13B0_P0R_IPC_CTRL_13_T         	p0r_ipc_ctrl_13;              //0x2534
	PE_H13B0_P0R_IPC_CTRL_14_T         	p0r_ipc_ctrl_14;              //0x2538
	PE_H13B0_P0R_IPC_CTRL_15_T         	p0r_ipc_ctrl_15;              //0x253c
	PE_H13B0_P0R_IPC_CTRL_16_T         	p0r_ipc_ctrl_16;              //0x2540
	PE_H13B0_P0R_IPC_CTRL_17_T         	p0r_ipc_ctrl_17;              //0x2544
	PE_H13B0_P0R_IPC_CTRL_18_T         	p0r_ipc_ctrl_18;              //0x2548
	PE_H13B0_P0R_IPC_CTRL_19_T         	p0r_ipc_ctrl_19;              //0x254c
	PE_H13B0_P0R_IPC_STATUS_00_T       	p0r_ipc_status_00;            //0x2550
	PE_H13B0_P0R_IPC_STATUS_01_T       	p0r_ipc_status_01;            //0x2554
	PE_H13B0_P0R_IPC_STATUS_02_T       	p0r_ipc_status_02;            //0x2558
	PE_H13B0_P0R_IPC_STATUS_03_T       	p0r_ipc_status_03;            //0x255c
	UINT32                             	reserved01[8];                //0x2560~0x257c
	PE_H13B0_P0R_VFILTER_00_T          	p0r_vfilter_00;               //0x2580
	PE_H13B0_P0R_CTI_CTRL_1_T          	p0r_cti_ctrl_1;               //0x2584
	PE_H13B0_P0R_MNR_CTRL_00_T         	p0r_mnr_ctrl_00;              //0x2588
	PE_H13B0_P0R_MNR_CTRL_01_T         	p0r_mnr_ctrl_01;              //0x258c
	PE_H13B0_P0R_MNR_CTRL_02_T         	p0r_mnr_ctrl_02;              //0x2590
	PE_H13B0_P0R_MNR_CTRL_03_T         	p0r_mnr_ctrl_03;              //0x2594
	PE_H13B0_P0R_MNR_CTRL_04_T         	p0r_mnr_ctrl_04;              //0x2598
	PE_H13B0_P0R_MNR_CTRL_05_T         	p0r_mnr_ctrl_05;              //0x259c
	UINT32                             	reserved02;                   //0x25a0
	PE_H13B0_P0R_VFILTER_02_T          	p0r_vfilter_02;               //0x25a4
	PE_H13B0_P0R_VFILTER_01_T          	p0r_vfilter_01;               //0x25a8
	PE_H13B0_P0R_BNR_DC_CTRL_00_T      	p0r_bnr_dc_ctrl_00;           //0x25ac
	PE_H13B0_P0R_BNR_DC_CTRL_01_T      	p0r_bnr_dc_ctrl_01;           //0x25b0
	PE_H13B0_P0R_BNR_DC_CTRL_02_T      	p0r_bnr_dc_ctrl_02;           //0x25b4
	PE_H13B0_P0R_BNR_DC_CTRL_03_T      	p0r_bnr_dc_ctrl_03;           //0x25b8
	PE_H13B0_P0R_BNR_AC_CTRL_06_T      	p0r_bnr_ac_ctrl_06;           //0x25bc
	PE_H13B0_P0R_BNR_AC_CTRL_00_T      	p0r_bnr_ac_ctrl_00;           //0x25c0
	PE_H13B0_P0R_BNR_AC_CTRL_01_T      	p0r_bnr_ac_ctrl_01;           //0x25c4
	PE_H13B0_P0R_BNR_AC_CTRL_02_T      	p0r_bnr_ac_ctrl_02;           //0x25c8
	PE_H13B0_P0R_BNR_AC_CTRL_03_T      	p0r_bnr_ac_ctrl_03;           //0x25cc
	PE_H13B0_P0R_BNR_AC_CTRL_04_T      	p0r_bnr_ac_ctrl_04;           //0x25d0
	PE_H13B0_P0R_BNR_AC_CTRL_05_T      	p0r_bnr_ac_ctrl_05;           //0x25d4
	PE_H13B0_P0R_DNR_MAX_CTRL_T        	p0r_dnr_max_ctrl;             //0x25d8
	PE_H13B0_P0R_DNR_DBAR_CTRL_T       	p0r_dnr_dbar_ctrl;            //0x25dc
	PE_H13B0_P0R_BNR_STAT_0_T          	p0r_bnr_stat_0;               //0x25e0
	PE_H13B0_P0R_CTI_CTRL_0_T          	p0r_cti_ctrl_0;               //0x25e4
	UINT32                             	reserved03[2];                //0x25e8~0x25ec
	PE_H13B0_P0R_IPC_CTRL_20_T         	p0r_ipc_ctrl_20;              //0x25f0
	PE_H13B0_P0R_IPC_CTRL_21_T         	p0r_ipc_ctrl_21;              //0x25f4
	PE_H13B0_P0R_IPC_CTRL_22_T         	p0r_ipc_ctrl_22;              //0x25f8
	PE_H13B0_P0R_IPC_CTRL_23_T         	p0r_ipc_ctrl_23;              //0x25fc
	PE_H13B0_P0R_IPC_CTRL_24_T         	p0r_ipc_ctrl_24;              //0x2600
	PE_H13B0_P0R_IPC_CTRL_25_T         	p0r_ipc_ctrl_25;              //0x2604
	PE_H13B0_P0R_IPC_CTRL_26_T         	p0r_ipc_ctrl_26;              //0x2608
	PE_H13B0_P0R_IPC_CTRL_27_T         	p0r_ipc_ctrl_27;              //0x260c
	PE_H13B0_P0R_IPC_CTRL_28_T         	p0r_ipc_ctrl_28;              //0x2610
	PE_H13B0_P0R_IPC_CTRL_29_T         	p0r_ipc_ctrl_29;              //0x2614
	PE_H13B0_P0R_IPC_CTRL_30_T         	p0r_ipc_ctrl_30;              //0x2618
	PE_H13B0_P0R_IPC_CTRL_31_T         	p0r_ipc_ctrl_31;              //0x261c
	UINT32                             	reserved04[2];                //0x2620~0x2624
	PE_H13B0_P0R_IPC_STATUS_04_T       	p0r_ipc_status_04;            //0x2628
	PE_H13B0_P0R_IPC_STATUS_05_T       	p0r_ipc_status_05;            //0x262c
	PE_H13B0_P0R_IPC_STATUS_06_T       	p0r_ipc_status_06;            //0x2630
	PE_H13B0_P0R_IPC_STATUS_07_T       	p0r_ipc_status_07;            //0x2634
	PE_H13B0_P0R_IPC_STATUS_08_T       	p0r_ipc_status_08;            //0x2638
	PE_H13B0_P0R_IPC_STATUS_09_T       	p0r_ipc_status_09;            //0x263c
	PE_H13B0_P0R_TPD_CTRL_00_T         	p0r_tpd_ctrl_00;              //0x2640
	PE_H13B0_P0R_TPD_CTRL_01_T         	p0r_tpd_ctrl_01;              //0x2644
	PE_H13B0_P0R_TPD_CTRL_02_T         	p0r_tpd_ctrl_02;              //0x2648
	PE_H13B0_P0R_TPD_CTRL_03_T         	p0r_tpd_ctrl_03;              //0x264c
	PE_H13B0_P0R_TPD_CTRL_04_T         	p0r_tpd_ctrl_04;              //0x2650
	PE_H13B0_P0R_TPD_CTRL_05_T         	p0r_tpd_ctrl_05;              //0x2654
	PE_H13B0_P0R_TPD_CTRL_06_T         	p0r_tpd_ctrl_06;              //0x2658
	PE_H13B0_P0R_TPD_CTRL_07_T         	p0r_tpd_ctrl_07;              //0x265c
	PE_H13B0_P0R_TPD_STAT_00_T         	p0r_tpd_stat_00;              //0x2660
	UINT32                             	reserved05[3];                //0x2664~0x266c
	PE_H13B0_P0R_BBD_CTRL_00_T         	p0r_bbd_ctrl_00;              //0x2670
	PE_H13B0_P0R_BBD_CTRL_01_T         	p0r_bbd_ctrl_01;              //0x2674
	PE_H13B0_P0R_BBD_CTRL_02_T         	p0r_bbd_ctrl_02;              //0x2678
	PE_H13B0_P0R_BBD_STAT_00_T         	p0r_bbd_stat_00;              //0x267c
	PE_H13B0_P0R_BBD_STAT_01_T         	p0r_bbd_stat_01;              //0x2680
	PE_H13B0_P0R_APL_STAT_00_T         	p0r_apl_stat_00;              //0x2684
	UINT32                             	reserved06[2];                //0x2688~0x268c
	PE_H13B0_P0R_TPD_HIST_AI_CTRL_T    	p0r_tpd_hist_ai_ctrl;         //0x2690
	PE_H13B0_P0R_TPD_HIST_AI_DATA_T    	p0r_tpd_hist_ai_data;         //0x2694
	PE_H13B0_P0R_LVCRTL_CTRL_00_T      	p0r_lvcrtl_ctrl_00;           //0x2698
	UINT32                             	reserved07[13];               //0x269c~0x26cc
	PE_H13B0_P0R_TNR_CTRL_38_T         	p0r_tnr_ctrl_38;              //0x26d0
	PE_H13B0_P0R_TNR_CTRL_39_T         	p0r_tnr_ctrl_39;              //0x26d4
	PE_H13B0_P0R_TNR_CTRL_40_T         	p0r_tnr_ctrl_40;              //0x26d8
	PE_H13B0_P0R_TNR_CTRL_41_T         	p0r_tnr_ctrl_41;              //0x26dc
	PE_H13B0_P0R_TNR_CTRL_42_T         	p0r_tnr_ctrl_42;              //0x26e0
	PE_H13B0_P0R_TNR_CTRL_43_T         	p0r_tnr_ctrl_43;              //0x26e4
	PE_H13B0_P0R_TNR_CTRL_44_T         	p0r_tnr_ctrl_44;              //0x26e8
	PE_H13B0_P0R_TNR_CTRL_45_T         	p0r_tnr_ctrl_45;              //0x26ec
	PE_H13B0_P0R_TNR_CTRL_46_T         	p0r_tnr_ctrl_46;              //0x26f0
	PE_H13B0_P0R_TNR_CTRL_47_T         	p0r_tnr_ctrl_47;              //0x26f4
	PE_H13B0_P0R_TNR_CTRL_48_T         	p0r_tnr_ctrl_48;              //0x26f8
}PE_P0R_REG_H13B0_T;

#endif
