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


#ifndef _DE_CVD_REG_H13Bx_H_
#define _DE_CVD_REG_H13Bx_H_
/*-----------------------------------------------------------------------------
		0x4100 cvd_intr1_enable0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_enable0                  : 32;  // 31: 0     
} H13Bx_CVD_INTR1_ENABLE0_T;

/*-----------------------------------------------------------------------------
		0x4104 cvd_intr1_intr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_chromalock_fal                 : 1 ;  //     0     
	UINT32 cvd_chromalock_ris                 : 1 ;  //     1     
	UINT32 cvd_vlock_fal                      : 1 ;  //     2     
	UINT32 cvd_vlock_ris                      : 1 ;  //     3     
	UINT32 cvd_hlock_fal                      : 1 ;  //     4     
	UINT32 cvd_hlock_ris                      : 1 ;  //     5     
	UINT32 cvd_no_signal_fal                  : 1 ;  //     6     
	UINT32 cvd_no_signal_ris                  : 1 ;  //     7     
	UINT32 cvd_noburst_detected_fal           : 1 ;  //     8     
	UINT32 cvd_noburst_detected_ris           : 1 ;  //     9     
	UINT32 cvd_vnon_standard_fal              : 1 ;  //    10     
	UINT32 cvd_vnon_standard_ris              : 1 ;  //    11     
	UINT32 cvd_hnon_standard_fal              : 1 ;  //    12     
	UINT32 cvd_hnon_standard_ris              : 1 ;  //    13     
	UINT32 cvd_proscan_detected_fal           : 1 ;  //    14     
	UINT32 cvd_proscan_detected_ris           : 1 ;  //    15     
	UINT32 cvd_vcr_fal                        : 1 ;  //    16     
	UINT32 cvd_vcr_ris                        : 1 ;  //    17     
	UINT32 cvd_noisy_fal                      : 1 ;  //    18     
	UINT32 cvd_noisy_ris                      : 1 ;  //    19     
	UINT32 cvd_vline_625_detected_fal         : 1 ;  //    20     
	UINT32 cvd_vline_625_detected_ris         : 1 ;  //    21     
	UINT32 cvd_secam_detected_fal             : 1 ;  //    22     
	UINT32 cvd_secam_detected_ris             : 1 ;  //    23     
	UINT32 cvd_pal_detected_fal               : 1 ;  //    24     
	UINT32 cvd_pal_detected_ris               : 1 ;  //    25     
	UINT32 cvd_vs_fal                         : 1 ;  //    26     
	UINT32 cvd_vs_ris                         : 1 ;  //    27     
	UINT32 cvd_ext_locked_fal                 : 1 ;  //    28     
	UINT32 cvd_ext_locked_ris                 : 1 ;  //    29     
} H13Bx_CVD_INTR1_INTR0_T;

/*-----------------------------------------------------------------------------
		0x4108 cvd_intr1_clear0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_clear0                   : 32;  // 31: 0     
} H13Bx_CVD_INTR1_CLEAR0_T;

/*-----------------------------------------------------------------------------
		0x410c cvd_intr1_enable1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_enable1                  : 32;  // 31: 0     
} H13Bx_CVD_INTR1_ENABLE1_T;

/*-----------------------------------------------------------------------------
		0x4110 cvd_intr1_intr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vbi_wss_rdy                        : 1 ;  //     0     
	UINT32 vbi_cc_rdy                         : 1 ;  //     1     
	UINT32 vbi_data_in                        : 1 ;  //     2     
	UINT32 cvd_fb_rst                         : 1 ;  //     3     
	UINT32 cvd_mif_err                        : 1 ;  //     4     
	UINT32 cs_chromalock_fal                  : 1 ;  //     5     
	UINT32 cs_chromalock_ris                  : 1 ;  //     6     
	UINT32 cs_pal_detected_fal                : 1 ;  //     7     
	UINT32 cs_pal_detected_ris                : 1 ;  //     8     
	UINT32 cs_secam_detected_fal              : 1 ;  //     9     
	UINT32 cs_secam_detected_ris              : 1 ;  //    10     
	UINT32 cs1_chromalock_fal                 : 1 ;  //    11     
	UINT32 cs1_chromalock_ris                 : 1 ;  //    12     
	UINT32 cs1_pal_detected_fal               : 1 ;  //    13     
	UINT32 cs1_pal_detected_ris               : 1 ;  //    14     
	UINT32 cs1_secam_detected_fal             : 1 ;  //    15     
	UINT32 cs1_secam_detected_ris             : 1 ;  //    16     
	UINT32 estfrdp_err_flag_fal               : 1 ;  //    17     
	UINT32 estfrdp_err_flag_ris               : 1 ;  //    18     
	UINT32 vdetect_vcount_fal                 : 1 ;  //    19     
	UINT32 vdetect_vcount_ris                 : 1 ;  //    20     
	UINT32 cvd_mif_err_md                     : 1 ;  //    21     
} H13Bx_CVD_INTR1_INTR1_T;

/*-----------------------------------------------------------------------------
		0x4114 cvd_intr1_clear1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr1_clear1                   : 32;  // 31: 0     
} H13Bx_CVD_INTR1_CLEAR1_T;

/*-----------------------------------------------------------------------------
		0x4118 cvd_intr2_enable0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_enable0                  : 32;  // 31: 0     
} H13Bx_CVD_INTR2_ENABLE0_T;

/*-----------------------------------------------------------------------------
		0x411c cvd_intr2_intr0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_chromalock_fal                 : 1 ;  //     0     
	UINT32 cvd_chromalock_ris                 : 1 ;  //     1     
	UINT32 cvd_vlock_fal                      : 1 ;  //     2     
	UINT32 cvd_vlock_ris                      : 1 ;  //     3     
	UINT32 cvd_hlock_fal                      : 1 ;  //     4     
	UINT32 cvd_hlock_ris                      : 1 ;  //     5     
	UINT32 cvd_no_signal_fal                  : 1 ;  //     6     
	UINT32 cvd_no_signal_ris                  : 1 ;  //     7     
	UINT32 cvd_noburst_detected_fal           : 1 ;  //     8     
	UINT32 cvd_noburst_detected_ris           : 1 ;  //     9     
	UINT32 cvd_vnon_standard_fal              : 1 ;  //    10     
	UINT32 cvd_vnon_standard_ris              : 1 ;  //    11     
	UINT32 cvd_hnon_standard_fal              : 1 ;  //    12     
	UINT32 cvd_hnon_standard_ris              : 1 ;  //    13     
	UINT32 cvd_proscan_detected_fal           : 1 ;  //    14     
	UINT32 cvd_proscan_detected_ris           : 1 ;  //    15     
	UINT32 cvd_vcr_fal                        : 1 ;  //    16     
	UINT32 cvd_vcr_ris                        : 1 ;  //    17     
	UINT32 cvd_noisy_fal                      : 1 ;  //    18     
	UINT32 cvd_noisy_ris                      : 1 ;  //    19     
	UINT32 cvd_vline_625_detected_fal         : 1 ;  //    20     
	UINT32 cvd_vline_625_detected_ris         : 1 ;  //    21     
	UINT32 cvd_secam_detected_fal             : 1 ;  //    22     
	UINT32 cvd_secam_detected_ris             : 1 ;  //    23     
	UINT32 cvd_pal_detected_fal               : 1 ;  //    24     
	UINT32 cvd_pal_detected_ris               : 1 ;  //    25     
	UINT32 cvd_vs_fal                         : 1 ;  //    26     
	UINT32 cvd_vs_ris                         : 1 ;  //    27     
	UINT32 cvd_ext_locked_fal                 : 1 ;  //    28     
	UINT32 cvd_ext_locked_ris                 : 1 ;  //    29     
} H13Bx_CVD_INTR2_INTR0_T;

/*-----------------------------------------------------------------------------
		0x4120 cvd_intr2_clear0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_clear0                   : 32;  // 31: 0     
} H13Bx_CVD_INTR2_CLEAR0_T;

/*-----------------------------------------------------------------------------
		0x4124 cvd_intr2_enable1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_enable1                  : 32;  // 31: 0     
} H13Bx_CVD_INTR2_ENABLE1_T;

/*-----------------------------------------------------------------------------
		0x4128 cvd_intr2_intr1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vbi_wss_rdy                        : 1 ;  //     0     
	UINT32 vbi_cc_rdy                         : 1 ;  //     1     
	UINT32 vbi_data_in                        : 1 ;  //     2     
	UINT32 cvd_fb_rst                         : 1 ;  //     3     
	UINT32 cvd_mif_err                        : 1 ;  //     4     
	UINT32 cs_chromalock_fal                  : 1 ;  //     5     
	UINT32 cs_chromalock_ris                  : 1 ;  //     6     
	UINT32 cs_pal_detected_fal                : 1 ;  //     7     
	UINT32 cs_pal_detected_ris                : 1 ;  //     8     
	UINT32 cs_secam_detected_fal              : 1 ;  //     9     
	UINT32 cs_secam_detected_ris              : 1 ;  //    10     
	UINT32 cs1_chromalock_fal                 : 1 ;  //    11     
	UINT32 cs1_chromalock_ris                 : 1 ;  //    12     
	UINT32 cs1_pal_detected_fal               : 1 ;  //    13     
	UINT32 cs1_pal_detected_ris               : 1 ;  //    14     
	UINT32 cs1_secam_detected_fal             : 1 ;  //    15     
	UINT32 cs1_secam_detected_ris             : 1 ;  //    16     
	UINT32 estfrdp_err_flag_fal               : 1 ;  //    17     
	UINT32 estfrdp_err_flag_ris               : 1 ;  //    18     
	UINT32 vdetect_vcount_fal                 : 1 ;  //    19     
	UINT32 vdetect_vcount_ris                 : 1 ;  //    20     
	UINT32 cvd_mif_err_md                     : 1 ;  //    21     
} H13Bx_CVD_INTR2_INTR1_T;

/*-----------------------------------------------------------------------------
		0x412c cvd_intr2_clear1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 cvd_intr2_clear1                   : 32;  // 31: 0     
} H13Bx_CVD_INTR2_CLEAR1_T;

/*-----------------------------------------------------------------------------
		0x4130 top_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 iris_ver                           : 32;  // 31: 0     
} H13Bx_TOP_001_T;

/*-----------------------------------------------------------------------------
		0x4140 iris_mif_gmua_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_comb3_buffer_size              : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_001_T;

/*-----------------------------------------------------------------------------
		0x4144 iris_mif_gmua_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld1_init_rd_pel               : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_002_T;

/*-----------------------------------------------------------------------------
		0x4148 iris_mif_gmua_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld2_init_rd_pel               : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_003_T;

/*-----------------------------------------------------------------------------
		0x414c iris_mif_gmua_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld3_init_rd_pel               : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_004_T;

/*-----------------------------------------------------------------------------
		0x4150 iris_mif_gmua_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld4_init_rd_pel               : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_005_T;

/*-----------------------------------------------------------------------------
		0x4154 iris_mif_gmua_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_to_cnt                    : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_gmau_cmd_dly_cnt               : 4 ;  // 15:12     
} H13Bx_IRIS_MIF_GMUA_006_T;

/*-----------------------------------------------------------------------------
		0x4158 iris_mif_gmua_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_cmd_base                  : 32;  // 31: 0     
} H13Bx_IRIS_MIF_GMUA_007_T;

/*-----------------------------------------------------------------------------
		0x415c iris_mif_gmua_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_ctrl                       : 32;  // 31: 0     
} H13Bx_IRIS_MIF_GMUA_008_T;

/*-----------------------------------------------------------------------------
		0x4160 iris_mif_gmua_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_endian_sw                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_gmau_cmd_pri2                  : 4 ;  //  7: 4     
	UINT32 reg_gmau_cmd_pri1                  : 4 ;  // 11: 8     
} H13Bx_IRIS_MIF_GMUA_009_T;

/*-----------------------------------------------------------------------------
		0x4164 iris_mif_gmua_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_stride_size               : 32;  // 31: 0     
} H13Bx_IRIS_MIF_GMUA_010_T;

/*-----------------------------------------------------------------------------
		0x4168 fastblank_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_latency                     : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_fb_blend_ratio                 : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_fb_lmflag_off                  : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_fb_en                          : 1 ;  //    20     
} H13Bx_FASTBLANK_001_T;

/*-----------------------------------------------------------------------------
		0x416c fastblank_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef1                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_fb_csc_coef0                   : 15;  // 30:16     
} H13Bx_FASTBLANK_002_T;

/*-----------------------------------------------------------------------------
		0x4170 fastblank_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef3                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_fb_csc_coef2                   : 15;  // 30:16     
} H13Bx_FASTBLANK_003_T;

/*-----------------------------------------------------------------------------
		0x4174 fastblank_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef5                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_fb_csc_coef4                   : 15;  // 30:16     
} H13Bx_FASTBLANK_004_T;

/*-----------------------------------------------------------------------------
		0x4178 fastblank_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef7                   : 15;  // 14: 0     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_fb_csc_coef6                   : 15;  // 30:16     
} H13Bx_FASTBLANK_005_T;

/*-----------------------------------------------------------------------------
		0x417c fastblank_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_coef8                   : 15;  // 14: 0     
} H13Bx_FASTBLANK_006_T;

/*-----------------------------------------------------------------------------
		0x4180 fastblank_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_ofst1                   : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_fb_csc_ofst0                   : 11;  // 22:12     
} H13Bx_FASTBLANK_007_T;

/*-----------------------------------------------------------------------------
		0x4184 fastblank_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_csc_ofst3                   : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_fb_csc_ofst2                   : 11;  // 22:12     
} H13Bx_FASTBLANK_008_T;

/*-----------------------------------------------------------------------------
		0x4188 fastblank_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_vstart_odd                  : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_fb_csc_ofst5                   : 11;  // 18: 8     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_fb_csc_ofst4                   : 11;  // 30:20     
} H13Bx_FASTBLANK_009_T;

/*-----------------------------------------------------------------------------
		0x418c fastblank_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_hstart                      : 8 ;  //  7: 0     
	UINT32 reg_fb_height_half                 : 8 ;  // 15: 8     
	UINT32 reg_fb_vstart_even                 : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_fb_ch3_br_0th                  : 2 ;  // 25:24     
} H13Bx_FASTBLANK_010_T;

/*-----------------------------------------------------------------------------
		0x4190 iris_de_ctrl_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_iris_hsout_width               : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_iris_static_de_start           : 12;  // 23:12     
	UINT32 reg_iris_static_de                 : 1 ;  //    24     
} H13Bx_IRIS_DE_CTRL_001_T;

/*-----------------------------------------------------------------------------
		0x4194 vbi_ctrl_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_little_endian              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_vbi_bits_swap                  : 1 ;  //     4     
} H13Bx_VBI_CTRL_001_T;

/*-----------------------------------------------------------------------------
		0x4198 vbi_ctrl_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi0_start_addr                : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_vbi0_data_cnt                  : 12;  // 23:12     
	UINT32 reg_vbi_buffer_number              : 2 ;  // 25:24     
} H13Bx_VBI_CTRL_002_T;

/*-----------------------------------------------------------------------------
		0x419c vbi_ctrl_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi0_end_addr                  : 10;  //  9: 0     
} H13Bx_VBI_CTRL_003_T;

/*-----------------------------------------------------------------------------
		0x41a0 vbi_ctrl_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi1_start_addr                : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_vbi1_data_cnt                  : 12;  // 23:12     
} H13Bx_VBI_CTRL_004_T;

/*-----------------------------------------------------------------------------
		0x41a4 vbi_ctrl_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi1_end_addr                  : 10;  //  9: 0     
} H13Bx_VBI_CTRL_005_T;

/*-----------------------------------------------------------------------------
		0x41a8 vbi_ctrl_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi2_start_addr                : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_vbi2_data_cnt                  : 12;  // 23:12     
} H13Bx_VBI_CTRL_006_T;

/*-----------------------------------------------------------------------------
		0x41ac vbi_ctrl_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi2_end_addr                  : 10;  //  9: 0     
} H13Bx_VBI_CTRL_007_T;

/*-----------------------------------------------------------------------------
		0x41b0 vbi_ctrl_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi3_start_addr                : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_vbi3_data_cnt                  : 12;  // 23:12     
} H13Bx_VBI_CTRL_008_T;

/*-----------------------------------------------------------------------------
		0x41b4 vbi_ctrl_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi3_end_addr                  : 10;  //  9: 0     
} H13Bx_VBI_CTRL_009_T;

/*-----------------------------------------------------------------------------
		0x41b8 vbi_ctrl_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_intr_sel                   : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_vbi_even_vcount                : 10;  // 13: 4     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_vbi_odd_vcount                 : 10;  // 25:16     
} H13Bx_VBI_CTRL_010_T;

/*-----------------------------------------------------------------------------
		0x41c0 fast_blank_status_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_state                       : 1 ;  //     0     
} H13Bx_FAST_BLANK_STATUS_001_T;

/*-----------------------------------------------------------------------------
		0x41c4 cvbsafe_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cvbs2_gc_int                   : 4 ;  //  3: 0     
	UINT32 reg_cvbs1_gc_int                   : 4 ;  //  7: 4     
	UINT32 reg_iris_fix_gain                  : 1 ;  //     8     
} H13Bx_CVBSAFE_001_T;

/*-----------------------------------------------------------------------------
		0x41c8 iris_mif_gmua_md_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_comb3_buffer_size_md           : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_MD_001_T;

/*-----------------------------------------------------------------------------
		0x41cc iris_mif_gmua_md_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld1_init_rd_pel_md            : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_MD_002_T;

/*-----------------------------------------------------------------------------
		0x41d0 iris_mif_gmua_md_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld2_init_rd_pel_md            : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_MD_003_T;

/*-----------------------------------------------------------------------------
		0x41d4 iris_mif_gmua_md_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld3_init_rd_pel_md            : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_MD_004_T;

/*-----------------------------------------------------------------------------
		0x41d8 iris_mif_gmua_md_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fld4_init_rd_pel_md            : 23;  // 22: 0     
} H13Bx_IRIS_MIF_GMUA_MD_005_T;

/*-----------------------------------------------------------------------------
		0x41dc iris_mif_gmua_md_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_to_cnt_md                 : 10;  //  9: 0     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_gmau_cmd_dly_cnt_md            : 4 ;  // 15:12     
} H13Bx_IRIS_MIF_GMUA_MD_006_T;

/*-----------------------------------------------------------------------------
		0x41e0 iris_mif_gmua_md_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_cmd_base_md               : 32;  // 31: 0     
} H13Bx_IRIS_MIF_GMUA_MD_007_T;

/*-----------------------------------------------------------------------------
		0x41e4 iris_mif_gmua_md_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_endian_sw_md              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_gmau_cmd_pri2_md               : 4 ;  //  7: 4     
	UINT32 reg_gmau_cmd_pri1_md               : 4 ;  // 11: 8     
} H13Bx_IRIS_MIF_GMUA_MD_008_T;

/*-----------------------------------------------------------------------------
		0x41e8 iris_mif_gmua_md_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gmau_stride_size_md            : 32;  // 31: 0     
} H13Bx_IRIS_MIF_GMUA_MD_009_T;

/*-----------------------------------------------------------------------------
		0x41ec iris_mif_gmua_md_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mif_ctrl_md                    : 32;  // 31: 0     
} H13Bx_IRIS_MIF_GMUA_MD_010_T;

/*-----------------------------------------------------------------------------
		0x41f0 iris_mif_gmua_md_011 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lbadrgen_ctrl_md               : 32;  // 31: 0     
} H13Bx_IRIS_MIF_GMUA_MD_011_T;

/*-----------------------------------------------------------------------------
		0x4200 iris_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_proscan_detected               : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_mv_colourstripes               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_mv_vbi_detected                : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_chromalock                     : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vlock                          : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_hlock                          : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_no_signal                      : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_status_vdetect_vcount_chg      : 1 ;  //    28     
} H13Bx_IRIS_001_T;

/*-----------------------------------------------------------------------------
		0x4204 iris_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_noisy                          : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_vline_625_detected             : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_secam_detected                 : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_pal_detected                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_status_comb3d_off              : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_noburst_detected               : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_vnon_standard                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_hnon_standard                  : 1 ;  //    28     
} H13Bx_IRIS_002_T;

/*-----------------------------------------------------------------------------
		0x4208 iris_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vcr_rew                        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_vcr_ff                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_vcr_trick                      : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_vcr                            : 1 ;  //    12     
} H13Bx_IRIS_003_T;

/*-----------------------------------------------------------------------------
		0x420c iris_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_burst_mag               : 16;  // 15: 0     
	UINT32 reg_status_hsync_width             : 8 ;  // 23:16     
} H13Bx_IRIS_004_T;

/*-----------------------------------------------------------------------------
		0x4210 iris_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_hdto_inc                : 30;  // 29: 0     
} H13Bx_IRIS_005_T;

/*-----------------------------------------------------------------------------
		0x4214 iris_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_cdto_inc                : 30;  // 29: 0     
} H13Bx_IRIS_006_T;

/*-----------------------------------------------------------------------------
		0x4218 iris_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_cmag                    : 8 ;  //  7: 0     
	UINT32 reg_status_agc_gain                : 16;  // 23: 8     
} H13Bx_IRIS_007_T;

/*-----------------------------------------------------------------------------
		0x421c iris_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_sync_height             : 8 ;  //  7: 0     
	UINT32 reg_status_cordic_freq             : 8 ;  // 15: 8     
	UINT32 reg_status_cgain                   : 14;  // 29:16     
} H13Bx_IRIS_008_T;

/*-----------------------------------------------------------------------------
		0x4220 iris_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_nstd                    : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_status_vbi_slicer_lvl          : 8 ;  // 19:12     
	UINT32 reg_status_noise                   : 10;  // 29:20     
} H13Bx_IRIS_009_T;

/*-----------------------------------------------------------------------------
		0x4224 iris_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_diff_sync_blank         : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_status_peak_gain               : 13;  // 28:16     
} H13Bx_IRIS_010_T;

/*-----------------------------------------------------------------------------
		0x4228 iris_011 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_cline_flag               : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_status_blank_level_diff        : 12;  // 15: 4     
	UINT32 reg_status_sync_level_diff         : 12;  // 27:16     
} H13Bx_IRIS_011_T;

/*-----------------------------------------------------------------------------
		0x4230 iris_012 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cvd_soft_reset                 : 1 ;  //     0     
} H13Bx_IRIS_012_T;

/*-----------------------------------------------------------------------------
		0x4234 iris_013 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hv_delay                       : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_hpixel                         : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_vline_625                      : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_colour_mode                    : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_yc_src                         : 1 ;  //    16     
} H13Bx_IRIS_013_T;

/*-----------------------------------------------------------------------------
		0x4238 iris_014 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cv_inv                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cv_src                         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_luma_notch_bw                  : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_chroma_bw_lo                   : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_chroma_burst5or10              : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_ped                            : 1 ;  //    20     
} H13Bx_IRIS_014_T;

/*-----------------------------------------------------------------------------
		0x423c iris_015 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hagc_field_mode                : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_mv_hagc_mode                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_dc_clamp_mode                  : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_dagc_en                        : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_agc_half_en                    : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_cagc_en                        : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_hagc_en                        : 1 ;  //    24     
} H13Bx_IRIS_015_T;

/*-----------------------------------------------------------------------------
		0x4240 iris_016 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pal60_mode                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_ntsc443_mode                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_adaptive_3dcomb_mode           : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_colour_trap                    : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_adaptive_mode                  : 3 ;  // 18:16     
} H13Bx_IRIS_016_T;

/*-----------------------------------------------------------------------------
		0x4244 iris_017 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_noise_thresh                   : 8 ;  //  7: 0     
	UINT32 reg_hagc                           : 8 ;  // 15: 8     
	UINT32 reg_agc_gain_update                : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_agc_lpfin_iir_gain             : 4 ;  // 23:20     
	UINT32 reg_agc_lpfin_div                  : 4 ;  // 27:24     
	UINT32 reg_agc_lpfin_sm_ctrl              : 4 ;  // 31:28     
} H13Bx_IRIS_017_T;

/*-----------------------------------------------------------------------------
		0x4248 iris_018 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_force_vcr_en                   : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_force_vcr_rew                  : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_force_vcr_ff                   : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_force_vcr_trick                : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_force_vcr                      : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_agc_lpfout_div                 : 4 ;  // 23:20     
	UINT32 reg_agc_lpfin_noise_ctrl           : 2 ;  // 25:24     
} H13Bx_IRIS_018_T;

/*-----------------------------------------------------------------------------
		0x424c iris_019 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adc_updn_swap                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_adc_input_swap                 : 1 ;  //     4     
} H13Bx_IRIS_019_T;

/*-----------------------------------------------------------------------------
		0x4250 iris_020 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ccir656_en                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cbcr_swap                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_blue_mode                      : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_yc_delay                       : 4 ;  // 15:12     
} H13Bx_IRIS_020_T;

/*-----------------------------------------------------------------------------
		0x4254 iris_021 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hue                            : 8 ;  //  7: 0     
	UINT32 reg_saturation                     : 8 ;  // 15: 8     
	UINT32 reg_brightness                     : 8 ;  // 23:16     
	UINT32 reg_contrast                       : 8 ;  // 31:24     
} H13Bx_IRIS_021_T;

/*-----------------------------------------------------------------------------
		0x4258 iris_022 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cagc                           : 8 ;  //  7: 0     
} H13Bx_IRIS_022_T;

/*-----------------------------------------------------------------------------
		0x425c iris_023 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_user_ckill_mode                : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_vbi_ckill                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_hlock_ckill                    : 1 ;  //     8     
} H13Bx_IRIS_023_T;

/*-----------------------------------------------------------------------------
		0x4260 iris_024 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_nstd_hysis                     : 4 ;  //  3: 0     
	UINT32 reg_disable_clamp_on_vsync         : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_bypass                         : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_noburst_ckill                  : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vnon_std_threshold             : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_hnon_std_threshold             : 6 ;  // 25:20     
} H13Bx_IRIS_024_T;

/*-----------------------------------------------------------------------------
		0x4264 iris_025 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_agc_peak_en                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_agc_peak_cntl                  : 4 ;  //  7: 4     
	UINT32 reg_agc_peak_nominal               : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_agc_peak_only                  : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_agc_peak_noise_ctrl            : 4 ;  // 23:20     
	UINT32 reg_blank_level_diff_div           : 1 ;  //    24     
} H13Bx_IRIS_025_T;

/*-----------------------------------------------------------------------------
		0x4268 iris_026 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_blue_cr                        : 8 ;  //  7: 0     
	UINT32 reg_blue_cb                        : 8 ;  // 15: 8     
	UINT32 reg_blue_y                         : 8 ;  // 23:16     
} H13Bx_IRIS_026_T;

/*-----------------------------------------------------------------------------
		0x426c iris_027 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hstate_fixed                   : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_disable_hfine                  : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_hstate_unlocked                : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_hstate_max                     : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_locked_count_noisy_max         : 4 ;  // 19:16     
	UINT32 reg_locked_count_clean_max         : 4 ;  // 23:20     
	UINT32 reg_hdetect_clamp_level            : 8 ;  // 31:24     
} H13Bx_IRIS_027_T;

/*-----------------------------------------------------------------------------
		0x4270 iris_028 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hlock_vsync_mode               : 2 ;  //  1: 0     
} H13Bx_IRIS_028_T;

/*-----------------------------------------------------------------------------
		0x4274 iris_029 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdto_inc                       : 32;  // 31: 0     
} H13Bx_IRIS_029_T;

/*-----------------------------------------------------------------------------
		0x4278 iris_030 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdto_inc                       : 32;  // 31: 0     
} H13Bx_IRIS_030_T;

/*-----------------------------------------------------------------------------
		0x427c iris_031 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_gate_end                 : 8 ;  //  7: 0     
	UINT32 reg_hsync_gate_start               : 8 ;  // 15: 8     
	UINT32 reg_hsync_phase_offset             : 8 ;  // 23:16     
	UINT32 reg_hsync_rising                   : 8 ;  // 31:24     
} H13Bx_IRIS_031_T;

/*-----------------------------------------------------------------------------
		0x4280 iris_032 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_similar                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_hsync_low                      : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_hdetect_noise_en               : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_hfine_lt_hcoarse               : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_hlpf_clamp_sel                 : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_hlpf_clamp_noisy_en            : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_hlpf_clamp_vbi_en              : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_hlpf_clamp_en                  : 1 ;  //    28     
} H13Bx_IRIS_032_T;

/*-----------------------------------------------------------------------------
		0x4284 iris_033 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_rising_end               : 8 ;  //  7: 0     
	UINT32 reg_hsync_rising_auto              : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_hsync_rising_start             : 6 ;  // 17:12     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_phase_diff_noise_ctrl          : 4 ;  // 23:20     
	UINT32 reg_hlpf_noise_ctrl                : 4 ;  // 27:24     
} H13Bx_IRIS_033_T;

/*-----------------------------------------------------------------------------
		0x4288 iris_034 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hblank_end                     : 8 ;  //  7: 0     
	UINT32 reg_hblank_start                   : 8 ;  // 15: 8     
	UINT32 reg_hsync_rising_msk               : 4 ;  // 19:16     
} H13Bx_IRIS_034_T;

/*-----------------------------------------------------------------------------
		0x428c iris_035 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst_gate_end                 : 8 ;  //  7: 0     
	UINT32 reg_burst_gate_start               : 8 ;  // 15: 8     
} H13Bx_IRIS_035_T;

/*-----------------------------------------------------------------------------
		0x4290 iris_036 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hactive_width                  : 8 ;  //  7: 0     
	UINT32 reg_hactive_start                  : 8 ;  // 15: 8     
} H13Bx_IRIS_036_T;

/*-----------------------------------------------------------------------------
		0x4294 iris_037 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactive_height                 : 8 ;  //  7: 0     
	UINT32 reg_vactive_start                  : 8 ;  // 15: 8     
} H13Bx_IRIS_037_T;

/*-----------------------------------------------------------------------------
		0x4298 iris_038 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_h_max                    : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_vsync_h_min                    : 7 ;  // 14: 8     
} H13Bx_IRIS_038_T;

/*-----------------------------------------------------------------------------
		0x429c iris_039 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_agc_max                  : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_vsync_agc_min                  : 7 ;  // 14: 8     
} H13Bx_IRIS_039_T;

/*-----------------------------------------------------------------------------
		0x42a0 iris_040 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_vbi_max                  : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_vsync_vbi_min                  : 7 ;  // 14: 8     
} H13Bx_IRIS_040_T;

/*-----------------------------------------------------------------------------
		0x42a4 iris_041 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vlock_wide_range               : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_vsync_l_cnt                    : 4 ;  //  7: 4     
	UINT32 reg_vsync_threshold_cnt512         : 11;  // 18: 8     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_vsync_threshold_cnt304         : 11;  // 30:20     
} H13Bx_IRIS_041_T;

/*-----------------------------------------------------------------------------
		0x42a8 iris_042 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_proscan_1field_mode            : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_vsync_cntl_noisy               : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_vsync_cntl_ff_rew              : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_vsync_cntl_trick               : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsync_cntl_vcr                 : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_vsync_cntl                     : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_vsync_lockout_cnt              : 7 ;  // 30:24     
} H13Bx_IRIS_042_T;

/*-----------------------------------------------------------------------------
		0x42ac iris_043 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_field_polarity                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_flip_field                     : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_veven_delayed                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_vodd_delayed                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_field_detect_mode              : 4 ;  // 19:16     
	UINT32 reg_vloop_tc                       : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_debug_digital                  : 8 ;  // 31:24     
} H13Bx_IRIS_043_T;

/*-----------------------------------------------------------------------------
		0x42b0 iris_044 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dbg_syncs                      : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_muxdigital                     : 4 ;  //  7: 4     
	UINT32 reg_muxanalogb                     : 4 ;  // 11: 8     
	UINT32 reg_muxanaloga                     : 4 ;  // 15:12     
	UINT32 reg_debug_analogb                  : 8 ;  // 23:16     
	UINT32 reg_debug_analoga                  : 8 ;  // 31:24     
} H13Bx_IRIS_044_T;

/*-----------------------------------------------------------------------------
		0x42b4 iris_045 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_ybw                      : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_peak_range                     : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_peak_gain                      : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_peak_en                        : 1 ;  //    12     
} H13Bx_IRIS_045_T;

/*-----------------------------------------------------------------------------
		0x42b8 iris_046 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_auto_secam_level               : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_sv_bf                          : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_secam_pll                      : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_secam_sw_old                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_palsw_level                    : 2 ;  // 17:16     
} H13Bx_IRIS_046_T;

/*-----------------------------------------------------------------------------
		0x42bc iris_047 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lose_chromalock_mode           : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_lose_chromalock_count          : 4 ;  //  7: 4     
	UINT32 reg_lose_chromalock_level          : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_lose_chromalock_ckill          : 1 ;  //    12     
} H13Bx_IRIS_047_T;

/*-----------------------------------------------------------------------------
		0x42c0 iris_048 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cstripe_detect_control         : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_cdct_demod_sel                 : 2 ;  //  5: 4     
} H13Bx_IRIS_048_T;

/*-----------------------------------------------------------------------------
		0x42c4 iris_049 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hfine_vcr_trick_en             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_hfine_vcr_en                   : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_hresampler_2up                 : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cstate                         : 3 ;  // 14:12     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_fixed_cstate                   : 1 ;  //    16     
} H13Bx_IRIS_049_T;

/*-----------------------------------------------------------------------------
		0x42c8 iris_050 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_pulse_ampl                 : 8 ;  //  7: 0     
} H13Bx_IRIS_050_T;

/*-----------------------------------------------------------------------------
		0x42cc iris_051 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_delay                    : 8 ;  //  7: 0     
	UINT32 reg_cpump_adjust                   : 8 ;  // 15: 8     
	UINT32 reg_cpump_adjust_polarity          : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_cpump_adjust_delay             : 6 ;  // 25:20     
} H13Bx_IRIS_051_T;

/*-----------------------------------------------------------------------------
		0x42d0 iris_052 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_colourstripes_sel           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_mv_vbi_sel                     : 1 ;  //     4     
} H13Bx_IRIS_052_T;

/*-----------------------------------------------------------------------------
		0x42d4 iris_053 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_kill_cr                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cpump_kill_cb                  : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cpump_kill_y                   : 1 ;  //     8     
} H13Bx_IRIS_053_T;

/*-----------------------------------------------------------------------------
		0x42d8 iris_054 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_auto_stip_nobp           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cpump_auto_stip_unlocked       : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cpump_auto_stip_no_signal      : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cpump_auto_stip_noisy          : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cpump_auto_stip_vactive        : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_cpump_auto_stip_mode           : 2 ;  // 21:20     
} H13Bx_IRIS_054_T;

/*-----------------------------------------------------------------------------
		0x42dc iris_055 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_accum_mode               : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cpump_fixed_syncmid            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cpump_level_filter_gain        : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_cpump_noisy_filter_en          : 1 ;  //    12     
} H13Bx_IRIS_055_T;

/*-----------------------------------------------------------------------------
		0x42e0 iris_056 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_dn_max                   : 7 ;  //  6: 0     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cpump_up_max                   : 7 ;  // 14: 8     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_cpump_vsync_blank_filter       : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_cpump_vsync_syncmid_filter     : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_cpump_vsync_mode               : 2 ;  // 25:24     
} H13Bx_IRIS_056_T;

/*-----------------------------------------------------------------------------
		0x42e4 iris_057 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_diff_noisy_only          : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cpump_dn_diff_max              : 7 ;  // 10: 4     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_cpump_diff_signal_only         : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cpump_up_diff_max              : 7 ;  // 22:16     
} H13Bx_IRIS_057_T;

/*-----------------------------------------------------------------------------
		0x42e8 iris_058 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cpump_pr_override              : 8 ;  //  7: 0     
	UINT32 reg_cpump_pb_override              : 8 ;  // 15: 8     
	UINT32 reg_cpump_y_override               : 8 ;  // 23:16     
} H13Bx_IRIS_058_T;

/*-----------------------------------------------------------------------------
		0x42ec iris_059 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_db_freq                        : 12;  // 11: 0     
	UINT32 reg_dr_freq                        : 12;  // 23:12     
	UINT32 reg_cvbs_y_delay                   : 5 ;  // 28:24     
} H13Bx_IRIS_059_T;

/*-----------------------------------------------------------------------------
		0x42f0 iris_060 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactivity_en                   : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_vactivity_on2frame             : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_vactivity_threshold            : 6 ;  // 13: 8     
} H13Bx_IRIS_060_T;

/*-----------------------------------------------------------------------------
		0x42f4 iris_061 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_horiz_diff_cgain               : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_horiz_diff_ygain               : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_chroma_vdiff_gain              : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_lowfreq_vdiff_gain             : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_noise_th_en                    : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_noise_th                       : 7 ;  // 26:20     
} H13Bx_IRIS_061_T;

/*-----------------------------------------------------------------------------
		0x42f8 iris_062 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vadap_burst_noise_th_gain      : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_burst_noise_th_gain            : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_c_noise_th_gain                : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_y_noise_th_gain                : 2 ;  // 13:12     
} H13Bx_IRIS_062_T;

/*-----------------------------------------------------------------------------
		0x42fc iris_063 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adaptive_chroma_mode           : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_dot_suppress_mode              : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_motion_mode                    : 2 ;  //  9: 8     
} H13Bx_IRIS_063_T;

/*-----------------------------------------------------------------------------
		0x4300 iris_064 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pal3dcomb_vactive_offset       : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_fb_sync                        : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_fb_hold                        : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_fb_ctl                         : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_field_latency                  : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_lbadrgen_rst                   : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_comb2d_only                    : 1 ;  //    24     
} H13Bx_IRIS_064_T;

/*-----------------------------------------------------------------------------
		0x4304 iris_065 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_noise_th_en                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_md_noise_th                    : 7 ;  // 10: 4     
} H13Bx_IRIS_065_T;

/*-----------------------------------------------------------------------------
		0x4308 iris_066 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pal3dcomb_vactive_offset_md    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_fb_sync_md                     : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_fb_hold_md                     : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_fb_ctl_md                      : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_field_latency_md               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_lbadrgen_rst_md                : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_comb2d_only_md                 : 1 ;  //    24     
} H13Bx_IRIS_066_T;

/*-----------------------------------------------------------------------------
		0x430c iris_067 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ldpause_threshold              : 4 ;  //  3: 0     
	UINT32 reg_vf_nstd_en                     : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_vcr_auto_switch_en             : 1 ;  //     8     
} H13Bx_IRIS_067_T;

/*-----------------------------------------------------------------------------
		0x4310 iris_068 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_comb_coring                    : 4 ;  //  3: 0     
	UINT32 reg_tcomb_gain                     : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_notch_gain                     : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_comb_gain                      : 3 ;  // 14:12     
} H13Bx_IRIS_068_T;

/*-----------------------------------------------------------------------------
		0x4314 iris_069 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactive_md_height              : 8 ;  //  7: 0     
	UINT32 reg_vactive_md_start               : 8 ;  // 15: 8     
} H13Bx_IRIS_069_T;

/*-----------------------------------------------------------------------------
		0x4318 iris_070 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hsync_pulse_width              : 4 ;  //  3: 0     
} H13Bx_IRIS_070_T;

/*-----------------------------------------------------------------------------
		0x431c iris_071 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cagc_tc_ismall                 : 3 ;  //  2: 0     
} H13Bx_IRIS_071_T;

/*-----------------------------------------------------------------------------
		0x4320 iris_072 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cagc_coring_threshold          : 4 ;  //  3: 0     
	UINT32 reg_cagc_unity_gain                : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cagc_coring                    : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_cagc_tc_p                      : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_cagc_tc_ibig                   : 3 ;  // 18:16     
} H13Bx_IRIS_072_T;

/*-----------------------------------------------------------------------------
		0x4324 iris_073 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_aaf_on                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_syncmidpt_lowblank_sel         : 4 ;  //  7: 4     
	UINT32 reg_syncmidpt_lowblank_ratio       : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_syncmidpt_syncheight_sel       : 4 ;  // 19:16     
	UINT32 reg_syncmidpt_syncheight_ratio     : 1 ;  //    20     
} H13Bx_IRIS_073_T;

/*-----------------------------------------------------------------------------
		0x4328 iris_074 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dcrestore_accum_width          : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_dcrestore_no_bad_bp            : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_dcrestore_kill_enable          : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_dcrestore_bp_delay             : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_syncmid_nobp_en                : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_syncmid_filter_en              : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_dcrestore_gain                 : 2 ;  // 29:28     
} H13Bx_IRIS_074_T;

/*-----------------------------------------------------------------------------
		0x432c iris_075 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dcrestore_kill_enable_noisy    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_sync_height_unclp              : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_sync_height_sel                : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_sync_height_update             : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_syncmidpt_lowsync_sel          : 4 ;  // 19:16     
	UINT32 reg_syncmidpt_lowsync_ratio        : 6 ;  // 25:20     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_syncmidpt_syncheight_iir_sel   : 4 ;  // 31:28     
} H13Bx_IRIS_075_T;

/*-----------------------------------------------------------------------------
		0x4330 iris_076 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dcrestore_hsync_mid            : 8 ;  //  7: 0     
	UINT32 reg_bp_kill_thresh                 : 8 ;  // 15: 8     
	UINT32 reg_hmgc                           : 8 ;  // 23:16     
	UINT32 reg_dcrestore_lpf_en               : 2 ;  // 25:24     
} H13Bx_IRIS_076_T;

/*-----------------------------------------------------------------------------
		0x4334 iris_077 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_auto_min_sync_height           : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_min_sync_height                : 7 ;  // 10: 4     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_syncmidpt_offset               : 8 ;  // 19:12     
	UINT32 reg_syncmidpt_offset_sel           : 4 ;  // 23:20     
	UINT32 reg_clampagc_syncs_ctrl            : 8 ;  // 31:24     
} H13Bx_IRIS_077_T;

/*-----------------------------------------------------------------------------
		0x4338 iris_078 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vcrtrick_proscan               : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_veven_early_delayed            : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_vodd_early_delayed             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_vfield_hoffset_fixed           : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vsync_no_signal_thresh         : 8 ;  // 23:16     
	UINT32 reg_vsync_signal_thresh            : 8 ;  // 31:24     
} H13Bx_IRIS_078_T;

/*-----------------------------------------------------------------------------
		0x433c iris_079 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vfield_hoffset                 : 9 ;  //  8: 0     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_vactive_half_lines             : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vdetect_noise_en               : 1 ;  //    16     
} H13Bx_IRIS_079_T;

/*-----------------------------------------------------------------------------
		0x4340 iris_080 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_dual_hedge_dis                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_dual_hedge_auto_width          : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_dual_fine_hedge_vbi            : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_dual_coarse_hedge_vbi          : 2 ;  // 13:12     
} H13Bx_IRIS_080_T;

/*-----------------------------------------------------------------------------
		0x4344 iris_081 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_hsync_rising_end            : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_vcr_state2_long                : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_slow_hdsw                      : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_mv_hsync_rising_start          : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_no_hsyncs_mode                 : 2 ;  // 25:24     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_many_hsyncs_mode               : 1 ;  //    28     
} H13Bx_IRIS_081_T;

/*-----------------------------------------------------------------------------
		0x4348 iris_082 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_no_hsyncs_weak                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_disable_hdsw_weak              : 1 ;  //     4     
} H13Bx_IRIS_082_T;

/*-----------------------------------------------------------------------------
		0x434c iris_083 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vactive_hdsw_mode              : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_disable_hdsw_mode              : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_hsync_falling_filter           : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_no_hsyncs_noisy                : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_hloop_range                    : 2 ;  // 17:16     
} H13Bx_IRIS_083_T;

/*-----------------------------------------------------------------------------
		0x4350 iris_084 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_hf_max                      : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_md_hf_shift                    : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_md_lf_sad                      : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_md_lf_shift                    : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_md_c_noise_th_en               : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_md_c_noise_th                  : 7 ;  // 26:20     
} H13Bx_IRIS_084_T;

/*-----------------------------------------------------------------------------
		0x4354 iris_085 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_cf_cactivity_high           : 4 ;  //  3: 0     
	UINT32 reg_md_cf_lactivity_high           : 8 ;  // 11: 4     
	UINT32 reg_md_cf_cactivity_low            : 8 ;  // 19:12     
	UINT32 reg_md_cf_lactivity_low            : 8 ;  // 27:20     
	UINT32 reg_md_cf_activity_enable          : 2 ;  // 29:28     
} H13Bx_IRIS_085_T;

/*-----------------------------------------------------------------------------
		0x4358 iris_086 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_md_k_thresh                    : 8 ;  //  7: 0     
} H13Bx_IRIS_086_T;

/*-----------------------------------------------------------------------------
		0x435c iris_087 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tcomb_chroma_level             : 8 ;  //  7: 0     
	UINT32 reg_hf_luma_chroma_offset          : 8 ;  // 15: 8     
	UINT32 reg_spatial_luma_level             : 8 ;  // 23:16     
	UINT32 reg_chroma_level                   : 8 ;  // 31:24     
} H13Bx_IRIS_087_T;

/*-----------------------------------------------------------------------------
		0x4360 iris_088 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_chroma_activity_level          : 8 ;  //  7: 0     
	UINT32 reg_lf_luma_offset                 : 8 ;  // 15: 8     
} H13Bx_IRIS_088_T;

/*-----------------------------------------------------------------------------
		0x4364 iris_089 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_avg_freq_range                 : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_issecam_th                     : 6 ;  //  9: 4     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_freq_offset_range              : 8 ;  // 19:12     
} H13Bx_IRIS_089_T;

/*-----------------------------------------------------------------------------
		0x4368 iris_090 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_chroma_bw_motion_th            : 8 ;  //  7: 0     
	UINT32 reg_motion_config                  : 8 ;  // 15: 8     
	UINT32 reg_hactive_md_width               : 8 ;  // 23:16     
	UINT32 reg_hactive_md_start               : 8 ;  // 31:24     
} H13Bx_IRIS_090_T;

/*-----------------------------------------------------------------------------
		0x436c iris_091 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_motion_mode             : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_chroma_bw_motion               : 2 ;  //  5: 4     
} H13Bx_IRIS_091_T;

/*-----------------------------------------------------------------------------
		0x4370 iris_092 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_flat_chroma_offset             : 8 ;  //  7: 0     
	UINT32 reg_flat_luma_offset               : 8 ;  // 15: 8     
	UINT32 reg_flat_chroma_shift              : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_flat_luma_shift                : 2 ;  // 21:20     
} H13Bx_IRIS_092_T;

/*-----------------------------------------------------------------------------
		0x4374 iris_093 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_motion_debug                   : 8 ;  //  7: 0     
	UINT32 reg_cf_flat_motion_shift           : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_motion_c_mode                  : 2 ;  // 13:12     
} H13Bx_IRIS_093_T;

/*-----------------------------------------------------------------------------
		0x4378 iris_094 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cordic_gate_end                : 8 ;  //  7: 0     
	UINT32 reg_cordic_gate_start              : 8 ;  // 15: 8     
	UINT32 reg_ispal_th                       : 8 ;  // 23:16     
	UINT32 reg_phase_offset_range             : 8 ;  // 31:24     
} H13Bx_IRIS_094_T;

/*-----------------------------------------------------------------------------
		0x437c iris_095 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pal3tap_only_c                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_pal3tap_only_y                 : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_adc_cpump_swap                 : 6 ;  // 13: 8     
} H13Bx_IRIS_095_T;

/*-----------------------------------------------------------------------------
		0x4380 iris_096 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ckill                          : 16;  // 15: 0     
	UINT32 reg_cagc_gate_end                  : 8 ;  // 23:16     
	UINT32 reg_cagc_gate_start                : 8 ;  // 31:24     
} H13Bx_IRIS_096_T;

/*-----------------------------------------------------------------------------
		0x4384 iris_097 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_icburst_half_ampl              : 10;  //  9: 0     
} H13Bx_IRIS_097_T;

/*-----------------------------------------------------------------------------
		0x4388 iris_098 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_agc_bypass                     : 1 ;  //     0     
} H13Bx_IRIS_098_T;

/*-----------------------------------------------------------------------------
		0x438c iris_099 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_clampagc_inv                   : 4 ;  //  3: 0     
} H13Bx_IRIS_099_T;

/*-----------------------------------------------------------------------------
		0x4390 iris_100 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_slicer_mode                : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_wss_slicer_mode                : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_tt_slicer_mode                 : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_cc_slicer_mode                 : 2 ;  // 13:12     
} H13Bx_IRIS_100_T;

/*-----------------------------------------------------------------------------
		0x4394 iris_101 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_perr_on                        : 1 ;  //     0     
} H13Bx_IRIS_101_T;

/*-----------------------------------------------------------------------------
		0x4398 iris_102 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_perr_dnref                     : 32;  // 31: 0     
} H13Bx_IRIS_102_T;

/*-----------------------------------------------------------------------------
		0x439c iris_103 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff0               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cres_lpfa_on                   : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_iris_cresampler_on             : 1 ;  //    20     
} H13Bx_IRIS_103_T;

/*-----------------------------------------------------------------------------
		0x43a0 iris_104 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff2               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cres_lpfa_coeff1               : 13;  // 28:16     
} H13Bx_IRIS_104_T;

/*-----------------------------------------------------------------------------
		0x43a4 iris_105 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff4               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cres_lpfa_coeff3               : 13;  // 28:16     
} H13Bx_IRIS_105_T;

/*-----------------------------------------------------------------------------
		0x43a8 iris_106 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfa_coeff5               : 13;  // 12: 0     
} H13Bx_IRIS_106_T;

/*-----------------------------------------------------------------------------
		0x43ac iris_107 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfb_coeff0               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cres_lpfb_on                   : 1 ;  //    16     
} H13Bx_IRIS_107_T;

/*-----------------------------------------------------------------------------
		0x43b0 iris_108 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfb_coeff2               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cres_lpfb_coeff1               : 13;  // 28:16     
} H13Bx_IRIS_108_T;

/*-----------------------------------------------------------------------------
		0x43b4 iris_109 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_lpfb_coeff4               : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cres_lpfb_coeff3               : 13;  // 28:16     
} H13Bx_IRIS_109_T;

/*-----------------------------------------------------------------------------
		0x43b8 iris_110 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_sel                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cres_blend_method              : 3 ;  //  6: 4     
} H13Bx_IRIS_110_T;

/*-----------------------------------------------------------------------------
		0x43bc iris_111 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcdefault           : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_cres_0px_sel                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cres_clksel                    : 2 ;  // 17:16     
} H13Bx_IRIS_111_T;

/*-----------------------------------------------------------------------------
		0x43c0 iris_112 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara1             : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_cres_blend_bcpara0             : 11;  // 22:12     
} H13Bx_IRIS_112_T;

/*-----------------------------------------------------------------------------
		0x43c4 iris_113 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara3             : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_cres_blend_bcpara2             : 11;  // 22:12     
} H13Bx_IRIS_113_T;

/*-----------------------------------------------------------------------------
		0x43c8 iris_114 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara5             : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_cres_blend_bcpara4             : 11;  // 22:12     
} H13Bx_IRIS_114_T;

/*-----------------------------------------------------------------------------
		0x43cc iris_115 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_bcpara7             : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_cres_blend_bcpara6             : 11;  // 22:12     
} H13Bx_IRIS_115_T;

/*-----------------------------------------------------------------------------
		0x43d0 iris_116 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_blend_diffth1             : 12;  // 11: 0     
	UINT32 reg_cres_blend_diffth0             : 12;  // 23:12     
} H13Bx_IRIS_116_T;

/*-----------------------------------------------------------------------------
		0x43d4 iris_117 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cres_alpha_off                 : 8 ;  //  7: 0     
	UINT32 reg_cres_blend_diffth2             : 12;  // 19: 8     
} H13Bx_IRIS_117_T;

/*-----------------------------------------------------------------------------
		0x43d8 iris_118 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrscbld_dnoff                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_hrscbld_upoff                  : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_hrsybld_dnoff                  : 3 ;  // 10: 8     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_hrsybld_upoff                  : 4 ;  // 15:12     
	UINT32 reg_dtrs_dlysel                    : 4 ;  // 19:16     
	UINT32 reg_dtrs_revalpha_sel              : 2 ;  // 21:20     
} H13Bx_IRIS_118_T;

/*-----------------------------------------------------------------------------
		0x43dc iris_119 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_y_offo                    : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_oadj_y_offi                    : 11;  // 22:12     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_oadj_yc_msk                    : 5 ;  // 28:24     
} H13Bx_IRIS_119_T;

/*-----------------------------------------------------------------------------
		0x43e0 iris_120 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_y_coeff                   : 14;  // 13: 0     
} H13Bx_IRIS_120_T;

/*-----------------------------------------------------------------------------
		0x43e4 iris_121 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_c_offo                    : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_oadj_c_offi                    : 11;  // 22:12     
} H13Bx_IRIS_121_T;

/*-----------------------------------------------------------------------------
		0x43e8 iris_122 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_oadj_c_coeff                   : 14;  // 13: 0     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_oadj_cr_coeff                  : 14;  // 29:16     
} H13Bx_IRIS_122_T;

/*-----------------------------------------------------------------------------
		0x43ec iris_123 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adc_unity_gain_value           : 4 ;  //  3: 0     
	UINT32 reg_oadj_cr_offo                   : 11;  // 14: 4     
	UINT32                                    : 1 ;  //    15     reserved
	UINT32 reg_oadj_cr_offi                   : 11;  // 26:16     
} H13Bx_IRIS_123_T;

/*-----------------------------------------------------------------------------
		0x43f0 iris_124 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_frame_motion_th1               : 16;  // 15: 0     
	UINT32 reg_frame_motion_th0               : 16;  // 31:16     
} H13Bx_IRIS_124_T;

/*-----------------------------------------------------------------------------
		0x43f4 iris_125 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_iblank_half_lvl0               : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_isynctip_lvl0                  : 10;  // 21:12     
} H13Bx_IRIS_125_T;

/*-----------------------------------------------------------------------------
		0x43f8 iris_126 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sync_int_blank_lvl0            : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_ipeak_chroma_m768_lvl0         : 10;  // 21:12     
} H13Bx_IRIS_126_T;

/*-----------------------------------------------------------------------------
		0x43fc iris_127 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sync_int_blank_vid_lvl0        : 8 ;  //  7: 0     
	UINT32 reg_int_black_ped_vid_lvl0         : 10;  // 17: 8     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_int_blank_vid_lvl0             : 10;  // 29:20     
} H13Bx_IRIS_127_T;

/*-----------------------------------------------------------------------------
		0x4400 iris_128 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_int_vbi_blank_vid_lvl0         : 8 ;  //  7: 0     
} H13Bx_IRIS_128_T;

/*-----------------------------------------------------------------------------
		0x4404 iris_129 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_internal_sync_midpt            : 13;  // 12: 0     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_internal_sync_level            : 13;  // 28:16     
} H13Bx_IRIS_129_T;

/*-----------------------------------------------------------------------------
		0x4408 iris_130 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sync_internal_sync_midpt       : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_internal_blank_level           : 13;  // 24:12     
} H13Bx_IRIS_130_T;

/*-----------------------------------------------------------------------------
		0x440c iris_131 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adc_blank_level                : 10;  //  9: 0     
} H13Bx_IRIS_131_T;

/*-----------------------------------------------------------------------------
		0x4410 iris_132 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_alpha_croff                : 8 ;  //  7: 0     
	UINT32 reg_hrs_alpha_cboff                : 8 ;  // 15: 8     
	UINT32 reg_hrs_alpha_yoff                 : 8 ;  // 23:16     
} H13Bx_IRIS_132_T;

/*-----------------------------------------------------------------------------
		0x4414 iris_133 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cphase_adjust                  : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_cphase_adjust_sel              : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cdct_actv_sel                  : 2 ;  //  9: 8     
} H13Bx_IRIS_133_T;

/*-----------------------------------------------------------------------------
		0x4418 iris_134 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_adap_clf_mode                  : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_cb_delay                       : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_yc444                          : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_ycadj_cr_p                     : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_ycadj_dly                      : 6 ;  // 21:16     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_oadj_swap                      : 1 ;  //    24     
} H13Bx_IRIS_134_T;

/*-----------------------------------------------------------------------------
		0x441c iris_135 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_0                    : 16;  // 15: 0     
	UINT32 reg_lpf_0_en                       : 1 ;  //    16     
} H13Bx_IRIS_135_T;

/*-----------------------------------------------------------------------------
		0x4420 iris_136 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_2                    : 16;  // 15: 0     
	UINT32 reg_lpf_tap_0_1                    : 16;  // 31:16     
} H13Bx_IRIS_136_T;

/*-----------------------------------------------------------------------------
		0x4424 iris_137 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_4                    : 16;  // 15: 0     
	UINT32 reg_lpf_tap_0_3                    : 16;  // 31:16     
} H13Bx_IRIS_137_T;

/*-----------------------------------------------------------------------------
		0x4428 iris_138 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_lpf_tap_0_6                    : 16;  // 15: 0     
	UINT32 reg_lpf_tap_0_5                    : 16;  // 31:16     
} H13Bx_IRIS_138_T;

/*-----------------------------------------------------------------------------
		0x442c iris_139 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_missing_max                 : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_hs_match_max                   : 6 ;  // 13: 8     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_hs_cnt_mode                    : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_hs_sync_mode                   : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_status_pseudo_sync             : 4 ;  // 27:24     
} H13Bx_IRIS_139_T;

/*-----------------------------------------------------------------------------
		0x4430 iris_140 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_0                : 24;  // 23: 0     
} H13Bx_IRIS_140_T;

/*-----------------------------------------------------------------------------
		0x4434 iris_141 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_141_T;

/*-----------------------------------------------------------------------------
		0x4438 iris_142 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_pos_0                : 24;  // 23: 0     
} H13Bx_IRIS_142_T;

/*-----------------------------------------------------------------------------
		0x443c iris_143 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_pos_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_143_T;

/*-----------------------------------------------------------------------------
		0x4440 iris_144 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_1                : 24;  // 23: 0     
} H13Bx_IRIS_144_T;

/*-----------------------------------------------------------------------------
		0x4444 iris_145 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_prd_th_1             : 24;  // 23: 0     
} H13Bx_IRIS_145_T;

/*-----------------------------------------------------------------------------
		0x4448 iris_146 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hs_length_pos_1                : 24;  // 23: 0     
} H13Bx_IRIS_146_T;

/*-----------------------------------------------------------------------------
		0x444c iris_147 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_cnt_mode                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_vs_sync_mode                   : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_hs_length_pos_th_1             : 24;  // 31: 8     
} H13Bx_IRIS_147_T;

/*-----------------------------------------------------------------------------
		0x4450 iris_148 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_missing_max                 : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_vs_match_max                   : 6 ;  // 13: 8     
} H13Bx_IRIS_148_T;

/*-----------------------------------------------------------------------------
		0x4454 iris_149 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_0                : 24;  // 23: 0     
} H13Bx_IRIS_149_T;

/*-----------------------------------------------------------------------------
		0x4458 iris_150 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_150_T;

/*-----------------------------------------------------------------------------
		0x445c iris_151 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_pos_0                : 24;  // 23: 0     
} H13Bx_IRIS_151_T;

/*-----------------------------------------------------------------------------
		0x4460 iris_152 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_pos_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_152_T;

/*-----------------------------------------------------------------------------
		0x4464 iris_153 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_1                : 24;  // 23: 0     
} H13Bx_IRIS_153_T;

/*-----------------------------------------------------------------------------
		0x4468 iris_154 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_prd_th_1             : 24;  // 23: 0     
} H13Bx_IRIS_154_T;

/*-----------------------------------------------------------------------------
		0x446c iris_155 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vs_length_pos_1                : 24;  // 23: 0     
} H13Bx_IRIS_155_T;

/*-----------------------------------------------------------------------------
		0x4470 iris_156 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_cnt_mode                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_ha_sync_mode                   : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_vs_length_pos_th_1             : 24;  // 31: 8     
} H13Bx_IRIS_156_T;

/*-----------------------------------------------------------------------------
		0x4474 iris_157 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_missing_max                 : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_ha_match_max                   : 6 ;  // 13: 8     
} H13Bx_IRIS_157_T;

/*-----------------------------------------------------------------------------
		0x4478 iris_158 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_0                : 24;  // 23: 0     
} H13Bx_IRIS_158_T;

/*-----------------------------------------------------------------------------
		0x447c iris_159 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_159_T;

/*-----------------------------------------------------------------------------
		0x4480 iris_160 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_pos_0                : 24;  // 23: 0     
} H13Bx_IRIS_160_T;

/*-----------------------------------------------------------------------------
		0x4484 iris_161 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_pos_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_161_T;

/*-----------------------------------------------------------------------------
		0x4488 iris_162 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_1                : 24;  // 23: 0     
} H13Bx_IRIS_162_T;

/*-----------------------------------------------------------------------------
		0x448c iris_163 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_prd_th_1             : 24;  // 23: 0     
} H13Bx_IRIS_163_T;

/*-----------------------------------------------------------------------------
		0x4490 iris_164 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ha_length_pos_1                : 24;  // 23: 0     
} H13Bx_IRIS_164_T;

/*-----------------------------------------------------------------------------
		0x4494 iris_165 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_cnt_mode                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_va_sync_mode                   : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_ha_length_pos_th_1             : 24;  // 31: 8     
} H13Bx_IRIS_165_T;

/*-----------------------------------------------------------------------------
		0x4498 iris_166 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_missing_max                 : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_va_match_max                   : 6 ;  // 13: 8     
} H13Bx_IRIS_166_T;

/*-----------------------------------------------------------------------------
		0x449c iris_167 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_0                : 24;  // 23: 0     
} H13Bx_IRIS_167_T;

/*-----------------------------------------------------------------------------
		0x44a0 iris_168 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_168_T;

/*-----------------------------------------------------------------------------
		0x44a4 iris_169 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_0                : 24;  // 23: 0     
} H13Bx_IRIS_169_T;

/*-----------------------------------------------------------------------------
		0x44a8 iris_170 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_th_0             : 24;  // 23: 0     
} H13Bx_IRIS_170_T;

/*-----------------------------------------------------------------------------
		0x44ac iris_171 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_1                : 24;  // 23: 0     
} H13Bx_IRIS_171_T;

/*-----------------------------------------------------------------------------
		0x44b0 iris_172 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_prd_th_1             : 24;  // 23: 0     
} H13Bx_IRIS_172_T;

/*-----------------------------------------------------------------------------
		0x44b4 iris_173 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_1                : 24;  // 23: 0     
} H13Bx_IRIS_173_T;

/*-----------------------------------------------------------------------------
		0x44b8 iris_174 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_va_length_pos_th_1             : 24;  // 23: 0     
} H13Bx_IRIS_174_T;

/*-----------------------------------------------------------------------------
		0x44bc iris_175 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 swrst_cdct                         : 1 ;  //     0     
	UINT32 swrst_hdct                         : 1 ;  //     1     
	UINT32 iris_clampagc_v2                   : 1 ;  //     2     
	UINT32 swrst_vdct                         : 1 ;  //     3     
	UINT32                                    : 2 ;  //  5: 4     reserved
	UINT32 iris_mif_gmau_md                   : 1 ;  //     6     
	UINT32 iris_mif_gmau                      : 1 ;  //     7     
} H13Bx_IRIS_175_T;

/*-----------------------------------------------------------------------------
		0x44c8 iris_178 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_updn_rst                : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_clampagc_updn                  : 5 ;  //  8: 4     
} H13Bx_IRIS_178_T;

/*-----------------------------------------------------------------------------
		0x44cc iris_179 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_updn                    : 32;  // 31: 0     
} H13Bx_IRIS_179_T;

/*-----------------------------------------------------------------------------
		0x44d0 iris_180 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_secam_pll                   : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs_secam_sw_old                : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cs_auto_secam_level            : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs_chroma_activity_level       : 8 ;  // 19:12     
	UINT32 reg_cs_adaptive_chroma_mode        : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_cs_sw_rst                      : 1 ;  //    24     
} H13Bx_IRIS_180_T;

/*-----------------------------------------------------------------------------
		0x44d4 iris_181 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_phase_offset_range          : 8 ;  //  7: 0     
	UINT32 reg_cs_avg_freq_range              : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_cs_issecam_th                  : 6 ;  // 17:12     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_cs_freq_offset_range           : 8 ;  // 27:20     
} H13Bx_IRIS_181_T;

/*-----------------------------------------------------------------------------
		0x44d8 iris_182 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_db_freq                     : 12;  // 11: 0     
	UINT32 reg_cs_dr_freq                     : 12;  // 23:12     
	UINT32 reg_cs_ispal_th                    : 8 ;  // 31:24     
} H13Bx_IRIS_182_T;

/*-----------------------------------------------------------------------------
		0x44dc iris_183 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cagc_en                     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs_chroma_burst5or10           : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cs_pal60_mode                  : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs_ntsc443_mode                : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cs_colour_mode                 : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cs_adaptive_mode               : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_cs_secam_ybw                   : 2 ;  // 25:24     
} H13Bx_IRIS_183_T;

/*-----------------------------------------------------------------------------
		0x44e0 iris_184 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cagc_coring_threshold       : 4 ;  //  3: 0     
	UINT32 reg_cs_cagc_coring                 : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cs_cagc_unity_gain             : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs_cagc_tc_p                   : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_cs_cagc_tc_ibig                : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cs_cagc_tc_ismall              : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_cs_cagc                        : 8 ;  // 31:24     
} H13Bx_IRIS_184_T;

/*-----------------------------------------------------------------------------
		0x44e4 iris_185 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_chroma_bw_lo                : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_cs_ckill                       : 16;  // 19: 4     
	UINT32 reg_cs_hlock_ckill                 : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_cs_vbi_ckill                   : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_cs_noburst_ckill               : 1 ;  //    28     
} H13Bx_IRIS_185_T;

/*-----------------------------------------------------------------------------
		0x44e8 iris_186 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cdto_inc                    : 32;  // 31: 0     
} H13Bx_IRIS_186_T;

/*-----------------------------------------------------------------------------
		0x44ec iris_187 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cstate                      : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_cs_fixed_cstate                : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cs_user_ckill_mode             : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_cs_lose_chromalock_mode        : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_cs_lose_chromalock_count       : 4 ;  // 19:16     
	UINT32 reg_cs_lose_chromalock_level       : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_cs_lose_chromalock_ckill       : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_cs_palsw_level                 : 2 ;  // 29:28     
} H13Bx_IRIS_187_T;

/*-----------------------------------------------------------------------------
		0x44f0 iris_188 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_chroma_sel                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs_cphase_adjust               : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cs_cphase_adjust_sel           : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs_mv_colourstripes_sel        : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cs_cstripe_detect_control      : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cs_cdct_demod_sel              : 2 ;  // 21:20     
} H13Bx_IRIS_188_T;

/*-----------------------------------------------------------------------------
		0x44f4 iris_189 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_secam_detected              : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs_mv_colourstripes            : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cs_status_cordic_freq          : 8 ;  // 15: 8     
	UINT32 reg_cs_status_burst_mag            : 16;  // 31:16     
} H13Bx_IRIS_189_T;

/*-----------------------------------------------------------------------------
		0x44f8 iris_190 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_chromalock                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs_status_cgain                : 14;  // 17: 4     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_cs_status_cmag                 : 8 ;  // 27:20     
	UINT32 reg_cs_pal_detected                : 1 ;  //    28     
} H13Bx_IRIS_190_T;

/*-----------------------------------------------------------------------------
		0x44fc iris_191 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_secam_pll                  : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs1_secam_sw_old               : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cs1_auto_secam_level           : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs1_chroma_activity_level      : 8 ;  // 19:12     
	UINT32 reg_cs1_adaptive_chroma_mode       : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_cs1_sw_rst                     : 1 ;  //    24     
} H13Bx_IRIS_191_T;

/*-----------------------------------------------------------------------------
		0x4500 iris_192 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_phase_offset_range         : 8 ;  //  7: 0     
	UINT32 reg_cs1_avg_freq_range             : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_cs1_issecam_th                 : 6 ;  // 17:12     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_cs1_freq_offset_range          : 8 ;  // 27:20     
} H13Bx_IRIS_192_T;

/*-----------------------------------------------------------------------------
		0x4504 iris_193 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_db_freq                    : 12;  // 11: 0     
	UINT32 reg_cs1_dr_freq                    : 12;  // 23:12     
	UINT32 reg_cs1_ispal_th                   : 8 ;  // 31:24     
} H13Bx_IRIS_193_T;

/*-----------------------------------------------------------------------------
		0x4508 iris_194 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cagc_en                    : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs1_chroma_burst5or10          : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cs1_pal60_mode                 : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs1_ntsc443_mode               : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cs1_colour_mode                : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cs1_adaptive_mode              : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_cs1_secam_ybw                  : 2 ;  // 25:24     
} H13Bx_IRIS_194_T;

/*-----------------------------------------------------------------------------
		0x450c iris_195 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cagc_coring_threshold      : 4 ;  //  3: 0     
	UINT32 reg_cs1_cagc_coring                : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cs1_cagc_unity_gain            : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs1_cagc_tc_p                  : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_cs1_cagc_tc_ibig               : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cs1_cagc_tc_ismall             : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_cs1_cagc                       : 8 ;  // 31:24     
} H13Bx_IRIS_195_T;

/*-----------------------------------------------------------------------------
		0x4510 iris_196 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_chroma_bw_lo               : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_cs1_ckill                      : 16;  // 19: 4     
	UINT32 reg_cs1_hlock_ckill                : 1 ;  //    20     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 reg_cs1_vbi_ckill                  : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_cs1_noburst_ckill              : 1 ;  //    28     
} H13Bx_IRIS_196_T;

/*-----------------------------------------------------------------------------
		0x4514 iris_197 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cdto_inc                   : 32;  // 31: 0     
} H13Bx_IRIS_197_T;

/*-----------------------------------------------------------------------------
		0x4518 iris_198 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cstate                     : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 reg_cs1_fixed_cstate               : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_cs1_user_ckill_mode            : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_cs1_lose_chromalock_mode       : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_cs1_lose_chromalock_count      : 4 ;  // 19:16     
	UINT32 reg_cs1_lose_chromalock_level      : 3 ;  // 22:20     
	UINT32                                    : 1 ;  //    23     reserved
	UINT32 reg_cs1_lose_chromalock_ckill      : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 reg_cs1_palsw_level                : 2 ;  // 29:28     
} H13Bx_IRIS_198_T;

/*-----------------------------------------------------------------------------
		0x451c iris_199 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_chroma_sel                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs1_cphase_adjust              : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cs1_cphase_adjust_sel          : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_cs1_mv_colourstripes_sel       : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_cs1_cstripe_detect_control     : 3 ;  // 18:16     
	UINT32                                    : 1 ;  //    19     reserved
	UINT32 reg_cs1_cdct_demod_sel             : 2 ;  // 21:20     
} H13Bx_IRIS_199_T;

/*-----------------------------------------------------------------------------
		0x4520 iris_200 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_secam_detected             : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs1_mv_colourstripes           : 3 ;  //  6: 4     
	UINT32                                    : 1 ;  //     7     reserved
	UINT32 reg_cs1_status_cordic_freq         : 8 ;  // 15: 8     
	UINT32 reg_cs1_status_burst_mag           : 16;  // 31:16     
} H13Bx_IRIS_200_T;

/*-----------------------------------------------------------------------------
		0x4524 iris_201 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_chromalock                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cs1_status_cgain               : 14;  // 17: 4     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_cs1_status_cmag                : 8 ;  // 27:20     
	UINT32 reg_cs1_pal_detected               : 1 ;  //    28     
} H13Bx_IRIS_201_T;

/*-----------------------------------------------------------------------------
		0x4528 iris_202 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_ctrl                   : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_estfrdp_on                     : 1 ;  //     8     
} H13Bx_IRIS_202_T;

/*-----------------------------------------------------------------------------
		0x452c iris_203 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_tgt_cf0cnt             : 20;  // 19: 0     
} H13Bx_IRIS_203_T;

/*-----------------------------------------------------------------------------
		0x4530 iris_204 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_tgt_cf1cnt             : 20;  // 19: 0     
} H13Bx_IRIS_204_T;

/*-----------------------------------------------------------------------------
		0x4534 iris_205 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_err_win1_u             : 8 ;  //  7: 0     
	UINT32 reg_estfrdp_err_win1_l             : 8 ;  // 15: 8     
	UINT32 reg_estfrdp_err_win0_u             : 8 ;  // 23:16     
	UINT32 reg_estfrdp_err_win0_l             : 8 ;  // 31:24     
} H13Bx_IRIS_205_T;

/*-----------------------------------------------------------------------------
		0x4538 iris_206 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_err_flag_th            : 8 ;  //  7: 0     
} H13Bx_IRIS_206_T;

/*-----------------------------------------------------------------------------
		0x453c iris_207 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_estfrdp_err                    : 8 ;  //  7: 0     
	UINT32 reg_estfrdp_err_flag               : 1 ;  //     8     
} H13Bx_IRIS_207_T;

/*-----------------------------------------------------------------------------
		0x4540 iris_208 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vsync_width                    : 4 ;  //  3: 0     
} H13Bx_IRIS_208_T;

/*-----------------------------------------------------------------------------
		0x4544 iris_209 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl0                 : 32;  // 31: 0     
} H13Bx_IRIS_209_T;

/*-----------------------------------------------------------------------------
		0x4548 iris_210 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl1                 : 32;  // 31: 0     
} H13Bx_IRIS_210_T;

/*-----------------------------------------------------------------------------
		0x454c iris_211 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl2                 : 32;  // 31: 0     
} H13Bx_IRIS_211_T;

/*-----------------------------------------------------------------------------
		0x4550 iris_212 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl3                 : 32;  // 31: 0     
} H13Bx_IRIS_212_T;

/*-----------------------------------------------------------------------------
		0x4554 iris_213 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl4                 : 32;  // 31: 0     
} H13Bx_IRIS_213_T;

/*-----------------------------------------------------------------------------
		0x4558 iris_214 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl5                 : 32;  // 31: 0     
} H13Bx_IRIS_214_T;

/*-----------------------------------------------------------------------------
		0x455c iris_215 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl6                 : 32;  // 31: 0     
} H13Bx_IRIS_215_T;

/*-----------------------------------------------------------------------------
		0x4560 iris_216 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl7                 : 32;  // 31: 0     
} H13Bx_IRIS_216_T;

/*-----------------------------------------------------------------------------
		0x4564 iris_217 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl8                 : 32;  // 31: 0     
} H13Bx_IRIS_217_T;

/*-----------------------------------------------------------------------------
		0x4568 iris_218 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl9                 : 32;  // 31: 0     
} H13Bx_IRIS_218_T;

/*-----------------------------------------------------------------------------
		0x456c iris_219 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl10                : 32;  // 31: 0     
} H13Bx_IRIS_219_T;

/*-----------------------------------------------------------------------------
		0x4570 iris_220 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl0                 : 32;  // 31: 0     
} H13Bx_IRIS_220_T;

/*-----------------------------------------------------------------------------
		0x4574 iris_221 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl1                 : 32;  // 31: 0     
} H13Bx_IRIS_221_T;

/*-----------------------------------------------------------------------------
		0x4578 iris_222 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl2                 : 32;  // 31: 0     
} H13Bx_IRIS_222_T;

/*-----------------------------------------------------------------------------
		0x457c iris_223 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl3                 : 32;  // 31: 0     
} H13Bx_IRIS_223_T;

/*-----------------------------------------------------------------------------
		0x4580 iris_224 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl4                 : 32;  // 31: 0     
} H13Bx_IRIS_224_T;

/*-----------------------------------------------------------------------------
		0x4584 iris_225 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl5                 : 32;  // 31: 0     
} H13Bx_IRIS_225_T;

/*-----------------------------------------------------------------------------
		0x4588 iris_226 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl6                 : 32;  // 31: 0     
} H13Bx_IRIS_226_T;

/*-----------------------------------------------------------------------------
		0x458c iris_227 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_ctrl7                 : 32;  // 31: 0     
} H13Bx_IRIS_227_T;

/*-----------------------------------------------------------------------------
		0x4590 iris_228 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl0                 : 32;  // 31: 0     
} H13Bx_IRIS_228_T;

/*-----------------------------------------------------------------------------
		0x4594 iris_229 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl1                 : 32;  // 31: 0     
} H13Bx_IRIS_229_T;

/*-----------------------------------------------------------------------------
		0x4598 iris_230 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl2                 : 32;  // 31: 0     
} H13Bx_IRIS_230_T;

/*-----------------------------------------------------------------------------
		0x459c iris_231 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl3                 : 32;  // 31: 0     
} H13Bx_IRIS_231_T;

/*-----------------------------------------------------------------------------
		0x45a0 iris_232 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl4                 : 32;  // 31: 0     
} H13Bx_IRIS_232_T;

/*-----------------------------------------------------------------------------
		0x45a4 iris_233 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl5                 : 32;  // 31: 0     
} H13Bx_IRIS_233_T;

/*-----------------------------------------------------------------------------
		0x45a8 iris_234 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl6                 : 32;  // 31: 0     
} H13Bx_IRIS_234_T;

/*-----------------------------------------------------------------------------
		0x45ac iris_235 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl7                 : 32;  // 31: 0     
} H13Bx_IRIS_235_T;

/*-----------------------------------------------------------------------------
		0x45b0 iris_236 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_blend_ctrl0              : 32;  // 31: 0     
} H13Bx_IRIS_236_T;

/*-----------------------------------------------------------------------------
		0x45b4 iris_237 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl11                : 32;  // 31: 0     
} H13Bx_IRIS_237_T;

/*-----------------------------------------------------------------------------
		0x45b8 iris_238 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl12                : 32;  // 31: 0     
} H13Bx_IRIS_238_T;

/*-----------------------------------------------------------------------------
		0x45bc iris_239 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl13                : 32;  // 31: 0     
} H13Bx_IRIS_239_T;

/*-----------------------------------------------------------------------------
		0x45c0 iris_240 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl14                : 32;  // 31: 0     
} H13Bx_IRIS_240_T;

/*-----------------------------------------------------------------------------
		0x45c4 iris_241 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl15                : 32;  // 31: 0     
} H13Bx_IRIS_241_T;

/*-----------------------------------------------------------------------------
		0x45c8 iris_242 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl0                    : 32;  // 31: 0     
} H13Bx_IRIS_242_T;

/*-----------------------------------------------------------------------------
		0x45cc iris_243 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl1                    : 32;  // 31: 0     
} H13Bx_IRIS_243_T;

/*-----------------------------------------------------------------------------
		0x45d0 iris_244 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl2                    : 32;  // 31: 0     
} H13Bx_IRIS_244_T;

/*-----------------------------------------------------------------------------
		0x45d4 iris_245 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl3                    : 32;  // 31: 0     
} H13Bx_IRIS_245_T;

/*-----------------------------------------------------------------------------
		0x45d8 iris_246 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl4                    : 32;  // 31: 0     
} H13Bx_IRIS_246_T;

/*-----------------------------------------------------------------------------
		0x45dc iris_247 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl5                    : 32;  // 31: 0     
} H13Bx_IRIS_247_T;

/*-----------------------------------------------------------------------------
		0x45e0 iris_248 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl6                    : 32;  // 31: 0     
} H13Bx_IRIS_248_T;

/*-----------------------------------------------------------------------------
		0x45e4 iris_249 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl7                    : 32;  // 31: 0     
} H13Bx_IRIS_249_T;

/*-----------------------------------------------------------------------------
		0x45e8 iris_250 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl8                    : 32;  // 31: 0     
} H13Bx_IRIS_250_T;

/*-----------------------------------------------------------------------------
		0x45ec iris_251 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl9                    : 32;  // 31: 0     
} H13Bx_IRIS_251_T;

/*-----------------------------------------------------------------------------
		0x45f0 iris_252 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl10                   : 32;  // 31: 0     
} H13Bx_IRIS_252_T;

/*-----------------------------------------------------------------------------
		0x45f4 iris_253 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_0             : 32;  // 31: 0     
} H13Bx_IRIS_253_T;

/*-----------------------------------------------------------------------------
		0x45f8 iris_254 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_1             : 32;  // 31: 0     
} H13Bx_IRIS_254_T;

/*-----------------------------------------------------------------------------
		0x45fc iris_255 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_2             : 32;  // 31: 0     
} H13Bx_IRIS_255_T;

/*-----------------------------------------------------------------------------
		0x4600 iris_256 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_3             : 32;  // 31: 0     
} H13Bx_IRIS_256_T;

/*-----------------------------------------------------------------------------
		0x4604 iris_257 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_4             : 32;  // 31: 0     
} H13Bx_IRIS_257_T;

/*-----------------------------------------------------------------------------
		0x4608 iris_258 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_5             : 32;  // 31: 0     
} H13Bx_IRIS_258_T;

/*-----------------------------------------------------------------------------
		0x460c iris_259 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef2                  : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_hrs_cx2_coef1                  : 11;  // 22:12     
} H13Bx_IRIS_259_T;

/*-----------------------------------------------------------------------------
		0x4610 iris_260 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef4                  : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_hrs_cx2_coef3                  : 11;  // 22:12     
} H13Bx_IRIS_260_T;

/*-----------------------------------------------------------------------------
		0x4614 iris_261 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef6                  : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_hrs_cx2_coef5                  : 11;  // 22:12     
} H13Bx_IRIS_261_T;

/*-----------------------------------------------------------------------------
		0x4618 iris_262 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_cx2_coef8                  : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_hrs_cx2_coef7                  : 11;  // 22:12     
} H13Bx_IRIS_262_T;

/*-----------------------------------------------------------------------------
		0x461c iris_263 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hrs_ha_width                   : 8 ;  //  7: 0     
	UINT32 reg_hrs_ha_start                   : 8 ;  // 15: 8     
} H13Bx_IRIS_263_T;

/*-----------------------------------------------------------------------------
		0x4620 iris_264 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cti_ctrl0                      : 32;  // 31: 0     
} H13Bx_IRIS_264_T;

/*-----------------------------------------------------------------------------
		0x4624 iris_265 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cti_ctrl1                      : 32;  // 31: 0     
} H13Bx_IRIS_265_T;

/*-----------------------------------------------------------------------------
		0x4630 iris_266 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_3d_status0               : 32;  // 31: 0     
} H13Bx_IRIS_266_T;

/*-----------------------------------------------------------------------------
		0x4634 iris_267 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_6             : 32;  // 31: 0     
} H13Bx_IRIS_267_T;

/*-----------------------------------------------------------------------------
		0x4638 iris_268 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_7             : 32;  // 31: 0     
} H13Bx_IRIS_268_T;

/*-----------------------------------------------------------------------------
		0x463c iris_269 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycadjust_sat_ctrl_0            : 32;  // 31: 0     
} H13Bx_IRIS_269_T;

/*-----------------------------------------------------------------------------
		0x4640 iris_270 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl16                : 32;  // 31: 0     
} H13Bx_IRIS_270_T;

/*-----------------------------------------------------------------------------
		0x4644 iris_271 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl17                : 32;  // 31: 0     
} H13Bx_IRIS_271_T;

/*-----------------------------------------------------------------------------
		0x4648 iris_272 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_burst2_gate_end                : 8 ;  //  7: 0     
	UINT32 reg_burst2_gate_start              : 8 ;  // 15: 8     
	UINT32 reg_burst1_gate_end                : 8 ;  // 23:16     
	UINT32 reg_burst1_gate_start              : 8 ;  // 31:24     
} H13Bx_IRIS_272_T;

/*-----------------------------------------------------------------------------
		0x464c iris_273 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cordic2_gate_end               : 8 ;  //  7: 0     
	UINT32 reg_cordic2_gate_start             : 8 ;  // 15: 8     
	UINT32 reg_cordic1_gate_end               : 8 ;  // 23:16     
	UINT32 reg_cordic1_gate_start             : 8 ;  // 31:24     
} H13Bx_IRIS_273_T;

/*-----------------------------------------------------------------------------
		0x4650 iris_274 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_cbcr_ctrl                : 16;  // 15: 0     
	UINT32 reg_status_vdetect_vcount          : 10;  // 25:16     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_vactive_height_auto            : 1 ;  //    28     
} H13Bx_IRIS_274_T;

/*-----------------------------------------------------------------------------
		0x4654 iris_275 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_1           : 17;  // 16: 0     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_secam_y_notch_mode             : 1 ;  //    20     
} H13Bx_IRIS_275_T;

/*-----------------------------------------------------------------------------
		0x4658 iris_276 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_2           : 17;  // 16: 0     
} H13Bx_IRIS_276_T;

/*-----------------------------------------------------------------------------
		0x465c iris_277 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_3           : 17;  // 16: 0     
} H13Bx_IRIS_277_T;

/*-----------------------------------------------------------------------------
		0x4660 iris_278 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_4           : 17;  // 16: 0     
} H13Bx_IRIS_278_T;

/*-----------------------------------------------------------------------------
		0x4664 iris_279 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_5           : 17;  // 16: 0     
} H13Bx_IRIS_279_T;

/*-----------------------------------------------------------------------------
		0x4668 iris_280 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_6           : 17;  // 16: 0     
} H13Bx_IRIS_280_T;

/*-----------------------------------------------------------------------------
		0x466c iris_281 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_7           : 17;  // 16: 0     
} H13Bx_IRIS_281_T;

/*-----------------------------------------------------------------------------
		0x4670 iris_282 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_8           : 17;  // 16: 0     
} H13Bx_IRIS_282_T;

/*-----------------------------------------------------------------------------
		0x4674 iris_283 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_y_notch_ctrl_9           : 17;  // 16: 0     
} H13Bx_IRIS_283_T;

/*-----------------------------------------------------------------------------
		0x4678 iris_284 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_1           : 17;  // 16: 0     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_secam_c_notch_sel              : 1 ;  //    20     
} H13Bx_IRIS_284_T;

/*-----------------------------------------------------------------------------
		0x467c iris_285 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_2           : 17;  // 16: 0     
} H13Bx_IRIS_285_T;

/*-----------------------------------------------------------------------------
		0x4680 iris_286 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_3           : 17;  // 16: 0     
} H13Bx_IRIS_286_T;

/*-----------------------------------------------------------------------------
		0x4684 iris_287 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_4           : 17;  // 16: 0     
} H13Bx_IRIS_287_T;

/*-----------------------------------------------------------------------------
		0x4688 iris_288 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_5           : 17;  // 16: 0     
} H13Bx_IRIS_288_T;

/*-----------------------------------------------------------------------------
		0x468c iris_289 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_6           : 17;  // 16: 0     
} H13Bx_IRIS_289_T;

/*-----------------------------------------------------------------------------
		0x4690 iris_290 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_7           : 17;  // 16: 0     
} H13Bx_IRIS_290_T;

/*-----------------------------------------------------------------------------
		0x4694 iris_291 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_8           : 17;  // 16: 0     
} H13Bx_IRIS_291_T;

/*-----------------------------------------------------------------------------
		0x4698 iris_292 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_c_notch_ctrl_9           : 17;  // 16: 0     
} H13Bx_IRIS_292_T;

/*-----------------------------------------------------------------------------
		0x469c iris_293 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_ctrl11                   : 32;  // 31: 0     
} H13Bx_IRIS_293_T;

/*-----------------------------------------------------------------------------
		0x46a0 iris_294 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_ybw2                     : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_secam_y_delay                  : 6 ;  //  9: 4     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_secam_dcr_passthrough          : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_v_valid_inv                    : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_secam_lb_en                    : 1 ;  //    20     
} H13Bx_IRIS_294_T;

/*-----------------------------------------------------------------------------
		0x46a4 iris_295 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_dcr_ctrl_0               : 32;  // 31: 0     
} H13Bx_IRIS_295_T;

/*-----------------------------------------------------------------------------
		0x46a8 iris_296 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_dcr_ctrl_1               : 32;  // 31: 0     
} H13Bx_IRIS_296_T;

/*-----------------------------------------------------------------------------
		0x46ac iris_297 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_secam_dcr_ctrl_2               : 32;  // 31: 0     
} H13Bx_IRIS_297_T;

/*-----------------------------------------------------------------------------
		0x46b0 iris_298 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_yup_rise                       : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_ydn_cnt                        : 8 ;  // 19:12     
	UINT32 reg_yup_cnt                        : 8 ;  // 27:20     
} H13Bx_IRIS_298_T;

/*-----------------------------------------------------------------------------
		0x46b4 iris_299 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ydn_rise                       : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 reg_yup_fall                       : 11;  // 22:12     
} H13Bx_IRIS_299_T;

/*-----------------------------------------------------------------------------
		0x46b8 iris_300 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_clmap_sel                      : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_ydn_fall                       : 11;  // 14: 4     
} H13Bx_IRIS_300_T;

/*-----------------------------------------------------------------------------
		0x46bc iris_301 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl18                : 32;  // 31: 0     
} H13Bx_IRIS_301_T;

/*-----------------------------------------------------------------------------
		0x46c0 iris_302 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl19                : 32;  // 31: 0     
} H13Bx_IRIS_302_T;

/*-----------------------------------------------------------------------------
		0x46c4 iris_303 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl20                : 32;  // 31: 0     
} H13Bx_IRIS_303_T;

/*-----------------------------------------------------------------------------
		0x46c8 iris_304 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wmask_ctrl_0                   : 32;  // 31: 0     
} H13Bx_IRIS_304_T;

/*-----------------------------------------------------------------------------
		0x46cc iris_305 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wmask_ctrl_1                   : 32;  // 31: 0     
} H13Bx_IRIS_305_T;

/*-----------------------------------------------------------------------------
		0x46d0 iris_306 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdetect_lpf_ctrl_8             : 32;  // 31: 0     
} H13Bx_IRIS_306_T;

/*-----------------------------------------------------------------------------
		0x46d4 iris_307 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cdct_ifcomp_ctrl_0             : 32;  // 31: 0     
} H13Bx_IRIS_307_T;

/*-----------------------------------------------------------------------------
		0x46d8 iris_308 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl21                : 32;  // 31: 0     
} H13Bx_IRIS_308_T;

/*-----------------------------------------------------------------------------
		0x46dc iris_309 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl22                : 32;  // 31: 0     
} H13Bx_IRIS_309_T;

/*-----------------------------------------------------------------------------
		0x46e0 iris_310 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl23                : 32;  // 31: 0     
} H13Bx_IRIS_310_T;

/*-----------------------------------------------------------------------------
		0x46e4 iris_311 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl24                : 32;  // 31: 0     
} H13Bx_IRIS_311_T;

/*-----------------------------------------------------------------------------
		0x46e8 iris_312 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl25                : 32;  // 31: 0     
} H13Bx_IRIS_312_T;

/*-----------------------------------------------------------------------------
		0x46ec iris_313 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl26                : 32;  // 31: 0     
} H13Bx_IRIS_313_T;

/*-----------------------------------------------------------------------------
		0x46f0 iris_314 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl27                : 32;  // 31: 0     
} H13Bx_IRIS_314_T;

/*-----------------------------------------------------------------------------
		0x46f4 iris_315 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl28                : 32;  // 31: 0     
} H13Bx_IRIS_315_T;

/*-----------------------------------------------------------------------------
		0x46f8 iris_316 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl29                : 32;  // 31: 0     
} H13Bx_IRIS_316_T;

/*-----------------------------------------------------------------------------
		0x46fc iris_317 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl30                : 32;  // 31: 0     
} H13Bx_IRIS_317_T;

/*-----------------------------------------------------------------------------
		0x4700 iris_318 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl31                : 32;  // 31: 0     
} H13Bx_IRIS_318_T;

/*-----------------------------------------------------------------------------
		0x4704 iris_319 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl32                : 32;  // 31: 0     
} H13Bx_IRIS_319_T;

/*-----------------------------------------------------------------------------
		0x4708 iris_320 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl33                : 32;  // 31: 0     
} H13Bx_IRIS_320_T;

/*-----------------------------------------------------------------------------
		0x470c iris_321 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl34                : 32;  // 31: 0     
} H13Bx_IRIS_321_T;

/*-----------------------------------------------------------------------------
		0x4710 iris_322 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl35                : 32;  // 31: 0     
} H13Bx_IRIS_322_T;

/*-----------------------------------------------------------------------------
		0x4714 iris_323 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl36                : 32;  // 31: 0     
} H13Bx_IRIS_323_T;

/*-----------------------------------------------------------------------------
		0x4718 iris_324 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl8                 : 32;  // 31: 0     
} H13Bx_IRIS_324_T;

/*-----------------------------------------------------------------------------
		0x471c iris_325 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl9                 : 32;  // 31: 0     
} H13Bx_IRIS_325_T;

/*-----------------------------------------------------------------------------
		0x4720 iris_326 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl10                : 32;  // 31: 0     
} H13Bx_IRIS_326_T;

/*-----------------------------------------------------------------------------
		0x4724 iris_327 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl11                : 32;  // 31: 0     
} H13Bx_IRIS_327_T;

/*-----------------------------------------------------------------------------
		0x4728 iris_328 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl12                : 32;  // 31: 0     
} H13Bx_IRIS_328_T;

/*-----------------------------------------------------------------------------
		0x472c iris_329 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl13                : 32;  // 31: 0     
} H13Bx_IRIS_329_T;

/*-----------------------------------------------------------------------------
		0x4730 iris_330 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl14                : 32;  // 31: 0     
} H13Bx_IRIS_330_T;

/*-----------------------------------------------------------------------------
		0x4734 iris_331 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl15                : 32;  // 31: 0     
} H13Bx_IRIS_331_T;

/*-----------------------------------------------------------------------------
		0x4738 iris_332 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl16                : 32;  // 31: 0     
} H13Bx_IRIS_332_T;

/*-----------------------------------------------------------------------------
		0x473c iris_333 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl17                : 32;  // 31: 0     
} H13Bx_IRIS_333_T;

/*-----------------------------------------------------------------------------
		0x4740 iris_334 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl37                : 32;  // 31: 0     
} H13Bx_IRIS_334_T;

/*-----------------------------------------------------------------------------
		0x4744 iris_335 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl38                : 32;  // 31: 0     
} H13Bx_IRIS_335_T;

/*-----------------------------------------------------------------------------
		0x4748 iris_336 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl39                : 32;  // 31: 0     
} H13Bx_IRIS_336_T;

/*-----------------------------------------------------------------------------
		0x474c iris_337 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl40                : 32;  // 31: 0     
} H13Bx_IRIS_337_T;

/*-----------------------------------------------------------------------------
		0x4750 iris_338 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl41                : 32;  // 31: 0     
} H13Bx_IRIS_338_T;

/*-----------------------------------------------------------------------------
		0x4754 iris_339 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl42                : 32;  // 31: 0     
} H13Bx_IRIS_339_T;

/*-----------------------------------------------------------------------------
		0x4758 iris_340 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl43                : 32;  // 31: 0     
} H13Bx_IRIS_340_T;

/*-----------------------------------------------------------------------------
		0x475c iris_341 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl44                : 32;  // 31: 0     
} H13Bx_IRIS_341_T;

/*-----------------------------------------------------------------------------
		0x4760 iris_342 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl45                : 32;  // 31: 0     
} H13Bx_IRIS_342_T;

/*-----------------------------------------------------------------------------
		0x4764 iris_343 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl46                : 32;  // 31: 0     
} H13Bx_IRIS_343_T;

/*-----------------------------------------------------------------------------
		0x4768 iris_344 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cordic_ctrl0                   : 32;  // 31: 0     
} H13Bx_IRIS_344_T;

/*-----------------------------------------------------------------------------
		0x476c iris_345 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs_cordic_ctrl0                : 32;  // 31: 0     
} H13Bx_IRIS_345_T;

/*-----------------------------------------------------------------------------
		0x4770 iris_346 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cs1_cordic_ctrl0               : 32;  // 31: 0     
} H13Bx_IRIS_346_T;

/*-----------------------------------------------------------------------------
		0x4774 iris_347 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl0               : 32;  // 31: 0     
} H13Bx_IRIS_347_T;

/*-----------------------------------------------------------------------------
		0x4778 iris_348 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl1               : 32;  // 31: 0     
} H13Bx_IRIS_348_T;

/*-----------------------------------------------------------------------------
		0x477c iris_349 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl2               : 32;  // 31: 0     
} H13Bx_IRIS_349_T;

/*-----------------------------------------------------------------------------
		0x4780 iris_350 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl3               : 32;  // 31: 0     
} H13Bx_IRIS_350_T;

/*-----------------------------------------------------------------------------
		0x4784 iris_351 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl47                : 32;  // 31: 0     
} H13Bx_IRIS_351_T;

/*-----------------------------------------------------------------------------
		0x4788 iris_352 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl48                : 32;  // 31: 0     
} H13Bx_IRIS_352_T;

/*-----------------------------------------------------------------------------
		0x478c iris_353 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl49                : 32;  // 31: 0     
} H13Bx_IRIS_353_T;

/*-----------------------------------------------------------------------------
		0x4790 iris_354 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl50                : 32;  // 31: 0     
} H13Bx_IRIS_354_T;

/*-----------------------------------------------------------------------------
		0x4794 iris_355 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_2d_ctrl51                : 32;  // 31: 0     
} H13Bx_IRIS_355_T;

/*-----------------------------------------------------------------------------
		0x4798 iris_356 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl4               : 32;  // 31: 0     
} H13Bx_IRIS_356_T;

/*-----------------------------------------------------------------------------
		0x479c iris_357 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl5               : 32;  // 31: 0     
} H13Bx_IRIS_357_T;

/*-----------------------------------------------------------------------------
		0x47a0 iris_358 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl6               : 32;  // 31: 0     
} H13Bx_IRIS_358_T;

/*-----------------------------------------------------------------------------
		0x47a4 iris_359 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hdetect_nr_ctrl7               : 32;  // 31: 0     
} H13Bx_IRIS_359_T;

/*-----------------------------------------------------------------------------
		0x47a8 iris_360 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_clpf_ctrl0                  : 32;  // 31: 0     
} H13Bx_IRIS_360_T;

/*-----------------------------------------------------------------------------
		0x47ac iris_361 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_clpf_ctrl1                  : 32;  // 31: 0     
} H13Bx_IRIS_361_T;

/*-----------------------------------------------------------------------------
		0x47b0 iris_362 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_clpf_ctrl2                  : 32;  // 31: 0     
} H13Bx_IRIS_362_T;

/*-----------------------------------------------------------------------------
		0x47b4 iris_363 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_fb_clpf_ctrl3                  : 32;  // 31: 0     
} H13Bx_IRIS_363_T;

/*-----------------------------------------------------------------------------
		0x47b8 iris_364 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_fb_in_cnt               : 20;  // 19: 0     
} H13Bx_IRIS_364_T;

/*-----------------------------------------------------------------------------
		0x47bc iris_365 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl18                : 32;  // 31: 0     
} H13Bx_IRIS_365_T;

/*-----------------------------------------------------------------------------
		0x47c0 iris_366 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl19                : 32;  // 31: 0     
} H13Bx_IRIS_366_T;

/*-----------------------------------------------------------------------------
		0x47c4 iris_367 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl20                : 32;  // 31: 0     
} H13Bx_IRIS_367_T;

/*-----------------------------------------------------------------------------
		0x47c8 iris_368 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl21                : 32;  // 31: 0     
} H13Bx_IRIS_368_T;

/*-----------------------------------------------------------------------------
		0x47cc iris_369 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl22                : 32;  // 31: 0     
} H13Bx_IRIS_369_T;

/*-----------------------------------------------------------------------------
		0x47d0 iris_370 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_st_ctrl23                : 32;  // 31: 0     
} H13Bx_IRIS_370_T;

/*-----------------------------------------------------------------------------
		0x4800 vbi_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wss625_rd_done                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cc_rd_done                     : 1 ;  //     4     
} H13Bx_VBI_001_T;

/*-----------------------------------------------------------------------------
		0x4804 vbi_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cc_rdy                         : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_cc_data0                       : 8 ;  // 11: 4     
	UINT32 reg_cc_data1                       : 8 ;  // 19:12     
} H13Bx_VBI_002_T;

/*-----------------------------------------------------------------------------
		0x4808 vbi_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_wssdata0                       : 8 ;  //  7: 0     
	UINT32 reg_wssdata1                       : 8 ;  // 15: 8     
	UINT32 reg_wssdata2                       : 8 ;  // 23:16     
	UINT32 reg_wss_rdy                        : 1 ;  //    24     
} H13Bx_VBI_003_T;

/*-----------------------------------------------------------------------------
		0x480c vbi_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cc_short_start                 : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 reg_vbi_muxout                     : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 reg_vbi_hsyncout                   : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_adap_slvl_en                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vbi_st_err_ignored             : 1 ;  //    16     
	UINT32                                    : 3 ;  // 19:17     reserved
	UINT32 reg_vbi_en                         : 1 ;  //    20     
} H13Bx_VBI_004_T;

/*-----------------------------------------------------------------------------
		0x4810 vbi_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_lpf_bw                     : 2 ;  //  1: 0     
} H13Bx_VBI_005_T;

/*-----------------------------------------------------------------------------
		0x4814 vbi_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_data_hlvl                  : 8 ;  //  7: 0     
	UINT32 reg_start_code                     : 8 ;  // 15: 8     
} H13Bx_VBI_006_T;

/*-----------------------------------------------------------------------------
		0x4818 vbi_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil9                          : 8 ;  //  7: 0     
	UINT32 reg_vbil8                          : 8 ;  // 15: 8     
	UINT32 reg_vbil7                          : 8 ;  // 23:16     
	UINT32 reg_vbil6                          : 8 ;  // 31:24     
} H13Bx_VBI_007_T;

/*-----------------------------------------------------------------------------
		0x481c vbi_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil13                         : 8 ;  //  7: 0     
	UINT32 reg_vbil12                         : 8 ;  // 15: 8     
	UINT32 reg_vbil11                         : 8 ;  // 23:16     
	UINT32 reg_vbil10                         : 8 ;  // 31:24     
} H13Bx_VBI_008_T;

/*-----------------------------------------------------------------------------
		0x4820 vbi_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil17                         : 8 ;  //  7: 0     
	UINT32 reg_vbil16                         : 8 ;  // 15: 8     
	UINT32 reg_vbil15                         : 8 ;  // 23:16     
	UINT32 reg_vbil14                         : 8 ;  // 31:24     
} H13Bx_VBI_009_T;

/*-----------------------------------------------------------------------------
		0x4824 vbi_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil21                         : 8 ;  //  7: 0     
	UINT32 reg_vbil20                         : 8 ;  // 15: 8     
	UINT32 reg_vbil19                         : 8 ;  // 23:16     
	UINT32 reg_vbil18                         : 8 ;  // 31:24     
} H13Bx_VBI_010_T;

/*-----------------------------------------------------------------------------
		0x4828 vbi_011 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil25                         : 8 ;  //  7: 0     
	UINT32 reg_vbil24                         : 8 ;  // 15: 8     
	UINT32 reg_vbil23                         : 8 ;  // 23:16     
	UINT32 reg_vbil22                         : 8 ;  // 31:24     
} H13Bx_VBI_011_T;

/*-----------------------------------------------------------------------------
		0x482c vbi_012 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbil26                         : 8 ;  //  7: 0     
	UINT32 reg_vbil5                          : 8 ;  // 15: 8     
} H13Bx_VBI_012_T;

/*-----------------------------------------------------------------------------
		0x4830 vbi_013 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_vps_lpfil_fine_gain        : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_vbi_wss625_lpfil_fine_gain     : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_vbi_tele_lpfil_fine_gain       : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_vbi_cc_lpfil_fine_gain         : 2 ;  // 13:12     
	UINT32                                    : 2 ;  // 15:14     reserved
	UINT32 reg_vbi_vps_slicer_mode            : 2 ;  // 17:16     
	UINT32                                    : 2 ;  // 19:18     reserved
	UINT32 reg_vbi_wss_slicer_mode            : 2 ;  // 21:20     
	UINT32                                    : 2 ;  // 23:22     reserved
	UINT32 reg_vbi_tt_slicer_mode             : 2 ;  // 25:24     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_vbi_cc_slicer_mode             : 2 ;  // 29:28     
} H13Bx_VBI_013_T;

/*-----------------------------------------------------------------------------
		0x4834 vbi_014 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_teletext_dto                   : 16;  // 15: 0     
	UINT32 reg_caption_dto                    : 16;  // 31:16     
} H13Bx_VBI_014_T;

/*-----------------------------------------------------------------------------
		0x4838 vbi_015 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_dto                        : 16;  // 15: 0     
	UINT32 reg_wss625_dto                     : 16;  // 31:16     
} H13Bx_VBI_015_T;

/*-----------------------------------------------------------------------------
		0x483c vbi_016 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_frame_start                : 2 ;  //  1: 0     
	UINT32                                    : 2 ;  //  3: 2     reserved
	UINT32 reg_teletext_frame_start           : 2 ;  //  5: 4     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 reg_wss625_frame_start             : 2 ;  //  9: 8     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 reg_caption_frame_start            : 2 ;  // 13:12     
} H13Bx_VBI_016_T;

/*-----------------------------------------------------------------------------
		0x4840 vbi_017 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_tele_lpfil_track_gain      : 4 ;  //  3: 0     
	UINT32 reg_vbi_tele_lpfil_acq_gain        : 4 ;  //  7: 4     
	UINT32 reg_vbi_cc_lpfil_track_gain        : 4 ;  // 11: 8     
	UINT32 reg_vbi_cc_lpfil_acq_gain          : 4 ;  // 15:12     
	UINT32 reg_wssj_delta_ampl                : 8 ;  // 23:16     
} H13Bx_VBI_017_T;

/*-----------------------------------------------------------------------------
		0x4844 vbi_018 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_vps_lpfil_track_gain       : 4 ;  //  3: 0     
	UINT32 reg_vbi_vps_lpfil_acq_gain         : 4 ;  //  7: 4     
	UINT32 reg_vbi_wss625_lpfil_track_gain    : 4 ;  // 11: 8     
	UINT32 reg_vbi_wss625_lpfil_acq_gain      : 4 ;  // 15:12     
} H13Bx_VBI_018_T;

/*-----------------------------------------------------------------------------
		0x4848 vbi_019 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_teletext_runin_accum_ampl      : 8 ;  //  7: 0     
	UINT32 reg_caption_runin_accum_ampl       : 8 ;  // 15: 8     
} H13Bx_VBI_019_T;

/*-----------------------------------------------------------------------------
		0x484c vbi_020 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vps_start                      : 8 ;  //  7: 0     
	UINT32 reg_teletext_start                 : 8 ;  // 15: 8     
	UINT32 reg_wss625_start                   : 8 ;  // 23:16     
	UINT32 reg_caption_start                  : 8 ;  // 31:24     
} H13Bx_VBI_020_T;

/*-----------------------------------------------------------------------------
		0x4850 vbi_021 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_frame_en_cnt                   : 9 ;  //  8: 0     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 reg_frame_en_adj                   : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_vbi_fixgate_en                 : 1 ;  //    16     
} H13Bx_VBI_021_T;

/*-----------------------------------------------------------------------------
		0x4854 vbi_022 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi1_detect_cnt                : 16;  // 15: 0     
	UINT32 reg_vbi_monitor_line               : 10;  // 25:16     
	UINT32                                    : 2 ;  // 27:26     reserved
	UINT32 reg_vbi_monitor_ctrl               : 3 ;  // 30:28     
} H13Bx_VBI_022_T;

/*-----------------------------------------------------------------------------
		0x4858 vbi_023 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl0                      : 32;  // 31: 0     
} H13Bx_VBI_023_T;

/*-----------------------------------------------------------------------------
		0x485c vbi_024 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl1                      : 32;  // 31: 0     
} H13Bx_VBI_024_T;

/*-----------------------------------------------------------------------------
		0x4860 vbi_025 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl2                      : 32;  // 31: 0     
} H13Bx_VBI_025_T;

/*-----------------------------------------------------------------------------
		0x4864 vbi_026 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl3                      : 32;  // 31: 0     
} H13Bx_VBI_026_T;

/*-----------------------------------------------------------------------------
		0x4868 vbi_027 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl4                      : 32;  // 31: 0     
} H13Bx_VBI_027_T;

/*-----------------------------------------------------------------------------
		0x486c vbi_028 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl5                      : 32;  // 31: 0     
} H13Bx_VBI_028_T;

/*-----------------------------------------------------------------------------
		0x4870 vbi_029 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl6                      : 32;  // 31: 0     
} H13Bx_VBI_029_T;

/*-----------------------------------------------------------------------------
		0x4874 vbi_030 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl7                      : 32;  // 31: 0     
} H13Bx_VBI_030_T;

/*-----------------------------------------------------------------------------
		0x4878 vbi_031 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl8                      : 32;  // 31: 0     
} H13Bx_VBI_031_T;

/*-----------------------------------------------------------------------------
		0x487c vbi_032 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl9                      : 32;  // 31: 0     
} H13Bx_VBI_032_T;

/*-----------------------------------------------------------------------------
		0x4880 vbi_033 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl10                     : 32;  // 31: 0     
} H13Bx_VBI_033_T;

/*-----------------------------------------------------------------------------
		0x4884 vbi_034 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl11                     : 32;  // 31: 0     
} H13Bx_VBI_034_T;

/*-----------------------------------------------------------------------------
		0x4888 vbi_035 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl12                     : 32;  // 31: 0     
} H13Bx_VBI_035_T;

/*-----------------------------------------------------------------------------
		0x488c vbi_036 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl13                     : 32;  // 31: 0     
} H13Bx_VBI_036_T;

/*-----------------------------------------------------------------------------
		0x4890 vbi_037 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl14                     : 32;  // 31: 0     
} H13Bx_VBI_037_T;

/*-----------------------------------------------------------------------------
		0x4894 vbi_038 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_vbi_ctrl15                     : 32;  // 31: 0     
} H13Bx_VBI_038_T;

/*-----------------------------------------------------------------------------
		0x4898 agc_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_sw_dgain                       : 12;  // 11: 0     
	UINT32 reg_sw_dgain_field_mode            : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 reg_sw_dgain_on                    : 1 ;  //    16     
} H13Bx_AGC_001_T;

/*-----------------------------------------------------------------------------
		0x489c lvl_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl0                : 32;  // 31: 0     
} H13Bx_LVL_001_T;

/*-----------------------------------------------------------------------------
		0x48a0 lvl_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl1                : 32;  // 31: 0     
} H13Bx_LVL_002_T;

/*-----------------------------------------------------------------------------
		0x48a4 lvl_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl2                : 32;  // 31: 0     
} H13Bx_LVL_003_T;

/*-----------------------------------------------------------------------------
		0x48a8 lvl_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl3                : 32;  // 31: 0     
} H13Bx_LVL_004_T;

/*-----------------------------------------------------------------------------
		0x48ac lvl_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl4                : 32;  // 31: 0     
} H13Bx_LVL_005_T;

/*-----------------------------------------------------------------------------
		0x48b0 lvl_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl5                : 32;  // 31: 0     
} H13Bx_LVL_006_T;

/*-----------------------------------------------------------------------------
		0x48b4 lvl_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl6                : 32;  // 31: 0     
} H13Bx_LVL_007_T;

/*-----------------------------------------------------------------------------
		0x48b8 lvl_008 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl7                : 32;  // 31: 0     
} H13Bx_LVL_008_T;

/*-----------------------------------------------------------------------------
		0x48bc lvl_009 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_ycsep_lvl_ctrl8                : 32;  // 31: 0     
} H13Bx_LVL_009_T;

/*-----------------------------------------------------------------------------
		0x48c0 lvl_010 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl0              : 32;  // 31: 0     
} H13Bx_LVL_010_T;

/*-----------------------------------------------------------------------------
		0x48c4 lvl_011 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl1              : 32;  // 31: 0     
} H13Bx_LVL_011_T;

/*-----------------------------------------------------------------------------
		0x48c8 lvl_012 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl2              : 32;  // 31: 0     
} H13Bx_LVL_012_T;

/*-----------------------------------------------------------------------------
		0x48cc lvl_013 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl3              : 32;  // 31: 0     
} H13Bx_LVL_013_T;

/*-----------------------------------------------------------------------------
		0x48d0 lvl_014 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl4              : 32;  // 31: 0     
} H13Bx_LVL_014_T;

/*-----------------------------------------------------------------------------
		0x48d4 lvl_015 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl5              : 32;  // 31: 0     
} H13Bx_LVL_015_T;

/*-----------------------------------------------------------------------------
		0x48d8 lvl_016 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl6              : 32;  // 31: 0     
} H13Bx_LVL_016_T;

/*-----------------------------------------------------------------------------
		0x48dc lvl_017 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl7              : 32;  // 31: 0     
} H13Bx_LVL_017_T;

/*-----------------------------------------------------------------------------
		0x48e0 lvl_018 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl8              : 32;  // 31: 0     
} H13Bx_LVL_018_T;

/*-----------------------------------------------------------------------------
		0x48e4 lvl_019 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl9              : 32;  // 31: 0     
} H13Bx_LVL_019_T;

/*-----------------------------------------------------------------------------
		0x48e8 lvl_020 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl10             : 32;  // 31: 0     
} H13Bx_LVL_020_T;

/*-----------------------------------------------------------------------------
		0x48ec lvl_021 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_status_ycsep_lvl11             : 32;  // 31: 0     
} H13Bx_LVL_021_T;

/*-----------------------------------------------------------------------------
		0x48f0 iris_mif_gmua_mon_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_rbuf2_pel_cnt                   : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 ro_rbuf2_empty                     : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 ro_rbuf1_pel_cnt                   : 11;  // 26:16     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 ro_rbuf1_empty                     : 1 ;  //    28     
} H13Bx_IRIS_MIF_GMUA_MON_001_T;

/*-----------------------------------------------------------------------------
		0x48f4 iris_mif_gmua_mon_002 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_rbuf4_pel_cnt                   : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 ro_rbuf4_empty                     : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 ro_rbuf3_pel_cnt                   : 11;  // 26:16     
	UINT32                                    : 1 ;  //    27     reserved
	UINT32 ro_rbuf3_empty                     : 1 ;  //    28     
} H13Bx_IRIS_MIF_GMUA_MON_002_T;

/*-----------------------------------------------------------------------------
		0x48f8 iris_mif_gmua_mon_003 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_wbuf_word_cnt                   : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 ro_wbuf_empty                      : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 ro_wbuf_full                       : 1 ;  //    12     
} H13Bx_IRIS_MIF_GMUA_MON_003_T;

/*-----------------------------------------------------------------------------
		0x48fc iris_mif_gmua_mon_004 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_wbuf_read_req_cnt_mx_lvl        : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 ro_wbuf_read_req_cnt               : 6 ;  //  9: 4     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 ro_read0_svid_cnt_mx_lvl           : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 ro_read0_svid_cnt                  : 10;  // 25:16     
} H13Bx_IRIS_MIF_GMUA_MON_004_T;

/*-----------------------------------------------------------------------------
		0x4900 iris_mif_gmua_mon_005 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_rbuf1_pel_cnt_md                : 11;  // 10: 0     
	UINT32                                    : 1 ;  //    11     reserved
	UINT32 ro_rbuf1_empty_md                  : 1 ;  //    12     
} H13Bx_IRIS_MIF_GMUA_MON_005_T;

/*-----------------------------------------------------------------------------
		0x4904 iris_mif_gmua_mon_006 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_wbuf_word_cnt_md                : 6 ;  //  5: 0     
	UINT32                                    : 2 ;  //  7: 6     reserved
	UINT32 ro_wbuf_empty_md                   : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 ro_wbuf_full_md                    : 1 ;  //    12     
} H13Bx_IRIS_MIF_GMUA_MON_006_T;

/*-----------------------------------------------------------------------------
		0x4908 iris_mif_gmua_mon_007 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ro_wbuf_read_req_cnt_mx_lvl_md     : 1 ;  //     0     
	UINT32                                    : 3 ;  //  3: 1     reserved
	UINT32 ro_wbuf_read_req_cnt_md            : 6 ;  //  9: 4     
	UINT32                                    : 2 ;  // 11:10     reserved
	UINT32 ro_read0_svid_cnt_mx_lvl_md        : 1 ;  //    12     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 ro_read0_svid_cnt_md               : 10;  // 25:16     
} H13Bx_IRIS_MIF_GMUA_MON_007_T;

/*-----------------------------------------------------------------------------
		0x490c iris_hif_ctrl0_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_pending_enable                 : 1 ;  //     0     
} H13Bx_IRIS_HIF_CTRL0_001_T;

/*-----------------------------------------------------------------------------
		0x4910 iris_hif_ctrl1_001 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_cv_timer_ctrl                  : 9 ;  //  8: 0     
} H13Bx_IRIS_HIF_CTRL1_001_T;
typedef struct {
	H13Bx_CVD_INTR1_ENABLE0_T          	cvd_intr1_enable0;            //0x4100
	H13Bx_CVD_INTR1_INTR0_T            	cvd_intr1_intr0;              //0x4104
	H13Bx_CVD_INTR1_CLEAR0_T           	cvd_intr1_clear0;             //0x4108
	H13Bx_CVD_INTR1_ENABLE1_T          	cvd_intr1_enable1;            //0x410c
	H13Bx_CVD_INTR1_INTR1_T            	cvd_intr1_intr1;              //0x4110
	H13Bx_CVD_INTR1_CLEAR1_T           	cvd_intr1_clear1;             //0x4114
	H13Bx_CVD_INTR2_ENABLE0_T          	cvd_intr2_enable0;            //0x4118
	H13Bx_CVD_INTR2_INTR0_T            	cvd_intr2_intr0;              //0x411c
	H13Bx_CVD_INTR2_CLEAR0_T           	cvd_intr2_clear0;             //0x4120
	H13Bx_CVD_INTR2_ENABLE1_T          	cvd_intr2_enable1;            //0x4124
	H13Bx_CVD_INTR2_INTR1_T            	cvd_intr2_intr1;              //0x4128
	H13Bx_CVD_INTR2_CLEAR1_T           	cvd_intr2_clear1;             //0x412c
	H13Bx_TOP_001_T                    	top_001;                      //0x4130
	UINT32                             	reserved00[3];                //0x4134~0x413c
	H13Bx_IRIS_MIF_GMUA_001_T          	iris_mif_gmua_001;            //0x4140
	H13Bx_IRIS_MIF_GMUA_002_T          	iris_mif_gmua_002;            //0x4144
	H13Bx_IRIS_MIF_GMUA_003_T          	iris_mif_gmua_003;            //0x4148
	H13Bx_IRIS_MIF_GMUA_004_T          	iris_mif_gmua_004;            //0x414c
	H13Bx_IRIS_MIF_GMUA_005_T          	iris_mif_gmua_005;            //0x4150
	H13Bx_IRIS_MIF_GMUA_006_T          	iris_mif_gmua_006;            //0x4154
	H13Bx_IRIS_MIF_GMUA_007_T          	iris_mif_gmua_007;            //0x4158
	H13Bx_IRIS_MIF_GMUA_008_T          	iris_mif_gmua_008;            //0x415c
	H13Bx_IRIS_MIF_GMUA_009_T          	iris_mif_gmua_009;            //0x4160
	H13Bx_IRIS_MIF_GMUA_010_T          	iris_mif_gmua_010;            //0x4164
	H13Bx_FASTBLANK_001_T              	fastblank_001;                //0x4168
	H13Bx_FASTBLANK_002_T              	fastblank_002;                //0x416c
	H13Bx_FASTBLANK_003_T              	fastblank_003;                //0x4170
	H13Bx_FASTBLANK_004_T              	fastblank_004;                //0x4174
	H13Bx_FASTBLANK_005_T              	fastblank_005;                //0x4178
	H13Bx_FASTBLANK_006_T              	fastblank_006;                //0x417c
	H13Bx_FASTBLANK_007_T              	fastblank_007;                //0x4180
	H13Bx_FASTBLANK_008_T              	fastblank_008;                //0x4184
	H13Bx_FASTBLANK_009_T              	fastblank_009;                //0x4188
	H13Bx_FASTBLANK_010_T              	fastblank_010;                //0x418c
	H13Bx_IRIS_DE_CTRL_001_T           	iris_de_ctrl_001;             //0x4190
	H13Bx_VBI_CTRL_001_T               	vbi_ctrl_001;                 //0x4194
	H13Bx_VBI_CTRL_002_T               	vbi_ctrl_002;                 //0x4198
	H13Bx_VBI_CTRL_003_T               	vbi_ctrl_003;                 //0x419c
	H13Bx_VBI_CTRL_004_T               	vbi_ctrl_004;                 //0x41a0
	H13Bx_VBI_CTRL_005_T               	vbi_ctrl_005;                 //0x41a4
	H13Bx_VBI_CTRL_006_T               	vbi_ctrl_006;                 //0x41a8
	H13Bx_VBI_CTRL_007_T               	vbi_ctrl_007;                 //0x41ac
	H13Bx_VBI_CTRL_008_T               	vbi_ctrl_008;                 //0x41b0
	H13Bx_VBI_CTRL_009_T               	vbi_ctrl_009;                 //0x41b4
	H13Bx_VBI_CTRL_010_T               	vbi_ctrl_010;                 //0x41b8
	UINT32                             	reserved01;                   //0x41bc
	H13Bx_FAST_BLANK_STATUS_001_T      	fast_blank_status_001;        //0x41c0
	H13Bx_CVBSAFE_001_T                	cvbsafe_001;                  //0x41c4
	H13Bx_IRIS_MIF_GMUA_MD_001_T       	iris_mif_gmua_md_001;         //0x41c8
	H13Bx_IRIS_MIF_GMUA_MD_002_T       	iris_mif_gmua_md_002;         //0x41cc
	H13Bx_IRIS_MIF_GMUA_MD_003_T       	iris_mif_gmua_md_003;         //0x41d0
	H13Bx_IRIS_MIF_GMUA_MD_004_T       	iris_mif_gmua_md_004;         //0x41d4
	H13Bx_IRIS_MIF_GMUA_MD_005_T       	iris_mif_gmua_md_005;         //0x41d8
	H13Bx_IRIS_MIF_GMUA_MD_006_T       	iris_mif_gmua_md_006;         //0x41dc
	H13Bx_IRIS_MIF_GMUA_MD_007_T       	iris_mif_gmua_md_007;         //0x41e0
	H13Bx_IRIS_MIF_GMUA_MD_008_T       	iris_mif_gmua_md_008;         //0x41e4
	H13Bx_IRIS_MIF_GMUA_MD_009_T       	iris_mif_gmua_md_009;         //0x41e8
	H13Bx_IRIS_MIF_GMUA_MD_010_T       	iris_mif_gmua_md_010;         //0x41ec
	H13Bx_IRIS_MIF_GMUA_MD_011_T       	iris_mif_gmua_md_011;         //0x41f0
	UINT32                             	reserved02[3];                //0x41f4~0x41fc
	H13Bx_IRIS_001_T                   	iris_001;                     //0x4200
	H13Bx_IRIS_002_T                   	iris_002;                     //0x4204
	H13Bx_IRIS_003_T                   	iris_003;                     //0x4208
	H13Bx_IRIS_004_T                   	iris_004;                     //0x420c
	H13Bx_IRIS_005_T                   	iris_005;                     //0x4210
	H13Bx_IRIS_006_T                   	iris_006;                     //0x4214
	H13Bx_IRIS_007_T                   	iris_007;                     //0x4218
	H13Bx_IRIS_008_T                   	iris_008;                     //0x421c
	H13Bx_IRIS_009_T                   	iris_009;                     //0x4220
	H13Bx_IRIS_010_T                   	iris_010;                     //0x4224
	H13Bx_IRIS_011_T                   	iris_011;                     //0x4228
	UINT32                             	reserved03;                   //0x422c
	H13Bx_IRIS_012_T                   	iris_012;                     //0x4230
	H13Bx_IRIS_013_T                   	iris_013;                     //0x4234
	H13Bx_IRIS_014_T                   	iris_014;                     //0x4238
	H13Bx_IRIS_015_T                   	iris_015;                     //0x423c
	H13Bx_IRIS_016_T                   	iris_016;                     //0x4240
	H13Bx_IRIS_017_T                   	iris_017;                     //0x4244
	H13Bx_IRIS_018_T                   	iris_018;                     //0x4248
	H13Bx_IRIS_019_T                   	iris_019;                     //0x424c
	H13Bx_IRIS_020_T                   	iris_020;                     //0x4250
	H13Bx_IRIS_021_T                   	iris_021;                     //0x4254
	H13Bx_IRIS_022_T                   	iris_022;                     //0x4258
	H13Bx_IRIS_023_T                   	iris_023;                     //0x425c
	H13Bx_IRIS_024_T                   	iris_024;                     //0x4260
	H13Bx_IRIS_025_T                   	iris_025;                     //0x4264
	H13Bx_IRIS_026_T                   	iris_026;                     //0x4268
	H13Bx_IRIS_027_T                   	iris_027;                     //0x426c
	H13Bx_IRIS_028_T                   	iris_028;                     //0x4270
	H13Bx_IRIS_029_T                   	iris_029;                     //0x4274
	H13Bx_IRIS_030_T                   	iris_030;                     //0x4278
	H13Bx_IRIS_031_T                   	iris_031;                     //0x427c
	H13Bx_IRIS_032_T                   	iris_032;                     //0x4280
	H13Bx_IRIS_033_T                   	iris_033;                     //0x4284
	H13Bx_IRIS_034_T                   	iris_034;                     //0x4288
	H13Bx_IRIS_035_T                   	iris_035;                     //0x428c
	H13Bx_IRIS_036_T                   	iris_036;                     //0x4290
	H13Bx_IRIS_037_T                   	iris_037;                     //0x4294
	H13Bx_IRIS_038_T                   	iris_038;                     //0x4298
	H13Bx_IRIS_039_T                   	iris_039;                     //0x429c
	H13Bx_IRIS_040_T                   	iris_040;                     //0x42a0
	H13Bx_IRIS_041_T                   	iris_041;                     //0x42a4
	H13Bx_IRIS_042_T                   	iris_042;                     //0x42a8
	H13Bx_IRIS_043_T                   	iris_043;                     //0x42ac
	H13Bx_IRIS_044_T                   	iris_044;                     //0x42b0
	H13Bx_IRIS_045_T                   	iris_045;                     //0x42b4
	H13Bx_IRIS_046_T                   	iris_046;                     //0x42b8
	H13Bx_IRIS_047_T                   	iris_047;                     //0x42bc
	H13Bx_IRIS_048_T                   	iris_048;                     //0x42c0
	H13Bx_IRIS_049_T                   	iris_049;                     //0x42c4
	H13Bx_IRIS_050_T                   	iris_050;                     //0x42c8
	H13Bx_IRIS_051_T                   	iris_051;                     //0x42cc
	H13Bx_IRIS_052_T                   	iris_052;                     //0x42d0
	H13Bx_IRIS_053_T                   	iris_053;                     //0x42d4
	H13Bx_IRIS_054_T                   	iris_054;                     //0x42d8
	H13Bx_IRIS_055_T                   	iris_055;                     //0x42dc
	H13Bx_IRIS_056_T                   	iris_056;                     //0x42e0
	H13Bx_IRIS_057_T                   	iris_057;                     //0x42e4
	H13Bx_IRIS_058_T                   	iris_058;                     //0x42e8
	H13Bx_IRIS_059_T                   	iris_059;                     //0x42ec
	H13Bx_IRIS_060_T                   	iris_060;                     //0x42f0
	H13Bx_IRIS_061_T                   	iris_061;                     //0x42f4
	H13Bx_IRIS_062_T                   	iris_062;                     //0x42f8
	H13Bx_IRIS_063_T                   	iris_063;                     //0x42fc
	H13Bx_IRIS_064_T                   	iris_064;                     //0x4300
	H13Bx_IRIS_065_T                   	iris_065;                     //0x4304
	H13Bx_IRIS_066_T                   	iris_066;                     //0x4308
	H13Bx_IRIS_067_T                   	iris_067;                     //0x430c
	H13Bx_IRIS_068_T                   	iris_068;                     //0x4310
	H13Bx_IRIS_069_T                   	iris_069;                     //0x4314
	H13Bx_IRIS_070_T                   	iris_070;                     //0x4318
	H13Bx_IRIS_071_T                   	iris_071;                     //0x431c
	H13Bx_IRIS_072_T                   	iris_072;                     //0x4320
	H13Bx_IRIS_073_T                   	iris_073;                     //0x4324
	H13Bx_IRIS_074_T                   	iris_074;                     //0x4328
	H13Bx_IRIS_075_T                   	iris_075;                     //0x432c
	H13Bx_IRIS_076_T                   	iris_076;                     //0x4330
	H13Bx_IRIS_077_T                   	iris_077;                     //0x4334
	H13Bx_IRIS_078_T                   	iris_078;                     //0x4338
	H13Bx_IRIS_079_T                   	iris_079;                     //0x433c
	H13Bx_IRIS_080_T                   	iris_080;                     //0x4340
	H13Bx_IRIS_081_T                   	iris_081;                     //0x4344
	H13Bx_IRIS_082_T                   	iris_082;                     //0x4348
	H13Bx_IRIS_083_T                   	iris_083;                     //0x434c
	H13Bx_IRIS_084_T                   	iris_084;                     //0x4350
	H13Bx_IRIS_085_T                   	iris_085;                     //0x4354
	H13Bx_IRIS_086_T                   	iris_086;                     //0x4358
	H13Bx_IRIS_087_T                   	iris_087;                     //0x435c
	H13Bx_IRIS_088_T                   	iris_088;                     //0x4360
	H13Bx_IRIS_089_T                   	iris_089;                     //0x4364
	H13Bx_IRIS_090_T                   	iris_090;                     //0x4368
	H13Bx_IRIS_091_T                   	iris_091;                     //0x436c
	H13Bx_IRIS_092_T                   	iris_092;                     //0x4370
	H13Bx_IRIS_093_T                   	iris_093;                     //0x4374
	H13Bx_IRIS_094_T                   	iris_094;                     //0x4378
	H13Bx_IRIS_095_T                   	iris_095;                     //0x437c
	H13Bx_IRIS_096_T                   	iris_096;                     //0x4380
	H13Bx_IRIS_097_T                   	iris_097;                     //0x4384
	H13Bx_IRIS_098_T                   	iris_098;                     //0x4388
	H13Bx_IRIS_099_T                   	iris_099;                     //0x438c
	H13Bx_IRIS_100_T                   	iris_100;                     //0x4390
	H13Bx_IRIS_101_T                   	iris_101;                     //0x4394
	H13Bx_IRIS_102_T                   	iris_102;                     //0x4398
	H13Bx_IRIS_103_T                   	iris_103;                     //0x439c
	H13Bx_IRIS_104_T                   	iris_104;                     //0x43a0
	H13Bx_IRIS_105_T                   	iris_105;                     //0x43a4
	H13Bx_IRIS_106_T                   	iris_106;                     //0x43a8
	H13Bx_IRIS_107_T                   	iris_107;                     //0x43ac
	H13Bx_IRIS_108_T                   	iris_108;                     //0x43b0
	H13Bx_IRIS_109_T                   	iris_109;                     //0x43b4
	H13Bx_IRIS_110_T                   	iris_110;                     //0x43b8
	H13Bx_IRIS_111_T                   	iris_111;                     //0x43bc
	H13Bx_IRIS_112_T                   	iris_112;                     //0x43c0
	H13Bx_IRIS_113_T                   	iris_113;                     //0x43c4
	H13Bx_IRIS_114_T                   	iris_114;                     //0x43c8
	H13Bx_IRIS_115_T                   	iris_115;                     //0x43cc
	H13Bx_IRIS_116_T                   	iris_116;                     //0x43d0
	H13Bx_IRIS_117_T                   	iris_117;                     //0x43d4
	H13Bx_IRIS_118_T                   	iris_118;                     //0x43d8
	H13Bx_IRIS_119_T                   	iris_119;                     //0x43dc
	H13Bx_IRIS_120_T                   	iris_120;                     //0x43e0
	H13Bx_IRIS_121_T                   	iris_121;                     //0x43e4
	H13Bx_IRIS_122_T                   	iris_122;                     //0x43e8
	H13Bx_IRIS_123_T                   	iris_123;                     //0x43ec
	H13Bx_IRIS_124_T                   	iris_124;                     //0x43f0
	H13Bx_IRIS_125_T                   	iris_125;                     //0x43f4
	H13Bx_IRIS_126_T                   	iris_126;                     //0x43f8
	H13Bx_IRIS_127_T                   	iris_127;                     //0x43fc
	H13Bx_IRIS_128_T                   	iris_128;                     //0x4400
	H13Bx_IRIS_129_T                   	iris_129;                     //0x4404
	H13Bx_IRIS_130_T                   	iris_130;                     //0x4408
	H13Bx_IRIS_131_T                   	iris_131;                     //0x440c
	H13Bx_IRIS_132_T                   	iris_132;                     //0x4410
	H13Bx_IRIS_133_T                   	iris_133;                     //0x4414
	H13Bx_IRIS_134_T                   	iris_134;                     //0x4418
	H13Bx_IRIS_135_T                   	iris_135;                     //0x441c
	H13Bx_IRIS_136_T                   	iris_136;                     //0x4420
	H13Bx_IRIS_137_T                   	iris_137;                     //0x4424
	H13Bx_IRIS_138_T                   	iris_138;                     //0x4428
	H13Bx_IRIS_139_T                   	iris_139;                     //0x442c
	H13Bx_IRIS_140_T                   	iris_140;                     //0x4430
	H13Bx_IRIS_141_T                   	iris_141;                     //0x4434
	H13Bx_IRIS_142_T                   	iris_142;                     //0x4438
	H13Bx_IRIS_143_T                   	iris_143;                     //0x443c
	H13Bx_IRIS_144_T                   	iris_144;                     //0x4440
	H13Bx_IRIS_145_T                   	iris_145;                     //0x4444
	H13Bx_IRIS_146_T                   	iris_146;                     //0x4448
	H13Bx_IRIS_147_T                   	iris_147;                     //0x444c
	H13Bx_IRIS_148_T                   	iris_148;                     //0x4450
	H13Bx_IRIS_149_T                   	iris_149;                     //0x4454
	H13Bx_IRIS_150_T                   	iris_150;                     //0x4458
	H13Bx_IRIS_151_T                   	iris_151;                     //0x445c
	H13Bx_IRIS_152_T                   	iris_152;                     //0x4460
	H13Bx_IRIS_153_T                   	iris_153;                     //0x4464
	H13Bx_IRIS_154_T                   	iris_154;                     //0x4468
	H13Bx_IRIS_155_T                   	iris_155;                     //0x446c
	H13Bx_IRIS_156_T                   	iris_156;                     //0x4470
	H13Bx_IRIS_157_T                   	iris_157;                     //0x4474
	H13Bx_IRIS_158_T                   	iris_158;                     //0x4478
	H13Bx_IRIS_159_T                   	iris_159;                     //0x447c
	H13Bx_IRIS_160_T                   	iris_160;                     //0x4480
	H13Bx_IRIS_161_T                   	iris_161;                     //0x4484
	H13Bx_IRIS_162_T                   	iris_162;                     //0x4488
	H13Bx_IRIS_163_T                   	iris_163;                     //0x448c
	H13Bx_IRIS_164_T                   	iris_164;                     //0x4490
	H13Bx_IRIS_165_T                   	iris_165;                     //0x4494
	H13Bx_IRIS_166_T                   	iris_166;                     //0x4498
	H13Bx_IRIS_167_T                   	iris_167;                     //0x449c
	H13Bx_IRIS_168_T                   	iris_168;                     //0x44a0
	H13Bx_IRIS_169_T                   	iris_169;                     //0x44a4
	H13Bx_IRIS_170_T                   	iris_170;                     //0x44a8
	H13Bx_IRIS_171_T                   	iris_171;                     //0x44ac
	H13Bx_IRIS_172_T                   	iris_172;                     //0x44b0
	H13Bx_IRIS_173_T                   	iris_173;                     //0x44b4
	H13Bx_IRIS_174_T                   	iris_174;                     //0x44b8
	H13Bx_IRIS_175_T                   	iris_175;                     //0x44bc
	UINT32                             	reserved04[2];                //0x44c0~0x44c4
	H13Bx_IRIS_178_T                   	iris_178;                     //0x44c8
	H13Bx_IRIS_179_T                   	iris_179;                     //0x44cc
	H13Bx_IRIS_180_T                   	iris_180;                     //0x44d0
	H13Bx_IRIS_181_T                   	iris_181;                     //0x44d4
	H13Bx_IRIS_182_T                   	iris_182;                     //0x44d8
	H13Bx_IRIS_183_T                   	iris_183;                     //0x44dc
	H13Bx_IRIS_184_T                   	iris_184;                     //0x44e0
	H13Bx_IRIS_185_T                   	iris_185;                     //0x44e4
	H13Bx_IRIS_186_T                   	iris_186;                     //0x44e8
	H13Bx_IRIS_187_T                   	iris_187;                     //0x44ec
	H13Bx_IRIS_188_T                   	iris_188;                     //0x44f0
	H13Bx_IRIS_189_T                   	iris_189;                     //0x44f4
	H13Bx_IRIS_190_T                   	iris_190;                     //0x44f8
	H13Bx_IRIS_191_T                   	iris_191;                     //0x44fc
	H13Bx_IRIS_192_T                   	iris_192;                     //0x4500
	H13Bx_IRIS_193_T                   	iris_193;                     //0x4504
	H13Bx_IRIS_194_T                   	iris_194;                     //0x4508
	H13Bx_IRIS_195_T                   	iris_195;                     //0x450c
	H13Bx_IRIS_196_T                   	iris_196;                     //0x4510
	H13Bx_IRIS_197_T                   	iris_197;                     //0x4514
	H13Bx_IRIS_198_T                   	iris_198;                     //0x4518
	H13Bx_IRIS_199_T                   	iris_199;                     //0x451c
	H13Bx_IRIS_200_T                   	iris_200;                     //0x4520
	H13Bx_IRIS_201_T                   	iris_201;                     //0x4524
	H13Bx_IRIS_202_T                   	iris_202;                     //0x4528
	H13Bx_IRIS_203_T                   	iris_203;                     //0x452c
	H13Bx_IRIS_204_T                   	iris_204;                     //0x4530
	H13Bx_IRIS_205_T                   	iris_205;                     //0x4534
	H13Bx_IRIS_206_T                   	iris_206;                     //0x4538
	H13Bx_IRIS_207_T                   	iris_207;                     //0x453c
	H13Bx_IRIS_208_T                   	iris_208;                     //0x4540
	H13Bx_IRIS_209_T                   	iris_209;                     //0x4544
	H13Bx_IRIS_210_T                   	iris_210;                     //0x4548
	H13Bx_IRIS_211_T                   	iris_211;                     //0x454c
	H13Bx_IRIS_212_T                   	iris_212;                     //0x4550
	H13Bx_IRIS_213_T                   	iris_213;                     //0x4554
	H13Bx_IRIS_214_T                   	iris_214;                     //0x4558
	H13Bx_IRIS_215_T                   	iris_215;                     //0x455c
	H13Bx_IRIS_216_T                   	iris_216;                     //0x4560
	H13Bx_IRIS_217_T                   	iris_217;                     //0x4564
	H13Bx_IRIS_218_T                   	iris_218;                     //0x4568
	H13Bx_IRIS_219_T                   	iris_219;                     //0x456c
	H13Bx_IRIS_220_T                   	iris_220;                     //0x4570
	H13Bx_IRIS_221_T                   	iris_221;                     //0x4574
	H13Bx_IRIS_222_T                   	iris_222;                     //0x4578
	H13Bx_IRIS_223_T                   	iris_223;                     //0x457c
	H13Bx_IRIS_224_T                   	iris_224;                     //0x4580
	H13Bx_IRIS_225_T                   	iris_225;                     //0x4584
	H13Bx_IRIS_226_T                   	iris_226;                     //0x4588
	H13Bx_IRIS_227_T                   	iris_227;                     //0x458c
	H13Bx_IRIS_228_T                   	iris_228;                     //0x4590
	H13Bx_IRIS_229_T                   	iris_229;                     //0x4594
	H13Bx_IRIS_230_T                   	iris_230;                     //0x4598
	H13Bx_IRIS_231_T                   	iris_231;                     //0x459c
	H13Bx_IRIS_232_T                   	iris_232;                     //0x45a0
	H13Bx_IRIS_233_T                   	iris_233;                     //0x45a4
	H13Bx_IRIS_234_T                   	iris_234;                     //0x45a8
	H13Bx_IRIS_235_T                   	iris_235;                     //0x45ac
	H13Bx_IRIS_236_T                   	iris_236;                     //0x45b0
	H13Bx_IRIS_237_T                   	iris_237;                     //0x45b4
	H13Bx_IRIS_238_T                   	iris_238;                     //0x45b8
	H13Bx_IRIS_239_T                   	iris_239;                     //0x45bc
	H13Bx_IRIS_240_T                   	iris_240;                     //0x45c0
	H13Bx_IRIS_241_T                   	iris_241;                     //0x45c4
	H13Bx_IRIS_242_T                   	iris_242;                     //0x45c8
	H13Bx_IRIS_243_T                   	iris_243;                     //0x45cc
	H13Bx_IRIS_244_T                   	iris_244;                     //0x45d0
	H13Bx_IRIS_245_T                   	iris_245;                     //0x45d4
	H13Bx_IRIS_246_T                   	iris_246;                     //0x45d8
	H13Bx_IRIS_247_T                   	iris_247;                     //0x45dc
	H13Bx_IRIS_248_T                   	iris_248;                     //0x45e0
	H13Bx_IRIS_249_T                   	iris_249;                     //0x45e4
	H13Bx_IRIS_250_T                   	iris_250;                     //0x45e8
	H13Bx_IRIS_251_T                   	iris_251;                     //0x45ec
	H13Bx_IRIS_252_T                   	iris_252;                     //0x45f0
	H13Bx_IRIS_253_T                   	iris_253;                     //0x45f4
	H13Bx_IRIS_254_T                   	iris_254;                     //0x45f8
	H13Bx_IRIS_255_T                   	iris_255;                     //0x45fc
	H13Bx_IRIS_256_T                   	iris_256;                     //0x4600
	H13Bx_IRIS_257_T                   	iris_257;                     //0x4604
	H13Bx_IRIS_258_T                   	iris_258;                     //0x4608
	H13Bx_IRIS_259_T                   	iris_259;                     //0x460c
	H13Bx_IRIS_260_T                   	iris_260;                     //0x4610
	H13Bx_IRIS_261_T                   	iris_261;                     //0x4614
	H13Bx_IRIS_262_T                   	iris_262;                     //0x4618
	H13Bx_IRIS_263_T                   	iris_263;                     //0x461c
	H13Bx_IRIS_264_T                   	iris_264;                     //0x4620
	H13Bx_IRIS_265_T                   	iris_265;                     //0x4624
	UINT32                             	reserved05[2];                //0x4628~0x462c
	H13Bx_IRIS_266_T                   	iris_266;                     //0x4630
	H13Bx_IRIS_267_T                   	iris_267;                     //0x4634
	H13Bx_IRIS_268_T                   	iris_268;                     //0x4638
	H13Bx_IRIS_269_T                   	iris_269;                     //0x463c
	H13Bx_IRIS_270_T                   	iris_270;                     //0x4640
	H13Bx_IRIS_271_T                   	iris_271;                     //0x4644
	H13Bx_IRIS_272_T                   	iris_272;                     //0x4648
	H13Bx_IRIS_273_T                   	iris_273;                     //0x464c
	H13Bx_IRIS_274_T                   	iris_274;                     //0x4650
	H13Bx_IRIS_275_T                   	iris_275;                     //0x4654
	H13Bx_IRIS_276_T                   	iris_276;                     //0x4658
	H13Bx_IRIS_277_T                   	iris_277;                     //0x465c
	H13Bx_IRIS_278_T                   	iris_278;                     //0x4660
	H13Bx_IRIS_279_T                   	iris_279;                     //0x4664
	H13Bx_IRIS_280_T                   	iris_280;                     //0x4668
	H13Bx_IRIS_281_T                   	iris_281;                     //0x466c
	H13Bx_IRIS_282_T                   	iris_282;                     //0x4670
	H13Bx_IRIS_283_T                   	iris_283;                     //0x4674
	H13Bx_IRIS_284_T                   	iris_284;                     //0x4678
	H13Bx_IRIS_285_T                   	iris_285;                     //0x467c
	H13Bx_IRIS_286_T                   	iris_286;                     //0x4680
	H13Bx_IRIS_287_T                   	iris_287;                     //0x4684
	H13Bx_IRIS_288_T                   	iris_288;                     //0x4688
	H13Bx_IRIS_289_T                   	iris_289;                     //0x468c
	H13Bx_IRIS_290_T                   	iris_290;                     //0x4690
	H13Bx_IRIS_291_T                   	iris_291;                     //0x4694
	H13Bx_IRIS_292_T                   	iris_292;                     //0x4698
	H13Bx_IRIS_293_T                   	iris_293;                     //0x469c
	H13Bx_IRIS_294_T                   	iris_294;                     //0x46a0
	H13Bx_IRIS_295_T                   	iris_295;                     //0x46a4
	H13Bx_IRIS_296_T                   	iris_296;                     //0x46a8
	H13Bx_IRIS_297_T                   	iris_297;                     //0x46ac
	H13Bx_IRIS_298_T                   	iris_298;                     //0x46b0
	H13Bx_IRIS_299_T                   	iris_299;                     //0x46b4
	H13Bx_IRIS_300_T                   	iris_300;                     //0x46b8
	H13Bx_IRIS_301_T                   	iris_301;                     //0x46bc
	H13Bx_IRIS_302_T                   	iris_302;                     //0x46c0
	H13Bx_IRIS_303_T                   	iris_303;                     //0x46c4
	H13Bx_IRIS_304_T                   	iris_304;                     //0x46c8
	H13Bx_IRIS_305_T                   	iris_305;                     //0x46cc
	H13Bx_IRIS_306_T                   	iris_306;                     //0x46d0
	H13Bx_IRIS_307_T                   	iris_307;                     //0x46d4
	H13Bx_IRIS_308_T                   	iris_308;                     //0x46d8
	H13Bx_IRIS_309_T                   	iris_309;                     //0x46dc
	H13Bx_IRIS_310_T                   	iris_310;                     //0x46e0
	H13Bx_IRIS_311_T                   	iris_311;                     //0x46e4
	H13Bx_IRIS_312_T                   	iris_312;                     //0x46e8
	H13Bx_IRIS_313_T                   	iris_313;                     //0x46ec
	H13Bx_IRIS_314_T                   	iris_314;                     //0x46f0
	H13Bx_IRIS_315_T                   	iris_315;                     //0x46f4
	H13Bx_IRIS_316_T                   	iris_316;                     //0x46f8
	H13Bx_IRIS_317_T                   	iris_317;                     //0x46fc
	H13Bx_IRIS_318_T                   	iris_318;                     //0x4700
	H13Bx_IRIS_319_T                   	iris_319;                     //0x4704
	H13Bx_IRIS_320_T                   	iris_320;                     //0x4708
	H13Bx_IRIS_321_T                   	iris_321;                     //0x470c
	H13Bx_IRIS_322_T                   	iris_322;                     //0x4710
	H13Bx_IRIS_323_T                   	iris_323;                     //0x4714
	H13Bx_IRIS_324_T                   	iris_324;                     //0x4718
	H13Bx_IRIS_325_T                   	iris_325;                     //0x471c
	H13Bx_IRIS_326_T                   	iris_326;                     //0x4720
	H13Bx_IRIS_327_T                   	iris_327;                     //0x4724
	H13Bx_IRIS_328_T                   	iris_328;                     //0x4728
	H13Bx_IRIS_329_T                   	iris_329;                     //0x472c
	H13Bx_IRIS_330_T                   	iris_330;                     //0x4730
	H13Bx_IRIS_331_T                   	iris_331;                     //0x4734
	H13Bx_IRIS_332_T                   	iris_332;                     //0x4738
	H13Bx_IRIS_333_T                   	iris_333;                     //0x473c
	H13Bx_IRIS_334_T                   	iris_334;                     //0x4740
	H13Bx_IRIS_335_T                   	iris_335;                     //0x4744
	H13Bx_IRIS_336_T                   	iris_336;                     //0x4748
	H13Bx_IRIS_337_T                   	iris_337;                     //0x474c
	H13Bx_IRIS_338_T                   	iris_338;                     //0x4750
	H13Bx_IRIS_339_T                   	iris_339;                     //0x4754
	H13Bx_IRIS_340_T                   	iris_340;                     //0x4758
	H13Bx_IRIS_341_T                   	iris_341;                     //0x475c
	H13Bx_IRIS_342_T                   	iris_342;                     //0x4760
	H13Bx_IRIS_343_T                   	iris_343;                     //0x4764
	H13Bx_IRIS_344_T                   	iris_344;                     //0x4768
	H13Bx_IRIS_345_T                   	iris_345;                     //0x476c
	H13Bx_IRIS_346_T                   	iris_346;                     //0x4770
	H13Bx_IRIS_347_T                   	iris_347;                     //0x4774
	H13Bx_IRIS_348_T                   	iris_348;                     //0x4778
	H13Bx_IRIS_349_T                   	iris_349;                     //0x477c
	H13Bx_IRIS_350_T                   	iris_350;                     //0x4780
	H13Bx_IRIS_351_T                   	iris_351;                     //0x4784
	H13Bx_IRIS_352_T                   	iris_352;                     //0x4788
	H13Bx_IRIS_353_T                   	iris_353;                     //0x478c
	H13Bx_IRIS_354_T                   	iris_354;                     //0x4790
	H13Bx_IRIS_355_T                   	iris_355;                     //0x4794
	H13Bx_IRIS_356_T                   	iris_356;                     //0x4798
	H13Bx_IRIS_357_T                   	iris_357;                     //0x479c
	H13Bx_IRIS_358_T                   	iris_358;                     //0x47a0
	H13Bx_IRIS_359_T                   	iris_359;                     //0x47a4
	H13Bx_IRIS_360_T                   	iris_360;                     //0x47a8
	H13Bx_IRIS_361_T                   	iris_361;                     //0x47ac
	H13Bx_IRIS_362_T                   	iris_362;                     //0x47b0
	H13Bx_IRIS_363_T                   	iris_363;                     //0x47b4
	H13Bx_IRIS_364_T                   	iris_364;                     //0x47b8
	H13Bx_IRIS_365_T                   	iris_365;                     //0x47bc
	H13Bx_IRIS_366_T                   	iris_366;                     //0x47c0
	H13Bx_IRIS_367_T                   	iris_367;                     //0x47c4
	H13Bx_IRIS_368_T                   	iris_368;                     //0x47c8
	H13Bx_IRIS_369_T                   	iris_369;                     //0x47cc
	H13Bx_IRIS_370_T                   	iris_370;                     //0x47d0
	UINT32                             	reserved06[11];               //0x47d4~0x47fc
	H13Bx_VBI_001_T                    	vbi_001;                      //0x4800
	H13Bx_VBI_002_T                    	vbi_002;                      //0x4804
	H13Bx_VBI_003_T                    	vbi_003;                      //0x4808
	H13Bx_VBI_004_T                    	vbi_004;                      //0x480c
	H13Bx_VBI_005_T                    	vbi_005;                      //0x4810
	H13Bx_VBI_006_T                    	vbi_006;                      //0x4814
	H13Bx_VBI_007_T                    	vbi_007;                      //0x4818
	H13Bx_VBI_008_T                    	vbi_008;                      //0x481c
	H13Bx_VBI_009_T                    	vbi_009;                      //0x4820
	H13Bx_VBI_010_T                    	vbi_010;                      //0x4824
	H13Bx_VBI_011_T                    	vbi_011;                      //0x4828
	H13Bx_VBI_012_T                    	vbi_012;                      //0x482c
	H13Bx_VBI_013_T                    	vbi_013;                      //0x4830
	H13Bx_VBI_014_T                    	vbi_014;                      //0x4834
	H13Bx_VBI_015_T                    	vbi_015;                      //0x4838
	H13Bx_VBI_016_T                    	vbi_016;                      //0x483c
	H13Bx_VBI_017_T                    	vbi_017;                      //0x4840
	H13Bx_VBI_018_T                    	vbi_018;                      //0x4844
	H13Bx_VBI_019_T                    	vbi_019;                      //0x4848
	H13Bx_VBI_020_T                    	vbi_020;                      //0x484c
	H13Bx_VBI_021_T                    	vbi_021;                      //0x4850
	H13Bx_VBI_022_T                    	vbi_022;                      //0x4854
	H13Bx_VBI_023_T                    	vbi_023;                      //0x4858
	H13Bx_VBI_024_T                    	vbi_024;                      //0x485c
	H13Bx_VBI_025_T                    	vbi_025;                      //0x4860
	H13Bx_VBI_026_T                    	vbi_026;                      //0x4864
	H13Bx_VBI_027_T                    	vbi_027;                      //0x4868
	H13Bx_VBI_028_T                    	vbi_028;                      //0x486c
	H13Bx_VBI_029_T                    	vbi_029;                      //0x4870
	H13Bx_VBI_030_T                    	vbi_030;                      //0x4874
	H13Bx_VBI_031_T                    	vbi_031;                      //0x4878
	H13Bx_VBI_032_T                    	vbi_032;                      //0x487c
	H13Bx_VBI_033_T                    	vbi_033;                      //0x4880
	H13Bx_VBI_034_T                    	vbi_034;                      //0x4884
	H13Bx_VBI_035_T                    	vbi_035;                      //0x4888
	H13Bx_VBI_036_T                    	vbi_036;                      //0x488c
	H13Bx_VBI_037_T                    	vbi_037;                      //0x4890
	H13Bx_VBI_038_T                    	vbi_038;                      //0x4894
	H13Bx_AGC_001_T                    	agc_001;                      //0x4898
	H13Bx_LVL_001_T                    	lvl_001;                      //0x489c
	H13Bx_LVL_002_T                    	lvl_002;                      //0x48a0
	H13Bx_LVL_003_T                    	lvl_003;                      //0x48a4
	H13Bx_LVL_004_T                    	lvl_004;                      //0x48a8
	H13Bx_LVL_005_T                    	lvl_005;                      //0x48ac
	H13Bx_LVL_006_T                    	lvl_006;                      //0x48b0
	H13Bx_LVL_007_T                    	lvl_007;                      //0x48b4
	H13Bx_LVL_008_T                    	lvl_008;                      //0x48b8
	H13Bx_LVL_009_T                    	lvl_009;                      //0x48bc
	H13Bx_LVL_010_T                    	lvl_010;                      //0x48c0
	H13Bx_LVL_011_T                    	lvl_011;                      //0x48c4
	H13Bx_LVL_012_T                    	lvl_012;                      //0x48c8
	H13Bx_LVL_013_T                    	lvl_013;                      //0x48cc
	H13Bx_LVL_014_T                    	lvl_014;                      //0x48d0
	H13Bx_LVL_015_T                    	lvl_015;                      //0x48d4
	H13Bx_LVL_016_T                    	lvl_016;                      //0x48d8
	H13Bx_LVL_017_T                    	lvl_017;                      //0x48dc
	H13Bx_LVL_018_T                    	lvl_018;                      //0x48e0
	H13Bx_LVL_019_T                    	lvl_019;                      //0x48e4
	H13Bx_LVL_020_T                    	lvl_020;                      //0x48e8
	H13Bx_LVL_021_T                    	lvl_021;                      //0x48ec
	H13Bx_IRIS_MIF_GMUA_MON_001_T      	iris_mif_gmua_mon_001;        //0x48f0
	H13Bx_IRIS_MIF_GMUA_MON_002_T      	iris_mif_gmua_mon_002;        //0x48f4
	H13Bx_IRIS_MIF_GMUA_MON_003_T      	iris_mif_gmua_mon_003;        //0x48f8
	H13Bx_IRIS_MIF_GMUA_MON_004_T      	iris_mif_gmua_mon_004;        //0x48fc
	H13Bx_IRIS_MIF_GMUA_MON_005_T      	iris_mif_gmua_mon_005;        //0x4900
	H13Bx_IRIS_MIF_GMUA_MON_006_T      	iris_mif_gmua_mon_006;        //0x4904
	H13Bx_IRIS_MIF_GMUA_MON_007_T      	iris_mif_gmua_mon_007;        //0x4908
	H13Bx_IRIS_HIF_CTRL0_001_T         	iris_hif_ctrl0_001;           //0x490c
	H13Bx_IRIS_HIF_CTRL1_001_T         	iris_hif_ctrl1_001;           //0x4910
}DE_CVD_REG_H13Bx_T;

/*
 * @{
 * Naming for register pointer.
 * gpRealRegCVD_H13Bx : real register of VPORT_L9B0.
 * gpRegCVD_H13Bx     : shadow register.
 *
 * @def CVD_H13Bx_RdFL: Read  FLushing : Shadow <- Real.
 * @def CVD_H13Bx_WrFL: Write FLushing : Shadow -> Real.
 * @def CVD_H13Bx_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def CVD_H13Bx_Wr  : Write whole register(UINT32) from Shadow register.
 * @def CVD_H13Bx_Rd01 ~ CVD_H13Bx_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def CVD_H13Bx_Wr01 ~ CVD_H13Bx_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define CVD_H13Bx_RdFL(_r)			((gpRegCVD_H13Bx->_r)=(gpRealRegCVD_H13Bx->_r))
#define CVD_H13Bx_WrFL(_r)			((gpRealRegCVD_H13Bx->_r)=(gpRegCVD_H13Bx->_r))

#define CVD_H13Bx_Rd(_r)			*((UINT32*)(&(gpRegCVD_H13Bx->_r)))
#define CVD_H13Bx_Wr(_r,_v)			((CVD_H13Bx_Rd(_r))=((UINT32)(_v)))

#define CVD_H13Bx_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
								} while(0)

#define CVD_H13Bx_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
								} while(0)

#define CVD_H13Bx_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
								} while(0)

#define CVD_H13Bx_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
								} while(0)

#define CVD_H13Bx_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
								} while(0)

#define CVD_H13Bx_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
								} while(0)

#define CVD_H13Bx_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
								} while(0)

#define CVD_H13Bx_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
								} while(0)

#define CVD_H13Bx_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
								} while(0)

#define CVD_H13Bx_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
									(_v10) = (gpRegCVD_H13Bx->_r._f10);				\
								} while(0)

#define CVD_H13Bx_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
									(_v10) = (gpRegCVD_H13Bx->_r._f10);				\
									(_v11) = (gpRegCVD_H13Bx->_r._f11);				\
								} while(0)

#define CVD_H13Bx_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
									(_v10) = (gpRegCVD_H13Bx->_r._f10);				\
									(_v11) = (gpRegCVD_H13Bx->_r._f11);				\
									(_v12) = (gpRegCVD_H13Bx->_r._f12);				\
								} while(0)

#define CVD_H13Bx_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
									(_v10) = (gpRegCVD_H13Bx->_r._f10);				\
									(_v11) = (gpRegCVD_H13Bx->_r._f11);				\
									(_v12) = (gpRegCVD_H13Bx->_r._f12);				\
									(_v13) = (gpRegCVD_H13Bx->_r._f13);				\
								} while(0)

#define CVD_H13Bx_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
									(_v10) = (gpRegCVD_H13Bx->_r._f10);				\
									(_v11) = (gpRegCVD_H13Bx->_r._f11);				\
									(_v12) = (gpRegCVD_H13Bx->_r._f12);				\
									(_v13) = (gpRegCVD_H13Bx->_r._f13);				\
									(_v14) = (gpRegCVD_H13Bx->_r._f14);				\
								} while(0)

#define CVD_H13Bx_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
									(_v10) = (gpRegCVD_H13Bx->_r._f10);				\
									(_v11) = (gpRegCVD_H13Bx->_r._f11);				\
									(_v12) = (gpRegCVD_H13Bx->_r._f12);				\
									(_v13) = (gpRegCVD_H13Bx->_r._f13);				\
									(_v14) = (gpRegCVD_H13Bx->_r._f14);				\
									(_v15) = (gpRegCVD_H13Bx->_r._f15);				\
								} while(0)

#define CVD_H13Bx_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegCVD_H13Bx->_r._f01);				\
									(_v02) = (gpRegCVD_H13Bx->_r._f02);				\
									(_v03) = (gpRegCVD_H13Bx->_r._f03);				\
									(_v04) = (gpRegCVD_H13Bx->_r._f04);				\
									(_v05) = (gpRegCVD_H13Bx->_r._f05);				\
									(_v06) = (gpRegCVD_H13Bx->_r._f06);				\
									(_v07) = (gpRegCVD_H13Bx->_r._f07);				\
									(_v08) = (gpRegCVD_H13Bx->_r._f08);				\
									(_v09) = (gpRegCVD_H13Bx->_r._f09);				\
									(_v10) = (gpRegCVD_H13Bx->_r._f10);				\
									(_v11) = (gpRegCVD_H13Bx->_r._f11);				\
									(_v12) = (gpRegCVD_H13Bx->_r._f12);				\
									(_v13) = (gpRegCVD_H13Bx->_r._f13);				\
									(_v14) = (gpRegCVD_H13Bx->_r._f14);				\
									(_v15) = (gpRegCVD_H13Bx->_r._f15);				\
									(_v16) = (gpRegCVD_H13Bx->_r._f16);				\
								} while(0)


#define CVD_H13Bx_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
								} while(0)

#define CVD_H13Bx_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
								} while(0)

#define CVD_H13Bx_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
								} while(0)

#define CVD_H13Bx_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
								} while(0)

#define CVD_H13Bx_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
								} while(0)

#define CVD_H13Bx_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
								} while(0)

#define CVD_H13Bx_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
								} while(0)

#define CVD_H13Bx_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
								} while(0)

#define CVD_H13Bx_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
								} while(0)

#define CVD_H13Bx_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
									(gpRegCVD_H13Bx->_r._f10) = (_v10);				\
								} while(0)

#define CVD_H13Bx_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
									(gpRegCVD_H13Bx->_r._f10) = (_v10);				\
									(gpRegCVD_H13Bx->_r._f11) = (_v11);				\
								} while(0)

#define CVD_H13Bx_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
									(gpRegCVD_H13Bx->_r._f10) = (_v10);				\
									(gpRegCVD_H13Bx->_r._f11) = (_v11);				\
									(gpRegCVD_H13Bx->_r._f12) = (_v12);				\
								} while(0)

#define CVD_H13Bx_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
									(gpRegCVD_H13Bx->_r._f10) = (_v10);				\
									(gpRegCVD_H13Bx->_r._f11) = (_v11);				\
									(gpRegCVD_H13Bx->_r._f12) = (_v12);				\
									(gpRegCVD_H13Bx->_r._f13) = (_v13);				\
								} while(0)

#define CVD_H13Bx_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
									(gpRegCVD_H13Bx->_r._f10) = (_v10);				\
									(gpRegCVD_H13Bx->_r._f11) = (_v11);				\
									(gpRegCVD_H13Bx->_r._f12) = (_v12);				\
									(gpRegCVD_H13Bx->_r._f13) = (_v13);				\
									(gpRegCVD_H13Bx->_r._f14) = (_v14);				\
								} while(0)

#define CVD_H13Bx_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
									(gpRegCVD_H13Bx->_r._f10) = (_v10);				\
									(gpRegCVD_H13Bx->_r._f11) = (_v11);				\
									(gpRegCVD_H13Bx->_r._f12) = (_v12);				\
									(gpRegCVD_H13Bx->_r._f13) = (_v13);				\
									(gpRegCVD_H13Bx->_r._f14) = (_v14);				\
									(gpRegCVD_H13Bx->_r._f15) = (_v15);				\
								} while(0)

#define CVD_H13Bx_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegCVD_H13Bx->_r._f01) = (_v01);				\
									(gpRegCVD_H13Bx->_r._f02) = (_v02);				\
									(gpRegCVD_H13Bx->_r._f03) = (_v03);				\
									(gpRegCVD_H13Bx->_r._f04) = (_v04);				\
									(gpRegCVD_H13Bx->_r._f05) = (_v05);				\
									(gpRegCVD_H13Bx->_r._f06) = (_v06);				\
									(gpRegCVD_H13Bx->_r._f07) = (_v07);				\
									(gpRegCVD_H13Bx->_r._f08) = (_v08);				\
									(gpRegCVD_H13Bx->_r._f09) = (_v09);				\
									(gpRegCVD_H13Bx->_r._f10) = (_v10);				\
									(gpRegCVD_H13Bx->_r._f11) = (_v11);				\
									(gpRegCVD_H13Bx->_r._f12) = (_v12);				\
									(gpRegCVD_H13Bx->_r._f13) = (_v13);				\
									(gpRegCVD_H13Bx->_r._f14) = (_v14);				\
									(gpRegCVD_H13Bx->_r._f15) = (_v15);				\
									(gpRegCVD_H13Bx->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after CVD_H13Bx_Wind(), 1 for CVD_H13Bx_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * CVD_H13Bx_Rind : General indexed register Read.(
 * CVD_H13Bx_Wind : General indexed register Read.
 *
 * CVD_H13Bx_Ridx : For 'index', 'rw', 'load' field name
 * CVD_H13Bx_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define CVD_H13Bx_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								CVD_H13Bx_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								CVD_H13Bx_WrFL(_r);									\
								CVD_H13Bx_RdFL(_r);									\
								CVD_H13Bx_Rd01(_r,_fname,_fval);						\
							} while (0)

#define CVD_H13Bx_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				CVD_H13Bx_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define CVD_H13Bx_Ridx(_r, _ival, _fname, _fval)	CVD_H13Bx_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define CVD_H13Bx_Widx(_r, _ival, _fname, _fval)	CVD_H13Bx_Wind(_r,load,rw,index,_ival,_fname,_fval)

#ifdef __cplusplus
}
#endif

#endif
