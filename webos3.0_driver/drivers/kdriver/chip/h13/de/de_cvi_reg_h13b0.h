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

#ifndef _REG_CVI_H13B0_h
#define _REG_CVI_H13B0_h

/*-----------------------------------------------------------------------------
	0x0100 cva_top_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 scan_type                       : 1;   //     0
	UINT32 field_gen_mode                  : 2;   //  1: 2
	UINT32 auto_field_gen                  : 2;   //  3: 4
	UINT32 data_bit_mode                   : 1;   //     5
	UINT32 chroma_sampling                 : 1;   //     6
	UINT32 aspect_ratio                    : 1;   //     7
	UINT32 yc_delay_mode_r                 : 2;   //  8: 9
	UINT32 yc_delay_mode_b                 : 2;   // 10:11
	UINT32 yc_delay_mode_g                 : 2;   // 12:13
	UINT32                                 : 5;   // 14:18 reserved
	UINT32 auto_sync_polarity              : 1;   //    19
	UINT32 manual_sync_polarity            : 2;   // 20:21
	UINT32 adc_cut_direction               : 1;   //    22
	UINT32 adc_bit_sel                     : 1;   //    23
	UINT32 br_port_swap                    : 1;   //    24
	UINT32 r_port_swap                     : 1;   //    25
	UINT32 b_port_swap                     : 1;   //    26
	UINT32 g_port_swap                     : 1;   //    27
	UINT32 bit_numbering_lsb2              : 3;   // 28:30
	UINT32 data_en_mode                    : 1;   //    31
} H13B0_CVA_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0104 cva_offset_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                        : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 offset_v                        : 12;  // 16:27
} H13B0_CVA_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0108 cva_size_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                          : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 size_v                          : 12;  // 16:27
} H13B0_CVA_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
	0x010c cva_int_sync_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                       : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 res_hfp                         : 10;  // 16:25
} H13B0_CVA_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0110 cva_int_sync_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                         : 5;   //  0: 4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 res_vsync                       : 5;   //  8:12
	UINT32 res_field_add_line              : 2;   // 13:14
	UINT32 res_field_sel                   : 1;   //    15
	UINT32 res_sync_sel                    : 1;   //    16
} H13B0_CVA_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x0114 cva_dig_filt_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient1                : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_coefficient0                : 10;  // 16:25
} H13B0_CVA_DIG_FILT_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0118 cva_dig_filt_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient3                : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_coefficient2                : 10;  // 16:25
} H13B0_CVA_DIG_FILT_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x011c cva_dig_filt_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient5                : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_coefficient4                : 10;  // 16:25
} H13B0_CVA_DIG_FILT_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0120 cva_dig_filt_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 25;  //  0:24 reserved
	UINT32 fir_sample_mode                 : 1;   //    25
	UINT32 fir_coeff_load                  : 1;   //    26
	UINT32 fir_en                          : 1;   //    27
	UINT32 fir_y_en                        : 1;   //    28
	UINT32 fir_sampling_rate               : 2;   // 29:30
	UINT32 fir_c_en                        : 1;   //    31
} H13B0_CVA_DIG_FILT_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x0124 cva_csc_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient0                : 15;  // 16:30
} H13B0_CVA_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0128 cva_csc_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient2                : 15;  // 16:30
} H13B0_CVA_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x012c cva_csc_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient4                : 15;  // 16:30
} H13B0_CVA_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0130 cva_csc_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient6                : 15;  // 16:30
} H13B0_CVA_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x0134 cva_csc_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                     : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 csc_coefficient8                : 15;  // 16:30
} H13B0_CVA_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x0138 cva_csc_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                     : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 csc_offset1                     : 11;  // 16:26
} H13B0_CVA_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
	0x013c cva_csc_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                     : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 csc_offset3                     : 11;  // 16:26
} H13B0_CVA_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
	0x0140 cva_csc_ctrl7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                  : 1;   //     0
	UINT32 csc_en                          : 1;   //     1
	UINT32 csc_msb_4b_only_mode            : 1;   //     2
	UINT32                                 : 13;  //  3:15 reserved
	UINT32 csc_offset5                     : 11;  // 16:26
} H13B0_CVA_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
	0x0144 cva_misc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_type                    : 3;   //  0: 2
	UINT32 pattern_csc                     : 1;   //     3
	UINT32 pattern_detail                  : 4;   //  4: 7
	UINT32 update_pulse                    : 1;   //     8
	UINT32 field_fix_mode                  : 2;   //  9:10
	UINT32 black_sel                       : 1;   //    11
	UINT32 write_inhibit                   : 1;   //    12
	UINT32 sim_mode                        : 1;   //    13
	UINT32 black_off                       : 1;   //    14
	UINT32 field_edge                      : 1;   //    15
	UINT32 ha_gen                          : 1;   //    16
	UINT32 xsize_divide                    : 2;   // 17:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 yc_delay_mode_r                 : 2;   // 20:21
	UINT32 yc_delay_mode_b                 : 2;   // 22:23
	UINT32 yc_delay_mode_g                 : 2;   // 24:25
	UINT32 field_detect_vris               : 1;   //    26
	UINT32                                 : 1;   //    27 reserved
	UINT32 update_position                 : 4;   // 28:31
} H13B0_CVA_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0148 cva_offset_read ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                    : 13;  //  0:12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 voffset_read                    : 13;  // 16:28
} H13B0_CVA_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
	0x014c cva_pre3d_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_sync_enable               : 1;   //     0
	UINT32 pre3d_sync_mode                 : 2;   //  1: 2
	UINT32 pre3d_field_mode                : 1;   //     3
	UINT32 pre3d_frame_lr_mode             : 1;   //     4
	UINT32 pre3d_mid_vsync_bypass          : 1;   //     5
	UINT32 pre3d_field_bypass              : 1;   //     6
	UINT32                                 : 9;   //  7:15 reserved
	UINT32 pre3d_active_line_num           : 12;  // 16:27
} H13B0_CVA_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0150 cva_pre3d_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pre3d_active_space2_line_num    : 12;  // 16:27
} H13B0_CVA_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x0154 cva_pre3d_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pre3d_sync_line_num             : 12;  // 16:27
} H13B0_CVA_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0158 cva_size_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_hoffset_indurance_read       : 8;   //  0: 7
	UINT32 sd_voffset_indurance_read       : 8;   //  8:15
	UINT32 sd_hsync_fault_flag_read        : 1;   //    16
	UINT32 sd_vsync_fault_flag_read        : 1;   //    17
	UINT32 h_sync_polarity                 : 1;   //    18
	UINT32 v_sync_polarity                 : 1;   //    19
	UINT32 field_signal                    : 4;   // 20:23
	UINT32 hsync_at_vsync_fall             : 4;   // 24:27
	UINT32 outfield_at_vactive_fall        : 2;   // 28:29
	UINT32 hoffset_over                    : 1;   //    30
	UINT32 v_offset_over                   : 1;   //    31
} H13B0_CVA_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x015c cva_size_status2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 de_line_count                   : 12;  // 16:27
} H13B0_CVA_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
	0x0160 cva_size_status3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 hd_line_size                    : 12;  // 16:27
} H13B0_CVA_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
	0x0164 cva_size_status4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                   : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 vd_line_count                   : 12;  // 16:27
} H13B0_CVA_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
	0x0168 cva_pix_sum_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                     : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 end_pos_x                       : 12;  // 16:27
	UINT32 pix_sum_enable                  : 1;   //    28
	UINT32 field_selection                 : 2;   // 29:30
	UINT32 debug_enable                    : 1;   //    31
} H13B0_CVA_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x016c cva_pix_sum_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                     : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 end_pos_y                       : 12;  // 16:27
} H13B0_CVA_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x0170 cva_pix_sum_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                       : 32;  //  0:31
} H13B0_CVA_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x0174 cva_pix_sum_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                       : 32;  //  0:31
} H13B0_CVA_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x0178 cva_pix_sum_status2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                       : 32;  //  0:31
} H13B0_CVA_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
	0x017c cva_sync_fil_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_fil_en                    : 1;   //     0
	UINT32 hsync_counter_en                : 1;   //     1
	UINT32 vsync_fil_en                    : 1;   //     2
	UINT32 vsync_counter_en                : 1;   //     3
	UINT32 de_fil_en                       : 1;   //     4
	UINT32 de_counter_en                   : 1;   //     5
	UINT32 field_fil_en                    : 1;   //     6
	UINT32 field_counter_en                : 1;   //     7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 line_count_sel                  : 12;  // 16:27
} H13B0_CVA_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0180 cva_sync_fil_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                   : 8;   //  0: 7
	UINT32 vsync_counter                   : 8;   //  8:15
	UINT32 de_counter                      : 8;   // 16:23
	UINT32 field_counter                   : 8;   // 24:31
} H13B0_CVA_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
	0x0184 cva_cti_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 master_enable                   : 1;   //     0
	UINT32                                 : 1;   //     1 reserved
	UINT32 cti_enable                      : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 usm_mode                        : 1;   //     4
	UINT32 pepeat_mode                     : 1;   //     5
	UINT32                                 : 18;  //  6:23 reserved
	UINT32 cti_gain                        : 8;   // 24:31
} H13B0_CVA_CTI_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0190 cva_sync_repeat_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_y_en                     : 1;   //     0
	UINT32 repeat_cb_en                    : 1;   //     1
	UINT32 repeat_cr_en                    : 1;   //     2
	UINT32 repeat_pixel_odd                : 1;   //     3
	UINT32                                 : 4;   //  4: 7 reserved
	UINT32 repeat_threshold_y              : 6;   //  8:13
	UINT32 avg_threshold_y                 : 6;   // 14:19
	UINT32 repeat_threshold_c              : 6;   // 20:25
	UINT32 avg_threshold_c                 : 6;   // 26:31
} H13B0_CVA_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0194 cva_sync_repeat_size0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                  : 21;  //  0:20
} H13B0_CVA_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
	0x0198 cva_sync_repeat_size1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                     : 21;  //  0:20
} H13B0_CVA_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
	0x019c cva_sync_repeat_size2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                 : 21;  //  0:20
} H13B0_CVA_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
	0x01a0 cva_sync_repeat_size3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                    : 21;  //  0:20
} H13B0_CVA_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
	0x01a4 cva_sync_repeat_size4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                 : 21;  //  0:20
} H13B0_CVA_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
	0x01a8 cva_sync_repeat_size5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                    : 21;  //  0:20
} H13B0_CVA_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
	0x01ac cva_dig_filt_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient1              : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_c_coefficient0              : 10;  // 16:25
} H13B0_CVA_DIG_FILT_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x01b0 cva_dig_filt_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient3              : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_c_coefficient2              : 10;  // 16:25
} H13B0_CVA_DIG_FILT_CTRL5_T;

/*-----------------------------------------------------------------------------
	0x01b4 cva_dig_filt_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient5              : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_c_coefficient4              : 10;  // 16:25
} H13B0_CVA_DIG_FILT_CTRL6_T;

/*-----------------------------------------------------------------------------
	0x01c0 cva_size_detect_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_vd_range                     : 2;   //  0: 1
	UINT32 sd_ext_range                    : 2;   //  2: 3
	UINT32 intr_mask_node                  : 1;   //     4
	UINT32 intr_mask_nohsync               : 1;   //     5
	UINT32 intrl_mask_novsync              : 1;   //     6
	UINT32 intr_mask_abnormal              : 1;   //     7
	UINT32 intr_mask_invalid               : 1;   //     8
	UINT32 intr_mask_de                    : 1;   //     9
	UINT32 intr_mask_hsync                 : 1;   //    10
	UINT32 intr_mask_vsync                 : 1;   //    11
	UINT32 intr_mask_normal                : 1;   //    12
	UINT32 intr_mask_valid                 : 1;   //    13
	UINT32 intr_mask_h_cnahge              : 1;   //    14
	UINT32 sd_en                           : 1;   //    15
	UINT32 sd_frame_cnt                    : 3;   // 16:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 sd_novald_duration              : 2;   // 20:21
	UINT32                                 : 1;   //    22 reserved
	UINT32 sd_one_field                    : 1;   //    23
	UINT32                                 : 2;   // 24:25 reserved
	UINT32 nosig_duration_de               : 2;   // 26:27
	UINT32 nosig_duration_hsync            : 2;   // 28:29
	UINT32 nosig_duration_vsync            : 2;   // 30:31
} H13B0_CVA_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
	0x01c4 cva_size_detect_read ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                   : 16;  //  0:15
	UINT32 line_width_read                 : 16;  // 16:31
} H13B0_CVA_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
	0x01c8 cva_size_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read        : 12;  //  0:11
	UINT32 de_signal_detect                : 1;   //    12
	UINT32 hsync_signal_detect             : 1;   //    13
	UINT32 vsync_signal_detect             : 1;   //    14
	UINT32 source_valid                    : 1;   //    15
	UINT32 sd_line_num                     : 12;  // 16:27
	UINT32 field_at_vactive_fall           : 4;   // 28:31
} H13B0_CVA_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x01cc cva_size_status5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                  : 24;  //  0:23
} H13B0_CVA_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
	0x01d0 cva_size_status6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                    : 16;  //  0:15
	UINT32 diff_ext_vcnt                   : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 vd_intr                         : 1;   //    31
} H13B0_CVA_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
	0x01d4 cva_size_detect_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal0_en                    : 1;   //     0
	UINT32 abnormal1_en                    : 1;   //     1
	UINT32 abnormal2_en                    : 1;   //     2
	UINT32 abnormal3_en                    : 1;   //     3
	UINT32 abnormal4_en                    : 1;   //     4
	UINT32 abnormal5_en                    : 1;   //     5
	UINT32 abnormal6_en                    : 1;   //     6
	UINT32 abnormal7_en                    : 1;   //     7
	UINT32 abnormal0_flag                  : 1;   //     8
	UINT32 abnormal1_flag                  : 1;   //     9
	UINT32 abnormal2_flag                  : 1;   //    10
	UINT32 abnormal3_flag                  : 1;   //    11
	UINT32 abnormal4_flag                  : 1;   //    12
	UINT32 abnormal5_flag                  : 1;   //    13
	UINT32 abnormal6_flag                  : 1;   //    14
	UINT32 abnormal7_flag                  : 1;   //    15
	UINT32 ext_clk_valid_count             : 15;  // 16:30
	UINT32 ext_clk_valid                   : 1;   //    31
} H13B0_CVA_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x01d8 cva_size_detect_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                   : 8;   //  0: 7
	UINT32 abnormal_out_h                  : 8;   //  8:15
	UINT32 abnormal_in_v                   : 8;   // 16:23
	UINT32 abnormal_out_v                  : 8;   // 24:31
} H13B0_CVA_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x01dc cva_pseudo_sync_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync_pesudo_sel                : 1;   //     0
	UINT32 hsync_pesudo_sel                : 1;   //     1
	UINT32 vactive_pesudo_sel              : 1;   //     2
	UINT32 hactive_pesudo_sel              : 1;   //     3
	UINT32 field_pesudo_sel                : 1;   //     4
	UINT32 y_pseudo_sel                    : 1;   //     5
	UINT32 cb_psuedo_sel                   : 1;   //     6
	UINT32 cr_psuedo_sel                   : 1;   //     7
	UINT32 valid_pseudo_sel                : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 auto_pseudo_sync_en             : 1;   //    12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 pseudo_sync_ctrl_en             : 1;   //    16
	UINT32 pseudo_y_data_sel               : 1;   //    17
	UINT32 pseudo_cb_data_sel              : 1;   //    18
	UINT32 pseudo_cr_data_sel              : 1;   //    19
	UINT32 pseudo_y                        : 4;   // 20:23
	UINT32 pseudo_cb                       : 4;   // 24:27
	UINT32 pseudo_cr                       : 4;   // 28:31
} H13B0_CVA_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x01e0 cva_pseudo_sync_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                      : 14;  //  0:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 pseudo_hsync                    : 14;  // 16:29
} H13B0_CVA_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x01e4 cva_pseudo_sync_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                     : 14;  //  0:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 pseudo_hn                       : 14;  // 16:29
} H13B0_CVA_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x01e8 cva_pseudo_sync_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pseudo_vsync                    : 12;  // 16:27
} H13B0_CVA_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x01ec cva_pseudo_sync_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                     : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pseudo_vn                       : 12;  // 16:27
} H13B0_CVA_PSEUDO_SYNC_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x01f0 size_detect_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 interrupt_mask_method           : 1;   //     0
	UINT32                                 : 3;   //  1: 3 reserved
	UINT32 interrupt_mask_count            : 12;  //  4:15
	UINT32 diff_hsize                      : 14;  // 16:29
} H13B0_SIZE_DETECT_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x0200 cvb_top_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 scan_type                       : 1;   //     0
	UINT32 field_gen_mode                  : 2;   //  1: 2
	UINT32 auto_field_gen                  : 2;   //  3: 4
	UINT32 data_bit_mode                   : 1;   //     5
	UINT32 chroma_sampling                 : 1;   //     6
	UINT32 aspect_ratio                    : 1;   //     7
	UINT32 yc_delay_mode_r                 : 2;   //  8: 9
	UINT32 yc_delay_mode_b                 : 2;   // 10:11
	UINT32 yc_delay_mode_g                 : 2;   // 12:13
	UINT32                                 : 5;   // 14:18 reserved
	UINT32 auto_sync_polarity              : 1;   //    19
	UINT32 manual_sync_polarity            : 2;   // 20:21
	UINT32 adc_cut_direction               : 1;   //    22
	UINT32 adc_bit_sel                     : 1;   //    23
	UINT32 br_port_swap                    : 1;   //    24
	UINT32 r_port_swap                     : 1;   //    25
	UINT32 b_port_swap                     : 1;   //    26
	UINT32 g_port_swap                     : 1;   //    27
	UINT32 bit_numbering_lsb2              : 3;   // 28:30
	UINT32 data_en_mode                    : 1;   //    31
} H13B0_CVB_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0204 cvb_offset_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                        : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 offset_v                        : 12;  // 16:27
} H13B0_CVB_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0208 cvb_size_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                          : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 size_v                          : 12;  // 16:27
} H13B0_CVB_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
	0x020c cvb_int_sync_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                       : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 res_hfp                         : 10;  // 16:25
} H13B0_CVB_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0210 cvb_int_sync_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                         : 5;   //  0: 4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 res_vsync                       : 5;   //  8:12
	UINT32 res_field_add_line              : 2;   // 13:14
	UINT32 res_field_sel                   : 1;   //    15
	UINT32 res_sync_sel                    : 1;   //    16
} H13B0_CVB_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x0214 cvb_dig_filt_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient1                : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_coefficient0                : 10;  // 16:25
} H13B0_CVB_DIG_FILT_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0218 cvb_dig_filt_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient3                : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_coefficient2                : 10;  // 16:25
} H13B0_CVB_DIG_FILT_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x021c cvb_dig_filt_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_coefficient5                : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_coefficient4                : 10;  // 16:25
} H13B0_CVB_DIG_FILT_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0220 cvb_dig_filt_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 25;  //  0:24 reserved
	UINT32 fir_sample_mode                 : 1;   //    25
	UINT32 fir_coeff_load                  : 1;   //    26
	UINT32 fir_en                          : 1;   //    27
	UINT32 fir_y_en                        : 1;   //    28
	UINT32 fir_sampling_rate               : 2;   // 29:30
	UINT32 fir_c_en                        : 1;   //    31
} H13B0_CVB_DIG_FILT_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x0224 cvb_csc_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient0                : 15;  // 16:30
} H13B0_CVB_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0228 cvb_csc_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient2                : 15;  // 16:30
} H13B0_CVB_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x022c cvb_csc_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient4                : 15;  // 16:30
} H13B0_CVB_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0230 cvb_csc_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                : 15;  //  0:14
	UINT32                                 : 1;   //    15 reserved
	UINT32 csc_coefficient6                : 15;  // 16:30
} H13B0_CVB_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x0234 cvb_csc_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                     : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 csc_coefficient8                : 15;  // 16:30
} H13B0_CVB_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x0238 cvb_csc_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                     : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 csc_offset1                     : 11;  // 16:26
} H13B0_CVB_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
	0x023c cvb_csc_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                     : 11;  //  0:10
	UINT32                                 : 5;   // 11:15 reserved
	UINT32 csc_offset3                     : 11;  // 16:26
} H13B0_CVB_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
	0x0240 cvb_csc_ctrl7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                  : 1;   //     0
	UINT32 csc_en                          : 1;   //     1
	UINT32 csc_msb_4b_only_mode            : 1;   //     2
	UINT32                                 : 13;  //  3:15 reserved
	UINT32 csc_offset5                     : 11;  // 16:26
} H13B0_CVB_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
	0x0244 cvb_misc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pattern_type                    : 3;   //  0: 2
	UINT32 pattern_csc                     : 1;   //     3
	UINT32 pattern_detail                  : 4;   //  4: 7
	UINT32 update_pulse                    : 1;   //     8
	UINT32 field_fix_mode                  : 2;   //  9:10
	UINT32 black_sel                       : 1;   //    11
	UINT32 write_inhibit                   : 1;   //    12
	UINT32 sim_mode                        : 1;   //    13
	UINT32 black_off                       : 1;   //    14
	UINT32 field_edge                      : 1;   //    15
	UINT32 ha_gen                          : 1;   //    16
	UINT32 xsize_divide                    : 2;   // 17:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 yc_delay_mode_r                 : 2;   // 20:21
	UINT32 yc_delay_mode_b                 : 2;   // 22:23
	UINT32 yc_delay_mode_g                 : 2;   // 24:25
	UINT32 field_detect_vris               : 1;   //    26
	UINT32                                 : 1;   //    27 reserved
	UINT32 update_position                 : 4;   // 28:31
} H13B0_CVB_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0248 cvb_offset_read ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                    : 13;  //  0:12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 voffset_read                    : 13;  // 16:28
} H13B0_CVB_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
	0x024c cvb_pre3d_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_sync_enable               : 1;   //     0
	UINT32 pre3d_sync_mode                 : 2;   //  1: 2
	UINT32 pre3d_field_mode                : 1;   //     3
	UINT32 pre3d_frame_lr_mode             : 1;   //     4
	UINT32 pre3d_mid_vsync_bypass          : 1;   //     5
	UINT32 pre3d_field_bypass              : 1;   //     6
	UINT32                                 : 9;   //  7:15 reserved
	UINT32 pre3d_active_line_num           : 12;  // 16:27
} H13B0_CVB_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0250 cvb_pre3d_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pre3d_active_space2_line_num    : 12;  // 16:27
} H13B0_CVB_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x0254 cvb_pre3d_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pre3d_sync_line_num             : 12;  // 16:27
} H13B0_CVB_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0258 cvb_size_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_hoffset_indurance_read       : 8;   //  0: 7
	UINT32 sd_voffset_indurance_read       : 8;   //  8:15
	UINT32 sd_hsync_fault_flag_read        : 1;   //    16
	UINT32 sd_vsync_fault_flag_read        : 1;   //    17
	UINT32 h_sync_polarity                 : 1;   //    18
	UINT32 v_sync_polarity                 : 1;   //    19
	UINT32 field_signal                    : 4;   // 20:23
	UINT32 hsync_at_vsync_fall             : 4;   // 24:27
	UINT32 outfield_at_vactive_fall        : 2;   // 28:29
	UINT32 hoffset_over                    : 1;   //    30
	UINT32 v_offset_over                   : 1;   //    31
} H13B0_CVB_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x025c cvb_size_status2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 de_line_count                   : 12;  // 16:27
} H13B0_CVB_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
	0x0260 cvb_size_status3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                    : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 hd_line_size                    : 12;  // 16:27
} H13B0_CVB_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
	0x0264 cvb_size_status4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                   : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 vd_line_count                   : 12;  // 16:27
} H13B0_CVB_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
	0x0268 cvb_pix_sum_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                     : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 end_pos_x                       : 12;  // 16:27
	UINT32 pix_sum_enable                  : 1;   //    28
	UINT32 field_selection                 : 2;   // 29:30
	UINT32 debug_enable                    : 1;   //    31
} H13B0_CVB_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x026c cvb_pix_sum_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                     : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 end_pos_y                       : 12;  // 16:27
} H13B0_CVB_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x0270 cvb_pix_sum_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                       : 32;  //  0:31
} H13B0_CVB_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x0274 cvb_pix_sum_status1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                       : 32;  //  0:31
} H13B0_CVB_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
	0x0278 cvb_pix_sum_status2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                       : 32;  //  0:31
} H13B0_CVB_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
	0x027c cvb_sync_fil_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_fil_en                    : 1;   //     0
	UINT32 hsync_counter_en                : 1;   //     1
	UINT32 vsync_fil_en                    : 1;   //     2
	UINT32 vsync_counter_en                : 1;   //     3
	UINT32 de_fil_en                       : 1;   //     4
	UINT32 de_counter_en                   : 1;   //     5
	UINT32 field_fil_en                    : 1;   //     6
	UINT32 field_counter_en                : 1;   //     7
	UINT32                                 : 8;   //  8:15 reserved
	UINT32 line_count_sel                  : 12;  // 16:27
} H13B0_CVB_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0280 cvb_sync_fil_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                   : 8;   //  0: 7
	UINT32 vsync_counter                   : 8;   //  8:15
	UINT32 de_counter                      : 8;   // 16:23
	UINT32 field_counter                   : 8;   // 24:31
} H13B0_CVB_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
	0x0284 cvb_cti_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 master_enable                   : 1;   //     0
	UINT32                                 : 1;   //     1 reserved
	UINT32 cti_enable                      : 1;   //     2
	UINT32                                 : 1;   //     3 reserved
	UINT32 usm_mode                        : 1;   //     4
	UINT32 pepeat_mode                     : 1;   //     5
	UINT32                                 : 18;  //  6:23 reserved
	UINT32 cti_gain                        : 8;   // 24:31
} H13B0_CVB_CTI_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0290 cvb_sync_repeat_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_y_en                     : 1;   //     0
	UINT32 repeat_cb_en                    : 1;   //     1
	UINT32 repeat_cr_en                    : 1;   //     2
	UINT32 repeat_pixel_odd                : 1;   //     3
	UINT32                                 : 4;   //  4: 7 reserved
	UINT32 repeat_threshold_y              : 6;   //  8:13
	UINT32 avg_threshold_y                 : 6;   // 14:19
	UINT32 repeat_threshold_c              : 6;   // 20:25
	UINT32 avg_threshold_c                 : 6;   // 26:31
} H13B0_CVB_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0294 cvb_sync_repeat_size0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                  : 21;  //  0:20
} H13B0_CVB_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
	0x0298 cvb_sync_repeat_size1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                     : 21;  //  0:20
} H13B0_CVB_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
	0x029c cvb_sync_repeat_size2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                 : 21;  //  0:20
} H13B0_CVB_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
	0x02a0 cvb_sync_repeat_size3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                    : 21;  //  0:20
} H13B0_CVB_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
	0x02a4 cvb_sync_repeat_size4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                 : 21;  //  0:20
} H13B0_CVB_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
	0x02a8 cvb_sync_repeat_size5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                    : 21;  //  0:20
} H13B0_CVB_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
	0x02ac cvb_dig_filt_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient1              : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_c_coefficient0              : 10;  // 16:25
} H13B0_CVB_DIG_FILT_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x02b0 cvb_dig_filt_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient3              : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_c_coefficient2              : 10;  // 16:25
} H13B0_CVB_DIG_FILT_CTRL5_T;

/*-----------------------------------------------------------------------------
	0x02b4 cvb_dig_filt_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coefficient5              : 10;  //  0: 9
	UINT32                                 : 6;   // 10:15 reserved
	UINT32 fir_c_coefficient4              : 10;  // 16:25
} H13B0_CVB_DIG_FILT_CTRL6_T;

/*-----------------------------------------------------------------------------
	0x02c0 cvb_size_detect_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_vd_range                     : 2;   //  0: 1
	UINT32 sd_ext_range                    : 2;   //  2: 3
	UINT32 intr_mask_node                  : 1;   //     4
	UINT32 intr_mask_nohsync               : 1;   //     5
	UINT32 intrl_mask_novsync              : 1;   //     6
	UINT32 intr_mask_abnormal              : 1;   //     7
	UINT32 intr_mask_invalid               : 1;   //     8
	UINT32 intr_mask_de                    : 1;   //     9
	UINT32 intr_mask_hsync                 : 1;   //    10
	UINT32 intr_mask_vsync                 : 1;   //    11
	UINT32 intr_mask_normal                : 1;   //    12
	UINT32 intr_mask_valid                 : 1;   //    13
	UINT32                                 : 1;   //    14 reserved
	UINT32 sd_en                           : 1;   //    15
	UINT32 sd_frame_cnt                    : 3;   // 16:18
	UINT32                                 : 1;   //    19 reserved
	UINT32 sd_novald_duration              : 2;   // 20:21
	UINT32                                 : 1;   //    22 reserved
	UINT32 sd_one_field                    : 1;   //    23
	UINT32 intr_mask_ext_diff              : 1;   //    24
	UINT32 intr_mask_vd_diff               : 1;   //    25
	UINT32 nosig_duration_de               : 2;   // 26:27
	UINT32 nosig_duration_hsync            : 2;   // 28:29
	UINT32 nosig_duration_vsync            : 2;   // 30:31
} H13B0_CVB_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
	0x02c4 cvb_size_detect_read ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                   : 16;  //  0:15
	UINT32 line_width_read                 : 16;  // 16:31
} H13B0_CVB_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
	0x02c8 cvb_size_status0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read        : 12;  //  0:11
	UINT32 de_signal_detect                : 1;   //    12
	UINT32 hsync_signal_detect             : 1;   //    13
	UINT32 vsync_signal_detect             : 1;   //    14
	UINT32 source_valid                    : 1;   //    15
	UINT32 sd_line_num                     : 12;  // 16:27
	UINT32 field_at_vactive_fall           : 4;   // 28:31
} H13B0_CVB_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
	0x02cc cvb_size_status5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                  : 24;  //  0:23
} H13B0_CVB_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
	0x02d0 cvb_size_status6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                    : 16;  //  0:15
	UINT32 diff_ext_vcnt                   : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 vd_intr                         : 1;   //    31
} H13B0_CVB_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
	0x02d4 cvb_size_detect_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal0_en                    : 1;   //     0
	UINT32 abnormal1_en                    : 1;   //     1
	UINT32 abnormal2_en                    : 1;   //     2
	UINT32 abnormal3_en                    : 1;   //     3
	UINT32 abnormal4_en                    : 1;   //     4
	UINT32 abnormal5_en                    : 1;   //     5
	UINT32 abnormal6_en                    : 1;   //     6
	UINT32 abnormal7_en                    : 1;   //     7
	UINT32 abnormal0_flag                  : 1;   //     8
	UINT32 abnormal1_flag                  : 1;   //     9
	UINT32 abnormal2_flag                  : 1;   //    10
	UINT32 abnormal3_flag                  : 1;   //    11
	UINT32 abnormal4_flag                  : 1;   //    12
	UINT32 abnormal5_flag                  : 1;   //    13
	UINT32 abnormal6_flag                  : 1;   //    14
	UINT32 abnormal7_flag                  : 1;   //    15
	UINT32 ext_clk_valid_count             : 15;  // 16:30
	UINT32 ext_clk_valid                   : 1;   //    31
} H13B0_CVB_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x02d8 cvb_size_detect_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                   : 8;   //  0: 7
	UINT32 abnormal_out_h                  : 8;   //  8:15
	UINT32 abnormal_in_v                   : 8;   // 16:23
	UINT32 abnormal_out_v                  : 8;   // 24:31
} H13B0_CVB_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x02dc cvb_pseudo_sync_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vsync_pesudo_sel                : 1;   //     0
	UINT32 hsync_pesudo_sel                : 1;   //     1
	UINT32 vactive_pesudo_sel              : 1;   //     2
	UINT32 hactive_pesudo_sel              : 1;   //     3
	UINT32 field_pesudo_sel                : 1;   //     4
	UINT32 y_pseudo_sel                    : 1;   //     5
	UINT32 cb_psuedo_sel                   : 1;   //     6
	UINT32 cr_psuedo_sel                   : 1;   //     7
	UINT32 valid_pseudo_sel                : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 auto_pseudo_sync_en             : 1;   //    12
	UINT32                                 : 3;   // 13:15 reserved
	UINT32 pseudo_sync_ctrl_en             : 1;   //    16
	UINT32 pseudo_y_data_sel               : 1;   //    17
	UINT32 pseudo_cb_data_sel              : 1;   //    18
	UINT32 pseudo_cr_data_sel              : 1;   //    19
	UINT32 pseudo_y                        : 4;   // 20:23
	UINT32 pseudo_cb                       : 4;   // 24:27
	UINT32 pseudo_cr                       : 4;   // 28:31
} H13B0_CVB_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x02e0 cvb_pseudo_sync_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                      : 14;  //  0:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 pseudo_hsync                    : 14;  // 16:29
} H13B0_CVB_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x02e4 cvb_pseudo_sync_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                     : 14;  //  0:13
	UINT32                                 : 2;   // 14:15 reserved
	UINT32 pseudo_hn                       : 14;  // 16:29
} H13B0_CVB_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x02e8 cvb_pseudo_sync_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pseudo_vsync                    : 12;  // 16:27
} H13B0_CVB_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x02ec cvb_pseudo_sync_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                     : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 pseudo_vn                       : 12;  // 16:27
} H13B0_CVB_PSEUDO_SYNC_CTRL4_T;

typedef struct {
	H13B0_CVA_TOP_CTRL_T            cva_top_ctrl;                    // 0x0100
	H13B0_CVA_OFFSET_CTRL_T         cva_offset_ctrl;                 // 0x0104
	H13B0_CVA_SIZE_CTRL_T           cva_size_ctrl;                   // 0x0108
	H13B0_CVA_INT_SYNC_CTRL0_T      cva_int_sync_ctrl0;              // 0x010c
	H13B0_CVA_INT_SYNC_CTRL1_T      cva_int_sync_ctrl1;              // 0x0110
	H13B0_CVA_DIG_FILT_CTRL0_T      cva_dig_filt_ctrl0;              // 0x0114
	H13B0_CVA_DIG_FILT_CTRL1_T      cva_dig_filt_ctrl1;              // 0x0118
	H13B0_CVA_DIG_FILT_CTRL2_T      cva_dig_filt_ctrl2;              // 0x011c
	H13B0_CVA_DIG_FILT_CTRL3_T      cva_dig_filt_ctrl3;              // 0x0120
	H13B0_CVA_CSC_CTRL0_T           cva_csc_ctrl0;                   // 0x0124
	H13B0_CVA_CSC_CTRL1_T           cva_csc_ctrl1;                   // 0x0128
	H13B0_CVA_CSC_CTRL2_T           cva_csc_ctrl2;                   // 0x012c
	H13B0_CVA_CSC_CTRL3_T           cva_csc_ctrl3;                   // 0x0130
	H13B0_CVA_CSC_CTRL4_T           cva_csc_ctrl4;                   // 0x0134
	H13B0_CVA_CSC_CTRL5_T           cva_csc_ctrl5;                   // 0x0138
	H13B0_CVA_CSC_CTRL6_T           cva_csc_ctrl6;                   // 0x013c
	H13B0_CVA_CSC_CTRL7_T           cva_csc_ctrl7;                   // 0x0140
	H13B0_CVA_MISC_CTRL_T           cva_misc_ctrl;                   // 0x0144
	H13B0_CVA_OFFSET_READ_T         cva_offset_read;                 // 0x0148
	H13B0_CVA_PRE3D_CTRL0_T         cva_pre3d_ctrl0;                 // 0x014c
	H13B0_CVA_PRE3D_CTRL1_T         cva_pre3d_ctrl1;                 // 0x0150
	H13B0_CVA_PRE3D_CTRL2_T         cva_pre3d_ctrl2;                 // 0x0154
	H13B0_CVA_SIZE_STATUS1_T        cva_size_status1;                // 0x0158
	H13B0_CVA_SIZE_STATUS2_T        cva_size_status2;                // 0x015c
	H13B0_CVA_SIZE_STATUS3_T        cva_size_status3;                // 0x0160
	H13B0_CVA_SIZE_STATUS4_T        cva_size_status4;                // 0x0164
	H13B0_CVA_PIX_SUM_CTRL0_T       cva_pix_sum_ctrl0;               // 0x0168
	H13B0_CVA_PIX_SUM_CTRL1_T       cva_pix_sum_ctrl1;               // 0x016c
	H13B0_CVA_PIX_SUM_STATUS0_T     cva_pix_sum_status0;             // 0x0170
	H13B0_CVA_PIX_SUM_STATUS1_T     cva_pix_sum_status1;             // 0x0174
	H13B0_CVA_PIX_SUM_STATUS2_T     cva_pix_sum_status2;             // 0x0178
	H13B0_CVA_SYNC_FIL_CTRL_T       cva_sync_fil_ctrl;               // 0x017c
	H13B0_CVA_SYNC_FIL_STATUS_T     cva_sync_fil_status;             // 0x0180
	H13B0_CVA_CTI_CTRL_T            cva_cti_ctrl;                    // 0x0184
	UINT32                          reserved0[2];                    // 0x0188~0x018c
	H13B0_CVA_SYNC_REPEAT_CTRL_T    cva_sync_repeat_ctrl;            // 0x0190
	H13B0_CVA_SYNC_REPEAT_SIZE0_T   cva_sync_repeat_size0;           // 0x0194
	H13B0_CVA_SYNC_REPEAT_SIZE1_T   cva_sync_repeat_size1;           // 0x0198
	H13B0_CVA_SYNC_REPEAT_SIZE2_T   cva_sync_repeat_size2;           // 0x019c
	H13B0_CVA_SYNC_REPEAT_SIZE3_T   cva_sync_repeat_size3;           // 0x01a0
	H13B0_CVA_SYNC_REPEAT_SIZE4_T   cva_sync_repeat_size4;           // 0x01a4
	H13B0_CVA_SYNC_REPEAT_SIZE5_T   cva_sync_repeat_size5;           // 0x01a8
	H13B0_CVA_DIG_FILT_CTRL4_T      cva_dig_filt_ctrl4;              // 0x01ac
	H13B0_CVA_DIG_FILT_CTRL5_T      cva_dig_filt_ctrl5;              // 0x01b0
	H13B0_CVA_DIG_FILT_CTRL6_T      cva_dig_filt_ctrl6;              // 0x01b4
	UINT32                          reserved1[2];                    // 0x01b8~0x01bc
	H13B0_CVA_SIZE_DETECT_CTRL_T    cva_size_detect_ctrl;            // 0x01c0
	H13B0_CVA_SIZE_DETECT_READ_T    cva_size_detect_read;            // 0x01c4
	H13B0_CVA_SIZE_STATUS0_T        cva_size_status0;                // 0x01c8
	H13B0_CVA_SIZE_STATUS5_T        cva_size_status5;                // 0x01cc
	H13B0_CVA_SIZE_STATUS6_T        cva_size_status6;                // 0x01d0
	H13B0_CVA_SIZE_DETECT_CTRL1_T   cva_size_detect_ctrl1;           // 0x01d4
	H13B0_CVA_SIZE_DETECT_CTRL2_T   cva_size_detect_ctrl2;           // 0x01d8
	H13B0_CVA_PSEUDO_SYNC_CTRL0_T   cva_pseudo_sync_ctrl0;           // 0x01dc
	H13B0_CVA_PSEUDO_SYNC_CTRL1_T   cva_pseudo_sync_ctrl1;           // 0x01e0
	H13B0_CVA_PSEUDO_SYNC_CTRL2_T   cva_pseudo_sync_ctrl2;           // 0x01e4
	H13B0_CVA_PSEUDO_SYNC_CTRL3_T   cva_pseudo_sync_ctrl3;           // 0x01e8
	H13B0_CVA_PSEUDO_SYNC_CTRL4_T   cva_pseudo_sync_ctrl4;           // 0x01ec
	H13B0_SIZE_DETECT_CTRL3_T       size_detect_ctrl3;               // 0x01f0
} DE_CVA_REG_H13B0_T;

typedef struct {
	H13B0_CVB_TOP_CTRL_T            cvb_top_ctrl;                    // 0x0200
	H13B0_CVB_OFFSET_CTRL_T         cvb_offset_ctrl;                 // 0x0204
	H13B0_CVB_SIZE_CTRL_T           cvb_size_ctrl;                   // 0x0208
	H13B0_CVB_INT_SYNC_CTRL0_T      cvb_int_sync_ctrl0;              // 0x020c
	H13B0_CVB_INT_SYNC_CTRL1_T      cvb_int_sync_ctrl1;              // 0x0210
	H13B0_CVB_DIG_FILT_CTRL0_T      cvb_dig_filt_ctrl0;              // 0x0214
	H13B0_CVB_DIG_FILT_CTRL1_T      cvb_dig_filt_ctrl1;              // 0x0218
	H13B0_CVB_DIG_FILT_CTRL2_T      cvb_dig_filt_ctrl2;              // 0x021c
	H13B0_CVB_DIG_FILT_CTRL3_T      cvb_dig_filt_ctrl3;              // 0x0220
	H13B0_CVB_CSC_CTRL0_T           cvb_csc_ctrl0;                   // 0x0224
	H13B0_CVB_CSC_CTRL1_T           cvb_csc_ctrl1;                   // 0x0228
	H13B0_CVB_CSC_CTRL2_T           cvb_csc_ctrl2;                   // 0x022c
	H13B0_CVB_CSC_CTRL3_T           cvb_csc_ctrl3;                   // 0x0230
	H13B0_CVB_CSC_CTRL4_T           cvb_csc_ctrl4;                   // 0x0234
	H13B0_CVB_CSC_CTRL5_T           cvb_csc_ctrl5;                   // 0x0238
	H13B0_CVB_CSC_CTRL6_T           cvb_csc_ctrl6;                   // 0x023c
	H13B0_CVB_CSC_CTRL7_T           cvb_csc_ctrl7;                   // 0x0240
	H13B0_CVB_MISC_CTRL_T           cvb_misc_ctrl;                   // 0x0244
	H13B0_CVB_OFFSET_READ_T         cvb_offset_read;                 // 0x0248
	H13B0_CVB_PRE3D_CTRL0_T         cvb_pre3d_ctrl0;                 // 0x024c
	H13B0_CVB_PRE3D_CTRL1_T         cvb_pre3d_ctrl1;                 // 0x0250
	H13B0_CVB_PRE3D_CTRL2_T         cvb_pre3d_ctrl2;                 // 0x0254
	H13B0_CVB_SIZE_STATUS1_T        cvb_size_status1;                // 0x0258
	H13B0_CVB_SIZE_STATUS2_T        cvb_size_status2;                // 0x025c
	H13B0_CVB_SIZE_STATUS3_T        cvb_size_status3;                // 0x0260
	H13B0_CVB_SIZE_STATUS4_T        cvb_size_status4;                // 0x0264
	H13B0_CVB_PIX_SUM_CTRL0_T       cvb_pix_sum_ctrl0;               // 0x0268
	H13B0_CVB_PIX_SUM_CTRL1_T       cvb_pix_sum_ctrl1;               // 0x026c
	H13B0_CVB_PIX_SUM_STATUS0_T     cvb_pix_sum_status0;             // 0x0270
	H13B0_CVB_PIX_SUM_STATUS1_T     cvb_pix_sum_status1;             // 0x0274
	H13B0_CVB_PIX_SUM_STATUS2_T     cvb_pix_sum_status2;             // 0x0278
	H13B0_CVB_SYNC_FIL_CTRL_T       cvb_sync_fil_ctrl;               // 0x027c
	H13B0_CVB_SYNC_FIL_STATUS_T     cvb_sync_fil_status;             // 0x0280
	H13B0_CVB_CTI_CTRL_T            cvb_cti_ctrl;                    // 0x0284
	UINT32                          reserved2[2];                    // 0x0288~0x028c
	H13B0_CVB_SYNC_REPEAT_CTRL_T    cvb_sync_repeat_ctrl;            // 0x0290
	H13B0_CVB_SYNC_REPEAT_SIZE0_T   cvb_sync_repeat_size0;           // 0x0294
	H13B0_CVB_SYNC_REPEAT_SIZE1_T   cvb_sync_repeat_size1;           // 0x0298
	H13B0_CVB_SYNC_REPEAT_SIZE2_T   cvb_sync_repeat_size2;           // 0x029c
	H13B0_CVB_SYNC_REPEAT_SIZE3_T   cvb_sync_repeat_size3;           // 0x02a0
	H13B0_CVB_SYNC_REPEAT_SIZE4_T   cvb_sync_repeat_size4;           // 0x02a4
	H13B0_CVB_SYNC_REPEAT_SIZE5_T   cvb_sync_repeat_size5;           // 0x02a8
	H13B0_CVB_DIG_FILT_CTRL4_T      cvb_dig_filt_ctrl4;              // 0x02ac
	H13B0_CVB_DIG_FILT_CTRL5_T      cvb_dig_filt_ctrl5;              // 0x02b0
	H13B0_CVB_DIG_FILT_CTRL6_T      cvb_dig_filt_ctrl6;              // 0x02b4
	UINT32                          reserved3[2];                    // 0x02b8~0x02bc
	H13B0_CVB_SIZE_DETECT_CTRL_T    cvb_size_detect_ctrl;            // 0x02c0
	H13B0_CVB_SIZE_DETECT_READ_T    cvb_size_detect_read;            // 0x02c4
	H13B0_CVB_SIZE_STATUS0_T        cvb_size_status0;                // 0x02c8
	H13B0_CVB_SIZE_STATUS5_T        cvb_size_status5;                // 0x02cc
	H13B0_CVB_SIZE_STATUS6_T        cvb_size_status6;                // 0x02d0
	H13B0_CVB_SIZE_DETECT_CTRL1_T   cvb_size_detect_ctrl1;           // 0x02d4
	H13B0_CVB_SIZE_DETECT_CTRL2_T   cvb_size_detect_ctrl2;           // 0x02d8
	H13B0_CVB_PSEUDO_SYNC_CTRL0_T   cvb_pseudo_sync_ctrl0;           // 0x02dc
	H13B0_CVB_PSEUDO_SYNC_CTRL1_T   cvb_pseudo_sync_ctrl1;           // 0x02e0
	H13B0_CVB_PSEUDO_SYNC_CTRL2_T   cvb_pseudo_sync_ctrl2;           // 0x02e4
	H13B0_CVB_PSEUDO_SYNC_CTRL3_T   cvb_pseudo_sync_ctrl3;           // 0x02e8
	H13B0_CVB_PSEUDO_SYNC_CTRL4_T   cvb_pseudo_sync_ctrl4;           // 0x02ec
} DE_CVB_REG_H13B0_T;

#endif
