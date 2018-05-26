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

#ifndef _REG_D3P_M14A0_h
#define _REG_D3P_M14A0_h

/*-----------------------------------------------------------------------------
	0x1300 debd_pp_top_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time           : 1;   //     0
	UINT32 load_enable         : 1;   //     1
	UINT32 load_type           : 1;   //     2
} M14A0_DEBD_PP_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1304 debd_pp_top_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_bypass    : 1;   //     0
	UINT32 debd_pp_l_bypass    : 1;   //     1
	UINT32                     : 3;   //  2: 4 reserved
	UINT32 async_delay_mode    : 1;   //     5
	UINT32                     : 2;   //  6: 7 reserved
	UINT32 vdpulse_pos         : 12;  //  8:19
} M14A0_DEBD_PP_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1308 debd_pp_top_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 va_debd_pp_in       : 1;   //     0
	UINT32 va_debd_pp_out      : 1;   //     1
	UINT32                     : 2;   //  2: 3 reserved
	UINT32 stat_ocsc_r         : 1;   //     4
	UINT32 stat_ocsc_l         : 1;   //     5
	UINT32 stat_lrcr           : 1;   //     6
	UINT32 stat_icsc_r         : 1;   //     7
	UINT32 stat_icsc_l         : 1;   //     8
	UINT32 stat_vspyc_r        : 1;   //     9
	UINT32 stat_vspyc_l        : 1;   //    10
	UINT32 stat_lvctrl_r       : 1;   //    11
	UINT32 stat_lvctrl_l       : 1;   //    12
	UINT32                     : 3;   // 13:15 reserved
	UINT32 vcnt                : 12;  // 16:27
} M14A0_DEBD_PP_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x1310 debd_pp_apl_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x0     : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 apl_win_ctrl_y0     : 12;  // 16:27
} M14A0_DEBD_PP_APL_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1314 debd_pp_apl_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x1     : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 apl_win_ctrl_y1     : 12;  // 16:27
} M14A0_DEBD_PP_APL_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1318 debd_pp_apl_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                     : 24;  //  0:23 reserved
	UINT32 apl_position_sel    : 2;   // 24:25
	UINT32 apl_color_sel       : 2;   // 26:27
	UINT32 act_channel_sel     : 1;   //    28
} M14A0_DEBD_PP_APL_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x131c debd_pp_apl_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_y               : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 apl_y_otherside     : 10;  // 16:25
} M14A0_DEBD_PP_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x1320 debd_pp_l_level_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable              : 1;   //     0
	UINT32                     : 3;   //  1: 3 reserved
	UINT32 y_offset            : 10;  //  4:13
	UINT32                     : 2;   // 14:15 reserved
	UINT32 center_position     : 8;   // 16:23
	UINT32 y_gain              : 8;   // 24:31
} M14A0_DEBD_PP_L_LEVEL_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1324 debd_pp_r_level_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable              : 1;   //     0
	UINT32                     : 3;   //  1: 3 reserved
	UINT32 y_offset            : 10;  //  4:13
	UINT32                     : 2;   // 14:15 reserved
	UINT32 center_position     : 8;   // 16:23
	UINT32 y_gain              : 8;   // 24:31
} M14A0_DEBD_PP_R_LEVEL_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1330 debd_pp_l_vspyc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable              : 1;   //     0
	UINT32 graycolor_enable    : 1;   //     1
	UINT32 color_only_enable   : 1;   //     2
	UINT32 cb_info             : 1;   //     3
	UINT32 cr_info             : 1;   //     4
	UINT32                     : 3;   //  5: 7 reserved
	UINT32 center_position     : 8;   //  8:15
	UINT32 contrast            : 10;  // 16:25
} M14A0_DEBD_PP_L_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1334 debd_pp_l_vspyc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                     : 8;   //  0: 7 reserved
	UINT32 saturation          : 8;   //  8:15
	UINT32 brightness          : 10;  // 16:25
} M14A0_DEBD_PP_L_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1338 debd_pp_r_vspyc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable              : 1;   //     0
	UINT32 graycolor_enable    : 1;   //     1
	UINT32 color_only_enable   : 1;   //     2
	UINT32 cb_info             : 1;   //     3
	UINT32 cr_info             : 1;   //     4
	UINT32                     : 3;   //  5: 7 reserved
	UINT32 center_position     : 8;   //  8:15
	UINT32 contrast            : 10;  // 16:25
} M14A0_DEBD_PP_R_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x133c debd_pp_r_vspyc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                     : 8;   //  0: 7 reserved
	UINT32 saturation          : 8;   //  8:15
	UINT32 brightness          : 10;  // 16:25
} M14A0_DEBD_PP_R_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1340 debd_pp_l_csc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef0         : 15;  // 16:30
} M14A0_DEBD_PP_L_CSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1344 debd_pp_l_csc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef2         : 15;  // 16:30
} M14A0_DEBD_PP_L_CSC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1348 debd_pp_l_csc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef4         : 15;  // 16:30
} M14A0_DEBD_PP_L_CSC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x134c debd_pp_l_csc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef6         : 15;  // 16:30
} M14A0_DEBD_PP_L_CSC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x1350 debd_pp_l_csc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_coef8         : 15;  // 16:30
} M14A0_DEBD_PP_L_CSC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x1354 debd_pp_l_csc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst1         : 11;  // 16:26
} M14A0_DEBD_PP_L_CSC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x1358 debd_pp_l_csc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst3         : 11;  // 16:26
} M14A0_DEBD_PP_L_CSC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x135c debd_pp_l_csc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 icsc_enable         : 1;   //     0
	UINT32 ocsc_enable         : 1;   //     1
	UINT32                     : 14;  //  2:15 reserved
	UINT32 r_csc_ofst5         : 11;  // 16:26
} M14A0_DEBD_PP_L_CSC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x1360 debd_pp_r_csc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef0         : 15;  // 16:30
} M14A0_DEBD_PP_R_CSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1364 debd_pp_r_csc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef2         : 15;  // 16:30
} M14A0_DEBD_PP_R_CSC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1368 debd_pp_r_csc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef4         : 15;  // 16:30
} M14A0_DEBD_PP_R_CSC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x136c debd_pp_r_csc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef6         : 15;  // 16:30
} M14A0_DEBD_PP_R_CSC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x1370 debd_pp_r_csc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_coef8         : 15;  // 16:30
} M14A0_DEBD_PP_R_CSC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x1374 debd_pp_r_csc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst1         : 11;  // 16:26
} M14A0_DEBD_PP_R_CSC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x1378 debd_pp_r_csc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst3         : 11;  // 16:26
} M14A0_DEBD_PP_R_CSC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x137c debd_pp_r_csc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 icsc_enable         : 1;   //     0
	UINT32 ocsc_enable         : 1;   //     1
	UINT32                     : 14;  //  2:15 reserved
	UINT32 r_csc_ofst5         : 11;  // 16:26
} M14A0_DEBD_PP_R_CSC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x1380 debd_pp_lrcr_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable              : 1;   //     0
	UINT32 adjust_en           : 1;   //     1
	UINT32 protection_en       : 1;   //     2
	UINT32 adjust_en2          : 1;   //     3
	UINT32 r_hist_data_sel     : 1;   //     4
	UINT32 l_hist_data_sel     : 1;   //     5
	UINT32 r_proc_data_sel     : 1;   //     6
	UINT32 l_proc_data_sel     : 1;   //     7
	UINT32 ff_mode_en          : 1;   //     8
	UINT32 ff_mode_parity      : 1;   //     9
	UINT32                     : 2;   // 10:11 reserved
	UINT32 r_out_data_sel      : 1;   //    12
	UINT32 l_out_data_sel      : 1;   //    13
	UINT32                     : 2;   // 14:15 reserved
	UINT32 lrcr_gain           : 4;   // 16:19
	UINT32 hist_rd_en          : 1;   //    20
	UINT32                     : 1;   //    21 reserved
	UINT32 hist_mode           : 2;   // 22:23
} M14A0_DEBD_PP_LRCR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1384 debd_pp_lrcr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 th_max_hist         : 8;   //  0: 7
	UINT32 th_valid_bins       : 8;   //  8:15
	UINT32 th1                 : 8;   // 16:23
	UINT32 th0                 : 8;   // 24:31
} M14A0_DEBD_PP_LRCR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1388 debd_pp_lrcr_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                  : 12;  //  0:11
	UINT32                     : 2;   // 12:13 reserved
	UINT32 hist_l_window_h_bbd : 1;   //    14
	UINT32 hist_l_window_h_en  : 1;   //    15
	UINT32 y0                  : 12;  // 16:27
	UINT32                     : 2;   // 28:29 reserved
	UINT32 hist_l_window_v_bbd : 1;   //    30
	UINT32 hist_l_window_v_en  : 1;   //    31
} M14A0_DEBD_PP_LRCR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x138c debd_pp_lrcr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} M14A0_DEBD_PP_LRCR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x1390 debd_pp_lrcr_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                  : 12;  //  0:11
	UINT32                     : 2;   // 12:13 reserved
	UINT32 hist_r_window_h_bbd : 1;   //    14
	UINT32 hist_r_window_h_en  : 1;   //    15
	UINT32 y0                  : 12;  // 16:27
	UINT32                     : 2;   // 28:29 reserved
	UINT32 hist_r_window_v_bbd : 1;   //    30
	UINT32 hist_r_window_v_en  : 1;   //    31
} M14A0_DEBD_PP_LRCR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x1394 debd_pp_lrcr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} M14A0_DEBD_PP_LRCR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x1398 debd_pp_lrcr_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                  : 12;  //  0:11
	UINT32                     : 2;   // 12:13 reserved
	UINT32 active_window_h_bbd : 1;   //    14
	UINT32 active_window_h_en  : 1;   //    15
	UINT32 y0                  : 12;  // 16:27
	UINT32                     : 2;   // 28:29 reserved
	UINT32 active_window_v_bbd : 1;   //    30
	UINT32 active_window_v_en  : 1;   //    31
} M14A0_DEBD_PP_LRCR_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x139c debd_pp_lrcr_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} M14A0_DEBD_PP_LRCR_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x13a0 debd_pp_lrcr_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist            : 8;   //  0: 7
	UINT32 n_valid_bins        : 8;   //  8:15
	UINT32 cdf_x1              : 8;   // 16:23
	UINT32 cdf_x0              : 8;   // 24:31
} M14A0_DEBD_PP_LRCR_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x13a4 debd_pp_lrcr_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist            : 8;   //  0: 7
	UINT32 n_valid_bins        : 8;   //  8:15
	UINT32 cdf_x1              : 8;   // 16:23
	UINT32 cdf_x0              : 8;   // 24:31
} M14A0_DEBD_PP_LRCR_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x13a8 debd_pp_lrcr_stat_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist            : 8;   //  0: 7
	UINT32 n_valid_bins        : 8;   //  8:15
	UINT32 cdf_x1              : 8;   // 16:23
	UINT32 cdf_x0              : 8;   // 24:31
} M14A0_DEBD_PP_LRCR_STAT_02_T;

/*-----------------------------------------------------------------------------
	0x13b0 debd_pp_l_g_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_g_available : 1;   //    28
} M14A0_DEBD_PP_L_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x13b4 debd_pp_l_g_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_l_g_hist_ia_data_f: 32;   // 31: 0
} M14A0_DEBD_PP_L_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x13b8 debd_pp_l_b_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_b_available : 1;   //    28
} M14A0_DEBD_PP_L_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x13bc debd_pp_l_b_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_l_b_hist_ia_data_f: 32;   // 31: 0
} M14A0_DEBD_PP_L_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x13c0 debd_pp_l_r_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_r_available : 1;   //    28
} M14A0_DEBD_PP_L_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x13c4 debd_pp_l_r_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_l_r_hist_ia_data_f: 32;   // 31: 0
} M14A0_DEBD_PP_L_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x13c8 debd_pp_r_g_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_g_available : 1;   //    28
} M14A0_DEBD_PP_R_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x13cc debd_pp_r_g_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_g_hist_ia_data_f: 32;   // 31: 0
} M14A0_DEBD_PP_R_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x13d0 debd_pp_r_b_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_b_available : 1;   //    28
} M14A0_DEBD_PP_R_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x13d4 debd_pp_r_b_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_b_hist_ia_data_f: 32;   // 31: 0
} M14A0_DEBD_PP_R_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x13d8 debd_pp_r_r_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_r_available : 1;   //    28
	UINT32                     : 2;   // 29:30 reserved
	UINT32 hist_rd_done        : 1;   //    31
} M14A0_DEBD_PP_R_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x13dc debd_pp_r_r_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_r_hist_ia_data_f: 32;   // 31: 0
} M14A0_DEBD_PP_R_R_HIST_IA_DATA_T;

typedef struct {
	M14A0_DEBD_PP_TOP_CTRL_00_T         debd_pp_top_ctrl_00;                 // 0x1300
	M14A0_DEBD_PP_TOP_CTRL_01_T         debd_pp_top_ctrl_01;                 // 0x1304
	M14A0_DEBD_PP_TOP_CTRL_02_T         debd_pp_top_ctrl_02;                 // 0x1308
	UINT32                              reserved0;                           // 0x130c
	M14A0_DEBD_PP_APL_CTRL_00_T         debd_pp_apl_ctrl_00;                 // 0x1310
	M14A0_DEBD_PP_APL_CTRL_01_T         debd_pp_apl_ctrl_01;                 // 0x1314
	M14A0_DEBD_PP_APL_CTRL_02_T         debd_pp_apl_ctrl_02;                 // 0x1318
	M14A0_DEBD_PP_APL_STAT_00_T         debd_pp_apl_stat_00;                 // 0x131c
	M14A0_DEBD_PP_L_LEVEL_CTRL_00_T     debd_pp_l_level_ctrl_00;             // 0x1320
	M14A0_DEBD_PP_R_LEVEL_CTRL_00_T     debd_pp_r_level_ctrl_00;             // 0x1324
	UINT32                              reserved1[2];                        // 0x1328~0x132c
	M14A0_DEBD_PP_L_VSPYC_CTRL_00_T     debd_pp_l_vspyc_ctrl_00;             // 0x1330
	M14A0_DEBD_PP_L_VSPYC_CTRL_01_T     debd_pp_l_vspyc_ctrl_01;             // 0x1334
	M14A0_DEBD_PP_R_VSPYC_CTRL_00_T     debd_pp_r_vspyc_ctrl_00;             // 0x1338
	M14A0_DEBD_PP_R_VSPYC_CTRL_01_T     debd_pp_r_vspyc_ctrl_01;             // 0x133c
	M14A0_DEBD_PP_L_CSC_CTRL_00_T       debd_pp_l_csc_ctrl_00;               // 0x1340
	M14A0_DEBD_PP_L_CSC_CTRL_01_T       debd_pp_l_csc_ctrl_01;               // 0x1344
	M14A0_DEBD_PP_L_CSC_CTRL_02_T       debd_pp_l_csc_ctrl_02;               // 0x1348
	M14A0_DEBD_PP_L_CSC_CTRL_03_T       debd_pp_l_csc_ctrl_03;               // 0x134c
	M14A0_DEBD_PP_L_CSC_CTRL_04_T       debd_pp_l_csc_ctrl_04;               // 0x1350
	M14A0_DEBD_PP_L_CSC_CTRL_05_T       debd_pp_l_csc_ctrl_05;               // 0x1354
	M14A0_DEBD_PP_L_CSC_CTRL_06_T       debd_pp_l_csc_ctrl_06;               // 0x1358
	M14A0_DEBD_PP_L_CSC_CTRL_07_T       debd_pp_l_csc_ctrl_07;               // 0x135c
	M14A0_DEBD_PP_R_CSC_CTRL_00_T       debd_pp_r_csc_ctrl_00;               // 0x1360
	M14A0_DEBD_PP_R_CSC_CTRL_01_T       debd_pp_r_csc_ctrl_01;               // 0x1364
	M14A0_DEBD_PP_R_CSC_CTRL_02_T       debd_pp_r_csc_ctrl_02;               // 0x1368
	M14A0_DEBD_PP_R_CSC_CTRL_03_T       debd_pp_r_csc_ctrl_03;               // 0x136c
	M14A0_DEBD_PP_R_CSC_CTRL_04_T       debd_pp_r_csc_ctrl_04;               // 0x1370
	M14A0_DEBD_PP_R_CSC_CTRL_05_T       debd_pp_r_csc_ctrl_05;               // 0x1374
	M14A0_DEBD_PP_R_CSC_CTRL_06_T       debd_pp_r_csc_ctrl_06;               // 0x1378
	M14A0_DEBD_PP_R_CSC_CTRL_07_T       debd_pp_r_csc_ctrl_07;               // 0x137c
	M14A0_DEBD_PP_LRCR_CTRL_00_T        debd_pp_lrcr_ctrl_00;                // 0x1380
	M14A0_DEBD_PP_LRCR_CTRL_01_T        debd_pp_lrcr_ctrl_01;                // 0x1384
	M14A0_DEBD_PP_LRCR_CTRL_02_T        debd_pp_lrcr_ctrl_02;                // 0x1388
	M14A0_DEBD_PP_LRCR_CTRL_03_T        debd_pp_lrcr_ctrl_03;                // 0x138c
	M14A0_DEBD_PP_LRCR_CTRL_04_T        debd_pp_lrcr_ctrl_04;                // 0x1390
	M14A0_DEBD_PP_LRCR_CTRL_05_T        debd_pp_lrcr_ctrl_05;                // 0x1394
	M14A0_DEBD_PP_LRCR_CTRL_06_T        debd_pp_lrcr_ctrl_06;                // 0x1398
	M14A0_DEBD_PP_LRCR_CTRL_07_T        debd_pp_lrcr_ctrl_07;                // 0x139c
	M14A0_DEBD_PP_LRCR_STAT_00_T        debd_pp_lrcr_stat_00;                // 0x13a0
	M14A0_DEBD_PP_LRCR_STAT_01_T        debd_pp_lrcr_stat_01;                // 0x13a4
	M14A0_DEBD_PP_LRCR_STAT_02_T        debd_pp_lrcr_stat_02;                // 0x13a8
	UINT32                              reserved2;                           // 0x13ac
	M14A0_DEBD_PP_L_G_HIST_IA_CTRL_T    debd_pp_l_g_hist_ia_ctrl;            // 0x13b0
	M14A0_DEBD_PP_L_G_HIST_IA_DATA_T    debd_pp_l_g_hist_ia_data;            // 0x13b4
	M14A0_DEBD_PP_L_B_HIST_IA_CTRL_T    debd_pp_l_b_hist_ia_ctrl;            // 0x13b8
	M14A0_DEBD_PP_L_B_HIST_IA_DATA_T    debd_pp_l_b_hist_ia_data;            // 0x13bc
	M14A0_DEBD_PP_L_R_HIST_IA_CTRL_T    debd_pp_l_r_hist_ia_ctrl;            // 0x13c0
	M14A0_DEBD_PP_L_R_HIST_IA_DATA_T    debd_pp_l_r_hist_ia_data;            // 0x13c4
	M14A0_DEBD_PP_R_G_HIST_IA_CTRL_T    debd_pp_r_g_hist_ia_ctrl;            // 0x13c8
	M14A0_DEBD_PP_R_G_HIST_IA_DATA_T    debd_pp_r_g_hist_ia_data;            // 0x13cc
	M14A0_DEBD_PP_R_B_HIST_IA_CTRL_T    debd_pp_r_b_hist_ia_ctrl;            // 0x13d0
	M14A0_DEBD_PP_R_B_HIST_IA_DATA_T    debd_pp_r_b_hist_ia_data;            // 0x13d4
	M14A0_DEBD_PP_R_R_HIST_IA_CTRL_T    debd_pp_r_r_hist_ia_ctrl;            // 0x13d8
	M14A0_DEBD_PP_R_R_HIST_IA_DATA_T    debd_pp_r_r_hist_ia_data;            // 0x13dc
} DE_P1L_REG_M14A0_T;

#endif
