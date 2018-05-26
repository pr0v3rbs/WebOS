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

#ifndef _REG_OSD_H14A0_h
#define _REG_OSD_H14A0_h

/*-----------------------------------------------------------------------------
	0x5100 osd_status_mon_cfg ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_cfg                      : 3;   //  0: 2
	UINT32                                     : 1;   //     3 reserved
	UINT32 status_mon_osd_sel                  : 2;   //  4: 5
} H14A0_OSD_STATUS_MON_CFG_T;

/*-----------------------------------------------------------------------------
	0x5104 osd_status_mon_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_ypos                     : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 status_mon_xpos                     : 12;  // 16:27
} H14A0_OSD_STATUS_MON_POS_T;

/*-----------------------------------------------------------------------------
	0x5108 osd_status_version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_version                      : 32;  //  0:31
} H14A0_OSD_STATUS_VERSION_T;

/*-----------------------------------------------------------------------------
	0x510c osd_status_mixer_line ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mixer_line                   : 12;  //  0:11
	UINT32 status_pic_init_count               : 4;   // 12:15
	UINT32 osd_60hz_status_mixer_line          : 12;  // 16:27
	UINT32 osd_60hz_status_pic_init_count      : 4;   // 28:31
} H14A0_OSD_STATUS_MIXER_LINE_T;

/*-----------------------------------------------------------------------------
	0x5110 osd_status_mon_data0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data0                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA0_T;

/*-----------------------------------------------------------------------------
	0x5114 osd_status_mon_data1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data1                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA1_T;

/*-----------------------------------------------------------------------------
	0x5118 osd_status_mon_data2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data2                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA2_T;

/*-----------------------------------------------------------------------------
	0x511c osd_status_mon_data3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data3                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA3_T;

/*-----------------------------------------------------------------------------
	0x5120 osd_status_mon_data4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data4                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA4_T;

/*-----------------------------------------------------------------------------
	0x5124 osd_status_mon_data5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data5                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA5_T;

/*-----------------------------------------------------------------------------
	0x5128 osd_status_mon_data6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data6                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA6_T;

/*-----------------------------------------------------------------------------
	0x512c osd_status_mon_data7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data7                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA7_T;

/*-----------------------------------------------------------------------------
	0x5130 osd_status_mon_data8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data8                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA8_T;

/*-----------------------------------------------------------------------------
	0x5134 osd_status_mon_data9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_data9                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATA9_T;

/*-----------------------------------------------------------------------------
	0x5138 osd_status_mon_dataa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_dataa                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATAA_T;

/*-----------------------------------------------------------------------------
	0x513c osd_status_mon_datab ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 status_mon_datab                    : 32;  //  0:31
} H14A0_OSD_STATUS_MON_DATAB_T;

/*-----------------------------------------------------------------------------
	0x5140 osd_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_disp_size_h                  : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 common_disp_size_w                  : 12;  // 16:27
} H14A0_OSD_COMMON_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
	0x5144 osd_common_ctrl_irq ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_ctrl_irq_clear_osd3          : 1;   //     0
	UINT32 common_ctrl_irq_clear_osd2          : 1;   //     1
	UINT32 common_ctrl_irq_clear_osd1          : 1;   //     2
	UINT32 common_ctrl_irq_clear_osd0          : 1;   //     3
	UINT32 common_ctrl_irq_clear_mixer         : 1;   //     4
	UINT32 common_ctrl_irq_clear_mixer_60hz    : 1;   //     5
	UINT32                                     : 10;  //  6:15 reserved
	UINT32 common_ctrl_irq_en_osd3             : 1;   //    16
	UINT32 common_ctrl_irq_en_osd2             : 1;   //    17
	UINT32 common_ctrl_irq_en_osd1             : 1;   //    18
	UINT32 common_ctrl_irq_en_osd0             : 1;   //    19
	UINT32 common_ctrl_irq_en_mixer            : 1;   //    20
	UINT32 common_ctrl_irq_en_mixer_60hz       : 1;   //    21
} H14A0_OSD_COMMON_CTRL_IRQ_T;

/*-----------------------------------------------------------------------------
	0x5148 osd_common_ctrl_pic_init ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_pic_start_sel                : 1;   //     0
	UINT32                                     : 3;   //  1: 3 reserved
	UINT32 common_pic_init_sel                 : 1;   //     4
	UINT32                                     : 3;   //  5: 7 reserved
	UINT32 common_self_init_src_sel            : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 common_60hz_init_src_sel            : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 common_2nd_pic_init_line            : 12;  // 16:27
	UINT32 common_2nd_pic_init_en              : 1;   //    28
} H14A0_OSD_COMMON_CTRL_PIC_INIT_T;

/*-----------------------------------------------------------------------------
	0x514c osd_common_ctrl_pic_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_pic_init_timer               : 32;  //  0:31
} H14A0_OSD_COMMON_CTRL_PIC_TIMER_T;

/*-----------------------------------------------------------------------------
	0x5150 osd_common_ctrl_3dfp_out ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_vact_space                   : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 common_vact_video                   : 12;  // 16:27
	UINT32 common_3dfp_en                      : 1;   //    28
} H14A0_OSD_COMMON_CTRL_3DFP_OUT_T;

/*-----------------------------------------------------------------------------
	0x5154 osd_common_ctrl_pq_path ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_pq_path_sel                  : 2;   //  0: 1
	UINT32                                     : 2;   //  2: 3 reserved
	UINT32 common_pq_path_en                   : 1;   //     4
} H14A0_OSD_COMMON_CTRL_PQ_PATH_T;

/*-----------------------------------------------------------------------------
	0x5158 osd_common_plte_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_plte_sel                     : 3;   //  0: 2
} H14A0_OSD_COMMON_PLTE_SEL_T;

/*-----------------------------------------------------------------------------
	0x515c osd_common_osd_valid_th ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_valid_area_th                   : 8;   //  0: 7
	UINT32                                     : 8;   //  8:15 reserved
	UINT32 osd_valid_area_en                   : 1;   //    16
	UINT32                                     : 3;   // 17:19 reserved
	UINT32 blend_sel_out_sep_tp_a              : 8;   // 20:27
	UINT32 blend_sel_out_sep_tp_ma             : 1;   //    28
} H14A0_OSD_COMMON_OSD_VALID_TH_T;

/*-----------------------------------------------------------------------------
	0x5160 osd_common_csc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_csc_osd_sel                  : 2;   //  0: 1
	UINT32                                     : 2;   //  2: 3 reserved
	UINT32 common_csc_all_bypass               : 1;   //     4
} H14A0_OSD_COMMON_CSC_CTRL_T;

/*-----------------------------------------------------------------------------
	0x5164 osd_common_csc_coef0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_csc0_datai                   : 11;  //  0:10
	UINT32 common_csc0_rw_n                    : 1;   //    11
	UINT32 common_csc0_addr                    : 4;   // 12:15
	UINT32 common_csc0_bypass                  : 1;   //    16
	UINT32                                     : 3;   // 17:19 reserved
	UINT32 common_csc0_sel                     : 1;   //    20
	UINT32                                     : 3;   // 21:23 reserved
	UINT32 common_csc0_out_ctrl                : 3;   // 24:26
	UINT32                                     : 1;   //    27 reserved
	UINT32 common_csc0_in_ctrl                 : 3;   // 28:30
} H14A0_OSD_COMMON_CSC_COEF0_T;

/*-----------------------------------------------------------------------------
	0x5168 osd_common_csc_coef1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_csc1_datai                   : 11;  //  0:10
	UINT32 common_csc1_rw_n                    : 1;   //    11
	UINT32 common_csc1_addr                    : 4;   // 12:15
	UINT32 common_csc1_bypass                  : 1;   //    16
	UINT32                                     : 3;   // 17:19 reserved
	UINT32 common_csc1_sel                     : 1;   //    20
	UINT32                                     : 3;   // 21:23 reserved
	UINT32 common_csc1_out_ctrl                : 3;   // 24:26
	UINT32                                     : 1;   //    27 reserved
	UINT32 common_csc1_in_ctrl                 : 3;   // 28:30
} H14A0_OSD_COMMON_CSC_COEF1_T;

/*-----------------------------------------------------------------------------
	0x516c osd_common_60hz_pic_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_60hz_pic_init_timer             : 32;  //  0:31
} H14A0_OSD_COMMON_60HZ_PIC_TIMER_T;

/*-----------------------------------------------------------------------------
	0x5170 osd_common_sep_osd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 common_sep_osd_hdr_en               : 1;   //     0
	UINT32                                     : 3;   //  1: 3 reserved
	UINT32 common_sep_osd_sync_sel             : 1;   //     4
	UINT32                                     : 26;  //  5:30 reserved
	UINT32 common_sep_osd_bypass               : 1;   //    31
} H14A0_OSD_COMMON_SEP_OSD_CTRL_T;

/*-----------------------------------------------------------------------------
	0x5174 osd_common_sep_osd_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_0                       : 32;  //  0:31
} H14A0_OSD_COMMON_SEP_OSD_HDR0_T;

/*-----------------------------------------------------------------------------
	0x5178 osd_common_sep_osd_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_1                       : 32;  //  0:31
} H14A0_OSD_COMMON_SEP_OSD_HDR1_T;

/*-----------------------------------------------------------------------------
	0x517c osd_common_sep_osd_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_2                       : 32;  //  0:31
} H14A0_OSD_COMMON_SEP_OSD_HDR2_T;

/*-----------------------------------------------------------------------------
	0x5180 osd_common_sep_osd_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_3                       : 32;  //  0:31
} H14A0_OSD_COMMON_SEP_OSD_HDR3_T;

/*-----------------------------------------------------------------------------
	0x5184 osd_common_sep_osd_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_4                       : 32;  //  0:31
} H14A0_OSD_COMMON_SEP_OSD_HDR4_T;

/*-----------------------------------------------------------------------------
	0x5188 osd_common_sep_osd_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_5                       : 32;  //  0:31
} H14A0_OSD_COMMON_SEP_OSD_HDR5_T;

/*-----------------------------------------------------------------------------
	0x518c osd_common_sep_osd_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_6                       : 32;  //  0:31
} H14A0_OSD_COMMON_SEP_OSD_HDR6_T;

/*-----------------------------------------------------------------------------
	0x5190 osd_common_ctrl_tpg ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_common_ctrl_tpg                 : 32;  //  0:31
} H14A0_OSD_COMMON_CTRL_TPG_T;

/*-----------------------------------------------------------------------------
	0x5194 osd_common_60hz_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_60hz_disp_size_h                : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd_60hz_disp_size_w                : 12;  // 16:27
} H14A0_OSD_COMMON_60HZ_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
	0x5198 osd_common_60hz_ctrl_path ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                     : 16;  //  0:15 reserved
	UINT32 osd_60hz_path_osd1_en               : 1;   //    16
	UINT32                                     : 7;   // 17:23 reserved
	UINT32 osd_60hz_path_osd0_en               : 1;   //    24
} H14A0_OSD_COMMON_60HZ_CTRL_PATH_T;

/*-----------------------------------------------------------------------------
	0x519c osd_common_60hz_osd_valid_th ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_60hz_l_osd_valid_th             : 8;   //  0: 7
	UINT32 osd_60hz_l_src_sel_dn_zero          : 1;   //     8
	UINT32 osd_60hz_l_src_sel_a_zero           : 1;   //     9
	UINT32 osd_60hz_l_src_sel_dn               : 1;   //    10
	UINT32 osd_60hz_l_src_sel_a                : 1;   //    11
	UINT32 osd_60hz_l_src_sel_out              : 1;   //    12
	UINT32 osd_60hz_l_mk_color                 : 3;   // 13:15
	UINT32 osd_60hz_r_osd_valid_th             : 8;   // 16:23
	UINT32 osd_60hz_r_src_sel_dn_zero          : 1;   //    24
	UINT32 osd_60hz_r_src_sel_a_zero           : 1;   //    25
	UINT32 osd_60hz_r_src_sel_dn               : 1;   //    26
	UINT32 osd_60hz_r_src_sel_a                : 1;   //    27
	UINT32 osd_60hz_r_src_sel_out              : 1;   //    28
	UINT32 osd_60hz_r_mk_color                 : 3;   // 29:31
} H14A0_OSD_COMMON_60HZ_OSD_VALID_TH_T;

/*-----------------------------------------------------------------------------
	0x51a0 osd_mixer_ctrl_onoff ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_mixer_en                       : 1;   //     0
	UINT32                                     : 7;   //  1: 7 reserved
	UINT32 osd2_mixer_en                       : 1;   //     8
	UINT32                                     : 7;   //  9:15 reserved
	UINT32 osd1_mixer_en                       : 1;   //    16
	UINT32                                     : 7;   // 17:23 reserved
	UINT32 osd0_mixer_en                       : 1;   //    24
} H14A0_OSD_MIXER_CTRL_ONOFF_T;

/*-----------------------------------------------------------------------------
	0x51a4 osd_mixer_ctrl_output ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_out_b                         : 8;   //  0: 7
	UINT32 mixer_out_g                         : 8;   //  8:15
	UINT32 mixer_out_r                         : 8;   // 16:23
	UINT32 mixer_out_fpr_mask_en               : 1;   //    24
	UINT32 mixer_out_fpr_mask_odd              : 1;   //    25
	UINT32                                     : 2;   // 26:27 reserved
	UINT32 mixer_out_en                        : 1;   //    28
	UINT32 mixer_out_valid_en                  : 1;   //    29
	UINT32 mixer_out_vid_de_only               : 1;   //    30
	UINT32 mixer_out_vid_init_en               : 1;   //    31
} H14A0_OSD_MIXER_CTRL_OUTPUT_T;

/*-----------------------------------------------------------------------------
	0x51a8 osd_mixer_ctrl_awb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_mixer_awb_b                    : 1;   //     0
	UINT32 osd3_mixer_awb_g                    : 1;   //     1
	UINT32 osd3_mixer_awb_r                    : 1;   //     2
	UINT32                                     : 1;   //     3 reserved
	UINT32 osd2_mixer_awb_b                    : 1;   //     4
	UINT32 osd2_mixer_awb_g                    : 1;   //     5
	UINT32 osd2_mixer_awb_r                    : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd1_mixer_awb_b                    : 1;   //     8
	UINT32 osd1_mixer_awb_g                    : 1;   //     9
	UINT32 osd1_mixer_awb_r                    : 1;   //    10
	UINT32                                     : 1;   //    11 reserved
	UINT32 osd0_mixer_awb_b                    : 1;   //    12
	UINT32 osd0_mixer_awb_g                    : 1;   //    13
	UINT32 osd0_mixer_awb_r                    : 1;   //    14
} H14A0_OSD_MIXER_CTRL_AWB_T;

/*-----------------------------------------------------------------------------
	0x51ac osd_mixer_ctrl_sync ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_osd3_tp_en                     : 1;   //     0
	UINT32 sync_osd3_tp_aa_en                  : 1;   //     1
	UINT32                                     : 2;   //  2: 3 reserved
	UINT32 sync_osd2_tp_en                     : 1;   //     4
	UINT32 sync_osd2_tp_aa_en                  : 1;   //     5
	UINT32                                     : 2;   //  6: 7 reserved
	UINT32 sync_osd1_tp_en                     : 1;   //     8
	UINT32 sync_osd1_tp_aa_en                  : 1;   //     9
	UINT32                                     : 2;   // 10:11 reserved
	UINT32 sync_osd0_tp_en                     : 1;   //    12
	UINT32 sync_osd0_tp_aa_en                  : 1;   //    13
	UINT32                                     : 2;   // 14:15 reserved
	UINT32 sync_tp_alpha                       : 8;   // 16:23
	UINT32                                     : 7;   // 24:30 reserved
	UINT32 sync_v_reverse                      : 1;   //    31
} H14A0_OSD_MIXER_CTRL_SYNC_T;

/*-----------------------------------------------------------------------------
	0x51b0 osd_mixer_ctrl_blend ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 blend_sel_0v_dn                     : 1;   //     0
	UINT32 blend_sel_0v_up                     : 1;   //     1
	UINT32 blend_sel_10_dn                     : 1;   //     2
	UINT32 blend_sel_10_up                     : 1;   //     3
	UINT32                                     : 2;   //  4: 5 reserved
	UINT32 blend_sel_31_dn                     : 1;   //     6
	UINT32 blend_sel_31_up                     : 1;   //     7
	UINT32 blend_div_dvdd                      : 1;   //     8
	UINT32 blend_div_dvsr                      : 1;   //     9
	UINT32 blend_sel_a10_dn                    : 1;   //    10
	UINT32 blend_sel_a10_a                     : 1;   //    11
	UINT32                                     : 2;   // 12:13 reserved
	UINT32 blend_sel_a31_dn                    : 1;   //    14
	UINT32 blend_sel_a31_a                     : 1;   //    15
	UINT32 blend_sel_v3d_src                   : 1;   //    16
	UINT32 blend_sel_v5d_src                   : 1;   //    17
	UINT32 blend_sel_v7d_src                   : 1;   //    18
	UINT32                                     : 1;   //    19 reserved
	UINT32 blend_sel_vid_src                   : 1;   //    20
	UINT32 blend_sel_31_dn_src                 : 1;   //    21
	UINT32 blend_sel_div_pel_src               : 1;   //    22
	UINT32 blend_sel_out_sepc_src              : 1;   //    23
	UINT32 blend_sel_a3d_src                   : 1;   //    24
	UINT32 blend_sel_a5d_src                   : 1;   //    25
	UINT32                                     : 1;   //    26 reserved
	UINT32 blend_sel_a31_z_3d_src              : 1;   //    27
	UINT32 blend_sel_out_vid                   : 1;   //    28
	UINT32 blend_sel_out_sepc                  : 1;   //    29
	UINT32 blend_sel_out_sepa                  : 1;   //    30
	UINT32 blend_sel_out_sep_tp_en             : 1;   //    31
} H14A0_OSD_MIXER_CTRL_BLEND_T;

/*-----------------------------------------------------------------------------
	0x51b4 osd_mixer_60hz_ctrl_awb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                     : 1;   //     0 reserved
	UINT32 awb_l_0                             : 3;   //  1: 3
	UINT32 vid_a_l                             : 8;   //  4:11
	UINT32 osd0_csc_en                         : 1;   //    12
	UINT32 awb_l_1                             : 3;   // 13:15
	UINT32                                     : 1;   //    16 reserved
	UINT32 awb_r_0                             : 3;   // 17:19
	UINT32 vid_r_a                             : 8;   // 20:27
	UINT32 osd1_csc_en                         : 1;   //    28
	UINT32 awb_r_1                             : 3;   // 29:31
} H14A0_OSD_MIXER_60HZ_CTRL_AWB_T;

/*-----------------------------------------------------------------------------
	0x51b8 osd_mixer_60hz_ctrl_blend ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_sel_l_0v_dn_zero            : 1;   //     0
	UINT32 reg_src_sel_l_0v_up_zero            : 1;   //     1
	UINT32 reg_src_sel_l_10_dn_zero            : 1;   //     2
	UINT32 reg_src_sel_l_10_up_zero            : 1;   //     3
	UINT32 reg_src_sel_l_0v_dn_osd0            : 1;   //     4
	UINT32 reg_src_sel_l_0v_up_osd1            : 1;   //     5
	UINT32 reg_src_sel_l_10_dn_video           : 1;   //     6
	UINT32 reg_src_sel_l_10_up_osd0            : 1;   //     7
	UINT32 reg_src_sel_l_0v_dn_osd1            : 1;   //     8
	UINT32 reg_src_sel_l_0v_up_video           : 1;   //     9
	UINT32 reg_src_sel_l_10_dn_osd1            : 1;   //    10
	UINT32 reg_src_sel_l_10_up_video           : 1;   //    11
	UINT32 reg_src_sel_l_vd_7d_0v_z            : 1;   //    12
	UINT32 reg_src_sel_l_vd_mk_vd8d            : 1;   //    13
	UINT32 reg_src_sel_l_vd_out_mk             : 1;   //    14
	UINT32 reg_src_sel_l_vd_out_zero           : 1;   //    15
	UINT32 reg_src_sel_r_0v_dn_zero            : 1;   //    16
	UINT32 reg_src_sel_r_0v_up_zero            : 1;   //    17
	UINT32 reg_src_sel_r_10_dn_zero            : 1;   //    18
	UINT32 reg_src_sel_r_10_up_zero            : 1;   //    19
	UINT32 reg_src_sel_r_0v_dn_osd0            : 1;   //    20
	UINT32 reg_src_sel_r_0v_up_osd1            : 1;   //    21
	UINT32 reg_src_sel_r_10_dn_video           : 1;   //    22
	UINT32 reg_src_sel_r_10_up_osd0            : 1;   //    23
	UINT32 reg_src_sel_r_0v_dn_osd1            : 1;   //    24
	UINT32 reg_src_sel_r_0v_up_video           : 1;   //    25
	UINT32 reg_src_sel_r_10_dn_osd1            : 1;   //    26
	UINT32 reg_src_sel_r_10_up_video           : 1;   //    27
	UINT32 reg_src_sel_r_vd_7d_0v_z            : 1;   //    28
	UINT32 reg_src_sel_r_vd_mk_vd8d            : 1;   //    29
	UINT32 reg_src_sel_r_vd_out_mk             : 1;   //    30
	UINT32 reg_src_sel_r_vd_out_zero           : 1;   //    31
} H14A0_OSD_MIXER_60HZ_CTRL_BLEND_T;

/*-----------------------------------------------------------------------------
	0x51bc osd_mixer_ctrl_hole ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hole2_en                       : 1;   //     0
	UINT32 osd3_hole1_en                       : 1;   //     1
	UINT32 osd3_hole0_en                       : 1;   //     2
	UINT32                                     : 1;   //     3 reserved
	UINT32 osd3_hole_en                        : 1;   //     4
	UINT32                                     : 3;   //  5: 7 reserved
	UINT32 osd2_hole2_en                       : 1;   //     8
	UINT32 osd2_hole1_en                       : 1;   //     9
	UINT32 osd2_hole0_en                       : 1;   //    10
	UINT32                                     : 1;   //    11 reserved
	UINT32 osd2_hole_en                        : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd1_hole2_en                       : 1;   //    16
	UINT32 osd1_hole1_en                       : 1;   //    17
	UINT32 osd1_hole0_en                       : 1;   //    18
	UINT32                                     : 1;   //    19 reserved
	UINT32 osd1_hole_en                        : 1;   //    20
	UINT32                                     : 3;   // 21:23 reserved
	UINT32 osd0_hole2_en                       : 1;   //    24
	UINT32 osd0_hole1_en                       : 1;   //    25
	UINT32 osd0_hole0_en                       : 1;   //    26
	UINT32                                     : 1;   //    27 reserved
	UINT32 osd0_hole_en                        : 1;   //    28
} H14A0_OSD_MIXER_CTRL_HOLE_T;

/*-----------------------------------------------------------------------------
	0x51c0 osd_mixer_ctrl_mux0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 layer_mux_3                         : 2;   //  0: 1
	UINT32                                     : 2;   //  2: 3 reserved
	UINT32 layer_mux_2                         : 2;   //  4: 5
	UINT32                                     : 2;   //  6: 7 reserved
	UINT32 layer_mux_1                         : 2;   //  8: 9
	UINT32                                     : 2;   // 10:11 reserved
	UINT32 layer_mux_0                         : 2;   // 12:13
	UINT32                                     : 2;   // 14:15 reserved
	UINT32 video_layer_alpha                   : 8;   // 16:23
	UINT32                                     : 4;   // 24:27 reserved
	UINT32 video_layer_off                     : 1;   //    28
} H14A0_OSD_MIXER_CTRL_MUX0_T;

/*-----------------------------------------------------------------------------
	0x51c4 osd_mixer_ctrl_mux1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window0_en                    : 1;   //     0
	UINT32                                     : 15;  //  1:15 reserved
	UINT32 mixer_window0_alpha                 : 8;   // 16:23
} H14A0_OSD_MIXER_CTRL_MUX1_T;

/*-----------------------------------------------------------------------------
	0x51c8 osd_mixer_ctrl_mux2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window0_ypos                  : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 mixer_window0_xpos                  : 12;  // 16:27
} H14A0_OSD_MIXER_CTRL_MUX2_T;

/*-----------------------------------------------------------------------------
	0x51cc osd_mixer_ctrl_mux3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window0_h                     : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 mixer_window0_w                     : 12;  // 16:27
} H14A0_OSD_MIXER_CTRL_MUX3_T;

/*-----------------------------------------------------------------------------
	0x51d0 osd_mixer_ctrl_mux4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window1_en                    : 1;   //     0
	UINT32                                     : 15;  //  1:15 reserved
	UINT32 mixer_window1_alpha                 : 8;   // 16:23
} H14A0_OSD_MIXER_CTRL_MUX4_T;

/*-----------------------------------------------------------------------------
	0x51d4 osd_mixer_ctrl_mux5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window1_ypos                  : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 mixer_window1_xpos                  : 12;  // 16:27
} H14A0_OSD_MIXER_CTRL_MUX5_T;

/*-----------------------------------------------------------------------------
	0x51d8 osd_mixer_ctrl_mux6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window1_h                     : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 mixer_window1_w                     : 12;  // 16:27
} H14A0_OSD_MIXER_CTRL_MUX6_T;

/*-----------------------------------------------------------------------------
	0x51dc osd_mixer_ctrl_mux7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window2_en                    : 1;   //     0
	UINT32                                     : 15;  //  1:15 reserved
	UINT32 mixer_window2_alpha                 : 8;   // 16:23
} H14A0_OSD_MIXER_CTRL_MUX7_T;

/*-----------------------------------------------------------------------------
	0x51e0 osd_mixer_ctrl_mux8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window2_ypos                  : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 mixer_window2_xpos                  : 12;  // 16:27
} H14A0_OSD_MIXER_CTRL_MUX8_T;

/*-----------------------------------------------------------------------------
	0x51e4 osd_mixer_ctrl_mux9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mixer_window2_h                     : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 mixer_window2_w                     : 12;  // 16:27
} H14A0_OSD_MIXER_CTRL_MUX9_T;

/*-----------------------------------------------------------------------------
	0x51e8 osd_mixer_ctrl_muxa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hole0_y                             : 12;  //  0:11
	UINT32 hole0_x                             : 12;  // 12:23
	UINT32 hole0_reverse_en                    : 1;   //    24
	UINT32                                     : 3;   // 25:27 reserved
	UINT32 hole0_en                            : 1;   //    28
} H14A0_OSD_MIXER_CTRL_MUXA_T;

/*-----------------------------------------------------------------------------
	0x51ec osd_mixer_ctrl_muxb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hole0_h                             : 12;  //  0:11
	UINT32 hole0_w                             : 12;  // 12:23
	UINT32 hole0_a                             : 8;   // 24:31
} H14A0_OSD_MIXER_CTRL_MUXB_T;

/*-----------------------------------------------------------------------------
	0x51f0 osd_mixer_ctrl_muxc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hole1_y                             : 12;  //  0:11
	UINT32 hole1_x                             : 12;  // 12:23
	UINT32 hole1_reverse_en                    : 1;   //    24
	UINT32                                     : 3;   // 25:27 reserved
	UINT32 hole1_en                            : 1;   //    28
} H14A0_OSD_MIXER_CTRL_MUXC_T;

/*-----------------------------------------------------------------------------
	0x51f4 osd_mixer_ctrl_muxd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hole1_h                             : 12;  //  0:11
	UINT32 hole1_w                             : 12;  // 12:23
	UINT32 hole1_a                             : 8;   // 24:31
} H14A0_OSD_MIXER_CTRL_MUXD_T;

/*-----------------------------------------------------------------------------
	0x51f8 osd_mixer_ctrl_muxe ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hole2_y                             : 12;  //  0:11
	UINT32 hole2_x                             : 12;  // 12:23
	UINT32 hole2_reverse_en                    : 1;   //    24
	UINT32                                     : 3;   // 25:27 reserved
	UINT32 hole2_en                            : 1;   //    28
} H14A0_OSD_MIXER_CTRL_MUXE_T;

/*-----------------------------------------------------------------------------
	0x51fc osd_mixer_ctrl_muxf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hole2_h                             : 12;  //  0:11
	UINT32 hole2_w                             : 12;  // 12:23
	UINT32 hole2_a                             : 8;   // 24:31
} H14A0_OSD_MIXER_CTRL_MUXF_T;

/*-----------------------------------------------------------------------------
	0x5200 osd0_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_en                             : 1;   //     0
	UINT32                                     : 3;   //  1: 3 reserved
	UINT32 osd0_hdr_src_sel                    : 1;   //     4
	UINT32 osd0_unaligned_addr_en              : 1;   //     5
	UINT32                                     : 2;   //  6: 7 reserved
	UINT32 osd0_v_reverse_en                   : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd0_s3d_en                         : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd0_manual_line_inc                : 2;   // 16:17
	UINT32                                     : 2;   // 18:19 reserved
	UINT32 osd0_manual_line_inc_en             : 1;   //    20
	UINT32                                     : 3;   // 21:23 reserved
	UINT32 osd0_yuyv_mode_en                   : 1;   //    24
	UINT32                                     : 3;   // 25:27 reserved
	UINT32 osd0_hdr_ready_flag_en              : 1;   //    28
	UINT32 osd0_hdr_ready_flag                 : 1;   //    29
} H14A0_OSD0_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x5204 osd0_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_base_addr                      : 32;  //  0:31
} H14A0_OSD0_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5208 osd0_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_bbs                            : 1;   //     0
	UINT32 osd0_bqws                           : 1;   //     1
	UINT32 osd0_bhws                           : 1;   //     2
	UINT32                                     : 1;   //     3 reserved
	UINT32 osd0_pbs                            : 1;   //     4
	UINT32 osd0_pqws                           : 1;   //     5
	UINT32 osd0_phws                           : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd0_hbs                            : 1;   //     8
	UINT32 osd0_hqws                           : 1;   //     9
	UINT32 osd0_hhws                           : 1;   //    10
} H14A0_OSD0_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x520c osd0_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_lfsr_seed                      : 19;  //  0:18
	UINT32 osd0_pure_ck                        : 1;   //    19
	UINT32 osd0_round                          : 3;   // 20:22
	UINT32                                     : 1;   //    23 reserved
	UINT32 osd0_ch_blue                        : 2;   // 24:25
	UINT32 osd0_ch_green                       : 2;   // 26:27
	UINT32 osd0_ch_red                         : 2;   // 28:29
	UINT32 osd0_ch_alpha                       : 2;   // 30:31
} H14A0_OSD0_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x5210 osd0_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_sc_bc_sa                       : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en              : 1;   //     2
	UINT32 osd0_sc_bilinear                    : 1;   //     3
	UINT32 osd0_sc_vdouble                     : 1;   //     4
	UINT32                                     : 1;   //     5 reserved
	UINT32 osd0_sc_v_buf_en                    : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd0_sc_sampling_mode               : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd0_sc_bnd_mode                    : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd0_sc_h_phase_off                 : 6;   // 16:21
	UINT32                                     : 2;   // 22:23 reserved
	UINT32 osd0_sc_v_phase_off                 : 6;   // 24:29
	UINT32 osd0_odd_width_en                   : 1;   //    30
	UINT32 osd0_fifo_en                        : 1;   //    31
} H14A0_OSD0_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x5214 osd0_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_r_3d_disparity_value           : 8;   //  0: 7
	UINT32 osd0_r_3d_disparity_sign            : 1;   //     8
	UINT32                                     : 7;   //  9:15 reserved
	UINT32 osd0_l_3d_disparity_value           : 8;   // 16:23
	UINT32 osd0_l_3d_disparity_sign            : 1;   //    24
} H14A0_OSD0_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x5218 osd0_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_ctrl_3d_blank                  : 32;  //  0:31
} H14A0_OSD0_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x5220 osd0_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ypos                       : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd0_hdr_xpos                       : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd0_hdr_color_key_en               : 1;   //    31
} H14A0_OSD0_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x5224 osd0_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_h_mem                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd0_hdr_w_mem                      : 12;  // 16:27
} H14A0_OSD0_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x5228 osd0_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_h_out                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd0_hdr_w_out                      : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd0_hdr_pixel_order                : 1;   //    31
} H14A0_OSD0_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x522c osd0_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_wpl                        : 16;  //  0:15
	UINT32 osd0_hdr_global_alpha               : 8;   // 16:23
	UINT32 osd0_hdr_format                     : 4;   // 24:27
	UINT32 osd0_hdr_depth                      : 3;   // 28:30
	UINT32 osd0_hdr_global_alpha_en            : 1;   //    31
} H14A0_OSD0_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x5230 osd0_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_color_key                  : 32;  //  0:31
} H14A0_OSD0_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x5234 osd0_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ptr_plte                   : 32;  //  0:31
} H14A0_OSD0_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x5238 osd0_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ptr_bmp                    : 32;  //  0:31
} H14A0_OSD0_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
	0x5240 osd1_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_en                             : 1;   //     0
	UINT32                                     : 3;   //  1: 3 reserved
	UINT32 osd1_hdr_src_sel                    : 1;   //     4
	UINT32 osd1_unaligned_addr_en              : 1;   //     5
	UINT32                                     : 2;   //  6: 7 reserved
	UINT32 osd1_v_reverse_en                   : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd1_s3d_en                         : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd1_manual_line_inc                : 2;   // 16:17
	UINT32                                     : 2;   // 18:19 reserved
	UINT32 osd1_manual_line_inc_en             : 1;   //    20
	UINT32                                     : 3;   // 21:23 reserved
	UINT32 osd1_yuyv_mode_en                   : 1;   //    24
	UINT32                                     : 3;   // 25:27 reserved
	UINT32 osd1_hdr_ready_flag_en              : 1;   //    28
	UINT32 osd1_hdr_ready_flag                 : 1;   //    29
} H14A0_OSD1_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x5244 osd1_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_base_addr                      : 32;  //  0:31
} H14A0_OSD1_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5248 osd1_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_bbs                            : 1;   //     0
	UINT32 osd1_bqws                           : 1;   //     1
	UINT32 osd1_bhws                           : 1;   //     2
	UINT32                                     : 1;   //     3 reserved
	UINT32 osd1_pbs                            : 1;   //     4
	UINT32 osd1_pqws                           : 1;   //     5
	UINT32 osd1_phws                           : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd1_hbs                            : 1;   //     8
	UINT32 osd1_hqws                           : 1;   //     9
	UINT32 osd1_hhws                           : 1;   //    10
} H14A0_OSD1_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x524c osd1_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_lfsr_seed                      : 19;  //  0:18
	UINT32 osd1_pure_ck                        : 1;   //    19
	UINT32 osd1_round                          : 3;   // 20:22
	UINT32                                     : 1;   //    23 reserved
	UINT32 osd1_ch_blue                        : 2;   // 24:25
	UINT32 osd1_ch_green                       : 2;   // 26:27
	UINT32 osd1_ch_red                         : 2;   // 28:29
	UINT32 osd1_ch_alpha                       : 2;   // 30:31
} H14A0_OSD1_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x5250 osd1_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_sc_bc_sa                       : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en              : 1;   //     2
	UINT32 osd1_sc_bilinear                    : 1;   //     3
	UINT32 osd1_sc_vdouble                     : 1;   //     4
	UINT32                                     : 1;   //     5 reserved
	UINT32 osd1_sc_v_buf_en                    : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd1_sc_sampling_mode               : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd1_sc_bnd_mode                    : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd1_sc_h_phase_off                 : 6;   // 16:21
	UINT32                                     : 2;   // 22:23 reserved
	UINT32 osd1_sc_v_phase_off                 : 6;   // 24:29
	UINT32 osd1_odd_width_en                   : 1;   //    30
	UINT32 osd1_fifo_en                        : 1;   //    31
} H14A0_OSD1_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x5254 osd1_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_r_3d_disparity_value           : 8;   //  0: 7
	UINT32 osd1_r_3d_disparity_sign            : 1;   //     8
	UINT32                                     : 7;   //  9:15 reserved
	UINT32 osd1_l_3d_disparity_value           : 8;   // 16:23
	UINT32 osd1_l_3d_disparity_sign            : 1;   //    24
} H14A0_OSD1_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x5258 osd1_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_ctrl_3d_blank                  : 32;  //  0:31
} H14A0_OSD1_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x5260 osd1_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ypos                       : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd1_hdr_xpos                       : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd1_hdr_color_key_en               : 1;   //    31
} H14A0_OSD1_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x5264 osd1_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_h_mem                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd1_hdr_w_mem                      : 12;  // 16:27
} H14A0_OSD1_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x5268 osd1_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_h_out                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd1_hdr_w_out                      : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd1_hdr_pixel_order                : 1;   //    31
} H14A0_OSD1_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x526c osd1_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_wpl                        : 16;  //  0:15
	UINT32 osd1_hdr_global_alpha               : 8;   // 16:23
	UINT32 osd1_hdr_format                     : 4;   // 24:27
	UINT32 osd1_hdr_depth                      : 3;   // 28:30
	UINT32 osd1_hdr_global_alpha_en            : 1;   //    31
} H14A0_OSD1_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x5270 osd1_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_color_key                  : 32;  //  0:31
} H14A0_OSD1_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x5274 osd1_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ptr_plte                   : 32;  //  0:31
} H14A0_OSD1_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x5278 osd1_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ptr_bmp                    : 32;  //  0:31
} H14A0_OSD1_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
	0x5280 osd2_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_en                             : 1;   //     0
	UINT32                                     : 3;   //  1: 3 reserved
	UINT32 osd2_hdr_src_sel                    : 1;   //     4
	UINT32 osd2_unaligned_addr_en              : 1;   //     5
	UINT32                                     : 2;   //  6: 7 reserved
	UINT32 osd2_v_reverse_en                   : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd2_s3d_en                         : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd2_manual_line_inc                : 2;   // 16:17
	UINT32                                     : 2;   // 18:19 reserved
	UINT32 osd2_manual_line_inc_en             : 1;   //    20
	UINT32                                     : 3;   // 21:23 reserved
	UINT32 osd2_yuyv_mode_en                   : 1;   //    24
	UINT32                                     : 3;   // 25:27 reserved
	UINT32 osd2_hdr_ready_flag_en              : 1;   //    28
	UINT32 osd2_hdr_ready_flag                 : 1;   //    29
} H14A0_OSD2_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x5284 osd2_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_base_addr                      : 32;  //  0:31
} H14A0_OSD2_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x5288 osd2_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_bbs                            : 1;   //     0
	UINT32 osd2_bqws                           : 1;   //     1
	UINT32 osd2_bhws                           : 1;   //     2
	UINT32                                     : 1;   //     3 reserved
	UINT32 osd2_pbs                            : 1;   //     4
	UINT32 osd2_pqws                           : 1;   //     5
	UINT32 osd2_phws                           : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd2_hbs                            : 1;   //     8
	UINT32 osd2_hqws                           : 1;   //     9
	UINT32 osd2_hhws                           : 1;   //    10
} H14A0_OSD2_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x528c osd2_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_lfsr_seed                      : 19;  //  0:18
	UINT32 osd2_pure_ck                        : 1;   //    19
	UINT32 osd2_round                          : 3;   // 20:22
	UINT32                                     : 1;   //    23 reserved
	UINT32 osd2_ch_blue                        : 2;   // 24:25
	UINT32 osd2_ch_green                       : 2;   // 26:27
	UINT32 osd2_ch_red                         : 2;   // 28:29
	UINT32 osd2_ch_alpha                       : 2;   // 30:31
} H14A0_OSD2_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x5290 osd2_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_sc_bc_sa                       : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en              : 1;   //     2
	UINT32 osd2_sc_bilinear                    : 1;   //     3
	UINT32 osd2_sc_vdouble                     : 1;   //     4
	UINT32                                     : 1;   //     5 reserved
	UINT32 osd2_sc_v_buf_en                    : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd2_sc_sampling_mode               : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd2_sc_bnd_mode                    : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd2_sc_h_phase_off                 : 6;   // 16:21
	UINT32                                     : 2;   // 22:23 reserved
	UINT32 osd2_sc_v_phase_off                 : 6;   // 24:29
	UINT32 osd2_odd_width_en                   : 1;   //    30
	UINT32 osd2_fifo_en                        : 1;   //    31
} H14A0_OSD2_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x5294 osd2_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_r_3d_disparity_value           : 8;   //  0: 7
	UINT32 osd2_r_3d_disparity_sign            : 1;   //     8
	UINT32                                     : 7;   //  9:15 reserved
	UINT32 osd2_l_3d_disparity_value           : 8;   // 16:23
	UINT32 osd2_l_3d_disparity_sign            : 1;   //    24
} H14A0_OSD2_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x5298 osd2_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_ctrl_3d_blank                  : 32;  //  0:31
} H14A0_OSD2_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x52a0 osd2_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ypos                       : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd2_hdr_xpos                       : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd2_hdr_color_key_en               : 1;   //    31
} H14A0_OSD2_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x52a4 osd2_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_h_mem                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd2_hdr_w_mem                      : 12;  // 16:27
} H14A0_OSD2_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x52a8 osd2_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_h_out                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd2_hdr_w_out                      : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd2_hdr_pixel_order                : 1;   //    31
} H14A0_OSD2_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x52ac osd2_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_wpl                        : 16;  //  0:15
	UINT32 osd2_hdr_global_alpha               : 8;   // 16:23
	UINT32 osd2_hdr_format                     : 4;   // 24:27
	UINT32 osd2_hdr_depth                      : 3;   // 28:30
	UINT32 osd2_hdr_global_alpha_en            : 1;   //    31
} H14A0_OSD2_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x52b0 osd2_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_color_key                  : 32;  //  0:31
} H14A0_OSD2_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x52b4 osd2_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ptr_plte                   : 32;  //  0:31
} H14A0_OSD2_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x52b8 osd2_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ptr_bmp                    : 32;  //  0:31
} H14A0_OSD2_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
	0x52c0 osd3_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_en                             : 1;   //     0
	UINT32                                     : 3;   //  1: 3 reserved
	UINT32 osd3_hdr_src_sel                    : 1;   //     4
	UINT32 osd3_unaligned_addr_en              : 1;   //     5
	UINT32                                     : 2;   //  6: 7 reserved
	UINT32 osd3_v_reverse_en                   : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd3_s3d_en                         : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd3_manual_line_inc                : 2;   // 16:17
	UINT32                                     : 2;   // 18:19 reserved
	UINT32 osd3_manual_line_inc_en             : 1;   //    20
	UINT32                                     : 3;   // 21:23 reserved
	UINT32 osd3_yuyv_mode_en                   : 1;   //    24
	UINT32                                     : 3;   // 25:27 reserved
	UINT32 osd3_hdr_ready_flag_en              : 1;   //    28
	UINT32 osd3_hdr_ready_flag                 : 1;   //    29
} H14A0_OSD3_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
	0x52c4 osd3_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_base_addr                      : 32;  //  0:31
} H14A0_OSD3_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
	0x52c8 osd3_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_bbs                            : 1;   //     0
	UINT32 osd3_bqws                           : 1;   //     1
	UINT32 osd3_bhws                           : 1;   //     2
	UINT32                                     : 1;   //     3 reserved
	UINT32 osd3_pbs                            : 1;   //     4
	UINT32 osd3_pqws                           : 1;   //     5
	UINT32 osd3_phws                           : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd3_hbs                            : 1;   //     8
	UINT32 osd3_hqws                           : 1;   //     9
	UINT32 osd3_hhws                           : 1;   //    10
} H14A0_OSD3_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
	0x52cc osd3_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_lfsr_seed                      : 19;  //  0:18
	UINT32 osd3_pure_ck                        : 1;   //    19
	UINT32 osd3_round                          : 3;   // 20:22
	UINT32                                     : 1;   //    23 reserved
	UINT32 osd3_ch_blue                        : 2;   // 24:25
	UINT32 osd3_ch_green                       : 2;   // 26:27
	UINT32 osd3_ch_red                         : 2;   // 28:29
	UINT32 osd3_ch_alpha                       : 2;   // 30:31
} H14A0_OSD3_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
	0x52d0 osd3_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_sc_bc_sa                       : 2;   //  0: 1
	UINT32 osd0_sc_force_vsize_en              : 1;   //     2
	UINT32 osd3_sc_bilinear                    : 1;   //     3
	UINT32 osd3_sc_vdouble                     : 1;   //     4
	UINT32                                     : 1;   //     5 reserved
	UINT32 osd3_sc_v_buf_en                    : 1;   //     6
	UINT32                                     : 1;   //     7 reserved
	UINT32 osd3_sc_sampling_mode               : 1;   //     8
	UINT32                                     : 3;   //  9:11 reserved
	UINT32 osd3_sc_bnd_mode                    : 1;   //    12
	UINT32                                     : 3;   // 13:15 reserved
	UINT32 osd3_sc_h_phase_off                 : 6;   // 16:21
	UINT32                                     : 2;   // 22:23 reserved
	UINT32 osd3_sc_v_phase_off                 : 6;   // 24:29
	UINT32 osd3_odd_width_en                   : 1;   //    30
	UINT32 osd3_fifo_en                        : 1;   //    31
} H14A0_OSD3_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
	0x52d4 osd3_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_r_3d_disparity_value           : 8;   //  0: 7
	UINT32 osd3_r_3d_disparity_sign            : 1;   //     8
	UINT32                                     : 7;   //  9:15 reserved
	UINT32 osd3_l_3d_disparity_value           : 8;   // 16:23
	UINT32 osd3_l_3d_disparity_sign            : 1;   //    24
} H14A0_OSD3_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
	0x52d8 osd3_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_ctrl_3d_blank                  : 32;  //  0:31
} H14A0_OSD3_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
	0x52e0 osd3_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ypos                       : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd3_hdr_xpos                       : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd3_hdr_color_key_en               : 1;   //    31
} H14A0_OSD3_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
	0x52e4 osd3_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_h_mem                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd3_hdr_w_mem                      : 12;  // 16:27
} H14A0_OSD3_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
	0x52e8 osd3_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_h_out                      : 12;  //  0:11
	UINT32                                     : 4;   // 12:15 reserved
	UINT32 osd3_hdr_w_out                      : 12;  // 16:27
	UINT32                                     : 3;   // 28:30 reserved
	UINT32 osd3_hdr_pixel_order                : 1;   //    31
} H14A0_OSD3_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
	0x52ec osd3_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_wpl                        : 16;  //  0:15
	UINT32 osd3_hdr_global_alpha               : 8;   // 16:23
	UINT32 osd3_hdr_format                     : 4;   // 24:27
	UINT32 osd3_hdr_depth                      : 3;   // 28:30
	UINT32 osd3_hdr_global_alpha_en            : 1;   //    31
} H14A0_OSD3_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
	0x52f0 osd3_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_color_key                  : 32;  //  0:31
} H14A0_OSD3_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
	0x52f4 osd3_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ptr_plte                   : 32;  //  0:31
} H14A0_OSD3_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
	0x52f8 osd3_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ptr_bmp                    : 32;  //  0:31
} H14A0_OSD3_CTRL_HDR6_T;

typedef struct {
	H14A0_OSD_STATUS_MON_CFG_T              osd_status_mon_cfg;                      // 0x5100
	H14A0_OSD_STATUS_MON_POS_T              osd_status_mon_pos;                      // 0x5104
	H14A0_OSD_STATUS_VERSION_T              osd_status_version;                      // 0x5108
	H14A0_OSD_STATUS_MIXER_LINE_T           osd_status_mixer_line;                   // 0x510c
	H14A0_OSD_STATUS_MON_DATA0_T            osd_status_mon_data0;                    // 0x5110
	H14A0_OSD_STATUS_MON_DATA1_T            osd_status_mon_data1;                    // 0x5114
	H14A0_OSD_STATUS_MON_DATA2_T            osd_status_mon_data2;                    // 0x5118
	H14A0_OSD_STATUS_MON_DATA3_T            osd_status_mon_data3;                    // 0x511c
	H14A0_OSD_STATUS_MON_DATA4_T            osd_status_mon_data4;                    // 0x5120
	H14A0_OSD_STATUS_MON_DATA5_T            osd_status_mon_data5;                    // 0x5124
	H14A0_OSD_STATUS_MON_DATA6_T            osd_status_mon_data6;                    // 0x5128
	H14A0_OSD_STATUS_MON_DATA7_T            osd_status_mon_data7;                    // 0x512c
	H14A0_OSD_STATUS_MON_DATA8_T            osd_status_mon_data8;                    // 0x5130
	H14A0_OSD_STATUS_MON_DATA9_T            osd_status_mon_data9;                    // 0x5134
	H14A0_OSD_STATUS_MON_DATAA_T            osd_status_mon_dataa;                    // 0x5138
	H14A0_OSD_STATUS_MON_DATAB_T            osd_status_mon_datab;                    // 0x513c
	H14A0_OSD_COMMON_DISP_SIZE_T            osd_common_disp_size;                    // 0x5140
	H14A0_OSD_COMMON_CTRL_IRQ_T             osd_common_ctrl_irq;                     // 0x5144
	H14A0_OSD_COMMON_CTRL_PIC_INIT_T        osd_common_ctrl_pic_init;                // 0x5148
	H14A0_OSD_COMMON_CTRL_PIC_TIMER_T       osd_common_ctrl_pic_timer;               // 0x514c
	H14A0_OSD_COMMON_CTRL_3DFP_OUT_T        osd_common_ctrl_3dfp_out;                // 0x5150
	H14A0_OSD_COMMON_CTRL_PQ_PATH_T         osd_common_ctrl_pq_path;                 // 0x5154
	H14A0_OSD_COMMON_PLTE_SEL_T             osd_common_plte_sel;                     // 0x5158
	H14A0_OSD_COMMON_OSD_VALID_TH_T         osd_common_osd_valid_th;                 // 0x515c
	H14A0_OSD_COMMON_CSC_CTRL_T             osd_common_csc_ctrl;                     // 0x5160
	H14A0_OSD_COMMON_CSC_COEF0_T            osd_common_csc_coef0;                    // 0x5164
	H14A0_OSD_COMMON_CSC_COEF1_T            osd_common_csc_coef1;                    // 0x5168
	H14A0_OSD_COMMON_60HZ_PIC_TIMER_T       osd_common_60hz_pic_timer;               // 0x516c
	H14A0_OSD_COMMON_SEP_OSD_CTRL_T         osd_common_sep_osd_ctrl;                 // 0x5170
	H14A0_OSD_COMMON_SEP_OSD_HDR0_T         osd_common_sep_osd_hdr0;                 // 0x5174
	H14A0_OSD_COMMON_SEP_OSD_HDR1_T         osd_common_sep_osd_hdr1;                 // 0x5178
	H14A0_OSD_COMMON_SEP_OSD_HDR2_T         osd_common_sep_osd_hdr2;                 // 0x517c
	H14A0_OSD_COMMON_SEP_OSD_HDR3_T         osd_common_sep_osd_hdr3;                 // 0x5180
	H14A0_OSD_COMMON_SEP_OSD_HDR4_T         osd_common_sep_osd_hdr4;                 // 0x5184
	H14A0_OSD_COMMON_SEP_OSD_HDR5_T         osd_common_sep_osd_hdr5;                 // 0x5188
	H14A0_OSD_COMMON_SEP_OSD_HDR6_T         osd_common_sep_osd_hdr6;                 // 0x518c
	H14A0_OSD_COMMON_CTRL_TPG_T             osd_common_ctrl_tpg;                     // 0x5190
	H14A0_OSD_COMMON_60HZ_DISP_SIZE_T       osd_common_60hz_disp_size;               // 0x5194
	H14A0_OSD_COMMON_60HZ_CTRL_PATH_T       osd_common_60hz_ctrl_path;               // 0x5198
	H14A0_OSD_COMMON_60HZ_OSD_VALID_TH_T    osd_common_60hz_osd_valid_th;            // 0x519c
	H14A0_OSD_MIXER_CTRL_ONOFF_T            osd_mixer_ctrl_onoff;                    // 0x51a0
	H14A0_OSD_MIXER_CTRL_OUTPUT_T           osd_mixer_ctrl_output;                   // 0x51a4
	H14A0_OSD_MIXER_CTRL_AWB_T              osd_mixer_ctrl_awb;                      // 0x51a8
	H14A0_OSD_MIXER_CTRL_SYNC_T             osd_mixer_ctrl_sync;                     // 0x51ac
	H14A0_OSD_MIXER_CTRL_BLEND_T            osd_mixer_ctrl_blend;                    // 0x51b0
	H14A0_OSD_MIXER_60HZ_CTRL_AWB_T         osd_mixer_60hz_ctrl_awb;                 // 0x51b4
	H14A0_OSD_MIXER_60HZ_CTRL_BLEND_T       osd_mixer_60hz_ctrl_blend;               // 0x51b8
	H14A0_OSD_MIXER_CTRL_HOLE_T             osd_mixer_ctrl_hole;                     // 0x51bc
	H14A0_OSD_MIXER_CTRL_MUX0_T             osd_mixer_ctrl_mux0;                     // 0x51c0
	H14A0_OSD_MIXER_CTRL_MUX1_T             osd_mixer_ctrl_mux1;                     // 0x51c4
	H14A0_OSD_MIXER_CTRL_MUX2_T             osd_mixer_ctrl_mux2;                     // 0x51c8
	H14A0_OSD_MIXER_CTRL_MUX3_T             osd_mixer_ctrl_mux3;                     // 0x51cc
	H14A0_OSD_MIXER_CTRL_MUX4_T             osd_mixer_ctrl_mux4;                     // 0x51d0
	H14A0_OSD_MIXER_CTRL_MUX5_T             osd_mixer_ctrl_mux5;                     // 0x51d4
	H14A0_OSD_MIXER_CTRL_MUX6_T             osd_mixer_ctrl_mux6;                     // 0x51d8
	H14A0_OSD_MIXER_CTRL_MUX7_T             osd_mixer_ctrl_mux7;                     // 0x51dc
	H14A0_OSD_MIXER_CTRL_MUX8_T             osd_mixer_ctrl_mux8;                     // 0x51e0
	H14A0_OSD_MIXER_CTRL_MUX9_T             osd_mixer_ctrl_mux9;                     // 0x51e4
	H14A0_OSD_MIXER_CTRL_MUXA_T             osd_mixer_ctrl_muxa;                     // 0x51e8
	H14A0_OSD_MIXER_CTRL_MUXB_T             osd_mixer_ctrl_muxb;                     // 0x51ec
	H14A0_OSD_MIXER_CTRL_MUXC_T             osd_mixer_ctrl_muxc;                     // 0x51f0
	H14A0_OSD_MIXER_CTRL_MUXD_T             osd_mixer_ctrl_muxd;                     // 0x51f4
	H14A0_OSD_MIXER_CTRL_MUXE_T             osd_mixer_ctrl_muxe;                     // 0x51f8
	H14A0_OSD_MIXER_CTRL_MUXF_T             osd_mixer_ctrl_muxf;                     // 0x51fc
	H14A0_OSD0_CTRL_MAIN_T                  osd0_ctrl_main;                          // 0x5200
	H14A0_OSD0_BASE_ADDR_T                  osd0_base_addr;                          // 0x5204
	H14A0_OSD0_CTRL_SWAP_T                  osd0_ctrl_swap;                          // 0x5208
	H14A0_OSD0_CTRL_PDEC_T                  osd0_ctrl_pdec;                          // 0x520c
	H14A0_OSD0_CTRL_SCALER_T                osd0_ctrl_scaler;                        // 0x5210
	H14A0_OSD0_CTRL_3D_DISPARITY_T          osd0_ctrl_3d_disparity;                  // 0x5214
	H14A0_OSD0_CTRL_3D_BLANK_T              osd0_ctrl_3d_blank;                      // 0x5218
	UINT32                                  reserved0;                               // 0x521c
	H14A0_OSD0_CTRL_HDR0_T                  osd0_ctrl_hdr0;                          // 0x5220
	H14A0_OSD0_CTRL_HDR1_T                  osd0_ctrl_hdr1;                          // 0x5224
	H14A0_OSD0_CTRL_HDR2_T                  osd0_ctrl_hdr2;                          // 0x5228
	H14A0_OSD0_CTRL_HDR3_T                  osd0_ctrl_hdr3;                          // 0x522c
	H14A0_OSD0_CTRL_HDR4_T                  osd0_ctrl_hdr4;                          // 0x5230
	H14A0_OSD0_CTRL_HDR5_T                  osd0_ctrl_hdr5;                          // 0x5234
	H14A0_OSD0_CTRL_HDR6_T                  osd0_ctrl_hdr6;                          // 0x5238
	UINT32                                  reserved1;                               // 0x523c
	H14A0_OSD1_CTRL_MAIN_T                  osd1_ctrl_main;                          // 0x5240
	H14A0_OSD1_CTRL_BASE_ADDR_T             osd1_ctrl_base_addr;                     // 0x5244
	H14A0_OSD1_CTRL_SWAP_T                  osd1_ctrl_swap;                          // 0x5248
	H14A0_OSD1_CTRL_PDEC_T                  osd1_ctrl_pdec;                          // 0x524c
	H14A0_OSD1_CTRL_SCALER_T                osd1_ctrl_scaler;                        // 0x5250
	H14A0_OSD1_CTRL_3D_DISPARITY_T          osd1_ctrl_3d_disparity;                  // 0x5254
	H14A0_OSD1_CTRL_3D_BLANK_T              osd1_ctrl_3d_blank;                      // 0x5258
	UINT32                                  reserved2;                               // 0x525c
	H14A0_OSD1_CTRL_HDR0_T                  osd1_ctrl_hdr0;                          // 0x5260
	H14A0_OSD1_CTRL_HDR1_T                  osd1_ctrl_hdr1;                          // 0x5264
	H14A0_OSD1_CTRL_HDR2_T                  osd1_ctrl_hdr2;                          // 0x5268
	H14A0_OSD1_CTRL_HDR3_T                  osd1_ctrl_hdr3;                          // 0x526c
	H14A0_OSD1_CTRL_HDR4_T                  osd1_ctrl_hdr4;                          // 0x5270
	H14A0_OSD1_CTRL_HDR5_T                  osd1_ctrl_hdr5;                          // 0x5274
	H14A0_OSD1_CTRL_HDR6_T                  osd1_ctrl_hdr6;                          // 0x5278
	UINT32                                  reserved3;                               // 0x527c
	H14A0_OSD2_CTRL_MAIN_T                  osd2_ctrl_main;                          // 0x5280
	H14A0_OSD2_CTRL_BASE_ADDR_T             osd2_ctrl_base_addr;                     // 0x5284
	H14A0_OSD2_CTRL_SWAP_T                  osd2_ctrl_swap;                          // 0x5288
	H14A0_OSD2_CTRL_PDEC_T                  osd2_ctrl_pdec;                          // 0x528c
	H14A0_OSD2_CTRL_SCALER_T                osd2_ctrl_scaler;                        // 0x5290
	H14A0_OSD2_CTRL_3D_DISPARITY_T          osd2_ctrl_3d_disparity;                  // 0x5294
	H14A0_OSD2_CTRL_3D_BLANK_T              osd2_ctrl_3d_blank;                      // 0x5298
	UINT32                                  reserved4;                               // 0x529c
	H14A0_OSD2_CTRL_HDR0_T                  osd2_ctrl_hdr0;                          // 0x52a0
	H14A0_OSD2_CTRL_HDR1_T                  osd2_ctrl_hdr1;                          // 0x52a4
	H14A0_OSD2_CTRL_HDR2_T                  osd2_ctrl_hdr2;                          // 0x52a8
	H14A0_OSD2_CTRL_HDR3_T                  osd2_ctrl_hdr3;                          // 0x52ac
	H14A0_OSD2_CTRL_HDR4_T                  osd2_ctrl_hdr4;                          // 0x52b0
	H14A0_OSD2_CTRL_HDR5_T                  osd2_ctrl_hdr5;                          // 0x52b4
	H14A0_OSD2_CTRL_HDR6_T                  osd2_ctrl_hdr6;                          // 0x52b8
	UINT32                                  reserved5;                               // 0x52bc
	H14A0_OSD3_CTRL_MAIN_T                  osd3_ctrl_main;                          // 0x52c0
	H14A0_OSD3_CTRL_BASE_ADDR_T             osd3_ctrl_base_addr;                     // 0x52c4
	H14A0_OSD3_CTRL_SWAP_T                  osd3_ctrl_swap;                          // 0x52c8
	H14A0_OSD3_CTRL_PDEC_T                  osd3_ctrl_pdec;                          // 0x52cc
	H14A0_OSD3_CTRL_SCALER_T                osd3_ctrl_scaler;                        // 0x52d0
	H14A0_OSD3_CTRL_3D_DISPARITY_T          osd3_ctrl_3d_disparity;                  // 0x52d4
	H14A0_OSD3_CTRL_3D_BLANK_T              osd3_ctrl_3d_blank;                      // 0x52d8
	UINT32                                  reserved6;                               // 0x52dc
	H14A0_OSD3_CTRL_HDR0_T                  osd3_ctrl_hdr0;                          // 0x52e0
	H14A0_OSD3_CTRL_HDR1_T                  osd3_ctrl_hdr1;                          // 0x52e4
	H14A0_OSD3_CTRL_HDR2_T                  osd3_ctrl_hdr2;                          // 0x52e8
	H14A0_OSD3_CTRL_HDR3_T                  osd3_ctrl_hdr3;                          // 0x52ec
	H14A0_OSD3_CTRL_HDR4_T                  osd3_ctrl_hdr4;                          // 0x52f0
	H14A0_OSD3_CTRL_HDR5_T                  osd3_ctrl_hdr5;                          // 0x52f4
	H14A0_OSD3_CTRL_HDR6_T                  osd3_ctrl_hdr6;                          // 0x52f8
} DE_OSD_REG_H14A0_T;

#endif
