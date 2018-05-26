#ifndef _PE_CCO_REG_H15A0_H_
#define _PE_CCO_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc8025000L pe1_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0
	UINT32 load_enable                        : 1 ;  //     1
	UINT32 load_type                          : 1 ;  //     2
} PE_H15A0_CE_PE1_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025004L pe1_top_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 width                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 height                             : 12;  // 27:16
} PE_H15A0_CE_PE1_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025008L pe1_top_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_position                       : 1 ;  //     0
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 pe1_detour                         : 1 ;  //     2
	UINT32                                    : 2 ;  //  4: 3     reserved
	UINT32 async_delay_mode                   : 1 ;  //     5
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 mode_3d                            : 1 ;  //     7
	UINT32 vdpulse_pos                        : 12;  // 19: 8
	UINT32 line_masking_mode                  : 2 ;  // 21:20
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 master_en                          : 1 ;  //    24
} PE_H15A0_CE_PE1_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802500cL pe1_top_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt                               : 12;  // 11: 0
	UINT32 va_pe1_in                          : 1 ;  //    12
	UINT32 va_pe1_out                         : 1 ;  //    13
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 vcnt2                              : 12;  // 27:16
} PE_H15A0_CE_PE1_TOP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025010L pe1_top_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_to_cnt                           : 12;  // 11: 0
} PE_H15A0_CE_PE1_TOP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025020L pe1_fsw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_s_status_0                     : 8 ;  //  7: 0
	UINT32 reg_s_status_1                     : 8 ;  // 15: 8
	UINT32 reg_s_apl_0                        : 8 ;  // 23:16
	UINT32 reg_fmd_en_0                       : 1 ;  //    24
	UINT32 reg_fmd_en_1                       : 1 ;  //    25
	UINT32 reg_tnr_auto_en                    : 1 ;  //    26
	UINT32 cl_filter_en_stat                  : 1 ;  //    27
	UINT32 clc_detection_en_stat              : 1 ;  //    28
	UINT32 reg_color_filter_status            : 2 ;  // 30:29
	UINT32 reg_shp_fw_en                      : 1 ;  //    31
} PE_H15A0_CE_PE1_FSW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025024L pe1_fsw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_histo_fw_en                    : 1 ;  //     0
	UINT32 reg_drc_dc_mode                    : 2 ;  //  2: 1
	UINT32 reg_drc_dc_bypass                  : 1 ;  //     3
	UINT32 reg_drc_dc_sc_cnt                  : 4 ;  //  7: 4
	UINT32 pre_121_blur_en                    : 1 ;  //     8
	UINT32 pre_median_en                      : 1 ;  //     9
	UINT32 tnr_sc_measure_en                  : 1 ;  //    10
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 tnr_sc_measure_th                  : 16;  // 31:16
} PE_H15A0_CE_PE1_FSW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025028L pe1_fsw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_info                       : 6 ;  //  5: 0
	UINT32 reg_src_freeze                     : 1 ;  //     6
} PE_H15A0_CE_PE1_FSW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802502cL pe1_fsw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_nt_lvl                         : 8 ;  //  7: 0
	UINT32 reg_s_status                       : 8 ;  // 15: 8
	UINT32 reg_s_apl                          : 8 ;  // 23:16
} PE_H15A0_CE_PE1_FSW_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025034L pe1_win2_ctrl_00 ''
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
} PE_H15A0_CE_PE1_WIN2_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025038L pe1_win2_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16
} PE_H15A0_CE_PE1_WIN2_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc802503cL pe1_win2_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16
} PE_H15A0_CE_PE1_WIN2_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8025040L pe1_win2_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16
} PE_H15A0_CE_PE1_WIN2_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025044L pe1_win2_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16
} PE_H15A0_CE_PE1_WIN2_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025050L pe1_apl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x0                    : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 apl_win_ctrl_y0                    : 12;  // 27:16
} PE_H15A0_CE_PE1_APL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025054L pe1_apl_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x1                    : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 apl_win_ctrl_y1                    : 12;  // 27:16
} PE_H15A0_CE_PE1_APL_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025058L pe1_apl_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_position                       : 2 ;  //  1: 0
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 apl_y_type                         : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 act_win_mode                       : 1 ;  //     8
} PE_H15A0_CE_PE1_APL_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802505cL pe1_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_cb                             : 10;  //  9: 0
	UINT32 apl_yg                             : 10;  // 19:10
	UINT32 apl_cr                             : 10;  // 29:20
} PE_H15A0_CE_PE1_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8025060L pe1_apl_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_cb_other_side                  : 10;  //  9: 0
	UINT32 apl_yg_other_side                  : 10;  // 19:10
	UINT32 apl_cr_other_side                  : 10;  // 29:20
} PE_H15A0_CE_PE1_APL_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8025070L pe1_tpg_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_enable                         : 1 ;  //     0
	UINT32 pattern_type                       : 2 ;  //  2: 1
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 blend_alpha                        : 4 ;  //  7: 4
	UINT32 blt_size_x                         : 12;  // 19: 8
	UINT32 blt_size_y                         : 12;  // 31:20
} PE_H15A0_CE_PE1_TPG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025074L pe1_tpg_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gradation_stride                   : 7 ;  //  6: 0
	UINT32 gradation_direction                : 1 ;  //     7
	UINT32 gradation_bar_0_en                 : 1 ;  //     8
	UINT32 gradation_bar_1_en                 : 1 ;  //     9
	UINT32 gradation_bar_2_en                 : 1 ;  //    10
	UINT32 gradation_bar_3_en                 : 1 ;  //    11
} PE_H15A0_CE_PE1_TPG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025078L pe1_tpg_ctrl_02 ''
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
} PE_H15A0_CE_PE1_TPG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802507cL pe1_tpg_ctrl_03 ''
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
} PE_H15A0_CE_PE1_TPG_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025080L pe1_tpg_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 10;  //  9: 0
	UINT32 bg_color_b                         : 10;  // 19:10
	UINT32 bg_color_g                         : 10;  // 29:20
} PE_H15A0_CE_PE1_TPG_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025084L pe1_tpg_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_indir_addr                     : 7 ;  //  6: 0
	UINT32                                    : 5 ;  // 11: 7     reserved
	UINT32 tpg_indir_ai_en                    : 1 ;  //    12
} PE_H15A0_CE_PE1_TPG_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025088L pe1_tpg_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_indir_data                     : 32;  // 31: 0
} PE_H15A0_CE_PE1_TPG_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc80250a0L pe1_vspyc_ctrl_00 ''
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
} PE_H15A0_CE_PE1_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80250a4L pe1_vspyc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 saturation                         : 8 ;  // 15: 8
	UINT32 brightness                         : 10;  // 25:16
} PE_H15A0_CE_PE1_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80250b0L pe1_debug_ctrl_00 ''
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
} PE_H15A0_CE_PE1_DEBUG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80250b4L pe1_debug_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 force_yy_enable                    : 1 ;  //     1
	UINT32 force_cb_enable                    : 1 ;  //     2
	UINT32 force_cr_enable                    : 1 ;  //     3
	UINT32 yc_debug_mode_sel                  : 3 ;  //  6: 4
} PE_H15A0_CE_PE1_DEBUG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80250b8L pe1_debug_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 10;  //  9: 0     reserved
	UINT32 force_cb                           : 10;  // 19:10
	UINT32 force_cr                           : 10;  // 29:20
} PE_H15A0_CE_PE1_DEBUG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80250e0L pe1_cen_ctrl_00 ''
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
} PE_H15A0_CE_PE1_CEN_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80250e4L pe1_cen_ctrl_01 ''
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
} PE_H15A0_CE_PE1_CEN_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80250e8L pe1_cen_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ihsv_sgain                         : 8 ;  //  7: 0
	UINT32 ihsv_vgain                         : 8 ;  // 15: 8
} PE_H15A0_CE_PE1_CEN_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80250ecL pe1_cen_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ihsv_hoffset                       : 8 ;  //  7: 0
	UINT32 ihsv_soffset                       : 8 ;  // 15: 8
	UINT32 ihsv_voffset                       : 8 ;  // 23:16
} PE_H15A0_CE_PE1_CEN_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc80250f0L pe1_cen_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr0                : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr1                : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr2                : 8 ;  // 23:16
	UINT32 reg_master_gain_cr3                : 8 ;  // 31:24
} PE_H15A0_CE_PE1_CEN_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80250f4L pe1_cen_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr4                : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr5                : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr6                : 8 ;  // 23:16
	UINT32 reg_master_gain_cr7                : 8 ;  // 31:24
} PE_H15A0_CE_PE1_CEN_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc80250f8L pe1_cen_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr8                : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr9                : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr10               : 8 ;  // 23:16
	UINT32 reg_master_gain_cr11               : 8 ;  // 31:24
} PE_H15A0_CE_PE1_CEN_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc80250fcL pe1_cen_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr12               : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr13               : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr14               : 8 ;  // 23:16
	UINT32 reg_master_gain_cr15               : 8 ;  // 31:24
} PE_H15A0_CE_PE1_CEN_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025110L pe1_cen_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_address                    : 8 ;  //  7: 0
	UINT32 hif_cen_ai_sel                     : 3 ;  // 10: 8
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 hif_cen_ai                         : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_cen_enable                     : 1 ;  //    15
} PE_H15A0_CE_PE1_CEN_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025114L pe1_cen_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_y_wdata                    : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_cen_x_wdata                    : 10;  // 25:16
} PE_H15A0_CE_PE1_CEN_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025118L pe1_cen_delta_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_delta_address              : 5 ;  //  4: 0
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_cen_delta_ai                   : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_cen_delta_enable               : 1 ;  //    15
} PE_H15A0_CE_PE1_CEN_DELTA_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc802511cL pe1_cen_delta_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_vr_data                    : 8 ;  //  7: 0
	UINT32 hif_cen_sb_data                    : 8 ;  // 15: 8
	UINT32 hif_cen_hg_data                    : 8 ;  // 23:16
} PE_H15A0_CE_PE1_CEN_DELTA_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025120L pe1_dce_ctrl_00 ''
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
} PE_H15A0_CE_PE1_DCE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025124L pe1_dce_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_bin_min_position              : 8 ;  //  7: 0
	UINT32 hist_bin_max_position              : 8 ;  // 15: 8
	UINT32 color_region_gain                  : 8 ;  // 23:16
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 blur_v_gain                        : 4 ;  // 31:28
} PE_H15A0_CE_PE1_DCE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025128L pe1_dce_ctrl_02 ''
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
} PE_H15A0_CE_PE1_DCE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802512cL pe1_dce_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_range_min                        : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 y_range_max                        : 10;  // 25:16
} PE_H15A0_CE_PE1_DCE_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025130L pe1_dce_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_range_min                       : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 cb_range_max                       : 10;  // 25:16
} PE_H15A0_CE_PE1_DCE_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025134L pe1_dce_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_range_min                       : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 cr_range_max                       : 10;  // 25:16
} PE_H15A0_CE_PE1_DCE_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025138L pe1_dce_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_address                    : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 hif_dce_load                       : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 hif_dce_ai                         : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_dce_enable                     : 1 ;  //    15
} PE_H15A0_CE_PE1_DCE_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc802513cL pe1_dce_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_wdata_y                    : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dyc_wdata_x                    : 10;  // 25:16
} PE_H15A0_CE_PE1_DCE_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025140L pe1_dce_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_wdata_y_32nd               : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dyc_wdata_x_32nd               : 10;  // 25:16
} PE_H15A0_CE_PE1_DCE_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025144L pe1_dce_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 dce_ktd_csc_sel                    : 1 ;  //     1
} PE_H15A0_CE_PE1_DCE_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025148L pe1_dce_hist_ia_ctrl ''
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
} PE_H15A0_CE_PE1_DCE_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc802514cL pe1_dce_hist_ia_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_DCE_HIST_IA_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8025150L pe1_dce_hist_ia_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_DCE_HIST_IA_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8025160L pe1_dse_ctrl_00 ''
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
} PE_H15A0_CE_PE1_DSE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025164L pe1_dse_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 color_region_gain                  : 8 ;  // 31:24
} PE_H15A0_CE_PE1_DSE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025168L pe1_dse_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_wdata_y_32nd               : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dse_wdata_x_32nd               : 10;  // 25:16
} PE_H15A0_CE_PE1_DSE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802516cL pe1_dse_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_address                    : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 hif_dse_load                       : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 hif_dse_ai                         : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_dse_enable                     : 1 ;  //    15
} PE_H15A0_CE_PE1_DSE_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025170L pe1_dse_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_wdata_y                    : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dse_wdata_x                    : 10;  // 25:16
} PE_H15A0_CE_PE1_DSE_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025180L pe1_cw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cw_en                              : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 cw_gain_selection                  : 1 ;  //     4
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 reg_cw_y4                          : 8 ;  // 23:16
	UINT32 reg_cw_x4                          : 8 ;  // 31:24
} PE_H15A0_CE_PE1_CW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025184L pe1_cw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 user_ctrl_g_gain                   : 8 ;  //  7: 0
	UINT32 user_ctrl_b_gain                   : 8 ;  // 15: 8
	UINT32 user_ctrl_r_gain                   : 8 ;  // 23:16
} PE_H15A0_CE_PE1_CW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025188L pe1_cw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cw_y2                          : 8 ;  //  7: 0
	UINT32 reg_cw_x2                          : 8 ;  // 15: 8
	UINT32 reg_cw_y3                          : 8 ;  // 23:16
	UINT32 reg_cw_x3                          : 8 ;  // 31:24
} PE_H15A0_CE_PE1_CW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802518cL pe1_cw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cw_y0                          : 8 ;  //  7: 0
	UINT32 reg_cw_x0                          : 8 ;  // 15: 8
	UINT32 reg_cw_y1                          : 8 ;  // 23:16
	UINT32 reg_cw_x1                          : 8 ;  // 31:24
} PE_H15A0_CE_PE1_CW_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025190L pe1_cw_ctrl_04 ''
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
} PE_H15A0_CE_PE1_CW_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80251a0L pe1_conv_ctrl_00 ''
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
} PE_H15A0_CE_PE1_CONV_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80251a4L pe1_conv_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_r                           : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 offset_l                           : 8 ;  // 23:16
} PE_H15A0_CE_PE1_CONV_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80251a8L pe1_conv_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_grey                      : 8 ;  //  7: 0
} PE_H15A0_CE_PE1_CONV_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80251acL pe1_conv_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CE_PE1_CONV_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc80251b0L pe1_conv_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CE_PE1_CONV_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80251b4L pe1_conv_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_CONV_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc80251b8L pe1_conv_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_CONV_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc80251bcL pe1_conv_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CE_PE1_CONV_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc80251c0L pe1_conv_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CE_PE1_CONV_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc80251c4L pe1_conv_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CE_PE1_CONV_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc80251c8L pe1_conv_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_CONV_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc80251ccL pe1_conv_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_CONV_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc80251d0L pe1_conv_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CE_PE1_CONV_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc80251d4L pe1_conv_ctrl_0d ''
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
} PE_H15A0_CE_PE1_CONV_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc80251d8L pe1_conv_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_l_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_l_window_v_en                  : 1 ;  //    31
} PE_H15A0_CE_PE1_CONV_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc80251dcL pe1_conv_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_CONV_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc80251e0L pe1_conv_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_r_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_r_window_v_en                  : 1 ;  //    31
} PE_H15A0_CE_PE1_CONV_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc80251e4L pe1_conv_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_CONV_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc80251e8L pe1_conv_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 8 ;  //  7: 0
	UINT32 bg_color_b                         : 8 ;  // 15: 8
	UINT32 bg_color_g                         : 8 ;  // 23:16
} PE_H15A0_CE_PE1_CONV_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc80251f0L pe1_mute_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CE_PE1_MUTE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80251f4L pe1_mute_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CE_PE1_MUTE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80251f8L pe1_mute_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_MUTE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80251fcL pe1_mute_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_MUTE_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025200L pe1_mute_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CE_PE1_MUTE_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025204L pe1_mute_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CE_PE1_MUTE_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025208L pe1_mute_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CE_PE1_MUTE_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc802520cL pe1_mute_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_MUTE_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025210L pe1_mute_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CE_PE1_MUTE_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025214L pe1_mute_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CE_PE1_MUTE_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025218L pe1_mute_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4
	UINT32 start_parity                       : 1 ;  //     6
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16
	UINT32 alpha0_gain                        : 8 ;  // 31:24
} PE_H15A0_CE_PE1_MUTE_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc802521cL pe1_mute_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_l_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_l_window_v_en                  : 1 ;  //    31
} PE_H15A0_CE_PE1_MUTE_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc8025220L pe1_mute_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_MUTE_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc8025224L pe1_mute_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_r_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_r_window_v_en                  : 1 ;  //    31
} PE_H15A0_CE_PE1_MUTE_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc8025228L pe1_mute_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_MUTE_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc802522cL pe1_mute_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 8 ;  //  7: 0
	UINT32 bg_color_b                         : 8 ;  // 15: 8
	UINT32 bg_color_g                         : 8 ;  // 23:16
} PE_H15A0_CE_PE1_MUTE_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc8025240L pe1_tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025244L pe1_tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 window_h_en                        : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 window_v_en                        : 1 ;  //    31
} PE_H15A0_CE_PE1_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025248L pe1_tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802524cL pe1_tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s_status                           : 8 ;  //  7: 0
} PE_H15A0_CE_PE1_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc802525cL pe1_ocsc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outputcsc_sel                      : 1 ;  //     0
} PE_H15A0_CE_PE1_OCSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025260L pe1_xvy_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outputcsc_en                       : 1 ;  //     0
	UINT32 degamma_en                         : 1 ;  //     1
	UINT32 pcc_en                             : 1 ;  //     2
	UINT32 gamma_en                           : 1 ;  //     3
	UINT32 scaler_en                          : 1 ;  //     4
	UINT32 hsv_hsl_selection                  : 1 ;  //     5
	UINT32 s_adpative_scaler_enable           : 1 ;  //     6
} PE_H15A0_CE_PE1_XVY_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025264L pe1_xvy_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef0                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025268L pe1_xvy_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef2                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802526cL pe1_xvy_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef4                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025270L pe1_xvy_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef6                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025274L pe1_xvy_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_csc_coef8                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025278L pe1_xvy_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_csc_ofst1                        : 11;  // 26:16
} PE_H15A0_CE_PE1_XVY_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc802527cL pe1_xvy_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4                        : 14;  // 13: 0
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 r_csc_ofst3                        : 14;  // 29:16
} PE_H15A0_CE_PE1_XVY_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025280L pe1_xvy_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef0                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_ofst5                        : 14;  // 29:16
} PE_H15A0_CE_PE1_XVY_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025284L pe1_xvy_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef2                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef1                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025288L pe1_xvy_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef4                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef3                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc802528cL pe1_xvy_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef6                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef5                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc8025290L pe1_xvy_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef8                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef7                        : 15;  // 30:16
} PE_H15A0_CE_PE1_XVY_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc8025294L pe1_xvy_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scaler                             : 8 ;  //  7: 0
} PE_H15A0_CE_PE1_XVY_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc80252a0L pe1_lrcr_ctrl_00 ''
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
} PE_H15A0_CE_PE1_LRCR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80252a4L pe1_lrcr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 th_max_hist                        : 8 ;  //  7: 0
	UINT32 th_valid_bins                      : 8 ;  // 15: 8
	UINT32 th1                                : 8 ;  // 23:16
	UINT32 th0                                : 8 ;  // 31:24
} PE_H15A0_CE_PE1_LRCR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80252a8L pe1_lrcr_ctrl_02 ''
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
} PE_H15A0_CE_PE1_LRCR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80252acL pe1_lrcr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_LRCR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc80252b0L pe1_lrcr_ctrl_04 ''
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
} PE_H15A0_CE_PE1_LRCR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80252b4L pe1_lrcr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_LRCR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc80252b8L pe1_lrcr_ctrl_06 ''
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
} PE_H15A0_CE_PE1_LRCR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc80252bcL pe1_lrcr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CE_PE1_LRCR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc80252c0L pe1_lrcr_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0
	UINT32 n_valid_bins                       : 8 ;  // 15: 8
	UINT32 cdf_x1                             : 8 ;  // 23:16
	UINT32 cdf_x0                             : 8 ;  // 31:24
} PE_H15A0_CE_PE1_LRCR_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc80252c4L pe1_lrcr_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0
	UINT32 n_valid_bins                       : 8 ;  // 15: 8
	UINT32 cdf_x1                             : 8 ;  // 23:16
	UINT32 cdf_x0                             : 8 ;  // 31:24
} PE_H15A0_CE_PE1_LRCR_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc80252c8L pe1_lrcr_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0
	UINT32 n_valid_bins                       : 8 ;  // 15: 8
	UINT32 cdf_x1                             : 8 ;  // 23:16
	UINT32 cdf_x0                             : 8 ;  // 31:24
} PE_H15A0_CE_PE1_LRCR_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc80252d0L pe1_l_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_g_available                : 1 ;  //    28
} PE_H15A0_CE_PE1_L_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80252d4L pe1_l_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_L_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc80252d8L pe1_l_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_b_available                : 1 ;  //    28
} PE_H15A0_CE_PE1_L_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80252dcL pe1_l_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_L_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc80252e0L pe1_l_r_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_r_available                : 1 ;  //    28
} PE_H15A0_CE_PE1_L_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80252e4L pe1_l_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_L_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc80252e8L pe1_r_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_g_available                : 1 ;  //    28
} PE_H15A0_CE_PE1_R_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80252ecL pe1_r_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_R_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc80252f0L pe1_r_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_b_available                : 1 ;  //    28
} PE_H15A0_CE_PE1_R_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80252f4L pe1_r_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_R_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc80252f8L pe1_r_r_hist_ia_ctrl ''
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
} PE_H15A0_CE_PE1_R_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80252fcL pe1_r_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CE_PE1_R_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025310L pe1_bt20_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef1                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef0                        : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025314L pe1_bt20_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef3a                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef2                        : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025318L pe1_bt20_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef5a                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef4a                       : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802531cL pe1_bt20_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef7a                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef6a                       : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025320L pe1_bt20_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_ofst0                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_2x3_coef8a                       : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025324L pe1_bt20_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_ofst2                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_2x3_ofst1                        : 11;  // 26:16
} PE_H15A0_CE_PE1_BT20_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025328L pe1_bt20_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_ofst3                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_2x3_ofst4                        : 11;  // 26:16
} PE_H15A0_CE_PE1_BT20_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc802532cL pe1_bt20_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_en                           : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 r_degamma_en                       : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 r_1x3_en                           : 1 ;  //     8
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 r_2x3_ofst5                        : 11;  // 26:16
} PE_H15A0_CE_PE1_BT20_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025330L pe1_bt20_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dither_en                          : 1 ;  //     0
	UINT32 decontour_en                       : 1 ;  //     1
	UINT32 dither_random_freeze_en            : 1 ;  //     2
	UINT32 demo_pattern_enable                : 1 ;  //     3
	UINT32 bit_mode                           : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 decontour_gain_r                   : 8 ;  // 15: 8
	UINT32 decontour_gain_g                   : 8 ;  // 23:16
	UINT32 decontour_gain_b                   : 8 ;  // 31:24
} PE_H15A0_CE_PE1_BT20_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025334L pe1_bt20_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef3b                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_csc_cb_th                      : 10;  // 25:16
} PE_H15A0_CE_PE1_BT20_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025338L pe1_bt20_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef5b                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef4b                       : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc802533cL pe1_bt20_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef7b                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef6b                       : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc8025340L pe1_bt20_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_csc_cr_th                      : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 r_2x3_coef8b                       : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc8025344L pe1_bt20_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef1                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef0                        : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc8025348L pe1_bt20_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef3                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef2                        : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc802534cL pe1_bt20_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef5                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef4                        : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc8025350L pe1_bt20_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef7                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef6                        : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc8025354L pe1_bt20_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 r_1x3_coef8                        : 15;  // 30:16
} PE_H15A0_CE_PE1_BT20_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8025358L pe1_bt20_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_ofst1                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_1x3_ofst0                        : 11;  // 26:16
} PE_H15A0_CE_PE1_BT20_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc802535cL pe1_bt20_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_ofst3                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_1x3_ofst2                        : 11;  // 26:16
} PE_H15A0_CE_PE1_BT20_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc8025360L pe1_bt20_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_ofst5                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_1x3_ofst4                        : 11;  // 26:16
} PE_H15A0_CE_PE1_BT20_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc8025370L pe1_drc_ctrl_00 ''
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
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_mode_2ch                       : 1 ;  //    20
	UINT32                                    : 7 ;  // 27:21     reserved
	UINT32 reg_dbg_disp_blf                   : 1 ;  //    28
	UINT32 reg_dbg_disp_lcont                 : 1 ;  //    29
	UINT32 reg_dbg_disp_reflect               : 1 ;  //    30
} PE_H15A0_CE_PE1_DRC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025374L pe1_drc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_rw_sel                  : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_drc_gm_op_sel                  : 1 ;  //     4
} PE_H15A0_CE_PE1_DRC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025378L pe1_drc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bnh                        : 7 ;  //  6: 0
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 reg_drc_bnv                        : 6 ;  // 21:16
} PE_H15A0_CE_PE1_DRC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802537cL pe1_drc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bsh                        : 9 ;  //  8: 0
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_drc_bsv                        : 9 ;  // 24:16
} PE_H15A0_CE_PE1_DRC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025380L pe1_drc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_apl2                    : 10;  //  9: 0
	UINT32 reg_drc_gm_apl1                    : 10;  // 19:10
	UINT32 reg_drc_gm_apl0                    : 10;  // 29:20
} PE_H15A0_CE_PE1_DRC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025384L pe1_drc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_apl5                    : 10;  //  9: 0
	UINT32 reg_drc_gm_apl4                    : 10;  // 19:10
	UINT32 reg_drc_gm_apl3                    : 10;  // 29:20
} PE_H15A0_CE_PE1_DRC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025388L pe1_drc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_blf_wei_cen                : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_drc_apl_iir                    : 7 ;  // 22:16
} PE_H15A0_CE_PE1_DRC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc802538cL pe1_drc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_shp_clip_n                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_drc_shp_clip_p                 : 12;  // 27:16
} PE_H15A0_CE_PE1_DRC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025390L pe1_drc_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_gn                     : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_th                     : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025394L pe1_drc_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_adj_l                  : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_drc_apl_adj_h                  : 8 ;  // 23:16
} PE_H15A0_CE_PE1_DRC_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025398L pe1_drc_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x2_p                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x1_p                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc802539cL pe1_drc_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x4_p                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x3_p                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc80253a0L pe1_drc_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y1_p                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y0_p                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc80253a4L pe1_drc_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y3_p                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y2_p                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc80253a8L pe1_drc_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y5_p                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y4_p                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_14_T;

/*-----------------------------------------------------------------------------
		0xc80253acL pe1_drc_ctrl_15 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x2_n                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x1_n                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_15_T;

/*-----------------------------------------------------------------------------
		0xc80253b0L pe1_drc_ctrl_16 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_x4_n                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_x3_n                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_16_T;

/*-----------------------------------------------------------------------------
		0xc80253b4L pe1_drc_ctrl_17 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y1_n                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y0_n                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_17_T;

/*-----------------------------------------------------------------------------
		0xc80253b8L pe1_drc_ctrl_18 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y3_n                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y2_n                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_18_T;

/*-----------------------------------------------------------------------------
		0xc80253bcL pe1_drc_ctrl_19 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_apl_y5_n                   : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_drc_apl_y4_n                   : 10;  // 25:16
} PE_H15A0_CE_PE1_DRC_CTRL_19_T;

/*-----------------------------------------------------------------------------
		0xc80253c0L pe1_drc_ctrl_20 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_lut_addr_clr               : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_drc_hfc_rd_sel                 : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_drc_vfc_rd_sel                 : 1 ;  //     8
} PE_H15A0_CE_PE1_DRC_CTRL_20_T;

/*-----------------------------------------------------------------------------
		0xc80253c4L pe1_drc_ctrl_21 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_lk_blf_hfc_data            : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_21_T;

/*-----------------------------------------------------------------------------
		0xc80253c8L pe1_drc_ctrl_22 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_lk_blf_vfc_data            : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_22_T;

/*-----------------------------------------------------------------------------
		0xc80253ccL pe1_drc_ctrl_23 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_hfc_rdata                  : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_23_T;

/*-----------------------------------------------------------------------------
		0xc80253d0L pe1_drc_ctrl_24 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_vfc_rdata                  : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_24_T;

/*-----------------------------------------------------------------------------
		0xc80253d4L pe1_drc_ctrl_25 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_blf_wei_wdata              : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_25_T;

/*-----------------------------------------------------------------------------
		0xc80253d8L pe1_drc_ctrl_26 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_blf_wei_rdata              : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_26_T;

/*-----------------------------------------------------------------------------
		0xc80253dcL pe1_drc_ctrl_27 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_wdata                   : 30;  // 29: 0
} PE_H15A0_CE_PE1_DRC_CTRL_27_T;

/*-----------------------------------------------------------------------------
		0xc80253e0L pe1_drc_ctrl_28 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_rdata                   : 30;  // 29: 0
} PE_H15A0_CE_PE1_DRC_CTRL_28_T;

/*-----------------------------------------------------------------------------
		0xc80253e4L pe1_drc_ctrl_29 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_wdata                   : 30;  // 29: 0
} PE_H15A0_CE_PE1_DRC_CTRL_29_T;

/*-----------------------------------------------------------------------------
		0xc80253e8L pe1_drc_ctrl_30 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_gm_rdata                   : 30;  // 29: 0
} PE_H15A0_CE_PE1_DRC_CTRL_30_T;

/*-----------------------------------------------------------------------------
		0xc80253ecL pe1_drc_ctrl_31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_hpf_wei_wdata              : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_31_T;

/*-----------------------------------------------------------------------------
		0xc80253f0L pe1_drc_ctrl_32 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_hpf_wei_rdata              : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_32_T;

/*-----------------------------------------------------------------------------
		0xc80253f4L pe1_drc_ctrl_33 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bl_apl_raddr_h             : 7 ;  //  6: 0
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 reg_drc_bl_apl_raddr_v             : 6 ;  // 21:16
} PE_H15A0_CE_PE1_DRC_CTRL_33_T;

/*-----------------------------------------------------------------------------
		0xc80253f8L pe1_drc_ctrl_34 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_drc_bl_apl_rdata               : 10;  //  9: 0
} PE_H15A0_CE_PE1_DRC_CTRL_34_T;

/*-----------------------------------------------------------------------------
		0xc8025800L pe1_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  //     0
	UINT32 load_enable                        : 1 ;  //     1
	UINT32 load_type                          : 1 ;  //     2
} PE_H15A0_CO_PE1_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025804L pe1_top_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 width                              : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 height                             : 12;  // 27:16
} PE_H15A0_CO_PE1_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025808L pe1_top_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_position                       : 1 ;  //     0
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 pe1_detour                         : 1 ;  //     2
	UINT32                                    : 2 ;  //  4: 3     reserved
	UINT32 async_delay_mode                   : 1 ;  //     5
	UINT32                                    : 1 ;  //     6     reserved
	UINT32 mode_3d                            : 1 ;  //     7
	UINT32 vdpulse_pos                        : 12;  // 19: 8
	UINT32 line_masking_mode                  : 2 ;  // 21:20
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 master_en                          : 1 ;  //    24
} PE_H15A0_CO_PE1_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802580cL pe1_top_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt                               : 12;  // 11: 0
	UINT32 va_pe1_in                          : 1 ;  //    12
	UINT32 va_pe1_out                         : 1 ;  //    13
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 vcnt2                              : 12;  // 27:16
} PE_H15A0_CO_PE1_TOP_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025810L pe1_top_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_to_cnt                           : 12;  // 11: 0
} PE_H15A0_CO_PE1_TOP_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025820L pe1_fsw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gsc_y_h_idx                        : 8 ;  //  7: 0
	UINT32 gsc_y_v_idx                        : 8 ;  // 15: 8
	UINT32 gsc_c_h_idx                        : 8 ;  // 23:16
	UINT32 gsc_c_v_idx                        : 8 ;  // 31:24
} PE_H15A0_CO_PE1_FSW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025824L pe1_fsw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 psc_y_h_idx                        : 8 ;  //  7: 0
	UINT32 psc_y_v_idx                        : 8 ;  // 15: 8
	UINT32 sub_y_h_idx                        : 8 ;  // 23:16
	UINT32 sub_y_v_idx                        : 8 ;  // 31:24
} PE_H15A0_CO_PE1_FSW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025828L pe1_fsw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_FSW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802582cL pe1_fsw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_FSW_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025834L pe1_win2_ctrl_00 ''
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
} PE_H15A0_CO_PE1_WIN2_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025838L pe1_win2_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x0                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y0                          : 12;  // 27:16
} PE_H15A0_CO_PE1_WIN2_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc802583cL pe1_win2_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w0_x1                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w0_y1                          : 12;  // 27:16
} PE_H15A0_CO_PE1_WIN2_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8025840L pe1_win2_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x0                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y0                          : 12;  // 27:16
} PE_H15A0_CO_PE1_WIN2_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025844L pe1_win2_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 win_w1_x1                          : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 win_w1_y1                          : 12;  // 27:16
} PE_H15A0_CO_PE1_WIN2_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025850L pe1_apl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x0                    : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 apl_win_ctrl_y0                    : 12;  // 27:16
} PE_H15A0_CO_PE1_APL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025854L pe1_apl_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x1                    : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 apl_win_ctrl_y1                    : 12;  // 27:16
} PE_H15A0_CO_PE1_APL_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025858L pe1_apl_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_position                       : 2 ;  //  1: 0
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 apl_y_type                         : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 act_win_mode                       : 1 ;  //     8
} PE_H15A0_CO_PE1_APL_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802585cL pe1_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_cb                             : 10;  //  9: 0
	UINT32 apl_yg                             : 10;  // 19:10
	UINT32 apl_cr                             : 10;  // 29:20
} PE_H15A0_CO_PE1_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8025860L pe1_apl_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_cb_other_side                  : 10;  //  9: 0
	UINT32 apl_yg_other_side                  : 10;  // 19:10
	UINT32 apl_cr_other_side                  : 10;  // 29:20
} PE_H15A0_CO_PE1_APL_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8025870L pe1_tpg_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_enable                         : 1 ;  //     0
	UINT32 pattern_type                       : 2 ;  //  2: 1
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 blend_alpha                        : 4 ;  //  7: 4
	UINT32 blt_size_x                         : 12;  // 19: 8
	UINT32 blt_size_y                         : 12;  // 31:20
} PE_H15A0_CO_PE1_TPG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025874L pe1_tpg_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 gradation_stride                   : 7 ;  //  6: 0
	UINT32 gradation_direction                : 1 ;  //     7
	UINT32 gradation_bar_0_en                 : 1 ;  //     8
	UINT32 gradation_bar_1_en                 : 1 ;  //     9
	UINT32 gradation_bar_2_en                 : 1 ;  //    10
	UINT32 gradation_bar_3_en                 : 1 ;  //    11
} PE_H15A0_CO_PE1_TPG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025878L pe1_tpg_ctrl_02 ''
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
} PE_H15A0_CO_PE1_TPG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802587cL pe1_tpg_ctrl_03 ''
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
} PE_H15A0_CO_PE1_TPG_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025880L pe1_tpg_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 10;  //  9: 0
	UINT32 bg_color_b                         : 10;  // 19:10
	UINT32 bg_color_g                         : 10;  // 29:20
} PE_H15A0_CO_PE1_TPG_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025884L pe1_tpg_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_indir_addr                     : 7 ;  //  6: 0
	UINT32                                    : 5 ;  // 11: 7     reserved
	UINT32 tpg_indir_ai_en                    : 1 ;  //    12
} PE_H15A0_CO_PE1_TPG_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025888L pe1_tpg_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tpg_indir_data                     : 32;  // 31: 0
} PE_H15A0_CO_PE1_TPG_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc80258a0L pe1_vspyc_ctrl_00 ''
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
} PE_H15A0_CO_PE1_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80258a4L pe1_vspyc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 saturation                         : 8 ;  // 15: 8
	UINT32 brightness                         : 10;  // 25:16
} PE_H15A0_CO_PE1_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80258b0L pe1_debug_ctrl_00 ''
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
} PE_H15A0_CO_PE1_DEBUG_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80258b4L pe1_debug_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 force_yy_enable                    : 1 ;  //     1
	UINT32 force_cb_enable                    : 1 ;  //     2
	UINT32 force_cr_enable                    : 1 ;  //     3
	UINT32 yc_debug_mode_sel                  : 3 ;  //  6: 4
} PE_H15A0_CO_PE1_DEBUG_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80258b8L pe1_debug_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 10;  //  9: 0     reserved
	UINT32 force_cb                           : 10;  // 19:10
	UINT32 force_cr                           : 10;  // 29:20
} PE_H15A0_CO_PE1_DEBUG_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80258e0L pe1_cen_ctrl_00 ''
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
} PE_H15A0_CO_PE1_CEN_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80258e4L pe1_cen_ctrl_01 ''
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
} PE_H15A0_CO_PE1_CEN_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80258e8L pe1_cen_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ihsv_sgain                         : 8 ;  //  7: 0
	UINT32 ihsv_vgain                         : 8 ;  // 15: 8
} PE_H15A0_CO_PE1_CEN_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80258ecL pe1_cen_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ihsv_hoffset                       : 8 ;  //  7: 0
	UINT32 ihsv_soffset                       : 8 ;  // 15: 8
	UINT32 ihsv_voffset                       : 8 ;  // 23:16
} PE_H15A0_CO_PE1_CEN_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc80258f0L pe1_cen_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr0                : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr1                : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr2                : 8 ;  // 23:16
	UINT32 reg_master_gain_cr3                : 8 ;  // 31:24
} PE_H15A0_CO_PE1_CEN_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80258f4L pe1_cen_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr4                : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr5                : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr6                : 8 ;  // 23:16
	UINT32 reg_master_gain_cr7                : 8 ;  // 31:24
} PE_H15A0_CO_PE1_CEN_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc80258f8L pe1_cen_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr8                : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr9                : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr10               : 8 ;  // 23:16
	UINT32 reg_master_gain_cr11               : 8 ;  // 31:24
} PE_H15A0_CO_PE1_CEN_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc80258fcL pe1_cen_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_master_gain_cr12               : 8 ;  //  7: 0
	UINT32 reg_master_gain_cr13               : 8 ;  // 15: 8
	UINT32 reg_master_gain_cr14               : 8 ;  // 23:16
	UINT32 reg_master_gain_cr15               : 8 ;  // 31:24
} PE_H15A0_CO_PE1_CEN_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025910L pe1_cen_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_address                    : 8 ;  //  7: 0
	UINT32 hif_cen_ai_sel                     : 3 ;  // 10: 8
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 hif_cen_ai                         : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_cen_enable                     : 1 ;  //    15
} PE_H15A0_CO_PE1_CEN_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025914L pe1_cen_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_y_wdata                    : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_cen_x_wdata                    : 10;  // 25:16
} PE_H15A0_CO_PE1_CEN_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025918L pe1_cen_delta_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_delta_address              : 5 ;  //  4: 0
	UINT32                                    : 7 ;  // 11: 5     reserved
	UINT32 hif_cen_delta_ai                   : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_cen_delta_enable               : 1 ;  //    15
} PE_H15A0_CO_PE1_CEN_DELTA_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc802591cL pe1_cen_delta_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_cen_vr_data                    : 8 ;  //  7: 0
	UINT32 hif_cen_sb_data                    : 8 ;  // 15: 8
	UINT32 hif_cen_hg_data                    : 8 ;  // 23:16
} PE_H15A0_CO_PE1_CEN_DELTA_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025920L pe1_dce_ctrl_00 ''
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
} PE_H15A0_CO_PE1_DCE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025924L pe1_dce_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_bin_min_position              : 8 ;  //  7: 0
	UINT32 hist_bin_max_position              : 8 ;  // 15: 8
	UINT32 color_region_gain                  : 8 ;  // 23:16
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 blur_v_gain                        : 4 ;  // 31:28
} PE_H15A0_CO_PE1_DCE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025928L pe1_dce_ctrl_02 ''
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
} PE_H15A0_CO_PE1_DCE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802592cL pe1_dce_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_range_min                        : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 y_range_max                        : 10;  // 25:16
} PE_H15A0_CO_PE1_DCE_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025930L pe1_dce_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cb_range_min                       : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 cb_range_max                       : 10;  // 25:16
} PE_H15A0_CO_PE1_DCE_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025934L pe1_dce_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cr_range_min                       : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 cr_range_max                       : 10;  // 25:16
} PE_H15A0_CO_PE1_DCE_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025938L pe1_dce_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_address                    : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 hif_dce_load                       : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 hif_dce_ai                         : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_dce_enable                     : 1 ;  //    15
} PE_H15A0_CO_PE1_DCE_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc802593cL pe1_dce_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_wdata_y                    : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dyc_wdata_x                    : 10;  // 25:16
} PE_H15A0_CO_PE1_DCE_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025940L pe1_dce_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dyc_wdata_y_32nd               : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dyc_wdata_x_32nd               : 10;  // 25:16
} PE_H15A0_CO_PE1_DCE_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025944L pe1_dce_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 dce_ktd_csc_sel                    : 1 ;  //     1
} PE_H15A0_CO_PE1_DCE_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025948L pe1_dce_hist_ia_ctrl ''
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
} PE_H15A0_CO_PE1_DCE_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc802594cL pe1_dce_hist_ia_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_DCE_HIST_IA_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc8025950L pe1_dce_hist_ia_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_DCE_HIST_IA_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc8025960L pe1_dse_ctrl_00 ''
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
} PE_H15A0_CO_PE1_DSE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025964L pe1_dse_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23: 0     reserved
	UINT32 color_region_gain                  : 8 ;  // 31:24
} PE_H15A0_CO_PE1_DSE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025968L pe1_dse_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_wdata_y_32nd               : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dse_wdata_x_32nd               : 10;  // 25:16
} PE_H15A0_CO_PE1_DSE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802596cL pe1_dse_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_address                    : 5 ;  //  4: 0
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 hif_dse_load                       : 1 ;  //     8
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 hif_dse_ai                         : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_dse_enable                     : 1 ;  //    15
} PE_H15A0_CO_PE1_DSE_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025970L pe1_dse_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_dse_wdata_y                    : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 hif_dse_wdata_x                    : 10;  // 25:16
} PE_H15A0_CO_PE1_DSE_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025980L pe1_cw_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cw_en                              : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 cw_gain_selection                  : 1 ;  //     4
	UINT32                                    : 11;  // 15: 5     reserved
	UINT32 reg_cw_y4                          : 8 ;  // 23:16
	UINT32 reg_cw_x4                          : 8 ;  // 31:24
} PE_H15A0_CO_PE1_CW_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025984L pe1_cw_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 user_ctrl_g_gain                   : 8 ;  //  7: 0
	UINT32 user_ctrl_b_gain                   : 8 ;  // 15: 8
	UINT32 user_ctrl_r_gain                   : 8 ;  // 23:16
} PE_H15A0_CO_PE1_CW_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025988L pe1_cw_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cw_y2                          : 8 ;  //  7: 0
	UINT32 reg_cw_x2                          : 8 ;  // 15: 8
	UINT32 reg_cw_y3                          : 8 ;  // 23:16
	UINT32 reg_cw_x3                          : 8 ;  // 31:24
} PE_H15A0_CO_PE1_CW_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc802598cL pe1_cw_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cw_y0                          : 8 ;  //  7: 0
	UINT32 reg_cw_x0                          : 8 ;  // 15: 8
	UINT32 reg_cw_y1                          : 8 ;  // 23:16
	UINT32 reg_cw_x1                          : 8 ;  // 31:24
} PE_H15A0_CO_PE1_CW_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025990L pe1_cw_ctrl_04 ''
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
} PE_H15A0_CO_PE1_CW_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80259a0L pe1_conv_ctrl_00 ''
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
} PE_H15A0_CO_PE1_CONV_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80259a4L pe1_conv_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_r                           : 8 ;  //  7: 0
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 offset_l                           : 8 ;  // 23:16
} PE_H15A0_CO_PE1_CONV_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80259a8L pe1_conv_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_grey                      : 8 ;  //  7: 0
} PE_H15A0_CO_PE1_CONV_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80259acL pe1_conv_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CO_PE1_CONV_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc80259b0L pe1_conv_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CO_PE1_CONV_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc80259b4L pe1_conv_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_CONV_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc80259b8L pe1_conv_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_CONV_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc80259bcL pe1_conv_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CO_PE1_CONV_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc80259c0L pe1_conv_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CO_PE1_CONV_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc80259c4L pe1_conv_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CO_PE1_CONV_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc80259c8L pe1_conv_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_CONV_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc80259ccL pe1_conv_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_CONV_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc80259d0L pe1_conv_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CO_PE1_CONV_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc80259d4L pe1_conv_ctrl_0d ''
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
} PE_H15A0_CO_PE1_CONV_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc80259d8L pe1_conv_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_l_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_l_window_v_en                  : 1 ;  //    31
} PE_H15A0_CO_PE1_CONV_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc80259dcL pe1_conv_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_CONV_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc80259e0L pe1_conv_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_r_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_r_window_v_en                  : 1 ;  //    31
} PE_H15A0_CO_PE1_CONV_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc80259e4L pe1_conv_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_CONV_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc80259e8L pe1_conv_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 8 ;  //  7: 0
	UINT32 bg_color_b                         : 8 ;  // 15: 8
	UINT32 bg_color_g                         : 8 ;  // 23:16
} PE_H15A0_CO_PE1_CONV_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc80259f0L pe1_mute_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CO_PE1_MUTE_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc80259f4L pe1_mute_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CO_PE1_MUTE_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc80259f8L pe1_mute_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_MUTE_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc80259fcL pe1_mute_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_MUTE_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025a00L pe1_mute_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CO_PE1_MUTE_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025a04L pe1_mute_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_xpos                       : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_ypos                       : 12;  // 27:16
	UINT32 pxl_rep_in                         : 1 ;  //    28
	UINT32 pxl_rep_out                        : 1 ;  //    29
} PE_H15A0_CO_PE1_MUTE_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025a08L pe1_mute_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_width                      : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pxl_rep_height                     : 12;  // 27:16
} PE_H15A0_CO_PE1_MUTE_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc8025a0cL pe1_mute_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_MUTE_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025a10L pe1_mute_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pxl_rep_value_g                    : 10;  //  9: 0
	UINT32 pxl_rep_value_b                    : 10;  // 19:10
	UINT32 pxl_rep_value_r                    : 10;  // 29:20
} PE_H15A0_CO_PE1_MUTE_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025a14L pe1_mute_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_col_fil_g_valid                : 1 ;  //     0
	UINT32 reg_col_fil_b_valid                : 1 ;  //     1
	UINT32 reg_col_fil_r_valid                : 1 ;  //     2
	UINT32 reg_col_fil_g_nvalid               : 1 ;  //     3
	UINT32 reg_col_fil_b_nvalid               : 1 ;  //     4
	UINT32 reg_col_fil_r_nvalid               : 1 ;  //     5
} PE_H15A0_CO_PE1_MUTE_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025a18L pe1_mute_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mux3d_en                           : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_out_mux                        : 2 ;  //  5: 4
	UINT32 start_parity                       : 1 ;  //     6
	UINT32                                    : 9 ;  // 15: 7     reserved
	UINT32 alpha1_gain                        : 8 ;  // 23:16
	UINT32 alpha0_gain                        : 8 ;  // 31:24
} PE_H15A0_CO_PE1_MUTE_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc8025a1cL pe1_mute_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_l_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_l_window_v_en                  : 1 ;  //    31
} PE_H15A0_CO_PE1_MUTE_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc8025a20L pe1_mute_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_MUTE_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc8025a24L pe1_mute_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 mux_r_window_h_en                  : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mux_r_window_v_en                  : 1 ;  //    31
} PE_H15A0_CO_PE1_MUTE_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc8025a28L pe1_mute_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_MUTE_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc8025a2cL pe1_mute_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bg_color_r                         : 8 ;  //  7: 0
	UINT32 bg_color_b                         : 8 ;  // 15: 8
	UINT32 bg_color_g                         : 8 ;  // 23:16
} PE_H15A0_CO_PE1_MUTE_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc8025a40L pe1_tpd_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_TPD_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025a44L pe1_tpd_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0
	UINT32                                    : 3 ;  // 14:12     reserved
	UINT32 window_h_en                        : 1 ;  //    15
	UINT32 y0                                 : 12;  // 27:16
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 window_v_en                        : 1 ;  //    31
} PE_H15A0_CO_PE1_TPD_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025a48L pe1_tpd_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_TPD_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8025a4cL pe1_tpd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s_status                           : 8 ;  //  7: 0
} PE_H15A0_CO_PE1_TPD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8025a5cL pe1_ocsc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outputcsc_sel                      : 1 ;  //     0
} PE_H15A0_CO_PE1_OCSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025a60L pe1_xvy_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 outputcsc_en                       : 1 ;  //     0
	UINT32 degamma_en                         : 1 ;  //     1
	UINT32 pcc_en                             : 1 ;  //     2
	UINT32 gamma_en                           : 1 ;  //     3
	UINT32 scaler_en                          : 1 ;  //     4
	UINT32 hsv_hsl_selection                  : 1 ;  //     5
	UINT32 s_adpative_scaler_enable           : 1 ;  //     6
} PE_H15A0_CO_PE1_XVY_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025a64L pe1_xvy_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef0                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025a68L pe1_xvy_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef2                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8025a6cL pe1_xvy_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef4                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025a70L pe1_xvy_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_coef6                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025a74L pe1_xvy_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_csc_coef8                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025a78L pe1_xvy_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_csc_ofst1                        : 11;  // 26:16
} PE_H15A0_CO_PE1_XVY_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc8025a7cL pe1_xvy_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4                        : 14;  // 13: 0
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 r_csc_ofst3                        : 14;  // 29:16
} PE_H15A0_CO_PE1_XVY_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025a80L pe1_xvy_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef0                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_csc_ofst5                        : 14;  // 29:16
} PE_H15A0_CO_PE1_XVY_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025a84L pe1_xvy_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef2                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef1                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025a88L pe1_xvy_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef4                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef3                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc8025a8cL pe1_xvy_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef6                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef5                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc8025a90L pe1_xvy_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_pcc_coef8                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_pcc_coef7                        : 15;  // 30:16
} PE_H15A0_CO_PE1_XVY_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc8025a94L pe1_xvy_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scaler                             : 8 ;  //  7: 0
} PE_H15A0_CO_PE1_XVY_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc8025aa0L pe1_lrcr_ctrl_00 ''
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
} PE_H15A0_CO_PE1_LRCR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025aa4L pe1_lrcr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 th_max_hist                        : 8 ;  //  7: 0
	UINT32 th_valid_bins                      : 8 ;  // 15: 8
	UINT32 th1                                : 8 ;  // 23:16
	UINT32 th0                                : 8 ;  // 31:24
} PE_H15A0_CO_PE1_LRCR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025aa8L pe1_lrcr_ctrl_02 ''
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
} PE_H15A0_CO_PE1_LRCR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8025aacL pe1_lrcr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_LRCR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025ab0L pe1_lrcr_ctrl_04 ''
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
} PE_H15A0_CO_PE1_LRCR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025ab4L pe1_lrcr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_LRCR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025ab8L pe1_lrcr_ctrl_06 ''
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
} PE_H15A0_CO_PE1_LRCR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc8025abcL pe1_lrcr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16
} PE_H15A0_CO_PE1_LRCR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025ac0L pe1_lrcr_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0
	UINT32 n_valid_bins                       : 8 ;  // 15: 8
	UINT32 cdf_x1                             : 8 ;  // 23:16
	UINT32 cdf_x0                             : 8 ;  // 31:24
} PE_H15A0_CO_PE1_LRCR_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc8025ac4L pe1_lrcr_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0
	UINT32 n_valid_bins                       : 8 ;  // 15: 8
	UINT32 cdf_x1                             : 8 ;  // 23:16
	UINT32 cdf_x0                             : 8 ;  // 31:24
} PE_H15A0_CO_PE1_LRCR_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc8025ac8L pe1_lrcr_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  //  7: 0
	UINT32 n_valid_bins                       : 8 ;  // 15: 8
	UINT32 cdf_x1                             : 8 ;  // 23:16
	UINT32 cdf_x0                             : 8 ;  // 31:24
} PE_H15A0_CO_PE1_LRCR_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc8025ad0L pe1_l_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_g_available                : 1 ;  //    28
} PE_H15A0_CO_PE1_L_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025ad4L pe1_l_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_L_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025ad8L pe1_l_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_b_available                : 1 ;  //    28
} PE_H15A0_CO_PE1_L_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025adcL pe1_l_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_L_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025ae0L pe1_l_r_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_r_available                : 1 ;  //    28
} PE_H15A0_CO_PE1_L_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025ae4L pe1_l_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_L_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025ae8L pe1_r_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_g_available                : 1 ;  //    28
} PE_H15A0_CO_PE1_R_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025aecL pe1_r_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_R_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025af0L pe1_r_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  //  7: 0
	UINT32                                    : 4 ;  // 11: 8     reserved
	UINT32 hif_hist_ai                        : 1 ;  //    12
	UINT32                                    : 2 ;  // 14:13     reserved
	UINT32 hif_hist_enable                    : 1 ;  //    15
	UINT32                                    : 12;  // 27:16     reserved
	UINT32 hist_rd_b_available                : 1 ;  //    28
} PE_H15A0_CO_PE1_R_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025af4L pe1_r_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_R_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025af8L pe1_r_r_hist_ia_ctrl ''
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
} PE_H15A0_CO_PE1_R_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8025afcL pe1_r_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H15A0_CO_PE1_R_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0xc8025b10L pe1_bt20_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef1                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef0                        : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0xc8025b14L pe1_bt20_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef3a                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef2                        : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0xc8025b18L pe1_bt20_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef5a                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef4a                       : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0xc8025b1cL pe1_bt20_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef7a                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef6a                       : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0xc8025b20L pe1_bt20_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_ofst0                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_2x3_coef8a                       : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0xc8025b24L pe1_bt20_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_ofst2                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_2x3_ofst1                        : 11;  // 26:16
} PE_H15A0_CO_PE1_BT20_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0xc8025b28L pe1_bt20_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_ofst3                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_2x3_ofst4                        : 11;  // 26:16
} PE_H15A0_CO_PE1_BT20_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0xc8025b2cL pe1_bt20_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_en                           : 1 ;  //     0
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 r_degamma_en                       : 1 ;  //     4
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 r_1x3_en                           : 1 ;  //     8
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 r_2x3_ofst5                        : 11;  // 26:16
} PE_H15A0_CO_PE1_BT20_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0xc8025b30L pe1_bt20_ctrl_08 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dither_en                          : 1 ;  //     0
	UINT32 decontour_en                       : 1 ;  //     1
	UINT32 dither_random_freeze_en            : 1 ;  //     2
	UINT32 demo_pattern_enable                : 1 ;  //     3
	UINT32 bit_mode                           : 2 ;  //  5: 4
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 decontour_gain_r                   : 8 ;  // 15: 8
	UINT32 decontour_gain_g                   : 8 ;  // 23:16
	UINT32 decontour_gain_b                   : 8 ;  // 31:24
} PE_H15A0_CO_PE1_BT20_CTRL_08_T;

/*-----------------------------------------------------------------------------
		0xc8025b34L pe1_bt20_ctrl_09 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef3b                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_csc_cb_th                      : 10;  // 25:16
} PE_H15A0_CO_PE1_BT20_CTRL_09_T;

/*-----------------------------------------------------------------------------
		0xc8025b38L pe1_bt20_ctrl_0a ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef5b                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef4b                       : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_0A_T;

/*-----------------------------------------------------------------------------
		0xc8025b3cL pe1_bt20_ctrl_0b ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_2x3_coef7b                       : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_2x3_coef6b                       : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_0B_T;

/*-----------------------------------------------------------------------------
		0xc8025b40L pe1_bt20_ctrl_0c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_csc_cr_th                      : 10;  //  9: 0
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 r_2x3_coef8b                       : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_0C_T;

/*-----------------------------------------------------------------------------
		0xc8025b44L pe1_bt20_ctrl_0d ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef1                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef0                        : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_0D_T;

/*-----------------------------------------------------------------------------
		0xc8025b48L pe1_bt20_ctrl_0e ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef3                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef2                        : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_0E_T;

/*-----------------------------------------------------------------------------
		0xc8025b4cL pe1_bt20_ctrl_0f ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef5                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef4                        : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_0F_T;

/*-----------------------------------------------------------------------------
		0xc8025b50L pe1_bt20_ctrl_10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_coef7                        : 15;  // 14: 0
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 r_1x3_coef6                        : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_10_T;

/*-----------------------------------------------------------------------------
		0xc8025b54L pe1_bt20_ctrl_11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 r_1x3_coef8                        : 15;  // 30:16
} PE_H15A0_CO_PE1_BT20_CTRL_11_T;

/*-----------------------------------------------------------------------------
		0xc8025b58L pe1_bt20_ctrl_12 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_ofst1                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_1x3_ofst0                        : 11;  // 26:16
} PE_H15A0_CO_PE1_BT20_CTRL_12_T;

/*-----------------------------------------------------------------------------
		0xc8025b5cL pe1_bt20_ctrl_13 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_ofst3                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_1x3_ofst2                        : 11;  // 26:16
} PE_H15A0_CO_PE1_BT20_CTRL_13_T;

/*-----------------------------------------------------------------------------
		0xc8025b60L pe1_bt20_ctrl_14 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_1x3_ofst5                        : 11;  // 10: 0
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 r_1x3_ofst4                        : 11;  // 26:16
} PE_H15A0_CO_PE1_BT20_CTRL_14_T;

typedef struct {
	PE_H15A0_CE_PE1_TOP_CTRL_00_T         	pe1_top_ctrl_00;           //0xc8025000L
	PE_H15A0_CE_PE1_TOP_CTRL_01_T         	pe1_top_ctrl_01;           //0xc8025004L
	PE_H15A0_CE_PE1_TOP_CTRL_02_T         	pe1_top_ctrl_02;           //0xc8025008L
	PE_H15A0_CE_PE1_TOP_CTRL_03_T         	pe1_top_ctrl_03;           //0xc802500cL
	PE_H15A0_CE_PE1_TOP_CTRL_04_T         	pe1_top_ctrl_04;           //0xc8025010L
	UINT32                             	    reserved00[3];             //0xc8025014L~0xc802501cL
	PE_H15A0_CE_PE1_FSW_CTRL_00_T         	pe1_fsw_ctrl_00;           //0xc8025020L
	PE_H15A0_CE_PE1_FSW_CTRL_01_T         	pe1_fsw_ctrl_01;           //0xc8025024L
	PE_H15A0_CE_PE1_FSW_CTRL_02_T         	pe1_fsw_ctrl_02;           //0xc8025028L
	PE_H15A0_CE_PE1_FSW_CTRL_03_T         	pe1_fsw_ctrl_03;           //0xc802502cL
	UINT32                             	    reserved01;                //0xc8025030L
	PE_H15A0_CE_PE1_WIN2_CTRL_00_T        	pe1_win2_ctrl_00;          //0xc8025034L
	PE_H15A0_CE_PE1_WIN2_CTRL_01_T        	pe1_win2_ctrl_01;          //0xc8025038L
	PE_H15A0_CE_PE1_WIN2_CTRL_02_T        	pe1_win2_ctrl_02;          //0xc802503cL
	PE_H15A0_CE_PE1_WIN2_CTRL_03_T        	pe1_win2_ctrl_03;          //0xc8025040L
	PE_H15A0_CE_PE1_WIN2_CTRL_04_T        	pe1_win2_ctrl_04;          //0xc8025044L
	UINT32                             	    reserved02[2];             //0xc8025048L~0xc802504cL
	PE_H15A0_CE_PE1_APL_CTRL_00_T         	pe1_apl_ctrl_00;           //0xc8025050L
	PE_H15A0_CE_PE1_APL_CTRL_01_T         	pe1_apl_ctrl_01;           //0xc8025054L
	PE_H15A0_CE_PE1_APL_CTRL_02_T         	pe1_apl_ctrl_02;           //0xc8025058L
	PE_H15A0_CE_PE1_APL_STAT_00_T         	pe1_apl_stat_00;           //0xc802505cL
	PE_H15A0_CE_PE1_APL_STAT_01_T         	pe1_apl_stat_01;           //0xc8025060L
	UINT32                             	    reserved03[3];             //0xc8025064L~0xc802506cL
	PE_H15A0_CE_PE1_TPG_CTRL_00_T         	pe1_tpg_ctrl_00;           //0xc8025070L
	PE_H15A0_CE_PE1_TPG_CTRL_01_T         	pe1_tpg_ctrl_01;           //0xc8025074L
	PE_H15A0_CE_PE1_TPG_CTRL_02_T         	pe1_tpg_ctrl_02;           //0xc8025078L
	PE_H15A0_CE_PE1_TPG_CTRL_03_T         	pe1_tpg_ctrl_03;           //0xc802507cL
	PE_H15A0_CE_PE1_TPG_CTRL_04_T         	pe1_tpg_ctrl_04;           //0xc8025080L
	PE_H15A0_CE_PE1_TPG_IA_CTRL_T         	pe1_tpg_ia_ctrl;           //0xc8025084L
	PE_H15A0_CE_PE1_TPG_IA_DATA_T         	pe1_tpg_ia_data;           //0xc8025088L
	UINT32                             	    reserved04[5];             //0xc802508cL~0xc802509cL
	PE_H15A0_CE_PE1_VSPYC_CTRL_00_T       	pe1_vspyc_ctrl_00;         //0xc80250a0L
	PE_H15A0_CE_PE1_VSPYC_CTRL_01_T       	pe1_vspyc_ctrl_01;         //0xc80250a4L
	UINT32                             	    reserved05[2];             //0xc80250a8L~0xc80250acL
	PE_H15A0_CE_PE1_DEBUG_CTRL_00_T       	pe1_debug_ctrl_00;         //0xc80250b0L
	PE_H15A0_CE_PE1_DEBUG_CTRL_01_T       	pe1_debug_ctrl_01;         //0xc80250b4L
	PE_H15A0_CE_PE1_DEBUG_CTRL_02_T       	pe1_debug_ctrl_02;         //0xc80250b8L
	UINT32                             	    reserved06[9];             //0xc80250bcL~0xc80250dcL
	PE_H15A0_CE_PE1_CEN_CTRL_00_T         	pe1_cen_ctrl_00;           //0xc80250e0L
	PE_H15A0_CE_PE1_CEN_CTRL_01_T         	pe1_cen_ctrl_01;           //0xc80250e4L
	PE_H15A0_CE_PE1_CEN_CTRL_02_T         	pe1_cen_ctrl_02;           //0xc80250e8L
	PE_H15A0_CE_PE1_CEN_CTRL_03_T         	pe1_cen_ctrl_03;           //0xc80250ecL
	PE_H15A0_CE_PE1_CEN_CTRL_04_T         	pe1_cen_ctrl_04;           //0xc80250f0L
	PE_H15A0_CE_PE1_CEN_CTRL_05_T         	pe1_cen_ctrl_05;           //0xc80250f4L
	PE_H15A0_CE_PE1_CEN_CTRL_06_T         	pe1_cen_ctrl_06;           //0xc80250f8L
	PE_H15A0_CE_PE1_CEN_CTRL_07_T         	pe1_cen_ctrl_07;           //0xc80250fcL
	UINT32                             	    reserved07[4];             //0xc8025100L~0xc802510cL
	PE_H15A0_CE_PE1_CEN_IA_CTRL_T         	pe1_cen_ia_ctrl;           //0xc8025110L
	PE_H15A0_CE_PE1_CEN_IA_DATA_T         	pe1_cen_ia_data;           //0xc8025114L
	PE_H15A0_CE_PE1_CEN_DELTA_IA_CTRL_T   	pe1_cen_delta_ia_ctrl;     //0xc8025118L
	PE_H15A0_CE_PE1_CEN_DELTA_IA_DATA_T   	pe1_cen_delta_ia_data;     //0xc802511cL
	PE_H15A0_CE_PE1_DCE_CTRL_00_T         	pe1_dce_ctrl_00;           //0xc8025120L
	PE_H15A0_CE_PE1_DCE_CTRL_01_T         	pe1_dce_ctrl_01;           //0xc8025124L
	PE_H15A0_CE_PE1_DCE_CTRL_02_T         	pe1_dce_ctrl_02;           //0xc8025128L
	PE_H15A0_CE_PE1_DCE_CTRL_03_T         	pe1_dce_ctrl_03;           //0xc802512cL
	PE_H15A0_CE_PE1_DCE_CTRL_04_T         	pe1_dce_ctrl_04;           //0xc8025130L
	PE_H15A0_CE_PE1_DCE_CTRL_05_T         	pe1_dce_ctrl_05;           //0xc8025134L
	PE_H15A0_CE_PE1_DCE_IA_CTRL_T         	pe1_dce_ia_ctrl;           //0xc8025138L
	PE_H15A0_CE_PE1_DCE_IA_DATA_T         	pe1_dce_ia_data;           //0xc802513cL
	PE_H15A0_CE_PE1_DCE_CTRL_08_T         	pe1_dce_ctrl_08;           //0xc8025140L
	PE_H15A0_CE_PE1_DCE_CTRL_09_T         	pe1_dce_ctrl_09;           //0xc8025144L
	PE_H15A0_CE_PE1_DCE_HIST_IA_CTRL_T    	pe1_dce_hist_ia_ctrl;      //0xc8025148L
	PE_H15A0_CE_PE1_DCE_HIST_IA_DATA1_T   	pe1_dce_hist_ia_data1;     //0xc802514cL
	PE_H15A0_CE_PE1_DCE_HIST_IA_DATA2_T   	pe1_dce_hist_ia_data2;     //0xc8025150L
	UINT32                             	    reserved08[3];             //0xc8025154L~0xc802515cL
	PE_H15A0_CE_PE1_DSE_CTRL_00_T         	pe1_dse_ctrl_00;           //0xc8025160L
	PE_H15A0_CE_PE1_DSE_CTRL_01_T         	pe1_dse_ctrl_01;           //0xc8025164L
	PE_H15A0_CE_PE1_DSE_CTRL_02_T         	pe1_dse_ctrl_02;           //0xc8025168L
	PE_H15A0_CE_PE1_DSE_IA_CTRL_T         	pe1_dse_ia_ctrl;           //0xc802516cL
	PE_H15A0_CE_PE1_DSE_IA_DATA_T         	pe1_dse_ia_data;           //0xc8025170L
	UINT32                             	    reserved09[3];             //0xc8025174L~0xc802517cL
	PE_H15A0_CE_PE1_CW_CTRL_00_T          	pe1_cw_ctrl_00;            //0xc8025180L
	PE_H15A0_CE_PE1_CW_CTRL_01_T          	pe1_cw_ctrl_01;            //0xc8025184L
	PE_H15A0_CE_PE1_CW_CTRL_02_T          	pe1_cw_ctrl_02;            //0xc8025188L
	PE_H15A0_CE_PE1_CW_CTRL_03_T          	pe1_cw_ctrl_03;            //0xc802518cL
	PE_H15A0_CE_PE1_CW_CTRL_04_T          	pe1_cw_ctrl_04;            //0xc8025190L
	UINT32                             	    reserved10[3];             //0xc8025194L~0xc802519cL
	PE_H15A0_CE_PE1_CONV_CTRL_00_T        	pe1_conv_ctrl_00;          //0xc80251a0L
	PE_H15A0_CE_PE1_CONV_CTRL_01_T        	pe1_conv_ctrl_01;          //0xc80251a4L
	PE_H15A0_CE_PE1_CONV_CTRL_02_T        	pe1_conv_ctrl_02;          //0xc80251a8L
	PE_H15A0_CE_PE1_CONV_CTRL_03_T        	pe1_conv_ctrl_03;          //0xc80251acL
	PE_H15A0_CE_PE1_CONV_CTRL_04_T        	pe1_conv_ctrl_04;          //0xc80251b0L
	PE_H15A0_CE_PE1_CONV_CTRL_05_T        	pe1_conv_ctrl_05;          //0xc80251b4L
	PE_H15A0_CE_PE1_CONV_CTRL_06_T        	pe1_conv_ctrl_06;          //0xc80251b8L
	PE_H15A0_CE_PE1_CONV_CTRL_07_T        	pe1_conv_ctrl_07;          //0xc80251bcL
	PE_H15A0_CE_PE1_CONV_CTRL_08_T        	pe1_conv_ctrl_08;          //0xc80251c0L
	PE_H15A0_CE_PE1_CONV_CTRL_09_T        	pe1_conv_ctrl_09;          //0xc80251c4L
	PE_H15A0_CE_PE1_CONV_CTRL_0A_T        	pe1_conv_ctrl_0a;          //0xc80251c8L
	PE_H15A0_CE_PE1_CONV_CTRL_0B_T        	pe1_conv_ctrl_0b;          //0xc80251ccL
	PE_H15A0_CE_PE1_CONV_CTRL_0C_T        	pe1_conv_ctrl_0c;          //0xc80251d0L
	PE_H15A0_CE_PE1_CONV_CTRL_0D_T        	pe1_conv_ctrl_0d;          //0xc80251d4L
	PE_H15A0_CE_PE1_CONV_CTRL_0E_T        	pe1_conv_ctrl_0e;          //0xc80251d8L
	PE_H15A0_CE_PE1_CONV_CTRL_0F_T        	pe1_conv_ctrl_0f;          //0xc80251dcL
	PE_H15A0_CE_PE1_CONV_CTRL_10_T        	pe1_conv_ctrl_10;          //0xc80251e0L
	PE_H15A0_CE_PE1_CONV_CTRL_11_T        	pe1_conv_ctrl_11;          //0xc80251e4L
	PE_H15A0_CE_PE1_CONV_CTRL_12_T        	pe1_conv_ctrl_12;          //0xc80251e8L
	UINT32                             	    reserved11;                //0xc80251ecL
	PE_H15A0_CE_PE1_MUTE_CTRL_00_T        	pe1_mute_ctrl_00;          //0xc80251f0L
	PE_H15A0_CE_PE1_MUTE_CTRL_01_T        	pe1_mute_ctrl_01;          //0xc80251f4L
	PE_H15A0_CE_PE1_MUTE_CTRL_02_T        	pe1_mute_ctrl_02;          //0xc80251f8L
	PE_H15A0_CE_PE1_MUTE_CTRL_03_T        	pe1_mute_ctrl_03;          //0xc80251fcL
	PE_H15A0_CE_PE1_MUTE_CTRL_04_T        	pe1_mute_ctrl_04;          //0xc8025200L
	PE_H15A0_CE_PE1_MUTE_CTRL_05_T        	pe1_mute_ctrl_05;          //0xc8025204L
	PE_H15A0_CE_PE1_MUTE_CTRL_06_T        	pe1_mute_ctrl_06;          //0xc8025208L
	PE_H15A0_CE_PE1_MUTE_CTRL_07_T        	pe1_mute_ctrl_07;          //0xc802520cL
	PE_H15A0_CE_PE1_MUTE_CTRL_08_T        	pe1_mute_ctrl_08;          //0xc8025210L
	PE_H15A0_CE_PE1_MUTE_CTRL_09_T        	pe1_mute_ctrl_09;          //0xc8025214L
	PE_H15A0_CE_PE1_MUTE_CTRL_0A_T        	pe1_mute_ctrl_0a;          //0xc8025218L
	PE_H15A0_CE_PE1_MUTE_CTRL_0B_T        	pe1_mute_ctrl_0b;          //0xc802521cL
	PE_H15A0_CE_PE1_MUTE_CTRL_0C_T        	pe1_mute_ctrl_0c;          //0xc8025220L
	PE_H15A0_CE_PE1_MUTE_CTRL_0D_T        	pe1_mute_ctrl_0d;          //0xc8025224L
	PE_H15A0_CE_PE1_MUTE_CTRL_0E_T        	pe1_mute_ctrl_0e;          //0xc8025228L
	PE_H15A0_CE_PE1_MUTE_CTRL_0F_T        	pe1_mute_ctrl_0f;          //0xc802522cL
	UINT32                             	    reserved12[4];             //0xc8025230L~0xc802523cL
	PE_H15A0_CE_PE1_TPD_CTRL_00_T         	pe1_tpd_ctrl_00;           //0xc8025240L
	PE_H15A0_CE_PE1_TPD_CTRL_01_T         	pe1_tpd_ctrl_01;           //0xc8025244L
	PE_H15A0_CE_PE1_TPD_CTRL_02_T         	pe1_tpd_ctrl_02;           //0xc8025248L
	PE_H15A0_CE_PE1_TPD_STAT_00_T         	pe1_tpd_stat_00;           //0xc802524cL
	UINT32                             	    reserved13[3];             //0xc8025250L~0xc8025258L
	PE_H15A0_CE_PE1_OCSC_CTRL_00_T        	pe1_ocsc_ctrl_00;          //0xc802525cL
	PE_H15A0_CE_PE1_XVY_CTRL_00_T         	pe1_xvy_ctrl_00;           //0xc8025260L
	PE_H15A0_CE_PE1_XVY_CTRL_01_T         	pe1_xvy_ctrl_01;           //0xc8025264L
	PE_H15A0_CE_PE1_XVY_CTRL_02_T         	pe1_xvy_ctrl_02;           //0xc8025268L
	PE_H15A0_CE_PE1_XVY_CTRL_03_T         	pe1_xvy_ctrl_03;           //0xc802526cL
	PE_H15A0_CE_PE1_XVY_CTRL_04_T         	pe1_xvy_ctrl_04;           //0xc8025270L
	PE_H15A0_CE_PE1_XVY_CTRL_05_T         	pe1_xvy_ctrl_05;           //0xc8025274L
	PE_H15A0_CE_PE1_XVY_CTRL_06_T         	pe1_xvy_ctrl_06;           //0xc8025278L
	PE_H15A0_CE_PE1_XVY_CTRL_07_T         	pe1_xvy_ctrl_07;           //0xc802527cL
	PE_H15A0_CE_PE1_XVY_CTRL_08_T         	pe1_xvy_ctrl_08;           //0xc8025280L
	PE_H15A0_CE_PE1_XVY_CTRL_09_T         	pe1_xvy_ctrl_09;           //0xc8025284L
	PE_H15A0_CE_PE1_XVY_CTRL_0A_T         	pe1_xvy_ctrl_0a;           //0xc8025288L
	PE_H15A0_CE_PE1_XVY_CTRL_0B_T         	pe1_xvy_ctrl_0b;           //0xc802528cL
	PE_H15A0_CE_PE1_XVY_CTRL_0C_T         	pe1_xvy_ctrl_0c;           //0xc8025290L
	PE_H15A0_CE_PE1_XVY_CTRL_0D_T         	pe1_xvy_ctrl_0d;           //0xc8025294L
	UINT32                             	    reserved14[2];             //0xc8025298L~0xc802529cL
	PE_H15A0_CE_PE1_LRCR_CTRL_00_T        	pe1_lrcr_ctrl_00;          //0xc80252a0L
	PE_H15A0_CE_PE1_LRCR_CTRL_01_T        	pe1_lrcr_ctrl_01;          //0xc80252a4L
	PE_H15A0_CE_PE1_LRCR_CTRL_02_T        	pe1_lrcr_ctrl_02;          //0xc80252a8L
	PE_H15A0_CE_PE1_LRCR_CTRL_03_T        	pe1_lrcr_ctrl_03;          //0xc80252acL
	PE_H15A0_CE_PE1_LRCR_CTRL_04_T        	pe1_lrcr_ctrl_04;          //0xc80252b0L
	PE_H15A0_CE_PE1_LRCR_CTRL_05_T        	pe1_lrcr_ctrl_05;          //0xc80252b4L
	PE_H15A0_CE_PE1_LRCR_CTRL_06_T        	pe1_lrcr_ctrl_06;          //0xc80252b8L
	PE_H15A0_CE_PE1_LRCR_CTRL_07_T        	pe1_lrcr_ctrl_07;          //0xc80252bcL
	PE_H15A0_CE_PE1_LRCR_STAT_00_T        	pe1_lrcr_stat_00;          //0xc80252c0L
	PE_H15A0_CE_PE1_LRCR_STAT_01_T        	pe1_lrcr_stat_01;          //0xc80252c4L
	PE_H15A0_CE_PE1_LRCR_STAT_02_T        	pe1_lrcr_stat_02;          //0xc80252c8L
	UINT32                             	    reserved15;                //0xc80252ccL
	PE_H15A0_CE_PE1_L_G_HIST_IA_CTRL_T    	pe1_l_g_hist_ia_ctrl;      //0xc80252d0L
	PE_H15A0_CE_PE1_L_G_HIST_IA_DATA_T    	pe1_l_g_hist_ia_data;      //0xc80252d4L
	PE_H15A0_CE_PE1_L_B_HIST_IA_CTRL_T    	pe1_l_b_hist_ia_ctrl;      //0xc80252d8L
	PE_H15A0_CE_PE1_L_B_HIST_IA_DATA_T    	pe1_l_b_hist_ia_data;      //0xc80252dcL
	PE_H15A0_CE_PE1_L_R_HIST_IA_CTRL_T    	pe1_l_r_hist_ia_ctrl;      //0xc80252e0L
	PE_H15A0_CE_PE1_L_R_HIST_IA_DATA_T    	pe1_l_r_hist_ia_data;      //0xc80252e4L
	PE_H15A0_CE_PE1_R_G_HIST_IA_CTRL_T    	pe1_r_g_hist_ia_ctrl;      //0xc80252e8L
	PE_H15A0_CE_PE1_R_G_HIST_IA_DATA_T    	pe1_r_g_hist_ia_data;      //0xc80252ecL
	PE_H15A0_CE_PE1_R_B_HIST_IA_CTRL_T    	pe1_r_b_hist_ia_ctrl;      //0xc80252f0L
	PE_H15A0_CE_PE1_R_B_HIST_IA_DATA_T    	pe1_r_b_hist_ia_data;      //0xc80252f4L
	PE_H15A0_CE_PE1_R_R_HIST_IA_CTRL_T    	pe1_r_r_hist_ia_ctrl;      //0xc80252f8L
	PE_H15A0_CE_PE1_R_R_HIST_IA_DATA_T    	pe1_r_r_hist_ia_data;      //0xc80252fcL
	UINT32                             	    reserved16[4];             //0xc8025300L~0xc802530cL
	PE_H15A0_CE_PE1_BT20_CTRL_00_T        	pe1_bt20_ctrl_00;          //0xc8025310L
	PE_H15A0_CE_PE1_BT20_CTRL_01_T        	pe1_bt20_ctrl_01;          //0xc8025314L
	PE_H15A0_CE_PE1_BT20_CTRL_02_T        	pe1_bt20_ctrl_02;          //0xc8025318L
	PE_H15A0_CE_PE1_BT20_CTRL_03_T        	pe1_bt20_ctrl_03;          //0xc802531cL
	PE_H15A0_CE_PE1_BT20_CTRL_04_T        	pe1_bt20_ctrl_04;          //0xc8025320L
	PE_H15A0_CE_PE1_BT20_CTRL_05_T        	pe1_bt20_ctrl_05;          //0xc8025324L
	PE_H15A0_CE_PE1_BT20_CTRL_06_T        	pe1_bt20_ctrl_06;          //0xc8025328L
	PE_H15A0_CE_PE1_BT20_CTRL_07_T        	pe1_bt20_ctrl_07;          //0xc802532cL
	PE_H15A0_CE_PE1_BT20_CTRL_08_T        	pe1_bt20_ctrl_08;          //0xc8025330L
	PE_H15A0_CE_PE1_BT20_CTRL_09_T        	pe1_bt20_ctrl_09;          //0xc8025334L
	PE_H15A0_CE_PE1_BT20_CTRL_0A_T        	pe1_bt20_ctrl_0a;          //0xc8025338L
	PE_H15A0_CE_PE1_BT20_CTRL_0B_T        	pe1_bt20_ctrl_0b;          //0xc802533cL
	PE_H15A0_CE_PE1_BT20_CTRL_0C_T        	pe1_bt20_ctrl_0c;          //0xc8025340L
	PE_H15A0_CE_PE1_BT20_CTRL_0D_T        	pe1_bt20_ctrl_0d;          //0xc8025344L
	PE_H15A0_CE_PE1_BT20_CTRL_0E_T        	pe1_bt20_ctrl_0e;          //0xc8025348L
	PE_H15A0_CE_PE1_BT20_CTRL_0F_T        	pe1_bt20_ctrl_0f;          //0xc802534cL
	PE_H15A0_CE_PE1_BT20_CTRL_10_T        	pe1_bt20_ctrl_10;          //0xc8025350L
	PE_H15A0_CE_PE1_BT20_CTRL_11_T        	pe1_bt20_ctrl_11;          //0xc8025354L
	PE_H15A0_CE_PE1_BT20_CTRL_12_T        	pe1_bt20_ctrl_12;          //0xc8025358L
	PE_H15A0_CE_PE1_BT20_CTRL_13_T        	pe1_bt20_ctrl_13;          //0xc802535cL
	PE_H15A0_CE_PE1_BT20_CTRL_14_T        	pe1_bt20_ctrl_14;          //0xc8025360L
	UINT32                             	    reserved17[3];             //0xc8025364L~0xc802536cL
	PE_H15A0_CE_PE1_DRC_CTRL_00_T         	pe1_drc_ctrl_00;           //0xc8025370L
	PE_H15A0_CE_PE1_DRC_CTRL_01_T         	pe1_drc_ctrl_01;           //0xc8025374L
	PE_H15A0_CE_PE1_DRC_CTRL_02_T         	pe1_drc_ctrl_02;           //0xc8025378L
	PE_H15A0_CE_PE1_DRC_CTRL_03_T         	pe1_drc_ctrl_03;           //0xc802537cL
	PE_H15A0_CE_PE1_DRC_CTRL_04_T         	pe1_drc_ctrl_04;           //0xc8025380L
	PE_H15A0_CE_PE1_DRC_CTRL_05_T         	pe1_drc_ctrl_05;           //0xc8025384L
	PE_H15A0_CE_PE1_DRC_CTRL_06_T         	pe1_drc_ctrl_06;           //0xc8025388L
	PE_H15A0_CE_PE1_DRC_CTRL_07_T         	pe1_drc_ctrl_07;           //0xc802538cL
	PE_H15A0_CE_PE1_DRC_CTRL_08_T         	pe1_drc_ctrl_08;           //0xc8025390L
	PE_H15A0_CE_PE1_DRC_CTRL_09_T         	pe1_drc_ctrl_09;           //0xc8025394L
	PE_H15A0_CE_PE1_DRC_CTRL_10_T         	pe1_drc_ctrl_10;           //0xc8025398L
	PE_H15A0_CE_PE1_DRC_CTRL_11_T         	pe1_drc_ctrl_11;           //0xc802539cL
	PE_H15A0_CE_PE1_DRC_CTRL_12_T         	pe1_drc_ctrl_12;           //0xc80253a0L
	PE_H15A0_CE_PE1_DRC_CTRL_13_T         	pe1_drc_ctrl_13;           //0xc80253a4L
	PE_H15A0_CE_PE1_DRC_CTRL_14_T         	pe1_drc_ctrl_14;           //0xc80253a8L
	PE_H15A0_CE_PE1_DRC_CTRL_15_T         	pe1_drc_ctrl_15;           //0xc80253acL
	PE_H15A0_CE_PE1_DRC_CTRL_16_T         	pe1_drc_ctrl_16;           //0xc80253b0L
	PE_H15A0_CE_PE1_DRC_CTRL_17_T         	pe1_drc_ctrl_17;           //0xc80253b4L
	PE_H15A0_CE_PE1_DRC_CTRL_18_T         	pe1_drc_ctrl_18;           //0xc80253b8L
	PE_H15A0_CE_PE1_DRC_CTRL_19_T         	pe1_drc_ctrl_19;           //0xc80253bcL
	PE_H15A0_CE_PE1_DRC_CTRL_20_T         	pe1_drc_ctrl_20;           //0xc80253c0L
	PE_H15A0_CE_PE1_DRC_CTRL_21_T         	pe1_drc_ctrl_21;           //0xc80253c4L
	PE_H15A0_CE_PE1_DRC_CTRL_22_T         	pe1_drc_ctrl_22;           //0xc80253c8L
	PE_H15A0_CE_PE1_DRC_CTRL_23_T         	pe1_drc_ctrl_23;           //0xc80253ccL
	PE_H15A0_CE_PE1_DRC_CTRL_24_T         	pe1_drc_ctrl_24;           //0xc80253d0L
	PE_H15A0_CE_PE1_DRC_CTRL_25_T         	pe1_drc_ctrl_25;           //0xc80253d4L
	PE_H15A0_CE_PE1_DRC_CTRL_26_T         	pe1_drc_ctrl_26;           //0xc80253d8L
	PE_H15A0_CE_PE1_DRC_CTRL_27_T         	pe1_drc_ctrl_27;           //0xc80253dcL
	PE_H15A0_CE_PE1_DRC_CTRL_28_T         	pe1_drc_ctrl_28;           //0xc80253e0L
	PE_H15A0_CE_PE1_DRC_CTRL_29_T         	pe1_drc_ctrl_29;           //0xc80253e4L
	PE_H15A0_CE_PE1_DRC_CTRL_30_T         	pe1_drc_ctrl_30;           //0xc80253e8L
	PE_H15A0_CE_PE1_DRC_CTRL_31_T         	pe1_drc_ctrl_31;           //0xc80253ecL
	PE_H15A0_CE_PE1_DRC_CTRL_32_T         	pe1_drc_ctrl_32;           //0xc80253f0L
	PE_H15A0_CE_PE1_DRC_CTRL_33_T         	pe1_drc_ctrl_33;           //0xc80253f4L
	PE_H15A0_CE_PE1_DRC_CTRL_34_T         	pe1_drc_ctrl_34;           //0xc80253f8L
}PE_CE_PE1_REG_H15A0_T;

typedef struct {
	PE_H15A0_CO_PE1_TOP_CTRL_00_T         	pe1_top_ctrl_00;           //0xc8025800L
	PE_H15A0_CO_PE1_TOP_CTRL_01_T         	pe1_top_ctrl_01;           //0xc8025804L
	PE_H15A0_CO_PE1_TOP_CTRL_02_T         	pe1_top_ctrl_02;           //0xc8025808L
	PE_H15A0_CO_PE1_TOP_CTRL_03_T         	pe1_top_ctrl_03;           //0xc802580cL
	PE_H15A0_CO_PE1_TOP_CTRL_04_T         	pe1_top_ctrl_04;           //0xc8025810L
	UINT32                             	    reserved00[3];             //0xc8025814L~0xc802581cL
	PE_H15A0_CO_PE1_FSW_CTRL_00_T         	pe1_fsw_ctrl_00;           //0xc8025820L
	PE_H15A0_CO_PE1_FSW_CTRL_01_T         	pe1_fsw_ctrl_01;           //0xc8025824L
	PE_H15A0_CO_PE1_FSW_CTRL_02_T         	pe1_fsw_ctrl_02;           //0xc8025828L
	PE_H15A0_CO_PE1_FSW_CTRL_03_T         	pe1_fsw_ctrl_03;           //0xc802582cL
	UINT32                             	    reserved01;                //0xc8025830L
	PE_H15A0_CO_PE1_WIN2_CTRL_00_T        	pe1_win2_ctrl_00;          //0xc8025834L
	PE_H15A0_CO_PE1_WIN2_CTRL_01_T        	pe1_win2_ctrl_01;          //0xc8025838L
	PE_H15A0_CO_PE1_WIN2_CTRL_02_T        	pe1_win2_ctrl_02;          //0xc802583cL
	PE_H15A0_CO_PE1_WIN2_CTRL_03_T        	pe1_win2_ctrl_03;          //0xc8025840L
	PE_H15A0_CO_PE1_WIN2_CTRL_04_T        	pe1_win2_ctrl_04;          //0xc8025844L
	UINT32                             	    reserved02[2];             //0xc8025848L~0xc802584cL
	PE_H15A0_CO_PE1_APL_CTRL_00_T         	pe1_apl_ctrl_00;           //0xc8025850L
	PE_H15A0_CO_PE1_APL_CTRL_01_T         	pe1_apl_ctrl_01;           //0xc8025854L
	PE_H15A0_CO_PE1_APL_CTRL_02_T         	pe1_apl_ctrl_02;           //0xc8025858L
	PE_H15A0_CO_PE1_APL_STAT_00_T         	pe1_apl_stat_00;           //0xc802585cL
	PE_H15A0_CO_PE1_APL_STAT_01_T         	pe1_apl_stat_01;           //0xc8025860L
	UINT32                             	    reserved03[3];             //0xc8025864L~0xc802586cL
	PE_H15A0_CO_PE1_TPG_CTRL_00_T         	pe1_tpg_ctrl_00;           //0xc8025870L
	PE_H15A0_CO_PE1_TPG_CTRL_01_T         	pe1_tpg_ctrl_01;           //0xc8025874L
	PE_H15A0_CO_PE1_TPG_CTRL_02_T         	pe1_tpg_ctrl_02;           //0xc8025878L
	PE_H15A0_CO_PE1_TPG_CTRL_03_T         	pe1_tpg_ctrl_03;           //0xc802587cL
	PE_H15A0_CO_PE1_TPG_CTRL_04_T         	pe1_tpg_ctrl_04;           //0xc8025880L
	PE_H15A0_CO_PE1_TPG_IA_CTRL_T         	pe1_tpg_ia_ctrl;           //0xc8025884L
	PE_H15A0_CO_PE1_TPG_IA_DATA_T         	pe1_tpg_ia_data;           //0xc8025888L
	UINT32                             	    reserved04[5];             //0xc802588cL~0xc802589cL
	PE_H15A0_CO_PE1_VSPYC_CTRL_00_T       	pe1_vspyc_ctrl_00;         //0xc80258a0L
	PE_H15A0_CO_PE1_VSPYC_CTRL_01_T       	pe1_vspyc_ctrl_01;         //0xc80258a4L
	UINT32                             	    reserved05[2];             //0xc80258a8L~0xc80258acL
	PE_H15A0_CO_PE1_DEBUG_CTRL_00_T       	pe1_debug_ctrl_00;         //0xc80258b0L
	PE_H15A0_CO_PE1_DEBUG_CTRL_01_T       	pe1_debug_ctrl_01;         //0xc80258b4L
	PE_H15A0_CO_PE1_DEBUG_CTRL_02_T       	pe1_debug_ctrl_02;         //0xc80258b8L
	UINT32                             	    reserved06[9];             //0xc80258bcL~0xc80258dcL
	PE_H15A0_CO_PE1_CEN_CTRL_00_T         	pe1_cen_ctrl_00;           //0xc80258e0L
	PE_H15A0_CO_PE1_CEN_CTRL_01_T         	pe1_cen_ctrl_01;           //0xc80258e4L
	PE_H15A0_CO_PE1_CEN_CTRL_02_T         	pe1_cen_ctrl_02;           //0xc80258e8L
	PE_H15A0_CO_PE1_CEN_CTRL_03_T         	pe1_cen_ctrl_03;           //0xc80258ecL
	PE_H15A0_CO_PE1_CEN_CTRL_04_T         	pe1_cen_ctrl_04;           //0xc80258f0L
	PE_H15A0_CO_PE1_CEN_CTRL_05_T         	pe1_cen_ctrl_05;           //0xc80258f4L
	PE_H15A0_CO_PE1_CEN_CTRL_06_T         	pe1_cen_ctrl_06;           //0xc80258f8L
	PE_H15A0_CO_PE1_CEN_CTRL_07_T         	pe1_cen_ctrl_07;           //0xc80258fcL
	UINT32                             	    reserved07[4];             //0xc8025900L~0xc802590cL
	PE_H15A0_CO_PE1_CEN_IA_CTRL_T         	pe1_cen_ia_ctrl;           //0xc8025910L
	PE_H15A0_CO_PE1_CEN_IA_DATA_T         	pe1_cen_ia_data;           //0xc8025914L
	PE_H15A0_CO_PE1_CEN_DELTA_IA_CTRL_T   	pe1_cen_delta_ia_ctrl;     //0xc8025918L
	PE_H15A0_CO_PE1_CEN_DELTA_IA_DATA_T   	pe1_cen_delta_ia_data;     //0xc802591cL
	PE_H15A0_CO_PE1_DCE_CTRL_00_T         	pe1_dce_ctrl_00;           //0xc8025920L
	PE_H15A0_CO_PE1_DCE_CTRL_01_T         	pe1_dce_ctrl_01;           //0xc8025924L
	PE_H15A0_CO_PE1_DCE_CTRL_02_T         	pe1_dce_ctrl_02;           //0xc8025928L
	PE_H15A0_CO_PE1_DCE_CTRL_03_T         	pe1_dce_ctrl_03;           //0xc802592cL
	PE_H15A0_CO_PE1_DCE_CTRL_04_T         	pe1_dce_ctrl_04;           //0xc8025930L
	PE_H15A0_CO_PE1_DCE_CTRL_05_T         	pe1_dce_ctrl_05;           //0xc8025934L
	PE_H15A0_CO_PE1_DCE_IA_CTRL_T         	pe1_dce_ia_ctrl;           //0xc8025938L
	PE_H15A0_CO_PE1_DCE_IA_DATA_T         	pe1_dce_ia_data;           //0xc802593cL
	PE_H15A0_CO_PE1_DCE_CTRL_08_T         	pe1_dce_ctrl_08;           //0xc8025940L
	PE_H15A0_CO_PE1_DCE_CTRL_09_T         	pe1_dce_ctrl_09;           //0xc8025944L
	PE_H15A0_CO_PE1_DCE_HIST_IA_CTRL_T    	pe1_dce_hist_ia_ctrl;      //0xc8025948L
	PE_H15A0_CO_PE1_DCE_HIST_IA_DATA1_T   	pe1_dce_hist_ia_data1;     //0xc802594cL
	PE_H15A0_CO_PE1_DCE_HIST_IA_DATA2_T   	pe1_dce_hist_ia_data2;     //0xc8025950L
	UINT32                             	    reserved08[3];             //0xc8025954L~0xc802595cL
	PE_H15A0_CO_PE1_DSE_CTRL_00_T         	pe1_dse_ctrl_00;           //0xc8025960L
	PE_H15A0_CO_PE1_DSE_CTRL_01_T         	pe1_dse_ctrl_01;           //0xc8025964L
	PE_H15A0_CO_PE1_DSE_CTRL_02_T         	pe1_dse_ctrl_02;           //0xc8025968L
	PE_H15A0_CO_PE1_DSE_IA_CTRL_T         	pe1_dse_ia_ctrl;           //0xc802596cL
	PE_H15A0_CO_PE1_DSE_IA_DATA_T         	pe1_dse_ia_data;           //0xc8025970L
	UINT32                             	    reserved09[3];             //0xc8025974L~0xc802597cL
	PE_H15A0_CO_PE1_CW_CTRL_00_T          	pe1_cw_ctrl_00;            //0xc8025980L
	PE_H15A0_CO_PE1_CW_CTRL_01_T          	pe1_cw_ctrl_01;            //0xc8025984L
	PE_H15A0_CO_PE1_CW_CTRL_02_T          	pe1_cw_ctrl_02;            //0xc8025988L
	PE_H15A0_CO_PE1_CW_CTRL_03_T          	pe1_cw_ctrl_03;            //0xc802598cL
	PE_H15A0_CO_PE1_CW_CTRL_04_T          	pe1_cw_ctrl_04;            //0xc8025990L
	UINT32                             	    reserved10[3];             //0xc8025994L~0xc802599cL
	PE_H15A0_CO_PE1_CONV_CTRL_00_T        	pe1_conv_ctrl_00;          //0xc80259a0L
	PE_H15A0_CO_PE1_CONV_CTRL_01_T        	pe1_conv_ctrl_01;          //0xc80259a4L
	PE_H15A0_CO_PE1_CONV_CTRL_02_T        	pe1_conv_ctrl_02;          //0xc80259a8L
	PE_H15A0_CO_PE1_CONV_CTRL_03_T        	pe1_conv_ctrl_03;          //0xc80259acL
	PE_H15A0_CO_PE1_CONV_CTRL_04_T        	pe1_conv_ctrl_04;          //0xc80259b0L
	PE_H15A0_CO_PE1_CONV_CTRL_05_T        	pe1_conv_ctrl_05;          //0xc80259b4L
	PE_H15A0_CO_PE1_CONV_CTRL_06_T        	pe1_conv_ctrl_06;          //0xc80259b8L
	PE_H15A0_CO_PE1_CONV_CTRL_07_T        	pe1_conv_ctrl_07;          //0xc80259bcL
	PE_H15A0_CO_PE1_CONV_CTRL_08_T        	pe1_conv_ctrl_08;          //0xc80259c0L
	PE_H15A0_CO_PE1_CONV_CTRL_09_T        	pe1_conv_ctrl_09;          //0xc80259c4L
	PE_H15A0_CO_PE1_CONV_CTRL_0A_T        	pe1_conv_ctrl_0a;          //0xc80259c8L
	PE_H15A0_CO_PE1_CONV_CTRL_0B_T        	pe1_conv_ctrl_0b;          //0xc80259ccL
	PE_H15A0_CO_PE1_CONV_CTRL_0C_T        	pe1_conv_ctrl_0c;          //0xc80259d0L
	PE_H15A0_CO_PE1_CONV_CTRL_0D_T        	pe1_conv_ctrl_0d;          //0xc80259d4L
	PE_H15A0_CO_PE1_CONV_CTRL_0E_T        	pe1_conv_ctrl_0e;          //0xc80259d8L
	PE_H15A0_CO_PE1_CONV_CTRL_0F_T        	pe1_conv_ctrl_0f;          //0xc80259dcL
	PE_H15A0_CO_PE1_CONV_CTRL_10_T        	pe1_conv_ctrl_10;          //0xc80259e0L
	PE_H15A0_CO_PE1_CONV_CTRL_11_T        	pe1_conv_ctrl_11;          //0xc80259e4L
	PE_H15A0_CO_PE1_CONV_CTRL_12_T        	pe1_conv_ctrl_12;          //0xc80259e8L
	UINT32                             	    reserved11;                //0xc80259ecL
	PE_H15A0_CO_PE1_MUTE_CTRL_00_T        	pe1_mute_ctrl_00;          //0xc80259f0L
	PE_H15A0_CO_PE1_MUTE_CTRL_01_T        	pe1_mute_ctrl_01;          //0xc80259f4L
	PE_H15A0_CO_PE1_MUTE_CTRL_02_T        	pe1_mute_ctrl_02;          //0xc80259f8L
	PE_H15A0_CO_PE1_MUTE_CTRL_03_T        	pe1_mute_ctrl_03;          //0xc80259fcL
	PE_H15A0_CO_PE1_MUTE_CTRL_04_T        	pe1_mute_ctrl_04;          //0xc8025a00L
	PE_H15A0_CO_PE1_MUTE_CTRL_05_T        	pe1_mute_ctrl_05;          //0xc8025a04L
	PE_H15A0_CO_PE1_MUTE_CTRL_06_T        	pe1_mute_ctrl_06;          //0xc8025a08L
	PE_H15A0_CO_PE1_MUTE_CTRL_07_T        	pe1_mute_ctrl_07;          //0xc8025a0cL
	PE_H15A0_CO_PE1_MUTE_CTRL_08_T        	pe1_mute_ctrl_08;          //0xc8025a10L
	PE_H15A0_CO_PE1_MUTE_CTRL_09_T        	pe1_mute_ctrl_09;          //0xc8025a14L
	PE_H15A0_CO_PE1_MUTE_CTRL_0A_T        	pe1_mute_ctrl_0a;          //0xc8025a18L
	PE_H15A0_CO_PE1_MUTE_CTRL_0B_T        	pe1_mute_ctrl_0b;          //0xc8025a1cL
	PE_H15A0_CO_PE1_MUTE_CTRL_0C_T        	pe1_mute_ctrl_0c;          //0xc8025a20L
	PE_H15A0_CO_PE1_MUTE_CTRL_0D_T        	pe1_mute_ctrl_0d;          //0xc8025a24L
	PE_H15A0_CO_PE1_MUTE_CTRL_0E_T        	pe1_mute_ctrl_0e;          //0xc8025a28L
	PE_H15A0_CO_PE1_MUTE_CTRL_0F_T        	pe1_mute_ctrl_0f;          //0xc8025a2cL
	UINT32                             	    reserved12[4];             //0xc8025a30L~0xc8025a3cL
	PE_H15A0_CO_PE1_TPD_CTRL_00_T         	pe1_tpd_ctrl_00;           //0xc8025a40L
	PE_H15A0_CO_PE1_TPD_CTRL_01_T         	pe1_tpd_ctrl_01;           //0xc8025a44L
	PE_H15A0_CO_PE1_TPD_CTRL_02_T         	pe1_tpd_ctrl_02;           //0xc8025a48L
	PE_H15A0_CO_PE1_TPD_STAT_00_T         	pe1_tpd_stat_00;           //0xc8025a4cL
	UINT32                             	    reserved13[3];             //0xc8025a50L~0xc8025a58L
	PE_H15A0_CO_PE1_OCSC_CTRL_00_T        	pe1_ocsc_ctrl_00;          //0xc8025a5cL
	PE_H15A0_CO_PE1_XVY_CTRL_00_T         	pe1_xvy_ctrl_00;           //0xc8025a60L
	PE_H15A0_CO_PE1_XVY_CTRL_01_T         	pe1_xvy_ctrl_01;           //0xc8025a64L
	PE_H15A0_CO_PE1_XVY_CTRL_02_T         	pe1_xvy_ctrl_02;           //0xc8025a68L
	PE_H15A0_CO_PE1_XVY_CTRL_03_T         	pe1_xvy_ctrl_03;           //0xc8025a6cL
	PE_H15A0_CO_PE1_XVY_CTRL_04_T         	pe1_xvy_ctrl_04;           //0xc8025a70L
	PE_H15A0_CO_PE1_XVY_CTRL_05_T         	pe1_xvy_ctrl_05;           //0xc8025a74L
	PE_H15A0_CO_PE1_XVY_CTRL_06_T         	pe1_xvy_ctrl_06;           //0xc8025a78L
	PE_H15A0_CO_PE1_XVY_CTRL_07_T         	pe1_xvy_ctrl_07;           //0xc8025a7cL
	PE_H15A0_CO_PE1_XVY_CTRL_08_T         	pe1_xvy_ctrl_08;           //0xc8025a80L
	PE_H15A0_CO_PE1_XVY_CTRL_09_T         	pe1_xvy_ctrl_09;           //0xc8025a84L
	PE_H15A0_CO_PE1_XVY_CTRL_0A_T         	pe1_xvy_ctrl_0a;           //0xc8025a88L
	PE_H15A0_CO_PE1_XVY_CTRL_0B_T         	pe1_xvy_ctrl_0b;           //0xc8025a8cL
	PE_H15A0_CO_PE1_XVY_CTRL_0C_T         	pe1_xvy_ctrl_0c;           //0xc8025a90L
	PE_H15A0_CO_PE1_XVY_CTRL_0D_T         	pe1_xvy_ctrl_0d;           //0xc8025a94L
	UINT32                             	    reserved14[2];             //0xc8025a98L~0xc8025a9cL
	PE_H15A0_CO_PE1_LRCR_CTRL_00_T        	pe1_lrcr_ctrl_00;          //0xc8025aa0L
	PE_H15A0_CO_PE1_LRCR_CTRL_01_T        	pe1_lrcr_ctrl_01;          //0xc8025aa4L
	PE_H15A0_CO_PE1_LRCR_CTRL_02_T        	pe1_lrcr_ctrl_02;          //0xc8025aa8L
	PE_H15A0_CO_PE1_LRCR_CTRL_03_T        	pe1_lrcr_ctrl_03;          //0xc8025aacL
	PE_H15A0_CO_PE1_LRCR_CTRL_04_T        	pe1_lrcr_ctrl_04;          //0xc8025ab0L
	PE_H15A0_CO_PE1_LRCR_CTRL_05_T        	pe1_lrcr_ctrl_05;          //0xc8025ab4L
	PE_H15A0_CO_PE1_LRCR_CTRL_06_T        	pe1_lrcr_ctrl_06;          //0xc8025ab8L
	PE_H15A0_CO_PE1_LRCR_CTRL_07_T        	pe1_lrcr_ctrl_07;          //0xc8025abcL
	PE_H15A0_CO_PE1_LRCR_STAT_00_T        	pe1_lrcr_stat_00;          //0xc8025ac0L
	PE_H15A0_CO_PE1_LRCR_STAT_01_T        	pe1_lrcr_stat_01;          //0xc8025ac4L
	PE_H15A0_CO_PE1_LRCR_STAT_02_T        	pe1_lrcr_stat_02;          //0xc8025ac8L
	UINT32                             	    reserved15;                //0xc8025accL
	PE_H15A0_CO_PE1_L_G_HIST_IA_CTRL_T    	pe1_l_g_hist_ia_ctrl;      //0xc8025ad0L
	PE_H15A0_CO_PE1_L_G_HIST_IA_DATA_T    	pe1_l_g_hist_ia_data;      //0xc8025ad4L
	PE_H15A0_CO_PE1_L_B_HIST_IA_CTRL_T    	pe1_l_b_hist_ia_ctrl;      //0xc8025ad8L
	PE_H15A0_CO_PE1_L_B_HIST_IA_DATA_T    	pe1_l_b_hist_ia_data;      //0xc8025adcL
	PE_H15A0_CO_PE1_L_R_HIST_IA_CTRL_T    	pe1_l_r_hist_ia_ctrl;      //0xc8025ae0L
	PE_H15A0_CO_PE1_L_R_HIST_IA_DATA_T    	pe1_l_r_hist_ia_data;      //0xc8025ae4L
	PE_H15A0_CO_PE1_R_G_HIST_IA_CTRL_T    	pe1_r_g_hist_ia_ctrl;      //0xc8025ae8L
	PE_H15A0_CO_PE1_R_G_HIST_IA_DATA_T    	pe1_r_g_hist_ia_data;      //0xc8025aecL
	PE_H15A0_CO_PE1_R_B_HIST_IA_CTRL_T    	pe1_r_b_hist_ia_ctrl;      //0xc8025af0L
	PE_H15A0_CO_PE1_R_B_HIST_IA_DATA_T    	pe1_r_b_hist_ia_data;      //0xc8025af4L
	PE_H15A0_CO_PE1_R_R_HIST_IA_CTRL_T    	pe1_r_r_hist_ia_ctrl;      //0xc8025af8L
	PE_H15A0_CO_PE1_R_R_HIST_IA_DATA_T    	pe1_r_r_hist_ia_data;      //0xc8025afcL
	UINT32                             	    reserved16[4];             //0xc8025b00L~0xc8025b0cL
	PE_H15A0_CO_PE1_BT20_CTRL_00_T        	pe1_bt20_ctrl_00;          //0xc8025b10L
	PE_H15A0_CO_PE1_BT20_CTRL_01_T        	pe1_bt20_ctrl_01;          //0xc8025b14L
	PE_H15A0_CO_PE1_BT20_CTRL_02_T        	pe1_bt20_ctrl_02;          //0xc8025b18L
	PE_H15A0_CO_PE1_BT20_CTRL_03_T        	pe1_bt20_ctrl_03;          //0xc8025b1cL
	PE_H15A0_CO_PE1_BT20_CTRL_04_T        	pe1_bt20_ctrl_04;          //0xc8025b20L
	PE_H15A0_CO_PE1_BT20_CTRL_05_T        	pe1_bt20_ctrl_05;          //0xc8025b24L
	PE_H15A0_CO_PE1_BT20_CTRL_06_T        	pe1_bt20_ctrl_06;          //0xc8025b28L
	PE_H15A0_CO_PE1_BT20_CTRL_07_T        	pe1_bt20_ctrl_07;          //0xc8025b2cL
	PE_H15A0_CO_PE1_BT20_CTRL_08_T        	pe1_bt20_ctrl_08;          //0xc8025b30L
	PE_H15A0_CO_PE1_BT20_CTRL_09_T        	pe1_bt20_ctrl_09;          //0xc8025b34L
	PE_H15A0_CO_PE1_BT20_CTRL_0A_T        	pe1_bt20_ctrl_0a;          //0xc8025b38L
	PE_H15A0_CO_PE1_BT20_CTRL_0B_T        	pe1_bt20_ctrl_0b;          //0xc8025b3cL
	PE_H15A0_CO_PE1_BT20_CTRL_0C_T        	pe1_bt20_ctrl_0c;          //0xc8025b40L
	PE_H15A0_CO_PE1_BT20_CTRL_0D_T        	pe1_bt20_ctrl_0d;          //0xc8025b44L
	PE_H15A0_CO_PE1_BT20_CTRL_0E_T        	pe1_bt20_ctrl_0e;          //0xc8025b48L
	PE_H15A0_CO_PE1_BT20_CTRL_0F_T        	pe1_bt20_ctrl_0f;          //0xc8025b4cL
	PE_H15A0_CO_PE1_BT20_CTRL_10_T        	pe1_bt20_ctrl_10;          //0xc8025b50L
	PE_H15A0_CO_PE1_BT20_CTRL_11_T        	pe1_bt20_ctrl_11;          //0xc8025b54L
	PE_H15A0_CO_PE1_BT20_CTRL_12_T        	pe1_bt20_ctrl_12;          //0xc8025b58L
	PE_H15A0_CO_PE1_BT20_CTRL_13_T        	pe1_bt20_ctrl_13;          //0xc8025b5cL
	PE_H15A0_CO_PE1_BT20_CTRL_14_T        	pe1_bt20_ctrl_14;          //0xc8025b60L
}PE_CO_PE1_REG_H15A0_T;

#endif
