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

#ifndef _PE_CVI_REG_M14A0_H_
#define _PE_CVI_REG_M14A0_H_
/*-----------------------------------------------------------------------------
		0xc0013100L cva_top_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scan_type                          : 1 ;  //     0     
	UINT32 field_gen_mode                     : 2 ;  //  2: 1     
	UINT32 auto_field_gen                     : 2 ;  //  4: 3     
	UINT32 data_bit_mode                      : 1 ;  //     5     
	UINT32 chroma_sampling                    : 1 ;  //     6     
	UINT32 aspect_ratio                       : 1 ;  //     7     
	UINT32 yc_delay_mode_r                    : 2 ;  //  9: 8     
	UINT32 yc_delay_mode_b                    : 2 ;  // 11:10     
	UINT32 yc_delay_mode_g                    : 2 ;  // 13:12     
	UINT32                                    : 5 ;  // 18:14     reserved
	UINT32 auto_sync_polarity                 : 1 ;  //    19     
	UINT32 manual_sync_polarity               : 2 ;  // 21:20     
	UINT32 adc_cut_direction                  : 1 ;  //    22     
	UINT32 adc_bit_sel                        : 1 ;  //    23     
	UINT32 br_port_swap                       : 1 ;  //    24     
	UINT32 r_port_swap                        : 1 ;  //    25     
	UINT32 b_port_swap                        : 1 ;  //    26     
	UINT32 g_port_swap                        : 1 ;  //    27     
	UINT32 bit_numbering_lsb2                 : 3 ;  // 30:28     
	UINT32 data_en_mode                       : 1 ;  //    31     
} PE_M14A0_CVA_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013104L cva_offset_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 offset_v                           : 12;  // 27:16     
} PE_M14A0_CVA_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013108L cva_size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 size_v                             : 12;  // 27:16     
} PE_M14A0_CVA_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc001310cL cva_int_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 res_hfp                            : 10;  // 25:16     
} PE_M14A0_CVA_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013110L cva_int_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 res_vsync                          : 5 ;  // 12: 8     
	UINT32 res_field_add_line                 : 2 ;  // 14:13     
	UINT32 res_field_sel                      : 1 ;  //    15     
	UINT32 res_sync_sel                       : 1 ;  //    16     
} PE_M14A0_CVA_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0013114L cva_dig_filt_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient1                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient0                   : 10;  // 25:16     
} PE_M14A0_CVA_DIG_FILT_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013118L cva_dig_filt_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient3                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient2                   : 10;  // 25:16     
} PE_M14A0_CVA_DIG_FILT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc001311cL cva_dig_filt_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient5                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient4                   : 10;  // 25:16     
} PE_M14A0_CVA_DIG_FILT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc0013120L cva_dig_filt_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 25;  // 24: 0     
	UINT32 fir_sample_mode                    : 1 ;  //    25     
	UINT32 fir_coeff_load                     : 1 ;  //    26     
	UINT32 fir_en                             : 1 ;  //    27     
	UINT32 fir_y_en                           : 1 ;  //    28     
	UINT32 fir_sampling_rate                  : 2 ;  // 30:29     
	UINT32 fir_c_en                           : 1 ;  //    31     
} PE_M14A0_CVA_DIG_FILT_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0013124L cva_csc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient0                   : 15;  // 30:16     
} PE_M14A0_CVA_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013128L cva_csc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient2                   : 15;  // 30:16     
} PE_M14A0_CVA_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc001312cL cva_csc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient4                   : 15;  // 30:16     
} PE_M14A0_CVA_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc0013130L cva_csc_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient6                   : 15;  // 30:16     
} PE_M14A0_CVA_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0013134L cva_csc_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_coefficient8                   : 15;  // 30:16     
} PE_M14A0_CVA_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc0013138L cva_csc_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset1                        : 11;  // 26:16     
} PE_M14A0_CVA_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc001313cL cva_csc_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset3                        : 11;  // 26:16     
} PE_M14A0_CVA_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc0013140L cva_csc_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                     : 1 ;  //     0     
	UINT32 csc_en                             : 1 ;  //     1     
	UINT32 csc_msb_4b_only_mode               : 1 ;  //     2     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 csc_offset5                        : 11;  // 26:16     
} PE_M14A0_CVA_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
		0xc0013144L cva_misc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_type                       : 3 ;  //  2: 0     
	UINT32 pattern_csc                        : 1 ;  //     3     
	UINT32 pattern_detail                     : 4 ;  //  7: 4     
	UINT32 reg_update_pulse                   : 1 ;  //     8     
	UINT32 field_fix_mode                     : 2 ;  // 10: 9     
	UINT32 black_sel                          : 1 ;  //    11     
	UINT32 write_inhibit                      : 1 ;  //    12     
	UINT32 sim_mode                           : 1 ;  //    13     
	UINT32 black_off                          : 1 ;  //    14     
	UINT32 field_edge                         : 1 ;  //    15     
	UINT32 ha_gen                             : 1 ;  //    16     
	UINT32 xsize_divide                       : 2 ;  // 18:17     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 yc_delay_mode_r                    : 2 ;  // 21:20     
	UINT32 yc_delay_mode_b                    : 2 ;  // 23:22     
	UINT32 yc_delay_mode_g                    : 2 ;  // 25:24     
	UINT32 field_detect_vris                  : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_update_position                : 4 ;  // 31:28     
} PE_M14A0_CVA_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013148L cva_offset_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 voffset_read                       : 13;  // 28:16     
} PE_M14A0_CVA_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
		0xc001314cL cva_pre3d_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_sync_enable                  : 1 ;  //     0     
	UINT32 pre3d_sync_mode                    : 2 ;  //  2: 1     
	UINT32 pre3d_field_mode                   : 1 ;  //     3     
	UINT32 pre3d_frame_lr_mode                : 1 ;  //     4     
	UINT32 pre3d_mid_vsync_bypass             : 1 ;  //     5     
	UINT32 pre3d_field_bypass                 : 1 ;  //     6     
	UINT32 reserved                           : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pre3d_active_line_num              : 12;  // 27:16     
} PE_M14A0_CVA_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013150L cva_pre3d_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_active_space2_line_num       : 12;  // 27:16     
} PE_M14A0_CVA_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0013154L cva_pre3d_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_sync_line_num                : 12;  // 27:16     
} PE_M14A0_CVA_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc0013158L cva_size_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_hoffset_indurance_read          : 8 ;  //  7: 0     
	UINT32 sd_voffset_indurance_read          : 8 ;  // 15: 8     
	UINT32 sd_hsync_fault_flag_read           : 1 ;  //    16     
	UINT32 sd_vsync_fault_flag_read           : 1 ;  //    17     
	UINT32 h_sync_polarity                    : 1 ;  //    18     
	UINT32 v_sync_polarity                    : 1 ;  //    19     
	UINT32 field_signal                       : 4 ;  // 23:20     
	UINT32 hsync_at_vsync_fall                : 4 ;  // 27:24     
	UINT32 outfield_at_vactive_fall           : 2 ;  // 29:28     
	UINT32 hoffset_over                       : 1 ;  //    30     
	UINT32 v_offset_over                      : 1 ;  //    31     
} PE_M14A0_CVA_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc001315cL cva_size_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 de_line_count                      : 12;  // 27:16     
} PE_M14A0_CVA_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc0013160L cva_size_status3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 hd_line_size                       : 12;  // 27:16     
} PE_M14A0_CVA_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
		0xc0013164L cva_size_status4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vd_line_count                      : 12;  // 27:16     
} PE_M14A0_CVA_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
		0xc0013168L cva_pix_sum_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_x                          : 12;  // 27:16     
	UINT32 pix_sum_enable                     : 1 ;  //    28     
	UINT32 field_selection                    : 2 ;  // 30:29     
	UINT32 debug_enable                       : 1 ;  //    31     
} PE_M14A0_CVA_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc001316cL cva_pix_sum_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_y                          : 12;  // 27:16     
} PE_M14A0_CVA_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0013170L cva_pix_sum_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                          : 32;  // 31: 0     
} PE_M14A0_CVA_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc0013174L cva_pix_sum_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                          : 32;  // 31: 0     
} PE_M14A0_CVA_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc0013178L cva_pix_sum_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                          : 32;  // 31: 0     
} PE_M14A0_CVA_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc001317cL cva_sync_fil_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_fil_en                       : 1 ;  //     0     
	UINT32 hsync_counter_en                   : 1 ;  //     1     
	UINT32 vsync_fil_en                       : 1 ;  //     2     
	UINT32 vsync_counter_en                   : 1 ;  //     3     
	UINT32 de_fil_en                          : 1 ;  //     4     
	UINT32 de_counter_en                      : 1 ;  //     5     
	UINT32 field_fil_en                       : 1 ;  //     6     
	UINT32 field_counter_en                   : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 line_count_sel                     : 12;  // 27:16     
} PE_M14A0_CVA_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013180L cva_sync_fil_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                      : 8 ;  //  7: 0     
	UINT32 vsync_counter                      : 8 ;  // 15: 8     
	UINT32 de_counter                         : 8 ;  // 23:16     
	UINT32 field_counter                      : 8 ;  // 31:24     
} PE_M14A0_CVA_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0013184L cva_cti_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 cti_enable                         : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 usm_mode                           : 1 ;  //     4     
	UINT32 pepeat_mode                        : 1 ;  //     5     
	UINT32                                    : 18;  // 23: 6     reserved
	UINT32 cti_gain                           : 8 ;  // 31:24     
} PE_M14A0_CVA_CTI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013190L cva_sync_repeat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_y_en                        : 1 ;  //     0     
	UINT32 repeat_cb_en                       : 1 ;  //     1     
	UINT32 repeat_cr_en                       : 1 ;  //     2     
	UINT32 repeat_pixel_odd                   : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 repeat_threshold_y                 : 6 ;  // 13: 8     
	UINT32 avg_threshold_y                    : 6 ;  // 19:14     
	UINT32 repeat_threshold_c                 : 6 ;  // 25:20     
	UINT32 avg_threshold_c                    : 6 ;  // 31:26     
} PE_M14A0_CVA_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013194L cva_sync_repeat_size0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                     : 21;  // 20: 0     
} PE_M14A0_CVA_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
		0xc0013198L cva_sync_repeat_size1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                        : 21;  // 20: 0     
} PE_M14A0_CVA_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
		0xc001319cL cva_sync_repeat_size2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                    : 21;  // 20: 0     
} PE_M14A0_CVA_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
		0xc00131a0L cva_sync_repeat_size3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_M14A0_CVA_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
		0xc00131a4L cva_sync_repeat_size4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                    : 21;  // 20: 0     
} PE_M14A0_CVA_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
		0xc00131a8L cva_sync_repeat_size5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_M14A0_CVA_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
		0xc00131acL cva_dig_filt_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient1                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient0                 : 10;  // 25:16     
} PE_M14A0_CVA_DIG_FILT_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc00131b0L cva_dig_filt_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient3                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient2                 : 10;  // 25:16     
} PE_M14A0_CVA_DIG_FILT_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc00131b4L cva_dig_filt_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient5                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient4                 : 10;  // 25:16     
} PE_M14A0_CVA_DIG_FILT_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc00131c0L cva_size_detect_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_vd_range                        : 2 ;  //  1: 0     
	UINT32 sd_ext_range                       : 2 ;  //  3: 2     
	UINT32 intr_mask_node                     : 1 ;  //     4     
	UINT32 intr_mask_nohsync                  : 1 ;  //     5     
	UINT32 intrl_mask_novsync                 : 1 ;  //     6     
	UINT32 intr_mask_abnormal                 : 1 ;  //     7     
	UINT32 intr_mask_invalid                  : 1 ;  //     8     
	UINT32 intr_mask_de                       : 1 ;  //     9     
	UINT32 intr_mask_hsync                    : 1 ;  //    10     
	UINT32 intr_mask_vsync                    : 1 ;  //    11     
	UINT32 intr_mask_normal                   : 1 ;  //    12     
	UINT32 intr_mask_valid                    : 1 ;  //    13     
	UINT32 intr_mask_h_cnahge                 : 1 ;  //    14     
	UINT32 sd_en                              : 1 ;  //    15     
	UINT32 sd_frame_cnt                       : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sd_novald_duration                 : 2 ;  // 21:20     
	UINT32                                    : 1 ;  //    22     reserved
	UINT32 sd_one_field                       : 1 ;  //    23     
	UINT32                                    : 2 ;  // 25:24     reserved
	UINT32 nosig_duration_de                  : 2 ;  // 27:26     
	UINT32 nosig_duration_hsync               : 2 ;  // 29:28     
	UINT32 nosig_duration_vsync               : 2 ;  // 31:30     
} PE_M14A0_CVA_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00131c4L cva_size_detect_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                      : 16;  // 15: 0     
	UINT32 line_width_read                    : 16;  // 31:16     
} PE_M14A0_CVA_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
		0xc00131c8L cva_size_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read           : 12;  // 11: 0     
	UINT32 de_signal_detect                   : 1 ;  //    12     
	UINT32 hsync_signal_detect                : 1 ;  //    13     
	UINT32 vsync_signal_detect                : 1 ;  //    14     
	UINT32 source_valid                       : 1 ;  //    15     
	UINT32 sd_line_num                        : 12;  // 27:16     
	UINT32 field_at_vactive_fall              : 4 ;  // 31:28     
} PE_M14A0_CVA_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc00131ccL cva_size_status5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                     : 24;  // 23: 0     
} PE_M14A0_CVA_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
		0xc00131d0L cva_size_status6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                       : 16;  // 15: 0     
	UINT32 diff_ext_vcnt                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 vd_intr                            : 1 ;  //    31     
} PE_M14A0_CVA_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
		0xc00131d4L cva_size_detect_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal0_en                       : 1 ;  //     0     
	UINT32 abnormal1_en                       : 1 ;  //     1     
	UINT32 abnormal2_en                       : 1 ;  //     2     
	UINT32 abnormal3_en                       : 1 ;  //     3     
	UINT32 abnormal4_en                       : 1 ;  //     4     
	UINT32 abnormal5_en                       : 1 ;  //     5     
	UINT32 abnormal6_en                       : 1 ;  //     6     
	UINT32 abnormal7_en                       : 1 ;  //     7     
	UINT32 abnormal0_flag                     : 1 ;  //     8     
	UINT32 abnormal1_flag                     : 1 ;  //     9     
	UINT32 abnormal2_flag                     : 1 ;  //    10     
	UINT32 abnormal3_flag                     : 1 ;  //    11     
	UINT32 abnormal4_flag                     : 1 ;  //    12     
	UINT32 abnormal5_flag                     : 1 ;  //    13     
	UINT32 abnormal6_flag                     : 1 ;  //    14     
	UINT32 abnormal7_flag                     : 1 ;  //    15     
	UINT32 ext_clk_valid_count                : 15;  // 30:16     
	UINT32 ext_clk_valid                      : 1 ;  //    31     
} PE_M14A0_CVA_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc00131d8L cva_size_detect_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                      : 8 ;  //  7: 0     
	UINT32 abnormal_out_h                     : 8 ;  // 15: 8     
	UINT32 abnormal_in_v                      : 8 ;  // 23:16     
	UINT32 abnormal_out_v                     : 8 ;  // 31:24     
} PE_M14A0_CVA_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc00131dcL cva_pseudo_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync_pesudo_sel                   : 1 ;  //     0     
	UINT32 hsync_pesudo_sel                   : 1 ;  //     1     
	UINT32 vactive_pesudo_sel                 : 1 ;  //     2     
	UINT32 hactive_pesudo_sel                 : 1 ;  //     3     
	UINT32 field_pesudo_sel                   : 1 ;  //     4     
	UINT32 y_pseudo_sel                       : 1 ;  //     5     
	UINT32 cb_psuedo_sel                      : 1 ;  //     6     
	UINT32 cr_psuedo_sel                      : 1 ;  //     7     
	UINT32 valid_pseudo_sel                   : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 auto_pseudo_sync_en                : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pseudo_sync_ctrl_en                : 1 ;  //    16     
	UINT32 pseudo_y_data_sel                  : 1 ;  //    17     
	UINT32 pseudo_cb_data_sel                 : 1 ;  //    18     
	UINT32 pseudo_cr_data_sel                 : 1 ;  //    19     
	UINT32 pseudo_y                           : 4 ;  // 23:20     
	UINT32 pseudo_cb                          : 4 ;  // 27:24     
	UINT32 pseudo_cr                          : 4 ;  // 31:28     
} PE_M14A0_CVA_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc00131e0L cva_pseudo_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                         : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hsync                       : 14;  // 29:16     
} PE_M14A0_CVA_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc00131e4L cva_pseudo_sync_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                        : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hn                          : 14;  // 29:16     
} PE_M14A0_CVA_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc00131e8L cva_pseudo_sync_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vsync                       : 12;  // 27:16     
} PE_M14A0_CVA_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc00131ecL cva_pseudo_sync_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vn                          : 12;  // 27:16     
} PE_M14A0_CVA_PSEUDO_SYNC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc00131f0L cva_size_detect_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 interrupt_mask_method              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 interrupt_mask_count               : 12;  // 15: 4     
	UINT32 diff_hsize                         : 14;  // 29:16     
} PE_M14A0_CVA_SIZE_DETECT_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0013200L cvb_top_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 scan_type                          : 1 ;  //     0     
	UINT32 field_gen_mode                     : 2 ;  //  2: 1     
	UINT32 auto_field_gen                     : 2 ;  //  4: 3     
	UINT32 data_bit_mode                      : 1 ;  //     5     
	UINT32 chroma_sampling                    : 1 ;  //     6     
	UINT32 aspect_ratio                       : 1 ;  //     7     
	UINT32 yc_delay_mode_r                    : 2 ;  //  9: 8     
	UINT32 yc_delay_mode_b                    : 2 ;  // 11:10     
	UINT32 yc_delay_mode_g                    : 2 ;  // 13:12     
	UINT32                                    : 5 ;  // 18:14     reserved
	UINT32 auto_sync_polarity                 : 1 ;  //    19     
	UINT32 manual_sync_polarity               : 2 ;  // 21:20     
	UINT32 adc_cut_direction                  : 1 ;  //    22     
	UINT32 adc_bit_sel                        : 1 ;  //    23     
	UINT32 br_port_swap                       : 1 ;  //    24     
	UINT32 r_port_swap                        : 1 ;  //    25     
	UINT32 b_port_swap                        : 1 ;  //    26     
	UINT32 g_port_swap                        : 1 ;  //    27     
	UINT32 bit_numbering_lsb2                 : 3 ;  // 30:28     
	UINT32 data_en_mode                       : 1 ;  //    31     
} PE_M14A0_CVB_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013204L cvb_offset_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 offset_v                           : 12;  // 27:16     
} PE_M14A0_CVB_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013208L cvb_size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 size_v                             : 12;  // 27:16     
} PE_M14A0_CVB_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc001320cL cvb_int_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 res_hfp                            : 10;  // 25:16     
} PE_M14A0_CVB_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013210L cvb_int_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 res_vsync                          : 5 ;  // 12: 8     
	UINT32 res_field_add_line                 : 2 ;  // 14:13     
	UINT32 res_field_sel                      : 1 ;  //    15     
	UINT32 res_sync_sel                       : 1 ;  //    16     
} PE_M14A0_CVB_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0013214L cvb_dig_filt_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient1                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient0                   : 10;  // 25:16     
} PE_M14A0_CVB_DIG_FILT_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013218L cvb_dig_filt_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient3                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient2                   : 10;  // 25:16     
} PE_M14A0_CVB_DIG_FILT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc001321cL cvb_dig_filt_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient5                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient4                   : 10;  // 25:16     
} PE_M14A0_CVB_DIG_FILT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc0013220L cvb_dig_filt_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 25;  // 24: 0     
	UINT32 fir_sample_mode                    : 1 ;  //    25     
	UINT32 fir_coeff_load                     : 1 ;  //    26     
	UINT32 fir_en                             : 1 ;  //    27     
	UINT32 fir_y_en                           : 1 ;  //    28     
	UINT32 fir_sampling_rate                  : 2 ;  // 30:29     
	UINT32 fir_c_en                           : 1 ;  //    31     
} PE_M14A0_CVB_DIG_FILT_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0013224L cvb_csc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient0                   : 15;  // 30:16     
} PE_M14A0_CVB_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013228L cvb_csc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient2                   : 15;  // 30:16     
} PE_M14A0_CVB_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc001322cL cvb_csc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient4                   : 15;  // 30:16     
} PE_M14A0_CVB_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc0013230L cvb_csc_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient6                   : 15;  // 30:16     
} PE_M14A0_CVB_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0013234L cvb_csc_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_coefficient8                   : 15;  // 30:16     
} PE_M14A0_CVB_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc0013238L cvb_csc_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset1                        : 11;  // 26:16     
} PE_M14A0_CVB_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc001323cL cvb_csc_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset3                        : 11;  // 26:16     
} PE_M14A0_CVB_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc0013240L cvb_csc_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                     : 1 ;  //     0     
	UINT32 csc_en                             : 1 ;  //     1     
	UINT32 csc_msb_4b_only_mode               : 1 ;  //     2     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 csc_offset5                        : 11;  // 26:16     
} PE_M14A0_CVB_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
		0xc0013244L cvb_misc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_type                       : 3 ;  //  2: 0     
	UINT32 pattern_csc                        : 1 ;  //     3     
	UINT32 pattern_detail                     : 4 ;  //  7: 4     
	UINT32 reg_update_pulse                   : 1 ;  //     8     
	UINT32 field_fix_mode                     : 2 ;  // 10: 9     
	UINT32 black_sel                          : 1 ;  //    11     
	UINT32 write_inhibit                      : 1 ;  //    12     
	UINT32 sim_mode                           : 1 ;  //    13     
	UINT32 black_off                          : 1 ;  //    14     
	UINT32 field_edge                         : 1 ;  //    15     
	UINT32 ha_gen                             : 1 ;  //    16     
	UINT32 xsize_divide                       : 2 ;  // 18:17     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 yc_delay_mode_r                    : 2 ;  // 21:20     
	UINT32 yc_delay_mode_b                    : 2 ;  // 23:22     
	UINT32 yc_delay_mode_g                    : 2 ;  // 25:24     
	UINT32 field_detect_vris                  : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_update_position                : 4 ;  // 31:28     
} PE_M14A0_CVB_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013248L cvb_offset_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 voffset_read                       : 13;  // 28:16     
} PE_M14A0_CVB_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
		0xc001324cL cvb_pre3d_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_sync_enable                  : 1 ;  //     0     
	UINT32 pre3d_sync_mode                    : 2 ;  //  2: 1     
	UINT32 pre3d_field_mode                   : 1 ;  //     3     
	UINT32 pre3d_frame_lr_mode                : 1 ;  //     4     
	UINT32 pre3d_mid_vsync_bypass             : 1 ;  //     5     
	UINT32 pre3d_field_bypass                 : 1 ;  //     6     
	UINT32 reserved                           : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 pre3d_active_line_num              : 12;  // 27:16     
} PE_M14A0_CVB_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0013250L cvb_pre3d_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_active_space2_line_num       : 12;  // 27:16     
} PE_M14A0_CVB_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0013254L cvb_pre3d_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_sync_line_num                : 12;  // 27:16     
} PE_M14A0_CVB_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc0013258L cvb_size_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_hoffset_indurance_read          : 8 ;  //  7: 0     
	UINT32 sd_voffset_indurance_read          : 8 ;  // 15: 8     
	UINT32 sd_hsync_fault_flag_read           : 1 ;  //    16     
	UINT32 sd_vsync_fault_flag_read           : 1 ;  //    17     
	UINT32 h_sync_polarity                    : 1 ;  //    18     
	UINT32 v_sync_polarity                    : 1 ;  //    19     
	UINT32 field_signal                       : 4 ;  // 23:20     
	UINT32 hsync_at_vsync_fall                : 4 ;  // 27:24     
	UINT32 outfield_at_vactive_fall           : 2 ;  // 29:28     
	UINT32 hoffset_over                       : 1 ;  //    30     
	UINT32 v_offset_over                      : 1 ;  //    31     
} PE_M14A0_CVB_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc001325cL cvb_size_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 de_line_count                      : 12;  // 27:16     
} PE_M14A0_CVB_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc0013260L cvb_size_status3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 hd_line_size                       : 12;  // 27:16     
} PE_M14A0_CVB_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
		0xc0013264L cvb_size_status4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vd_line_count                      : 12;  // 27:16     
} PE_M14A0_CVB_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
		0xc0013268L cvb_pix_sum_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_x                          : 12;  // 27:16     
	UINT32 pix_sum_enable                     : 1 ;  //    28     
	UINT32 field_selection                    : 2 ;  // 30:29     
	UINT32 debug_enable                       : 1 ;  //    31     
} PE_M14A0_CVB_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc001326cL cvb_pix_sum_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_y                          : 12;  // 27:16     
} PE_M14A0_CVB_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0013270L cvb_pix_sum_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                          : 32;  // 31: 0     
} PE_M14A0_CVB_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc0013274L cvb_pix_sum_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                          : 32;  // 31: 0     
} PE_M14A0_CVB_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc0013278L cvb_pix_sum_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                          : 32;  // 31: 0     
} PE_M14A0_CVB_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc001327cL cvb_sync_fil_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_fil_en                       : 1 ;  //     0     
	UINT32 hsync_counter_en                   : 1 ;  //     1     
	UINT32 vsync_fil_en                       : 1 ;  //     2     
	UINT32 vsync_counter_en                   : 1 ;  //     3     
	UINT32 de_fil_en                          : 1 ;  //     4     
	UINT32 de_counter_en                      : 1 ;  //     5     
	UINT32 field_fil_en                       : 1 ;  //     6     
	UINT32 field_counter_en                   : 1 ;  //     7     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 line_count_sel                     : 12;  // 27:16     
} PE_M14A0_CVB_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013280L cvb_sync_fil_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                      : 8 ;  //  7: 0     
	UINT32 vsync_counter                      : 8 ;  // 15: 8     
	UINT32 de_counter                         : 8 ;  // 23:16     
	UINT32 field_counter                      : 8 ;  // 31:24     
} PE_M14A0_CVB_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc0013284L cvb_cti_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 cti_enable                         : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 usm_mode                           : 1 ;  //     4     
	UINT32 pepeat_mode                        : 1 ;  //     5     
	UINT32                                    : 18;  // 23: 6     reserved
	UINT32 cti_gain                           : 8 ;  // 31:24     
} PE_M14A0_CVB_CTI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013290L cvb_sync_repeat_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_y_en                        : 1 ;  //     0     
	UINT32 repeat_cb_en                       : 1 ;  //     1     
	UINT32 repeat_cr_en                       : 1 ;  //     2     
	UINT32 repeat_pixel_odd                   : 1 ;  //     3     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 repeat_threshold_y                 : 6 ;  // 13: 8     
	UINT32 avg_threshold_y                    : 6 ;  // 19:14     
	UINT32 repeat_threshold_c                 : 6 ;  // 25:20     
	UINT32 avg_threshold_c                    : 6 ;  // 31:26     
} PE_M14A0_CVB_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0013294L cvb_sync_repeat_size0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                     : 21;  // 20: 0     
} PE_M14A0_CVB_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
		0xc0013298L cvb_sync_repeat_size1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                        : 21;  // 20: 0     
} PE_M14A0_CVB_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
		0xc001329cL cvb_sync_repeat_size2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                    : 21;  // 20: 0     
} PE_M14A0_CVB_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
		0xc00132a0L cvb_sync_repeat_size3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_M14A0_CVB_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
		0xc00132a4L cvb_sync_repeat_size4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                    : 21;  // 20: 0     
} PE_M14A0_CVB_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
		0xc00132a8L cvb_sync_repeat_size5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_M14A0_CVB_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
		0xc00132acL cvb_dig_filt_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient1                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient0                 : 10;  // 25:16     
} PE_M14A0_CVB_DIG_FILT_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc00132b0L cvb_dig_filt_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient3                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient2                 : 10;  // 25:16     
} PE_M14A0_CVB_DIG_FILT_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc00132b4L cvb_dig_filt_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient5                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient4                 : 10;  // 25:16     
} PE_M14A0_CVB_DIG_FILT_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc00132c0L cvb_size_detect_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_vd_range                        : 2 ;  //  1: 0     
	UINT32 sd_ext_range                       : 2 ;  //  3: 2     
	UINT32 intr_mask_node                     : 1 ;  //     4     
	UINT32 intr_mask_nohsync                  : 1 ;  //     5     
	UINT32 intrl_mask_novsync                 : 1 ;  //     6     
	UINT32 intr_mask_abnormal                 : 1 ;  //     7     
	UINT32 intr_mask_invalid                  : 1 ;  //     8     
	UINT32 intr_mask_de                       : 1 ;  //     9     
	UINT32 intr_mask_hsync                    : 1 ;  //    10     
	UINT32 intr_mask_vsync                    : 1 ;  //    11     
	UINT32 intr_mask_normal                   : 1 ;  //    12     
	UINT32 intr_mask_valid                    : 1 ;  //    13     
	UINT32 intr_mask_h_cnahge                 : 1 ;  //    14     
	UINT32 sd_en                              : 1 ;  //    15     
	UINT32 sd_frame_cnt                       : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sd_novald_duration                 : 2 ;  // 21:20     
	UINT32                                    : 1 ;  //    22     reserved
	UINT32 sd_one_field                       : 1 ;  //    23     
	UINT32                                    : 2 ;  // 25:24     reserved
	UINT32 nosig_duration_de                  : 2 ;  // 27:26     
	UINT32 nosig_duration_hsync               : 2 ;  // 29:28     
	UINT32 nosig_duration_vsync               : 2 ;  // 31:30     
} PE_M14A0_CVB_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc00132c4L cvb_size_detect_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                      : 16;  // 15: 0     
	UINT32 line_width_read                    : 16;  // 31:16     
} PE_M14A0_CVB_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
		0xc00132c8L cvb_size_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read           : 12;  // 11: 0     
	UINT32 de_signal_detect                   : 1 ;  //    12     
	UINT32 hsync_signal_detect                : 1 ;  //    13     
	UINT32 vsync_signal_detect                : 1 ;  //    14     
	UINT32 source_valid                       : 1 ;  //    15     
	UINT32 sd_line_num                        : 12;  // 27:16     
	UINT32 field_at_vactive_fall              : 4 ;  // 31:28     
} PE_M14A0_CVB_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc00132ccL cvb_size_status5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                     : 24;  // 23: 0     
} PE_M14A0_CVB_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
		0xc00132d0L cvb_size_status6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                       : 16;  // 15: 0     
	UINT32 diff_ext_vcnt                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 vd_intr                            : 1 ;  //    31     
} PE_M14A0_CVB_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
		0xc00132d4L cvb_size_detect_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal0_en                       : 1 ;  //     0     
	UINT32 abnormal1_en                       : 1 ;  //     1     
	UINT32 abnormal2_en                       : 1 ;  //     2     
	UINT32 abnormal3_en                       : 1 ;  //     3     
	UINT32 abnormal4_en                       : 1 ;  //     4     
	UINT32 abnormal5_en                       : 1 ;  //     5     
	UINT32 abnormal6_en                       : 1 ;  //     6     
	UINT32 abnormal7_en                       : 1 ;  //     7     
	UINT32 abnormal0_flag                     : 1 ;  //     8     
	UINT32 abnormal1_flag                     : 1 ;  //     9     
	UINT32 abnormal2_flag                     : 1 ;  //    10     
	UINT32 abnormal3_flag                     : 1 ;  //    11     
	UINT32 abnormal4_flag                     : 1 ;  //    12     
	UINT32 abnormal5_flag                     : 1 ;  //    13     
	UINT32 abnormal6_flag                     : 1 ;  //    14     
	UINT32 abnormal7_flag                     : 1 ;  //    15     
	UINT32 ext_clk_valid_count                : 15;  // 30:16     
	UINT32 ext_clk_valid                      : 1 ;  //    31     
} PE_M14A0_CVB_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc00132d8L cvb_size_detect_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                      : 8 ;  //  7: 0     
	UINT32 abnormal_out_h                     : 8 ;  // 15: 8     
	UINT32 abnormal_in_v                      : 8 ;  // 23:16     
	UINT32 abnormal_out_v                     : 8 ;  // 31:24     
} PE_M14A0_CVB_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc00132dcL cvb_pseudo_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync_pesudo_sel                   : 1 ;  //     0     
	UINT32 hsync_pesudo_sel                   : 1 ;  //     1     
	UINT32 vactive_pesudo_sel                 : 1 ;  //     2     
	UINT32 hactive_pesudo_sel                 : 1 ;  //     3     
	UINT32 field_pesudo_sel                   : 1 ;  //     4     
	UINT32 y_pseudo_sel                       : 1 ;  //     5     
	UINT32 cb_psuedo_sel                      : 1 ;  //     6     
	UINT32 cr_psuedo_sel                      : 1 ;  //     7     
	UINT32 valid_pseudo_sel                   : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 auto_pseudo_sync_en                : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pseudo_sync_ctrl_en                : 1 ;  //    16     
	UINT32 pseudo_y_data_sel                  : 1 ;  //    17     
	UINT32 pseudo_cb_data_sel                 : 1 ;  //    18     
	UINT32 pseudo_cr_data_sel                 : 1 ;  //    19     
	UINT32 pseudo_y                           : 4 ;  // 23:20     
	UINT32 pseudo_cb                          : 4 ;  // 27:24     
	UINT32 pseudo_cr                          : 4 ;  // 31:28     
} PE_M14A0_CVB_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc00132e0L cvb_pseudo_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                         : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hsync                       : 14;  // 29:16     
} PE_M14A0_CVB_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc00132e4L cvb_pseudo_sync_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                        : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hn                          : 14;  // 29:16     
} PE_M14A0_CVB_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc00132e8L cvb_pseudo_sync_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vsync                       : 12;  // 27:16     
} PE_M14A0_CVB_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc00132ecL cvb_pseudo_sync_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vn                          : 12;  // 27:16     
} PE_M14A0_CVB_PSEUDO_SYNC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc00132f0L cvb_size_detect_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 interrupt_mask_method              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 interrupt_mask_count               : 12;  // 15: 4     
	UINT32 diff_hsize                         : 14;  // 29:16     
} PE_M14A0_CVB_SIZE_DETECT_CTRL3_T;

typedef struct {
	PE_M14A0_CVA_TOP_CTRL_T            	cva_top_ctrl;                 //0xc0013100L
	PE_M14A0_CVA_OFFSET_CTRL_T         	cva_offset_ctrl;              //0xc0013104L
	PE_M14A0_CVA_SIZE_CTRL_T           	cva_size_ctrl;                //0xc0013108L
	PE_M14A0_CVA_INT_SYNC_CTRL0_T      	cva_int_sync_ctrl0;           //0xc001310cL
	PE_M14A0_CVA_INT_SYNC_CTRL1_T      	cva_int_sync_ctrl1;           //0xc0013110L
	PE_M14A0_CVA_DIG_FILT_CTRL0_T      	cva_dig_filt_ctrl0;           //0xc0013114L
	PE_M14A0_CVA_DIG_FILT_CTRL1_T      	cva_dig_filt_ctrl1;           //0xc0013118L
	PE_M14A0_CVA_DIG_FILT_CTRL2_T      	cva_dig_filt_ctrl2;           //0xc001311cL
	PE_M14A0_CVA_DIG_FILT_CTRL3_T      	cva_dig_filt_ctrl3;           //0xc0013120L
	PE_M14A0_CVA_CSC_CTRL0_T           	cva_csc_ctrl0;                //0xc0013124L
	PE_M14A0_CVA_CSC_CTRL1_T           	cva_csc_ctrl1;                //0xc0013128L
	PE_M14A0_CVA_CSC_CTRL2_T           	cva_csc_ctrl2;                //0xc001312cL
	PE_M14A0_CVA_CSC_CTRL3_T           	cva_csc_ctrl3;                //0xc0013130L
	PE_M14A0_CVA_CSC_CTRL4_T           	cva_csc_ctrl4;                //0xc0013134L
	PE_M14A0_CVA_CSC_CTRL5_T           	cva_csc_ctrl5;                //0xc0013138L
	PE_M14A0_CVA_CSC_CTRL6_T           	cva_csc_ctrl6;                //0xc001313cL
	PE_M14A0_CVA_CSC_CTRL7_T           	cva_csc_ctrl7;                //0xc0013140L
	PE_M14A0_CVA_MISC_CTRL_T           	cva_misc_ctrl;                //0xc0013144L
	PE_M14A0_CVA_OFFSET_READ_T         	cva_offset_read;              //0xc0013148L
	PE_M14A0_CVA_PRE3D_CTRL0_T         	cva_pre3d_ctrl0;              //0xc001314cL
	PE_M14A0_CVA_PRE3D_CTRL1_T         	cva_pre3d_ctrl1;              //0xc0013150L
	PE_M14A0_CVA_PRE3D_CTRL2_T         	cva_pre3d_ctrl2;              //0xc0013154L
	PE_M14A0_CVA_SIZE_STATUS1_T        	cva_size_status1;             //0xc0013158L
	PE_M14A0_CVA_SIZE_STATUS2_T        	cva_size_status2;             //0xc001315cL
	PE_M14A0_CVA_SIZE_STATUS3_T        	cva_size_status3;             //0xc0013160L
	PE_M14A0_CVA_SIZE_STATUS4_T        	cva_size_status4;             //0xc0013164L
	PE_M14A0_CVA_PIX_SUM_CTRL0_T       	cva_pix_sum_ctrl0;            //0xc0013168L
	PE_M14A0_CVA_PIX_SUM_CTRL1_T       	cva_pix_sum_ctrl1;            //0xc001316cL
	PE_M14A0_CVA_PIX_SUM_STATUS0_T     	cva_pix_sum_status0;          //0xc0013170L
	PE_M14A0_CVA_PIX_SUM_STATUS1_T     	cva_pix_sum_status1;          //0xc0013174L
	PE_M14A0_CVA_PIX_SUM_STATUS2_T     	cva_pix_sum_status2;          //0xc0013178L
	PE_M14A0_CVA_SYNC_FIL_CTRL_T       	cva_sync_fil_ctrl;            //0xc001317cL
	PE_M14A0_CVA_SYNC_FIL_STATUS_T     	cva_sync_fil_status;          //0xc0013180L
	PE_M14A0_CVA_CTI_CTRL_T            	cva_cti_ctrl;                 //0xc0013184L
	UINT32                             	reserved00[2];                //0xc0013188L~0xc001318cL
	PE_M14A0_CVA_SYNC_REPEAT_CTRL_T    	cva_sync_repeat_ctrl;         //0xc0013190L
	PE_M14A0_CVA_SYNC_REPEAT_SIZE0_T   	cva_sync_repeat_size0;        //0xc0013194L
	PE_M14A0_CVA_SYNC_REPEAT_SIZE1_T   	cva_sync_repeat_size1;        //0xc0013198L
	PE_M14A0_CVA_SYNC_REPEAT_SIZE2_T   	cva_sync_repeat_size2;        //0xc001319cL
	PE_M14A0_CVA_SYNC_REPEAT_SIZE3_T   	cva_sync_repeat_size3;        //0xc00131a0L
	PE_M14A0_CVA_SYNC_REPEAT_SIZE4_T   	cva_sync_repeat_size4;        //0xc00131a4L
	PE_M14A0_CVA_SYNC_REPEAT_SIZE5_T   	cva_sync_repeat_size5;        //0xc00131a8L
	PE_M14A0_CVA_DIG_FILT_CTRL4_T      	cva_dig_filt_ctrl4;           //0xc00131acL
	PE_M14A0_CVA_DIG_FILT_CTRL5_T      	cva_dig_filt_ctrl5;           //0xc00131b0L
	PE_M14A0_CVA_DIG_FILT_CTRL6_T      	cva_dig_filt_ctrl6;           //0xc00131b4L
	UINT32                             	reserved01[2];                //0xc00131b8L~0xc00131bcL
	PE_M14A0_CVA_SIZE_DETECT_CTRL_T    	cva_size_detect_ctrl;         //0xc00131c0L
	PE_M14A0_CVA_SIZE_DETECT_READ_T    	cva_size_detect_read;         //0xc00131c4L
	PE_M14A0_CVA_SIZE_STATUS0_T        	cva_size_status0;             //0xc00131c8L
	PE_M14A0_CVA_SIZE_STATUS5_T        	cva_size_status5;             //0xc00131ccL
	PE_M14A0_CVA_SIZE_STATUS6_T        	cva_size_status6;             //0xc00131d0L
	PE_M14A0_CVA_SIZE_DETECT_CTRL1_T   	cva_size_detect_ctrl1;        //0xc00131d4L
	PE_M14A0_CVA_SIZE_DETECT_CTRL2_T   	cva_size_detect_ctrl2;        //0xc00131d8L
	PE_M14A0_CVA_PSEUDO_SYNC_CTRL0_T   	cva_pseudo_sync_ctrl0;        //0xc00131dcL
	PE_M14A0_CVA_PSEUDO_SYNC_CTRL1_T   	cva_pseudo_sync_ctrl1;        //0xc00131e0L
	PE_M14A0_CVA_PSEUDO_SYNC_CTRL2_T   	cva_pseudo_sync_ctrl2;        //0xc00131e4L
	PE_M14A0_CVA_PSEUDO_SYNC_CTRL3_T   	cva_pseudo_sync_ctrl3;        //0xc00131e8L
	PE_M14A0_CVA_PSEUDO_SYNC_CTRL4_T   	cva_pseudo_sync_ctrl4;        //0xc00131ecL
	PE_M14A0_CVA_SIZE_DETECT_CTRL3_T   	cva_size_detect_ctrl3;        //0xc00131f0L
}PE_CVA_REG_M14A0_T;
typedef struct {
	PE_M14A0_CVB_TOP_CTRL_T            	cvb_top_ctrl;                 //0xc0013200L
	PE_M14A0_CVB_OFFSET_CTRL_T         	cvb_offset_ctrl;              //0xc0013204L
	PE_M14A0_CVB_SIZE_CTRL_T           	cvb_size_ctrl;                //0xc0013208L
	PE_M14A0_CVB_INT_SYNC_CTRL0_T      	cvb_int_sync_ctrl0;           //0xc001320cL
	PE_M14A0_CVB_INT_SYNC_CTRL1_T      	cvb_int_sync_ctrl1;           //0xc0013210L
	PE_M14A0_CVB_DIG_FILT_CTRL0_T      	cvb_dig_filt_ctrl0;           //0xc0013214L
	PE_M14A0_CVB_DIG_FILT_CTRL1_T      	cvb_dig_filt_ctrl1;           //0xc0013218L
	PE_M14A0_CVB_DIG_FILT_CTRL2_T      	cvb_dig_filt_ctrl2;           //0xc001321cL
	PE_M14A0_CVB_DIG_FILT_CTRL3_T      	cvb_dig_filt_ctrl3;           //0xc0013220L
	PE_M14A0_CVB_CSC_CTRL0_T           	cvb_csc_ctrl0;                //0xc0013224L
	PE_M14A0_CVB_CSC_CTRL1_T           	cvb_csc_ctrl1;                //0xc0013228L
	PE_M14A0_CVB_CSC_CTRL2_T           	cvb_csc_ctrl2;                //0xc001322cL
	PE_M14A0_CVB_CSC_CTRL3_T           	cvb_csc_ctrl3;                //0xc0013230L
	PE_M14A0_CVB_CSC_CTRL4_T           	cvb_csc_ctrl4;                //0xc0013234L
	PE_M14A0_CVB_CSC_CTRL5_T           	cvb_csc_ctrl5;                //0xc0013238L
	PE_M14A0_CVB_CSC_CTRL6_T           	cvb_csc_ctrl6;                //0xc001323cL
	PE_M14A0_CVB_CSC_CTRL7_T           	cvb_csc_ctrl7;                //0xc0013240L
	PE_M14A0_CVB_MISC_CTRL_T           	cvb_misc_ctrl;                //0xc0013244L
	PE_M14A0_CVB_OFFSET_READ_T         	cvb_offset_read;              //0xc0013248L
	PE_M14A0_CVB_PRE3D_CTRL0_T         	cvb_pre3d_ctrl0;              //0xc001324cL
	PE_M14A0_CVB_PRE3D_CTRL1_T         	cvb_pre3d_ctrl1;              //0xc0013250L
	PE_M14A0_CVB_PRE3D_CTRL2_T         	cvb_pre3d_ctrl2;              //0xc0013254L
	PE_M14A0_CVB_SIZE_STATUS1_T        	cvb_size_status1;             //0xc0013258L
	PE_M14A0_CVB_SIZE_STATUS2_T        	cvb_size_status2;             //0xc001325cL
	PE_M14A0_CVB_SIZE_STATUS3_T        	cvb_size_status3;             //0xc0013260L
	PE_M14A0_CVB_SIZE_STATUS4_T        	cvb_size_status4;             //0xc0013264L
	PE_M14A0_CVB_PIX_SUM_CTRL0_T       	cvb_pix_sum_ctrl0;            //0xc0013268L
	PE_M14A0_CVB_PIX_SUM_CTRL1_T       	cvb_pix_sum_ctrl1;            //0xc001326cL
	PE_M14A0_CVB_PIX_SUM_STATUS0_T     	cvb_pix_sum_status0;          //0xc0013270L
	PE_M14A0_CVB_PIX_SUM_STATUS1_T     	cvb_pix_sum_status1;          //0xc0013274L
	PE_M14A0_CVB_PIX_SUM_STATUS2_T     	cvb_pix_sum_status2;          //0xc0013278L
	PE_M14A0_CVB_SYNC_FIL_CTRL_T       	cvb_sync_fil_ctrl;            //0xc001327cL
	PE_M14A0_CVB_SYNC_FIL_STATUS_T     	cvb_sync_fil_status;          //0xc0013280L
	PE_M14A0_CVB_CTI_CTRL_T            	cvb_cti_ctrl;                 //0xc0013284L
	UINT32                             	reserved00[2];                //0xc0013288L~0xc001328cL
	PE_M14A0_CVB_SYNC_REPEAT_CTRL_T    	cvb_sync_repeat_ctrl;         //0xc0013290L
	PE_M14A0_CVB_SYNC_REPEAT_SIZE0_T   	cvb_sync_repeat_size0;        //0xc0013294L
	PE_M14A0_CVB_SYNC_REPEAT_SIZE1_T   	cvb_sync_repeat_size1;        //0xc0013298L
	PE_M14A0_CVB_SYNC_REPEAT_SIZE2_T   	cvb_sync_repeat_size2;        //0xc001329cL
	PE_M14A0_CVB_SYNC_REPEAT_SIZE3_T   	cvb_sync_repeat_size3;        //0xc00132a0L
	PE_M14A0_CVB_SYNC_REPEAT_SIZE4_T   	cvb_sync_repeat_size4;        //0xc00132a4L
	PE_M14A0_CVB_SYNC_REPEAT_SIZE5_T   	cvb_sync_repeat_size5;        //0xc00132a8L
	PE_M14A0_CVB_DIG_FILT_CTRL4_T      	cvb_dig_filt_ctrl4;           //0xc00132acL
	PE_M14A0_CVB_DIG_FILT_CTRL5_T      	cvb_dig_filt_ctrl5;           //0xc00132b0L
	PE_M14A0_CVB_DIG_FILT_CTRL6_T      	cvb_dig_filt_ctrl6;           //0xc00132b4L
	UINT32                             	reserved01[2];                //0xc00132b8L~0xc00132bcL
	PE_M14A0_CVB_SIZE_DETECT_CTRL_T    	cvb_size_detect_ctrl;         //0xc00132c0L
	PE_M14A0_CVB_SIZE_DETECT_READ_T    	cvb_size_detect_read;         //0xc00132c4L
	PE_M14A0_CVB_SIZE_STATUS0_T        	cvb_size_status0;             //0xc00132c8L
	PE_M14A0_CVB_SIZE_STATUS5_T        	cvb_size_status5;             //0xc00132ccL
	PE_M14A0_CVB_SIZE_STATUS6_T        	cvb_size_status6;             //0xc00132d0L
	PE_M14A0_CVB_SIZE_DETECT_CTRL1_T   	cvb_size_detect_ctrl1;        //0xc00132d4L
	PE_M14A0_CVB_SIZE_DETECT_CTRL2_T   	cvb_size_detect_ctrl2;        //0xc00132d8L
	PE_M14A0_CVB_PSEUDO_SYNC_CTRL0_T   	cvb_pseudo_sync_ctrl0;        //0xc00132dcL
	PE_M14A0_CVB_PSEUDO_SYNC_CTRL1_T   	cvb_pseudo_sync_ctrl1;        //0xc00132e0L
	PE_M14A0_CVB_PSEUDO_SYNC_CTRL2_T   	cvb_pseudo_sync_ctrl2;        //0xc00132e4L
	PE_M14A0_CVB_PSEUDO_SYNC_CTRL3_T   	cvb_pseudo_sync_ctrl3;        //0xc00132e8L
	PE_M14A0_CVB_PSEUDO_SYNC_CTRL4_T   	cvb_pseudo_sync_ctrl4;        //0xc00132ecL
	PE_M14A0_CVB_SIZE_DETECT_CTRL3_T   	cvb_size_detect_ctrl3;        //0xc00132f0L
}PE_CVB_REG_M14A0_T;
#endif