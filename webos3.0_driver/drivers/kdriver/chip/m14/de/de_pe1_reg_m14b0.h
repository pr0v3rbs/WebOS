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

#ifndef _REG_PE1_M14B0_H_
#define _REG_PE1_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0024300L pe1_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0     
	UINT32 load_enable                        : 1 ;  //     1     
	UINT32 load_type                          : 1 ;  //     2     
} M14B0_PE1_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024304L pe1_top_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 width                              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 height                             : 12;  // 27:16     
} M14B0_PE1_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024308L pe1_top_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_position                       : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 pe1_detour                         : 1 ;  //     2     
	UINT32 line_memory_detour                 : 1 ;  //     3     
	UINT32                                    : 1 ;  //     4     reserved
	UINT32 async_delay_mode                   : 1 ;  //     5     
	UINT32 ud_mode                            : 1 ;  //     6     
	UINT32 mode_3d                            : 1 ;  //     7     
	UINT32 vdpulse_pos                        : 12;  // 19: 8     
	UINT32 line_masking_mode                  : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 master_en                          : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 pmux_position                      : 1 ;  //    28     
} M14B0_PE1_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002430cL pe1_top_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt                               : 12;  // 11: 0     
	UINT32 va_pe1_in                          : 1 ;  //    12     
	UINT32 va_pe1_out                         : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 vcnt2                              : 12;  // 27:16     
} M14B0_PE1_TOP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024320L pe1_win1_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} M14B0_PE1_WIN1_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024324L pe1_win1_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} M14B0_PE1_WIN1_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024328L pe1_win1_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} M14B0_PE1_WIN1_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002432cL pe1_win1_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} M14B0_PE1_WIN1_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024330L pe1_win1_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} M14B0_PE1_WIN1_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024334L pe1_win2_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} M14B0_PE1_WIN2_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024338L pe1_win2_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} M14B0_PE1_WIN2_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc002433cL pe1_win2_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} M14B0_PE1_WIN2_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0024340L pe1_win2_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} M14B0_PE1_WIN2_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024344L pe1_win2_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} M14B0_PE1_WIN2_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024350L pe1_apl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x0                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 apl_win_ctrl_y0                    : 12;  // 27:16     
} M14B0_PE1_APL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024354L pe1_apl_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x1                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 apl_win_ctrl_y1                    : 12;  // 27:16     
} M14B0_PE1_APL_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024358L pe1_apl_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_position                       : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 apl_y_type                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 act_win_mode                       : 1 ;  //     8     
} M14B0_PE1_APL_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002435cL pe1_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_cb                             : 10;  //  9: 0     
	UINT32 apl_yg                             : 10;  // 19:10     
	UINT32 apl_cr                             : 10;  // 29:20     
} M14B0_PE1_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0024360L pe1_apl_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_cb_other_side                  : 10;  //  9: 0     
	UINT32 apl_yg_other_side                  : 10;  // 19:10     
	UINT32 apl_cr_other_side                  : 10;  // 29:20     
} M14B0_PE1_APL_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0024370L pe1_tpg_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_enable                         : 1 ;  //     0     
	UINT32 pattern_type                       : 2 ;  //  2: 1     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 blend_alpha                        : 4 ;  //  7: 4     
	UINT32 blt_size_x                         : 12;  // 19: 8     
	UINT32 blt_size_y                         : 12;  // 31:20     
} M14B0_PE1_TPG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024374L pe1_tpg_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gradation_stride                   : 7 ;  //  6: 0     
	UINT32 gradation_direction                : 1 ;  //     7     
	UINT32 gradation_bar_0_en                 : 1 ;  //     8     
	UINT32 gradation_bar_1_en                 : 1 ;  //     9     
	UINT32 gradation_bar_2_en                 : 1 ;  //    10     
	UINT32 gradation_bar_3_en                 : 1 ;  //    11     
} M14B0_PE1_TPG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024378L pe1_tpg_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 patch_0_en                         : 1 ;  //     0     
	UINT32 patch_1_en                         : 1 ;  //     1     
	UINT32 patch_2_en                         : 1 ;  //     2     
	UINT32 patch_3_en                         : 1 ;  //     3     
	UINT32 patch_4_en                         : 1 ;  //     4     
	UINT32 patch_5_en                         : 1 ;  //     5     
	UINT32 patch_6_en                         : 1 ;  //     6     
	UINT32 patch_7_en                         : 1 ;  //     7     
	UINT32 patch_8_en                         : 1 ;  //     8     
	UINT32 patch_9_en                         : 1 ;  //     9     
	UINT32 patch_10_en                        : 1 ;  //    10     
	UINT32 patch_11_en                        : 1 ;  //    11     
	UINT32 patch_12_en                        : 1 ;  //    12     
	UINT32 patch_13_en                        : 1 ;  //    13     
	UINT32 patch_14_en                        : 1 ;  //    14     
	UINT32 patch_15_en                        : 1 ;  //    15     
} M14B0_PE1_TPG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002437cL pe1_tpg_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 patch_0_inout_sel                  : 1 ;  //     0     
	UINT32 patch_1_inout_sel                  : 1 ;  //     1     
	UINT32 patch_2_inout_sel                  : 1 ;  //     2     
	UINT32 patch_3_inout_sel                  : 1 ;  //     3     
	UINT32 patch_4_inout_sel                  : 1 ;  //     4     
	UINT32 patch_5_inout_sel                  : 1 ;  //     5     
	UINT32 patch_6_inout_sel                  : 1 ;  //     6     
	UINT32 patch_7_inout_sel                  : 1 ;  //     7     
	UINT32 patch_8_inout_sel                  : 1 ;  //     8     
	UINT32 patch_9_inout_sel                  : 1 ;  //     9     
	UINT32 patch_10_inout_sel                 : 1 ;  //    10     
	UINT32 patch_11_inout_sel                 : 1 ;  //    11     
	UINT32 patch_12_inout_sel                 : 1 ;  //    12     
	UINT32 patch_13_inout_sel                 : 1 ;  //    13     
	UINT32 patch_14_inout_sel                 : 1 ;  //    14     
	UINT32 patch_15_inout_sel                 : 1 ;  //    15     
} M14B0_PE1_TPG_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024380L pe1_tpg_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 10;  //  9: 0     
	UINT32 bg_color_b                         : 10;  // 19:10     
	UINT32 bg_color_g                         : 10;  // 29:20     
} M14B0_PE1_TPG_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024384L pe1_tpg_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_indir_addr                     : 7 ;  //  6: 0     
	UINT32                                    : 5 ;  // 11: 7     reserved
	UINT32 tpg_indir_ai_en                    : 1 ;  //    12     
} M14B0_PE1_TPG_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0024388L pe1_tpg_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_indir_data                     : 32;  // 31: 0     
} M14B0_PE1_TPG_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0024390L pe1_win3_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win0_en                            : 1 ;  //     0     
	UINT32 win1_en                            : 1 ;  //     1     
	UINT32 win01_en                           : 1 ;  //     2     
	UINT32                                    : 3 ;  //  5: 3     reserved
	UINT32 win_outside                        : 1 ;  //     6     
	UINT32 win_en                             : 1 ;  //     7     
	UINT32 bdr_alpha                          : 5 ;  // 12: 8     
	UINT32 bdr_wid                            : 2 ;  // 14:13     
	UINT32 bdr_en                             : 1 ;  //    15     
	UINT32 cr5                                : 5 ;  // 20:16     
	UINT32 cb5                                : 5 ;  // 25:21     
	UINT32 yy6                                : 6 ;  // 31:26     
} M14B0_PE1_WIN3_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024394L pe1_win3_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16     
} M14B0_PE1_WIN3_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024398L pe1_win3_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16     
} M14B0_PE1_WIN3_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002439cL pe1_win3_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16     
} M14B0_PE1_WIN3_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00243a0L pe1_win3_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16     
} M14B0_PE1_WIN3_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00243b0L pe1_derh_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsum_mode                          : 2 ;  //  1: 0     
	UINT32 hsum_mode                          : 2 ;  //  3: 2     
	UINT32 vsum121_en                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 vertical_min_max                   : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 a_flt_mux                          : 2 ;  // 17:16     
	UINT32 a_e_mux                            : 2 ;  // 19:18     
	UINT32 t_mux                              : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_gen_width                        : 4 ;  // 27:24     
} M14B0_PE1_DERH_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00243b4L pe1_derh_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_gen_of_y                         : 8 ;  //  7: 0     
	UINT32 a_gen_of_c                         : 8 ;  // 15: 8     
	UINT32 avg_flt_mode                       : 3 ;  // 18:16     
	UINT32 avg_flt_en                         : 1 ;  //    19     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 max_flt_mode                       : 3 ;  // 26:24     
} M14B0_PE1_DERH_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00243b8L pe1_derh_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a_exp_gain_ctrl_y0                 : 4 ;  //  3: 0     
	UINT32 a_exp2_gain_ctrl_x0                : 4 ;  //  7: 4     
	UINT32 a_exp2_gain_ctrl_y1                : 4 ;  // 11: 8     
	UINT32 a_exp2_gain_ctrl_x1                : 4 ;  // 15:12     
	UINT32 a_avg2_flt_wid                     : 4 ;  // 19:16     
	UINT32 a_avg2_flt_en                      : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 a_exp_max2_en                      : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 y_sum_mode                         : 2 ;  // 27:26     
} M14B0_PE1_DERH_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00243bcL pe1_derh_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 5 ;  //  4: 0     reserved
	UINT32 a_mux_for_edge_gaintable           : 3 ;  //  7: 5     
	UINT32                                    : 9 ;  // 16: 8     reserved
	UINT32 a_mux_for_detail_filter            : 3 ;  // 19:17     
	UINT32                                    : 1 ;  //    20     reserved
	UINT32 a_mux_for_edge_filter              : 3 ;  // 23:21     
	UINT32 a_scaling_flat_filter_             : 4 ;  // 27:24     
	UINT32                                    : 1 ;  //    28     reserved
	UINT32 a_mux_for_flat_filter              : 3 ;  // 31:29     
} M14B0_PE1_DERH_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00243c0L pe1_derh_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 reg_dbg_scale                      : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sum_mux                            : 4 ;  //  7: 4     
	UINT32 reg_enh_en_cc                      : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_enh_en_yy                      : 3 ;  // 14:12     
} M14B0_PE1_DERH_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00243c4L pe1_derh_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vmm_param                      : 2 ;  //  1: 0     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 reg_csft_gain                      : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_th_gain_edge                   : 6 ;  // 21:16     
	UINT32 reg_th_gain_flat                   : 2 ;  // 23:22     
	UINT32 reg_th_manual_th                   : 7 ;  // 30:24     
	UINT32 reg_th_manual_en                   : 1 ;  //    31     
} M14B0_PE1_DERH_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00243c8L pe1_derh_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 edge_filter_white_gain             : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 edge_filter_black_gain             : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_amean_en                       : 1 ;  //    24     
	UINT32                                    : 1 ;  //    25     reserved
	UINT32 edge_filter_v_tap                  : 2 ;  // 27:26     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 edge_c_filter_en                   : 1 ;  //    30     
	UINT32 edge_y_filter_en                   : 1 ;  //    31     
} M14B0_PE1_DERH_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc00243ccL pe1_derh_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_filter_tap                     : 2 ;  //  1: 0     
	UINT32                                    : 22;  // 23: 2     reserved
	UINT32 flat_filter_gain                   : 6 ;  // 29:24     
	UINT32 flat_filter_type                   : 1 ;  //    30     
	UINT32 flat_filter_en                     : 1 ;  //    31     
} M14B0_PE1_DERH_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc00243d0L pe1_derh_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl0_y0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl0_x0                      : 8 ;  // 15: 8     
	UINT32 amod_ctrl0_y1                      : 8 ;  // 23:16     
	UINT32 amod_ctrl0_x1                      : 8 ;  // 31:24     
} M14B0_PE1_DERH_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc00243d4L pe1_derh_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 amod_ctrl1_x0                      : 8 ;  //  7: 0     
	UINT32 amod_ctrl1_x1                      : 8 ;  // 15: 8     
	UINT32 y_c_mux_control                    : 3 ;  // 18:16     
	UINT32 chroma_weight                      : 5 ;  // 23:19     
} M14B0_PE1_DERH_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc00243d8L pe1_derh_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 a2g_mode_e                         : 1 ;  //     0     
	UINT32 a2g_mode_f                         : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 a2g_mgain_e                        : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 a2g_mgain_f                        : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 debug_mode                         : 4 ;  // 27:24     
} M14B0_PE1_DERH_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc00243dcL pe1_derh_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_th1                         : 8 ;  //  7: 0     
	UINT32 e_gain_th2                         : 8 ;  // 15: 8     
	UINT32 f_gain_th1                         : 8 ;  // 23:16     
	UINT32 f_gain_th2                         : 8 ;  // 31:24     
} M14B0_PE1_DERH_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc00243e0L pe1_derh_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_gain_max                         : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 f_gain_max                         : 6 ;  // 13: 8     
} M14B0_PE1_DERH_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc00243e4L pe1_derh_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_DERH_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc00243f0L pe1_derv_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 der_v_en                           : 1 ;  //     0     
	UINT32 der_gain_mapping                   : 2 ;  //  2: 1     
	UINT32 bif_en                             : 1 ;  //     3     
	UINT32 output_mux                         : 4 ;  //  7: 4     
	UINT32 bif_manual_th                      : 8 ;  // 15: 8     
	UINT32 th_gain                            : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 th_mode                            : 1 ;  //    24     
} M14B0_PE1_DERV_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc00243f4L pe1_derv_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csft_gain                          : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 csft_mode                          : 1 ;  //     8     
} M14B0_PE1_DERV_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc00243f8L pe1_derv_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mmd_sel                            : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 max_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 avg_sel                            : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 gain_th1                           : 8 ;  // 23:16     
	UINT32 gain_th2                           : 8 ;  // 31:24     
} M14B0_PE1_DERV_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc00243fcL pe1_derv_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain_b                             : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 gain_w                             : 7 ;  // 14: 8     
} M14B0_PE1_DERV_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0024410L pe1_dp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debug_display                      : 3 ;  //  2: 0     
} M14B0_PE1_DP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024414L pe1_dp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} M14B0_PE1_DP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024418L pe1_dp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_DP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002441cL pe1_dp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_DP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024430L pe1_sp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} M14B0_PE1_SP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024434L pe1_sp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} M14B0_PE1_SP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024438L pe1_sp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} M14B0_PE1_SP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002443cL pe1_sp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_SP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024440L pe1_sp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} M14B0_PE1_SP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024444L pe1_sp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_SP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024448L pe1_sp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 lap_v_mode                         : 1 ;  //     4     
} M14B0_PE1_SP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002444cL pe1_sp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} M14B0_PE1_SP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0024450L pe1_sp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} M14B0_PE1_SP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0024454L pe1_sp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} M14B0_PE1_SP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0024458L pe1_sp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} M14B0_PE1_SP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc002445cL pe1_sp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} M14B0_PE1_SP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc0024460L pe1_sp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} M14B0_PE1_SP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc0024470L pe1_mp_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_enhance_enable                : 1 ;  //     0     
	UINT32                                    : 11;  // 11: 1     reserved
	UINT32 edge_operator_selection            : 2 ;  // 13:12     
} M14B0_PE1_MP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024474L pe1_mp_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 white_gain                         : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 black_gain                         : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 horizontal_gain                    : 8 ;  // 23:16     
	UINT32 vertical_gain                      : 8 ;  // 31:24     
} M14B0_PE1_MP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024478L pe1_mp_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sobel_weight                       : 8 ;  //  7: 0     
	UINT32 laplacian_weight                   : 8 ;  // 15: 8     
	UINT32 sobel_manual_mode_en               : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 sobel_manual_gain                  : 8 ;  // 31:24     
} M14B0_PE1_MP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002447cL pe1_mp_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_MP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024480L pe1_mp_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 display_mode                       : 4 ;  // 11: 8     
	UINT32 gx_weight_manual_mode              : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 gx_weight_manual_gain              : 8 ;  // 23:16     
} M14B0_PE1_MP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024484L pe1_mp_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_MP_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024488L pe1_mp_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lap_h_mode                         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 lap_v_mode                         : 3 ;  //  6: 4     
} M14B0_PE1_MP_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002448cL pe1_mp_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 gb_mode                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 gb_x1                              : 8 ;  // 15: 8     
	UINT32 gb_y1                              : 8 ;  // 23:16     
} M14B0_PE1_MP_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0024490L pe1_mp_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gb_x2                              : 8 ;  //  7: 0     
	UINT32 gb_y2                              : 8 ;  // 15: 8     
	UINT32 gb_y3                              : 8 ;  // 23:16     
} M14B0_PE1_MP_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0024494L pe1_mp_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_x_l0                          : 8 ;  //  7: 0     
	UINT32 lum1_x_l1                          : 8 ;  // 15: 8     
	UINT32 lum1_x_h0                          : 8 ;  // 23:16     
	UINT32 lum1_x_h1                          : 8 ;  // 31:24     
} M14B0_PE1_MP_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0024498L pe1_mp_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum1_y0                            : 8 ;  //  7: 0     
	UINT32 lum1_y1                            : 8 ;  // 15: 8     
	UINT32 lum1_y2                            : 8 ;  // 23:16     
	UINT32 lum2_x_l0                          : 8 ;  // 31:24     
} M14B0_PE1_MP_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc002449cL pe1_mp_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_x_l1                          : 8 ;  //  7: 0     
	UINT32 lum2_x_h0                          : 8 ;  // 15: 8     
	UINT32 lum2_x_h1                          : 8 ;  // 23:16     
	UINT32 lum2_y0                            : 8 ;  // 31:24     
} M14B0_PE1_MP_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc00244a0L pe1_mp_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lum2_y1                            : 8 ;  //  7: 0     
	UINT32 lum2_y2                            : 8 ;  // 15: 8     
} M14B0_PE1_MP_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc00244b0L pe1_coring_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ga_max                             : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 ga_th0                             : 8 ;  // 15: 8     
	UINT32 ga_th1                             : 8 ;  // 23:16     
	UINT32 amap1_sel                          : 2 ;  // 25:24     
	UINT32 amap2_sel                          : 2 ;  // 27:26     
	UINT32 edge_a_sel                         : 1 ;  //    28     
	UINT32 debug_a_sel                        : 1 ;  //    29     
} M14B0_PE1_CORING_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00244b4L pe1_coring_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_sel                            : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 avg_sel                            : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 a2tw_min                           : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 a2tw_th0                           : 8 ;  // 23:16     
	UINT32 a2tw_th1                           : 8 ;  // 31:24     
} M14B0_PE1_CORING_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00244b8L pe1_coring_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nrv_en                             : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 nrv_th1                            : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 nrv_th2                            : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 var_h_th                           : 8 ;  // 31:24     
} M14B0_PE1_CORING_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00244bcL pe1_coring_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 nrh_en                             : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 nrh_th1                            : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 nrh_th2                            : 6 ;  // 21:16     
} M14B0_PE1_CORING_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00244c0L pe1_coring_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dflt_sel                           : 2 ;  //  1: 0     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 var_v_th                           : 8 ;  // 15: 8     
	UINT32 coring_mode1                       : 2 ;  // 17:16     
	UINT32 coring_mode2                       : 2 ;  // 19:18     
	UINT32 ac_direction                       : 1 ;  //    20     
	UINT32 tflt_en                            : 1 ;  //    21     
	UINT32 exp_mode                           : 2 ;  // 23:22     
	UINT32 tmap_sc_var_th                     : 8 ;  // 31:24     
} M14B0_PE1_CORING_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00244c4L pe1_coring_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gt_max                             : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 gt_th0                             : 8 ;  // 15: 8     
	UINT32 gt_th1                             : 8 ;  // 23:16     
	UINT32 a2tw_en                            : 1 ;  //    24     
} M14B0_PE1_CORING_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00244c8L pe1_coring_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gt_th0a                            : 8 ;  //  7: 0     
	UINT32 gt_th0b                            : 8 ;  // 15: 8     
	UINT32 gt_gain0a                          : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 gt_gain0b                          : 6 ;  // 29:24     
} M14B0_PE1_CORING_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc00244ccL pe1_coring_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 g_th0                              : 8 ;  //  7: 0     
	UINT32 g_th1                              : 8 ;  // 15: 8     
	UINT32 amap_gain                          : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 tmap_gain                          : 7 ;  // 30:24     
} M14B0_PE1_CORING_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc00244d0L pe1_coring_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mp_coring_mode                     : 2 ;  //  1: 0     
	UINT32 sp_coring_mode                     : 2 ;  //  3: 2     
	UINT32 mp_coring_en                       : 1 ;  //     4     
	UINT32 sp_coring_en                       : 1 ;  //     5     
} M14B0_PE1_CORING_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc00244d4L pe1_coring_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edge_gain_b                        : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 edge_gain_w                        : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 texture_gain_b                     : 7 ;  // 22:16     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 texture_gain_w                     : 7 ;  // 30:24     
} M14B0_PE1_CORING_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc00244e0L pe1_coring_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tm_h                               : 16;  // 15: 0     
	UINT32 tm_v                               : 16;  // 31:16     
} M14B0_PE1_CORING_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc00244e4L pe1_coring_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tm_h                               : 16;  // 15: 0     
} M14B0_PE1_CORING_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc00244e8L pe1_coring_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_CORING_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc00244ecL pe1_coring_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_CORING_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc0024500L pe1_ti_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32 debug_coring                       : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 coring_step                        : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 coring_th                          : 8 ;  // 15: 8     
	UINT32 y_gain                             : 8 ;  // 23:16     
	UINT32 c_gain                             : 8 ;  // 31:24     
} M14B0_PE1_TI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0024504L pe1_ti_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain0_en                           : 1 ;  //     0     
	UINT32 gain1_en                           : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 output_mux                         : 2 ;  //  5: 4     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 gain0_th0                          : 8 ;  // 23:16     
	UINT32 gain0_th1                          : 8 ;  // 31:24     
} M14B0_PE1_TI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0024508L pe1_ti_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gain1_div_mode                     : 2 ;  //  1: 0     
} M14B0_PE1_TI_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc0024510L pe1_dce_blur_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 avg_filter_tap                     : 2 ;  //  3: 2     
	UINT32                                    : 2 ;  //  5: 4     reserved
	UINT32 map_blend_mode                     : 2 ;  //  7: 6     
} M14B0_PE1_DCE_BLUR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024514L pe1_dce_blur_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sc_map_sel                         : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 sc_amap_filter_tap                 : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 amap_gain                          : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 tmap_gain                          : 5 ;  // 20:16     
} M14B0_PE1_DCE_BLUR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024518L pe1_dce_blur_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 e_to_w_th_x0                       : 8 ;  //  7: 0     
	UINT32 e_to_w_th_x1                       : 8 ;  // 15: 8     
	UINT32 e_to_w_th_y0                       : 8 ;  // 23:16     
	UINT32 e_to_w_th_y1                       : 8 ;  // 31:24     
} M14B0_PE1_DCE_BLUR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002451cL pe1_dce_blur_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_blur_mode                   : 2 ;  //  1: 0     
} M14B0_PE1_DCE_BLUR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024520L pe1_dj_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 edf_en                             : 1 ;  //     0     
	UINT32 hv_filter_en                       : 1 ;  //     1     
	UINT32 g0_feature_norm_en                 : 1 ;  //     2     
	UINT32 g0_f2g_mapping_en                  : 1 ;  //     3     
	UINT32 line_variation_mode                : 1 ;  //     4     
	UINT32 l_type_protection                  : 1 ;  //     5     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 direction_type_mode                : 1 ;  //     7     
	UINT32 count_diff_th                      : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 output_mux                         : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 center_blur_mode                   : 2 ;  // 21:20     
	UINT32 n_avg_mode                         : 1 ;  //    22     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 line_variation_diff_threshold      : 8 ;  // 31:24     
} M14B0_PE1_DJ_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024524L pe1_dj_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 level_th                           : 8 ;  //  7: 0     
	UINT32 protect_th                         : 8 ;  // 15: 8     
	UINT32 n_avg_gain                         : 8 ;  // 23:16     
} M14B0_PE1_DJ_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024528L pe1_dj_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g0_cnt_min                     : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_g0_mul                         : 5 ;  // 12: 8     
} M14B0_PE1_DJ_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002452cL pe1_dj_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_g1_protect_min                 : 8 ;  //  7: 0     
	UINT32 reg_g1_mul                         : 7 ;  // 14: 8     
} M14B0_PE1_DJ_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024530L pe1_cti_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cti_en                             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 filter_tap_size                    : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 cti_gain                           : 8 ;  // 15: 8     
} M14B0_PE1_CTI_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024534L pe1_cti_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 coring_th0                         : 8 ;  //  7: 0     
	UINT32 coring_th1                         : 8 ;  // 15: 8     
	UINT32 coring_map_filter                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 coring_tap_size                    : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 debug_mode                         : 2 ;  // 25:24     
} M14B0_PE1_CTI_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024538L pe1_cti_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ycm_en0                            : 1 ;  //     0     
	UINT32 ycm_en1                            : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 ycm_band_sel                       : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 ycm_diff_th                        : 8 ;  // 15: 8     
	UINT32 ycm_y_gain                         : 4 ;  // 19:16     
	UINT32 ycm_c_gain                         : 4 ;  // 23:20     
} M14B0_PE1_CTI_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0024540L pe1_pmux_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} M14B0_PE1_PMUX_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024544L pe1_pmux_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_main_window_h_en               : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_main_window_v_en               : 1 ;  //    31     
} M14B0_PE1_PMUX_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024548L pe1_pmux_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_PMUX_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002454cL pe1_pmux_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_sub_window_h_en                : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_sub_window_v_en                : 1 ;  //    31     
} M14B0_PE1_PMUX_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024550L pe1_pmux_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_PMUX_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024554L pe1_pmux_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_cr                        : 8 ;  //  7: 0     
	UINT32 bg_color_cb                        : 8 ;  // 15: 8     
	UINT32 bg_color_y                         : 8 ;  // 23:16     
} M14B0_PE1_PMUX_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024560L pe1_vspyc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32 graycolor_enable                   : 1 ;  //     1     
	UINT32 color_only_enable                  : 1 ;  //     2     
	UINT32 cb_info                            : 1 ;  //     3     
	UINT32 cr_info                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 center_position                    : 8 ;  // 15: 8     
	UINT32 contrast                           : 10;  // 25:16     
} M14B0_PE1_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024564L pe1_vspyc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 saturation                         : 8 ;  // 15: 8     
	UINT32 brightness                         : 10;  // 25:16     
} M14B0_PE1_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024570L pe1_debug_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable_debug_mode                  : 1 ;  //     0     
	UINT32 status_enable                      : 1 ;  //     1     
	UINT32 debug_mode                         : 1 ;  //     2     
	UINT32 show_dce_blur                      : 1 ;  //     3     
	UINT32                                    : 5 ;  //  8: 4     reserved
	UINT32 show_sqm2                          : 1 ;  //     9     
	UINT32 sqm2_debug_image                   : 4 ;  // 13:10     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 show_bbd                           : 2 ;  // 16:15     
	UINT32 bbd_line_width                     : 3 ;  // 19:17     
	UINT32 show_apl                           : 1 ;  //    20     
} M14B0_PE1_DEBUG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024574L pe1_debug_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 force_yy_enable                    : 1 ;  //     1     
	UINT32 force_cb_enable                    : 1 ;  //     2     
	UINT32 force_cr_enable                    : 1 ;  //     3     
	UINT32 yc_debug_mode_sel                  : 3 ;  //  6: 4     
} M14B0_PE1_DEBUG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024578L pe1_debug_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 10;  //  9: 0     reserved
	UINT32 force_cb                           : 10;  // 19:10     
	UINT32 force_cr                           : 10;  // 29:20     
} M14B0_PE1_DEBUG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc0024580L pe1_ocsc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1                        : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef0                        : 15;  // 30:16     
} M14B0_PE1_OCSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024584L pe1_ocsc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3                        : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef2                        : 15;  // 30:16     
} M14B0_PE1_OCSC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024588L pe1_ocsc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5                        : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef4                        : 15;  // 30:16     
} M14B0_PE1_OCSC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002458cL pe1_ocsc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7                        : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef6                        : 15;  // 30:16     
} M14B0_PE1_OCSC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024590L pe1_ocsc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_csc_coef8                        : 15;  // 30:16     
} M14B0_PE1_OCSC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024594L pe1_ocsc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_csc_ofst1                        : 11;  // 26:16     
} M14B0_PE1_OCSC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024598L pe1_ocsc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4                        : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 r_csc_ofst3                        : 14;  // 29:16     
} M14B0_PE1_OCSC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002459cL pe1_ocsc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outputcsc_en                       : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 r_csc_ofst5                        : 14;  // 29:16     
} M14B0_PE1_OCSC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc00245a0L pe1_cen_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cen_detour                         : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 select_hsv                         : 1 ;  //     2     
	UINT32 select_rgb                         : 1 ;  //     3     
	UINT32                                    : 1 ;  //     4     reserved
	UINT32 vsp_sel                            : 1 ;  //     5     
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 reg_cen_bypass                     : 1 ;  //     7     
	UINT32 reg_cen_debug_mode                 : 1 ;  //     8     
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 core_1st__gain_disable             : 1 ;  //    10     
	UINT32 core_2nd_gain_disable              : 1 ;  //    11     
	UINT32                                    : 1 ;  //    12     reserved
	UINT32 debugging_mode_core                : 2 ;  // 14:13     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 demo_mode                          : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 win_control_mode                   : 1 ;  //    31     
} M14B0_PE1_CEN_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00245a4L pe1_cen_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 show_color_region0                 : 1 ;  //     0     
	UINT32 show_color_region1                 : 1 ;  //     1     
	UINT32 show_color_region2                 : 1 ;  //     2     
	UINT32 show_color_region3                 : 1 ;  //     3     
	UINT32 show_color_region4                 : 1 ;  //     4     
	UINT32 show_color_region5                 : 1 ;  //     5     
	UINT32 show_color_region6                 : 1 ;  //     6     
	UINT32 show_color_region7                 : 1 ;  //     7     
	UINT32 show_color_region8                 : 1 ;  //     8     
	UINT32 show_color_region9                 : 1 ;  //     9     
	UINT32 show_color_region10                : 1 ;  //    10     
	UINT32 show_color_region11                : 1 ;  //    11     
	UINT32 show_color_region12                : 1 ;  //    12     
	UINT32 show_color_region13                : 1 ;  //    13     
	UINT32 show_color_region14                : 1 ;  //    14     
	UINT32 show_color_region15                : 1 ;  //    15     
	UINT32 color_region_en0                   : 1 ;  //    16     
	UINT32 color_region_en1                   : 1 ;  //    17     
	UINT32 color_region_en2                   : 1 ;  //    18     
	UINT32 color_region_en3                   : 1 ;  //    19     
	UINT32 color_region_en4                   : 1 ;  //    20     
	UINT32 color_region_en5                   : 1 ;  //    21     
	UINT32 color_region_en6                   : 1 ;  //    22     
	UINT32 color_region_en7                   : 1 ;  //    23     
	UINT32 color_region_en8                   : 1 ;  //    24     
	UINT32 color_region_en9                   : 1 ;  //    25     
	UINT32 color_region_en10                  : 1 ;  //    26     
	UINT32 color_region_en11                  : 1 ;  //    27     
	UINT32 color_region_en12                  : 1 ;  //    28     
	UINT32 color_region_en13                  : 1 ;  //    29     
	UINT32 color_region_en14                  : 1 ;  //    30     
	UINT32 color_region_en15                  : 1 ;  //    31     
} M14B0_PE1_CEN_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00245a8L pe1_cen_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ihsv_sgain                         : 8 ;  //  7: 0     
	UINT32 ihsv_vgain                         : 8 ;  // 15: 8     
} M14B0_PE1_CEN_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00245acL pe1_cen_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ihsv_hoffset                       : 8 ;  //  7: 0     
	UINT32 ihsv_soffset                       : 8 ;  // 15: 8     
	UINT32 ihsv_voffset                       : 8 ;  // 23:16     
} M14B0_PE1_CEN_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00245b0L pe1_cen_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr0                : 8 ;  //  7: 0     
	UINT32 reg_master_gain_cr1                : 8 ;  // 15: 8     
	UINT32 reg_master_gain_cr2                : 8 ;  // 23:16     
	UINT32 reg_master_gain_cr3                : 8 ;  // 31:24     
} M14B0_PE1_CEN_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00245b4L pe1_cen_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr4                : 8 ;  //  7: 0     
	UINT32 reg_master_gain_cr5                : 8 ;  // 15: 8     
	UINT32 reg_master_gain_cr6                : 8 ;  // 23:16     
	UINT32 reg_master_gain_cr7                : 8 ;  // 31:24     
} M14B0_PE1_CEN_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00245b8L pe1_cen_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr8                : 8 ;  //  7: 0     
	UINT32 reg_master_gain_cr9                : 8 ;  // 15: 8     
	UINT32 reg_master_gain_cr10               : 8 ;  // 23:16     
	UINT32 reg_master_gain_cr11               : 8 ;  // 31:24     
} M14B0_PE1_CEN_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc00245bcL pe1_cen_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr12               : 8 ;  //  7: 0     
	UINT32 reg_master_gain_cr13               : 8 ;  // 15: 8     
	UINT32 reg_master_gain_cr14               : 8 ;  // 23:16     
	UINT32 reg_master_gain_cr15               : 8 ;  // 31:24     
} M14B0_PE1_CEN_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc00245d0L pe1_cen_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_address                    : 8 ;  //  7: 0     
	UINT32 hif_cen_ai_sel                     : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 hif_cen_ai                         : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_cen_enable                     : 1 ;  //    15     
} M14B0_PE1_CEN_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00245d4L pe1_cen_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_y_wdata                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_cen_x_wdata                    : 10;  // 25:16     
} M14B0_PE1_CEN_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00245d8L pe1_cen_delta_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_delta_address              : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_cen_delta_ai                   : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_cen_delta_enable               : 1 ;  //    15     
} M14B0_PE1_CEN_DELTA_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00245dcL pe1_cen_delta_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_vr_data                    : 8 ;  //  7: 0     
	UINT32 hif_cen_sb_data                    : 8 ;  // 15: 8     
	UINT32 hif_cen_hg_data                    : 8 ;  // 23:16     
} M14B0_PE1_CEN_DELTA_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00245e0L pe1_dce_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dynamic_contrast_en                : 1 ;  //     0     
	UINT32 hist_bin_mode                      : 2 ;  //  2: 1     
	UINT32 hist_clear                         : 1 ;  //     3     
	UINT32 window01_enable                    : 1 ;  //     4     
	UINT32 window0_enable                     : 1 ;  //     5     
	UINT32 window1_enable                     : 1 ;  //     6     
	UINT32 window_acquire                     : 1 ;  //     7     
	UINT32 color_region0_sel                  : 1 ;  //     8     
	UINT32 color_region1_sel                  : 1 ;  //     9     
	UINT32 color_region2_sel                  : 1 ;  //    10     
	UINT32 color_region3_sel                  : 1 ;  //    11     
	UINT32 color_region4_sel                  : 1 ;  //    12     
	UINT32 color_region5_sel                  : 1 ;  //    13     
	UINT32 color_region6_sel                  : 1 ;  //    14     
	UINT32 color_region7_sel                  : 1 ;  //    15     
	UINT32 color_region8_sel                  : 1 ;  //    16     
	UINT32 color_region9_sel                  : 1 ;  //    17     
	UINT32 color_region10_sel                 : 1 ;  //    18     
	UINT32 color_region11_sel                 : 1 ;  //    19     
	UINT32 color_region12_sel                 : 1 ;  //    20     
	UINT32 color_region13_sel                 : 1 ;  //    21     
	UINT32 color_region14_sel                 : 1 ;  //    22     
	UINT32 color_region15_sel                 : 1 ;  //    23     
	UINT32 dce_domain_sel                     : 1 ;  //    24     
	UINT32 win_selection                      : 1 ;  //    25     
	UINT32 window_mode_enable                 : 1 ;  //    26     
	UINT32 window_inout_sel                   : 1 ;  //    27     
	UINT32 histogram_mode                     : 2 ;  // 29:28     
} M14B0_PE1_DCE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00245e4L pe1_dce_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_bin_min_position              : 8 ;  //  7: 0     
	UINT32 hist_bin_max_position              : 8 ;  // 15: 8     
	UINT32 color_region_gain                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 blur_v_gain                        : 4 ;  // 31:28     
} M14B0_PE1_DCE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00245e8L pe1_dce_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 color_region_en                    : 1 ;  //     0     
	UINT32 color_debug_en                     : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 y_grad_gain                        : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 cb_grad_gain                       : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 cr_grad_gain                       : 2 ;  // 13:12     
} M14B0_PE1_DCE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00245ecL pe1_dce_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_range_min                        : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 y_range_max                        : 10;  // 25:16     
} M14B0_PE1_DCE_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00245f0L pe1_dce_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_range_min                       : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 cb_range_max                       : 10;  // 25:16     
} M14B0_PE1_DCE_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00245f4L pe1_dce_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_range_min                       : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 cr_range_max                       : 10;  // 25:16     
} M14B0_PE1_DCE_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00245f8L pe1_dce_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_address                    : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 hif_dce_load                       : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 hif_dce_ai                         : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_dce_enable                     : 1 ;  //    15     
} M14B0_PE1_DCE_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00245fcL pe1_dce_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_wdata_y                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dyc_wdata_x                    : 10;  // 25:16     
} M14B0_PE1_DCE_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0024600L pe1_dce_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_wdata_y_32nd               : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dyc_wdata_x_32nd               : 10;  // 25:16     
} M14B0_PE1_DCE_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0024604L pe1_dce_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 dce_ktd_csc_sel                    : 1 ;  //     1     
} M14B0_PE1_DCE_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0024608L pe1_dce_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 5 ;  //  4: 0     
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32 hif_status_address                 : 3 ;  // 18:16     
	UINT32                                    : 9 ;  // 27:19     reserved
	UINT32 hif_status_ai                      : 1 ;  //    28     
	UINT32 hif_mask                           : 1 ;  //    29     
} M14B0_PE1_DCE_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc002460cL pe1_dce_hist_ia_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_DCE_HIST_IA_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc0024610L pe1_dce_hist_ia_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_DCE_HIST_IA_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc0024620L pe1_dse_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dynamic_saturation_en              : 1 ;  //     0     
	UINT32 window_mode_enable                 : 1 ;  //     1     
	UINT32 window_inout_sel                   : 1 ;  //     2     
	UINT32 win_selection                      : 1 ;  //     3     
	UINT32 window01_enable                    : 1 ;  //     4     
	UINT32 window0_enable                     : 1 ;  //     5     
	UINT32 window1_enable                     : 1 ;  //     6     
	UINT32 window_acquire                     : 1 ;  //     7     
	UINT32 color_region0_sel                  : 1 ;  //     8     
	UINT32 color_region1_sel                  : 1 ;  //     9     
	UINT32 color_region2_sel                  : 1 ;  //    10     
	UINT32 color_region3_sel                  : 1 ;  //    11     
	UINT32 color_region4_sel                  : 1 ;  //    12     
	UINT32 color_region5_sel                  : 1 ;  //    13     
	UINT32 color_region6_sel                  : 1 ;  //    14     
	UINT32 color_region7_sel                  : 1 ;  //    15     
	UINT32 color_region8_sel                  : 1 ;  //    16     
	UINT32 color_region9_sel                  : 1 ;  //    17     
	UINT32 color_region10_sel                 : 1 ;  //    18     
	UINT32 color_region11_sel                 : 1 ;  //    19     
	UINT32 color_region12_sel                 : 1 ;  //    20     
	UINT32 color_region13_sel                 : 1 ;  //    21     
	UINT32 color_region14_sel                 : 1 ;  //    22     
	UINT32 color_region15_sel                 : 1 ;  //    23     
} M14B0_PE1_DSE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024624L pe1_dse_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 color_region_gain                  : 8 ;  // 31:24     
} M14B0_PE1_DSE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024628L pe1_dse_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_wdata_y_32nd               : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dse_wdata_x_32nd               : 10;  // 25:16     
} M14B0_PE1_DSE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002462cL pe1_dse_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_address                    : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 hif_dse_load                       : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 hif_dse_ai                         : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_dse_enable                     : 1 ;  //    15     
} M14B0_PE1_DSE_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0024630L pe1_dse_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_wdata_y                    : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dse_wdata_x                    : 10;  // 25:16     
} M14B0_PE1_DSE_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0024640L pe1_cw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cw_en                              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 cw_gain_selection                  : 1 ;  //     4     
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 reg_cw_y4                          : 8 ;  // 23:16     
	UINT32 reg_cw_x4                          : 8 ;  // 31:24     
} M14B0_PE1_CW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024644L pe1_cw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 user_ctrl_g_gain                   : 8 ;  //  7: 0     
	UINT32 user_ctrl_b_gain                   : 8 ;  // 15: 8     
	UINT32 user_ctrl_r_gain                   : 8 ;  // 23:16     
} M14B0_PE1_CW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024648L pe1_cw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cw_y2                          : 8 ;  //  7: 0     
	UINT32 reg_cw_x2                          : 8 ;  // 15: 8     
	UINT32 reg_cw_y3                          : 8 ;  // 23:16     
	UINT32 reg_cw_x3                          : 8 ;  // 31:24     
} M14B0_PE1_CW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002464cL pe1_cw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cw_y0                          : 8 ;  //  7: 0     
	UINT32 reg_cw_x0                          : 8 ;  // 15: 8     
	UINT32 reg_cw_y1                          : 8 ;  // 23:16     
	UINT32 reg_cw_x1                          : 8 ;  // 31:24     
} M14B0_PE1_CW_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024650L pe1_cw_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 color_region0_sel                  : 1 ;  //     0     
	UINT32 color_region1_sel                  : 1 ;  //     1     
	UINT32 color_region2_sel                  : 1 ;  //     2     
	UINT32 color_region3_sel                  : 1 ;  //     3     
	UINT32 color_region4_sel                  : 1 ;  //     4     
	UINT32 color_region5_sel                  : 1 ;  //     5     
	UINT32 color_region6_sel                  : 1 ;  //     6     
	UINT32 color_region7_sel                  : 1 ;  //     7     
	UINT32 color_region8_sel                  : 1 ;  //     8     
	UINT32 color_region9_sel                  : 1 ;  //     9     
	UINT32 color_region10_sel                 : 1 ;  //    10     
	UINT32 color_region11_sel                 : 1 ;  //    11     
	UINT32 color_region12_sel                 : 1 ;  //    12     
	UINT32 color_region13_sel                 : 1 ;  //    13     
	UINT32 color_region14_sel                 : 1 ;  //    14     
	UINT32 color_region15_sel                 : 1 ;  //    15     
	UINT32 color_region_gain                  : 8 ;  // 23:16     
} M14B0_PE1_CW_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024660L pe1_conv_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 conv_3d_r_en                       : 1 ;  //     0     
	UINT32 fpacking_r_en                      : 1 ;  //     1     
	UINT32 ffmode_r_en                        : 1 ;  //     2     
	UINT32 load_parity_r                      : 1 ;  //     3     
	UINT32 conv_3d_l_en                       : 1 ;  //     4     
	UINT32 fpacking_l_en                      : 1 ;  //     5     
	UINT32 ffmode_l_en                        : 1 ;  //     6     
	UINT32 load_parity_l                      : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 start_parity_r                     : 1 ;  //    16     
	UINT32 start_parity_l                     : 1 ;  //    17     
} M14B0_PE1_CONV_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024664L pe1_conv_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_r                           : 7 ;  //  6: 0     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 offset_l                           : 7 ;  // 22:16     
} M14B0_PE1_CONV_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024668L pe1_conv_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_grey                      : 8 ;  //  7: 0     
} M14B0_PE1_CONV_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002466cL pe1_conv_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16     
	UINT32 pxl_rep_in                         : 1 ;  //    28     
	UINT32 pxl_rep_out                        : 1 ;  //    29     
} M14B0_PE1_CONV_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024670L pe1_conv_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16     
} M14B0_PE1_CONV_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024674L pe1_conv_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_CONV_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024678L pe1_conv_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_CONV_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002467cL pe1_conv_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0     
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1     
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2     
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3     
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4     
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5     
} M14B0_PE1_CONV_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0024680L pe1_conv_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16     
	UINT32 pxl_rep_in                         : 1 ;  //    28     
	UINT32 pxl_rep_out                        : 1 ;  //    29     
} M14B0_PE1_CONV_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0024684L pe1_conv_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16     
} M14B0_PE1_CONV_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0024688L pe1_conv_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_CONV_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc002468cL pe1_conv_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_CONV_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc0024690L pe1_conv_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0     
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1     
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2     
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3     
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4     
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5     
} M14B0_PE1_CONV_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc0024694L pe1_conv_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32 load_parity                        : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} M14B0_PE1_CONV_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc0024698L pe1_conv_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_l_window_h_en                  : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_l_window_v_en                  : 1 ;  //    31     
} M14B0_PE1_CONV_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc002469cL pe1_conv_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_CONV_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc00246a0L pe1_conv_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_r_window_h_en                  : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_r_window_v_en                  : 1 ;  //    31     
} M14B0_PE1_CONV_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc00246a4L pe1_conv_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_CONV_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc00246a8L pe1_conv_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 8 ;  //  7: 0     
	UINT32 bg_color_b                         : 8 ;  // 15: 8     
	UINT32 bg_color_g                         : 8 ;  // 23:16     
} M14B0_PE1_CONV_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc00246b0L pe1_mute_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16     
	UINT32 pxl_rep_in                         : 1 ;  //    28     
	UINT32 pxl_rep_out                        : 1 ;  //    29     
} M14B0_PE1_MUTE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00246b4L pe1_mute_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16     
} M14B0_PE1_MUTE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00246b8L pe1_mute_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_MUTE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00246bcL pe1_mute_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_MUTE_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00246c0L pe1_mute_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0     
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1     
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2     
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3     
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4     
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5     
} M14B0_PE1_MUTE_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00246c4L pe1_mute_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16     
	UINT32 pxl_rep_in                         : 1 ;  //    28     
	UINT32 pxl_rep_out                        : 1 ;  //    29     
} M14B0_PE1_MUTE_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00246c8L pe1_mute_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16     
} M14B0_PE1_MUTE_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc00246ccL pe1_mute_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_MUTE_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc00246d0L pe1_mute_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0     
	UINT32 pxl_rep_value_b                    : 10;  // 19:10     
	UINT32 pxl_rep_value_r                    : 10;  // 29:20     
} M14B0_PE1_MUTE_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc00246d4L pe1_mute_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0     
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1     
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2     
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3     
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4     
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5     
} M14B0_PE1_MUTE_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc00246d8L pe1_mute_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4     
	UINT32 start_parity                       : 1 ;  //     6     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16     
	UINT32 alpha0_gain                        : 8 ;  // 31:24     
} M14B0_PE1_MUTE_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc00246dcL pe1_mute_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_l_window_h_en                  : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_l_window_v_en                  : 1 ;  //    31     
} M14B0_PE1_MUTE_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc00246e0L pe1_mute_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_MUTE_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc00246e4L pe1_mute_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_r_window_h_en                  : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_r_window_v_en                  : 1 ;  //    31     
} M14B0_PE1_MUTE_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc00246e8L pe1_mute_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_MUTE_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc00246ecL pe1_mute_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 8 ;  //  7: 0     
	UINT32 bg_color_b                         : 8 ;  // 15: 8     
	UINT32 bg_color_g                         : 8 ;  // 23:16     
} M14B0_PE1_MUTE_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc0024700L pe1_sqm_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outmux_debugmap                    : 4 ;  //  3: 0     
	UINT32 iir                                : 4 ;  //  7: 4     
	UINT32 i_score                            : 8 ;  // 15: 8     
	UINT32 v_score                            : 8 ;  // 23:16     
	UINT32 l_score                            : 8 ;  // 31:24     
} M14B0_PE1_SQM_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024704L pe1_sqm_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_limit                            : 8 ;  //  7: 0     
	UINT32 l_limit                            : 8 ;  // 15: 8     
	UINT32 score_threshold                    : 8 ;  // 23:16     
	UINT32 flat_threshold                     : 8 ;  // 31:24     
} M14B0_PE1_SQM_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024708L pe1_sqm_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_x0                             : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 win_y0                             : 11;  // 26:16     
} M14B0_PE1_SQM_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002470cL pe1_sqm_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_x1                             : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 win_y1                             : 11;  // 26:16     
} M14B0_PE1_SQM_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024710L pe1_sqm_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 charcter_delta_limit               : 8 ;  //  7: 0     
	UINT32 boundary_width                     : 8 ;  // 15: 8     
	UINT32 video_height                       : 8 ;  // 23:16     
} M14B0_PE1_SQM_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024714L pe1_sqm_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pc_threshold                       : 8 ;  //  7: 0     
	UINT32 pc_delta_limit                     : 8 ;  // 15: 8     
	UINT32 pc_pixel_minimal_limit             : 8 ;  // 23:16     
} M14B0_PE1_SQM_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024718L pe1_sqm_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blur_pixel                         : 10;  //  9: 0     
	UINT32 sharp_pixel                        : 10;  // 19:10     
	UINT32 sqm_output                         : 10;  // 29:20     
	UINT32 pc_detect                          : 1 ;  //    30     
} M14B0_PE1_SQM_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0024720L pe1_bbd2d_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0     
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1     
	UINT32 reg_op_mode                        : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cnt_th                         : 8 ;  // 15: 8     
	UINT32 reg_diff_th                        : 8 ;  // 23:16     
} M14B0_PE1_BBD2D_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024724L pe1_bbd2d_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x0                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y0                             : 12;  // 27:16     
} M14B0_PE1_BBD2D_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024728L pe1_bbd2d_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x1                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y1                             : 12;  // 27:16     
} M14B0_PE1_BBD2D_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002472cL pe1_bbd2d_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} M14B0_PE1_BBD2D_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0024730L pe1_bbd2d_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_BBD2D_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0024740L pe1_bbd3d_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bbd_en                         : 1 ;  //     0     
	UINT32 reg_hys_mode                       : 5 ;  //  5: 1     
	UINT32 reg_op_mode                        : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 cnt_th                             : 8 ;  // 15: 8     
	UINT32 diff_th                            : 8 ;  // 23:16     
} M14B0_PE1_BBD3D_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024744L pe1_bbd3d_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x0                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y0                             : 12;  // 27:16     
	UINT32 bbd_read_control                   : 2 ;  // 29:28     
} M14B0_PE1_BBD3D_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024748L pe1_bbd3d_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bbd_x1                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 bbd_y1                             : 12;  // 27:16     
} M14B0_PE1_BBD3D_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002474cL pe1_bbd3d_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x                                  : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y                                  : 12;  // 27:16     
} M14B0_PE1_BBD3D_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0024760L pe1_lrcr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  //     0     
	UINT32 adjust_en                          : 1 ;  //     1     
	UINT32 protection_en                      : 1 ;  //     2     
	UINT32 adjust_en2                         : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 ff_mode_en                         : 1 ;  //     8     
	UINT32 ff_mode_parity                     : 1 ;  //     9     
	UINT32 ll_mode_enable                     : 1 ;  //    10     
	UINT32 line_parity_select                 : 1 ;  //    11     
	UINT32 lrcr_out_mux                       : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_lrcr_gain                      : 4 ;  // 19:16     
	UINT32 hist_rd_en                         : 1 ;  //    20     
	UINT32                                    : 1 ;  //    21     reserved
	UINT32 hist_mode                          : 2 ;  // 23:22     
} M14B0_PE1_LRCR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024764L pe1_lrcr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 th_max_hist                        : 8 ;  //  7: 0     
	UINT32 th_valid_bins                      : 8 ;  // 15: 8     
	UINT32 th1                                : 8 ;  // 23:16     
	UINT32 th0                                : 8 ;  // 31:24     
} M14B0_PE1_LRCR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024768L pe1_lrcr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 hist_l_window_h_bbd                : 1 ;  //    14     
	UINT32 hist_l_window_h_en                 : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 hist_l_window_v_bbd                : 1 ;  //    30     
	UINT32 hist_l_window_v_en                 : 1 ;  //    31     
} M14B0_PE1_LRCR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002476cL pe1_lrcr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_LRCR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024770L pe1_lrcr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 hist_r_window_h_bbd                : 1 ;  //    14     
	UINT32 hist_r_window_h_en                 : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 hist_r_window_v_bbd                : 1 ;  //    30     
	UINT32 hist_r_window_v_en                 : 1 ;  //    31     
} M14B0_PE1_LRCR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024774L pe1_lrcr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_LRCR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024778L pe1_lrcr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 active_window_h_bbd                : 1 ;  //    14     
	UINT32 active_window_h_en                 : 1 ;  //    15     
	UINT32 y0                                 : 12;  // 27:16     
	UINT32                                    : 2 ;  // 29:28     reserved
	UINT32 active_window_v_bbd                : 1 ;  //    30     
	UINT32 active_window_v_en                 : 1 ;  //    31     
} M14B0_PE1_LRCR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002477cL pe1_lrcr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} M14B0_PE1_LRCR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0024780L pe1_lrcr_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0     
	UINT32 n_valid_bins                       : 8 ;  // 15: 8     
	UINT32 cdf_x1                             : 8 ;  // 23:16     
	UINT32 cdf_x0                             : 8 ;  // 31:24     
} M14B0_PE1_LRCR_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0024784L pe1_lrcr_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0     
	UINT32 n_valid_bins                       : 8 ;  // 15: 8     
	UINT32 cdf_x1                             : 8 ;  // 23:16     
	UINT32 cdf_x0                             : 8 ;  // 31:24     
} M14B0_PE1_LRCR_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0024788L pe1_lrcr_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0     
	UINT32 n_valid_bins                       : 8 ;  // 15: 8     
	UINT32 cdf_x1                             : 8 ;  // 23:16     
	UINT32 cdf_x0                             : 8 ;  // 31:24     
} M14B0_PE1_LRCR_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc0024790L pe1_l_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_g_available                : 1 ;  //    28     
} M14B0_PE1_L_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0024794L pe1_l_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_L_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0024798L pe1_l_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_b_available                : 1 ;  //    28     
} M14B0_PE1_L_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc002479cL pe1_l_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_L_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00247a0L pe1_l_r_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_r_available                : 1 ;  //    28     
} M14B0_PE1_L_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00247a4L pe1_l_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_L_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00247a8L pe1_r_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_g_available                : 1 ;  //    28     
} M14B0_PE1_R_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00247acL pe1_r_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_R_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00247b0L pe1_r_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_b_available                : 1 ;  //    28     
} M14B0_PE1_R_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00247b4L pe1_r_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_R_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00247b8L pe1_r_r_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0     
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15     
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_r_available                : 1 ;  //    28     
	UINT32                                    : 2 ;  // 30:29     reserved
	UINT32 hist_rd_done                       : 1 ;  //    31     
} M14B0_PE1_R_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00247bcL pe1_r_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_R_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00247d0L pe1_drc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_en                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_drc_shp_apl_sel                : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_drc_rgb_sel                    : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_drc_gm_mode                    : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_drc_lk_blf_apf                 : 1 ;  //    16     
	UINT32                                    : 11;  // 27:17     reserved
	UINT32 reg_dbg_disp_blf                   : 1 ;  //    28     
	UINT32 reg_dbg_disp_lcont                 : 1 ;  //    29     
	UINT32 reg_dbg_disp_reflect               : 1 ;  //    30     
} M14B0_PE1_DRC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00247d4L pe1_drc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_rw_sel                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_drc_gm_op_sel                  : 1 ;  //     4     
} M14B0_PE1_DRC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00247d8L pe1_drc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bnh                        : 7 ;  //  6: 0     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 reg_drc_bnv                        : 6 ;  // 21:16     
} M14B0_PE1_DRC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00247dcL pe1_drc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bsh                        : 9 ;  //  8: 0     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_drc_bsv                        : 9 ;  // 24:16     
} M14B0_PE1_DRC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc00247e0L pe1_drc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_apl2                    : 10;  //  9: 0     
	UINT32 reg_drc_gm_apl1                    : 10;  // 19:10     
	UINT32 reg_drc_gm_apl0                    : 10;  // 29:20     
} M14B0_PE1_DRC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc00247e4L pe1_drc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_apl5                    : 10;  //  9: 0     
	UINT32 reg_drc_gm_apl4                    : 10;  // 19:10     
	UINT32 reg_drc_gm_apl3                    : 10;  // 29:20     
} M14B0_PE1_DRC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc00247e8L pe1_drc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_blf_wei_cen                : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_drc_apl_iir                    : 7 ;  // 22:16     
} M14B0_PE1_DRC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc00247ecL pe1_drc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_shp_clip_n                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_drc_shp_clip_p                 : 12;  // 27:16     
} M14B0_PE1_DRC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc00247f0L pe1_drc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_gn                     : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_th                     : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc00247f4L pe1_drc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_adj_l                  : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_drc_apl_adj_h                  : 8 ;  // 23:16     
} M14B0_PE1_DRC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc00247f8L pe1_drc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x2_p                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x1_p                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc00247fcL pe1_drc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x4_p                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x3_p                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc0024800L pe1_drc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y1_p                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y0_p                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc0024804L pe1_drc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y3_p                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y2_p                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc0024808L pe1_drc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y5_p                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y4_p                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc002480cL pe1_drc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x2_n                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x1_n                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc0024810L pe1_drc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x4_n                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x3_n                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc0024814L pe1_drc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y1_n                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y0_n                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc0024818L pe1_drc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y3_n                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y2_n                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc002481cL pe1_drc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y5_n                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y4_n                   : 10;  // 25:16     
} M14B0_PE1_DRC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc0024820L pe1_drc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_lut_addr_clr               : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_drc_hfc_rd_sel                 : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_drc_vfc_rd_sel                 : 1 ;  //     8     
} M14B0_PE1_DRC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc0024824L pe1_drc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_lk_blf_hfc_data            : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc0024828L pe1_drc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_lk_blf_vfc_data            : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc002482cL pe1_drc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_hfc_re                     : 1 ;  //     0     
	UINT32 reg_drc_hfc_rdata                  : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc0024830L pe1_drc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_vfc_re                     : 1 ;  //     0     
	UINT32 reg_drc_vfc_rdata                  : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc0024834L pe1_drc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_blf_wei_wdata              : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc0024838L pe1_drc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_blf_wei_re                 : 1 ;  //     0     
	UINT32 reg_drc_blf_wei_rdata              : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc002483cL pe1_drc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_wdata                   : 30;  // 29: 0     
} M14B0_PE1_DRC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc0024840L pe1_drc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_re                      : 2 ;  //  1: 0     
	UINT32 reg_drc_gm_rdata                   : 30;  // 29: 0     
} M14B0_PE1_DRC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc0024844L pe1_drc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_wdata                   : 30;  // 29: 0     
} M14B0_PE1_DRC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc0024848L pe1_drc_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_re                      : 2 ;  //  1: 0     
	UINT32 reg_drc_gm_rdata                   : 30;  // 29: 0     
} M14B0_PE1_DRC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc002484cL pe1_drc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_hpf_wei_wdata              : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc0024850L pe1_drc_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_hpf_wei_re                 : 1 ;  //     0     
	UINT32 reg_drc_hpf_wei_rdata              : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc0024854L pe1_drc_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bl_apl_raddr_h             : 7 ;  //  6: 0     
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 reg_drc_bl_apl_raddr_v             : 6 ;  // 21:16     
} M14B0_PE1_DRC_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc0024858L pe1_drc_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bl_apl_rdata               : 10;  //  9: 0     
} M14B0_PE1_DRC_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc0024860L pe1_sre_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_out_mux_sel                    : 8 ;  //  7: 0     
	UINT32 reg_clip2_en                       : 1 ;  //     8     
	UINT32 reg_clip2_th                       : 9 ;  // 17: 9     
	UINT32 reg_clip2_str                      : 4 ;  // 21:18     
	UINT32 reg_clip2_base                     : 9 ;  // 30:22     
} M14B0_PE1_SRE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc0024864L pe1_sre_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hd_core                        : 1 ;  //     0     
	UINT32 reg_hd_interp_core                 : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_bypass_th                      : 3 ;  //  6: 4     
	UINT32 reg_bypass_en                      : 2 ;  //  8: 7     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_ls2_en                         : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_ls2_lap_offset                 : 4 ;  // 19:16     
	UINT32 reg_ls2_lap_cost_scale             : 3 ;  // 22:20     
} M14B0_PE1_SRE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc0024868L pe1_sre_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bcode_ldr_th0                  : 8 ;  //  7: 0     
	UINT32 reg_bcode_ldr_th1                  : 8 ;  // 15: 8     
	UINT32 reg_bcode_ldr_th2                  : 8 ;  // 23:16     
} M14B0_PE1_SRE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc002486cL pe1_sre_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ls2_lap_lv                     : 32;  // 31: 0     
} M14B0_PE1_SRE_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc0024870L pe1_sre_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ls2_lap_scale_avg_lap_th       : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_ls2_lap_scale_high_lap_str     : 4 ;  //  7: 4     
	UINT32 reg_ls2_lap_lap_th                 : 5 ;  // 12: 8     
	UINT32 reg_ls2_srb_reduce                 : 4 ;  // 16:13     
	UINT32                                    : 1 ;  //    17     reserved
	UINT32 reg_ls2_lap_scale_ldr_th           : 6 ;  // 23:18     
	UINT32 reg_ls2_lap_ldr_th                 : 7 ;  // 30:24     
} M14B0_PE1_SRE_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc0024874L pe1_sre_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sra_en                         : 1 ;  //     0     
	UINT32 reg_srb_en                         : 1 ;  //     1     
	UINT32 reg_use_srarf                      : 1 ;  //     2     
	UINT32 reg_mode_srarf                     : 1 ;  //     3     
	UINT32 reg_srarf_adp_ldr_en               : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 reg_srarf_adp_ldr_sh               : 2 ;  //  7: 6     
	UINT32 reg_th_srarf_med                   : 4 ;  // 11: 8     
	UINT32 reg_th_srarf_sra                   : 4 ;  // 15:12     
	UINT32                                    : 2 ;  // 17:16     reserved
	UINT32 reg_use_srbrf                      : 1 ;  //    18     
	UINT32 reg_mode_srbrf                     : 1 ;  //    19     
	UINT32 reg_srbrf_adp_ldr_en               : 1 ;  //    20     
	UINT32                                    : 1 ;  //    21     reserved
	UINT32 reg_srbrf_adp_ldr_sh               : 2 ;  // 23:22     
	UINT32 reg_th_srbrf_med                   : 4 ;  // 27:24     
	UINT32 reg_th_srbrf_srb                   : 4 ;  // 31:28     
} M14B0_PE1_SRE_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc0024878L pe1_sre_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_bypass_str_sra                 : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_sc_use_ls_sra                  : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_sc_gs_sra                      : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_bypass_str_srb                 : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_sc_use_ls_srb                  : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_sc_gs_srb                      : 7 ;  // 30:24     
} M14B0_PE1_SRE_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc002487cL pe1_sre_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_ls_lv_sra                   : 32;  // 31: 0     
} M14B0_PE1_SRE_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc0024880L pe1_sre_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sc_ls_lv_srb                   : 32;  // 31: 0     
} M14B0_PE1_SRE_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc0024884L pe1_sre_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lc_offset_sra                  : 10;  //  9: 0     
	UINT32 reg_ls_ldr_sh_sra                  : 2 ;  // 11:10     
	UINT32 reg_ls_grd_sh_sra                  : 2 ;  // 13:12     
	UINT32 reg_ls_mode_scaling_sra            : 2 ;  // 15:14     
	UINT32 reg_lc_offset_srb                  : 10;  // 25:16     
	UINT32 reg_ls_ldr_sh_srb                  : 2 ;  // 27:26     
	UINT32 reg_ls_grd_sh_srb                  : 2 ;  // 29:28     
	UINT32 reg_ls_mode_scaling_srb            : 2 ;  // 31:30     
} M14B0_PE1_SRE_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc0024888L pe1_sre_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_auto_clip_str                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_reduce_clip_srb_th             : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_clipstr_max                    : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_clipstr_min                    : 5 ;  // 20:16     
} M14B0_PE1_SRE_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc002488cL pe1_sre_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_clip_en                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_clipr2_max_d                   : 5 ;  //  8: 4     
	UINT32 reg_clipr2_min_d                   : 5 ;  // 13: 9     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_clipr2_min_diff                : 8 ;  // 23:16     
	UINT32 reg_use_clipr2                     : 2 ;  // 25:24     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_reduce_clip_srb                : 4 ;  // 31:28     
} M14B0_PE1_SRE_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc0024890L pe1_sre_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_demo_mode                      : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_dbg_disp                       : 14;  // 21: 8     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_x_mov                          : 4 ;  // 27:24     
	UINT32 reg_y_mov                          : 4 ;  // 31:28     
} M14B0_PE1_SRE_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc0024894L pe1_sre_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_x_size                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_y_size                         : 12;  // 27:16     
} M14B0_PE1_SRE_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc0024898L pe1_sre_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_x_min                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_x_max                          : 12;  // 27:16     
} M14B0_PE1_SRE_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc002489cL pe1_sre_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_y_min                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_y_max                          : 12;  // 27:16     
} M14B0_PE1_SRE_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc00248a0L pe1_sre_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_yy                             : 30;  // 29: 0     
} M14B0_PE1_SRE_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc00248a4L pe1_sre_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wei_sra_ltv_th                 : 4 ;  //  3: 0     
	UINT32 reg_flag_wei_sra                   : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_wei_sra_grd_th                 : 4 ;  // 11: 8     
	UINT32 reg_ctl_wei_srb_ltv                : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_min_wei_sra                    : 4 ;  // 19:16     
	UINT32 reg_mode_wei                       : 3 ;  // 22:20     
} M14B0_PE1_SRE_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc00248b0L pe1_sre_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_sre_address                    : 9 ;  //  8: 0     
	UINT32 hif_sre_ai_sel                     : 2 ;  // 10: 9     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 hif_sre_ai                         : 1 ;  //    12     
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_sre_enable                     : 1 ;  //    15     
} M14B0_PE1_SRE_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00248b4L pe1_sre_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_sre_wdata                      : 32;  // 31: 0     
} M14B0_PE1_SRE_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc00248c0L pe1_fsw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_FSW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc00248c4L pe1_fsw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_FSW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc00248c8L pe1_fsw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_FSW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc00248ccL pe1_fsw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} M14B0_PE1_FSW_CTRL_03_T;

typedef struct {
	M14B0_PE1_TOP_CTRL_00_T            	pe1_top_ctrl_00;              //0xc0024300L
	M14B0_PE1_TOP_CTRL_01_T            	pe1_top_ctrl_01;              //0xc0024304L
	M14B0_PE1_TOP_CTRL_02_T            	pe1_top_ctrl_02;              //0xc0024308L
	M14B0_PE1_TOP_CTRL_03_T            	pe1_top_ctrl_03;              //0xc002430cL
	UINT32                             	reserved00[4];                //0xc0024310L~0xc002431cL
	M14B0_PE1_WIN1_CTRL_00_T           	pe1_win1_ctrl_00;             //0xc0024320L
	M14B0_PE1_WIN1_CTRL_01_T           	pe1_win1_ctrl_01;             //0xc0024324L
	M14B0_PE1_WIN1_CTRL_02_T           	pe1_win1_ctrl_02;             //0xc0024328L
	M14B0_PE1_WIN1_CTRL_03_T           	pe1_win1_ctrl_03;             //0xc002432cL
	M14B0_PE1_WIN1_CTRL_04_T           	pe1_win1_ctrl_04;             //0xc0024330L
	M14B0_PE1_WIN2_CTRL_00_T           	pe1_win2_ctrl_00;             //0xc0024334L
	M14B0_PE1_WIN2_CTRL_01_T           	pe1_win2_ctrl_01;             //0xc0024338L
	M14B0_PE1_WIN2_CTRL_02_T           	pe1_win2_ctrl_02;             //0xc002433cL
	M14B0_PE1_WIN2_CTRL_03_T           	pe1_win2_ctrl_03;             //0xc0024340L
	M14B0_PE1_WIN2_CTRL_04_T           	pe1_win2_ctrl_04;             //0xc0024344L
	UINT32                             	reserved01[2];                //0xc0024348L~0xc002434cL
	M14B0_PE1_APL_CTRL_00_T            	pe1_apl_ctrl_00;              //0xc0024350L
	M14B0_PE1_APL_CTRL_01_T            	pe1_apl_ctrl_01;              //0xc0024354L
	M14B0_PE1_APL_CTRL_02_T            	pe1_apl_ctrl_02;              //0xc0024358L
	M14B0_PE1_APL_STAT_00_T            	pe1_apl_stat_00;              //0xc002435cL
	M14B0_PE1_APL_STAT_01_T            	pe1_apl_stat_01;              //0xc0024360L
	UINT32                             	reserved02[3];                //0xc0024364L~0xc002436cL
	M14B0_PE1_TPG_CTRL_00_T            	pe1_tpg_ctrl_00;              //0xc0024370L
	M14B0_PE1_TPG_CTRL_01_T            	pe1_tpg_ctrl_01;              //0xc0024374L
	M14B0_PE1_TPG_CTRL_02_T            	pe1_tpg_ctrl_02;              //0xc0024378L
	M14B0_PE1_TPG_CTRL_03_T            	pe1_tpg_ctrl_03;              //0xc002437cL
	M14B0_PE1_TPG_CTRL_04_T            	pe1_tpg_ctrl_04;              //0xc0024380L
	M14B0_PE1_TPG_IA_CTRL_T            	pe1_tpg_ia_ctrl;              //0xc0024384L
	M14B0_PE1_TPG_IA_DATA_T            	pe1_tpg_ia_data;              //0xc0024388L
	UINT32                             	reserved03;                   //0xc002438cL
	M14B0_PE1_WIN3_CTRL_00_T           	pe1_win3_ctrl_00;             //0xc0024390L
	M14B0_PE1_WIN3_CTRL_01_T           	pe1_win3_ctrl_01;             //0xc0024394L
	M14B0_PE1_WIN3_CTRL_02_T           	pe1_win3_ctrl_02;             //0xc0024398L
	M14B0_PE1_WIN3_CTRL_03_T           	pe1_win3_ctrl_03;             //0xc002439cL
	M14B0_PE1_WIN3_CTRL_04_T           	pe1_win3_ctrl_04;             //0xc00243a0L
	UINT32                             	reserved04[3];                //0xc00243a4L~0xc00243acL
	M14B0_PE1_DERH_CTRL_00_T           	pe1_derh_ctrl_00;             //0xc00243b0L
	M14B0_PE1_DERH_CTRL_01_T           	pe1_derh_ctrl_01;             //0xc00243b4L
	M14B0_PE1_DERH_CTRL_02_T           	pe1_derh_ctrl_02;             //0xc00243b8L
	M14B0_PE1_DERH_CTRL_03_T           	pe1_derh_ctrl_03;             //0xc00243bcL
	M14B0_PE1_DERH_CTRL_04_T           	pe1_derh_ctrl_04;             //0xc00243c0L
	M14B0_PE1_DERH_CTRL_05_T           	pe1_derh_ctrl_05;             //0xc00243c4L
	M14B0_PE1_DERH_CTRL_06_T           	pe1_derh_ctrl_06;             //0xc00243c8L
	M14B0_PE1_DERH_CTRL_07_T           	pe1_derh_ctrl_07;             //0xc00243ccL
	M14B0_PE1_DERH_CTRL_08_T           	pe1_derh_ctrl_08;             //0xc00243d0L
	M14B0_PE1_DERH_CTRL_09_T           	pe1_derh_ctrl_09;             //0xc00243d4L
	M14B0_PE1_DERH_CTRL_0A_T           	pe1_derh_ctrl_0a;             //0xc00243d8L
	M14B0_PE1_DERH_CTRL_0B_T           	pe1_derh_ctrl_0b;             //0xc00243dcL
	M14B0_PE1_DERH_CTRL_0C_T           	pe1_derh_ctrl_0c;             //0xc00243e0L
	M14B0_PE1_DERH_CTRL_0D_T           	pe1_derh_ctrl_0d;             //0xc00243e4L
	UINT32                             	reserved05[2];                //0xc00243e8L~0xc00243ecL
	M14B0_PE1_DERV_CTRL_0_T            	pe1_derv_ctrl_0;              //0xc00243f0L
	M14B0_PE1_DERV_CTRL_1_T            	pe1_derv_ctrl_1;              //0xc00243f4L
	M14B0_PE1_DERV_CTRL_2_T            	pe1_derv_ctrl_2;              //0xc00243f8L
	M14B0_PE1_DERV_CTRL_3_T            	pe1_derv_ctrl_3;              //0xc00243fcL
	UINT32                             	reserved06[4];                //0xc0024400L~0xc002440cL
	M14B0_PE1_DP_CTRL_00_T             	pe1_dp_ctrl_00;               //0xc0024410L
	M14B0_PE1_DP_CTRL_01_T             	pe1_dp_ctrl_01;               //0xc0024414L
	M14B0_PE1_DP_CTRL_02_T             	pe1_dp_ctrl_02;               //0xc0024418L
	M14B0_PE1_DP_CTRL_03_T             	pe1_dp_ctrl_03;               //0xc002441cL
	UINT32                             	reserved07[4];                //0xc0024420L~0xc002442cL
	M14B0_PE1_SP_CTRL_00_T             	pe1_sp_ctrl_00;               //0xc0024430L
	M14B0_PE1_SP_CTRL_01_T             	pe1_sp_ctrl_01;               //0xc0024434L
	M14B0_PE1_SP_CTRL_02_T             	pe1_sp_ctrl_02;               //0xc0024438L
	M14B0_PE1_SP_CTRL_03_T             	pe1_sp_ctrl_03;               //0xc002443cL
	M14B0_PE1_SP_CTRL_04_T             	pe1_sp_ctrl_04;               //0xc0024440L
	M14B0_PE1_SP_CTRL_05_T             	pe1_sp_ctrl_05;               //0xc0024444L
	M14B0_PE1_SP_CTRL_06_T             	pe1_sp_ctrl_06;               //0xc0024448L
	M14B0_PE1_SP_CTRL_07_T             	pe1_sp_ctrl_07;               //0xc002444cL
	M14B0_PE1_SP_CTRL_08_T             	pe1_sp_ctrl_08;               //0xc0024450L
	M14B0_PE1_SP_CTRL_09_T             	pe1_sp_ctrl_09;               //0xc0024454L
	M14B0_PE1_SP_CTRL_0A_T             	pe1_sp_ctrl_0a;               //0xc0024458L
	M14B0_PE1_SP_CTRL_0B_T             	pe1_sp_ctrl_0b;               //0xc002445cL
	M14B0_PE1_SP_CTRL_0C_T             	pe1_sp_ctrl_0c;               //0xc0024460L
	UINT32                             	reserved08[3];                //0xc0024464L~0xc002446cL
	M14B0_PE1_MP_CTRL_00_T             	pe1_mp_ctrl_00;               //0xc0024470L
	M14B0_PE1_MP_CTRL_01_T             	pe1_mp_ctrl_01;               //0xc0024474L
	M14B0_PE1_MP_CTRL_02_T             	pe1_mp_ctrl_02;               //0xc0024478L
	M14B0_PE1_MP_CTRL_03_T             	pe1_mp_ctrl_03;               //0xc002447cL
	M14B0_PE1_MP_CTRL_04_T             	pe1_mp_ctrl_04;               //0xc0024480L
	M14B0_PE1_MP_CTRL_05_T             	pe1_mp_ctrl_05;               //0xc0024484L
	M14B0_PE1_MP_CTRL_06_T             	pe1_mp_ctrl_06;               //0xc0024488L
	M14B0_PE1_MP_CTRL_07_T             	pe1_mp_ctrl_07;               //0xc002448cL
	M14B0_PE1_MP_CTRL_08_T             	pe1_mp_ctrl_08;               //0xc0024490L
	M14B0_PE1_MP_CTRL_09_T             	pe1_mp_ctrl_09;               //0xc0024494L
	M14B0_PE1_MP_CTRL_0A_T             	pe1_mp_ctrl_0a;               //0xc0024498L
	M14B0_PE1_MP_CTRL_0B_T             	pe1_mp_ctrl_0b;               //0xc002449cL
	M14B0_PE1_MP_CTRL_0C_T             	pe1_mp_ctrl_0c;               //0xc00244a0L
	UINT32                             	reserved09[3];                //0xc00244a4L~0xc00244acL
	M14B0_PE1_CORING_CTRL_00_T         	pe1_coring_ctrl_00;           //0xc00244b0L
	M14B0_PE1_CORING_CTRL_01_T         	pe1_coring_ctrl_01;           //0xc00244b4L
	M14B0_PE1_CORING_CTRL_02_T         	pe1_coring_ctrl_02;           //0xc00244b8L
	M14B0_PE1_CORING_CTRL_03_T         	pe1_coring_ctrl_03;           //0xc00244bcL
	M14B0_PE1_CORING_CTRL_04_T         	pe1_coring_ctrl_04;           //0xc00244c0L
	M14B0_PE1_CORING_CTRL_05_T         	pe1_coring_ctrl_05;           //0xc00244c4L
	M14B0_PE1_CORING_CTRL_06_T         	pe1_coring_ctrl_06;           //0xc00244c8L
	M14B0_PE1_CORING_CTRL_07_T         	pe1_coring_ctrl_07;           //0xc00244ccL
	M14B0_PE1_CORING_CTRL_08_T         	pe1_coring_ctrl_08;           //0xc00244d0L
	M14B0_PE1_CORING_CTRL_09_T         	pe1_coring_ctrl_09;           //0xc00244d4L
	UINT32                             	reserved10[2];                //0xc00244d8L~0xc00244dcL
	M14B0_PE1_CORING_STAT_00_T         	pe1_coring_stat_00;           //0xc00244e0L
	M14B0_PE1_CORING_STAT_01_T         	pe1_coring_stat_01;           //0xc00244e4L
	M14B0_PE1_CORING_STAT_02_T         	pe1_coring_stat_02;           //0xc00244e8L
	M14B0_PE1_CORING_STAT_03_T         	pe1_coring_stat_03;           //0xc00244ecL
	UINT32                             	reserved11[4];                //0xc00244f0L~0xc00244fcL
	M14B0_PE1_TI_CTRL_0_T              	pe1_ti_ctrl_0;                //0xc0024500L
	M14B0_PE1_TI_CTRL_1_T              	pe1_ti_ctrl_1;                //0xc0024504L
	M14B0_PE1_TI_CTRL_2_T              	pe1_ti_ctrl_2;                //0xc0024508L
	UINT32                             	reserved12;                   //0xc002450cL
	M14B0_PE1_DCE_BLUR_CTRL_00_T       	pe1_dce_blur_ctrl_00;         //0xc0024510L
	M14B0_PE1_DCE_BLUR_CTRL_01_T       	pe1_dce_blur_ctrl_01;         //0xc0024514L
	M14B0_PE1_DCE_BLUR_CTRL_02_T       	pe1_dce_blur_ctrl_02;         //0xc0024518L
	M14B0_PE1_DCE_BLUR_CTRL_03_T       	pe1_dce_blur_ctrl_03;         //0xc002451cL
	M14B0_PE1_DJ_CTRL_00_T             	pe1_dj_ctrl_00;               //0xc0024520L
	M14B0_PE1_DJ_CTRL_01_T             	pe1_dj_ctrl_01;               //0xc0024524L
	M14B0_PE1_DJ_CTRL_02_T             	pe1_dj_ctrl_02;               //0xc0024528L
	M14B0_PE1_DJ_CTRL_03_T             	pe1_dj_ctrl_03;               //0xc002452cL
	M14B0_PE1_CTI_CTRL_00_T            	pe1_cti_ctrl_00;              //0xc0024530L
	M14B0_PE1_CTI_CTRL_01_T            	pe1_cti_ctrl_01;              //0xc0024534L
	M14B0_PE1_CTI_CTRL_02_T            	pe1_cti_ctrl_02;              //0xc0024538L
	UINT32                             	reserved13;                   //0xc002453cL
	M14B0_PE1_PMUX_CTRL_00_T           	pe1_pmux_ctrl_00;             //0xc0024540L
	M14B0_PE1_PMUX_CTRL_01_T           	pe1_pmux_ctrl_01;             //0xc0024544L
	M14B0_PE1_PMUX_CTRL_02_T           	pe1_pmux_ctrl_02;             //0xc0024548L
	M14B0_PE1_PMUX_CTRL_03_T           	pe1_pmux_ctrl_03;             //0xc002454cL
	M14B0_PE1_PMUX_CTRL_04_T           	pe1_pmux_ctrl_04;             //0xc0024550L
	M14B0_PE1_PMUX_CTRL_05_T           	pe1_pmux_ctrl_05;             //0xc0024554L
	UINT32                             	reserved14[2];                //0xc0024558L~0xc002455cL
	M14B0_PE1_VSPYC_CTRL_00_T          	pe1_vspyc_ctrl_00;            //0xc0024560L
	M14B0_PE1_VSPYC_CTRL_01_T          	pe1_vspyc_ctrl_01;            //0xc0024564L
	UINT32                             	reserved15[2];                //0xc0024568L~0xc002456cL
	M14B0_PE1_DEBUG_CTRL_00_T          	pe1_debug_ctrl_00;            //0xc0024570L
	M14B0_PE1_DEBUG_CTRL_01_T          	pe1_debug_ctrl_01;            //0xc0024574L
	M14B0_PE1_DEBUG_CTRL_02_T          	pe1_debug_ctrl_02;            //0xc0024578L
	UINT32                             	reserved16;                   //0xc002457cL
	M14B0_PE1_OCSC_CTRL_00_T           	pe1_ocsc_ctrl_00;             //0xc0024580L
	M14B0_PE1_OCSC_CTRL_01_T           	pe1_ocsc_ctrl_01;             //0xc0024584L
	M14B0_PE1_OCSC_CTRL_02_T           	pe1_ocsc_ctrl_02;             //0xc0024588L
	M14B0_PE1_OCSC_CTRL_03_T           	pe1_ocsc_ctrl_03;             //0xc002458cL
	M14B0_PE1_OCSC_CTRL_04_T           	pe1_ocsc_ctrl_04;             //0xc0024590L
	M14B0_PE1_OCSC_CTRL_05_T           	pe1_ocsc_ctrl_05;             //0xc0024594L
	M14B0_PE1_OCSC_CTRL_06_T           	pe1_ocsc_ctrl_06;             //0xc0024598L
	M14B0_PE1_OCSC_CTRL_07_T           	pe1_ocsc_ctrl_07;             //0xc002459cL
	M14B0_PE1_CEN_CTRL_00_T            	pe1_cen_ctrl_00;              //0xc00245a0L
	M14B0_PE1_CEN_CTRL_01_T            	pe1_cen_ctrl_01;              //0xc00245a4L
	M14B0_PE1_CEN_CTRL_02_T            	pe1_cen_ctrl_02;              //0xc00245a8L
	M14B0_PE1_CEN_CTRL_03_T            	pe1_cen_ctrl_03;              //0xc00245acL
	M14B0_PE1_CEN_CTRL_04_T            	pe1_cen_ctrl_04;              //0xc00245b0L
	M14B0_PE1_CEN_CTRL_05_T            	pe1_cen_ctrl_05;              //0xc00245b4L
	M14B0_PE1_CEN_CTRL_06_T            	pe1_cen_ctrl_06;              //0xc00245b8L
	M14B0_PE1_CEN_CTRL_07_T            	pe1_cen_ctrl_07;              //0xc00245bcL
	UINT32                             	reserved17[4];                //0xc00245c0L~0xc00245ccL
	M14B0_PE1_CEN_IA_CTRL_T            	pe1_cen_ia_ctrl;              //0xc00245d0L
	M14B0_PE1_CEN_IA_DATA_T            	pe1_cen_ia_data;              //0xc00245d4L
	M14B0_PE1_CEN_DELTA_IA_CTRL_T      	pe1_cen_delta_ia_ctrl;        //0xc00245d8L
	M14B0_PE1_CEN_DELTA_IA_DATA_T      	pe1_cen_delta_ia_data;        //0xc00245dcL
	M14B0_PE1_DCE_CTRL_00_T            	pe1_dce_ctrl_00;              //0xc00245e0L
	M14B0_PE1_DCE_CTRL_01_T            	pe1_dce_ctrl_01;              //0xc00245e4L
	M14B0_PE1_DCE_CTRL_02_T            	pe1_dce_ctrl_02;              //0xc00245e8L
	M14B0_PE1_DCE_CTRL_03_T            	pe1_dce_ctrl_03;              //0xc00245ecL
	M14B0_PE1_DCE_CTRL_04_T            	pe1_dce_ctrl_04;              //0xc00245f0L
	M14B0_PE1_DCE_CTRL_05_T            	pe1_dce_ctrl_05;              //0xc00245f4L
	M14B0_PE1_DCE_IA_CTRL_T            	pe1_dce_ia_ctrl;              //0xc00245f8L
	M14B0_PE1_DCE_IA_DATA_T            	pe1_dce_ia_data;              //0xc00245fcL
	M14B0_PE1_DCE_CTRL_08_T            	pe1_dce_ctrl_08;              //0xc0024600L
	M14B0_PE1_DCE_CTRL_09_T            	pe1_dce_ctrl_09;              //0xc0024604L
	M14B0_PE1_DCE_HIST_IA_CTRL_T       	pe1_dce_hist_ia_ctrl;         //0xc0024608L
	M14B0_PE1_DCE_HIST_IA_DATA1_T      	pe1_dce_hist_ia_data1;        //0xc002460cL
	M14B0_PE1_DCE_HIST_IA_DATA2_T      	pe1_dce_hist_ia_data2;        //0xc0024610L
	UINT32                             	reserved18[3];                //0xc0024614L~0xc002461cL
	M14B0_PE1_DSE_CTRL_00_T            	pe1_dse_ctrl_00;              //0xc0024620L
	M14B0_PE1_DSE_CTRL_01_T            	pe1_dse_ctrl_01;              //0xc0024624L
	M14B0_PE1_DSE_CTRL_02_T            	pe1_dse_ctrl_02;              //0xc0024628L
	M14B0_PE1_DSE_IA_CTRL_T            	pe1_dse_ia_ctrl;              //0xc002462cL
	M14B0_PE1_DSE_IA_DATA_T            	pe1_dse_ia_data;              //0xc0024630L
	UINT32                             	reserved19[3];                //0xc0024634L~0xc002463cL
	M14B0_PE1_CW_CTRL_00_T             	pe1_cw_ctrl_00;               //0xc0024640L
	M14B0_PE1_CW_CTRL_01_T             	pe1_cw_ctrl_01;               //0xc0024644L
	M14B0_PE1_CW_CTRL_02_T             	pe1_cw_ctrl_02;               //0xc0024648L
	M14B0_PE1_CW_CTRL_03_T             	pe1_cw_ctrl_03;               //0xc002464cL
	M14B0_PE1_CW_CTRL_04_T             	pe1_cw_ctrl_04;               //0xc0024650L
	UINT32                             	reserved20[3];                //0xc0024654L~0xc002465cL
	M14B0_PE1_CONV_CTRL_00_T           	pe1_conv_ctrl_00;             //0xc0024660L
	M14B0_PE1_CONV_CTRL_01_T           	pe1_conv_ctrl_01;             //0xc0024664L
	M14B0_PE1_CONV_CTRL_02_T           	pe1_conv_ctrl_02;             //0xc0024668L
	M14B0_PE1_CONV_CTRL_03_T           	pe1_conv_ctrl_03;             //0xc002466cL
	M14B0_PE1_CONV_CTRL_04_T           	pe1_conv_ctrl_04;             //0xc0024670L
	M14B0_PE1_CONV_CTRL_05_T           	pe1_conv_ctrl_05;             //0xc0024674L
	M14B0_PE1_CONV_CTRL_06_T           	pe1_conv_ctrl_06;             //0xc0024678L
	M14B0_PE1_CONV_CTRL_07_T           	pe1_conv_ctrl_07;             //0xc002467cL
	M14B0_PE1_CONV_CTRL_08_T           	pe1_conv_ctrl_08;             //0xc0024680L
	M14B0_PE1_CONV_CTRL_09_T           	pe1_conv_ctrl_09;             //0xc0024684L
	M14B0_PE1_CONV_CTRL_0A_T           	pe1_conv_ctrl_0a;             //0xc0024688L
	M14B0_PE1_CONV_CTRL_0B_T           	pe1_conv_ctrl_0b;             //0xc002468cL
	M14B0_PE1_CONV_CTRL_0C_T           	pe1_conv_ctrl_0c;             //0xc0024690L
	M14B0_PE1_CONV_CTRL_0D_T           	pe1_conv_ctrl_0d;             //0xc0024694L
	M14B0_PE1_CONV_CTRL_0E_T           	pe1_conv_ctrl_0e;             //0xc0024698L
	M14B0_PE1_CONV_CTRL_0F_T           	pe1_conv_ctrl_0f;             //0xc002469cL
	M14B0_PE1_CONV_CTRL_10_T           	pe1_conv_ctrl_10;             //0xc00246a0L
	M14B0_PE1_CONV_CTRL_11_T           	pe1_conv_ctrl_11;             //0xc00246a4L
	M14B0_PE1_CONV_CTRL_12_T           	pe1_conv_ctrl_12;             //0xc00246a8L
	UINT32                             	reserved21;                   //0xc00246acL
	M14B0_PE1_MUTE_CTRL_00_T           	pe1_mute_ctrl_00;             //0xc00246b0L
	M14B0_PE1_MUTE_CTRL_01_T           	pe1_mute_ctrl_01;             //0xc00246b4L
	M14B0_PE1_MUTE_CTRL_02_T           	pe1_mute_ctrl_02;             //0xc00246b8L
	M14B0_PE1_MUTE_CTRL_03_T           	pe1_mute_ctrl_03;             //0xc00246bcL
	M14B0_PE1_MUTE_CTRL_04_T           	pe1_mute_ctrl_04;             //0xc00246c0L
	M14B0_PE1_MUTE_CTRL_05_T           	pe1_mute_ctrl_05;             //0xc00246c4L
	M14B0_PE1_MUTE_CTRL_06_T           	pe1_mute_ctrl_06;             //0xc00246c8L
	M14B0_PE1_MUTE_CTRL_07_T           	pe1_mute_ctrl_07;             //0xc00246ccL
	M14B0_PE1_MUTE_CTRL_08_T           	pe1_mute_ctrl_08;             //0xc00246d0L
	M14B0_PE1_MUTE_CTRL_09_T           	pe1_mute_ctrl_09;             //0xc00246d4L
	M14B0_PE1_MUTE_CTRL_0A_T           	pe1_mute_ctrl_0a;             //0xc00246d8L
	M14B0_PE1_MUTE_CTRL_0B_T           	pe1_mute_ctrl_0b;             //0xc00246dcL
	M14B0_PE1_MUTE_CTRL_0C_T           	pe1_mute_ctrl_0c;             //0xc00246e0L
	M14B0_PE1_MUTE_CTRL_0D_T           	pe1_mute_ctrl_0d;             //0xc00246e4L
	M14B0_PE1_MUTE_CTRL_0E_T           	pe1_mute_ctrl_0e;             //0xc00246e8L
	M14B0_PE1_MUTE_CTRL_0F_T           	pe1_mute_ctrl_0f;             //0xc00246ecL
	UINT32                             	reserved22[4];                //0xc00246f0L~0xc00246fcL
	M14B0_PE1_SQM_CTRL_00_T            	pe1_sqm_ctrl_00;              //0xc0024700L
	M14B0_PE1_SQM_CTRL_01_T            	pe1_sqm_ctrl_01;              //0xc0024704L
	M14B0_PE1_SQM_CTRL_02_T            	pe1_sqm_ctrl_02;              //0xc0024708L
	M14B0_PE1_SQM_CTRL_03_T            	pe1_sqm_ctrl_03;              //0xc002470cL
	M14B0_PE1_SQM_CTRL_04_T            	pe1_sqm_ctrl_04;              //0xc0024710L
	M14B0_PE1_SQM_CTRL_05_T            	pe1_sqm_ctrl_05;              //0xc0024714L
	M14B0_PE1_SQM_STAT_00_T            	pe1_sqm_stat_00;              //0xc0024718L
	UINT32                             	reserved23;                   //0xc002471cL
	M14B0_PE1_BBD2D_CTRL_00_T          	pe1_bbd2d_ctrl_00;            //0xc0024720L
	M14B0_PE1_BBD2D_CTRL_01_T          	pe1_bbd2d_ctrl_01;            //0xc0024724L
	M14B0_PE1_BBD2D_CTRL_02_T          	pe1_bbd2d_ctrl_02;            //0xc0024728L
	M14B0_PE1_BBD2D_STAT_00_T          	pe1_bbd2d_stat_00;            //0xc002472cL
	M14B0_PE1_BBD2D_STAT_01_T          	pe1_bbd2d_stat_01;            //0xc0024730L
	UINT32                             	reserved24[3];                //0xc0024734L~0xc002473cL
	M14B0_PE1_BBD3D_CTRL_00_T          	pe1_bbd3d_ctrl_00;            //0xc0024740L
	M14B0_PE1_BBD3D_CTRL_01_T          	pe1_bbd3d_ctrl_01;            //0xc0024744L
	M14B0_PE1_BBD3D_CTRL_02_T          	pe1_bbd3d_ctrl_02;            //0xc0024748L
	M14B0_PE1_BBD3D_STAT_00_T          	pe1_bbd3d_stat_00;            //0xc002474cL
	UINT32                             	reserved25[4];                //0xc0024750L~0xc002475cL
	M14B0_PE1_LRCR_CTRL_00_T           	pe1_lrcr_ctrl_00;             //0xc0024760L
	M14B0_PE1_LRCR_CTRL_01_T           	pe1_lrcr_ctrl_01;             //0xc0024764L
	M14B0_PE1_LRCR_CTRL_02_T           	pe1_lrcr_ctrl_02;             //0xc0024768L
	M14B0_PE1_LRCR_CTRL_03_T           	pe1_lrcr_ctrl_03;             //0xc002476cL
	M14B0_PE1_LRCR_CTRL_04_T           	pe1_lrcr_ctrl_04;             //0xc0024770L
	M14B0_PE1_LRCR_CTRL_05_T           	pe1_lrcr_ctrl_05;             //0xc0024774L
	M14B0_PE1_LRCR_CTRL_06_T           	pe1_lrcr_ctrl_06;             //0xc0024778L
	M14B0_PE1_LRCR_CTRL_07_T           	pe1_lrcr_ctrl_07;             //0xc002477cL
	M14B0_PE1_LRCR_STAT_00_T           	pe1_lrcr_stat_00;             //0xc0024780L
	M14B0_PE1_LRCR_STAT_01_T           	pe1_lrcr_stat_01;             //0xc0024784L
	M14B0_PE1_LRCR_STAT_02_T           	pe1_lrcr_stat_02;             //0xc0024788L
	UINT32                             	reserved26;                   //0xc002478cL
	M14B0_PE1_L_G_HIST_IA_CTRL_T       	pe1_l_g_hist_ia_ctrl;         //0xc0024790L
	M14B0_PE1_L_G_HIST_IA_DATA_T       	pe1_l_g_hist_ia_data;         //0xc0024794L
	M14B0_PE1_L_B_HIST_IA_CTRL_T       	pe1_l_b_hist_ia_ctrl;         //0xc0024798L
	M14B0_PE1_L_B_HIST_IA_DATA_T       	pe1_l_b_hist_ia_data;         //0xc002479cL
	M14B0_PE1_L_R_HIST_IA_CTRL_T       	pe1_l_r_hist_ia_ctrl;         //0xc00247a0L
	M14B0_PE1_L_R_HIST_IA_DATA_T       	pe1_l_r_hist_ia_data;         //0xc00247a4L
	M14B0_PE1_R_G_HIST_IA_CTRL_T       	pe1_r_g_hist_ia_ctrl;         //0xc00247a8L
	M14B0_PE1_R_G_HIST_IA_DATA_T       	pe1_r_g_hist_ia_data;         //0xc00247acL
	M14B0_PE1_R_B_HIST_IA_CTRL_T       	pe1_r_b_hist_ia_ctrl;         //0xc00247b0L
	M14B0_PE1_R_B_HIST_IA_DATA_T       	pe1_r_b_hist_ia_data;         //0xc00247b4L
	M14B0_PE1_R_R_HIST_IA_CTRL_T       	pe1_r_r_hist_ia_ctrl;         //0xc00247b8L
	M14B0_PE1_R_R_HIST_IA_DATA_T       	pe1_r_r_hist_ia_data;         //0xc00247bcL
	UINT32                             	reserved27[4];                //0xc00247c0L~0xc00247ccL
	M14B0_PE1_DRC_CTRL_00_T            	pe1_drc_ctrl_00;              //0xc00247d0L
	M14B0_PE1_DRC_CTRL_01_T            	pe1_drc_ctrl_01;              //0xc00247d4L
	M14B0_PE1_DRC_CTRL_02_T            	pe1_drc_ctrl_02;              //0xc00247d8L
	M14B0_PE1_DRC_CTRL_03_T            	pe1_drc_ctrl_03;              //0xc00247dcL
	M14B0_PE1_DRC_CTRL_04_T            	pe1_drc_ctrl_04;              //0xc00247e0L
	M14B0_PE1_DRC_CTRL_05_T            	pe1_drc_ctrl_05;              //0xc00247e4L
	M14B0_PE1_DRC_CTRL_06_T            	pe1_drc_ctrl_06;              //0xc00247e8L
	M14B0_PE1_DRC_CTRL_07_T            	pe1_drc_ctrl_07;              //0xc00247ecL
	M14B0_PE1_DRC_CTRL_08_T            	pe1_drc_ctrl_08;              //0xc00247f0L
	M14B0_PE1_DRC_CTRL_09_T            	pe1_drc_ctrl_09;              //0xc00247f4L
	M14B0_PE1_DRC_CTRL_10_T            	pe1_drc_ctrl_10;              //0xc00247f8L
	M14B0_PE1_DRC_CTRL_11_T            	pe1_drc_ctrl_11;              //0xc00247fcL
	M14B0_PE1_DRC_CTRL_12_T            	pe1_drc_ctrl_12;              //0xc0024800L
	M14B0_PE1_DRC_CTRL_13_T            	pe1_drc_ctrl_13;              //0xc0024804L
	M14B0_PE1_DRC_CTRL_14_T            	pe1_drc_ctrl_14;              //0xc0024808L
	M14B0_PE1_DRC_CTRL_15_T            	pe1_drc_ctrl_15;              //0xc002480cL
	M14B0_PE1_DRC_CTRL_16_T            	pe1_drc_ctrl_16;              //0xc0024810L
	M14B0_PE1_DRC_CTRL_17_T            	pe1_drc_ctrl_17;              //0xc0024814L
	M14B0_PE1_DRC_CTRL_18_T            	pe1_drc_ctrl_18;              //0xc0024818L
	M14B0_PE1_DRC_CTRL_19_T            	pe1_drc_ctrl_19;              //0xc002481cL
	M14B0_PE1_DRC_CTRL_20_T            	pe1_drc_ctrl_20;              //0xc0024820L
	M14B0_PE1_DRC_CTRL_21_T            	pe1_drc_ctrl_21;              //0xc0024824L
	M14B0_PE1_DRC_CTRL_22_T            	pe1_drc_ctrl_22;              //0xc0024828L
	M14B0_PE1_DRC_CTRL_23_T            	pe1_drc_ctrl_23;              //0xc002482cL
	M14B0_PE1_DRC_CTRL_24_T            	pe1_drc_ctrl_24;              //0xc0024830L
	M14B0_PE1_DRC_CTRL_25_T            	pe1_drc_ctrl_25;              //0xc0024834L
	M14B0_PE1_DRC_CTRL_26_T            	pe1_drc_ctrl_26;              //0xc0024838L
	M14B0_PE1_DRC_CTRL_27_T            	pe1_drc_ctrl_27;              //0xc002483cL
	M14B0_PE1_DRC_CTRL_28_T            	pe1_drc_ctrl_28;              //0xc0024840L
	M14B0_PE1_DRC_CTRL_29_T            	pe1_drc_ctrl_29;              //0xc0024844L
	M14B0_PE1_DRC_CTRL_30_T            	pe1_drc_ctrl_30;              //0xc0024848L
	M14B0_PE1_DRC_CTRL_31_T            	pe1_drc_ctrl_31;              //0xc002484cL
	M14B0_PE1_DRC_CTRL_32_T            	pe1_drc_ctrl_32;              //0xc0024850L
	M14B0_PE1_DRC_CTRL_33_T            	pe1_drc_ctrl_33;              //0xc0024854L
	M14B0_PE1_DRC_CTRL_34_T            	pe1_drc_ctrl_34;              //0xc0024858L
	UINT32                             	reserved28;                   //0xc002485cL
	M14B0_PE1_SRE_CTRL_00_T            	pe1_sre_ctrl_00;              //0xc0024860L
	M14B0_PE1_SRE_CTRL_01_T            	pe1_sre_ctrl_01;              //0xc0024864L
	M14B0_PE1_SRE_CTRL_02_T            	pe1_sre_ctrl_02;              //0xc0024868L
	M14B0_PE1_SRE_CTRL_03_T            	pe1_sre_ctrl_03;              //0xc002486cL
	M14B0_PE1_SRE_CTRL_04_T            	pe1_sre_ctrl_04;              //0xc0024870L
	M14B0_PE1_SRE_CTRL_05_T            	pe1_sre_ctrl_05;              //0xc0024874L
	M14B0_PE1_SRE_CTRL_06_T            	pe1_sre_ctrl_06;              //0xc0024878L
	M14B0_PE1_SRE_CTRL_07_T            	pe1_sre_ctrl_07;              //0xc002487cL
	M14B0_PE1_SRE_CTRL_08_T            	pe1_sre_ctrl_08;              //0xc0024880L
	M14B0_PE1_SRE_CTRL_09_T            	pe1_sre_ctrl_09;              //0xc0024884L
	M14B0_PE1_SRE_CTRL_0A_T            	pe1_sre_ctrl_0a;              //0xc0024888L
	M14B0_PE1_SRE_CTRL_0B_T            	pe1_sre_ctrl_0b;              //0xc002488cL
	M14B0_PE1_SRE_CTRL_0C_T            	pe1_sre_ctrl_0c;              //0xc0024890L
	M14B0_PE1_SRE_CTRL_0D_T            	pe1_sre_ctrl_0d;              //0xc0024894L
	M14B0_PE1_SRE_CTRL_0E_T            	pe1_sre_ctrl_0e;              //0xc0024898L
	M14B0_PE1_SRE_CTRL_0F_T            	pe1_sre_ctrl_0f;              //0xc002489cL
	M14B0_PE1_SRE_CTRL_10_T            	pe1_sre_ctrl_10;              //0xc00248a0L
	M14B0_PE1_SRE_CTRL_11_T            	pe1_sre_ctrl_11;              //0xc00248a4L
	UINT32                             	reserved29[2];                //0xc00248a8L~0xc00248acL
	M14B0_PE1_SRE_IA_CTRL_T            	pe1_sre_ia_ctrl;              //0xc00248b0L
	M14B0_PE1_SRE_IA_DATA_T            	pe1_sre_ia_data;              //0xc00248b4L
	UINT32                             	reserved30[2];                //0xc00248b8L~0xc00248bcL
	M14B0_PE1_FSW_CTRL_00_T            	pe1_fsw_ctrl_00;              //0xc00248c0L
	M14B0_PE1_FSW_CTRL_01_T            	pe1_fsw_ctrl_01;              //0xc00248c4L
	M14B0_PE1_FSW_CTRL_02_T            	pe1_fsw_ctrl_02;              //0xc00248c8L
	M14B0_PE1_FSW_CTRL_03_T            	pe1_fsw_ctrl_03;              //0xc00248ccL
} DE_PE1_REG_M14B0_T;

#endif
