#ifndef _DE_CVI_REG_H15A0_H_
#define _DE_CVI_REG_H15A0_H_
/*-----------------------------------------------------------------------------
		0xc8000200L top_ctrl ''
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
	UINT32 data_in_sel                        : 4 ;  // 17:14     
	UINT32                                    : 1 ;  //    18     reserved
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
} H15A0_TOP_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000204L offset_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 offset_h                           : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 offset_v                           : 13;  // 28:16     
} H15A0_OFFSET_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000208L size_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 size_h                             : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 size_v                             : 13;  // 28:16     
} H15A0_SIZE_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc800020cL int_sync_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_hsync                          : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 res_hfp                            : 10;  // 25:16     
} H15A0_INT_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc8000210L int_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 res_vfp                            : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 res_vsync                          : 5 ;  // 12: 8     
	UINT32 res_field_add_line                 : 2 ;  // 14:13     
	UINT32 res_field_sel                      : 1 ;  //    15     
	UINT32 res_sync_sel                       : 1 ;  //    16     
} H15A0_INT_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000224L csc_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient1                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient0                   : 15;  // 30:16     
} H15A0_CSC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc8000228L csc_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient3                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient2                   : 15;  // 30:16     
} H15A0_CSC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc800022cL csc_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient5                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient4                   : 15;  // 30:16     
} H15A0_CSC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc8000230L csc_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coefficient7                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 csc_coefficient6                   : 15;  // 30:16     
} H15A0_CSC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc8000234L csc_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset0                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_coefficient8                   : 15;  // 30:16     
} H15A0_CSC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc8000238L csc_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset2                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset1                        : 11;  // 26:16     
} H15A0_CSC_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc800023cL csc_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_offset4                        : 11;  // 10: 0     
	UINT32                                    : 5 ;  // 15:11     reserved
	UINT32 csc_offset3                        : 11;  // 26:16     
} H15A0_CSC_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc8000240L csc_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_coeff_load                     : 1 ;  //     0     
	UINT32 csc_en                             : 1 ;  //     1     
	UINT32 csc_msb_4b_only_mode               : 1 ;  //     2     
	UINT32                                    : 13;  // 15: 3     reserved
	UINT32 csc_offset5                        : 11;  // 26:16     
} H15A0_CSC_CTRL7_T;

/*-----------------------------------------------------------------------------
		0xc8000244L misc_ctrl ''
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
} H15A0_MISC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000248L offset_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hoffset_read                       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 voffset_read                       : 13;  // 28:16     
} H15A0_OFFSET_READ_T;

/*-----------------------------------------------------------------------------
		0xc800024cL pre3d_ctrl0 ''
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
	UINT32 pre3d_active_line_num              : 13;  // 28:16     
} H15A0_PRE3D_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc8000250L pre3d_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_active_space1_line_num       : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pre3d_active_space2_line_num       : 13;  // 28:16     
} H15A0_PRE3D_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000254L pre3d_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pre3d_front_porch_line_num         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 pre3d_sync_line_num                : 13;  // 28:16     
} H15A0_PRE3D_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc8000258L size_status1 ''
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
} H15A0_SIZE_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc800025cL size_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_line_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 de_line_count                      : 12;  // 27:16     
} H15A0_SIZE_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc8000260L size_status3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hd_sync_size                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 hd_line_size                       : 12;  // 27:16     
} H15A0_SIZE_STATUS3_T;

/*-----------------------------------------------------------------------------
		0xc8000264L size_status4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vd_sync_count                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 vd_line_count                      : 12;  // 27:16     
} H15A0_SIZE_STATUS4_T;

/*-----------------------------------------------------------------------------
		0xc8000268L pix_sum_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_x                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_x                          : 12;  // 27:16     
	UINT32 pix_sum_enable                     : 1 ;  //    28     
	UINT32 field_selection                    : 2 ;  // 30:29     
	UINT32 debug_enable                       : 1 ;  //    31     
} H15A0_PIX_SUM_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc800026cL pix_sum_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 start_pos_y                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 end_pos_y                          : 12;  // 27:16     
} H15A0_PIX_SUM_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc8000270L pix_sum_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_g                          : 32;  // 31: 0     
} H15A0_PIX_SUM_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc8000274L pix_sum_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_b                          : 32;  // 31: 0     
} H15A0_PIX_SUM_STATUS1_T;

/*-----------------------------------------------------------------------------
		0xc8000278L pix_sum_status2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pix_sum_r                          : 32;  // 31: 0     
} H15A0_PIX_SUM_STATUS2_T;

/*-----------------------------------------------------------------------------
		0xc800027cL sync_fil_ctrl ''
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
	UINT32 line_count_sel                     : 13;  // 28:16     
} H15A0_SYNC_FIL_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000280L sync_fil_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hsync_coutner                      : 8 ;  //  7: 0     
	UINT32 vsync_counter                      : 8 ;  // 15: 8     
	UINT32 de_counter                         : 8 ;  // 23:16     
	UINT32 field_counter                      : 8 ;  // 31:24     
} H15A0_SYNC_FIL_STATUS_T;

/*-----------------------------------------------------------------------------
		0xc8000290L sync_repeat_ctrl ''
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
} H15A0_SYNC_REPEAT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc8000294L sync_repeat_size0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_y                     : 21;  // 20: 0     
} H15A0_SYNC_REPEAT_SIZE0_T;

/*-----------------------------------------------------------------------------
		0xc8000298L sync_repeat_size1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_y                        : 21;  // 20: 0     
} H15A0_SYNC_REPEAT_SIZE1_T;

/*-----------------------------------------------------------------------------
		0xc800029cL sync_repeat_size2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cb                    : 21;  // 20: 0     
} H15A0_SYNC_REPEAT_SIZE2_T;

/*-----------------------------------------------------------------------------
		0xc80002a0L sync_repeat_size3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} H15A0_SYNC_REPEAT_SIZE3_T;

/*-----------------------------------------------------------------------------
		0xc80002a4L sync_repeat_size4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 repeat_pixel_cr                    : 21;  // 20: 0     
} H15A0_SYNC_REPEAT_SIZE4_T;

/*-----------------------------------------------------------------------------
		0xc80002a8L sync_repeat_size5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 avg_pixel_cb                       : 21;  // 20: 0     
} H15A0_SYNC_REPEAT_SIZE5_T;

/*-----------------------------------------------------------------------------
		0xc80002c0L size_detect_ctrl ''
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
} H15A0_SIZE_DETECT_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc80002c4L size_detect_read ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 line_num_read                      : 16;  // 15: 0     
	UINT32 line_width_read                    : 16;  // 31:16     
} H15A0_SIZE_DETECT_READ_T;

/*-----------------------------------------------------------------------------
		0xc80002c8L size_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 current_line_number_read           : 12;  // 11: 0     
	UINT32 de_signal_detect                   : 1 ;  //    12     
	UINT32 hsync_signal_detect                : 1 ;  //    13     
	UINT32 vsync_signal_detect                : 1 ;  //    14     
	UINT32 source_valid                       : 1 ;  //    15     
	UINT32 sd_line_num                        : 12;  // 27:16     
	UINT32 field_at_vactive_fall              : 4 ;  // 31:28     
} H15A0_SIZE_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc80002ccL size_status5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sd_2field_size                     : 24;  // 23: 0     
} H15A0_SIZE_STATUS5_T;

/*-----------------------------------------------------------------------------
		0xc80002d0L size_status6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 diff_vd_vcnt                       : 16;  // 15: 0     
	UINT32 diff_ext_vcnt                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 vd_intr                            : 1 ;  //    31     
} H15A0_SIZE_STATUS6_T;

/*-----------------------------------------------------------------------------
		0xc80002d4L size_detect_ctrl1 ''
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
} H15A0_SIZE_DETECT_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc80002d8L size_detect_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 abnormal_in_h                      : 8 ;  //  7: 0     
	UINT32 abnormal_out_h                     : 8 ;  // 15: 8     
	UINT32 abnormal_in_v                      : 8 ;  // 23:16     
	UINT32 abnormal_out_v                     : 8 ;  // 31:24     
} H15A0_SIZE_DETECT_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc80002dcL pseudo_sync_ctrl0 ''
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
} H15A0_PSEUDO_SYNC_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc80002e0L pseudo_sync_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hfp                         : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hsync                       : 14;  // 29:16     
} H15A0_PSEUDO_SYNC_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc80002e4L pseudo_sync_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_hblk                        : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 pseudo_hn                          : 14;  // 29:16     
} H15A0_PSEUDO_SYNC_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc80002e8L pseudo_sync_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vfp                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vsync                       : 12;  // 27:16     
} H15A0_PSEUDO_SYNC_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc80002ecL pseudo_sync_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pseudo_vblk                        : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 pseudo_vn                          : 12;  // 27:16     
} H15A0_PSEUDO_SYNC_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc80002f0L size_detect_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 interrupt_mask_method              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 interrupt_mask_count               : 12;  // 15: 4     
	UINT32 diff_hsize                         : 14;  // 29:16     
} H15A0_SIZE_DETECT_CTRL3_T;

typedef struct {
	H15A0_TOP_CTRL_T                	cv0_top_ctrl;                     //0xc8000200L
	H15A0_OFFSET_CTRL_T             	cv0_offset_ctrl;                  //0xc8000204L
	H15A0_SIZE_CTRL_T               	cv0_size_ctrl;                    //0xc8000208L
	H15A0_INT_SYNC_CTRL0_T          	cv0_int_sync_ctrl0;               //0xc800020cL
	H15A0_INT_SYNC_CTRL1_T          	cv0_int_sync_ctrl1;               //0xc8000210L
	UINT32                             	reserved00[4];                //0xc8000214L~0xc8000220L
	H15A0_CSC_CTRL0_T               	cv0_csc_ctrl0;                    //0xc8000224L
	H15A0_CSC_CTRL1_T               	cv0_csc_ctrl1;                    //0xc8000228L
	H15A0_CSC_CTRL2_T               	cv0_csc_ctrl2;                    //0xc800022cL
	H15A0_CSC_CTRL3_T               	cv0_csc_ctrl3;                    //0xc8000230L
	H15A0_CSC_CTRL4_T               	cv0_csc_ctrl4;                    //0xc8000234L
	H15A0_CSC_CTRL5_T               	cv0_csc_ctrl5;                    //0xc8000238L
	H15A0_CSC_CTRL6_T               	cv0_csc_ctrl6;                    //0xc800023cL
	H15A0_CSC_CTRL7_T               	cv0_csc_ctrl7;                    //0xc8000240L
	H15A0_MISC_CTRL_T               	cv0_misc_ctrl;                    //0xc8000244L
	H15A0_OFFSET_READ_T             	cv0_offset_read;                  //0xc8000248L
	H15A0_PRE3D_CTRL0_T             	cv0_pre3d_ctrl0;                  //0xc800024cL
	H15A0_PRE3D_CTRL1_T             	cv0_pre3d_ctrl1;                  //0xc8000250L
	H15A0_PRE3D_CTRL2_T             	cv0_pre3d_ctrl2;                  //0xc8000254L
	H15A0_SIZE_STATUS1_T            	cv0_size_status1;                 //0xc8000258L
	H15A0_SIZE_STATUS2_T            	cv0_size_status2;                 //0xc800025cL
	H15A0_SIZE_STATUS3_T            	cv0_size_status3;                 //0xc8000260L
	H15A0_SIZE_STATUS4_T            	cv0_size_status4;                 //0xc8000264L
	H15A0_PIX_SUM_CTRL0_T           	cv0_pix_sum_ctrl0;                //0xc8000268L
	H15A0_PIX_SUM_CTRL1_T           	cv0_pix_sum_ctrl1;                //0xc800026cL
	H15A0_PIX_SUM_STATUS0_T         	cv0_pix_sum_status0;              //0xc8000270L
	H15A0_PIX_SUM_STATUS1_T         	cv0_pix_sum_status1;              //0xc8000274L
	H15A0_PIX_SUM_STATUS2_T         	cv0_pix_sum_status2;              //0xc8000278L
	H15A0_SYNC_FIL_CTRL_T           	cv0_sync_fil_ctrl;                //0xc800027cL
	H15A0_SYNC_FIL_STATUS_T         	cv0_sync_fil_status;              //0xc8000280L
	UINT32                             	reserved01[3];                //0xc8000284L~0xc800028cL
	H15A0_SYNC_REPEAT_CTRL_T        	cv0_sync_repeat_ctrl;             //0xc8000290L
	H15A0_SYNC_REPEAT_SIZE0_T       	cv0_sync_repeat_size0;            //0xc8000294L
	H15A0_SYNC_REPEAT_SIZE1_T       	cv0_sync_repeat_size1;            //0xc8000298L
	H15A0_SYNC_REPEAT_SIZE2_T       	cv0_sync_repeat_size2;            //0xc800029cL
	H15A0_SYNC_REPEAT_SIZE3_T       	cv0_sync_repeat_size3;            //0xc80002a0L
	H15A0_SYNC_REPEAT_SIZE4_T       	cv0_sync_repeat_size4;            //0xc80002a4L
	H15A0_SYNC_REPEAT_SIZE5_T       	cv0_sync_repeat_size5;            //0xc80002a8L
	UINT32                             	reserved02[5];                //0xc80002acL~0xc80002bcL
	H15A0_SIZE_DETECT_CTRL_T        	cv0_size_detect_ctrl;             //0xc80002c0L
	H15A0_SIZE_DETECT_READ_T        	cv0_size_detect_read;             //0xc80002c4L
	H15A0_SIZE_STATUS0_T            	cv0_size_status0;                 //0xc80002c8L
	H15A0_SIZE_STATUS5_T            	cv0_size_status5;                 //0xc80002ccL
	H15A0_SIZE_STATUS6_T            	cv0_size_status6;                 //0xc80002d0L
	H15A0_SIZE_DETECT_CTRL1_T       	cv0_size_detect_ctrl1;            //0xc80002d4L
	H15A0_SIZE_DETECT_CTRL2_T       	cv0_size_detect_ctrl2;            //0xc80002d8L
	H15A0_PSEUDO_SYNC_CTRL0_T       	cv0_pseudo_sync_ctrl0;            //0xc80002dcL
	H15A0_PSEUDO_SYNC_CTRL1_T       	cv0_pseudo_sync_ctrl1;            //0xc80002e0L
	H15A0_PSEUDO_SYNC_CTRL2_T       	cv0_pseudo_sync_ctrl2;            //0xc80002e4L
	H15A0_PSEUDO_SYNC_CTRL3_T       	cv0_pseudo_sync_ctrl3;            //0xc80002e8L
	H15A0_PSEUDO_SYNC_CTRL4_T       	cv0_pseudo_sync_ctrl4;            //0xc80002ecL
	H15A0_SIZE_DETECT_CTRL3_T       	cv0_size_detect_ctrl3;            //0xc80002f0L

	UINT32                             	reserved03[3];
	
	H15A0_TOP_CTRL_T                	cv1_top_ctrl;                     //0xc8000300L
	H15A0_OFFSET_CTRL_T             	cv1_offset_ctrl;                  //0xc8000304L
	H15A0_SIZE_CTRL_T               	cv1_size_ctrl;                    //0xc8000308L
	H15A0_INT_SYNC_CTRL0_T          	cv1_int_sync_ctrl0;               //0xc800030cL
	H15A0_INT_SYNC_CTRL1_T          	cv1_int_sync_ctrl1;               //0xc8000310L
	UINT32                             	reserved04[4];                //0xc8000214L30xc8000220L
	H15A0_CSC_CTRL0_T               	cv1_csc_ctrl0;                    //0xc8000324L
	H15A0_CSC_CTRL1_T               	cv1_csc_ctrl1;                    //0xc8000328L
	H15A0_CSC_CTRL2_T               	cv1_csc_ctrl2;                    //0xc800032cL
	H15A0_CSC_CTRL3_T               	cv1_csc_ctrl3;                    //0xc8000330L
	H15A0_CSC_CTRL4_T               	cv1_csc_ctrl4;                    //0xc8000334L
	H15A0_CSC_CTRL5_T               	cv1_csc_ctrl5;                    //0xc8000338L
	H15A0_CSC_CTRL6_T               	cv1_csc_ctrl6;                    //0xc800033cL
	H15A0_CSC_CTRL7_T               	cv1_csc_ctrl7;                    //0xc8000340L
	H15A0_MISC_CTRL_T               	cv1_misc_ctrl;                    //0xc8000344L
	H15A0_OFFSET_READ_T             	cv1_offset_read;                  //0xc8000348L
	H15A0_PRE3D_CTRL0_T             	cv1_pre3d_ctrl0;                  //0xc800034cL
	H15A0_PRE3D_CTRL1_T             	cv1_pre3d_ctrl1;                  //0xc8000350L
	H15A0_PRE3D_CTRL2_T             	cv1_pre3d_ctrl2;                  //0xc8000354L
	H15A0_SIZE_STATUS1_T            	cv1_size_status1;                 //0xc8000358L
	H15A0_SIZE_STATUS2_T            	cv1_size_status2;                 //0xc800035cL
	H15A0_SIZE_STATUS3_T            	cv1_size_status3;                 //0xc8000360L
	H15A0_SIZE_STATUS4_T            	cv1_size_status4;                 //0xc8000364L
	H15A0_PIX_SUM_CTRL0_T           	cv1_pix_sum_ctrl0;                //0xc8000368L
	H15A0_PIX_SUM_CTRL1_T           	cv1_pix_sum_ctrl1;                //0xc800036cL
	H15A0_PIX_SUM_STATUS0_T         	cv1_pix_sum_status0;              //0xc8000370L
	H15A0_PIX_SUM_STATUS1_T         	cv1_pix_sum_status1;              //0xc8000374L
	H15A0_PIX_SUM_STATUS2_T         	cv1_pix_sum_status2;              //0xc8000378L
	H15A0_SYNC_FIL_CTRL_T           	cv1_sync_fil_ctrl;                //0xc800037cL
	H15A0_SYNC_FIL_STATUS_T         	cv1_sync_fil_status;              //0xc8000380L
	UINT32                             	reserved05[3];                //0xc8000284L30xc800028cL
	H15A0_SYNC_REPEAT_CTRL_T        	cv1_sync_repeat_ctrl;             //0xc8000390L
	H15A0_SYNC_REPEAT_SIZE0_T       	cv1_sync_repeat_size0;            //0xc8000394L
	H15A0_SYNC_REPEAT_SIZE1_T       	cv1_sync_repeat_size1;            //0xc8000398L
	H15A0_SYNC_REPEAT_SIZE2_T       	cv1_sync_repeat_size2;            //0xc800039cL
	H15A0_SYNC_REPEAT_SIZE3_T       	cv1_sync_repeat_size3;            //0xc80003a0L
	H15A0_SYNC_REPEAT_SIZE4_T       	cv1_sync_repeat_size4;            //0xc80003a4L
	H15A0_SYNC_REPEAT_SIZE5_T       	cv1_sync_repeat_size5;            //0xc80003a8L
	UINT32                             	reserved06[5];                //0xc80002acL30xc80002bcL
	H15A0_SIZE_DETECT_CTRL_T        	cv1_size_detect_ctrl;             //0xc80003c0L
	H15A0_SIZE_DETECT_READ_T        	cv1_size_detect_read;             //0xc80003c4L
	H15A0_SIZE_STATUS0_T            	cv1_size_status0;                 //0xc80003c8L
	H15A0_SIZE_STATUS5_T            	cv1_size_status5;                 //0xc80003ccL
	H15A0_SIZE_STATUS6_T            	cv1_size_status6;                 //0xc80003d0L
	H15A0_SIZE_DETECT_CTRL1_T       	cv1_size_detect_ctrl1;            //0xc80003d4L
	H15A0_SIZE_DETECT_CTRL2_T       	cv1_size_detect_ctrl2;            //0xc80003d8L
	H15A0_PSEUDO_SYNC_CTRL0_T       	cv1_pseudo_sync_ctrl0;            //0xc80003dcL
	H15A0_PSEUDO_SYNC_CTRL1_T       	cv1_pseudo_sync_ctrl1;            //0xc80003e0L
	H15A0_PSEUDO_SYNC_CTRL2_T       	cv1_pseudo_sync_ctrl2;            //0xc80003e4L
	H15A0_PSEUDO_SYNC_CTRL3_T       	cv1_pseudo_sync_ctrl3;            //0xc80003e8L
	H15A0_PSEUDO_SYNC_CTRL4_T       	cv1_pseudo_sync_ctrl4;            //0xc80003ecL
	H15A0_SIZE_DETECT_CTRL3_T       	cv1_size_detect_ctrl3;            //0xc80003f0L

	UINT32                             	reserved07[3];
	
	H15A0_TOP_CTRL_T                	cv2_top_ctrl;                     //0xc8000400L
	H15A0_OFFSET_CTRL_T             	cv2_offset_ctrl;                  //0xc8000404L
	H15A0_SIZE_CTRL_T               	cv2_size_ctrl;                    //0xc8000408L
	H15A0_INT_SYNC_CTRL0_T          	cv2_int_sync_ctrl0;               //0xc800040cL
	H15A0_INT_SYNC_CTRL1_T          	cv2_int_sync_ctrl1;               //0xc8000410L
	UINT32                             	reserved08[4];                //0xc8000214L40xc8000220L
	H15A0_CSC_CTRL0_T               	cv2_csc_ctrl0;                    //0xc8000424L
	H15A0_CSC_CTRL1_T               	cv2_csc_ctrl1;                    //0xc8000428L
	H15A0_CSC_CTRL2_T               	cv2_csc_ctrl2;                    //0xc800042cL
	H15A0_CSC_CTRL3_T               	cv2_csc_ctrl3;                    //0xc8000430L
	H15A0_CSC_CTRL4_T               	cv2_csc_ctrl4;                    //0xc8000434L
	H15A0_CSC_CTRL5_T               	cv2_csc_ctrl5;                    //0xc8000438L
	H15A0_CSC_CTRL6_T               	cv2_csc_ctrl6;                    //0xc800043cL
	H15A0_CSC_CTRL7_T               	cv2_csc_ctrl7;                    //0xc8000440L
	H15A0_MISC_CTRL_T               	cv2_misc_ctrl;                    //0xc8000444L
	H15A0_OFFSET_READ_T             	cv2_offset_read;                  //0xc8000448L
	H15A0_PRE3D_CTRL0_T             	cv2_pre3d_ctrl0;                  //0xc800044cL
	H15A0_PRE3D_CTRL1_T             	cv2_pre3d_ctrl1;                  //0xc8000450L
	H15A0_PRE3D_CTRL2_T             	cv2_pre3d_ctrl2;                  //0xc8000454L
	H15A0_SIZE_STATUS1_T            	cv2_size_status1;                 //0xc8000458L
	H15A0_SIZE_STATUS2_T            	cv2_size_status2;                 //0xc800045cL
	H15A0_SIZE_STATUS3_T            	cv2_size_status3;                 //0xc8000460L
	H15A0_SIZE_STATUS4_T            	cv2_size_status4;                 //0xc8000464L
	H15A0_PIX_SUM_CTRL0_T           	cv2_pix_sum_ctrl0;                //0xc8000468L
	H15A0_PIX_SUM_CTRL1_T           	cv2_pix_sum_ctrl1;                //0xc800046cL
	H15A0_PIX_SUM_STATUS0_T         	cv2_pix_sum_status0;              //0xc8000470L
	H15A0_PIX_SUM_STATUS1_T         	cv2_pix_sum_status1;              //0xc8000474L
	H15A0_PIX_SUM_STATUS2_T         	cv2_pix_sum_status2;              //0xc8000478L
	H15A0_SYNC_FIL_CTRL_T           	cv2_sync_fil_ctrl;                //0xc800047cL
	H15A0_SYNC_FIL_STATUS_T         	cv2_sync_fil_status;              //0xc8000480L
	UINT32                             	reserved09[3];                //0xc8000284L40xc800028cL
	H15A0_SYNC_REPEAT_CTRL_T        	cv2_sync_repeat_ctrl;             //0xc8000490L
	H15A0_SYNC_REPEAT_SIZE0_T       	cv2_sync_repeat_size0;            //0xc8000494L
	H15A0_SYNC_REPEAT_SIZE1_T       	cv2_sync_repeat_size1;            //0xc8000498L
	H15A0_SYNC_REPEAT_SIZE2_T       	cv2_sync_repeat_size2;            //0xc800049cL
	H15A0_SYNC_REPEAT_SIZE3_T       	cv2_sync_repeat_size3;            //0xc80004a0L
	H15A0_SYNC_REPEAT_SIZE4_T       	cv2_sync_repeat_size4;            //0xc80004a4L
	H15A0_SYNC_REPEAT_SIZE5_T       	cv2_sync_repeat_size5;            //0xc80004a8L
	UINT32                             	reserved10[5];                //0xc80002acL40xc80002bcL
	H15A0_SIZE_DETECT_CTRL_T        	cv2_size_detect_ctrl;             //0xc80004c0L
	H15A0_SIZE_DETECT_READ_T        	cv2_size_detect_read;             //0xc80004c4L
	H15A0_SIZE_STATUS0_T            	cv2_size_status0;                 //0xc80004c8L
	H15A0_SIZE_STATUS5_T            	cv2_size_status5;                 //0xc80004ccL
	H15A0_SIZE_STATUS6_T            	cv2_size_status6;                 //0xc80004d0L
	H15A0_SIZE_DETECT_CTRL1_T       	cv2_size_detect_ctrl1;            //0xc80004d4L
	H15A0_SIZE_DETECT_CTRL2_T       	cv2_size_detect_ctrl2;            //0xc80004d8L
	H15A0_PSEUDO_SYNC_CTRL0_T       	cv2_pseudo_sync_ctrl0;            //0xc80004dcL
	H15A0_PSEUDO_SYNC_CTRL1_T       	cv2_pseudo_sync_ctrl1;            //0xc80004e0L
	H15A0_PSEUDO_SYNC_CTRL2_T       	cv2_pseudo_sync_ctrl2;            //0xc80004e4L
	H15A0_PSEUDO_SYNC_CTRL3_T       	cv2_pseudo_sync_ctrl3;            //0xc80004e8L
	H15A0_PSEUDO_SYNC_CTRL4_T       	cv2_pseudo_sync_ctrl4;            //0xc80004ecL
	H15A0_SIZE_DETECT_CTRL3_T       	cv2_size_detect_ctrl3;            //0xc80004f0L

	UINT32                             	reserved11[3];
	
	H15A0_TOP_CTRL_T                	cv3_top_ctrl;                     //0xc8000500L
	H15A0_OFFSET_CTRL_T             	cv3_offset_ctrl;                  //0xc8000504L
	H15A0_SIZE_CTRL_T               	cv3_size_ctrl;                    //0xc8000508L
	H15A0_INT_SYNC_CTRL0_T          	cv3_int_sync_ctrl0;               //0xc800050cL
	H15A0_INT_SYNC_CTRL1_T          	cv3_int_sync_ctrl1;               //0xc8000510L
	UINT32                             	reserved12[4];                //0xc8000214L50xc8000220L
	H15A0_CSC_CTRL0_T               	cv3_csc_ctrl0;                    //0xc8000524L
	H15A0_CSC_CTRL1_T               	cv3_csc_ctrl1;                    //0xc8000528L
	H15A0_CSC_CTRL2_T               	cv3_csc_ctrl2;                    //0xc800052cL
	H15A0_CSC_CTRL3_T               	cv3_csc_ctrl3;                    //0xc8000530L
	H15A0_CSC_CTRL4_T               	cv3_csc_ctrl4;                    //0xc8000534L
	H15A0_CSC_CTRL5_T               	cv3_csc_ctrl5;                    //0xc8000538L
	H15A0_CSC_CTRL6_T               	cv3_csc_ctrl6;                    //0xc800053cL
	H15A0_CSC_CTRL7_T               	cv3_csc_ctrl7;                    //0xc8000540L
	H15A0_MISC_CTRL_T               	cv3_misc_ctrl;                    //0xc8000544L
	H15A0_OFFSET_READ_T             	cv3_offset_read;                  //0xc8000548L
	H15A0_PRE3D_CTRL0_T             	cv3_pre3d_ctrl0;                  //0xc800054cL
	H15A0_PRE3D_CTRL1_T             	cv3_pre3d_ctrl1;                  //0xc8000550L
	H15A0_PRE3D_CTRL2_T             	cv3_pre3d_ctrl2;                  //0xc8000554L
	H15A0_SIZE_STATUS1_T            	cv3_size_status1;                 //0xc8000558L
	H15A0_SIZE_STATUS2_T            	cv3_size_status2;                 //0xc800055cL
	H15A0_SIZE_STATUS3_T            	cv3_size_status3;                 //0xc8000560L
	H15A0_SIZE_STATUS4_T            	cv3_size_status4;                 //0xc8000564L
	H15A0_PIX_SUM_CTRL0_T           	cv3_pix_sum_ctrl0;                //0xc8000568L
	H15A0_PIX_SUM_CTRL1_T           	cv3_pix_sum_ctrl1;                //0xc800056cL
	H15A0_PIX_SUM_STATUS0_T         	cv3_pix_sum_status0;              //0xc8000570L
	H15A0_PIX_SUM_STATUS1_T         	cv3_pix_sum_status1;              //0xc8000574L
	H15A0_PIX_SUM_STATUS2_T         	cv3_pix_sum_status2;              //0xc8000578L
	H15A0_SYNC_FIL_CTRL_T           	cv3_sync_fil_ctrl;                //0xc800057cL
	H15A0_SYNC_FIL_STATUS_T         	cv3_sync_fil_status;              //0xc8000580L
	UINT32                             	reserved13[3];                //0xc8000284L50xc800028cL
	H15A0_SYNC_REPEAT_CTRL_T        	cv3_sync_repeat_ctrl;             //0xc8000590L
	H15A0_SYNC_REPEAT_SIZE0_T       	cv3_sync_repeat_size0;            //0xc8000594L
	H15A0_SYNC_REPEAT_SIZE1_T       	cv3_sync_repeat_size1;            //0xc8000598L
	H15A0_SYNC_REPEAT_SIZE2_T       	cv3_sync_repeat_size2;            //0xc800059cL
	H15A0_SYNC_REPEAT_SIZE3_T       	cv3_sync_repeat_size3;            //0xc80005a0L
	H15A0_SYNC_REPEAT_SIZE4_T       	cv3_sync_repeat_size4;            //0xc80005a4L
	H15A0_SYNC_REPEAT_SIZE5_T       	cv3_sync_repeat_size5;            //0xc80005a8L
	UINT32                             	reserved14[5];                //0xc80002acL50xc80002bcL
	H15A0_SIZE_DETECT_CTRL_T        	cv3_size_detect_ctrl;             //0xc80005c0L
	H15A0_SIZE_DETECT_READ_T        	cv3_size_detect_read;             //0xc80005c4L
	H15A0_SIZE_STATUS0_T            	cv3_size_status0;                 //0xc80005c8L
	H15A0_SIZE_STATUS5_T            	cv3_size_status5;                 //0xc80005ccL
	H15A0_SIZE_STATUS6_T            	cv3_size_status6;                 //0xc80005d0L
	H15A0_SIZE_DETECT_CTRL1_T       	cv3_size_detect_ctrl1;            //0xc80005d4L
	H15A0_SIZE_DETECT_CTRL2_T       	cv3_size_detect_ctrl2;            //0xc80005d8L
	H15A0_PSEUDO_SYNC_CTRL0_T       	cv3_pseudo_sync_ctrl0;            //0xc80005dcL
	H15A0_PSEUDO_SYNC_CTRL1_T       	cv3_pseudo_sync_ctrl1;            //0xc80005e0L
	H15A0_PSEUDO_SYNC_CTRL2_T       	cv3_pseudo_sync_ctrl2;            //0xc80005e4L
	H15A0_PSEUDO_SYNC_CTRL3_T       	cv3_pseudo_sync_ctrl3;            //0xc80005e8L
	H15A0_PSEUDO_SYNC_CTRL4_T       	cv3_pseudo_sync_ctrl4;            //0xc80005ecL
	H15A0_SIZE_DETECT_CTRL3_T       	cv3_size_detect_ctrl3;            //0xc80005f0L
} DE_CVI_REG_H15A0_T;
#endif
