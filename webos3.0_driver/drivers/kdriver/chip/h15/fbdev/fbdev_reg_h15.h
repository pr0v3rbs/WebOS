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

/*
 *
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef __FBDEV_REG_H15_H__
#define __FBDEV_REG_H15_H__

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/
#define	FBDEV_USE_REG_GROUP
#define	FBDEV_REG_TEST

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
} OSD_STATUS_MON_CFG;

/*-----------------------------------------------------------------------------
	0x0004 osd_status_mon_pos ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_ypos             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_status_mon_xpos             :12;	// 16:27
} OSD_STATUS_MON_POS;

/*-----------------------------------------------------------------------------
	0x0008 osd_status_version ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_version              ;   	// 31: 0
} OSD_STATUS_VERSION;

/*-----------------------------------------------------------------------------
	0x000c osd_status_mixer_line ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mixer_line           :12,	//  0:11
	reg_status_pic_init_count       : 4,	// 12:15
	reg_60hz_status_mixer_line      :12,	// 16:27
	reg_60hz_status_pic_init_count  : 4;	// 28:31
} OSD_STATUS_MIXER_LINE;

#ifdef FBDEV_USE_REG_GROUP
typedef struct {
	UINT32
	reg_status_mon_data            ;   	// 31: 0
} OSDX_STATUS_MON_DATA;
#else
/*-----------------------------------------------------------------------------
	0x0010 osd_status_mon_data0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data0            ;   	// 31: 0
} OSD_STATUS_MON_DATA0;

/*-----------------------------------------------------------------------------
	0x0014 osd_status_mon_data1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data1            ;   	// 31: 0
} OSD_STATUS_MON_DATA1;

/*-----------------------------------------------------------------------------
	0x0018 osd_status_mon_data2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data2            ;   	// 31: 0
} OSD_STATUS_MON_DATA2;

/*-----------------------------------------------------------------------------
	0x001c osd_status_mon_data3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data3            ;   	// 31: 0
} OSD_STATUS_MON_DATA3;

/*-----------------------------------------------------------------------------
	0x0020 osd_status_mon_data4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data4            ;   	// 31: 0
} OSD_STATUS_MON_DATA4;

/*-----------------------------------------------------------------------------
	0x0024 osd_status_mon_data5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data5            ;   	// 31: 0
} OSD_STATUS_MON_DATA5;

/*-----------------------------------------------------------------------------
	0x0028 osd_status_mon_data6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data6            ;   	// 31: 0
} OSD_STATUS_MON_DATA6;

/*-----------------------------------------------------------------------------
	0x002c osd_status_mon_data7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data7            ;   	// 31: 0
} OSD_STATUS_MON_DATA7;

/*-----------------------------------------------------------------------------
	0x0030 osd_status_mon_data8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data8            ;   	// 31: 0
} OSD_STATUS_MON_DATA8;

/*-----------------------------------------------------------------------------
	0x0034 osd_status_mon_data9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_data9            ;   	// 31: 0
} OSD_STATUS_MON_DATA9;

/*-----------------------------------------------------------------------------
	0x0038 osd_status_mon_dataa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_dataa            ;   	// 31: 0
} OSD_STATUS_MON_DATAA;

/*-----------------------------------------------------------------------------
	0x003c osd_status_mon_datab ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_mon_datab            ;   	// 31: 0
} OSD_STATUS_MON_DATAB;
#endif

/*-----------------------------------------------------------------------------
	0x0040 osd_common_plte_sel ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lm_mode_osd01               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_lm_mode_osd23               : 1,	//     4
	                                :11,	//  5:15 reserved
	reg_common_plte_sel             : 3;	// 16:18
} OSD_COMMON_PLTE_SEL;

/*-----------------------------------------------------------------------------
	0x0044 osd_common_ctrl_irq ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_ctrl_irq_clear_osd0  : 1,	//     0
	reg_common_ctrl_irq_clear_osd1  : 1,	//     1
	reg_common_ctrl_irq_clear_osd2  : 1,	//     2
	reg_common_ctrl_irq_clear_osd3  : 1,	//     3
	reg_common_ctrl_irq_clear_osd4  : 1,	//     4
	reg_common_ctrl_irq_clear_osd5  : 1,	//     5
	reg_common_ctrl_irq_clear_osd6  : 1,	//     6
	reg_common_ctrl_irq_clear_osd7  : 1,	//     7
	reg_common_ctrl_irq_clear_bosd  : 1,	//     8
	reg_common_ctrl_irq_clear_sosd  : 1,	//     9
	                                : 6,	// 10:15 reserved
	reg_common_ctrl_irq_en_osd0     : 1,	//    16
	reg_common_ctrl_irq_en_osd1     : 1,	//    17
	reg_common_ctrl_irq_en_osd2     : 1,	//    18
	reg_common_ctrl_irq_en_osd3     : 1,	//    19
	reg_common_ctrl_irq_en_osd4     : 1,	//    20
	reg_common_ctrl_irq_en_osd5     : 1,	//    21
	reg_common_ctrl_irq_en_osd6     : 1,	//    22
	reg_common_ctrl_irq_en_osd7     : 1,	//    23
	reg_common_ctrl_irq_en_mixer_bos: 1,	//    24
	reg_common_ctrl_irq_en_mixer_sos: 1;	//    25
} OSD_COMMON_CTRL_IRQ;

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
} OSD_COMMON_CTRL_PIC_INIT;

/*-----------------------------------------------------------------------------
	0x004c osd_common_ctrl_pic_timer ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_pic_init_timer       ;   	// 31: 0
} OSD_COMMON_CTRL_PIC_TIMER;

#ifdef FBDEV_USE_REG_GROUP
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSDX_COMMON_DISP_SIZE;
#else
/*-----------------------------------------------------------------------------
	0x0050 osd0_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD0_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0054 osd1_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD1_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0058 osd2_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD2_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x005c osd3_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD3_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0060 osd4_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD4_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0064 osd5_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD5_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0068 osd6_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD6_COMMON_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x006c osd7_common_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_disp_size_h          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_common_disp_size_w          :12;	// 16:27
} OSD7_COMMON_DISP_SIZE;
#endif

/*-----------------------------------------------------------------------------
	0x0080 ctrl_sosd_mixer_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sosd_disp_size_h            :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_sosd_disp_size_w            :12;	// 16:27
} CTRL_SOSD_MIXER_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0084 ctrl_sosd_mixer_output ''
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
} CTRL_SOSD_MIXER_OUTPUT;

/*-----------------------------------------------------------------------------
	0x0088 ctrl_sosd_mixer_awb ''
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
} CTRL_SOSD_MIXER_AWB;

/*-----------------------------------------------------------------------------
	0x008c ctrl_sosd_mixer_sync ''
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
} CTRL_SOSD_MIXER_SYNC;

/*-----------------------------------------------------------------------------
	0x0090 ctrl_sosd_mixer_alpha ''
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
} CTRL_SOSD_MIXER_ALPHA;

/*-----------------------------------------------------------------------------
	0x009c ctrl_sosd_mixer_hole ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_en                         : 1,	//     0
	osd0_hole_sel                   : 3,	//  1: 3
	osd1_en                         : 1,	//     4
	osd1_hole_sel                   : 3,	//  5: 7
	osd2_en                         : 1,	//     8
	osd2_hole_sel                   : 3,	//  9:11
	osd3_en                         : 1,	//    12
	osd3_hole_sel                   : 3,	// 13:15
	osd4_en                         : 1,	//    16
	osd4_hole_sel                   : 3,	// 17:19
	osd5_en                         : 1,	//    20
	osd5_hole_sel                   : 3,	// 21:23
	osd6_en                         : 1,	//    24
	osd6_hole_sel                   : 3,	// 25:27
	osd7_en                         : 1,	//    28
	osd7_hole_sel                   : 3;	// 29:31
} CTRL_SOSD_MIXER_HOLE;

/*-----------------------------------------------------------------------------
	0x00a0 ctrl_sosd_mixer_mux0 ''
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
} CTRL_SOSD_MIXER_MUX0;

/*-----------------------------------------------------------------------------
	0x00a4 ctrl_sosd_mixer_mux1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window0_alpha         : 8;	// 16:23
} CTRL_SOSD_MIXER_MUX1;

/*-----------------------------------------------------------------------------
	0x00a8 ctrl_sosd_mixer_mux2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_xpos          :12;	// 16:27
} CTRL_SOSD_MIXER_MUX2;

/*-----------------------------------------------------------------------------
	0x00ac ctrl_sosd_mixer_mux3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_w             :12;	// 16:27
} CTRL_SOSD_MIXER_MUX3;

/*-----------------------------------------------------------------------------
	0x00b0 ctrl_sosd_mixer_mux4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window1_alpha         : 8;	// 16:23
} CTRL_SOSD_MIXER_MUX4;

/*-----------------------------------------------------------------------------
	0x00b4 ctrl_sosd_mixer_mux5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_xpos          :12;	// 16:27
} CTRL_SOSD_MIXER_MUX5;

/*-----------------------------------------------------------------------------
	0x00b8 ctrl_sosd_mixer_mux6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_w             :12;	// 16:27
} CTRL_SOSD_MIXER_MUX6;

/*-----------------------------------------------------------------------------
	0x00bc ctrl_sosd_mixer_mux7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window2_alpha         : 8;	// 16:23
} CTRL_SOSD_MIXER_MUX7;

/*-----------------------------------------------------------------------------
	0x00c0 ctrl_sosd_mixer_mux8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_xpos          :12;	// 16:27
} CTRL_SOSD_MIXER_MUX8;

/*-----------------------------------------------------------------------------
	0x00c4 ctrl_sosd_mixer_mux9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_w             :12;	// 16:27
} CTRL_SOSD_MIXER_MUX9;

/*-----------------------------------------------------------------------------
	0x00c8 ctrl_sosd_mixer_muxa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_w                     :13,	//  0:12
	reg_hole0_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole0_en                    : 1,	//    28
	reg_hole0_reverse_en            : 1;	//    29
} CTRL_SOSD_MIXER_MUXA;

/*-----------------------------------------------------------------------------
	0x00cc ctrl_sosd_mixer_muxb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_h                     :12,	//  0:11
	reg_hole0_y                     :12,	// 12:23
	reg_hole0_a                     : 8;	// 24:31
} CTRL_SOSD_MIXER_MUXB;

/*-----------------------------------------------------------------------------
	0x00d0 ctrl_sosd_mixer_muxc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_w                     :13,	//  0:12
	reg_hole1_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole1_en                    : 1,	//    28
	reg_hole1_reverse_en            : 1;	//    29
} CTRL_SOSD_MIXER_MUXC;

/*-----------------------------------------------------------------------------
	0x00d4 ctrl_sosd_mixer_muxd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_h                     :12,	//  0:11
	reg_hole1_y                     :12,	// 12:23
	reg_hole1_a                     : 8;	// 24:31
} CTRL_SOSD_MIXER_MUXD;

/*-----------------------------------------------------------------------------
	0x00d8 ctrl_sosd_mixer_muxe ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_w                     :13,	//  0:12
	reg_hole2_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole2_en                    : 1,	//    28
	reg_hole2_reverse_en            : 1;	//    29
} CTRL_SOSD_MIXER_MUXE;

/*-----------------------------------------------------------------------------
	0x00dc ctrl_sosd_mixer_muxf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_h                     :12,	//  0:11
	reg_hole2_y                     :12,	// 12:23
	reg_hole2_a                     : 8;	// 24:31
} CTRL_SOSD_MIXER_MUXF;

/*-----------------------------------------------------------------------------
	0x00e0 osd_common_sep_osd_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_common_sep_osd_hdr_en       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_common_sep_osd_sync_sel     : 1,	//     4
	                                :26,	//  5:30 reserved
	reg_common_sep_osd_bypass       : 1;	//    31
} OSD_COMMON_SEP_OSD_CTRL;

/*-----------------------------------------------------------------------------
	0x00e4 osd_common_sep_osd_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_0                   ;   	// 31: 0
} OSD_COMMON_SEP_OSD_HDR0;

/*-----------------------------------------------------------------------------
	0x00e8 osd_common_sep_osd_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_1                   ;   	// 31: 0
} OSD_COMMON_SEP_OSD_HDR1;

/*-----------------------------------------------------------------------------
	0x00ec osd_common_sep_osd_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_2                   ;   	// 31: 0
} OSD_COMMON_SEP_OSD_HDR2;

/*-----------------------------------------------------------------------------
	0x00f0 osd_common_sep_osd_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_3                   ;   	// 31: 0
} OSD_COMMON_SEP_OSD_HDR3;

/*-----------------------------------------------------------------------------
	0x00f4 osd_common_sep_osd_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_4                   ;   	// 31: 0
} OSD_COMMON_SEP_OSD_HDR4;

/*-----------------------------------------------------------------------------
	0x00f8 osd_common_sep_osd_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_5                   ;   	// 31: 0
} OSD_COMMON_SEP_OSD_HDR5;

/*-----------------------------------------------------------------------------
	0x00fc osd_common_sep_osd_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_frc_hdr_6                   ;   	// 31: 0
} OSD_COMMON_SEP_OSD_HDR6;

/*-----------------------------------------------------------------------------
	0x0100 ctrl_bosd0_mixer_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sosd_disp_size_h            :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_sosd_disp_size_w            :12;	// 16:27
} CTRL_BOSD0_MIXER_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0104 ctrl_bosd0_mixer_output ''
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
} CTRL_BOSD0_MIXER_OUTPUT;

/*-----------------------------------------------------------------------------
	0x0108 ctrl_bosd0_mixer_awb ''
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
} CTRL_BOSD0_MIXER_AWB;

/*-----------------------------------------------------------------------------
	0x010c ctrl_bosd0_mixer_sync ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sync_osd3_tp_en             : 1,	//     0
	reg_sync_osd3_tp_aa_en          : 1,	//     1
	reg_blend_sel_10_dn             : 1,	//     2
	reg_blend_sel_10_up             : 1,	//     3
	reg_sync_osd2_tp_en             : 1,	//     4
	reg_sync_osd2_tp_aa_en          : 1,	//     5
	reg_blend_sel_31_dn             : 1,	//     6
	reg_blend_sel_31_up             : 1,	//     7
	reg_sync_osd1_tp_en             : 1,	//     8
	reg_sync_osd1_tp_aa_en          : 1,	//     9
	reg_blend_sel_a10_dn            : 1,	//    10
	reg_blend_sel_a10_a             : 1,	//    11
	reg_sync_osd0_tp_en             : 1,	//    12
	reg_sync_osd0_tp_aa_en          : 1,	//    13
	reg_blend_sel_a31_dn            : 1,	//    14
	reg_blend_sel_a31_a             : 1,	//    15
	reg_sync_tp_alpha               : 8,	// 16:23
	reg_blend_sel_a3d_src           : 1,	//    24
	reg_blend_sel_a5d_src           : 1,	//    25
	                                : 1,	//    26 reserved
	reg_blend_sel_a31_z_3d_src      : 1,	//    27
	reg_blend_sel_out_vid           : 1,	//    28
	reg_blend_sel_out_sepc          : 1,	//    29
	reg_blend_sel_out_sepa          : 1,	//    30
	reg_sync_v_reverse              : 1;	//    31
} CTRL_BOSD0_MIXER_SYNC;

/*-----------------------------------------------------------------------------
	0x011c ctrl_bosd0_mixer_hole ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_en                         : 1,	//     0
	osd0_hole_sel                   : 3,	//  1: 3
	osd1_en                         : 1,	//     4
	osd1_hole_sel                   : 3,	//  5: 7
	osd2_en                         : 1,	//     8
	osd2_hole_sel                   : 3,	//  9:11
	osd3_en                         : 1,	//    12
	osd3_hole_sel                   : 3,	// 13:15
	osd4_en                         : 1,	//    16
	osd4_hole_sel                   : 3,	// 17:19
	osd5_en                         : 1,	//    20
	osd5_hole_sel                   : 3,	// 21:23
	osd6_en                         : 1,	//    24
	osd6_hole_sel                   : 3,	// 25:27
	osd7_en                         : 1,	//    28
	osd7_hole_sel                   : 3;	// 29:31
} CTRL_BOSD0_MIXER_HOLE;

/*-----------------------------------------------------------------------------
	0x0120 ctrl_bosd0_mixer_mux0 ''
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
} CTRL_BOSD0_MIXER_MUX0;

/*-----------------------------------------------------------------------------
	0x0124 ctrl_bosd0_mixer_mux1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window0_alpha         : 8;	// 16:23
} CTRL_BOSD0_MIXER_MUX1;

/*-----------------------------------------------------------------------------
	0x0128 ctrl_bosd0_mixer_mux2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_xpos          :12;	// 16:27
} CTRL_BOSD0_MIXER_MUX2;

/*-----------------------------------------------------------------------------
	0x012c ctrl_bosd0_mixer_mux3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_w             :12;	// 16:27
} CTRL_BOSD0_MIXER_MUX3;

/*-----------------------------------------------------------------------------
	0x0130 ctrl_bosd0_mixer_mux4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window1_alpha         : 8;	// 16:23
} CTRL_BOSD0_MIXER_MUX4;

/*-----------------------------------------------------------------------------
	0x0134 ctrl_bosd0_mixer_mux5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_xpos          :12;	// 16:27
} CTRL_BOSD0_MIXER_MUX5;

/*-----------------------------------------------------------------------------
	0x0138 ctrl_bosd0_mixer_mux6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_w             :12;	// 16:27
} CTRL_BOSD0_MIXER_MUX6;

/*-----------------------------------------------------------------------------
	0x013c ctrl_bosd0_mixer_mux7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window2_alpha         : 8;	// 16:23
} CTRL_BOSD0_MIXER_MUX7;

/*-----------------------------------------------------------------------------
	0x0140 ctrl_bosd0_mixer_mux8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_xpos          :12;	// 16:27
} CTRL_BOSD0_MIXER_MUX8;

/*-----------------------------------------------------------------------------
	0x0144 ctrl_bosd0_mixer_mux9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_w             :12;	// 16:27
} CTRL_BOSD0_MIXER_MUX9;

/*-----------------------------------------------------------------------------
	0x0148 ctrl_bosd0_mixer_muxa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_w                     :13,	//  0:12
	reg_hole0_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole0_en                    : 1,	//    28
	reg_hole0_reverse_en            : 1;	//    29
} CTRL_BOSD0_MIXER_MUXA;

/*-----------------------------------------------------------------------------
	0x014c ctrl_bosd0_mixer_muxb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_h                     :12,	//  0:11
	reg_hole0_y                     :12,	// 12:23
	reg_hole0_a                     : 8;	// 24:31
} CTRL_BOSD0_MIXER_MUXB;

/*-----------------------------------------------------------------------------
	0x0150 ctrl_bosd0_mixer_muxc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_w                     :13,	//  0:12
	reg_hole1_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole1_en                    : 1,	//    28
	reg_hole1_reverse_en            : 1;	//    29
} CTRL_BOSD0_MIXER_MUXC;

/*-----------------------------------------------------------------------------
	0x0154 ctrl_bosd0_mixer_muxd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_h                     :12,	//  0:11
	reg_hole1_y                     :12,	// 12:23
	reg_hole1_a                     : 8;	// 24:31
} CTRL_BOSD0_MIXER_MUXD;

/*-----------------------------------------------------------------------------
	0x0158 ctrl_bosd0_mixer_muxe ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_w                     :13,	//  0:12
	reg_hole2_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole2_en                    : 1,	//    28
	reg_hole2_reverse_en            : 1;	//    29
} CTRL_BOSD0_MIXER_MUXE;

/*-----------------------------------------------------------------------------
	0x015c ctrl_bosd0_mixer_muxf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_h                     :12,	//  0:11
	reg_hole2_y                     :12,	// 12:23
	reg_hole2_a                     : 8;	// 24:31
} CTRL_BOSD0_MIXER_MUXF;

/*-----------------------------------------------------------------------------
	0x0180 ctrl_bosd1_mixer_disp_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sosd_disp_size_h            :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_sosd_disp_size_w            :12;	// 16:27
} CTRL_BOSD1_MIXER_DISP_SIZE;

/*-----------------------------------------------------------------------------
	0x0184 ctrl_bosd1_mixer_output ''
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
} CTRL_BOSD1_MIXER_OUTPUT;

/*-----------------------------------------------------------------------------
	0x0188 ctrl_bosd1_mixer_awb ''
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
} CTRL_BOSD1_MIXER_AWB;

/*-----------------------------------------------------------------------------
	0x018c ctrl_bosd1_mixer_sync ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sync_osd3_tp_en             : 1,	//     0
	reg_sync_osd3_tp_aa_en          : 1,	//     1
	reg_blend_sel_10_dn             : 1,	//     2
	reg_blend_sel_10_up             : 1,	//     3
	reg_sync_osd2_tp_en             : 1,	//     4
	reg_sync_osd2_tp_aa_en          : 1,	//     5
	reg_blend_sel_31_dn             : 1,	//     6
	reg_blend_sel_31_up             : 1,	//     7
	reg_sync_osd1_tp_en             : 1,	//     8
	reg_sync_osd1_tp_aa_en          : 1,	//     9
	reg_blend_sel_a10_dn            : 1,	//    10
	reg_blend_sel_a10_a             : 1,	//    11
	reg_sync_osd0_tp_en             : 1,	//    12
	reg_sync_osd0_tp_aa_en          : 1,	//    13
	reg_blend_sel_a31_dn            : 1,	//    14
	reg_blend_sel_a31_a             : 1,	//    15
	reg_sync_tp_alpha               : 8,	// 16:23
	reg_blend_sel_a3d_src           : 1,	//    24
	reg_blend_sel_a5d_src           : 1,	//    25
	                                : 1,	//    26 reserved
	reg_blend_sel_a31_z_3d_src      : 1,	//    27
	reg_blend_sel_out_vid           : 1,	//    28
	reg_blend_sel_out_sepc          : 1,	//    29
	reg_blend_sel_out_sepa          : 1,	//    30
	reg_sync_v_reverse              : 1;	//    31
} CTRL_BOSD1_MIXER_SYNC;

/*-----------------------------------------------------------------------------
	0x019c ctrl_bosd1_mixer_hole ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd0_en                         : 1,	//     0
	osd0_hole_sel                   : 3,	//  1: 3
	osd1_en                         : 1,	//     4
	osd1_hole_sel                   : 3,	//  5: 7
	osd2_en                         : 1,	//     8
	osd2_hole_sel                   : 3,	//  9:11
	osd3_en                         : 1,	//    12
	osd3_hole_sel                   : 3,	// 13:15
	osd4_en                         : 1,	//    16
	osd4_hole_sel                   : 3,	// 17:19
	osd5_en                         : 1,	//    20
	osd5_hole_sel                   : 3,	// 21:23
	osd6_en                         : 1,	//    24
	osd6_hole_sel                   : 3,	// 25:27
	osd7_en                         : 1,	//    28
	osd7_hole_sel                   : 3;	// 29:31
} CTRL_BOSD1_MIXER_HOLE;

/*-----------------------------------------------------------------------------
	0x01a0 ctrl_bosd1_mixer_mux0 ''
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
} CTRL_BOSD1_MIXER_MUX0;

/*-----------------------------------------------------------------------------
	0x01a4 ctrl_bosd1_mixer_mux1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window0_alpha         : 8;	// 16:23
} CTRL_BOSD1_MIXER_MUX1;

/*-----------------------------------------------------------------------------
	0x01a8 ctrl_bosd1_mixer_mux2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_xpos          :12;	// 16:27
} CTRL_BOSD1_MIXER_MUX2;

/*-----------------------------------------------------------------------------
	0x01ac ctrl_bosd1_mixer_mux3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window0_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window0_w             :12;	// 16:27
} CTRL_BOSD1_MIXER_MUX3;

/*-----------------------------------------------------------------------------
	0x01b0 ctrl_bosd1_mixer_mux4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window1_alpha         : 8;	// 16:23
} CTRL_BOSD1_MIXER_MUX4;

/*-----------------------------------------------------------------------------
	0x01b4 ctrl_bosd1_mixer_mux5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_xpos          :12;	// 16:27
} CTRL_BOSD1_MIXER_MUX5;

/*-----------------------------------------------------------------------------
	0x01b8 ctrl_bosd1_mixer_mux6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window1_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window1_w             :12;	// 16:27
} CTRL_BOSD1_MIXER_MUX6;

/*-----------------------------------------------------------------------------
	0x01bc ctrl_bosd1_mixer_mux7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_en            : 1,	//     0
	                                :15,	//  1:15 reserved
	reg_mixer_window2_alpha         : 8;	// 16:23
} CTRL_BOSD1_MIXER_MUX7;

/*-----------------------------------------------------------------------------
	0x01c0 ctrl_bosd1_mixer_mux8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_ypos          :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_xpos          :12;	// 16:27
} CTRL_BOSD1_MIXER_MUX8;

/*-----------------------------------------------------------------------------
	0x01c4 ctrl_bosd1_mixer_mux9 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mixer_window2_h             :12,	//  0:11
	                                : 4,	// 12:15 reserved
	reg_mixer_window2_w             :12;	// 16:27
} CTRL_BOSD1_MIXER_MUX9;

/*-----------------------------------------------------------------------------
	0x01c8 ctrl_bosd1_mixer_muxa ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_w                     :13,	//  0:12
	reg_hole0_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole0_en                    : 1,	//    28
	reg_hole0_reverse_en            : 1;	//    29
} CTRL_BOSD1_MIXER_MUXA;

/*-----------------------------------------------------------------------------
	0x01cc ctrl_bosd1_mixer_muxb ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole0_h                     :12,	//  0:11
	reg_hole0_y                     :12,	// 12:23
	reg_hole0_a                     : 8;	// 24:31
} CTRL_BOSD1_MIXER_MUXB;

/*-----------------------------------------------------------------------------
	0x01d0 ctrl_bosd1_mixer_muxc ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_w                     :13,	//  0:12
	reg_hole1_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole1_en                    : 1,	//    28
	reg_hole1_reverse_en            : 1;	//    29
} CTRL_BOSD1_MIXER_MUXC;

/*-----------------------------------------------------------------------------
	0x01d4 ctrl_bosd1_mixer_muxd ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole1_h                     :12,	//  0:11
	reg_hole1_y                     :12,	// 12:23
	reg_hole1_a                     : 8;	// 24:31
} CTRL_BOSD1_MIXER_MUXD;

/*-----------------------------------------------------------------------------
	0x01d8 ctrl_bosd1_mixer_muxe ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_w                     :13,	//  0:12
	reg_hole2_x                     :13,	// 13:25
	                                : 2,	// 26:27 reserved
	reg_hole2_en                    : 1,	//    28
	reg_hole2_reverse_en            : 1;	//    29
} CTRL_BOSD1_MIXER_MUXE;

/*-----------------------------------------------------------------------------
	0x01dc ctrl_bosd1_mixer_muxf ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hole2_h                     :12,	//  0:11
	reg_hole2_y                     :12,	// 12:23
	reg_hole2_a                     : 8;	// 24:31
} CTRL_BOSD1_MIXER_MUXF;

/*-----------------------------------------------------------------------------
	0x0200 osd_ctrl_main ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_en                         : 1,	//     0
	                                : 3,	//  1: 3 reserved
	osd_hdr_src_sel                : 1,	//     4
	osd_unaligned_addr_en          : 1,	//     5
	osd_bitmap_only_mode_en        : 1,	//     6
	                                : 1,	//     7 reserved
	osd_v_reverse_en               : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd_s3d_en                     : 1,	//    12
	osd_s3d_v_reverse_en           : 1,	//    13
	                                : 2,	// 14:15 reserved
	osd_manual_line_inc            : 2,	// 16:17
	osd_manual_dlr                 : 2,	// 18:19
	osd_manual_line_inc_en         : 1,	//    20
	                                : 3,	// 21:23 reserved
	osd_yuyv_mode_en               : 1,	//    24
	                                : 3,	// 25:27 reserved
	osd_hdr_ready_flag_en          : 1,	//    28
	osd_hdr_ready_flag             : 1;	//    29
} OSDX_CTRL_MAIN;

/*-----------------------------------------------------------------------------
	0x0204 osd_base_addr ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_base_addr                  ;   	// 31: 0
} OSDX_BASE_ADDR;

/*-----------------------------------------------------------------------------
	0x0208 osd_ctrl_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_bbs                        : 1,	//     0
	osd_bqws                       : 1,	//     1
	osd_bhws                       : 1,	//     2
	                                : 1,	//     3 reserved
	osd_pbs                        : 1,	//     4
	osd_pqws                       : 1,	//     5
	osd_phws                       : 1,	//     6
	                                : 1,	//     7 reserved
	osd_hbs                        : 1,	//     8
	osd_hqws                       : 1,	//     9
	osd_hhws                       : 1;	//    10
} OSDX_CTRL_SWAP;

/*-----------------------------------------------------------------------------
	0x020c osd_ctrl_pdec ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_lfsr_seed                  :19,	//  0:18
	osd_pure_ck                    : 1,	//    19
	osd_round                      : 3,	// 20:22
	                                : 1,	//    23 reserved
	osd_ch_blue                    : 2,	// 24:25
	osd_ch_green                   : 2,	// 26:27
	osd_ch_red                     : 2,	// 28:29
	osd_ch_alpha                   : 2;	// 30:31
} OSDX_CTRL_PDEC;

/*-----------------------------------------------------------------------------
	0x0210 osd_ctrl_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_sc_bc_sa                   : 2,	//  0: 1
	osd_sc_force_vsize_en          : 1,	//     2
	osd_sc_bilinear                : 1,	//     3
	osd_sc_vdouble                 : 1,	//     4
	                                : 1,	//     5 reserved
	osd_sc_v_buf_en                : 1,	//     6
	                                : 1,	//     7 reserved
	osd_sc_sampling_mode           : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd_sc_bnd_mode                : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd_sc_h_phase_off             : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	osd_sc_v_phase_off             : 6,	// 24:29
	osd_odd_width_en               : 1,	//    30
	osd_fifo_en                    : 1;	//    31
} OSDX_CTRL_SCALER;

/*-----------------------------------------------------------------------------
	0x0214 osd_info_scaler ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_info_line_memory           :16,	//  0:15
	osd_info_line_memory_min       :16;	// 16:31
} OSDX_INFO_SCALER;

/*-----------------------------------------------------------------------------
	0x0218 osd_ctrl_path ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_ctrl_direc                 : 1,	//     0
	                                : 3,	//  1: 3 reserved
	osd_ctrl_bosd_pos              : 1,	//     4
	                                : 3,	//  5: 7 reserved
	osd_ctrl_eo2s                  : 1,	//     8
	                                : 3,	//  9:11 reserved
	osd_ctrl_eo2eeoo               : 1,	//    12
	                                : 3,	// 13:15 reserved
	osd_ctrl_path_width            :13;	// 16:28
} OSDX_CTRL_PATH;

/*-----------------------------------------------------------------------------
	0x0220 osd_ctrl_hdr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_hdr_ypos                   :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd_hdr_xpos                   :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd_hdr_color_key_en           : 1;	//    31
} OSDX_CTRL_HDR0;

/*-----------------------------------------------------------------------------
	0x0224 osd_ctrl_hdr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_hdr_h_mem                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd_hdr_w_mem                  :12;	// 16:27
} OSDX_CTRL_HDR1;

/*-----------------------------------------------------------------------------
	0x0228 osd_ctrl_hdr2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_hdr_h_out                  :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd_hdr_w_out                  :12,	// 16:27
	                                : 3,	// 28:30 reserved
	osd_hdr_pixel_order            : 1;	//    31
} OSDX_CTRL_HDR2;

/*-----------------------------------------------------------------------------
	0x022c osd_ctrl_hdr3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_hdr_wpl                    :16,	//  0:15
	osd_hdr_global_alpha           : 8,	// 16:23
	osd_hdr_format                 : 4,	// 24:27
	osd_hdr_depth                  : 3,	// 28:30
	osd_hdr_global_alpha_en        : 1;	//    31
} OSDX_CTRL_HDR3;

/*-----------------------------------------------------------------------------
	0x0230 osd_ctrl_hdr4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_hdr_color_key              ;   	// 31: 0
} OSDX_CTRL_HDR4;

/*-----------------------------------------------------------------------------
	0x0234 osd_ctrl_hdr5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_hdr_ptr_plte               ;   	// 31: 0
} OSDX_CTRL_HDR5;

/*-----------------------------------------------------------------------------
	0x0238 osd_ctrl_hdr6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_hdr_ptr_bmp                ;   	// 31: 0
} OSDX_CTRL_HDR6;

/*-----------------------------------------------------------------------------
	0x0240 osd_ctrl_sync0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_sync_enable                : 1,	//     0
	                                : 3,	//  1: 3 reserved
	osd_sync_hdouble_en            : 1,	//     4
	osd_sync_vdouble_en            : 1,	//     5
	osd_sync_v_pg_en               : 1,	//     6
	osd_sync_v_pg_sel              : 1,	//     7
	osd_sync_3dfp_en               : 1,	//     8
	                                : 7,	//  9:15 reserved
	osd_sync_v_reverse             : 1,	//    16
	                                : 3,	// 17:19 reserved
	osd_sync_tp_en                 : 1,	//    20
	osd_sync_tp_aa_en              : 1,	//    21
	                                : 2,	// 22:23 reserved
	osd_sync_tp_alpha              : 8;	// 24:31
} OSDX_CTRL_SYNC0;

/*-----------------------------------------------------------------------------
	0x0244 osd_ctrl_sync1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_sync_disp_h                :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd_sync_disp_w                :13;	// 16:28
} OSDX_CTRL_SYNC1;

/*-----------------------------------------------------------------------------
	0x0248 osd_ctrl_sync2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_r_3d_disparity_value       : 8,	//  0: 7
	osd_r_3d_disparity_sign        : 1,	//     8
	                                : 7,	//  9:15 reserved
	osd_l_3d_disparity_value       : 8,	// 16:23
	osd_l_3d_disparity_sign        : 1;	//    24
} OSDX_CTRL_SYNC2;

/*-----------------------------------------------------------------------------
	0x024c osd_ctrl_sync3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_ctrl_3d_blank              ;   	// 31: 0
} OSDX_CTRL_SYNC3;

/*-----------------------------------------------------------------------------
	0x0250 osd_ctrl_sync4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_sync_vact_video            :12,	//  0:11
	                                : 4,	// 12:15 reserved
	osd_sync_vact_space            :12;	// 16:27
} OSDX_CTRL_SYNC4;

/*-----------------------------------------------------------------------------
	0x0254 osd_ctrl_sync5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	osd_sync_reserved              ;   	// 31: 0
} OSDX_CTRL_SYNC5;

/*-----------------------------------------------------------------------------
	0x0254 osd0_ctrl_sync5 ''
------------------------------------------------------------------------------*/
typedef struct
{
	OSDX_CTRL_MAIN                  	osd_ctrl_main                  ;	// 0x0200 : ''
	OSDX_BASE_ADDR                  	osd_base_addr                  ;	// 0x0204 : ''
	OSDX_CTRL_SWAP                  	osd_ctrl_swap                  ;	// 0x0208 : ''
	OSDX_CTRL_PDEC                  	osd_ctrl_pdec                  ;	// 0x020c : ''
	OSDX_CTRL_SCALER                	osd_ctrl_scaler                ;	// 0x0210 : ''
	OSDX_INFO_SCALER                	osd_info_scaler                ;	// 0x0214 : ''
	OSDX_CTRL_PATH                  	osd_ctrl_path                  ;	// 0x0218 : ''
	UINT32                          	                 __rsvd_06[   1];	// 0x021c
	OSDX_CTRL_HDR0                  	osd_ctrl_hdr0                  ;	// 0x0220 : ''
	OSDX_CTRL_HDR1                  	osd_ctrl_hdr1                  ;	// 0x0224 : ''
	OSDX_CTRL_HDR2                  	osd_ctrl_hdr2                  ;	// 0x0228 : ''
	OSDX_CTRL_HDR3                  	osd_ctrl_hdr3                  ;	// 0x022c : ''
	OSDX_CTRL_HDR4                  	osd_ctrl_hdr4                  ;	// 0x0230 : ''
	OSDX_CTRL_HDR5                  	osd_ctrl_hdr5                  ;	// 0x0234 : ''
	OSDX_CTRL_HDR6                  	osd_ctrl_hdr6                  ;	// 0x0238 : ''
	UINT32                          	                 __rsvd_07[   1];	// 0x023c
	OSDX_CTRL_SYNC0                 	osd_ctrl_sync0                 ;	// 0x0240 : ''
	OSDX_CTRL_SYNC1                 	osd_ctrl_sync1                 ;	// 0x0244 : ''
	OSDX_CTRL_SYNC2                 	osd_ctrl_sync2                 ;	// 0x0248 : ''
	OSDX_CTRL_SYNC3                 	osd_ctrl_sync3                 ;	// 0x024c : ''
	OSDX_CTRL_SYNC4                 	osd_ctrl_sync4                 ;	// 0x0250 : ''
	OSDX_CTRL_SYNC5                 	osd_ctrl_sync5                 ;	// 0x0254 : ''ㅋ
	UINT32                          	                 __rsvd_08[  10];	// 0x0258 ~ 0x027c
}
OSDX_CTRL_T;

typedef struct
{
	CTRL_SOSD_MIXER_DISP_SIZE       	ctrl_sosd_mixer_disp_size       ;	// 0x0080 : ''
	CTRL_SOSD_MIXER_OUTPUT          	ctrl_sosd_mixer_output          ;	// 0x0084 : ''
	CTRL_SOSD_MIXER_AWB             	ctrl_sosd_mixer_awb             ;	// 0x0088 : ''
	CTRL_SOSD_MIXER_SYNC            	ctrl_sosd_mixer_sync            ;	// 0x008c : ''
	CTRL_SOSD_MIXER_ALPHA           	ctrl_sosd_mixer_alpha           ;	// 0x0090 : ''
	UINT32                          	                 __rsvd_01[   2];	// 0x0094 ~ 0x0098
	CTRL_SOSD_MIXER_HOLE            	ctrl_sosd_mixer_hole            ;	// 0x009c : ''
	CTRL_SOSD_MIXER_MUX0            	ctrl_sosd_mixer_mux0            ;	// 0x00a0 : ''
	CTRL_SOSD_MIXER_MUX1            	ctrl_sosd_mixer_mux1            ;	// 0x00a4 : ''
	CTRL_SOSD_MIXER_MUX2            	ctrl_sosd_mixer_mux2            ;	// 0x00a8 : ''
	CTRL_SOSD_MIXER_MUX3            	ctrl_sosd_mixer_mux3            ;	// 0x00ac : ''
	CTRL_SOSD_MIXER_MUX4            	ctrl_sosd_mixer_mux4            ;	// 0x00b0 : ''
	CTRL_SOSD_MIXER_MUX5            	ctrl_sosd_mixer_mux5            ;	// 0x00b4 : ''
	CTRL_SOSD_MIXER_MUX6            	ctrl_sosd_mixer_mux6            ;	// 0x00b8 : ''
	CTRL_SOSD_MIXER_MUX7            	ctrl_sosd_mixer_mux7            ;	// 0x00bc : ''
	CTRL_SOSD_MIXER_MUX8            	ctrl_sosd_mixer_mux8            ;	// 0x00c0 : ''
	CTRL_SOSD_MIXER_MUX9            	ctrl_sosd_mixer_mux9            ;	// 0x00c4 : ''
	CTRL_SOSD_MIXER_MUXA            	ctrl_sosd_mixer_muxa            ;	// 0x00c8 : ''
	CTRL_SOSD_MIXER_MUXB            	ctrl_sosd_mixer_muxb            ;	// 0x00cc : ''
	CTRL_SOSD_MIXER_MUXC            	ctrl_sosd_mixer_muxc            ;	// 0x00d0 : ''
	CTRL_SOSD_MIXER_MUXD            	ctrl_sosd_mixer_muxd            ;	// 0x00d4 : ''
	CTRL_SOSD_MIXER_MUXE            	ctrl_sosd_mixer_muxe            ;	// 0x00d8 : ''
	CTRL_SOSD_MIXER_MUXF            	ctrl_sosd_mixer_muxf            ;	// 0x00dc : ''
}
OSDX_SEP_OSD_MIXER;

typedef struct
{
	CTRL_BOSD0_MIXER_DISP_SIZE      	ctrl_bosd_mixer_disp_size      ;	// 0x0100 : ''
	CTRL_BOSD0_MIXER_OUTPUT         	ctrl_bosd_mixer_output         ;	// 0x0104 : ''
	CTRL_BOSD0_MIXER_AWB            	ctrl_bosd_mixer_awb            ;	// 0x0108 : ''
	CTRL_BOSD0_MIXER_SYNC           	ctrl_bosd_mixer_sync           ;	// 0x010c : ''
	UINT32                          	                 __rsvd_02[   3];	// 0x0110 ~ 0x0118
	CTRL_BOSD0_MIXER_HOLE           	ctrl_bosd_mixer_hole           ;	// 0x011c : ''
	CTRL_BOSD0_MIXER_MUX0           	ctrl_bosd_mixer_mux0           ;	// 0x0120 : ''
	CTRL_BOSD0_MIXER_MUX1           	ctrl_bosd_mixer_mux1           ;	// 0x0124 : ''
	CTRL_BOSD0_MIXER_MUX2           	ctrl_bosd_mixer_mux2           ;	// 0x0128 : ''
	CTRL_BOSD0_MIXER_MUX3           	ctrl_bosd_mixer_mux3           ;	// 0x012c : ''
	CTRL_BOSD0_MIXER_MUX4           	ctrl_bosd_mixer_mux4           ;	// 0x0130 : ''
	CTRL_BOSD0_MIXER_MUX5           	ctrl_bosd_mixer_mux5           ;	// 0x0134 : ''
	CTRL_BOSD0_MIXER_MUX6           	ctrl_bosd_mixer_mux6           ;	// 0x0138 : ''
	CTRL_BOSD0_MIXER_MUX7           	ctrl_bosd_mixer_mux7           ;	// 0x013c : ''
	CTRL_BOSD0_MIXER_MUX8           	ctrl_bosd_mixer_mux8           ;	// 0x0140 : ''
	CTRL_BOSD0_MIXER_MUX9           	ctrl_bosd_mixer_mux9           ;	// 0x0144 : ''
	CTRL_BOSD0_MIXER_MUXA           	ctrl_bosd_mixer_muxa           ;	// 0x0148 : ''
	CTRL_BOSD0_MIXER_MUXB           	ctrl_bosd_mixer_muxb           ;	// 0x014c : ''
	CTRL_BOSD0_MIXER_MUXC           	ctrl_bosd_mixer_muxc           ;	// 0x0150 : ''
	CTRL_BOSD0_MIXER_MUXD           	ctrl_bosd_mixer_muxd           ;	// 0x0154 : ''
	CTRL_BOSD0_MIXER_MUXE           	ctrl_bosd_mixer_muxe           ;	// 0x0158 : ''
	CTRL_BOSD0_MIXER_MUXF           	ctrl_bosd_mixer_muxf           ;	// 0x015c : ''
	UINT32                          	                 __rsvd_03[   8];	// 0x0160 ~ 0x017c
}
OSDX_BLEND_OSD_MIXER;

typedef struct {
	OSD_STATUS_MON_CFG              	osd_status_mon_cfg              ;	// 0x0000 : ''
	OSD_STATUS_MON_POS              	osd_status_mon_pos              ;	// 0x0004 : ''
	OSD_STATUS_VERSION              	osd_status_version              ;	// 0x0008 : ''
	OSD_STATUS_MIXER_LINE           	osd_status_mixer_line           ;	// 0x000c : ''
#ifdef FBDEV_USE_REG_GROUP
	OSDX_STATUS_MON_DATA				osd_status_mon_data[12];			// 0x0010 ~ 0x003c
#else
	OSD_STATUS_MON_DATA0            	osd_status_mon_data0            ;	// 0x0010 : ''
	OSD_STATUS_MON_DATA1            	osd_status_mon_data1            ;	// 0x0014 : ''
	OSD_STATUS_MON_DATA2            	osd_status_mon_data2            ;	// 0x0018 : ''
	OSD_STATUS_MON_DATA3            	osd_status_mon_data3            ;	// 0x001c : ''
	OSD_STATUS_MON_DATA4            	osd_status_mon_data4            ;	// 0x0020 : ''
	OSD_STATUS_MON_DATA5            	osd_status_mon_data5            ;	// 0x0024 : ''
	OSD_STATUS_MON_DATA6            	osd_status_mon_data6            ;	// 0x0028 : ''
	OSD_STATUS_MON_DATA7            	osd_status_mon_data7            ;	// 0x002c : ''
	OSD_STATUS_MON_DATA8            	osd_status_mon_data8            ;	// 0x0030 : ''
	OSD_STATUS_MON_DATA9            	osd_status_mon_data9            ;	// 0x0034 : ''
	OSD_STATUS_MON_DATAA            	osd_status_mon_dataa            ;	// 0x0038 : ''
	OSD_STATUS_MON_DATAB            	osd_status_mon_datab            ;	// 0x003c : ''
#endif
	OSD_COMMON_PLTE_SEL             	osd_common_plte_sel             ;	// 0x0040 : ''
	OSD_COMMON_CTRL_IRQ             	osd_common_ctrl_irq             ;	// 0x0044 : ''
	OSD_COMMON_CTRL_PIC_INIT        	osd_common_ctrl_pic_init        ;	// 0x0048 : ''
	OSD_COMMON_CTRL_PIC_TIMER       	osd_common_ctrl_pic_timer       ;	// 0x004c : ''
#ifdef FBDEV_USE_REG_GROUP
	OSDX_COMMON_DISP_SIZE				osdx_common_disp_size[8];
#else
	OSD0_COMMON_DISP_SIZE           	osd0_common_disp_size           ;	// 0x0050 : ''
	OSD1_COMMON_DISP_SIZE           	osd1_common_disp_size           ;	// 0x0054 : ''
	OSD2_COMMON_DISP_SIZE           	osd2_common_disp_size           ;	// 0x0058 : ''
	OSD3_COMMON_DISP_SIZE           	osd3_common_disp_size           ;	// 0x005c : ''
	OSD4_COMMON_DISP_SIZE           	osd4_common_disp_size           ;	// 0x0060 : ''
	OSD5_COMMON_DISP_SIZE           	osd5_common_disp_size           ;	// 0x0064 : ''
	OSD6_COMMON_DISP_SIZE           	osd6_common_disp_size           ;	// 0x0068 : ''
	OSD7_COMMON_DISP_SIZE           	osd7_common_disp_size           ;	// 0x006c : ''
#endif
	UINT32                          	                 __rsvd_00[   4];	// 0x0070 ~ 0x007c


	OSDX_SEP_OSD_MIXER					osdx_sep_osd_mixer[1];


	OSD_COMMON_SEP_OSD_CTRL         	osd_common_sep_osd_ctrl         ;	// 0x00e0 : ''
	OSD_COMMON_SEP_OSD_HDR0         	osd_common_sep_osd_hdr0         ;	// 0x00e4 : ''
	OSD_COMMON_SEP_OSD_HDR1         	osd_common_sep_osd_hdr1         ;	// 0x00e8 : ''
	OSD_COMMON_SEP_OSD_HDR2         	osd_common_sep_osd_hdr2         ;	// 0x00ec : ''
	OSD_COMMON_SEP_OSD_HDR3         	osd_common_sep_osd_hdr3         ;	// 0x00f0 : ''
	OSD_COMMON_SEP_OSD_HDR4         	osd_common_sep_osd_hdr4         ;	// 0x00f4 : ''
	OSD_COMMON_SEP_OSD_HDR5         	osd_common_sep_osd_hdr5         ;	// 0x00f8 : ''
	OSD_COMMON_SEP_OSD_HDR6         	osd_common_sep_osd_hdr6         ;	// 0x00fc : ''

	OSDX_BLEND_OSD_MIXER				osdx_blend_osd_mixer[2];

#ifdef FBDEV_USE_REG_GROUP
	OSDX_CTRL_T							osdx_ctrl[8];
#else
	....
#endif

#define OSDX_H15_Off(_g,_r)				((UINT32)&(g_osd_h15_reg_cached->_g._r) - (UINT32)(g_osd_h15_reg_cached))

#define OSDX_H15_RdFL(_g,_r)			((g_osd_h15_reg_cached->_g._r)=(g_osd_h15_reg->_g._r))
#define OSDX_H15_WrFL(_g,_r)			((g_osd_h15_reg->_g._r)=(g_osd_h15_reg_cached->_g._r))
#define OSDX_H15_Rd(_g,_r)				*((UINT32*)(&(g_osd_h15_reg_cached->_g._r)))
#define OSDX_H15_Wr(_g,_r,_v)			((OSDX_H15_Rd(_g,_r))=((UINT32)(_v)))
#define OSDX_H15_Rd00(_g,_r,_f)			(g_osd_h15_reg_cached->_g._r._f)
#define OSDX_H15_Rd01(_g,_r,_f01,_v01) 	do { 												\
											(_v01) = (g_osd_h15_reg_cached->_g._r._f01);	\
										} while(0)
#define OSDX_H15_Wr01(_g,_r,_f01,_v01)	do { 												\
											(g_osd_h15_reg_cached->_g._r._f01) = (_v01);	\
										} while(0)
#define OSDX_H15_Wr02(_g,_r,_f01,_v01,_f02,_v02)	do { 									\
											(g_osd_h15_reg_cached->_g._r._f01) = (_v01);	\
											(g_osd_h15_reg_cached->_g._r._f02) = (_v02);	\
										} while(0)

} OSD_H15_REG_T;

/* 120 regs, 120 types */

/* 120 regs, 120 types in Total*/

/*
 * @{
 * Naming for register pointer.
 * g_osd_h15_reg : real register of OSD.
 * g_osd_h15_reg_cached     : shadow register.
 *
 * @def OSD_H15_RdFL: Read  FLushing : Shadow <- Real.
 * @def OSD_H15_WrFL: Write FLushing : Shadow -> Real.
 * @def OSD_H15_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def OSD_H15_Wr  : Write whole register(UINT32) from Shadow register.
 * @def OSD_H15_Rd01 ~ OSD_H15_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def OSD_H15_Wr01 ~ OSD_H15_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define OSD_H15_RdFL(_r)			((g_osd_h15_reg_cached->_r)=(g_osd_h15_reg->_r))
#define OSD_H15_WrFL(_r)			((g_osd_h15_reg->_r)=(g_osd_h15_reg_cached->_r))

#define OSD_H15_Rd(_r)				*((UINT32*)(&(g_osd_h15_reg_cached->_r)))
#define OSD_H15_Wr(_r,_v)			((OSD_H15_Rd(_r))=((UINT32)(_v)))

#define OSD_H15_Rd00(_r,_f)         (g_osd_h15_reg_cached->_r._f)

#define OSD_H15_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
								} while(0)

#define OSD_H15_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
								} while(0)

#define OSD_H15_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
								} while(0)

#define OSD_H15_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
								} while(0)

#define OSD_H15_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
								} while(0)

#define OSD_H15_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
								} while(0)

#define OSD_H15_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
								} while(0)

#define OSD_H15_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
								} while(0)

#define OSD_H15_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
								} while(0)

#define OSD_H15_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_h15_reg_cached->_r._f10);				\
								} while(0)

#define OSD_H15_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_h15_reg_cached->_r._f11);				\
								} while(0)

#define OSD_H15_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_h15_reg_cached->_r._f12);				\
								} while(0)

#define OSD_H15_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_h15_reg_cached->_r._f13);				\
								} while(0)

#define OSD_H15_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_h15_reg_cached->_r._f13);				\
									(_v14) = (g_osd_h15_reg_cached->_r._f14);				\
								} while(0)

#define OSD_H15_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_h15_reg_cached->_r._f13);				\
									(_v14) = (g_osd_h15_reg_cached->_r._f14);				\
									(_v15) = (g_osd_h15_reg_cached->_r._f15);				\
								} while(0)

#define OSD_H15_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (g_osd_h15_reg_cached->_r._f01);				\
									(_v02) = (g_osd_h15_reg_cached->_r._f02);				\
									(_v03) = (g_osd_h15_reg_cached->_r._f03);				\
									(_v04) = (g_osd_h15_reg_cached->_r._f04);				\
									(_v05) = (g_osd_h15_reg_cached->_r._f05);				\
									(_v06) = (g_osd_h15_reg_cached->_r._f06);				\
									(_v07) = (g_osd_h15_reg_cached->_r._f07);				\
									(_v08) = (g_osd_h15_reg_cached->_r._f08);				\
									(_v09) = (g_osd_h15_reg_cached->_r._f09);				\
									(_v10) = (g_osd_h15_reg_cached->_r._f10);				\
									(_v11) = (g_osd_h15_reg_cached->_r._f11);				\
									(_v12) = (g_osd_h15_reg_cached->_r._f12);				\
									(_v13) = (g_osd_h15_reg_cached->_r._f13);				\
									(_v14) = (g_osd_h15_reg_cached->_r._f14);				\
									(_v15) = (g_osd_h15_reg_cached->_r._f15);				\
									(_v16) = (g_osd_h15_reg_cached->_r._f16);				\
								} while(0)


#define OSD_H15_Wr01(_r,_f01,_v01)													\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
								} while(0)

#define OSD_H15_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
								} while(0)

#define OSD_H15_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
								} while(0)

#define OSD_H15_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
								} while(0)

#define OSD_H15_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
								} while(0)

#define OSD_H15_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
								} while(0)

#define OSD_H15_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
								} while(0)

#define OSD_H15_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
								} while(0)

#define OSD_H15_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
								} while(0)

#define OSD_H15_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_h15_reg_cached->_r._f10) = (_v10);				\
								} while(0)

#define OSD_H15_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_h15_reg_cached->_r._f11) = (_v11);				\
								} while(0)

#define OSD_H15_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_h15_reg_cached->_r._f12) = (_v12);				\
								} while(0)

#define OSD_H15_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_h15_reg_cached->_r._f13) = (_v13);				\
								} while(0)

#define OSD_H15_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_h15_reg_cached->_r._f13) = (_v13);				\
									(g_osd_h15_reg_cached->_r._f14) = (_v14);				\
								} while(0)

#define OSD_H15_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_h15_reg_cached->_r._f13) = (_v13);				\
									(g_osd_h15_reg_cached->_r._f14) = (_v14);				\
									(g_osd_h15_reg_cached->_r._f15) = (_v15);				\
								} while(0)

#define OSD_H15_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(g_osd_h15_reg_cached->_r._f01) = (_v01);				\
									(g_osd_h15_reg_cached->_r._f02) = (_v02);				\
									(g_osd_h15_reg_cached->_r._f03) = (_v03);				\
									(g_osd_h15_reg_cached->_r._f04) = (_v04);				\
									(g_osd_h15_reg_cached->_r._f05) = (_v05);				\
									(g_osd_h15_reg_cached->_r._f06) = (_v06);				\
									(g_osd_h15_reg_cached->_r._f07) = (_v07);				\
									(g_osd_h15_reg_cached->_r._f08) = (_v08);				\
									(g_osd_h15_reg_cached->_r._f09) = (_v09);				\
									(g_osd_h15_reg_cached->_r._f10) = (_v10);				\
									(g_osd_h15_reg_cached->_r._f11) = (_v11);				\
									(g_osd_h15_reg_cached->_r._f12) = (_v12);				\
									(g_osd_h15_reg_cached->_r._f13) = (_v13);				\
									(g_osd_h15_reg_cached->_r._f14) = (_v14);				\
									(g_osd_h15_reg_cached->_r._f15) = (_v15);				\
									(g_osd_h15_reg_cached->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after OSD_H15_Wind(), 1 for OSD_H15_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * OSD_H15_Rind : General indexed register Read.(
 * OSD_H15_Wind : General indexed register Read.
 *
 * OSD_H15_Ridx : For 'index', 'rw', 'load' field name
 * OSD_H15_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define OSD_H15_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								OSD_H15_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								OSD_H15_WrFL(_r);									\
								OSD_H15_RdFL(_r);									\
								OSD_H15_Rd01(_r,_fname,_fval);						\
							} while (0)

#define OSD_H15_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				OSD_H15_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define OSD_H15_Ridx(_r, _ival, _fname, _fval)	OSD_H15_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define OSD_H15_Widx(_r, _ival, _fname, _fval)	OSD_H15_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _#MOD#_REG_H_ */

/* from 'H15A0_RegFile_DE_OSD.csv'  대한민국 표준시 by getregs v2.8 */

