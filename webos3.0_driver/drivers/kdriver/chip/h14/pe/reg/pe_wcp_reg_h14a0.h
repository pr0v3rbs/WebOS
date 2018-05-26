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

#ifndef _PE_WCP_REG_H14A0_H_
#define _PE_WCP_REG_H14A0_H_
/*-----------------------------------------------------------------------------
		0x1200 ovl_ov_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_444                           : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 main_420                           : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 main_lsr_2b                        : 2 ;  //  5: 4     
	UINT32 main_cflt_mode                     : 2 ;  //  7: 6     
	UINT32 sub_444                            : 1 ;  //     8     
	UINT32                                    : 1 ;  //     9     reserved
	UINT32 sub_420                            : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 mif_last_mode                      : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 ov_out_sync_mux                    : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 use_ba_sync                        : 1 ;  //    24     
	UINT32                                    : 5 ;  // 29:25     reserved
	UINT32 load_enable                        : 1 ;  //    30     
	UINT32 load_type                          : 1 ;  //    31     
} PE_H14A0_OVL_OV_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x1204 ovl_ov_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 border_color_cr                    : 4 ;  //  3: 0     
	UINT32 border_color_cb                    : 4 ;  //  7: 4     
	UINT32 border_color_yy                    : 8 ;  // 15: 8     
	UINT32 lr_compare_th                      : 8 ;  // 23:16     
	UINT32 lr_compare_sel                     : 2 ;  // 25:24     
	UINT32 lr_compare_sync_sel                : 1 ;  //    26     
	UINT32 lr_compare_mark_org_nact           : 1 ;  //    27     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 lr_compare_en                      : 1 ;  //    31     
} PE_H14A0_OVL_OV_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x1208 ovl_ov_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 back_color_cr                      : 4 ;  //  3: 0     
	UINT32 back_color_cb                      : 4 ;  //  7: 4     
	UINT32 back_color_yy                      : 8 ;  // 15: 8     
	UINT32 dark_color_cr                      : 4 ;  // 19:16     
	UINT32 dark_color_cb                      : 4 ;  // 23:20     
	UINT32 dark_color_yy                      : 8 ;  // 31:24     
} PE_H14A0_OVL_OV_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x120c ovl_ov_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_blending_alpha                  : 8 ;  //  7: 0     
	UINT32 w1_blending_alpha                  : 8 ;  // 15: 8     
	UINT32 w2_blending_alpha                  : 8 ;  // 23:16     
	UINT32 w0_blending_mode                   : 2 ;  // 25:24     
	UINT32 w1_blending_mode                   : 2 ;  // 27:26     
	UINT32 w2_blending_mode                   : 2 ;  // 29:28     
} PE_H14A0_OVL_OV_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x1210 ovl_ov_moffset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hoffset                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 m_voffset                          : 12;  // 27:16     
} PE_H14A0_OVL_OV_MOFFSET_T;

/*-----------------------------------------------------------------------------
		0x1214 ovl_ov_msize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hsize                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 m_vsize                            : 12;  // 27:16     
} PE_H14A0_OVL_OV_MSIZE_T;

/*-----------------------------------------------------------------------------
		0x1218 ovl_ov_soffset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s_hoffset                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 s_voffset                          : 12;  // 27:16     
} PE_H14A0_OVL_OV_SOFFSET_T;

/*-----------------------------------------------------------------------------
		0x121c ovl_ov_ssize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 s_hsize                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 s_vsize                            : 12;  // 27:16     
} PE_H14A0_OVL_OV_SSIZE_T;

/*-----------------------------------------------------------------------------
		0x1220 ovl_ov_border_width ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_line_subsample_parity            : 1 ;  //     0     
	UINT32 m_line_subsample_en                : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 s_line_subsample_parity            : 1 ;  //     8     
	UINT32 s_line_subsample_en                : 1 ;  //     9     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 m_right_wid                        : 4 ;  // 19:16     
	UINT32 m_left_wid                         : 4 ;  // 23:20     
	UINT32 m_down_wid                         : 4 ;  // 27:24     
	UINT32 m_up_wid                           : 4 ;  // 31:28     
} PE_H14A0_OVL_OV_BORDER_WIDTH_T;

/*-----------------------------------------------------------------------------
		0x1224 ovl_cflt_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_1                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 cflt_coef_0                        : 12;  // 27:16     
} PE_H14A0_OVL_CFLT_COEF0_T;

/*-----------------------------------------------------------------------------
		0x1228 ovl_cflt_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_3                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 cflt_coef_2                        : 12;  // 27:16     
} PE_H14A0_OVL_CFLT_COEF1_T;

/*-----------------------------------------------------------------------------
		0x1240 ovl_ov_intr_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32 intr_line_pos_enable               : 1 ;  //     1     
	UINT32                                    : 8 ;  //  9: 2     reserved
	UINT32 intr_mbuf_c_empty_en               : 1 ;  //    10     
	UINT32 intr_mbuf_y_empty_en               : 1 ;  //    11     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 line_position                      : 12;  // 27:16     
} PE_H14A0_OVL_OV_INTR_CTRL_T;

/*-----------------------------------------------------------------------------
		0x1244 ovl_ov_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 mbuf_c_empty                       : 1 ;  //     2     
	UINT32 mbuf_y_empty                       : 1 ;  //     3     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 line_cnt                           : 12;  // 27:16     
} PE_H14A0_OVL_OV_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0x1248 ovl_ov_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_imux_ctrl                       : 2 ;  //  1: 0     
	UINT32 w0_dark_en                         : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 w1_imux_ctrl                       : 2 ;  //  5: 4     
	UINT32 w1_dark_en                         : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 w2_imux_ctrl                       : 2 ;  //  9: 8     
	UINT32 w2_dark_en                         : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 w0_line_mode_en                    : 1 ;  //    16     
	UINT32 w0_line_parity                     : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 w1_line_mode_en                    : 1 ;  //    20     
	UINT32 w1_line_parity                     : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 line_mode_sync_sel                 : 2 ;  // 25:24     
} PE_H14A0_OVL_OV_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x1258 ovl_ov_yc_delay ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_delay_h                          : 4 ;  //  3: 0     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 c_delay_v                          : 4 ;  // 19:16     
	UINT32                                    : 9 ;  // 28:20     reserved
	UINT32 h_delay_en                         : 1 ;  //    29     
	UINT32 v_delay_en                         : 1 ;  //    30     
	UINT32 delay_disable                      : 1 ;  //    31     
} PE_H14A0_OVL_OV_YC_DELAY_T;

/*-----------------------------------------------------------------------------
		0x125c ovl_ov_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_c_mif_line_offset             : 12;  // 11: 0     
	UINT32 main_c_mif_line_inc                : 2 ;  // 13:12     
	UINT32 main_c_mif_v_inversion             : 1 ;  //    14     
	UINT32 main_c_manual_mif_ctrl_en          : 1 ;  //    15     
	UINT32 main_y_mif_line_offset             : 12;  // 27:16     
	UINT32 main_y_mif_line_inc                : 2 ;  // 29:28     
	UINT32 main_y_mif_v_inversion             : 1 ;  //    30     
	UINT32 main_y_manual_mif_ctrl_en          : 1 ;  //    31     
} PE_H14A0_OVL_OV_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x1260 ovl_ov_mif_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sub_c_mif_line_offset              : 12;  // 11: 0     
	UINT32 sub_c_mif_line_inc                 : 2 ;  // 13:12     
	UINT32 sub_c_mif_v_inversion              : 1 ;  //    14     
	UINT32 sub_c_manual_mif_ctrl_en           : 1 ;  //    15     
	UINT32 sub_y_mif_line_offset              : 12;  // 27:16     
	UINT32 sub_y_mif_line_inc                 : 2 ;  // 29:28     
	UINT32 sub_y_mif_v_inversion              : 1 ;  //    30     
	UINT32 sub_y_manual_mif_ctrl_en           : 1 ;  //    31     
} PE_H14A0_OVL_OV_MIF_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x1280 ovl_ov_mif_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_mif_y_line_cnt                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 main_mif_c_line_cnt                : 12;  // 27:16     
	UINT32 pic_init_cnt                       : 4 ;  // 31:28     
} PE_H14A0_OVL_OV_MIF_STATUS_T;

/*-----------------------------------------------------------------------------
		0x1284 ovl_ov_lr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lr_compare_diff_cnt                : 24;  // 23: 0     
} PE_H14A0_OVL_OV_LR_STATUS_T;

/*-----------------------------------------------------------------------------
		0x1290 ovl_ov_buf_size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 buf_size_y                         : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 buf_size_c                         : 3 ;  // 18:16     
} PE_H14A0_OVL_OV_BUF_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0x12a0 ovl_ov_buf_rdy_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 buf_rdy_th_y                       : 10;  //  9: 0     
	UINT32                                    : 5 ;  // 14:10     reserved
	UINT32 mif_x16_stride_y                   : 1 ;  //    15     
	UINT32 buf_rdy_th_c                       : 10;  // 25:16     
	UINT32                                    : 5 ;  // 30:26     reserved
	UINT32 mif_x16_stride_c                   : 1 ;  //    31     
} PE_H14A0_OVL_OV_BUF_RDY_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0x12a4 ovl_ov_ba_sync_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ba_sync_en                         : 1 ;  //     0     
	UINT32 ba_vfall_enforce                   : 1 ;  //     1     
	UINT32 ba_va_mode                         : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 buf_check_l_y                      : 1 ;  //     4     
	UINT32 buf_check_l_c                      : 1 ;  //     5     
	UINT32 buf_check_r_y                      : 1 ;  //     6     
	UINT32 buf_check_r_c                      : 1 ;  //     7     
	UINT32 ha_size                            : 12;  // 19: 8     
	UINT32 va_size                            : 12;  // 31:20     
} PE_H14A0_OVL_OV_BA_SYNC_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0x12a8 ovl_ov_ba_sync_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ha_min_th                          : 12;  // 11: 0     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 buf_check_s_y                      : 1 ;  //    14     
	UINT32 buf_check_s_c                      : 1 ;  //    15     
	UINT32 ha_max_th                          : 16;  // 31:16     
} PE_H14A0_OVL_OV_BA_SYNC_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0x12ac ovl_ov_ba_sync_st ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ba_line_cnt                        : 16;  // 15: 0     
} PE_H14A0_OVL_OV_BA_SYNC_ST_T;

/*-----------------------------------------------------------------------------
		0x12b0 ovl_ov_buf_rdy_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 buf_rdy_th_y                       : 10;  //  9: 0     
	UINT32                                    : 5 ;  // 14:10     reserved
	UINT32 mif_x16_stride_y                   : 1 ;  //    15     
	UINT32 buf_rdy_th_c                       : 10;  // 25:16     
	UINT32                                    : 5 ;  // 30:26     reserved
	UINT32 mif_x16_stride_c                   : 1 ;  //    31     
} PE_H14A0_OVL_OV_BUF_RDY_CTRL_1_T;
/*-----------------------------------------------------------------------------
		0x3200 ovr_ov_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_444                           : 1 ;  //     0     
	UINT32                                    : 1 ;  //     1     reserved
	UINT32 main_420                           : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 main_lsr_2b                        : 2 ;  //  5: 4     
	UINT32 main_cflt_mode                     : 2 ;  //  7: 6     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 mif_last_mode                      : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 ov_out_sync_mux                    : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 use_ba_sync                        : 1 ;  //    24     
	UINT32                                    : 5 ;  // 29:25     reserved
	UINT32 load_enable                        : 1 ;  //    30     
	UINT32 load_type                          : 1 ;  //    31     
} PE_H14A0_OVR_OV_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x3204 ovr_ov_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 border_color_cr                    : 4 ;  //  3: 0     
	UINT32 border_color_cb                    : 4 ;  //  7: 4     
	UINT32 border_color_yy                    : 8 ;  // 15: 8     
	UINT32 lr_compare_th                      : 8 ;  // 23:16     
	UINT32 lr_compare_sel                     : 2 ;  // 25:24     
	UINT32 lr_compare_sync_sel                : 1 ;  //    26     
	UINT32 lr_compare_mark_org_nact           : 1 ;  //    27     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 lr_compare_en                      : 1 ;  //    31     
} PE_H14A0_OVR_OV_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x3208 ovr_ov_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 back_color_cr                      : 4 ;  //  3: 0     
	UINT32 back_color_cb                      : 4 ;  //  7: 4     
	UINT32 back_color_yy                      : 8 ;  // 15: 8     
	UINT32 dark_color_cr                      : 4 ;  // 19:16     
	UINT32 dark_color_cb                      : 4 ;  // 23:20     
	UINT32 dark_color_yy                      : 8 ;  // 31:24     
} PE_H14A0_OVR_OV_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x320c ovr_ov_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_blending_alpha                  : 8 ;  //  7: 0     
	UINT32 w1_blending_alpha                  : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 w0_blending_mode                   : 2 ;  // 25:24     
	UINT32 w1_blending_mode                   : 2 ;  // 27:26     
} PE_H14A0_OVR_OV_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x3210 ovr_ov_moffset ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hoffset                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 m_voffset                          : 12;  // 27:16     
} PE_H14A0_OVR_OV_MOFFSET_T;

/*-----------------------------------------------------------------------------
		0x3214 ovr_ov_msize ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_hsize                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 m_vsize                            : 12;  // 27:16     
} PE_H14A0_OVR_OV_MSIZE_T;

/*-----------------------------------------------------------------------------
		0x3220 ovr_ov_border_width ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 m_line_subsample_parity            : 1 ;  //     0     
	UINT32 m_line_subsample_en                : 1 ;  //     1     
	UINT32                                    : 14;  // 15: 2     reserved
	UINT32 m_right_wid                        : 4 ;  // 19:16     
	UINT32 m_left_wid                         : 4 ;  // 23:20     
	UINT32 m_down_wid                         : 4 ;  // 27:24     
	UINT32 m_up_wid                           : 4 ;  // 31:28     
} PE_H14A0_OVR_OV_BORDER_WIDTH_T;

/*-----------------------------------------------------------------------------
		0x3224 ovr_cflt_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_1                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 cflt_coef_0                        : 12;  // 27:16     
} PE_H14A0_OVR_CFLT_COEF0_T;

/*-----------------------------------------------------------------------------
		0x3228 ovr_cflt_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cflt_coef_3                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 cflt_coef_2                        : 12;  // 27:16     
} PE_H14A0_OVR_CFLT_COEF1_T;

/*-----------------------------------------------------------------------------
		0x3240 ovr_ov_intr_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 intr_enable                        : 1 ;  //     0     
	UINT32 intr_line_pos_enable               : 1 ;  //     1     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 intr_sbuf_c_empty_en               : 1 ;  //     8     
	UINT32 intr_sbuf_y_empty_en               : 1 ;  //     9     
	UINT32 intr_mbuf_c_empty_en               : 1 ;  //    10     
	UINT32 intr_mbuf_y_empty_en               : 1 ;  //    11     
	UINT32 intr_pdb_end_en                    : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 line_position                      : 11;  // 26:16     
} PE_H14A0_OVR_OV_INTR_CTRL_T;

/*-----------------------------------------------------------------------------
		0x3244 ovr_ov_intr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 mbuf_c_empty                       : 1 ;  //     2     
	UINT32 mbuf_y_empty                       : 1 ;  //     3     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 line_cnt                           : 11;  // 26:16     
} PE_H14A0_OVR_OV_INTR_STATUS_T;

/*-----------------------------------------------------------------------------
		0x3248 ovr_ov_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 w0_imux_ctrl                       : 2 ;  //  1: 0     
	UINT32 w0_dark_en                         : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 w1_imux_ctrl                       : 2 ;  //  5: 4     
	UINT32 w1_dark_en                         : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 w2_imux_ctrl                       : 2 ;  //  9: 8     
	UINT32 w2_dark_en                         : 1 ;  //    10     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 w0_line_mode_en                    : 1 ;  //    16     
	UINT32 w0_line_parity                     : 1 ;  //    17     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 w1_line_mode_en                    : 1 ;  //    20     
	UINT32 w1_line_parity                     : 1 ;  //    21     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 line_mode_sync_sel                 : 2 ;  // 25:24     
} PE_H14A0_OVR_OV_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x3258 ovr_ov_yc_delay ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_delay_h                          : 4 ;  //  3: 0     
	UINT32                                    : 12;  // 15: 4     reserved
	UINT32 c_delay_v                          : 4 ;  // 19:16     
	UINT32                                    : 9 ;  // 28:20     reserved
	UINT32 h_delay_en                         : 1 ;  //    29     
	UINT32 v_delay_en                         : 1 ;  //    30     
	UINT32 delay_disable                      : 1 ;  //    31     
} PE_H14A0_OVR_OV_YC_DELAY_T;

/*-----------------------------------------------------------------------------
		0x325c ovr_ov_mif_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_c_mif_line_offset             : 12;  // 11: 0     
	UINT32 main_c_mif_line_inc                : 2 ;  // 13:12     
	UINT32 main_c_mif_v_inversion             : 1 ;  //    14     
	UINT32 main_c_manual_mif_ctrl_en          : 1 ;  //    15     
	UINT32 main_y_mif_line_offset             : 12;  // 27:16     
	UINT32 main_y_mif_line_inc                : 2 ;  // 29:28     
	UINT32 main_y_mif_v_inversion             : 1 ;  //    30     
	UINT32 main_y_manual_mif_ctrl_en          : 1 ;  //    31     
} PE_H14A0_OVR_OV_MIF_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x3280 ovr_ov_mif_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 main_mif_y_line_cnt                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 main_mif_c_line_cnt                : 12;  // 27:16     
	UINT32 pic_init_cnt                       : 4 ;  // 31:28     
} PE_H14A0_OVR_OV_MIF_STATUS_T;

/*-----------------------------------------------------------------------------
		0x3284 ovr_ov_lr_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 lr_compare_diff_cnt                : 24;  // 23: 0     
} PE_H14A0_OVR_OV_LR_STATUS_T;

/*-----------------------------------------------------------------------------
		0x3290 ovr_ov_buf_size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 buf_size_y                         : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 buf_size_c                         : 3 ;  // 18:16     
} PE_H14A0_OVR_OV_BUF_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0x32a0 ovr_ov_buf_rdy_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 buf_rdy_th_y                       : 10;  //  9: 0     
	UINT32                                    : 5 ;  // 14:10     reserved
	UINT32 mif_x16_stride_y                   : 1 ;  //    15     
	UINT32 buf_rdy_th_c                       : 10;  // 25:16     
	UINT32                                    : 5 ;  // 30:26     reserved
	UINT32 mif_x16_stride_c                   : 1 ;  //    31     
} PE_H14A0_OVR_OV_BUF_RDY_CTRL_T;

typedef struct {
	PE_H14A0_OVL_OV_CTRL0_T               	ovl_ov_ctrl0;                 //0x1200
	PE_H14A0_OVL_OV_CTRL1_T               	ovl_ov_ctrl1;                 //0x1204
	PE_H14A0_OVL_OV_CTRL2_T               	ovl_ov_ctrl2;                 //0x1208
	PE_H14A0_OVL_OV_CTRL3_T               	ovl_ov_ctrl3;                 //0x120c
	PE_H14A0_OVL_OV_MOFFSET_T             	ovl_ov_moffset;               //0x1210
	PE_H14A0_OVL_OV_MSIZE_T               	ovl_ov_msize;                 //0x1214
	PE_H14A0_OVL_OV_SOFFSET_T             	ovl_ov_soffset;               //0x1218
	PE_H14A0_OVL_OV_SSIZE_T               	ovl_ov_ssize;                 //0x121c
	PE_H14A0_OVL_OV_BORDER_WIDTH_T        	ovl_ov_border_width;          //0x1220
	PE_H14A0_OVL_CFLT_COEF0_T             	ovl_cflt_coef0;               //0x1224
	PE_H14A0_OVL_CFLT_COEF1_T             	ovl_cflt_coef1;               //0x1228
	UINT32                              	reserved00[5];                //0x122c~0x123c
	PE_H14A0_OVL_OV_INTR_CTRL_T           	ovl_ov_intr_ctrl;             //0x1240
	PE_H14A0_OVL_OV_INTR_STATUS_T         	ovl_ov_intr_status;           //0x1244
	PE_H14A0_OVL_OV_CTRL4_T               	ovl_ov_ctrl4;                 //0x1248
	UINT32                              	reserved01[3];                //0x124c~0x1254
	PE_H14A0_OVL_OV_YC_DELAY_T            	ovl_ov_yc_delay;              //0x1258
	PE_H14A0_OVL_OV_MIF_CTRL0_T           	ovl_ov_mif_ctrl0;             //0x125c
	PE_H14A0_OVL_OV_MIF_CTRL1_T           	ovl_ov_mif_ctrl1;             //0x1260
	UINT32                              	reserved02[7];                //0x1264~0x127c
	PE_H14A0_OVL_OV_MIF_STATUS_T          	ovl_ov_mif_status;            //0x1280
	PE_H14A0_OVL_OV_LR_STATUS_T           	ovl_ov_lr_status;             //0x1284
	UINT32                              	reserved03[2];                //0x1288~0x128c
	PE_H14A0_OVL_OV_BUF_SIZE_CTRL_T       	ovl_ov_buf_size_ctrl;         //0x1290
	UINT32                              	reserved04[3];                //0x1294~0x129c
	PE_H14A0_OVL_OV_BUF_RDY_CTRL_0_T      	ovl_ov_buf_rdy_ctrl_0;        //0x12a0
	PE_H14A0_OVL_OV_BA_SYNC_CTRL_0_T      	ovl_ov_ba_sync_ctrl_0;        //0x12a4
	PE_H14A0_OVL_OV_BA_SYNC_CTRL_1_T      	ovl_ov_ba_sync_ctrl_1;        //0x12a8
	PE_H14A0_OVL_OV_BA_SYNC_ST_T          	ovl_ov_ba_sync_st;            //0x12ac
	PE_H14A0_OVL_OV_BUF_RDY_CTRL_1_T      	ovl_ov_buf_rdy_ctrl_1;        //0x12b0
}PE_OVL_REG_H14A0_T;
typedef struct {
	PE_H14A0_OVR_OV_CTRL0_T               	ovr_ov_ctrl0;                 //0x3200
	PE_H14A0_OVR_OV_CTRL1_T               	ovr_ov_ctrl1;                 //0x3204
	PE_H14A0_OVR_OV_CTRL2_T               	ovr_ov_ctrl2;                 //0x3208
	PE_H14A0_OVR_OV_CTRL3_T               	ovr_ov_ctrl3;                 //0x320c
	PE_H14A0_OVR_OV_MOFFSET_T             	ovr_ov_moffset;               //0x3210
	PE_H14A0_OVR_OV_MSIZE_T               	ovr_ov_msize;                 //0x3214
	UINT32                              	reserved00[2];                //0x3218~0x321c
	PE_H14A0_OVR_OV_BORDER_WIDTH_T        	ovr_ov_border_width;          //0x3220
	PE_H14A0_OVR_CFLT_COEF0_T             	ovr_cflt_coef0;               //0x3224
	PE_H14A0_OVR_CFLT_COEF1_T             	ovr_cflt_coef1;               //0x3228
	UINT32                              	reserved01[5];                //0x322c~0x323c
	PE_H14A0_OVR_OV_INTR_CTRL_T           	ovr_ov_intr_ctrl;             //0x3240
	PE_H14A0_OVR_OV_INTR_STATUS_T         	ovr_ov_intr_status;           //0x3244
	PE_H14A0_OVR_OV_CTRL4_T               	ovr_ov_ctrl4;                 //0x3248
	UINT32                              	reserved02[3];                //0x324c~0x3254
	PE_H14A0_OVR_OV_YC_DELAY_T            	ovr_ov_yc_delay;              //0x3258
	PE_H14A0_OVR_OV_MIF_CTRL0_T           	ovr_ov_mif_ctrl0;             //0x325c
	UINT32                              	reserved03[8];                //0x3260~0x327c
	PE_H14A0_OVR_OV_MIF_STATUS_T          	ovr_ov_mif_status;            //0x3280
	PE_H14A0_OVR_OV_LR_STATUS_T           	ovr_ov_lr_status;             //0x3284
	UINT32                              	reserved04[2];                //0x3288~0x328c
	PE_H14A0_OVR_OV_BUF_SIZE_CTRL_T       	ovr_ov_buf_size_ctrl;         //0x3290
	UINT32                              	reserved05[3];                //0x3294~0x329c
	PE_H14A0_OVR_OV_BUF_RDY_CTRL_T        	ovr_ov_buf_rdy_ctrl;          //0x32a0
}PE_OVR_REG_H14A0_T;
#endif