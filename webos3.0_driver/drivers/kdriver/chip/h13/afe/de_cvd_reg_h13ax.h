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


#ifndef _REG_CVD_H13Ax_h
#define _REG_CVD_H13Ax_h

/*-----------------------------------------------------------------------------
		0xc0017100L cvd_intr1_enable0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_enable0                  : 32;  // 31:0   
} H13Ax_CVD_INTR1_ENABLE0_T;

/*-----------------------------------------------------------------------------
		0xc0017104L cvd_intr1_intr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_chromalock_fal                 : 1 ;  // 0      
	UINT32 cvd_chromalock_ris                 : 1 ;  // 1      
	UINT32 cvd_vlock_fal                      : 1 ;  // 2      
	UINT32 cvd_vlock_ris                      : 1 ;  // 3      
	UINT32 cvd_hlock_fal                      : 1 ;  // 4      
	UINT32 cvd_hlock_ris                      : 1 ;  // 5      
	UINT32 cvd_no_signal_fal                  : 1 ;  // 6      
	UINT32 cvd_no_signal_ris                  : 1 ;  // 7      
	UINT32 cvd_noburst_detected_fal           : 1 ;  // 8      
	UINT32 cvd_noburst_detected_ris           : 1 ;  // 9      
	UINT32 cvd_vnon_standard_fal              : 1 ;  // 10     
	UINT32 cvd_vnon_standard_ris              : 1 ;  // 11     
	UINT32 cvd_hnon_standard_fal              : 1 ;  // 12     
	UINT32 cvd_hnon_standard_ris              : 1 ;  // 13     
	UINT32 cvd_proscan_detected_fal           : 1 ;  // 14     
	UINT32 cvd_proscan_detected_ris           : 1 ;  // 15     
	UINT32 cvd_vcr_fal                        : 1 ;  // 16     
	UINT32 cvd_vcr_ris                        : 1 ;  // 17     
	UINT32 cvd_noisy_fal                      : 1 ;  // 18     
	UINT32 cvd_noisy_ris                      : 1 ;  // 19     
	UINT32 cvd_vline_625_detected_fal         : 1 ;  // 20     
	UINT32 cvd_vline_625_detected_ris         : 1 ;  // 21     
	UINT32 cvd_secam_detected_fal             : 1 ;  // 22     
	UINT32 cvd_secam_detected_ris             : 1 ;  // 23     
	UINT32 cvd_pal_detected_fal               : 1 ;  // 24     
	UINT32 cvd_pal_detected_ris               : 1 ;  // 25     
	UINT32 cvd_vs_fal                         : 1 ;  // 26     
	UINT32 cvd_vs_ris                         : 1 ;  // 27     
	UINT32 cvd_ext_locked_fal                 : 1 ;  // 28     
	UINT32 cvd_ext_locked_ris                 : 1 ;  // 29     
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_CVD_INTR1_INTR0_T;

/*-----------------------------------------------------------------------------
		0xc0017108L cvd_intr1_clear0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_clear0                   : 32;  // 31:0   
} H13Ax_CVD_INTR1_CLEAR0_T;

/*-----------------------------------------------------------------------------
		0xc001710cL cvd_intr1_enable1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_enable1                  : 32;  // 31:0   
} H13Ax_CVD_INTR1_ENABLE1_T;

/*-----------------------------------------------------------------------------
		0xc0017110L cvd_intr1_intr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vbi_wss_rdy                        : 1 ;  // 0      
	UINT32 vbi_cc_rdy                         : 1 ;  // 1      
	UINT32 vbi_data_in                        : 1 ;  // 2      
	UINT32 cvd_fb_rst                         : 1 ;  // 3      
	UINT32 cvd_mif_err                        : 1 ;  // 4      
	UINT32 cs_chromalock_fal                  : 1 ;  // 5      
	UINT32 cs_chromalock_ris                  : 1 ;  // 6      
	UINT32 cs_pal_detected_fal                : 1 ;  // 7      
	UINT32 cs_pal_detected_ris                : 1 ;  // 8      
	UINT32 cs_secam_detected_fal              : 1 ;  // 9      
	UINT32 cs_secam_detected_ris              : 1 ;  // 10     
	UINT32 cs1_chromalock_fal                 : 1 ;  // 11     
	UINT32 cs1_chromalock_ris                 : 1 ;  // 12     
	UINT32 cs1_pal_detected_fal               : 1 ;  // 13     
	UINT32 cs1_pal_detected_ris               : 1 ;  // 14     
	UINT32 cs1_secam_detected_fal             : 1 ;  // 15     
	UINT32 cs1_secam_detected_ris             : 1 ;  // 16     
	UINT32 estfrdp_err_flag_fal               : 1 ;  // 17     
	UINT32 estfrdp_err_flag_ris               : 1 ;  // 18     
	UINT32 vdetect_vcount_fal                 : 1 ;  // 19     
	UINT32 vdetect_vcount_ris                 : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_CVD_INTR1_INTR1_T;

/*-----------------------------------------------------------------------------
		0xc0017114L cvd_intr1_clear1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_clear1                   : 32;  // 31:0   
} H13Ax_CVD_INTR1_CLEAR1_T;

/*-----------------------------------------------------------------------------
		0xc0017118L cvd_intr2_enable0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_enable0                  : 32;  // 31:0   
} H13Ax_CVD_INTR2_ENABLE0_T;

/*-----------------------------------------------------------------------------
		0xc001711cL cvd_intr2_intr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_chromalock_fal                 : 1 ;  // 0      
	UINT32 cvd_chromalock_ris                 : 1 ;  // 1      
	UINT32 cvd_vlock_fal                      : 1 ;  // 2      
	UINT32 cvd_vlock_ris                      : 1 ;  // 3      
	UINT32 cvd_hlock_fal                      : 1 ;  // 4      
	UINT32 cvd_hlock_ris                      : 1 ;  // 5      
	UINT32 cvd_no_signal_fal                  : 1 ;  // 6      
	UINT32 cvd_no_signal_ris                  : 1 ;  // 7      
	UINT32 cvd_noburst_detected_fal           : 1 ;  // 8      
	UINT32 cvd_noburst_detected_ris           : 1 ;  // 9      
	UINT32 cvd_vnon_standard_fal              : 1 ;  // 10     
	UINT32 cvd_vnon_standard_ris              : 1 ;  // 11     
	UINT32 cvd_hnon_standard_fal              : 1 ;  // 12     
	UINT32 cvd_hnon_standard_ris              : 1 ;  // 13     
	UINT32 cvd_proscan_detected_fal           : 1 ;  // 14     
	UINT32 cvd_proscan_detected_ris           : 1 ;  // 15     
	UINT32 cvd_vcr_fal                        : 1 ;  // 16     
	UINT32 cvd_vcr_ris                        : 1 ;  // 17     
	UINT32 cvd_noisy_fal                      : 1 ;  // 18     
	UINT32 cvd_noisy_ris                      : 1 ;  // 19     
	UINT32 cvd_vline_625_detected_fal         : 1 ;  // 20     
	UINT32 cvd_vline_625_detected_ris         : 1 ;  // 21     
	UINT32 cvd_secam_detected_fal             : 1 ;  // 22     
	UINT32 cvd_secam_detected_ris             : 1 ;  // 23     
	UINT32 cvd_pal_detected_fal               : 1 ;  // 24     
	UINT32 cvd_pal_detected_ris               : 1 ;  // 25     
	UINT32 cvd_vs_fal                         : 1 ;  // 26     
	UINT32 cvd_vs_ris                         : 1 ;  // 27     
	UINT32 cvd_ext_locked_fal                 : 1 ;  // 28     
	UINT32 cvd_ext_locked_ris                 : 1 ;  // 29     
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_CVD_INTR2_INTR0_T;

/*-----------------------------------------------------------------------------
		0xc0017120L cvd_intr2_clear0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_clear0                   : 32;  // 31:0   
} H13Ax_CVD_INTR2_CLEAR0_T;

/*-----------------------------------------------------------------------------
		0xc0017124L cvd_intr2_enable1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_enable1                  : 32;  // 31:0   
} H13Ax_CVD_INTR2_ENABLE1_T;

/*-----------------------------------------------------------------------------
		0xc0017128L cvd_intr2_intr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vbi_wss_rdy                        : 1 ;  // 0      
	UINT32 vbi_cc_rdy                         : 1 ;  // 1      
	UINT32 vbi_data_in                        : 1 ;  // 2      
	UINT32 cvd_fb_rst                         : 1 ;  // 3      
	UINT32 cvd_mif_err                        : 1 ;  // 4      
	UINT32 cs_chromalock_fal                  : 1 ;  // 5      
	UINT32 cs_chromalock_ris                  : 1 ;  // 6      
	UINT32 cs_pal_detected_fal                : 1 ;  // 7      
	UINT32 cs_pal_detected_ris                : 1 ;  // 8      
	UINT32 cs_secam_detected_fal              : 1 ;  // 9      
	UINT32 cs_secam_detected_ris              : 1 ;  // 10     
	UINT32 cs1_chromalock_fal                 : 1 ;  // 11     
	UINT32 cs1_chromalock_ris                 : 1 ;  // 12     
	UINT32 cs1_pal_detected_fal               : 1 ;  // 13     
	UINT32 cs1_pal_detected_ris               : 1 ;  // 14     
	UINT32 cs1_secam_detected_fal             : 1 ;  // 15     
	UINT32 cs1_secam_detected_ris             : 1 ;  // 16     
	UINT32 estfrdp_err_flag_fal               : 1 ;  // 17     
	UINT32 estfrdp_err_flag_ris               : 1 ;  // 18     
	UINT32                                    : 13;  // 31:19   reserved
} H13Ax_CVD_INTR2_INTR1_T;

/*-----------------------------------------------------------------------------
		0xc001712cL cvd_intr2_clear1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_clear1                   : 32;  // 31:0   
} H13Ax_CVD_INTR2_CLEAR1_T;

/*-----------------------------------------------------------------------------
		0xc0017130L top_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 iris_ver                           : 32;  // 31:0   
} H13Ax_TOP_001_T;

/*-----------------------------------------------------------------------------
		0xc0017140L iris_mif_gmua_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_comb3_buffer_size              : 23;  // 22:0   
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_MIF_GMUA_001_T;

/*-----------------------------------------------------------------------------
		0xc0017144L iris_mif_gmua_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld1_init_rd_pel               : 23;  // 22:0   
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_MIF_GMUA_002_T;

/*-----------------------------------------------------------------------------
		0xc0017148L iris_mif_gmua_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld2_init_rd_pel               : 23;  // 22:0   
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_MIF_GMUA_003_T;

/*-----------------------------------------------------------------------------
		0xc001714cL iris_mif_gmua_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld3_init_rd_pel               : 23;  // 22:0   
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_MIF_GMUA_004_T;

/*-----------------------------------------------------------------------------
		0xc0017150L iris_mif_gmua_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld4_init_rd_pel               : 23;  // 22:0   
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_MIF_GMUA_005_T;

/*-----------------------------------------------------------------------------
		0xc0017154L iris_mif_gmua_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_to_cnt                    : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_gmau_cmd_dly_cnt               : 4 ;  // 15:12  
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_MIF_GMUA_006_T;

/*-----------------------------------------------------------------------------
		0xc0017158L iris_mif_gmua_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_cmd_base                  : 32;  // 31:0   
} H13Ax_IRIS_MIF_GMUA_007_T;

/*-----------------------------------------------------------------------------
		0xc0017160L iris_mif_gmua_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_endian_sw                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_gmau_cmd_pri2                  : 4 ;  // 7:4    
	UINT32 reg_gmau_cmd_pri1                  : 4 ;  // 11:8   
	UINT32                                    : 20;  // 31:12   reserved
} H13Ax_IRIS_MIF_GMUA_008_T;

/*-----------------------------------------------------------------------------
		0xc0017164L iris_mif_gmua_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_stride_size               : 32;  // 31:0   
} H13Ax_IRIS_MIF_GMUA_009_T;

/*-----------------------------------------------------------------------------
		0xc0017168L fastblank_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_latency                     : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_fb_blend_ratio                 : 5 ;  // 12:8   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_fb_lmflag_off                  : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_fb_en                          : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_FASTBLANK_001_T;

/*-----------------------------------------------------------------------------
		0xc001716cL fastblank_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef1                   : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_fb_csc_coef0                   : 15;  // 30:16  
	UINT32                                    : 1 ;  // 31:31   reserved
} H13Ax_FASTBLANK_002_T;

/*-----------------------------------------------------------------------------
		0xc0017170L fastblank_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef3                   : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_fb_csc_coef2                   : 15;  // 30:16  
	UINT32                                    : 1 ;  // 31:31   reserved
} H13Ax_FASTBLANK_003_T;

/*-----------------------------------------------------------------------------
		0xc0017174L fastblank_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef5                   : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_fb_csc_coef4                   : 15;  // 30:16  
	UINT32                                    : 1 ;  // 31:31   reserved
} H13Ax_FASTBLANK_004_T;

/*-----------------------------------------------------------------------------
		0xc0017178L fastblank_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef7                   : 15;  // 14:0   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_fb_csc_coef6                   : 15;  // 30:16  
	UINT32                                    : 1 ;  // 31:31   reserved
} H13Ax_FASTBLANK_005_T;

/*-----------------------------------------------------------------------------
		0xc001717cL fastblank_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef8                   : 15;  // 14:0   
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_FASTBLANK_006_T;

/*-----------------------------------------------------------------------------
		0xc0017180L fastblank_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_ofst1                   : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_fb_csc_ofst0                   : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_FASTBLANK_007_T;

/*-----------------------------------------------------------------------------
		0xc0017184L fastblank_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_ofst3                   : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_fb_csc_ofst2                   : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_FASTBLANK_008_T;

/*-----------------------------------------------------------------------------
		0xc0017188L fastblank_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_vstart_odd                  : 5 ;  // 4:0    
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_fb_csc_ofst5                   : 11;  // 18:8   
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_fb_csc_ofst4                   : 11;  // 30:20  
	UINT32                                    : 1 ;  // 31:31   reserved
} H13Ax_FASTBLANK_009_T;

/*-----------------------------------------------------------------------------
		0xc001718cL fastblank_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_hstart                      : 8 ;  // 7:0    
	UINT32 reg_fb_height_half                 : 8 ;  // 15:8   
	UINT32 reg_fb_vstart_even                 : 5 ;  // 20:16  
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_FASTBLANK_010_T;

/*-----------------------------------------------------------------------------
		0xc0017190L iris_de_ctrl_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_iris_hsout_width               : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_iris_static_de_start           : 12;  // 23:12  
	UINT32 reg_iris_static_de                 : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_DE_CTRL_001_T;

/*-----------------------------------------------------------------------------
		0xc0017194L vbi_ctrl_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_little_endian              : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_vbi_bits_swap                  : 1 ;  // 4      
	UINT32                                    : 27;  // 31:5    reserved
} H13Ax_VBI_CTRL_001_T;

/*-----------------------------------------------------------------------------
		0xc0017198L vbi_ctrl_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi0_start_addr                : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_vbi0_data_cnt                  : 12;  // 23:12  
	UINT32 reg_vbi_buffer_number              : 2 ;  // 25:24  
	UINT32                                    : 6 ;  // 31:26   reserved
} H13Ax_VBI_CTRL_002_T;

/*-----------------------------------------------------------------------------
		0xc001719cL vbi_ctrl_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi0_end_addr                  : 10;  // 9:0    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_VBI_CTRL_003_T;

/*-----------------------------------------------------------------------------
		0xc00171a0L vbi_ctrl_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi1_start_addr                : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_vbi1_data_cnt                  : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_VBI_CTRL_004_T;

/*-----------------------------------------------------------------------------
		0xc00171a4L vbi_ctrl_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi1_end_addr                  : 10;  // 9:0    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_VBI_CTRL_005_T;

/*-----------------------------------------------------------------------------
		0xc00171a8L vbi_ctrl_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi2_start_addr                : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_vbi2_data_cnt                  : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_VBI_CTRL_006_T;

/*-----------------------------------------------------------------------------
		0xc00171acL vbi_ctrl_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi2_end_addr                  : 10;  // 9:0    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_VBI_CTRL_007_T;

/*-----------------------------------------------------------------------------
		0xc00171b0L vbi_ctrl_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi3_start_addr                : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_vbi3_data_cnt                  : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_VBI_CTRL_008_T;

/*-----------------------------------------------------------------------------
		0xc00171b4L vbi_ctrl_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi3_end_addr                  : 10;  // 9:0    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_VBI_CTRL_009_T;

/*-----------------------------------------------------------------------------
		0xc00171b8L vbi_ctrl_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_intr_sel                   : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_vbi_even_vcount                : 10;  // 13:4   
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_vbi_odd_vcount                 : 10;  // 25:16  
	UINT32                                    : 6 ;  // 31:26   reserved
} H13Ax_VBI_CTRL_010_T;

/*-----------------------------------------------------------------------------
		0xc00171c0L fast_blank_status_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_state                       : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_FAST_BLANK_STATUS_001_T;

/*-----------------------------------------------------------------------------
		0xc00171c4L cvbsafe_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cvbs2_gc_int                   : 4 ;  // 3:0    
	UINT32 reg_cvbs1_gc_int                   : 4 ;  // 7:4    
	UINT32 reg_iris_fix_gain                  : 1 ;  // 8      
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_CVBSAFE_001_T;

/*-----------------------------------------------------------------------------
		0xc0017200L iris_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_proscan_detected               : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_mv_colourstripes               : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_mv_vbi_detected                : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_chromalock                     : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_vlock                          : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_hlock                          : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_no_signal                      : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_status_vdetect_vcount_chg      : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_001_T;

/*-----------------------------------------------------------------------------
		0xc0017204L iris_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_noisy                          : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_vline_625_detected             : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_secam_detected                 : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_pal_detected                   : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_status_comb3d_off              : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_noburst_detected               : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_vnon_standard                  : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_hnon_standard                  : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_002_T;

/*-----------------------------------------------------------------------------
		0xc0017208L iris_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vcr_rew                        : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_vcr_ff                         : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_vcr_trick                      : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_vcr                            : 1 ;  // 12     
	UINT32                                    : 19;  // 31:13   reserved
} H13Ax_IRIS_003_T;

/*-----------------------------------------------------------------------------
		0xc001720cL iris_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_burst_mag               : 16;  // 15:0   
	UINT32 reg_status_hsync_width             : 8 ;  // 23:16  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_004_T;

/*-----------------------------------------------------------------------------
		0xc0017210L iris_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_hdto_inc                : 30;  // 29:0   
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_005_T;

/*-----------------------------------------------------------------------------
		0xc0017214L iris_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_cdto_inc                : 30;  // 29:0   
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_006_T;

/*-----------------------------------------------------------------------------
		0xc0017218L iris_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_cmag                    : 8 ;  // 7:0    
	UINT32 reg_status_agc_gain                : 16;  // 23:8   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_007_T;

/*-----------------------------------------------------------------------------
		0xc001721cL iris_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_sync_height             : 8 ;  // 7:0    
	UINT32 reg_status_cordic_freq             : 8 ;  // 15:8   
	UINT32 reg_status_cgain                   : 14;  // 29:16  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_008_T;

/*-----------------------------------------------------------------------------
		0xc0017220L iris_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_nstd                    : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_status_vbi_slicer_lvl          : 8 ;  // 19:12  
	UINT32 reg_status_noise                   : 10;  // 29:20  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_009_T;

/*-----------------------------------------------------------------------------
		0xc0017224L iris_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_comb3d_motion           : 32;  // 31:0   
} H13Ax_IRIS_010_T;

/*-----------------------------------------------------------------------------
		0xc0017228L iris_011 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_cline_flag               : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_status_blank_level_diff        : 12;  // 15:4   
	UINT32 reg_status_sync_level_diff         : 12;  // 27:16  
	UINT32                                    : 4 ;  // 31:28   reserved
} H13Ax_IRIS_011_T;

/*-----------------------------------------------------------------------------
		0xc0017230L iris_012 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cvd_soft_reset                 : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_IRIS_012_T;

/*-----------------------------------------------------------------------------
		0xc0017234L iris_013 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hv_delay                       : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_hpixel                         : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_vline_625                      : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_colour_mode                    : 3 ;  // 14:12  
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_yc_src                         : 1 ;  // 16     
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_013_T;

/*-----------------------------------------------------------------------------
		0xc0017238L iris_014 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cv_inv                         : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cv_src                         : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_luma_notch_bw                  : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_chroma_bw_lo                   : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_chroma_burst5or10              : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_ped                            : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_IRIS_014_T;

/*-----------------------------------------------------------------------------
		0xc001723cL iris_015 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hagc_field_mode                : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_mv_hagc_mode                   : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_dc_clamp_mode                  : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_dagc_en                        : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_agc_half_en                    : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_cagc_en                        : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_hagc_en                        : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_015_T;

/*-----------------------------------------------------------------------------
		0xc0017240L iris_016 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pal60_mode                     : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_ntsc443_mode                   : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_adaptive_3dcomb_mode           : 3 ;  // 10:8   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_colour_trap                    : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_adaptive_mode                  : 3 ;  // 18:16  
	UINT32                                    : 13;  // 31:19   reserved
} H13Ax_IRIS_016_T;

/*-----------------------------------------------------------------------------
		0xc0017244L iris_017 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_noise_thresh                   : 8 ;  // 7:0    
	UINT32 reg_hagc                           : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_017_T;

/*-----------------------------------------------------------------------------
		0xc0017248L iris_018 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_vcr_en                   : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_force_vcr_rew                  : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_force_vcr_ff                   : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_force_vcr_trick                : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_force_vcr                      : 1 ;  // 16     
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_018_T;

/*-----------------------------------------------------------------------------
		0xc001724cL iris_019 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adc_updn_swap                  : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_adc_input_swap                 : 1 ;  // 4      
	UINT32                                    : 27;  // 31:5    reserved
} H13Ax_IRIS_019_T;

/*-----------------------------------------------------------------------------
		0xc0017250L iris_020 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ccir656_en                     : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cbcr_swap                      : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_blue_mode                      : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_yc_delay                       : 4 ;  // 15:12  
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_020_T;

/*-----------------------------------------------------------------------------
		0xc0017254L iris_021 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hue                            : 8 ;  // 7:0    
	UINT32 reg_saturation                     : 8 ;  // 15:8   
	UINT32 reg_brightness                     : 8 ;  // 23:16  
	UINT32 reg_contrast                       : 8 ;  // 31:24  
} H13Ax_IRIS_021_T;

/*-----------------------------------------------------------------------------
		0xc0017258L iris_022 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cagc                           : 8 ;  // 7:0    
	UINT32                                    : 24;  // 31:8    reserved
} H13Ax_IRIS_022_T;

/*-----------------------------------------------------------------------------
		0xc001725cL iris_023 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_user_ckill_mode                : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_vbi_ckill                      : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_hlock_ckill                    : 1 ;  // 8      
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_IRIS_023_T;

/*-----------------------------------------------------------------------------
		0xc0017260L iris_024 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_nstd_hysis                     : 4 ;  // 3:0    
	UINT32 reg_disable_clamp_on_vsync         : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_bypass                         : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_noburst_ckill                  : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_vnon_std_threshold             : 2 ;  // 17:16  
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_hnon_std_threshold             : 6 ;  // 25:20  
	UINT32                                    : 6 ;  // 31:26   reserved
} H13Ax_IRIS_024_T;

/*-----------------------------------------------------------------------------
		0xc0017264L iris_025 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_agc_peak_en                    : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_agc_peak_cntl                  : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_agc_peak_nominal               : 7 ;  // 14:8   
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_025_T;

/*-----------------------------------------------------------------------------
		0xc0017268L iris_026 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_blue_cr                        : 8 ;  // 7:0    
	UINT32 reg_blue_cb                        : 8 ;  // 15:8   
	UINT32 reg_blue_y                         : 8 ;  // 23:16  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_026_T;

/*-----------------------------------------------------------------------------
		0xc001726cL iris_027 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hstate_fixed                   : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_disable_hfine                  : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_hstate_unlocked                : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_hstate_max                     : 3 ;  // 14:12  
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_locked_count_noisy_max         : 4 ;  // 19:16  
	UINT32 reg_locked_count_clean_max         : 4 ;  // 23:20  
	UINT32 reg_hdetect_clamp_level            : 8 ;  // 31:24  
} H13Ax_IRIS_027_T;

/*-----------------------------------------------------------------------------
		0xc0017270L iris_028 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hlock_vsync_mode               : 2 ;  // 1:0    
	UINT32                                    : 30;  // 31:2    reserved
} H13Ax_IRIS_028_T;

/*-----------------------------------------------------------------------------
		0xc0017274L iris_029 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdto_inc                       : 32;  // 31:0   
} H13Ax_IRIS_029_T;

/*-----------------------------------------------------------------------------
		0xc0017278L iris_030 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdto_inc                       : 32;  // 31:0   
} H13Ax_IRIS_030_T;

/*-----------------------------------------------------------------------------
		0xc001727cL iris_031 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_gate_end                 : 8 ;  // 7:0    
	UINT32 reg_hsync_gate_start               : 8 ;  // 15:8   
	UINT32 reg_hsync_phase_offset             : 8 ;  // 23:16  
	UINT32 reg_hsync_rising                   : 8 ;  // 31:24  
} H13Ax_IRIS_031_T;

/*-----------------------------------------------------------------------------
		0xc0017280L iris_032 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_similar                  : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_hsync_low                      : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_hdetect_noise_en               : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_hfine_lt_hcoarse               : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_hlpf_clamp_sel                 : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_hlpf_clamp_noisy_en            : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_hlpf_clamp_vbi_en              : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_hlpf_clamp_en                  : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_032_T;

/*-----------------------------------------------------------------------------
		0xc0017284L iris_033 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_rising_end               : 8 ;  // 7:0    
	UINT32 reg_hsync_rising_auto              : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_hsync_rising_start             : 6 ;  // 17:12  
	UINT32                                    : 14;  // 31:18   reserved
} H13Ax_IRIS_033_T;

/*-----------------------------------------------------------------------------
		0xc0017288L iris_034 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hblank_end                     : 8 ;  // 7:0    
	UINT32 reg_hblank_start                   : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_034_T;

/*-----------------------------------------------------------------------------
		0xc001728cL iris_035 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst_gate_end                 : 8 ;  // 7:0    
	UINT32 reg_burst_gate_start               : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_035_T;

/*-----------------------------------------------------------------------------
		0xc0017290L iris_036 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_width                  : 8 ;  // 7:0    
	UINT32 reg_hactive_start                  : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_036_T;

/*-----------------------------------------------------------------------------
		0xc0017294L iris_037 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactive_height                 : 8 ;  // 7:0    
	UINT32 reg_vactive_start                  : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_037_T;

/*-----------------------------------------------------------------------------
		0xc0017298L iris_038 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_h_max                    : 7 ;  // 6:0    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_vsync_h_min                    : 7 ;  // 14:8   
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_038_T;

/*-----------------------------------------------------------------------------
		0xc001729cL iris_039 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_agc_max                  : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_vsync_agc_min                  : 7 ;  // 14:8   
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_039_T;

/*-----------------------------------------------------------------------------
		0xc00172a0L iris_040 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_vbi_max                  : 7 ;  // 6:0    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_vsync_vbi_min                  : 7 ;  // 14:8   
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_040_T;

/*-----------------------------------------------------------------------------
		0xc00172a4L iris_041 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vlock_wide_range               : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_IRIS_041_T;

/*-----------------------------------------------------------------------------
		0xc00172a8L iris_042 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_proscan_1field_mode            : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_vsync_cntl_noisy               : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_vsync_cntl_ff_rew              : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_vsync_cntl_trick               : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_vsync_cntl_vcr                 : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_vsync_cntl                     : 2 ;  // 21:20  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_042_T;

/*-----------------------------------------------------------------------------
		0xc00172acL iris_043 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_field_polarity                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_flip_field                     : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_veven_delayed                  : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_vodd_delayed                   : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_field_detect_mode              : 2 ;  // 17:16  
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_vloop_tc                       : 2 ;  // 21:20  
	UINT32                                    : 2 ;  // 23:22   reserved
	UINT32 reg_debug_digital                  : 8 ;  // 31:24  
} H13Ax_IRIS_043_T;

/*-----------------------------------------------------------------------------
		0xc00172b0L iris_044 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_syncs                      : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_muxdigital                     : 4 ;  // 7:4    
	UINT32 reg_muxanalogb                     : 4 ;  // 11:8   
	UINT32 reg_muxanaloga                     : 4 ;  // 15:12  
	UINT32 reg_debug_analogb                  : 8 ;  // 23:16  
	UINT32 reg_debug_analoga                  : 8 ;  // 31:24  
} H13Ax_IRIS_044_T;

/*-----------------------------------------------------------------------------
		0xc00172b4L iris_045 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_ybw                      : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_peak_range                     : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_peak_gain                      : 3 ;  // 10:8   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_peak_en                        : 1 ;  // 12     
	UINT32                                    : 19;  // 31:13   reserved
} H13Ax_IRIS_045_T;

/*-----------------------------------------------------------------------------
		0xc00172b8L iris_046 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_auto_secam_level               : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_sv_bf                          : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_secam_pll                      : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_secam_sw_old                   : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_palsw_level                    : 2 ;  // 17:16  
	UINT32                                    : 14;  // 31:18   reserved
} H13Ax_IRIS_046_T;

/*-----------------------------------------------------------------------------
		0xc00172bcL iris_047 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lose_chromalock_mode           : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_lose_chromalock_count          : 4 ;  // 7:4    
	UINT32 reg_lose_chromalock_level          : 3 ;  // 10:8   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_lose_chromalock_ckill          : 1 ;  // 12     
	UINT32                                    : 19;  // 31:13   reserved
} H13Ax_IRIS_047_T;

/*-----------------------------------------------------------------------------
		0xc00172c0L iris_048 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cstripe_detect_control         : 3 ;  // 2:0    
	UINT32                                    : 1 ;  // 3:3     reserved
	UINT32 reg_cdct_demod_sel                 : 2 ;  // 5:4    
	UINT32                                    : 26;  // 31:6    reserved
} H13Ax_IRIS_048_T;

/*-----------------------------------------------------------------------------
		0xc00172c4L iris_049 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hfine_vcr_trick_en             : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_hfine_vcr_en                   : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_hresampler_2up                 : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cstate                         : 3 ;  // 14:12  
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_fixed_cstate                   : 1 ;  // 16     
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_049_T;

/*-----------------------------------------------------------------------------
		0xc00172c8L iris_050 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_pulse_ampl                 : 8 ;  // 7:0    
	UINT32                                    : 24;  // 31:8    reserved
} H13Ax_IRIS_050_T;

/*-----------------------------------------------------------------------------
		0xc00172ccL iris_051 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_delay                    : 8 ;  // 7:0    
	UINT32 reg_cpump_adjust                   : 8 ;  // 15:8   
	UINT32 reg_cpump_adjust_polarity          : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_cpump_adjust_delay             : 6 ;  // 25:20  
	UINT32                                    : 6 ;  // 31:26   reserved
} H13Ax_IRIS_051_T;

/*-----------------------------------------------------------------------------
		0xc00172d0L iris_052 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_colourstripes_sel           : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_mv_vbi_sel                     : 1 ;  // 4      
	UINT32                                    : 27;  // 31:5    reserved
} H13Ax_IRIS_052_T;

/*-----------------------------------------------------------------------------
		0xc00172d4L iris_053 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_kill_cr                  : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cpump_kill_cb                  : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cpump_kill_y                   : 1 ;  // 8      
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_IRIS_053_T;

/*-----------------------------------------------------------------------------
		0xc00172d8L iris_054 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_auto_stip_nobp           : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cpump_auto_stip_unlocked       : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cpump_auto_stip_no_signal      : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cpump_auto_stip_noisy          : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cpump_auto_stip_vactive        : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_cpump_auto_stip_mode           : 2 ;  // 21:20  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_054_T;

/*-----------------------------------------------------------------------------
		0xc00172dcL iris_055 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_accum_mode               : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cpump_fixed_syncmid            : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cpump_level_filter_gain        : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_cpump_noisy_filter_en          : 1 ;  // 12     
	UINT32                                    : 19;  // 31:13   reserved
} H13Ax_IRIS_055_T;

/*-----------------------------------------------------------------------------
		0xc00172e0L iris_056 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_dn_max                   : 7 ;  // 6:0    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_cpump_up_max                   : 7 ;  // 14:8   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_cpump_vsync_blank_filter       : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_cpump_vsync_syncmid_filter     : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_cpump_vsync_mode               : 2 ;  // 25:24  
	UINT32                                    : 6 ;  // 31:26   reserved
} H13Ax_IRIS_056_T;

/*-----------------------------------------------------------------------------
		0xc00172e4L iris_057 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_diff_noisy_only          : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cpump_dn_diff_max              : 7 ;  // 10:4   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_cpump_diff_signal_only         : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cpump_up_diff_max              : 7 ;  // 22:16  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_057_T;

/*-----------------------------------------------------------------------------
		0xc00172e8L iris_058 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_pr_override              : 8 ;  // 7:0    
	UINT32 reg_cpump_pb_override              : 8 ;  // 15:8   
	UINT32 reg_cpump_y_override               : 8 ;  // 23:16  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_058_T;

/*-----------------------------------------------------------------------------
		0xc00172ecL iris_059 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_db_freq                        : 12;  // 11:0   
	UINT32 reg_dr_freq                        : 12;  // 23:12  
	UINT32 reg_cvbs_y_delay                   : 5 ;  // 28:24  
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_059_T;

/*-----------------------------------------------------------------------------
		0xc00172f0L iris_060 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactivity_en                   : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_vactivity_on2frame             : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_vactivity_threshold            : 6 ;  // 13:8   
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_060_T;

/*-----------------------------------------------------------------------------
		0xc00172f4L iris_061 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_horiz_diff_cgain               : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_horiz_diff_ygain               : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_chroma_vdiff_gain              : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_lowfreq_vdiff_gain             : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_noise_th_en                    : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_noise_th                       : 7 ;  // 26:20  
	UINT32                                    : 5 ;  // 31:27   reserved
} H13Ax_IRIS_061_T;

/*-----------------------------------------------------------------------------
		0xc00172f8L iris_062 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vadap_burst_noise_th_gain      : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_burst_noise_th_gain            : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_c_noise_th_gain                : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_y_noise_th_gain                : 2 ;  // 13:12  
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_062_T;

/*-----------------------------------------------------------------------------
		0xc00172fcL iris_063 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adaptive_chroma_mode           : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_dot_suppress_mode              : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_motion_mode                    : 2 ;  // 9:8    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_IRIS_063_T;

/*-----------------------------------------------------------------------------
		0xc0017300L iris_064 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pal3dcomb_vactive_offset       : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_fb_sync                        : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_fb_hold                        : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_fb_ctl                         : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_field_latency                  : 3 ;  // 18:16  
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_lbadrgen_rst                   : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_comb2d_only                    : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_064_T;

/*-----------------------------------------------------------------------------
		0xc0017304L iris_065 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_noise_th_en                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_md_noise_th                    : 7 ;  // 10:4   
	UINT32                                    : 21;  // 31:11   reserved
} H13Ax_IRIS_065_T;

/*-----------------------------------------------------------------------------
		0xc0017308L iris_066 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_schroma_peak_en                : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_schroma_coring_en              : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_schroma_peak                   : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_pchroma_peak_en                : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_pchroma_coring_en              : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_pchroma_peak                   : 2 ;  // 21:20  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_066_T;

/*-----------------------------------------------------------------------------
		0xc001730cL iris_067 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ldpause_threshold              : 4 ;  // 3:0    
	UINT32 reg_vf_nstd_en                     : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_vcr_auto_switch_en             : 1 ;  // 8      
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_IRIS_067_T;

/*-----------------------------------------------------------------------------
		0xc0017310L iris_068 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_comb_coring                    : 4 ;  // 3:0    
	UINT32 reg_tcomb_gain                     : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_notch_gain                     : 3 ;  // 10:8   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_comb_gain                      : 3 ;  // 14:12  
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_068_T;

/*-----------------------------------------------------------------------------
		0xc0017314L iris_069 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactive_md_height              : 8 ;  // 7:0    
	UINT32 reg_vactive_md_start               : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_069_T;

/*-----------------------------------------------------------------------------
		0xc0017318L iris_070 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_pulse_width              : 4 ;  // 3:0    
	UINT32                                    : 28;  // 31:4    reserved
} H13Ax_IRIS_070_T;

/*-----------------------------------------------------------------------------
		0xc001731cL iris_071 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cagc_tc_ismall                 : 3 ;  // 2:0    
	UINT32                                    : 29;  // 31:3    reserved
} H13Ax_IRIS_071_T;

/*-----------------------------------------------------------------------------
		0xc0017320L iris_072 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cagc_coring_threshold          : 4 ;  // 3:0    
	UINT32 reg_cagc_unity_gain                : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cagc_coring                    : 3 ;  // 10:8   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_cagc_tc_p                      : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_cagc_tc_ibig                   : 3 ;  // 18:16  
	UINT32                                    : 13;  // 31:19   reserved
} H13Ax_IRIS_072_T;

/*-----------------------------------------------------------------------------
		0xc0017324L iris_073 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_aaf_on                         : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_IRIS_073_T;

/*-----------------------------------------------------------------------------
		0xc0017328L iris_074 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dcrestore_accum_width          : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_dcrestore_no_bad_bp            : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_dcrestore_kill_enable          : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_dcrestore_bp_delay             : 2 ;  // 17:16  
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_syncmid_nobp_en                : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_syncmid_filter_en              : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_dcrestore_gain                 : 2 ;  // 29:28  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_074_T;

/*-----------------------------------------------------------------------------
		0xc001732cL iris_075 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dcrestore_kill_enable_noisy    : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_sync_height_unclp              : 1 ;  // 4      
	UINT32                                    : 27;  // 31:5    reserved
} H13Ax_IRIS_075_T;

/*-----------------------------------------------------------------------------
		0xc0017330L iris_076 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dcrestore_hsync_mid            : 8 ;  // 7:0    
	UINT32 reg_bp_kill_thresh                 : 8 ;  // 15:8   
	UINT32 reg_hmgc                           : 8 ;  // 23:16  
	UINT32 reg_dcrestore_lpf_en               : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_076_T;

/*-----------------------------------------------------------------------------
		0xc0017334L iris_077 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_auto_min_sync_height           : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_min_sync_height                : 7 ;  // 10:4   
	UINT32                                    : 21;  // 31:11   reserved
} H13Ax_IRIS_077_T;

/*-----------------------------------------------------------------------------
		0xc0017338L iris_078 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vcrtrick_proscan               : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_veven_early_delayed            : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_vodd_early_delayed             : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_vfield_hoffset_fixed           : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_vsync_no_signal_thresh         : 8 ;  // 23:16  
	UINT32 reg_vsync_signal_thresh            : 8 ;  // 31:24  
} H13Ax_IRIS_078_T;

/*-----------------------------------------------------------------------------
		0xc001733cL iris_079 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vfield_hoffset                 : 9 ;  // 8:0    
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_vactive_half_lines             : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_vdetect_noise_en               : 1 ;  // 16     
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_079_T;

/*-----------------------------------------------------------------------------
		0xc0017340L iris_080 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dual_hedge_dis                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_dual_hedge_auto_width          : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_dual_fine_hedge_vbi            : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_dual_coarse_hedge_vbi          : 2 ;  // 13:12  
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_080_T;

/*-----------------------------------------------------------------------------
		0xc0017344L iris_081 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_hsync_rising_end            : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_vcr_state2_long                : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_slow_hdsw                      : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_mv_hsync_rising_start          : 6 ;  // 21:16  
	UINT32                                    : 2 ;  // 23:22   reserved
	UINT32 reg_no_hsyncs_mode                 : 2 ;  // 25:24  
	UINT32                                    : 2 ;  // 27:26   reserved
	UINT32 reg_many_hsyncs_mode               : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_081_T;

/*-----------------------------------------------------------------------------
		0xc0017348L iris_082 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_no_hsyncs_weak                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_disable_hdsw_weak              : 1 ;  // 4      
	UINT32                                    : 27;  // 31:5    reserved
} H13Ax_IRIS_082_T;

/*-----------------------------------------------------------------------------
		0xc001734cL iris_083 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactive_hdsw_mode              : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_disable_hdsw_mode              : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_hsync_falling_filter           : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_no_hsyncs_noisy                : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_hloop_range                    : 2 ;  // 17:16  
	UINT32                                    : 14;  // 31:18   reserved
} H13Ax_IRIS_083_T;

/*-----------------------------------------------------------------------------
		0xc0017350L iris_084 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_hf_max                      : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_md_hf_shift                    : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_md_lf_sad                      : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_md_lf_shift                    : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_md_c_noise_th_en               : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_md_c_noise_th                  : 7 ;  // 26:20  
	UINT32                                    : 5 ;  // 31:27   reserved
} H13Ax_IRIS_084_T;

/*-----------------------------------------------------------------------------
		0xc0017354L iris_085 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_cf_cactivity_high           : 4 ;  // 3:0    
	UINT32 reg_md_cf_lactivity_high           : 8 ;  // 11:4   
	UINT32 reg_md_cf_cactivity_low            : 8 ;  // 19:12  
	UINT32 reg_md_cf_lactivity_low            : 8 ;  // 27:20  
	UINT32 reg_md_cf_activity_enable          : 2 ;  // 29:28  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_085_T;

/*-----------------------------------------------------------------------------
		0xc0017358L iris_086 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_k_thresh                    : 8 ;  // 7:0    
	UINT32                                    : 24;  // 31:8    reserved
} H13Ax_IRIS_086_T;

/*-----------------------------------------------------------------------------
		0xc001735cL iris_087 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tcomb_chroma_level             : 8 ;  // 7:0    
	UINT32 reg_hf_luma_chroma_offset          : 8 ;  // 15:8   
	UINT32 reg_spatial_luma_level             : 8 ;  // 23:16  
	UINT32 reg_chroma_level                   : 8 ;  // 31:24  
} H13Ax_IRIS_087_T;

/*-----------------------------------------------------------------------------
		0xc0017360L iris_088 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_chroma_activity_level          : 8 ;  // 7:0    
	UINT32 reg_lf_luma_offset                 : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_088_T;

/*-----------------------------------------------------------------------------
		0xc0017364L iris_089 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_avg_freq_range                 : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_issecam_th                     : 6 ;  // 9:4    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_freq_offset_range              : 8 ;  // 19:12  
	UINT32                                    : 12;  // 31:20   reserved
} H13Ax_IRIS_089_T;

/*-----------------------------------------------------------------------------
		0xc0017368L iris_090 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_chroma_bw_motion_th            : 8 ;  // 7:0    
	UINT32 reg_motion_config                  : 8 ;  // 15:8   
	UINT32 reg_hactive_md_width               : 8 ;  // 23:16  
	UINT32 reg_hactive_md_start               : 8 ;  // 31:24  
} H13Ax_IRIS_090_T;

/*-----------------------------------------------------------------------------
		0xc001736cL iris_091 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_motion_mode             : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_chroma_bw_motion               : 2 ;  // 5:4    
	UINT32                                    : 26;  // 31:6    reserved
} H13Ax_IRIS_091_T;

/*-----------------------------------------------------------------------------
		0xc0017370L iris_092 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flat_chroma_offset             : 8 ;  // 7:0    
	UINT32 reg_flat_luma_offset               : 8 ;  // 15:8   
	UINT32 reg_flat_chroma_shift              : 2 ;  // 17:16  
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_flat_luma_shift                : 2 ;  // 21:20  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_092_T;

/*-----------------------------------------------------------------------------
		0xc0017374L iris_093 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_debug                   : 8 ;  // 7:0    
	UINT32 reg_cf_flat_motion_shift           : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_motion_c_mode                  : 2 ;  // 13:12  
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_093_T;

/*-----------------------------------------------------------------------------
		0xc0017378L iris_094 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cordic_gate_end                : 8 ;  // 7:0    
	UINT32 reg_cordic_gate_start              : 8 ;  // 15:8   
	UINT32 reg_ispal_th                       : 8 ;  // 23:16  
	UINT32 reg_phase_offset_range             : 8 ;  // 31:24  
} H13Ax_IRIS_094_T;

/*-----------------------------------------------------------------------------
		0xc001737cL iris_095 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pal3tap_only_c                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_pal3tap_only_y                 : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_adc_cpump_swap                 : 6 ;  // 13:8   
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_095_T;

/*-----------------------------------------------------------------------------
		0xc0017380L iris_096 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ckill                          : 16;  // 15:0   
	UINT32 reg_cagc_gate_end                  : 8 ;  // 23:16  
	UINT32 reg_cagc_gate_start                : 8 ;  // 31:24  
} H13Ax_IRIS_096_T;

/*-----------------------------------------------------------------------------
		0xc0017384L iris_097 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_icburst_half_ampl              : 10;  // 9:0    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_IRIS_097_T;

/*-----------------------------------------------------------------------------
		0xc0017388L iris_098 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_agc_bypass                     : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_IRIS_098_T;

/*-----------------------------------------------------------------------------
		0xc001738cL iris_099 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_clampagc_inv                   : 4 ;  // 3:0    
	UINT32                                    : 28;  // 31:4    reserved
} H13Ax_IRIS_099_T;

/*-----------------------------------------------------------------------------
		0xc0017390L iris_100 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_slicer_mode                : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_wss_slicer_mode                : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_tt_slicer_mode                 : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_cc_slicer_mode                 : 2 ;  // 13:12  
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_100_T;

/*-----------------------------------------------------------------------------
		0xc0017394L iris_101 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_perr_on                        : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_IRIS_101_T;

/*-----------------------------------------------------------------------------
		0xc0017398L iris_102 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_perr_dnref                     : 32;  // 31:0   
} H13Ax_IRIS_102_T;

/*-----------------------------------------------------------------------------
		0xc001739cL iris_103 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff0               : 13;  // 12:0   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cres_lpfa_on                   : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_iris_cresampler_on             : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_IRIS_103_T;

/*-----------------------------------------------------------------------------
		0xc00173a0L iris_104 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff2               : 13;  // 12:0   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cres_lpfa_coeff1               : 13;  // 28:16  
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_104_T;

/*-----------------------------------------------------------------------------
		0xc00173a4L iris_105 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff4               : 13;  // 12:0   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cres_lpfa_coeff3               : 13;  // 28:16  
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_105_T;

/*-----------------------------------------------------------------------------
		0xc00173a8L iris_106 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff5               : 13;  // 12:0   
	UINT32                                    : 19;  // 31:13   reserved
} H13Ax_IRIS_106_T;

/*-----------------------------------------------------------------------------
		0xc00173acL iris_107 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfb_coeff0               : 13;  // 12:0   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cres_lpfb_on                   : 1 ;  // 16     
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_107_T;

/*-----------------------------------------------------------------------------
		0xc00173b0L iris_108 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfb_coeff2               : 13;  // 12:0   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cres_lpfb_coeff1               : 13;  // 28:16  
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_108_T;

/*-----------------------------------------------------------------------------
		0xc00173b4L iris_109 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfb_coeff4               : 13;  // 12:0   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cres_lpfb_coeff3               : 13;  // 28:16  
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_109_T;

/*-----------------------------------------------------------------------------
		0xc00173b8L iris_110 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_sel                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cres_blend_method              : 3 ;  // 6:4    
	UINT32                                    : 25;  // 31:7    reserved
} H13Ax_IRIS_110_T;

/*-----------------------------------------------------------------------------
		0xc00173bcL iris_111 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcdefault           : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_cres_0px_sel                   : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cres_clksel                    : 2 ;  // 17:16  
	UINT32                                    : 14;  // 31:18   reserved
} H13Ax_IRIS_111_T;

/*-----------------------------------------------------------------------------
		0xc00173c0L iris_112 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara1             : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_cres_blend_bcpara0             : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_112_T;

/*-----------------------------------------------------------------------------
		0xc00173c4L iris_113 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara3             : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_cres_blend_bcpara2             : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_113_T;

/*-----------------------------------------------------------------------------
		0xc00173c8L iris_114 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara5             : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_cres_blend_bcpara4             : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_114_T;

/*-----------------------------------------------------------------------------
		0xc00173ccL iris_115 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara7             : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_cres_blend_bcpara6             : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_115_T;

/*-----------------------------------------------------------------------------
		0xc00173d0L iris_116 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_diffth1             : 12;  // 11:0   
	UINT32 reg_cres_blend_diffth0             : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_116_T;

/*-----------------------------------------------------------------------------
		0xc00173d4L iris_117 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_alpha_off                 : 8 ;  // 7:0    
	UINT32 reg_cres_blend_diffth2             : 12;  // 19:8   
	UINT32                                    : 12;  // 31:20   reserved
} H13Ax_IRIS_117_T;

/*-----------------------------------------------------------------------------
		0xc00173d8L iris_118 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrscbld_dnoff                  : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_hrscbld_upoff                  : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_hrsybld_dnoff                  : 3 ;  // 10:8   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_hrsybld_upoff                  : 4 ;  // 15:12  
	UINT32 reg_dtrs_dlysel                    : 4 ;  // 19:16  
	UINT32 reg_dtrs_revalpha_sel              : 2 ;  // 21:20  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_118_T;

/*-----------------------------------------------------------------------------
		0xc00173dcL iris_119 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_y_offo                    : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_oadj_y_offi                    : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_119_T;

/*-----------------------------------------------------------------------------
		0xc00173e0L iris_120 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_y_coeff                   : 14;  // 13:0   
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_120_T;

/*-----------------------------------------------------------------------------
		0xc00173e4L iris_121 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_c_offo                    : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_oadj_c_offi                    : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_121_T;

/*-----------------------------------------------------------------------------
		0xc00173e8L iris_122 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_c_coeff                   : 14;  // 13:0   
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_oadj_cr_coeff                  : 14;  // 29:16  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_122_T;

/*-----------------------------------------------------------------------------
		0xc00173ecL iris_123 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adc_unity_gain_value           : 4 ;  // 3:0    
	UINT32 reg_oadj_cr_offo                   : 11;  // 14:4   
	UINT32                                    : 1 ;  // 15:15   reserved
	UINT32 reg_oadj_cr_offi                   : 11;  // 26:16  
	UINT32                                    : 5 ;  // 31:27   reserved
} H13Ax_IRIS_123_T;

/*-----------------------------------------------------------------------------
		0xc00173f0L iris_124 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_frame_motion_th1               : 16;  // 15:0   
	UINT32 reg_frame_motion_th0               : 16;  // 31:16  
} H13Ax_IRIS_124_T;

/*-----------------------------------------------------------------------------
		0xc00173f4L iris_125 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_iblank_half_lvl0               : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_isynctip_lvl0                  : 10;  // 21:12  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_125_T;

/*-----------------------------------------------------------------------------
		0xc00173f8L iris_126 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sync_int_blank_lvl0            : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_ipeak_chroma_m768_lvl0         : 10;  // 21:12  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_126_T;

/*-----------------------------------------------------------------------------
		0xc00173fcL iris_127 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sync_int_blank_vid_lvl0        : 8 ;  // 7:0    
	UINT32 reg_int_black_ped_vid_lvl0         : 10;  // 17:8   
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_int_blank_vid_lvl0             : 10;  // 29:20  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_127_T;

/*-----------------------------------------------------------------------------
		0xc0017400L iris_128 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_int_vbi_blank_vid_lvl0         : 8 ;  // 7:0    
	UINT32                                    : 24;  // 31:8    reserved
} H13Ax_IRIS_128_T;

/*-----------------------------------------------------------------------------
		0xc0017404L iris_129 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_internal_sync_midpt            : 13;  // 12:0   
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_internal_sync_level            : 13;  // 28:16  
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_129_T;

/*-----------------------------------------------------------------------------
		0xc0017408L iris_130 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sync_internal_sync_midpt       : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_internal_blank_level           : 13;  // 24:12  
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_130_T;

/*-----------------------------------------------------------------------------
		0xc001740cL iris_131 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adc_blank_level                : 10;  // 9:0    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_IRIS_131_T;

/*-----------------------------------------------------------------------------
		0xc0017410L iris_132 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_alpha_croff                : 8 ;  // 7:0    
	UINT32 reg_hrs_alpha_cboff                : 8 ;  // 15:8   
	UINT32 reg_hrs_alpha_yoff                 : 8 ;  // 23:16  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_132_T;

/*-----------------------------------------------------------------------------
		0xc0017414L iris_133 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cphase_adjust                  : 3 ;  // 2:0    
	UINT32                                    : 1 ;  // 3:3     reserved
	UINT32 reg_cphase_adjust_sel              : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cdct_actv_sel                  : 2 ;  // 9:8    
	UINT32                                    : 22;  // 31:10   reserved
} H13Ax_IRIS_133_T;

/*-----------------------------------------------------------------------------
		0xc0017418L iris_134 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adap_clf_mode                  : 3 ;  // 2:0    
	UINT32                                    : 1 ;  // 3:3     reserved
	UINT32 reg_cb_delay                       : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_yc444                          : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_ycadj_cr_p                     : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_ycadj_dly                      : 6 ;  // 21:16  
	UINT32                                    : 2 ;  // 23:22   reserved
	UINT32 reg_oadj_swap                      : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_134_T;

/*-----------------------------------------------------------------------------
		0xc001741cL iris_135 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_0                    : 16;  // 15:0   
	UINT32 reg_lpf_0_en                       : 1 ;  // 16     
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_135_T;

/*-----------------------------------------------------------------------------
		0xc0017420L iris_136 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_2                    : 16;  // 15:0   
	UINT32 reg_lpf_tap_0_1                    : 16;  // 31:16  
} H13Ax_IRIS_136_T;

/*-----------------------------------------------------------------------------
		0xc0017424L iris_137 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_4                    : 16;  // 15:0   
	UINT32 reg_lpf_tap_0_3                    : 16;  // 31:16  
} H13Ax_IRIS_137_T;

/*-----------------------------------------------------------------------------
		0xc0017428L iris_138 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_6                    : 16;  // 15:0   
	UINT32 reg_lpf_tap_0_5                    : 16;  // 31:16  
} H13Ax_IRIS_138_T;

/*-----------------------------------------------------------------------------
		0xc001742cL iris_139 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_missing_max                 : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_hs_match_max                   : 6 ;  // 13:8   
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_hs_cnt_mode                    : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_hs_sync_mode                   : 3 ;  // 22:20  
	UINT32                                    : 1 ;  // 23:23   reserved
	UINT32 reg_status_pseudo_sync             : 4 ;  // 27:24  
	UINT32                                    : 4 ;  // 31:28   reserved
} H13Ax_IRIS_139_T;

/*-----------------------------------------------------------------------------
		0xc0017430L iris_140 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_140_T;

/*-----------------------------------------------------------------------------
		0xc0017434L iris_141 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_141_T;

/*-----------------------------------------------------------------------------
		0xc0017438L iris_142 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_pos_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_142_T;

/*-----------------------------------------------------------------------------
		0xc001743cL iris_143 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_pos_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_143_T;

/*-----------------------------------------------------------------------------
		0xc0017440L iris_144 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_144_T;

/*-----------------------------------------------------------------------------
		0xc0017444L iris_145 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_th_1             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_145_T;

/*-----------------------------------------------------------------------------
		0xc0017448L iris_146 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_pos_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_146_T;

/*-----------------------------------------------------------------------------
		0xc001744cL iris_147 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_cnt_mode                    : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_vs_sync_mode                   : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_hs_length_pos_th_1             : 24;  // 31:8   
} H13Ax_IRIS_147_T;

/*-----------------------------------------------------------------------------
		0xc0017450L iris_148 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_missing_max                 : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_vs_match_max                   : 6 ;  // 13:8   
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_148_T;

/*-----------------------------------------------------------------------------
		0xc0017454L iris_149 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_149_T;

/*-----------------------------------------------------------------------------
		0xc0017458L iris_150 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_150_T;

/*-----------------------------------------------------------------------------
		0xc001745cL iris_151 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_pos_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_151_T;

/*-----------------------------------------------------------------------------
		0xc0017460L iris_152 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_pos_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_152_T;

/*-----------------------------------------------------------------------------
		0xc0017464L iris_153 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_153_T;

/*-----------------------------------------------------------------------------
		0xc0017468L iris_154 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_th_1             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_154_T;

/*-----------------------------------------------------------------------------
		0xc001746cL iris_155 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_pos_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_155_T;

/*-----------------------------------------------------------------------------
		0xc0017470L iris_156 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_cnt_mode                    : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_ha_sync_mode                   : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_vs_length_pos_th_1             : 24;  // 31:8   
} H13Ax_IRIS_156_T;

/*-----------------------------------------------------------------------------
		0xc0017474L iris_157 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_missing_max                 : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_ha_match_max                   : 6 ;  // 13:8   
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_157_T;

/*-----------------------------------------------------------------------------
		0xc0017478L iris_158 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_158_T;

/*-----------------------------------------------------------------------------
		0xc001747cL iris_159 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_159_T;

/*-----------------------------------------------------------------------------
		0xc0017480L iris_160 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_pos_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_160_T;

/*-----------------------------------------------------------------------------
		0xc0017484L iris_161 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_pos_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_161_T;

/*-----------------------------------------------------------------------------
		0xc0017488L iris_162 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_162_T;

/*-----------------------------------------------------------------------------
		0xc001748cL iris_163 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_th_1             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_163_T;

/*-----------------------------------------------------------------------------
		0xc0017490L iris_164 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_pos_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_164_T;

/*-----------------------------------------------------------------------------
		0xc0017494L iris_165 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_cnt_mode                    : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_va_sync_mode                   : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_ha_length_pos_th_1             : 24;  // 31:8   
} H13Ax_IRIS_165_T;

/*-----------------------------------------------------------------------------
		0xc0017498L iris_166 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_missing_max                 : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_va_match_max                   : 6 ;  // 13:8   
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_IRIS_166_T;

/*-----------------------------------------------------------------------------
		0xc001749cL iris_167 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_167_T;

/*-----------------------------------------------------------------------------
		0xc00174a0L iris_168 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_168_T;

/*-----------------------------------------------------------------------------
		0xc00174a4L iris_169 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_0                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_169_T;

/*-----------------------------------------------------------------------------
		0xc00174a8L iris_170 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_th_0             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_170_T;

/*-----------------------------------------------------------------------------
		0xc00174acL iris_171 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_171_T;

/*-----------------------------------------------------------------------------
		0xc00174b0L iris_172 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_th_1             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_172_T;

/*-----------------------------------------------------------------------------
		0xc00174b4L iris_173 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_1                : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_173_T;

/*-----------------------------------------------------------------------------
		0xc00174b8L iris_174 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_th_1             : 24;  // 23:0   
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_174_T;

/*-----------------------------------------------------------------------------
		0xc00174bcL iris_175 ''
-----------------------------------------------------------------------------*/
	typedef struct {
//	UINT32 reg_sw_rst_blk                     : 8 ;  // 7:00   
	UINT32 swrst_cdct						: 1; //	0:0				
	UINT32 swrst_hdct						: 1; //	1:1				
	UINT32 iris_clampagc_v2					: 1; //	2:2				
	UINT32 									: 4; //	6:3				
	UINT32 iris_mif_gmau					: 1; //	7:7				
	UINT32                                    : 24;  // 31:8    reserved
} H13Ax_IRIS_175_T;


/*-----------------------------------------------------------------------------
		0xc00174c0L iris_176 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_cenable_f0cnt           : 20;  // 19:0   
	UINT32 reg_status_cenable_ctrl            : 7 ;  // 26:20  
	UINT32                                    : 5 ;  // 31:27   reserved
} H13Ax_IRIS_176_T;

/*-----------------------------------------------------------------------------
		0xc00174c4L iris_177 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_cenable_lcnt            : 12;  // 11:0   
	UINT32 reg_status_cenable_f1cnt           : 20;  // 31:12  
} H13Ax_IRIS_177_T;

/*-----------------------------------------------------------------------------
		0xc00174c8L iris_178 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_updn_rst                : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_clampagc_updn                  : 5 ;  // 8:4    
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_IRIS_178_T;

/*-----------------------------------------------------------------------------
		0xc00174ccL iris_179 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_updn                    : 32;  // 31:0   
} H13Ax_IRIS_179_T;

/*-----------------------------------------------------------------------------
		0xc00174d0L iris_180 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_secam_pll                   : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs_secam_sw_old                : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cs_auto_secam_level            : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs_chroma_activity_level       : 8 ;  // 19:12  
	UINT32 reg_cs_adaptive_chroma_mode        : 2 ;  // 21:20  
	UINT32                                    : 2 ;  // 23:22   reserved
	UINT32 reg_cs_sw_rst                      : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_180_T;

/*-----------------------------------------------------------------------------
		0xc00174d4L iris_181 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_phase_offset_range          : 8 ;  // 7:0    
	UINT32 reg_cs_avg_freq_range              : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_cs_issecam_th                  : 6 ;  // 17:12  
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_cs_freq_offset_range           : 8 ;  // 27:20  
	UINT32                                    : 4 ;  // 31:28   reserved
} H13Ax_IRIS_181_T;

/*-----------------------------------------------------------------------------
		0xc00174d8L iris_182 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_db_freq                     : 12;  // 11:0   
	UINT32 reg_cs_dr_freq                     : 12;  // 23:12  
	UINT32 reg_cs_ispal_th                    : 8 ;  // 31:24  
} H13Ax_IRIS_182_T;

/*-----------------------------------------------------------------------------
		0xc00174dcL iris_183 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cagc_en                     : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs_chroma_burst5or10           : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cs_pal60_mode                  : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs_ntsc443_mode                : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cs_colour_mode                 : 3 ;  // 18:16  
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_cs_adaptive_mode               : 3 ;  // 22:20  
	UINT32                                    : 1 ;  // 23:23   reserved
	UINT32 reg_cs_secam_ybw                   : 2 ;  // 25:24  
	UINT32                                    : 6 ;  // 31:26   reserved
} H13Ax_IRIS_183_T;

/*-----------------------------------------------------------------------------
		0xc00174e0L iris_184 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cagc_coring_threshold       : 4 ;  // 3:0    
	UINT32 reg_cs_cagc_coring                 : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_cs_cagc_unity_gain             : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs_cagc_tc_p                   : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_cs_cagc_tc_ibig                : 3 ;  // 18:16  
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_cs_cagc_tc_ismall              : 3 ;  // 22:20  
	UINT32                                    : 1 ;  // 23:23   reserved
	UINT32 reg_cs_cagc                        : 8 ;  // 31:24  
} H13Ax_IRIS_184_T;

/*-----------------------------------------------------------------------------
		0xc00174e4L iris_185 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_chroma_bw_lo                : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_cs_ckill                       : 16;  // 19:4   
	UINT32 reg_cs_hlock_ckill                 : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_cs_vbi_ckill                   : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_cs_noburst_ckill               : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_185_T;

/*-----------------------------------------------------------------------------
		0xc00174e8L iris_186 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cdto_inc                    : 32;  // 31:0   
} H13Ax_IRIS_186_T;

/*-----------------------------------------------------------------------------
		0xc00174ecL iris_187 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cstate                      : 3 ;  // 2:0    
	UINT32                                    : 1 ;  // 3:3     reserved
	UINT32 reg_cs_fixed_cstate                : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cs_user_ckill_mode             : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_cs_lose_chromalock_mode        : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_cs_lose_chromalock_count       : 4 ;  // 19:16  
	UINT32 reg_cs_lose_chromalock_level       : 3 ;  // 22:20  
	UINT32                                    : 1 ;  // 23:23   reserved
	UINT32 reg_cs_lose_chromalock_ckill       : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_cs_palsw_level                 : 2 ;  // 29:28  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_187_T;

/*-----------------------------------------------------------------------------
		0xc00174f0L iris_188 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_chroma_sel                  : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs_cphase_adjust               : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_cs_cphase_adjust_sel           : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs_mv_colourstripes_sel        : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cs_cstripe_detect_control      : 3 ;  // 18:16  
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_cs_cdct_demod_sel              : 2 ;  // 21:20  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_188_T;

/*-----------------------------------------------------------------------------
		0xc00174f4L iris_189 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_secam_detected              : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs_mv_colourstripes            : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_cs_status_cordic_freq          : 8 ;  // 15:8   
	UINT32 reg_cs_status_burst_mag            : 16;  // 31:16  
} H13Ax_IRIS_189_T;

/*-----------------------------------------------------------------------------
		0xc00174f8L iris_190 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_chromalock                  : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs_status_cgain                : 14;  // 17:4   
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_cs_status_cmag                 : 8 ;  // 27:20  
	UINT32 reg_cs_pal_detected                : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_190_T;

/*-----------------------------------------------------------------------------
		0xc00174fcL iris_191 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_secam_pll                  : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs1_secam_sw_old               : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cs1_auto_secam_level           : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs1_chroma_activity_level      : 8 ;  // 19:12  
	UINT32 reg_cs1_adaptive_chroma_mode       : 2 ;  // 21:20  
	UINT32                                    : 2 ;  // 23:22   reserved
	UINT32 reg_cs1_sw_rst                     : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_IRIS_191_T;

/*-----------------------------------------------------------------------------
		0xc0017500L iris_192 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_phase_offset_range         : 8 ;  // 7:0    
	UINT32 reg_cs1_avg_freq_range             : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_cs1_issecam_th                 : 6 ;  // 17:12  
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_cs1_freq_offset_range          : 8 ;  // 27:20  
	UINT32                                    : 4 ;  // 31:28   reserved
} H13Ax_IRIS_192_T;

/*-----------------------------------------------------------------------------
		0xc0017504L iris_193 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_db_freq                    : 12;  // 11:0   
	UINT32 reg_cs1_dr_freq                    : 12;  // 23:12  
	UINT32 reg_cs1_ispal_th                   : 8 ;  // 31:24  
} H13Ax_IRIS_193_T;

/*-----------------------------------------------------------------------------
		0xc0017508L iris_194 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cagc_en                    : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs1_chroma_burst5or10          : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cs1_pal60_mode                 : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs1_ntsc443_mode               : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cs1_colour_mode                : 3 ;  // 18:16  
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_cs1_adaptive_mode              : 3 ;  // 22:20  
	UINT32                                    : 1 ;  // 23:23   reserved
	UINT32 reg_cs1_secam_ybw                  : 2 ;  // 25:24  
	UINT32                                    : 6 ;  // 31:26   reserved
} H13Ax_IRIS_194_T;

/*-----------------------------------------------------------------------------
		0xc001750cL iris_195 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cagc_coring_threshold      : 4 ;  // 3:0    
	UINT32 reg_cs1_cagc_coring                : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_cs1_cagc_unity_gain            : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs1_cagc_tc_p                  : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_cs1_cagc_tc_ibig               : 3 ;  // 18:16  
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_cs1_cagc_tc_ismall             : 3 ;  // 22:20  
	UINT32                                    : 1 ;  // 23:23   reserved
	UINT32 reg_cs1_cagc                       : 8 ;  // 31:24  
} H13Ax_IRIS_195_T;

/*-----------------------------------------------------------------------------
		0xc0017510L iris_196 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_chroma_bw_lo               : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_cs1_ckill                      : 16;  // 19:4   
	UINT32 reg_cs1_hlock_ckill                : 1 ;  // 20     
	UINT32                                    : 3 ;  // 23:21   reserved
	UINT32 reg_cs1_vbi_ckill                  : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_cs1_noburst_ckill              : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_196_T;

/*-----------------------------------------------------------------------------
		0xc0017514L iris_197 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cdto_inc                   : 32;  // 31:0   
} H13Ax_IRIS_197_T;

/*-----------------------------------------------------------------------------
		0xc0017518L iris_198 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cstate                     : 3 ;  // 2:0    
	UINT32                                    : 1 ;  // 3:3     reserved
	UINT32 reg_cs1_fixed_cstate               : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_cs1_user_ckill_mode            : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_cs1_lose_chromalock_mode       : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_cs1_lose_chromalock_count      : 4 ;  // 19:16  
	UINT32 reg_cs1_lose_chromalock_level      : 3 ;  // 22:20  
	UINT32                                    : 1 ;  // 23:23   reserved
	UINT32 reg_cs1_lose_chromalock_ckill      : 1 ;  // 24     
	UINT32                                    : 3 ;  // 27:25   reserved
	UINT32 reg_cs1_palsw_level                : 2 ;  // 29:28  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_IRIS_198_T;

/*-----------------------------------------------------------------------------
		0xc001751cL iris_199 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_chroma_sel                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs1_cphase_adjust              : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_cs1_cphase_adjust_sel          : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_cs1_mv_colourstripes_sel       : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_cs1_cstripe_detect_control     : 3 ;  // 18:16  
	UINT32                                    : 1 ;  // 19:19   reserved
	UINT32 reg_cs1_cdct_demod_sel             : 2 ;  // 21:20  
	UINT32                                    : 10;  // 31:22   reserved
} H13Ax_IRIS_199_T;

/*-----------------------------------------------------------------------------
		0xc0017520L iris_200 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_secam_detected             : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs1_mv_colourstripes           : 3 ;  // 6:4    
	UINT32                                    : 1 ;  // 7:7     reserved
	UINT32 reg_cs1_status_cordic_freq         : 8 ;  // 15:8   
	UINT32 reg_cs1_status_burst_mag           : 16;  // 31:16  
} H13Ax_IRIS_200_T;

/*-----------------------------------------------------------------------------
		0xc0017524L iris_201 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_chromalock                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cs1_status_cgain               : 14;  // 17:4   
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_cs1_status_cmag                : 8 ;  // 27:20  
	UINT32 reg_cs1_pal_detected               : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_201_T;

/*-----------------------------------------------------------------------------
		0xc0017528L iris_202 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_ctrl                   : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_estfrdp_on                     : 1 ;  // 8      
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_IRIS_202_T;

/*-----------------------------------------------------------------------------
		0xc001752cL iris_203 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_tgt_cf0cnt             : 20;  // 19:0   
	UINT32                                    : 12;  // 31:20   reserved
} H13Ax_IRIS_203_T;

/*-----------------------------------------------------------------------------
		0xc0017530L iris_204 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_tgt_cf1cnt             : 20;  // 19:0   
	UINT32                                    : 12;  // 31:20   reserved
} H13Ax_IRIS_204_T;

/*-----------------------------------------------------------------------------
		0xc0017534L iris_205 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_err_win1_u             : 8 ;  // 7:0    
	UINT32 reg_estfrdp_err_win1_l             : 8 ;  // 15:8   
	UINT32 reg_estfrdp_err_win0_u             : 8 ;  // 23:16  
	UINT32 reg_estfrdp_err_win0_l             : 8 ;  // 31:24  
} H13Ax_IRIS_205_T;

/*-----------------------------------------------------------------------------
		0xc0017538L iris_206 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_err_flag_th            : 8 ;  // 7:0    
	UINT32                                    : 24;  // 31:8    reserved
} H13Ax_IRIS_206_T;

/*-----------------------------------------------------------------------------
		0xc001753cL iris_207 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_err                    : 8 ;  // 7:0    
	UINT32 reg_estfrdp_err_flag               : 1 ;  // 8      
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_IRIS_207_T;

/*-----------------------------------------------------------------------------
		0xc0017540L iris_208 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_width                    : 4 ;  // 3:0    
	UINT32                                    : 28;  // 31:4    reserved
} H13Ax_IRIS_208_T;

/*-----------------------------------------------------------------------------
		0xc0017544L iris_209 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl0                 : 32;  // 31:0   
} H13Ax_IRIS_209_T;

/*-----------------------------------------------------------------------------
		0xc0017548L iris_210 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl1                 : 32;  // 31:0   
} H13Ax_IRIS_210_T;

/*-----------------------------------------------------------------------------
		0xc001754cL iris_211 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl2                 : 32;  // 31:0   
} H13Ax_IRIS_211_T;

/*-----------------------------------------------------------------------------
		0xc0017550L iris_212 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl3                 : 32;  // 31:0   
} H13Ax_IRIS_212_T;

/*-----------------------------------------------------------------------------
		0xc0017554L iris_213 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl4                 : 32;  // 31:0   
} H13Ax_IRIS_213_T;

/*-----------------------------------------------------------------------------
		0xc0017558L iris_214 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl5                 : 32;  // 31:0   
} H13Ax_IRIS_214_T;

/*-----------------------------------------------------------------------------
		0xc001755cL iris_215 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl6                 : 32;  // 31:0   
} H13Ax_IRIS_215_T;

/*-----------------------------------------------------------------------------
		0xc0017560L iris_216 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl7                 : 32;  // 31:0   
} H13Ax_IRIS_216_T;

/*-----------------------------------------------------------------------------
		0xc0017564L iris_217 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl8                 : 32;  // 31:0   
} H13Ax_IRIS_217_T;

/*-----------------------------------------------------------------------------
		0xc0017568L iris_218 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl9                 : 32;  // 31:0   
} H13Ax_IRIS_218_T;

/*-----------------------------------------------------------------------------
		0xc001756cL iris_219 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl10                : 32;  // 31:0   
} H13Ax_IRIS_219_T;

/*-----------------------------------------------------------------------------
		0xc0017570L iris_220 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl0                 : 32;  // 31:0   
} H13Ax_IRIS_220_T;

/*-----------------------------------------------------------------------------
		0xc0017574L iris_221 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl1                 : 32;  // 31:0   
} H13Ax_IRIS_221_T;

/*-----------------------------------------------------------------------------
		0xc0017578L iris_222 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl2                 : 32;  // 31:0   
} H13Ax_IRIS_222_T;

/*-----------------------------------------------------------------------------
		0xc001757cL iris_223 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl3                 : 32;  // 31:0   
} H13Ax_IRIS_223_T;

/*-----------------------------------------------------------------------------
		0xc0017580L iris_224 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl4                 : 32;  // 31:0   
} H13Ax_IRIS_224_T;

/*-----------------------------------------------------------------------------
		0xc0017584L iris_225 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl5                 : 32;  // 31:0   
} H13Ax_IRIS_225_T;

/*-----------------------------------------------------------------------------
		0xc0017588L iris_226 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl6                 : 32;  // 31:0   
} H13Ax_IRIS_226_T;

/*-----------------------------------------------------------------------------
		0xc001758cL iris_227 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl7                 : 32;  // 31:0   
} H13Ax_IRIS_227_T;

/*-----------------------------------------------------------------------------
		0xc0017590L iris_228 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl0                 : 32;  // 31:0   
} H13Ax_IRIS_228_T;

/*-----------------------------------------------------------------------------
		0xc0017594L iris_229 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl1                 : 32;  // 31:0   
} H13Ax_IRIS_229_T;

/*-----------------------------------------------------------------------------
		0xc0017598L iris_230 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl2                 : 32;  // 31:0   
} H13Ax_IRIS_230_T;

/*-----------------------------------------------------------------------------
		0xc001759cL iris_231 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl3                 : 32;  // 31:0   
} H13Ax_IRIS_231_T;

/*-----------------------------------------------------------------------------
		0xc00175a0L iris_232 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl4                 : 32;  // 31:0   
} H13Ax_IRIS_232_T;

/*-----------------------------------------------------------------------------
		0xc00175a4L iris_233 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl5                 : 32;  // 31:0   
} H13Ax_IRIS_233_T;

/*-----------------------------------------------------------------------------
		0xc00175a8L iris_234 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl6                 : 32;  // 31:0   
} H13Ax_IRIS_234_T;

/*-----------------------------------------------------------------------------
		0xc00175acL iris_235 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl7                 : 32;  // 31:0   
} H13Ax_IRIS_235_T;

/*-----------------------------------------------------------------------------
		0xc00175b0L iris_236 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_blend_ctrl0              : 32;  // 31:0   
} H13Ax_IRIS_236_T;

/*-----------------------------------------------------------------------------
		0xc00175b4L iris_237 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl11                : 32;  // 31:0   
} H13Ax_IRIS_237_T;

/*-----------------------------------------------------------------------------
		0xc00175b8L iris_238 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl12                : 32;  // 31:0   
} H13Ax_IRIS_238_T;

/*-----------------------------------------------------------------------------
		0xc00175bcL iris_239 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl13                : 32;  // 31:0   
} H13Ax_IRIS_239_T;

/*-----------------------------------------------------------------------------
		0xc00175c0L iris_240 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl14                : 32;  // 31:0   
} H13Ax_IRIS_240_T;

/*-----------------------------------------------------------------------------
		0xc00175c4L iris_241 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl15                : 32;  // 31:0   
} H13Ax_IRIS_241_T;

/*-----------------------------------------------------------------------------
		0xc00175c8L iris_242 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl0                    : 32;  // 31:0   
} H13Ax_IRIS_242_T;

/*-----------------------------------------------------------------------------
		0xc00175ccL iris_243 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl1                    : 32;  // 31:0   
} H13Ax_IRIS_243_T;

/*-----------------------------------------------------------------------------
		0xc00175d0L iris_244 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl2                    : 32;  // 31:0   
} H13Ax_IRIS_244_T;

/*-----------------------------------------------------------------------------
		0xc00175d4L iris_245 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl3                    : 32;  // 31:0   
} H13Ax_IRIS_245_T;

/*-----------------------------------------------------------------------------
		0xc00175d8L iris_246 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl4                    : 32;  // 31:0   
} H13Ax_IRIS_246_T;

/*-----------------------------------------------------------------------------
		0xc00175dcL iris_247 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl5                    : 32;  // 31:0   
} H13Ax_IRIS_247_T;

/*-----------------------------------------------------------------------------
		0xc00175e0L iris_248 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl6                    : 32;  // 31:0   
} H13Ax_IRIS_248_T;

/*-----------------------------------------------------------------------------
		0xc00175e4L iris_249 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl7                    : 32;  // 31:0   
} H13Ax_IRIS_249_T;

/*-----------------------------------------------------------------------------
		0xc00175e8L iris_250 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl8                    : 32;  // 31:0   
} H13Ax_IRIS_250_T;

/*-----------------------------------------------------------------------------
		0xc00175ecL iris_251 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl9                    : 32;  // 31:0   
} H13Ax_IRIS_251_T;

/*-----------------------------------------------------------------------------
		0xc00175f0L iris_252 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl10                   : 32;  // 31:0   
} H13Ax_IRIS_252_T;

/*-----------------------------------------------------------------------------
		0xc00175f4L iris_253 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_0             : 32;  // 31:0   
} H13Ax_IRIS_253_T;

/*-----------------------------------------------------------------------------
		0xc00175f8L iris_254 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_1             : 32;  // 31:0   
} H13Ax_IRIS_254_T;

/*-----------------------------------------------------------------------------
		0xc00175fcL iris_255 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_2             : 32;  // 31:0   
} H13Ax_IRIS_255_T;

/*-----------------------------------------------------------------------------
		0xc0017600L iris_256 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_3             : 32;  // 31:0   
} H13Ax_IRIS_256_T;

/*-----------------------------------------------------------------------------
		0xc0017604L iris_257 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_4             : 32;  // 31:0   
} H13Ax_IRIS_257_T;

/*-----------------------------------------------------------------------------
		0xc0017608L iris_258 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_5             : 32;  // 31:0   
} H13Ax_IRIS_258_T;

/*-----------------------------------------------------------------------------
		0xc001760cL iris_259 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef2                  : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_hrs_cx2_coef1                  : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_259_T;

/*-----------------------------------------------------------------------------
		0xc0017610L iris_260 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef4                  : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_hrs_cx2_coef3                  : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_260_T;

/*-----------------------------------------------------------------------------
		0xc0017614L iris_261 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef6                  : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_hrs_cx2_coef5                  : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_261_T;

/*-----------------------------------------------------------------------------
		0xc0017618L iris_262 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef8                  : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_hrs_cx2_coef7                  : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_262_T;

/*-----------------------------------------------------------------------------
		0xc001761cL iris_263 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_ha_width                   : 8 ;  // 7:0    
	UINT32 reg_hrs_ha_start                   : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_IRIS_263_T;

/*-----------------------------------------------------------------------------
		0xc0017620L iris_264 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cti_ctrl0                      : 32;  // 31:0   
} H13Ax_IRIS_264_T;

/*-----------------------------------------------------------------------------
		0xc0017624L iris_265 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cti_ctrl1                      : 32;  // 31:0   
} H13Ax_IRIS_265_T;

/*-----------------------------------------------------------------------------
		0xc0017630L iris_266 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_status0               : 32;  // 31:0   
} H13Ax_IRIS_266_T;

/*-----------------------------------------------------------------------------
		0xc0017634L iris_267 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_6             : 32;  // 31:0   
} H13Ax_IRIS_267_T;

/*-----------------------------------------------------------------------------
		0xc0017638L iris_268 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_7             : 32;  // 31:0   
} H13Ax_IRIS_268_T;

/*-----------------------------------------------------------------------------
		0xc001763cL iris_269 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycadjust_sat_ctrl_0            : 32;  // 31:0   
} H13Ax_IRIS_269_T;

/*-----------------------------------------------------------------------------
		0xc0017640L iris_270 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl16                : 32;  // 31:0   
} H13Ax_IRIS_270_T;

/*-----------------------------------------------------------------------------
		0xc0017644L iris_271 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl17                : 32;  // 31:0   
} H13Ax_IRIS_271_T;

/*-----------------------------------------------------------------------------
		0xc0017648L iris_272 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst2_gate_end                : 8 ;  // 7:0    
	UINT32 reg_burst2_gate_start              : 8 ;  // 15:8   
	UINT32 reg_burst1_gate_end                : 8 ;  // 23:16  
	UINT32 reg_burst1_gate_start              : 8 ;  // 31:24  
} H13Ax_IRIS_272_T;

/*-----------------------------------------------------------------------------
		0xc001764cL iris_273 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cordic2_gate_end               : 8 ;  // 7:0    
	UINT32 reg_cordic2_gate_start             : 8 ;  // 15:8   
	UINT32 reg_cordic1_gate_end               : 8 ;  // 23:16  
	UINT32 reg_cordic1_gate_start             : 8 ;  // 31:24  
} H13Ax_IRIS_273_T;

/*-----------------------------------------------------------------------------
		0xc0017650L iris_274 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_cbcr_ctrl                : 16;  // 15:0   
	UINT32 reg_status_vdetect_vcount          : 10;  // 25:16  
	UINT32                                    : 2 ;  // 27:26   reserved
	UINT32 reg_vactive_height_auto            : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_274_T;

/*-----------------------------------------------------------------------------
		0xc0017654L iris_275 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_1           : 17;  // 16:0   
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_secam_y_notch_mode             : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_IRIS_275_T;

/*-----------------------------------------------------------------------------
		0xc0017658L iris_276 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_2           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_276_T;

/*-----------------------------------------------------------------------------
		0xc001765cL iris_277 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_3           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_277_T;

/*-----------------------------------------------------------------------------
		0xc0017660L iris_278 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_4           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_278_T;

/*-----------------------------------------------------------------------------
		0xc0017664L iris_279 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_5           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_279_T;

/*-----------------------------------------------------------------------------
		0xc0017668L iris_280 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_6           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_280_T;

/*-----------------------------------------------------------------------------
		0xc001766cL iris_281 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_7           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_281_T;

/*-----------------------------------------------------------------------------
		0xc0017670L iris_282 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_8           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_282_T;

/*-----------------------------------------------------------------------------
		0xc0017674L iris_283 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_9           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_283_T;

/*-----------------------------------------------------------------------------
		0xc0017678L iris_284 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_1           : 17;  // 16:0   
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_secam_c_notch_sel              : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_IRIS_284_T;

/*-----------------------------------------------------------------------------
		0xc001767cL iris_285 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_2           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_285_T;

/*-----------------------------------------------------------------------------
		0xc0017680L iris_286 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_3           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_286_T;

/*-----------------------------------------------------------------------------
		0xc0017684L iris_287 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_4           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_287_T;

/*-----------------------------------------------------------------------------
		0xc0017688L iris_288 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_5           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_288_T;

/*-----------------------------------------------------------------------------
		0xc001768cL iris_289 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_6           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_289_T;

/*-----------------------------------------------------------------------------
		0xc0017690L iris_290 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_7           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_290_T;

/*-----------------------------------------------------------------------------
		0xc0017694L iris_291 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_8           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_291_T;

/*-----------------------------------------------------------------------------
		0xc0017698L iris_292 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_9           : 17;  // 16:0   
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_IRIS_292_T;

/*-----------------------------------------------------------------------------
		0xc001769cL iris_293 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl11                   : 32;  // 31:0   
} H13Ax_IRIS_293_T;

/*-----------------------------------------------------------------------------
		0xc00176a0L iris_294 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_ybw2                     : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_secam_y_delay                  : 6 ;  // 9:4    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_secam_dcr_passthrough          : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_v_valid_inv                    : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_secam_lb_en                    : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_IRIS_294_T;

/*-----------------------------------------------------------------------------
		0xc00176a4L iris_295 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_dcr_ctrl_0               : 32;  // 31:0   
} H13Ax_IRIS_295_T;

/*-----------------------------------------------------------------------------
		0xc00176a8L iris_296 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_dcr_ctrl_1               : 32;  // 31:0   
} H13Ax_IRIS_296_T;

/*-----------------------------------------------------------------------------
		0xc00176acL iris_297 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_dcr_ctrl_2               : 32;  // 31:0   
} H13Ax_IRIS_297_T;

/*-----------------------------------------------------------------------------
		0xc00176b0L iris_298 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_yup_rise                       : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_ydn_cnt                        : 8 ;  // 19:12  
	UINT32 reg_yup_cnt                        : 8 ;  // 27:20  
	UINT32                                    : 4 ;  // 31:28   reserved
} H13Ax_IRIS_298_T;

/*-----------------------------------------------------------------------------
		0xc00176b4L iris_299 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ydn_rise                       : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 reg_yup_fall                       : 11;  // 22:12  
	UINT32                                    : 9 ;  // 31:23   reserved
} H13Ax_IRIS_299_T;

/*-----------------------------------------------------------------------------
		0xc00176b8L iris_300 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_clmap_sel                      : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_ydn_fall                       : 11;  // 14:4   
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_300_T;

/*-----------------------------------------------------------------------------
		0xc00176bcL iris_301 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl18                : 32;  // 31:0   
} H13Ax_IRIS_301_T;

/*-----------------------------------------------------------------------------
		0xc00176c0L iris_302 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl19                : 32;  // 31:0   
} H13Ax_IRIS_302_T;

/*-----------------------------------------------------------------------------
		0xc00176c4L iris_303 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl20                : 32;  // 31:0   
} H13Ax_IRIS_303_T;

/*-----------------------------------------------------------------------------
		0xc00176c8L iris_304 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wmask_ctrl_0                   : 32;  // 31:0   
} H13Ax_IRIS_304_T;

/*-----------------------------------------------------------------------------
		0xc00176ccL iris_305 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wmask_ctrl_1                   : 32;  // 31:0   
} H13Ax_IRIS_305_T;

/*-----------------------------------------------------------------------------
		0xc00176d0L iris_306 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_8             : 32;  // 31:0   
} H13Ax_IRIS_306_T;

/*-----------------------------------------------------------------------------
		0xc00176d4L iris_307 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdct_ifcomp_ctrl_0             : 32;  // 31:0   
} H13Ax_IRIS_307_T;

/*-----------------------------------------------------------------------------
		0xc0017800L vbi_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wss625_rd_done                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cc_rd_done                     : 1 ;  // 4      
	UINT32                                    : 27;  // 31:5    reserved
} H13Ax_VBI_001_T;

/*-----------------------------------------------------------------------------
		0xc0017804L vbi_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cc_rdy                         : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_cc_data0                       : 8 ;  // 11:4   
	UINT32 reg_cc_data1                       : 8 ;  // 19:12  
	UINT32                                    : 12;  // 31:20   reserved
} H13Ax_VBI_002_T;

/*-----------------------------------------------------------------------------
		0xc0017808L vbi_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wssdata0                       : 8 ;  // 7:0    
	UINT32 reg_wssdata1                       : 8 ;  // 15:8   
	UINT32 reg_wssdata2                       : 8 ;  // 23:16  
	UINT32 reg_wss_rdy                        : 1 ;  // 24     
	UINT32                                    : 7 ;  // 31:25   reserved
} H13Ax_VBI_003_T;

/*-----------------------------------------------------------------------------
		0xc001780cL vbi_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cc_short_start                 : 1 ;  // 0      
	UINT32                                    : 3 ;  // 3:1     reserved
	UINT32 reg_vbi_muxout                     : 1 ;  // 4      
	UINT32                                    : 3 ;  // 7:5     reserved
	UINT32 reg_vbi_hsyncout                   : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_adap_slvl_en                   : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_vbi_st_err_ignored             : 1 ;  // 16     
	UINT32                                    : 3 ;  // 19:17   reserved
	UINT32 reg_vbi_en                         : 1 ;  // 20     
	UINT32                                    : 11;  // 31:21   reserved
} H13Ax_VBI_004_T;

/*-----------------------------------------------------------------------------
		0xc0017810L vbi_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_lpf_bw                     : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_vbi_scmux_sw                   : 5 ;  // 8:4    
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_vbi_data_sel                   : 2 ;  // 13:12  
	UINT32 reg_vbi_peak_sel                   : 1 ;  // 14     
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_VBI_005_T;

/*-----------------------------------------------------------------------------
		0xc0017814L vbi_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_data_hlvl                  : 8 ;  // 7:0    
	UINT32 reg_start_code                     : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_VBI_006_T;

/*-----------------------------------------------------------------------------
		0xc0017818L vbi_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil9                          : 8 ;  // 7:0    
	UINT32 reg_vbil8                          : 8 ;  // 15:8   
	UINT32 reg_vbil7                          : 8 ;  // 23:16  
	UINT32 reg_vbil6                          : 8 ;  // 31:24  
} H13Ax_VBI_007_T;

/*-----------------------------------------------------------------------------
		0xc001781cL vbi_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil13                         : 8 ;  // 7:0    
	UINT32 reg_vbil12                         : 8 ;  // 15:8   
	UINT32 reg_vbil11                         : 8 ;  // 23:16  
	UINT32 reg_vbil10                         : 8 ;  // 31:24  
} H13Ax_VBI_008_T;

/*-----------------------------------------------------------------------------
		0xc0017820L vbi_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil17                         : 8 ;  // 7:0    
	UINT32 reg_vbil16                         : 8 ;  // 15:8   
	UINT32 reg_vbil15                         : 8 ;  // 23:16  
	UINT32 reg_vbil14                         : 8 ;  // 31:24  
} H13Ax_VBI_009_T;

/*-----------------------------------------------------------------------------
		0xc0017824L vbi_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil21                         : 8 ;  // 7:0    
	UINT32 reg_vbil20                         : 8 ;  // 15:8   
	UINT32 reg_vbil19                         : 8 ;  // 23:16  
	UINT32 reg_vbil18                         : 8 ;  // 31:24  
} H13Ax_VBI_010_T;

/*-----------------------------------------------------------------------------
		0xc0017828L vbi_011 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil25                         : 8 ;  // 7:0    
	UINT32 reg_vbil24                         : 8 ;  // 15:8   
	UINT32 reg_vbil23                         : 8 ;  // 23:16  
	UINT32 reg_vbil22                         : 8 ;  // 31:24  
} H13Ax_VBI_011_T;

/*-----------------------------------------------------------------------------
		0xc001782cL vbi_012 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil26                         : 8 ;  // 7:0    
	UINT32 reg_vbil5                          : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_VBI_012_T;

/*-----------------------------------------------------------------------------
		0xc0017830L vbi_013 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_vps_lpfil_fine_gain        : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_vbi_wss625_lpfil_fine_gain     : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_vbi_tele_lpfil_fine_gain       : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_vbi_cc_lpfil_fine_gain         : 2 ;  // 13:12  
	UINT32                                    : 2 ;  // 15:14   reserved
	UINT32 reg_vbi_vps_slicer_mode            : 2 ;  // 17:16  
	UINT32                                    : 2 ;  // 19:18   reserved
	UINT32 reg_vbi_wss_slicer_mode            : 2 ;  // 21:20  
	UINT32                                    : 2 ;  // 23:22   reserved
	UINT32 reg_vbi_tt_slicer_mode             : 2 ;  // 25:24  
	UINT32                                    : 2 ;  // 27:26   reserved
	UINT32 reg_vbi_cc_slicer_mode             : 2 ;  // 29:28  
	UINT32                                    : 2 ;  // 31:30   reserved
} H13Ax_VBI_013_T;

/*-----------------------------------------------------------------------------
		0xc0017834L vbi_014 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_teletext_dto                   : 16;  // 15:0   
	UINT32 reg_caption_dto                    : 16;  // 31:16  
} H13Ax_VBI_014_T;

/*-----------------------------------------------------------------------------
		0xc0017838L vbi_015 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_dto                        : 16;  // 15:0   
	UINT32 reg_wss625_dto                     : 16;  // 31:16  
} H13Ax_VBI_015_T;

/*-----------------------------------------------------------------------------
		0xc001783cL vbi_016 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_frame_start                : 2 ;  // 1:0    
	UINT32                                    : 2 ;  // 3:2     reserved
	UINT32 reg_teletext_frame_start           : 2 ;  // 5:4    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 reg_wss625_frame_start             : 2 ;  // 9:8    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 reg_caption_frame_start            : 2 ;  // 13:12  
	UINT32                                    : 18;  // 31:14   reserved
} H13Ax_VBI_016_T;

/*-----------------------------------------------------------------------------
		0xc0017840L vbi_017 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_tele_lpfil_track_gain      : 4 ;  // 3:0    
	UINT32 reg_vbi_tele_lpfil_acq_gain        : 4 ;  // 7:4    
	UINT32 reg_vbi_cc_lpfil_track_gain        : 4 ;  // 11:8   
	UINT32 reg_vbi_cc_lpfil_acq_gain          : 4 ;  // 15:12  
	UINT32 reg_wssj_delta_ampl                : 8 ;  // 23:16  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_VBI_017_T;

/*-----------------------------------------------------------------------------
		0xc0017844L vbi_018 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_vps_lpfil_track_gain       : 4 ;  // 3:0    
	UINT32 reg_vbi_vps_lpfil_acq_gain         : 4 ;  // 7:4    
	UINT32 reg_vbi_wss625_lpfil_track_gain    : 4 ;  // 11:8   
	UINT32 reg_vbi_wss625_lpfil_acq_gain      : 4 ;  // 15:12  
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_VBI_018_T;

/*-----------------------------------------------------------------------------
		0xc0017848L vbi_019 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_teletext_runin_accum_ampl      : 8 ;  // 7:0    
	UINT32 reg_caption_runin_accum_ampl       : 8 ;  // 15:8   
	UINT32                                    : 16;  // 31:16   reserved
} H13Ax_VBI_019_T;

/*-----------------------------------------------------------------------------
		0xc001784cL vbi_020 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_start                      : 8 ;  // 7:0    
	UINT32 reg_teletext_start                 : 8 ;  // 15:8   
	UINT32 reg_wss625_start                   : 8 ;  // 23:16  
	UINT32 reg_caption_start                  : 8 ;  // 31:24  
} H13Ax_VBI_020_T;

/*-----------------------------------------------------------------------------
		0xc0017850L vbi_021 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_frame_en_cnt                   : 9 ;  // 8:0    
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 reg_frame_en_adj                   : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 reg_vbi_fixgate_en                 : 1 ;  // 16     
	UINT32                                    : 15;  // 31:17   reserved
} H13Ax_VBI_021_T;

/*-----------------------------------------------------------------------------
		0xc0017854L vbi_022 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi1_detect_cnt                : 16;  // 15:0   
	UINT32 reg_vbi_monitor_line               : 10;  // 25:16  
	UINT32                                    : 2 ;  // 27:26   reserved
	UINT32 reg_vbi_monitor_ctrl               : 3 ;  // 30:28  
	UINT32                                    : 1 ;  // 31:31   reserved
} H13Ax_VBI_022_T;

/*-----------------------------------------------------------------------------
		0xc0017858L vbi_023 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_odd_flip                   : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_VBI_023_T;

/*-----------------------------------------------------------------------------
		0xc00178a8L iris_mif_gmua_mon_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_rbuf2_pel_cnt                   : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 ro_rbuf2_empty                     : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 ro_rbuf1_pel_cnt                   : 11;  // 26:16  
	UINT32                                    : 1 ;  // 27:27   reserved
	UINT32 ro_rbuf1_empty                     : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_MIF_GMUA_MON_001_T;

/*-----------------------------------------------------------------------------
		0xc00178acL iris_mif_gmua_mon_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_rbuf4_pel_cnt                   : 11;  // 10:0   
	UINT32                                    : 1 ;  // 11:11   reserved
	UINT32 ro_rbuf4_empty                     : 1 ;  // 12     
	UINT32                                    : 3 ;  // 15:13   reserved
	UINT32 ro_rbuf3_pel_cnt                   : 11;  // 26:16  
	UINT32                                    : 1 ;  // 27:27   reserved
	UINT32 ro_rbuf3_empty                     : 1 ;  // 28     
	UINT32                                    : 3 ;  // 31:29   reserved
} H13Ax_IRIS_MIF_GMUA_MON_002_T;

/*-----------------------------------------------------------------------------
		0xc00178b0L iris_mif_gmua_mon_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_wbuf_word_cnt                   : 6 ;  // 5:0    
	UINT32                                    : 2 ;  // 7:6     reserved
	UINT32 ro_wbuf_empty                      : 1 ;  // 8      
	UINT32                                    : 3 ;  // 11:9    reserved
	UINT32 ro_wbuf_full                       : 1 ;  // 12     
	UINT32                                    : 19;  // 31:13   reserved
} H13Ax_IRIS_MIF_GMUA_MON_003_T;

/*-----------------------------------------------------------------------------
		0xc00178b4L iris_mif_gmua_mon_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mon_wait_max_cnt               : 12;  // 11:0   
	UINT32                                    : 20;  // 31:12   reserved
} H13Ax_IRIS_MIF_GMUA_MON_004_T;

/*-----------------------------------------------------------------------------
		0xc00178b8L iris_mif_gmua_mon_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_wr_wait_err                 : 8 ;  // 7:0    
	UINT32                                    : 24;  // 31:8    reserved
} H13Ax_IRIS_MIF_GMUA_MON_005_T;

/*-----------------------------------------------------------------------------
		0xc00178bcL iris_mif_gmua_mon_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd4_wait_err                : 8 ;  // 7:0    
	UINT32 ro_mon_rd3_wait_err                : 8 ;  // 15:8   
	UINT32 ro_mon_rd2_wait_err                : 8 ;  // 23:16  
	UINT32 ro_mon_rd1_wait_err                : 8 ;  // 31:24  
} H13Ax_IRIS_MIF_GMUA_MON_006_T;

/*-----------------------------------------------------------------------------
		0xc00178c0L iris_mif_gmua_mon_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_wr_req_num                  : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 ro_mon_wr_req_que_cnt              : 3 ;  // 14:12  
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_MIF_GMUA_MON_007_T;

/*-----------------------------------------------------------------------------
		0xc00178c4L iris_mif_gmua_mon_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_wr_wait_cnt1                : 12;  // 11:0   
	UINT32 ro_mon_wr_wait_cnt0                : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_008_T;

/*-----------------------------------------------------------------------------
		0xc00178c8L iris_mif_gmua_mon_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_wr_wait_cnt3                : 12;  // 11:0   
	UINT32 ro_mon_wr_wait_cnt2                : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_009_T;

/*-----------------------------------------------------------------------------
		0xc00178ccL iris_mif_gmua_mon_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd1_req_num                 : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 ro_mon_rd1_req_que_cnt             : 3 ;  // 14:12  
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_MIF_GMUA_MON_010_T;

/*-----------------------------------------------------------------------------
		0xc00178d0L iris_mif_gmua_mon_011 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd1_wait_cnt1               : 12;  // 11:0   
	UINT32 ro_mon_rd1_wait_cnt0               : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_011_T;

/*-----------------------------------------------------------------------------
		0xc00178d4L iris_mif_gmua_mon_012 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd1_wait_cnt3               : 12;  // 11:0   
	UINT32 ro_mon_rd1_wait_cnt2               : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_012_T;

/*-----------------------------------------------------------------------------
		0xc00178d8L iris_mif_gmua_mon_013 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd2_req_num                 : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 ro_mon_rd2_req_que_cnt             : 3 ;  // 14:12  
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_MIF_GMUA_MON_013_T;

/*-----------------------------------------------------------------------------
		0xc00178dcL iris_mif_gmua_mon_014 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd2_wait_cnt1               : 12;  // 11:0   
	UINT32 ro_mon_rd2_wait_cnt0               : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_014_T;

/*-----------------------------------------------------------------------------
		0xc00178e0L iris_mif_gmua_mon_015 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd2_wait_cnt3               : 12;  // 11:0   
	UINT32 ro_mon_rd2_wait_cnt2               : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_015_T;

/*-----------------------------------------------------------------------------
		0xc00178e4L iris_mif_gmua_mon_016 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd3_req_num                 : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 ro_mon_rd3_req_que_cnt             : 3 ;  // 14:12  
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_MIF_GMUA_MON_016_T;

/*-----------------------------------------------------------------------------
		0xc00178e8L iris_mif_gmua_mon_017 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd3_wait_cnt1               : 12;  // 11:0   
	UINT32 ro_mon_rd3_wait_cnt0               : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_017_T;

/*-----------------------------------------------------------------------------
		0xc00178ecL iris_mif_gmua_mon_018 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd3_wait_cnt3               : 12;  // 11:0   
	UINT32 ro_mon_rd3_wait_cnt2               : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_018_T;

/*-----------------------------------------------------------------------------
		0xc00178f0L iris_mif_gmua_mon_019 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd4_req_num                 : 10;  // 9:0    
	UINT32                                    : 2 ;  // 11:10   reserved
	UINT32 ro_mon_rd4_req_que_cnt             : 3 ;  // 14:12  
	UINT32                                    : 17;  // 31:15   reserved
} H13Ax_IRIS_MIF_GMUA_MON_019_T;

/*-----------------------------------------------------------------------------
		0xc00178f4L iris_mif_gmua_mon_020 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd4_wait_cnt0               : 12;  // 11:0   
	UINT32                                    : 20;  // 31:12   reserved
} H13Ax_IRIS_MIF_GMUA_MON_020_T;

/*-----------------------------------------------------------------------------
		0xc00178f8L iris_mif_gmua_mon_021 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd4_wait_cnt1               : 12;  // 11:0   
	UINT32                                    : 20;  // 31:12   reserved
} H13Ax_IRIS_MIF_GMUA_MON_021_T;

/*-----------------------------------------------------------------------------
		0xc00178fcL iris_mif_gmua_mon_022 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_mon_rd4_wait_cnt3               : 12;  // 11:0   
	UINT32 ro_mon_rd4_wait_cnt2               : 12;  // 23:12  
	UINT32                                    : 8 ;  // 31:24   reserved
} H13Ax_IRIS_MIF_GMUA_MON_022_T;

/*-----------------------------------------------------------------------------
		0xc0017900L iris_hif_ctrl0_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pending_enable                 : 1 ;  // 0      
	UINT32                                    : 31;  // 31:1    reserved
} H13Ax_IRIS_HIF_CTRL0_001_T;

/*-----------------------------------------------------------------------------
		0xc0017904L iris_hif_ctrl1_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cv_timer_ctrl                  : 9 ;  // 8:0    
	UINT32                                    : 23;  // 31:9    reserved
} H13Ax_IRIS_HIF_CTRL1_001_T;


typedef struct {
	H13Ax_CVD_INTR1_ENABLE0_T          cvd_intr1_enable0;            //0xc0017100L
	H13Ax_CVD_INTR1_INTR0_T            cvd_intr1_intr0;              //0xc0017104L
	H13Ax_CVD_INTR1_CLEAR0_T           cvd_intr1_clear0;             //0xc0017108L
	H13Ax_CVD_INTR1_ENABLE1_T          cvd_intr1_enable1;            //0xc001710cL
	H13Ax_CVD_INTR1_INTR1_T            cvd_intr1_intr1;              //0xc0017110L
	H13Ax_CVD_INTR1_CLEAR1_T           cvd_intr1_clear1;             //0xc0017114L
	H13Ax_CVD_INTR2_ENABLE0_T          cvd_intr2_enable0;            //0xc0017118L
	H13Ax_CVD_INTR2_INTR0_T            cvd_intr2_intr0;              //0xc001711cL
	H13Ax_CVD_INTR2_CLEAR0_T           cvd_intr2_clear0;             //0xc0017120L
	H13Ax_CVD_INTR2_ENABLE1_T          cvd_intr2_enable1;            //0xc0017124L
	H13Ax_CVD_INTR2_INTR1_T            cvd_intr2_intr1;              //0xc0017128L
	H13Ax_CVD_INTR2_CLEAR1_T           cvd_intr2_clear1;             //0xc001712cL
	H13Ax_TOP_001_T                    top_001;                      //0xc0017130L
	UINT32                             reserved00[3];                //0xc0017134L~0xc001713cL
	H13Ax_IRIS_MIF_GMUA_001_T          iris_mif_gmua_001;            //0xc0017140L
	H13Ax_IRIS_MIF_GMUA_002_T          iris_mif_gmua_002;            //0xc0017144L
	H13Ax_IRIS_MIF_GMUA_003_T          iris_mif_gmua_003;            //0xc0017148L
	H13Ax_IRIS_MIF_GMUA_004_T          iris_mif_gmua_004;            //0xc001714cL
	H13Ax_IRIS_MIF_GMUA_005_T          iris_mif_gmua_005;            //0xc0017150L
	H13Ax_IRIS_MIF_GMUA_006_T          iris_mif_gmua_006;            //0xc0017154L
	H13Ax_IRIS_MIF_GMUA_007_T          iris_mif_gmua_007;            //0xc0017158L
	UINT32                             reserved01;                   //0xc001715cL
	H13Ax_IRIS_MIF_GMUA_008_T          iris_mif_gmua_008;            //0xc0017160L
	H13Ax_IRIS_MIF_GMUA_009_T          iris_mif_gmua_009;            //0xc0017164L
	H13Ax_FASTBLANK_001_T              fastblank_001;                //0xc0017168L
	H13Ax_FASTBLANK_002_T              fastblank_002;                //0xc001716cL
	H13Ax_FASTBLANK_003_T              fastblank_003;                //0xc0017170L
	H13Ax_FASTBLANK_004_T              fastblank_004;                //0xc0017174L
	H13Ax_FASTBLANK_005_T              fastblank_005;                //0xc0017178L
	H13Ax_FASTBLANK_006_T              fastblank_006;                //0xc001717cL
	H13Ax_FASTBLANK_007_T              fastblank_007;                //0xc0017180L
	H13Ax_FASTBLANK_008_T              fastblank_008;                //0xc0017184L
	H13Ax_FASTBLANK_009_T              fastblank_009;                //0xc0017188L
	H13Ax_FASTBLANK_010_T              fastblank_010;                //0xc001718cL
	H13Ax_IRIS_DE_CTRL_001_T           iris_de_ctrl_001;             //0xc0017190L
	H13Ax_VBI_CTRL_001_T               vbi_ctrl_001;                 //0xc0017194L
	H13Ax_VBI_CTRL_002_T               vbi_ctrl_002;                 //0xc0017198L
	H13Ax_VBI_CTRL_003_T               vbi_ctrl_003;                 //0xc001719cL
	H13Ax_VBI_CTRL_004_T               vbi_ctrl_004;                 //0xc00171a0L
	H13Ax_VBI_CTRL_005_T               vbi_ctrl_005;                 //0xc00171a4L
	H13Ax_VBI_CTRL_006_T               vbi_ctrl_006;                 //0xc00171a8L
	H13Ax_VBI_CTRL_007_T               vbi_ctrl_007;                 //0xc00171acL
	H13Ax_VBI_CTRL_008_T               vbi_ctrl_008;                 //0xc00171b0L
	H13Ax_VBI_CTRL_009_T               vbi_ctrl_009;                 //0xc00171b4L
	H13Ax_VBI_CTRL_010_T               vbi_ctrl_010;                 //0xc00171b8L
	UINT32                             reserved02;                   //0xc00171bcL
	H13Ax_FAST_BLANK_STATUS_001_T      fast_blank_status_001;        //0xc00171c0L
	H13Ax_CVBSAFE_001_T                cvbsafe_001;                  //0xc00171c4L
	UINT32                             reserved03[14];               //0xc00171c8L~0xc00171fcL
	H13Ax_IRIS_001_T                   iris_001;                     //0xc0017200L
	H13Ax_IRIS_002_T                   iris_002;                     //0xc0017204L
	H13Ax_IRIS_003_T                   iris_003;                     //0xc0017208L
	H13Ax_IRIS_004_T                   iris_004;                     //0xc001720cL
	H13Ax_IRIS_005_T                   iris_005;                     //0xc0017210L
	H13Ax_IRIS_006_T                   iris_006;                     //0xc0017214L
	H13Ax_IRIS_007_T                   iris_007;                     //0xc0017218L
	H13Ax_IRIS_008_T                   iris_008;                     //0xc001721cL
	H13Ax_IRIS_009_T                   iris_009;                     //0xc0017220L
	H13Ax_IRIS_010_T                   iris_010;                     //0xc0017224L
	H13Ax_IRIS_011_T                   iris_011;                     //0xc0017228L
	UINT32                             reserved04;                   //0xc001722cL
	H13Ax_IRIS_012_T                   iris_012;                     //0xc0017230L
	H13Ax_IRIS_013_T                   iris_013;                     //0xc0017234L
	H13Ax_IRIS_014_T                   iris_014;                     //0xc0017238L
	H13Ax_IRIS_015_T                   iris_015;                     //0xc001723cL
	H13Ax_IRIS_016_T                   iris_016;                     //0xc0017240L
	H13Ax_IRIS_017_T                   iris_017;                     //0xc0017244L
	H13Ax_IRIS_018_T                   iris_018;                     //0xc0017248L
	H13Ax_IRIS_019_T                   iris_019;                     //0xc001724cL
	H13Ax_IRIS_020_T                   iris_020;                     //0xc0017250L
	H13Ax_IRIS_021_T                   iris_021;                     //0xc0017254L
	H13Ax_IRIS_022_T                   iris_022;                     //0xc0017258L
	H13Ax_IRIS_023_T                   iris_023;                     //0xc001725cL
	H13Ax_IRIS_024_T                   iris_024;                     //0xc0017260L
	H13Ax_IRIS_025_T                   iris_025;                     //0xc0017264L
	H13Ax_IRIS_026_T                   iris_026;                     //0xc0017268L
	H13Ax_IRIS_027_T                   iris_027;                     //0xc001726cL
	H13Ax_IRIS_028_T                   iris_028;                     //0xc0017270L
	H13Ax_IRIS_029_T                   iris_029;                     //0xc0017274L
	H13Ax_IRIS_030_T                   iris_030;                     //0xc0017278L
	H13Ax_IRIS_031_T                   iris_031;                     //0xc001727cL
	H13Ax_IRIS_032_T                   iris_032;                     //0xc0017280L
	H13Ax_IRIS_033_T                   iris_033;                     //0xc0017284L
	H13Ax_IRIS_034_T                   iris_034;                     //0xc0017288L
	H13Ax_IRIS_035_T                   iris_035;                     //0xc001728cL
	H13Ax_IRIS_036_T                   iris_036;                     //0xc0017290L
	H13Ax_IRIS_037_T                   iris_037;                     //0xc0017294L
	H13Ax_IRIS_038_T                   iris_038;                     //0xc0017298L
	H13Ax_IRIS_039_T                   iris_039;                     //0xc001729cL
	H13Ax_IRIS_040_T                   iris_040;                     //0xc00172a0L
	H13Ax_IRIS_041_T                   iris_041;                     //0xc00172a4L
	H13Ax_IRIS_042_T                   iris_042;                     //0xc00172a8L
	H13Ax_IRIS_043_T                   iris_043;                     //0xc00172acL
	H13Ax_IRIS_044_T                   iris_044;                     //0xc00172b0L
	H13Ax_IRIS_045_T                   iris_045;                     //0xc00172b4L
	H13Ax_IRIS_046_T                   iris_046;                     //0xc00172b8L
	H13Ax_IRIS_047_T                   iris_047;                     //0xc00172bcL
	H13Ax_IRIS_048_T                   iris_048;                     //0xc00172c0L
	H13Ax_IRIS_049_T                   iris_049;                     //0xc00172c4L
	H13Ax_IRIS_050_T                   iris_050;                     //0xc00172c8L
	H13Ax_IRIS_051_T                   iris_051;                     //0xc00172ccL
	H13Ax_IRIS_052_T                   iris_052;                     //0xc00172d0L
	H13Ax_IRIS_053_T                   iris_053;                     //0xc00172d4L
	H13Ax_IRIS_054_T                   iris_054;                     //0xc00172d8L
	H13Ax_IRIS_055_T                   iris_055;                     //0xc00172dcL
	H13Ax_IRIS_056_T                   iris_056;                     //0xc00172e0L
	H13Ax_IRIS_057_T                   iris_057;                     //0xc00172e4L
	H13Ax_IRIS_058_T                   iris_058;                     //0xc00172e8L
	H13Ax_IRIS_059_T                   iris_059;                     //0xc00172ecL
	H13Ax_IRIS_060_T                   iris_060;                     //0xc00172f0L
	H13Ax_IRIS_061_T                   iris_061;                     //0xc00172f4L
	H13Ax_IRIS_062_T                   iris_062;                     //0xc00172f8L
	H13Ax_IRIS_063_T                   iris_063;                     //0xc00172fcL
	H13Ax_IRIS_064_T                   iris_064;                     //0xc0017300L
	H13Ax_IRIS_065_T                   iris_065;                     //0xc0017304L
	H13Ax_IRIS_066_T                   iris_066;                     //0xc0017308L
	H13Ax_IRIS_067_T                   iris_067;                     //0xc001730cL
	H13Ax_IRIS_068_T                   iris_068;                     //0xc0017310L
	H13Ax_IRIS_069_T                   iris_069;                     //0xc0017314L
	H13Ax_IRIS_070_T                   iris_070;                     //0xc0017318L
	H13Ax_IRIS_071_T                   iris_071;                     //0xc001731cL
	H13Ax_IRIS_072_T                   iris_072;                     //0xc0017320L
	H13Ax_IRIS_073_T                   iris_073;                     //0xc0017324L
	H13Ax_IRIS_074_T                   iris_074;                     //0xc0017328L
	H13Ax_IRIS_075_T                   iris_075;                     //0xc001732cL
	H13Ax_IRIS_076_T                   iris_076;                     //0xc0017330L
	H13Ax_IRIS_077_T                   iris_077;                     //0xc0017334L
	H13Ax_IRIS_078_T                   iris_078;                     //0xc0017338L
	H13Ax_IRIS_079_T                   iris_079;                     //0xc001733cL
	H13Ax_IRIS_080_T                   iris_080;                     //0xc0017340L
	H13Ax_IRIS_081_T                   iris_081;                     //0xc0017344L
	H13Ax_IRIS_082_T                   iris_082;                     //0xc0017348L
	H13Ax_IRIS_083_T                   iris_083;                     //0xc001734cL
	H13Ax_IRIS_084_T                   iris_084;                     //0xc0017350L
	H13Ax_IRIS_085_T                   iris_085;                     //0xc0017354L
	H13Ax_IRIS_086_T                   iris_086;                     //0xc0017358L
	H13Ax_IRIS_087_T                   iris_087;                     //0xc001735cL
	H13Ax_IRIS_088_T                   iris_088;                     //0xc0017360L
	H13Ax_IRIS_089_T                   iris_089;                     //0xc0017364L
	H13Ax_IRIS_090_T                   iris_090;                     //0xc0017368L
	H13Ax_IRIS_091_T                   iris_091;                     //0xc001736cL
	H13Ax_IRIS_092_T                   iris_092;                     //0xc0017370L
	H13Ax_IRIS_093_T                   iris_093;                     //0xc0017374L
	H13Ax_IRIS_094_T                   iris_094;                     //0xc0017378L
	H13Ax_IRIS_095_T                   iris_095;                     //0xc001737cL
	H13Ax_IRIS_096_T                   iris_096;                     //0xc0017380L
	H13Ax_IRIS_097_T                   iris_097;                     //0xc0017384L
	H13Ax_IRIS_098_T                   iris_098;                     //0xc0017388L
	H13Ax_IRIS_099_T                   iris_099;                     //0xc001738cL
	H13Ax_IRIS_100_T                   iris_100;                     //0xc0017390L
	H13Ax_IRIS_101_T                   iris_101;                     //0xc0017394L
	H13Ax_IRIS_102_T                   iris_102;                     //0xc0017398L
	H13Ax_IRIS_103_T                   iris_103;                     //0xc001739cL
	H13Ax_IRIS_104_T                   iris_104;                     //0xc00173a0L
	H13Ax_IRIS_105_T                   iris_105;                     //0xc00173a4L
	H13Ax_IRIS_106_T                   iris_106;                     //0xc00173a8L
	H13Ax_IRIS_107_T                   iris_107;                     //0xc00173acL
	H13Ax_IRIS_108_T                   iris_108;                     //0xc00173b0L
	H13Ax_IRIS_109_T                   iris_109;                     //0xc00173b4L
	H13Ax_IRIS_110_T                   iris_110;                     //0xc00173b8L
	H13Ax_IRIS_111_T                   iris_111;                     //0xc00173bcL
	H13Ax_IRIS_112_T                   iris_112;                     //0xc00173c0L
	H13Ax_IRIS_113_T                   iris_113;                     //0xc00173c4L
	H13Ax_IRIS_114_T                   iris_114;                     //0xc00173c8L
	H13Ax_IRIS_115_T                   iris_115;                     //0xc00173ccL
	H13Ax_IRIS_116_T                   iris_116;                     //0xc00173d0L
	H13Ax_IRIS_117_T                   iris_117;                     //0xc00173d4L
	H13Ax_IRIS_118_T                   iris_118;                     //0xc00173d8L
	H13Ax_IRIS_119_T                   iris_119;                     //0xc00173dcL
	H13Ax_IRIS_120_T                   iris_120;                     //0xc00173e0L
	H13Ax_IRIS_121_T                   iris_121;                     //0xc00173e4L
	H13Ax_IRIS_122_T                   iris_122;                     //0xc00173e8L
	H13Ax_IRIS_123_T                   iris_123;                     //0xc00173ecL
	H13Ax_IRIS_124_T                   iris_124;                     //0xc00173f0L
	H13Ax_IRIS_125_T                   iris_125;                     //0xc00173f4L
	H13Ax_IRIS_126_T                   iris_126;                     //0xc00173f8L
	H13Ax_IRIS_127_T                   iris_127;                     //0xc00173fcL
	H13Ax_IRIS_128_T                   iris_128;                     //0xc0017400L
	H13Ax_IRIS_129_T                   iris_129;                     //0xc0017404L
	H13Ax_IRIS_130_T                   iris_130;                     //0xc0017408L
	H13Ax_IRIS_131_T                   iris_131;                     //0xc001740cL
	H13Ax_IRIS_132_T                   iris_132;                     //0xc0017410L
	H13Ax_IRIS_133_T                   iris_133;                     //0xc0017414L
	H13Ax_IRIS_134_T                   iris_134;                     //0xc0017418L
	H13Ax_IRIS_135_T                   iris_135;                     //0xc001741cL
	H13Ax_IRIS_136_T                   iris_136;                     //0xc0017420L
	H13Ax_IRIS_137_T                   iris_137;                     //0xc0017424L
	H13Ax_IRIS_138_T                   iris_138;                     //0xc0017428L
	H13Ax_IRIS_139_T                   iris_139;                     //0xc001742cL
	H13Ax_IRIS_140_T                   iris_140;                     //0xc0017430L
	H13Ax_IRIS_141_T                   iris_141;                     //0xc0017434L
	H13Ax_IRIS_142_T                   iris_142;                     //0xc0017438L
	H13Ax_IRIS_143_T                   iris_143;                     //0xc001743cL
	H13Ax_IRIS_144_T                   iris_144;                     //0xc0017440L
	H13Ax_IRIS_145_T                   iris_145;                     //0xc0017444L
	H13Ax_IRIS_146_T                   iris_146;                     //0xc0017448L
	H13Ax_IRIS_147_T                   iris_147;                     //0xc001744cL
	H13Ax_IRIS_148_T                   iris_148;                     //0xc0017450L
	H13Ax_IRIS_149_T                   iris_149;                     //0xc0017454L
	H13Ax_IRIS_150_T                   iris_150;                     //0xc0017458L
	H13Ax_IRIS_151_T                   iris_151;                     //0xc001745cL
	H13Ax_IRIS_152_T                   iris_152;                     //0xc0017460L
	H13Ax_IRIS_153_T                   iris_153;                     //0xc0017464L
	H13Ax_IRIS_154_T                   iris_154;                     //0xc0017468L
	H13Ax_IRIS_155_T                   iris_155;                     //0xc001746cL
	H13Ax_IRIS_156_T                   iris_156;                     //0xc0017470L
	H13Ax_IRIS_157_T                   iris_157;                     //0xc0017474L
	H13Ax_IRIS_158_T                   iris_158;                     //0xc0017478L
	H13Ax_IRIS_159_T                   iris_159;                     //0xc001747cL
	H13Ax_IRIS_160_T                   iris_160;                     //0xc0017480L
	H13Ax_IRIS_161_T                   iris_161;                     //0xc0017484L
	H13Ax_IRIS_162_T                   iris_162;                     //0xc0017488L
	H13Ax_IRIS_163_T                   iris_163;                     //0xc001748cL
	H13Ax_IRIS_164_T                   iris_164;                     //0xc0017490L
	H13Ax_IRIS_165_T                   iris_165;                     //0xc0017494L
	H13Ax_IRIS_166_T                   iris_166;                     //0xc0017498L
	H13Ax_IRIS_167_T                   iris_167;                     //0xc001749cL
	H13Ax_IRIS_168_T                   iris_168;                     //0xc00174a0L
	H13Ax_IRIS_169_T                   iris_169;                     //0xc00174a4L
	H13Ax_IRIS_170_T                   iris_170;                     //0xc00174a8L
	H13Ax_IRIS_171_T                   iris_171;                     //0xc00174acL
	H13Ax_IRIS_172_T                   iris_172;                     //0xc00174b0L
	H13Ax_IRIS_173_T                   iris_173;                     //0xc00174b4L
	H13Ax_IRIS_174_T                   iris_174;                     //0xc00174b8L
	H13Ax_IRIS_175_T                   iris_175;                     //0xc00174bcL
	H13Ax_IRIS_176_T                   iris_176;                     //0xc00174c0L
	H13Ax_IRIS_177_T                   iris_177;                     //0xc00174c4L
	H13Ax_IRIS_178_T                   iris_178;                     //0xc00174c8L
	H13Ax_IRIS_179_T                   iris_179;                     //0xc00174ccL
	H13Ax_IRIS_180_T                   iris_180;                     //0xc00174d0L
	H13Ax_IRIS_181_T                   iris_181;                     //0xc00174d4L
	H13Ax_IRIS_182_T                   iris_182;                     //0xc00174d8L
	H13Ax_IRIS_183_T                   iris_183;                     //0xc00174dcL
	H13Ax_IRIS_184_T                   iris_184;                     //0xc00174e0L
	H13Ax_IRIS_185_T                   iris_185;                     //0xc00174e4L
	H13Ax_IRIS_186_T                   iris_186;                     //0xc00174e8L
	H13Ax_IRIS_187_T                   iris_187;                     //0xc00174ecL
	H13Ax_IRIS_188_T                   iris_188;                     //0xc00174f0L
	H13Ax_IRIS_189_T                   iris_189;                     //0xc00174f4L
	H13Ax_IRIS_190_T                   iris_190;                     //0xc00174f8L
	H13Ax_IRIS_191_T                   iris_191;                     //0xc00174fcL
	H13Ax_IRIS_192_T                   iris_192;                     //0xc0017500L
	H13Ax_IRIS_193_T                   iris_193;                     //0xc0017504L
	H13Ax_IRIS_194_T                   iris_194;                     //0xc0017508L
	H13Ax_IRIS_195_T                   iris_195;                     //0xc001750cL
	H13Ax_IRIS_196_T                   iris_196;                     //0xc0017510L
	H13Ax_IRIS_197_T                   iris_197;                     //0xc0017514L
	H13Ax_IRIS_198_T                   iris_198;                     //0xc0017518L
	H13Ax_IRIS_199_T                   iris_199;                     //0xc001751cL
	H13Ax_IRIS_200_T                   iris_200;                     //0xc0017520L
	H13Ax_IRIS_201_T                   iris_201;                     //0xc0017524L
	H13Ax_IRIS_202_T                   iris_202;                     //0xc0017528L
	H13Ax_IRIS_203_T                   iris_203;                     //0xc001752cL
	H13Ax_IRIS_204_T                   iris_204;                     //0xc0017530L
	H13Ax_IRIS_205_T                   iris_205;                     //0xc0017534L
	H13Ax_IRIS_206_T                   iris_206;                     //0xc0017538L
	H13Ax_IRIS_207_T                   iris_207;                     //0xc001753cL
	H13Ax_IRIS_208_T                   iris_208;                     //0xc0017540L
	H13Ax_IRIS_209_T                   iris_209;                     //0xc0017544L
	H13Ax_IRIS_210_T                   iris_210;                     //0xc0017548L
	H13Ax_IRIS_211_T                   iris_211;                     //0xc001754cL
	H13Ax_IRIS_212_T                   iris_212;                     //0xc0017550L
	H13Ax_IRIS_213_T                   iris_213;                     //0xc0017554L
	H13Ax_IRIS_214_T                   iris_214;                     //0xc0017558L
	H13Ax_IRIS_215_T                   iris_215;                     //0xc001755cL
	H13Ax_IRIS_216_T                   iris_216;                     //0xc0017560L
	H13Ax_IRIS_217_T                   iris_217;                     //0xc0017564L
	H13Ax_IRIS_218_T                   iris_218;                     //0xc0017568L
	H13Ax_IRIS_219_T                   iris_219;                     //0xc001756cL
	H13Ax_IRIS_220_T                   iris_220;                     //0xc0017570L
	H13Ax_IRIS_221_T                   iris_221;                     //0xc0017574L
	H13Ax_IRIS_222_T                   iris_222;                     //0xc0017578L
	H13Ax_IRIS_223_T                   iris_223;                     //0xc001757cL
	H13Ax_IRIS_224_T                   iris_224;                     //0xc0017580L
	H13Ax_IRIS_225_T                   iris_225;                     //0xc0017584L
	H13Ax_IRIS_226_T                   iris_226;                     //0xc0017588L
	H13Ax_IRIS_227_T                   iris_227;                     //0xc001758cL
	H13Ax_IRIS_228_T                   iris_228;                     //0xc0017590L
	H13Ax_IRIS_229_T                   iris_229;                     //0xc0017594L
	H13Ax_IRIS_230_T                   iris_230;                     //0xc0017598L
	H13Ax_IRIS_231_T                   iris_231;                     //0xc001759cL
	H13Ax_IRIS_232_T                   iris_232;                     //0xc00175a0L
	H13Ax_IRIS_233_T                   iris_233;                     //0xc00175a4L
	H13Ax_IRIS_234_T                   iris_234;                     //0xc00175a8L
	H13Ax_IRIS_235_T                   iris_235;                     //0xc00175acL
	H13Ax_IRIS_236_T                   iris_236;                     //0xc00175b0L
	H13Ax_IRIS_237_T                   iris_237;                     //0xc00175b4L
	H13Ax_IRIS_238_T                   iris_238;                     //0xc00175b8L
	H13Ax_IRIS_239_T                   iris_239;                     //0xc00175bcL
	H13Ax_IRIS_240_T                   iris_240;                     //0xc00175c0L
	H13Ax_IRIS_241_T                   iris_241;                     //0xc00175c4L
	H13Ax_IRIS_242_T                   iris_242;                     //0xc00175c8L
	H13Ax_IRIS_243_T                   iris_243;                     //0xc00175ccL
	H13Ax_IRIS_244_T                   iris_244;                     //0xc00175d0L
	H13Ax_IRIS_245_T                   iris_245;                     //0xc00175d4L
	H13Ax_IRIS_246_T                   iris_246;                     //0xc00175d8L
	H13Ax_IRIS_247_T                   iris_247;                     //0xc00175dcL
	H13Ax_IRIS_248_T                   iris_248;                     //0xc00175e0L
	H13Ax_IRIS_249_T                   iris_249;                     //0xc00175e4L
	H13Ax_IRIS_250_T                   iris_250;                     //0xc00175e8L
	H13Ax_IRIS_251_T                   iris_251;                     //0xc00175ecL
	H13Ax_IRIS_252_T                   iris_252;                     //0xc00175f0L
	H13Ax_IRIS_253_T                   iris_253;                     //0xc00175f4L
	H13Ax_IRIS_254_T                   iris_254;                     //0xc00175f8L
	H13Ax_IRIS_255_T                   iris_255;                     //0xc00175fcL
	H13Ax_IRIS_256_T                   iris_256;                     //0xc0017600L
	H13Ax_IRIS_257_T                   iris_257;                     //0xc0017604L
	H13Ax_IRIS_258_T                   iris_258;                     //0xc0017608L
	H13Ax_IRIS_259_T                   iris_259;                     //0xc001760cL
	H13Ax_IRIS_260_T                   iris_260;                     //0xc0017610L
	H13Ax_IRIS_261_T                   iris_261;                     //0xc0017614L
	H13Ax_IRIS_262_T                   iris_262;                     //0xc0017618L
	H13Ax_IRIS_263_T                   iris_263;                     //0xc001761cL
	H13Ax_IRIS_264_T                   iris_264;                     //0xc0017620L
	H13Ax_IRIS_265_T                   iris_265;                     //0xc0017624L
	UINT32                             reserved05[2];                //0xc0017628L~0xc001762cL
	H13Ax_IRIS_266_T                   iris_266;                     //0xc0017630L
	H13Ax_IRIS_267_T                   iris_267;                     //0xc0017634L
	H13Ax_IRIS_268_T                   iris_268;                     //0xc0017638L
	H13Ax_IRIS_269_T                   iris_269;                     //0xc001763cL
	H13Ax_IRIS_270_T                   iris_270;                     //0xc0017640L
	H13Ax_IRIS_271_T                   iris_271;                     //0xc0017644L
	H13Ax_IRIS_272_T                   iris_272;                     //0xc0017648L
	H13Ax_IRIS_273_T                   iris_273;                     //0xc001764cL
	H13Ax_IRIS_274_T                   iris_274;                     //0xc0017650L
	H13Ax_IRIS_275_T                   iris_275;                     //0xc0017654L
	H13Ax_IRIS_276_T                   iris_276;                     //0xc0017658L
	H13Ax_IRIS_277_T                   iris_277;                     //0xc001765cL
	H13Ax_IRIS_278_T                   iris_278;                     //0xc0017660L
	H13Ax_IRIS_279_T                   iris_279;                     //0xc0017664L
	H13Ax_IRIS_280_T                   iris_280;                     //0xc0017668L
	H13Ax_IRIS_281_T                   iris_281;                     //0xc001766cL
	H13Ax_IRIS_282_T                   iris_282;                     //0xc0017670L
	H13Ax_IRIS_283_T                   iris_283;                     //0xc0017674L
	H13Ax_IRIS_284_T                   iris_284;                     //0xc0017678L
	H13Ax_IRIS_285_T                   iris_285;                     //0xc001767cL
	H13Ax_IRIS_286_T                   iris_286;                     //0xc0017680L
	H13Ax_IRIS_287_T                   iris_287;                     //0xc0017684L
	H13Ax_IRIS_288_T                   iris_288;                     //0xc0017688L
	H13Ax_IRIS_289_T                   iris_289;                     //0xc001768cL
	H13Ax_IRIS_290_T                   iris_290;                     //0xc0017690L
	H13Ax_IRIS_291_T                   iris_291;                     //0xc0017694L
	H13Ax_IRIS_292_T                   iris_292;                     //0xc0017698L
	H13Ax_IRIS_293_T                   iris_293;                     //0xc001769cL
	H13Ax_IRIS_294_T                   iris_294;                     //0xc00176a0L
	H13Ax_IRIS_295_T                   iris_295;                     //0xc00176a4L
	H13Ax_IRIS_296_T                   iris_296;                     //0xc00176a8L
	H13Ax_IRIS_297_T                   iris_297;                     //0xc00176acL
	H13Ax_IRIS_298_T                   iris_298;                     //0xc00176b0L
	H13Ax_IRIS_299_T                   iris_299;                     //0xc00176b4L
	H13Ax_IRIS_300_T                   iris_300;                     //0xc00176b8L
	H13Ax_IRIS_301_T                   iris_301;                     //0xc00176bcL
	H13Ax_IRIS_302_T                   iris_302;                     //0xc00176c0L
	H13Ax_IRIS_303_T                   iris_303;                     //0xc00176c4L
	H13Ax_IRIS_304_T                   iris_304;                     //0xc00176c8L
	H13Ax_IRIS_305_T                   iris_305;                     //0xc00176ccL
	H13Ax_IRIS_306_T                   iris_306;                     //0xc00176d0L
	H13Ax_IRIS_307_T                   iris_307;                     //0xc00176d4L
	UINT32                             reserved06[74];               //0xc00176d8L~0xc00177fcL
	H13Ax_VBI_001_T                    vbi_001;                      //0xc0017800L
	H13Ax_VBI_002_T                    vbi_002;                      //0xc0017804L
	H13Ax_VBI_003_T                    vbi_003;                      //0xc0017808L
	H13Ax_VBI_004_T                    vbi_004;                      //0xc001780cL
	H13Ax_VBI_005_T                    vbi_005;                      //0xc0017810L
	H13Ax_VBI_006_T                    vbi_006;                      //0xc0017814L
	H13Ax_VBI_007_T                    vbi_007;                      //0xc0017818L
	H13Ax_VBI_008_T                    vbi_008;                      //0xc001781cL
	H13Ax_VBI_009_T                    vbi_009;                      //0xc0017820L
	H13Ax_VBI_010_T                    vbi_010;                      //0xc0017824L
	H13Ax_VBI_011_T                    vbi_011;                      //0xc0017828L
	H13Ax_VBI_012_T                    vbi_012;                      //0xc001782cL
	H13Ax_VBI_013_T                    vbi_013;                      //0xc0017830L
	H13Ax_VBI_014_T                    vbi_014;                      //0xc0017834L
	H13Ax_VBI_015_T                    vbi_015;                      //0xc0017838L
	H13Ax_VBI_016_T                    vbi_016;                      //0xc001783cL
	H13Ax_VBI_017_T                    vbi_017;                      //0xc0017840L
	H13Ax_VBI_018_T                    vbi_018;                      //0xc0017844L
	H13Ax_VBI_019_T                    vbi_019;                      //0xc0017848L
	H13Ax_VBI_020_T                    vbi_020;                      //0xc001784cL
	H13Ax_VBI_021_T                    vbi_021;                      //0xc0017850L
	H13Ax_VBI_022_T                    vbi_022;                      //0xc0017854L
	H13Ax_VBI_023_T                    vbi_023;                      //0xc0017858L
	UINT32                             reserved07[19];               //0xc001785cL~0xc00178a4L
	H13Ax_IRIS_MIF_GMUA_MON_001_T      iris_mif_gmua_mon_001;        //0xc00178a8L
	H13Ax_IRIS_MIF_GMUA_MON_002_T      iris_mif_gmua_mon_002;        //0xc00178acL
	H13Ax_IRIS_MIF_GMUA_MON_003_T      iris_mif_gmua_mon_003;        //0xc00178b0L
	H13Ax_IRIS_MIF_GMUA_MON_004_T      iris_mif_gmua_mon_004;        //0xc00178b4L
	H13Ax_IRIS_MIF_GMUA_MON_005_T      iris_mif_gmua_mon_005;        //0xc00178b8L
	H13Ax_IRIS_MIF_GMUA_MON_006_T      iris_mif_gmua_mon_006;        //0xc00178bcL
	H13Ax_IRIS_MIF_GMUA_MON_007_T      iris_mif_gmua_mon_007;        //0xc00178c0L
	H13Ax_IRIS_MIF_GMUA_MON_008_T      iris_mif_gmua_mon_008;        //0xc00178c4L
	H13Ax_IRIS_MIF_GMUA_MON_009_T      iris_mif_gmua_mon_009;        //0xc00178c8L
	H13Ax_IRIS_MIF_GMUA_MON_010_T      iris_mif_gmua_mon_010;        //0xc00178ccL
	H13Ax_IRIS_MIF_GMUA_MON_011_T      iris_mif_gmua_mon_011;        //0xc00178d0L
	H13Ax_IRIS_MIF_GMUA_MON_012_T      iris_mif_gmua_mon_012;        //0xc00178d4L
	H13Ax_IRIS_MIF_GMUA_MON_013_T      iris_mif_gmua_mon_013;        //0xc00178d8L
	H13Ax_IRIS_MIF_GMUA_MON_014_T      iris_mif_gmua_mon_014;        //0xc00178dcL
	H13Ax_IRIS_MIF_GMUA_MON_015_T      iris_mif_gmua_mon_015;        //0xc00178e0L
	H13Ax_IRIS_MIF_GMUA_MON_016_T      iris_mif_gmua_mon_016;        //0xc00178e4L
	H13Ax_IRIS_MIF_GMUA_MON_017_T      iris_mif_gmua_mon_017;        //0xc00178e8L
	H13Ax_IRIS_MIF_GMUA_MON_018_T      iris_mif_gmua_mon_018;        //0xc00178ecL
	H13Ax_IRIS_MIF_GMUA_MON_019_T      iris_mif_gmua_mon_019;        //0xc00178f0L
	H13Ax_IRIS_MIF_GMUA_MON_020_T      iris_mif_gmua_mon_020;        //0xc00178f4L
	H13Ax_IRIS_MIF_GMUA_MON_021_T      iris_mif_gmua_mon_021;        //0xc00178f8L
	H13Ax_IRIS_MIF_GMUA_MON_022_T      iris_mif_gmua_mon_022;        //0xc00178fcL
	H13Ax_IRIS_HIF_CTRL0_001_T         iris_hif_ctrl0_001;           //0xc0017900L
	H13Ax_IRIS_HIF_CTRL1_001_T         iris_hif_ctrl1_001;           //0xc0017904L
}DE_CVD_REG_H13Ax_T;
/*
 * @{
 * Naming for register pointer.
 * gpRealRegCVD_H13Ax : real register of VPORT_L9B0.
 * gpRegCVD_H13Ax     : shadow register.
 *
 * @def CVD_H13Ax_RdFL: Read  FLushing : Shadow <- Real.
 * @def CVD_H13Ax_WrFL: Write FLushing : Shadow -> Real.
 * @def CVD_H13Ax_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def CVD_H13Ax_Wr  : Write whole register(UINT32) from Shadow register.
 * @def CVD_H13Ax_Rd01 ~ CVD_H13Ax_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def CVD_H13Ax_Wr01 ~ CVD_H13Ax_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define CVD_H13Ax_RdFL(_r)			((gpRegCVD_H13Ax->_r)=(gpRealRegCVD_H13Ax->_r))
#define CVD_H13Ax_WrFL(_r)			((gpRealRegCVD_H13Ax->_r)=(gpRegCVD_H13Ax->_r))

#define CVD_H13Ax_Rd(_r)			*((UINT32*)(&(gpRegCVD_H13Ax->_r)))
#define CVD_H13Ax_Wr(_r,_v)			((CVD_H13Ax_Rd(_r))=((UINT32)(_v)))

#define CVD_H13Ax_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
								} while(0)

#define CVD_H13Ax_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
								} while(0)

#define CVD_H13Ax_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
								} while(0)

#define CVD_H13Ax_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
								} while(0)

#define CVD_H13Ax_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
								} while(0)

#define CVD_H13Ax_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
								} while(0)

#define CVD_H13Ax_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
								} while(0)

#define CVD_H13Ax_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
								} while(0)

#define CVD_H13Ax_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
								} while(0)

#define CVD_H13Ax_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
									(_v10) = (gpRegCVD_H13Ax->_r._f10);				\
								} while(0)

#define CVD_H13Ax_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
									(_v10) = (gpRegCVD_H13Ax->_r._f10);				\
									(_v11) = (gpRegCVD_H13Ax->_r._f11);				\
								} while(0)

#define CVD_H13Ax_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
									(_v10) = (gpRegCVD_H13Ax->_r._f10);				\
									(_v11) = (gpRegCVD_H13Ax->_r._f11);				\
									(_v12) = (gpRegCVD_H13Ax->_r._f12);				\
								} while(0)

#define CVD_H13Ax_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
									(_v10) = (gpRegCVD_H13Ax->_r._f10);				\
									(_v11) = (gpRegCVD_H13Ax->_r._f11);				\
									(_v12) = (gpRegCVD_H13Ax->_r._f12);				\
									(_v13) = (gpRegCVD_H13Ax->_r._f13);				\
								} while(0)

#define CVD_H13Ax_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
									(_v10) = (gpRegCVD_H13Ax->_r._f10);				\
									(_v11) = (gpRegCVD_H13Ax->_r._f11);				\
									(_v12) = (gpRegCVD_H13Ax->_r._f12);				\
									(_v13) = (gpRegCVD_H13Ax->_r._f13);				\
									(_v14) = (gpRegCVD_H13Ax->_r._f14);				\
								} while(0)

#define CVD_H13Ax_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
									(_v10) = (gpRegCVD_H13Ax->_r._f10);				\
									(_v11) = (gpRegCVD_H13Ax->_r._f11);				\
									(_v12) = (gpRegCVD_H13Ax->_r._f12);				\
									(_v13) = (gpRegCVD_H13Ax->_r._f13);				\
									(_v14) = (gpRegCVD_H13Ax->_r._f14);				\
									(_v15) = (gpRegCVD_H13Ax->_r._f15);				\
								} while(0)

#define CVD_H13Ax_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Ax->_r._f01);				\
									(_v02) = (gpRegCVD_H13Ax->_r._f02);				\
									(_v03) = (gpRegCVD_H13Ax->_r._f03);				\
									(_v04) = (gpRegCVD_H13Ax->_r._f04);				\
									(_v05) = (gpRegCVD_H13Ax->_r._f05);				\
									(_v06) = (gpRegCVD_H13Ax->_r._f06);				\
									(_v07) = (gpRegCVD_H13Ax->_r._f07);				\
									(_v08) = (gpRegCVD_H13Ax->_r._f08);				\
									(_v09) = (gpRegCVD_H13Ax->_r._f09);				\
									(_v10) = (gpRegCVD_H13Ax->_r._f10);				\
									(_v11) = (gpRegCVD_H13Ax->_r._f11);				\
									(_v12) = (gpRegCVD_H13Ax->_r._f12);				\
									(_v13) = (gpRegCVD_H13Ax->_r._f13);				\
									(_v14) = (gpRegCVD_H13Ax->_r._f14);				\
									(_v15) = (gpRegCVD_H13Ax->_r._f15);				\
									(_v16) = (gpRegCVD_H13Ax->_r._f16);				\
								} while(0)


#define CVD_H13Ax_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
								} while(0)

#define CVD_H13Ax_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
								} while(0)

#define CVD_H13Ax_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
								} while(0)

#define CVD_H13Ax_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
								} while(0)

#define CVD_H13Ax_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
								} while(0)

#define CVD_H13Ax_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
								} while(0)

#define CVD_H13Ax_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
								} while(0)

#define CVD_H13Ax_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
								} while(0)

#define CVD_H13Ax_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
								} while(0)

#define CVD_H13Ax_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
									(gpRegCVD_H13Ax->_r._f10) = (_v10);				\
								} while(0)

#define CVD_H13Ax_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
									(gpRegCVD_H13Ax->_r._f10) = (_v10);				\
									(gpRegCVD_H13Ax->_r._f11) = (_v11);				\
								} while(0)

#define CVD_H13Ax_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
									(gpRegCVD_H13Ax->_r._f10) = (_v10);				\
									(gpRegCVD_H13Ax->_r._f11) = (_v11);				\
									(gpRegCVD_H13Ax->_r._f12) = (_v12);				\
								} while(0)

#define CVD_H13Ax_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
									(gpRegCVD_H13Ax->_r._f10) = (_v10);				\
									(gpRegCVD_H13Ax->_r._f11) = (_v11);				\
									(gpRegCVD_H13Ax->_r._f12) = (_v12);				\
									(gpRegCVD_H13Ax->_r._f13) = (_v13);				\
								} while(0)

#define CVD_H13Ax_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
									(gpRegCVD_H13Ax->_r._f10) = (_v10);				\
									(gpRegCVD_H13Ax->_r._f11) = (_v11);				\
									(gpRegCVD_H13Ax->_r._f12) = (_v12);				\
									(gpRegCVD_H13Ax->_r._f13) = (_v13);				\
									(gpRegCVD_H13Ax->_r._f14) = (_v14);				\
								} while(0)

#define CVD_H13Ax_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
									(gpRegCVD_H13Ax->_r._f10) = (_v10);				\
									(gpRegCVD_H13Ax->_r._f11) = (_v11);				\
									(gpRegCVD_H13Ax->_r._f12) = (_v12);				\
									(gpRegCVD_H13Ax->_r._f13) = (_v13);				\
									(gpRegCVD_H13Ax->_r._f14) = (_v14);				\
									(gpRegCVD_H13Ax->_r._f15) = (_v15);				\
								} while(0)

#define CVD_H13Ax_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegCVD_H13Ax->_r._f01) = (_v01);				\
									(gpRegCVD_H13Ax->_r._f02) = (_v02);				\
									(gpRegCVD_H13Ax->_r._f03) = (_v03);				\
									(gpRegCVD_H13Ax->_r._f04) = (_v04);				\
									(gpRegCVD_H13Ax->_r._f05) = (_v05);				\
									(gpRegCVD_H13Ax->_r._f06) = (_v06);				\
									(gpRegCVD_H13Ax->_r._f07) = (_v07);				\
									(gpRegCVD_H13Ax->_r._f08) = (_v08);				\
									(gpRegCVD_H13Ax->_r._f09) = (_v09);				\
									(gpRegCVD_H13Ax->_r._f10) = (_v10);				\
									(gpRegCVD_H13Ax->_r._f11) = (_v11);				\
									(gpRegCVD_H13Ax->_r._f12) = (_v12);				\
									(gpRegCVD_H13Ax->_r._f13) = (_v13);				\
									(gpRegCVD_H13Ax->_r._f14) = (_v14);				\
									(gpRegCVD_H13Ax->_r._f15) = (_v15);				\
									(gpRegCVD_H13Ax->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after CVD_H13Ax_Wind(), 1 for CVD_H13Ax_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * CVD_H13Ax_Rind : General indexed register Read.(
 * CVD_H13Ax_Wind : General indexed register Read.
 *
 * CVD_H13Ax_Ridx : For 'index', 'rw', 'load' field name
 * CVD_H13Ax_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define CVD_H13Ax_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								CVD_H13Ax_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								CVD_H13Ax_WrFL(_r);									\
								CVD_H13Ax_RdFL(_r);									\
								CVD_H13Ax_Rd01(_r,_fname,_fval);						\
							} while (0)

#define CVD_H13Ax_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				CVD_H13Ax_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define CVD_H13Ax_Ridx(_r, _ival, _fname, _fval)	CVD_H13Ax_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define CVD_H13Ax_Widx(_r, _ival, _fname, _fval)	CVD_H13Ax_Wind(_r,load,rw,index,_ival,_fname,_fval)

#ifdef __cplusplus
}
#endif
#endif
