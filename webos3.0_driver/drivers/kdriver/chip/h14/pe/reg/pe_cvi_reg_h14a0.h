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

#ifndef _PE_CVI_REG_H14A0_H_
#define _PE_CVI_REG_H14A0_H_
/*-----------------------------------------------------------------------------
		0x100 cva_top_ctrl ''
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
} PE_H14A0_CVA_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
		0x104 cva_offset_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 offset_v                           : 12;  // 27:16     
} PE_H14A0_CVA_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
		0x108 cva_size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 size_v                             : 12;  // 27:16     
} PE_H14A0_CVA_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0x10c cva_int_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 res_hfp                            : 10;  // 25:16     
} PE_H14A0_CVA_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x110 cva_int_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 res_vsync                          : 5 ;  // 12: 8     
	UINT32 res_field_add_line                 : 2 ;  // 14:13     
	UINT32 res_field_sel                      : 1 ;  //    15     
	UINT32 res_sync_sel                       : 1 ;  //    16     
} PE_H14A0_CVA_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x114 cva_dig_filt_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient1                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient0                   : 10;  // 25:16     
} PE_H14A0_CVA_DIG_FILT_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x118 cva_dig_filt_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient3                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient2                   : 10;  // 25:16     
} PE_H14A0_CVA_DIG_FILT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x11c cva_dig_filt_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient5                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient4                   : 10;  // 25:16     
} PE_H14A0_CVA_DIG_FILT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x120 cva_dig_filt_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 24;  // 23: 0     
	UINT32 fir_boundary                       : 1 ;  //    24     
	UINT32 fir_sample_mode                    : 1 ;  //    25     
	UINT32 fir_coeff_load                     : 1 ;  //    26     
	UINT32 fir_en                             : 1 ;  //    27     
	UINT32 fir_y_en                           : 1 ;  //    28     
	UINT32 fir_sampling_rate                  : 2 ;  // 30:29     
	UINT32 fir_c_en                           : 1 ;  //    31     
} PE_H14A0_CVA_DIG_FILT_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x124 cva_csc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient0                   : 15;  // 30:16     
} PE_H14A0_CVA_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x128 cva_csc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient2                   : 15;  // 30:16     
} PE_H14A0_CVA_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x12c cva_csc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient4                   : 15;  // 30:16     
} PE_H14A0_CVA_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x130 cva_csc_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient6                   : 15;  // 30:16     
} PE_H14A0_CVA_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x134 cva_csc_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_coefficient8                   : 15;  // 30:16     
} PE_H14A0_CVA_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x138 cva_csc_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset1                        : 11;  // 26:16     
} PE_H14A0_CVA_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
		0x13c cva_csc_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset3                        : 11;  // 26:16     
} PE_H14A0_CVA_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
		0x140 cva_csc_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                     : 1 ;  //     0     
	UINT32 csc_en                             : 1 ;  //     1     
	UINT32 csc_msb_4b_only_mode               : 1 ;  //     2     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 csc_offset5                        : 11;  // 26:16     
} PE_H14A0_CVA_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
		0x144 cva_misc_ctrl ''
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
} PE_H14A0_CVA_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
		0x148 cva_offset_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 voffset_read                       : 13;  // 28:16     
} PE_H14A0_CVA_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
		0x14c cva_pre3d_ctrl0 ''
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
} PE_H14A0_CVA_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x150 cva_pre3d_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_active_space2_line_num       : 12;  // 27:16     
} PE_H14A0_CVA_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x154 cva_pre3d_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_sync_line_num                : 12;  // 27:16     
} PE_H14A0_CVA_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x158 cva_size_status1 ''
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
} PE_H14A0_CVA_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
		0x15c cva_size_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 de_line_count                      : 12;  // 27:16     
} PE_H14A0_CVA_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
		0x160 cva_size_status3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 hd_line_size                       : 12;  // 27:16     
} PE_H14A0_CVA_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
		0x164 cva_size_status4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vd_line_count                      : 12;  // 27:16     
} PE_H14A0_CVA_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
		0x168 cva_pix_sum_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_x                          : 12;  // 27:16     
	UINT32 pix_sum_enable                     : 1 ;  //    28     
	UINT32 field_selection                    : 2 ;  // 30:29     
	UINT32 debug_enable                       : 1 ;  //    31     
} PE_H14A0_CVA_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x16c cva_pix_sum_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_y                          : 12;  // 27:16     
} PE_H14A0_CVA_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x170 cva_pix_sum_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                          : 32;  // 31: 0     
} PE_H14A0_CVA_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
		0x174 cva_pix_sum_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                          : 32;  // 31: 0     
} PE_H14A0_CVA_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
		0x178 cva_pix_sum_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                          : 32;  // 31: 0     
} PE_H14A0_CVA_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
		0x17c cva_sync_fil_ctrl ''
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
} PE_H14A0_CVA_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0x180 cva_sync_fil_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                      : 8 ;  //  7: 0     
	UINT32 vsync_counter                      : 8 ;  // 15: 8     
	UINT32 de_counter                         : 8 ;  // 23:16     
	UINT32 field_counter                      : 8 ;  // 31:24     
} PE_H14A0_CVA_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
		0x184 cva_cti_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 cti_enable                         : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 usm_mode                           : 1 ;  //     4     
	UINT32 pepeat_mode                        : 1 ;  //     5     
	UINT32                                    : 18;  // 23: 6     reserved
	UINT32 cti_gain                           : 8 ;  // 31:24     
} PE_H14A0_CVA_CTI_CTRL_T;

/*-----------------------------------------------------------------------------
		0x190 cva_sync_repeat_ctrl ''
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
} PE_H14A0_CVA_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0x194 cva_sync_repeat_size0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                     : 21;  // 20: 0     
} PE_H14A0_CVA_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
		0x198 cva_sync_repeat_size1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                        : 21;  // 20: 0     
} PE_H14A0_CVA_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
		0x19c cva_sync_repeat_size2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                    : 21;  // 20: 0     
} PE_H14A0_CVA_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
		0x1a0 cva_sync_repeat_size3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_H14A0_CVA_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
		0x1a4 cva_sync_repeat_size4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                    : 21;  // 20: 0     
} PE_H14A0_CVA_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
		0x1a8 cva_sync_repeat_size5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_H14A0_CVA_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
		0x1ac cva_dig_filt_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient1                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient0                 : 10;  // 25:16     
} PE_H14A0_CVA_DIG_FILT_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x1b0 cva_dig_filt_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient3                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient2                 : 10;  // 25:16     
} PE_H14A0_CVA_DIG_FILT_CTRL5_T;

/*-----------------------------------------------------------------------------
		0x1b4 cva_dig_filt_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient5                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient4                 : 10;  // 25:16     
} PE_H14A0_CVA_DIG_FILT_CTRL6_T;

/*-----------------------------------------------------------------------------
		0x1c0 cva_size_detect_ctrl ''
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
} PE_H14A0_CVA_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
		0x1c4 cva_size_detect_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                      : 16;  // 15: 0     
	UINT32 line_width_read                    : 16;  // 31:16     
} PE_H14A0_CVA_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
		0x1c8 cva_size_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read           : 12;  // 11: 0     
	UINT32 de_signal_detect                   : 1 ;  //    12     
	UINT32 hsync_signal_detect                : 1 ;  //    13     
	UINT32 vsync_signal_detect                : 1 ;  //    14     
	UINT32 source_valid                       : 1 ;  //    15     
	UINT32 sd_line_num                        : 12;  // 27:16     
	UINT32 field_at_vactive_fall              : 4 ;  // 31:28     
} PE_H14A0_CVA_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
		0x1cc cva_size_status5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                     : 24;  // 23: 0     
} PE_H14A0_CVA_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
		0x1d0 cva_size_status6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                       : 16;  // 15: 0     
	UINT32 diff_ext_vcnt                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 vd_intr                            : 1 ;  //    31     
} PE_H14A0_CVA_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
		0x1d4 cva_size_detect_ctrl1 ''
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
} PE_H14A0_CVA_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x1d8 cva_size_detect_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                      : 8 ;  //  7: 0     
	UINT32 abnormal_out_h                     : 8 ;  // 15: 8     
	UINT32 abnormal_in_v                      : 8 ;  // 23:16     
	UINT32 abnormal_out_v                     : 8 ;  // 31:24     
} PE_H14A0_CVA_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x1dc cva_pseudo_sync_ctrl0 ''
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
} PE_H14A0_CVA_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x1e0 cva_pseudo_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                         : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hsync                       : 14;  // 29:16     
} PE_H14A0_CVA_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x1e4 cva_pseudo_sync_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                        : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hn                          : 14;  // 29:16     
} PE_H14A0_CVA_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x1e8 cva_pseudo_sync_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vsync                       : 12;  // 27:16     
} PE_H14A0_CVA_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x1ec cva_pseudo_sync_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vn                          : 12;  // 27:16     
} PE_H14A0_CVA_PSEUDO_SYNC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x1f0 cva_size_detect_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 interrupt_mask_method              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 interrupt_mask_count               : 12;  // 15: 4     
	UINT32 diff_hsize                         : 14;  // 29:16     
} PE_H14A0_CVA_SIZE_DETECT_CTRL3_T;
/*-----------------------------------------------------------------------------
		0x200 cvb_top_ctrl ''
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
} PE_H14A0_CVB_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
		0x204 cvb_offset_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                           : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 offset_v                           : 12;  // 27:16     
} PE_H14A0_CVB_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
		0x208 cvb_size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 size_v                             : 12;  // 27:16     
} PE_H14A0_CVB_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0x20c cvb_int_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 res_hfp                            : 10;  // 25:16     
} PE_H14A0_CVB_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x210 cvb_int_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 res_vsync                          : 5 ;  // 12: 8     
	UINT32 res_field_add_line                 : 2 ;  // 14:13     
	UINT32 res_field_sel                      : 1 ;  //    15     
	UINT32 res_sync_sel                       : 1 ;  //    16     
} PE_H14A0_CVB_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x214 cvb_dig_filt_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient1                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient0                   : 10;  // 25:16     
} PE_H14A0_CVB_DIG_FILT_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x218 cvb_dig_filt_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient3                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient2                   : 10;  // 25:16     
} PE_H14A0_CVB_DIG_FILT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x21c cvb_dig_filt_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient5                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_coefficient4                   : 10;  // 25:16     
} PE_H14A0_CVB_DIG_FILT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x220 cvb_dig_filt_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 24;  // 23: 0     
	UINT32 fir_boundary                       : 1 ;  //    24     
	UINT32 fir_sample_mode                    : 1 ;  //    25     
	UINT32 fir_coeff_load                     : 1 ;  //    26     
	UINT32 fir_en                             : 1 ;  //    27     
	UINT32 fir_y_en                           : 1 ;  //    28     
	UINT32 fir_sampling_rate                  : 2 ;  // 30:29     
	UINT32 fir_c_en                           : 1 ;  //    31     
} PE_H14A0_CVB_DIG_FILT_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x224 cvb_csc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient0                   : 15;  // 30:16     
} PE_H14A0_CVB_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x228 cvb_csc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient2                   : 15;  // 30:16     
} PE_H14A0_CVB_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x22c cvb_csc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient4                   : 15;  // 30:16     
} PE_H14A0_CVB_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x230 cvb_csc_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient6                   : 15;  // 30:16     
} PE_H14A0_CVB_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x234 cvb_csc_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_coefficient8                   : 15;  // 30:16     
} PE_H14A0_CVB_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x238 cvb_csc_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset1                        : 11;  // 26:16     
} PE_H14A0_CVB_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
		0x23c cvb_csc_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset3                        : 11;  // 26:16     
} PE_H14A0_CVB_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
		0x240 cvb_csc_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                     : 1 ;  //     0     
	UINT32 csc_en                             : 1 ;  //     1     
	UINT32 csc_msb_4b_only_mode               : 1 ;  //     2     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 csc_offset5                        : 11;  // 26:16     
} PE_H14A0_CVB_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
		0x244 cvb_misc_ctrl ''
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
} PE_H14A0_CVB_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
		0x248 cvb_offset_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 voffset_read                       : 13;  // 28:16     
} PE_H14A0_CVB_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
		0x24c cvb_pre3d_ctrl0 ''
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
} PE_H14A0_CVB_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x250 cvb_pre3d_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_active_space2_line_num       : 12;  // 27:16     
} PE_H14A0_CVB_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x254 cvb_pre3d_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pre3d_sync_line_num                : 12;  // 27:16     
} PE_H14A0_CVB_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x258 cvb_size_status1 ''
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
} PE_H14A0_CVB_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
		0x25c cvb_size_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 de_line_count                      : 12;  // 27:16     
} PE_H14A0_CVB_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
		0x260 cvb_size_status3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 hd_line_size                       : 12;  // 27:16     
} PE_H14A0_CVB_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
		0x264 cvb_size_status4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vd_line_count                      : 12;  // 27:16     
} PE_H14A0_CVB_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
		0x268 cvb_pix_sum_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_x                          : 12;  // 27:16     
	UINT32 pix_sum_enable                     : 1 ;  //    28     
	UINT32 field_selection                    : 2 ;  // 30:29     
	UINT32 debug_enable                       : 1 ;  //    31     
} PE_H14A0_CVB_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x26c cvb_pix_sum_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_y                          : 12;  // 27:16     
} PE_H14A0_CVB_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x270 cvb_pix_sum_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                          : 32;  // 31: 0     
} PE_H14A0_CVB_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
		0x274 cvb_pix_sum_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                          : 32;  // 31: 0     
} PE_H14A0_CVB_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
		0x278 cvb_pix_sum_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                          : 32;  // 31: 0     
} PE_H14A0_CVB_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
		0x27c cvb_sync_fil_ctrl ''
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
} PE_H14A0_CVB_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0x280 cvb_sync_fil_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                      : 8 ;  //  7: 0     
	UINT32 vsync_counter                      : 8 ;  // 15: 8     
	UINT32 de_counter                         : 8 ;  // 23:16     
	UINT32 field_counter                      : 8 ;  // 31:24     
} PE_H14A0_CVB_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
		0x284 cvb_cti_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 2 ;  //  1: 0     reserved
	UINT32 cti_enable                         : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 usm_mode                           : 1 ;  //     4     
	UINT32 pepeat_mode                        : 1 ;  //     5     
	UINT32                                    : 18;  // 23: 6     reserved
	UINT32 cti_gain                           : 8 ;  // 31:24     
} PE_H14A0_CVB_CTI_CTRL_T;

/*-----------------------------------------------------------------------------
		0x290 cvb_sync_repeat_ctrl ''
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
} PE_H14A0_CVB_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0x294 cvb_sync_repeat_size0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                     : 21;  // 20: 0     
} PE_H14A0_CVB_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
		0x298 cvb_sync_repeat_size1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                        : 21;  // 20: 0     
} PE_H14A0_CVB_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
		0x29c cvb_sync_repeat_size2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                    : 21;  // 20: 0     
} PE_H14A0_CVB_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
		0x2a0 cvb_sync_repeat_size3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_H14A0_CVB_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
		0x2a4 cvb_sync_repeat_size4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                    : 21;  // 20: 0     
} PE_H14A0_CVB_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
		0x2a8 cvb_sync_repeat_size5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} PE_H14A0_CVB_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
		0x2ac cvb_dig_filt_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient1                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient0                 : 10;  // 25:16     
} PE_H14A0_CVB_DIG_FILT_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x2b0 cvb_dig_filt_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient3                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient2                 : 10;  // 25:16     
} PE_H14A0_CVB_DIG_FILT_CTRL5_T;

/*-----------------------------------------------------------------------------
		0x2b4 cvb_dig_filt_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient5                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fir_c_coefficient4                 : 10;  // 25:16     
} PE_H14A0_CVB_DIG_FILT_CTRL6_T;

/*-----------------------------------------------------------------------------
		0x2c0 cvb_size_detect_ctrl ''
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
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 sd_en                              : 1 ;  //    15     
	UINT32 sd_frame_cnt                       : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 sd_novald_duration                 : 2 ;  // 21:20     
	UINT32                                    : 1 ;  //    22     reserved
	UINT32 sd_one_field                       : 1 ;  //    23     
	UINT32 intr_mask_ext_diff                 : 1 ;  //    24     
	UINT32 intr_mask_vd_diff                  : 1 ;  //    25     
	UINT32 nosig_duration_de                  : 2 ;  // 27:26     
	UINT32 nosig_duration_hsync               : 2 ;  // 29:28     
	UINT32 nosig_duration_vsync               : 2 ;  // 31:30     
} PE_H14A0_CVB_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
		0x2c4 cvb_size_detect_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                      : 16;  // 15: 0     
	UINT32 line_width_read                    : 16;  // 31:16     
} PE_H14A0_CVB_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
		0x2c8 cvb_size_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read           : 12;  // 11: 0     
	UINT32 de_signal_detect                   : 1 ;  //    12     
	UINT32 hsync_signal_detect                : 1 ;  //    13     
	UINT32 vsync_signal_detect                : 1 ;  //    14     
	UINT32 source_valid                       : 1 ;  //    15     
	UINT32 sd_line_num                        : 12;  // 27:16     
	UINT32 field_at_vactive_fall              : 4 ;  // 31:28     
} PE_H14A0_CVB_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
		0x2cc cvb_size_status5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                     : 24;  // 23: 0     
} PE_H14A0_CVB_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
		0x2d0 cvb_size_status6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                       : 16;  // 15: 0     
	UINT32 diff_ext_vcnt                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 vd_intr                            : 1 ;  //    31     
} PE_H14A0_CVB_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
		0x2d4 cvb_size_detect_ctrl1 ''
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
} PE_H14A0_CVB_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x2d8 cvb_size_detect_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                      : 8 ;  //  7: 0     
	UINT32 abnormal_out_h                     : 8 ;  // 15: 8     
	UINT32 abnormal_in_v                      : 8 ;  // 23:16     
	UINT32 abnormal_out_v                     : 8 ;  // 31:24     
} PE_H14A0_CVB_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x2dc cvb_pseudo_sync_ctrl0 ''
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
} PE_H14A0_CVB_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0x2e0 cvb_pseudo_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                         : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hsync                       : 14;  // 29:16     
} PE_H14A0_CVB_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0x2e4 cvb_pseudo_sync_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                        : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hn                          : 14;  // 29:16     
} PE_H14A0_CVB_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0x2e8 cvb_pseudo_sync_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vsync                       : 12;  // 27:16     
} PE_H14A0_CVB_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0x2ec cvb_pseudo_sync_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vn                          : 12;  // 27:16     
} PE_H14A0_CVB_PSEUDO_SYNC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0x2f0 cvb_size_detect_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 interrupt_mask_method              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 interrupt_mask_count               : 12;  // 15: 4     
	UINT32 diff_hsize                         : 14;  // 29:16     
} PE_H14A0_CVB_SIZE_DETECT_CTRL3_T;

typedef struct {
	PE_H14A0_CVA_TOP_CTRL_T               	cva_top_ctrl;                 //0x100
	PE_H14A0_CVA_OFFSET_CTRL_T            	cva_offset_ctrl;              //0x104
	PE_H14A0_CVA_SIZE_CTRL_T              	cva_size_ctrl;                //0x108
	PE_H14A0_CVA_INT_SYNC_CTRL0_T         	cva_int_sync_ctrl0;           //0x10c
	PE_H14A0_CVA_INT_SYNC_CTRL1_T         	cva_int_sync_ctrl1;           //0x110
	PE_H14A0_CVA_DIG_FILT_CTRL0_T         	cva_dig_filt_ctrl0;           //0x114
	PE_H14A0_CVA_DIG_FILT_CTRL1_T         	cva_dig_filt_ctrl1;           //0x118
	PE_H14A0_CVA_DIG_FILT_CTRL2_T         	cva_dig_filt_ctrl2;           //0x11c
	PE_H14A0_CVA_DIG_FILT_CTRL3_T         	cva_dig_filt_ctrl3;           //0x120
	PE_H14A0_CVA_CSC_CTRL0_T              	cva_csc_ctrl0;                //0x124
	PE_H14A0_CVA_CSC_CTRL1_T              	cva_csc_ctrl1;                //0x128
	PE_H14A0_CVA_CSC_CTRL2_T              	cva_csc_ctrl2;                //0x12c
	PE_H14A0_CVA_CSC_CTRL3_T              	cva_csc_ctrl3;                //0x130
	PE_H14A0_CVA_CSC_CTRL4_T              	cva_csc_ctrl4;                //0x134
	PE_H14A0_CVA_CSC_CTRL5_T              	cva_csc_ctrl5;                //0x138
	PE_H14A0_CVA_CSC_CTRL6_T              	cva_csc_ctrl6;                //0x13c
	PE_H14A0_CVA_CSC_CTRL7_T              	cva_csc_ctrl7;                //0x140
	PE_H14A0_CVA_MISC_CTRL_T              	cva_misc_ctrl;                //0x144
	PE_H14A0_CVA_OFFSET_READ_T            	cva_offset_read;              //0x148
	PE_H14A0_CVA_PRE3D_CTRL0_T            	cva_pre3d_ctrl0;              //0x14c
	PE_H14A0_CVA_PRE3D_CTRL1_T            	cva_pre3d_ctrl1;              //0x150
	PE_H14A0_CVA_PRE3D_CTRL2_T            	cva_pre3d_ctrl2;              //0x154
	PE_H14A0_CVA_SIZE_STATUS1_T           	cva_size_status1;             //0x158
	PE_H14A0_CVA_SIZE_STATUS2_T           	cva_size_status2;             //0x15c
	PE_H14A0_CVA_SIZE_STATUS3_T           	cva_size_status3;             //0x160
	PE_H14A0_CVA_SIZE_STATUS4_T           	cva_size_status4;             //0x164
	PE_H14A0_CVA_PIX_SUM_CTRL0_T          	cva_pix_sum_ctrl0;            //0x168
	PE_H14A0_CVA_PIX_SUM_CTRL1_T          	cva_pix_sum_ctrl1;            //0x16c
	PE_H14A0_CVA_PIX_SUM_STATUS0_T        	cva_pix_sum_status0;          //0x170
	PE_H14A0_CVA_PIX_SUM_STATUS1_T        	cva_pix_sum_status1;          //0x174
	PE_H14A0_CVA_PIX_SUM_STATUS2_T        	cva_pix_sum_status2;          //0x178
	PE_H14A0_CVA_SYNC_FIL_CTRL_T          	cva_sync_fil_ctrl;            //0x17c
	PE_H14A0_CVA_SYNC_FIL_STATUS_T        	cva_sync_fil_status;          //0x180
	PE_H14A0_CVA_CTI_CTRL_T               	cva_cti_ctrl;                 //0x184
	UINT32                              	reserved00[2];                //0x188~0x18c
	PE_H14A0_CVA_SYNC_REPEAT_CTRL_T       	cva_sync_repeat_ctrl;         //0x190
	PE_H14A0_CVA_SYNC_REPEAT_SIZE0_T      	cva_sync_repeat_size0;        //0x194
	PE_H14A0_CVA_SYNC_REPEAT_SIZE1_T      	cva_sync_repeat_size1;        //0x198
	PE_H14A0_CVA_SYNC_REPEAT_SIZE2_T      	cva_sync_repeat_size2;        //0x19c
	PE_H14A0_CVA_SYNC_REPEAT_SIZE3_T      	cva_sync_repeat_size3;        //0x1a0
	PE_H14A0_CVA_SYNC_REPEAT_SIZE4_T      	cva_sync_repeat_size4;        //0x1a4
	PE_H14A0_CVA_SYNC_REPEAT_SIZE5_T      	cva_sync_repeat_size5;        //0x1a8
	PE_H14A0_CVA_DIG_FILT_CTRL4_T         	cva_dig_filt_ctrl4;           //0x1ac
	PE_H14A0_CVA_DIG_FILT_CTRL5_T         	cva_dig_filt_ctrl5;           //0x1b0
	PE_H14A0_CVA_DIG_FILT_CTRL6_T         	cva_dig_filt_ctrl6;           //0x1b4
	UINT32                              	reserved01[2];                //0x1b8~0x1bc
	PE_H14A0_CVA_SIZE_DETECT_CTRL_T       	cva_size_detect_ctrl;         //0x1c0
	PE_H14A0_CVA_SIZE_DETECT_READ_T       	cva_size_detect_read;         //0x1c4
	PE_H14A0_CVA_SIZE_STATUS0_T           	cva_size_status0;             //0x1c8
	PE_H14A0_CVA_SIZE_STATUS5_T           	cva_size_status5;             //0x1cc
	PE_H14A0_CVA_SIZE_STATUS6_T           	cva_size_status6;             //0x1d0
	PE_H14A0_CVA_SIZE_DETECT_CTRL1_T      	cva_size_detect_ctrl1;        //0x1d4
	PE_H14A0_CVA_SIZE_DETECT_CTRL2_T      	cva_size_detect_ctrl2;        //0x1d8
	PE_H14A0_CVA_PSEUDO_SYNC_CTRL0_T      	cva_pseudo_sync_ctrl0;        //0x1dc
	PE_H14A0_CVA_PSEUDO_SYNC_CTRL1_T      	cva_pseudo_sync_ctrl1;        //0x1e0
	PE_H14A0_CVA_PSEUDO_SYNC_CTRL2_T      	cva_pseudo_sync_ctrl2;        //0x1e4
	PE_H14A0_CVA_PSEUDO_SYNC_CTRL3_T      	cva_pseudo_sync_ctrl3;        //0x1e8
	PE_H14A0_CVA_PSEUDO_SYNC_CTRL4_T      	cva_pseudo_sync_ctrl4;        //0x1ec
	PE_H14A0_CVA_SIZE_DETECT_CTRL3_T      	cva_size_detect_ctrl3;        //0x1f0
}PE_CVA_REG_H14A0_T;
typedef struct {
	PE_H14A0_CVB_TOP_CTRL_T               	cvb_top_ctrl;                 //0x200
	PE_H14A0_CVB_OFFSET_CTRL_T            	cvb_offset_ctrl;              //0x204
	PE_H14A0_CVB_SIZE_CTRL_T              	cvb_size_ctrl;                //0x208
	PE_H14A0_CVB_INT_SYNC_CTRL0_T         	cvb_int_sync_ctrl0;           //0x20c
	PE_H14A0_CVB_INT_SYNC_CTRL1_T         	cvb_int_sync_ctrl1;           //0x210
	PE_H14A0_CVB_DIG_FILT_CTRL0_T         	cvb_dig_filt_ctrl0;           //0x214
	PE_H14A0_CVB_DIG_FILT_CTRL1_T         	cvb_dig_filt_ctrl1;           //0x218
	PE_H14A0_CVB_DIG_FILT_CTRL2_T         	cvb_dig_filt_ctrl2;           //0x21c
	PE_H14A0_CVB_DIG_FILT_CTRL3_T         	cvb_dig_filt_ctrl3;           //0x220
	PE_H14A0_CVB_CSC_CTRL0_T              	cvb_csc_ctrl0;                //0x224
	PE_H14A0_CVB_CSC_CTRL1_T              	cvb_csc_ctrl1;                //0x228
	PE_H14A0_CVB_CSC_CTRL2_T              	cvb_csc_ctrl2;                //0x22c
	PE_H14A0_CVB_CSC_CTRL3_T              	cvb_csc_ctrl3;                //0x230
	PE_H14A0_CVB_CSC_CTRL4_T              	cvb_csc_ctrl4;                //0x234
	PE_H14A0_CVB_CSC_CTRL5_T              	cvb_csc_ctrl5;                //0x238
	PE_H14A0_CVB_CSC_CTRL6_T              	cvb_csc_ctrl6;                //0x23c
	PE_H14A0_CVB_CSC_CTRL7_T              	cvb_csc_ctrl7;                //0x240
	PE_H14A0_CVB_MISC_CTRL_T              	cvb_misc_ctrl;                //0x244
	PE_H14A0_CVB_OFFSET_READ_T            	cvb_offset_read;              //0x248
	PE_H14A0_CVB_PRE3D_CTRL0_T            	cvb_pre3d_ctrl0;              //0x24c
	PE_H14A0_CVB_PRE3D_CTRL1_T            	cvb_pre3d_ctrl1;              //0x250
	PE_H14A0_CVB_PRE3D_CTRL2_T            	cvb_pre3d_ctrl2;              //0x254
	PE_H14A0_CVB_SIZE_STATUS1_T           	cvb_size_status1;             //0x258
	PE_H14A0_CVB_SIZE_STATUS2_T           	cvb_size_status2;             //0x25c
	PE_H14A0_CVB_SIZE_STATUS3_T           	cvb_size_status3;             //0x260
	PE_H14A0_CVB_SIZE_STATUS4_T           	cvb_size_status4;             //0x264
	PE_H14A0_CVB_PIX_SUM_CTRL0_T          	cvb_pix_sum_ctrl0;            //0x268
	PE_H14A0_CVB_PIX_SUM_CTRL1_T          	cvb_pix_sum_ctrl1;            //0x26c
	PE_H14A0_CVB_PIX_SUM_STATUS0_T        	cvb_pix_sum_status0;          //0x270
	PE_H14A0_CVB_PIX_SUM_STATUS1_T        	cvb_pix_sum_status1;          //0x274
	PE_H14A0_CVB_PIX_SUM_STATUS2_T        	cvb_pix_sum_status2;          //0x278
	PE_H14A0_CVB_SYNC_FIL_CTRL_T          	cvb_sync_fil_ctrl;            //0x27c
	PE_H14A0_CVB_SYNC_FIL_STATUS_T        	cvb_sync_fil_status;          //0x280
	PE_H14A0_CVB_CTI_CTRL_T               	cvb_cti_ctrl;                 //0x284
	UINT32                              	reserved00[2];                //0x288~0x28c
	PE_H14A0_CVB_SYNC_REPEAT_CTRL_T       	cvb_sync_repeat_ctrl;         //0x290
	PE_H14A0_CVB_SYNC_REPEAT_SIZE0_T      	cvb_sync_repeat_size0;        //0x294
	PE_H14A0_CVB_SYNC_REPEAT_SIZE1_T      	cvb_sync_repeat_size1;        //0x298
	PE_H14A0_CVB_SYNC_REPEAT_SIZE2_T      	cvb_sync_repeat_size2;        //0x29c
	PE_H14A0_CVB_SYNC_REPEAT_SIZE3_T      	cvb_sync_repeat_size3;        //0x2a0
	PE_H14A0_CVB_SYNC_REPEAT_SIZE4_T      	cvb_sync_repeat_size4;        //0x2a4
	PE_H14A0_CVB_SYNC_REPEAT_SIZE5_T      	cvb_sync_repeat_size5;        //0x2a8
	PE_H14A0_CVB_DIG_FILT_CTRL4_T         	cvb_dig_filt_ctrl4;           //0x2ac
	PE_H14A0_CVB_DIG_FILT_CTRL5_T         	cvb_dig_filt_ctrl5;           //0x2b0
	PE_H14A0_CVB_DIG_FILT_CTRL6_T         	cvb_dig_filt_ctrl6;           //0x2b4
	UINT32                              	reserved01[2];                //0x2b8~0x2bc
	PE_H14A0_CVB_SIZE_DETECT_CTRL_T       	cvb_size_detect_ctrl;         //0x2c0
	PE_H14A0_CVB_SIZE_DETECT_READ_T       	cvb_size_detect_read;         //0x2c4
	PE_H14A0_CVB_SIZE_STATUS0_T           	cvb_size_status0;             //0x2c8
	PE_H14A0_CVB_SIZE_STATUS5_T           	cvb_size_status5;             //0x2cc
	PE_H14A0_CVB_SIZE_STATUS6_T           	cvb_size_status6;             //0x2d0
	PE_H14A0_CVB_SIZE_DETECT_CTRL1_T      	cvb_size_detect_ctrl1;        //0x2d4
	PE_H14A0_CVB_SIZE_DETECT_CTRL2_T      	cvb_size_detect_ctrl2;        //0x2d8
	PE_H14A0_CVB_PSEUDO_SYNC_CTRL0_T      	cvb_pseudo_sync_ctrl0;        //0x2dc
	PE_H14A0_CVB_PSEUDO_SYNC_CTRL1_T      	cvb_pseudo_sync_ctrl1;        //0x2e0
	PE_H14A0_CVB_PSEUDO_SYNC_CTRL2_T      	cvb_pseudo_sync_ctrl2;        //0x2e4
	PE_H14A0_CVB_PSEUDO_SYNC_CTRL3_T      	cvb_pseudo_sync_ctrl3;        //0x2e8
	PE_H14A0_CVB_PSEUDO_SYNC_CTRL4_T      	cvb_pseudo_sync_ctrl4;        //0x2ec
	PE_H14A0_CVB_SIZE_DETECT_CTRL3_T      	cvb_size_detect_ctrl3;        //0x2f0
}PE_CVB_REG_H14A0_T;
#endif