/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file
 *
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _FBDEV_REG_H14_H_
#define _FBDEV_REG_H14_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
	0x0000 osd_status_mon_cfg ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_cfg              : 3,	//  0: 2
	                                : 1,	//     3 reserved
	reg_status_mon_osd_sel          : 2;	//  4: 5
} OSD_H14_STATUS_MON_CFG;

/*-----------------------------------------------------------------------------
	0x0004 osd_status_mon_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_ypos             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_status_mon_xpos             :12;	// 16:27
} OSD_H14_STATUS_MON_POS;

/*-----------------------------------------------------------------------------
	0x0008 osd_status_version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_version              ;   	// 31: 0
} OSD_H14_STATUS_VERSION;

/*-----------------------------------------------------------------------------
	0x000c osd_status_mixer_line ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mixer_line           :16,	//  0:15
	reg_status_pic_init_count       : 8;	// 16:23
} OSD_H14_STATUS_MIXER_LINE;

/*-----------------------------------------------------------------------------
	0x0010 osd_status_mon_data0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data0            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA0;

/*-----------------------------------------------------------------------------
	0x0014 osd_status_mon_data1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data1            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA1;

/*-----------------------------------------------------------------------------
	0x0018 osd_status_mon_data2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data2            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA2;

/*-----------------------------------------------------------------------------
	0x001c osd_status_mon_data3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data3            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA3;

/*-----------------------------------------------------------------------------
	0x0020 osd_status_mon_data4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data4            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA4;

/*-----------------------------------------------------------------------------
	0x0024 osd_status_mon_data5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data5            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA5;

/*-----------------------------------------------------------------------------
	0x0028 osd_status_mon_data6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data6            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA6;

/*-----------------------------------------------------------------------------
	0x002c osd_status_mon_data7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data7            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA7;

/*-----------------------------------------------------------------------------
	0x0030 osd_status_mon_data8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data8            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA8;

/*-----------------------------------------------------------------------------
	0x0034 osd_status_mon_data9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data9            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATA9;

/*-----------------------------------------------------------------------------
	0x0038 osd_status_mon_dataa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_dataa            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATAA;

/*-----------------------------------------------------------------------------
	0x003c osd_status_mon_datab ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_datab            ;   	// 31: 0
} OSD_H14_STATUS_MON_DATAB;

/*-----------------------------------------------------------------------------
	0x0040 osd_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD_H14_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0044 osd_common_ctrl_irq ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_ctrl_irq_clear_osd3  	: 1,	//     0
	reg_common_ctrl_irq_clear_osd2  	: 1,	//     1
	reg_common_ctrl_irq_clear_osd1  	: 1,	//     2
	reg_common_ctrl_irq_clear_osd0  	: 1,	//     3
	reg_common_ctrl_irq_clear_mixer 	: 1,	//     4
	reg_common_ctrl_irq_clear_mixer_60hz : 1,	//	   5

	                                	:10,	//  6:15 reserved
	reg_common_ctrl_irq_en_osd3     	: 1,	//    16
	reg_common_ctrl_irq_en_osd2     	: 1,	//    17
	reg_common_ctrl_irq_en_osd1     	: 1,	//    18
	reg_common_ctrl_irq_en_osd0     	: 1,	//    19
	reg_common_ctrl_irq_en_mixer    	: 1,	//    20
	reg_common_ctrl_irq_en_mixer_60hz   : 1;	//    21
} OSD_H14_COMMON_CTRL_IRQ;

/*-----------------------------------------------------------------------------
	0x0048 osd_common_ctrl_pic_init ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_pic_start_sel        : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_common_pic_init_sel         : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_common_self_init_src_sel    : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_common_60hz_init_src_sel    : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_common_2nd_pic_init_line    :12,	// 16:27
	reg_common_2nd_pic_init_en      : 1;	//    28
} OSD_H14_COMMON_CTRL_PIC_INIT;

/*-----------------------------------------------------------------------------
	0x004c osd_common_ctrl_pic_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_pic_init_timer       ;   	// 31: 0
} OSD_H14_COMMON_CTRL_PIC_TIMER;

/*-----------------------------------------------------------------------------
	0x0050 osd_common_ctrl_3dfp_out ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_vact_space           :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_vact_video           :12,	// 16:27
	reg_common_3dfp_en              : 1;	//    28
} OSD_H14_COMMON_CTRL_3DFP_OUT;

/*-----------------------------------------------------------------------------
	0x0054 osd_common_ctrl_pq_path ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_pq_path_sel          : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_common_pq_path_en           : 1;	//     4
} OSD_H14_COMMON_CTRL_PQ_PATH;

/*-----------------------------------------------------------------------------
	0x0058 osd_common_plte_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_plte_sel             : 3;	//  0: 2
} OSD_H14_COMMON_PLTE_SEL;

/*-----------------------------------------------------------------------------
	0x005c osd_common_osd_valid_th ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_osd_valid_area_th           : 8,	//  0: 7
	                                : 8,	//  8:15 reserved
	reg_osd_valid_area_en           : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_blend_sel_out_sep_tp_a      : 8,	// 20:27
	reg_blend_sel_out_sep_tp_ma     : 1;	//    28
} OSD_H14_COMMON_OSD_H14_VALID_TH;

/*-----------------------------------------------------------------------------
	0x0060 osd_common_csc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_csc_osd_sel          : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_common_csc_all_bypass       : 1;	//     4
} OSD_H14_COMMON_CSC_CTRL;

/*-----------------------------------------------------------------------------
	0x0064 osd_common_csc_coef0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_csc0_datai           :11,	//  0:10
	reg_common_csc0_rw_n            : 1,	//    11
	reg_common_csc0_addr            : 4,	// 12:15
	reg_common_csc0_bypass          : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_common_csc0_sel             : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_common_csc0_out_ctrl        : 3,	// 24:26
	                                : 1,	//    27 reserved
	reg_common_csc0_in_ctrl         : 3;	// 28:30
} OSD_H14_COMMON_CSC_COEF0;

/*-----------------------------------------------------------------------------
	0x0068 osd_common_csc_coef1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_csc1_datai           :11,	//  0:10
	reg_common_csc1_rw_n            : 1,	//    11
	reg_common_csc1_addr            : 4,	// 12:15
	reg_common_csc1_bypass          : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_common_csc1_sel             : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_common_csc1_out_ctrl        : 3,	// 24:26
	                                : 1,	//    27 reserved
	reg_common_csc1_in_ctrl         : 3;	// 28:30
} OSD_H14_COMMON_CSC_COEF1;

/*-----------------------------------------------------------------------------
	0x0070 osd_common_sep_osd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_sep_osd_hdr_en       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_common_sep_osd_sync_sel     : 1,	//     4
	                                :26,	//  5:30 reserved
	reg_common_sep_osd_bypass       : 1;	//    31
} OSD_H14_COMMON_SEP_OSD_H14_CTRL;

/*-----------------------------------------------------------------------------
	0x0074 osd_common_sep_osd_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_0                   ;   	// 31: 0
} OSD_H14_COMMON_SEP_OSD_H14_HDR0;

/*-----------------------------------------------------------------------------
	0x0078 osd_common_sep_osd_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_1                   ;   	// 31: 0
} OSD_H14_COMMON_SEP_OSD_H14_HDR1;

/*-----------------------------------------------------------------------------
	0x007c osd_common_sep_osd_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_2                   ;   	// 31: 0
} OSD_H14_COMMON_SEP_OSD_H14_HDR2;

/*-----------------------------------------------------------------------------
	0x0080 osd_common_sep_osd_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_3                   ;   	// 31: 0
} OSD_H14_COMMON_SEP_OSD_H14_HDR3;

/*-----------------------------------------------------------------------------
	0x0084 osd_common_sep_osd_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_4                   ;   	// 31: 0
} OSD_H14_COMMON_SEP_OSD_H14_HDR4;

/*-----------------------------------------------------------------------------
	0x0088 osd_common_sep_osd_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_5                   ;   	// 31: 0
} OSD_H14_COMMON_SEP_OSD_H14_HDR5;

/*-----------------------------------------------------------------------------
	0x008c osd_common_sep_osd_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_6                   ;   	// 31: 0
} OSD_H14_COMMON_SEP_OSD_H14_HDR6;

/*-----------------------------------------------------------------------------
	0x0090 osd_common_ctrl_tpg ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_common_ctrl_tpg             ;   	// 31: 0
} OSD_H14_COMMON_CTRL_TPG;

/*-----------------------------------------------------------------------------
	0x0094 osd_common_60hz_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_60hz_disp_size_h            :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_60hz_disp_size_w            :12;	// 16:27
} OSD_H14_COMMON_60HZ_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0098 osd_common_60hz_ctrl_path ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                :16,	//  0:15 reserved
	reg_60hz_path_osd1_en           : 1,	//    16
	                                : 7,	// 17:23 reserved
	reg_60hz_path_osd0_en           : 1;	//    24
} OSD_H14_COMMON_60HZ_CTRL_PATH;

/*-----------------------------------------------------------------------------
	0x009c osd_common_60hz_osd_valid_th ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_60hz_l_osd_valid_th         : 8,	//  0: 7
	reg_60hz_l_src_sel_dn_zero      : 1,	//     8
	reg_60hz_l_src_sel_a_zero       : 1,	//     9
	reg_60hz_l_src_sel_dn           : 1,	//    10
	reg_60hz_l_src_sel_a            : 1,	//    11
	reg_60hz_l_src_sel_out          : 1,	//    12
	reg_60hz_l_mk_color             : 3,	// 13:15
	reg_60hz_r_osd_valid_th         : 8,	// 16:23
	reg_60hz_r_src_sel_dn_zero      : 1,	//    24
	reg_60hz_r_src_sel_a_zero       : 1,	//    25
	reg_60hz_r_src_sel_dn           : 1,	//    26
	reg_60hz_r_src_sel_a            : 1,	//    27
	reg_60hz_r_src_sel_out          : 1,	//    28
	reg_60hz_r_mk_color             : 3;	// 29:31
} OSD_H14_COMMON_60HZ_OSD_H14_VALID_TH;

/*-----------------------------------------------------------------------------
	0x00a0 osd_mixer_ctrl_onoff ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_osd3_mixer_en               : 1,	//     0
	                                : 7,	//  1: 7 reserved
	reg_osd2_mixer_en               : 1,	//     8
	                                : 7,	//  9:15 reserved
	reg_osd1_mixer_en               : 1,	//    16
	                                : 7,	// 17:23 reserved
	reg_osd0_mixer_en               : 1;	//    24
} OSD_H14_MIXER_CTRL_ONOFF;

/*-----------------------------------------------------------------------------
	0x00a4 osd_mixer_ctrl_output ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_out_b                 : 8,	//  0: 7
	reg_mixer_out_g                 : 8,	//  8:15
	reg_mixer_out_r                 : 8,	// 16:23
	reg_mixer_out_fpr_mask_en       : 1,	//    24
	reg_mixer_out_fpr_mask_odd      : 1,	//    25
	                                : 2,	// 26:27 reserved
	reg_mixer_out_en                : 1,	//    28
	reg_mixer_out_valid_en          : 1,	//    29
	reg_mixer_out_vid_de_only       : 1,	//    30
	reg_mixer_out_vid_init_en       : 1;	//    31
} OSD_H14_MIXER_CTRL_OUTPUT;

/*-----------------------------------------------------------------------------
	0x00a8 osd_mixer_ctrl_awb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_osd3_mixer_awb_b            : 1,	//     0
	reg_osd3_mixer_awb_g            : 1,	//     1
	reg_osd3_mixer_awb_r            : 1,	//     2
	                                : 1,	//     3 reserved
	reg_osd2_mixer_awb_b            : 1,	//     4
	reg_osd2_mixer_awb_g            : 1,	//     5
	reg_osd2_mixer_awb_r            : 1,	//     6
	                                : 1,	//     7 reserved
	reg_osd1_mixer_awb_b            : 1,	//     8
	reg_osd1_mixer_awb_g            : 1,	//     9
	reg_osd1_mixer_awb_r            : 1,	//    10
	                                : 1,	//    11 reserved
	reg_osd0_mixer_awb_b            : 1,	//    12
	reg_osd0_mixer_awb_g            : 1,	//    13
	reg_osd0_mixer_awb_r            : 1;	//    14
} OSD_H14_MIXER_CTRL_AWB;

/*-----------------------------------------------------------------------------
	0x00ac osd_mixer_ctrl_sync ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sync_osd3_tp_en             : 1,	//     0
	reg_sync_osd3_tp_aa_en          : 1,	//     1
	                                : 2,	//  2: 3 reserved
	reg_sync_osd2_tp_en             : 1,	//     4
	reg_sync_osd2_tp_aa_en          : 1,	//     5
	                                : 2,	//  6: 7 reserved
	reg_sync_osd1_tp_en             : 1,	//     8
	reg_sync_osd1_tp_aa_en          : 1,	//     9
	                                : 2,	// 10:11 reserved
	reg_sync_osd0_tp_en             : 1,	//    12
	reg_sync_osd0_tp_aa_en          : 1,	//    13
	                                : 2,	// 14:15 reserved
	reg_sync_tp_alpha               : 8,	// 16:23
	                                : 7,	// 24:30 reserved
	reg_sync_v_reverse              : 1;	//    31
} OSD_H14_MIXER_CTRL_SYNC;

/*-----------------------------------------------------------------------------
	0x00b0 osd_mixer_ctrl_blend ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_blend_sel_0v_dn             : 1,	//     0
	reg_blend_sel_0v_up             : 1,	//     1
	reg_blend_sel_10_dn             : 1,	//     2
	reg_blend_sel_10_up             : 1,	//     3
	                                : 2,	//  4: 5 reserved
	reg_blend_sel_31_dn             : 1,	//     6
	reg_blend_sel_31_up             : 1,	//     7
	reg_blend_div_dvdd              : 1,	//     8
	reg_blend_div_dvsr              : 1,	//     9
	reg_blend_sel_a10_dn            : 1,	//    10
	reg_blend_sel_a10_a             : 1,	//    11
	                                : 2,	// 12:13 reserved
	reg_blend_sel_a31_dn            : 1,	//    14
	reg_blend_sel_a31_a             : 1,	//    15
	reg_blend_sel_v3d_src           : 1,	//    16
	reg_blend_sel_v5d_src           : 1,	//    17
	reg_blend_sel_v7d_src           : 1,	//    18
	                                : 1,	//    19 reserved
	reg_blend_sel_vid_src           : 1,	//    20
	reg_blend_sel_31_dn_src         : 1,	//    21
	reg_blend_sel_div_pel_src       : 1,	//    22
	reg_blend_sel_out_sepc_src      : 1,	//    23
	reg_blend_sel_a3d_src           : 1,	//    24
	reg_blend_sel_a5d_src           : 1,	//    25
	                                : 1,	//    26 reserved
	reg_blend_sel_a31_z_3d_src      : 1,	//    27
	reg_blend_sel_out_vid           : 1,	//    28
	reg_blend_sel_out_sepc          : 1,	//    29
	reg_blend_sel_out_sepa          : 1,	//    30
	reg_blend_sel_out_sep_tp_en     : 1;	//    31
} OSD_H14_MIXER_CTRL_BLEND;

/*-----------------------------------------------------------------------------
	0x00b4 osd_mixer_60hz_ctrl_awb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	                                : 1,	//     0 reserved
	osd_mixer_60hz_ctrl_awb00         : 1,	//     1
	osd_mixer_60hz_ctrl_awb01         : 1,	//     2
	osd_mixer_60hz_ctrl_awb02         : 1,	//     3
	osd_mixer_60hz_ctrl_awb03         : 8,	//  4:11
	reg_csc_en_1_0                    : 1,	//    12
	osd_mixer_60hz_ctrl_awb04         : 1,	//    13
	osd_mixer_60hz_ctrl_awb05         : 1,	//    14
	osd_mixer_60hz_ctrl_awb06         : 1,	//    15
	                                : 1,	//    16 reserved
	osd_mixer_60hz_ctrl_awb07         : 1,	//    17
	osd_mixer_60hz_ctrl_awb08         : 1,	//    18
	osd_mixer_60hz_ctrl_awb09         : 1,	//    19
	osd_mixer_60hz_ctrl_awb10         : 8,	// 20:27
	reg_csc_en_1_1                    : 1,	//    28
	osd_mixer_60hz_ctrl_awb11         : 1,	//    29
	osd_mixer_60hz_ctrl_awb12         : 1,	//    30
	osd_mixer_60hz_ctrl_awb13         : 1;	//    31
} OSD_H14_MIXER_60HZ_CTRL_AWB;

/*-----------------------------------------------------------------------------
	0x00b8 osd_mixer_60hz_ctrl_blend ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_src_sel_l_0v_dn_zero        : 1,	//     0
	reg_src_sel_l_0v_up_zero        : 1,	//     1
	reg_src_sel_l_10_dn_zero        : 1,	//     2
	reg_src_sel_l_10_up_zero        : 1,	//     3
	reg_src_sel_l_0v_dn_osd0        : 1,	//     4
	reg_src_sel_l_0v_up_osd1        : 1,	//     5
	reg_src_sel_l_10_dn_video       : 1,	//     6
	reg_src_sel_l_10_up_osd0        : 1,	//     7
	reg_src_sel_l_0v_dn_osd1        : 1,	//     8
	reg_src_sel_l_0v_up_video       : 1,	//     9
	reg_src_sel_l_10_dn_osd1        : 1,	//    10
	reg_src_sel_l_10_up_video       : 1,	//    11
	reg_src_sel_l_vd_7d_0v_z        : 1,	//    12
	reg_src_sel_l_vd_mk_vd8d        : 1,	//    13
	reg_src_sel_l_vd_out_mk         : 1,	//    14
	reg_src_sel_l_vd_out_zero       : 1,	//    15
	reg_src_sel_r_0v_dn_zero        : 1,	//    16
	reg_src_sel_r_0v_up_zero        : 1,	//    17
	reg_src_sel_r_10_dn_zero        : 1,	//    18
	reg_src_sel_r_10_up_zero        : 1,	//    19
	reg_src_sel_r_0v_dn_osd0        : 1,	//    20
	reg_src_sel_r_0v_up_osd1        : 1,	//    21
	reg_src_sel_r_10_dn_video       : 1,	//    22
	reg_src_sel_r_10_up_osd0        : 1,	//    23
	reg_src_sel_r_0v_dn_osd1        : 1,	//    24
	reg_src_sel_r_0v_up_video       : 1,	//    25
	reg_src_sel_r_10_dn_osd1        : 1,	//    26
	reg_src_sel_r_10_up_video       : 1,	//    27
	reg_src_sel_r_vd_7d_0v_z        : 1,	//    28
	reg_src_sel_r_vd_mk_vd8d        : 1,	//    29
	reg_src_sel_r_vd_out_mk         : 1,	//    30
	reg_src_sel_r_vd_out_zero       : 1;	//    31
} OSD_H14_MIXER_60HZ_CTRL_BLEND;

/*-----------------------------------------------------------------------------
	0x00bc osd_mixer_ctrl_hole ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hole2_en                   : 1,	//     0
	osd3_hole1_en                   : 1,	//     1
	osd3_hole0_en                   : 1,	//     2
	                                : 1,	//     3 reserved
	osd3_hole_en                    : 1,	//     4
	                                : 3,	//  5: 7 reserved
	osd2_hole2_en                   : 1,	//     8
	osd2_hole1_en                   : 1,	//     9
	osd2_hole0_en                   : 1,	//    10
	                                : 1,	//    11 reserved
	osd2_hole_en                    : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd1_hole2_en                   : 1,	//    16
	osd1_hole1_en                   : 1,	//    17
	osd1_hole0_en                   : 1,	//    18
	                                : 1,	//    19 reserved
	osd1_hole_en                    : 1,	//    20
	                                : 3,	// 21:23 reserved
	osd0_hole2_en                   : 1,	//    24
	osd0_hole1_en                   : 1,	//    25
	osd0_hole0_en                   : 1,	//    26
	                                : 1,	//    27 reserved
	osd0_hole_en                    : 1;	//    28
} OSD_H14_MIXER_CTRL_HOLE;

/*-----------------------------------------------------------------------------
	0x00c0 osd_mixer_ctrl_mux0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	layer_mux_3                     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	layer_mux_2                     : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	layer_mux_1                     : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	layer_mux_0                     : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	video_layer_alpha               : 8,	// 16:23
	                                : 4,	// 24:27 reserved
	video_layer_off                 : 1;	//    28
} OSD_H14_MIXER_CTRL_MUX0;

/*-----------------------------------------------------------------------------
	0x00c4 osd_mixer_ctrl_mux1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window0_alpha         : 8;	// 16:23
} OSD_H14_MIXER_CTRL_MUX1;

/*-----------------------------------------------------------------------------
	0x00c8 osd_mixer_ctrl_mux2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_xpos          :12;	// 16:27
} OSD_H14_MIXER_CTRL_MUX2;

/*-----------------------------------------------------------------------------
	0x00cc osd_mixer_ctrl_mux3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_w             :12;	// 16:27
} OSD_H14_MIXER_CTRL_MUX3;

/*-----------------------------------------------------------------------------
	0x00d0 osd_mixer_ctrl_mux4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window1_alpha         : 8;	// 16:23
} OSD_H14_MIXER_CTRL_MUX4;

/*-----------------------------------------------------------------------------
	0x00d4 osd_mixer_ctrl_mux5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_xpos          :12;	// 16:27
} OSD_H14_MIXER_CTRL_MUX5;

/*-----------------------------------------------------------------------------
	0x00d8 osd_mixer_ctrl_mux6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_w             :12;	// 16:27
} OSD_H14_MIXER_CTRL_MUX6;

/*-----------------------------------------------------------------------------
	0x00dc osd_mixer_ctrl_mux7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window2_alpha         : 8;	// 16:23
} OSD_H14_MIXER_CTRL_MUX7;

/*-----------------------------------------------------------------------------
	0x00e0 osd_mixer_ctrl_mux8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_xpos          :12;	// 16:27
} OSD_H14_MIXER_CTRL_MUX8;

/*-----------------------------------------------------------------------------
	0x00e4 osd_mixer_ctrl_mux9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_w             :12;	// 16:27
} OSD_H14_MIXER_CTRL_MUX9;

/*-----------------------------------------------------------------------------
	0x00e8 osd_mixer_ctrl_muxa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_y                     :12,	//  0:11
	reg_hole0_x                     :12,	// 12:23
	reg_hole0_reverse_en            : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_hole0_en                    : 1;	//    28
} OSD_H14_MIXER_CTRL_MUXA;

/*-----------------------------------------------------------------------------
	0x00ec osd_mixer_ctrl_muxb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_h                     :12,	//  0:11
	reg_hole0_w                     :12,	// 12:23
	reg_hole0_a                     : 8;	// 24:31
} OSD_H14_MIXER_CTRL_MUXB;

/*-----------------------------------------------------------------------------
	0x00f0 osd_mixer_ctrl_muxc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_y                     :12,	//  0:11
	reg_hole1_x                     :12,	// 12:23
	reg_hole1_reverse_en            : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_hole1_en                    : 1;	//    28
} OSD_H14_MIXER_CTRL_MUXC;

/*-----------------------------------------------------------------------------
	0x00f4 osd_mixer_ctrl_muxd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_h                     :12,	//  0:11
	reg_hole1_w                     :12,	// 12:23
	reg_hole1_a                     : 8;	// 24:31
} OSD_H14_MIXER_CTRL_MUXD;

/*-----------------------------------------------------------------------------
	0x00f8 osd_mixer_ctrl_muxe ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_y                     :12,	//  0:11
	reg_hole2_x                     :12,	// 12:23
	reg_hole2_reverse_en            : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_hole2_en                    : 1;	//    28
} OSD_H14_MIXER_CTRL_MUXE;

/*-----------------------------------------------------------------------------
	0x00fc osd_mixer_ctrl_muxf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_h                     :12,	//  0:11
	reg_hole2_w                     :12,	// 12:23
	reg_hole2_a                     : 8;	// 24:31
} OSD_H14_MIXER_CTRL_MUXF;

/*-----------------------------------------------------------------------------
	0x0100 osd0_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_en                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	osd0_hdr_src_sel                : 1,	//     4
	osd0_unaligned_addr_en          : 1,	//     5
	                                : 2,	//  6: 7 reserved
	osd0_v_reverse_en               : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd0_s3d_en                     : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd0_manual_line_inc            : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	osd0_manual_line_inc_en         : 1,	//    20
	                                : 3,	// 21:23 reserved
	osd0_yuyv_mode_en               : 1,	//    24
	                                : 3,	// 25:27 reserved
	osd0_hdr_ready_flag_en          : 1,	//    28
	osd0_hdr_ready_flag             : 1;	//    29
} OSD0_H14_CTRL_MAIN;

/*-----------------------------------------------------------------------------
	0x0104 osd0_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_base_addr                  ;   	// 31: 0
} OSD0_H14_BASE_ADDR;

/*-----------------------------------------------------------------------------
	0x0108 osd0_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_bbs                        : 1,	//     0
	osd0_bqws                       : 1,	//     1
	osd0_bhws                       : 1,	//     2
	                                : 1,	//     3 reserved
	osd0_pbs                        : 1,	//     4
	osd0_pqws                       : 1,	//     5
	osd0_phws                       : 1,	//     6
	                                : 1,	//     7 reserved
	osd0_hbs                        : 1,	//     8
	osd0_hqws                       : 1,	//     9
	osd0_hhws                       : 1;	//    10
} OSD0_H14_CTRL_SWAP;

/*-----------------------------------------------------------------------------
	0x010c osd0_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_lfsr_seed                  :19,	//  0:18
	osd0_pure_ck                    : 1,	//    19
	osd0_round                      : 3,	// 20:22
	                                : 1,	//    23 reserved
	osd0_ch_blue                    : 2,	// 24:25
	osd0_ch_green                   : 2,	// 26:27
	osd0_ch_red                     : 2,	// 28:29
	osd0_ch_alpha                   : 2;	// 30:31
} OSD0_H14_CTRL_PDEC;

/*-----------------------------------------------------------------------------
	0x0110 osd0_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_sc_bc_sa                   : 2,	//  0: 1
	osd0_sc_force_vsize_en          : 1,	//     2
	osd0_sc_bilinear                : 1,	//     3
	osd0_sc_vdouble                 : 1,	//     4
	                                : 3,	//  5: 7 reserved
	osd0_sc_sampling_mode           : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd0_sc_bnd_mode                : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd0_sc_h_phase_off             : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	osd0_sc_v_phase_off             : 6,	// 24:29
	osd0_odd_width_en               : 1,	//    30
	osd0_fifo_en                    : 1;	//    31
} OSD0_H14_CTRL_SCALER;

/*-----------------------------------------------------------------------------
	0x0114 osd0_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
#if 1
	osd0_r_3d_disparity_value       : 16,	//  0:15
	osd0_l_3d_disparity_value       : 16;	// 16:31
#else
	osd0_r_3d_disparity_value       : 8,	//  0: 7
	osd0_r_3d_disparity_sign        : 1,	//     8
	                                : 7,	//  9:15 reserved
	osd0_l_3d_disparity_value       : 8,	// 16:23
	osd0_l_3d_disparity_sign        : 1;	//    24
#endif
} OSD0_H14_CTRL_3D_DISPARITY;

/*-----------------------------------------------------------------------------
	0x0118 osd0_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_ctrl_3d_blank              ;   	// 31: 0
} OSD0_H14_CTRL_3D_BLANK;

/*-----------------------------------------------------------------------------
	0x0120 osd0_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_hdr_ypos                   :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd0_hdr_xpos                   :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd0_hdr_color_key_en           : 1;	//    31
} OSD0_H14_CTRL_HDR0;

/*-----------------------------------------------------------------------------
	0x0124 osd0_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_hdr_h_mem                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd0_hdr_w_mem                  :12;	// 16:27
} OSD0_H14_CTRL_HDR1;

/*-----------------------------------------------------------------------------
	0x0128 osd0_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_hdr_h_out                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd0_hdr_w_out                  :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd0_hdr_pixel_order            : 1;	//    31
} OSD0_H14_CTRL_HDR2;

/*-----------------------------------------------------------------------------
	0x012c osd0_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_hdr_wpl                    :16,	//  0:15
	osd0_hdr_global_alpha           : 8,	// 16:23
	osd0_hdr_format                 : 4,	// 24:27
	osd0_hdr_depth                  : 3,	// 28:30
	osd0_hdr_global_alpha_en        : 1;	//    31
} OSD0_H14_CTRL_HDR3;

/*-----------------------------------------------------------------------------
	0x0130 osd0_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_hdr_color_key              ;   	// 31: 0
} OSD0_H14_CTRL_HDR4;

/*-----------------------------------------------------------------------------
	0x0134 osd0_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_hdr_ptr_plte               ;   	// 31: 0
} OSD0_H14_CTRL_HDR5;

/*-----------------------------------------------------------------------------
	0x0138 osd0_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_hdr_ptr_bmp                ;   	// 31: 0
} OSD0_H14_CTRL_HDR6;

/*-----------------------------------------------------------------------------
	0x0140 osd1_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_en                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	osd1_hdr_src_sel                : 1,	//     4
	osd1_unaligned_addr_en          : 1,	//     5
	                                : 2,	//  6: 7 reserved
	osd1_v_reverse_en               : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd1_s3d_en                     : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd1_manual_line_inc            : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	osd1_manual_line_inc_en         : 1,	//    20
	                                : 3,	// 21:23 reserved
	osd1_yuyv_mode_en               : 1,	//    24
	                                : 3,	// 25:27 reserved
	osd1_hdr_ready_flag_en          : 1,	//    28
	osd1_hdr_ready_flag             : 1;	//    29
} OSD1_H14_CTRL_MAIN;

/*-----------------------------------------------------------------------------
	0x0144 osd1_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_base_addr                  ;   	// 31: 0
} OSD1_H14_CTRL_BASE_ADDR;

/*-----------------------------------------------------------------------------
	0x0148 osd1_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_bbs                        : 1,	//     0
	osd1_bqws                       : 1,	//     1
	osd1_bhws                       : 1,	//     2
	                                : 1,	//     3 reserved
	osd1_pbs                        : 1,	//     4
	osd1_pqws                       : 1,	//     5
	osd1_phws                       : 1,	//     6
	                                : 1,	//     7 reserved
	osd1_hbs                        : 1,	//     8
	osd1_hqws                       : 1,	//     9
	osd1_hhws                       : 1;	//    10
} OSD1_H14_CTRL_SWAP;

/*-----------------------------------------------------------------------------
	0x014c osd1_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_lfsr_seed                  :19,	//  0:18
	osd1_pure_ck                    : 1,	//    19
	osd1_round                      : 3,	// 20:22
	                                : 1,	//    23 reserved
	osd1_ch_blue                    : 2,	// 24:25
	osd1_ch_green                   : 2,	// 26:27
	osd1_ch_red                     : 2,	// 28:29
	osd1_ch_alpha                   : 2;	// 30:31
} OSD1_H14_CTRL_PDEC;

/*-----------------------------------------------------------------------------
	0x0150 osd1_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_sc_bc_sa                   : 2,	//  0: 1
	osd0_sc_force_vsize_en          : 1,	//     2
	osd1_sc_bilinear                : 1,	//     3
	osd1_sc_vdouble                 : 1,	//     4
	                                : 3,	//  5: 7 reserved
	osd1_sc_sampling_mode           : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd1_sc_bnd_mode                : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd1_sc_h_phase_off             : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	osd1_sc_v_phase_off             : 6,	// 24:29
	osd1_odd_width_en               : 1,	//    30
	osd1_fifo_en                    : 1;	//    31
} OSD1_H14_CTRL_SCALER;

/*-----------------------------------------------------------------------------
	0x0154 osd1_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
#if 1
	osd1_r_3d_disparity_value       : 16,	//  0:15
	osd1_l_3d_disparity_value       : 16;	// 16:31
#else
	osd1_r_3d_disparity_value       : 8,	//  0: 7
	osd1_r_3d_disparity_sign        : 1,	//     8
	                                : 7,	//  9:15 reserved
	osd1_l_3d_disparity_value       : 8,	// 16:23
	osd1_l_3d_disparity_sign        : 1;	//    24
#endif
} OSD1_H14_CTRL_3D_DISPARITY;

/*-----------------------------------------------------------------------------
	0x0158 osd1_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_ctrl_3d_blank              ;   	// 31: 0
} OSD1_H14_CTRL_3D_BLANK;

/*-----------------------------------------------------------------------------
	0x0160 osd1_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_hdr_ypos                   :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd1_hdr_xpos                   :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd1_hdr_color_key_en           : 1;	//    31
} OSD1_H14_CTRL_HDR0;

/*-----------------------------------------------------------------------------
	0x0164 osd1_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_hdr_h_mem                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd1_hdr_w_mem                  :12;	// 16:27
} OSD1_H14_CTRL_HDR1;

/*-----------------------------------------------------------------------------
	0x0168 osd1_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_hdr_h_out                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd1_hdr_w_out                  :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd1_hdr_pixel_order            : 1;	//    31
} OSD1_H14_CTRL_HDR2;

/*-----------------------------------------------------------------------------
	0x016c osd1_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_hdr_wpl                    :16,	//  0:15
	osd1_hdr_global_alpha           : 8,	// 16:23
	osd1_hdr_format                 : 4,	// 24:27
	osd1_hdr_depth                  : 3,	// 28:30
	osd1_hdr_global_alpha_en        : 1;	//    31
} OSD1_H14_CTRL_HDR3;

/*-----------------------------------------------------------------------------
	0x0170 osd1_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_hdr_color_key              ;   	// 31: 0
} OSD1_H14_CTRL_HDR4;

/*-----------------------------------------------------------------------------
	0x0174 osd1_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_hdr_ptr_plte               ;   	// 31: 0
} OSD1_H14_CTRL_HDR5;

/*-----------------------------------------------------------------------------
	0x0178 osd1_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd1_hdr_ptr_bmp                ;   	// 31: 0
} OSD1_H14_CTRL_HDR6;

/*-----------------------------------------------------------------------------
	0x0180 osd2_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_en                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	osd2_hdr_src_sel                : 1,	//     4
	osd2_unaligned_addr_en          : 1,	//     5
	                                : 2,	//  6: 7 reserved
	osd2_v_reverse_en               : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd2_s3d_en                     : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd2_manual_line_inc            : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	osd2_manual_line_inc_en         : 1,	//    20
	                                : 3,	// 21:23 reserved
	osd2_yuyv_mode_en               : 1,	//    24
	                                : 3,	// 25:27 reserved
	osd2_hdr_ready_flag_en          : 1,	//    28
	osd2_hdr_ready_flag             : 1;	//    29
} OSD2_H14_CTRL_MAIN;

/*-----------------------------------------------------------------------------
	0x0184 osd2_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_base_addr                  ;   	// 31: 0
} OSD2_H14_CTRL_BASE_ADDR;

/*-----------------------------------------------------------------------------
	0x0188 osd2_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_bbs                        : 1,	//     0
	osd2_bqws                       : 1,	//     1
	osd2_bhws                       : 1,	//     2
	                                : 1,	//     3 reserved
	osd2_pbs                        : 1,	//     4
	osd2_pqws                       : 1,	//     5
	osd2_phws                       : 1,	//     6
	                                : 1,	//     7 reserved
	osd2_hbs                        : 1,	//     8
	osd2_hqws                       : 1,	//     9
	osd2_hhws                       : 1;	//    10
} OSD2_H14_CTRL_SWAP;

/*-----------------------------------------------------------------------------
	0x018c osd2_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_lfsr_seed                  :19,	//  0:18
	osd2_pure_ck                    : 1,	//    19
	osd2_round                      : 3,	// 20:22
	                                : 1,	//    23 reserved
	osd2_ch_blue                    : 2,	// 24:25
	osd2_ch_green                   : 2,	// 26:27
	osd2_ch_red                     : 2,	// 28:29
	osd2_ch_alpha                   : 2;	// 30:31
} OSD2_H14_CTRL_PDEC;

/*-----------------------------------------------------------------------------
	0x0190 osd2_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_sc_bc_sa                   : 2,	//  0: 1
	osd0_sc_force_vsize_en          : 1,	//     2
	osd2_sc_bilinear                : 1,	//     3
	osd2_sc_vdouble                 : 1,	//     4
	                                : 3,	//  5: 7 reserved
	osd2_sc_sampling_mode           : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd2_sc_bnd_mode                : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd2_sc_h_phase_off             : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	osd2_sc_v_phase_off             : 6,	// 24:29
	osd2_odd_width_en               : 1,	//    30
	osd2_fifo_en                    : 1;	//    31
} OSD2_H14_CTRL_SCALER;

/*-----------------------------------------------------------------------------
	0x0194 osd2_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
#if 1
	osd2_r_3d_disparity_value       : 16,	//  0:15
	osd2_l_3d_disparity_value       : 16;	// 16:31
#else
	osd2_r_3d_disparity_value       : 8,	//  0: 7
	osd2_r_3d_disparity_sign        : 1,	//     8
	                                : 7,	//  9:15 reserved
	osd2_l_3d_disparity_value       : 8,	// 16:23
	osd2_l_3d_disparity_sign        : 1;	//    24
#endif
} OSD2_H14_CTRL_3D_DISPARITY;

/*-----------------------------------------------------------------------------
	0x0198 osd2_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_ctrl_3d_blank              ;   	// 31: 0
} OSD2_H14_CTRL_3D_BLANK;

/*-----------------------------------------------------------------------------
	0x01a0 osd2_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_hdr_ypos                   :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd2_hdr_xpos                   :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd2_hdr_color_key_en           : 1;	//    31
} OSD2_H14_CTRL_HDR0;

/*-----------------------------------------------------------------------------
	0x01a4 osd2_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_hdr_h_mem                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd2_hdr_w_mem                  :12;	// 16:27
} OSD2_H14_CTRL_HDR1;

/*-----------------------------------------------------------------------------
	0x01a8 osd2_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_hdr_h_out                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd2_hdr_w_out                  :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd2_hdr_pixel_order            : 1;	//    31
} OSD2_H14_CTRL_HDR2;

/*-----------------------------------------------------------------------------
	0x01ac osd2_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_hdr_wpl                    :16,	//  0:15
	osd2_hdr_global_alpha           : 8,	// 16:23
	osd2_hdr_format                 : 4,	// 24:27
	osd2_hdr_depth                  : 3,	// 28:30
	osd2_hdr_global_alpha_en        : 1;	//    31
} OSD2_H14_CTRL_HDR3;

/*-----------------------------------------------------------------------------
	0x01b0 osd2_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_hdr_color_key              ;   	// 31: 0
} OSD2_H14_CTRL_HDR4;

/*-----------------------------------------------------------------------------
	0x01b4 osd2_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_hdr_ptr_plte               ;   	// 31: 0
} OSD2_H14_CTRL_HDR5;

/*-----------------------------------------------------------------------------
	0x01b8 osd2_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd2_hdr_ptr_bmp                ;   	// 31: 0
} OSD2_H14_CTRL_HDR6;

/*-----------------------------------------------------------------------------
	0x01c0 osd3_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_en                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	osd3_hdr_src_sel                : 1,	//     4
	osd3_unaligned_addr_en          : 1,	//     5
	                                : 2,	//  6: 7 reserved
	osd3_v_reverse_en               : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd3_s3d_en                     : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd3_manual_line_inc            : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	osd3_manual_line_inc_en         : 1,	//    20
	                                : 3,	// 21:23 reserved
	osd3_yuyv_mode_en               : 1,	//    24
	                                : 3,	// 25:27 reserved
	osd3_hdr_ready_flag_en          : 1,	//    28
	osd3_hdr_ready_flag             : 1;	//    29
} OSD3_H14_CTRL_MAIN;

/*-----------------------------------------------------------------------------
	0x01c4 osd3_ctrl_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_base_addr                  ;   	// 31: 0
} OSD3_H14_CTRL_BASE_ADDR;

/*-----------------------------------------------------------------------------
	0x01c8 osd3_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_bbs                        : 1,	//     0
	osd3_bqws                       : 1,	//     1
	osd3_bhws                       : 1,	//     2
	                                : 1,	//     3 reserved
	osd3_pbs                        : 1,	//     4
	osd3_pqws                       : 1,	//     5
	osd3_phws                       : 1,	//     6
	                                : 1,	//     7 reserved
	osd3_hbs                        : 1,	//     8
	osd3_hqws                       : 1,	//     9
	osd3_hhws                       : 1;	//    10
} OSD3_H14_CTRL_SWAP;

/*-----------------------------------------------------------------------------
	0x01cc osd3_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_lfsr_seed                  :19,	//  0:18
	osd3_pure_ck                    : 1,	//    19
	osd3_round                      : 3,	// 20:22
	                                : 1,	//    23 reserved
	osd3_ch_blue                    : 2,	// 24:25
	osd3_ch_green                   : 2,	// 26:27
	osd3_ch_red                     : 2,	// 28:29
	osd3_ch_alpha                   : 2;	// 30:31
} OSD3_H14_CTRL_PDEC;

/*-----------------------------------------------------------------------------
	0x01d0 osd3_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_sc_bc_sa                   : 2,	//  0: 1
	osd0_sc_force_vsize_en          : 1,	//     2
	osd3_sc_bilinear                : 1,	//     3
	osd3_sc_vdouble                 : 1,	//     4
	                                : 3,	//  5: 7 reserved
	osd3_sc_sampling_mode           : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd3_sc_bnd_mode                : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd3_sc_h_phase_off             : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	osd3_sc_v_phase_off             : 6,	// 24:29
	osd3_odd_width_en               : 1,	//    30
	osd3_fifo_en                    : 1;	//    31
} OSD3_H14_CTRL_SCALER;

/*-----------------------------------------------------------------------------
	0x01d4 osd3_ctrl_3d_disparity ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
#if 1
	osd3_r_3d_disparity_value       : 16,	//  0:15
	osd3_l_3d_disparity_value       : 16;	// 16:31
#else
	osd3_r_3d_disparity_value       : 8,	//  0: 7
	osd3_r_3d_disparity_sign        : 1,	//     8
	                                : 7,	//  9:15 reserved
	osd3_l_3d_disparity_value       : 8,	// 16:23
	osd3_l_3d_disparity_sign        : 1;	//    24
#endif
} OSD3_H14_CTRL_3D_DISPARITY;

/*-----------------------------------------------------------------------------
	0x01d8 osd3_ctrl_3d_blank ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_ctrl_3d_blank              ;   	// 31: 0
} OSD3_H14_CTRL_3D_BLANK;

/*-----------------------------------------------------------------------------
	0x01e0 osd3_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hdr_ypos                   :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd3_hdr_xpos                   :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd3_hdr_color_key_en           : 1;	//    31
} OSD3_H14_CTRL_HDR0;

/*-----------------------------------------------------------------------------
	0x01e4 osd3_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hdr_h_mem                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd3_hdr_w_mem                  :12;	// 16:27
} OSD3_H14_CTRL_HDR1;

/*-----------------------------------------------------------------------------
	0x01e8 osd3_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hdr_h_out                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd3_hdr_w_out                  :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd3_hdr_pixel_order            : 1;	//    31
} OSD3_H14_CTRL_HDR2;

/*-----------------------------------------------------------------------------
	0x01ec osd3_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hdr_wpl                    :16,	//  0:15
	osd3_hdr_global_alpha           : 8,	// 16:23
	osd3_hdr_format                 : 4,	// 24:27
	osd3_hdr_depth                  : 3,	// 28:30
	osd3_hdr_global_alpha_en        : 1;	//    31
} OSD3_H14_CTRL_HDR3;

/*-----------------------------------------------------------------------------
	0x01f0 osd3_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hdr_color_key              ;   	// 31: 0
} OSD3_H14_CTRL_HDR4;

/*-----------------------------------------------------------------------------
	0x01f4 osd3_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hdr_ptr_plte               ;   	// 31: 0
} OSD3_H14_CTRL_HDR5;

/*-----------------------------------------------------------------------------
	0x01f8 osd3_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd3_hdr_ptr_bmp                ;   	// 31: 0
} OSD3_H14_CTRL_HDR6;

typedef struct {
	OSD_H14_STATUS_MON_CFG              	osd_status_mon_cfg              ;	// 0x0000 : ''
	OSD_H14_STATUS_MON_POS              	osd_status_mon_pos              ;	// 0x0004 : ''
	OSD_H14_STATUS_VERSION              	osd_status_version              ;	// 0x0008 : ''
	OSD_H14_STATUS_MIXER_LINE           	osd_status_mixer_line           ;	// 0x000c : ''
	OSD_H14_STATUS_MON_DATA0            	osd_status_mon_data0            ;	// 0x0010 : ''
	OSD_H14_STATUS_MON_DATA1            	osd_status_mon_data1            ;	// 0x0014 : ''
	OSD_H14_STATUS_MON_DATA2            	osd_status_mon_data2            ;	// 0x0018 : ''
	OSD_H14_STATUS_MON_DATA3            	osd_status_mon_data3            ;	// 0x001c : ''
	OSD_H14_STATUS_MON_DATA4            	osd_status_mon_data4            ;	// 0x0020 : ''
	OSD_H14_STATUS_MON_DATA5            	osd_status_mon_data5            ;	// 0x0024 : ''
	OSD_H14_STATUS_MON_DATA6            	osd_status_mon_data6            ;	// 0x0028 : ''
	OSD_H14_STATUS_MON_DATA7            	osd_status_mon_data7            ;	// 0x002c : ''
	OSD_H14_STATUS_MON_DATA8            	osd_status_mon_data8            ;	// 0x0030 : ''
	OSD_H14_STATUS_MON_DATA9            	osd_status_mon_data9            ;	// 0x0034 : ''
	OSD_H14_STATUS_MON_DATAA            	osd_status_mon_dataa            ;	// 0x0038 : ''
	OSD_H14_STATUS_MON_DATAB            	osd_status_mon_datab            ;	// 0x003c : ''
	OSD_H14_COMMON_DISP_SIZE            	osd_common_disp_size            ;	// 0x0040 : ''
	OSD_H14_COMMON_CTRL_IRQ             	osd_common_ctrl_irq             ;	// 0x0044 : ''
	OSD_H14_COMMON_CTRL_PIC_INIT        	osd_common_ctrl_pic_init        ;	// 0x0048 : ''
	OSD_H14_COMMON_CTRL_PIC_TIMER       	osd_common_ctrl_pic_timer       ;	// 0x004c : ''
	OSD_H14_COMMON_CTRL_3DFP_OUT        	osd_common_ctrl_3dfp_out        ;	// 0x0050 : ''
	OSD_H14_COMMON_CTRL_PQ_PATH         	osd_common_ctrl_pq_path         ;	// 0x0054 : ''
	OSD_H14_COMMON_PLTE_SEL             	osd_common_plte_sel             ;	// 0x0058 : ''
	OSD_H14_COMMON_OSD_H14_VALID_TH         	osd_common_osd_valid_th         ;	// 0x005c : ''
	OSD_H14_COMMON_CSC_CTRL             	osd_common_csc_ctrl             ;	// 0x0060 : ''
	OSD_H14_COMMON_CSC_COEF0            	osd_common_csc_coef0            ;	// 0x0064 : ''
	OSD_H14_COMMON_CSC_COEF1            	osd_common_csc_coef1            ;	// 0x0068 : ''
	UINT32                          	                 __rsvd_00[   1];	// 0x006c
	OSD_H14_COMMON_SEP_OSD_H14_CTRL         	osd_common_sep_osd_ctrl         ;	// 0x0070 : ''
	OSD_H14_COMMON_SEP_OSD_H14_HDR0         	osd_common_sep_osd_hdr0         ;	// 0x0074 : ''
	OSD_H14_COMMON_SEP_OSD_H14_HDR1         	osd_common_sep_osd_hdr1         ;	// 0x0078 : ''
	OSD_H14_COMMON_SEP_OSD_H14_HDR2         	osd_common_sep_osd_hdr2         ;	// 0x007c : ''
	OSD_H14_COMMON_SEP_OSD_H14_HDR3         	osd_common_sep_osd_hdr3         ;	// 0x0080 : ''
	OSD_H14_COMMON_SEP_OSD_H14_HDR4         	osd_common_sep_osd_hdr4         ;	// 0x0084 : ''
	OSD_H14_COMMON_SEP_OSD_H14_HDR5         	osd_common_sep_osd_hdr5         ;	// 0x0088 : ''
	OSD_H14_COMMON_SEP_OSD_H14_HDR6         	osd_common_sep_osd_hdr6         ;	// 0x008c : ''
	OSD_H14_COMMON_CTRL_TPG             	osd_common_ctrl_tpg             ;	// 0x0090 : ''
	OSD_H14_COMMON_60HZ_DISP_SIZE       	osd_common_60hz_disp_size       ;	// 0x0094 : ''
	OSD_H14_COMMON_60HZ_CTRL_PATH       	osd_common_60hz_ctrl_path       ;	// 0x0098 : ''
	OSD_H14_COMMON_60HZ_OSD_H14_VALID_TH    	osd_common_60hz_osd_valid_th    ;	// 0x009c : ''
	OSD_H14_MIXER_CTRL_ONOFF            	osd_mixer_ctrl_onoff            ;	// 0x00a0 : ''
	OSD_H14_MIXER_CTRL_OUTPUT           	osd_mixer_ctrl_output           ;	// 0x00a4 : ''
	OSD_H14_MIXER_CTRL_AWB              	osd_mixer_ctrl_awb              ;	// 0x00a8 : ''
	OSD_H14_MIXER_CTRL_SYNC             	osd_mixer_ctrl_sync             ;	// 0x00ac : ''
	OSD_H14_MIXER_CTRL_BLEND            	osd_mixer_ctrl_blend            ;	// 0x00b0 : ''
	OSD_H14_MIXER_60HZ_CTRL_AWB         	osd_mixer_60hz_ctrl_awb         ;	// 0x00b4 : ''
	OSD_H14_MIXER_60HZ_CTRL_BLEND       	osd_mixer_60hz_ctrl_blend       ;	// 0x00b8 : ''
	OSD_H14_MIXER_CTRL_HOLE             	osd_mixer_ctrl_hole             ;	// 0x00bc : ''
	OSD_H14_MIXER_CTRL_MUX0             	osd_mixer_ctrl_mux0             ;	// 0x00c0 : ''
	OSD_H14_MIXER_CTRL_MUX1             	osd_mixer_ctrl_mux1             ;	// 0x00c4 : ''
	OSD_H14_MIXER_CTRL_MUX2             	osd_mixer_ctrl_mux2             ;	// 0x00c8 : ''
	OSD_H14_MIXER_CTRL_MUX3             	osd_mixer_ctrl_mux3             ;	// 0x00cc : ''
	OSD_H14_MIXER_CTRL_MUX4             	osd_mixer_ctrl_mux4             ;	// 0x00d0 : ''
	OSD_H14_MIXER_CTRL_MUX5             	osd_mixer_ctrl_mux5             ;	// 0x00d4 : ''
	OSD_H14_MIXER_CTRL_MUX6             	osd_mixer_ctrl_mux6             ;	// 0x00d8 : ''
	OSD_H14_MIXER_CTRL_MUX7             	osd_mixer_ctrl_mux7             ;	// 0x00dc : ''
	OSD_H14_MIXER_CTRL_MUX8             	osd_mixer_ctrl_mux8             ;	// 0x00e0 : ''
	OSD_H14_MIXER_CTRL_MUX9             	osd_mixer_ctrl_mux9             ;	// 0x00e4 : ''
	OSD_H14_MIXER_CTRL_MUXA             	osd_mixer_ctrl_muxa             ;	// 0x00e8 : ''
	OSD_H14_MIXER_CTRL_MUXB             	osd_mixer_ctrl_muxb             ;	// 0x00ec : ''
	OSD_H14_MIXER_CTRL_MUXC             	osd_mixer_ctrl_muxc             ;	// 0x00f0 : ''
	OSD_H14_MIXER_CTRL_MUXD             	osd_mixer_ctrl_muxd             ;	// 0x00f4 : ''
	OSD_H14_MIXER_CTRL_MUXE             	osd_mixer_ctrl_muxe             ;	// 0x00f8 : ''
	OSD_H14_MIXER_CTRL_MUXF             	osd_mixer_ctrl_muxf             ;	// 0x00fc : ''
	OSD0_H14_CTRL_MAIN                  	osd0_ctrl_main                  ;	// 0x0100 : ''
	OSD0_H14_BASE_ADDR                  	osd0_ctrl_base_addr             ;	// 0x0104 : ''
	OSD0_H14_CTRL_SWAP                  	osd0_ctrl_swap                  ;	// 0x0108 : ''
	OSD0_H14_CTRL_PDEC                  	osd0_ctrl_pdec                  ;	// 0x010c : ''
	OSD0_H14_CTRL_SCALER                	osd0_ctrl_scaler                ;	// 0x0110 : ''
	OSD0_H14_CTRL_3D_DISPARITY          	osd0_ctrl_3d_disparity          ;	// 0x0114 : ''
	OSD0_H14_CTRL_3D_BLANK              	osd0_ctrl_3d_blank              ;	// 0x0118 : ''
	UINT32                          	                 __rsvd_01[   1];	// 0x011c
	OSD0_H14_CTRL_HDR0                  	osd0_ctrl_hdr0                  ;	// 0x0120 : ''
	OSD0_H14_CTRL_HDR1                  	osd0_ctrl_hdr1                  ;	// 0x0124 : ''
	OSD0_H14_CTRL_HDR2                  	osd0_ctrl_hdr2                  ;	// 0x0128 : ''
	OSD0_H14_CTRL_HDR3                  	osd0_ctrl_hdr3                  ;	// 0x012c : ''
	OSD0_H14_CTRL_HDR4                  	osd0_ctrl_hdr4                  ;	// 0x0130 : ''
	OSD0_H14_CTRL_HDR5                  	osd0_ctrl_hdr5                  ;	// 0x0134 : ''
	OSD0_H14_CTRL_HDR6                  	osd0_ctrl_hdr6                  ;	// 0x0138 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x013c
	OSD1_H14_CTRL_MAIN                  	osd1_ctrl_main                  ;	// 0x0140 : ''
	OSD1_H14_CTRL_BASE_ADDR             	osd1_ctrl_base_addr             ;	// 0x0144 : ''
	OSD1_H14_CTRL_SWAP                  	osd1_ctrl_swap                  ;	// 0x0148 : ''
	OSD1_H14_CTRL_PDEC                  	osd1_ctrl_pdec                  ;	// 0x014c : ''
	OSD1_H14_CTRL_SCALER                	osd1_ctrl_scaler                ;	// 0x0150 : ''
	OSD1_H14_CTRL_3D_DISPARITY          	osd1_ctrl_3d_disparity          ;	// 0x0154 : ''
	OSD1_H14_CTRL_3D_BLANK              	osd1_ctrl_3d_blank              ;	// 0x0158 : ''
	UINT32                          	                 __rsvd_03[   1];	// 0x015c
	OSD1_H14_CTRL_HDR0                  	osd1_ctrl_hdr0                  ;	// 0x0160 : ''
	OSD1_H14_CTRL_HDR1                  	osd1_ctrl_hdr1                  ;	// 0x0164 : ''
	OSD1_H14_CTRL_HDR2                  	osd1_ctrl_hdr2                  ;	// 0x0168 : ''
	OSD1_H14_CTRL_HDR3                  	osd1_ctrl_hdr3                  ;	// 0x016c : ''
	OSD1_H14_CTRL_HDR4                  	osd1_ctrl_hdr4                  ;	// 0x0170 : ''
	OSD1_H14_CTRL_HDR5                  	osd1_ctrl_hdr5                  ;	// 0x0174 : ''
	OSD1_H14_CTRL_HDR6                  	osd1_ctrl_hdr6                  ;	// 0x0178 : ''
	UINT32                          	                 __rsvd_04[   1];	// 0x017c
	OSD2_H14_CTRL_MAIN                  	osd2_ctrl_main                  ;	// 0x0180 : ''
	OSD2_H14_CTRL_BASE_ADDR             	osd2_ctrl_base_addr             ;	// 0x0184 : ''
	OSD2_H14_CTRL_SWAP                  	osd2_ctrl_swap                  ;	// 0x0188 : ''
	OSD2_H14_CTRL_PDEC                  	osd2_ctrl_pdec                  ;	// 0x018c : ''
	OSD2_H14_CTRL_SCALER                	osd2_ctrl_scaler                ;	// 0x0190 : ''
	OSD2_H14_CTRL_3D_DISPARITY          	osd2_ctrl_3d_disparity          ;	// 0x0194 : ''
	OSD2_H14_CTRL_3D_BLANK              	osd2_ctrl_3d_blank              ;	// 0x0198 : ''
	UINT32                          	                 __rsvd_05[   1];	// 0x019c
	OSD2_H14_CTRL_HDR0                  	osd2_ctrl_hdr0                  ;	// 0x01a0 : ''
	OSD2_H14_CTRL_HDR1                  	osd2_ctrl_hdr1                  ;	// 0x01a4 : ''
	OSD2_H14_CTRL_HDR2                  	osd2_ctrl_hdr2                  ;	// 0x01a8 : ''
	OSD2_H14_CTRL_HDR3                  	osd2_ctrl_hdr3                  ;	// 0x01ac : ''
	OSD2_H14_CTRL_HDR4                  	osd2_ctrl_hdr4                  ;	// 0x01b0 : ''
	OSD2_H14_CTRL_HDR5                  	osd2_ctrl_hdr5                  ;	// 0x01b4 : ''
	OSD2_H14_CTRL_HDR6                  	osd2_ctrl_hdr6                  ;	// 0x01b8 : ''
	UINT32                          	                 __rsvd_06[   1];	// 0x01bc
	OSD3_H14_CTRL_MAIN                  	osd3_ctrl_main                  ;	// 0x01c0 : ''
	OSD3_H14_CTRL_BASE_ADDR             	osd3_ctrl_base_addr             ;	// 0x01c4 : ''
	OSD3_H14_CTRL_SWAP                  	osd3_ctrl_swap                  ;	// 0x01c8 : ''
	OSD3_H14_CTRL_PDEC                  	osd3_ctrl_pdec                  ;	// 0x01cc : ''
	OSD3_H14_CTRL_SCALER                	osd3_ctrl_scaler                ;	// 0x01d0 : ''
	OSD3_H14_CTRL_3D_DISPARITY          	osd3_ctrl_3d_disparity          ;	// 0x01d4 : ''
	OSD3_H14_CTRL_3D_BLANK              	osd3_ctrl_3d_blank              ;	// 0x01d8 : ''
	UINT32                          	                 __rsvd_07[   1];	// 0x01dc
	OSD3_H14_CTRL_HDR0                  	osd3_ctrl_hdr0                  ;	// 0x01e0 : ''
	OSD3_H14_CTRL_HDR1                  	osd3_ctrl_hdr1                  ;	// 0x01e4 : ''
	OSD3_H14_CTRL_HDR2                  	osd3_ctrl_hdr2                  ;	// 0x01e8 : ''
	OSD3_H14_CTRL_HDR3                  	osd3_ctrl_hdr3                  ;	// 0x01ec : ''
	OSD3_H14_CTRL_HDR4                  	osd3_ctrl_hdr4                  ;	// 0x01f0 : ''
	OSD3_H14_CTRL_HDR5                  	osd3_ctrl_hdr5                  ;	// 0x01f4 : ''
	OSD3_H14_CTRL_HDR6                  	osd3_ctrl_hdr6                  ;	// 0x01f8 : ''
} OSD_H14_REG_T;
/* 119 regs, 119 types */

/* 119 regs, 119 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * g_fbdev_h14_reg : real register of OSD.
 * g_fbdev_h14_reg_cached     : shadow register.
 *
 * @def OSD_H14_RdFL: Read  FLushing : Shadow <- Real.
 * @def OSD_H14_WrFL: Write FLushing : Shadow -> Real.
 * @def OSD_H14_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def OSD_H14_Wr  : Write whole register(UINT32) from Shadow register.
 * @def OSD_H14_Rd01 ~ OSD_H14_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def OSD_H14_Wr01 ~ OSD_H14_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define OSD_H14_RdFL(_r)			((g_fbdev_h14_reg_cached->_r)=(g_fbdev_h14_reg->_r))
#define OSD_H14_WrFL(_r)			((g_fbdev_h14_reg->_r)=(g_fbdev_h14_reg_cached->_r))

#define OSD_H14_Rd(_r)			*((UINT32*)(&(g_fbdev_h14_reg_cached->_r)))
#define OSD_H14_Wr(_r,_v)			((OSD_H14_Rd(_r))=((UINT32)(_v)))

#define OSD_H14_Rd00(_r,_f)         (g_fbdev_h14_reg_cached->_r._f)


#define OSD_H14_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
								} while(0)

#define OSD_H14_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
								} while(0)

#define OSD_H14_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
								} while(0)

#define OSD_H14_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
								} while(0)

#define OSD_H14_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
								} while(0)

#define OSD_H14_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
								} while(0)

#define OSD_H14_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
								} while(0)

#define OSD_H14_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
								} while(0)

#define OSD_H14_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
								} while(0)

#define OSD_H14_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
									(_v10) = (g_fbdev_h14_reg_cached->_r._f10);				\
								} while(0)

#define OSD_H14_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
									(_v10) = (g_fbdev_h14_reg_cached->_r._f10);				\
									(_v11) = (g_fbdev_h14_reg_cached->_r._f11);				\
								} while(0)

#define OSD_H14_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
									(_v10) = (g_fbdev_h14_reg_cached->_r._f10);				\
									(_v11) = (g_fbdev_h14_reg_cached->_r._f11);				\
									(_v12) = (g_fbdev_h14_reg_cached->_r._f12);				\
								} while(0)

#define OSD_H14_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
									(_v10) = (g_fbdev_h14_reg_cached->_r._f10);				\
									(_v11) = (g_fbdev_h14_reg_cached->_r._f11);				\
									(_v12) = (g_fbdev_h14_reg_cached->_r._f12);				\
									(_v13) = (g_fbdev_h14_reg_cached->_r._f13);				\
								} while(0)

#define OSD_H14_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
									(_v10) = (g_fbdev_h14_reg_cached->_r._f10);				\
									(_v11) = (g_fbdev_h14_reg_cached->_r._f11);				\
									(_v12) = (g_fbdev_h14_reg_cached->_r._f12);				\
									(_v13) = (g_fbdev_h14_reg_cached->_r._f13);				\
									(_v14) = (g_fbdev_h14_reg_cached->_r._f14);				\
								} while(0)

#define OSD_H14_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
									(_v10) = (g_fbdev_h14_reg_cached->_r._f10);				\
									(_v11) = (g_fbdev_h14_reg_cached->_r._f11);				\
									(_v12) = (g_fbdev_h14_reg_cached->_r._f12);				\
									(_v13) = (g_fbdev_h14_reg_cached->_r._f13);				\
									(_v14) = (g_fbdev_h14_reg_cached->_r._f14);				\
									(_v15) = (g_fbdev_h14_reg_cached->_r._f15);				\
								} while(0)

#define OSD_H14_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (g_fbdev_h14_reg_cached->_r._f01);				\
									(_v02) = (g_fbdev_h14_reg_cached->_r._f02);				\
									(_v03) = (g_fbdev_h14_reg_cached->_r._f03);				\
									(_v04) = (g_fbdev_h14_reg_cached->_r._f04);				\
									(_v05) = (g_fbdev_h14_reg_cached->_r._f05);				\
									(_v06) = (g_fbdev_h14_reg_cached->_r._f06);				\
									(_v07) = (g_fbdev_h14_reg_cached->_r._f07);				\
									(_v08) = (g_fbdev_h14_reg_cached->_r._f08);				\
									(_v09) = (g_fbdev_h14_reg_cached->_r._f09);				\
									(_v10) = (g_fbdev_h14_reg_cached->_r._f10);				\
									(_v11) = (g_fbdev_h14_reg_cached->_r._f11);				\
									(_v12) = (g_fbdev_h14_reg_cached->_r._f12);				\
									(_v13) = (g_fbdev_h14_reg_cached->_r._f13);				\
									(_v14) = (g_fbdev_h14_reg_cached->_r._f14);				\
									(_v15) = (g_fbdev_h14_reg_cached->_r._f15);				\
									(_v16) = (g_fbdev_h14_reg_cached->_r._f16);				\
								} while(0)


#define OSD_H14_Wr01(_r,_f01,_v01)													\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
								} while(0)

#define OSD_H14_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
								} while(0)

#define OSD_H14_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
								} while(0)

#define OSD_H14_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
								} while(0)

#define OSD_H14_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
								} while(0)

#define OSD_H14_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
								} while(0)

#define OSD_H14_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
								} while(0)

#define OSD_H14_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
								} while(0)

#define OSD_H14_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
								} while(0)

#define OSD_H14_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
									(g_fbdev_h14_reg_cached->_r._f10) = (_v10);				\
								} while(0)

#define OSD_H14_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
									(g_fbdev_h14_reg_cached->_r._f10) = (_v10);				\
									(g_fbdev_h14_reg_cached->_r._f11) = (_v11);				\
								} while(0)

#define OSD_H14_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
									(g_fbdev_h14_reg_cached->_r._f10) = (_v10);				\
									(g_fbdev_h14_reg_cached->_r._f11) = (_v11);				\
									(g_fbdev_h14_reg_cached->_r._f12) = (_v12);				\
								} while(0)

#define OSD_H14_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
									(g_fbdev_h14_reg_cached->_r._f10) = (_v10);				\
									(g_fbdev_h14_reg_cached->_r._f11) = (_v11);				\
									(g_fbdev_h14_reg_cached->_r._f12) = (_v12);				\
									(g_fbdev_h14_reg_cached->_r._f13) = (_v13);				\
								} while(0)

#define OSD_H14_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
									(g_fbdev_h14_reg_cached->_r._f10) = (_v10);				\
									(g_fbdev_h14_reg_cached->_r._f11) = (_v11);				\
									(g_fbdev_h14_reg_cached->_r._f12) = (_v12);				\
									(g_fbdev_h14_reg_cached->_r._f13) = (_v13);				\
									(g_fbdev_h14_reg_cached->_r._f14) = (_v14);				\
								} while(0)

#define OSD_H14_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
									(g_fbdev_h14_reg_cached->_r._f10) = (_v10);				\
									(g_fbdev_h14_reg_cached->_r._f11) = (_v11);				\
									(g_fbdev_h14_reg_cached->_r._f12) = (_v12);				\
									(g_fbdev_h14_reg_cached->_r._f13) = (_v13);				\
									(g_fbdev_h14_reg_cached->_r._f14) = (_v14);				\
									(g_fbdev_h14_reg_cached->_r._f15) = (_v15);				\
								} while(0)

#define OSD_H14_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(g_fbdev_h14_reg_cached->_r._f01) = (_v01);				\
									(g_fbdev_h14_reg_cached->_r._f02) = (_v02);				\
									(g_fbdev_h14_reg_cached->_r._f03) = (_v03);				\
									(g_fbdev_h14_reg_cached->_r._f04) = (_v04);				\
									(g_fbdev_h14_reg_cached->_r._f05) = (_v05);				\
									(g_fbdev_h14_reg_cached->_r._f06) = (_v06);				\
									(g_fbdev_h14_reg_cached->_r._f07) = (_v07);				\
									(g_fbdev_h14_reg_cached->_r._f08) = (_v08);				\
									(g_fbdev_h14_reg_cached->_r._f09) = (_v09);				\
									(g_fbdev_h14_reg_cached->_r._f10) = (_v10);				\
									(g_fbdev_h14_reg_cached->_r._f11) = (_v11);				\
									(g_fbdev_h14_reg_cached->_r._f12) = (_v12);				\
									(g_fbdev_h14_reg_cached->_r._f13) = (_v13);				\
									(g_fbdev_h14_reg_cached->_r._f14) = (_v14);				\
									(g_fbdev_h14_reg_cached->_r._f15) = (_v15);				\
									(g_fbdev_h14_reg_cached->_r._f16) = (_v16);				\
								} while(0)

/* Indexed Register Access.
 * 
 * There is in-direct field specified by 'index' field within a register.
 * Normally a register has only one meaning for a 'field_name', but indexed register
 * can hold several data for a 'field_name' specifed by 'index' field of indexed register.
 * When writing an 3rd data for given 'field_name' register, you need to set 'rw' = 0, 'index' = 2,
 * and 'load' = 0.
 * 
 * ASSUMPTION
 * For Writing indexed register load bit 
 * 
 * parameter list
 * _r     : name of register
 * _lname : name of load  bit field	: shall be 0 after macro executed.
 * _rwname: name of rw    bit field : shall be 0 after OSD_H14_Wind(), 1 for OSD_H14_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * OSD_H14_Rind : General indexed register Read.(
 * OSD_H14_Wind : General indexed register Read.
 *
 * OSD_H14_Ridx : For 'index', 'rw', 'load' field name
 * OSD_H14_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define OSD_H14_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								OSD_H14_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								OSD_H14_WrFL(_r);									\
								OSD_H14_RdFL(_r);									\
								OSD_H14_Rd01(_r,_fname,_fval);						\
							} while (0)

#define OSD_H14_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				OSD_H14_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define OSD_H14_Ridx(_r, _ival, _fname, _fval)	OSD_H14_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define OSD_H14_Widx(_r, _ival, _fname, _fval)	OSD_H14_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _FBDEV_REG_H14_H_ */

/* from 'H14_OSD_H14_RegFile.csv' 20120409 16:25:56   대한민국 표준시 by getregs v2.7 */
