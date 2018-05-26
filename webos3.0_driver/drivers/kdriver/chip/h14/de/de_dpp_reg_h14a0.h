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

#ifndef _REG_DPP_H14A0_h
#define _REG_DPP_H14A0_h

/*-----------------------------------------------------------------------------
	0x1800 debd_pp_top_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time           : 1;   //     0
	UINT32 load_enable         : 1;   //     1
	UINT32 load_type           : 1;   //     2
} H14A0_DEBD_PP_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1804 debd_pp_top_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_bypass    : 1;   //     0
	UINT32 debd_pp_l_bypass    : 1;   //     1
	UINT32                     : 3;   //  2: 4 reserved
	UINT32 async_delay_mode    : 1;   //     5
	UINT32                     : 2;   //  6: 7 reserved
	UINT32 vdpulse_pos         : 12;  //  8:19
} H14A0_DEBD_PP_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1808 debd_pp_top_ctrl_02 ''
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
} H14A0_DEBD_PP_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x1810 debd_pp_apl_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x0     : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 apl_win_ctrl_y0     : 12;  // 16:27
} H14A0_DEBD_PP_APL_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1814 debd_pp_apl_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x1     : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 apl_win_ctrl_y1     : 12;  // 16:27
} H14A0_DEBD_PP_APL_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1818 debd_pp_apl_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                     : 24;  //  0:23 reserved
	UINT32 apl_position_sel    : 2;   // 24:25
	UINT32 apl_color_sel       : 2;   // 26:27
	UINT32 act_channel_sel     : 1;   //    28
} H14A0_DEBD_PP_APL_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x181c debd_pp_apl_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_y               : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 apl_y_otherside     : 10;  // 16:25
} H14A0_DEBD_PP_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x1820 debd_pp_l_level_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable              : 1;   //     0
	UINT32                     : 3;   //  1: 3 reserved
	UINT32 y_offset            : 10;  //  4:13
	UINT32                     : 2;   // 14:15 reserved
	UINT32 center_position     : 8;   // 16:23
	UINT32 y_gain              : 8;   // 24:31
} H14A0_DEBD_PP_L_LEVEL_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1824 debd_pp_r_level_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable              : 1;   //     0
	UINT32                     : 3;   //  1: 3 reserved
	UINT32 y_offset            : 10;  //  4:13
	UINT32                     : 2;   // 14:15 reserved
	UINT32 center_position     : 8;   // 16:23
	UINT32 y_gain              : 8;   // 24:31
} H14A0_DEBD_PP_R_LEVEL_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1830 debd_pp_l_vspyc_ctrl_00 ''
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
} H14A0_DEBD_PP_L_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1834 debd_pp_l_vspyc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                     : 8;   //  0: 7 reserved
	UINT32 saturation          : 8;   //  8:15
	UINT32 brightness          : 10;  // 16:25
} H14A0_DEBD_PP_L_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1838 debd_pp_r_vspyc_ctrl_00 ''
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
} H14A0_DEBD_PP_R_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x183c debd_pp_r_vspyc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                     : 8;   //  0: 7 reserved
	UINT32 saturation          : 8;   //  8:15
	UINT32 brightness          : 10;  // 16:25
} H14A0_DEBD_PP_R_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1840 debd_pp_l_csc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef0         : 15;  // 16:30
} H14A0_DEBD_PP_L_CSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1844 debd_pp_l_csc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef2         : 15;  // 16:30
} H14A0_DEBD_PP_L_CSC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1848 debd_pp_l_csc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef4         : 15;  // 16:30
} H14A0_DEBD_PP_L_CSC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x184c debd_pp_l_csc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef6         : 15;  // 16:30
} H14A0_DEBD_PP_L_CSC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x1850 debd_pp_l_csc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_coef8         : 15;  // 16:30
} H14A0_DEBD_PP_L_CSC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x1854 debd_pp_l_csc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst1         : 11;  // 16:26
} H14A0_DEBD_PP_L_CSC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x1858 debd_pp_l_csc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst3         : 11;  // 16:26
} H14A0_DEBD_PP_L_CSC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x185c debd_pp_l_csc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 icsc_enable         : 1;   //     0
	UINT32 ocsc_enable         : 1;   //     1
	UINT32                     : 14;  //  2:15 reserved
	UINT32 r_csc_ofst5         : 11;  // 16:26
} H14A0_DEBD_PP_L_CSC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x1860 debd_pp_r_csc_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef0         : 15;  // 16:30
} H14A0_DEBD_PP_R_CSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1864 debd_pp_r_csc_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef2         : 15;  // 16:30
} H14A0_DEBD_PP_R_CSC_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1868 debd_pp_r_csc_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef4         : 15;  // 16:30
} H14A0_DEBD_PP_R_CSC_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x186c debd_pp_r_csc_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7         : 15;  //  0:14
	UINT32                     : 1;   //    15 reserved
	UINT32 r_csc_coef6         : 15;  // 16:30
} H14A0_DEBD_PP_R_CSC_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x1870 debd_pp_r_csc_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_coef8         : 15;  // 16:30
} H14A0_DEBD_PP_R_CSC_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x1874 debd_pp_r_csc_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst1         : 11;  // 16:26
} H14A0_DEBD_PP_R_CSC_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x1878 debd_pp_r_csc_ctrl_06 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4         : 11;  //  0:10
	UINT32                     : 5;   // 11:15 reserved
	UINT32 r_csc_ofst3         : 11;  // 16:26
} H14A0_DEBD_PP_R_CSC_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x187c debd_pp_r_csc_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 icsc_enable         : 1;   //     0
	UINT32 ocsc_enable         : 1;   //     1
	UINT32                     : 14;  //  2:15 reserved
	UINT32 r_csc_ofst5         : 11;  // 16:26
} H14A0_DEBD_PP_R_CSC_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x1880 debd_pp_lrcr_ctrl_00 ''
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
	UINT32 r_out_sync_sel      : 1;   //    10
	UINT32                     : 1;   //    11 reserved
	UINT32 r_out_data_sel      : 1;   //    12
	UINT32 l_out_data_sel      : 1;   //    13
	UINT32                     : 2;   // 14:15 reserved
	UINT32 lrcr_gain           : 4;   // 16:19
	UINT32 hist_rd_en          : 1;   //    20
	UINT32                     : 1;   //    21 reserved
	UINT32 hist_mode           : 2;   // 22:23
} H14A0_DEBD_PP_LRCR_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x1884 debd_pp_lrcr_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 th_max_hist         : 8;   //  0: 7
	UINT32 th_valid_bins       : 8;   //  8:15
	UINT32 th1                 : 8;   // 16:23
	UINT32 th0                 : 8;   // 24:31
} H14A0_DEBD_PP_LRCR_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x1888 debd_pp_lrcr_ctrl_02 ''
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
} H14A0_DEBD_PP_LRCR_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x188c debd_pp_lrcr_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} H14A0_DEBD_PP_LRCR_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x1890 debd_pp_lrcr_ctrl_04 ''
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
} H14A0_DEBD_PP_LRCR_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x1894 debd_pp_lrcr_ctrl_05 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} H14A0_DEBD_PP_LRCR_CTRL_05_T;

/*-----------------------------------------------------------------------------
	0x1898 debd_pp_lrcr_ctrl_06 ''
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
} H14A0_DEBD_PP_LRCR_CTRL_06_T;

/*-----------------------------------------------------------------------------
	0x189c debd_pp_lrcr_ctrl_07 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} H14A0_DEBD_PP_LRCR_CTRL_07_T;

/*-----------------------------------------------------------------------------
	0x18a0 debd_pp_lrcr_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist            : 8;   //  0: 7
	UINT32 n_valid_bins        : 8;   //  8:15
	UINT32 cdf_x1              : 8;   // 16:23
	UINT32 cdf_x0              : 8;   // 24:31
} H14A0_DEBD_PP_LRCR_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x18a4 debd_pp_lrcr_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist            : 8;   //  0: 7
	UINT32 n_valid_bins        : 8;   //  8:15
	UINT32 cdf_x1              : 8;   // 16:23
	UINT32 cdf_x0              : 8;   // 24:31
} H14A0_DEBD_PP_LRCR_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x18a8 debd_pp_lrcr_stat_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist            : 8;   //  0: 7
	UINT32 n_valid_bins        : 8;   //  8:15
	UINT32 cdf_x1              : 8;   // 16:23
	UINT32 cdf_x0              : 8;   // 24:31
} H14A0_DEBD_PP_LRCR_STAT_02_T;

/*-----------------------------------------------------------------------------
	0x18b0 debd_pp_l_g_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_g_available : 1;   //    28
} H14A0_DEBD_PP_L_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x18b4 debd_pp_l_g_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_l_g_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_L_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x18b8 debd_pp_l_b_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_b_available : 1;   //    28
} H14A0_DEBD_PP_L_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x18bc debd_pp_l_b_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_l_b_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_L_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x18c0 debd_pp_l_r_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_r_available : 1;   //    28
} H14A0_DEBD_PP_L_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x18c4 debd_pp_l_r_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_l_r_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_L_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x18c8 debd_pp_r_g_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_g_available : 1;   //    28
} H14A0_DEBD_PP_R_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x18cc debd_pp_r_g_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_g_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_R_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x18d0 debd_pp_r_b_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_b_available : 1;   //    28
} H14A0_DEBD_PP_R_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x18d4 debd_pp_r_b_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_b_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_R_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x18d8 debd_pp_r_r_hist_ia_ctrl ''
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
} H14A0_DEBD_PP_R_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x18dc debd_pp_r_r_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_r_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_R_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x18e0 debd_pp_hist_ctrl_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_l_ch_sel       : 1;   //     0
	UINT32                     : 3;   //  1: 3 reserved
	UINT32 hist_r_ch_sel       : 1;   //     4
	UINT32                     : 3;   //  5: 7 reserved
	UINT32 hist_l_data_sel     : 4;   //  8:11
	UINT32 hist_r_data_sel     : 4;   // 12:15
	UINT32                     : 4;   // 16:19 reserved
	UINT32 hist_rd_en          : 1;   //    20
	UINT32                     : 3;   // 21:23 reserved
	UINT32 csc_l_hsv_hsl_sel   : 1;   //    24
	UINT32 csc_l_vscaling_en   : 1;   //    25
	UINT32                     : 2;   // 26:27 reserved
	UINT32 csc_r_hsv_hsl_sel   : 1;   //    28
	UINT32 csc_r_vscaling_en   : 1;   //    29
} H14A0_DEBD_PP_HIST_CTRL_00_T;

/*-----------------------------------------------------------------------------
	0x18e4 debd_pp_hist_ctrl_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                  : 12;  //  0:11
	UINT32                     : 3;   // 12:14 reserved
	UINT32 hist_l_window_h_en  : 1;   //    15
	UINT32 y0                  : 12;  // 16:27
	UINT32                     : 3;   // 28:30 reserved
	UINT32 hist_l_window_v_en  : 1;   //    31
} H14A0_DEBD_PP_HIST_CTRL_01_T;

/*-----------------------------------------------------------------------------
	0x18e8 debd_pp_hist_ctrl_02 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} H14A0_DEBD_PP_HIST_CTRL_02_T;

/*-----------------------------------------------------------------------------
	0x18ec debd_pp_hist_ctrl_03 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                  : 12;  //  0:11
	UINT32                     : 3;   // 12:14 reserved
	UINT32 hist_r_window_h_en  : 1;   //    15
	UINT32 y0                  : 12;  // 16:27
	UINT32                     : 3;   // 28:30 reserved
	UINT32 hist_r_window_v_en  : 1;   //    31
} H14A0_DEBD_PP_HIST_CTRL_03_T;

/*-----------------------------------------------------------------------------
	0x18f0 debd_pp_hist_ctrl_04 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                  : 12;  //  0:11
	UINT32                     : 4;   // 12:15 reserved
	UINT32 y1                  : 12;  // 16:27
} H14A0_DEBD_PP_HIST_CTRL_04_T;

/*-----------------------------------------------------------------------------
	0x18f4 debd_pp_hist_stat_00 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pixel_count         : 22;  //  0:21
} H14A0_DEBD_PP_HIST_STAT_00_T;

/*-----------------------------------------------------------------------------
	0x18f8 debd_pp_hist_stat_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pixel_count         : 22;  //  0:21
} H14A0_DEBD_PP_HIST_STAT_01_T;

/*-----------------------------------------------------------------------------
	0x1900 debd_pp_l_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_available   : 1;   //    28
} H14A0_DEBD_PP_L_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x1904 debd_pp_l_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_l_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_L_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
	0x1908 debd_pp_r_hist_ia_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address    : 8;   //  0: 7
	UINT32                     : 4;   //  8:11 reserved
	UINT32 hif_hist_ai         : 1;   //    12
	UINT32                     : 2;   // 13:14 reserved
	UINT32 hif_hist_enable     : 1;   //    15
	UINT32                     : 12;  // 16:27 reserved
	UINT32 hist_rd_available   : 1;   //    28
	UINT32                     : 2;   // 29:30 reserved
	UINT32 hist_read_done      : 1;   //    31
} H14A0_DEBD_PP_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
	0x190c debd_pp_r_hist_ia_data ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_hist_ia_data_f: 32;   // 31: 0
} H14A0_DEBD_PP_R_HIST_IA_DATA_T;

typedef struct {
	H14A0_DEBD_PP_TOP_CTRL_00_T         debd_pp_top_ctrl_00;                 // 0x1800
	H14A0_DEBD_PP_TOP_CTRL_01_T         debd_pp_top_ctrl_01;                 // 0x1804
	H14A0_DEBD_PP_TOP_CTRL_02_T         debd_pp_top_ctrl_02;                 // 0x1808
	UINT32                              reserved0;                           // 0x180c
	H14A0_DEBD_PP_APL_CTRL_00_T         debd_pp_apl_ctrl_00;                 // 0x1810
	H14A0_DEBD_PP_APL_CTRL_01_T         debd_pp_apl_ctrl_01;                 // 0x1814
	H14A0_DEBD_PP_APL_CTRL_02_T         debd_pp_apl_ctrl_02;                 // 0x1818
	H14A0_DEBD_PP_APL_STAT_00_T         debd_pp_apl_stat_00;                 // 0x181c
	H14A0_DEBD_PP_L_LEVEL_CTRL_00_T     debd_pp_l_level_ctrl_00;             // 0x1820
	H14A0_DEBD_PP_R_LEVEL_CTRL_00_T     debd_pp_r_level_ctrl_00;             // 0x1824
	UINT32                              reserved1[2];                        // 0x1828~0x182c
	H14A0_DEBD_PP_L_VSPYC_CTRL_00_T     debd_pp_l_vspyc_ctrl_00;             // 0x1830
	H14A0_DEBD_PP_L_VSPYC_CTRL_01_T     debd_pp_l_vspyc_ctrl_01;             // 0x1834
	H14A0_DEBD_PP_R_VSPYC_CTRL_00_T     debd_pp_r_vspyc_ctrl_00;             // 0x1838
	H14A0_DEBD_PP_R_VSPYC_CTRL_01_T     debd_pp_r_vspyc_ctrl_01;             // 0x183c
	H14A0_DEBD_PP_L_CSC_CTRL_00_T       debd_pp_l_csc_ctrl_00;               // 0x1840
	H14A0_DEBD_PP_L_CSC_CTRL_01_T       debd_pp_l_csc_ctrl_01;               // 0x1844
	H14A0_DEBD_PP_L_CSC_CTRL_02_T       debd_pp_l_csc_ctrl_02;               // 0x1848
	H14A0_DEBD_PP_L_CSC_CTRL_03_T       debd_pp_l_csc_ctrl_03;               // 0x184c
	H14A0_DEBD_PP_L_CSC_CTRL_04_T       debd_pp_l_csc_ctrl_04;               // 0x1850
	H14A0_DEBD_PP_L_CSC_CTRL_05_T       debd_pp_l_csc_ctrl_05;               // 0x1854
	H14A0_DEBD_PP_L_CSC_CTRL_06_T       debd_pp_l_csc_ctrl_06;               // 0x1858
	H14A0_DEBD_PP_L_CSC_CTRL_07_T       debd_pp_l_csc_ctrl_07;               // 0x185c
	H14A0_DEBD_PP_R_CSC_CTRL_00_T       debd_pp_r_csc_ctrl_00;               // 0x1860
	H14A0_DEBD_PP_R_CSC_CTRL_01_T       debd_pp_r_csc_ctrl_01;               // 0x1864
	H14A0_DEBD_PP_R_CSC_CTRL_02_T       debd_pp_r_csc_ctrl_02;               // 0x1868
	H14A0_DEBD_PP_R_CSC_CTRL_03_T       debd_pp_r_csc_ctrl_03;               // 0x186c
	H14A0_DEBD_PP_R_CSC_CTRL_04_T       debd_pp_r_csc_ctrl_04;               // 0x1870
	H14A0_DEBD_PP_R_CSC_CTRL_05_T       debd_pp_r_csc_ctrl_05;               // 0x1874
	H14A0_DEBD_PP_R_CSC_CTRL_06_T       debd_pp_r_csc_ctrl_06;               // 0x1878
	H14A0_DEBD_PP_R_CSC_CTRL_07_T       debd_pp_r_csc_ctrl_07;               // 0x187c
	H14A0_DEBD_PP_LRCR_CTRL_00_T        debd_pp_lrcr_ctrl_00;                // 0x1880
	H14A0_DEBD_PP_LRCR_CTRL_01_T        debd_pp_lrcr_ctrl_01;                // 0x1884
	H14A0_DEBD_PP_LRCR_CTRL_02_T        debd_pp_lrcr_ctrl_02;                // 0x1888
	H14A0_DEBD_PP_LRCR_CTRL_03_T        debd_pp_lrcr_ctrl_03;                // 0x188c
	H14A0_DEBD_PP_LRCR_CTRL_04_T        debd_pp_lrcr_ctrl_04;                // 0x1890
	H14A0_DEBD_PP_LRCR_CTRL_05_T        debd_pp_lrcr_ctrl_05;                // 0x1894
	H14A0_DEBD_PP_LRCR_CTRL_06_T        debd_pp_lrcr_ctrl_06;                // 0x1898
	H14A0_DEBD_PP_LRCR_CTRL_07_T        debd_pp_lrcr_ctrl_07;                // 0x189c
	H14A0_DEBD_PP_LRCR_STAT_00_T        debd_pp_lrcr_stat_00;                // 0x18a0
	H14A0_DEBD_PP_LRCR_STAT_01_T        debd_pp_lrcr_stat_01;                // 0x18a4
	H14A0_DEBD_PP_LRCR_STAT_02_T        debd_pp_lrcr_stat_02;                // 0x18a8
	UINT32                              reserved2;                           // 0x18ac
	H14A0_DEBD_PP_L_G_HIST_IA_CTRL_T    debd_pp_l_g_hist_ia_ctrl;            // 0x18b0
	H14A0_DEBD_PP_L_G_HIST_IA_DATA_T    debd_pp_l_g_hist_ia_data;            // 0x18b4
	H14A0_DEBD_PP_L_B_HIST_IA_CTRL_T    debd_pp_l_b_hist_ia_ctrl;            // 0x18b8
	H14A0_DEBD_PP_L_B_HIST_IA_DATA_T    debd_pp_l_b_hist_ia_data;            // 0x18bc
	H14A0_DEBD_PP_L_R_HIST_IA_CTRL_T    debd_pp_l_r_hist_ia_ctrl;            // 0x18c0
	H14A0_DEBD_PP_L_R_HIST_IA_DATA_T    debd_pp_l_r_hist_ia_data;            // 0x18c4
	H14A0_DEBD_PP_R_G_HIST_IA_CTRL_T    debd_pp_r_g_hist_ia_ctrl;            // 0x18c8
	H14A0_DEBD_PP_R_G_HIST_IA_DATA_T    debd_pp_r_g_hist_ia_data;            // 0x18cc
	H14A0_DEBD_PP_R_B_HIST_IA_CTRL_T    debd_pp_r_b_hist_ia_ctrl;            // 0x18d0
	H14A0_DEBD_PP_R_B_HIST_IA_DATA_T    debd_pp_r_b_hist_ia_data;            // 0x18d4
	H14A0_DEBD_PP_R_R_HIST_IA_CTRL_T    debd_pp_r_r_hist_ia_ctrl;            // 0x18d8
	H14A0_DEBD_PP_R_R_HIST_IA_DATA_T    debd_pp_r_r_hist_ia_data;            // 0x18dc
	H14A0_DEBD_PP_HIST_CTRL_00_T        debd_pp_hist_ctrl_00;                // 0x18e0
	H14A0_DEBD_PP_HIST_CTRL_01_T        debd_pp_hist_ctrl_01;                // 0x18e4
	H14A0_DEBD_PP_HIST_CTRL_02_T        debd_pp_hist_ctrl_02;                // 0x18e8
	H14A0_DEBD_PP_HIST_CTRL_03_T        debd_pp_hist_ctrl_03;                // 0x18ec
	H14A0_DEBD_PP_HIST_CTRL_04_T        debd_pp_hist_ctrl_04;                // 0x18f0
	H14A0_DEBD_PP_HIST_STAT_00_T        debd_pp_hist_stat_00;                // 0x18f4
	H14A0_DEBD_PP_HIST_STAT_01_T        debd_pp_hist_stat_01;                // 0x18f8
	UINT32                              reserved3;                           // 0x18fc
	H14A0_DEBD_PP_L_HIST_IA_CTRL_T      debd_pp_l_hist_ia_ctrl;              // 0x1900
	H14A0_DEBD_PP_L_HIST_IA_DATA_T      debd_pp_l_hist_ia_data;              // 0x1904
	H14A0_DEBD_PP_R_HIST_IA_CTRL_T      debd_pp_r_hist_ia_ctrl;              // 0x1908
	H14A0_DEBD_PP_R_HIST_IA_DATA_T      debd_pp_r_hist_ia_data;              // 0x190c
} DE_DPP_REG_H14A0_T;

#endif
