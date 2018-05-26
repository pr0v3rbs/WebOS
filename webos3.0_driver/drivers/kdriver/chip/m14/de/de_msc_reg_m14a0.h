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

#ifndef _REG_MSC_M14A0_h
#define _REG_MSC_M14A0_h

/*-----------------------------------------------------------------------------
	0x1100 msc_ctrl_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnd_pel_filtering_scheme        : 1;   //     0
	UINT32 adaptive_fir_filter_en          : 1;   //     1
	UINT32 v_scaler_config                 : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 filter_type_sel_luma            : 1;   //     4
	UINT32 filter_type_sel_chroma          : 1;   //     5
	UINT32 manual_sampling_mode            : 1;   //     6
	UINT32                                 : 1;   //     7 reserved
	UINT32 size_info_manual_load           : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 bilinear_y_h_off                : 1;   //    12
	UINT32 bilinear_y_v_off                : 1;   //    13
	UINT32 bilinear_c_h_off                : 1;   //    14
	UINT32 bilinear_c_v_off                : 1;   //    15
	UINT32                                 : 15;  // 16:30 reserved
	UINT32 bit_8_mode_en                   : 1;   //    31
} M14A0_MSC_CTRL_L_T;

/*-----------------------------------------------------------------------------
	0x1104 msc_y_init_pos_h_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                    : 6;   //  0: 5
} M14A0_MSC_Y_INIT_POS_H_L_T;

/*-----------------------------------------------------------------------------
	0x1108 msc_y_init_pos_v_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                    : 6;   //  0: 5
} M14A0_MSC_Y_INIT_POS_V_L_T;

/*-----------------------------------------------------------------------------
	0x110c msc_c_init_pos_h_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                    : 6;   //  0: 5
} M14A0_MSC_C_INIT_POS_H_L_T;

/*-----------------------------------------------------------------------------
	0x1110 msc_c_init_pos_v_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                    : 6;   //  0: 5
} M14A0_MSC_C_INIT_POS_V_L_T;

/*-----------------------------------------------------------------------------
	0x1114 msc_adaptive_th_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_th                          : 4;   //  0: 3
	UINT32 blending_range                  : 2;   //  4: 5
} M14A0_MSC_ADAPTIVE_TH_L_T;

/*-----------------------------------------------------------------------------
	0x1118 msc_scr_size_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 input_v_size                    : 12;  // 16:27
} M14A0_MSC_SCR_SIZE_L_T;

/*-----------------------------------------------------------------------------
	0x111c msc_win_out_size_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 out_win_v_size                  : 12;  // 16:27
} M14A0_MSC_WIN_OUT_SIZE_L_T;

/*-----------------------------------------------------------------------------
	0x1120 msc_win_in_offset_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset                 : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 in_win_v_offset                 : 12;  // 16:27
} M14A0_MSC_WIN_IN_OFFSET_L_T;

/*-----------------------------------------------------------------------------
	0x1124 msc_win_in_size_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size                   : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 in_win_v_size                   : 12;  // 16:27
} M14A0_MSC_WIN_IN_SIZE_L_T;

/*-----------------------------------------------------------------------------
	0x1128 msc_c_format_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                     : 3;   //  0: 2
	UINT32                                 : 13;  //  3:15 reserved
	UINT32 out_c_format                    : 3;   // 16:18
} M14A0_MSC_C_FORMAT_L_T;

/*-----------------------------------------------------------------------------
	0x1130 msc_y_fir_coef_data0_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient1                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA0_L_T;

/*-----------------------------------------------------------------------------
	0x1134 msc_y_fir_coef_data1_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient3                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA1_L_T;

/*-----------------------------------------------------------------------------
	0x1138 msc_y_fir_coef_data2_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient5                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA2_L_T;

/*-----------------------------------------------------------------------------
	0x113c msc_y_fir_coef_data3_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient7                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA3_L_T;

/*-----------------------------------------------------------------------------
	0x1140 msc_y_fir_coef_data4_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient9                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA4_L_T;

/*-----------------------------------------------------------------------------
	0x1144 msc_y_fir_coef_data5_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                 : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient11                 : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA5_L_T;

/*-----------------------------------------------------------------------------
	0x1148 msc_y_fir_coef_ctrl_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                            : 4;   //  0: 3
	UINT32 h_v_sel                         : 3;   //  4: 6
	UINT32                                 : 1;   //     7 reserved
	UINT32 update                          : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 rnw                             : 1;   //    12
} M14A0_MSC_Y_FIR_COEF_CTRL_L_T;

/*-----------------------------------------------------------------------------
	0x1150 msc_c_fir_coef_data0_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient0                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient1                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA0_L_T;

/*-----------------------------------------------------------------------------
	0x1154 msc_c_fir_coef_data1_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient2                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient3                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA1_L_T;

/*-----------------------------------------------------------------------------
	0x1158 msc_c_fir_coef_data2_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient4                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient5                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA2_L_T;

/*-----------------------------------------------------------------------------
	0x115c msc_c_fir_coef_data3_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient6                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient7                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA3_L_T;

/*-----------------------------------------------------------------------------
	0x1160 msc_c_fir_coef_ctrl_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                            : 4;   //  0: 3
	UINT32 h_v_sel                         : 3;   //  4: 6
	UINT32                                 : 1;   //     7 reserved
	UINT32 update                          : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 rnw                             : 1;   //    12
} M14A0_MSC_C_FIR_COEF_CTRL_L_T;

/*-----------------------------------------------------------------------------
	0x1168 msc_status_rev_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                        : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_status                        : 12;  // 16:27
} M14A0_MSC_STATUS_REV_L_T;

/*-----------------------------------------------------------------------------
	0x116c msc_manual_load_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size           : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 update_wout_size                : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 update_win_offset               : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 update_win_size                 : 1;   //    12
} M14A0_MSC_MANUAL_LOAD_L_T;

/*-----------------------------------------------------------------------------
	0x1170 msc_wout_offset_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wout_v_offset                   : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 wout_h_offset                   : 12;  // 16:27
} M14A0_MSC_WOUT_OFFSET_L_T;

/*-----------------------------------------------------------------------------
	0x1174 msc_chroma_tear_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tearing_count_result            : 8;   //  0: 7
} M14A0_MSC_CHROMA_TEAR_L_T;

/*-----------------------------------------------------------------------------
	0x1178 msc_chroma_tear_ctrl_l ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_chroma_filter_en           : 1;   //     0
	UINT32 vsc_chroma_pre_filter_en        : 1;   //     1
	UINT32 vsc_chroma_median_filter_en     : 1;   //     2
	UINT32 manual_chroma_pre_filter_on     : 1;   //     3
	UINT32 manual_chroma_median_filter_on  : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 detect_level_th                 : 8;   //  8:15
	UINT32 tearing_count_th                : 8;   // 16:23
	UINT32 frame_count_th                  : 4;   // 24:27
} M14A0_MSC_CHROMA_TEAR_CTRL_L_T;

/*-----------------------------------------------------------------------------
	0x1180 msc_min_max_ctrl_l_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en               : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 min_max_th                      : 10;  //  4:13
} M14A0_MSC_MIN_MAX_CTRL_L_Y_T;

/*-----------------------------------------------------------------------------
	0x1184 msc_min_max_ctrl_l_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en               : 1;   //     0
	UINT32 h_filter_en                     : 1;   //     1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 min_max_th                      : 10;  //  4:13
} M14A0_MSC_MIN_MAX_CTRL_L_C_T;

/*-----------------------------------------------------------------------------
	0x1190 msl_manual_sampling_y_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSL_MANUAL_SAMPLING_Y_H_T;

/*-----------------------------------------------------------------------------
	0x1194 msl_manual_sampling_y_v ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSL_MANUAL_SAMPLING_Y_V_T;

/*-----------------------------------------------------------------------------
	0x1198 msl_manual_sampling_c_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSL_MANUAL_SAMPLING_C_H_T;

/*-----------------------------------------------------------------------------
	0x119c msl_manual_sampling_c_v ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSL_MANUAL_SAMPLING_C_V_T;

/*-----------------------------------------------------------------------------
	0x11a0 chroma_v_cti ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_v_cti_enable             : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 v_cti_strength                  : 2;   //  4: 5
} M14A0_CHROMA_V_CTI_T;

/*-----------------------------------------------------------------------------
	0x3100 msc_ctrl_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnd_pel_filtering_scheme        : 1;   //     0
	UINT32 adaptive_fir_filter_en          : 1;   //     1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 filter_type_sel_luma            : 1;   //     4
	UINT32 filter_type_sel_chroma          : 1;   //     5
	UINT32 manual_sampling_mode            : 1;   //     6
	UINT32                                 : 1;   //     7 reserved
	UINT32 size_info_manual_load           : 1;   //     8
} M14A0_MSC_CTRL_R_T;

/*-----------------------------------------------------------------------------
	0x3104 msc_y_init_pos_h_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                    : 6;   //  0: 5
} M14A0_MSC_Y_INIT_POS_H_R_T;

/*-----------------------------------------------------------------------------
	0x3108 msc_y_init_pos_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                    : 6;   //  0: 5
} M14A0_MSC_Y_INIT_POS_V_R_T;

/*-----------------------------------------------------------------------------
	0x310c msc_c_init_pos_h_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                    : 6;   //  0: 5
} M14A0_MSC_C_INIT_POS_H_R_T;

/*-----------------------------------------------------------------------------
	0x3110 msc_c_init_pos_v_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                    : 6;   //  0: 5
} M14A0_MSC_C_INIT_POS_V_R_T;

/*-----------------------------------------------------------------------------
	0x3118 msc_scr_size_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 input_v_size                    : 12;  // 16:27
} M14A0_MSC_SCR_SIZE_R_T;

/*-----------------------------------------------------------------------------
	0x311c msc_win_out_size_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 out_win_v_size                  : 12;  // 16:27
} M14A0_MSC_WIN_OUT_SIZE_R_T;

/*-----------------------------------------------------------------------------
	0x3120 msc_win_in_offset_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset                 : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 in_win_v_offset                 : 12;  // 16:27
} M14A0_MSC_WIN_IN_OFFSET_R_T;

/*-----------------------------------------------------------------------------
	0x3124 msc_win_in_size_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size                   : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 in_win_v_size                   : 12;  // 16:27
} M14A0_MSC_WIN_IN_SIZE_R_T;

/*-----------------------------------------------------------------------------
	0x3128 msc_c_format_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                     : 3;   //  0: 2
	UINT32                                 : 13;  //  3:15 reserved
	UINT32 out_c_format                    : 3;   // 16:18
} M14A0_MSC_C_FORMAT_R_T;

/*-----------------------------------------------------------------------------
	0x3130 msc_y_fir_coef_data0_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient1                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA0_R_T;

/*-----------------------------------------------------------------------------
	0x3134 msc_y_fir_coef_data1_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient3                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA1_R_T;

/*-----------------------------------------------------------------------------
	0x3138 msc_y_fir_coef_data2_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient5                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA2_R_T;

/*-----------------------------------------------------------------------------
	0x313c msc_y_fir_coef_data3_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient7                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA3_R_T;

/*-----------------------------------------------------------------------------
	0x3140 msc_y_fir_coef_data4_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient9                  : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA4_R_T;

/*-----------------------------------------------------------------------------
	0x3144 msc_y_fir_coef_data5_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                 : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_coefficient11                 : 12;  // 16:27
} M14A0_MSC_Y_FIR_COEF_DATA5_R_T;

/*-----------------------------------------------------------------------------
	0x3148 msc_y_fir_coef_ctrl_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                            : 4;   //  0: 3
	UINT32 h_v_sel                         : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 update                          : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 rnw                             : 1;   //    12
} M14A0_MSC_Y_FIR_COEF_CTRL_R_T;

/*-----------------------------------------------------------------------------
	0x3150 msc_c_fir_coef_data0_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient0                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient1                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA0_R_T;

/*-----------------------------------------------------------------------------
	0x3154 msc_c_fir_coef_data1_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient2                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient3                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA1_R_T;

/*-----------------------------------------------------------------------------
	0x3158 msc_c_fir_coef_data2_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient4                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient5                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA2_R_T;

/*-----------------------------------------------------------------------------
	0x315c msc_c_fir_coef_data3_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient6                  : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_coefficient7                  : 12;  // 16:27
} M14A0_MSC_C_FIR_COEF_DATA3_R_T;

/*-----------------------------------------------------------------------------
	0x3160 msc_c_fir_coef_ctrl_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                            : 4;   //  0: 3
	UINT32 h_v_sel                         : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 update                          : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 rnw                             : 1;   //    12
} M14A0_MSC_C_FIR_COEF_CTRL_R_T;

/*-----------------------------------------------------------------------------
	0x3168 msc_status_rev_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                        : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_status                        : 12;  // 16:27
} M14A0_MSC_STATUS_REV_R_T;

/*-----------------------------------------------------------------------------
	0x316c msc_manual_road_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size           : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 update_wout_size                : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 update_win_offset               : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 update_win_size                 : 1;   //    12
} M14A0_MSC_MANUAL_ROAD_R_T;

/*-----------------------------------------------------------------------------
	0x3170 msc_wout_offset_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 wout_v_offset                   : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 wout_h_offset                   : 12;  // 16:27
} M14A0_MSC_WOUT_OFFSET_R_T;

/*-----------------------------------------------------------------------------
	0x3174 msc_chroma_tear_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tearing_count_result            : 8;   //  0: 7
} M14A0_MSC_CHROMA_TEAR_R_T;

/*-----------------------------------------------------------------------------
	0x3178 msc_chroma_tear_ctrl_r ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_chroma_filter_en           : 1;   //     0
	UINT32 vsc_chroma_pre_filter_en        : 1;   //     1
	UINT32 vsc_chroma_median_filter_en     : 1;   //     2
	UINT32 manual_chroma_pre_filter_on     : 1;   //     3
	UINT32 manual_chroma_median_filter_on  : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 detect_level_th                 : 8;   //  8:15
	UINT32 tearing_count_th                : 8;   // 16:23
	UINT32 frame_count_th                  : 4;   // 24:27
} M14A0_MSC_CHROMA_TEAR_CTRL_R_T;

/*-----------------------------------------------------------------------------
	0x3180 msc_min_max_ctrl_r_y ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en               : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 min_max_th                      : 10;  //  4:13
} M14A0_MSC_MIN_MAX_CTRL_R_Y_T;

/*-----------------------------------------------------------------------------
	0x3184 msc_min_max_ctrl_r_c ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en               : 1;   //     0
	UINT32 h_filter_en                     : 1;   //     1
	UINT32                                 : 2;   //  2: 3 reserved
	UINT32 min_max_th                      : 10;  //  4:13
} M14A0_MSC_MIN_MAX_CTRL_R_C_T;

/*-----------------------------------------------------------------------------
	0x3190 msr_manual_sampling_y_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSR_MANUAL_SAMPLING_Y_H_T;

/*-----------------------------------------------------------------------------
	0x3194 msr_manual_sampling_y_v ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSR_MANUAL_SAMPLING_Y_V_T;

/*-----------------------------------------------------------------------------
	0x3198 msr_manual_sampling_c_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSR_MANUAL_SAMPLING_C_H_T;

/*-----------------------------------------------------------------------------
	0x319c msr_manual_sampling_c_v ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 denominator                     : 12;  // 16:27
} M14A0_MSR_MANUAL_SAMPLING_C_V_T;

typedef struct {
	M14A0_MSC_CTRL_L_T                  msc_ctrl_l;                          // 0x1100
	M14A0_MSC_Y_INIT_POS_H_L_T          msc_y_init_pos_h_l;                  // 0x1104
	M14A0_MSC_Y_INIT_POS_V_L_T          msc_y_init_pos_v_l;                  // 0x1108
	M14A0_MSC_C_INIT_POS_H_L_T          msc_c_init_pos_h_l;                  // 0x110c
	M14A0_MSC_C_INIT_POS_V_L_T          msc_c_init_pos_v_l;                  // 0x1110
	M14A0_MSC_ADAPTIVE_TH_L_T           msc_adaptive_th_l;                   // 0x1114
	M14A0_MSC_SCR_SIZE_L_T              msc_scr_size_l;                      // 0x1118
	M14A0_MSC_WIN_OUT_SIZE_L_T          msc_win_out_size_l;                  // 0x111c
	M14A0_MSC_WIN_IN_OFFSET_L_T         msc_win_in_offset_l;                 // 0x1120
	M14A0_MSC_WIN_IN_SIZE_L_T           msc_win_in_size_l;                   // 0x1124
	M14A0_MSC_C_FORMAT_L_T              msc_c_format_l;                      // 0x1128
	UINT32                              reserved0;                           // 0x112c
	M14A0_MSC_Y_FIR_COEF_DATA0_L_T      msc_y_fir_coef_data0_l;              // 0x1130
	M14A0_MSC_Y_FIR_COEF_DATA1_L_T      msc_y_fir_coef_data1_l;              // 0x1134
	M14A0_MSC_Y_FIR_COEF_DATA2_L_T      msc_y_fir_coef_data2_l;              // 0x1138
	M14A0_MSC_Y_FIR_COEF_DATA3_L_T      msc_y_fir_coef_data3_l;              // 0x113c
	M14A0_MSC_Y_FIR_COEF_DATA4_L_T      msc_y_fir_coef_data4_l;              // 0x1140
	M14A0_MSC_Y_FIR_COEF_DATA5_L_T      msc_y_fir_coef_data5_l;              // 0x1144
	M14A0_MSC_Y_FIR_COEF_CTRL_L_T       msc_y_fir_coef_ctrl_l;               // 0x1148
	UINT32                              reserved1;                           // 0x114c
	M14A0_MSC_C_FIR_COEF_DATA0_L_T      msc_c_fir_coef_data0_l;              // 0x1150
	M14A0_MSC_C_FIR_COEF_DATA1_L_T      msc_c_fir_coef_data1_l;              // 0x1154
	M14A0_MSC_C_FIR_COEF_DATA2_L_T      msc_c_fir_coef_data2_l;              // 0x1158
	M14A0_MSC_C_FIR_COEF_DATA3_L_T      msc_c_fir_coef_data3_l;              // 0x115c
	M14A0_MSC_C_FIR_COEF_CTRL_L_T       msc_c_fir_coef_ctrl_l;               // 0x1160
	UINT32                              reserved2;                           // 0x1164
	M14A0_MSC_STATUS_REV_L_T            msc_status_rev_l;                    // 0x1168
	M14A0_MSC_MANUAL_LOAD_L_T           msc_manual_load_l;                   // 0x116c
	M14A0_MSC_WOUT_OFFSET_L_T           msc_wout_offset_l;                   // 0x1170
	M14A0_MSC_CHROMA_TEAR_L_T           msc_chroma_tear_l;                   // 0x1174
	M14A0_MSC_CHROMA_TEAR_CTRL_L_T      msc_chroma_tear_ctrl_l;              // 0x1178
	UINT32                              reserved3;                           // 0x117c
	M14A0_MSC_MIN_MAX_CTRL_L_Y_T        msc_min_max_ctrl_l_y;                // 0x1180
	M14A0_MSC_MIN_MAX_CTRL_L_C_T        msc_min_max_ctrl_l_c;                // 0x1184
	UINT32                              reserved4[2];                        // 0x1188~0x118c
	M14A0_MSL_MANUAL_SAMPLING_Y_H_T     msl_manual_sampling_y_h;             // 0x1190
	M14A0_MSL_MANUAL_SAMPLING_Y_V_T     msl_manual_sampling_y_v;             // 0x1194
	M14A0_MSL_MANUAL_SAMPLING_C_H_T     msl_manual_sampling_c_h;             // 0x1198
	M14A0_MSL_MANUAL_SAMPLING_C_V_T     msl_manual_sampling_c_v;             // 0x119c
	M14A0_CHROMA_V_CTI_T                chroma_v_cti;                        // 0x11a0
} DE_MSL_REG_M14A0_T;

typedef struct {
	M14A0_MSC_CTRL_R_T                  msc_ctrl_r;                          // 0x3100
	M14A0_MSC_Y_INIT_POS_H_R_T          msc_y_init_pos_h_r;                  // 0x3104
	M14A0_MSC_Y_INIT_POS_V_R_T          msc_y_init_pos_v_r;                  // 0x3108
	M14A0_MSC_C_INIT_POS_H_R_T          msc_c_init_pos_h_r;                  // 0x310c
	M14A0_MSC_C_INIT_POS_V_R_T          msc_c_init_pos_v_r;                  // 0x3110
	UINT32                              reserved5;                           // 0x3114
	M14A0_MSC_SCR_SIZE_R_T              msc_scr_size_r;                      // 0x3118
	M14A0_MSC_WIN_OUT_SIZE_R_T          msc_win_out_size_r;                  // 0x311c
	M14A0_MSC_WIN_IN_OFFSET_R_T         msc_win_in_offset_r;                 // 0x3120
	M14A0_MSC_WIN_IN_SIZE_R_T           msc_win_in_size_r;                   // 0x3124
	M14A0_MSC_C_FORMAT_R_T              msc_c_format_r;                      // 0x3128
	UINT32                              reserved6;                           // 0x312c
	M14A0_MSC_Y_FIR_COEF_DATA0_R_T      msc_y_fir_coef_data0_r;              // 0x3130
	M14A0_MSC_Y_FIR_COEF_DATA1_R_T      msc_y_fir_coef_data1_r;              // 0x3134
	M14A0_MSC_Y_FIR_COEF_DATA2_R_T      msc_y_fir_coef_data2_r;              // 0x3138
	M14A0_MSC_Y_FIR_COEF_DATA3_R_T      msc_y_fir_coef_data3_r;              // 0x313c
	M14A0_MSC_Y_FIR_COEF_DATA4_R_T      msc_y_fir_coef_data4_r;              // 0x3140
	M14A0_MSC_Y_FIR_COEF_DATA5_R_T      msc_y_fir_coef_data5_r;              // 0x3144
	M14A0_MSC_Y_FIR_COEF_CTRL_R_T       msc_y_fir_coef_ctrl_r;               // 0x3148
	UINT32                              reserved7;                           // 0x314c
	M14A0_MSC_C_FIR_COEF_DATA0_R_T      msc_c_fir_coef_data0_r;              // 0x3150
	M14A0_MSC_C_FIR_COEF_DATA1_R_T      msc_c_fir_coef_data1_r;              // 0x3154
	M14A0_MSC_C_FIR_COEF_DATA2_R_T      msc_c_fir_coef_data2_r;              // 0x3158
	M14A0_MSC_C_FIR_COEF_DATA3_R_T      msc_c_fir_coef_data3_r;              // 0x315c
	M14A0_MSC_C_FIR_COEF_CTRL_R_T       msc_c_fir_coef_ctrl_r;               // 0x3160
	UINT32                              reserved8;                           // 0x3164
	M14A0_MSC_STATUS_REV_R_T            msc_status_rev_r;                    // 0x3168
	M14A0_MSC_MANUAL_ROAD_R_T           msc_manual_road_r;                   // 0x316c
	M14A0_MSC_WOUT_OFFSET_R_T           msc_wout_offset_r;                   // 0x3170
	M14A0_MSC_CHROMA_TEAR_R_T           msc_chroma_tear_r;                   // 0x3174
	M14A0_MSC_CHROMA_TEAR_CTRL_R_T      msc_chroma_tear_ctrl_r;              // 0x3178
	UINT32                              reserved9;                           // 0x317c
	M14A0_MSC_MIN_MAX_CTRL_R_Y_T        msc_min_max_ctrl_r_y;                // 0x3180
	M14A0_MSC_MIN_MAX_CTRL_R_C_T        msc_min_max_ctrl_r_c;                // 0x3184
	UINT32                              reserved10[2];                       // 0x3188~0x318c
	M14A0_MSR_MANUAL_SAMPLING_Y_H_T     msr_manual_sampling_y_h;             // 0x3190
	M14A0_MSR_MANUAL_SAMPLING_Y_V_T     msr_manual_sampling_y_v;             // 0x3194
	M14A0_MSR_MANUAL_SAMPLING_C_H_T     msr_manual_sampling_c_h;             // 0x3198
	M14A0_MSR_MANUAL_SAMPLING_C_V_T     msr_manual_sampling_c_v;             // 0x319c
} DE_MSR_REG_M14A0_T;

#endif
