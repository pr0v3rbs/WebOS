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

#ifndef _PE_PE0_REG_M14A0_H_
#define _PE_PE0_REG_M14A0_H_
/*-----------------------------------------------------------------------------
		0xc0013400L p0l_pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} PE_M14A0_P0L_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc0013404L p0l_pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16     
} PE_M14A0_P0L_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0xc0013408L p0l_pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                       : 4 ;  //  3: 0     
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4     
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8     
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12     
	UINT32 dnr_frame_id                       : 8 ;  // 23:16     
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24     
} PE_M14A0_P0L_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc001340cL p0l_pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0     
} PE_M14A0_P0L_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc0013410L p0l_pe0_opmode ''
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
} PE_M14A0_P0L_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc0013414L p0l_pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0     
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 cs_type                            : 3 ;  // 15:13     
	UINT32 vsize                              : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31     
} PE_M14A0_P0L_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0013418L p0l_pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 secam_line                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12     
} PE_M14A0_P0L_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc001341cL p0l_pe0_param_by_ddr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 9 ;  //  8: 0     reserved
	UINT32 stride                             : 2 ;  // 10: 9     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 vsize                              : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 endian                             : 1 ;  //    30     
	UINT32 update_by_ddr                      : 1 ;  //    31     
} PE_M14A0_P0L_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
		0xc0013420L p0l_win_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_M14A0_P0L_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0013424L p0l_win_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0L_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0013428L p0l_win_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32 ac_bnr_feature_cal_mode            : 2 ;  // 29:28     
} PE_M14A0_P0L_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc001342cL p0l_win_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0L_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0013430L p0l_pe0_debug_mem_req ''
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
} PE_M14A0_P0L_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0xc0013434L p0l_pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0     
	UINT32 lreq_dly_c_en                      : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16     
	UINT32 lreq_dly_y_en                      : 1 ;  //    27     
} PE_M14A0_P0L_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0013440L p0l_tnr_ctrl_00 ''
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
} PE_M14A0_P0L_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0013444L p0l_tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                            : 8 ;  //  7: 0     
	UINT32 y_th_hi                            : 8 ;  // 15: 8     
	UINT32 dnr_motion_scale                   : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 tnr_filter_lsb_ctrl                : 2 ;  // 21:20     
	UINT32 svar_mux_ctrl                      : 2 ;  // 23:22     
	UINT32 nt_lvl_hist_n_pct                  : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0013448L p0l_tnr_ctrl_02 ''
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
} PE_M14A0_P0L_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001344cL p0l_tnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                              : 8 ;  //  7: 0     
	UINT32 wa_y1                              : 8 ;  // 15: 8     
	UINT32 wa_x0                              : 8 ;  // 23:16     
	UINT32 wa_x1                              : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0013450L p0l_tnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x0                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x1                  : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0013454L p0l_tnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                             : 8 ;  //  7: 0     
	UINT32 nt_max                             : 8 ;  // 15: 8     
	UINT32 hist_svar_th                       : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th0               : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0013458L p0l_tnr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                          : 8 ;  //  7: 0     
	UINT32 nt_iir_alpha                       : 8 ;  // 15: 8     
	UINT32 y_th_lo2                           : 8 ;  // 23:16     
	UINT32 y_th_hi2                           : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc001345cL p0l_tnr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind1                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind2                     : 8 ;  // 23:16     
	UINT32 alpha_lut_ind3                     : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0013460L p0l_tnr_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind5                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind6                     : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th1               : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0013464L p0l_tnr_ctrl_09 ''
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
} PE_M14A0_P0L_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0013468L p0l_tnr_ctrl_10 ''
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
} PE_M14A0_P0L_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc001346cL p0l_tnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                    : 8 ;  //  7: 0     
	UINT32 dbg_alpha                          : 8 ;  // 15: 8     
	UINT32 peak0_x_th                         : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 scene_change_th_mv                 : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0013470L p0l_tnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x2                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x3                  : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0013474L p0l_tnr_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 force_nt_lvl_val                   : 8 ;  // 15: 8     
	UINT32 adjust_nt_lvl                      : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 adjust_nt_lvl_val                  : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0013478L p0l_tnr_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 noise_measure_win_en               : 1 ;  //    31     
} PE_M14A0_P0L_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc001347cL p0l_tnr_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0L_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0013480L p0l_tnr_ctrl_16 ''
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
} PE_M14A0_P0L_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0013484L p0l_tnr_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y0                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x0                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y1                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x1                     : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0013488L p0l_tnr_ctrl_18 ''
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
} PE_M14A0_P0L_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc001348cL p0l_tnr_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_x                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_x                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_x                     : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0013490L p0l_tnr_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_y                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_y                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_y                     : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc0013494L p0l_tnr_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_mamc_blend                     : 8 ;  //  7: 0     
} PE_M14A0_P0L_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc0013498L p0l_tnr_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_protect_control_y0              : 8 ;  //  7: 0     
	UINT32 mv_protect_control_x0              : 8 ;  // 15: 8     
	UINT32 mv_protect_control_y1              : 8 ;  // 23:16     
	UINT32 mv_protect_control_x1              : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc001349cL p0l_tnr_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x1                 : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc00134a0L p0l_tnr_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x3                 : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc00134a4L p0l_tnr_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x1                 : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc00134a8L p0l_tnr_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x3                 : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc00134acL p0l_tnr_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                     : 8 ;  //  7: 0     
	UINT32 crgn_gain_rgn1                     : 8 ;  // 15: 8     
	UINT32 crgn_gain_rgn2                     : 8 ;  // 23:16     
	UINT32 crgn_gain_rgn3                     : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc00134b0L p0l_tnr_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x0                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x1                  : 7 ;  // 30:24     
} PE_M14A0_P0L_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc00134b4L p0l_tnr_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                              : 8 ;  //  7: 0     
	UINT32 wa_y3                              : 8 ;  // 15: 8     
	UINT32 wa_x2                              : 8 ;  // 23:16     
	UINT32 wa_x3                              : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc00134b8L p0l_tnr_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x2                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x3                  : 7 ;  // 30:24     
} PE_M14A0_P0L_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc00134bcL p0l_tnr_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c               : 1 ;  //     0     
	UINT32 spatial_pre_flt_en_y               : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 spatial_pre_flt_th_c               : 8 ;  // 23:16     
	UINT32 spatial_pre_flt_th_y               : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc00134c0L p0l_tnr_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                            : 8 ;  //  7: 0     
	UINT32 x_peak_t2                          : 8 ;  // 15: 8     
	UINT32 nt_lvl                             : 8 ;  // 23:16     
	UINT32 x_avg_s                            : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc00134c4L p0l_tnr_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sum_valid                          : 16;  // 15: 0     
	UINT32 scene_diff                         : 16;  // 31:16     
} PE_M14A0_P0L_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc00134c8L p0l_tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_l                               : 4 ;  //  3: 0     
	UINT32 mv_r                               : 4 ;  //  7: 4     
	UINT32 dfd                                : 8 ;  // 15: 8     
	UINT32 apl_y                              : 8 ;  // 23:16     
	UINT32 lp                                 : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc00134ccL p0l_tnr_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_valid_cnt                      : 16;  // 15: 0     
	UINT32 fd                                 : 8 ;  // 23:16     
	UINT32 gmv_lock_cnt                       : 6 ;  // 29:24     
} PE_M14A0_P0L_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc00134d0L p0l_tnr_main_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 main_lut_ai_enable                 : 1 ;  //    12     
} PE_M14A0_P0L_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
		0xc00134d4L p0l_tnr_main_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0               : 32;  // 31: 0     
} PE_M14A0_P0L_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
		0xc00134d8L p0l_tnr_main_lut_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1               : 32;  // 31: 0     
} PE_M14A0_P0L_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
		0xc00134dcL p0l_tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_r_cnt                           : 16;  // 15: 0     
	UINT32 mv_l_cnt                           : 16;  // 31:16     
} PE_M14A0_P0L_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc00134e0L p0l_tnr_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x0                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y1                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x1                : 7 ;  // 30:24     
} PE_M14A0_P0L_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc00134e4L p0l_tnr_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x2                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y3                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x3                : 7 ;  // 30:24     
} PE_M14A0_P0L_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc00134e8L p0l_tnr_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_ctrl_y0                    : 8 ;  //  7: 0     
	UINT32 mc_blur_ctrl_x0                    : 8 ;  // 15: 8     
	UINT32 mc_blur_ctrl_y1                    : 8 ;  // 23:16     
	UINT32 mc_blur_ctrl_x1                    : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc00134ecL p0l_tnr_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y2                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x2                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y3                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x3                     : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc00134f0L p0l_tnr_crg_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr               : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 crgn_ctrl_ai_enable                : 1 ;  //    12     
} PE_M14A0_P0L_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00134f4L p0l_tnr_crg_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data               : 32;  // 31: 0     
} PE_M14A0_P0L_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00134f8L p0l_tnr_ctrl_36 ''
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
} PE_M14A0_P0L_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc00134fcL p0l_tnr_ctrl_37 ''
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
} PE_M14A0_P0L_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc0013500L p0l_ipc_ctrl_00 ''
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
} PE_M14A0_P0L_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0013504L p0l_ipc_ctrl_01 ''
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
} PE_M14A0_P0L_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0013508L p0l_ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001350cL p0l_ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0013510L p0l_ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0     
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8     
	UINT32 tearing_gain                       : 8 ;  // 23:16     
	UINT32 motion_gain                        : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0013514L p0l_ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha              : 8 ;  //  7: 0     
	UINT32 cr_t_grad_weight                   : 8 ;  // 15: 8     
	UINT32 cr_v_grad_weight                   : 8 ;  // 23:16     
	UINT32 cr_h_grad_weight                   : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0013518L p0l_ipc_ctrl_06 ''
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
} PE_M14A0_P0L_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc001351cL p0l_ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1     
	UINT32 cr_45_en                           : 1 ;  //     2     
	UINT32 cr_15_en                           : 1 ;  //     3     
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 tnr_motion_gain                    : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0013520L p0l_ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30     
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31     
} PE_M14A0_P0L_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0013524L p0l_ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16     
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0013528L p0l_ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16     
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc001352cL p0l_ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0     
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8     
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16     
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0013530L p0l_ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0013534L p0l_ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    16     
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 23:19     
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 29:24     
} PE_M14A0_P0L_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0013538L p0l_ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reg_manual_en                      : 1 ;  //     1     
	UINT32 reg_manual_mode                    : 2 ;  //  3: 2     
	UINT32 reg_lock_speed                     : 4 ;  //  7: 4     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc001353cL p0l_ipc_ctrl_15 ''
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
} PE_M14A0_P0L_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0013540L p0l_ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32 reg_same_min_ratio                 : 4 ;  // 27:24     
	UINT32 reg_same_adder_gain                : 4 ;  // 31:28     
} PE_M14A0_P0L_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0013544L p0l_ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0     
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2     
	UINT32 cr_field_inverse                   : 1 ;  //     4     
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0013548L p0l_ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0     
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8     
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16     
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc001354cL p0l_ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0     
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8     
	UINT32 reg_2d_offset                      : 8 ;  // 23:16     
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0013550L p0l_ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                       : 16;  // 15: 0     
	UINT32 r_nc_v_count                       : 16;  // 31:16     
} PE_M14A0_P0L_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc0013554L p0l_ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                       : 16;  // 15: 0     
	UINT32 r_pn_v_count                       : 16;  // 31:16     
} PE_M14A0_P0L_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc0013558L p0l_ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_same                       : 16;  // 15: 0     
	UINT32 r_pc_pn_same                       : 16;  // 31:16     
} PE_M14A0_P0L_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc001355cL p0l_ipc_status_03 ''
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
} PE_M14A0_P0L_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc0013560L p0l_clc_ctrl_00 ''
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
} PE_M14A0_P0L_CLC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0013564L p0l_clc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_motion_a_th                     : 8 ;  //  7: 0     
	UINT32 cl_motion_b_th                     : 8 ;  // 15: 8     
	UINT32 cl_y_protect_th0                   : 8 ;  // 23:16     
	UINT32 cl_y_protect_th1                   : 8 ;  // 31:24     
} PE_M14A0_P0L_CLC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0013568L p0l_clc_ctrl_02 ''
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
} PE_M14A0_P0L_CLC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001356cL p0l_clc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_sat_min                         : 8 ;  //  7: 0     
	UINT32                                    : 2 ;  //  9: 8     reserved
	UINT32 cl_var_measure_th1                 : 10;  // 19:10     
	UINT32 cl_var_measure_th2                 : 10;  // 29:20     
	UINT32 cl_pt_enable                       : 2 ;  // 31:30     
} PE_M14A0_P0L_CLC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0013570L p0l_clc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_tearing_pt_th1                  : 12;  // 11: 0     
	UINT32                                    : 8 ;  // 19:12     reserved
	UINT32 cl_frame_min                       : 12;  // 31:20     
} PE_M14A0_P0L_CLC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0013574L p0l_clc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_data_line_switching_sel         : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 clc_bin_flt_th                     : 3 ;  // 10: 8     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 clc_bin_flt_en                     : 1 ;  //    15     
	UINT32 clc_dbg_show_mask                  : 13;  // 28:16     
} PE_M14A0_P0L_CLC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0013578L p0l_clc_ctrl_06 ''
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
} PE_M14A0_P0L_CLC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc001357cL p0l_clc_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 cl_detection_count                 : 16;  // 31:16     
} PE_M14A0_P0L_CLC_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0013580L p0l_vflt_ctrl ''
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
} PE_M14A0_P0L_VFLT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013584L p0l_cti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                     : 8 ;  //  7: 0     
	UINT32 cti_coring_th1                     : 8 ;  // 15: 8     
	UINT32 cti_coring_smooth                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 cti_coring_map_size                : 2 ;  // 21:20     
} PE_M14A0_P0L_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0013588L p0l_mnr_ctrl_00 ''
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
} PE_M14A0_P0L_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc001358cL p0l_mnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_s1_mmd_min                 : 8 ;  //  7: 0     
	UINT32 reg_mnr_s2_ratio_min               : 8 ;  // 15: 8     
	UINT32 reg_mnr_s2_ratio_max               : 8 ;  // 23:16     
	UINT32 reg_mnr_s2_mmd_min                 : 8 ;  // 31:24     
} PE_M14A0_P0L_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0013590L p0l_mnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_mnr_h_gain                     : 8 ;  // 23:16     
	UINT32 reg_mnr_v_gain                     : 8 ;  // 31:24     
} PE_M14A0_P0L_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0013594L p0l_mnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_filter_x0                      : 8 ;  //  7: 0     
	UINT32 reg_filter_x1                      : 8 ;  // 15: 8     
	UINT32 reg_filter_y0                      : 8 ;  // 23:16     
	UINT32 reg_filter_y1                      : 8 ;  // 31:24     
} PE_M14A0_P0L_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0013598L p0l_mnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_en                  : 1 ;  //     0     
	UINT32 reg_motion_mnr_filter              : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 reg_mnr_motion_min                 : 8 ;  // 23:16     
	UINT32 reg_mnr_motion_max                 : 8 ;  // 31:24     
} PE_M14A0_P0L_MNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc001359cL p0l_mnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_x0                  : 8 ;  //  7: 0     
	UINT32 reg_motion_mnr_x1                  : 8 ;  // 15: 8     
	UINT32 reg_motion_mnr_y0                  : 8 ;  // 23:16     
	UINT32 reg_motion_mnr_y1                  : 8 ;  // 31:24     
} PE_M14A0_P0L_MNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00135a4L p0l_vfilter_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable              : 1 ;  //     0     
	UINT32 cr_hmc_protect_only_hmc_line       : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 cr_hmc_protect_motion_th           : 8 ;  // 15: 8     
	UINT32 cr_hmc_protect_th_max              : 8 ;  // 23:16     
	UINT32 cr_hmc_protect_th_min              : 8 ;  // 31:24     
} PE_M14A0_P0L_VFILTER_02_T;

/*-----------------------------------------------------------------------------
		0xc00135a8L p0l_vfilter_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en               : 1 ;  //     0     
	UINT32 cr_vfilter_force_en                : 1 ;  //     1     
	UINT32 cr_global_frame_num_th             : 4 ;  //  5: 2     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cr_vfilter_global_th               : 12;  // 19: 8     
	UINT32 tearing_count                      : 12;  // 31:20     
} PE_M14A0_P0L_VFILTER_01_T;

/*-----------------------------------------------------------------------------
		0xc00135acL p0l_bnr_dc_ctrl_00 ''
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
} PE_M14A0_P0L_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00135b0L p0l_bnr_dc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_var_th3                 : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_var_th2                 : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_var_th1                 : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_var_th0                 : 8 ;  // 31:24     
} PE_M14A0_P0L_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00135b4L p0l_bnr_dc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_motion_th3              : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_motion_th2              : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_motion_th1              : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_motion_th0              : 8 ;  // 31:24     
} PE_M14A0_P0L_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00135b8L p0l_bnr_dc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_chroma_variance             : 8 ;  //  7: 0     
	UINT32 reg_dc_var_h_gain                  : 8 ;  // 15: 8     
	UINT32 reg_dc_protection_th               : 8 ;  // 23:16     
} PE_M14A0_P0L_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00135bcL p0l_bnr_ac_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_hys_mode                    : 4 ;  //  3: 0     
	UINT32 reg_sc_bdr_type                    : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 ac_bnr_protect_motion_max          : 8 ;  // 23:16     
	UINT32 ac_bnr_protect_motion_min          : 8 ;  // 31:24     
} PE_M14A0_P0L_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc00135c0L p0l_bnr_ac_ctrl_00 ''
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
} PE_M14A0_P0L_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00135c4L p0l_bnr_ac_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_h_x0                      : 8 ;  //  7: 0     
	UINT32 strength_h_x1                      : 8 ;  // 15: 8     
	UINT32 strength_h_max                     : 8 ;  // 23:16     
	UINT32 detect_min_th                      : 8 ;  // 31:24     
} PE_M14A0_P0L_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00135c8L p0l_bnr_ac_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_v_x0                      : 8 ;  //  7: 0     
	UINT32 strength_v_x1                      : 8 ;  // 15: 8     
	UINT32 strength_v_max                     : 8 ;  // 23:16     
} PE_M14A0_P0L_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00135ccL p0l_bnr_ac_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset_mode                      : 1 ;  //     0     
	UINT32 manual_offset_h_value              : 3 ;  //  3: 1     
	UINT32 v_offset_mode                      : 1 ;  //     4     
	UINT32 manual_offset_v_value              : 3 ;  //  7: 5     
	UINT32 use_of_hysterisis                  : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 t_filter_weight                    : 8 ;  // 23:16     
} PE_M14A0_P0L_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00135d0L p0l_bnr_ac_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_delta_th0                      : 8 ;  //  7: 0     
	UINT32 max_delta_th1                      : 8 ;  // 15: 8     
	UINT32 h_blockness_th                     : 8 ;  // 23:16     
	UINT32 h_weight_max                       : 8 ;  // 31:24     
} PE_M14A0_P0L_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00135d4L p0l_bnr_ac_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_gain3                   : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_gain2                   : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_gain1                   : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_gain0                   : 8 ;  // 31:24     
} PE_M14A0_P0L_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00135d8L p0l_dnr_max_ctrl ''
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
} PE_M14A0_P0L_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00135dcL p0l_dnr_dbar_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                       : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 show_debug_bar_for_bnr             : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 gain                               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sd_hd_sel                          : 2 ;  //  9: 8     
} PE_M14A0_P0L_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00135e0L p0l_bnr_stat_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                    : 8 ;  //  7: 0     
	UINT32 ac_bnr_v_status                    : 8 ;  // 15: 8     
	UINT32 chroma_420_result                  : 16;  // 31:16     
} PE_M14A0_P0L_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
		0xc00135e4L p0l_cti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32 cti_clipping_mode                  : 1 ;  //     1     
	UINT32 cti_pre_flt_mode                   : 2 ;  //  3: 2     
	UINT32 cti_tap                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
	UINT32 cti_debug_mode                     : 3 ;  // 18:16     
} PE_M14A0_P0L_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc00135e8L p0l_reg_c420_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tearing_count_th               : 8 ;  //  7: 0     
	UINT32 reg_frame_count_th                 : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_detect_level_th                : 10;  // 25:16     
} PE_M14A0_P0L_REG_C420_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00135f0L p0l_ipc_ctrl_20 ''
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
} PE_M14A0_P0L_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc00135f4L p0l_ipc_ctrl_21 ''
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
} PE_M14A0_P0L_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc00135f8L p0l_ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0     
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8     
} PE_M14A0_P0L_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc00135fcL p0l_ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0     
	UINT32 cr_lock_th                         : 7 ;  //  7: 1     
	UINT32 cr_g_motion_th                     : 8 ;  // 15: 8     
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16     
	UINT32 cr_hmc_half_en                     : 1 ;  //    19     
	UINT32 cr_robustness                      : 4 ;  // 23:20     
	UINT32 cr_center_weight                   : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc0013600L p0l_ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8     
	UINT32 cr_gds_resolution                  : 3 ;  // 18:16     
} PE_M14A0_P0L_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc0013604L p0l_ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8     
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16     
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc0013608L p0l_ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc001360cL p0l_ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0     
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8     
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16     
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc0013610L p0l_ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hmc_bonus_es_en                : 1 ;  //     0     
	UINT32 reg_hmc_bonus_et_en                : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_hmc_bonus_gain                 : 8 ;  // 15: 8     
	UINT32 reg_hmc_bonus_th                   : 8 ;  // 23:16     
} PE_M14A0_P0L_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc0013614L p0l_ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16     
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24     
} PE_M14A0_P0L_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc0013618L p0l_ipc_ctrl_30 ''
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
} PE_M14A0_P0L_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc001361cL p0l_ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} PE_M14A0_P0L_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc0013628L p0l_ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_lr_count                      : 16;  // 15: 0     
	UINT32 r_pc_lr_count                      : 16;  // 31:16     
} PE_M14A0_P0L_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc001362cL p0l_ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_lr_same                    : 16;  // 15: 0     
	UINT32 r_pndiff_count                     : 16;  // 31:16     
} PE_M14A0_P0L_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc0013630L p0l_ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_lr_same                    : 16;  // 15: 0     
	UINT32 r_pc_pn_lr_same                    : 16;  // 31:16     
} PE_M14A0_P0L_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc0013634L p0l_ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0     
	UINT32 r_inv_original_cnt                 : 16;  // 31:16     
} PE_M14A0_P0L_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc0013638L p0l_ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0L_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc001363cL p0l_ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0L_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0xc0013640L p0l_tpd_ctrl_00 ''
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
} PE_M14A0_P0L_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0013644L p0l_tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15     
	UINT32 wina_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31     
} PE_M14A0_P0L_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0013648L p0l_tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30     
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31     
} PE_M14A0_P0L_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001364cL p0l_tpd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winb_hen                       : 1 ;  //    15     
	UINT32 winb_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winb_ven                       : 1 ;  //    31     
} PE_M14A0_P0L_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0013650L p0l_tpd_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winb_y1                            : 12;  // 27:16     
} PE_M14A0_P0L_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0013654L p0l_tpd_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_y                        : 10;  //  9: 0     
	UINT32 reg_force_y_max_th                 : 10;  // 19:10     
	UINT32 reg_force_y_min_th                 : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_y_enable                 : 1 ;  //    31     
} PE_M14A0_P0L_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0013658L p0l_tpd_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cb                       : 10;  //  9: 0     
	UINT32 reg_force_cb_max_th                : 10;  // 19:10     
	UINT32 reg_force_cb_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cb_enable                : 1 ;  //    31     
} PE_M14A0_P0L_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc001365cL p0l_tpd_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cr                       : 10;  //  9: 0     
	UINT32 reg_force_cr_max_th                : 10;  // 19:10     
	UINT32 reg_force_cr_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cr_enable                : 1 ;  //    31     
} PE_M14A0_P0L_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0013660L p0l_tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                   : 22;  // 21: 0
} PE_M14A0_P0L_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc001366cL p0l_clc_ctrl_07 ''
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
} PE_M14A0_P0L_CLC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0013670L p0l_bbd_ctrl_00 ''
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
} PE_M14A0_P0L_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0013674L p0l_bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_M14A0_P0L_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0013678L p0l_bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0L_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001367cL p0l_bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_M14A0_P0L_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0013680L p0l_bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0L_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0013684L p0l_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0     
	UINT32 cb_b_s_apl                         : 10;  // 19:10     
	UINT32 cr_r_s_apl                         : 10;  // 29:20     
} PE_M14A0_P0L_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0013690L p0l_tpd_hist_ai_ctrl ''
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
} PE_M14A0_P0L_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013694L p0l_tpd_hist_ai_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_status                        : 22;  // 21: 0     
} PE_M14A0_P0L_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0013698L p0l_lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16     
	UINT32 y_gain                             : 8 ;  // 31:24     
} PE_M14A0_P0L_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00136a0L p0l_3dfd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_offset_x                        : 8 ;  //  7: 0     
	UINT32 aw_offset_y                        : 8 ;  // 15: 8     
	UINT32 cb_th1                             : 8 ;  // 23:16     
	UINT32 cb_th2                             : 8 ;  // 31:24     
} PE_M14A0_P0L_3DFD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00136a4L p0l_3dfd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_th1                             : 8 ;  //  7: 0     
	UINT32 cc_th2                             : 8 ;  // 15: 8     
	UINT32 ll_th1                             : 8 ;  // 23:16     
	UINT32 ll_th2                             : 8 ;  // 31:24     
} PE_M14A0_P0L_3DFD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00136a8L p0l_3dfd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pt_th                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 sstb_th                            : 8 ;  // 23:16     
	UINT32 debug_mode                         : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 div_mode                           : 1 ;  //    28     
} PE_M14A0_P0L_3DFD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00136acL p0l_3dfd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 small_delta_th                     : 19;  // 18: 0     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 small_delta_en                     : 1 ;  //    24     
} PE_M14A0_P0L_3DFD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00136b0L p0l_3dfd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_count                           : 16;  // 15: 0     
	UINT32 cc_count                           : 16;  // 31:16     
} PE_M14A0_P0L_3DFD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc00136b4L p0l_3dfd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ll_count                           : 16;  // 15: 0     
	UINT32 trid_format                        : 4 ;  // 19:16     
} PE_M14A0_P0L_3DFD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc00136b8L p0l_3dfd_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_h                         : 23;  // 22: 0     
} PE_M14A0_P0L_3DFD_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc00136bcL p0l_3dfd_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_v                         : 23;  // 22: 0     
} PE_M14A0_P0L_3DFD_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc00136c0L p0l_3dfd_stat_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_v                         : 23;  // 22: 0     
} PE_M14A0_P0L_3DFD_STAT_04_T;

/*-----------------------------------------------------------------------------
		0xc00136c4L p0l_3dfd_stat_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_h                         : 23;  // 22: 0     
} PE_M14A0_P0L_3DFD_STAT_05_T;

/*-----------------------------------------------------------------------------
		0xc00136c8L p0l_3dfd_stat_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_ratio                           : 23;  // 22: 0     
} PE_M14A0_P0L_3DFD_STAT_06_T;

/*-----------------------------------------------------------------------------
		0xc00136ccL p0l_3dfd_stat_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_ratio                           : 23;  // 22: 0     
} PE_M14A0_P0L_3DFD_STAT_07_T;

/*-----------------------------------------------------------------------------
		0xc00136d0L p0l_tnr_ctrl_38 ''
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
} PE_M14A0_P0L_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc00136d4L p0l_tnr_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 secam_mot_c_th                     : 8 ;  //  7: 0     
	UINT32 secam_mot_y_th                     : 8 ;  // 15: 8     
	UINT32 secam_c_v_th                       : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 secam_c_v_calc_mode                : 2 ;  // 31:30     
} PE_M14A0_P0L_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc00136d8L p0l_tnr_ctrl_40 ''
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
} PE_M14A0_P0L_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc00136dcL p0l_tnr_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_th2                             : 8 ;  //  7: 0     
	UINT32 sf_th3                             : 8 ;  // 15: 8     
	UINT32 sf_th4                             : 8 ;  // 23:16     
	UINT32 sf_f0_sel                          : 2 ;  // 25:24     
	UINT32 sf_f1_sel                          : 2 ;  // 27:26     
	UINT32 sf_f2_sel                          : 2 ;  // 29:28     
	UINT32 sf_f3_sel                          : 2 ;  // 31:30     
} PE_M14A0_P0L_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc00136e0L p0l_tnr_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y0                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x0                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y1                           : 8 ;  // 23:16     
	UINT32 sf_mb_x1                           : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc00136e4L p0l_tnr_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0L_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0xc00136e8L p0l_tnr_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y2                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x2                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y3                           : 8 ;  // 23:16     
	UINT32 sf_mb_x3                           : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0xc00136ecL p0l_tnr_ctrl_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 sf_debug_map_sel                   : 2 ;  // 17:16     
	UINT32 sf_gain_mode                       : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sf_gain                            : 4 ;  // 23:20     
} PE_M14A0_P0L_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
		0xc00136f0L p0l_tnr_ctrl_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_blend_y0                      : 8 ;  //  7: 0     
	UINT32 mamc_blend_x0                      : 8 ;  // 15: 8     
	UINT32 mamc_blend_y1                      : 8 ;  // 23:16     
	UINT32 mamc_blend_x1                      : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
		0xc00136f4L p0l_tnr_ctrl_47 ''
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
} PE_M14A0_P0L_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
		0xc00136f8L p0l_tnr_ctrl_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_gain_ctrl2_y0                    : 8 ;  //  7: 0     
	UINT32 m_gain_ctrl2_x0                    : 8 ;  // 15: 8     
	UINT32 m_gain_ctrl2_y1                    : 8 ;  // 23:16     
	UINT32 m_gain_ctrl2_x1                    : 8 ;  // 31:24     
} PE_M14A0_P0L_TNR_CTRL_48_T;
/*-----------------------------------------------------------------------------
		0xc0015400L p0r_pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} PE_M14A0_P0R_PE0_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc0015404L p0r_pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16     
} PE_M14A0_P0R_PE0_INTR_T;

/*-----------------------------------------------------------------------------
		0xc0015408L p0r_pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                       : 4 ;  //  3: 0     
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4     
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8     
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12     
	UINT32 dnr_frame_id                       : 8 ;  // 23:16     
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24     
} PE_M14A0_P0R_PE0_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc001540cL p0r_pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0     
} PE_M14A0_P0R_PE0_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc0015410L p0r_pe0_opmode ''
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
} PE_M14A0_P0R_PE0_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc0015414L p0r_pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0     
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 cs_type                            : 3 ;  // 15:13     
	UINT32 vsize                              : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31     
} PE_M14A0_P0R_PE0_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0015418L p0r_pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 secam_line                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12     
} PE_M14A0_P0R_PE0_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc001541cL p0r_pe0_param_by_ddr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 9 ;  //  8: 0     reserved
	UINT32 stride                             : 2 ;  // 10: 9     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 vsize                              : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 endian                             : 1 ;  //    30     
	UINT32 update_by_ddr                      : 1 ;  //    31     
} PE_M14A0_P0R_PE0_PARAM_BY_DDR_T;

/*-----------------------------------------------------------------------------
		0xc0015420L p0r_win_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_M14A0_P0R_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0015424L p0r_win_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0R_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0015428L p0r_win_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32 ac_bnr_feature_cal_mode            : 2 ;  // 29:28     
} PE_M14A0_P0R_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc001542cL p0r_win_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0R_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0015430L p0r_pe0_debug_mem_req ''
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
} PE_M14A0_P0R_PE0_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0xc0015434L p0r_pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0     
	UINT32 lreq_dly_c_en                      : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16     
	UINT32 lreq_dly_y_en                      : 1 ;  //    27     
} PE_M14A0_P0R_PE0_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0015440L p0r_tnr_ctrl_00 ''
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
} PE_M14A0_P0R_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0015444L p0r_tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_th_lo                            : 8 ;  //  7: 0     
	UINT32 y_th_hi                            : 8 ;  // 15: 8     
	UINT32 dnr_motion_scale                   : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 tnr_filter_lsb_ctrl                : 2 ;  // 21:20     
	UINT32 svar_mux_ctrl                      : 2 ;  // 23:22     
	UINT32 nt_lvl_hist_n_pct                  : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0015448L p0r_tnr_ctrl_02 ''
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
} PE_M14A0_P0R_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001544cL p0r_tnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y0                              : 8 ;  //  7: 0     
	UINT32 wa_y1                              : 8 ;  // 15: 8     
	UINT32 wa_x0                              : 8 ;  // 23:16     
	UINT32 wa_x1                              : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0015450L p0r_tnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x0                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x1                  : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0015454L p0r_tnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_min                             : 8 ;  //  7: 0     
	UINT32 nt_max                             : 8 ;  // 15: 8     
	UINT32 hist_svar_th                       : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th0               : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0015458L p0r_tnr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 peak_0_th                          : 8 ;  //  7: 0     
	UINT32 nt_iir_alpha                       : 8 ;  // 15: 8     
	UINT32 y_th_lo2                           : 8 ;  // 23:16     
	UINT32 y_th_hi2                           : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc001545cL p0r_tnr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind0                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind1                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind2                     : 8 ;  // 23:16     
	UINT32 alpha_lut_ind3                     : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0015460L p0r_tnr_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_lut_ind4                     : 8 ;  //  7: 0     
	UINT32 alpha_lut_ind5                     : 8 ;  // 15: 8     
	UINT32 alpha_lut_ind6                     : 8 ;  // 23:16     
	UINT32 adj_alpha_nt_lvl_th1               : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0015464L p0r_tnr_ctrl_09 ''
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
} PE_M14A0_P0R_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0015468L p0r_tnr_ctrl_10 ''
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
} PE_M14A0_P0R_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc001546cL p0r_tnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dbg_calc_motion                    : 8 ;  //  7: 0     
	UINT32 dbg_alpha                          : 8 ;  // 15: 8     
	UINT32 peak0_x_th                         : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 scene_change_th_mv                 : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0015470L p0r_tnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_gm_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_gm_x2                  : 8 ;  // 15: 8     
	UINT32 nt_lvl_gain_gm_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_gm_x3                  : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0015474L p0r_tnr_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_nt_lvl                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 force_nt_lvl_val                   : 8 ;  // 15: 8     
	UINT32 adjust_nt_lvl                      : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 adjust_nt_lvl_val                  : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0015478L p0r_tnr_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 noise_measure_win_en               : 1 ;  //    31     
} PE_M14A0_P0R_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc001547cL p0r_tnr_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0R_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0015480L p0r_tnr_ctrl_16 ''
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
} PE_M14A0_P0R_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0015484L p0r_tnr_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y0                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x0                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y1                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x1                     : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0015488L p0r_tnr_ctrl_18 ''
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
} PE_M14A0_P0R_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc001548cL p0r_tnr_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_x                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_x                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_x                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_x                     : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0015490L p0r_tnr_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_p0_y                     : 8 ;  //  7: 0     
	UINT32 luma_gain_p1_y                     : 8 ;  // 15: 8     
	UINT32 luma_gain_p2_y                     : 8 ;  // 23:16     
	UINT32 luma_gain_p3_y                     : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc0015494L p0r_tnr_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_mamc_blend                     : 8 ;  //  7: 0     
} PE_M14A0_P0R_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc0015498L p0r_tnr_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_protect_control_y0              : 8 ;  //  7: 0     
	UINT32 mv_protect_control_x0              : 8 ;  // 15: 8     
	UINT32 mv_protect_control_y1              : 8 ;  // 23:16     
	UINT32 mv_protect_control_x1              : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc001549cL p0r_tnr_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x1                 : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc00154a0L p0r_tnr_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_y_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_y_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_y_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_y_x3                 : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc00154a4L p0r_tnr_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y0                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x0                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y1                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x1                 : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc00154a8L p0r_tnr_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 alpha_mapping_c_y2                 : 8 ;  //  7: 0     
	UINT32 alpha_mapping_c_x2                 : 8 ;  // 15: 8     
	UINT32 alpha_mapping_c_y3                 : 8 ;  // 23:16     
	UINT32 alpha_mapping_c_x3                 : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc00154acL p0r_tnr_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_gain_rgn0                     : 8 ;  //  7: 0     
	UINT32 crgn_gain_rgn1                     : 8 ;  // 15: 8     
	UINT32 crgn_gain_rgn2                     : 8 ;  // 23:16     
	UINT32 crgn_gain_rgn3                     : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc00154b0L p0r_tnr_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y0                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x0                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y1                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x1                  : 7 ;  // 30:24     
} PE_M14A0_P0R_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc00154b4L p0r_tnr_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wa_y2                              : 8 ;  //  7: 0     
	UINT32 wa_y3                              : 8 ;  // 15: 8     
	UINT32 wa_x2                              : 8 ;  // 23:16     
	UINT32 wa_x3                              : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc00154b8L p0r_tnr_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_st_y2                  : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_st_x2                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_st_y3                  : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_st_x3                  : 7 ;  // 30:24     
} PE_M14A0_P0R_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc00154bcL p0r_tnr_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 spatial_pre_flt_en_c               : 1 ;  //     0     
	UINT32 spatial_pre_flt_en_y               : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 spatial_pre_flt_th_c               : 8 ;  // 23:16     
	UINT32 spatial_pre_flt_th_y               : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc00154c0L p0r_tnr_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x_avg_t                            : 8 ;  //  7: 0     
	UINT32 x_peak_t2                          : 8 ;  // 15: 8     
	UINT32 nt_lvl                             : 8 ;  // 23:16     
	UINT32 x_avg_s                            : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc00154c4L p0r_tnr_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sum_valid                          : 16;  // 15: 0     
	UINT32 scene_diff                         : 16;  // 31:16     
} PE_M14A0_P0R_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc00154c8L p0r_tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_l                               : 4 ;  //  3: 0     
	UINT32 mv_r                               : 4 ;  //  7: 4     
	UINT32 dfd                                : 8 ;  // 15: 8     
	UINT32 apl_y                              : 8 ;  // 23:16     
	UINT32 lp                                 : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc00154ccL p0r_tnr_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gmv_valid_cnt                      : 16;  // 15: 0     
	UINT32 fd                                 : 8 ;  // 23:16     
	UINT32 gmv_lock_cnt                       : 6 ;  // 29:24     
} PE_M14A0_P0R_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc00154d0L p0r_tnr_main_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 main_lut_ai_enable                 : 1 ;  //    12     
} PE_M14A0_P0R_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
		0xc00154d4L p0r_tnr_main_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0               : 32;  // 31: 0     
} PE_M14A0_P0R_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
		0xc00154d8L p0r_tnr_main_lut_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1               : 32;  // 31: 0     
} PE_M14A0_P0R_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
		0xc00154dcL p0r_tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_r_cnt                           : 16;  // 15: 0     
	UINT32 mv_l_cnt                           : 16;  // 31:16     
} PE_M14A0_P0R_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc00154e0L p0r_tnr_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y0                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x0                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y1                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x1                : 7 ;  // 30:24     
} PE_M14A0_P0R_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc00154e4L p0r_tnr_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nt_lvl_gain_lpct_y2                : 8 ;  //  7: 0     
	UINT32 nt_lvl_gain_lpct_x2                : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 nt_lvl_gain_lpct_y3                : 8 ;  // 23:16     
	UINT32 nt_lvl_gain_lpct_x3                : 7 ;  // 30:24     
} PE_M14A0_P0R_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc00154e8L p0r_tnr_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_blur_ctrl_y0                    : 8 ;  //  7: 0     
	UINT32 mc_blur_ctrl_x0                    : 8 ;  // 15: 8     
	UINT32 mc_blur_ctrl_y1                    : 8 ;  // 23:16     
	UINT32 mc_blur_ctrl_x1                    : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc00154ecL p0r_tnr_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_tiir_ctrl_y2                     : 8 ;  //  7: 0     
	UINT32 m_tiir_ctrl_x2                     : 8 ;  // 15: 8     
	UINT32 m_tiir_ctrl_y3                     : 8 ;  // 23:16     
	UINT32 m_tiir_ctrl_x3                     : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc00154f0L p0r_tnr_crg_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_addr               : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 crgn_ctrl_ai_enable                : 1 ;  //    12     
} PE_M14A0_P0R_TNR_CRG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00154f4L p0r_tnr_crg_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 crgn_ctrl_indir_data               : 32;  // 31: 0     
} PE_M14A0_P0R_TNR_CRG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00154f8L p0r_tnr_ctrl_36 ''
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
} PE_M14A0_P0R_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc00154fcL p0r_tnr_ctrl_37 ''
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
} PE_M14A0_P0R_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc0015500L p0r_ipc_ctrl_00 ''
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
} PE_M14A0_P0R_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0015504L p0r_ipc_ctrl_01 ''
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
} PE_M14A0_P0R_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0015508L p0r_ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001550cL p0r_ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0015510L p0r_ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0     
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8     
	UINT32 tearing_gain                       : 8 ;  // 23:16     
	UINT32 motion_gain                        : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0015514L p0r_ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha              : 8 ;  //  7: 0     
	UINT32 cr_t_grad_weight                   : 8 ;  // 15: 8     
	UINT32 cr_v_grad_weight                   : 8 ;  // 23:16     
	UINT32 cr_h_grad_weight                   : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0015518L p0r_ipc_ctrl_06 ''
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
} PE_M14A0_P0R_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc001551cL p0r_ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1     
	UINT32 cr_45_en                           : 1 ;  //     2     
	UINT32 cr_15_en                           : 1 ;  //     3     
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 tnr_motion_gain                    : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0015520L p0r_ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30     
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31     
} PE_M14A0_P0R_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0015524L p0r_ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16     
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0015528L p0r_ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16     
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc001552cL p0r_ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0     
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8     
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16     
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0015530L p0r_ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0015534L p0r_ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    16     
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 23:19     
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 29:24     
} PE_M14A0_P0R_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0015538L p0r_ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reg_manual_en                      : 1 ;  //     1     
	UINT32 reg_manual_mode                    : 2 ;  //  3: 2     
	UINT32 reg_lock_speed                     : 4 ;  //  7: 4     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc001553cL p0r_ipc_ctrl_15 ''
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
} PE_M14A0_P0R_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0015540L p0r_ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32 reg_same_min_ratio                 : 4 ;  // 27:24     
	UINT32 reg_same_adder_gain                : 4 ;  // 31:28     
} PE_M14A0_P0R_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0015544L p0r_ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0     
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2     
	UINT32 cr_field_inverse                   : 1 ;  //     4     
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0015548L p0r_ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0     
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8     
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16     
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc001554cL p0r_ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0     
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8     
	UINT32 reg_2d_offset                      : 8 ;  // 23:16     
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0015550L p0r_ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                       : 16;  // 15: 0     
	UINT32 r_nc_v_count                       : 16;  // 31:16     
} PE_M14A0_P0R_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc0015554L p0r_ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                       : 16;  // 15: 0     
	UINT32 r_pn_v_count                       : 16;  // 31:16     
} PE_M14A0_P0R_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc0015558L p0r_ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_same                       : 16;  // 15: 0     
	UINT32 r_pc_pn_same                       : 16;  // 31:16     
} PE_M14A0_P0R_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc001555cL p0r_ipc_status_03 ''
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
} PE_M14A0_P0R_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc0015580L p0r_vfilter_00 ''
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
} PE_M14A0_P0R_VFILTER_00_T;

/*-----------------------------------------------------------------------------
		0xc0015584L p0r_cti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                     : 8 ;  //  7: 0     
	UINT32 cti_coring_th1                     : 8 ;  // 15: 8     
	UINT32 cti_coring_smooth                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 cti_coring_map_size                : 2 ;  // 21:20     
} PE_M14A0_P0R_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0015588L p0r_mnr_ctrl_00 ''
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
} PE_M14A0_P0R_MNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc001558cL p0r_mnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_s1_mmd_min                 : 8 ;  //  7: 0     
	UINT32 reg_mnr_s2_ratio_min               : 8 ;  // 15: 8     
	UINT32 reg_mnr_s2_ratio_max               : 8 ;  // 23:16     
	UINT32 reg_mnr_s2_mmd_min                 : 8 ;  // 31:24     
} PE_M14A0_P0R_MNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0015590L p0r_mnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_mnr_h_gain                     : 8 ;  // 23:16     
	UINT32 reg_mnr_v_gain                     : 8 ;  // 31:24     
} PE_M14A0_P0R_MNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0015594L p0r_mnr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_filter_x0                      : 8 ;  //  7: 0     
	UINT32 reg_filter_x1                      : 8 ;  // 15: 8     
	UINT32 reg_filter_y0                      : 8 ;  // 23:16     
	UINT32 reg_filter_y1                      : 8 ;  // 31:24     
} PE_M14A0_P0R_MNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0015598L p0r_mnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_en                  : 1 ;  //     0     
	UINT32 reg_motion_mnr_filter              : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 reg_mnr_motion_min                 : 8 ;  // 23:16     
	UINT32 reg_mnr_motion_max                 : 8 ;  // 31:24     
} PE_M14A0_P0R_MNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc001559cL p0r_mnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_x0                  : 8 ;  //  7: 0     
	UINT32 reg_motion_mnr_x1                  : 8 ;  // 15: 8     
	UINT32 reg_motion_mnr_y0                  : 8 ;  // 23:16     
	UINT32 reg_motion_mnr_y1                  : 8 ;  // 31:24     
} PE_M14A0_P0R_MNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00155a4L p0r_vfilter_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable              : 1 ;  //     0     
	UINT32 cr_hmc_protect_only_hmc_line       : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 cr_hmc_protect_motion_th           : 8 ;  // 15: 8     
	UINT32 cr_hmc_protect_th_max              : 8 ;  // 23:16     
	UINT32 cr_hmc_protect_th_min              : 8 ;  // 31:24     
} PE_M14A0_P0R_VFILTER_02_T;

/*-----------------------------------------------------------------------------
		0xc00155a8L p0r_vfilter_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en               : 1 ;  //     0     
	UINT32 cr_vfilter_force_en                : 1 ;  //     1     
	UINT32 cr_global_frame_num_th             : 4 ;  //  5: 2     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cr_vfilter_global_th               : 12;  // 19: 8     
	UINT32 tearing_count                      : 12;  // 31:20     
} PE_M14A0_P0R_VFILTER_01_T;

/*-----------------------------------------------------------------------------
		0xc00155acL p0r_bnr_dc_ctrl_00 ''
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
} PE_M14A0_P0R_BNR_DC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00155b0L p0r_bnr_dc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_var_th3                 : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_var_th2                 : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_var_th1                 : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_var_th0                 : 8 ;  // 31:24     
} PE_M14A0_P0R_BNR_DC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00155b4L p0r_bnr_dc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_motion_th3              : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_motion_th2              : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_motion_th1              : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_motion_th0              : 8 ;  // 31:24     
} PE_M14A0_P0R_BNR_DC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00155b8L p0r_bnr_dc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_chroma_variance             : 8 ;  //  7: 0     
	UINT32 reg_dc_var_h_gain                  : 8 ;  // 15: 8     
	UINT32 reg_dc_protection_th               : 8 ;  // 23:16     
} PE_M14A0_P0R_BNR_DC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00155bcL p0r_bnr_ac_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_hys_mode                    : 4 ;  //  3: 0     
	UINT32 reg_sc_bdr_type                    : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 ac_bnr_protect_motion_max          : 8 ;  // 23:16     
	UINT32 ac_bnr_protect_motion_min          : 8 ;  // 31:24     
} PE_M14A0_P0R_BNR_AC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc00155c0L p0r_bnr_ac_ctrl_00 ''
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
} PE_M14A0_P0R_BNR_AC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00155c4L p0r_bnr_ac_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_h_x0                      : 8 ;  //  7: 0     
	UINT32 strength_h_x1                      : 8 ;  // 15: 8     
	UINT32 strength_h_max                     : 8 ;  // 23:16     
	UINT32 detect_min_th                      : 8 ;  // 31:24     
} PE_M14A0_P0R_BNR_AC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00155c8L p0r_bnr_ac_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 strength_v_x0                      : 8 ;  //  7: 0     
	UINT32 strength_v_x1                      : 8 ;  // 15: 8     
	UINT32 strength_v_max                     : 8 ;  // 23:16     
} PE_M14A0_P0R_BNR_AC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00155ccL p0r_bnr_ac_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset_mode                      : 1 ;  //     0     
	UINT32 manual_offset_h_value              : 3 ;  //  3: 1     
	UINT32 v_offset_mode                      : 1 ;  //     4     
	UINT32 manual_offset_v_value              : 3 ;  //  7: 5     
	UINT32 use_of_hysterisis                  : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 t_filter_weight                    : 8 ;  // 23:16     
} PE_M14A0_P0R_BNR_AC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00155d0L p0r_bnr_ac_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_delta_th0                      : 8 ;  //  7: 0     
	UINT32 max_delta_th1                      : 8 ;  // 15: 8     
	UINT32 h_blockness_th                     : 8 ;  // 23:16     
	UINT32 h_weight_max                       : 8 ;  // 31:24     
} PE_M14A0_P0R_BNR_AC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00155d4L p0r_bnr_ac_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_gain3                   : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_gain2                   : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_gain1                   : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_gain0                   : 8 ;  // 31:24     
} PE_M14A0_P0R_BNR_AC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00155d8L p0r_dnr_max_ctrl ''
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
} PE_M14A0_P0R_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00155dcL p0r_dnr_dbar_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_bar_en                       : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 show_debug_bar_for_bnr             : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 gain                               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sd_hd_sel                          : 2 ;  //  9: 8     
} PE_M14A0_P0R_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00155e0L p0r_bnr_stat_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ac_bnr_h_status                    : 8 ;  //  7: 0     
	UINT32 ac_bnr_v_status                    : 8 ;  // 15: 8     
	UINT32 dc_bnr_th                          : 14;  // 29:16     
} PE_M14A0_P0R_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
		0xc00155e4L p0r_cti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32 cti_clipping_mode                  : 1 ;  //     1     
	UINT32 cti_pre_flt_mode                   : 2 ;  //  3: 2     
	UINT32 cti_tap                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
	UINT32 cti_debug_mode                     : 3 ;  // 18:16     
} PE_M14A0_P0R_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc00155e8L p0r_reg_c420_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tearing_count_th               : 8 ;  //  7: 0     
	UINT32 reg_frame_count_th                 : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_detect_level_th                : 10;  // 25:16     
} PE_M14A0_P0R_REG_C420_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00155f0L p0r_ipc_ctrl_20 ''
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
} PE_M14A0_P0R_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc00155f4L p0r_ipc_ctrl_21 ''
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
} PE_M14A0_P0R_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc00155f8L p0r_ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0     
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8     
} PE_M14A0_P0R_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc00155fcL p0r_ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0     
	UINT32 cr_lock_th                         : 7 ;  //  7: 1     
	UINT32 cr_g_motion_th                     : 8 ;  // 15: 8     
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16     
	UINT32 cr_hmc_half_en                     : 1 ;  //    19     
	UINT32 cr_robustness                      : 4 ;  // 23:20     
	UINT32 cr_center_weight                   : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc0015600L p0r_ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8     
	UINT32 cr_gds_resolution                  : 3 ;  // 18:16     
} PE_M14A0_P0R_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc0015604L p0r_ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8     
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16     
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc0015608L p0r_ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc001560cL p0r_ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0     
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8     
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16     
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc0015610L p0r_ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0R_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc0015614L p0r_ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16     
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24     
} PE_M14A0_P0R_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc0015618L p0r_ipc_ctrl_30 ''
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
} PE_M14A0_P0R_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc001561cL p0r_ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} PE_M14A0_P0R_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc0015628L p0r_ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_lr_count                      : 16;  // 15: 0     
	UINT32 r_pc_lr_count                      : 16;  // 31:16     
} PE_M14A0_P0R_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc001562cL p0r_ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_lr_same                    : 16;  // 15: 0     
	UINT32 r_pndiff_count                     : 16;  // 31:16     
} PE_M14A0_P0R_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc0015630L p0r_ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_nc_pn_lr_same                    : 16;  // 15: 0     
	UINT32 r_pc_pn_lr_same                    : 16;  // 31:16     
} PE_M14A0_P0R_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc0015634L p0r_ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0     
	UINT32 r_inv_original_cnt                 : 16;  // 31:16     
} PE_M14A0_P0R_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc0015638L p0r_ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0R_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc001563cL p0r_ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0R_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0xc0015640L p0r_tpd_ctrl_00 ''
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
} PE_M14A0_P0R_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0015644L p0r_tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15     
	UINT32 wina_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31     
} PE_M14A0_P0R_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0015648L p0r_tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30     
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31     
} PE_M14A0_P0R_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001564cL p0r_tpd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winb_hen                       : 1 ;  //    15     
	UINT32 winb_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winb_ven                       : 1 ;  //    31     
} PE_M14A0_P0R_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0015650L p0r_tpd_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winb_y1                            : 12;  // 27:16     
} PE_M14A0_P0R_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0015654L p0r_tpd_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_y                        : 10;  //  9: 0     
	UINT32 reg_force_y_max_th                 : 10;  // 19:10     
	UINT32 reg_force_y_min_th                 : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_y_enable                 : 1 ;  //    31     
} PE_M14A0_P0R_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0015658L p0r_tpd_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cb                       : 10;  //  9: 0     
	UINT32 reg_force_cb_max_th                : 10;  // 19:10     
	UINT32 reg_force_cb_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cb_enable                : 1 ;  //    31     
} PE_M14A0_P0R_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc001565cL p0r_tpd_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cr                       : 10;  //  9: 0     
	UINT32 reg_force_cr_max_th                : 10;  // 19:10     
	UINT32 reg_force_cr_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cr_enable                : 1 ;  //    31     
} PE_M14A0_P0R_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0015660L p0r_tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_pixel_count                   : 22;  // 21: 0
} PE_M14A0_P0R_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0015670L p0r_bbd_ctrl_00 ''
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
} PE_M14A0_P0R_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0015674L p0r_bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_M14A0_P0R_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0015678L p0r_bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0R_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc001567cL p0r_bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_M14A0_P0R_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0015680L p0r_bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_M14A0_P0R_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0015684L p0r_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0     
	UINT32 cb_b_s_apl                         : 10;  // 19:10     
	UINT32 cr_r_s_apl                         : 10;  // 29:20     
} PE_M14A0_P0R_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0015690L p0r_tpd_hist_ai_ctrl ''
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
} PE_M14A0_P0R_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0015694L p0r_tpd_hist_ai_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0R_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0015698L p0r_lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16     
	UINT32 y_gain                             : 8 ;  // 31:24     
} PE_M14A0_P0R_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00156d0L p0r_tnr_ctrl_38 ''
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
} PE_M14A0_P0R_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc00156d4L p0r_tnr_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 secam_mot_c_th                     : 8 ;  //  7: 0     
	UINT32 secam_mot_y_th                     : 8 ;  // 15: 8     
	UINT32 secam_c_v_th                       : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 secam_c_v_calc_mode                : 2 ;  // 31:30     
} PE_M14A0_P0R_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc00156d8L p0r_tnr_ctrl_40 ''
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
} PE_M14A0_P0R_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc00156dcL p0r_tnr_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_th2                             : 8 ;  //  7: 0     
	UINT32 sf_th3                             : 8 ;  // 15: 8     
	UINT32 sf_th4                             : 8 ;  // 23:16     
	UINT32 sf_f0_sel                          : 2 ;  // 25:24     
	UINT32 sf_f1_sel                          : 2 ;  // 27:26     
	UINT32 sf_f2_sel                          : 2 ;  // 29:28     
	UINT32 sf_f3_sel                          : 2 ;  // 31:30     
} PE_M14A0_P0R_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc00156e0L p0r_tnr_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y0                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x0                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y1                           : 8 ;  // 23:16     
	UINT32 sf_mb_x1                           : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc00156e4L p0r_tnr_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_M14A0_P0R_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0xc00156e8L p0r_tnr_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mb_y2                           : 8 ;  //  7: 0     
	UINT32 sf_mb_x2                           : 8 ;  // 15: 8     
	UINT32 sf_mb_y3                           : 8 ;  // 23:16     
	UINT32 sf_mb_x3                           : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0xc00156ecL p0r_tnr_ctrl_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 sf_debug_map_sel                   : 2 ;  // 17:16     
	UINT32 sf_gain_mode                       : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sf_gain                            : 4 ;  // 23:20     
} PE_M14A0_P0R_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
		0xc00156f0L p0r_tnr_ctrl_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mamc_blend_y0                      : 8 ;  //  7: 0     
	UINT32 mamc_blend_x0                      : 8 ;  // 15: 8     
	UINT32 mamc_blend_y1                      : 8 ;  // 23:16     
	UINT32 mamc_blend_x1                      : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
		0xc00156f4L p0r_tnr_ctrl_47 ''
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
} PE_M14A0_P0R_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
		0xc00156f8L p0r_tnr_ctrl_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_gain_ctrl2_y0                    : 8 ;  //  7: 0     
	UINT32 m_gain_ctrl2_x0                    : 8 ;  // 15: 8     
	UINT32 m_gain_ctrl2_y1                    : 8 ;  // 23:16     
	UINT32 m_gain_ctrl2_x1                    : 8 ;  // 31:24     
} PE_M14A0_P0R_TNR_CTRL_48_T;

typedef struct {
	PE_M14A0_P0L_PE0_LOAD_T            	p0l_pe0_load;                 //0xc0013400L
	PE_M14A0_P0L_PE0_INTR_T            	p0l_pe0_intr;                 //0xc0013404L
	PE_M14A0_P0L_PE0_STATUS0_T         	p0l_pe0_status0;              //0xc0013408L
	PE_M14A0_P0L_PE0_STATUS1_T         	p0l_pe0_status1;              //0xc001340cL
	PE_M14A0_P0L_PE0_OPMODE_T          	p0l_pe0_opmode;               //0xc0013410L
	PE_M14A0_P0L_PE0_SRC_SIZE_T        	p0l_pe0_src_size;             //0xc0013414L
	PE_M14A0_P0L_PE0_FIELD_T           	p0l_pe0_field;                //0xc0013418L
	PE_M14A0_P0L_PE0_PARAM_BY_DDR_T    	p0l_pe0_param_by_ddr;         //0xc001341cL
	PE_M14A0_P0L_WIN_CTRL_0_T          	p0l_win_ctrl_0;               //0xc0013420L
	PE_M14A0_P0L_WIN_CTRL_1_T          	p0l_win_ctrl_1;               //0xc0013424L
	PE_M14A0_P0L_WIN_CTRL_2_T          	p0l_win_ctrl_2;               //0xc0013428L
	PE_M14A0_P0L_WIN_CTRL_3_T          	p0l_win_ctrl_3;               //0xc001342cL
	PE_M14A0_P0L_PE0_DEBUG_MEM_REQ_T   	p0l_pe0_debug_mem_req;        //0xc0013430L
	PE_M14A0_P0L_PE0_DEBUG_LREQ_DLY_T  	p0l_pe0_debug_lreq_dly;       //0xc0013434L
	UINT32                             	reserved00[2];                //0xc0013438L~0xc001343cL
	PE_M14A0_P0L_TNR_CTRL_00_T         	p0l_tnr_ctrl_00;              //0xc0013440L
	PE_M14A0_P0L_TNR_CTRL_01_T         	p0l_tnr_ctrl_01;              //0xc0013444L
	PE_M14A0_P0L_TNR_CTRL_02_T         	p0l_tnr_ctrl_02;              //0xc0013448L
	PE_M14A0_P0L_TNR_CTRL_03_T         	p0l_tnr_ctrl_03;              //0xc001344cL
	PE_M14A0_P0L_TNR_CTRL_04_T         	p0l_tnr_ctrl_04;              //0xc0013450L
	PE_M14A0_P0L_TNR_CTRL_05_T         	p0l_tnr_ctrl_05;              //0xc0013454L
	PE_M14A0_P0L_TNR_CTRL_06_T         	p0l_tnr_ctrl_06;              //0xc0013458L
	PE_M14A0_P0L_TNR_CTRL_07_T         	p0l_tnr_ctrl_07;              //0xc001345cL
	PE_M14A0_P0L_TNR_CTRL_08_T         	p0l_tnr_ctrl_08;              //0xc0013460L
	PE_M14A0_P0L_TNR_CTRL_09_T         	p0l_tnr_ctrl_09;              //0xc0013464L
	PE_M14A0_P0L_TNR_CTRL_10_T         	p0l_tnr_ctrl_10;              //0xc0013468L
	PE_M14A0_P0L_TNR_CTRL_11_T         	p0l_tnr_ctrl_11;              //0xc001346cL
	PE_M14A0_P0L_TNR_CTRL_12_T         	p0l_tnr_ctrl_12;              //0xc0013470L
	PE_M14A0_P0L_TNR_CTRL_13_T         	p0l_tnr_ctrl_13;              //0xc0013474L
	PE_M14A0_P0L_TNR_CTRL_14_T         	p0l_tnr_ctrl_14;              //0xc0013478L
	PE_M14A0_P0L_TNR_CTRL_15_T         	p0l_tnr_ctrl_15;              //0xc001347cL
	PE_M14A0_P0L_TNR_CTRL_16_T         	p0l_tnr_ctrl_16;              //0xc0013480L
	PE_M14A0_P0L_TNR_CTRL_17_T         	p0l_tnr_ctrl_17;              //0xc0013484L
	PE_M14A0_P0L_TNR_CTRL_18_T         	p0l_tnr_ctrl_18;              //0xc0013488L
	PE_M14A0_P0L_TNR_CTRL_19_T         	p0l_tnr_ctrl_19;              //0xc001348cL
	PE_M14A0_P0L_TNR_CTRL_20_T         	p0l_tnr_ctrl_20;              //0xc0013490L
	PE_M14A0_P0L_TNR_CTRL_21_T         	p0l_tnr_ctrl_21;              //0xc0013494L
	PE_M14A0_P0L_TNR_CTRL_22_T         	p0l_tnr_ctrl_22;              //0xc0013498L
	PE_M14A0_P0L_TNR_CTRL_23_T         	p0l_tnr_ctrl_23;              //0xc001349cL
	PE_M14A0_P0L_TNR_CTRL_24_T         	p0l_tnr_ctrl_24;              //0xc00134a0L
	PE_M14A0_P0L_TNR_CTRL_25_T         	p0l_tnr_ctrl_25;              //0xc00134a4L
	PE_M14A0_P0L_TNR_CTRL_26_T         	p0l_tnr_ctrl_26;              //0xc00134a8L
	PE_M14A0_P0L_TNR_CTRL_27_T         	p0l_tnr_ctrl_27;              //0xc00134acL
	PE_M14A0_P0L_TNR_CTRL_28_T         	p0l_tnr_ctrl_28;              //0xc00134b0L
	PE_M14A0_P0L_TNR_CTRL_29_T         	p0l_tnr_ctrl_29;              //0xc00134b4L
	PE_M14A0_P0L_TNR_CTRL_30_T         	p0l_tnr_ctrl_30;              //0xc00134b8L
	PE_M14A0_P0L_TNR_CTRL_31_T         	p0l_tnr_ctrl_31;              //0xc00134bcL
	PE_M14A0_P0L_TNR_STATUS_00_T       	p0l_tnr_status_00;            //0xc00134c0L
	PE_M14A0_P0L_TNR_STATUS_01_T       	p0l_tnr_status_01;            //0xc00134c4L
	PE_M14A0_P0L_TNR_STATUS_02_T       	p0l_tnr_status_02;            //0xc00134c8L
	PE_M14A0_P0L_TNR_STATUS_03_T       	p0l_tnr_status_03;            //0xc00134ccL
	PE_M14A0_P0L_TNR_MAIN_LUT_00_T     	p0l_tnr_main_lut_00;          //0xc00134d0L
	PE_M14A0_P0L_TNR_MAIN_LUT_01_T     	p0l_tnr_main_lut_01;          //0xc00134d4L
	PE_M14A0_P0L_TNR_MAIN_LUT_02_T     	p0l_tnr_main_lut_02;          //0xc00134d8L
	PE_M14A0_P0L_TNR_STATUS_04_T       	p0l_tnr_status_04;            //0xc00134dcL
	PE_M14A0_P0L_TNR_CTRL_32_T         	p0l_tnr_ctrl_32;              //0xc00134e0L
	PE_M14A0_P0L_TNR_CTRL_33_T         	p0l_tnr_ctrl_33;              //0xc00134e4L
	PE_M14A0_P0L_TNR_CTRL_34_T         	p0l_tnr_ctrl_34;              //0xc00134e8L
	PE_M14A0_P0L_TNR_CTRL_35_T         	p0l_tnr_ctrl_35;              //0xc00134ecL
	PE_M14A0_P0L_TNR_CRG_CTRL_00_T     	p0l_tnr_crg_ctrl_00;          //0xc00134f0L
	PE_M14A0_P0L_TNR_CRG_CTRL_01_T     	p0l_tnr_crg_ctrl_01;          //0xc00134f4L
	PE_M14A0_P0L_TNR_CTRL_36_T         	p0l_tnr_ctrl_36;              //0xc00134f8L
	PE_M14A0_P0L_TNR_CTRL_37_T         	p0l_tnr_ctrl_37;              //0xc00134fcL
	PE_M14A0_P0L_IPC_CTRL_00_T         	p0l_ipc_ctrl_00;              //0xc0013500L
	PE_M14A0_P0L_IPC_CTRL_01_T         	p0l_ipc_ctrl_01;              //0xc0013504L
	PE_M14A0_P0L_IPC_CTRL_02_T         	p0l_ipc_ctrl_02;              //0xc0013508L
	PE_M14A0_P0L_IPC_CTRL_03_T         	p0l_ipc_ctrl_03;              //0xc001350cL
	PE_M14A0_P0L_IPC_CTRL_04_T         	p0l_ipc_ctrl_04;              //0xc0013510L
	PE_M14A0_P0L_IPC_CTRL_05_T         	p0l_ipc_ctrl_05;              //0xc0013514L
	PE_M14A0_P0L_IPC_CTRL_06_T         	p0l_ipc_ctrl_06;              //0xc0013518L
	PE_M14A0_P0L_IPC_CTRL_07_T         	p0l_ipc_ctrl_07;              //0xc001351cL
	PE_M14A0_P0L_IPC_CTRL_08_T         	p0l_ipc_ctrl_08;              //0xc0013520L
	PE_M14A0_P0L_IPC_CTRL_09_T         	p0l_ipc_ctrl_09;              //0xc0013524L
	PE_M14A0_P0L_IPC_CTRL_10_T         	p0l_ipc_ctrl_10;              //0xc0013528L
	PE_M14A0_P0L_IPC_CTRL_11_T         	p0l_ipc_ctrl_11;              //0xc001352cL
	PE_M14A0_P0L_IPC_CTRL_12_T         	p0l_ipc_ctrl_12;              //0xc0013530L
	PE_M14A0_P0L_IPC_CTRL_13_T         	p0l_ipc_ctrl_13;              //0xc0013534L
	PE_M14A0_P0L_IPC_CTRL_14_T         	p0l_ipc_ctrl_14;              //0xc0013538L
	PE_M14A0_P0L_IPC_CTRL_15_T         	p0l_ipc_ctrl_15;              //0xc001353cL
	PE_M14A0_P0L_IPC_CTRL_16_T         	p0l_ipc_ctrl_16;              //0xc0013540L
	PE_M14A0_P0L_IPC_CTRL_17_T         	p0l_ipc_ctrl_17;              //0xc0013544L
	PE_M14A0_P0L_IPC_CTRL_18_T         	p0l_ipc_ctrl_18;              //0xc0013548L
	PE_M14A0_P0L_IPC_CTRL_19_T         	p0l_ipc_ctrl_19;              //0xc001354cL
	PE_M14A0_P0L_IPC_STATUS_00_T       	p0l_ipc_status_00;            //0xc0013550L
	PE_M14A0_P0L_IPC_STATUS_01_T       	p0l_ipc_status_01;            //0xc0013554L
	PE_M14A0_P0L_IPC_STATUS_02_T       	p0l_ipc_status_02;            //0xc0013558L
	PE_M14A0_P0L_IPC_STATUS_03_T       	p0l_ipc_status_03;            //0xc001355cL
	PE_M14A0_P0L_CLC_CTRL_00_T         	p0l_clc_ctrl_00;              //0xc0013560L
	PE_M14A0_P0L_CLC_CTRL_01_T         	p0l_clc_ctrl_01;              //0xc0013564L
	PE_M14A0_P0L_CLC_CTRL_02_T         	p0l_clc_ctrl_02;              //0xc0013568L
	PE_M14A0_P0L_CLC_CTRL_03_T         	p0l_clc_ctrl_03;              //0xc001356cL
	PE_M14A0_P0L_CLC_CTRL_04_T         	p0l_clc_ctrl_04;              //0xc0013570L
	PE_M14A0_P0L_CLC_CTRL_05_T         	p0l_clc_ctrl_05;              //0xc0013574L
	PE_M14A0_P0L_CLC_CTRL_06_T         	p0l_clc_ctrl_06;              //0xc0013578L
	PE_M14A0_P0L_CLC_STAT_00_T         	p0l_clc_stat_00;              //0xc001357cL
	PE_M14A0_P0L_VFLT_CTRL_T           	p0l_vflt_ctrl;                //0xc0013580L
	PE_M14A0_P0L_CTI_CTRL_1_T          	p0l_cti_ctrl_1;               //0xc0013584L
	PE_M14A0_P0L_MNR_CTRL_00_T         	p0l_mnr_ctrl_00;              //0xc0013588L
	PE_M14A0_P0L_MNR_CTRL_01_T         	p0l_mnr_ctrl_01;              //0xc001358cL
	PE_M14A0_P0L_MNR_CTRL_02_T         	p0l_mnr_ctrl_02;              //0xc0013590L
	PE_M14A0_P0L_MNR_CTRL_03_T         	p0l_mnr_ctrl_03;              //0xc0013594L
	PE_M14A0_P0L_MNR_CTRL_04_T         	p0l_mnr_ctrl_04;              //0xc0013598L
	PE_M14A0_P0L_MNR_CTRL_05_T         	p0l_mnr_ctrl_05;              //0xc001359cL
	UINT32                             	reserved01;                   //0xc00135a0L
	PE_M14A0_P0L_VFILTER_02_T          	p0l_vfilter_02;               //0xc00135a4L
	PE_M14A0_P0L_VFILTER_01_T          	p0l_vfilter_01;               //0xc00135a8L
	PE_M14A0_P0L_BNR_DC_CTRL_00_T      	p0l_bnr_dc_ctrl_00;           //0xc00135acL
	PE_M14A0_P0L_BNR_DC_CTRL_01_T      	p0l_bnr_dc_ctrl_01;           //0xc00135b0L
	PE_M14A0_P0L_BNR_DC_CTRL_02_T      	p0l_bnr_dc_ctrl_02;           //0xc00135b4L
	PE_M14A0_P0L_BNR_DC_CTRL_03_T      	p0l_bnr_dc_ctrl_03;           //0xc00135b8L
	PE_M14A0_P0L_BNR_AC_CTRL_06_T      	p0l_bnr_ac_ctrl_06;           //0xc00135bcL
	PE_M14A0_P0L_BNR_AC_CTRL_00_T      	p0l_bnr_ac_ctrl_00;           //0xc00135c0L
	PE_M14A0_P0L_BNR_AC_CTRL_01_T      	p0l_bnr_ac_ctrl_01;           //0xc00135c4L
	PE_M14A0_P0L_BNR_AC_CTRL_02_T      	p0l_bnr_ac_ctrl_02;           //0xc00135c8L
	PE_M14A0_P0L_BNR_AC_CTRL_03_T      	p0l_bnr_ac_ctrl_03;           //0xc00135ccL
	PE_M14A0_P0L_BNR_AC_CTRL_04_T      	p0l_bnr_ac_ctrl_04;           //0xc00135d0L
	PE_M14A0_P0L_BNR_AC_CTRL_05_T      	p0l_bnr_ac_ctrl_05;           //0xc00135d4L
	PE_M14A0_P0L_DNR_MAX_CTRL_T        	p0l_dnr_max_ctrl;             //0xc00135d8L
	PE_M14A0_P0L_DNR_DBAR_CTRL_T       	p0l_dnr_dbar_ctrl;            //0xc00135dcL
	PE_M14A0_P0L_BNR_STAT_0_T          	p0l_bnr_stat_0;               //0xc00135e0L
	PE_M14A0_P0L_CTI_CTRL_0_T          	p0l_cti_ctrl_0;               //0xc00135e4L
	PE_M14A0_P0L_REG_C420_CTRL_T       	p0l_reg_c420_ctrl;            //0xc00135e8L
	UINT32                             	reserved02;                   //0xc00135ecL
	PE_M14A0_P0L_IPC_CTRL_20_T         	p0l_ipc_ctrl_20;              //0xc00135f0L
	PE_M14A0_P0L_IPC_CTRL_21_T         	p0l_ipc_ctrl_21;              //0xc00135f4L
	PE_M14A0_P0L_IPC_CTRL_22_T         	p0l_ipc_ctrl_22;              //0xc00135f8L
	PE_M14A0_P0L_IPC_CTRL_23_T         	p0l_ipc_ctrl_23;              //0xc00135fcL
	PE_M14A0_P0L_IPC_CTRL_24_T         	p0l_ipc_ctrl_24;              //0xc0013600L
	PE_M14A0_P0L_IPC_CTRL_25_T         	p0l_ipc_ctrl_25;              //0xc0013604L
	PE_M14A0_P0L_IPC_CTRL_26_T         	p0l_ipc_ctrl_26;              //0xc0013608L
	PE_M14A0_P0L_IPC_CTRL_27_T         	p0l_ipc_ctrl_27;              //0xc001360cL
	PE_M14A0_P0L_IPC_CTRL_28_T         	p0l_ipc_ctrl_28;              //0xc0013610L
	PE_M14A0_P0L_IPC_CTRL_29_T         	p0l_ipc_ctrl_29;              //0xc0013614L
	PE_M14A0_P0L_IPC_CTRL_30_T         	p0l_ipc_ctrl_30;              //0xc0013618L
	PE_M14A0_P0L_IPC_CTRL_31_T         	p0l_ipc_ctrl_31;              //0xc001361cL
	UINT32                             	reserved03[2];                //0xc0013620L~0xc0013624L
	PE_M14A0_P0L_IPC_STATUS_04_T       	p0l_ipc_status_04;            //0xc0013628L
	PE_M14A0_P0L_IPC_STATUS_05_T       	p0l_ipc_status_05;            //0xc001362cL
	PE_M14A0_P0L_IPC_STATUS_06_T       	p0l_ipc_status_06;            //0xc0013630L
	PE_M14A0_P0L_IPC_STATUS_07_T       	p0l_ipc_status_07;            //0xc0013634L
	PE_M14A0_P0L_IPC_STATUS_08_T       	p0l_ipc_status_08;            //0xc0013638L
	PE_M14A0_P0L_IPC_STATUS_09_T       	p0l_ipc_status_09;            //0xc001363cL
	PE_M14A0_P0L_TPD_CTRL_00_T         	p0l_tpd_ctrl_00;              //0xc0013640L
	PE_M14A0_P0L_TPD_CTRL_01_T         	p0l_tpd_ctrl_01;              //0xc0013644L
	PE_M14A0_P0L_TPD_CTRL_02_T         	p0l_tpd_ctrl_02;              //0xc0013648L
	PE_M14A0_P0L_TPD_CTRL_03_T         	p0l_tpd_ctrl_03;              //0xc001364cL
	PE_M14A0_P0L_TPD_CTRL_04_T         	p0l_tpd_ctrl_04;              //0xc0013650L
	PE_M14A0_P0L_TPD_CTRL_05_T         	p0l_tpd_ctrl_05;              //0xc0013654L
	PE_M14A0_P0L_TPD_CTRL_06_T         	p0l_tpd_ctrl_06;              //0xc0013658L
	PE_M14A0_P0L_TPD_CTRL_07_T         	p0l_tpd_ctrl_07;              //0xc001365cL
	PE_M14A0_P0L_TPD_STAT_00_T         	p0l_tpd_stat_00;              //0xc0013660L
	UINT32                             	reserved04[2];                //0xc0013664L~0xc0013668L
	PE_M14A0_P0L_CLC_CTRL_07_T         	p0l_clc_ctrl_07;              //0xc001366cL
	PE_M14A0_P0L_BBD_CTRL_00_T         	p0l_bbd_ctrl_00;              //0xc0013670L
	PE_M14A0_P0L_BBD_CTRL_01_T         	p0l_bbd_ctrl_01;              //0xc0013674L
	PE_M14A0_P0L_BBD_CTRL_02_T         	p0l_bbd_ctrl_02;              //0xc0013678L
	PE_M14A0_P0L_BBD_STAT_00_T         	p0l_bbd_stat_00;              //0xc001367cL
	PE_M14A0_P0L_BBD_STAT_01_T         	p0l_bbd_stat_01;              //0xc0013680L
	PE_M14A0_P0L_APL_STAT_00_T         	p0l_apl_stat_00;              //0xc0013684L
	UINT32                             	reserved05[2];                //0xc0013688L~0xc001368cL
	PE_M14A0_P0L_TPD_HIST_AI_CTRL_T    	p0l_tpd_hist_ai_ctrl;         //0xc0013690L
	PE_M14A0_P0L_TPD_HIST_AI_DATA_T    	p0l_tpd_hist_ai_data;         //0xc0013694L
	PE_M14A0_P0L_LVCRTL_CTRL_00_T      	p0l_lvcrtl_ctrl_00;           //0xc0013698L
	UINT32                             	reserved06;                   //0xc001369cL
	PE_M14A0_P0L_3DFD_CTRL_00_T        	p0l_3dfd_ctrl_00;             //0xc00136a0L
	PE_M14A0_P0L_3DFD_CTRL_01_T        	p0l_3dfd_ctrl_01;             //0xc00136a4L
	PE_M14A0_P0L_3DFD_CTRL_02_T        	p0l_3dfd_ctrl_02;             //0xc00136a8L
	PE_M14A0_P0L_3DFD_CTRL_03_T        	p0l_3dfd_ctrl_03;             //0xc00136acL
	PE_M14A0_P0L_3DFD_STAT_00_T        	p0l_3dfd_stat_00;             //0xc00136b0L
	PE_M14A0_P0L_3DFD_STAT_01_T        	p0l_3dfd_stat_01;             //0xc00136b4L
	PE_M14A0_P0L_3DFD_STAT_02_T        	p0l_3dfd_stat_02;             //0xc00136b8L
	PE_M14A0_P0L_3DFD_STAT_03_T        	p0l_3dfd_stat_03;             //0xc00136bcL
	PE_M14A0_P0L_3DFD_STAT_04_T        	p0l_3dfd_stat_04;             //0xc00136c0L
	PE_M14A0_P0L_3DFD_STAT_05_T        	p0l_3dfd_stat_05;             //0xc00136c4L
	PE_M14A0_P0L_3DFD_STAT_06_T        	p0l_3dfd_stat_06;             //0xc00136c8L
	PE_M14A0_P0L_3DFD_STAT_07_T        	p0l_3dfd_stat_07;             //0xc00136ccL
	PE_M14A0_P0L_TNR_CTRL_38_T         	p0l_tnr_ctrl_38;              //0xc00136d0L
	PE_M14A0_P0L_TNR_CTRL_39_T         	p0l_tnr_ctrl_39;              //0xc00136d4L
	PE_M14A0_P0L_TNR_CTRL_40_T         	p0l_tnr_ctrl_40;              //0xc00136d8L
	PE_M14A0_P0L_TNR_CTRL_41_T         	p0l_tnr_ctrl_41;              //0xc00136dcL
	PE_M14A0_P0L_TNR_CTRL_42_T         	p0l_tnr_ctrl_42;              //0xc00136e0L
	PE_M14A0_P0L_TNR_CTRL_43_T         	p0l_tnr_ctrl_43;              //0xc00136e4L
	PE_M14A0_P0L_TNR_CTRL_44_T         	p0l_tnr_ctrl_44;              //0xc00136e8L
	PE_M14A0_P0L_TNR_CTRL_45_T         	p0l_tnr_ctrl_45;              //0xc00136ecL
	PE_M14A0_P0L_TNR_CTRL_46_T         	p0l_tnr_ctrl_46;              //0xc00136f0L
	PE_M14A0_P0L_TNR_CTRL_47_T         	p0l_tnr_ctrl_47;              //0xc00136f4L
	PE_M14A0_P0L_TNR_CTRL_48_T         	p0l_tnr_ctrl_48;              //0xc00136f8L
}PE_P0L_REG_M14A0_T;

typedef struct {
	PE_M14A0_P0R_PE0_LOAD_T            	p0r_pe0_load;                 //0xc0015400L
	PE_M14A0_P0R_PE0_INTR_T            	p0r_pe0_intr;                 //0xc0015404L
	PE_M14A0_P0R_PE0_STATUS0_T         	p0r_pe0_status0;              //0xc0015408L
	PE_M14A0_P0R_PE0_STATUS1_T         	p0r_pe0_status1;              //0xc001540cL
	PE_M14A0_P0R_PE0_OPMODE_T          	p0r_pe0_opmode;               //0xc0015410L
	PE_M14A0_P0R_PE0_SRC_SIZE_T        	p0r_pe0_src_size;             //0xc0015414L
	PE_M14A0_P0R_PE0_FIELD_T           	p0r_pe0_field;                //0xc0015418L
	PE_M14A0_P0R_PE0_PARAM_BY_DDR_T    	p0r_pe0_param_by_ddr;         //0xc001541cL
	PE_M14A0_P0R_WIN_CTRL_0_T          	p0r_win_ctrl_0;               //0xc0015420L
	PE_M14A0_P0R_WIN_CTRL_1_T          	p0r_win_ctrl_1;               //0xc0015424L
	PE_M14A0_P0R_WIN_CTRL_2_T          	p0r_win_ctrl_2;               //0xc0015428L
	PE_M14A0_P0R_WIN_CTRL_3_T          	p0r_win_ctrl_3;               //0xc001542cL
	PE_M14A0_P0R_PE0_DEBUG_MEM_REQ_T   	p0r_pe0_debug_mem_req;        //0xc0015430L
	PE_M14A0_P0R_PE0_DEBUG_LREQ_DLY_T  	p0r_pe0_debug_lreq_dly;       //0xc0015434L
	UINT32                             	reserved00[2];                //0xc0015438L~0xc001543cL
	PE_M14A0_P0R_TNR_CTRL_00_T         	p0r_tnr_ctrl_00;              //0xc0015440L
	PE_M14A0_P0R_TNR_CTRL_01_T         	p0r_tnr_ctrl_01;              //0xc0015444L
	PE_M14A0_P0R_TNR_CTRL_02_T         	p0r_tnr_ctrl_02;              //0xc0015448L
	PE_M14A0_P0R_TNR_CTRL_03_T         	p0r_tnr_ctrl_03;              //0xc001544cL
	PE_M14A0_P0R_TNR_CTRL_04_T         	p0r_tnr_ctrl_04;              //0xc0015450L
	PE_M14A0_P0R_TNR_CTRL_05_T         	p0r_tnr_ctrl_05;              //0xc0015454L
	PE_M14A0_P0R_TNR_CTRL_06_T         	p0r_tnr_ctrl_06;              //0xc0015458L
	PE_M14A0_P0R_TNR_CTRL_07_T         	p0r_tnr_ctrl_07;              //0xc001545cL
	PE_M14A0_P0R_TNR_CTRL_08_T         	p0r_tnr_ctrl_08;              //0xc0015460L
	PE_M14A0_P0R_TNR_CTRL_09_T         	p0r_tnr_ctrl_09;              //0xc0015464L
	PE_M14A0_P0R_TNR_CTRL_10_T         	p0r_tnr_ctrl_10;              //0xc0015468L
	PE_M14A0_P0R_TNR_CTRL_11_T         	p0r_tnr_ctrl_11;              //0xc001546cL
	PE_M14A0_P0R_TNR_CTRL_12_T         	p0r_tnr_ctrl_12;              //0xc0015470L
	PE_M14A0_P0R_TNR_CTRL_13_T         	p0r_tnr_ctrl_13;              //0xc0015474L
	PE_M14A0_P0R_TNR_CTRL_14_T         	p0r_tnr_ctrl_14;              //0xc0015478L
	PE_M14A0_P0R_TNR_CTRL_15_T         	p0r_tnr_ctrl_15;              //0xc001547cL
	PE_M14A0_P0R_TNR_CTRL_16_T         	p0r_tnr_ctrl_16;              //0xc0015480L
	PE_M14A0_P0R_TNR_CTRL_17_T         	p0r_tnr_ctrl_17;              //0xc0015484L
	PE_M14A0_P0R_TNR_CTRL_18_T         	p0r_tnr_ctrl_18;              //0xc0015488L
	PE_M14A0_P0R_TNR_CTRL_19_T         	p0r_tnr_ctrl_19;              //0xc001548cL
	PE_M14A0_P0R_TNR_CTRL_20_T         	p0r_tnr_ctrl_20;              //0xc0015490L
	PE_M14A0_P0R_TNR_CTRL_21_T         	p0r_tnr_ctrl_21;              //0xc0015494L
	PE_M14A0_P0R_TNR_CTRL_22_T         	p0r_tnr_ctrl_22;              //0xc0015498L
	PE_M14A0_P0R_TNR_CTRL_23_T         	p0r_tnr_ctrl_23;              //0xc001549cL
	PE_M14A0_P0R_TNR_CTRL_24_T         	p0r_tnr_ctrl_24;              //0xc00154a0L
	PE_M14A0_P0R_TNR_CTRL_25_T         	p0r_tnr_ctrl_25;              //0xc00154a4L
	PE_M14A0_P0R_TNR_CTRL_26_T         	p0r_tnr_ctrl_26;              //0xc00154a8L
	PE_M14A0_P0R_TNR_CTRL_27_T         	p0r_tnr_ctrl_27;              //0xc00154acL
	PE_M14A0_P0R_TNR_CTRL_28_T         	p0r_tnr_ctrl_28;              //0xc00154b0L
	PE_M14A0_P0R_TNR_CTRL_29_T         	p0r_tnr_ctrl_29;              //0xc00154b4L
	PE_M14A0_P0R_TNR_CTRL_30_T         	p0r_tnr_ctrl_30;              //0xc00154b8L
	PE_M14A0_P0R_TNR_CTRL_31_T         	p0r_tnr_ctrl_31;              //0xc00154bcL
	PE_M14A0_P0R_TNR_STATUS_00_T       	p0r_tnr_status_00;            //0xc00154c0L
	PE_M14A0_P0R_TNR_STATUS_01_T       	p0r_tnr_status_01;            //0xc00154c4L
	PE_M14A0_P0R_TNR_STATUS_02_T       	p0r_tnr_status_02;            //0xc00154c8L
	PE_M14A0_P0R_TNR_STATUS_03_T       	p0r_tnr_status_03;            //0xc00154ccL
	PE_M14A0_P0R_TNR_MAIN_LUT_00_T     	p0r_tnr_main_lut_00;          //0xc00154d0L
	PE_M14A0_P0R_TNR_MAIN_LUT_01_T     	p0r_tnr_main_lut_01;          //0xc00154d4L
	PE_M14A0_P0R_TNR_MAIN_LUT_02_T     	p0r_tnr_main_lut_02;          //0xc00154d8L
	PE_M14A0_P0R_TNR_STATUS_04_T       	p0r_tnr_status_04;            //0xc00154dcL
	PE_M14A0_P0R_TNR_CTRL_32_T         	p0r_tnr_ctrl_32;              //0xc00154e0L
	PE_M14A0_P0R_TNR_CTRL_33_T         	p0r_tnr_ctrl_33;              //0xc00154e4L
	PE_M14A0_P0R_TNR_CTRL_34_T         	p0r_tnr_ctrl_34;              //0xc00154e8L
	PE_M14A0_P0R_TNR_CTRL_35_T         	p0r_tnr_ctrl_35;              //0xc00154ecL
	PE_M14A0_P0R_TNR_CRG_CTRL_00_T     	p0r_tnr_crg_ctrl_00;          //0xc00154f0L
	PE_M14A0_P0R_TNR_CRG_CTRL_01_T     	p0r_tnr_crg_ctrl_01;          //0xc00154f4L
	PE_M14A0_P0R_TNR_CTRL_36_T         	p0r_tnr_ctrl_36;              //0xc00154f8L
	PE_M14A0_P0R_TNR_CTRL_37_T         	p0r_tnr_ctrl_37;              //0xc00154fcL
	PE_M14A0_P0R_IPC_CTRL_00_T         	p0r_ipc_ctrl_00;              //0xc0015500L
	PE_M14A0_P0R_IPC_CTRL_01_T         	p0r_ipc_ctrl_01;              //0xc0015504L
	PE_M14A0_P0R_IPC_CTRL_02_T         	p0r_ipc_ctrl_02;              //0xc0015508L
	PE_M14A0_P0R_IPC_CTRL_03_T         	p0r_ipc_ctrl_03;              //0xc001550cL
	PE_M14A0_P0R_IPC_CTRL_04_T         	p0r_ipc_ctrl_04;              //0xc0015510L
	PE_M14A0_P0R_IPC_CTRL_05_T         	p0r_ipc_ctrl_05;              //0xc0015514L
	PE_M14A0_P0R_IPC_CTRL_06_T         	p0r_ipc_ctrl_06;              //0xc0015518L
	PE_M14A0_P0R_IPC_CTRL_07_T         	p0r_ipc_ctrl_07;              //0xc001551cL
	PE_M14A0_P0R_IPC_CTRL_08_T         	p0r_ipc_ctrl_08;              //0xc0015520L
	PE_M14A0_P0R_IPC_CTRL_09_T         	p0r_ipc_ctrl_09;              //0xc0015524L
	PE_M14A0_P0R_IPC_CTRL_10_T         	p0r_ipc_ctrl_10;              //0xc0015528L
	PE_M14A0_P0R_IPC_CTRL_11_T         	p0r_ipc_ctrl_11;              //0xc001552cL
	PE_M14A0_P0R_IPC_CTRL_12_T         	p0r_ipc_ctrl_12;              //0xc0015530L
	PE_M14A0_P0R_IPC_CTRL_13_T         	p0r_ipc_ctrl_13;              //0xc0015534L
	PE_M14A0_P0R_IPC_CTRL_14_T         	p0r_ipc_ctrl_14;              //0xc0015538L
	PE_M14A0_P0R_IPC_CTRL_15_T         	p0r_ipc_ctrl_15;              //0xc001553cL
	PE_M14A0_P0R_IPC_CTRL_16_T         	p0r_ipc_ctrl_16;              //0xc0015540L
	PE_M14A0_P0R_IPC_CTRL_17_T         	p0r_ipc_ctrl_17;              //0xc0015544L
	PE_M14A0_P0R_IPC_CTRL_18_T         	p0r_ipc_ctrl_18;              //0xc0015548L
	PE_M14A0_P0R_IPC_CTRL_19_T         	p0r_ipc_ctrl_19;              //0xc001554cL
	PE_M14A0_P0R_IPC_STATUS_00_T       	p0r_ipc_status_00;            //0xc0015550L
	PE_M14A0_P0R_IPC_STATUS_01_T       	p0r_ipc_status_01;            //0xc0015554L
	PE_M14A0_P0R_IPC_STATUS_02_T       	p0r_ipc_status_02;            //0xc0015558L
	PE_M14A0_P0R_IPC_STATUS_03_T       	p0r_ipc_status_03;            //0xc001555cL
	UINT32                             	reserved01[8];                //0xc0015560L~0xc001557cL
	PE_M14A0_P0R_VFILTER_00_T          	p0r_vfilter_00;               //0xc0015580L
	PE_M14A0_P0R_CTI_CTRL_1_T          	p0r_cti_ctrl_1;               //0xc0015584L
	PE_M14A0_P0R_MNR_CTRL_00_T         	p0r_mnr_ctrl_00;              //0xc0015588L
	PE_M14A0_P0R_MNR_CTRL_01_T         	p0r_mnr_ctrl_01;              //0xc001558cL
	PE_M14A0_P0R_MNR_CTRL_02_T         	p0r_mnr_ctrl_02;              //0xc0015590L
	PE_M14A0_P0R_MNR_CTRL_03_T         	p0r_mnr_ctrl_03;              //0xc0015594L
	PE_M14A0_P0R_MNR_CTRL_04_T         	p0r_mnr_ctrl_04;              //0xc0015598L
	PE_M14A0_P0R_MNR_CTRL_05_T         	p0r_mnr_ctrl_05;              //0xc001559cL
	UINT32                             	reserved02;                   //0xc00155a0L
	PE_M14A0_P0R_VFILTER_02_T          	p0r_vfilter_02;               //0xc00155a4L
	PE_M14A0_P0R_VFILTER_01_T          	p0r_vfilter_01;               //0xc00155a8L
	PE_M14A0_P0R_BNR_DC_CTRL_00_T      	p0r_bnr_dc_ctrl_00;           //0xc00155acL
	PE_M14A0_P0R_BNR_DC_CTRL_01_T      	p0r_bnr_dc_ctrl_01;           //0xc00155b0L
	PE_M14A0_P0R_BNR_DC_CTRL_02_T      	p0r_bnr_dc_ctrl_02;           //0xc00155b4L
	PE_M14A0_P0R_BNR_DC_CTRL_03_T      	p0r_bnr_dc_ctrl_03;           //0xc00155b8L
	PE_M14A0_P0R_BNR_AC_CTRL_06_T      	p0r_bnr_ac_ctrl_06;           //0xc00155bcL
	PE_M14A0_P0R_BNR_AC_CTRL_00_T      	p0r_bnr_ac_ctrl_00;           //0xc00155c0L
	PE_M14A0_P0R_BNR_AC_CTRL_01_T      	p0r_bnr_ac_ctrl_01;           //0xc00155c4L
	PE_M14A0_P0R_BNR_AC_CTRL_02_T      	p0r_bnr_ac_ctrl_02;           //0xc00155c8L
	PE_M14A0_P0R_BNR_AC_CTRL_03_T      	p0r_bnr_ac_ctrl_03;           //0xc00155ccL
	PE_M14A0_P0R_BNR_AC_CTRL_04_T      	p0r_bnr_ac_ctrl_04;           //0xc00155d0L
	PE_M14A0_P0R_BNR_AC_CTRL_05_T      	p0r_bnr_ac_ctrl_05;           //0xc00155d4L
	PE_M14A0_P0R_DNR_MAX_CTRL_T        	p0r_dnr_max_ctrl;             //0xc00155d8L
	PE_M14A0_P0R_DNR_DBAR_CTRL_T       	p0r_dnr_dbar_ctrl;            //0xc00155dcL
	PE_M14A0_P0R_BNR_STAT_0_T          	p0r_bnr_stat_0;               //0xc00155e0L
	PE_M14A0_P0R_CTI_CTRL_0_T          	p0r_cti_ctrl_0;               //0xc00155e4L
	PE_M14A0_P0R_REG_C420_CTRL_T       	p0r_reg_c420_ctrl;            //0xc00155e8L
	UINT32                             	reserved03;                   //0xc00155ecL
	PE_M14A0_P0R_IPC_CTRL_20_T         	p0r_ipc_ctrl_20;              //0xc00155f0L
	PE_M14A0_P0R_IPC_CTRL_21_T         	p0r_ipc_ctrl_21;              //0xc00155f4L
	PE_M14A0_P0R_IPC_CTRL_22_T         	p0r_ipc_ctrl_22;              //0xc00155f8L
	PE_M14A0_P0R_IPC_CTRL_23_T         	p0r_ipc_ctrl_23;              //0xc00155fcL
	PE_M14A0_P0R_IPC_CTRL_24_T         	p0r_ipc_ctrl_24;              //0xc0015600L
	PE_M14A0_P0R_IPC_CTRL_25_T         	p0r_ipc_ctrl_25;              //0xc0015604L
	PE_M14A0_P0R_IPC_CTRL_26_T         	p0r_ipc_ctrl_26;              //0xc0015608L
	PE_M14A0_P0R_IPC_CTRL_27_T         	p0r_ipc_ctrl_27;              //0xc001560cL
	PE_M14A0_P0R_IPC_CTRL_28_T         	p0r_ipc_ctrl_28;              //0xc0015610L
	PE_M14A0_P0R_IPC_CTRL_29_T         	p0r_ipc_ctrl_29;              //0xc0015614L
	PE_M14A0_P0R_IPC_CTRL_30_T         	p0r_ipc_ctrl_30;              //0xc0015618L
	PE_M14A0_P0R_IPC_CTRL_31_T         	p0r_ipc_ctrl_31;              //0xc001561cL
	UINT32                             	reserved04[2];                //0xc0015620L~0xc0015624L
	PE_M14A0_P0R_IPC_STATUS_04_T       	p0r_ipc_status_04;            //0xc0015628L
	PE_M14A0_P0R_IPC_STATUS_05_T       	p0r_ipc_status_05;            //0xc001562cL
	PE_M14A0_P0R_IPC_STATUS_06_T       	p0r_ipc_status_06;            //0xc0015630L
	PE_M14A0_P0R_IPC_STATUS_07_T       	p0r_ipc_status_07;            //0xc0015634L
	PE_M14A0_P0R_IPC_STATUS_08_T       	p0r_ipc_status_08;            //0xc0015638L
	PE_M14A0_P0R_IPC_STATUS_09_T       	p0r_ipc_status_09;            //0xc001563cL
	PE_M14A0_P0R_TPD_CTRL_00_T         	p0r_tpd_ctrl_00;              //0xc0015640L
	PE_M14A0_P0R_TPD_CTRL_01_T         	p0r_tpd_ctrl_01;              //0xc0015644L
	PE_M14A0_P0R_TPD_CTRL_02_T         	p0r_tpd_ctrl_02;              //0xc0015648L
	PE_M14A0_P0R_TPD_CTRL_03_T         	p0r_tpd_ctrl_03;              //0xc001564cL
	PE_M14A0_P0R_TPD_CTRL_04_T         	p0r_tpd_ctrl_04;              //0xc0015650L
	PE_M14A0_P0R_TPD_CTRL_05_T         	p0r_tpd_ctrl_05;              //0xc0015654L
	PE_M14A0_P0R_TPD_CTRL_06_T         	p0r_tpd_ctrl_06;              //0xc0015658L
	PE_M14A0_P0R_TPD_CTRL_07_T         	p0r_tpd_ctrl_07;              //0xc001565cL
	PE_M14A0_P0R_TPD_STAT_00_T         	p0r_tpd_stat_00;              //0xc0015660L
	UINT32                             	reserved05[3];                //0xc0015664L~0xc001566cL
	PE_M14A0_P0R_BBD_CTRL_00_T         	p0r_bbd_ctrl_00;              //0xc0015670L
	PE_M14A0_P0R_BBD_CTRL_01_T         	p0r_bbd_ctrl_01;              //0xc0015674L
	PE_M14A0_P0R_BBD_CTRL_02_T         	p0r_bbd_ctrl_02;              //0xc0015678L
	PE_M14A0_P0R_BBD_STAT_00_T         	p0r_bbd_stat_00;              //0xc001567cL
	PE_M14A0_P0R_BBD_STAT_01_T         	p0r_bbd_stat_01;              //0xc0015680L
	PE_M14A0_P0R_APL_STAT_00_T         	p0r_apl_stat_00;              //0xc0015684L
	UINT32                             	reserved06[2];                //0xc0015688L~0xc001568cL
	PE_M14A0_P0R_TPD_HIST_AI_CTRL_T    	p0r_tpd_hist_ai_ctrl;         //0xc0015690L
	PE_M14A0_P0R_TPD_HIST_AI_DATA_T    	p0r_tpd_hist_ai_data;         //0xc0015694L
	PE_M14A0_P0R_LVCRTL_CTRL_00_T      	p0r_lvcrtl_ctrl_00;           //0xc0015698L
	UINT32                             	reserved07[13];               //0xc001569cL~0xc00156ccL
	PE_M14A0_P0R_TNR_CTRL_38_T         	p0r_tnr_ctrl_38;              //0xc00156d0L
	PE_M14A0_P0R_TNR_CTRL_39_T         	p0r_tnr_ctrl_39;              //0xc00156d4L
	PE_M14A0_P0R_TNR_CTRL_40_T         	p0r_tnr_ctrl_40;              //0xc00156d8L
	PE_M14A0_P0R_TNR_CTRL_41_T         	p0r_tnr_ctrl_41;              //0xc00156dcL
	PE_M14A0_P0R_TNR_CTRL_42_T         	p0r_tnr_ctrl_42;              //0xc00156e0L
	PE_M14A0_P0R_TNR_CTRL_43_T         	p0r_tnr_ctrl_43;              //0xc00156e4L
	PE_M14A0_P0R_TNR_CTRL_44_T         	p0r_tnr_ctrl_44;              //0xc00156e8L
	PE_M14A0_P0R_TNR_CTRL_45_T         	p0r_tnr_ctrl_45;              //0xc00156ecL
	PE_M14A0_P0R_TNR_CTRL_46_T         	p0r_tnr_ctrl_46;              //0xc00156f0L
	PE_M14A0_P0R_TNR_CTRL_47_T         	p0r_tnr_ctrl_47;              //0xc00156f4L
	PE_M14A0_P0R_TNR_CTRL_48_T         	p0r_tnr_ctrl_48;              //0xc00156f8L
}PE_P0R_REG_M14A0_T;
#endif
