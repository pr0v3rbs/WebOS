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

#ifndef _REG_OIF_H14A0_h
#define _REG_OIF_H14A0_h

/*-----------------------------------------------------------------------------
	0x5c00 out_video_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 de1_out_sel                 : 1;   //     0
	UINT32 de2_out_sel                 : 1;   //     1
} H14A0_OUT_VIDEO_INFO_T;

/*-----------------------------------------------------------------------------
	0x5c04 data_color_from ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 b_data_from                 : 2;   //  0: 1
	UINT32 g_data_from                 : 2;   //  2: 3
	UINT32 r_data_from                 : 2;   //  4: 5
} H14A0_DATA_COLOR_FROM_T;

/*-----------------------------------------------------------------------------
	0x5c08 data_color ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 data_color_b                : 10;  //  0: 9
	UINT32 data_color_g                : 10;  // 10:19
	UINT32 data_color_r                : 10;  // 20:29
} H14A0_DATA_COLOR_T;

/*-----------------------------------------------------------------------------
	0x5c0c sync_color ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sync_color_b                : 10;  //  0: 9
	UINT32 sync_color_g                : 10;  // 10:19
	UINT32 sync_color_r                : 10;  // 20:29
} H14A0_SYNC_COLOR_T;

/*-----------------------------------------------------------------------------
	0x5c10 delay_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 b_delay_l                   : 3;   //  0: 2
	UINT32 g_delay_l                   : 3;   //  3: 5
	UINT32 r_delay_l                   : 3;   //  6: 8
	UINT32 de_delay_l                  : 4;   //  9:12
	UINT32                             : 3;   // 13:15 reserved
	UINT32 b_delay_r                   : 3;   // 16:18
	UINT32 g_delay_r                   : 3;   // 19:21
	UINT32 r_delay_r                   : 3;   // 22:24
	UINT32 de_delay_r                  : 4;   // 25:28
} H14A0_DELAY_DATA_T;

/*-----------------------------------------------------------------------------
	0x5c14 sync_de_control ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 de_pol                      : 1;   //     0
	UINT32 de_disable                  : 1;   //     1
	UINT32 hsync_pol                   : 1;   //     2
	UINT32 hsync_disable               : 1;   //     3
	UINT32 vsync_pol                   : 1;   //     4
	UINT32 vsync_disable               : 1;   //     5
	UINT32 alpha_sel_76                : 2;   //  6: 7
	UINT32 alpha_sel_54                : 2;   //  8: 9
	UINT32 alpha_sel_32                : 2;   // 10:11
	UINT32 alpha_sel_10                : 2;   // 12:13
} H14A0_SYNC_DE_CONTROL_T;

/*-----------------------------------------------------------------------------
	0x5c20 lvds_if_control ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 lvds_empty_data_ch1         : 2;   //  0: 1
	UINT32 lvds_empty_data_ch2         : 2;   //  2: 3
	UINT32 lvds_8bit_mode              : 1;   //     4
	UINT32 lvds_lsb_first              : 1;   //     5
	UINT32 lvds_sync_order             : 3;   //  6: 8
	UINT32 lvds_ch1_from               : 2;   //  9:10
	UINT32 lvds_ch2_from               : 2;   // 11:12
	UINT32 dual_out_mode               : 1;   //    13
	UINT32 even_pix_sel                : 1;   //    14
	UINT32 lvds_d_lane_empty_data_ch1  : 3;   // 15:17
	UINT32 lvds_e_lane_empty_data_ch1  : 3;   // 18:20
	UINT32 lvds_d_lane_empty_data_ch2  : 3;   // 21:23
	UINT32 lvds_e_lane_empty_data_ch2  : 3;   // 24:26
} H14A0_LVDS_IF_CONTROL_T;

/*-----------------------------------------------------------------------------
	0x5c24 lvds_port_swap ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 lvds_a_lane_sel_ch1         : 3;   //  0: 2
	UINT32 lvds_b_lane_sel_ch1         : 3;   //  3: 5
	UINT32 lvds_c_lane_sel_ch1         : 3;   //  6: 8
	UINT32 lvds_d_lane_sel_ch1         : 3;   //  9:11
	UINT32 lvds_e_lane_sel_ch1         : 3;   // 12:14
	UINT32                             : 1;   //    15 reserved
	UINT32 lvds_a_lane_sel_ch2         : 3;   // 16:18
	UINT32 lvds_b_lane_sel_ch2         : 3;   // 19:21
	UINT32 lvds_c_lane_sel_ch2         : 3;   // 22:24
	UINT32 lvds_d_lane_sel_ch2         : 3;   // 25:27
	UINT32 lvds_e_lane_sel_ch2         : 3;   // 28:30
} H14A0_LVDS_PORT_SWAP_T;

/*-----------------------------------------------------------------------------
	0x5c38 lvds_force_data_en ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                             : 2;   //  0: 1 reserved
	UINT32 de1_hsync_en                : 1;   //     2
	UINT32 de1_vsyn_en                 : 1;   //     3
	UINT32                             : 2;   //  4: 5 reserved
	UINT32 de2_hsync_en                : 1;   //     6
	UINT32 de2_vsync_en                : 1;   //     7
} H14A0_LVDS_FORCE_DATA_EN_T;

/*-----------------------------------------------------------------------------
	0x5c3c lvds_force_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 de1_empty_data0             : 1;   //     0
	UINT32 de1_empty_data1             : 1;   //     1
	UINT32 de1_hsync                   : 1;   //     2
	UINT32 de1_vsyn                    : 1;   //     3
	UINT32 de2_empty_data0             : 1;   //     4
	UINT32 de2_empty_data1             : 1;   //     5
	UINT32 de2_hsync                   : 1;   //     6
	UINT32 de2_vsync                   : 1;   //     7
} H14A0_LVDS_FORCE_DATA_T;

typedef struct {
	H14A0_OUT_VIDEO_INFO_T       out_video_info;              // 0x5c00
	H14A0_DATA_COLOR_FROM_T      data_color_from;             // 0x5c04
	H14A0_DATA_COLOR_T           data_color;                  // 0x5c08
	H14A0_SYNC_COLOR_T           sync_color;                  // 0x5c0c
	H14A0_DELAY_DATA_T           delay_data;                  // 0x5c10
	H14A0_SYNC_DE_CONTROL_T      sync_de_control;             // 0x5c14
	UINT32                      reserved0[2];                // 0x5c18~0x5c1c
	H14A0_LVDS_IF_CONTROL_T      lvds_if_control;             // 0x5c20
	H14A0_LVDS_PORT_SWAP_T       lvds_port_swap;              // 0x5c24
	UINT32                      reserved1[4];                // 0x5c28~0x5c34
	H14A0_LVDS_FORCE_DATA_EN_T   lvds_force_data_en;          // 0x5c38
	H14A0_LVDS_FORCE_DATA_T      lvds_force_data;             // 0x5c3c
} DE_OIF_REG_H14A0_T;

#endif
