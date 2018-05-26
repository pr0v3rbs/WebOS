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

#ifndef _PE_DPP_REG_H13A0_H_
#define _PE_DPP_REG_H13A0_H_

/*-----------------------------------------------------------------------------
		0x1800 debd_pp_top_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 load_time                          : 1 ;  // 0      
	UINT32 load_enable                        : 1 ;  // 1      
	UINT32 load_type                          : 1 ;  // 2      
} PE_H13A0_DEBD_PP_TOP_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1804 debd_pp_top_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 debd_pp_r_bypass                   : 1 ;  // 0      
	UINT32 debd_pp_l_bypass                   : 1 ;  // 1      
	UINT32                                    : 3 ;  // 4:2     reserved
	UINT32 async_delay_mode                   : 1 ;  // 5      
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 vdpulse_pos                        : 12;  // 19:8   
} PE_H13A0_DEBD_PP_TOP_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x1808 debd_pp_top_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 va_debd_pp_in                      : 1 ;  // 0      
	UINT32 va_debd_pp_out                     : 1 ;  // 1      
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 stat_ocsc_r                        : 1 ;  // 4      
	UINT32 stat_ocsc_l                        : 1 ;  // 5      
	UINT32 stat_lrcr                          : 1 ;  // 6      
	UINT32 stat_icsc_r                        : 1 ;  // 7      
	UINT32 stat_icsc_l                        : 1 ;  // 8      
	UINT32 stat_vspyc_r                       : 1 ;  // 9      
	UINT32 stat_vspyc_l                       : 1 ;  // 10     
	UINT32 stat_lvctrl_r                      : 1 ;  // 11     
	UINT32 stat_lvctrl_l                      : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 vcnt                               : 12;  // 27:16  
} PE_H13A0_DEBD_PP_TOP_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x1810 debd_pp_apl_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x0                    : 12;  // 11:0   
	UINT32                                    : 4 ;  // 15:12   reserved
	UINT32 apl_win_ctrl_y0                    : 12;  // 27:16  
} PE_H13A0_DEBD_PP_APL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1814 debd_pp_apl_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_win_ctrl_x1                    : 12;  // 11:0   
	UINT32                                    : 4 ;  // 15:12   reserved
	UINT32 apl_win_ctrl_y1                    : 12;  // 27:16  
} PE_H13A0_DEBD_PP_APL_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x1818 debd_pp_apl_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 24;  // 23:0    reserved
	UINT32 apl_position_sel                   : 2 ;  // 25:24  
	UINT32 apl_color_sel                      : 2 ;  // 27:26  
	UINT32 act_channel_sel                    : 1 ;  // 28     
} PE_H13A0_DEBD_PP_APL_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x181c debd_pp_apl_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_y                              : 10;  // 9:0    
	UINT32                                    : 6 ;  // 15:10   reserved
	UINT32 apl_y_otherside                    : 10;  // 25:16  
} PE_H13A0_DEBD_PP_APL_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x1820 debd_pp_l_level_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 y_offset                           : 10;  // 13:4   
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 center_position                    : 8 ;  // 23:16  
	UINT32 y_gain                             : 8 ;  // 31:24  
} PE_H13A0_DEBD_PP_L_LEVEL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1824 debd_pp_r_level_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 y_offset                           : 10;  // 13:4   
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 center_position                    : 8 ;  // 23:16  
	UINT32 y_gain                             : 8 ;  // 31:24  
} PE_H13A0_DEBD_PP_R_LEVEL_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1830 debd_pp_l_vspyc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  // 0      
	UINT32 graycolor_enable                   : 1 ;  // 1      
	UINT32 color_only_enable                  : 1 ;  // 2      
	UINT32 cb_info                            : 1 ;  // 3      
	UINT32 cr_info                            : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 center_position                    : 8 ;  // 15:8   
	UINT32 contrast                           : 10;  // 25:16  
} PE_H13A0_DEBD_PP_L_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1834 debd_pp_l_vspyc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  // 7:0     reserved
	UINT32 saturation                         : 8 ;  // 15:8   
	UINT32 brightness                         : 10;  // 25:16  
} PE_H13A0_DEBD_PP_L_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x1838 debd_pp_r_vspyc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  // 0      
	UINT32 graycolor_enable                   : 1 ;  // 1      
	UINT32 color_only_enable                  : 1 ;  // 2      
	UINT32 cb_info                            : 1 ;  // 3      
	UINT32 cr_info                            : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 center_position                    : 8 ;  // 15:8   
	UINT32 contrast                           : 10;  // 25:16  
} PE_H13A0_DEBD_PP_R_VSPYC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x183c debd_pp_r_vspyc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  // 7:0     reserved
	UINT32 saturation                         : 8 ;  // 15:8   
	UINT32 brightness                         : 10;  // 25:16  
} PE_H13A0_DEBD_PP_R_VSPYC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x1840 debd_pp_l_csc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef0                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1844 debd_pp_l_csc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef2                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x1848 debd_pp_l_csc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef4                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x184c debd_pp_l_csc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef6                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x1850 debd_pp_l_csc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0                        : 11;  // 10:0   
	UINT32                                    : 5 ;  // 15:11   reserved
	UINT32 r_csc_coef8                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x1854 debd_pp_l_csc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2                        : 11;  // 10:0   
	UINT32                                    : 5 ;  // 15:11   reserved
	UINT32 r_csc_ofst1                        : 11;  // 26:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x1858 debd_pp_l_csc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4                        : 11;  // 10:0   
	UINT32                                    : 5 ;  // 15:11   reserved
	UINT32 r_csc_ofst3                        : 11;  // 26:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x185c debd_pp_l_csc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 icsc_enable                        : 1 ;  // 0      
	UINT32 ocsc_enable                        : 1 ;  // 1      
	UINT32                                    : 14;  // 15:2    reserved
	UINT32 r_csc_ofst5                        : 11;  // 26:16  
} PE_H13A0_DEBD_PP_L_CSC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x1860 debd_pp_r_csc_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef1                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef0                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1864 debd_pp_r_csc_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef3                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef2                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x1868 debd_pp_r_csc_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef5                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef4                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x186c debd_pp_r_csc_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_coef7                        : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 r_csc_coef6                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x1870 debd_pp_r_csc_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst0                        : 11;  // 10:0   
	UINT32                                    : 5 ;  // 15:11   reserved
	UINT32 r_csc_coef8                        : 15;  // 30:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x1874 debd_pp_r_csc_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst2                        : 11;  // 10:0   
	UINT32                                    : 5 ;  // 15:11   reserved
	UINT32 r_csc_ofst1                        : 11;  // 26:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x1878 debd_pp_r_csc_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 r_csc_ofst4                        : 11;  // 10:0   
	UINT32                                    : 5 ;  // 15:11   reserved
	UINT32 r_csc_ofst3                        : 11;  // 26:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x187c debd_pp_r_csc_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 icsc_enable                        : 1 ;  // 0      
	UINT32 ocsc_enable                        : 1 ;  // 1      
	UINT32                                    : 14;  // 15:2    reserved
	UINT32 r_csc_ofst5                        : 11;  // 26:16  
} PE_H13A0_DEBD_PP_R_CSC_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x1880 debd_pp_lrcr_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 enable                             : 1 ;  // 0      
	UINT32 adjust_en                          : 1 ;  // 1      
	UINT32 protection_en                      : 1 ;  // 2      
	UINT32 adjust_en2                         : 1 ;  // 3      
	UINT32 r_hist_data_sel                    : 1 ;  // 4      
	UINT32 l_hist_data_sel                    : 1 ;  // 5      
	UINT32 r_proc_data_sel                    : 1 ;  // 6      
	UINT32 l_proc_data_sel                    : 1 ;  // 7      
	UINT32 ff_mode_en                         : 1 ;  // 8      
	UINT32 ff_mode_parity                     : 1 ;  // 9      
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 r_out_data_sel                     : 1 ;  // 12     
	UINT32 l_out_data_sel                     : 1 ;  // 13     
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_lrcr_gain                      : 4 ;  // 19:16  
	UINT32 hist_rd_en                         : 1 ;  // 20     
	UINT32                                    : 1 ;  // 21:21   reserved
	UINT32 hist_mode                          : 2 ;  // 23:22  
} PE_H13A0_DEBD_PP_LRCR_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x1884 debd_pp_lrcr_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 th_max_hist                        : 8 ;  // 7:0    
	UINT32 th_valid_bins                      : 8 ;  // 15:8   
	UINT32 th1                                : 8 ;  // 23:16  
	UINT32 th0                                : 8 ;  // 31:24  
} PE_H13A0_DEBD_PP_LRCR_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x1888 debd_pp_lrcr_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11:0   
	UINT32                                    : 2 ;  // 13:12   reserved
	UINT32 hist_l_window_h_bbd                : 1 ;  // 14     
	UINT32 hist_l_window_h_en                 : 1 ;  // 15     
	UINT32 y0                                 : 12;  // 27:16  
	UINT32                                    : 2 ;  // 29:28   reserved
	UINT32 hist_l_window_v_bbd                : 1 ;  // 30     
	UINT32 hist_l_window_v_en                 : 1 ;  // 31     
} PE_H13A0_DEBD_PP_LRCR_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x188c debd_pp_lrcr_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11:0   
	UINT32                                    : 4 ;  // 15:12   reserved
	UINT32 y1                                 : 12;  // 27:16  
} PE_H13A0_DEBD_PP_LRCR_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x1890 debd_pp_lrcr_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11:0   
	UINT32                                    : 2 ;  // 13:12   reserved
	UINT32 hist_r_window_h_bbd                : 1 ;  // 14     
	UINT32 hist_r_window_h_en                 : 1 ;  // 15     
	UINT32 y0                                 : 12;  // 27:16  
	UINT32                                    : 2 ;  // 29:28   reserved
	UINT32 hist_r_window_v_bbd                : 1 ;  // 30     
	UINT32 hist_r_window_v_en                 : 1 ;  // 31     
} PE_H13A0_DEBD_PP_LRCR_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x1894 debd_pp_lrcr_ctrl_05 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11:0   
	UINT32                                    : 4 ;  // 15:12   reserved
	UINT32 y1                                 : 12;  // 27:16  
} PE_H13A0_DEBD_PP_LRCR_CTRL_05_T;

/*-----------------------------------------------------------------------------
		0x1898 debd_pp_lrcr_ctrl_06 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11:0   
	UINT32                                    : 2 ;  // 13:12   reserved
	UINT32 active_window_h_bbd                : 1 ;  // 14     
	UINT32 active_window_h_en                 : 1 ;  // 15     
	UINT32 y0                                 : 12;  // 27:16  
	UINT32                                    : 2 ;  // 29:28   reserved
	UINT32 active_window_v_bbd                : 1 ;  // 30     
	UINT32 active_window_v_en                 : 1 ;  // 31     
} PE_H13A0_DEBD_PP_LRCR_CTRL_06_T;

/*-----------------------------------------------------------------------------
		0x189c debd_pp_lrcr_ctrl_07 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11:0   
	UINT32                                    : 4 ;  // 15:12   reserved
	UINT32 y1                                 : 12;  // 27:16  
} PE_H13A0_DEBD_PP_LRCR_CTRL_07_T;

/*-----------------------------------------------------------------------------
		0x18a0 debd_pp_lrcr_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  // 7:0    
	UINT32 n_valid_bins                       : 8 ;  // 15:8   
	UINT32 cdf_x1                             : 8 ;  // 23:16  
	UINT32 cdf_x0                             : 8 ;  // 31:24  
} PE_H13A0_DEBD_PP_LRCR_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x18a4 debd_pp_lrcr_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  // 7:0    
	UINT32 n_valid_bins                       : 8 ;  // 15:8   
	UINT32 cdf_x1                             : 8 ;  // 23:16  
	UINT32 cdf_x0                             : 8 ;  // 31:24  
} PE_H13A0_DEBD_PP_LRCR_STAT_01_T;

/*-----------------------------------------------------------------------------
		0x18a8 debd_pp_lrcr_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 max_hist                           : 8 ;  // 7:0    
	UINT32 n_valid_bins                       : 8 ;  // 15:8   
	UINT32 cdf_x1                             : 8 ;  // 23:16  
	UINT32 cdf_x0                             : 8 ;  // 31:24  
} PE_H13A0_DEBD_PP_LRCR_STAT_02_T;

/*-----------------------------------------------------------------------------
		0x18b0 debd_pp_l_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_g_available                : 1 ;  // 28     
} PE_H13A0_DEBD_PP_L_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x18b4 debd_pp_l_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_L_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0x18b8 debd_pp_l_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_b_available                : 1 ;  // 28     
} PE_H13A0_DEBD_PP_L_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x18bc debd_pp_l_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_L_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0x18c0 debd_pp_l_r_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_r_available                : 1 ;  // 28     
} PE_H13A0_DEBD_PP_L_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x18c4 debd_pp_l_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_L_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0x18c8 debd_pp_r_g_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_g_available                : 1 ;  // 28     
} PE_H13A0_DEBD_PP_R_G_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x18cc debd_pp_r_g_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_R_G_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0x18d0 debd_pp_r_b_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_b_available                : 1 ;  // 28     
} PE_H13A0_DEBD_PP_R_B_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x18d4 debd_pp_r_b_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_R_B_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0x18d8 debd_pp_r_r_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_r_available                : 1 ;  // 28     
	UINT32                                    : 2 ;  // 30:29   reserved
	UINT32 hist_rd_done                       : 1 ;  // 31     
} PE_H13A0_DEBD_PP_R_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x18dc debd_pp_r_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_R_R_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0x18e0 debd_pp_hist_ctrl_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hist_l_ch_sel                      : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 hist_r_ch_sel                      : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 hist_l_data_sel                    : 4 ;  // 11:8   
	UINT32 hist_r_data_sel                    : 4 ;  // 15:12  
	UINT32                                    : 4 ;  // 19:16   reserved
	UINT32 hist_rd_en                         : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 csc_l_hsv_hsl_sel                  : 1 ;  // 24     
	UINT32 csc_l_vscaling_en                  : 1 ;  // 25     
	UINT32                                    : 2 ;  // 27:26   reserved
	UINT32 csc_r_hsv_hsl_sel                  : 1 ;  // 28     
	UINT32 csc_r_vscaling_en                  : 1 ;  // 29     
} PE_H13A0_DEBD_PP_HIST_CTRL_00_T;

/*-----------------------------------------------------------------------------
		0x18e4 debd_pp_hist_ctrl_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11:0   
	UINT32                                    : 3 ;  // 14:12   reserved
	UINT32 hist_l_window_h_en                 : 1 ;  // 15     
	UINT32 y0                                 : 12;  // 27:16  
	UINT32                                    : 3 ;  // 30:28   reserved
	UINT32 hist_l_window_v_en                 : 1 ;  // 31     
} PE_H13A0_DEBD_PP_HIST_CTRL_01_T;

/*-----------------------------------------------------------------------------
		0x18e8 debd_pp_hist_ctrl_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11:0   
	UINT32                                    : 4 ;  // 15:12   reserved
	UINT32 y1                                 : 12;  // 27:16  
} PE_H13A0_DEBD_PP_HIST_CTRL_02_T;

/*-----------------------------------------------------------------------------
		0x18ec debd_pp_hist_ctrl_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11:0   
	UINT32                                    : 3 ;  // 14:12   reserved
	UINT32 hist_r_window_h_en                 : 1 ;  // 15     
	UINT32 y0                                 : 12;  // 27:16  
	UINT32                                    : 3 ;  // 30:28   reserved
	UINT32 hist_r_window_v_en                 : 1 ;  // 31     
} PE_H13A0_DEBD_PP_HIST_CTRL_03_T;

/*-----------------------------------------------------------------------------
		0x18f0 debd_pp_hist_ctrl_04 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11:0   
	UINT32                                    : 4 ;  // 15:12   reserved
	UINT32 y1                                 : 12;  // 27:16  
} PE_H13A0_DEBD_PP_HIST_CTRL_04_T;

/*-----------------------------------------------------------------------------
		0x18f4 debd_pp_hist_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pixel_count                        : 22;  // 21:0   
} PE_H13A0_DEBD_PP_HIST_STAT_00_T;

/*-----------------------------------------------------------------------------
		0x18f8 debd_pp_hist_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pixel_count                        : 22;  // 21:0   
} PE_H13A0_DEBD_PP_HIST_STAT_01_T;

/*-----------------------------------------------------------------------------
		0x1900 debd_pp_l_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_available                  : 1 ;  // 28     
} PE_H13A0_DEBD_PP_L_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x1904 debd_pp_l_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_L_HIST_IA_DATA_T;

/*-----------------------------------------------------------------------------
		0x1908 debd_pp_r_hist_ia_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 hif_hist_address                   : 8 ;  // 7:0    
	UINT32                                    : 4 ;  // 11:8    reserved
	UINT32 hif_hist_ai                        : 1 ;  // 12     
	UINT32                                    : 2 ;  // 14:13   reserved
	UINT32 hif_hist_enable                    : 1 ;  // 15     
	UINT32                                    : 12;  // 27:16   reserved
	UINT32 hist_rd_available                  : 1 ;  // 28     
	UINT32                                    : 2 ;  // 30:29   reserved
	UINT32 hist_read_done                     : 1 ;  // 31     
} PE_H13A0_DEBD_PP_R_HIST_IA_CTRL_T;

/*-----------------------------------------------------------------------------
		0x190c debd_pp_r_hist_ia_data ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32 ;  // 31:0    reserved
} PE_H13A0_DEBD_PP_R_HIST_IA_DATA_T;


typedef struct {
	PE_H13A0_DEBD_PP_TOP_CTRL_00_T          debd_pp_top_ctrl_00;          //0x1800
	PE_H13A0_DEBD_PP_TOP_CTRL_01_T          debd_pp_top_ctrl_01;          //0x1804
	PE_H13A0_DEBD_PP_TOP_CTRL_02_T          debd_pp_top_ctrl_02;          //0x1808
	UINT32                                  reserved00;                   //0x180c
	PE_H13A0_DEBD_PP_APL_CTRL_00_T          debd_pp_apl_ctrl_00;          //0x1810
	PE_H13A0_DEBD_PP_APL_CTRL_01_T          debd_pp_apl_ctrl_01;          //0x1814
	PE_H13A0_DEBD_PP_APL_CTRL_02_T          debd_pp_apl_ctrl_02;          //0x1818
	PE_H13A0_DEBD_PP_APL_STAT_00_T          debd_pp_apl_stat_00;          //0x181c
	PE_H13A0_DEBD_PP_L_LEVEL_CTRL_00_T      debd_pp_l_level_ctrl_00;      //0x1820
	PE_H13A0_DEBD_PP_R_LEVEL_CTRL_00_T      debd_pp_r_level_ctrl_00;      //0x1824
	UINT32                                  reserved01[2];                //0x1828~0x182c
	PE_H13A0_DEBD_PP_L_VSPYC_CTRL_00_T      debd_pp_l_vspyc_ctrl_00;      //0x1830
	PE_H13A0_DEBD_PP_L_VSPYC_CTRL_01_T      debd_pp_l_vspyc_ctrl_01;      //0x1834
	PE_H13A0_DEBD_PP_R_VSPYC_CTRL_00_T      debd_pp_r_vspyc_ctrl_00;      //0x1838
	PE_H13A0_DEBD_PP_R_VSPYC_CTRL_01_T      debd_pp_r_vspyc_ctrl_01;      //0x183c
	PE_H13A0_DEBD_PP_L_CSC_CTRL_00_T        debd_pp_l_csc_ctrl_00;        //0x1840
	PE_H13A0_DEBD_PP_L_CSC_CTRL_01_T        debd_pp_l_csc_ctrl_01;        //0x1844
	PE_H13A0_DEBD_PP_L_CSC_CTRL_02_T        debd_pp_l_csc_ctrl_02;        //0x1848
	PE_H13A0_DEBD_PP_L_CSC_CTRL_03_T        debd_pp_l_csc_ctrl_03;        //0x184c
	PE_H13A0_DEBD_PP_L_CSC_CTRL_04_T        debd_pp_l_csc_ctrl_04;        //0x1850
	PE_H13A0_DEBD_PP_L_CSC_CTRL_05_T        debd_pp_l_csc_ctrl_05;        //0x1854
	PE_H13A0_DEBD_PP_L_CSC_CTRL_06_T        debd_pp_l_csc_ctrl_06;        //0x1858
	PE_H13A0_DEBD_PP_L_CSC_CTRL_07_T        debd_pp_l_csc_ctrl_07;        //0x185c
	PE_H13A0_DEBD_PP_R_CSC_CTRL_00_T        debd_pp_r_csc_ctrl_00;        //0x1860
	PE_H13A0_DEBD_PP_R_CSC_CTRL_01_T        debd_pp_r_csc_ctrl_01;        //0x1864
	PE_H13A0_DEBD_PP_R_CSC_CTRL_02_T        debd_pp_r_csc_ctrl_02;        //0x1868
	PE_H13A0_DEBD_PP_R_CSC_CTRL_03_T        debd_pp_r_csc_ctrl_03;        //0x186c
	PE_H13A0_DEBD_PP_R_CSC_CTRL_04_T        debd_pp_r_csc_ctrl_04;        //0x1870
	PE_H13A0_DEBD_PP_R_CSC_CTRL_05_T        debd_pp_r_csc_ctrl_05;        //0x1874
	PE_H13A0_DEBD_PP_R_CSC_CTRL_06_T        debd_pp_r_csc_ctrl_06;        //0x1878
	PE_H13A0_DEBD_PP_R_CSC_CTRL_07_T        debd_pp_r_csc_ctrl_07;        //0x187c
	PE_H13A0_DEBD_PP_LRCR_CTRL_00_T         debd_pp_lrcr_ctrl_00;         //0x1880
	PE_H13A0_DEBD_PP_LRCR_CTRL_01_T         debd_pp_lrcr_ctrl_01;         //0x1884
	PE_H13A0_DEBD_PP_LRCR_CTRL_02_T         debd_pp_lrcr_ctrl_02;         //0x1888
	PE_H13A0_DEBD_PP_LRCR_CTRL_03_T         debd_pp_lrcr_ctrl_03;         //0x188c
	PE_H13A0_DEBD_PP_LRCR_CTRL_04_T         debd_pp_lrcr_ctrl_04;         //0x1890
	PE_H13A0_DEBD_PP_LRCR_CTRL_05_T         debd_pp_lrcr_ctrl_05;         //0x1894
	PE_H13A0_DEBD_PP_LRCR_CTRL_06_T         debd_pp_lrcr_ctrl_06;         //0x1898
	PE_H13A0_DEBD_PP_LRCR_CTRL_07_T         debd_pp_lrcr_ctrl_07;         //0x189c
	PE_H13A0_DEBD_PP_LRCR_STAT_00_T         debd_pp_lrcr_stat_00;         //0x18a0
	PE_H13A0_DEBD_PP_LRCR_STAT_01_T         debd_pp_lrcr_stat_01;         //0x18a4
	PE_H13A0_DEBD_PP_LRCR_STAT_02_T         debd_pp_lrcr_stat_02;         //0x18a8
	UINT32                                  reserved02;                   //0x18ac
	PE_H13A0_DEBD_PP_L_G_HIST_IA_CTRL_T     debd_pp_l_g_hist_ia_ctrl;     //0x18b0
	PE_H13A0_DEBD_PP_L_G_HIST_IA_DATA_T     debd_pp_l_g_hist_ia_data;     //0x18b4
	PE_H13A0_DEBD_PP_L_B_HIST_IA_CTRL_T     debd_pp_l_b_hist_ia_ctrl;     //0x18b8
	PE_H13A0_DEBD_PP_L_B_HIST_IA_DATA_T     debd_pp_l_b_hist_ia_data;     //0x18bc
	PE_H13A0_DEBD_PP_L_R_HIST_IA_CTRL_T     debd_pp_l_r_hist_ia_ctrl;     //0x18c0
	PE_H13A0_DEBD_PP_L_R_HIST_IA_DATA_T     debd_pp_l_r_hist_ia_data;     //0x18c4
	PE_H13A0_DEBD_PP_R_G_HIST_IA_CTRL_T     debd_pp_r_g_hist_ia_ctrl;     //0x18c8
	PE_H13A0_DEBD_PP_R_G_HIST_IA_DATA_T     debd_pp_r_g_hist_ia_data;     //0x18cc
	PE_H13A0_DEBD_PP_R_B_HIST_IA_CTRL_T     debd_pp_r_b_hist_ia_ctrl;     //0x18d0
	PE_H13A0_DEBD_PP_R_B_HIST_IA_DATA_T     debd_pp_r_b_hist_ia_data;     //0x18d4
	PE_H13A0_DEBD_PP_R_R_HIST_IA_CTRL_T     debd_pp_r_r_hist_ia_ctrl;     //0x18d8
	PE_H13A0_DEBD_PP_R_R_HIST_IA_DATA_T     debd_pp_r_r_hist_ia_data;     //0x18dc
	PE_H13A0_DEBD_PP_HIST_CTRL_00_T         debd_pp_hist_ctrl_00;         //0x18e0
	PE_H13A0_DEBD_PP_HIST_CTRL_01_T         debd_pp_hist_ctrl_01;         //0x18e4
	PE_H13A0_DEBD_PP_HIST_CTRL_02_T         debd_pp_hist_ctrl_02;         //0x18e8
	PE_H13A0_DEBD_PP_HIST_CTRL_03_T         debd_pp_hist_ctrl_03;         //0x18ec
	PE_H13A0_DEBD_PP_HIST_CTRL_04_T         debd_pp_hist_ctrl_04;         //0x18f0
	PE_H13A0_DEBD_PP_HIST_STAT_00_T         debd_pp_hist_stat_00;         //0x18f4
	PE_H13A0_DEBD_PP_HIST_STAT_01_T         debd_pp_hist_stat_01;         //0x18f8
	UINT32                                  reserved03;                   //0x18fc
	PE_H13A0_DEBD_PP_L_HIST_IA_CTRL_T       debd_pp_l_hist_ia_ctrl;       //0x1900
	PE_H13A0_DEBD_PP_L_HIST_IA_DATA_T       debd_pp_l_hist_ia_data;       //0x1904
	PE_H13A0_DEBD_PP_R_HIST_IA_CTRL_T       debd_pp_r_hist_ia_ctrl;       //0x1908
	PE_H13A0_DEBD_PP_R_HIST_IA_DATA_T       debd_pp_r_hist_ia_data;       //0x190c
}PE_DPP_REG_H13A0_T;
#endif
