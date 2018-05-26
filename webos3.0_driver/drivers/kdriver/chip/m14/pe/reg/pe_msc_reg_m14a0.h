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

#ifndef _PE_MSC_REG_M14A0_H_
#define _PE_MSC_REG_M14A0_H_
/*-----------------------------------------------------------------------------
		0xc0014100L msc_ctrl_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnd_pel_filtering_scheme           : 1 ;  //     0     
	UINT32 adaptive_fir_filter_en             : 1 ;  //     1     
	UINT32 v_scaler_config                    : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 filter_type_sel_luma               : 1 ;  //     4     
	UINT32 filter_type_sel_chroma             : 1 ;  //     5     
	UINT32 manual_sampling_mode               : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 size_info_manual_load              : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 bilinear_y_h_off                   : 1 ;  //    12     
	UINT32 bilinear_y_v_off                   : 1 ;  //    13     
	UINT32 bilinear_c_h_off                   : 1 ;  //    14     
	UINT32 bilinear_c_v_off                   : 1 ;  //    15     
	UINT32                                    : 14;  // 29:16     reserved
	UINT32 dvr_write_mode                     : 1 ;  //    30     
	UINT32 bit_8_mode_en                      : 1 ;  //    31     
} PE_M14A0_MSC_CTRL_L_T;

/*-----------------------------------------------------------------------------
		0xc0014104L msc_y_init_pos_h_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_Y_INIT_POS_H_L_T;

/*-----------------------------------------------------------------------------
		0xc0014108L msc_y_init_pos_v_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_Y_INIT_POS_V_L_T;

/*-----------------------------------------------------------------------------
		0xc001410cL msc_c_init_pos_h_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_C_INIT_POS_H_L_T;

/*-----------------------------------------------------------------------------
		0xc0014110L msc_c_init_pos_v_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_C_INIT_POS_V_L_T;

/*-----------------------------------------------------------------------------
		0xc0014114L msc_adaptive_th_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_th                             : 4 ;  //  3: 0     
	UINT32 blending_range                     : 2 ;  //  5: 4     
} PE_M14A0_MSC_ADAPTIVE_TH_L_T;

/*-----------------------------------------------------------------------------
		0xc0014118L msc_scr_size_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 input_v_size                       : 12;  // 27:16     
} PE_M14A0_MSC_SCR_SIZE_L_T;

/*-----------------------------------------------------------------------------
		0xc001411cL msc_win_out_size_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 out_win_v_size                     : 12;  // 27:16     
} PE_M14A0_MSC_WIN_OUT_SIZE_L_T;

/*-----------------------------------------------------------------------------
		0xc0014120L msc_win_in_offset_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 in_win_v_offset                    : 12;  // 27:16     
} PE_M14A0_MSC_WIN_IN_OFFSET_L_T;

/*-----------------------------------------------------------------------------
		0xc0014124L msc_win_in_size_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 in_win_v_size                      : 12;  // 27:16     
} PE_M14A0_MSC_WIN_IN_SIZE_L_T;

/*-----------------------------------------------------------------------------
		0xc0014128L msc_c_format_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                        : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 out_c_format                       : 3 ;  // 18:16     
} PE_M14A0_MSC_C_FORMAT_L_T;

/*-----------------------------------------------------------------------------
		0xc0014130L msc_y_fir_coef_data0_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA0_L_T;

/*-----------------------------------------------------------------------------
		0xc0014134L msc_y_fir_coef_data1_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA1_L_T;

/*-----------------------------------------------------------------------------
		0xc0014138L msc_y_fir_coef_data2_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA2_L_T;

/*-----------------------------------------------------------------------------
		0xc001413cL msc_y_fir_coef_data3_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA3_L_T;

/*-----------------------------------------------------------------------------
		0xc0014140L msc_y_fir_coef_data4_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA4_L_T;

/*-----------------------------------------------------------------------------
		0xc0014144L msc_y_fir_coef_data5_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA5_L_T;

/*-----------------------------------------------------------------------------
		0xc0014148L msc_y_fir_coef_ctrl_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} PE_M14A0_MSC_Y_FIR_COEF_CTRL_L_T;

/*-----------------------------------------------------------------------------
		0xc0014150L msc_c_fir_coef_data0_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient1                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA0_L_T;

/*-----------------------------------------------------------------------------
		0xc0014154L msc_c_fir_coef_data1_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient3                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA1_L_T;

/*-----------------------------------------------------------------------------
		0xc0014158L msc_c_fir_coef_data2_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient5                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA2_L_T;

/*-----------------------------------------------------------------------------
		0xc001415cL msc_c_fir_coef_data3_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient7                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA3_L_T;

/*-----------------------------------------------------------------------------
		0xc0014160L msc_c_fir_coef_ctrl_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} PE_M14A0_MSC_C_FIR_COEF_CTRL_L_T;

/*-----------------------------------------------------------------------------
		0xc0014168L msc_status_rev_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_status                           : 12;  // 27:16     
	UINT32 pic_init_cnt                       : 4 ;  // 31:28     
} PE_M14A0_MSC_STATUS_REV_L_T;

/*-----------------------------------------------------------------------------
		0xc001416cL msc_manual_load_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 update_wout_size                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update_win_offset                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 update_win_size                    : 1 ;  //    12     
} PE_M14A0_MSC_MANUAL_LOAD_L_T;

/*-----------------------------------------------------------------------------
		0xc0014170L msc_wout_offset_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wout_v_offset                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wout_h_offset                      : 12;  // 27:16     
} PE_M14A0_MSC_WOUT_OFFSET_L_T;

/*-----------------------------------------------------------------------------
		0xc0014174L msc_chroma_tear_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tearing_count_result               : 8 ;  //  7: 0     
} PE_M14A0_MSC_CHROMA_TEAR_L_T;

/*-----------------------------------------------------------------------------
		0xc0014178L msc_chroma_tear_ctrl_l ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_chroma_filter_en              : 1 ;  //     0     
	UINT32 vsc_chroma_pre_filter_en           : 1 ;  //     1     
	UINT32 vsc_chroma_median_filter_en        : 1 ;  //     2     
	UINT32 manual_chroma_pre_filter_on        : 1 ;  //     3     
	UINT32 manual_chroma_median_filter_on     : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 detect_level_th                    : 8 ;  // 15: 8     
	UINT32 tearing_count_th                   : 8 ;  // 23:16     
	UINT32 frame_count_th                     : 4 ;  // 27:24     
} PE_M14A0_MSC_CHROMA_TEAR_CTRL_L_T;

/*-----------------------------------------------------------------------------
		0xc0014180L msc_min_max_ctrl_l_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 min_max_th                         : 10;  // 13: 4     
} PE_M14A0_MSC_MIN_MAX_CTRL_L_Y_T;

/*-----------------------------------------------------------------------------
		0xc0014184L msc_min_max_ctrl_l_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en                  : 1 ;  //     0     
	UINT32 h_filter_en                        : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 min_max_th                         : 10;  // 13: 4     
} PE_M14A0_MSC_MIN_MAX_CTRL_L_C_T;

/*-----------------------------------------------------------------------------
		0xc0014190L manual_sampling_y_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_L_Y_H_T;

/*-----------------------------------------------------------------------------
		0xc0014194L manual_sampling_y_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_L_Y_V_T;

/*-----------------------------------------------------------------------------
		0xc0014198L manual_sampling_c_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_L_C_H_T;

/*-----------------------------------------------------------------------------
		0xc001419cL manual_sampling_c_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_L_C_V_T;

/*-----------------------------------------------------------------------------
		0xc00141a0L chroma_v_cti ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 chroma_v_cti_enable                : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 v_cti_strength                     : 2 ;  //  5: 4     
} PE_M14A0_CHROMA_L_V_CTI_T;

/*-----------------------------------------------------------------------------
		0xc00141a4L msc_frame_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 frame_cnt1                         : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 frame_cnt2                         : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 frame_cnt_reset                    : 1 ;  //    31     
} PE_M14A0_MSC_FRAME_CNT_L_T;

/*-----------------------------------------------------------------------------
		0xc0016100L msc_ctrl_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 bnd_pel_filtering_scheme           : 1 ;  //     0     
	UINT32 adaptive_fir_filter_en             : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 filter_type_sel_luma               : 1 ;  //     4     
	UINT32 filter_type_sel_chroma             : 1 ;  //     5     
	UINT32 manual_sampling_mode               : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 size_info_manual_load              : 1 ;  //     8     
	UINT32                                    : 21;  // 29: 9     reserved
	UINT32 dvr_write_mode                     : 1 ;  //    30     
} PE_M14A0_MSC_CTRL_R_T;

/*-----------------------------------------------------------------------------
		0xc0016104L msc_y_init_pos_h_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_Y_INIT_POS_H_R_T;

/*-----------------------------------------------------------------------------
		0xc0016108L msc_y_init_pos_v_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_Y_INIT_POS_V_R_T;

/*-----------------------------------------------------------------------------
		0xc001610cL msc_c_init_pos_h_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_C_INIT_POS_H_R_T;

/*-----------------------------------------------------------------------------
		0xc0016110L msc_c_init_pos_v_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                       : 6 ;  //  5: 0     
} PE_M14A0_MSC_C_INIT_POS_V_R_T;

/*-----------------------------------------------------------------------------
		0xc0016118L msc_scr_size_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 input_v_size                       : 12;  // 27:16     
} PE_M14A0_MSC_SCR_SIZE_R_T;

/*-----------------------------------------------------------------------------
		0xc001611cL msc_win_out_size_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 out_win_v_size                     : 12;  // 27:16     
} PE_M14A0_MSC_WIN_OUT_SIZE_R_T;

/*-----------------------------------------------------------------------------
		0xc0016120L msc_win_in_offset_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 in_win_v_offset                    : 12;  // 27:16     
} PE_M14A0_MSC_WIN_IN_OFFSET_R_T;

/*-----------------------------------------------------------------------------
		0xc0016124L msc_win_in_size_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 in_win_v_size                      : 12;  // 27:16     
} PE_M14A0_MSC_WIN_IN_SIZE_R_T;

/*-----------------------------------------------------------------------------
		0xc0016128L msc_c_format_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                        : 3 ;  //  2: 0     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 out_c_format                       : 3 ;  // 18:16     
} PE_M14A0_MSC_C_FORMAT_R_T;

/*-----------------------------------------------------------------------------
		0xc0016130L msc_y_fir_coef_data0_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient1                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA0_R_T;

/*-----------------------------------------------------------------------------
		0xc0016134L msc_y_fir_coef_data1_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient3                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA1_R_T;

/*-----------------------------------------------------------------------------
		0xc0016138L msc_y_fir_coef_data2_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient5                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA2_R_T;

/*-----------------------------------------------------------------------------
		0xc001613cL msc_y_fir_coef_data3_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient7                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA3_R_T;

/*-----------------------------------------------------------------------------
		0xc0016140L msc_y_fir_coef_data4_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient8                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient9                     : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA4_R_T;

/*-----------------------------------------------------------------------------
		0xc0016144L msc_y_fir_coef_data5_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_coefficient10                    : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_coefficient11                    : 12;  // 27:16     
} PE_M14A0_MSC_Y_FIR_COEF_DATA5_R_T;

/*-----------------------------------------------------------------------------
		0xc0016148L msc_y_fir_coef_ctrl_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} PE_M14A0_MSC_Y_FIR_COEF_CTRL_R_T;

/*-----------------------------------------------------------------------------
		0xc0016150L msc_c_fir_coef_data0_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient0                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient1                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA0_R_T;

/*-----------------------------------------------------------------------------
		0xc0016154L msc_c_fir_coef_data1_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient2                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient3                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA1_R_T;

/*-----------------------------------------------------------------------------
		0xc0016158L msc_c_fir_coef_data2_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient4                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient5                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA2_R_T;

/*-----------------------------------------------------------------------------
		0xc001615cL msc_c_fir_coef_data3_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_coefficient6                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_coefficient7                     : 12;  // 27:16     
} PE_M14A0_MSC_C_FIR_COEF_DATA3_R_T;

/*-----------------------------------------------------------------------------
		0xc0016160L msc_c_fir_coef_ctrl_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                               : 4 ;  //  3: 0     
	UINT32 h_v_sel                            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update                             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 rnw                                : 1 ;  //    12     
} PE_M14A0_MSC_C_FIR_COEF_CTRL_R_T;

/*-----------------------------------------------------------------------------
		0xc0016168L msc_status_rev_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_status                           : 12;  // 27:16     
	UINT32 pic_init_cnt                       : 4 ;  // 31:28     
} PE_M14A0_MSC_STATUS_REV_R_T;

/*-----------------------------------------------------------------------------
		0xc001616cL msc_manual_road_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 update_wout_size                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 update_win_offset                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 update_win_size                    : 1 ;  //    12     
} PE_M14A0_MSC_MANUAL_ROAD_R_T;

/*-----------------------------------------------------------------------------
		0xc0016170L msc_wout_offset_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 wout_v_offset                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 wout_h_offset                      : 12;  // 27:16     
} PE_M14A0_MSC_WOUT_OFFSET_R_T;

/*-----------------------------------------------------------------------------
		0xc0016174L msc_chroma_tear_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tearing_count_result               : 8 ;  //  7: 0     
} PE_M14A0_MSC_CHROMA_TEAR_R_T;

/*-----------------------------------------------------------------------------
		0xc0016178L msc_chroma_tear_ctrl_r ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 auto_chroma_filter_en              : 1 ;  //     0     
	UINT32 vsc_chroma_pre_filter_en           : 1 ;  //     1     
	UINT32 vsc_chroma_median_filter_en        : 1 ;  //     2     
	UINT32 manual_chroma_pre_filter_on        : 1 ;  //     3     
	UINT32 manual_chroma_median_filter_on     : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 detect_level_th                    : 8 ;  // 15: 8     
	UINT32 tearing_count_th                   : 8 ;  // 23:16     
	UINT32 frame_count_th                     : 4 ;  // 27:24     
} PE_M14A0_MSC_CHROMA_TEAR_CTRL_R_T;

/*-----------------------------------------------------------------------------
		0xc0016180L msc_min_max_ctrl_r_y ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 min_max_th                         : 10;  // 13: 4     
} PE_M14A0_MSC_MIN_MAX_CTRL_R_Y_T;

/*-----------------------------------------------------------------------------
		0xc0016184L msc_min_max_ctrl_r_c ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 min_max_filter_en                  : 1 ;  //     0     
	UINT32 h_filter_en                        : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 min_max_th                         : 10;  // 13: 4     
} PE_M14A0_MSC_MIN_MAX_CTRL_R_C_T;

/*-----------------------------------------------------------------------------
		0xc0016190L manual_sampling_y_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_R_Y_H_T;

/*-----------------------------------------------------------------------------
		0xc0016194L manual_sampling_y_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_R_Y_V_T;

/*-----------------------------------------------------------------------------
		0xc0016198L manual_sampling_c_h ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_R_C_H_T;

/*-----------------------------------------------------------------------------
		0xc001619cL manual_sampling_c_v ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 numerator                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 denominator                        : 12;  // 27:16     
} PE_M14A0_MANUAL_SAMPLING_R_C_V_T;

/*-----------------------------------------------------------------------------
		0xc00161a4L msc_frame_cnt ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 frame_cnt1                         : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 frame_cnt2                         : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 frame_cnt_reset                    : 1 ;  //    31     
} PE_M14A0_MSC_FRAME_CNT_R_T;

typedef struct {
	PE_M14A0_MSC_CTRL_L_T              	msc_ctrl_l;                   //0xc0014100L
	PE_M14A0_MSC_Y_INIT_POS_H_L_T      	msc_y_init_pos_h_l;           //0xc0014104L
	PE_M14A0_MSC_Y_INIT_POS_V_L_T      	msc_y_init_pos_v_l;           //0xc0014108L
	PE_M14A0_MSC_C_INIT_POS_H_L_T      	msc_c_init_pos_h_l;           //0xc001410cL
	PE_M14A0_MSC_C_INIT_POS_V_L_T      	msc_c_init_pos_v_l;           //0xc0014110L
	PE_M14A0_MSC_ADAPTIVE_TH_L_T       	msc_adaptive_th_l;            //0xc0014114L
	PE_M14A0_MSC_SCR_SIZE_L_T          	msc_scr_size_l;               //0xc0014118L
	PE_M14A0_MSC_WIN_OUT_SIZE_L_T      	msc_win_out_size_l;           //0xc001411cL
	PE_M14A0_MSC_WIN_IN_OFFSET_L_T     	msc_win_in_offset_l;          //0xc0014120L
	PE_M14A0_MSC_WIN_IN_SIZE_L_T       	msc_win_in_size_l;            //0xc0014124L
	PE_M14A0_MSC_C_FORMAT_L_T          	msc_c_format_l;               //0xc0014128L
	UINT32                             	reserved00;                   //0xc001412cL
	PE_M14A0_MSC_Y_FIR_COEF_DATA0_L_T  	msc_y_fir_coef_data0_l;       //0xc0014130L
	PE_M14A0_MSC_Y_FIR_COEF_DATA1_L_T  	msc_y_fir_coef_data1_l;       //0xc0014134L
	PE_M14A0_MSC_Y_FIR_COEF_DATA2_L_T  	msc_y_fir_coef_data2_l;       //0xc0014138L
	PE_M14A0_MSC_Y_FIR_COEF_DATA3_L_T  	msc_y_fir_coef_data3_l;       //0xc001413cL
	PE_M14A0_MSC_Y_FIR_COEF_DATA4_L_T  	msc_y_fir_coef_data4_l;       //0xc0014140L
	PE_M14A0_MSC_Y_FIR_COEF_DATA5_L_T  	msc_y_fir_coef_data5_l;       //0xc0014144L
	PE_M14A0_MSC_Y_FIR_COEF_CTRL_L_T   	msc_y_fir_coef_ctrl_l;        //0xc0014148L
	UINT32                             	reserved01;                   //0xc001414cL
	PE_M14A0_MSC_C_FIR_COEF_DATA0_L_T  	msc_c_fir_coef_data0_l;       //0xc0014150L
	PE_M14A0_MSC_C_FIR_COEF_DATA1_L_T  	msc_c_fir_coef_data1_l;       //0xc0014154L
	PE_M14A0_MSC_C_FIR_COEF_DATA2_L_T  	msc_c_fir_coef_data2_l;       //0xc0014158L
	PE_M14A0_MSC_C_FIR_COEF_DATA3_L_T  	msc_c_fir_coef_data3_l;       //0xc001415cL
	PE_M14A0_MSC_C_FIR_COEF_CTRL_L_T   	msc_c_fir_coef_ctrl_l;        //0xc0014160L
	UINT32                             	reserved02;                   //0xc0014164L
	PE_M14A0_MSC_STATUS_REV_L_T        	msc_status_rev_l;             //0xc0014168L
	PE_M14A0_MSC_MANUAL_LOAD_L_T       	msc_manual_load_l;            //0xc001416cL
	PE_M14A0_MSC_WOUT_OFFSET_L_T       	msc_wout_offset_l;            //0xc0014170L
	PE_M14A0_MSC_CHROMA_TEAR_L_T       	msc_chroma_tear_l;            //0xc0014174L
	PE_M14A0_MSC_CHROMA_TEAR_CTRL_L_T  	msc_chroma_tear_ctrl_l;       //0xc0014178L
	UINT32                             	reserved03;                   //0xc001417cL
	PE_M14A0_MSC_MIN_MAX_CTRL_L_Y_T    	msc_min_max_ctrl_l_y;         //0xc0014180L
	PE_M14A0_MSC_MIN_MAX_CTRL_L_C_T    	msc_min_max_ctrl_l_c;         //0xc0014184L
	UINT32                             	reserved04[2];                //0xc0014188L~0xc001418cL
	PE_M14A0_MANUAL_SAMPLING_L_Y_H_T    manual_sampling_l_y_h;        //0xc0014190L
	PE_M14A0_MANUAL_SAMPLING_L_Y_V_T    manual_sampling_l_y_v;        //0xc0014194L
	PE_M14A0_MANUAL_SAMPLING_L_C_H_T    manual_sampling_l_c_h;        //0xc0014198L
	PE_M14A0_MANUAL_SAMPLING_L_C_V_T    manual_sampling_l_c_v;        //0xc001419cL
	PE_M14A0_CHROMA_L_V_CTI_T           chroma_l_v_cti;               //0xc00141a0L
	PE_M14A0_MSC_FRAME_CNT_L_T          msc_frame_cnt_l;              //0xc00141a4L
}PE_MSL_REG_M14A0_T;
typedef struct {
	PE_M14A0_MSC_CTRL_R_T              	msc_ctrl_r;                   //0xc0016100L
	PE_M14A0_MSC_Y_INIT_POS_H_R_T      	msc_y_init_pos_h_r;           //0xc0016104L
	PE_M14A0_MSC_Y_INIT_POS_V_R_T      	msc_y_init_pos_v_r;           //0xc0016108L
	PE_M14A0_MSC_C_INIT_POS_H_R_T      	msc_c_init_pos_h_r;           //0xc001610cL
	PE_M14A0_MSC_C_INIT_POS_V_R_T      	msc_c_init_pos_v_r;           //0xc0016110L
	UINT32                             	reserved00;                   //0xc0016114L
	PE_M14A0_MSC_SCR_SIZE_R_T          	msc_scr_size_r;               //0xc0016118L
	PE_M14A0_MSC_WIN_OUT_SIZE_R_T      	msc_win_out_size_r;           //0xc001611cL
	PE_M14A0_MSC_WIN_IN_OFFSET_R_T     	msc_win_in_offset_r;          //0xc0016120L
	PE_M14A0_MSC_WIN_IN_SIZE_R_T       	msc_win_in_size_r;            //0xc0016124L
	PE_M14A0_MSC_C_FORMAT_R_T          	msc_c_format_r;               //0xc0016128L
	UINT32                             	reserved01;                   //0xc001612cL
	PE_M14A0_MSC_Y_FIR_COEF_DATA0_R_T  	msc_y_fir_coef_data0_r;       //0xc0016130L
	PE_M14A0_MSC_Y_FIR_COEF_DATA1_R_T  	msc_y_fir_coef_data1_r;       //0xc0016134L
	PE_M14A0_MSC_Y_FIR_COEF_DATA2_R_T  	msc_y_fir_coef_data2_r;       //0xc0016138L
	PE_M14A0_MSC_Y_FIR_COEF_DATA3_R_T  	msc_y_fir_coef_data3_r;       //0xc001613cL
	PE_M14A0_MSC_Y_FIR_COEF_DATA4_R_T  	msc_y_fir_coef_data4_r;       //0xc0016140L
	PE_M14A0_MSC_Y_FIR_COEF_DATA5_R_T  	msc_y_fir_coef_data5_r;       //0xc0016144L
	PE_M14A0_MSC_Y_FIR_COEF_CTRL_R_T   	msc_y_fir_coef_ctrl_r;        //0xc0016148L
	UINT32                             	reserved02;                   //0xc001614cL
	PE_M14A0_MSC_C_FIR_COEF_DATA0_R_T  	msc_c_fir_coef_data0_r;       //0xc0016150L
	PE_M14A0_MSC_C_FIR_COEF_DATA1_R_T  	msc_c_fir_coef_data1_r;       //0xc0016154L
	PE_M14A0_MSC_C_FIR_COEF_DATA2_R_T  	msc_c_fir_coef_data2_r;       //0xc0016158L
	PE_M14A0_MSC_C_FIR_COEF_DATA3_R_T  	msc_c_fir_coef_data3_r;       //0xc001615cL
	PE_M14A0_MSC_C_FIR_COEF_CTRL_R_T   	msc_c_fir_coef_ctrl_r;        //0xc0016160L
	UINT32                             	reserved03;                   //0xc0016164L
	PE_M14A0_MSC_STATUS_REV_R_T        	msc_status_rev_r;             //0xc0016168L
	PE_M14A0_MSC_MANUAL_ROAD_R_T       	msc_manual_road_r;            //0xc001616cL
	PE_M14A0_MSC_WOUT_OFFSET_R_T       	msc_wout_offset_r;            //0xc0016170L
	PE_M14A0_MSC_CHROMA_TEAR_R_T       	msc_chroma_tear_r;            //0xc0016174L
	PE_M14A0_MSC_CHROMA_TEAR_CTRL_R_T  	msc_chroma_tear_ctrl_r;       //0xc0016178L
	UINT32                             	reserved04;                   //0xc001617cL
	PE_M14A0_MSC_MIN_MAX_CTRL_R_Y_T    	msc_min_max_ctrl_r_y;         //0xc0016180L
	PE_M14A0_MSC_MIN_MAX_CTRL_R_C_T    	msc_min_max_ctrl_r_c;         //0xc0016184L
	UINT32                             	reserved05[2];                //0xc0016188L~0xc001618cL
	PE_M14A0_MANUAL_SAMPLING_R_Y_H_T    manual_sampling_r_y_h;        //0xc0016190L
	PE_M14A0_MANUAL_SAMPLING_R_Y_V_T    manual_sampling_r_y_v;        //0xc0016194L
	PE_M14A0_MANUAL_SAMPLING_R_C_H_T    manual_sampling_r_c_h;        //0xc0016198L
	PE_M14A0_MANUAL_SAMPLING_R_C_V_T    manual_sampling_r_c_v;        //0xc001619cL
	UINT32                             	reserved06;                   //0xc00161a0L
	PE_M14A0_MSC_FRAME_CNT_R_T          msc_frame_cnt_r;              //0xc00161a4L
}PE_MSR_REG_M14A0_T;
#endif