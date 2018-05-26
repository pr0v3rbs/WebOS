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

#ifndef _REG_OSD_M14B0_H_
#define _REG_OSD_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0024100L osd_status_mon_cfg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_cfg                 : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_status_mon_osd_sel             : 2 ;  //  5: 4     
} M14B0_OSD_STATUS_MON_CFG_T;

/*-----------------------------------------------------------------------------
		0xc0024104L osd_status_mon_pos ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_ypos                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_status_mon_xpos                : 12;  // 27:16     
} M14B0_OSD_STATUS_MON_POS_T;

/*-----------------------------------------------------------------------------
		0xc0024108L osd_status_version ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_version                 : 32;  // 31: 0     
} M14B0_OSD_STATUS_VERSION_T;

/*-----------------------------------------------------------------------------
		0xc002410cL osd_status_mixer_line ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mixer_line              : 12;  // 11: 0     
	UINT32 reg_status_pic_init_count          : 4 ;  // 15:12     
	UINT32 reg_60hz_status_mixer_line         : 12;  // 27:16     
	UINT32 reg_60hz_status_pic_init_count     : 4 ;  // 31:28     
} M14B0_OSD_STATUS_MIXER_LINE_T;

/*-----------------------------------------------------------------------------
		0xc0024110L osd_status_mon_data0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data0               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA0_T;

/*-----------------------------------------------------------------------------
		0xc0024114L osd_status_mon_data1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data1               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA1_T;

/*-----------------------------------------------------------------------------
		0xc0024118L osd_status_mon_data2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data2               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA2_T;

/*-----------------------------------------------------------------------------
		0xc002411cL osd_status_mon_data3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data3               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA3_T;

/*-----------------------------------------------------------------------------
		0xc0024120L osd_status_mon_data4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data4               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA4_T;

/*-----------------------------------------------------------------------------
		0xc0024124L osd_status_mon_data5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data5               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA5_T;

/*-----------------------------------------------------------------------------
		0xc0024128L osd_status_mon_data6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data6               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA6_T;

/*-----------------------------------------------------------------------------
		0xc002412cL osd_status_mon_data7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data7               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA7_T;

/*-----------------------------------------------------------------------------
		0xc0024130L osd_status_mon_data8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data8               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA8_T;

/*-----------------------------------------------------------------------------
		0xc0024134L osd_status_mon_data9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_data9               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATA9_T;

/*-----------------------------------------------------------------------------
		0xc0024138L osd_status_mon_dataa ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_dataa               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATAA_T;

/*-----------------------------------------------------------------------------
		0xc002413cL osd_status_mon_datab ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_mon_datab               : 32;  // 31: 0     
} M14B0_OSD_STATUS_MON_DATAB_T;

/*-----------------------------------------------------------------------------
		0xc0024140L osd_common_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_disp_size_h             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_common_disp_size_w             : 12;  // 27:16     
} M14B0_OSD_COMMON_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0024144L osd_common_ctrl_irq ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_ctrl_irq_clear_osd3     : 1 ;  //     0     
	UINT32 reg_common_ctrl_irq_clear_osd2     : 1 ;  //     1     
	UINT32 reg_common_ctrl_irq_clear_osd1     : 1 ;  //     2     
	UINT32 reg_common_ctrl_irq_clear_osd0     : 1 ;  //     3     
	UINT32 reg_common_ctrl_irq_clear_mixer    : 1 ;  //     4     
	UINT32 reg_common_ctrl_irq_clear_mixer_60hz: 1 ;  //     5     
	UINT32                                    : 10;  // 15: 6     reserved
	UINT32 reg_common_ctrl_irq_en_osd3        : 1 ;  //    16     
	UINT32 reg_common_ctrl_irq_en_osd2        : 1 ;  //    17     
	UINT32 reg_common_ctrl_irq_en_osd1        : 1 ;  //    18     
	UINT32 reg_common_ctrl_irq_en_osd0        : 1 ;  //    19     
	UINT32 reg_common_ctrl_irq_en_mixer       : 1 ;  //    20     
	UINT32 reg_common_ctrl_irq_en_mixer_60hz  : 1 ;  //    21     
} M14B0_OSD_COMMON_CTRL_IRQ_T;

/*-----------------------------------------------------------------------------
		0xc0024148L osd_common_ctrl_pic_init ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_pic_start_sel           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_common_pic_init_sel            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_common_self_init_src_sel       : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_common_60hz_init_src_sel       : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_common_2nd_pic_init_line       : 12;  // 27:16     
	UINT32 reg_common_2nd_pic_init_en         : 1 ;  //    28     
} M14B0_OSD_COMMON_CTRL_PIC_INIT_T;

/*-----------------------------------------------------------------------------
		0xc002414cL osd_common_ctrl_pic_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_pic_init_timer          : 32;  // 31: 0     
} M14B0_OSD_COMMON_CTRL_PIC_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc0024150L osd_common_ctrl_3dfp_out ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_vact_space              : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_common_vact_video              : 12;  // 27:16     
	UINT32 reg_common_3dfp_en                 : 1 ;  //    28     
} M14B0_OSD_COMMON_CTRL_3DFP_OUT_T;

/*-----------------------------------------------------------------------------
		0xc0024154L osd_common_ctrl_pq_path ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_pq_path_sel             : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_common_pq_path_en              : 1 ;  //     4     
} M14B0_OSD_COMMON_CTRL_PQ_PATH_T;

/*-----------------------------------------------------------------------------
		0xc0024158L osd_common_plte_sel ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_plte_sel                : 3 ;  //  2: 0     
} M14B0_OSD_COMMON_PLTE_SEL_T;

/*-----------------------------------------------------------------------------
		0xc002415cL osd_common_osd_valid_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_osd_valid_area_th              : 8 ;  //  7: 0     
	UINT32                                    : 8 ;  // 15: 8     reserved
	UINT32 reg_osd_valid_area_en              : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_blend_sel_out_sep_tp_a         : 8 ;  // 27:20     
	UINT32 reg_blend_sel_out_sep_tp_ma        : 1 ;  //    28     
} M14B0_OSD_COMMON_OSD_VALID_TH_T;

/*-----------------------------------------------------------------------------
		0xc0024160L osd_common_csc_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_csc_osd_sel             : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_common_csc_all_bypass          : 1 ;  //     4     
} M14B0_OSD_COMMON_CSC_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0024164L osd_common_csc_coef0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_csc0_datai              : 11;  // 10: 0     
	UINT32 reg_common_csc0_rw_n               : 1 ;  //    11     
	UINT32 reg_common_csc0_addr               : 4 ;  // 15:12     
	UINT32 reg_common_csc0_bypass             : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_common_csc0_sel                : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_common_csc0_out_ctrl           : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_common_csc0_in_ctrl            : 3 ;  // 30:28     
} M14B0_OSD_COMMON_CSC_COEF0_T;

/*-----------------------------------------------------------------------------
		0xc0024168L osd_common_csc_coef1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_csc1_datai              : 11;  // 10: 0     
	UINT32 reg_common_csc1_rw_n               : 1 ;  //    11     
	UINT32 reg_common_csc1_addr               : 4 ;  // 15:12     
	UINT32 reg_common_csc1_bypass             : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_common_csc1_sel                : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_common_csc1_out_ctrl           : 3 ;  // 26:24     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 reg_common_csc1_in_ctrl            : 3 ;  // 30:28     
} M14B0_OSD_COMMON_CSC_COEF1_T;

/*-----------------------------------------------------------------------------
		0xc002416cL osd_common_60hz_pic_timer ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_60hz_pic_init_timer            : 32;  // 31: 0     
} M14B0_OSD_COMMON_60HZ_PIC_TIMER_T;

/*-----------------------------------------------------------------------------
		0xc0024170L osd_common_sep_osd_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_common_sep_osd_hdr_en          : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_common_sep_osd_sync_sel        : 1 ;  //     4     
	UINT32                                    : 26;  // 30: 5     reserved
	UINT32 reg_common_sep_osd_bypass          : 1 ;  //    31     
} M14B0_OSD_COMMON_SEP_OSD_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0024174L osd_common_sep_osd_hdr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_0                      : 32;  // 31: 0     
} M14B0_OSD_COMMON_SEP_OSD_HDR0_T;

/*-----------------------------------------------------------------------------
		0xc0024178L osd_common_sep_osd_hdr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_1                      : 32;  // 31: 0     
} M14B0_OSD_COMMON_SEP_OSD_HDR1_T;

/*-----------------------------------------------------------------------------
		0xc002417cL osd_common_sep_osd_hdr2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_2                      : 32;  // 31: 0     
} M14B0_OSD_COMMON_SEP_OSD_HDR2_T;

/*-----------------------------------------------------------------------------
		0xc0024180L osd_common_sep_osd_hdr3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_3                      : 32;  // 31: 0     
} M14B0_OSD_COMMON_SEP_OSD_HDR3_T;

/*-----------------------------------------------------------------------------
		0xc0024184L osd_common_sep_osd_hdr4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_4                      : 32;  // 31: 0     
} M14B0_OSD_COMMON_SEP_OSD_HDR4_T;

/*-----------------------------------------------------------------------------
		0xc0024188L osd_common_sep_osd_hdr5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_5                      : 32;  // 31: 0     
} M14B0_OSD_COMMON_SEP_OSD_HDR5_T;

/*-----------------------------------------------------------------------------
		0xc002418cL osd_common_sep_osd_hdr6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_frc_hdr_6                      : 32;  // 31: 0     
} M14B0_OSD_COMMON_SEP_OSD_HDR6_T;

/*-----------------------------------------------------------------------------
		0xc0024190L osd_common_ctrl_tpg ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd_common_ctrl_tpg                : 32;  // 31: 0     
} M14B0_OSD_COMMON_CTRL_TPG_T;

/*-----------------------------------------------------------------------------
		0xc0024194L osd_common_60hz_disp_size ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_60hz_disp_size_h               : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_60hz_disp_size_w               : 12;  // 27:16     
} M14B0_OSD_COMMON_60HZ_DISP_SIZE_T;

/*-----------------------------------------------------------------------------
		0xc0024198L osd_common_60hz_ctrl_path ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 16;  // 15: 0     reserved
	UINT32 reg_60hz_path_osd1_en              : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 reg_60hz_path_osd0_en              : 1 ;  //    24     
} M14B0_OSD_COMMON_60HZ_CTRL_PATH_T;

/*-----------------------------------------------------------------------------
		0xc002419cL osd_common_60hz_osd_valid_th ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_60hz_l_osd_valid_th            : 8 ;  //  7: 0     
	UINT32 reg_60hz_l_src_sel_dn_zero         : 1 ;  //     8     
	UINT32 reg_60hz_l_src_sel_a_zero          : 1 ;  //     9     
	UINT32 reg_60hz_l_src_sel_dn              : 1 ;  //    10     
	UINT32 reg_60hz_l_src_sel_a               : 1 ;  //    11     
	UINT32 reg_60hz_l_src_sel_out             : 1 ;  //    12     
	UINT32 reg_60hz_l_mk_color                : 3 ;  // 15:13     
	UINT32 reg_60hz_r_osd_valid_th            : 8 ;  // 23:16     
	UINT32 reg_60hz_r_src_sel_dn_zero         : 1 ;  //    24     
	UINT32 reg_60hz_r_src_sel_a_zero          : 1 ;  //    25     
	UINT32 reg_60hz_r_src_sel_dn              : 1 ;  //    26     
	UINT32 reg_60hz_r_src_sel_a               : 1 ;  //    27     
	UINT32 reg_60hz_r_src_sel_out             : 1 ;  //    28     
	UINT32 reg_60hz_r_mk_color                : 3 ;  // 31:29     
} M14B0_OSD_COMMON_60HZ_OSD_VALID_TH_T;

/*-----------------------------------------------------------------------------
		0xc00241a0L osd_mixer_ctrl_onoff ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_osd3_mixer_en                  : 1 ;  //     0     
	UINT32                                    : 7 ;  //  7: 1     reserved
	UINT32 reg_osd2_mixer_en                  : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 reg_osd1_mixer_en                  : 1 ;  //    16     
	UINT32                                    : 7 ;  // 23:17     reserved
	UINT32 reg_osd0_mixer_en                  : 1 ;  //    24     
} M14B0_OSD_MIXER_CTRL_ONOFF_T;

/*-----------------------------------------------------------------------------
		0xc00241a4L osd_mixer_ctrl_output ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_out_b                    : 8 ;  //  7: 0     
	UINT32 reg_mixer_out_g                    : 8 ;  // 15: 8     
	UINT32 reg_mixer_out_r                    : 8 ;  // 23:16     
	UINT32 reg_mixer_out_fpr_mask_en          : 1 ;  //    24     
	UINT32 reg_mixer_out_fpr_mask_odd         : 1 ;  //    25     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_mixer_out_en                   : 1 ;  //    28     
	UINT32 reg_mixer_out_valid_en             : 1 ;  //    29     
	UINT32 reg_mixer_out_vid_de_only          : 1 ;  //    30     
	UINT32 reg_mixer_out_vid_init_en          : 1 ;  //    31     
} M14B0_OSD_MIXER_CTRL_OUTPUT_T;

/*-----------------------------------------------------------------------------
		0xc00241a8L osd_mixer_ctrl_awb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_osd3_mixer_awb_b               : 1 ;  //     0     
	UINT32 reg_osd3_mixer_awb_g               : 1 ;  //     1     
	UINT32 reg_osd3_mixer_awb_r               : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_osd2_mixer_awb_b               : 1 ;  //     4     
	UINT32 reg_osd2_mixer_awb_g               : 1 ;  //     5     
	UINT32 reg_osd2_mixer_awb_r               : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_osd1_mixer_awb_b               : 1 ;  //     8     
	UINT32 reg_osd1_mixer_awb_g               : 1 ;  //     9     
	UINT32 reg_osd1_mixer_awb_r               : 1 ;  //    10     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_osd0_mixer_awb_b               : 1 ;  //    12     
	UINT32 reg_osd0_mixer_awb_g               : 1 ;  //    13     
	UINT32 reg_osd0_mixer_awb_r               : 1 ;  //    14     
} M14B0_OSD_MIXER_CTRL_AWB_T;

/*-----------------------------------------------------------------------------
		0xc00241acL osd_mixer_ctrl_sync ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sync_osd3_tp_en                : 1 ;  //     0     
	UINT32 reg_sync_osd3_tp_aa_en             : 1 ;  //     1     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_sync_osd2_tp_en                : 1 ;  //     4     
	UINT32 reg_sync_osd2_tp_aa_en             : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_sync_osd1_tp_en                : 1 ;  //     8     
	UINT32 reg_sync_osd1_tp_aa_en             : 1 ;  //     9     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_sync_osd0_tp_en                : 1 ;  //    12     
	UINT32 reg_sync_osd0_tp_aa_en             : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_sync_tp_alpha                  : 8 ;  // 23:16     
	UINT32                                    : 7 ;  // 30:24     reserved
	UINT32 reg_sync_v_reverse                 : 1 ;  //    31     
} M14B0_OSD_MIXER_CTRL_SYNC_T;

/*-----------------------------------------------------------------------------
		0xc00241b0L osd_mixer_ctrl_blend ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_blend_sel_0v_dn                : 1 ;  //     0     
	UINT32 reg_blend_sel_0v_up                : 1 ;  //     1     
	UINT32 reg_blend_sel_10_dn                : 1 ;  //     2     
	UINT32 reg_blend_sel_10_up                : 1 ;  //     3     
	UINT32                                    : 2 ;  //  5: 4     reserved
	UINT32 reg_blend_sel_31_dn                : 1 ;  //     6     
	UINT32 reg_blend_sel_31_up                : 1 ;  //     7     
	UINT32 reg_blend_div_dvdd                 : 1 ;  //     8     
	UINT32 reg_blend_div_dvsr                 : 1 ;  //     9     
	UINT32 reg_blend_sel_a10_dn               : 1 ;  //    10     
	UINT32 reg_blend_sel_a10_a                : 1 ;  //    11     
	UINT32                                    : 2 ;  // 13:12     reserved
	UINT32 reg_blend_sel_a31_dn               : 1 ;  //    14     
	UINT32 reg_blend_sel_a31_a                : 1 ;  //    15     
	UINT32 reg_blend_sel_v3d_src              : 1 ;  //    16     
	UINT32 reg_blend_sel_v5d_src              : 1 ;  //    17     
	UINT32 reg_blend_sel_v7d_src              : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_blend_sel_vid_src              : 1 ;  //    20     
	UINT32 reg_blend_sel_31_dn_src            : 1 ;  //    21     
	UINT32 reg_blend_sel_div_pel_src          : 1 ;  //    22     
	UINT32 reg_blend_sel_out_sepc_src         : 1 ;  //    23     
	UINT32 reg_blend_sel_a3d_src              : 1 ;  //    24     
	UINT32 reg_blend_sel_a5d_src              : 1 ;  //    25     
	UINT32                                    : 1 ;  //    26     reserved
	UINT32 reg_blend_sel_a31_z_3d_src         : 1 ;  //    27     
	UINT32 reg_blend_sel_out_vid              : 1 ;  //    28     
	UINT32 reg_blend_sel_out_sepc             : 1 ;  //    29     
	UINT32 reg_blend_sel_out_sepa             : 1 ;  //    30     
	UINT32 reg_blend_sel_out_sep_tp_en        : 1 ;  //    31     
} M14B0_OSD_MIXER_CTRL_BLEND_T;

/*-----------------------------------------------------------------------------
		0xc00241b4L osd_mixer_60hz_ctrl_awb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 1 ;  //     0     reserved
	UINT32 reg_awb_l_0                        : 3 ;  //  3: 1     
	UINT32 reg_vid_a_l                        : 8 ;  // 11: 4     
	UINT32 reg_osd0_csc_en                    : 1 ;  //    12     
	UINT32 reg_awb_l_1                        : 3 ;  // 15:13     
	UINT32                                    : 1 ;  //    16     reserved
	UINT32 reg_awb_r_0                        : 3 ;  // 19:17     
	UINT32 reg_vid_r_a                        : 8 ;  // 27:20     
	UINT32 reg_osd1_csc_en                    : 1 ;  //    28     
	UINT32 reg_awb_r_1                        : 3 ;  // 31:29     
} M14B0_OSD_MIXER_60HZ_CTRL_AWB_T;

/*-----------------------------------------------------------------------------
		0xc00241b8L osd_mixer_60hz_ctrl_blend ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_src_sel_l_0v_dn_zero           : 1 ;  //     0     
	UINT32 reg_src_sel_l_0v_up_zero           : 1 ;  //     1     
	UINT32 reg_src_sel_l_10_dn_zero           : 1 ;  //     2     
	UINT32 reg_src_sel_l_10_up_zero           : 1 ;  //     3     
	UINT32 reg_src_sel_l_0v_dn_osd0           : 1 ;  //     4     
	UINT32 reg_src_sel_l_0v_up_osd1           : 1 ;  //     5     
	UINT32 reg_src_sel_l_10_dn_video          : 1 ;  //     6     
	UINT32 reg_src_sel_l_10_up_osd0           : 1 ;  //     7     
	UINT32 reg_src_sel_l_0v_dn_osd1           : 1 ;  //     8     
	UINT32 reg_src_sel_l_0v_up_video          : 1 ;  //     9     
	UINT32 reg_src_sel_l_10_dn_osd1           : 1 ;  //    10     
	UINT32 reg_src_sel_l_10_up_video          : 1 ;  //    11     
	UINT32 reg_src_sel_l_vd_7d_0v_z           : 1 ;  //    12     
	UINT32 reg_src_sel_l_vd_mk_vd8d           : 1 ;  //    13     
	UINT32 reg_src_sel_l_vd_out_mk            : 1 ;  //    14     
	UINT32 reg_src_sel_l_vd_out_zero          : 1 ;  //    15     
	UINT32 reg_src_sel_r_0v_dn_zero           : 1 ;  //    16     
	UINT32 reg_src_sel_r_0v_up_zero           : 1 ;  //    17     
	UINT32 reg_src_sel_r_10_dn_zero           : 1 ;  //    18     
	UINT32 reg_src_sel_r_10_up_zero           : 1 ;  //    19     
	UINT32 reg_src_sel_r_0v_dn_osd0           : 1 ;  //    20     
	UINT32 reg_src_sel_r_0v_up_osd1           : 1 ;  //    21     
	UINT32 reg_src_sel_r_10_dn_video          : 1 ;  //    22     
	UINT32 reg_src_sel_r_10_up_osd0           : 1 ;  //    23     
	UINT32 reg_src_sel_r_0v_dn_osd1           : 1 ;  //    24     
	UINT32 reg_src_sel_r_0v_up_video          : 1 ;  //    25     
	UINT32 reg_src_sel_r_10_dn_osd1           : 1 ;  //    26     
	UINT32 reg_src_sel_r_10_up_video          : 1 ;  //    27     
	UINT32 reg_src_sel_r_vd_7d_0v_z           : 1 ;  //    28     
	UINT32 reg_src_sel_r_vd_mk_vd8d           : 1 ;  //    29     
	UINT32 reg_src_sel_r_vd_out_mk            : 1 ;  //    30     
	UINT32 reg_src_sel_r_vd_out_zero          : 1 ;  //    31     
} M14B0_OSD_MIXER_60HZ_CTRL_BLEND_T;

/*-----------------------------------------------------------------------------
		0xc00241bcL osd_mixer_ctrl_hole ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hole2_en                      : 1 ;  //     0     
	UINT32 osd3_hole1_en                      : 1 ;  //     1     
	UINT32 osd3_hole0_en                      : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 osd3_hole_en                       : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 osd2_hole2_en                      : 1 ;  //     8     
	UINT32 osd2_hole1_en                      : 1 ;  //     9     
	UINT32 osd2_hole0_en                      : 1 ;  //    10     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 osd2_hole_en                       : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 osd1_hole2_en                      : 1 ;  //    16     
	UINT32 osd1_hole1_en                      : 1 ;  //    17     
	UINT32 osd1_hole0_en                      : 1 ;  //    18     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 osd1_hole_en                       : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 osd0_hole2_en                      : 1 ;  //    24     
	UINT32 osd0_hole1_en                      : 1 ;  //    25     
	UINT32 osd0_hole0_en                      : 1 ;  //    26     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 osd0_hole_en                       : 1 ;  //    28     
} M14B0_OSD_MIXER_CTRL_HOLE_T;

/*-----------------------------------------------------------------------------
		0xc00241c0L osd_mixer_ctrl_mux0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 layer_mux_3                        : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 layer_mux_2                        : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 layer_mux_1                        : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 layer_mux_0                        : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 video_layer_alpha                  : 8 ;  // 23:16     
	UINT32                                    : 4 ;  // 27:24     reserved
	UINT32 video_layer_off                    : 1 ;  //    28     
} M14B0_OSD_MIXER_CTRL_MUX0_T;

/*-----------------------------------------------------------------------------
		0xc00241c4L osd_mixer_ctrl_mux1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window0_en               : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 reg_mixer_window0_alpha            : 8 ;  // 23:16     
} M14B0_OSD_MIXER_CTRL_MUX1_T;

/*-----------------------------------------------------------------------------
		0xc00241c8L osd_mixer_ctrl_mux2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window0_ypos             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_mixer_window0_xpos             : 12;  // 27:16     
} M14B0_OSD_MIXER_CTRL_MUX2_T;

/*-----------------------------------------------------------------------------
		0xc00241ccL osd_mixer_ctrl_mux3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window0_h                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_mixer_window0_w                : 12;  // 27:16     
} M14B0_OSD_MIXER_CTRL_MUX3_T;

/*-----------------------------------------------------------------------------
		0xc00241d0L osd_mixer_ctrl_mux4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window1_en               : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 reg_mixer_window1_alpha            : 8 ;  // 23:16     
} M14B0_OSD_MIXER_CTRL_MUX4_T;

/*-----------------------------------------------------------------------------
		0xc00241d4L osd_mixer_ctrl_mux5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window1_ypos             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_mixer_window1_xpos             : 12;  // 27:16     
} M14B0_OSD_MIXER_CTRL_MUX5_T;

/*-----------------------------------------------------------------------------
		0xc00241d8L osd_mixer_ctrl_mux6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window1_h                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_mixer_window1_w                : 12;  // 27:16     
} M14B0_OSD_MIXER_CTRL_MUX6_T;

/*-----------------------------------------------------------------------------
		0xc00241dcL osd_mixer_ctrl_mux7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window2_en               : 1 ;  //     0     
	UINT32                                    : 15;  // 15: 1     reserved
	UINT32 reg_mixer_window2_alpha            : 8 ;  // 23:16     
} M14B0_OSD_MIXER_CTRL_MUX7_T;

/*-----------------------------------------------------------------------------
		0xc00241e0L osd_mixer_ctrl_mux8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window2_ypos             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_mixer_window2_xpos             : 12;  // 27:16     
} M14B0_OSD_MIXER_CTRL_MUX8_T;

/*-----------------------------------------------------------------------------
		0xc00241e4L osd_mixer_ctrl_mux9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mixer_window2_h                : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_mixer_window2_w                : 12;  // 27:16     
} M14B0_OSD_MIXER_CTRL_MUX9_T;

/*-----------------------------------------------------------------------------
		0xc00241e8L osd_mixer_ctrl_muxa ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hole0_y                        : 12;  // 11: 0     
	UINT32 reg_hole0_x                        : 12;  // 23:12     
	UINT32 reg_hole0_reverse_en               : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_hole0_en                       : 1 ;  //    28     
} M14B0_OSD_MIXER_CTRL_MUXA_T;

/*-----------------------------------------------------------------------------
		0xc00241ecL osd_mixer_ctrl_muxb ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hole0_h                        : 12;  // 11: 0     
	UINT32 reg_hole0_w                        : 12;  // 23:12     
	UINT32 reg_hole0_a                        : 8 ;  // 31:24     
} M14B0_OSD_MIXER_CTRL_MUXB_T;

/*-----------------------------------------------------------------------------
		0xc00241f0L osd_mixer_ctrl_muxc ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hole1_y                        : 12;  // 11: 0     
	UINT32 reg_hole1_x                        : 12;  // 23:12     
	UINT32 reg_hole1_reverse_en               : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_hole1_en                       : 1 ;  //    28     
} M14B0_OSD_MIXER_CTRL_MUXC_T;

/*-----------------------------------------------------------------------------
		0xc00241f4L osd_mixer_ctrl_muxd ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hole1_h                        : 12;  // 11: 0     
	UINT32 reg_hole1_w                        : 12;  // 23:12     
	UINT32 reg_hole1_a                        : 8 ;  // 31:24     
} M14B0_OSD_MIXER_CTRL_MUXD_T;

/*-----------------------------------------------------------------------------
		0xc00241f8L osd_mixer_ctrl_muxe ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hole2_y                        : 12;  // 11: 0     
	UINT32 reg_hole2_x                        : 12;  // 23:12     
	UINT32 reg_hole2_reverse_en               : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_hole2_en                       : 1 ;  //    28     
} M14B0_OSD_MIXER_CTRL_MUXE_T;

/*-----------------------------------------------------------------------------
		0xc00241fcL osd_mixer_ctrl_muxf ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hole2_h                        : 12;  // 11: 0     
	UINT32 reg_hole2_w                        : 12;  // 23:12     
	UINT32 reg_hole2_a                        : 8 ;  // 31:24     
} M14B0_OSD_MIXER_CTRL_MUXF_T;

/*-----------------------------------------------------------------------------
		0xc0024200L osd0_ctrl_main ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_en                            : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 osd0_hdr_src_sel                   : 1 ;  //     4     
	UINT32 osd0_unaligned_addr_en             : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 osd0_v_reverse_en                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd0_s3d_en                        : 1 ;  //    12     
	UINT32 osd0_s3d_v_reverse_en              : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 osd0_manual_line_inc               : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 osd0_manual_line_inc_en            : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 osd0_yuyv_mode_en                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 osd0_hdr_ready_flag_en             : 1 ;  //    28     
	UINT32 osd0_hdr_ready_flag                : 1 ;  //    29     
} M14B0_OSD0_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
		0xc0024204L osd0_base_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_base_addr                     : 32;  // 31: 0     
} M14B0_OSD0_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
		0xc0024208L osd0_ctrl_swap ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_bbs                           : 1 ;  //     0     
	UINT32 osd0_bqws                          : 1 ;  //     1     
	UINT32 osd0_bhws                          : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 osd0_pbs                           : 1 ;  //     4     
	UINT32 osd0_pqws                          : 1 ;  //     5     
	UINT32 osd0_phws                          : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd0_hbs                           : 1 ;  //     8     
	UINT32 osd0_hqws                          : 1 ;  //     9     
	UINT32 osd0_hhws                          : 1 ;  //    10     
} M14B0_OSD0_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
		0xc002420cL osd0_ctrl_pdec ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_lfsr_seed                     : 19;  // 18: 0     
	UINT32 osd0_pure_ck                       : 1 ;  //    19     
	UINT32 osd0_round                         : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 osd0_ch_blue                       : 2 ;  // 25:24     
	UINT32 osd0_ch_green                      : 2 ;  // 27:26     
	UINT32 osd0_ch_red                        : 2 ;  // 29:28     
	UINT32 osd0_ch_alpha                      : 2 ;  // 31:30     
} M14B0_OSD0_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
		0xc0024210L osd0_ctrl_scaler ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_sc_bc_sa                      : 2 ;  //  1: 0     
	UINT32 osd0_sc_force_vsize_en             : 1 ;  //     2     
	UINT32 osd0_sc_bilinear                   : 1 ;  //     3     
	UINT32 osd0_sc_vdouble                    : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 osd0_sc_v_buf_en                   : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd0_sc_sampling_mode              : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd0_sc_bnd_mode                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 osd0_sc_h_phase_off                : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 osd0_sc_v_phase_off                : 6 ;  // 29:24     
	UINT32 osd0_odd_width_en                  : 1 ;  //    30     
	UINT32 osd0_fifo_en                       : 1 ;  //    31     
} M14B0_OSD0_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
		0xc0024214L osd0_ctrl_3d_disparity ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_r_3d_disparity_value          : 8 ;  //  7: 0     
	UINT32 osd0_r_3d_disparity_sign           : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 osd0_l_3d_disparity_value          : 8 ;  // 23:16     
	UINT32 osd0_l_3d_disparity_sign           : 1 ;  //    24     
} M14B0_OSD0_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
		0xc0024218L osd0_ctrl_3d_blank ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_ctrl_3d_blank                 : 32;  // 31: 0     
} M14B0_OSD0_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
		0xc002421cL osd0_info_line_memory ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_info_line_memory              : 16;  // 15: 0     
	UINT32 osd0_info_line_memory_min          : 16;  // 31:16     
} M14B0_OSD0_INFO_LINE_MEMORY_T;

/*-----------------------------------------------------------------------------
		0xc0024220L osd0_ctrl_hdr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ypos                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd0_hdr_xpos                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd0_hdr_color_key_en              : 1 ;  //    31     
} M14B0_OSD0_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
		0xc0024224L osd0_ctrl_hdr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_h_mem                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd0_hdr_w_mem                     : 12;  // 27:16     
} M14B0_OSD0_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
		0xc0024228L osd0_ctrl_hdr2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_h_out                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd0_hdr_w_out                     : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd0_hdr_pixel_order               : 1 ;  //    31     
} M14B0_OSD0_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
		0xc002422cL osd0_ctrl_hdr3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_wpl                       : 16;  // 15: 0     
	UINT32 osd0_hdr_global_alpha              : 8 ;  // 23:16     
	UINT32 osd0_hdr_format                    : 4 ;  // 27:24     
	UINT32 osd0_hdr_depth                     : 3 ;  // 30:28     
	UINT32 osd0_hdr_global_alpha_en           : 1 ;  //    31     
} M14B0_OSD0_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
		0xc0024230L osd0_ctrl_hdr4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_color_key                 : 32;  // 31: 0     
} M14B0_OSD0_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
		0xc0024234L osd0_ctrl_hdr5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ptr_plte                  : 32;  // 31: 0     
} M14B0_OSD0_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
		0xc0024238L osd0_ctrl_hdr6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd0_hdr_ptr_bmp                   : 32;  // 31: 0     
} M14B0_OSD0_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
		0xc0024240L osd1_ctrl_main ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_en                            : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 osd1_hdr_src_sel                   : 1 ;  //     4     
	UINT32 osd1_unaligned_addr_en             : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 osd1_v_reverse_en                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd1_s3d_en                        : 1 ;  //    12     
	UINT32 osd1_s3d_v_reverse_en              : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 osd1_manual_line_inc               : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 osd1_manual_line_inc_en            : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 osd1_yuyv_mode_en                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 osd1_hdr_ready_flag_en             : 1 ;  //    28     
	UINT32 osd1_hdr_ready_flag                : 1 ;  //    29     
} M14B0_OSD1_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
		0xc0024244L osd1_ctrl_base_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_base_addr                     : 32;  // 31: 0     
} M14B0_OSD1_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
		0xc0024248L osd1_ctrl_swap ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_bbs                           : 1 ;  //     0     
	UINT32 osd1_bqws                          : 1 ;  //     1     
	UINT32 osd1_bhws                          : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 osd1_pbs                           : 1 ;  //     4     
	UINT32 osd1_pqws                          : 1 ;  //     5     
	UINT32 osd1_phws                          : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd1_hbs                           : 1 ;  //     8     
	UINT32 osd1_hqws                          : 1 ;  //     9     
	UINT32 osd1_hhws                          : 1 ;  //    10     
} M14B0_OSD1_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
		0xc002424cL osd1_ctrl_pdec ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_lfsr_seed                     : 19;  // 18: 0     
	UINT32 osd1_pure_ck                       : 1 ;  //    19     
	UINT32 osd1_round                         : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 osd1_ch_blue                       : 2 ;  // 25:24     
	UINT32 osd1_ch_green                      : 2 ;  // 27:26     
	UINT32 osd1_ch_red                        : 2 ;  // 29:28     
	UINT32 osd1_ch_alpha                      : 2 ;  // 31:30     
} M14B0_OSD1_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
		0xc0024250L osd1_ctrl_scaler ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_sc_bc_sa                      : 2 ;  //  1: 0     
	UINT32 osd0_sc_force_vsize_en             : 1 ;  //     2     
	UINT32 osd1_sc_bilinear                   : 1 ;  //     3     
	UINT32 osd1_sc_vdouble                    : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 osd1_sc_v_buf_en                   : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd1_sc_sampling_mode              : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd1_sc_bnd_mode                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 osd1_sc_h_phase_off                : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 osd1_sc_v_phase_off                : 6 ;  // 29:24     
	UINT32 osd1_odd_width_en                  : 1 ;  //    30     
	UINT32 osd1_fifo_en                       : 1 ;  //    31     
} M14B0_OSD1_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
		0xc0024254L osd1_ctrl_3d_disparity ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_r_3d_disparity_value          : 8 ;  //  7: 0     
	UINT32 osd1_r_3d_disparity_sign           : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 osd1_l_3d_disparity_value          : 8 ;  // 23:16     
	UINT32 osd1_l_3d_disparity_sign           : 1 ;  //    24     
} M14B0_OSD1_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
		0xc0024258L osd1_ctrl_3d_blank ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_ctrl_3d_blank                 : 32;  // 31: 0     
} M14B0_OSD1_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
		0xc002425cL osd1_info_line_memory ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_info_line_memory              : 16;  // 15: 0     
	UINT32 osd1_info_line_memory_min          : 16;  // 31:16     
} M14B0_OSD1_INFO_LINE_MEMORY_T;

/*-----------------------------------------------------------------------------
		0xc0024260L osd1_ctrl_hdr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ypos                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd1_hdr_xpos                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd1_hdr_color_key_en              : 1 ;  //    31     
} M14B0_OSD1_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
		0xc0024264L osd1_ctrl_hdr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_h_mem                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd1_hdr_w_mem                     : 12;  // 27:16     
} M14B0_OSD1_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
		0xc0024268L osd1_ctrl_hdr2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_h_out                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd1_hdr_w_out                     : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd1_hdr_pixel_order               : 1 ;  //    31     
} M14B0_OSD1_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
		0xc002426cL osd1_ctrl_hdr3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_wpl                       : 16;  // 15: 0     
	UINT32 osd1_hdr_global_alpha              : 8 ;  // 23:16     
	UINT32 osd1_hdr_format                    : 4 ;  // 27:24     
	UINT32 osd1_hdr_depth                     : 3 ;  // 30:28     
	UINT32 osd1_hdr_global_alpha_en           : 1 ;  //    31     
} M14B0_OSD1_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
		0xc0024270L osd1_ctrl_hdr4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_color_key                 : 32;  // 31: 0     
} M14B0_OSD1_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
		0xc0024274L osd1_ctrl_hdr5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ptr_plte                  : 32;  // 31: 0     
} M14B0_OSD1_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
		0xc0024278L osd1_ctrl_hdr6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd1_hdr_ptr_bmp                   : 32;  // 31: 0     
} M14B0_OSD1_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
		0xc0024280L osd2_ctrl_main ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_en                            : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 osd2_hdr_src_sel                   : 1 ;  //     4     
	UINT32 osd2_unaligned_addr_en             : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 osd2_v_reverse_en                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd2_s3d_en                        : 1 ;  //    12     
	UINT32 osd2_s3d_v_reverse_en              : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 osd2_manual_line_inc               : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 osd2_manual_line_inc_en            : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 osd2_yuyv_mode_en                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 osd2_hdr_ready_flag_en             : 1 ;  //    28     
	UINT32 osd2_hdr_ready_flag                : 1 ;  //    29     
} M14B0_OSD2_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
		0xc0024284L osd2_ctrl_base_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_base_addr                     : 32;  // 31: 0     
} M14B0_OSD2_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
		0xc0024288L osd2_ctrl_swap ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_bbs                           : 1 ;  //     0     
	UINT32 osd2_bqws                          : 1 ;  //     1     
	UINT32 osd2_bhws                          : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 osd2_pbs                           : 1 ;  //     4     
	UINT32 osd2_pqws                          : 1 ;  //     5     
	UINT32 osd2_phws                          : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd2_hbs                           : 1 ;  //     8     
	UINT32 osd2_hqws                          : 1 ;  //     9     
	UINT32 osd2_hhws                          : 1 ;  //    10     
} M14B0_OSD2_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
		0xc002428cL osd2_ctrl_pdec ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_lfsr_seed                     : 19;  // 18: 0     
	UINT32 osd2_pure_ck                       : 1 ;  //    19     
	UINT32 osd2_round                         : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 osd2_ch_blue                       : 2 ;  // 25:24     
	UINT32 osd2_ch_green                      : 2 ;  // 27:26     
	UINT32 osd2_ch_red                        : 2 ;  // 29:28     
	UINT32 osd2_ch_alpha                      : 2 ;  // 31:30     
} M14B0_OSD2_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
		0xc0024290L osd2_ctrl_scaler ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_sc_bc_sa                      : 2 ;  //  1: 0     
	UINT32 osd0_sc_force_vsize_en             : 1 ;  //     2     
	UINT32 osd2_sc_bilinear                   : 1 ;  //     3     
	UINT32 osd2_sc_vdouble                    : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 osd2_sc_v_buf_en                   : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd2_sc_sampling_mode              : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd2_sc_bnd_mode                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 osd2_sc_h_phase_off                : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 osd2_sc_v_phase_off                : 6 ;  // 29:24     
	UINT32 osd2_odd_width_en                  : 1 ;  //    30     
	UINT32 osd2_fifo_en                       : 1 ;  //    31     
} M14B0_OSD2_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
		0xc0024294L osd2_ctrl_3d_disparity ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_r_3d_disparity_value          : 8 ;  //  7: 0     
	UINT32 osd2_r_3d_disparity_sign           : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 osd2_l_3d_disparity_value          : 8 ;  // 23:16     
	UINT32 osd2_l_3d_disparity_sign           : 1 ;  //    24     
} M14B0_OSD2_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
		0xc0024298L osd2_ctrl_3d_blank ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_ctrl_3d_blank                 : 32;  // 31: 0     
} M14B0_OSD2_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
		0xc002429cL osd2_info_line_memory ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_info_line_memory              : 16;  // 15: 0     
	UINT32 osd2_info_line_memory_min          : 16;  // 31:16     
} M14B0_OSD2_INFO_LINE_MEMORY_T;

/*-----------------------------------------------------------------------------
		0xc00242a0L osd2_ctrl_hdr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ypos                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd2_hdr_xpos                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd2_hdr_color_key_en              : 1 ;  //    31     
} M14B0_OSD2_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
		0xc00242a4L osd2_ctrl_hdr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_h_mem                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd2_hdr_w_mem                     : 12;  // 27:16     
} M14B0_OSD2_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
		0xc00242a8L osd2_ctrl_hdr2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_h_out                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd2_hdr_w_out                     : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd2_hdr_pixel_order               : 1 ;  //    31     
} M14B0_OSD2_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
		0xc00242acL osd2_ctrl_hdr3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_wpl                       : 16;  // 15: 0     
	UINT32 osd2_hdr_global_alpha              : 8 ;  // 23:16     
	UINT32 osd2_hdr_format                    : 4 ;  // 27:24     
	UINT32 osd2_hdr_depth                     : 3 ;  // 30:28     
	UINT32 osd2_hdr_global_alpha_en           : 1 ;  //    31     
} M14B0_OSD2_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
		0xc00242b0L osd2_ctrl_hdr4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_color_key                 : 32;  // 31: 0     
} M14B0_OSD2_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
		0xc00242b4L osd2_ctrl_hdr5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ptr_plte                  : 32;  // 31: 0     
} M14B0_OSD2_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
		0xc00242b8L osd2_ctrl_hdr6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd2_hdr_ptr_bmp                   : 32;  // 31: 0     
} M14B0_OSD2_CTRL_HDR6_T;

/*-----------------------------------------------------------------------------
		0xc00242c0L osd3_ctrl_main ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_en                            : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 osd3_hdr_src_sel                   : 1 ;  //     4     
	UINT32 osd3_unaligned_addr_en             : 1 ;  //     5     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 osd3_v_reverse_en                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd3_s3d_en                        : 1 ;  //    12     
	UINT32 osd3_s3d_v_reverse_en              : 1 ;  //    13     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 osd3_manual_line_inc               : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 osd3_manual_line_inc_en            : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 osd3_yuyv_mode_en                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 osd3_hdr_ready_flag_en             : 1 ;  //    28     
	UINT32 osd3_hdr_ready_flag                : 1 ;  //    29     
} M14B0_OSD3_CTRL_MAIN_T;

/*-----------------------------------------------------------------------------
		0xc00242c4L osd3_ctrl_base_addr ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_base_addr                     : 32;  // 31: 0     
} M14B0_OSD3_CTRL_BASE_ADDR_T;

/*-----------------------------------------------------------------------------
		0xc00242c8L osd3_ctrl_swap ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_bbs                           : 1 ;  //     0     
	UINT32 osd3_bqws                          : 1 ;  //     1     
	UINT32 osd3_bhws                          : 1 ;  //     2     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 osd3_pbs                           : 1 ;  //     4     
	UINT32 osd3_pqws                          : 1 ;  //     5     
	UINT32 osd3_phws                          : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd3_hbs                           : 1 ;  //     8     
	UINT32 osd3_hqws                          : 1 ;  //     9     
	UINT32 osd3_hhws                          : 1 ;  //    10     
} M14B0_OSD3_CTRL_SWAP_T;

/*-----------------------------------------------------------------------------
		0xc00242ccL osd3_ctrl_pdec ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_lfsr_seed                     : 19;  // 18: 0     
	UINT32 osd3_pure_ck                       : 1 ;  //    19     
	UINT32 osd3_round                         : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 osd3_ch_blue                       : 2 ;  // 25:24     
	UINT32 osd3_ch_green                      : 2 ;  // 27:26     
	UINT32 osd3_ch_red                        : 2 ;  // 29:28     
	UINT32 osd3_ch_alpha                      : 2 ;  // 31:30     
} M14B0_OSD3_CTRL_PDEC_T;

/*-----------------------------------------------------------------------------
		0xc00242d0L osd3_ctrl_scaler ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_sc_bc_sa                      : 2 ;  //  1: 0     
	UINT32 osd0_sc_force_vsize_en             : 1 ;  //     2     
	UINT32 osd3_sc_bilinear                   : 1 ;  //     3     
	UINT32 osd3_sc_vdouble                    : 1 ;  //     4     
	UINT32                                    : 1 ;  //     5     reserved
	UINT32 osd3_sc_v_buf_en                   : 1 ;  //     6     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 osd3_sc_sampling_mode              : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 osd3_sc_bnd_mode                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 osd3_sc_h_phase_off                : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 osd3_sc_v_phase_off                : 6 ;  // 29:24     
	UINT32 osd3_odd_width_en                  : 1 ;  //    30     
	UINT32 osd3_fifo_en                       : 1 ;  //    31     
} M14B0_OSD3_CTRL_SCALER_T;

/*-----------------------------------------------------------------------------
		0xc00242d4L osd3_ctrl_3d_disparity ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_r_3d_disparity_value          : 8 ;  //  7: 0     
	UINT32 osd3_r_3d_disparity_sign           : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 osd3_l_3d_disparity_value          : 8 ;  // 23:16     
	UINT32 osd3_l_3d_disparity_sign           : 1 ;  //    24     
} M14B0_OSD3_CTRL_3D_DISPARITY_T;

/*-----------------------------------------------------------------------------
		0xc00242d8L osd3_ctrl_3d_blank ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_ctrl_3d_blank                 : 32;  // 31: 0     
} M14B0_OSD3_CTRL_3D_BLANK_T;

/*-----------------------------------------------------------------------------
		0xc00242dcL osd3_info_line_memory ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_info_line_memory              : 16;  // 15: 0     
	UINT32 osd3_info_line_memory_min          : 16;  // 31:16     
} M14B0_OSD3_INFO_LINE_MEMORY_T;

/*-----------------------------------------------------------------------------
		0xc00242e0L osd3_ctrl_hdr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ypos                      : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd3_hdr_xpos                      : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd3_hdr_color_key_en              : 1 ;  //    31     
} M14B0_OSD3_CTRL_HDR0_T;

/*-----------------------------------------------------------------------------
		0xc00242e4L osd3_ctrl_hdr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_h_mem                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd3_hdr_w_mem                     : 12;  // 27:16     
} M14B0_OSD3_CTRL_HDR1_T;

/*-----------------------------------------------------------------------------
		0xc00242e8L osd3_ctrl_hdr2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_h_out                     : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 osd3_hdr_w_out                     : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 osd3_hdr_pixel_order               : 1 ;  //    31     
} M14B0_OSD3_CTRL_HDR2_T;

/*-----------------------------------------------------------------------------
		0xc00242ecL osd3_ctrl_hdr3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_wpl                       : 16;  // 15: 0     
	UINT32 osd3_hdr_global_alpha              : 8 ;  // 23:16     
	UINT32 osd3_hdr_format                    : 4 ;  // 27:24     
	UINT32 osd3_hdr_depth                     : 3 ;  // 30:28     
	UINT32 osd3_hdr_global_alpha_en           : 1 ;  //    31     
} M14B0_OSD3_CTRL_HDR3_T;

/*-----------------------------------------------------------------------------
		0xc00242f0L osd3_ctrl_hdr4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_color_key                 : 32;  // 31: 0     
} M14B0_OSD3_CTRL_HDR4_T;

/*-----------------------------------------------------------------------------
		0xc00242f4L osd3_ctrl_hdr5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ptr_plte                  : 32;  // 31: 0     
} M14B0_OSD3_CTRL_HDR5_T;

/*-----------------------------------------------------------------------------
		0xc00242f8L osd3_ctrl_hdr6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 osd3_hdr_ptr_bmp                   : 32;  // 31: 0     
} M14B0_OSD3_CTRL_HDR6_T;

typedef struct {
	M14B0_OSD_STATUS_MON_CFG_T         	osd_status_mon_cfg;           //0xc0024100L
	M14B0_OSD_STATUS_MON_POS_T         	osd_status_mon_pos;           //0xc0024104L
	M14B0_OSD_STATUS_VERSION_T         	osd_status_version;           //0xc0024108L
	M14B0_OSD_STATUS_MIXER_LINE_T      	osd_status_mixer_line;        //0xc002410cL
	M14B0_OSD_STATUS_MON_DATA0_T       	osd_status_mon_data0;         //0xc0024110L
	M14B0_OSD_STATUS_MON_DATA1_T       	osd_status_mon_data1;         //0xc0024114L
	M14B0_OSD_STATUS_MON_DATA2_T       	osd_status_mon_data2;         //0xc0024118L
	M14B0_OSD_STATUS_MON_DATA3_T       	osd_status_mon_data3;         //0xc002411cL
	M14B0_OSD_STATUS_MON_DATA4_T       	osd_status_mon_data4;         //0xc0024120L
	M14B0_OSD_STATUS_MON_DATA5_T       	osd_status_mon_data5;         //0xc0024124L
	M14B0_OSD_STATUS_MON_DATA6_T       	osd_status_mon_data6;         //0xc0024128L
	M14B0_OSD_STATUS_MON_DATA7_T       	osd_status_mon_data7;         //0xc002412cL
	M14B0_OSD_STATUS_MON_DATA8_T       	osd_status_mon_data8;         //0xc0024130L
	M14B0_OSD_STATUS_MON_DATA9_T       	osd_status_mon_data9;         //0xc0024134L
	M14B0_OSD_STATUS_MON_DATAA_T       	osd_status_mon_dataa;         //0xc0024138L
	M14B0_OSD_STATUS_MON_DATAB_T       	osd_status_mon_datab;         //0xc002413cL
	M14B0_OSD_COMMON_DISP_SIZE_T       	osd_common_disp_size;         //0xc0024140L
	M14B0_OSD_COMMON_CTRL_IRQ_T        	osd_common_ctrl_irq;          //0xc0024144L
	M14B0_OSD_COMMON_CTRL_PIC_INIT_T   	osd_common_ctrl_pic_init;     //0xc0024148L
	M14B0_OSD_COMMON_CTRL_PIC_TIMER_T  	osd_common_ctrl_pic_timer;    //0xc002414cL
	M14B0_OSD_COMMON_CTRL_3DFP_OUT_T   	osd_common_ctrl_3dfp_out;     //0xc0024150L
	M14B0_OSD_COMMON_CTRL_PQ_PATH_T    	osd_common_ctrl_pq_path;      //0xc0024154L
	M14B0_OSD_COMMON_PLTE_SEL_T        	osd_common_plte_sel;          //0xc0024158L
	M14B0_OSD_COMMON_OSD_VALID_TH_T    	osd_common_osd_valid_th;      //0xc002415cL
	M14B0_OSD_COMMON_CSC_CTRL_T        	osd_common_csc_ctrl;          //0xc0024160L
	M14B0_OSD_COMMON_CSC_COEF0_T       	osd_common_csc_coef0;         //0xc0024164L
	M14B0_OSD_COMMON_CSC_COEF1_T       	osd_common_csc_coef1;         //0xc0024168L
	M14B0_OSD_COMMON_60HZ_PIC_TIMER_T  	osd_common_60hz_pic_timer;    //0xc002416cL
	M14B0_OSD_COMMON_SEP_OSD_CTRL_T    	osd_common_sep_osd_ctrl;      //0xc0024170L
	M14B0_OSD_COMMON_SEP_OSD_HDR0_T    	osd_common_sep_osd_hdr0;      //0xc0024174L
	M14B0_OSD_COMMON_SEP_OSD_HDR1_T    	osd_common_sep_osd_hdr1;      //0xc0024178L
	M14B0_OSD_COMMON_SEP_OSD_HDR2_T    	osd_common_sep_osd_hdr2;      //0xc002417cL
	M14B0_OSD_COMMON_SEP_OSD_HDR3_T    	osd_common_sep_osd_hdr3;      //0xc0024180L
	M14B0_OSD_COMMON_SEP_OSD_HDR4_T    	osd_common_sep_osd_hdr4;      //0xc0024184L
	M14B0_OSD_COMMON_SEP_OSD_HDR5_T    	osd_common_sep_osd_hdr5;      //0xc0024188L
	M14B0_OSD_COMMON_SEP_OSD_HDR6_T    	osd_common_sep_osd_hdr6;      //0xc002418cL
	M14B0_OSD_COMMON_CTRL_TPG_T        	osd_common_ctrl_tpg;          //0xc0024190L
	M14B0_OSD_COMMON_60HZ_DISP_SIZE_T  	osd_common_60hz_disp_size;    //0xc0024194L
	M14B0_OSD_COMMON_60HZ_CTRL_PATH_T  	osd_common_60hz_ctrl_path;    //0xc0024198L
	M14B0_OSD_COMMON_60HZ_OSD_VALID_TH_T	osd_common_60hz_osd_valid_th; //0xc002419cL
	M14B0_OSD_MIXER_CTRL_ONOFF_T       	osd_mixer_ctrl_onoff;         //0xc00241a0L
	M14B0_OSD_MIXER_CTRL_OUTPUT_T      	osd_mixer_ctrl_output;        //0xc00241a4L
	M14B0_OSD_MIXER_CTRL_AWB_T         	osd_mixer_ctrl_awb;           //0xc00241a8L
	M14B0_OSD_MIXER_CTRL_SYNC_T        	osd_mixer_ctrl_sync;          //0xc00241acL
	M14B0_OSD_MIXER_CTRL_BLEND_T       	osd_mixer_ctrl_blend;         //0xc00241b0L
	M14B0_OSD_MIXER_60HZ_CTRL_AWB_T    	osd_mixer_60hz_ctrl_awb;      //0xc00241b4L
	M14B0_OSD_MIXER_60HZ_CTRL_BLEND_T  	osd_mixer_60hz_ctrl_blend;    //0xc00241b8L
	M14B0_OSD_MIXER_CTRL_HOLE_T        	osd_mixer_ctrl_hole;          //0xc00241bcL
	M14B0_OSD_MIXER_CTRL_MUX0_T        	osd_mixer_ctrl_mux0;          //0xc00241c0L
	M14B0_OSD_MIXER_CTRL_MUX1_T        	osd_mixer_ctrl_mux1;          //0xc00241c4L
	M14B0_OSD_MIXER_CTRL_MUX2_T        	osd_mixer_ctrl_mux2;          //0xc00241c8L
	M14B0_OSD_MIXER_CTRL_MUX3_T        	osd_mixer_ctrl_mux3;          //0xc00241ccL
	M14B0_OSD_MIXER_CTRL_MUX4_T        	osd_mixer_ctrl_mux4;          //0xc00241d0L
	M14B0_OSD_MIXER_CTRL_MUX5_T        	osd_mixer_ctrl_mux5;          //0xc00241d4L
	M14B0_OSD_MIXER_CTRL_MUX6_T        	osd_mixer_ctrl_mux6;          //0xc00241d8L
	M14B0_OSD_MIXER_CTRL_MUX7_T        	osd_mixer_ctrl_mux7;          //0xc00241dcL
	M14B0_OSD_MIXER_CTRL_MUX8_T        	osd_mixer_ctrl_mux8;          //0xc00241e0L
	M14B0_OSD_MIXER_CTRL_MUX9_T        	osd_mixer_ctrl_mux9;          //0xc00241e4L
	M14B0_OSD_MIXER_CTRL_MUXA_T        	osd_mixer_ctrl_muxa;          //0xc00241e8L
	M14B0_OSD_MIXER_CTRL_MUXB_T        	osd_mixer_ctrl_muxb;          //0xc00241ecL
	M14B0_OSD_MIXER_CTRL_MUXC_T        	osd_mixer_ctrl_muxc;          //0xc00241f0L
	M14B0_OSD_MIXER_CTRL_MUXD_T        	osd_mixer_ctrl_muxd;          //0xc00241f4L
	M14B0_OSD_MIXER_CTRL_MUXE_T        	osd_mixer_ctrl_muxe;          //0xc00241f8L
	M14B0_OSD_MIXER_CTRL_MUXF_T        	osd_mixer_ctrl_muxf;          //0xc00241fcL
	M14B0_OSD0_CTRL_MAIN_T             	osd0_ctrl_main;               //0xc0024200L
	M14B0_OSD0_BASE_ADDR_T             	osd0_base_addr;               //0xc0024204L
	M14B0_OSD0_CTRL_SWAP_T             	osd0_ctrl_swap;               //0xc0024208L
	M14B0_OSD0_CTRL_PDEC_T             	osd0_ctrl_pdec;               //0xc002420cL
	M14B0_OSD0_CTRL_SCALER_T           	osd0_ctrl_scaler;             //0xc0024210L
	M14B0_OSD0_CTRL_3D_DISPARITY_T     	osd0_ctrl_3d_disparity;       //0xc0024214L
	M14B0_OSD0_CTRL_3D_BLANK_T         	osd0_ctrl_3d_blank;           //0xc0024218L
	M14B0_OSD0_INFO_LINE_MEMORY_T      	osd0_info_line_memory;        //0xc002421cL
	M14B0_OSD0_CTRL_HDR0_T             	osd0_ctrl_hdr0;               //0xc0024220L
	M14B0_OSD0_CTRL_HDR1_T             	osd0_ctrl_hdr1;               //0xc0024224L
	M14B0_OSD0_CTRL_HDR2_T             	osd0_ctrl_hdr2;               //0xc0024228L
	M14B0_OSD0_CTRL_HDR3_T             	osd0_ctrl_hdr3;               //0xc002422cL
	M14B0_OSD0_CTRL_HDR4_T             	osd0_ctrl_hdr4;               //0xc0024230L
	M14B0_OSD0_CTRL_HDR5_T             	osd0_ctrl_hdr5;               //0xc0024234L
	M14B0_OSD0_CTRL_HDR6_T             	osd0_ctrl_hdr6;               //0xc0024238L
	UINT32                             	reserved00;                   //0xc002423cL
	M14B0_OSD1_CTRL_MAIN_T             	osd1_ctrl_main;               //0xc0024240L
	M14B0_OSD1_CTRL_BASE_ADDR_T        	osd1_ctrl_base_addr;          //0xc0024244L
	M14B0_OSD1_CTRL_SWAP_T             	osd1_ctrl_swap;               //0xc0024248L
	M14B0_OSD1_CTRL_PDEC_T             	osd1_ctrl_pdec;               //0xc002424cL
	M14B0_OSD1_CTRL_SCALER_T           	osd1_ctrl_scaler;             //0xc0024250L
	M14B0_OSD1_CTRL_3D_DISPARITY_T     	osd1_ctrl_3d_disparity;       //0xc0024254L
	M14B0_OSD1_CTRL_3D_BLANK_T         	osd1_ctrl_3d_blank;           //0xc0024258L
	M14B0_OSD1_INFO_LINE_MEMORY_T      	osd1_info_line_memory;        //0xc002425cL
	M14B0_OSD1_CTRL_HDR0_T             	osd1_ctrl_hdr0;               //0xc0024260L
	M14B0_OSD1_CTRL_HDR1_T             	osd1_ctrl_hdr1;               //0xc0024264L
	M14B0_OSD1_CTRL_HDR2_T             	osd1_ctrl_hdr2;               //0xc0024268L
	M14B0_OSD1_CTRL_HDR3_T             	osd1_ctrl_hdr3;               //0xc002426cL
	M14B0_OSD1_CTRL_HDR4_T             	osd1_ctrl_hdr4;               //0xc0024270L
	M14B0_OSD1_CTRL_HDR5_T             	osd1_ctrl_hdr5;               //0xc0024274L
	M14B0_OSD1_CTRL_HDR6_T             	osd1_ctrl_hdr6;               //0xc0024278L
	UINT32                             	reserved01;                   //0xc002427cL
	M14B0_OSD2_CTRL_MAIN_T             	osd2_ctrl_main;               //0xc0024280L
	M14B0_OSD2_CTRL_BASE_ADDR_T        	osd2_ctrl_base_addr;          //0xc0024284L
	M14B0_OSD2_CTRL_SWAP_T             	osd2_ctrl_swap;               //0xc0024288L
	M14B0_OSD2_CTRL_PDEC_T             	osd2_ctrl_pdec;               //0xc002428cL
	M14B0_OSD2_CTRL_SCALER_T           	osd2_ctrl_scaler;             //0xc0024290L
	M14B0_OSD2_CTRL_3D_DISPARITY_T     	osd2_ctrl_3d_disparity;       //0xc0024294L
	M14B0_OSD2_CTRL_3D_BLANK_T         	osd2_ctrl_3d_blank;           //0xc0024298L
	M14B0_OSD2_INFO_LINE_MEMORY_T      	osd2_info_line_memory;        //0xc002429cL
	M14B0_OSD2_CTRL_HDR0_T             	osd2_ctrl_hdr0;               //0xc00242a0L
	M14B0_OSD2_CTRL_HDR1_T             	osd2_ctrl_hdr1;               //0xc00242a4L
	M14B0_OSD2_CTRL_HDR2_T             	osd2_ctrl_hdr2;               //0xc00242a8L
	M14B0_OSD2_CTRL_HDR3_T             	osd2_ctrl_hdr3;               //0xc00242acL
	M14B0_OSD2_CTRL_HDR4_T             	osd2_ctrl_hdr4;               //0xc00242b0L
	M14B0_OSD2_CTRL_HDR5_T             	osd2_ctrl_hdr5;               //0xc00242b4L
	M14B0_OSD2_CTRL_HDR6_T             	osd2_ctrl_hdr6;               //0xc00242b8L
	UINT32                             	reserved02;                   //0xc00242bcL
	M14B0_OSD3_CTRL_MAIN_T             	osd3_ctrl_main;               //0xc00242c0L
	M14B0_OSD3_CTRL_BASE_ADDR_T        	osd3_ctrl_base_addr;          //0xc00242c4L
	M14B0_OSD3_CTRL_SWAP_T             	osd3_ctrl_swap;               //0xc00242c8L
	M14B0_OSD3_CTRL_PDEC_T             	osd3_ctrl_pdec;               //0xc00242ccL
	M14B0_OSD3_CTRL_SCALER_T           	osd3_ctrl_scaler;             //0xc00242d0L
	M14B0_OSD3_CTRL_3D_DISPARITY_T     	osd3_ctrl_3d_disparity;       //0xc00242d4L
	M14B0_OSD3_CTRL_3D_BLANK_T         	osd3_ctrl_3d_blank;           //0xc00242d8L
	M14B0_OSD3_INFO_LINE_MEMORY_T      	osd3_info_line_memory;        //0xc00242dcL
	M14B0_OSD3_CTRL_HDR0_T             	osd3_ctrl_hdr0;               //0xc00242e0L
	M14B0_OSD3_CTRL_HDR1_T             	osd3_ctrl_hdr1;               //0xc00242e4L
	M14B0_OSD3_CTRL_HDR2_T             	osd3_ctrl_hdr2;               //0xc00242e8L
	M14B0_OSD3_CTRL_HDR3_T             	osd3_ctrl_hdr3;               //0xc00242ecL
	M14B0_OSD3_CTRL_HDR4_T             	osd3_ctrl_hdr4;               //0xc00242f0L
	M14B0_OSD3_CTRL_HDR5_T             	osd3_ctrl_hdr5;               //0xc00242f4L
	M14B0_OSD3_CTRL_HDR6_T             	osd3_ctrl_hdr6;               //0xc00242f8L
} DE_OSD_REG_M14B0_T;

#endif
