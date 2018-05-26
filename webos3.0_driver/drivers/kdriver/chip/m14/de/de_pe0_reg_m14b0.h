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

#ifndef _REG_PE0_M14B0_H_
#define _REG_PE0_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0022100L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} M14B0_PE0_M_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc0022104L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16     
} M14B0_PE0_M_INTR_T;

/*-----------------------------------------------------------------------------
		0xc0022108L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_frame_id                       : 4 ;  //  3: 0     
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4     
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8     
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12     
	UINT32 dnr_frame_id                       : 8 ;  // 23:16     
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24     
} M14B0_PE0_M_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc002210cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16     
} M14B0_PE0_M_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc0022110L pe0_opmode ''
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
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 tnr_m_en                           : 1 ;  //    13     
	UINT32 force_444_to_422                   : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 detour_enable                      : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ibuf_en                            : 2 ;  // 21:20     
	UINT32 minfo_vhalf_mode                   : 2 ;  // 23:22     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 tnr_in_lsb_mode                    : 2 ;  // 29:28     
	UINT32 m_run_id                           : 2 ;  // 31:30     
} M14B0_PE0_M_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc0022114L pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0     
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 cs_type                            : 3 ;  // 15:13     
	UINT32 vsize                              : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31     
} M14B0_PE0_M_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0022118L pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4     
	UINT32 field_inv                          : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 secam_line                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12     
} M14B0_PE0_M_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc0022130L pe0_debug_mem_req ''
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
} M14B0_PE0_M_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0xc0022134L pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0     
	UINT32 lreq_dly_c_en                      : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16     
	UINT32 lreq_dly_y_en                      : 1 ;  //    27     
} M14B0_PE0_M_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0022140L tnr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tnr_enable                         : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 sc_detect_hres                     : 1 ;  //     2     
	UINT32 clc_mem_wr_mode                    : 1 ;  //     3     
	UINT32                                    : 6 ;  //  9: 4     reserved
	UINT32 minfo_sw                           : 2 ;  // 11:10     
	UINT32 minfo_maerr_mamotion_mux           : 2 ;  // 13:12     
	UINT32 minfo_bli_en_tnr                   : 1 ;  //    14     
	UINT32 minfo_bli_en_clc                   : 1 ;  //    15     
	UINT32 dnr_motion_scale                   : 2 ;  // 17:16     
	UINT32 minfo_mv_sel                       : 1 ;  //    18     
	UINT32 minfo_meconf_sel                   : 1 ;  //    19     
	UINT32 minfo_maerr_mamotion_4b_mode       : 1 ;  //    20     
	UINT32 minfo_me_vavg_en                   : 1 ;  //    21     
	UINT32 minfo_ma_vavg_en                   : 1 ;  //    22     
} M14B0_TNR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0022144L tnr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_out_mode                     : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 debug_tnrw_mode                    : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 tnr_black_line_en_for_i            : 1 ;  //    20     
	UINT32 tnr_debug_map_en_for_i             : 1 ;  //    21     
} M14B0_TNR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0022148L tnr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_en                              : 1 ;  //     0     
	UINT32 sc_max_flt_en                      : 1 ;  //     1     
	UINT32 sc_calc_prev_hist                  : 1 ;  //     2     
	UINT32 sc_chroma_en                       : 1 ;  //     3     
	UINT32 sc_scale                           : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 sc_max_flt_tap                     : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 sc_chroma_gain                     : 3 ;  // 14:12     
	UINT32 sc_out_mode                        : 1 ;  //    15     
	UINT32 sc_th_ma                           : 8 ;  // 23:16     
	UINT32 sc_th_memc                         : 8 ;  // 31:24     
} M14B0_TNR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002214cL tnr_ctrl_03 ''
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
} M14B0_TNR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0022150L tnr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 global_x0                          : 8 ;  //  7: 0     
	UINT32 global_x1                          : 8 ;  // 15: 8     
	UINT32 global_x2                          : 8 ;  // 23:16     
	UINT32 global_y0                          : 4 ;  // 27:24     
	UINT32 global_y1                          : 4 ;  // 31:28     
} M14B0_TNR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0022154L tnr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 global_y2                          : 4 ;  //  3: 0     
	UINT32 global_y3                          : 4 ;  //  7: 4     
} M14B0_TNR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0022158L tnr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 stat_var_th                        : 8 ;  //  7: 0     
	UINT32 mv_var_res                         : 2 ;  //  9: 8     
	UINT32 ma_var_max_en                      : 1 ;  //    10     
	UINT32 var_sel                            : 1 ;  //    11     
	UINT32 memc_debug_sel                     : 2 ;  // 13:12     
	UINT32 me_conf_sel_cc                     : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 stat_me_conf_th                    : 8 ;  // 23:16     
} M14B0_TNR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002215cL tnr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 global_refine_en                   : 1 ;  //     1     
	UINT32 past_refine_en                     : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 past_refine_ratio                  : 4 ;  //  7: 4     
	UINT32 global_past_refine_ratio           : 4 ;  // 11: 8     
	UINT32 global_mv_h                        : 8 ;  // 19:12     
	UINT32 global_mv_v                        : 4 ;  // 23:20     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 conf_sel                           : 1 ;  //    30     
	UINT32 refine_ratio_auto                  : 1 ;  //    31     
} M14B0_TNR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0022160L tnr_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_h_fix_value                     : 8 ;  //  7: 0     
	UINT32 mv_v_fix_value                     : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 me_debug_sel                       : 3 ;  // 18:16     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 mv_refine_ratio                    : 4 ;  // 27:24     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 me_intput_sel                      : 1 ;  //    30     
	UINT32 mv_fix_en                          : 1 ;  //    31     
} M14B0_TNR_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0022164L tnr_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 temporal_refine_adj1               : 3 ;  //  2: 0     
	UINT32 temporal_refine_adj2               : 3 ;  //  5: 3     
	UINT32 zero_refine_en                     : 1 ;  //     6     
	UINT32 reserved0                          : 3 ;  //  9: 7     
	UINT32 reserved1                          : 3 ;  // 12:10     
	UINT32                                    : 1 ;  //    13     reserved
	UINT32 conf_mul_sel                       : 2 ;  // 15:14     
	UINT32 conf_iir_sel                       : 2 ;  // 17:16     
	UINT32 mv_refine_mv_reverse               : 1 ;  //    18     
	UINT32 sad_chroma_gain                    : 3 ;  // 21:19     
	UINT32 ma_conf_iir_en                     : 1 ;  //    22     
	UINT32 refine_chroma_gain                 : 3 ;  // 25:23     
	UINT32 reserved2                          : 1 ;  //    26     
	UINT32 zero_refine_ratio                  : 4 ;  // 30:27     
} M14B0_TNR_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0022168L tnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0     
} M14B0_TNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc002216cL tnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_sad_diff_ratio                 : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 zero_sad_diff_ratio                : 2 ;  //  5: 4     
	UINT32                                    : 7 ;  // 12: 6     reserved
	UINT32 past_refine_conf_sel               : 1 ;  //    13     
	UINT32                                    : 4 ;  // 17:14     reserved
	UINT32 sad_variance_scale                 : 4 ;  // 21:18     
	UINT32 sad_variance_h_gain                : 4 ;  // 25:22     
	UINT32 sad_variance_v_gain                : 4 ;  // 29:26     
	UINT32 mc_chroma_enhance_en               : 1 ;  //    30     
	UINT32 refine_chroma_enhance_en           : 1 ;  //    31     
} M14B0_TNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0022170L tnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 me_conf_scale_ma                   : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 me_conf_scale_mc                   : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 half_pel_mode                      : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reserved0                          : 1 ;  //    12     
	UINT32 reserved1                          : 19;  // 31:13     
} M14B0_TNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0022174L tnr_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_bnd_motion                       : 8 ;  //  7: 0     
	UINT32 v_bnd_motion                       : 8 ;  // 15: 8     
	UINT32 reserved                           : 16;  // 31:16     
} M14B0_TNR_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0022178L tnr_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32 manual_alpha_en                    : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 th_mode                            : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 alpha_gain                         : 8 ;  // 15: 8     
	UINT32 th_value                           : 8 ;  // 23:16     
	UINT32 manual_alpha_val                   : 8 ;  // 31:24     
} M14B0_TNR_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc002217cL tnr_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_zero_vector_th               : 10;  //  9: 0     
	UINT32 force_zero_vector_th2              : 8 ;  // 17:10     
} M14B0_TNR_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0022180L tnr_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mcb_mmd_preflt                     : 2 ;  //  1: 0     
	UINT32 mcb_mmd_postflt                    : 2 ;  //  3: 2     
	UINT32 am_th_mode                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 am_th_val                          : 8 ;  // 15: 8     
} M14B0_TNR_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0022184L tnr_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mmd_th_x0                          : 8 ;  //  7: 0     
	UINT32 mmd_th_x1                          : 8 ;  // 15: 8     
	UINT32 mmd_th_y1                          : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 cc_flt_enable                      : 1 ;  //    24     
} M14B0_TNR_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0022188L tnr_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mcb_en                             : 1 ;  //     0     
	UINT32 mcblnd_mode                        : 2 ;  //  2: 1     
	UINT32 mcb_tap                            : 1 ;  //     3     
	UINT32 mcb_th_mode                        : 2 ;  //  5: 4     
	UINT32 mcblnd_w_mode                      : 2 ;  //  7: 6     
	UINT32 mcblnd_th                          : 8 ;  // 15: 8     
	UINT32 mcblnd_th_mode                     : 2 ;  // 17:16     
	UINT32 motion_th_mode                     : 2 ;  // 19:18     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 motion_th                          : 8 ;  // 31:24     
} M14B0_TNR_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc002218cL tnr_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mcblnd_th_x0                       : 8 ;  //  7: 0     
	UINT32 mcblnd_th_x1                       : 8 ;  // 15: 8     
	UINT32 motion_c_gain                      : 6 ;  // 21:16     
	UINT32 mc_en                              : 1 ;  //    22     
	UINT32 manual_alpha_en                    : 1 ;  //    23     
	UINT32 manual_alpha                       : 8 ;  // 31:24     
} M14B0_TNR_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0022190L tnr_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_lsb2_mode                       : 3 ;  //  2: 0     
	UINT32 oob_mode                           : 1 ;  //     3     
	UINT32 alpha_mode                         : 2 ;  //  5: 4     
	UINT32 alpha_gain_en                      : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 alpha_gain_y                       : 6 ;  // 13: 8     
	UINT32 diff_map_gain                      : 2 ;  // 15:14     
	UINT32 alpha_gain_c                       : 6 ;  // 21:16     
	UINT32 sc_alpha_mode                      : 2 ;  // 23:22     
	UINT32 motion_gain                        : 8 ;  // 31:24     
} M14B0_TNR_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc0022194L tnr_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mc_debug_mode                      : 3 ;  //  2: 0     
	UINT32 mc_debug_sel                       : 3 ;  //  5: 3     
	UINT32 ma_error_scaling                   : 2 ;  //  7: 6     
	UINT32 sf_en                              : 1 ;  //     8     
	UINT32 sf_map_tap                         : 1 ;  //     9     
	UINT32 sf_filter_tap                      : 1 ;  //    10     
	UINT32 sf_map_flt_en                      : 1 ;  //    11     
	UINT32 sf_map_gain                        : 6 ;  // 17:12     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 sf_var_th_mode                     : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 sf_var_th_val                      : 8 ;  // 31:24     
} M14B0_TNR_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc0022198L tnr_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mcb_gain_en                        : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 mcb_gain                           : 8 ;  // 15: 8     
} M14B0_TNR_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc002219cL tnr_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sf_mode                            : 2 ;  //  1: 0     
	UINT32 sf_motion_th_mode                  : 2 ;  //  3: 2     
	UINT32 chroma_flt_cc_en                   : 1 ;  //     4     
	UINT32 chroma_flt_p2_en                   : 1 ;  //     5     
	UINT32 sad_x_meconf_en                    : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 me_conf_gain                       : 8 ;  // 15: 8     
	UINT32 sf_motion_th_val                   : 8 ;  // 23:16     
	UINT32 chroma_flt_gain                    : 6 ;  // 29:24     
	UINT32 mc_error_scaling                   : 2 ;  // 31:30     
} M14B0_TNR_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc00221a0L tnr_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ma_top_mc_enable                   : 1 ;  //     0     
	UINT32 ma_top_ma_enable                   : 1 ;  //     1     
	UINT32 ma_top_ma_force_en                 : 1 ;  //     2     
	UINT32 ma_top_ma_err_mw_sel               : 1 ;  //     3     
	UINT32 ma_top_low_2bit_fix                : 1 ;  //     4     
	UINT32 ma_top_secam_line_toggl            : 1 ;  //     5     
	UINT32 ma_top_low_2bit                    : 2 ;  //  7: 6     
	UINT32 ma_top_luma_adap_en                : 1 ;  //     8     
	UINT32 ma_top_g_motion_mode               : 1 ;  //     9     
	UINT32 ma_top_mw_4bit_mode                : 1 ;  //    10     
	UINT32 ma_top_skin_adap_en                : 1 ;  //    11     
	UINT32 ma_top_secam_mode_en               : 1 ;  //    12     
	UINT32 ma_top_secam_xn                    : 3 ;  // 15:13     
	UINT32 ma_top_ma_force_gain               : 8 ;  // 23:16     
	UINT32 ma_top_sc_reset_mode               : 2 ;  // 25:24     
	UINT32 ma_top_sc_mc_off                   : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 ma_top_debug_show                  : 4 ;  // 31:28     
} M14B0_TNR_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc00221a4L tnr_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_preblur_en              : 1 ;  //     0     
	UINT32 reg_var_preblur_en                 : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_f_variance_res                 : 4 ;  // 11: 8     
	UINT32 reg_f_motion_res                   : 4 ;  // 15:12     
	UINT32 reg_chroma_gain                    : 8 ;  // 23:16     
	UINT32 reg_g_blend_a                      : 8 ;  // 31:24     
} M14B0_TNR_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc00221a8L tnr_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 reg_cr_max_gain                    : 8 ;  // 15: 8     
	UINT32 ma_variance_chroma_en              : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ma_variance_cut_resolution         : 4 ;  // 23:20     
	UINT32 ma_variance_h_gain                 : 4 ;  // 27:24     
	UINT32 ma_variance_v_gain                 : 4 ;  // 31:28     
} M14B0_TNR_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc00221acL tnr_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 normalize_enable                   : 1 ;  //     0     
	UINT32 normalize_increase_only            : 1 ;  //     1     
	UINT32 ma_iir_enable                      : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 ma_motion_gain                     : 4 ;  //  7: 4     
	UINT32 ma_offset                          : 8 ;  // 15: 8     
	UINT32 normalize_base                     : 8 ;  // 23:16     
	UINT32 normalize_center                   : 8 ;  // 31:24     
} M14B0_TNR_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc00221b0L tnr_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 errma_iir_max_gain                 : 8 ;  //  7: 0     
	UINT32 errma_iir_diff_min                 : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 errma_iir_diff_max                 : 8 ;  // 23:16     
	UINT32 errma_iir_decrease_ratio           : 8 ;  // 31:24     
} M14B0_TNR_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc00221b4L tnr_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmotion_adap_en                : 1 ;  //     0     
	UINT32 reg_lut_sel                        : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_strong_255_fix                 : 1 ;  //     8     
	UINT32 reg_weak_000_fix                   : 1 ;  //     9     
} M14B0_TNR_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc00221b8L tnr_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 secam_c_mot_secam_th               : 8 ;  //  7: 0     
	UINT32 secam_y_mot_secam_th               : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 secam_c_v_secam_th                 : 8 ;  // 23:16     
	UINT32                                    : 6 ;  // 29:24     reserved
	UINT32 secam_c_v_calc_mode                : 2 ;  // 31:30     
} M14B0_TNR_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc00221bcL tnr_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 motion_chroma_en                   : 1 ;  //     0     
	UINT32 motion_iir_en                      : 1 ;  //     1     
	UINT32 motion_sc_reset_en                 : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 motion_cut_resolution              : 4 ;  //  7: 4     
	UINT32 motion_cut_noise_es                : 4 ;  // 11: 8     
	UINT32 motion_cut_secam                   : 4 ;  // 15:12     
	UINT32 motion_iir_alpha                   : 8 ;  // 23:16     
	UINT32 motion_iir_diff_max                : 8 ;  // 31:24     
} M14B0_TNR_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc00221c0L tnr_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sad_min                            : 12;  // 11: 0     
	UINT32 sad_mv0                            : 12;  // 23:12     
	UINT32 mv_diff                            : 8 ;  // 31:24     
} M14B0_TNR_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc00221c4L tnr_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mv_h                               : 8 ;  //  7: 0     
	UINT32 mv_v                               : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 cnt_var                            : 16;  // 31:16     
} M14B0_TNR_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc00221c8L tnr_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 ma_var                             : 8 ;  // 23:16     
	UINT32 ma_motion                          : 8 ;  // 31:24     
} M14B0_TNR_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc00221ccL tnr_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abs_nr_cc                          : 8 ;  //  7: 0     
	UINT32 abs_nr_p2                          : 8 ;  // 15: 8     
	UINT32 cnt_me_conf                        : 16;  // 31:16     
} M14B0_TNR_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc00221d0L tnr_main_lut_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_addr                : 4 ;  //  3: 0     
	UINT32                                    : 8 ;  // 11: 4     reserved
	UINT32 main_lut_ai_enable                 : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 main_lut_load                      : 1 ;  //    15     
} M14B0_TNR_MAIN_LUT_00_T;

/*-----------------------------------------------------------------------------
		0xc00221d4L tnr_main_lut_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data0               : 32;  // 31: 0     
} M14B0_TNR_MAIN_LUT_01_T;

/*-----------------------------------------------------------------------------
		0xc00221d8L tnr_main_lut_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_lut_indir_data1               : 32;  // 31: 0     
} M14B0_TNR_MAIN_LUT_02_T;

/*-----------------------------------------------------------------------------
		0xc00221dcL tnr_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_measure                         : 8 ;  //  7: 0     
	UINT32 apl_y                              : 8 ;  // 15: 8     
	UINT32 nlvl                               : 6 ;  // 21:16     
} M14B0_TNR_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc00221e0L tnr_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 skingain_motioncontrol_y0          : 8 ;  //  7: 0     
	UINT32 skingain_motioncontrol_x0          : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 skingain_motioncontrol_y1          : 8 ;  // 23:16     
	UINT32 skingain_motioncontrol_x1          : 7 ;  // 30:24     
} M14B0_TNR_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc00221e4L tnr_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_ctrl_x3                  : 8 ;  //  7: 0     
	UINT32 luma_gain_ctrl_x2                  : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 luma_gain_ctrl_x1                  : 8 ;  // 23:16     
	UINT32 luma_gain_ctrl_x0                  : 7 ;  // 30:24     
} M14B0_TNR_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc00221e8L tnr_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 luma_gain_ctrl_y3                  : 8 ;  //  7: 0     
	UINT32 luma_gain_ctrl_y2                  : 8 ;  // 15: 8     
	UINT32 luma_gain_ctrl_y1                  : 8 ;  // 23:16     
	UINT32 luma_gain_ctrl_y0                  : 8 ;  // 31:24     
} M14B0_TNR_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc00221ecL tnr_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 skin_lut_y0                        : 8 ;  //  7: 0     
	UINT32 skin_lut_y1                        : 8 ;  // 15: 8     
	UINT32 skin_lut_y2                        : 8 ;  // 23:16     
	UINT32 skin_lut_y3                        : 8 ;  // 31:24     
} M14B0_TNR_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc00221f0L tnr_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 skin_lut_cb0                       : 8 ;  //  7: 0     
	UINT32 skin_lut_cb1                       : 8 ;  // 15: 8     
	UINT32 skin_lut_cb2                       : 8 ;  // 23:16     
	UINT32 skin_lut_cb3                       : 8 ;  // 31:24     
} M14B0_TNR_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc00221f4L tnr_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 skin_lut_cr0                       : 8 ;  //  7: 0     
	UINT32 skin_lut_cr1                       : 8 ;  // 15: 8     
	UINT32 skin_lut_cr2                       : 8 ;  // 23:16     
	UINT32 skin_lut_cr3                       : 8 ;  // 31:24     
} M14B0_TNR_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc00221f8L tnr_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 past_y2                            : 8 ;  //  7: 0     
	UINT32 past_y3                            : 8 ;  // 15: 8     
} M14B0_TNR_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc00221fcL tnr_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 past_x0                            : 8 ;  //  7: 0     
	UINT32 past_x1                            : 8 ;  // 15: 8     
	UINT32 past_x2                            : 8 ;  // 23:16     
	UINT32 past_y0                            : 4 ;  // 27:24     
	UINT32 past_y1                            : 4 ;  // 31:28     
} M14B0_TNR_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc0022200L ipc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_chroma_mode                     : 2 ;  //  5: 4     
	UINT32 cr_integer_edi                     : 1 ;  //     6     
	UINT32 cr_chroma_edi                      : 1 ;  //     7     
	UINT32 cr_st_2d_v_flt_range2              : 4 ;  // 11: 8     
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 15:12     
	UINT32 cr_ori_3d_blur_y                   : 2 ;  // 17:16     
	UINT32 cr_ori_3d_blur_c                   : 2 ;  // 19:18     
	UINT32 cr_tava_v_edge_adap                : 1 ;  //    20     
	UINT32 cr_tava_debug_mode                 : 1 ;  //    21     
	UINT32 cr_tava_st_adap                    : 1 ;  //    22     
	UINT32 cr_tava_en                         : 1 ;  //    23     
	UINT32 cr_hmc_force_fmd                   : 1 ;  //    24     
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    25     
	UINT32 cr_clc_fmd_off                     : 1 ;  //    26     
	UINT32 cr_chroma_tava_only                : 1 ;  //    27     
	UINT32 cr_film_auto_gain                  : 1 ;  //    28     
	UINT32 cr_film_apply_c                    : 1 ;  //    29     
	UINT32 cr_film_apply_y                    : 1 ;  //    30     
	UINT32 cr_film_mode_enable                : 1 ;  //    31     
} M14B0_M_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0022204L ipc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_en_sel1                     : 2 ;  //  1: 0     
	UINT32 cr_hmc_en_sel2                     : 2 ;  //  3: 2     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 cr_fmd_vfilter_en                  : 1 ;  //     8     
	UINT32                                    : 6 ;  // 14: 9     reserved
	UINT32 cr_fend_auto                       : 1 ;  //    15     
	UINT32 cr_st_flt_enable                   : 1 ;  //    16     
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    17     
	UINT32 cr_hmc_flt_enable                  : 1 ;  //    18     
	UINT32 cr_game_mode                       : 1 ;  //    19     
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 23:20     
	UINT32                                    : 1 ;  //    24     reserved
	UINT32 cr_black_mask                      : 1 ;  //    25     
	UINT32 cr_ipc_debug_show                  : 5 ;  // 30:26     
	UINT32 cr_5f_mode                         : 1 ;  //    31     
} M14B0_M_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0022208L ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002220cL ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0022210L ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0     
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8     
	UINT32 tearing_gain                       : 8 ;  // 23:16     
	UINT32 motion_gain                        : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0022214L ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha              : 8 ;  //  7: 0     
	UINT32 cr_t_grad_weight                   : 8 ;  // 15: 8     
	UINT32 cr_v_grad_weight                   : 8 ;  // 23:16     
	UINT32 cr_h_grad_weight                   : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0022218L ipc_ctrl_06 ''
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
} M14B0_M_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002221cL ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1     
	UINT32 cr_45_en                           : 1 ;  //     2     
	UINT32 cr_15_en                           : 1 ;  //     3     
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 tnr_motion_gain                    : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0022220L ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30     
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31     
} M14B0_M_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0022224L ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16     
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0022228L ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16     
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc002222cL ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0     
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8     
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16     
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0022230L ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0022234L ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    16     
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 23:19     
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 29:24     
} M14B0_M_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0022238L ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reserved                           : 1 ;  //     1     
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc002223cL ipc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0     
	UINT32 reg_3_2_detect_only                : 1 ;  //     1     
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2     
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3     
	UINT32 reg_hd_mode                        : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8     
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16     
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0022240L ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31     
} M14B0_M_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0022244L ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0     
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2     
	UINT32 cr_field_inverse                   : 1 ;  //     4     
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0022248L ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0     
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8     
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16     
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc002224cL ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0     
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8     
	UINT32 reg_2d_offset                      : 8 ;  // 23:16     
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0022250L ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count                       : 16;  // 15: 0     
	UINT32 r_nc_v_count                       : 16;  // 31:16     
} M14B0_M_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc0022254L ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same                       : 16;  // 15: 0     
	UINT32 r_pn_v_count                       : 16;  // 31:16     
} M14B0_M_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc0022258L ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0     
	UINT32 r_fmd_32_min_th_flag_u             : 1 ;  //     6     
	UINT32 r_fmd_32_max_th_flag_u             : 1 ;  //     7     
	UINT32 r_fmd_still_limit_flag_u           : 1 ;  //     8     
	UINT32 r_fmd_still_pn_th2_flag_u          : 1 ;  //     9     
	UINT32 r_fmd_still_pn_th_flag_u           : 1 ;  //    10     
	UINT32 r_fmd_still_prev_ratio_flag_u      : 1 ;  //    11     
	UINT32 r_fmd_bigger_flag_u                : 1 ;  //    12     
	UINT32 r_fmd_smaller_flag_u               : 1 ;  //    13     
	UINT32 r_fmd_3_2_fmd_comb_mode_u          : 2 ;  // 15:14     
	UINT32 r_pn_diff_count                    : 16;  // 31:16     
} M14B0_M_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc002225cL ipc_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 same_ng_flag_d                     : 1 ;  //     0     
	UINT32 too_small_flag_d                   : 1 ;  //     1     
	UINT32 r_fmd_still_flag_d                 : 1 ;  //     2     
	UINT32 r_fmd_possibility_flag_d           : 1 ;  //     3     
	UINT32 base_pn_ratio_flag_d               : 1 ;  //     4     
	UINT32 r_32_detected_flag_d               : 1 ;  //     5     
	UINT32 r_fmd_comb_mode_d                  : 2 ;  //  7: 6     
	UINT32 r_fmd_lock_count_d                 : 8 ;  // 15: 8     
	UINT32 same_ng_flag_u                     : 1 ;  //    16     
	UINT32 too_small_flag_u                   : 1 ;  //    17     
	UINT32 r_fmd_still_flag_u                 : 1 ;  //    18     
	UINT32 r_fmd_possibility_flag_u           : 1 ;  //    19     
	UINT32 base_pn_ratio_flag_u               : 1 ;  //    20     
	UINT32 r_32_uetected_flag_u               : 1 ;  //    21     
	UINT32 r_fmd_comb_mode_u                  : 2 ;  // 23:22     
	UINT32 r_fmd_lock_count_u                 : 8 ;  // 31:24     
} M14B0_M_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc0022260L clc_ctrl_00 ''
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
	UINT32 cl_motion_a_mode                   : 1 ;  //    30     
} M14B0_CLC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0022264L clc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_motion_a_th                     : 8 ;  //  7: 0     
	UINT32 cl_motion_b_th                     : 8 ;  // 15: 8     
	UINT32 cl_y_protect_th0                   : 8 ;  // 23:16     
	UINT32 cl_y_protect_th1                   : 8 ;  // 31:24     
} M14B0_CLC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0022268L clc_ctrl_02 ''
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
	UINT32 cl_motionp2n2_th_pal_high          : 2 ;  // 31:30     
} M14B0_CLC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002226cL clc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_sat_min                         : 8 ;  //  7: 0     
	UINT32                                    : 2 ;  //  9: 8     reserved
	UINT32 cl_var_measure_th1                 : 10;  // 19:10     
	UINT32 cl_var_measure_th2                 : 10;  // 29:20     
	UINT32 cl_pt_enable                       : 2 ;  // 31:30     
} M14B0_CLC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0022270L clc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_tearing_pt_th1                  : 12;  // 11: 0     
	UINT32 cl_motionp2n2_th_pal_low           : 8 ;  // 19:12     
	UINT32 cl_frame_min                       : 12;  // 31:20     
} M14B0_CLC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0022274L clc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cl_data_line_switching_sel         : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 clc_bin_flt_th                     : 3 ;  // 10: 8     
	UINT32                                    : 4 ;  // 14:11     reserved
	UINT32 clc_bin_flt_en                     : 1 ;  //    15     
	UINT32 clc_dbg_show_mask                  : 13;  // 28:16     
} M14B0_CLC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0022278L clc_ctrl_06 ''
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
} M14B0_CLC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002227cL clc_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 cl_detection_count                 : 16;  // 31:16     
} M14B0_CLC_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0022280L ipc_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 tava_v_diff_x1                     : 8 ;  // 31:24     
	UINT32 tava_v_diff_y1                     : 8 ;  // 23:16     
	UINT32 tava_v_diff_x0                     : 8 ;  // 15: 8     
	UINT32 tava_v_diff_y0                     : 8 ;  //  7: 0     
} M14B0_M_IPC_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc0022284L ipc_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_div_position                   : 8 ;  // 31:24     
	UINT32 reg_manual_mode1                   : 2 ;  // 23:22     
	UINT32 reg_manual_mode2                   : 2 ;  // 21:20     
	UINT32 reg_manual_en                      : 1 ;  //    16     
	UINT32 cr_sub_main_sel                    : 1 ;  //     4     
	UINT32 cr_bound_exp_en                    : 1 ;  //     1     
	UINT32 reg_div_en                         : 1 ;  //     0     
} M14B0_M_IPC_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc0022288L ipc_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc002228cL ipc_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reserved                           : 1 ;  //     1     
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc0022290L ipc_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0     
	UINT32 reg_3_2_detect_only                : 1 ;  //     1     
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2     
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3     
	UINT32 reg_hd_mode                        : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8     
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16     
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc0022294L ipc_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31     
} M14B0_M_IPC_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc0022298L ipc_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} M14B0_M_IPC_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc002229cL ipc_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fend_position                  : 11;  // 10: 0     
} M14B0_M_IPC_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc00222a0L ipc_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_yy_diff_gain                   : 4 ;  //  3: 0     
	UINT32 reg_hv_add_gain                    : 4 ;  //  7: 4     
	UINT32 reg_v_motion_gain                  : 4 ;  // 11: 8     
	UINT32 reg_h_motion_gain                  : 4 ;  // 15:12     
	UINT32 reg_cc_diff_gain                   : 4 ;  // 19:16     
} M14B0_M_IPC_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc00222f0L ipc_ctrl_20 ''
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
} M14B0_M_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc00222f4L ipc_ctrl_21 ''
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
} M14B0_M_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc00222f8L ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0     
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8     
} M14B0_M_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc00222fcL ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0     
	UINT32 cr_lock_th                         : 7 ;  //  7: 1     
	UINT32 cr_g_motion_th                     : 8 ;  // 15: 8     
	UINT32 cr_hmc_expend                      : 2 ;  // 17:16     
	UINT32 cr_hmc_half_en                     : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 cr_robustness                      : 4 ;  // 23:20     
	UINT32 cr_center_weight                   : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc0022300L ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8     
	UINT32 cr_gds_resolution                  : 3 ;  // 18:16     
} M14B0_M_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc0022304L ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8     
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16     
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc0022308L ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc002230cL ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0     
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8     
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16     
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc0022310L ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hmc_bonus_es_en                : 1 ;  //     0     
	UINT32 reg_hmc_bonus_et_en                : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_hmc_bonus_gain                 : 8 ;  // 15: 8     
	UINT32 reg_hmc_bonus_th                   : 8 ;  // 23:16     
} M14B0_M_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc0022314L ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16     
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24     
} M14B0_M_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc0022318L ipc_ctrl_30 ''
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
} M14B0_M_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc002231cL ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} M14B0_M_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc0022320L ipc_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 tava_errt_x3                       : 8 ;  // 31:24     
	UINT32 tava_errt_y3                       : 8 ;  // 23:16     
	UINT32 tava_errt_x2                       : 8 ;  // 15: 8     
	UINT32 tava_errt_y2                       : 8 ;  //  7: 0     
} M14B0_M_IPC_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc0022324L ipc_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 tava_errt_x1                       : 8 ;  // 31:24     
	UINT32 tava_errt_y1                       : 8 ;  // 23:16     
	UINT32 tava_errt_x0                       : 8 ;  // 15: 8     
	UINT32 tava_errt_y0                       : 8 ;  //  7: 0     
} M14B0_M_IPC_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc0022328L ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count_d                     : 16;  // 15: 0     
	UINT32 r_nc_v_count_d                     : 16;  // 31:16     
} M14B0_M_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc002232cL ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same_d                     : 16;  // 15: 0     
	UINT32 r_pn_v_count_d                     : 16;  // 31:16     
} M14B0_M_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc0022330L ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0     
	UINT32 r_fmd_32_min_th_flag_d             : 1 ;  //     6     
	UINT32 r_fmd_32_max_th_flag_d             : 1 ;  //     7     
	UINT32 r_fmd_still_limit_flag_d           : 1 ;  //     8     
	UINT32 r_fmd_still_pn_th2_flag_d          : 1 ;  //     9     
	UINT32 r_fmd_still_pn_th_flag_d           : 1 ;  //    10     
	UINT32 r_fmd_still_prev_ratio_flag_d      : 1 ;  //    11     
	UINT32 r_fmd_bigger_flag_d                : 1 ;  //    12     
	UINT32 r_fmd_smaller_flag_d               : 1 ;  //    13     
	UINT32 r_fmd_3_2_fmd_comb_mode_d          : 2 ;  // 15:14     
	UINT32 r_pn_diff_count_d                  : 16;  // 31:16     
} M14B0_M_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc0022334L ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0     
	UINT32 r_inv_original_cnt                 : 16;  // 31:16     
} M14B0_M_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc0022338L ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_M_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc002233cL ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_M_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0xc0022340L tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_show_scale                     : 3 ;  //  2: 0     
	UINT32 reg_bbd_show                       : 1 ;  //     3     
	UINT32 reg_show_bar_1                     : 1 ;  //     4     
	UINT32 reg_show_bar_2                     : 1 ;  //     5     
	UINT32 reg_show_bar_3                     : 1 ;  //     6     
	UINT32 reg_bar1_data_mux                  : 2 ;  //  8: 7     
	UINT32 reg_bar2_data_mux                  : 2 ;  // 10: 9     
	UINT32 reg_bar3_data_mux                  : 2 ;  // 12:11     
	UINT32 reg_hist0_data_mux                 : 4 ;  // 16:13     
	UINT32 reg_hist1_data_mux                 : 4 ;  // 20:17     
	UINT32 reg_hist2_data_mux                 : 4 ;  // 24:21     
	UINT32 reg_hist0_win_sel                  : 2 ;  // 26:25     
	UINT32 reg_hist1_win_sel                  : 2 ;  // 28:27     
	UINT32 reg_hist2_win_sel                  : 2 ;  // 30:29     
	UINT32 hist_rd_en                         : 1 ;  //    31     
} M14B0_M_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0022344L tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15     
	UINT32 wina_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31     
} M14B0_M_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0022348L tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30     
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31     
} M14B0_M_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002234cL tpd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winb_hen                       : 1 ;  //    15     
	UINT32 winb_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winb_ven                       : 1 ;  //    31     
} M14B0_M_TPD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0022350L tpd_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winb_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winb_y1                            : 12;  // 27:16     
} M14B0_M_TPD_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0022354L tpd_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_winc_hen                       : 1 ;  //    15     
	UINT32 winc_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_winc_ven                       : 1 ;  //    31     
} M14B0_M_TPD_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0022358L tpd_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 winc_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 winc_y1                            : 12;  // 27:16     
} M14B0_M_TPD_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002235cL tpd_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_y                        : 10;  //  9: 0     
	UINT32 reg_force_y_max_th                 : 10;  // 19:10     
	UINT32 reg_force_y_min_th                 : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_y_enable                 : 1 ;  //    31     
} M14B0_M_TPD_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0022360L tpd_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cb                       : 10;  //  9: 0     
	UINT32 reg_force_cb_max_th                : 10;  // 19:10     
	UINT32 reg_force_cb_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cb_enable                : 1 ;  //    31     
} M14B0_M_TPD_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0022364L tpd_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_cr                       : 10;  //  9: 0     
	UINT32 reg_force_cr_max_th                : 10;  // 19:10     
	UINT32 reg_force_cr_min_th                : 10;  // 29:20     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 reg_force_cr_enable                : 1 ;  //    31     
} M14B0_M_TPD_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0022368L tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc002236cL clc_ctrl_07 ''
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
} M14B0_CLC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0022370L bbd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0     
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1     
	UINT32 reg_op_mode                        : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8     
	UINT32 reg_diff_th                        : 8 ;  // 23:16     
	UINT32 reg_bbd_mux                        : 3 ;  // 26:24     
	UINT32 reg_apl_mux                        : 3 ;  // 29:27     
	UINT32 reg_apl_win_sel                    : 2 ;  // 31:30     
} M14B0_BBD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0022374L bbd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} M14B0_BBD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0022378L bbd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_BBD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002237cL bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} M14B0_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0022380L bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0022384L apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0     
	UINT32 cb_b_s_apl                         : 10;  // 19:10     
	UINT32 cr_r_s_apl                         : 10;  // 29:20     
} M14B0_M_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0022390L tpd_hist_ai_ctrl ''
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
} M14B0_TPD_HIST_AI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0022394L tpd_hist_ai_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_status                        : 22;  // 21: 0     
} M14B0_TPD_HIST_AI_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0022398L lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16     
	UINT32 y_gain                             : 8 ;  // 31:24     
} M14B0_M_LVCRTL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00223d0L tnr_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 reg_low_2bit_fix                   : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 reg_low_2bit                       : 2 ;  //  7: 6     
	UINT32                                    : 1 ;  //     8     reserved
	UINT32 reg_ne_sel                         : 1 ;  //     9     
	UINT32                                    : 14;  // 23:10     reserved
	UINT32 reg_ne_th                          : 8 ;  // 31:24     
} M14B0_TNR_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc00223d4L tnr_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_blend_en                       : 1 ;  //     0     
	UINT32 reg_ma_sel                         : 1 ;  //     1     
	UINT32 reg_iir_en                         : 1 ;  //     2     
	UINT32 reg_force_en                       : 1 ;  //     3     
	UINT32 reg_force_gain                     : 4 ;  //  7: 4     
	UINT32 reg_iir_max_gain                   : 8 ;  // 15: 8     
	UINT32 reg_iir_diff_min                   : 8 ;  // 23:16     
	UINT32 reg_iir_diff_max                   : 8 ;  // 31:24     
} M14B0_TNR_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc00223d8L tnr_ctrl_42 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 3 ;  //  2: 0     reserved
	UINT32 reg_mc_conf_y1                     : 5 ;  //  7: 3     
	UINT32                                    : 3 ;  // 10: 8     reserved
	UINT32 reg_mc_conf_y0                     : 5 ;  // 15:11     
	UINT32 reg_mc_conf_x1                     : 8 ;  // 23:16     
	UINT32 reg_mc_conf_x0                     : 8 ;  // 31:24     
} M14B0_TNR_CTRL_42_T;

/*-----------------------------------------------------------------------------
		0xc00223dcL tnr_ctrl_43 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 3 ;  //  2: 0     reserved
	UINT32 reg_ma_conf_x0                     : 5 ;  //  7: 3     
	UINT32                                    : 3 ;  // 10: 8     reserved
	UINT32 reg_ma_conf_x1                     : 5 ;  // 15:11     
	UINT32 reg_ma_conf_y0                     : 8 ;  // 23:16     
	UINT32 reg_ma_conf_y1                     : 8 ;  // 31:24     
} M14B0_TNR_CTRL_43_T;

/*-----------------------------------------------------------------------------
		0xc00223e0L tnr_ctrl_44 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_resolution                     : 4 ;  //  3: 0     
} M14B0_TNR_CTRL_44_T;

/*-----------------------------------------------------------------------------
		0xc00223e4L tnr_ctrl_45 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_noise_es_res                   : 4 ;  //  3: 0     
	UINT32 reg_motion_es_res                  : 4 ;  //  7: 4     
	UINT32 reg_nema_res                       : 4 ;  // 11: 8     
} M14B0_TNR_CTRL_45_T;

/*-----------------------------------------------------------------------------
		0xc00223e8L tnr_ctrl_46 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_debug_show                     : 4 ;  //  3: 0     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 reg_snr_en                         : 1 ;  //    16     
	UINT32 reg_var_adaptive_en                : 1 ;  //    17     
	UINT32 reg_blend_a_adaptive_en            : 1 ;  //    18     
	UINT32 reg_errma_adaptive_mode            : 1 ;  //    19     
} M14B0_TNR_CTRL_46_T;

/*-----------------------------------------------------------------------------
		0xc00223ecL tnr_ctrl_47 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 snr_var_y0                         : 8 ;  //  7: 0     
	UINT32 snr_var_y1                         : 8 ;  // 15: 8     
	UINT32 snr_var_x0                         : 8 ;  // 23:16     
	UINT32 snr_var_x1                         : 8 ;  // 31:24     
} M14B0_TNR_CTRL_47_T;

/*-----------------------------------------------------------------------------
		0xc00223f0L tnr_ctrl_48 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_snr_master_gain                : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_snr_c_blur_gain                : 8 ;  // 23:16     
	UINT32 reg_snr_y_blur_gain                : 8 ;  // 31:24     
} M14B0_TNR_CTRL_48_T;

/*-----------------------------------------------------------------------------
		0xc00223f4L tnr_ctrl_49 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0     
} M14B0_TNR_CTRL_49_T;

/*-----------------------------------------------------------------------------
		0xc00223f8L tnr_ctrl_50 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 snr_blendgain_y0                   : 8 ;  //  7: 0     
	UINT32 snr_blendgain_y1                   : 8 ;  // 15: 8     
	UINT32 snr_blendgain_x0                   : 8 ;  // 23:16     
	UINT32 snr_blendgain_x1                   : 8 ;  // 31:24     
} M14B0_TNR_CTRL_50_T;

/*-----------------------------------------------------------------------------
		0xc00223fcL tnr_ctrl_51 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nlvl_motion_scale                  : 2 ;  //  1: 0     
	UINT32 nlvl_3pct_sel                      : 2 ;  //  3: 2     
	UINT32 nlvl_0peak_x_th                    : 4 ;  //  7: 4     
	UINT32 nlvl_0peak_y_th                    : 8 ;  // 15: 8     
	UINT32 nlvl_y_th0                         : 8 ;  // 23:16     
	UINT32 nlvl_y_th1                         : 8 ;  // 31:24     
} M14B0_TNR_CTRL_51_T;

/*-----------------------------------------------------------------------------
		0xc0023a00L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} M14B0_PE0_DNR_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc0023a04L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16     
} M14B0_PE0_DNR_INTR_T;

/*-----------------------------------------------------------------------------
		0xc0023a08L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 dnr_frame_id                       : 8 ;  // 23:16     
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24     
} M14B0_PE0_DNR_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc0023a0cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16     
} M14B0_PE0_DNR_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc0023a10L pe0_opmode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 12;  // 11: 0     reserved
	UINT32 dnr_out_420_vsample_mode           : 1 ;  //    12     
	UINT32 dnr_out_cs_type                    : 3 ;  // 15:13     
	UINT32 detour_enable                      : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 dnr_out_422_hsample_mode           : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 dnr_in_lsb_mode                    : 2 ;  // 25:24     
	UINT32 dnr_out_lsb_mode                   : 2 ;  // 27:26     
} M14B0_PE0_DNR_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc0023a14L pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0     
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 dnr_in_cs_type                     : 3 ;  // 15:13     
	UINT32 vsize                              : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31     
} M14B0_PE0_DNR_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0023a18L pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4     
} M14B0_PE0_DNR_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc0023a20L win_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
} M14B0_WIN_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0023a24L win_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_WIN_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0023a28L win_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y0                                 : 12;  // 27:16     
	UINT32 ac_bnr_feature_cal_mode            : 2 ;  // 29:28     
} M14B0_WIN_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc0023a2cL win_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_WIN_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0023a30L cup_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bypass                             : 1 ;  //     0     
	UINT32 v_upsample_en                      : 1 ;  //     1     
	UINT32 h_upsample_en                      : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 v_cubic_en                         : 1 ;  //     4     
	UINT32 h_cubic_en                         : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 sd_mode_en                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 odd_val                            : 1 ;  //    12     
} M14B0_CUP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0023a34L cup_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cubic_coef0                        : 8 ;  //  7: 0     
	UINT32 cubic_coef1                        : 8 ;  // 15: 8     
} M14B0_CUP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0023a38L cup_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0     
} M14B0_CUP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0023a40L lbaf_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 lm_mode                            : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 lm_422_en                          : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 manual_en                          : 1 ;  //    15     
} M14B0_LBAF_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0023a50L _3dfd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 aw_offset_x                        : 8 ;  //  7: 0     
	UINT32 aw_offset_y                        : 8 ;  // 15: 8     
	UINT32 cb_th1                             : 8 ;  // 23:16     
	UINT32 cb_th2                             : 8 ;  // 31:24     
} M14B0_3DFD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0023a54L _3dfd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cc_th1                             : 8 ;  //  7: 0     
	UINT32 cc_th2                             : 8 ;  // 15: 8     
	UINT32 ll_th1                             : 8 ;  // 23:16     
	UINT32 ll_th2                             : 8 ;  // 31:24     
} M14B0_3DFD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0023a58L _3dfd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pt_th                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 sstb_th                            : 8 ;  // 23:16     
	UINT32 debug_mode                         : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 div_mode                           : 1 ;  //    28     
} M14B0_3DFD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0023a5cL _3dfd_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 small_delta_th                     : 19;  // 18: 0     
	UINT32                                    : 5 ;  // 23:19     reserved
	UINT32 small_delta_en                     : 1 ;  //    24     
} M14B0_3DFD_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0023a60L _3dfd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_count                           : 16;  // 15: 0     
	UINT32 cc_count                           : 16;  // 31:16     
} M14B0_3DFD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0023a64L _3dfd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ll_count                           : 16;  // 15: 0     
	UINT32 trid_format                        : 4 ;  // 19:16     
} M14B0_3DFD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0023a68L _3dfd_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_h                         : 23;  // 22: 0     
} M14B0_3DFD_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc0023a6cL _3dfd_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_ss_v                         : 23;  // 22: 0     
} M14B0_3DFD_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc0023a70L _3dfd_stat_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_v                         : 23;  // 22: 0     
} M14B0_3DFD_STAT_04_T;

/*-----------------------------------------------------------------------------
		0xc0023a74L _3dfd_stat_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 delta_tb_h                         : 23;  // 22: 0     
} M14B0_3DFD_STAT_05_T;

/*-----------------------------------------------------------------------------
		0xc0023a78L _3dfd_stat_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_ratio                           : 23;  // 22: 0     
} M14B0_3DFD_STAT_06_T;

/*-----------------------------------------------------------------------------
		0xc0023a7cL _3dfd_stat_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_ratio                           : 23;  // 22: 0     
} M14B0_3DFD_STAT_07_T;

/*-----------------------------------------------------------------------------
		0xc0023a80L vfilter_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_enable                  : 1 ;  //     0     
	UINT32 cr_vfilter_local_en                : 1 ;  //     1     
	UINT32 cr_vfilter_strength                : 1 ;  //     2     
	UINT32 cr_vfilter_max_clipping            : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 cr_vfilter_ath                     : 8 ;  // 15: 8     
	UINT32 cr_vfilter_in_field_var            : 8 ;  // 23:16     
	UINT32 cr_vfilter_use_mc_mask             : 1 ;  //    24     
	UINT32 cr_vfilter_expand                  : 1 ;  //    25     
	UINT32 cr_vfilter_center                  : 1 ;  //    26     
	UINT32 cr_vfilter_u14                     : 1 ;  //    27     
	UINT32 cr_dbg_show_level                  : 2 ;  // 29:28     
	UINT32                                    : 1 ;  //    30     reserved
	UINT32 cr_dbg_show_en                     : 1 ;  //    31     
} M14B0_VFILTER_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0023a84L vfilter_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_vfilter_global_en               : 1 ;  //     0     
	UINT32 cr_vfilter_force_en                : 1 ;  //     1     
	UINT32 cr_global_frame_num_th             : 4 ;  //  5: 2     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cr_vfilter_global_th               : 12;  // 19: 8     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 cr_vfilter_max_th                  : 8 ;  // 31:24     
} M14B0_VFILTER_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0023a88L vfilter_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_protect_enable              : 1 ;  //     0     
	UINT32 cr_hmc_only_hmc_region             : 1 ;  //     1     
	UINT32 reg_v_filter_master_gain           : 6 ;  //  7: 2     
	UINT32 cr_hmc_protect_motion_th           : 8 ;  // 15: 8     
	UINT32 cr_hmc_protect_th_max              : 8 ;  // 23:16     
	UINT32 cr_hmc_protect_th_min              : 8 ;  // 31:24     
} M14B0_VFILTER_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc0023a8cL cti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32 cti_clipping_mode                  : 1 ;  //     1     
	UINT32 cti_pre_flt_mode                   : 2 ;  //  3: 2     
	UINT32 cti_tap                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
	UINT32 cti_debug_mode                     : 3 ;  // 18:16     
} M14B0_CTI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0023a90L cti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_coring_th0                     : 8 ;  //  7: 0     
	UINT32 cti_coring_th1                     : 8 ;  // 15: 8     
	UINT32 cti_coring_smooth                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cti_tap2                       : 2 ;  // 21:20     
} M14B0_CTI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0023a94L dnr_max_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dnr_max_enable                 : 1 ;  //     0     
	UINT32 reg_ifc_max_enable                 : 1 ;  //     1     
	UINT32 reg_dnr_ifc_sel                    : 1 ;  //     2     
	UINT32                                    : 5 ;  //  7: 3     reserved
	UINT32 debug_enable                       : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 win_control_enable                 : 1 ;  //    16     
	UINT32 border_enable                      : 1 ;  //    17     
	UINT32 reg_win_inout                      : 1 ;  //    18     
	UINT32 mnr_motion_ctrl                    : 1 ;  //    19     
	UINT32 bnr_motion_ctrl                    : 1 ;  //    20     
	UINT32 reg_motion_mux_set                 : 1 ;  //    21     
	UINT32 reg_motion_bit_ctrl                : 3 ;  // 24:22     
	UINT32 reg_ifc_delta_th                   : 7 ;  // 31:25     
} M14B0_DNR_MAX_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023a98L dnr_dbar_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_en                         : 1 ;  //     0     
	UINT32 reg_show_f3d                       : 1 ;  //     1     
	UINT32 reg_show_bnr                       : 1 ;  //     2     
	UINT32 reg_f3d_mode                       : 1 ;  //     3     
	UINT32 reg_acness_or_acness_gain          : 1 ;  //     4     
} M14B0_DNR_DBAR_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023a9cL reg_420c_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tearing_count_th               : 8 ;  //  7: 0     
	UINT32 reg_frame_count_th                 : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_detect_level_th                : 10;  // 25:16     
} M14B0_REG_420C_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023aa0L mnr_ctrl_0 ''
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
} M14B0_MNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0023aa4L mnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_s1_mmd_min                 : 8 ;  //  7: 0     
	UINT32 reg_mnr_s2_ratio_min               : 8 ;  // 15: 8     
	UINT32 reg_mnr_s2_ratio_max               : 8 ;  // 23:16     
	UINT32 reg_mnr_s2_mmd_min                 : 8 ;  // 31:24     
} M14B0_MNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0023aa8L mnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mnr_motion_y_max               : 8 ;  //  7: 0     
	UINT32 reg_mnr_motion_y_min               : 8 ;  // 15: 8     
	UINT32 reg_mnr_v_gain                     : 8 ;  // 23:16     
	UINT32 reg_mnr_h_gain                     : 8 ;  // 31:24     
} M14B0_MNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc0023aacL mnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_filter_x0                      : 8 ;  //  7: 0     
	UINT32 reg_filter_x1                      : 8 ;  // 15: 8     
	UINT32 reg_filter_y0                      : 8 ;  // 23:16     
	UINT32 reg_filter_y1                      : 8 ;  // 31:24     
} M14B0_MNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0023ab0L mnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_en                  : 1 ;  //     0     
	UINT32 reg_motion_mnr_filter              : 1 ;  //     1     
	UINT32 reg_motion_mnr_filter_c            : 1 ;  //     2     
	UINT32 reg_mnr_filter                     : 1 ;  //     3     
	UINT32 reg_mnr_filter_c                   : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 reg_mnr_motion_min                 : 8 ;  // 23:16     
	UINT32 reg_mnr_motion_max                 : 8 ;  // 31:24     
} M14B0_MNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc0023ab4L mnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_mnr_x0                  : 8 ;  //  7: 0     
	UINT32 reg_motion_mnr_x1                  : 8 ;  // 15: 8     
	UINT32 reg_motion_mnr_y0                  : 8 ;  // 23:16     
	UINT32 reg_motion_mnr_y1                  : 8 ;  // 31:24     
} M14B0_MNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc0023ab8L detail_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_detail_max              : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_detail_min              : 8 ;  // 15: 8     
	UINT32 reg_bnr_diff_l                     : 8 ;  // 23:16     
	UINT32 reg_bnr_diff_p                     : 8 ;  // 31:24     
} M14B0_DETAIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0023abcL dc_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_enable                  : 1 ;  //     0     
	UINT32 reg_dc_blur_debug_mode             : 1 ;  //     1     
	UINT32 reg_dc_blur_sel                    : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_dc_output_debug_mode           : 4 ;  //  7: 4     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_dc_motion_max                  : 8 ;  // 23:16     
	UINT32 reg_dc_motion_min                  : 8 ;  // 31:24     
} M14B0_DC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0023ac0L dc_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_detail_max                  : 8 ;  //  7: 0     
	UINT32 reg_dc_detail_min                  : 8 ;  // 15: 8     
	UINT32 reg_var_chroma_en                  : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_var_v_gain                     : 4 ;  // 23:20     
	UINT32 reg_var_h_gain                     : 4 ;  // 27:24     
	UINT32 reg_var_cut_resolution             : 4 ;  // 31:28     
} M14B0_DC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0023ac4L dc_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_var_en                      : 1 ;  //     0     
	UINT32 reg_dc_motion_en                   : 1 ;  //     1     
	UINT32 reg_dc_protection_en               : 1 ;  //     2     
	UINT32 reg_dc_detail_en                   : 1 ;  //     3     
	UINT32 reg_dc_global_motion_th            : 4 ;  //  7: 4     
	UINT32                                    : 2 ;  //  9: 8     reserved
	UINT32 reg_dc_bnr_mastergain              : 6 ;  // 15:10     
	UINT32 reg_dc_bnr_chromagain              : 8 ;  // 23:16     
	UINT32 reg_dc_protection_th               : 8 ;  // 31:24     
} M14B0_DC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc0023ac8L dc_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_bnr_var_th3                 : 8 ;  //  7: 0     
	UINT32 reg_dc_bnr_var_th2                 : 8 ;  // 15: 8     
	UINT32 reg_dc_bnr_var_th1                 : 8 ;  // 23:16     
	UINT32 reg_dc_bnr_var_th0                 : 8 ;  // 31:24     
} M14B0_DC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0023accL dc_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_motion_y_min                : 8 ;  //  7: 0     
	UINT32 reg_dc_motion_y_max                : 8 ;  // 15: 8     
	UINT32 reg_dc_detail_y_min                : 8 ;  // 23:16     
	UINT32 reg_dc_detail_y_max                : 8 ;  // 31:24     
} M14B0_DC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc0023ad0L dc_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dc_var_y_th0                   : 8 ;  //  7: 0     
	UINT32 reg_dc_var_y_th1                   : 8 ;  // 15: 8     
	UINT32 reg_dc_var_y_th2                   : 8 ;  // 23:16     
	UINT32 reg_dc_var_y_th3                   : 8 ;  // 31:24     
} M14B0_DC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc0023ad4L ac_bnr_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_h_en                    : 1 ;  //     0     
	UINT32 reg_bnr_ac_v_en                    : 1 ;  //     1     
	UINT32 reg_bnr_ac_h_chroma_en             : 1 ;  //     2     
	UINT32 reg_bnr_ac_v_chroma_en             : 1 ;  //     3     
	UINT32 reg_bnr_ac_acness_resol_h          : 2 ;  //  5: 4     
	UINT32 reg_bnr_ac_sdhd_sel                : 1 ;  //     6     
	UINT32 reg_ac_bnr_gain_sel                : 1 ;  //     7     
	UINT32 reg_bnr_ac_diff_min_v_th           : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_diff_min_h_th           : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_iir_gain                : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0023ad8L ac_bnr_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_global_motion_th        : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_acness_max              : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_acness_min              : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_bin_th                  : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0023adcL ac_bnr_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_motion_max_offset_h2    : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_motion_min_offset_h2    : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_motion_max_offset_h1    : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_motion_min_offset_h1    : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc0023ae0L bnr_stat_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c420_tearing_cnt                   : 16;  // 15: 0     
	UINT32 global_motion_iir                  : 8 ;  // 23:16     
	UINT32 global_dc_gain                     : 8 ;  // 31:24     
} M14B0_BNR_STAT_0_T;

/*-----------------------------------------------------------------------------
		0xc0023ae4L ac_bnr_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_motion_acness_th1       : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_motion_acness_th0       : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_motion_max_offset_h3    : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_motion_min_offset_h3    : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0023ae8L ac_bnr_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_v_init_offset           : 3 ;  //  2: 0     
	UINT32 reg_bnr_ac_h_init_offset           : 3 ;  //  5: 3     
	UINT32 reg_bnr_ac_acness_resol_v          : 2 ;  //  7: 6     
	UINT32 reg_bnr_ac_debug_mode              : 4 ;  // 11: 8     
	UINT32 reg_bnr_ac_hsy_mode                : 4 ;  // 15:12     
	UINT32 reg_bnr_ac_motion_max              : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_motion_min              : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc0023aecL bnr_stat_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 acness_or_gain_v_l                 : 8 ;  //  7: 0     
	UINT32 acness_or_gain_h_l                 : 8 ;  // 15: 8     
	UINT32 acness_or_gain_v_g                 : 8 ;  // 23:16     
	UINT32 acness_or_gain_h_g                 : 8 ;  // 31:24     
} M14B0_BNR_STAT_1_T;

/*-----------------------------------------------------------------------------
		0xc0023af0L ac_bnr_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_detail_th4              : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_detail_th3              : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_detail_th2              : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_detail_th1              : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc0023af4L ac_bnr_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_pos_gain_h2             : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_pos_gain_h1             : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_pos_gain_h0             : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_detail_gain_th          : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc0023af8L ac_bnr_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_pos_gain_l2             : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_pos_gain_l1             : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_pos_gain_l0             : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_pos_gain_h3             : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc0023afcL bnr_stat_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_init                             : 8 ;  //  7: 0     
	UINT32 v_init                             : 8 ;  // 15: 8     
	UINT32                                    : 4 ;  // 19:16     reserved
	UINT32 reg_vfilter_tearing_cnt            : 12;  // 31:20     
} M14B0_BNR_STAT_2_T;

/*-----------------------------------------------------------------------------
		0xc0023b00L ac_bnr_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_master_c_gain               : 8 ;  //  7: 0     
	UINT32 reg_ac_bnr_enable                  : 1 ;  //     8     
	UINT32 reg_ac_bnr_motion_en               : 1 ;  //     9     
	UINT32 reg_ac_bnr_acness_en               : 1 ;  //    10     
	UINT32 reg_ac_bnr_position_en             : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_bnr_ac_manual_init_v           : 3 ;  // 18:16     
	UINT32 reg_ac_manual_init_en_v            : 1 ;  //    19     
	UINT32 reg_bnr_ac_manual_init_h           : 3 ;  // 22:20     
	UINT32 reg_ac_manual_init_en_h            : 1 ;  //    23     
	UINT32 reg_bnr_ac_pos_gain_l3             : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc0023b04L ac_bnr_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 7 ;  //  6: 0     reserved
	UINT32 reg_acness_or_gain_sel             : 1 ;  //     7     
	UINT32 reg_final_ac_bnr_gain_debug_h_en   : 1 ;  //     8     
	UINT32 reg_final_ac_bnr_gain_debug_v_en   : 1 ;  //     9     
	UINT32 reg_ac_master_gain                 : 6 ;  // 15:10     
} M14B0_AC_BNR_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc0023b08L ac_bnr_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_detail_gain_y_min           : 8 ;  //  7: 0     
	UINT32 reg_ac_detail_gain_y_max           : 8 ;  // 15: 8     
	UINT32 reg_bnr_ac_acness_y_min            : 8 ;  // 23:16     
	UINT32 reg_bnr_ac_acness_y_max            : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc0023b0cL ac_bnr_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ac_detail_alpha2_y_min         : 8 ;  //  7: 0     
	UINT32 reg_ac_detail_alpha2_y_max         : 8 ;  // 15: 8     
	UINT32 reg_ac_detail_alpha4_y_min         : 8 ;  // 23:16     
	UINT32 reg_ac_detail_alpha4_y_max         : 8 ;  // 31:24     
} M14B0_AC_BNR_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0023b10L ac_bnr_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bnr_ac_motion_y_min            : 8 ;  //  7: 0     
	UINT32 reg_bnr_ac_motion_y_max            : 8 ;  // 15: 8     
} M14B0_AC_BNR_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0023b14L ifc_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_y_drvt_hcoef_sel               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_y_ddrvt_hcoef_sel              : 2 ;  //  5: 4     
	UINT32 reg_y_edge_gain_res                : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_y_drvt2_hcoef_sel              : 3 ;  // 10: 8     
	UINT32 reg_medge_meth                     : 1 ;  //    11     
	UINT32 reg_medge_th                       : 8 ;  // 19:12     
	UINT32 reg_medge_coring_th                : 8 ;  // 27:20     
	UINT32 reg_medge_ch_tap                   : 2 ;  // 29:28     
	UINT32 reg_edge_meth                      : 2 ;  // 31:30     
} M14B0_IFC_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0023b18L ifc_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_edge_ddrvt_gain_x0             : 8 ;  // 31:24     
	UINT32 reg_edge_ddrvt_gain_y0             : 8 ;  // 23:16     
	UINT32 reg_edge_ddrvt_gain_x1             : 8 ;  // 15: 8     
	UINT32 reg_edge_ddrvt_gain_y1             : 8 ;  //  7: 0     
} M14B0_IFC_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0023b1cL ifc_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_c_drvt_hcoef_sel               : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_c_ddrvt_hcoef_sel              : 2 ;  //  5: 4     
	UINT32 reg_c_edge_gain_res                : 1 ;  //     6     
} M14B0_IFC_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc0023b20L ifc_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_c_edge_gain_x0                 : 8 ;  // 31:24     
	UINT32 reg_c_edge_gain_y0                 : 8 ;  // 23:16     
	UINT32 reg_c_edge_gain_x1                 : 8 ;  // 15: 8     
	UINT32 reg_c_edge_gain_y1                 : 8 ;  //  7: 0     
} M14B0_IFC_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0023b24L ifc_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_crs_para_tap                   : 2 ;  //  1: 0     
	UINT32 reg_cdst_th                        : 10;  // 11: 2     
	UINT32 reg_crs_op_tap                     : 2 ;  // 13:12     
	UINT32 reg_crs_gain_res                   : 1 ;  //    14     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cdiff_hcoef_sel                : 2 ;  // 17:16     
	UINT32                                    : 10;  // 27:18     reserved
	UINT32 reg_crs_point                      : 2 ;  // 29:28     
	UINT32 reg_crs_meth                       : 2 ;  // 31:30     
} M14B0_IFC_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc0023b28L ifc_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_crs_gain_x0                    : 8 ;  // 31:24     
	UINT32 reg_crs_gain_y0                    : 8 ;  // 23:16     
	UINT32 reg_crs_gain_x1                    : 8 ;  // 15: 8     
	UINT32 reg_crs_gain_y1                    : 8 ;  //  7: 0     
} M14B0_IFC_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc0023b2cL ifc_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_6_T;

/*-----------------------------------------------------------------------------
		0xc0023b30L ifc_ctrl_7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_7_T;

/*-----------------------------------------------------------------------------
		0xc0023b34L ifc_ctrl_8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ifc_en                         : 1 ;  //     0     
	UINT32 reg_clp_on                         : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_edge_en                        : 1 ;  //     4     
	UINT32                                    : 15;  // 19: 5     reserved
	UINT32 reg_clp_tap                        : 2 ;  // 21:20     
	UINT32 reg_ifc_flt_sel                    : 2 ;  // 23:22     
	UINT32 reg_debug_en                       : 2 ;  // 25:24     
	UINT32 reg_debug_mode                     : 4 ;  // 29:26     
	UINT32 reg_bld_tap                        : 2 ;  // 31:30     
} M14B0_IFC_CTRL_8_T;

/*-----------------------------------------------------------------------------
		0xc0023b38L ifc_ctrl_9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_9_T;

/*-----------------------------------------------------------------------------
		0xc0023b3cL ifc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc0023b40L ifc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0023b44L ifc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_crs_zero_th                    : 8 ;  // 23:16     
	UINT32 reg_dst_cdst_th                    : 8 ;  // 31:24     
} M14B0_IFC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0023b48L ifc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0023b4cL ifc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc0023b50L ifc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_medge_gain_x0                  : 8 ;  // 31:24     
	UINT32 reg_medge_gain_y0                  : 8 ;  // 23:16     
	UINT32 reg_medge_gain_x1                  : 8 ;  // 15: 8     
	UINT32 reg_medge_gain_y1                  : 8 ;  //  7: 0     
} M14B0_IFC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0023b54L ifc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_crs_op_gain_isel               : 2 ;  //  1: 0     
	UINT32 reg_crs_op_gain_res                : 1 ;  //     2     
	UINT32 reg_crs_cdst_flt                   : 3 ;  //  5: 3     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 reg_crs_cdst_vmdn                  : 1 ;  //     7     
	UINT32 reg_crs_cdst_op_flt                : 3 ;  // 10: 8     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 reg_op_wgain_coef1                 : 2 ;  // 17:16     
	UINT32 reg_op_wgain_coef2                 : 2 ;  // 19:18     
	UINT32 reg_op_wgain_coef3                 : 2 ;  // 21:20     
} M14B0_IFC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0023b58L ifc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_crs_op_gain_x0                 : 8 ;  // 31:24     
	UINT32 reg_crs_op_gain_y0                 : 8 ;  // 23:16     
	UINT32 reg_crs_op_gain_x1                 : 8 ;  // 15: 8     
	UINT32 reg_crs_op_gain_y1                 : 8 ;  //  7: 0     
} M14B0_IFC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0023b5cL ifc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc0023b60L ifc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_medge_op_th                    : 8 ;  // 23:16     
	UINT32 reg_mmd_gain_en                    : 1 ;  //    24     
	UINT32 reg_mmd_gain_flt                   : 2 ;  // 26:25     
	UINT32 reg_mmd_gain_tap                   : 2 ;  // 28:27     
	UINT32 reg_mmd_gain_res                   : 1 ;  //    29     
} M14B0_IFC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0023b64L ifc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_IFC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc0023b68L ifc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_mmd_gain_x0                    : 8 ;  // 31:24     
	UINT32 reg_mmd_gain_y0                    : 8 ;  // 23:16     
	UINT32 reg_mmd_gain_x1                    : 8 ;  // 15: 8     
	UINT32 reg_mmd_gain_y1                    : 8 ;  //  7: 0     
} M14B0_IFC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc0023100L pe0_load ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} M14B0_PE0_S_LOAD_T;

/*-----------------------------------------------------------------------------
		0xc0023104L pe0_intr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 intr_src                           : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 timer_rst_sel                      : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 intr_line_pos                      : 11;  // 26:16     
} M14B0_PE0_S_INTR_T;

/*-----------------------------------------------------------------------------
		0xc0023108L pe0_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 4 ;  //  3: 0     reserved
	UINT32 ipc_frame_id                       : 4 ;  //  7: 4     
	UINT32 tnrw_y_frame_id                    : 4 ;  // 11: 8     
	UINT32 tnrw_c_frame_id                    : 4 ;  // 15:12     
	UINT32 dnr_frame_id                       : 8 ;  // 23:16     
	UINT32 pic_init_frame_id                  : 4 ;  // 27:24     
} M14B0_PE0_S_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc002310cL pe0_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_cnt                           : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 timer_cnt                          : 16;  // 31:16     
} M14B0_PE0_S_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc0023110L pe0_opmode ''
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
	UINT32                                    : 6 ;  // 27:22     reserved
	UINT32 tnr_in_lsb_mode                    : 2 ;  // 29:28     
} M14B0_PE0_S_OPMODE_T;

/*-----------------------------------------------------------------------------
		0xc0023114L pe0_src_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsize                              : 11;  // 10: 0     
	UINT32                                    : 2 ;  // 12:11     reserved
	UINT32 cs_type                            : 3 ;  // 15:13     
	UINT32 vsize                              : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 scan_type                          : 1 ;  //    31     
} M14B0_PE0_S_SRC_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0023118L pe0_field ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 field_id                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 field_mode                         : 2 ;  //  5: 4     
	UINT32 field_inv                          : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 secam_line                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 secam_line_mode                    : 2 ;  // 13:12     
} M14B0_PE0_S_FIELD_T;

/*-----------------------------------------------------------------------------
		0xc0023130L pe0_debug_mem_req ''
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
} M14B0_PE0_S_DEBUG_MEM_REQ_T;

/*-----------------------------------------------------------------------------
		0xc0023134L pe0_debug_lreq_dly ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lreq_dly_c                         : 11;  // 10: 0     
	UINT32 lreq_dly_c_en                      : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 lreq_dly_y                         : 11;  // 26:16     
	UINT32 lreq_dly_y_en                      : 1 ;  //    27     
} M14B0_PE0_S_DEBUG_LREQ_DLY_T;

/*-----------------------------------------------------------------------------
		0xc0023200L ipc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_luma_mode                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 cr_chroma_mode                     : 2 ;  //  5: 4     
	UINT32 cr_integer_edi                     : 1 ;  //     6     
	UINT32 cr_chroma_edi                      : 1 ;  //     7     
	UINT32 cr_st_2d_v_flt_range2              : 4 ;  // 11: 8     
	UINT32 cr_st_2d_v_flt_range               : 4 ;  // 15:12     
	UINT32 cr_ori_3d_blur_y                   : 2 ;  // 17:16     
	UINT32 cr_ori_3d_blur_c                   : 2 ;  // 19:18     
	UINT32 cr_tava_v_edge_adap                : 1 ;  //    20     
	UINT32 cr_tava_debug_mode                 : 1 ;  //    21     
	UINT32 cr_tava_st_adap                    : 1 ;  //    22     
	UINT32 cr_tava_en                         : 1 ;  //    23     
	UINT32 cr_hmc_force_fmd                   : 1 ;  //    24     
	UINT32 cr_st_2d_v_flt_en                  : 1 ;  //    25     
	UINT32 cr_clc_fmd_off                     : 1 ;  //    26     
	UINT32 cr_chroma_tava_only                : 1 ;  //    27     
	UINT32 cr_film_auto_gain                  : 1 ;  //    28     
	UINT32 cr_film_apply_c                    : 1 ;  //    29     
	UINT32 cr_film_apply_y                    : 1 ;  //    30     
	UINT32 cr_film_mode_enable                : 1 ;  //    31     
} M14B0_S_IPC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0023204L ipc_ctrl_01 ''
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
} M14B0_S_IPC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0023208L ipc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x0                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_x4                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x1                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y1                : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002320cL ipc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fuzzy_ctrl_point_x2                : 8 ;  //  7: 0     
	UINT32 fuzzy_ctrl_point_y2                : 8 ;  // 15: 8     
	UINT32 fuzzy_ctrl_point_x3                : 8 ;  // 23:16     
	UINT32 fuzzy_ctrl_point_y3                : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0023210L ipc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 et_offset__s7                      : 8 ;  //  7: 0     
	UINT32 motion_x_tearing_gain              : 8 ;  // 15: 8     
	UINT32 tearing_gain                       : 8 ;  // 23:16     
	UINT32 motion_gain                        : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0023214L ipc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ipc_c_motion_alpha              : 8 ;  //  7: 0     
	UINT32 cr_t_grad_weight                   : 8 ;  // 15: 8     
	UINT32 cr_v_grad_weight                   : 8 ;  // 23:16     
	UINT32 cr_h_grad_weight                   : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0023218L ipc_ctrl_06 ''
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
} M14B0_S_IPC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002321cL ipc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 cr_v_en                            : 1 ;  //     1     
	UINT32 cr_45_en                           : 1 ;  //     2     
	UINT32 cr_15_en                           : 1 ;  //     3     
	UINT32                                    : 20;  // 23: 4     reserved
	UINT32 tnr_motion_gain                    : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0023220L ipc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fmd_caption_range_top              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 fmd_caption_range_bot              : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 caption_flow_detected              : 1 ;  //    30     
	UINT32 hmc_when_cap_not_detected          : 1 ;  //    31     
} M14B0_S_IPC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0023224L ipc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pn_fmd_a_th                        : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pn_variance_th                     : 8 ;  // 23:16     
	UINT32 pn_fmd_motion_th                   : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0023228L ipc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pcnc_fmd_a_th                      : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pcnc_variance_th                   : 8 ;  // 23:16     
	UINT32 pcnc_fmd_motion_th                 : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc002322cL ipc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_pndiff_th                       : 8 ;  //  7: 0     
	UINT32 cr_center_bound_ctrl               : 8 ;  // 15: 8     
	UINT32 cr_dn_bound_ctrl                   : 8 ;  // 23:16     
	UINT32 cr_up_bound_ctrl                   : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0023230L ipc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0023234L ipc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_debug_view                  : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 cr_hmc_debug_mode                  : 1 ;  //    16     
	UINT32                                    : 2 ;  // 18:17     reserved
	UINT32 cr_hmc_debug_dir                   : 5 ;  // 23:19     
	UINT32 cr_hmc_debug_gain                  : 6 ;  // 29:24     
} M14B0_S_IPC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0023238L ipc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reserved                           : 1 ;  //     1     
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc002323cL ipc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0     
	UINT32 reg_3_2_detect_only                : 1 ;  //     1     
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2     
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3     
	UINT32 reg_hd_mode                        : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8     
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16     
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0023240L ipc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31     
} M14B0_S_IPC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0023244L ipc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_inv_resolution                  : 2 ;  //  1: 0     
	UINT32 cr_field_manual_mode               : 2 ;  //  3: 2     
	UINT32 cr_field_inverse                   : 1 ;  //     4     
	UINT32                                    : 19;  // 23: 5     reserved
	UINT32 cr_tearing_a_th                    : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0023248L ipc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_ud_gain                         : 8 ;  //  7: 0     
	UINT32 cr_lr_gain                         : 8 ;  // 15: 8     
	UINT32 cr_h_grad_gain                     : 8 ;  // 23:16     
	UINT32 cr_v_grad_gain                     : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc002324cL ipc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_errs_gain                      : 8 ;  //  7: 0     
	UINT32 reg_confidance_gain                : 8 ;  // 15: 8     
	UINT32 reg_2d_offset                      : 8 ;  // 23:16     
	UINT32 reg_flicker_gain                   : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0023250L ipc_status_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count_u                     : 16;  // 15: 0     
	UINT32 r_nc_v_count_u                     : 16;  // 31:16     
} M14B0_S_IPC_STATUS_00_T;

/*-----------------------------------------------------------------------------
		0xc0023254L ipc_status_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same_u                     : 16;  // 15: 0     
	UINT32 r_pn_v_count_u                     : 16;  // 31:16     
} M14B0_S_IPC_STATUS_01_T;

/*-----------------------------------------------------------------------------
		0xc0023258L ipc_status_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0     
	UINT32 r_fmd_32_min_th_flag_u             : 1 ;  //     6     
	UINT32 r_fmd_32_max_th_flag_u             : 1 ;  //     7     
	UINT32 r_fmd_still_limit_flag_u           : 1 ;  //     8     
	UINT32 r_fmd_still_pn_th2_flag_u          : 1 ;  //     9     
	UINT32 r_fmd_still_pn_th_flag_u           : 1 ;  //    10     
	UINT32 r_fmd_still_prev_ratio_flag_u      : 1 ;  //    11     
	UINT32 r_fmd_bigger_flag_u                : 1 ;  //    12     
	UINT32 r_fmd_smaller_flag_u               : 1 ;  //    13     
	UINT32 r_fmd_3_2_fmd_comb_mode_u          : 2 ;  // 15:14     
	UINT32 r_pn_diff_count_u                  : 16;  // 31:16     
} M14B0_S_IPC_STATUS_02_T;

/*-----------------------------------------------------------------------------
		0xc002325cL ipc_status_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 same_ng_flag_d                     : 1 ;  //     0     
	UINT32 too_small_flag_d                   : 1 ;  //     1     
	UINT32 r_fmd_still_flag_d                 : 1 ;  //     2     
	UINT32 r_fmd_possibility_flag_d           : 1 ;  //     3     
	UINT32 base_pn_ratio_flag_d               : 1 ;  //     4     
	UINT32 r_32_detected_flag_d               : 1 ;  //     5     
	UINT32 r_fmd_comb_mode_d                  : 2 ;  //  7: 6     
	UINT32 r_fmd_lock_count_d                 : 8 ;  // 15: 8     
	UINT32 same_ng_flag_u                     : 1 ;  //    16     
	UINT32 too_small_flag_u                   : 1 ;  //    17     
	UINT32 r_fmd_still_flag_u                 : 1 ;  //    18     
	UINT32 r_fmd_possibility_flag_u           : 1 ;  //    19     
	UINT32 base_pn_ratio_flag_u               : 1 ;  //    20     
	UINT32 r_32_uetected_flag_u               : 1 ;  //    21     
	UINT32 r_fmd_comb_mode_u                  : 2 ;  // 23:22     
	UINT32 r_fmd_lock_count_u                 : 8 ;  // 31:24     
} M14B0_S_IPC_STATUS_03_T;

/*-----------------------------------------------------------------------------
		0xc0023280L ipc_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 tava_v_diff_x1                     : 8 ;  // 31:24     
	UINT32 tava_v_diff_y1                     : 8 ;  // 23:16     
	UINT32 tava_v_diff_x0                     : 8 ;  // 15: 8     
	UINT32 tava_v_diff_y0                     : 8 ;  //  7: 0     
} M14B0_S_IPC_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc0023284L ipc_ctrl_35 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 reg_div_position                   : 8 ;  // 31:24     
	UINT32 reg_manual_mode1                   : 2 ;  // 23:22     
	UINT32 reg_manual_mode2                   : 2 ;  // 21:20     
	UINT32 reg_manual_en                      : 1 ;  //    16     
	UINT32 cr_sub_main_sel                    : 1 ;  //     4     
	UINT32 cr_bound_exp_en                    : 1 ;  //     1     
	UINT32 reg_div_en                         : 1 ;  //     0     
} M14B0_S_IPC_CTRL_35_T;

/*-----------------------------------------------------------------------------
		0xc0023288L ipc_ctrl_36 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th                    : 8 ;  //  7: 0     
	UINT32 reg_fmd_ratio_th                   : 8 ;  // 15: 8     
	UINT32 reg_fmd_max_th                     : 8 ;  // 23:16     
	UINT32 reg_fmd_min_th                     : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_36_T;

/*-----------------------------------------------------------------------------
		0xc002328cL ipc_ctrl_37 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lockcnt_en                     : 1 ;  //     0     
	UINT32 reserved                           : 1 ;  //     1     
	UINT32 reg_lock_speed                     : 6 ;  //  7: 2     
	UINT32 reg_lockcnt_th                     : 8 ;  // 15: 8     
	UINT32 reg_same_th                        : 8 ;  // 23:16     
	UINT32 reg_still_frame_limit              : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_37_T;

/*-----------------------------------------------------------------------------
		0xc0023290L ipc_ctrl_38 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_3_2_detect_en                  : 1 ;  //     0     
	UINT32 reg_3_2_detect_only                : 1 ;  //     1     
	UINT32 reg_3_2_lockcnt_255_en             : 1 ;  //     2     
	UINT32 reg_3_2_pndiff_disable             : 1 ;  //     3     
	UINT32 reg_hd_mode                        : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_3_2_pndiff_th                  : 8 ;  // 15: 8     
	UINT32 reg_3_2_pn_ratio_max_th            : 8 ;  // 23:16     
	UINT32 reg_3_2_pn_ratio_min_th            : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_38_T;

/*-----------------------------------------------------------------------------
		0xc0023294L ipc_ctrl_39 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_still_pn_th2                   : 8 ;  //  7: 0     
	UINT32 reg_still_prev_ratio               : 8 ;  // 15: 8     
	UINT32 reg_lock_reducer                   : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_badedit_protect                : 1 ;  //    31     
} M14B0_S_IPC_CTRL_39_T;

/*-----------------------------------------------------------------------------
		0xc0023298L ipc_ctrl_40 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} M14B0_S_IPC_CTRL_40_T;

/*-----------------------------------------------------------------------------
		0xc002329cL ipc_ctrl_41 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fend_position                  : 11;  // 10: 0     
} M14B0_S_IPC_CTRL_41_T;

/*-----------------------------------------------------------------------------
		0xc00232f0L ipc_ctrl_20 ''
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
} M14B0_S_IPC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc00232f4L ipc_ctrl_21 ''
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
} M14B0_S_IPC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc00232f8L ipc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_flicker_diff_gain               : 8 ;  //  7: 0     
	UINT32 cr_flicker_amp_gain                : 8 ;  // 15: 8     
} M14B0_S_IPC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc00232fcL ipc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_hmc_enable                      : 1 ;  //     0     
	UINT32 cr_lock_th                         : 7 ;  //  7: 1     
	UINT32 cr_g_motion_th                     : 8 ;  // 15: 8     
	UINT32 cr_hmc_expend                      : 3 ;  // 18:16     
	UINT32 cr_hmc_half_en                     : 1 ;  //    19     
	UINT32 cr_robustness                      : 4 ;  // 23:20     
	UINT32 cr_center_weight                   : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc0023300L ipc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_noise_en                        : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_noise_th                        : 8 ;  // 15: 8     
	UINT32 cr_gds_resolution                  : 3 ;  // 18:16     
} M14B0_S_IPC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc0023304L ipc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vdiff_en                       : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_vdiff_th                       : 8 ;  // 15: 8     
	UINT32 reg_hmc_err_max                    : 8 ;  // 23:16     
	UINT32 reg_hmc_err_min                    : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc0023308L ipc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_fmd_check_en                    : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 cr_fmd_ath                         : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 cr_fmd_ratio_th                    : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc002330cL ipc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_st_flt_ctrl_x0              : 8 ;  //  7: 0     
	UINT32 chroma_st_flt_ctrl_x1              : 8 ;  // 15: 8     
	UINT32 luma_st_flt_ctrl_x0                : 8 ;  // 23:16     
	UINT32 luma_st_flt_ctrl_x1                : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc0023310L ipc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hmc_bonus_es_en                : 1 ;  //     0     
	UINT32 reg_hmc_bonus_et_en                : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_hmc_bonus_gain                 : 8 ;  // 15: 8     
	UINT32 reg_hmc_bonus_th                   : 8 ;  // 23:16     
} M14B0_S_IPC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc0023314L ipc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 hmc_flt_ctrl_x0                    : 8 ;  // 23:16     
	UINT32 hmc_st_flt_ctrl_x1                 : 8 ;  // 31:24     
} M14B0_S_IPC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc0023318L ipc_ctrl_30 ''
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
} M14B0_S_IPC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc002331cL ipc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_toosmall_en                    : 1 ;  //     0     
	UINT32 reg_toosmall_min                   : 7 ;  //  7: 1     
	UINT32 reg_toosmall_max                   : 8 ;  // 15: 8     
	UINT32 reg_pndiff_disable                 : 1 ;  //    16     
} M14B0_S_IPC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc0023320L ipc_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 tava_errt_x3                       : 8 ;  // 31:24     
	UINT32 tava_errt_y3                       : 8 ;  // 23:16     
	UINT32 tava_errt_x2                       : 8 ;  // 15: 8     
	UINT32 tava_errt_y2                       : 8 ;  //  7: 0     
} M14B0_S_IPC_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc0023324L ipc_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 tava_errt_x1                       : 8 ;  // 31:24     
	UINT32 tava_errt_y1                       : 8 ;  // 23:16     
	UINT32 tava_errt_x0                       : 8 ;  // 15: 8     
	UINT32 tava_errt_y0                       : 8 ;  //  7: 0     
} M14B0_S_IPC_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc0023328L ipc_status_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_v_count_d                     : 16;  // 15: 0     
	UINT32 r_nc_v_count_d                     : 16;  // 31:16     
} M14B0_S_IPC_STATUS_04_T;

/*-----------------------------------------------------------------------------
		0xc002332cL ipc_status_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pc_nc_same_d                     : 16;  // 15: 0     
	UINT32 r_pn_v_count_d                     : 16;  // 31:16     
} M14B0_S_IPC_STATUS_05_T;

/*-----------------------------------------------------------------------------
		0xc0023330L ipc_status_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 6 ;  //  5: 0     
	UINT32 r_fmd_32_min_th_flag_d             : 1 ;  //     6     
	UINT32 r_fmd_32_max_th_flag_d             : 1 ;  //     7     
	UINT32 r_fmd_still_limit_flag_d           : 1 ;  //     8     
	UINT32 r_fmd_still_pn_th2_flag_d          : 1 ;  //     9     
	UINT32 r_fmd_still_pn_th_flag_d           : 1 ;  //    10     
	UINT32 r_fmd_still_prev_ratio_flag_d      : 1 ;  //    11     
	UINT32 r_fmd_bigger_flag_d                : 1 ;  //    12     
	UINT32 r_fmd_smaller_flag_d               : 1 ;  //    13     
	UINT32 r_fmd_3_2_fmd_comb_mode_d          : 2 ;  // 15:14     
	UINT32 r_pn_diff_count_d                  : 16;  // 31:16     
} M14B0_S_IPC_STATUS_06_T;

/*-----------------------------------------------------------------------------
		0xc0023334L ipc_status_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_inv_inversed_cnt                 : 16;  // 15: 0     
	UINT32 r_inv_original_cnt                 : 16;  // 31:16     
} M14B0_S_IPC_STATUS_07_T;

/*-----------------------------------------------------------------------------
		0xc0023338L ipc_status_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_S_IPC_STATUS_08_T;

/*-----------------------------------------------------------------------------
		0xc002333cL ipc_status_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_S_IPC_STATUS_09_T;

/*-----------------------------------------------------------------------------
		0xc0023340L tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_show_scale                     : 3 ;  //  2: 0     
	UINT32 reg_bbd_show                       : 1 ;  //     3     
	UINT32 reg_show_bar_1                     : 1 ;  //     4     
	UINT32 reg_show_bar_2                     : 1 ;  //     5     
	UINT32 reg_show_bar_3                     : 1 ;  //     6     
	UINT32 reg_bar1_data_mux                  : 2 ;  //  8: 7     
	UINT32 reg_bar2_data_mux                  : 2 ;  // 10: 9     
	UINT32 reg_bar3_data_mux                  : 2 ;  // 12:11     
	UINT32 reg_hist0_data_mux                 : 4 ;  // 16:13     
	UINT32 reg_hist1_data_mux                 : 4 ;  // 20:17     
	UINT32 reg_hist2_data_mux                 : 4 ;  // 24:21     
	UINT32 reg_hist0_win_sel                  : 2 ;  // 26:25     
	UINT32 reg_hist1_win_sel                  : 2 ;  // 28:27     
	UINT32 reg_hist2_win_sel                  : 2 ;  // 30:29     
	UINT32 hist_rd_en                         : 1 ;  //    31     
} M14B0_S_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0023344L tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x0                            : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 reg_wina_hen                       : 1 ;  //    15     
	UINT32 wina_y0                            : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 reg_wina_ven                       : 1 ;  //    31     
} M14B0_S_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0023348L tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wina_x1                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wina_y1                            : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 reg_hist_hsv_hsl_sel               : 1 ;  //    30     
	UINT32 reg_hist_vscaling_en               : 1 ;  //    31     
} M14B0_S_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0023384L apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_g_s_apl                          : 10;  //  9: 0     
	UINT32 cb_b_s_apl                         : 10;  // 19:10     
	UINT32 cr_r_s_apl                         : 10;  // 29:20     
} M14B0_S_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0023398L lvcrtl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 y_offset                           : 10;  // 13: 4     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 center_position                    : 8 ;  // 23:16     
	UINT32 y_gain                             : 8 ;  // 31:24     
} M14B0_S_LVCRTL_CTRL_00_T;

typedef struct {
	M14B0_PE0_M_LOAD_T                   	pe0_load;                     //0xc0022100L
	M14B0_PE0_M_INTR_T                   	pe0_intr;                     //0xc0022104L
	M14B0_PE0_M_STATUS0_T                	pe0_status0;                  //0xc0022108L
	M14B0_PE0_M_STATUS1_T                	pe0_status1;                  //0xc002210cL
	M14B0_PE0_M_OPMODE_T                 	pe0_opmode;                   //0xc0022110L
	M14B0_PE0_M_SRC_SIZE_T               	pe0_src_size;                 //0xc0022114L
	M14B0_PE0_M_FIELD_T                  	pe0_field;                    //0xc0022118L
	UINT32                             	reserved00[5];                //0xc002211cL~0xc002212cL
	M14B0_PE0_M_DEBUG_MEM_REQ_T          	pe0_debug_mem_req;            //0xc0022130L
	M14B0_PE0_M_DEBUG_LREQ_DLY_T         	pe0_debug_lreq_dly;           //0xc0022134L
	UINT32                             	reserved01[2];                //0xc0022138L~0xc002213cL
	M14B0_TNR_CTRL_00_T                	tnr_ctrl_00;                  //0xc0022140L
	M14B0_TNR_CTRL_01_T                	tnr_ctrl_01;                  //0xc0022144L
	M14B0_TNR_CTRL_02_T                	tnr_ctrl_02;                  //0xc0022148L
	M14B0_TNR_CTRL_03_T                	tnr_ctrl_03;                  //0xc002214cL
	M14B0_TNR_CTRL_04_T                	tnr_ctrl_04;                  //0xc0022150L
	M14B0_TNR_CTRL_05_T                	tnr_ctrl_05;                  //0xc0022154L
	M14B0_TNR_CTRL_06_T                	tnr_ctrl_06;                  //0xc0022158L
	M14B0_TNR_CTRL_07_T                	tnr_ctrl_07;                  //0xc002215cL
	M14B0_TNR_CTRL_08_T                	tnr_ctrl_08;                  //0xc0022160L
	M14B0_TNR_CTRL_09_T                	tnr_ctrl_09;                  //0xc0022164L
	M14B0_TNR_CTRL_10_T                	tnr_ctrl_10;                  //0xc0022168L
	M14B0_TNR_CTRL_11_T                	tnr_ctrl_11;                  //0xc002216cL
	M14B0_TNR_CTRL_12_T                	tnr_ctrl_12;                  //0xc0022170L
	M14B0_TNR_CTRL_13_T                	tnr_ctrl_13;                  //0xc0022174L
	M14B0_TNR_CTRL_14_T                	tnr_ctrl_14;                  //0xc0022178L
	M14B0_TNR_CTRL_15_T                	tnr_ctrl_15;                  //0xc002217cL
	M14B0_TNR_CTRL_16_T                	tnr_ctrl_16;                  //0xc0022180L
	M14B0_TNR_CTRL_17_T                	tnr_ctrl_17;                  //0xc0022184L
	M14B0_TNR_CTRL_18_T                	tnr_ctrl_18;                  //0xc0022188L
	M14B0_TNR_CTRL_19_T                	tnr_ctrl_19;                  //0xc002218cL
	M14B0_TNR_CTRL_20_T                	tnr_ctrl_20;                  //0xc0022190L
	M14B0_TNR_CTRL_21_T                	tnr_ctrl_21;                  //0xc0022194L
	M14B0_TNR_CTRL_22_T                	tnr_ctrl_22;                  //0xc0022198L
	M14B0_TNR_CTRL_23_T                	tnr_ctrl_23;                  //0xc002219cL
	M14B0_TNR_CTRL_24_T                	tnr_ctrl_24;                  //0xc00221a0L
	M14B0_TNR_CTRL_25_T                	tnr_ctrl_25;                  //0xc00221a4L
	M14B0_TNR_CTRL_26_T                	tnr_ctrl_26;                  //0xc00221a8L
	M14B0_TNR_CTRL_27_T                	tnr_ctrl_27;                  //0xc00221acL
	M14B0_TNR_CTRL_28_T                	tnr_ctrl_28;                  //0xc00221b0L
	M14B0_TNR_CTRL_29_T                	tnr_ctrl_29;                  //0xc00221b4L
	M14B0_TNR_CTRL_30_T                	tnr_ctrl_30;                  //0xc00221b8L
	M14B0_TNR_CTRL_31_T                	tnr_ctrl_31;                  //0xc00221bcL
	M14B0_TNR_STATUS_00_T              	tnr_status_00;                //0xc00221c0L
	M14B0_TNR_STATUS_01_T              	tnr_status_01;                //0xc00221c4L
	M14B0_TNR_STATUS_02_T              	tnr_status_02;                //0xc00221c8L
	M14B0_TNR_STATUS_03_T              	tnr_status_03;                //0xc00221ccL
	M14B0_TNR_MAIN_LUT_00_T            	tnr_main_lut_00;              //0xc00221d0L
	M14B0_TNR_MAIN_LUT_01_T            	tnr_main_lut_01;              //0xc00221d4L
	M14B0_TNR_MAIN_LUT_02_T            	tnr_main_lut_02;              //0xc00221d8L
	M14B0_TNR_STATUS_04_T              	tnr_status_04;                //0xc00221dcL
	M14B0_TNR_CTRL_32_T                	tnr_ctrl_32;                  //0xc00221e0L
	M14B0_TNR_CTRL_33_T                	tnr_ctrl_33;                  //0xc00221e4L
	M14B0_TNR_CTRL_34_T                	tnr_ctrl_34;                  //0xc00221e8L
	M14B0_TNR_CTRL_35_T                	tnr_ctrl_35;                  //0xc00221ecL
	M14B0_TNR_CTRL_36_T                	tnr_ctrl_36;                  //0xc00221f0L
	M14B0_TNR_CTRL_37_T                	tnr_ctrl_37;                  //0xc00221f4L
	M14B0_TNR_CTRL_38_T                	tnr_ctrl_38;                  //0xc00221f8L
	M14B0_TNR_CTRL_39_T                	tnr_ctrl_39;                  //0xc00221fcL
	M14B0_M_IPC_CTRL_00_T                	ipc_ctrl_00;                  //0xc0022200L
	M14B0_M_IPC_CTRL_01_T                	ipc_ctrl_01;                  //0xc0022204L
	M14B0_M_IPC_CTRL_02_T                	ipc_ctrl_02;                  //0xc0022208L
	M14B0_M_IPC_CTRL_03_T                	ipc_ctrl_03;                  //0xc002220cL
	M14B0_M_IPC_CTRL_04_T                	ipc_ctrl_04;                  //0xc0022210L
	M14B0_M_IPC_CTRL_05_T                	ipc_ctrl_05;                  //0xc0022214L
	M14B0_M_IPC_CTRL_06_T                	ipc_ctrl_06;                  //0xc0022218L
	M14B0_M_IPC_CTRL_07_T                	ipc_ctrl_07;                  //0xc002221cL
	M14B0_M_IPC_CTRL_08_T                	ipc_ctrl_08;                  //0xc0022220L
	M14B0_M_IPC_CTRL_09_T                	ipc_ctrl_09;                  //0xc0022224L
	M14B0_M_IPC_CTRL_10_T                	ipc_ctrl_10;                  //0xc0022228L
	M14B0_M_IPC_CTRL_11_T                	ipc_ctrl_11;                  //0xc002222cL
	M14B0_M_IPC_CTRL_12_T                	ipc_ctrl_12;                  //0xc0022230L
	M14B0_M_IPC_CTRL_13_T                	ipc_ctrl_13;                  //0xc0022234L
	M14B0_M_IPC_CTRL_14_T                	ipc_ctrl_14;                  //0xc0022238L
	M14B0_M_IPC_CTRL_15_T                	ipc_ctrl_15;                  //0xc002223cL
	M14B0_M_IPC_CTRL_16_T                	ipc_ctrl_16;                  //0xc0022240L
	M14B0_M_IPC_CTRL_17_T                	ipc_ctrl_17;                  //0xc0022244L
	M14B0_M_IPC_CTRL_18_T                	ipc_ctrl_18;                  //0xc0022248L
	M14B0_M_IPC_CTRL_19_T                	ipc_ctrl_19;                  //0xc002224cL
	M14B0_M_IPC_STATUS_00_T              	ipc_status_00;                //0xc0022250L
	M14B0_M_IPC_STATUS_01_T              	ipc_status_01;                //0xc0022254L
	M14B0_M_IPC_STATUS_02_T              	ipc_status_02;                //0xc0022258L
	M14B0_M_IPC_STATUS_03_T              	ipc_status_03;                //0xc002225cL
	M14B0_CLC_CTRL_00_T                	clc_ctrl_00;                  //0xc0022260L
	M14B0_CLC_CTRL_01_T                	clc_ctrl_01;                  //0xc0022264L
	M14B0_CLC_CTRL_02_T                	clc_ctrl_02;                  //0xc0022268L
	M14B0_CLC_CTRL_03_T                	clc_ctrl_03;                  //0xc002226cL
	M14B0_CLC_CTRL_04_T                	clc_ctrl_04;                  //0xc0022270L
	M14B0_CLC_CTRL_05_T                	clc_ctrl_05;                  //0xc0022274L
	M14B0_CLC_CTRL_06_T                	clc_ctrl_06;                  //0xc0022278L
	M14B0_CLC_STAT_00_T                	clc_stat_00;                  //0xc002227cL
	M14B0_M_IPC_CTRL_34_T                	ipc_ctrl_34;                  //0xc0022280L
	M14B0_M_IPC_CTRL_35_T                	ipc_ctrl_35;                  //0xc0022284L
	M14B0_M_IPC_CTRL_36_T                	ipc_ctrl_36;                  //0xc0022288L
	M14B0_M_IPC_CTRL_37_T                	ipc_ctrl_37;                  //0xc002228cL
	M14B0_M_IPC_CTRL_38_T                	ipc_ctrl_38;                  //0xc0022290L
	M14B0_M_IPC_CTRL_39_T                	ipc_ctrl_39;                  //0xc0022294L
	M14B0_M_IPC_CTRL_40_T                	ipc_ctrl_40;                  //0xc0022298L
	M14B0_M_IPC_CTRL_41_T                	ipc_ctrl_41;                  //0xc002229cL
	M14B0_M_IPC_CTRL_42_T                	ipc_ctrl_42;                  //0xc00222a0L
	UINT32                             	reserved02[19];               //0xc00222a4L~0xc00222ecL
	M14B0_M_IPC_CTRL_20_T                	ipc_ctrl_20;                  //0xc00222f0L
	M14B0_M_IPC_CTRL_21_T                	ipc_ctrl_21;                  //0xc00222f4L
	M14B0_M_IPC_CTRL_22_T                	ipc_ctrl_22;                  //0xc00222f8L
	M14B0_M_IPC_CTRL_23_T                	ipc_ctrl_23;                  //0xc00222fcL
	M14B0_M_IPC_CTRL_24_T                	ipc_ctrl_24;                  //0xc0022300L
	M14B0_M_IPC_CTRL_25_T                	ipc_ctrl_25;                  //0xc0022304L
	M14B0_M_IPC_CTRL_26_T                	ipc_ctrl_26;                  //0xc0022308L
	M14B0_M_IPC_CTRL_27_T                	ipc_ctrl_27;                  //0xc002230cL
	M14B0_M_IPC_CTRL_28_T                	ipc_ctrl_28;                  //0xc0022310L
	M14B0_M_IPC_CTRL_29_T                	ipc_ctrl_29;                  //0xc0022314L
	M14B0_M_IPC_CTRL_30_T                	ipc_ctrl_30;                  //0xc0022318L
	M14B0_M_IPC_CTRL_31_T                	ipc_ctrl_31;                  //0xc002231cL
	M14B0_M_IPC_CTRL_32_T                	ipc_ctrl_32;                  //0xc0022320L
	M14B0_M_IPC_CTRL_33_T                	ipc_ctrl_33;                  //0xc0022324L
	M14B0_M_IPC_STATUS_04_T              	ipc_status_04;                //0xc0022328L
	M14B0_M_IPC_STATUS_05_T              	ipc_status_05;                //0xc002232cL
	M14B0_M_IPC_STATUS_06_T              	ipc_status_06;                //0xc0022330L
	M14B0_M_IPC_STATUS_07_T              	ipc_status_07;                //0xc0022334L
	M14B0_M_IPC_STATUS_08_T              	ipc_status_08;                //0xc0022338L
	M14B0_M_IPC_STATUS_09_T              	ipc_status_09;                //0xc002233cL
	M14B0_M_TPD_CTRL_00_T                	tpd_ctrl_00;                  //0xc0022340L
	M14B0_M_TPD_CTRL_01_T                	tpd_ctrl_01;                  //0xc0022344L
	M14B0_M_TPD_CTRL_02_T                	tpd_ctrl_02;                  //0xc0022348L
	M14B0_M_TPD_CTRL_03_T                	tpd_ctrl_03;                  //0xc002234cL
	M14B0_M_TPD_CTRL_04_T                	tpd_ctrl_04;                  //0xc0022350L
	M14B0_M_TPD_CTRL_05_T                	tpd_ctrl_05;                  //0xc0022354L
	M14B0_M_TPD_CTRL_06_T                	tpd_ctrl_06;                  //0xc0022358L
	M14B0_M_TPD_CTRL_07_T                	tpd_ctrl_07;                  //0xc002235cL
	M14B0_M_TPD_CTRL_08_T                	tpd_ctrl_08;                  //0xc0022360L
	M14B0_M_TPD_CTRL_09_T                	tpd_ctrl_09;                  //0xc0022364L
	M14B0_TPD_STAT_00_T                	tpd_stat_00;                  //0xc0022368L
	M14B0_CLC_CTRL_07_T                	clc_ctrl_07;                  //0xc002236cL
	M14B0_BBD_CTRL_00_T                	bbd_ctrl_00;                  //0xc0022370L
	M14B0_BBD_CTRL_01_T                	bbd_ctrl_01;                  //0xc0022374L
	M14B0_BBD_CTRL_02_T                	bbd_ctrl_02;                  //0xc0022378L
	M14B0_BBD_STAT_00_T                	bbd_stat_00;                  //0xc002237cL
	M14B0_BBD_STAT_01_T                	bbd_stat_01;                  //0xc0022380L
	M14B0_M_APL_STAT_00_T                	apl_stat_00;                  //0xc0022384L
	UINT32                             	reserved03[2];                //0xc0022388L~0xc002238cL
	M14B0_TPD_HIST_AI_CTRL_T           	tpd_hist_ai_ctrl;             //0xc0022390L
	M14B0_TPD_HIST_AI_DATA_T           	tpd_hist_ai_data;             //0xc0022394L
	M14B0_M_LVCRTL_CTRL_00_T             	lvcrtl_ctrl_00;               //0xc0022398L
	UINT32                             	reserved04[13];               //0xc002239cL~0xc00223ccL
	M14B0_TNR_CTRL_40_T                	tnr_ctrl_40;                  //0xc00223d0L
	M14B0_TNR_CTRL_41_T                	tnr_ctrl_41;                  //0xc00223d4L
	M14B0_TNR_CTRL_42_T                	tnr_ctrl_42;                  //0xc00223d8L
	M14B0_TNR_CTRL_43_T                	tnr_ctrl_43;                  //0xc00223dcL
	M14B0_TNR_CTRL_44_T                	tnr_ctrl_44;                  //0xc00223e0L
	M14B0_TNR_CTRL_45_T                	tnr_ctrl_45;                  //0xc00223e4L
	M14B0_TNR_CTRL_46_T                	tnr_ctrl_46;                  //0xc00223e8L
	M14B0_TNR_CTRL_47_T                	tnr_ctrl_47;                  //0xc00223ecL
	M14B0_TNR_CTRL_48_T                	tnr_ctrl_48;                  //0xc00223f0L
	M14B0_TNR_CTRL_49_T                	tnr_ctrl_49;                  //0xc00223f4L
	M14B0_TNR_CTRL_50_T                	tnr_ctrl_50;                  //0xc00223f8L
	M14B0_TNR_CTRL_51_T                	tnr_ctrl_51;                  //0xc00223fcL
} DE_P0M_REG_M14B0_T; // PE0 M

typedef struct {
	M14B0_PE0_DNR_LOAD_T                   	pe0_load;                     //0xc0023a00L
	M14B0_PE0_DNR_INTR_T                   	pe0_intr;                     //0xc0023a04L
	M14B0_PE0_DNR_STATUS0_T                	pe0_status0;                  //0xc0023a08L
	M14B0_PE0_DNR_STATUS1_T                	pe0_status1;                  //0xc0023a0cL
	M14B0_PE0_DNR_OPMODE_T                 	pe0_opmode;                   //0xc0023a10L
	M14B0_PE0_DNR_SRC_SIZE_T               	pe0_src_size;                 //0xc0023a14L
	M14B0_PE0_DNR_FIELD_T                  	pe0_field;                    //0xc0023a18L
	UINT32                             	reserved00;                   //0xc0023a1cL
	M14B0_WIN_CTRL_0_T                 	win_ctrl_0;                   //0xc0023a20L
	M14B0_WIN_CTRL_1_T                 	win_ctrl_1;                   //0xc0023a24L
	M14B0_WIN_CTRL_2_T                 	win_ctrl_2;                   //0xc0023a28L
	M14B0_WIN_CTRL_3_T                 	win_ctrl_3;                   //0xc0023a2cL
	M14B0_CUP_CTRL_00_T                	cup_ctrl_00;                  //0xc0023a30L
	M14B0_CUP_CTRL_01_T                	cup_ctrl_01;                  //0xc0023a34L
	M14B0_CUP_CTRL_02_T                	cup_ctrl_02;                  //0xc0023a38L
	UINT32                             	reserved01;                   //0xc0023a3cL
	M14B0_LBAF_CTRL_00_T               	lbaf_ctrl_00;                 //0xc0023a40L
	UINT32                             	reserved02[3];                //0xc0023a44L~0xc0023a4cL
	M14B0_3DFD_CTRL_00_T               	_3dfd_ctrl_00;                 //0xc0023a50L
	M14B0_3DFD_CTRL_01_T               	_3dfd_ctrl_01;                 //0xc0023a54L
	M14B0_3DFD_CTRL_02_T               	_3dfd_ctrl_02;                 //0xc0023a58L
	M14B0_3DFD_CTRL_03_T               	_3dfd_ctrl_03;                 //0xc0023a5cL
	M14B0_3DFD_STAT_00_T               	_3dfd_stat_00;                 //0xc0023a60L
	M14B0_3DFD_STAT_01_T               	_3dfd_stat_01;                 //0xc0023a64L
	M14B0_3DFD_STAT_02_T               	_3dfd_stat_02;                 //0xc0023a68L
	M14B0_3DFD_STAT_03_T               	_3dfd_stat_03;                 //0xc0023a6cL
	M14B0_3DFD_STAT_04_T               	_3dfd_stat_04;                 //0xc0023a70L
	M14B0_3DFD_STAT_05_T               	_3dfd_stat_05;                 //0xc0023a74L
	M14B0_3DFD_STAT_06_T               	_3dfd_stat_06;                 //0xc0023a78L
	M14B0_3DFD_STAT_07_T               	_3dfd_stat_07;                 //0xc0023a7cL
	M14B0_VFILTER_CTRL_0_T             	vfilter_ctrl_0;               //0xc0023a80L
	M14B0_VFILTER_CTRL_1_T             	vfilter_ctrl_1;               //0xc0023a84L
	M14B0_VFILTER_CTRL_2_T             	vfilter_ctrl_2;               //0xc0023a88L
	M14B0_CTI_CTRL_0_T                 	cti_ctrl_0;                   //0xc0023a8cL
	M14B0_CTI_CTRL_1_T                 	cti_ctrl_1;                   //0xc0023a90L
	M14B0_DNR_MAX_CTRL_T               	dnr_max_ctrl;                 //0xc0023a94L
	M14B0_DNR_DBAR_CTRL_T              	dnr_dbar_ctrl;                //0xc0023a98L
	M14B0_REG_420C_CTRL_T              	reg_420c_ctrl;                //0xc0023a9cL
	M14B0_MNR_CTRL_0_T                 	mnr_ctrl_0;                   //0xc0023aa0L
	M14B0_MNR_CTRL_1_T                 	mnr_ctrl_1;                   //0xc0023aa4L
	M14B0_MNR_CTRL_2_T                 	mnr_ctrl_2;                   //0xc0023aa8L
	M14B0_MNR_CTRL_3_T                 	mnr_ctrl_3;                   //0xc0023aacL
	M14B0_MNR_CTRL_4_T                 	mnr_ctrl_4;                   //0xc0023ab0L
	M14B0_MNR_CTRL_5_T                 	mnr_ctrl_5;                   //0xc0023ab4L
	M14B0_DETAIL_CTRL_T                	detail_ctrl;                  //0xc0023ab8L
	M14B0_DC_BNR_CTRL_0_T              	dc_bnr_ctrl_0;                //0xc0023abcL
	M14B0_DC_BNR_CTRL_1_T              	dc_bnr_ctrl_1;                //0xc0023ac0L
	M14B0_DC_BNR_CTRL_2_T              	dc_bnr_ctrl_2;                //0xc0023ac4L
	M14B0_DC_BNR_CTRL_3_T              	dc_bnr_ctrl_3;                //0xc0023ac8L
	M14B0_DC_BNR_CTRL_4_T              	dc_bnr_ctrl_4;                //0xc0023accL
	M14B0_DC_BNR_CTRL_5_T              	dc_bnr_ctrl_5;                //0xc0023ad0L
	M14B0_AC_BNR_CTRL_0_T              	ac_bnr_ctrl_0;                //0xc0023ad4L
	M14B0_AC_BNR_CTRL_1_T              	ac_bnr_ctrl_1;                //0xc0023ad8L
	M14B0_AC_BNR_CTRL_2_T              	ac_bnr_ctrl_2;                //0xc0023adcL
	M14B0_BNR_STAT_0_T                 	bnr_stat_0;                   //0xc0023ae0L
	M14B0_AC_BNR_CTRL_3_T              	ac_bnr_ctrl_3;                //0xc0023ae4L
	M14B0_AC_BNR_CTRL_4_T              	ac_bnr_ctrl_4;                //0xc0023ae8L
	M14B0_BNR_STAT_1_T                 	bnr_stat_1;                   //0xc0023aecL
	M14B0_AC_BNR_CTRL_5_T              	ac_bnr_ctrl_5;                //0xc0023af0L
	M14B0_AC_BNR_CTRL_6_T              	ac_bnr_ctrl_6;                //0xc0023af4L
	M14B0_AC_BNR_CTRL_7_T              	ac_bnr_ctrl_7;                //0xc0023af8L
	M14B0_BNR_STAT_2_T                 	bnr_stat_2;                   //0xc0023afcL
	M14B0_AC_BNR_CTRL_8_T              	ac_bnr_ctrl_8;                //0xc0023b00L
	M14B0_AC_BNR_CTRL_9_T              	ac_bnr_ctrl_9;                //0xc0023b04L
	M14B0_AC_BNR_CTRL_10_T             	ac_bnr_ctrl_10;               //0xc0023b08L
	M14B0_AC_BNR_CTRL_11_T             	ac_bnr_ctrl_11;               //0xc0023b0cL
	M14B0_AC_BNR_CTRL_12_T             	ac_bnr_ctrl_12;               //0xc0023b10L
	M14B0_IFC_CTRL_0_T                 	ifc_ctrl_0;                   //0xc0023b14L
	M14B0_IFC_CTRL_1_T                 	ifc_ctrl_1;                   //0xc0023b18L
	M14B0_IFC_CTRL_2_T                 	ifc_ctrl_2;                   //0xc0023b1cL
	M14B0_IFC_CTRL_3_T                 	ifc_ctrl_3;                   //0xc0023b20L
	M14B0_IFC_CTRL_4_T                 	ifc_ctrl_4;                   //0xc0023b24L
	M14B0_IFC_CTRL_5_T                 	ifc_ctrl_5;                   //0xc0023b28L
	M14B0_IFC_CTRL_6_T                 	ifc_ctrl_6;                   //0xc0023b2cL
	M14B0_IFC_CTRL_7_T                 	ifc_ctrl_7;                   //0xc0023b30L
	M14B0_IFC_CTRL_8_T                 	ifc_ctrl_8;                   //0xc0023b34L
	M14B0_IFC_CTRL_9_T                 	ifc_ctrl_9;                   //0xc0023b38L
	M14B0_IFC_CTRL_10_T                	ifc_ctrl_10;                  //0xc0023b3cL
	M14B0_IFC_CTRL_11_T                	ifc_ctrl_11;                  //0xc0023b40L
	M14B0_IFC_CTRL_12_T                	ifc_ctrl_12;                  //0xc0023b44L
	M14B0_IFC_CTRL_13_T                	ifc_ctrl_13;                  //0xc0023b48L
	M14B0_IFC_CTRL_14_T                	ifc_ctrl_14;                  //0xc0023b4cL
	M14B0_IFC_CTRL_15_T                	ifc_ctrl_15;                  //0xc0023b50L
	M14B0_IFC_CTRL_16_T                	ifc_ctrl_16;                  //0xc0023b54L
	M14B0_IFC_CTRL_17_T                	ifc_ctrl_17;                  //0xc0023b58L
	M14B0_IFC_CTRL_18_T                	ifc_ctrl_18;                  //0xc0023b5cL
	M14B0_IFC_CTRL_19_T                	ifc_ctrl_19;                  //0xc0023b60L
	M14B0_IFC_CTRL_20_T                	ifc_ctrl_20;                  //0xc0023b64L
	M14B0_IFC_CTRL_21_T                	ifc_ctrl_21;                  //0xc0023b68L
} DE_P0D_REG_M14B0_T; // PE0 DNR

typedef struct {
	M14B0_PE0_S_LOAD_T                   	pe0_load;                     //0xc0023100L
	M14B0_PE0_S_INTR_T                   	pe0_intr;                     //0xc0023104L
	M14B0_PE0_S_STATUS0_T                	pe0_status0;                  //0xc0023108L
	M14B0_PE0_S_STATUS1_T                	pe0_status1;                  //0xc002310cL
	M14B0_PE0_S_OPMODE_T                 	pe0_opmode;                   //0xc0023110L
	M14B0_PE0_S_SRC_SIZE_T               	pe0_src_size;                 //0xc0023114L
	M14B0_PE0_S_FIELD_T                  	pe0_field;                    //0xc0023118L
	UINT32                             	reserved00[5];                //0xc002311cL~0xc002312cL
	M14B0_PE0_S_DEBUG_MEM_REQ_T          	pe0_debug_mem_req;            //0xc0023130L
	M14B0_PE0_S_DEBUG_LREQ_DLY_T         	pe0_debug_lreq_dly;           //0xc0023134L
	UINT32                             	reserved01[50];               //0xc0023138L~0xc00231fcL
	M14B0_S_IPC_CTRL_00_T                	ipc_ctrl_00;                  //0xc0023200L
	M14B0_S_IPC_CTRL_01_T                	ipc_ctrl_01;                  //0xc0023204L
	M14B0_S_IPC_CTRL_02_T                	ipc_ctrl_02;                  //0xc0023208L
	M14B0_S_IPC_CTRL_03_T                	ipc_ctrl_03;                  //0xc002320cL
	M14B0_S_IPC_CTRL_04_T                	ipc_ctrl_04;                  //0xc0023210L
	M14B0_S_IPC_CTRL_05_T                	ipc_ctrl_05;                  //0xc0023214L
	M14B0_S_IPC_CTRL_06_T                	ipc_ctrl_06;                  //0xc0023218L
	M14B0_S_IPC_CTRL_07_T                	ipc_ctrl_07;                  //0xc002321cL
	M14B0_S_IPC_CTRL_08_T                	ipc_ctrl_08;                  //0xc0023220L
	M14B0_S_IPC_CTRL_09_T                	ipc_ctrl_09;                  //0xc0023224L
	M14B0_S_IPC_CTRL_10_T                	ipc_ctrl_10;                  //0xc0023228L
	M14B0_S_IPC_CTRL_11_T                	ipc_ctrl_11;                  //0xc002322cL
	M14B0_S_IPC_CTRL_12_T                	ipc_ctrl_12;                  //0xc0023230L
	M14B0_S_IPC_CTRL_13_T                	ipc_ctrl_13;                  //0xc0023234L
	M14B0_S_IPC_CTRL_14_T                	ipc_ctrl_14;                  //0xc0023238L
	M14B0_S_IPC_CTRL_15_T                	ipc_ctrl_15;                  //0xc002323cL
	M14B0_S_IPC_CTRL_16_T                	ipc_ctrl_16;                  //0xc0023240L
	M14B0_S_IPC_CTRL_17_T                	ipc_ctrl_17;                  //0xc0023244L
	M14B0_S_IPC_CTRL_18_T                	ipc_ctrl_18;                  //0xc0023248L
	M14B0_S_IPC_CTRL_19_T                	ipc_ctrl_19;                  //0xc002324cL
	M14B0_S_IPC_STATUS_00_T              	ipc_status_00;                //0xc0023250L
	M14B0_S_IPC_STATUS_01_T              	ipc_status_01;                //0xc0023254L
	M14B0_S_IPC_STATUS_02_T              	ipc_status_02;                //0xc0023258L
	M14B0_S_IPC_STATUS_03_T              	ipc_status_03;                //0xc002325cL
	UINT32                             	reserved02[8];                //0xc0023260L~0xc002327cL
	M14B0_S_IPC_CTRL_34_T                	ipc_ctrl_34;                  //0xc0023280L
	M14B0_S_IPC_CTRL_35_T                	ipc_ctrl_35;                  //0xc0023284L
	M14B0_S_IPC_CTRL_36_T                	ipc_ctrl_36;                  //0xc0023288L
	M14B0_S_IPC_CTRL_37_T                	ipc_ctrl_37;                  //0xc002328cL
	M14B0_S_IPC_CTRL_38_T                	ipc_ctrl_38;                  //0xc0023290L
	M14B0_S_IPC_CTRL_39_T                	ipc_ctrl_39;                  //0xc0023294L
	M14B0_S_IPC_CTRL_40_T                	ipc_ctrl_40;                  //0xc0023298L
	M14B0_S_IPC_CTRL_41_T                	ipc_ctrl_41;                  //0xc002329cL
	UINT32                             	reserved03[20];               //0xc00232a0L~0xc00232ecL
	M14B0_S_IPC_CTRL_20_T                	ipc_ctrl_20;                  //0xc00232f0L
	M14B0_S_IPC_CTRL_21_T                	ipc_ctrl_21;                  //0xc00232f4L
	M14B0_S_IPC_CTRL_22_T                	ipc_ctrl_22;                  //0xc00232f8L
	M14B0_S_IPC_CTRL_23_T                	ipc_ctrl_23;                  //0xc00232fcL
	M14B0_S_IPC_CTRL_24_T                	ipc_ctrl_24;                  //0xc0023300L
	M14B0_S_IPC_CTRL_25_T                	ipc_ctrl_25;                  //0xc0023304L
	M14B0_S_IPC_CTRL_26_T                	ipc_ctrl_26;                  //0xc0023308L
	M14B0_S_IPC_CTRL_27_T                	ipc_ctrl_27;                  //0xc002330cL
	M14B0_S_IPC_CTRL_28_T                	ipc_ctrl_28;                  //0xc0023310L
	M14B0_S_IPC_CTRL_29_T                	ipc_ctrl_29;                  //0xc0023314L
	M14B0_S_IPC_CTRL_30_T                	ipc_ctrl_30;                  //0xc0023318L
	M14B0_S_IPC_CTRL_31_T                	ipc_ctrl_31;                  //0xc002331cL
	M14B0_S_IPC_CTRL_32_T                	ipc_ctrl_32;                  //0xc0023320L
	M14B0_S_IPC_CTRL_33_T                	ipc_ctrl_33;                  //0xc0023324L
	M14B0_S_IPC_STATUS_04_T              	ipc_status_04;                //0xc0023328L
	M14B0_S_IPC_STATUS_05_T              	ipc_status_05;                //0xc002332cL
	M14B0_S_IPC_STATUS_06_T              	ipc_status_06;                //0xc0023330L
	M14B0_S_IPC_STATUS_07_T              	ipc_status_07;                //0xc0023334L
	M14B0_S_IPC_STATUS_08_T              	ipc_status_08;                //0xc0023338L
	M14B0_S_IPC_STATUS_09_T              	ipc_status_09;                //0xc002333cL
	M14B0_S_TPD_CTRL_00_T                	tpd_ctrl_00;                  //0xc0023340L
	M14B0_S_TPD_CTRL_01_T                	tpd_ctrl_01;                  //0xc0023344L
	M14B0_S_TPD_CTRL_02_T                	tpd_ctrl_02;                  //0xc0023348L
	UINT32                             	reserved04[14];               //0xc002334cL~0xc0023380L
	M14B0_S_APL_STAT_00_T                	apl_stat_00;                  //0xc0023384L
	UINT32                             	reserved05[4];                //0xc0023388L~0xc0023394L
	M14B0_S_LVCRTL_CTRL_00_T             	lvcrtl_ctrl_00;               //0xc0023398L
} DE_P0S_REG_M14B0_T; // PE0 S

#endif
