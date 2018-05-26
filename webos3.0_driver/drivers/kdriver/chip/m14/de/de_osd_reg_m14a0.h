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

#ifndef _REG_OSD_M14A0_h
#define _REG_OSD_M14A0_h

/*-----------------------------------------------------------------------------
	0x5100 osd_status_mon_cfg ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_cfg              : 3;   //  0: 2
	UINT32                             : 1;   //     3 reserved
	UINT32 status_mon_osd_sel          : 2;   //  4: 5
} M14A0_OSD_STATUS_MON_CFG_T;

/*-----------------------------------------------------------------------------
	0x5104 osd_status_mon_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_ypos             : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 status_mon_xpos             : 12;  // 16:27
} M14A0_OSD_STATUS_MON_POS_T;

/*-----------------------------------------------------------------------------
	0x5108 osd_status_version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_version              : 32;  //  0:31
} M14A0_OSD_STATUS_VERSION_T;

/*-----------------------------------------------------------------------------
	0x510c osd_status_mixer_line ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mixer_line           : 16;  //  0:15
	UINT32 status_pic_init_count       : 8;   // 16:23
} M14A0_OSD_STATUS_MIXER_LINE_T;

/*-----------------------------------------------------------------------------
	0x5110 osd_status_mon_data0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data0            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA0_T;

/*-----------------------------------------------------------------------------
	0x5114 osd_status_mon_data1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data1            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA1_T;

/*-----------------------------------------------------------------------------
	0x5118 osd_status_mon_data2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data2            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA2_T;

/*-----------------------------------------------------------------------------
	0x511c osd_status_mon_data3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data3            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA3_T;

/*-----------------------------------------------------------------------------
	0x5120 osd_status_mon_data4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data4            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA4_T;

/*-----------------------------------------------------------------------------
	0x5124 osd_status_mon_data5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data5            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA5_T;

/*-----------------------------------------------------------------------------
	0x5128 osd_status_mon_data6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data6            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA6_T;

/*-----------------------------------------------------------------------------
	0x512c osd_status_mon_data7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data7            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA7_T;

/*-----------------------------------------------------------------------------
	0x5130 osd_status_mon_data8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data8            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA8_T;

/*-----------------------------------------------------------------------------
	0x5134 osd_status_mon_data9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data9            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATA9_T;

/*-----------------------------------------------------------------------------
	0x5138 osd_status_mon_dataa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_dataa            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATAA_T;

/*-----------------------------------------------------------------------------
	0x513c osd_status_mon_datab ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_datab            : 32;  //  0:31
} M14A0_OSD_STATUS_MON_DATAB_T;

/*-----------------------------------------------------------------------------
	0x5140 osd_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_disp_size_h          : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 common_disp_size_w          : 12;  // 16:27
} M14A0_OSD_COMMON_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
	0x5144 osd_common_ctrl_irq ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_ctrl_irq_clear_osd3  : 1;   //     0
	UINT32 common_ctrl_irq_clear_osd2  : 1;   //     1
	UINT32 common_ctrl_irq_clear_osd1  : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 common_ctrl_irq_clear_mixer : 1;   //     4
	UINT32                             : 11;  //  5:15 reserved
	UINT32 common_ctrl_irq_en_osd3     : 1;   //    16
	UINT32 common_ctrl_irq_en_osd2     : 1;   //    17
	UINT32 common_ctrl_irq_en_osd1     : 1;   //    18
	UINT32 common_ctrl_irq_en_osd0     : 1;   //    19
	UINT32 common_ctrl_irq_en_mixer    : 1;   //    20
} M14A0_OSD_COMMON_CTRL_IRQ_T;

/*-----------------------------------------------------------------------------
	0x5148 osd_common_ctrl_pic_init ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_pic_start_sel        : 1;   //     0
	UINT32                             : 3;   //  1: 3 reserved
	UINT32 common_pic_init_sel         : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 common_self_init_src_sel    : 1;   //     8
} M14A0_OSD_COMMON_CTRL_PIC_INIT_T;

/*-----------------------------------------------------------------------------
	0x514c osd_common_ctrl_pic_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_pic_init_timer       : 32;  //  0:31
} M14A0_OSD_COMMON_CTRL_PIC_TIMER_T;

/*-----------------------------------------------------------------------------
	0x5154 osd_common_ctrl_pq_path ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_pq_path_sel          : 2;   //  0: 1
	UINT32                             : 2;   //  2: 3 reserved
	UINT32 common_pq_path_en           : 1;   //     4
} M14A0_OSD_COMMON_CTRL_PQ_PATH_T;

/*-----------------------------------------------------------------------------
	0x5158 osd_common_plte_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_plte_sel             : 3;   //  0: 2
} M14A0_OSD_COMMON_PLTE_SEL_T;

/*-----------------------------------------------------------------------------
	0x515c osd_common_osd_valid_th ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_valid_area_th           : 8;   //  0: 7
	UINT32                             : 8;   //  8:15 reserved
	UINT32 osd_valid_area_en           : 1;   //    16
	UINT32                             : 3;   // 17:19 reserved
	UINT32 blend_sel_out_sep_tp_a      : 8;   // 20:27
	UINT32 blend_sel_out_sep_tp_ma     : 1;   //    28
} M14A0_OSD_COMMON_OSD_VALID_TH_T;

/*-----------------------------------------------------------------------------
	0x5160 osd_common_csc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_csc_osd_sel          : 2;   //  0: 1
	UINT32                             : 2;   //  2: 3 reserved
	UINT32 common_csc_all_bypass       : 1;   //     4
} M14A0_OSD_COMMON_CSC_CTRL_T;

/*-----------------------------------------------------------------------------
	0x5164 osd_common_csc_coef0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_csc0_datai           : 11;  //  0:10
	UINT32 common_csc0_rw_n            : 1;   //    11
	UINT32 common_csc0_addr            : 4;   // 12:15
	UINT32 common_csc0_bypass          : 1;   //    16
	UINT32                             : 3;   // 17:19 reserved
	UINT32 common_csc0_sel             : 1;   //    20
	UINT32                             : 3;   // 21:23 reserved
	UINT32 common_csc0_out_ctrl        : 3;   // 24:26
	UINT32                             : 1;   //    27 reserved
	UINT32 common_csc0_in_ctrl         : 3;   // 28:30
} M14A0_OSD_COMMON_CSC_COEF0_T;

/*-----------------------------------------------------------------------------
	0x5168 osd_common_csc_coef1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_csc1_datai           : 11;  //  0:10
	UINT32 common_csc1_rw_n            : 1;   //    11
	UINT32 common_csc1_addr            : 4;   // 12:15
	UINT32 common_csc1_bypass          : 1;   //    16
	UINT32                             : 3;   // 17:19 reserved
	UINT32 common_csc1_sel             : 1;   //    20
	UINT32                             : 3;   // 21:23 reserved
	UINT32 common_csc1_out_ctrl        : 3;   // 24:26
	UINT32                             : 1;   //    27 reserved
	UINT32 common_csc1_in_ctrl         : 3;   // 28:30
} M14A0_OSD_COMMON_CSC_COEF1_T;

/*-----------------------------------------------------------------------------
	0x5170 osd_common_sep_osd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_sep_osd_hdr_en       : 1;   //     0
	UINT32                             : 3;   //  1: 3 reserved
	UINT32 common_sep_osd_sync_sel     : 1;   //     4
	UINT32                             : 26;  //  5:30 reserved
	UINT32 common_sep_osd_bypass       : 1;   //    31
} M14A0_OSD_COMMON_SEP_OSD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x5174 osd_common_sep_osd_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_0               : 32;  //  0:31
} M14A0_OSD_COMMON_SEP_OSD_HDR0_T;

/*-----------------------------------------------------------------------------
	0x5178 osd_common_sep_osd_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_1               : 32;  //  0:31
} M14A0_OSD_COMMON_SEP_OSD_HDR1_T;

/*-----------------------------------------------------------------------------
	0x517c osd_common_sep_osd_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_2               : 32;  //  0:31
} M14A0_OSD_COMMON_SEP_OSD_HDR2_T;

/*-----------------------------------------------------------------------------
	0x5180 osd_common_sep_osd_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_3               : 32;  //  0:31
} M14A0_OSD_COMMON_SEP_OSD_HDR3_T;

/*-----------------------------------------------------------------------------
	0x5184 osd_common_sep_osd_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_4               : 32;  //  0:31
} M14A0_OSD_COMMON_SEP_OSD_HDR4_T;

/*-----------------------------------------------------------------------------
	0x5188 osd_common_sep_osd_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_5               : 32;  //  0:31
} M14A0_OSD_COMMON_SEP_OSD_HDR5_T;

/*-----------------------------------------------------------------------------
	0x518c osd_common_sep_osd_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_6               : 32;  //  0:31
} M14A0_OSD_COMMON_SEP_OSD_HDR6_T;

/*-----------------------------------------------------------------------------
	0x51a0 osd_mixer_ctrl_onoff ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_mixer_en               : 1;   //     0
	UINT32                             : 7;   //  1: 7 reserved
	UINT32 osd2_mixer_en               : 1;   //     8
	UINT32                             : 7;   //  9:15 reserved
	UINT32 osd1_mixer_en               : 1;   //    16
	UINT32                             : 7;   // 17:23 reserved
	UINT32 osd0_mixer_en               : 1;   //    24
} M14A0_OSD_MIXER_CTRL_ONOFF_T;

/*-----------------------------------------------------------------------------
	0x51a4 osd_mixer_ctrl_output ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_out_b                 : 8;   //  0: 7
	UINT32 mixer_out_g                 : 8;   //  8:15
	UINT32 mixer_out_r                 : 8;   // 16:23
	UINT32 mixer_out_fpr_mask_en       : 1;   //    24
	UINT32 mixer_out_fpr_mask_odd      : 1;   //    25
	UINT32                             : 2;   // 26:27 reserved
	UINT32 mixer_out_en                : 1;   //    28
	UINT32 mixer_out_valid_en          : 1;   //    29
	UINT32 mixer_out_vid_de_only       : 1;   //    30
	UINT32 mixer_out_vid_init_en       : 1;   //    31
} M14A0_OSD_MIXER_CTRL_OUTPUT_T;

/*-----------------------------------------------------------------------------
	0x51a8 osd_mixer_ctrl_awb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_mixer_awb_b            : 1;   //     0
	UINT32 osd3_mixer_awb_g            : 1;   //     1
	UINT32 osd3_mixer_awb_r            : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 osd2_mixer_awb_b            : 1;   //     4
	UINT32 osd2_mixer_awb_g            : 1;   //     5
	UINT32 osd2_mixer_awb_r            : 1;   //     6
	UINT32                             : 1;   //     7 reserved
	UINT32 osd1_mixer_awb_b            : 1;   //     8
	UINT32 osd1_mixer_awb_g            : 1;   //     9
	UINT32 osd1_mixer_awb_r            : 1;   //    10
	UINT32                             : 1;   //    11 reserved
	UINT32 osd0_mixer_awb_b            : 1;   //    12
	UINT32 osd0_mixer_awb_g            : 1;   //    13
	UINT32 osd0_mixer_awb_r            : 1;   //    14
} M14A0_OSD_MIXER_CTRL_AWB_T;

/*-----------------------------------------------------------------------------
	0x51ac osd_mixer_ctrl_sync ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_osd3_tp_en             : 1;   //     0
	UINT32 sync_osd3_tp_aa_en          : 1;   //     1
	UINT32                             : 2;   //  2: 3 reserved
	UINT32 sync_osd2_tp_en             : 1;   //     4
	UINT32 sync_osd2_tp_aa_en          : 1;   //     5
	UINT32                             : 2;   //  6: 7 reserved
	UINT32 sync_osd1_tp_en             : 1;   //     8
	UINT32 sync_osd1_tp_aa_en          : 1;   //     9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 sync_osd0_tp_en             : 1;   //    12
	UINT32 sync_osd0_tp_aa_en          : 1;   //    13
	UINT32                             : 2;   // 14:15 reserved
	UINT32 sync_tp_alpha               : 8;   // 16:23
	UINT32                             : 7;   // 24:30 reserved
	UINT32 sync_v_reverse              : 1;   //    31
} M14A0_OSD_MIXER_CTRL_SYNC_T;

/*-----------------------------------------------------------------------------
	0x51b0 osd_mixer_ctrl_blend ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 blend_sel_0v_dn             : 1;   //     0
	UINT32 blend_sel_0v_up             : 1;   //     1
	UINT32 blend_sel_10_dn             : 1;   //     2
	UINT32 blend_sel_10_up             : 1;   //     3
	UINT32                             : 2;   //  4: 5 reserved
	UINT32 blend_sel_31_dn             : 1;   //     6
	UINT32 blend_sel_31_up             : 1;   //     7
	UINT32 blend_div_dvdd              : 1;   //     8
	UINT32 blend_div_dvsr              : 1;   //     9
	UINT32 blend_sel_a10_dn            : 1;   //    10
	UINT32 blend_sel_a10_a             : 1;   //    11
	UINT32                             : 2;   // 12:13 reserved
	UINT32 blend_sel_a31_dn            : 1;   //    14
	UINT32 blend_sel_a31_a             : 1;   //    15
	UINT32 blend_sel_v3d_src           : 1;   //    16
	UINT32 blend_sel_v5d_src           : 1;   //    17
	UINT32 blend_sel_v7d_src           : 1;   //    18
	UINT32                             : 1;   //    19 reserved
	UINT32 blend_sel_vid_src           : 1;   //    20
	UINT32 blend_sel_31_dn_src         : 1;   //    21
	UINT32 blend_sel_div_pel_src       : 1;   //    22
	UINT32 blend_sel_out_sepc_src      : 1;   //    23
	UINT32 blend_sel_a3d_src           : 1;   //    24
	UINT32 blend_sel_a5d_src           : 1;   //    25
	UINT32                             : 1;   //    26 reserved
	UINT32 blend_sel_a31_z_3d_src      : 1;   //    27
	UINT32 blend_sel_out_vid           : 1;   //    28
	UINT32 blend_sel_out_sepc          : 1;   //    29
	UINT32 blend_sel_out_sepa          : 1;   //    30
	UINT32 blend_sel_out_sep_tp_en     : 1;   //    31
} M14A0_OSD_MIXER_CTRL_BLEND_T;

/*-----------------------------------------------------------------------------
	0x51c0 osd_mixer_ctrl_mux0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 layer_mux_3                 : 2;   //  0: 1
	UINT32                             : 2;   //  2: 3 reserved
	UINT32 layer_mux_2                 : 2;   //  4: 5
	UINT32                             : 2;   //  6: 7 reserved
	UINT32 layer_mux_1                 : 2;   //  8: 9
	UINT32                             : 2;   // 10:11 reserved
	UINT32 layer_mux_0                 : 2;   // 12:13
	UINT32                             : 2;   // 14:15 reserved
	UINT32 video_layer_alpha           : 8;   // 16:23
	UINT32                             : 4;   // 24:27 reserved
	UINT32 video_layer_off             : 1;   //    28
} M14A0_OSD_MIXER_CTRL_MUX0_T;

/*-----------------------------------------------------------------------------
	0x51c4 osd_mixer_ctrl_mux1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window0_en            : 1;   //     0
	UINT32                             : 15;  //  1:15 reserved
	UINT32 mixer_window0_alpha         : 8;   // 16:23
} M14A0_OSD_MIXER_CTRL_MUX1_T;

/*-----------------------------------------------------------------------------
	0x51c8 osd_mixer_ctrl_mux2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window0_ypos          : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 mixer_window0_xpos          : 12;  // 16:27
} M14A0_OSD_MIXER_CTRL_MUX2_T;

/*-----------------------------------------------------------------------------
	0x51cc osd_mixer_ctrl_mux3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window0_h             : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 mixer_window0_w             : 12;  // 16:27
} M14A0_OSD_MIXER_CTRL_MUX3_T;

/*-----------------------------------------------------------------------------
	0x51d0 osd_mixer_ctrl_mux4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window1_en            : 1;   //     0
	UINT32                             : 15;  //  1:15 reserved
	UINT32 mixer_window1_alpha         : 8;   // 16:23
} M14A0_OSD_MIXER_CTRL_MUX4_T;

/*-----------------------------------------------------------------------------
	0x51d4 osd_mixer_ctrl_mux5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window1_ypos          : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 mixer_window1_xpos          : 12;  // 16:27
} M14A0_OSD_MIXER_CTRL_MUX5_T;

/*-----------------------------------------------------------------------------
	0x51d8 osd_mixer_ctrl_mux6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window1_h             : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 mixer_window1_w             : 12;  // 16:27
} M14A0_OSD_MIXER_CTRL_MUX6_T;

/*-----------------------------------------------------------------------------
	0x51dc osd_mixer_ctrl_mux7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window2_en            : 1;   //     0
	UINT32                             : 15;  //  1:15 reserved
	UINT32 mixer_window2_alpha         : 8;   // 16:23
} M14A0_OSD_MIXER_CTRL_MUX7_T;

/*-----------------------------------------------------------------------------
	0x51e0 osd_mixer_ctrl_mux8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window2_ypos          : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 mixer_window2_xpos          : 12;  // 16:27
} M14A0_OSD_MIXER_CTRL_MUX8_T;

/*-----------------------------------------------------------------------------
	0x51e4 osd_mixer_ctrl_mux9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window2_h             : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 mixer_window2_w             : 12;  // 16:27
} M14A0_OSD_MIXER_CTRL_MUX9_T;

/*-----------------------------------------------------------------------------
	0x5200 osd0_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_en                     : 1;   //     0
	UINT32                             : 3;   //  1: 3 reserved
	UINT32 osd0_hdr_src_sel            : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd0_v_reverse_en           : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd0_s3d_en                 : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd0_manual_line_inc        : 2;   // 16:17
	UINT32                             : 2;   // 18:19 reserved
	UINT32 osd0_manual_line_inc_en     : 1;   //    20
	UINT32                             : 3;   // 21:23 reserved
	UINT32 osd0_yuyv_mode_en           : 1;   //    24
} M14A0_OSD0_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x5204 osd0_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_base_addr              : 32;  //  0:31
} M14A0_OSD0_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5208 osd0_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_bbs                    : 1;   //     0
	UINT32 osd0_bqws                   : 1;   //     1
	UINT32 osd0_bhws                   : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 osd0_pbs                    : 1;   //     4
	UINT32 osd0_pqws                   : 1;   //     5
	UINT32 osd0_phws                   : 1;   //     6
	UINT32                             : 1;   //     7 reserved
	UINT32 osd0_hbs                    : 1;   //     8
	UINT32 osd0_hqws                   : 1;   //     9
	UINT32 osd0_hhws                   : 1;   //    10
} M14A0_OSD0_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x520c osd0_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_lfsr_seed              : 19;  //  0:18
	UINT32 osd0_pure_ck                : 1;   //    19
	UINT32 osd0_round                  : 3;   // 20:22
	UINT32                             : 1;   //    23 reserved
	UINT32 osd0_ch_blue                : 2;   // 24:25
	UINT32 osd0_ch_green               : 2;   // 26:27
	UINT32 osd0_ch_red                 : 2;   // 28:29
	UINT32 osd0_ch_alpha               : 2;   // 30:31
} M14A0_OSD0_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x5210 osd0_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_sc_bc_sa               : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en      : 1;   //     2
	UINT32 osd0_sc_bilinear            : 1;   //     3
	UINT32 osd0_sc_vdouble             : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd0_sc_sampling_mode       : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd0_sc_bnd_mode            : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd0_sc_h_phase_off         : 6;   // 16:21
	UINT32                             : 2;   // 22:23 reserved
	UINT32 osd0_sc_v_phase_off         : 6;   // 24:29
	UINT32 osd0_odd_width_en           : 1;   //    30
	UINT32 osd0_fifo_en                : 1;   //    31
} M14A0_OSD0_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x5214 osd0_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_r_3d_disparity_value   : 8;   //  0: 7
	UINT32 osd0_r_3d_disparity_sign    : 1;   //     8
	UINT32                             : 7;   //  9:15 reserved
	UINT32 osd0_l_3d_disparity_value   : 8;   // 16:23
	UINT32 osd0_l_3d_disparity_sign    : 1;   //    24
} M14A0_OSD0_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x5218 osd0_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_ctrl_3d_blank          : 32;  //  0:31
} M14A0_OSD0_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x5220 osd0_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ypos               : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd0_hdr_xpos               : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd0_hdr_color_key_en       : 1;   //    31
} M14A0_OSD0_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x5224 osd0_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_h_mem              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd0_hdr_w_mem              : 12;  // 16:27
} M14A0_OSD0_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x5228 osd0_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_h_out              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd0_hdr_w_out              : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd0_hdr_pixel_order        : 1;   //    31
} M14A0_OSD0_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x522c osd0_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_wpl                : 16;  //  0:15
	UINT32 osd0_hdr_global_alpha       : 8;   // 16:23
	UINT32 osd0_hdr_format             : 4;   // 24:27
	UINT32 osd0_hdr_depth              : 3;   // 28:30
	UINT32 osd0_hdr_global_alpha_en    : 1;   //    31
} M14A0_OSD0_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x5230 osd0_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_color_key          : 32;  //  0:31
} M14A0_OSD0_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x5234 osd0_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ptr_plte           : 32;  //  0:31
} M14A0_OSD0_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x5238 osd0_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ptr_bmp            : 32;  //  0:31
} M14A0_OSD0_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
	0x5240 osd1_ctrl_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_en                     : 1;   //     0
	UINT32                             : 3;   //  1: 3 reserved
	UINT32 osd1_hdr_src_sel            : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd1_v_reverse_en           : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd1_s3d_en                 : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd1_manual_line_inc        : 2;   // 16:17
	UINT32                             : 2;   // 18:19 reserved
	UINT32 osd1_manual_line_inc_en     : 1;   //    20
	UINT32                             : 3;   // 21:23 reserved
	UINT32 osd1_yuyv_mode_en           : 1;   //    24
} M14A0_OSD1_CTRL_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x5244 osd1_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_base_addr              : 32;  //  0:31
} M14A0_OSD1_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5248 osd1_ctrl_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_bbs                    : 1;   //     0
	UINT32 osd1_bqws                   : 1;   //     1
	UINT32 osd1_bhws                   : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 osd1_pbs                    : 1;   //     4
	UINT32 osd1_pqws                   : 1;   //     5
	UINT32 osd1_phws                   : 1;   //     6
	UINT32                             : 1;   //     7 reserved
	UINT32 osd1_hbs                    : 1;   //     8
	UINT32 osd1_hqws                   : 1;   //     9
	UINT32 osd1_hhws                   : 1;   //    10
} M14A0_OSD1_CTRL_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x524c osd1_ctrl_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_lfsr_seed              : 19;  //  0:18
	UINT32 osd1_pure_ck                : 1;   //    19
	UINT32 osd1_round                  : 3;   // 20:22
	UINT32                             : 1;   //    23 reserved
	UINT32 osd1_ch_blue                : 2;   // 24:25
	UINT32 osd1_ch_green               : 2;   // 26:27
	UINT32 osd1_ch_red                 : 2;   // 28:29
	UINT32 osd1_ch_alpha               : 2;   // 30:31
} M14A0_OSD1_CTRL_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x5250 osd1_ctrl_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_sc_bc_sa               : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en      : 1;   //     2
	UINT32 osd1_sc_bilinear            : 1;   //     3
	UINT32 osd1_sc_vdouble             : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd1_sc_sampling_mode       : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd1_sc_bnd_mode            : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd1_sc_h_phase_off         : 6;   // 16:21
	UINT32                             : 2;   // 22:23 reserved
	UINT32 osd1_sc_v_phase_off         : 6;   // 24:29
	UINT32 osd1_odd_width_en           : 1;   //    30
	UINT32 osd1_fifo_en                : 1;   //    31
} M14A0_OSD1_CTRL_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x5254 osd1_ctrl_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_r_3d_disparity_value   : 8;   //  0: 7
	UINT32 osd1_r_3d_disparity_sign    : 1;   //     8
	UINT32                             : 7;   //  9:15 reserved
	UINT32 osd1_l_3d_disparity_value   : 8;   // 16:23
	UINT32 osd1_l_3d_disparity_sign    : 1;   //    24
} M14A0_OSD1_CTRL_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x5258 osd1_ctrl_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_ctrl_3d_blank          : 32;  //  0:31
} M14A0_OSD1_CTRL_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x5260 osd1_ctrl_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ypos               : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd1_hdr_xpos               : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd1_hdr_color_key_en       : 1;   //    31
} M14A0_OSD1_CTRL_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x5264 osd1_ctrl_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_h_mem              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd1_hdr_w_mem              : 12;  // 16:27
} M14A0_OSD1_CTRL_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x5268 osd1_ctrl_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_h_out              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd1_hdr_w_out              : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd1_hdr_pixel_order        : 1;   //    31
} M14A0_OSD1_CTRL_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x526c osd1_ctrl_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_wpl                : 16;  //  0:15
	UINT32 osd1_hdr_global_alpha       : 8;   // 16:23
	UINT32 osd1_hdr_format             : 4;   // 24:27
	UINT32 osd1_hdr_depth              : 3;   // 28:30
	UINT32 osd1_hdr_global_alpha_en    : 1;   //    31
} M14A0_OSD1_CTRL_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x5270 osd1_ctrl_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_color_key          : 32;  //  0:31
} M14A0_OSD1_CTRL_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x5274 osd1_ctrl_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ptr_plte           : 32;  //  0:31
} M14A0_OSD1_CTRL_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x5278 osd1_ctrl_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ptr_bmp            : 32;  //  0:31
} M14A0_OSD1_CTRL_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
	0x5280 osd2_ctrl_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_en                     : 1;   //     0
	UINT32                             : 3;   //  1: 3 reserved
	UINT32 osd2_hdr_src_sel            : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd2_v_reverse_en           : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd2_s3d_en                 : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd2_manual_line_inc        : 2;   // 16:17
	UINT32                             : 2;   // 18:19 reserved
	UINT32 osd2_manual_line_inc_en     : 1;   //    20
	UINT32                             : 3;   // 21:23 reserved
	UINT32 osd2_yuyv_mode_en           : 1;   //    24
} M14A0_OSD2_CTRL_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x5284 osd2_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_base_addr              : 32;  //  0:31
} M14A0_OSD2_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5288 osd2_ctrl_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_bbs                    : 1;   //     0
	UINT32 osd2_bqws                   : 1;   //     1
	UINT32 osd2_bhws                   : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 osd2_pbs                    : 1;   //     4
	UINT32 osd2_pqws                   : 1;   //     5
	UINT32 osd2_phws                   : 1;   //     6
	UINT32                             : 1;   //     7 reserved
	UINT32 osd2_hbs                    : 1;   //     8
	UINT32 osd2_hqws                   : 1;   //     9
	UINT32 osd2_hhws                   : 1;   //    10
} M14A0_OSD2_CTRL_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x528c osd2_ctrl_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_lfsr_seed              : 19;  //  0:18
	UINT32 osd2_pure_ck                : 1;   //    19
	UINT32 osd2_round                  : 3;   // 20:22
	UINT32                             : 1;   //    23 reserved
	UINT32 osd2_ch_blue                : 2;   // 24:25
	UINT32 osd2_ch_green               : 2;   // 26:27
	UINT32 osd2_ch_red                 : 2;   // 28:29
	UINT32 osd2_ch_alpha               : 2;   // 30:31
} M14A0_OSD2_CTRL_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x5290 osd2_ctrl_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_sc_bc_sa               : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en      : 1;   //     2
	UINT32 osd2_sc_bilinear            : 1;   //     3
	UINT32 osd2_sc_vdouble             : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd2_sc_sampling_mode       : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd2_sc_bnd_mode            : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd2_sc_h_phase_off         : 6;   // 16:21
	UINT32                             : 2;   // 22:23 reserved
	UINT32 osd2_sc_v_phase_off         : 6;   // 24:29
	UINT32 osd2_odd_width_en           : 1;   //    30
	UINT32 osd2_fifo_en                : 1;   //    31
} M14A0_OSD2_CTRL_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x5294 osd2_ctrl_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_r_3d_disparity_value   : 8;   //  0: 7
	UINT32 osd2_r_3d_disparity_sign    : 1;   //     8
	UINT32                             : 7;   //  9:15 reserved
	UINT32 osd2_l_3d_disparity_value   : 8;   // 16:23
	UINT32 osd2_l_3d_disparity_sign    : 1;   //    24
} M14A0_OSD2_CTRL_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x5298 osd2_ctrl_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_ctrl_3d_blank          : 32;  //  0:31
} M14A0_OSD2_CTRL_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x52a0 osd2_ctrl_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ypos               : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd2_hdr_xpos               : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd2_hdr_color_key_en       : 1;   //    31
} M14A0_OSD2_CTRL_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x52a4 osd2_ctrl_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_h_mem              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd2_hdr_w_mem              : 12;  // 16:27
} M14A0_OSD2_CTRL_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x52a8 osd2_ctrl_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_h_out              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd2_hdr_w_out              : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd2_hdr_pixel_order        : 1;   //    31
} M14A0_OSD2_CTRL_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x52ac osd2_ctrl_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_wpl                : 16;  //  0:15
	UINT32 osd2_hdr_global_alpha       : 8;   // 16:23
	UINT32 osd2_hdr_format             : 4;   // 24:27
	UINT32 osd2_hdr_depth              : 3;   // 28:30
	UINT32 osd2_hdr_global_alpha_en    : 1;   //    31
} M14A0_OSD2_CTRL_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x52b0 osd2_ctrl_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_color_key          : 32;  //  0:31
} M14A0_OSD2_CTRL_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x52b4 osd2_ctrl_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ptr_plte           : 32;  //  0:31
} M14A0_OSD2_CTRL_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x52b8 osd2_ctrl_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ptr_bmp            : 32;  //  0:31
} M14A0_OSD2_CTRL_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
	0x52c0 osd3_ctrl_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_en                     : 1;   //     0
	UINT32                             : 3;   //  1: 3 reserved
	UINT32 osd3_hdr_src_sel            : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd3_v_reverse_en           : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd3_s3d_en                 : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd3_manual_line_inc        : 2;   // 16:17
	UINT32                             : 2;   // 18:19 reserved
	UINT32 osd3_manual_line_inc_en     : 1;   //    20
	UINT32                             : 3;   // 21:23 reserved
	UINT32 osd3_yuyv_mode_en           : 1;   //    24
} M14A0_OSD3_CTRL_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x52c4 osd3_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_base_addr              : 32;  //  0:31
} M14A0_OSD3_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x52c8 osd3_ctrl_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_bbs                    : 1;   //     0
	UINT32 osd3_bqws                   : 1;   //     1
	UINT32 osd3_bhws                   : 1;   //     2
	UINT32                             : 1;   //     3 reserved
	UINT32 osd3_pbs                    : 1;   //     4
	UINT32 osd3_pqws                   : 1;   //     5
	UINT32 osd3_phws                   : 1;   //     6
	UINT32                             : 1;   //     7 reserved
	UINT32 osd3_hbs                    : 1;   //     8
	UINT32 osd3_hqws                   : 1;   //     9
	UINT32 osd3_hhws                   : 1;   //    10
} M14A0_OSD3_CTRL_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x52cc osd3_ctrl_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_lfsr_seed              : 19;  //  0:18
	UINT32 osd3_pure_ck                : 1;   //    19
	UINT32 osd3_round                  : 3;   // 20:22
	UINT32                             : 1;   //    23 reserved
	UINT32 osd3_ch_blue                : 2;   // 24:25
	UINT32 osd3_ch_green               : 2;   // 26:27
	UINT32 osd3_ch_red                 : 2;   // 28:29
	UINT32 osd3_ch_alpha               : 2;   // 30:31
} M14A0_OSD3_CTRL_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x52d0 osd3_ctrl_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_sc_bc_sa               : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en      : 1;   //     2
	UINT32 osd3_sc_bilinear            : 1;   //     3
	UINT32 osd3_sc_vdouble             : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 osd3_sc_sampling_mode       : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 osd3_sc_bnd_mode            : 1;   //    12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 osd3_sc_h_phase_off         : 6;   // 16:21
	UINT32                             : 2;   // 22:23 reserved
	UINT32 osd3_sc_v_phase_off         : 6;   // 24:29
	UINT32 osd3_odd_width_en           : 1;   //    30
	UINT32 osd3_fifo_en                : 1;   //    31
} M14A0_OSD3_CTRL_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x52d4 osd3_ctrl_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_r_3d_disparity_value   : 8;   //  0: 7
	UINT32 osd3_r_3d_disparity_sign    : 1;   //     8
	UINT32                             : 7;   //  9:15 reserved
	UINT32 osd3_l_3d_disparity_value   : 8;   // 16:23
	UINT32 osd3_l_3d_disparity_sign    : 1;   //    24
} M14A0_OSD3_CTRL_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x52d8 osd3_ctrl_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_ctrl_3d_blank          : 32;  //  0:31
} M14A0_OSD3_CTRL_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x52e0 osd3_ctrl_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ypos               : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd3_hdr_xpos               : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd3_hdr_color_key_en       : 1;   //    31
} M14A0_OSD3_CTRL_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x52e4 osd3_ctrl_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_h_mem              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd3_hdr_w_mem              : 12;  // 16:27
} M14A0_OSD3_CTRL_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x52e8 osd3_ctrl_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_h_out              : 12;  //  0:11
	UINT32                             : 4;   // 12:15 reserved
	UINT32 osd3_hdr_w_out              : 12;  // 16:27
	UINT32                             : 3;   // 28:30 reserved
	UINT32 osd3_hdr_pixel_order        : 1;   //    31
} M14A0_OSD3_CTRL_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x52ec osd3_ctrl_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_wpl                : 16;  //  0:15
	UINT32 osd3_hdr_global_alpha       : 8;   // 16:23
	UINT32 osd3_hdr_format             : 4;   // 24:27
	UINT32 osd3_hdr_depth              : 3;   // 28:30
	UINT32 osd3_hdr_global_alpha_en    : 1;   //    31
} M14A0_OSD3_CTRL_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x52f0 osd3_ctrl_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_color_key          : 32;  //  0:31
} M14A0_OSD3_CTRL_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x52f4 osd3_ctrl_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ptr_plte           : 32;  //  0:31
} M14A0_OSD3_CTRL_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x52f8 osd3_ctrl_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ptr_bmp            : 32;  //  0:31
} M14A0_OSD3_CTRL_CTRL_HDR6_T;

typedef struct {
	M14A0_OSD_STATUS_MON_CFG_T              osd_status_mon_cfg;                      // 0x5100
	M14A0_OSD_STATUS_MON_POS_T              osd_status_mon_pos;                      // 0x5104
	M14A0_OSD_STATUS_VERSION_T              osd_status_version;                      // 0x5108
	M14A0_OSD_STATUS_MIXER_LINE_T           osd_status_mixer_line;                   // 0x510c
	M14A0_OSD_STATUS_MON_DATA0_T            osd_status_mon_data0;                    // 0x5110
	M14A0_OSD_STATUS_MON_DATA1_T            osd_status_mon_data1;                    // 0x5114
	M14A0_OSD_STATUS_MON_DATA2_T            osd_status_mon_data2;                    // 0x5118
	M14A0_OSD_STATUS_MON_DATA3_T            osd_status_mon_data3;                    // 0x511c
	M14A0_OSD_STATUS_MON_DATA4_T            osd_status_mon_data4;                    // 0x5120
	M14A0_OSD_STATUS_MON_DATA5_T            osd_status_mon_data5;                    // 0x5124
	M14A0_OSD_STATUS_MON_DATA6_T            osd_status_mon_data6;                    // 0x5128
	M14A0_OSD_STATUS_MON_DATA7_T            osd_status_mon_data7;                    // 0x512c
	M14A0_OSD_STATUS_MON_DATA8_T            osd_status_mon_data8;                    // 0x5130
	M14A0_OSD_STATUS_MON_DATA9_T            osd_status_mon_data9;                    // 0x5134
	M14A0_OSD_STATUS_MON_DATAA_T            osd_status_mon_dataa;                    // 0x5138
	M14A0_OSD_STATUS_MON_DATAB_T            osd_status_mon_datab;                    // 0x513c
	M14A0_OSD_COMMON_DISP_SIZE_T            osd_common_disp_size;                    // 0x5140
	M14A0_OSD_COMMON_CTRL_IRQ_T             osd_common_ctrl_irq;                     // 0x5144
	M14A0_OSD_COMMON_CTRL_PIC_INIT_T        osd_common_ctrl_pic_init;                // 0x5148
	M14A0_OSD_COMMON_CTRL_PIC_TIMER_T       osd_common_ctrl_pic_timer;               // 0x514c
	UINT32                                  reserved0;                               // 0x5150
	M14A0_OSD_COMMON_CTRL_PQ_PATH_T         osd_common_ctrl_pq_path;                 // 0x5154
	M14A0_OSD_COMMON_PLTE_SEL_T             osd_common_plte_sel;                     // 0x5158
	M14A0_OSD_COMMON_OSD_VALID_TH_T         osd_common_osd_valid_th;                 // 0x515c
	M14A0_OSD_COMMON_CSC_CTRL_T             osd_common_csc_ctrl;                     // 0x5160
	M14A0_OSD_COMMON_CSC_COEF0_T            osd_common_csc_coef0;                    // 0x5164
	M14A0_OSD_COMMON_CSC_COEF1_T            osd_common_csc_coef1;                    // 0x5168
	UINT32                                  reserved1;                               // 0x516c
	M14A0_OSD_COMMON_SEP_OSD_CTRL_T         osd_common_sep_osd_ctrl;                 // 0x5170
	M14A0_OSD_COMMON_SEP_OSD_HDR0_T         osd_common_sep_osd_hdr0;                 // 0x5174
	M14A0_OSD_COMMON_SEP_OSD_HDR1_T         osd_common_sep_osd_hdr1;                 // 0x5178
	M14A0_OSD_COMMON_SEP_OSD_HDR2_T         osd_common_sep_osd_hdr2;                 // 0x517c
	M14A0_OSD_COMMON_SEP_OSD_HDR3_T         osd_common_sep_osd_hdr3;                 // 0x5180
	M14A0_OSD_COMMON_SEP_OSD_HDR4_T         osd_common_sep_osd_hdr4;                 // 0x5184
	M14A0_OSD_COMMON_SEP_OSD_HDR5_T         osd_common_sep_osd_hdr5;                 // 0x5188
	M14A0_OSD_COMMON_SEP_OSD_HDR6_T         osd_common_sep_osd_hdr6;                 // 0x518c
	UINT32                                  reserved2[4];                            // 0x5190~0x519c
	M14A0_OSD_MIXER_CTRL_ONOFF_T            osd_mixer_ctrl_onoff;                    // 0x51a0
	M14A0_OSD_MIXER_CTRL_OUTPUT_T           osd_mixer_ctrl_output;                   // 0x51a4
	M14A0_OSD_MIXER_CTRL_AWB_T              osd_mixer_ctrl_awb;                      // 0x51a8
	M14A0_OSD_MIXER_CTRL_SYNC_T             osd_mixer_ctrl_sync;                     // 0x51ac
	M14A0_OSD_MIXER_CTRL_BLEND_T            osd_mixer_ctrl_blend;                    // 0x51b0
	UINT32                                  reserved3[3];                            // 0x51b4~0x51bc
	M14A0_OSD_MIXER_CTRL_MUX0_T             osd_mixer_ctrl_mux0;                     // 0x51c0
	M14A0_OSD_MIXER_CTRL_MUX1_T             osd_mixer_ctrl_mux1;                     // 0x51c4
	M14A0_OSD_MIXER_CTRL_MUX2_T             osd_mixer_ctrl_mux2;                     // 0x51c8
	M14A0_OSD_MIXER_CTRL_MUX3_T             osd_mixer_ctrl_mux3;                     // 0x51cc
	M14A0_OSD_MIXER_CTRL_MUX4_T             osd_mixer_ctrl_mux4;                     // 0x51d0
	M14A0_OSD_MIXER_CTRL_MUX5_T             osd_mixer_ctrl_mux5;                     // 0x51d4
	M14A0_OSD_MIXER_CTRL_MUX6_T             osd_mixer_ctrl_mux6;                     // 0x51d8
	M14A0_OSD_MIXER_CTRL_MUX7_T             osd_mixer_ctrl_mux7;                     // 0x51dc
	M14A0_OSD_MIXER_CTRL_MUX8_T             osd_mixer_ctrl_mux8;                     // 0x51e0
	M14A0_OSD_MIXER_CTRL_MUX9_T             osd_mixer_ctrl_mux9;                     // 0x51e4
	UINT32                                  reserved4[6];                            // 0x51e8~0x51fc
	M14A0_OSD0_CTRL_MAIN_T                  osd0_ctrl_main;                          // 0x5200
	M14A0_OSD0_BASE_ADDR_T                  osd0_base_addr;                          // 0x5204
	M14A0_OSD0_CTRL_SWAP_T                  osd0_ctrl_swap;                          // 0x5208
	M14A0_OSD0_CTRL_PDEC_T                  osd0_ctrl_pdec;                          // 0x520c
	M14A0_OSD0_CTRL_SCALER_T                osd0_ctrl_scaler;                        // 0x5210
	M14A0_OSD0_CTRL_3D_DISPARITY_T          osd0_ctrl_3d_disparity;                  // 0x5214
	M14A0_OSD0_CTRL_3D_BLANK_T              osd0_ctrl_3d_blank;                      // 0x5218
	UINT32                                  reserved5;                               // 0x521c
	M14A0_OSD0_CTRL_HDR0_T                  osd0_ctrl_hdr0;                          // 0x5220
	M14A0_OSD0_CTRL_HDR1_T                  osd0_ctrl_hdr1;                          // 0x5224
	M14A0_OSD0_CTRL_HDR2_T                  osd0_ctrl_hdr2;                          // 0x5228
	M14A0_OSD0_CTRL_HDR3_T                  osd0_ctrl_hdr3;                          // 0x522c
	M14A0_OSD0_CTRL_HDR4_T                  osd0_ctrl_hdr4;                          // 0x5230
	M14A0_OSD0_CTRL_HDR5_T                  osd0_ctrl_hdr5;                          // 0x5234
	M14A0_OSD0_CTRL_HDR6_T                  osd0_ctrl_hdr6;                          // 0x5238
	UINT32                                  reserved6;                               // 0x523c
	M14A0_OSD1_CTRL_CTRL_MAIN_T             osd1_ctrl_ctrl_main;                     // 0x5240
	M14A0_OSD1_CTRL_BASE_ADDR_T             osd1_ctrl_base_addr;                     // 0x5244
	M14A0_OSD1_CTRL_CTRL_SWAP_T             osd1_ctrl_ctrl_swap;                     // 0x5248
	M14A0_OSD1_CTRL_CTRL_PDEC_T             osd1_ctrl_ctrl_pdec;                     // 0x524c
	M14A0_OSD1_CTRL_CTRL_SCALER_T           osd1_ctrl_ctrl_scaler;                   // 0x5250
	M14A0_OSD1_CTRL_CTRL_3D_DISPARITY_T     osd1_ctrl_ctrl_3d_disparity;             // 0x5254
	M14A0_OSD1_CTRL_CTRL_3D_BLANK_T         osd1_ctrl_ctrl_3d_blank;                 // 0x5258
	UINT32                                  reserved7;                               // 0x525c
	M14A0_OSD1_CTRL_CTRL_HDR0_T             osd1_ctrl_ctrl_hdr0;                     // 0x5260
	M14A0_OSD1_CTRL_CTRL_HDR1_T             osd1_ctrl_ctrl_hdr1;                     // 0x5264
	M14A0_OSD1_CTRL_CTRL_HDR2_T             osd1_ctrl_ctrl_hdr2;                     // 0x5268
	M14A0_OSD1_CTRL_CTRL_HDR3_T             osd1_ctrl_ctrl_hdr3;                     // 0x526c
	M14A0_OSD1_CTRL_CTRL_HDR4_T             osd1_ctrl_ctrl_hdr4;                     // 0x5270
	M14A0_OSD1_CTRL_CTRL_HDR5_T             osd1_ctrl_ctrl_hdr5;                     // 0x5274
	M14A0_OSD1_CTRL_CTRL_HDR6_T             osd1_ctrl_ctrl_hdr6;                     // 0x5278
	UINT32                                  reserved8;                               // 0x527c
	M14A0_OSD2_CTRL_CTRL_MAIN_T             osd2_ctrl_ctrl_main;                     // 0x5280
	M14A0_OSD2_CTRL_BASE_ADDR_T             osd2_ctrl_base_addr;                     // 0x5284
	M14A0_OSD2_CTRL_CTRL_SWAP_T             osd2_ctrl_ctrl_swap;                     // 0x5288
	M14A0_OSD2_CTRL_CTRL_PDEC_T             osd2_ctrl_ctrl_pdec;                     // 0x528c
	M14A0_OSD2_CTRL_CTRL_SCALER_T           osd2_ctrl_ctrl_scaler;                   // 0x5290
	M14A0_OSD2_CTRL_CTRL_3D_DISPARITY_T     osd2_ctrl_ctrl_3d_disparity;             // 0x5294
	M14A0_OSD2_CTRL_CTRL_3D_BLANK_T         osd2_ctrl_ctrl_3d_blank;                 // 0x5298
	UINT32                                  reserved9;                               // 0x529c
	M14A0_OSD2_CTRL_CTRL_HDR0_T             osd2_ctrl_ctrl_hdr0;                     // 0x52a0
	M14A0_OSD2_CTRL_CTRL_HDR1_T             osd2_ctrl_ctrl_hdr1;                     // 0x52a4
	M14A0_OSD2_CTRL_CTRL_HDR2_T             osd2_ctrl_ctrl_hdr2;                     // 0x52a8
	M14A0_OSD2_CTRL_CTRL_HDR3_T             osd2_ctrl_ctrl_hdr3;                     // 0x52ac
	M14A0_OSD2_CTRL_CTRL_HDR4_T             osd2_ctrl_ctrl_hdr4;                     // 0x52b0
	M14A0_OSD2_CTRL_CTRL_HDR5_T             osd2_ctrl_ctrl_hdr5;                     // 0x52b4
	M14A0_OSD2_CTRL_CTRL_HDR6_T             osd2_ctrl_ctrl_hdr6;                     // 0x52b8
	UINT32                                  reserved10;                              // 0x52bc
	M14A0_OSD3_CTRL_CTRL_MAIN_T             osd3_ctrl_ctrl_main;                     // 0x52c0
	M14A0_OSD3_CTRL_BASE_ADDR_T             osd3_ctrl_base_addr;                     // 0x52c4
	M14A0_OSD3_CTRL_CTRL_SWAP_T             osd3_ctrl_ctrl_swap;                     // 0x52c8
	M14A0_OSD3_CTRL_CTRL_PDEC_T             osd3_ctrl_ctrl_pdec;                     // 0x52cc
	M14A0_OSD3_CTRL_CTRL_SCALER_T           osd3_ctrl_ctrl_scaler;                   // 0x52d0
	M14A0_OSD3_CTRL_CTRL_3D_DISPARITY_T     osd3_ctrl_ctrl_3d_disparity;             // 0x52d4
	M14A0_OSD3_CTRL_CTRL_3D_BLANK_T         osd3_ctrl_ctrl_3d_blank;                 // 0x52d8
	UINT32                                  reserved11;                              // 0x52dc
	M14A0_OSD3_CTRL_CTRL_HDR0_T             osd3_ctrl_ctrl_hdr0;                     // 0x52e0
	M14A0_OSD3_CTRL_CTRL_HDR1_T             osd3_ctrl_ctrl_hdr1;                     // 0x52e4
	M14A0_OSD3_CTRL_CTRL_HDR2_T             osd3_ctrl_ctrl_hdr2;                     // 0x52e8
	M14A0_OSD3_CTRL_CTRL_HDR3_T             osd3_ctrl_ctrl_hdr3;                     // 0x52ec
	M14A0_OSD3_CTRL_CTRL_HDR4_T             osd3_ctrl_ctrl_hdr4;                     // 0x52f0
	M14A0_OSD3_CTRL_CTRL_HDR5_T             osd3_ctrl_ctrl_hdr5;                     // 0x52f4
	M14A0_OSD3_CTRL_CTRL_HDR6_T             osd3_ctrl_ctrl_hdr6;                     // 0x52f8
} DE_OSD_REG_M14A0_T;

#endif
