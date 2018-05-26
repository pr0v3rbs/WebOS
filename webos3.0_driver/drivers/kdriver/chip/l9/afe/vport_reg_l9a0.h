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


/** @file
 *
 *  #MOD# register details. ( used only within kdriver )
 *
 *  author     user name (user_name@lge.com)
 *  version    1.0
 *  date       2010.xx.xx
 *
 */

#ifndef _VPORT_REG_L9A0_H_
#define _VPORT_REG_L9A0_H_

/*----------------------------------------------------------------------------------------
    Control Constants
----------------------------------------------------------------------------------------*/
#if !defined(_readw) && !defined(_writew)
#define _readw(addr)		(*((volatile unsigned int *)(addr)))
#define _writew(addr, val)	(_readw(addr) = val)
#endif

#if !defined(_iow)
#define _iow(addr, size, off, val)	{	\
	unsigned int tmp = _readw(addr);	\
	unsigned int mask;					\
	switch(size)						\
	{									\
		case 8:							\
			mask = 0x000000ff;			\
		break;						\
		case 16:						\
			mask = 0x0000ffff;			\
		break;						\
		case 32:						\
			mask = 0xffffffff;			\
		break;						\
	}									\
	tmp &= ~(mask << off);				\
	tmp |= ((val & mask) << off);		\
	_writew(addr, tmp);					\
}
#endif

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif


typedef struct { // 0x4100
	UINT32	cvd_intr1_enable0               : 32;  // 31: 0
} L9A0_CVD_INTR1_ENABLE0_T;

typedef struct { // 0x4104
	UINT32	cvd_chromalock_fal              : 1,   //     0
			cvd_chromalock_ris              : 1,   //     1
			cvd_vlock_fal                   : 1,   //     2
			cvd_vlock_ris                   : 1,   //     3
			cvd_hlock_fal                   : 1,   //     4
			cvd_hlock_ris                   : 1,   //     5
			cvd_no_signal_fal               : 1,   //     6
			cvd_no_signal_ris               : 1,   //     7
			cvd_noburst_detected_fal        : 1,   //     8
			cvd_noburst_detected_ris        : 1,   //     9
			cvd_vnon_standard_fal           : 1,   //    10
			cvd_vnon_standard_ris           : 1,   //    11
			cvd_hnon_standard_fal           : 1,   //    12
			cvd_hnon_standard_ris           : 1,   //    13
			cvd_proscan_detected_fal        : 1,   //    14
			cvd_proscan_detected_ris        : 1,   //    15
			cvd_vcr_fal                     : 1,   //    16
			cvd_vcr_ris                     : 1,   //    17
			cvd_noisy_fal                   : 1,   //    18
			cvd_noisy_ris                   : 1,   //    19
			cvd_vline_625_detected_fal      : 1,   //    20
			cvd_vline_625_detected_ris      : 1,   //    21
			cvd_secam_detected_fal          : 1,   //    22
			cvd_secam_detected_ris          : 1,   //    23
			cvd_pal_detected_fal            : 1,   //    24
			cvd_pal_detected_ris            : 1,   //    25
			cvd_vs_fal                      : 1,   //    26
			cvd_vs_ris                      : 1,   //    27
			cvd_ext_locked_fal              : 1,   //    28
			cvd_ext_locked_ris              : 1,   //    29
			b_rsvd1 			            : 2;   //    30~31
} L9A0_CVD_INTR1_INTR0_T;

typedef struct { // 0x4108
	UINT32	cvd_intr1_clear0                : 32;  // 31: 0
} L9A0_CVD_INTR1_CLEAR0_T;

typedef struct { // 0x410c
	UINT32	cvd_intr1_enable1               : 32;  // 31: 0
} L9A0_CVD_INTR1_ENABLE1_T;

typedef struct { // 0x4110
	UINT32	vbi_wss_rdy                     : 1,   //     0
			vbi_cc_rdy                      : 1,   //     1
			vbi_data_in                     : 1,   //     2
			cvd_fb_rst                      : 1,   //     3
			cvd_mif_err                     : 1,   //     4
			cs_chromalock_fal               : 1,   //     5
			cs_chromalock_ris               : 1,   //     6
			cs_pal_detected_fal             : 1,   //     7
			cs_pal_detected_ris             : 1,   //     8
			cs_secam_detected_fal           : 1,   //     9
			cs_secam_detected_ris           : 1,   //    10
			cs1_chromalock_fal              : 1,   //    11
			cs1_chromalock_ris              : 1,   //    12
			cs1_pal_detected_fal            : 1,   //    13
			cs1_pal_detected_ris            : 1,   //    14
			cs1_secam_detected_fal          : 1,   //    15
			cs1_secam_detected_ris          : 1,   //    16
			estfrdp_err_flag_fal            : 1,   //    17
			estfrdp_err_flag_ris            : 1,   //    18
			b_rsvd1 			            : 13;  //    19~31
} L9A0_CVD_INTR1_INTR1_T;

typedef struct { // 0x4114
	UINT32	cvd_intr1_clear1                : 32;  // 31: 0
} L9A0_CVD_INTR1_CLEAR1_T;

typedef struct { // 0x4118
	UINT32	cvd_intr2_enable0               : 32;  // 31: 0
} L9A0_CVD_INTR2_ENABLE0_T;

typedef struct { // 0x411c
	UINT32	cvd_chromalock_fal              : 1,   //     0
			cvd_chromalock_ris              : 1,   //     1
			cvd_vlock_fal                   : 1,   //     2
			cvd_vlock_ris                   : 1,   //     3
			cvd_hlock_fal                   : 1,   //     4
			cvd_hlock_ris                   : 1,   //     5
			cvd_no_signal_fal               : 1,   //     6
			cvd_no_signal_ris               : 1,   //     7
			cvd_noburst_detected_fal        : 1,   //     8
			cvd_noburst_detected_ris        : 1,   //     9
			cvd_vnon_standard_fal           : 1,   //    10
			cvd_vnon_standard_ris           : 1,   //    11
			cvd_hnon_standard_fal           : 1,   //    12
			cvd_hnon_standard_ris           : 1,   //    13
			cvd_proscan_detected_fal        : 1,   //    14
			cvd_proscan_detected_ris        : 1,   //    15
			cvd_vcr_fal                     : 1,   //    16
			cvd_vcr_ris                     : 1,   //    17
			cvd_noisy_fal                   : 1,   //    18
			cvd_noisy_ris                   : 1,   //    19
			cvd_vline_625_detected_fal      : 1,   //    20
			cvd_vline_625_detected_ris      : 1,   //    21
			cvd_secam_detected_fal          : 1,   //    22
			cvd_secam_detected_ris          : 1,   //    23
			cvd_pal_detected_fal            : 1,   //    24
			cvd_pal_detected_ris            : 1,   //    25
			cvd_vs_fal                      : 1,   //    26
			cvd_vs_ris                      : 1,   //    27
			cvd_ext_locked_fal              : 1,   //    28
			cvd_ext_locked_ris              : 1,   //    29
			b_rsvd1 			            : 2;   //    30~31
} L9A0_CVD_INTR2_INTR0_T;

typedef struct { // 0x4120
	UINT32	cvd_intr2_clear0                : 32;  // 31: 0
} L9A0_CVD_INTR2_CLEAR0_T;

typedef struct { // 0x4124
	UINT32	cvd_intr2_enable1               : 32;  // 31: 0
} L9A0_CVD_INTR2_ENABLE1_T;

typedef struct { // 0x4128
	UINT32	vbi_wss_rdy                     : 1,   //     0
			vbi_cc_rdy                      : 1,   //     1
			vbi_data_in                     : 1,   //     2
			cvd_fb_rst                      : 1,   //     3
			cvd_mif_err                     : 1,   //     4
			cs_chromalock_fal               : 1,   //     5
			cs_chromalock_ris               : 1,   //     6
			cs_pal_detected_fal             : 1,   //     7
			cs_pal_detected_ris             : 1,   //     8
			cs_secam_detected_fal           : 1,   //     9
			cs_secam_detected_ris           : 1,   //    10
			cs1_chromalock_fal              : 1,   //    11
			cs1_chromalock_ris              : 1,   //    12
			cs1_pal_detected_fal            : 1,   //    13
			cs1_pal_detected_ris            : 1,   //    14
			cs1_secam_detected_fal          : 1,   //    15
			cs1_secam_detected_ris          : 1,   //    16
			estfrdp_err_flag_fal            : 1,   //    17
			estfrdp_err_flag_ris            : 1,   //    18
			b_rsvd1 			            : 13;   //    19~31
} L9A0_CVD_INTR2_INTR1_T;

typedef struct { // 0x412c
	UINT32	cvd_intr2_clear1                : 32;  // 31: 0
} L9A0_CVD_INTR2_CLEAR1_T;

typedef struct { // 0x4130
	UINT32	iris_ver                        : 32;  // 31: 0
} L9A0_TOP_001_T;

typedef struct { // 0x4134
	UINT32	exta_dither_blu                 : 3,   //  2: 0
			b_rsvd1 			            : 1,   //     3
			exta_dither_grn                 : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			exta_dither_red                 : 3,   // 10: 8
			b_rsvd3                         : 1,   //    11
			exta_dither_common_seed         : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			exta_dither_blu_en              : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			exta_dither_grn_en              : 1,   //    20
			b_rsvd6                         : 3,   // 23:21
			exta_dither_red_en              : 1,   //    24
			b_rsvd7                         : 7;   // 31:25
} L9A0_DITHERMUX_001_T;

typedef struct { // 0x4140
	UINT32	comb3_buffer_size               : 23,  // 22: 0
			b_rsvd1 			            : 9;  // 31:23
} L9A0_IRIS_MIF_GMUA_001_T;

typedef struct { // 0x4144
	UINT32	fld1_init_rd_pel                : 23,  // 22: 0
			b_rsvd1 			            : 9;  // 31:23
} L9A0_IRIS_MIF_GMUA_002_T;

typedef struct { // 0x4148
	UINT32	fld2_init_rd_pel                : 23,  // 22: 0
			b_rsvd1 			            : 9;  // 31:23
} L9A0_IRIS_MIF_GMUA_003_T;

typedef struct { // 0x414c
	UINT32	fld3_init_rd_pel                : 23,  // 22: 0
			b_rsvd1 			            : 9;  // 31:23
} L9A0_IRIS_MIF_GMUA_004_T;

typedef struct { // 0x4150
	UINT32	fld4_init_rd_pel                : 23,  // 22: 0
			b_rsvd1 			            : 9;  // 31:23
} L9A0_IRIS_MIF_GMUA_005_T;

typedef struct { // 0x4154
	UINT32	gmau_to_cnt                     : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			gmau_cmd_dly_cnt                : 4,   // 15:12
			b_rsvd2 			            : 16;  // 31:16
} L9A0_IRIS_MIF_GMUA_006_T;

typedef struct { // 0x4158
	UINT32 	gmau_cmd_base                   : 32;  // 31: 0
} L9A0_IRIS_MIF_GMUA_007_T;

typedef struct { // 0x4160
	UINT32	gmau_endian_sw                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			gmau_cmd_pri2                   : 4,   //  7: 4
			gmau_cmd_pri1                   : 4,   // 11: 8
			b_rsvd2 			            : 20;  // 31:12
} L9A0_IRIS_MIF_GMUA_008_T;

typedef struct { // 0x4164
		UINT32	gmau_stride_size                : 32;  // 31: 0
} L9A0_IRIS_MIF_GMUA_009_T;

typedef struct { // 0x4168
	UINT32	fb_latency                      : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			fb3ch_delay                     : 5,   // 12: 8
			b_rsvd2                         : 3,   // 15:13
			fb_blend_ratio                  : 5,   // 20:16
			b_rsvd3                         : 3,   // 23:21
			fb_2line_delay                  : 1,   //    24
			b_rsvd4                         : 3,   // 27:25
			fb_en                           : 1,   //    28
			b_rsvd5 			            : 3;   // 31:29
} L9A0_FASTBLANK_001_T;

typedef struct { // 0x416c
	UINT32	fb_csc_coef1                    : 15,  // 14: 0
			b_rsvd1                         : 1,   //    15
			fb_csc_coef0                    : 15,  // 30:16
			b_rsvd2 			            : 1;   // 31:31
} L9A0_FASTBLANK_002_T;

typedef struct { // 0x4170
	UINT32	fb_csc_coef3                    : 15,  // 14: 0
			b_rsvd1                         : 1,   //    15
			fb_csc_coef2                    : 15,  // 30:16
			b_rsvd2 			            : 1;   // 31:31
} L9A0_FASTBLANK_003_T;

typedef struct { // 0x4174
	UINT32	fb_csc_coef5                    : 15,  // 14: 0
			b_rsvd1                         : 1,   //    15
			fb_csc_coef4                    : 15,  // 30:16
			b_rsvd2 			            : 1;   // 31:31
} L9A0_FASTBLANK_004_T;

typedef struct { // 0x4178
	UINT32	fb_csc_coef7                    : 15,  // 14: 0
			b_rsvd1                         : 1,   //    15
			fb_csc_coef6                    : 15,  // 30:16
			b_rsvd2 			            : 1;   // 31:31
} L9A0_FASTBLANK_005_T;

typedef struct { // 0x417c
	UINT32	fb_csc_coef8                    : 15,  // 14: 0
			b_rsvd1                         : 17;  // 31:15
} L9A0_FASTBLANK_006_T;

typedef struct { // 0x4180
	UINT32	fb_csc_ofst1                    : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			fb_csc_ofst0                    : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_FASTBLANK_007_T;

typedef struct { // 0x4184
	UINT32	fb_csc_ofst3                    : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			fb_csc_ofst2                    : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_FASTBLANK_008_T;

typedef struct { // 0x4188
	UINT32	fb_csc_ofst5                    : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			fb_csc_ofst4                    : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_FASTBLANK_009_T;

typedef struct { // 0x4190
	UINT32	iris_hsout_width                : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			iris_static_de_start            : 12,  // 23:12
			iris_static_de                  : 1,   //    24
			b_rsvd2                         : 7;   // 31:25
} L9A0_IRIS_DE_CTRL_001_T;

typedef struct { // 0x4194
	UINT32	vbi_little_endian               : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			vbi_bits_swap                   : 1,   //     4
			b_rsvd2                         : 27;   // 31:5
} L9A0_VBI_CTRL_001_T;

typedef struct { // 0x4198
	UINT32	vbi0_start_addr                 : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			vbi0_data_cnt                   : 12,  // 23:12
			vbi_buffer_number               : 2,   // 25:24
			b_rsvd2                         : 6;   // 31:26
} L9A0_VBI_CTRL_002_T;

typedef struct { // 0x419c
	UINT32	vbi0_end_addr                   : 10,  //  9: 0
			b_rsvd1                         : 22;   // 31:10
} L9A0_VBI_CTRL_003_T;

typedef struct { // 0x41a0
	UINT32	vbi1_start_addr                 : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			vbi1_data_cnt                   : 12,  // 23:12
			b_rsvd2                         : 8;   // 31:24
} L9A0_VBI_CTRL_004_T;

typedef struct { // 0x41a4
	UINT32	vbi1_end_addr                   : 10,  //  9: 0
			b_rsvd1                         : 22;   // 31:10
} L9A0_VBI_CTRL_005_T;

typedef struct { // 0x41a8
	UINT32	vbi2_start_addr                 : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			vbi2_data_cnt                   : 12,  // 23:12
			b_rsvd2                         : 8;   // 31:24
} L9A0_VBI_CTRL_006_T;

typedef struct { // 0x41ac
	UINT32	vbi2_end_addr                   : 10,  //  9: 0
			b_rsvd1                         : 22;   // 31:10
} L9A0_VBI_CTRL_007_T;

typedef struct { // 0x41b0
	UINT32	vbi3_start_addr                 : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			vbi3_data_cnt                   : 12,  // 23:12
			b_rsvd2                         : 8;   // 31:24
} L9A0_VBI_CTRL_008_T;

typedef struct { // 0x41b4
	UINT32	vbi3_end_addr                   : 10,  //  9: 0
			b_rsvd1                         : 22;   // 31:10
} L9A0_VBI_CTRL_009_T;

typedef struct { // 0x41c0
	UINT32	fb_state                        : 1,   //     0
			b_rsvd1                         : 31;   // 31:1
} L9A0_FAST_BLANK_STATUS_001_T;

typedef struct { // 0x41c4
	UINT32	cvbs2_gc_int                    : 4,   //  3: 0
			cvbs1_gc_int                    : 4,   //  7: 4
			iris_fix_gain                   : 1,   //     8
			b_rsvd1                         : 23;  // 31:9
} L9A0_CVBSAFE_001_T;

typedef struct { // 0x41c8
	UINT32	sel_2nd_clamp                   : 1,   //     0
			b_rsvd1                         : 31;   // 31:1
} L9A0_CVBSAFE_002_T;

typedef struct { // 0x4200
	UINT32	proscan_detected                : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			mv_colourstripes                : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			mv_vbi_detected                 : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			chromalock                      : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			vlock                           : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			hlock                           : 1,   //    20
			b_rsvd6                         : 3,   // 23:21
			no_signal                       : 1,   //    24
			b_rsvd7                         : 7;   // 31:25
} L9A0_IRIS_001_T;

typedef struct { // 0x4204
	UINT32	noisy                           : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			vline_625_detected              : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			secam_detected                  : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			pal_detected                    : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			status_comb3d_off               : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			noburst_detected                : 1,   //    20
			b_rsvd6                         : 3,   // 23:21
			vnon_standard                   : 1,   //    24
			b_rsvd7                         : 3,   // 27:25
			hnon_standard                   : 1,   //    28
			b_rsvd8                         : 3;   // 31:29
} L9A0_IRIS_002_T;

typedef struct { // 0x4208
	UINT32	vcr_rew                         : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			vcr_ff                          : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			vcr_trick                       : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			reg_vcr                         : 1,   //    12
			b_rsvd4                         : 19;  // 31:13
} L9A0_IRIS_003_T;

typedef struct { // 0x420c
	UINT32	status_burst_mag                : 16,  // 15: 0
			status_hsync_width              : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_004_T;

typedef struct { // 0x4210
	UINT32	status_hdto_inc                 : 30,  // 29: 0
			b_rsvd1                         : 1;   // 31:30
} L9A0_IRIS_005_T;

typedef struct { // 0x4214
	UINT32	status_cdto_inc                 : 30,  // 29: 0
			b_rsvd1                         : 1;   // 31:30
} L9A0_IRIS_006_T;

typedef struct { // 0x4218
	UINT32	status_cmag                     : 8,   //  7: 0
			status_agc_gain                 : 16,  // 23: 8
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_007_T;

typedef struct { // 0x421c
	UINT32	status_sync_height              : 8,   //  7: 0
			status_cordic_freq              : 8,   // 15: 8
			status_cgain                    : 14,  // 29:16
			b_rsvd1                         : 2;   // 31:30
} L9A0_IRIS_008_T;

typedef struct { // 0x4220
	UINT32	status_nstd                     : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			status_vbi_slicer_lvl           : 8,   // 19:12
			status_noise                    : 8,   // 27:20
			b_rsvd2                         : 4;   // 31:28
} L9A0_IRIS_009_T;

typedef struct { // 0x4224
	UINT32	status_comb3d_motion            : 32;  // 31: 0
} L9A0_IRIS_010_T;

typedef struct { // 0x4230
	UINT32	cvd_soft_reset                  : 1,   //     0
			b_rsvd1                         : 31;   // 31:1
} L9A0_IRIS_013_T;

typedef struct { // 0x4234
	UINT32	hv_delay                        : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			hpixel                          : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			vline_625                       : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			colour_mode                     : 3,   // 14:12
			b_rsvd4                         : 1,   //    15
			yc_src                          : 1,   //    16
			b_rsvd5                         : 15;  // 31:17
} L9A0_IRIS_014_T;

typedef struct { // 0x4238
	UINT32	cv_inv                          : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cv_src                          : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			luma_notch_bw                   : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			chroma_bw_lo                    : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			chroma_burst5or10               : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			reg_ped                         : 1,   //    20
			b_rsvd6                         : 11;  // 31:21
} L9A0_IRIS_015_T;

typedef struct { // 0x423c
	UINT32	hagc_field_mode                 : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			mv_hagc_mode                    : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			dc_clamp_mode                   : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			dagc_en                         : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			agc_half_en                     : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			cagc_en                         : 1,   //    20
			b_rsvd6                         : 3,   // 23:21
			hagc_en                         : 1,   //    24
			b_rsvd7                         : 7;   // 31:25
} L9A0_IRIS_016_T;

typedef struct { // 0x4240
	UINT32	pal60_mode                      : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			ntsc443_mode                    : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			adaptive_3dcomb_mode            : 3,   // 10: 8
			b_rsvd3                         : 1,   //    11
			colour_trap                     : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			adaptive_mode                   : 3,   // 18:16
			b_rsvd5                         : 13;  // 31:19
} L9A0_IRIS_017_T;

typedef struct { // 0x4244
	UINT32	noise_thresh                    : 8,   //  7: 0
			reg_hagc                        : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_018_T;

typedef struct { // 0x4248
	UINT32	force_vcr_en                    : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			force_vcr_rew                   : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			force_vcr_ff                    : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			force_vcr_trick                 : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			force_vcr                       : 1,   //    16
			b_rsvd5                         : 15;  // 31:17
} L9A0_IRIS_019_T;

typedef struct { // 0x424c
	UINT32	adc_updn_swap                   : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			adc_input_swap                  : 1,   //     4
			b_rsvd2                         : 27;  //  31:5
} L9A0_IRIS_020_T;

typedef struct { // 0x4250
	UINT32	ccir656_en                      : 1,   //     0
 		    b_rsvd1                         : 3,   //  3: 1
			cbcr_swap                       : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			blue_mode                       : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			yc_delay                        : 4,   // 15:12
			b_rsvd4                         : 16;  // 31:16
} L9A0_IRIS_021_T;

typedef struct { // 0x4254
	UINT32	reg_hue                         : 8,   //  7: 0
			saturation                      : 8,   // 15: 8
			brightness                      : 8,   // 23:16
			contrast                        : 8;   // 31:24
} L9A0_IRIS_022_T;

typedef struct { // 0x4258
	UINT32	reg_cagc                        : 8,   //  7: 0
			b_rsvd1                         : 24;  // 31: 8
} L9A0_IRIS_023_T;

typedef struct { // 0x425c
	UINT32	user_ckill_mode                 : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			vbi_ckill                       : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			hlock_ckill                     : 1,   //     8
			b_rsvd3                         : 23;  // 31: 9
} L9A0_IRIS_024_T;

typedef struct { // 0x4260
	UINT32	nstd_hysis                      : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			disable_clamp_on_vsync          : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			bypass                          : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			noburst_ckill                   : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			vnon_std_threshold              : 2,   // 17:16
			b_rsvd5                         : 2,   // 19:18
			hnon_std_threshold              : 6,   // 25:20
			b_rsvd6                         : 6;   // 31:26
} L9A0_IRIS_025_T;

typedef struct { // 0x4264
	UINT32	agc_peak_en                     : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			agc_peak_cntl                   : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			agc_peak_nominal                : 7,   // 14: 8
			b_rsvd3                         : 17;  // 31:15
} L9A0_IRIS_026_T;

typedef struct { // 0x4268
	UINT32	blue_cr                         : 8,   //  7: 0
			blue_cb                         : 8,   // 15: 8
			blue_y                          : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_027_T;

typedef struct { // 0x426c
	UINT32	hstate_fixed                    : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			disable_hfine                   : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			hstate_unlocked                 : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			hstate_max                      : 3,   // 14:12
			b_rsvd4                         : 1,   //    15
			locked_count_noisy_max          : 4,   // 19:16
			locked_count_clean_max          : 4,   // 23:20
			hdetect_clamp_level             : 8;   // 31:24
} L9A0_IRIS_028_T;

typedef struct { // 0x4270
	UINT32	hlock_vsync_mode                : 2,   //  1: 0
			b_rsvd1                         : 30;  // 31: 2
} L9A0_IRIS_029_T;

typedef struct { // 0x4274
	UINT32	cdto_inc                        : 32;  // 31: 0
} L9A0_IRIS_030_T;

typedef struct { // 0x4278
	UINT32	hdto_inc                        : 32;  // 31: 0
} L9A0_IRIS_031_T;

typedef struct { // 0x427c
	UINT32	hsync_gate_end                  : 8,   //  7: 0
			hsync_gate_start                : 8,   // 15: 8
			hsync_phase_offset              : 8,   // 23:16
			hsync_rising                    : 8;   // 31:24
} L9A0_IRIS_032_T;

typedef struct { // 0x4280
	UINT32	hsync_similar                   : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			hsync_low                       : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			hdetect_noise_en                : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			hfine_lt_hcoarse                : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			hlpf_clamp_sel                  : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			hlpf_clamp_noisy_en             : 1,   //    20
			b_rsvd6                         : 3,   // 23:21
			hlpf_clamp_vbi_en               : 1,   //    24
			b_rsvd7                         : 3,   // 27:25
			hlpf_clamp_en                   : 1,   //    28
			b_rsvd8                         : 3;   // 31:29
} L9A0_IRIS_033_T;

typedef struct { // 0x4284
	UINT32	hsync_rising_end                : 8,   //  7: 0
			hsync_rising_auto               : 2,   //  9: 8
			b_rsvd1                         : 2,   // 11:10
			hsync_rising_start              : 6,   // 17:12
			b_rsvd2                         : 14;  // 31:18
} L9A0_IRIS_034_T;

typedef struct { // 0x4288
	UINT32	hblank_end                      : 8,   //  7: 0
			hblank_start                    : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_035_T;

typedef struct { // 0x428c
	UINT32	burst_gate_end                  : 8,   //  7: 0
			burst_gate_start                : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_036_T;

typedef struct { // 0x4290
	UINT32	hactive_width                   : 8,   //  7: 0
			hactive_start                   : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_037_T;

typedef struct { // 0x4294
	UINT32	vactive_height                  : 8,   //  7: 0
			vactive_start                   : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_038_T;

typedef struct { // 0x4298
	UINT32	vsync_h_max                     : 7,   //  6: 0
			b_rsvd1                         : 1,   //     7
			vsync_h_min                     : 7,   // 14: 8
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_039_T;

typedef struct { // 0x429c
	UINT32	vsync_agc_max                   : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			vsync_agc_min                   : 7,   // 14: 8
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_040_T;

typedef struct { // 0x42a0
	UINT32	vsync_vbi_max                   : 7,   //  6: 0
			b_rsvd1                         : 1,   //     7
			vsync_vbi_min                   : 7,   // 14: 8
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_041_T;

typedef struct { // 0x42a4
	UINT32	vlock_wide_range                : 1,   //     0
			b_rsvd1                         : 31;  // 31: 1
} L9A0_IRIS_042_T;

typedef struct { // 0x42a8
	UINT32	proscan_1field_mode             : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			vsync_cntl_noisy                : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			vsync_cntl_ff_rew               : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			vsync_cntl_trick                : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			vsync_cntl_vcr                  : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			vsync_cntl                      : 2,   // 21:20
			b_rsvd6                         : 10;  // 31:22
} L9A0_IRIS_043_T;

typedef struct { // 0x42ac
	UINT32	field_polarity                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			flip_field                      : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			veven_delayed                   : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			vodd_delayed                    : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			field_detect_mode               : 2,   // 17:16
			b_rsvd5                         : 2,   // 19:18
			vloop_tc                        : 2,   // 21:20
			b_rsvd6                         : 10;  // 31:22
} L9A0_IRIS_044_T;

typedef struct { // 0x42b0
	UINT32	dbg_syncs                       : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			muxdigital                      : 4,   //  7: 4
			muxanalogb                      : 4,   // 11: 8
			muxanaloga                      : 4,   // 15:12
			b_rsvd2                         : 16;  // 31:16
} L9A0_IRIS_045_T;

typedef struct { // 0x42b4
	UINT32	secam_ybw                       : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			peak_range                      : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			peak_gain                       : 3,   // 10: 8
			b_rsvd3                         : 1,   //    11
			peak_en                         : 1,   //    12
			b_rsvd4                         : 19;  // 31:13
} L9A0_IRIS_046_T;

typedef struct { // 0x42b8
	UINT32	auto_secam_level                : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			sv_bf                           : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			secam_pll                       : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			secam_sw_old                    : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			palsw_level                     : 2,   // 17:16
			b_rsvd5                         : 14;  // 31:18
} L9A0_IRIS_047_T;

typedef struct { // 0x42bc
	UINT32	lose_chromalock_mode            : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			lose_chromalock_count           : 4,   //  7: 4
			lose_chromalock_level           : 3,   // 10: 8
			b_rsvd2                         : 1,   //    11
			lose_chromalock_ckill           : 1,   //    12
			b_rsvd3                         : 19;  // 31:13
} L9A0_IRIS_048_T;

typedef struct { // 0x42c0
	UINT32	cstripe_detect_control          : 3,   //  2: 0
			b_rsvd1                         : 29;  // 31: 3
} L9A0_IRIS_049_T;

typedef struct { // 0x42c4
	UINT32	hfine_vcr_trick_en              : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			hfine_vcr_en                    : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			hresampler_2up                  : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cstate                          : 3,   // 14:12
			b_rsvd4                         : 1,   //    15
			fixed_cstate                    : 1,   //    16
			b_rsvd5                         : 15;  // 31:17
} L9A0_IRIS_050_T;

typedef struct { // 0x42c8
	UINT32	vbi_pulse_ampl                  : 8,   //  7: 0
			b_rsvd1                         : 24;  // 31: 8
} L9A0_IRIS_051_T;

typedef struct { // 0x42cc
	UINT32	cpump_delay                     : 8,   //  7: 0
			cpump_adjust                    : 8,   // 15: 8
			cpump_adjust_polarity           : 1,   //    16
			b_rsvd1                         : 3,   // 19:17
			cpump_adjust_delay              : 6,   // 25:20
			b_rsvd2                         : 6;   // 31:26
} L9A0_IRIS_052_T;

typedef struct { // 0x42d0
	UINT32	mv_colourstripes_sel            : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			mv_vbi_sel                      : 1,   //     4
			b_rsvd2                         : 27;  // 31: 5
} L9A0_IRIS_053_T;

typedef struct { // 0x42d4
	UINT32	cpump_kill_cr                   : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cpump_kill_cb                   : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cpump_kill_y                    : 1,   //     8
			b_rsvd3                         : 23;  // 31: 9
} L9A0_IRIS_054_T;

typedef struct { // 0x42d8
	UINT32	cpump_auto_stip_nobp            : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cpump_auto_stip_unlocked        : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cpump_auto_stip_no_signal       : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cpump_auto_stip_noisy           : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			cpump_auto_stip_vactive         : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			cpump_auto_stip_mode            : 2,   // 21:20
			b_rsvd6                         : 10;  // 31:22
} L9A0_IRIS_055_T;

typedef struct { // 0x42dc
	UINT32	cpump_accum_mode                : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cpump_fixed_syncmid             : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cpump_level_filter_gain         : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			cpump_noisy_filter_en           : 1,   //    12
			b_rsvd4                         : 19;  // 31:13
} L9A0_IRIS_056_T;

typedef struct { // 0x42e0
	UINT32	cpump_dn_max                    : 7,   //  6: 0
			b_rsvd1                         : 1,   //     7
			cpump_up_max                    : 7,   // 14: 8
			b_rsvd2                         : 1,   //    15
			cpump_vsync_blank_filter        : 1,   //    16
			b_rsvd3                         : 3,   // 19:17
			cpump_vsync_syncmid_filter      : 1,   //    20
			b_rsvd4                         : 3,   // 23:21
			cpump_vsync_mode                : 2,   // 25:24
			b_rsvd5                         : 6;   // 31:26
} L9A0_IRIS_057_T;

typedef struct { // 0x42e4
	UINT32	cpump_diff_noisy_only           : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cpump_dn_diff_max               : 7,   // 10: 4
			b_rsvd2                         : 1,   //    11
			cpump_diff_signal_only          : 1,   //    12
			b_rsvd3                         : 3,   // 15:13
			cpump_up_diff_max               : 7,   // 22:16
			b_rsvd4                         : 9;   // 31:23
} L9A0_IRIS_058_T;

typedef struct { // 0x42e8
	UINT32	cpump_pr_override               : 8,   //  7: 0
			cpump_pb_override               : 8,   // 15: 8
			cpump_y_override                : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_059_T;

typedef struct { // 0x42ec
	UINT32	db_freq                         : 12,  // 11: 0
			dr_freq                         : 12,  // 23:12
			cvbs_y_delay                    : 5,   // 28:24
			b_rsvd1                         : 3;   // 31:29
} L9A0_IRIS_060_T;

typedef struct { // 0x42f0
	UINT32	vactivity_en                    : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			vactivity_on2frame              : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			vactivity_threshold             : 6,   // 13: 8
			b_rsvd3                         : 18;  // 31:14
} L9A0_IRIS_061_T;

typedef struct { // 0x42f4
	UINT32	horiz_diff_cgain                : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			horiz_diff_ygain                : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			chroma_vdiff_gain               : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			lowfreq_vdiff_gain              : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			noise_th_en                     : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			noise_th                        : 7,   // 26:20
			b_rsvd6                         : 5;   // 31:27
} L9A0_IRIS_062_T;

typedef struct { // 0x42f8
	UINT32	vadap_burst_noise_th_gain       : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			burst_noise_th_gain             : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			c_noise_th_gain                 : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			y_noise_th_gain                 : 2,   // 13:12
			b_rsvd4                         : 18;  // 31:14
} L9A0_IRIS_063_T;

typedef struct { // 0x42fc
	UINT32	adaptive_chroma_mode            : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			dot_suppress_mode               : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			motion_mode                     : 2,   //  9: 8
			b_rsvd3                         : 22;  // 31:10
} L9A0_IRIS_064_T;

typedef struct { // 0x4300
	UINT32	pal3dcomb_vactive_offset        : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			fb_sync                         : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			fb_hold                         : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			fb_ctl                          : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			field_latency                   : 3,   // 18:16
			b_rsvd5                         : 1,   //    19
			lbadrgen_rst                    : 1,   //    20
			b_rsvd6                         : 3,   // 23:21
			comb2d_only                     : 1,   //    24
			b_rsvd7                         : 7;   // 31:25
} L9A0_IRIS_065_T;

typedef struct { // 0x4304
	UINT32	md_noise_th_en                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			md_noise_th                     : 7,   // 10: 4
			b_rsvd2                         : 21;  // 31:11
} L9A0_IRIS_066_T;

typedef struct { // 0x4308
	UINT32	schroma_peak_en                 : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			schroma_coring_en               : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			schroma_peak                    : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			pchroma_peak_en                 : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			pchroma_coring_en               : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			pchroma_peak                    : 2,   // 21:20
			b_rsvd6                         : 10;  // 31:22
} L9A0_IRIS_067_T;

typedef struct { // 0x430c
	UINT32	ldpause_threshold               : 4,   //  3: 0
			vf_nstd_en                      : 1,   //     4
			b_rsvd1                         : 3,   //  7: 5
			vcr_auto_switch_en              : 1,   //     8
			b_rsvd2                         : 23;  // 31: 9
} L9A0_IRIS_068_T;

typedef struct { // 0x4310
	UINT32	comb_coring                     : 4,   //  3: 0
			tcomb_gain                      : 3,   //  6: 4
			b_rsvd1                         : 1,   //     7
			notch_gain                      : 3,   // 10: 8
			b_rsvd2                         : 1,   //    11
			comb_gain                       : 3,   // 14:12
			b_rsvd3                         : 17;  // 31:15
} L9A0_IRIS_069_T;

typedef struct { // 0x4314
	UINT32	vactive_md_height               : 8,   //  7: 0
			vactive_md_start                : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_070_T;

typedef struct { // 0x4318
	UINT32	hsync_pulse_width               : 4,   //  3: 0
			b_rsvd1                         : 28;  // 31: 4
} L9A0_IRIS_071_T;

typedef struct { // 0x431c
	UINT32	cagc_tc_ismall                  : 3,   //  2: 0
			b_rsvd1                         : 29;  // 31: 3
} L9A0_IRIS_072_T;

typedef struct { // 0x4320
	UINT32	cagc_coring_threshold           : 4,   //  3: 0
			cagc_unity_gain                 : 1,   //     4
			b_rsvd1                         : 3,   //  7: 5
			cagc_coring                     : 3,   // 10: 8
			b_rsvd2                         : 1,   //    11
			cagc_tc_p                       : 2,   // 13:12
			b_rsvd3                         : 2,   // 15:14
			cagc_tc_ibig                    : 3,   // 18:16
			b_rsvd4                         : 13;  // 31:19
} L9A0_IRIS_073_T;

typedef struct { // 0x4324
	UINT32	aaf_on                          : 1,   //     0
			b_rsvd1                         : 31;  // 31: 1
} L9A0_IRIS_074_T;

typedef struct { // 0x4328
	UINT32	dcrestore_accum_width           : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			dcrestore_no_bad_bp             : 1,   //     8
			b_rsvd2                         : 3,   // 11: 9
			dcrestore_kill_enable           : 1,   //    12
			b_rsvd3                         : 3,   // 15:13
			dcrestore_bp_delay              : 2,   // 17:16
			b_rsvd4                         : 2,   // 19:18
			syncmid_nobp_en                 : 1,   //    20
			b_rsvd5                         : 3,   // 23:21
			syncmid_filter_en               : 1,   //    24
			b_rsvd6                         : 3,   // 27:25
			dcrestore_gain                  : 2,   // 29:28
			b_rsvd7                         : 2;   // 31:30
} L9A0_IRIS_075_T;

typedef struct { // 0x432c
	UINT32	dcrestore_kill_enable_noisy     : 1,   //     0
			b_rsvd1                         : 31;  // 31: 1
} L9A0_IRIS_076_T;

typedef struct { // 0x4330
	UINT32	dcrestore_hsync_mid             : 8,   //  7: 0
			bp_kill_thresh                  : 8,   // 15: 8
			reg_hmgc                        : 8,   // 23:16
			dcrestore_lpf_en                : 1,   //    24
			b_rsvd1                         : 7;   // 31:25
} L9A0_IRIS_077_T;

typedef struct { // 0x4334
	UINT32	auto_min_sync_height            : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			min_sync_height                 : 7,   // 10: 4
			b_rsvd2                         : 21;  // 31:11
} L9A0_IRIS_078_T;

typedef struct { // 0x4338
	UINT32	vcrtrick_proscan                : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			veven_early_delayed             : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			vodd_early_delayed              : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			vfield_hoffset_fixed            : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			vsync_no_signal_thresh          : 8,   // 23:16
			vsync_signal_thresh             : 8;   // 31:24
} L9A0_IRIS_079_T;

typedef struct { // 0x433c
	UINT32	vfield_hoffset                  : 9,   //  8: 0
			b_rsvd1                         : 3,   // 11: 9
			vactive_half_lines              : 1,   //    12
			b_rsvd2                         : 3,   // 15:13
			vdetect_noise_en                : 1,   //    16
			b_rsvd3                         : 15;  // 31:17
} L9A0_IRIS_080_T;

typedef struct { // 0x4340
	UINT32	dual_hedge_dis                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			dual_hedge_auto_width           : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			dual_fine_hedge_vbi             : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			dual_coarse_hedge_vbi           : 2,   // 13:12
			b_rsvd4                         : 18;  // 31:14
} L9A0_IRIS_081_T;

typedef struct { // 0x4344
	UINT32	mv_hsync_rising_end             : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			vcr_state2_long                 : 1,   //     8
			b_rsvd2                         : 3,   // 11: 9
			slow_hdsw                       : 1,   //    12
			b_rsvd3                         : 3,   // 15:13
			mv_hsync_rising_start           : 6,   // 21:16
			b_rsvd4                         : 2,   // 23:22
			no_hsyncs_mode                  : 2,   // 25:24
			b_rsvd5                         : 2,   // 27:26
			many_hsyncs_mode                : 1,   //    28
			b_rsvd6                         : 3;   // 31:29
} L9A0_IRIS_082_T;

typedef struct { // 0x4348
	UINT32	no_hsyncs_weak                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			disable_hdsw_weak               : 1,   //     4
			b_rsvd2                         : 27;  // 31: 5
} L9A0_IRIS_083_T;

typedef struct { // 0x434c
	UINT32	vactive_hdsw_mode               : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			disable_hdsw_mode               : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			hsync_falling_filter            : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			no_hsyncs_noisy                 : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			hloop_range                     : 2,   // 17:16
			b_rsvd5                         : 14;  // 31:18
} L9A0_IRIS_084_T;

typedef struct { // 0x4350
	UINT32	md_hf_max                       : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			md_hf_shift                     : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			md_lf_sad                       : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			md_lf_shift                     : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			md_c_noise_th_en                : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			md_c_noise_th                   : 7,   // 26:20
			b_rsvd6                         : 5;   // 31:27
} L9A0_IRIS_085_T;

typedef struct { // 0x4354
	UINT32	md_cf_cactivity_high            : 4,   //  3: 0
			md_cf_lactivity_high            : 8,   // 11: 4
			md_cf_cactivity_low             : 8,   // 19:12
			md_cf_lactivity_low             : 8,   // 27:20
			md_cf_activity_enable           : 2,   // 29:28
			b_rsvd1                         : 2;   // 31:30
} L9A0_IRIS_086_T;

typedef struct { // 0x4358
	UINT32	md_k_thresh                     : 8,   //  7: 0
			b_rsvd1                         : 24;  // 31: 8
} L9A0_IRIS_087_T;

typedef struct { // 0x435c
	UINT32	tcomb_chroma_level              : 8,   //  7: 0
			hf_luma_chroma_offset           : 8,   // 15: 8
			spatial_luma_level              : 8,   // 23:16
			chroma_level                    : 8;   // 31:24
} L9A0_IRIS_088_T;

typedef struct { // 0x4360
	UINT32	chroma_activity_level           : 8,   //  7: 0
			lf_luma_offset                  : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_089_T;

typedef struct { // 0x4364
	UINT32	avg_freq_range                  : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			issecam_th                      : 6,   //  9: 4
			b_rsvd2                         : 2,   // 11:10
			freq_offset_range               : 8,   // 19:12
			b_rsvd3                         : 12;  // 31:20
} L9A0_IRIS_090_T;

typedef struct { // 0x4368
	UINT32	chroma_bw_motion_th             : 8,   //  7: 0
			motion_config                   : 8,   // 15: 8
			hactive_md_width                : 8,   // 23:16
			hactive_md_start                : 8;   // 31:24
} L9A0_IRIS_091_T;

typedef struct { // 0x436c
	UINT32	status_motion_mode              : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			chroma_bw_motion                : 2,   //  5: 4
			b_rsvd2                         : 26;  // 31: 6
} L9A0_IRIS_092_T;

typedef struct { // 0x4370
	UINT32	flat_chroma_offset              : 8,   //  7: 0
			flat_luma_offset                : 8,   // 15: 8
			flat_chroma_shift               : 2,   // 17:16
			b_rsvd1                         : 2,   // 19:18
			flat_luma_shift                 : 2,   // 21:20
			b_rsvd2                         : 10;  // 31:22
} L9A0_IRIS_093_T;

typedef struct { // 0x4374
	UINT32	motion_debug                    : 8,   //  7: 0
			cf_flat_motion_shift            : 2,   //  9: 8
			b_rsvd1                         : 2,   // 11:10
			motion_c_mode                   : 2,   // 13:12
			b_rsvd2                         : 18;  // 31:14
} L9A0_IRIS_094_T;

typedef struct { // 0x4378
	UINT32	cordic_gate_end                 : 8,   //  7: 0
			cordic_gate_start               : 8,   // 15: 8
			ispal_th                        : 8,   // 23:16
			phase_offset_range              : 8;   // 31:24
} L9A0_IRIS_095_T;

typedef struct { // 0x437c
	UINT32	pal3tap_only_c                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			pal3tap_only_y                  : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			adc_cpump_swap                  : 6,   // 13: 8
			b_rsvd3                         : 18;  // 31:14
} L9A0_IRIS_096_T;

typedef struct { // 0x4380
	UINT32	ckill                           : 16,  // 15: 0
			cagc_gate_end                   : 8,   // 23:16
			cagc_gate_start                 : 8;   // 31:24
} L9A0_IRIS_097_T;

typedef struct { // 0x4384
	UINT32	icburst_half_ampl               : 10,  //  9: 0
			b_rsvd1                         : 22;  // 31:10
} L9A0_IRIS_098_T;

typedef struct { // 0x4388
	UINT32	agc_bypass                      : 1,   //     0
			b_rsvd1                         : 31;  // 31: 1
} L9A0_IRIS_099_T;

typedef struct { // 0x438c
	UINT32	clampagc_inv                    : 4,   //  3: 0
			b_rsvd1                         : 28;  // 31: 4
} L9A0_IRIS_100_T;

typedef struct { // 0x43d8
	UINT32	vps_slicer_mode                 : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			wss_slicer_mode                 : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			tt_slicer_mode                  : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			cc_slicer_mode                  : 2,   // 13:12
			b_rsvd4                         : 18;  // 31:14
} L9A0_IRIS_119_T;

typedef struct { // 0x43dc
	UINT32	perr_on                         : 1,   //     0
			b_rsvd1                         : 31;  // 31: 1
} L9A0_IRIS_120_T;

typedef struct { // 0x43e4
	UINT32	perr_dnref                      : 32;  // 31: 0
} L9A0_IRIS_122_T;

typedef struct { // 0x4428
	UINT32	cres_lpfa_coeff0                : 13,  // 12: 0
			b_rsvd1                         : 3,   // 15:13
			cres_lpfa_on                    : 1,   //    16
			b_rsvd2                         : 3,   // 19:17
			iris_cresampler_on              : 1,   //    20
			b_rsvd3                         : 11;  // 31:21
} L9A0_IRIS_139_T;

typedef struct { // 0x442c
	UINT32	cres_lpfa_coeff2                : 13,  // 12: 0
			b_rsvd1                         : 3,   // 15:13
			cres_lpfa_coeff1                : 13,  // 28:16
			b_rsvd2                         : 3;   // 31:29
} L9A0_IRIS_140_T;

typedef struct { // 0x4430
	UINT32	cres_lpfa_coeff4                : 13,  // 12: 0
			b_rsvd1                         : 3,   // 15:13
			cres_lpfa_coeff3                : 13,  // 28:16
			b_rsvd2                         : 3;   // 31:29
} L9A0_IRIS_141_T;

typedef struct { // 0x4434
	UINT32	cres_lpfa_coeff5                : 13,  // 12: 0
			b_rsvd1                         : 19;  // 31:13
} L9A0_IRIS_142_T;

typedef struct { // 0x4438
	UINT32	cres_lpfb_coeff0                : 13,  // 12: 0
			b_rsvd1                         : 3,   // 15:13
			cres_lpfb_on                    : 1,   //    16
			b_rsvd2                         : 15;  // 31:17
} L9A0_IRIS_143_T;

typedef struct { // 0x443c
	UINT32	cres_lpfb_coeff2                : 13,  // 12: 0
			b_rsvd1                         : 3,   // 15:13
			cres_lpfb_coeff1                : 13,  // 28:16
			b_rsvd2                         : 3;   // 31:29
} L9A0_IRIS_144_T;

typedef struct { // 0x4440
	UINT32	cres_lpfb_coeff4                : 13,  // 12: 0
			b_rsvd1                         : 3,   // 15:13
			cres_lpfb_coeff3                : 13,  // 28:16
			b_rsvd2                         : 3;   // 31:29
} L9A0_IRIS_145_T;

typedef struct { // 0x4444
	UINT32	cres_blend_sel                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cres_blend_method               : 3,   //  6: 4
			b_rsvd2                         : 25;  // 31: 7
} L9A0_IRIS_146_T;

typedef struct { // 0x4448
	UINT32	cres_blend_bcdefault            : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			cres_0px_sel                    : 1,   //    12
			b_rsvd2                         : 3,   // 15:13
			cres_clksel                     : 2,   // 17:16
			b_rsvd3                         : 14;  // 31:18
} L9A0_IRIS_147_T;

typedef struct { // 0x444c
	UINT32	cres_blend_bcpara1              : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			cres_blend_bcpara0              : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_IRIS_148_T;

typedef struct { // 0x4450
	UINT32	cres_blend_bcpara3              : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			cres_blend_bcpara2              : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_IRIS_149_T;

typedef struct { // 0x4454
	UINT32	cres_blend_bcpara5              : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			cres_blend_bcpara4              : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_IRIS_150_T;

typedef struct { // 0x4458
	UINT32	cres_blend_bcpara7              : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			cres_blend_bcpara6              : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_IRIS_151_T;

typedef struct { // 0x445c
	UINT32	cres_blend_diffth1              : 12,  // 11: 0
			cres_blend_diffth0              : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_152_T;

typedef struct { // 0x4460
	UINT32	cres_alpha_off                  : 8,   //  7: 0
			cres_blend_diffth2              : 12,  // 19: 8
			b_rsvd1                         : 12;  // 31:20
} L9A0_IRIS_153_T;

typedef struct { // 0x4464
	UINT32	hrscbld_dnoff                   : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			hrscbld_upoff                   : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			hrsybld_dnoff                   : 3,   // 10: 8
			b_rsvd3                         : 1,   //    11
			hrsybld_upoff                   : 4,   // 15:12
			dtrs_dlysel                     : 4,   // 19:16
			dtrs_revalpha_sel               : 2,   // 21:20
			b_rsvd4                         : 10;  // 31:22
} L9A0_IRIS_154_T;

typedef struct { // 0x4468
	UINT32	cvd_2dcomb_ycrdc_lrsel          : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cvd_2dcomb_ycrdc_chsel          : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			cvd_2dcomb_ycrdc_cvsel          : 3,   // 10: 8
			b_rsvd3                         : 1,   //    11
			cvd_2dcomb_ccombo_sel           : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			cvd_2dcomb_cband_sel            : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			cvd_2dcomb_byp                  : 1,   //    20
			b_rsvd6                         : 11;  // 31:21
} L9A0_IRIS_155_T;

typedef struct { // 0x446c
	UINT32	cvd_2dcomb_ycrdc_yhdiffth       : 12,  // 11: 0
			cvd_2dcomb_ycrdc_yhsymth        : 12,  // 23:12
			cvd_2dcomb_divider_th           : 8;   // 31:24
} L9A0_IRIS_156_T;

typedef struct { // 0x4470
	UINT32	cvd_2dcomb_ycrdc_cvdiffth       : 12,  // 11: 0
			b_rsvd1                         : 20;  // 31:19
} L9A0_IRIS_157_T;

typedef struct { // 0x4474
	UINT32	cvd_2dcomb_ycrdc_method         : 4,   //  3: 0
			cvd_2dcomb_ycrdc_lcr_on         : 3,   //  6: 4
			b_rsvd1                         : 1,   //     7
			cvd_2dcomb_ycrdc_dsel           : 4,   // 11: 8
			cvd_2dcomb_ycrdc_lrtype         : 1,   //    12
			b_rsvd2                         : 19;  // 31:13
} L9A0_IRIS_158_T;

typedef struct { // 0x4478
	UINT32	cvd_2dcomb_chroma_diffth3       : 7,   //  6: 0
			b_rsvd1                         : 1,   //     7
			cvd_2dcomb_chroma_diffth2       : 7,   // 14: 8
			b_rsvd2                         : 1,   //    15
			cvd_2dcomb_chroma_diffth1       : 7,   // 22:16
			b_rsvd3                         : 9;   // 31:23
} L9A0_IRIS_159_T;

typedef struct { // 0x447c
	UINT32	cvd_2dcomb_ycrdc_hth            : 12,  // 11: 0
			cvd_2dcomb_ycrdc_hth_sel        : 2,   // 13:12
			b_rsvd1                         : 2,   // 15:14
			cvd_2dcomb_ycrdc_hth_on         : 1,   //    16
			b_rsvd2                         : 3,   // 19:17
			cvd_2dcomb_ycrdc_yhsel          : 4,   // 23:20
			cvd_2dcomb_ycrdc_yvsel          : 3,   // 26:24
			b_rsvd3                         : 5;   // 31:27
} L9A0_IRIS_160_T;

typedef struct { // 0x4480
	UINT32	cvd_2dcomb_cyrdc_dsel1          : 4,   //  3: 0
			cvd_2dcomb_cyrdc_dsel           : 4,   //  7: 4
			cvd_2dcomb_cyrdc_lrtype         : 1,   //     8
			b_rsvd1                         : 3,   // 11: 9
			cvd_2dcomb_cyrdc_crvalid        : 2,   // 13:12
			b_rsvd2                         : 2,   // 15:14
			cvd_2dcomb_cyrdc_lcr_on         : 3,   // 18:16
			b_rsvd3                         : 1,   //    19
			cvd_2dcomb_cyrdc_method         : 4,   // 23:20
			b_rsvd4                         : 8;   // 31:24
} L9A0_IRIS_161_T;

typedef struct { // 0x4484
	UINT32	cvd_2dcomb_cyrdc_chsymth1       : 12,  // 11: 0
			cvd_2dcomb_cyrdc_chsymth        : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_162_T;

typedef struct { // 0x4488
	UINT32	cvd_2dcomb_cyrdc_chdiffth       : 12,  // 11: 0
			b_rsvd1                         : 20;  // 31:12
} L9A0_IRIS_163_T;

typedef struct { // 0x448c
	UINT32	cvd_2dcomb_cyrdc_chdiffsel      : 4,   //  3: 0
			cvd_2dcomb_cyrdc_chdiffon       : 8,   // 11: 4
			b_rsvd1                         : 20;  // 31:12
} L9A0_IRIS_164_T;

typedef struct { // 0x4490
	UINT32	cvd_2dcomb_cyrdc_hth            : 12,  // 11: 0
			cvd_2dcomb_cyrdc_hth_on         : 1,   //    12
			b_rsvd1                         : 19;  // 31:13
} L9A0_IRIS_165_T;

typedef struct { // 0x4494
	UINT32	oadj_y_offo                     : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			oadj_y_offi                     : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_IRIS_166_T;

typedef struct { // 0x4498
	UINT32	oadj_y_coeff                    : 14,  // 13: 0
			b_rsvd1                         : 18;  // 31:14
} L9A0_IRIS_167_T;

typedef struct { // 0x449c
	UINT32	oadj_c_offo                     : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			oadj_c_offi                     : 11,  // 22:12
			b_rsvd2                         : 9;   // 31:23
} L9A0_IRIS_168_T;

typedef struct { // 0x44a0
	UINT32	oadj_c_coeff                    : 14,  // 13: 0
			b_rsvd1                         : 18;  // 31:14
} L9A0_IRIS_169_T;

typedef struct { // 0x44a4
	UINT32	adc_unity_gain_value            : 4,   //  3: 0
			b_rsvd1                         : 28;  // 31: 4
} L9A0_IRIS_170_T;

typedef struct { // 0x44a8
	UINT32	frame_motion_th1                : 16,  // 15: 0
			frame_motion_th0                : 16;  // 31:16
			// dws 110420
			//b_rsvd1                         : 18;  // 31:14
} L9A0_IRIS_171_T;

typedef struct { // 0x44ac
	UINT32	iblank_half_lvl0                : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			isynctip_lvl0                   : 10,  // 21:12
			b_rsvd2                         : 10;  // 31:22
} L9A0_IRIS_172_T;

typedef struct { // 0x44b0
	UINT32	sync_int_blank_lvl0             : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			ipeak_chroma_m768_lvl0          : 10,  // 21:12
			b_rsvd2                         : 10;  // 31:22
} L9A0_IRIS_173_T;

typedef struct { // 0x44b4
	UINT32	sync_int_blank_vid_lvl0         : 8,   //  7: 0
			int_black_ped_vid_lvl0          : 10,  // 17: 8
			b_rsvd1                         : 2,   // 19:18
			int_blank_vid_lvl0              : 10,  // 29:20
			b_rsvd2                         : 2;   // 31:30
} L9A0_IRIS_174_T;

typedef struct { // 0x44b8
	UINT32	int_vbi_blank_vid_lvl0          : 8,   //  7: 0
			b_rsvd1                         : 24;  // 31: 8
} L9A0_IRIS_175_T;

typedef struct { // 0x44bc
	UINT32	internal_sync_midpt             : 13,  // 12: 0
			b_rsvd1                         : 3,   // 15:13
			internal_sync_level             : 13,  // 28:16
			b_rsvd2                         : 3;   // 31:29
} L9A0_IRIS_176_T;

typedef struct { // 0x44c0
	UINT32	sync_internal_sync_midpt        : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			internal_blank_level            : 13,  // 24:12
			b_rsvd2                         : 7;   // 31:25
} L9A0_IRIS_177_T;

typedef struct { // 0x44c4
	UINT32	adc_blank_level                 : 10,  //  9: 0
			b_rsvd1                         : 22;  // 31:10
} L9A0_IRIS_178_T;

typedef struct { // 0x44c8
	UINT32	hrs_alpha_croff                 : 8,   //  7: 0
			hrs_alpha_cboff                 : 8,   // 15: 8
			hrs_alpha_yoff                  : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_179_T;

typedef struct { // 0x44cc
	UINT32	cvd_2dcomb_notch_coefi1         : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			cvd_2dcomb_notch_coefi0         : 10,  // 21:12
			b_rsvd2                         : 10;  // 31:22
} L9A0_IRIS_180_T;

typedef struct { // 0x44d0
	UINT32	cvd_2dcomb_notch_coefi3         : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			cvd_2dcomb_notch_coefi2         : 10,  // 21:12
			b_rsvd2                         : 10;  // 31:22
} L9A0_IRIS_181_T;

typedef struct { // 0x44d4
	UINT32	cvd_2dcomb_notch_coefi1_2nd     : 8,   //  7: 0
			cvd_2dcomb_notch_coefi0_2nd     : 8,   // 15: 8
			cvd_2dcomb_notch_coefi4         : 10,  // 25:16
			b_rsvd1                         : 6;   // 31:26
} L9A0_IRIS_182_T;

typedef struct { // 0x44d8
	UINT32	cvd_2dcomb_notch_coefi4_2nd     : 8,   //  7: 0
			cvd_2dcomb_notch_coefi3_2nd     : 8,   // 15: 8
			cvd_2dcomb_notch_coefi2_2nd     : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_183_T;

typedef struct { // 0x44dc
	UINT32	cvd_3dcomb_coefi_dly            : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cvd_3dcomb_coefi_disp_on        : 1,   //     4
			b_rsvd2                         : 27;  // 31: 5
} L9A0_IRIS_184_T;

typedef struct { // 0x44e0
	UINT32	cvd_3dcomb_coefi_pow            : 4,   //  3: 0
			b_rsvd1                         : 28;  // 31: 4
} L9A0_IRIS_185_T;

typedef struct { // 0x44e4
	UINT32	cvd_3dcomb_coefi                : 16,  // 15: 0
			b_rsvd1                         : 16;  // 31:16
} L9A0_IRIS_186_T;

typedef struct { // 0x44e8
	UINT32	cvd_3dcomb_coefi__2              : 32;  // 31: 0
} L9A0_IRIS_187_T;

typedef struct { // 0x44ec
	UINT32	cvd_3dcomb_coefi__3              : 32;  // 31: 0
} L9A0_IRIS_188_T;

typedef struct { // 0x44f0
	UINT32	cvd_3dcomb_mdy_sel              : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			cvd_3dcomb_mdy_disel1           : 5,   //  8: 4
			b_rsvd2                         : 3,   // 11: 9
			cvd_3dcomb_mdy_disel            : 3,   // 14:12
			b_rsvd3                         : 1,   //    15
			cvd_3dcomb_mdy_dir              : 4,   // 19:16
			cvd_3dcomb_md_mode              : 2,   // 21:20
			b_rsvd4                         : 2,   // 23:22
			cvd_3dcomb_dhf_dinsel           : 2,   // 25:24
			b_rsvd5                         : 2,   // 27:26
			cvd_3dcomb_avg_sad_divsel       : 2,   // 29:28
			b_rsvd6                         : 2;   // 31:30
} L9A0_IRIS_189_T;

typedef struct { // 0x44f4
	UINT32	cvd_3dcomb_mdc_sel              : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cvd_3dcomb_mdc_avgsel           : 4,   //  7: 4
			cvd_3dcomb_mdc_dir              : 4,   // 11: 8
			cvd_3dcomb_mdc_disel1           : 4,   // 15:12
			cvd_3dcomb_mdc_disel            : 1,   //    16
			b_rsvd2                         : 3,   // 19:17
			cvd_3dcomb_mdy_sad_sel          : 1,   //    20
			b_rsvd3                         : 3,   // 23:21
			cvd_3dcomb_mdy_sad_th           : 7,   // 30:24
			b_rsvd4                         : 1;   // 31:31
} L9A0_IRIS_190_T;

typedef struct { // 0x44f8
	UINT32	cvd_3dcomb_mdi_mdnwgt           : 8,   //  7: 0
			cvd_3dcomb_mdi_wgt              : 8,   // 15: 8
			cvd_3dcomb_mdi_disel            : 5,   // 20:16
			b_rsvd1                         : 3,   // 23:21
			cvd_3dcomb_mdc_wgt              : 8;   // 31:24
} L9A0_IRIS_191_T;

typedef struct { // 0x44fc
	UINT32	cvd_3dcomb_mdi_lsel             : 4,   //  3: 0
			cvd_3dcomb_mdi_th_on            : 8,   // 11: 4
			cvd_3dcomb_mdi_sadsel           : 8,   // 19:12
			cvd_3dcomb_mdi_svcnt            : 4,   // 23:20
			cvd_3dcomb_mdi_svth             : 7,   // 30:24
			b_rsvd1                         : 1;   // 31:31
} L9A0_IRIS_192_T;

typedef struct { // 0x4500
	UINT32	cvd_3dcomb_mdi_frm_th           : 24,  // 23: 0
			cvd_3dcomb_mdi_lsel1            : 4,   // 27:24
			b_rsvd1                         : 4;   // 31:28
} L9A0_IRIS_193_T;

typedef struct { // 0x4504
	UINT32	cvd_3dcomb_mdi_loc_th0          : 7,   //  6: 0
			b_rsvd1                         : 1,   //     7
			cvd_3dcomb_mdi_frm_th1          : 24;  // 31: 8
} L9A0_IRIS_194_T;

typedef struct { // 0x4508
	UINT32	cvd_3dcomb_svy_disp_on          : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cvd_3dcomb_mdi_loc_th3          : 7,   // 10: 4
			b_rsvd2                         : 1,   //    11
			cvd_3dcomb_mdi_loc_th2          : 7,   // 18:12
			b_rsvd3                         : 1,   //    19
			cvd_3dcomb_mdi_loc_th1          : 7,   // 26:20
			b_rsvd4                         : 5;   // 31:27
} L9A0_IRIS_195_T;

typedef struct { // 0x450c
	UINT32	cphase_adjust                   : 3,   //  2: 0
			b_rsvd1                         : 1,   //     3
			cphase_adjust_sel               : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cvd_3dcomb_mdclass_fldsel       : 3,   // 10: 8
			b_rsvd3                         : 1,   //    11
			cvd_3dcomb_mdclass_dsel         : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			cvd_3dcomb_mdclass_csel         : 3,   // 18:16
			b_rsvd5                         : 1,   //    19
			cvd_3dcomb_svy_loc_th           : 4,   // 23:20
			cvd_3dcomb_svy_th               : 7,   // 30:24
			b_rsvd6                         : 1;   // 31:31
} L9A0_IRIS_196_T;

typedef struct { // 0x4510
	UINT32	cvd_3dcomb_mdcf_actv_th         : 7,   //  6: 0
			b_rsvd1                         : 1,   //     7
			cvd_3dcomb_mdhf_actv_th         : 7,   // 14: 8
			b_rsvd2                         : 1,   //    15
			cvd_3dcomb_mdlf_actv_th         : 7,   // 22:16
			b_rsvd3                         : 1,   //    23
			cvd_3dcomb_mdlf_actv_sel        : 1,   //    24
			b_rsvd4                         : 7;   // 31:25
} L9A0_IRIS_197_T;

typedef struct { // 0x4514
	UINT32	cvd_3dcomb_mdcf_blk_th          : 8,   //  7: 0
			cvd_3dcomb_mdhf_blk_th          : 8,   // 15: 8
			cvd_3dcomb_mdlf_blk_th          : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_198_T;

typedef struct { // 0x4518
	UINT32	cvd_3dcomb_mdlf_fld_th1         : 12,  // 11: 0
			cvd_3dcomb_mdlf_fld_th0         : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_199_T;

typedef struct { // 0x451c
	UINT32	cvd_3dcomb_mdhf_fld_sel         : 4,   //  3: 0
			cvd_3dcomb_mdlf_fld_th3         : 12,  // 15: 4
			cvd_3dcomb_mdlf_fld_th2         : 12,  // 27:16
			b_rsvd1                         : 4;   // 31:28
} L9A0_IRIS_200_T;

typedef struct { // 0x4520
	UINT32	cvd_3dcomb_mdhf_fld_th1         : 12,  // 11: 0
			cvd_3dcomb_mdhf_fld_th0         : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_201_T;

typedef struct { // 0x4524
	UINT32	cvd_3dcomb_mdhf_fld_th3         : 12,  // 11: 0
			cvd_3dcomb_mdhf_fld_th2         : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_202_T;

typedef struct { // 0x4528
	UINT32	cvd_3dcomb_mdhf_fld_grd         : 8,   //  7: 0
			cvd_3dcomb_mdlf_fld_grd         : 8,   // 15: 8
			cvd_3dcomb_mdhlf_fld_sub_th0    : 16;  // 31:16
} L9A0_IRIS_203_T;

typedef struct { // 0x452c
	UINT32	cvd_3dcomb_mdlf_fld_grd_alp     : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_204_T;

typedef struct { // 0x4530
	UINT32	cvd_3dcomb_mdlf_fld_grd_alp__2    : 32;  // 31: 0
} L9A0_IRIS_205_T;

typedef struct { // 0x4534
	UINT32	cvd_3dcomb_mdhf_fld_grd_alp     : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_206_T;

typedef struct { // 0x4538
	UINT32	cvd_3dcomb_mdhf_fld_grd_alp__2    : 32;  // 31: 0
} L9A0_IRIS_207_T;

typedef struct { // 0x453c
	UINT32	status_fld_hfcnt                : 12,  // 11: 0
			status_fld_lfcnt                : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_208_T;

typedef struct { // 0x4540
	UINT32	cvd_3dcomb_mdpd32_sel           : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			cvd_3dcomb_mdpd32_on            : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			status_fld_hlfcnt               : 12,  // 19: 8
			status_fld_cfcnt                : 12;  // 31:20
} L9A0_IRIS_209_T;

typedef struct { // 0x4544
	UINT32	cvd_3dcomb_mdglo_mode           : 4,   //  3: 0
			cvd_3dcomb_mdph32_cs            : 3,   //  6: 4
			b_rsvd1                         : 1,   //     7
			cvd_3dcomb_mdpd32_th            : 24;  // 31: 8
} L9A0_IRIS_210_T;

typedef struct { // 0x4548
	UINT32	cvd_3dcomb_mdloc_hsize          : 7,   //  6: 0
			b_rsvd1                         : 1,   //     7
			cvd_3dcomb_mdglo_m1_sameth      : 8,   // 15: 8
			cvd_3dcomb_mdglo_m1_uth0        : 5,   // 20:16
			b_rsvd2                         : 3,   // 23:21
			cvd_3dcomb_mdglo_m1_lth0        : 5,   // 28:24
			b_rsvd3                         : 3;   // 31:29
} L9A0_IRIS_211_T;

typedef struct { // 0x454c
	UINT32	cvd_3dcomb_mdloc_hfgrd_off      : 8,   //  7: 0
			cvd_3dcomb_mdloc_lfgrd_off      : 8,   // 15: 8
			cvd_3dcomb_mdloc_hfgrd          : 8,   // 23:16
			cvd_3dcomb_mdloc_lfgrd          : 8;   // 31:24
} L9A0_IRIS_212_T;

typedef struct { // 0x4550
	UINT32	cvd_3dcomb_mdloc_lfgrd_alp      : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_213_T;

typedef struct { // 0x4554
	UINT32	cvd_3dcomb_mdloc_lfgrd_alp_     : 32;  // 31: 0
} L9A0_IRIS_214_T;

typedef struct { // 0x4558
	UINT32	cvd_3dcomb_mdloc_hfgrd_alp      : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_215_T;

typedef struct { // 0x455c
	UINT32	cvd_3dcomb_mdloc_hfgrd_alp_     : 32;  // 31: 0
} L9A0_IRIS_216_T;

typedef struct { // 0x4560
	UINT32	cvd_3dcomb_mdloc_hfth0          : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			cvd_3dcomb_mdloc_lfth0          : 10,  // 21:12
			b_rsvd2                         : 10;  // 31:22
} L9A0_IRIS_217_T;

typedef struct { // 0x4564
	UINT32	cvd_3dcomb_mdloc_hfth1          : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			cvd_3dcomb_mdloc_lfth1          : 10,  // 21:12
			b_rsvd2                         : 10;  // 31:22
} L9A0_IRIS_218_T;

typedef struct { // 0x4568
	UINT32	cvd_3dcomb_mdloc_on             : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cvd_3dcomb_mdloc_dispon         : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cvd_3dcomb_mdloc_hfth2          : 10,  // 17: 8
			b_rsvd3                         : 2,   // 19:18
			cvd_3dcomb_mdloc_lfth2          : 10,  // 29:20
			b_rsvd4                         : 2;   // 31:30
} L9A0_IRIS_219_T;

typedef struct { // 0x456c
	UINT32	cvd_3dcomb_mdloc_gld_lth1       : 5,   //  4: 0
			b_rsvd1                         : 3,   //  7: 5
			cvd_3dcomb_mdloc_gld_uth0       : 5,   // 12: 8
			b_rsvd2                         : 3,   // 15:13
			cvd_3dcomb_mdloc_gld_lth0       : 5,   // 20:16
			b_rsvd3                         : 3,   // 23:21
			cvd_3dcomb_mdloc_sel            : 1,   //    24
			b_rsvd4                         : 7;   // 31:25
} L9A0_IRIS_220_T;

typedef struct { // 0x4570
	UINT32	cvd_3dcomb_mdloc_loc_lth1       : 5,   //  4: 0
			b_rsvd1                         : 3,   //  7: 5
			cvd_3dcomb_mdloc_loc_uth0       : 5,   // 12: 8
			b_rsvd2                         : 3,   // 15:13
			cvd_3dcomb_mdloc_loc_lth0       : 5,   // 20:16
			b_rsvd3                         : 3,   // 23:21
			cvd_3dcomb_mdloc_gld_uth1       : 5,   // 28:24
			b_rsvd4                         : 3;   // 31:29
} L9A0_IRIS_221_T;

typedef struct { // 0x4574
	UINT32	cvd_2dcomb_comb_byp             : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			cvd_3dcomb_mdloc_actv_hfth      : 7,   // 10: 4
			b_rsvd2                         : 1,   //    11
			cvd_3dcomb_mdloc_actv_lfth      : 7,   // 18:12
			b_rsvd3                         : 1,   //    19
			cvd_3dcomb_mdloc_actv_lfsel     : 1,   //    20
			b_rsvd4                         : 3,   // 23:21
			cvd_3dcomb_mdloc_loc_uth1       : 5,   // 28:24
			b_rsvd5                         : 3;   // 31:29
} L9A0_IRIS_222_T;

typedef struct { // 0x4578
	UINT32	cvd_2dcomb_ycrdc_v1b0_th        : 8,   //  7: 0
			cvd_2dcomb_ycrdc_v1b4_th        : 8,   // 15: 8
			cvd_2dcomb_ycrdc_v1b4_dinsel    : 1,   //    16
			b_rsvd1                         : 3,   // 19:17
			cvd_2dcomb_ycrdc_v1b4_cinsel    : 1,   //    20
			b_rsvd2                         : 3,   // 23:21
			cvd_2dcomb_ycrdc_v0b1_cinsel    : 1,   //    24
			b_rsvd3                         : 3,   // 27:25
			cvd_2dcomb_ycrdc_dir_on         : 1,   //    28
			b_rsvd4                         : 3;   // 31:29
} L9A0_IRIS_223_T;

typedef struct { // 0x457c
	UINT32	cvd_2dcomb_ycrdc_yhsymsel       : 1,   //     0
			b_rsvd1                         : 31;  // 31: 1
} L9A0_IRIS_224_T;

typedef struct { // 0x4580
	UINT32	cvd_2dcomb_ycrdc_yvdiv_th0      : 12,  // 11: 0
			cvd_2dcomb_ycrdc_yvdiv_xsel     : 4,   // 15:12
			cvd_2dcomb_ycrdc_vth            : 8,   // 23:16
			cvd_2dcomb_ycrdc_vth_on         : 1,   //    24
			b_rsvd1                         : 7;   // 31:25
} L9A0_IRIS_225_T;

typedef struct { // 0x4584
	UINT32	cvd_2dcomb_ycrdc_yvdiv_th2      : 12,  // 11: 0
			cvd_2dcomb_ycrdc_yvdiv_th1      : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_226_T;

typedef struct { // 0x4588
	UINT32	cvd_2dcomb_ycrdc_yvdiv_th4      : 12,  // 11: 0
			cvd_2dcomb_ycrdc_yvdiv_th3      : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_227_T;

typedef struct { // 0x458c
	UINT32	cvd_2dcomb_cyrdc_v0b1_dinsel    : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cvd_2dcomb_cyrdc_dir_on         : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cvd_2dcomb_ycrdc_yvdiv_th6      : 12,  // 19: 8
			cvd_2dcomb_ycrdc_yvdiv_th5      : 12;  // 31:20
} L9A0_IRIS_228_T;

typedef struct { // 0x4590
	UINT32	cvd_2dcomb_cyrdc_v1b4_th        : 8,   //  7: 0
			cvd_2dcomb_cyrdc_v1b4_dinsel    : 1,   //     8
			b_rsvd1                         : 3,   // 11: 9
			cvd_2dcomb_cyrdc_v1b4_cinsel    : 1,   //    12
			b_rsvd2                         : 19;  // 31:13
} L9A0_IRIS_229_T;

typedef struct { // 0x4594
	UINT32	cvd_2dcomb_cyrdc_cvdiv_th0      : 12,  // 11: 0
			cvd_2dcomb_cyrdc_cvdiv_xsel     : 4,   // 15:12
			cvd_2dcomb_cyrdc_hvth           : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_230_T;

typedef struct { // 0x4598
	UINT32	cvd_2dcomb_cyrdc_cvdiv_th2      : 12,  // 11: 0
			cvd_2dcomb_cyrdc_cvdiv_th1      : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_231_T;

typedef struct { // 0x459c
	UINT32	cvd_2dcomb_cyrdc_cvdiv_th4      : 12,  // 11: 0
			cvd_2dcomb_cyrdc_cvdiv_th3      : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_232_T;

typedef struct { // 0x45a0
	UINT32	cvd_2dcomb_cyrdc_cvdiv_th6      : 12,  // 11: 0
			cvd_2dcomb_cyrdc_cvdiv_th5      : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_233_T;

typedef struct { // 0x45a4
	UINT32	adap_clf_svy_loc_th             : 4,   //  3: 0
			adap_clf_mode                   : 3,   //  6: 4
			b_rsvd1                         : 1,   //     7
			cb_delay                        : 2,   //  9: 8
			b_rsvd2                         : 2,   // 11:10
			yc444                           : 1,   //    12
			b_rsvd3                         : 3,   // 15:13
			ycadj_cr_p                      : 1,   //    16
			b_rsvd4                         : 3,   // 19:17
			oadj_swap                       : 1,   //    20
			b_rsvd5                         : 11;  // 31:21
} L9A0_IRIS_234_T;

typedef struct { // 0x45a8
	UINT32	lpf_tap_0_0                     : 16,  // 15: 0
			lpf_0_en                        : 1,   //    16
			b_rsvd1                         : 15;  // 31:17
} L9A0_IRIS_235_T;

typedef struct { // 0x45ac
	UINT32	lpf_tap_0_2                     : 16,  // 15: 0
			lpf_tap_0_1                     : 16;  // 31:16
} L9A0_IRIS_236_T;

typedef struct { // 0x45b0
	UINT32	lpf_tap_0_4                     : 16,  // 15: 0
			lpf_tap_0_3                     : 16;  // 31:16
} L9A0_IRIS_237_T;

typedef struct { // 0x45b4
	UINT32	lpf_tap_0_6                     : 16,  // 15: 0
			lpf_tap_0_5                     : 16;  // 31:16
} L9A0_IRIS_238_T;

typedef struct { // 0x45c4
	UINT32	hs_missing_max                  : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			hs_match_max                    : 6,   // 13: 8
			b_rsvd2                         : 2,   // 15:14
			hs_cnt_mode                     : 1,   //    16
			b_rsvd3                         : 3,   // 19:17
			hs_sync_mode                    : 3,   // 22:20
			b_rsvd4                         : 1,   //    23
			status_pseudo_sync              : 4,   // 27:24
			b_rsvd5                         : 4;   // 31:28
} L9A0_IRIS_242_T;

typedef struct { // 0x45c8
	UINT32	hs_length_prd_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_243_T;

typedef struct { // 0x45cc
	UINT32	hs_length_prd_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_244_T;

typedef struct { // 0x45d0
	UINT32	hs_length_pos_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_245_T;

typedef struct { // 0x45d4
	UINT32	hs_length_pos_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_246_T;

typedef struct { // 0x45d8
	UINT32	hs_length_prd_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_247_T;

typedef struct { // 0x45dc
	UINT32	hs_length_prd_th_1              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_248_T;

typedef struct { // 0x45e0
	UINT32	hs_length_pos_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_249_T;

typedef struct { // 0x45e4
	UINT32	vs_cnt_mode                     : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			vs_sync_mode                    : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			hs_length_pos_th_1              : 24;  // 31: 8
} L9A0_IRIS_250_T;

typedef struct { // 0x45e8
	UINT32	vs_missing_max                  : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			vs_match_max                    : 6,   // 13: 8
			b_rsvd2                         : 18;  // 31:14
} L9A0_IRIS_251_T;

typedef struct { // 0x45ec
	UINT32	vs_length_prd_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_252_T;

typedef struct { // 0x45f0
	UINT32	vs_length_prd_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_253_T;

typedef struct { // 0x45f4
	UINT32	vs_length_pos_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_254_T;

typedef struct { // 0x45f8
	UINT32	vs_length_pos_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_255_T;

typedef struct { // 0x45fc
	UINT32	vs_length_prd_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_256_T;

typedef struct { // 0x4600
	UINT32	vs_length_prd_th_1              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_257_T;

typedef struct { // 0x4604
	UINT32	vs_length_pos_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_258_T;

typedef struct { // 0x4608
	UINT32	ha_cnt_mode                     : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			ha_sync_mode                    : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			vs_length_pos_th_1              : 24;  // 31: 8
} L9A0_IRIS_259_T;

typedef struct { // 0x460c
	UINT32	ha_missing_max                  : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			ha_match_max                    : 6,   // 13: 8
			b_rsvd2                         : 18;  // 31:14
} L9A0_IRIS_260_T;

typedef struct { // 0x4610
	UINT32	ha_length_prd_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_261_T;

typedef struct { // 0x4614
	UINT32	ha_length_prd_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_262_T;

typedef struct { // 0x4618
	UINT32	ha_length_pos_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_263_T;

typedef struct { // 0x461c
	UINT32	ha_length_pos_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_264_T;

typedef struct { // 0x4620
	UINT32	ha_length_prd_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_265_T;

typedef struct { // 0x4624
	UINT32	ha_length_prd_th_1              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_266_T;

typedef struct { // 0x4628
	UINT32	ha_length_pos_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_267_T;

typedef struct { // 0x462c
	UINT32	va_cnt_mode                     : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			va_sync_mode                    : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			ha_length_pos_th_1              : 24;  // 31: 8
} L9A0_IRIS_268_T;

typedef struct { // 0x4630
	UINT32	va_missing_max                  : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			va_match_max                    : 6,   // 13: 8
			b_rsvd2                         : 18;  // 31:14
} L9A0_IRIS_269_T;

typedef struct { // 0x4634
	UINT32	va_length_prd_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_270_T;

typedef struct { // 0x4638
	UINT32	va_length_prd_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_271_T;

typedef struct { // 0x463c
	UINT32	va_length_pos_0                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_272_T;

typedef struct { // 0x4640
	UINT32	va_length_pos_th_0              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_273_T;

typedef struct { // 0x4644
	UINT32	va_length_prd_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_274_T;

typedef struct { // 0x4648
	UINT32	va_length_prd_th_1              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_275_T;

typedef struct { // 0x464c
	UINT32	va_length_pos_1                 : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_276_T;

typedef struct { // 0x4650
	UINT32	va_length_pos_th_1              : 24,  // 23: 0
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_277_T;

typedef struct { // 0x4654
	UINT32	swrst_cdct						: 1,   //  0: 0
			swrst_hdct			            : 1,   //  1: 1
			iris_clampagc_v2                : 1,   //  2: 2
			b_rsvd1							: 4,   //  3: 6
			iris_mif_gmau                   : 1,   //  7: 7
			b_rsvd2                         : 24;  // 31: 8
} L9A0_IRIS_278_T;

typedef struct { // 0x4658
	UINT32	status_cenable_f0cnt            : 20,  // 19: 0
			status_cenable_ctrl             : 7,   // 26:20
			b_rsvd1                         : 5;   // 31:27
} L9A0_IRIS_279_T;

typedef struct { // 0x465c
	UINT32	status_cenable_lcnt             : 12,  // 11: 0
			status_cenable_f1cnt            : 20;  // 31:12
} L9A0_IRIS_280_T;

typedef struct { // 0x4660
	UINT32	status_updn_rst                 : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			clampagc_updn                   : 5,   //  8: 4
			b_rsvd2                         : 23;  // 31: 9
} L9A0_IRIS_281_T;

typedef struct { // 0x4664
	UINT32	status_updn                     : 32;  // 31: 0
} L9A0_IRIS_282_T;

typedef struct { // 0x4668
	UINT32	cvd_3dcomb_coefi_u              : 32;  // 31: 0
} L9A0_IRIS_283_T;

typedef struct { // 0x466c
	UINT32	cvd_3dcomb_coefi_u_             : 32;  // 31: 0
} L9A0_IRIS_284_T;

typedef struct { // 0x4670
	UINT32	cvd_3dcomb_coefi_thd            : 32;  // 31: 0
} L9A0_IRIS_285_T;

typedef struct { // 0x4674
	UINT32	cvd_3dcomb_coefi_thd_           : 32;  // 31: 0
} L9A0_IRIS_286_T;

typedef struct { // 0x4678
	UINT32	cvd_3dcomb_coefi_sel_uth        : 5,   //  4: 0
			b_rsvd1                         : 3,   //  7: 5
			cvd_3dcomb_coefi_sel_lth        : 5,   // 12: 8
			b_rsvd2                         : 19;  // 31:13
} L9A0_IRIS_287_T;

typedef struct { // 0x467c
	UINT32	cvd_3dcomb_coefi2               : 16,  // 15: 0
			b_rsvd1                         : 16;  // 31:13
} L9A0_IRIS_288_T;

typedef struct { // 0x4680
	UINT32	cvd_3dcomb_coefi2_              : 32;  // 31: 0
} L9A0_IRIS_289_T;

typedef struct { // 0x4684
	UINT32	cvd_3dcomb_coefi2_              : 32;  // 31: 0
} L9A0_IRIS_290_T;

typedef struct { // 0x4688
	UINT32	cvd_3dcomb_coefi2_              : 32;  // 31: 0
} L9A0_IRIS_291_T;

typedef struct { // 0x468c
	UINT32	cvd_3dcomb_coefi2_              : 32;  // 31: 0
} L9A0_IRIS_292_T;

typedef struct { // 0x4690
	UINT32	cvd_3dcomb_coefi2_thd           : 32;  // 31: 0
} L9A0_IRIS_293_T;

typedef struct { // 0x4694
	UINT32	cvd_3dcomb_coefi2_thd_          : 32;  // 31: 0
} L9A0_IRIS_294_T;

typedef struct { // 0x4698
	UINT32	cs_secam_pll                    : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs_secam_sw_old                 : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cs_auto_secam_level             : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cs_chroma_activity_level        : 8,   // 19:12
			cs_adaptive_chroma_mode         : 2,   // 21:20
			b_rsvd4                         : 2,   // 23:22
			cs_sw_rst                       : 1,   //    24
			b_rsvd5                         : 7;   // 31:25
} L9A0_IRIS_295_T;

typedef struct { // 0x469c
	UINT32	cs_phase_offset_range           : 8,   //  7: 0
			cs_avg_freq_range               : 2,   //  9: 8
			b_rsvd1                         : 2,   // 11:10
			cs_issecam_th                   : 6,   // 17:12
			b_rsvd2                         : 2,   // 19:18
			cs_freq_offset_range            : 8,   // 27:20
			b_rsvd3                         : 4;   // 31:28
} L9A0_IRIS_296_T;

typedef struct { // 0x46a0
	UINT32	cs_db_freq                      : 12,  // 11: 0
			cs_dr_freq                      : 12,  // 23:12
			cs_ispal_th                     : 8;   // 31:24
} L9A0_IRIS_297_T;

typedef struct { // 0x46a4
	UINT32	cs_cagc_en                      : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs_chroma_burst5or10            : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cs_pal60_mode                   : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cs_ntsc443_mode                 : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			cs_colour_mode                  : 3,   // 18:16
			b_rsvd5                         : 1,   //    19
			cs_adaptive_mode                : 3,   // 22:20
			b_rsvd6                         : 1,   //    23
			cs_secam_ybw                    : 2,   // 25:24
			b_rsvd7                         : 6;   // 31:26
} L9A0_IRIS_298_T;

typedef struct { // 0x46a8
	UINT32	cs_cagc_coring_threshold        : 4,   //  3: 0
			cs_cagc_coring                  : 3,   //  6: 4
			b_rsvd1                         : 1,   //     7
			cs_cagc_unity_gain              : 1,   //     8
			b_rsvd2                         : 3,   // 11: 9
			cs_cagc_tc_p                    : 2,   // 13:12
			b_rsvd3                         : 2,   // 15:14
			cs_cagc_tc_ibig                 : 3,   // 18:16
			b_rsvd4                         : 1,   //    19
			cs_cagc_tc_ismall               : 3,   // 22:20
			b_rsvd5                         : 1,   //    23
			cs_cagc                         : 8;   // 31:24
} L9A0_IRIS_299_T;

typedef struct { // 0x46ac
	UINT32	cs_chroma_bw_lo                 : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			cs_ckill                        : 16,  // 19: 4
			cs_hlock_ckill                  : 1,   //    20
			b_rsvd2                         : 3,   // 23:21
			cs_vbi_ckill                    : 1,   //    24
			b_rsvd3                         : 3,   // 27:25
			cs_noburst_ckill                : 1,   //    28
			b_rsvd4                         : 3;   // 31:29
} L9A0_IRIS_300_T;

typedef struct { // 0x46b0
	UINT32	cs_cdto_inc                     : 32;  // 31: 0
} L9A0_IRIS_301_T;

typedef struct { // 0x46b4
	UINT32	cs_cstate                       : 3,   //  2: 0
			b_rsvd1                         : 1,   //     3
			cs_fixed_cstate                 : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cs_user_ckill_mode              : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			cs_lose_chromalock_mode         : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			cs_lose_chromalock_count        : 4,   // 19:16
			cs_lose_chromalock_level        : 3,   // 22:20
			b_rsvd5                         : 1,   //    23
			cs_lose_chromalock_ckill        : 1,   //    24
			b_rsvd6                         : 3,   // 27:25
			cs_palsw_level                  : 2,   // 29:28
			b_rsvd7                         : 2;   // 31:30
} L9A0_IRIS_302_T;

typedef struct { // 0x46b8
	UINT32	cs_chroma_sel                   : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs_cphase_adjust                : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			cs_cphase_adjust_sel            : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cs_mv_colourstripes_sel         : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			cs_cstripe_detect_control       : 3,   // 18:16
			b_rsvd5                         : 13;  // 31:19
} L9A0_IRIS_303_T;

typedef struct { // 0x46bc
	UINT32	cs_secam_detected           	: 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs_mv_colourstripes             : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			cs_status_cordic_freq           : 8,   // 15: 8
			cs_status_burst_mag             : 16;  // 31:16
} L9A0_IRIS_304_T;

typedef struct { // 0x46c0
	UINT32	cs_chromalock                   : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs_status_cgain                 : 14,  // 17: 4
			b_rsvd2                         : 2,   // 19:18
			cs_status_cmag                  : 8,   // 27:20
			cs_pal_detected                 : 1,   //    28
			b_rsvd3                         : 3;   // 31:29
} L9A0_IRIS_305_T;

typedef struct { // 0x46c4
	UINT32	cs1_secam_pll                   : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs1_secam_sw_old                : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cs1_auto_secam_level            : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cs1_chroma_activity_level       : 8,   // 19:12
			cs1_adaptive_chroma_mode        : 2,   // 21:20
			b_rsvd4                         : 2,   // 23:22
			cs1_sw_rst                      : 1,   //    24
			b_rsvd5                         : 7;   // 31:25
} L9A0_IRIS_306_T;

typedef struct { // 0x46c8
	UINT32	cs1_phase_offset_range          : 8,   //  7: 0
			cs1_avg_freq_range              : 2,   //  9: 8
			b_rsvd1                         : 2,   // 11:10
			cs1_issecam_th                  : 6,   // 17:12
			b_rsvd2                         : 2,   // 19:18
			cs1_freq_offset_range           : 8,   // 27:20
			b_rsvd3                         : 4;   // 31:28
} L9A0_IRIS_307_T;

typedef struct { // 0x46cc
	UINT32	cs1_db_freq                     : 12,  // 11: 0
			cs1_dr_freq                     : 12,  // 23:12
			cs1_ispal_th                    : 8;   // 31:24
} L9A0_IRIS_308_T;

typedef struct { // 0x46d0
	UINT32	cs1_cagc_en                     : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs1_chroma_burst5or10           : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cs1_pal60_mode                  : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cs1_ntsc443_mode                : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			cs1_colour_mode                 : 3,   // 18:16
			b_rsvd5                         : 1,   //    19
			cs1_adaptive_mode               : 3,   // 22:20
			b_rsvd6                         : 1,   //    23
			cs1_secam_ybw                   : 2,   // 25:24
			b_rsvd7                         : 6;   // 31:26
} L9A0_IRIS_309_T;

typedef struct { // 0x46d4
	UINT32	cs1_cagc_coring_threshold       : 4,   //  3: 0
			cs1_cagc_coring                 : 3,   //  6: 4
			b_rsvd1                         : 1,   //     7
			cs1_cagc_unity_gain             : 1,   //     8
			b_rsvd2                         : 3,   // 11: 9
			cs1_cagc_tc_p                   : 2,   // 13:12
			b_rsvd3                         : 2,   // 15:14
			cs1_cagc_tc_ibig                : 3,   // 18:16
			b_rsvd4                         : 1,   //    19
			cs1_cagc_tc_ismall              : 3,   // 22:20
			b_rsvd5                         : 1,   //    23
			cs1_cagc                        : 8;   // 31:24
} L9A0_IRIS_310_T;

typedef struct { // 0x46d8
	UINT32	cs1_chroma_bw_lo                : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			cs1_ckill                       : 16,  // 19: 4
			cs1_hlock_ckill                 : 1,   //    20
			b_rsvd2                         : 3,   // 23:21
			cs1_vbi_ckill                   : 1,   //    24
			b_rsvd3                         : 3,   // 27:25
			cs1_noburst_ckill               : 1,   //    28
			b_rsvd4                         : 3;   // 31:29
} L9A0_IRIS_311_T;

typedef struct { // 0x46dc
	UINT32	cs1_cdto_inc                    : 32;  // 31: 0
} L9A0_IRIS_312_T;

typedef struct { // 0x46e0
	UINT32	cs1_cstate                      : 3,   //  2: 0
			b_rsvd1                         : 1,   //     3
			cs1_fixed_cstate                : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			cs1_user_ckill_mode             : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			cs1_lose_chromalock_mode        : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			cs1_lose_chromalock_count       : 4,   // 19:16
			cs1_lose_chromalock_level       : 3,   // 22:20
			b_rsvd5                         : 1,   //    23
			cs1_lose_chromalock_ckill       : 1,   //    24
			b_rsvd6                         : 3,   // 27:25
			cs1_palsw_level                 : 2,   // 29:28
			b_rsvd7                         : 2;   // 31:30
} L9A0_IRIS_313_T;

typedef struct { // 0x46e4
	UINT32	cs1_chroma_sel                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs1_cphase_adjust               : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			cs1_cphase_adjust_sel           : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			cs1_mv_colourstripes_sel        : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			cs1_cstripe_detect_control      : 3,   // 18:16
			b_rsvd5                         : 13;  // 31:19
} L9A0_IRIS_314_T;

typedef struct { // 0x46e8
	UINT32	cs1_secam_detected              : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs1_mv_colourstripes            : 3,   //  6: 4
			b_rsvd2                         : 1,   //     7
			cs1_status_cordic_freq          : 8,   // 15: 8
			cs1_status_burst_mag            : 16;  // 31:16
} L9A0_IRIS_315_T;

typedef struct { // 0x46ec
	UINT32	cs1_chromalock                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cs1_status_cgain                : 14,  // 17: 4
			b_rsvd2                         : 2,   // 19:18
			cs1_status_cmag                 : 8,   // 27:20
			cs1_pal_detected                : 1,   //    28
			b_rsvd3                         : 3;   // 31:29
} L9A0_IRIS_316_T;

typedef struct { // 0x46f0
	UINT32	estfrdp_ctrl                    : 5,   //  4: 0
			b_rsvd1                         : 3,   //  7: 5
			estfrdp_on                      : 1,   //     8
			b_rsvd2                         : 23;  // 31: 9
} L9A0_IRIS_317_T;

typedef struct { // 0x46f4
	UINT32	estfrdp_tgt_cf0cnt              : 20,  // 19: 0
			b_rsvd1                         : 12;  // 31:20
} L9A0_IRIS_318_T;

typedef struct { // 0x46f8
	UINT32	estfrdp_tgt_cf1cnt              : 20,  // 19: 0
			b_rsvd1                         : 12;  // 31:20
} L9A0_IRIS_319_T;

typedef struct { // 0x46fc
	UINT32	estfrdp_err_win1_u              : 8,   //  7: 0
			estfrdp_err_win1_l              : 8,   // 15: 8
			estfrdp_err_win0_u              : 8,   // 23:16
			estfrdp_err_win0_l              : 8;   // 31:24
} L9A0_IRIS_320_T;

typedef struct { // 0x4700
	UINT32	estfrdp_err_flag_th             : 8,   //  7: 0
			b_rsvd1                         : 24;  // 31: 8
} L9A0_IRIS_321_T;

typedef struct { // 0x4704
	UINT32	estfrdp_err                     : 8,   //  7: 0
			estfrdp_err_flag                : 1,   //     8
			b_rsvd1                         : 23;  // 31: 9
} L9A0_IRIS_322_T;

typedef struct { // 0x4708
	UINT32	vsync_width                     : 4,   //  3: 0
			b_rsvd1                         : 28;  // 31: 4
} L9A0_IRIS_323_T;

typedef struct { // 0x4800
	UINT32	wss625_rd_done                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cc_rd_done                      : 1,   //     4
			b_rsvd2                         : 27;  // 31: 5
} L9A0_VBI_001_T;

typedef struct { // 0x4804
	UINT32	cc_rdy                          : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			cc_data0                        : 8,   // 11: 4
			cc_data1                        : 8,   // 19:12
			b_rsvd2                         : 12;  // 31:20
} L9A0_VBI_002_T;

typedef struct { // 0x4808
	UINT32	wssdata0                        : 8,   //  7: 0
			wssdata1                        : 8,   // 15: 8
			wssdata2                        : 8,   // 23:16
			wss_rdy                         : 1,   //    24
			b_rsvd1                         : 7;   // 31:25
} L9A0_VBI_003_T;

typedef struct { // 0x480c
	UINT32	cc_short_start                  : 1,   //     0
			b_rsvd1                         : 3,   //  3: 1
			vbi_muxout                      : 1,   //     4
			b_rsvd2                         : 3,   //  7: 5
			vbi_hsyncout                    : 1,   //     8
			b_rsvd3                         : 3,   // 11: 9
			adap_slvl_en                    : 1,   //    12
			b_rsvd4                         : 3,   // 15:13
			vbi_st_err_ignored              : 1,   //    16
			b_rsvd5                         : 3,   // 19:17
			vbi_en                          : 1,   //    20
			b_rsvd6                         : 11;  // 31:21
} L9A0_VBI_004_T;

typedef struct { // 0x4810
	UINT32	vbi_lpf_bw                      : 2,   //  1: 0
			b_rsvd1                         : 30;  // 31: 2
} L9A0_VBI_005_T;

typedef struct { // 0x4814
	UINT32	vbi_data_hlvl                   : 8,   //  7: 0
			start_code                      : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_VBI_006_T;

typedef struct { // 0x4818
	UINT32	vbil9                           : 8,   //  7: 0
			vbil8                           : 8,   // 15: 8
			vbil7                           : 8,   // 23:16
			vbil6                           : 8;   // 31:24
} L9A0_VBI_007_T;

typedef struct { // 0x481c
	UINT32	vbil13                          : 8,   //  7: 0
			vbil12                          : 8,   // 15: 8
			vbil11                          : 8,   // 23:16
			vbil10                          : 8;   // 31:24
} L9A0_VBI_008_T;

typedef struct { // 0x4820
	UINT32	vbil17                          : 8,   //  7: 0
			vbil16                          : 8,   // 15: 8
			vbil15                          : 8,   // 23:16
			vbil14                          : 8;   // 31:24
} L9A0_VBI_009_T;

typedef struct { // 0x4824
	UINT32	vbil21                          : 8,   //  7: 0
			vbil20                          : 8,   // 15: 8
			vbil19                          : 8,   // 23:16
			vbil18                          : 8;   // 31:24
} L9A0_VBI_010_T;

typedef struct { // 0x4828
	UINT32	vbil25                          : 8,   //  7: 0
			vbil24                          : 8,   // 15: 8
			vbil23                          : 8,   // 23:16
			vbil22                          : 8;   // 31:24
} L9A0_VBI_011_T;

typedef struct { // 0x482c
	UINT32	vbil26                          : 8,   //  7: 0
			b_rsvd1                         : 24;  // 31: 8
} L9A0_VBI_012_T;

typedef struct { // 0x4830
	UINT32	vbi_vps_lpfil_fine_gain         : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			vbi_wss625_lpfil_fine_gain      : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			vbi_tele_lpfil_fine_gain        : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			vbi_cc_lpfil_fine_gain          : 2,   // 13:12
			b_rsvd4                         : 2,   // 15:14
			vbi_vps_slicer_mode             : 2,   // 17:16
			b_rsvd5                         : 2,   // 19:18
			vbi_wss_slicer_mode             : 2,   // 21:20
			b_rsvd6                         : 2,   // 23:22
			vbi_tt_slicer_mode              : 2,   // 25:24
			b_rsvd7                         : 2,   // 27:26
			vbi_cc_slicer_mode              : 2,   // 29:28
			b_rsvd8                         : 2;   // 31:30
} L9A0_VBI_013_T;

typedef struct { // 0x4834
	UINT32	teletext_dto                    : 16,  // 15: 0
			caption_dto                     : 16;  // 31:16
} L9A0_VBI_014_T;

typedef struct { // 0x4838
	UINT32	vps_dto                         : 16,  // 15: 0
			wss625_dto                      : 16;  // 31:16
} L9A0_VBI_015_T;

typedef struct { // 0x483c
	UINT32	vps_frame_start                 : 2,   //  1: 0
			b_rsvd1                         : 2,   //  3: 2
			teletext_frame_start            : 2,   //  5: 4
			b_rsvd2                         : 2,   //  7: 6
			wss625_frame_start              : 2,   //  9: 8
			b_rsvd3                         : 2,   // 11:10
			caption_frame_start             : 2,   // 13:12
			b_rsvd4                         : 18;  // 31:14
} L9A0_VBI_016_T;

typedef struct { // 0x4840
	UINT32	vbi_tele_lpfil_track_gain       : 4,   //  3: 0
			vbi_tele_lpfil_acq_gain         : 4,   //  7: 4
			vbi_cc_lpfil_track_gain         : 4,   // 11: 8
			vbi_cc_lpfil_acq_gain           : 4,   // 15:12
			wssj_delta_ampl                 : 8,   // 23:16
			b_rsvd1                         : 8;   // 31:24
} L9A0_VBI_017_T;

typedef struct { // 0x4844
	UINT32	vbi_vps_lpfil_track_gain        : 4,   //  3: 0
			vbi_vps_lpfil_acq_gain          : 4,   //  7: 4
			vbi_wss625_lpfil_track_gain     : 4,   // 11: 8
			vbi_wss625_lpfil_acq_gain       : 4,   // 15:12
			b_rsvd1                         : 16;  // 31:16
} L9A0_VBI_018_T;

typedef struct { // 0x4848
	UINT32	teletext_runin_accum_ampl       : 8,   //  7: 0
			caption_runin_accum_ampl        : 8,   // 15: 8
			b_rsvd1                         : 16;  // 31:16
} L9A0_VBI_019_T;

typedef struct { // 0x484c
	UINT32	vps_start                       : 8,   //  7: 0
			teletext_start                  : 8,   // 15: 8
			wss625_start                    : 8,   // 23:16
			caption_start                   : 8;   // 31:24
} L9A0_VBI_020_T;

typedef struct { // 0x4850
	UINT32	vbi_fixgate_en                  : 1,   //     0
			b_rsvd1                         : 31;  // 31: 1
} L9A0_VBI_021_T;

typedef struct { // 0x4854
	UINT32	exta_post_lpf_r_tap_0           : 16,  // 15: 0
			exta_post_lpf_bypass            : 1,   //    16
			b_rsvd1                         : 15;  // 31:17
} L9A0_POST_LPF_001_T;

typedef struct { // 0x4858
	UINT32	exta_post_lpf_r_tap_2           : 16,  // 15: 0
			exta_post_lpf_r_tap_1           : 16;  // 31:16
} L9A0_POST_LPF_002_T;

typedef struct { // 0x485c
	UINT32	exta_post_lpf_r_tap_4           : 16,  // 15: 0
			exta_post_lpf_r_tap_3           : 16;  // 31:16
} L9A0_POST_LPF_003_T;

typedef struct { // 0x4860
	UINT32	exta_post_lpf_r_tap_6           : 16,  // 15: 0
			exta_post_lpf_r_tap_5           : 16;  // 31:16
} L9A0_POST_LPF_004_T;

typedef struct { // 0x4870
	UINT32	exta_post_lpf_g_tap_0           : 16,  // 15: 0
			b_rsvd1                         : 16;  // 31:16
} L9A0_POST_LPF_008_T;

typedef struct { // 0x4874
	UINT32	exta_post_lpf_g_tap_2           : 16,  // 15: 0
			exta_post_lpf_g_tap_1           : 16;  // 31:16
} L9A0_POST_LPF_009_T;

typedef struct { // 0x4878
	UINT32	exta_post_lpf_g_tap_4           : 16,  // 15: 0
			exta_post_lpf_g_tap_3           : 16;  // 31:16
} L9A0_POST_LPF_010_T;

typedef struct { // 0x487c
	UINT32	exta_post_lpf_g_tap_6           : 16,  // 15: 0
			exta_post_lpf_g_tap_5           : 16;  // 31:16
} L9A0_POST_LPF_011_T;

typedef struct { // 0x488c
	UINT32	exta_post_lpf_b_tap_0           : 16,  // 15: 0
			b_rsvd1                         : 16;  // 31:16
} L9A0_POST_LPF_015_T;

typedef struct { // 0x4890
	UINT32	exta_post_lpf_b_tap_2           : 16,  // 15: 0
			exta_post_lpf_b_tap_1           : 16;  // 31:16
} L9A0_POST_LPF_016_T;

typedef struct { // 0x4894
	UINT32	exta_post_lpf_b_tap_4           : 16,  // 15: 0
			exta_post_lpf_b_tap_3           : 16;  // 31:16
} L9A0_POST_LPF_017_T;

typedef struct { // 0x4898
	UINT32	exta_post_lpf_b_tap_6           : 16,  // 15: 0
			exta_post_lpf_b_tap_5           : 16;  // 31:16
} L9A0_POST_LPF_018_T;

typedef struct { // 0x48a8
	UINT32	ro_rbuf2_pel_cnt                : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			ro_rbuf2_empty                  : 1,   //    12
			b_rsvd2                         : 3,   // 15:13
			ro_rbuf1_pel_cnt                : 11,  // 26:16
			b_rsvd3                         : 1,   //    27
			ro_rbuf1_empty                  : 1,   //    28
			b_rsvd4                         : 3;   // 31:29
} L9A0_IRIS_MIF_GMUA_MON_001_T;

typedef struct { // 0x48ac
	UINT32	ro_rbuf4_pel_cnt                : 11,  // 10: 0
			b_rsvd1                         : 1,   //    11
			ro_rbuf4_empty                  : 1,   //    12
			b_rsvd2                         : 3,   // 15:13
			ro_rbuf3_pel_cnt                : 11,  // 26:16
			b_rsvd3                         : 1,   //    27
			ro_rbuf3_empty                  : 1,   //    28
			b_rsvd4                         : 3;   // 31:29
} L9A0_IRIS_MIF_GMUA_MON_002_T;

typedef struct { // 0x48b0
	UINT32	ro_wbuf_word_cnt                : 6,   //  5: 0
			b_rsvd1                         : 2,   //  7: 6
			ro_wbuf_empty                   : 1,   //     8
			b_rsvd2                         : 3,   // 11: 9
			ro_wbuf_full                    : 1,   //    12
			b_rsvd3                         : 19;  // 31:13
} L9A0_IRIS_MIF_GMUA_MON_003_T;

typedef struct { // 0x48b4
	UINT32	mon_wait_max_cnt                : 12,  // 11: 0
			b_rsvd1                         : 20;  // 31:12
} L9A0_IRIS_MIF_GMUA_MON_004_T;

typedef struct { // 0x48b8
	UINT32	ro_mon_wr_wait_err              : 8,   //  7: 0
			b_rsvd1                         : 24;  // 31: 8
} L9A0_IRIS_MIF_GMUA_MON_005_T;

typedef struct { // 0x48bc
	UINT32	ro_mon_rd4_wait_err             : 8,   //  7: 0
			ro_mon_rd3_wait_err             : 8,   // 15: 8
			ro_mon_rd2_wait_err             : 8,   // 23:16
			ro_mon_rd1_wait_err             : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_006_T;

typedef struct { // 0x48c0
	UINT32	ro_mon_wr_req_num               : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			ro_mon_wr_req_que_cnt           : 3,   // 14:12
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_MIF_GMUA_MON_007_T;

typedef struct { // 0x48c4
	UINT32	ro_mon_wr_wait_cnt1             : 12,  // 11: 0
			ro_mon_wr_wait_cnt0             : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_008_T;

typedef struct { // 0x48c8
	UINT32	ro_mon_wr_wait_cnt3             : 12,  // 11: 0
			ro_mon_wr_wait_cnt2             : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_009_T;

typedef struct { // 0x48cc
	UINT32	ro_mon_rd1_req_num              : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			ro_mon_rd1_req_que_cnt          : 3,   // 14:12
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_MIF_GMUA_MON_010_T;

typedef struct { // 0x48d0
	UINT32	ro_mon_rd1_wait_cnt1            : 12,  // 11: 0
			ro_mon_rd1_wait_cnt0            : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_011_T;

typedef struct { // 0x48d4
	UINT32	ro_mon_rd1_wait_cnt3            : 12,  // 11: 0
			ro_mon_rd1_wait_cnt2            : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_012_T;

typedef struct { // 0x48d8
	UINT32	ro_mon_rd2_req_num              : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			ro_mon_rd2_req_que_cnt          : 3,   // 14:12
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_MIF_GMUA_MON_013_T;

typedef struct { // 0x48dc
	UINT32	ro_mon_rd2_wait_cnt1            : 12,  // 11: 0
			ro_mon_rd2_wait_cnt0            : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_014_T;

typedef struct { // 0x48e0
	UINT32	ro_mon_rd2_wait_cnt3            : 12,  // 11: 0
			ro_mon_rd2_wait_cnt2            : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_015_T;

typedef struct { // 0x48e4
	UINT32	ro_mon_rd3_req_num              : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			ro_mon_rd3_req_que_cnt          : 3,   // 14:12
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_MIF_GMUA_MON_016_T;

typedef struct { // 0x48e8
	UINT32	ro_mon_rd3_wait_cnt1            : 12,  // 11: 0
			ro_mon_rd3_wait_cnt0            : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_017_T;

typedef struct { // 0x48ec
	UINT32	ro_mon_rd3_wait_cnt3            : 12,  // 11: 0
			ro_mon_rd3_wait_cnt2            : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_018_T;

typedef struct { // 0x48f0
	UINT32	ro_mon_rd4_req_num              : 10,  //  9: 0
			b_rsvd1                         : 2,   // 11:10
			ro_mon_rd4_req_que_cnt          : 3,   // 14:12
			b_rsvd2                         : 17;  // 31:15
} L9A0_IRIS_MIF_GMUA_MON_019_T;

typedef struct { // 0x48f4
	UINT32	ro_mon_rd4_wait_cnt0            : 12,  // 11: 0
			b_rsvd1                         : 20;  // 31:12
} L9A0_IRIS_MIF_GMUA_MON_020_T;

typedef struct { // 0x48f8
	UINT32	ro_mon_rd4_wait_cnt1            : 12,  // 11: 0
			b_rsvd1                         : 20;  // 31:12
} L9A0_IRIS_MIF_GMUA_MON_021_T;

typedef struct { // 0x48fc
	UINT32	ro_mon_rd4_wait_cnt3            : 12,  // 11: 0
			ro_mon_rd4_wait_cnt2            : 12,  // 23:12
			b_rsvd1                         : 8;   // 31:24
} L9A0_IRIS_MIF_GMUA_MON_022_T;

typedef struct {
	L9A0_CVD_INTR1_ENABLE0_T        cvd_intr1_enable0;               // 0x4100
	L9A0_CVD_INTR1_INTR0_T          cvd_intr1_intr0;                 // 0x4104
	L9A0_CVD_INTR1_CLEAR0_T         cvd_intr1_clear0;                // 0x4108
	L9A0_CVD_INTR1_ENABLE1_T        cvd_intr1_enable1;               // 0x410c
	L9A0_CVD_INTR1_INTR1_T          cvd_intr1_intr1;                 // 0x4110
	L9A0_CVD_INTR1_CLEAR1_T         cvd_intr1_clear1;                // 0x4114
	L9A0_CVD_INTR2_ENABLE0_T        cvd_intr2_enable0;               // 0x4118
	L9A0_CVD_INTR2_INTR0_T          cvd_intr2_intr0;                 // 0x411c
	L9A0_CVD_INTR2_CLEAR0_T         cvd_intr2_clear0;                // 0x4120
	L9A0_CVD_INTR2_ENABLE1_T        cvd_intr2_enable1;               // 0x4124
	L9A0_CVD_INTR2_INTR1_T          cvd_intr2_intr1;                 // 0x4128
	L9A0_CVD_INTR2_CLEAR1_T         cvd_intr2_clear1;                // 0x412c
	L9A0_TOP_001_T                  top_001;                         // 0x4130
	L9A0_DITHERMUX_001_T            dithermux_001;                   // 0x4134
	UINT32                          reserved0[2];                    // 0x4138~0x413c
	L9A0_IRIS_MIF_GMUA_001_T        iris_mif_gmua_001;               // 0x4140
	L9A0_IRIS_MIF_GMUA_002_T        iris_mif_gmua_002;               // 0x4144
	L9A0_IRIS_MIF_GMUA_003_T        iris_mif_gmua_003;               // 0x4148
	L9A0_IRIS_MIF_GMUA_004_T        iris_mif_gmua_004;               // 0x414c
	L9A0_IRIS_MIF_GMUA_005_T        iris_mif_gmua_005;               // 0x4150
	L9A0_IRIS_MIF_GMUA_006_T        iris_mif_gmua_006;               // 0x4154
	L9A0_IRIS_MIF_GMUA_007_T        iris_mif_gmua_007;               // 0x4158
	UINT32							reserved0_0;
	L9A0_IRIS_MIF_GMUA_008_T        iris_mif_gmua_008;               // 0x415c
	L9A0_IRIS_MIF_GMUA_009_T        iris_mif_gmua_009;               // 0x4160
	L9A0_FASTBLANK_001_T            fastblank_001;                   // 0x4164
	L9A0_FASTBLANK_002_T            fastblank_002;                   // 0x4168
	L9A0_FASTBLANK_003_T            fastblank_003;                   // 0x416c
	L9A0_FASTBLANK_004_T            fastblank_004;                   // 0x4170
	L9A0_FASTBLANK_005_T            fastblank_005;                   // 0x4174
	L9A0_FASTBLANK_006_T            fastblank_006;                   // 0x4178
	L9A0_FASTBLANK_007_T            fastblank_007;                   // 0x417c
	L9A0_FASTBLANK_008_T            fastblank_008;                   // 0x4180
	L9A0_FASTBLANK_009_T            fastblank_009;                   // 0x4184
	UINT32							reserved2;                       // 0x4188
	L9A0_IRIS_DE_CTRL_001_T         iris_de_ctrl_001;                // 0x418c
	L9A0_VBI_CTRL_001_T             vbi_ctrl_001;                    // 0x4190
	L9A0_VBI_CTRL_002_T             vbi_ctrl_002;                    // 0x4194
	L9A0_VBI_CTRL_003_T             vbi_ctrl_003;                    // 0x4198
	L9A0_VBI_CTRL_004_T             vbi_ctrl_004;                    // 0x419c
	L9A0_VBI_CTRL_005_T             vbi_ctrl_005;                    // 0x41a0
	L9A0_VBI_CTRL_006_T             vbi_ctrl_006;                    // 0x41a4
	L9A0_VBI_CTRL_007_T             vbi_ctrl_007;                    // 0x41a8
	L9A0_VBI_CTRL_008_T             vbi_ctrl_008;                    // 0x41ac
	L9A0_VBI_CTRL_009_T             vbi_ctrl_009;                    // 0x41b0
	UINT32                          reserved3[2];                    // 0x41b4
	L9A0_FAST_BLANK_STATUS_001_T    fast_blank_status_001;           // 0x41b8~0x41bc
	L9A0_CVBSAFE_001_T              cvbsafe_001;                     // 0x41c0
	L9A0_CVBSAFE_002_T              cvbsafe_002;                     // 0x41c4
	UINT32                          reserved4[13];                   // 0x41c8
	L9A0_IRIS_001_T                 iris_001;                        // 0x41cc~0x41fc
	L9A0_IRIS_002_T                 iris_002;                        // 0x4200
	L9A0_IRIS_003_T                 iris_003;                        // 0x4204
	L9A0_IRIS_004_T                 iris_004;                        // 0x4208
	L9A0_IRIS_005_T                 iris_005;                        // 0x420c
	L9A0_IRIS_006_T                 iris_006;                        // 0x4210
	L9A0_IRIS_007_T                 iris_007;                        // 0x4214
	L9A0_IRIS_008_T                 iris_008;                        // 0x4218
	L9A0_IRIS_009_T                 iris_009;                        // 0x421c
	L9A0_IRIS_010_T                 iris_010;                        // 0x4220
	UINT32                          reserved5[2];                    // 0x4224
	L9A0_IRIS_013_T                 iris_013;                        // 0x4228~0x422c
	L9A0_IRIS_014_T                 iris_014;                        // 0x4230
	L9A0_IRIS_015_T                 iris_015;                        // 0x4234
	L9A0_IRIS_016_T                 iris_016;                        // 0x4238
	L9A0_IRIS_017_T                 iris_017;                        // 0x423c
	L9A0_IRIS_018_T                 iris_018;                        // 0x4240
	L9A0_IRIS_019_T                 iris_019;                        // 0x4244
	L9A0_IRIS_020_T                 iris_020;                        // 0x4248
	L9A0_IRIS_021_T                 iris_021;                        // 0x424c
	L9A0_IRIS_022_T                 iris_022;                        // 0x4250
	L9A0_IRIS_023_T                 iris_023;                        // 0x4254
	L9A0_IRIS_024_T                 iris_024;                        // 0x4258
	L9A0_IRIS_025_T                 iris_025;                        // 0x425c
	L9A0_IRIS_026_T                 iris_026;                        // 0x4260
	L9A0_IRIS_027_T                 iris_027;                        // 0x4264
	L9A0_IRIS_028_T                 iris_028;                        // 0x4268
	L9A0_IRIS_029_T                 iris_029;                        // 0x426c
	L9A0_IRIS_030_T                 iris_030;                        // 0x4270
	L9A0_IRIS_031_T                 iris_031;                        // 0x4274
	L9A0_IRIS_032_T                 iris_032;                        // 0x4278
	L9A0_IRIS_033_T                 iris_033;                        // 0x427c
	L9A0_IRIS_034_T                 iris_034;                        // 0x4280
	L9A0_IRIS_035_T                 iris_035;                        // 0x4284
	L9A0_IRIS_036_T                 iris_036;                        // 0x4288
	L9A0_IRIS_037_T                 iris_037;                        // 0x428c
	L9A0_IRIS_038_T                 iris_038;                        // 0x4290
	L9A0_IRIS_039_T                 iris_039;                        // 0x4294
	L9A0_IRIS_040_T                 iris_040;                        // 0x4298
	L9A0_IRIS_041_T                 iris_041;                        // 0x429c
	L9A0_IRIS_042_T                 iris_042;                        // 0x42a0
	L9A0_IRIS_043_T                 iris_043;                        // 0x42a4
	L9A0_IRIS_044_T                 iris_044;                        // 0x42a8
	L9A0_IRIS_045_T                 iris_045;                        // 0x42ac
	L9A0_IRIS_046_T                 iris_046;                        // 0x42b0
	L9A0_IRIS_047_T                 iris_047;                        // 0x42b4
	L9A0_IRIS_048_T                 iris_048;                        // 0x42b8
	L9A0_IRIS_049_T                 iris_049;                        // 0x42bc
	L9A0_IRIS_050_T                 iris_050;                        // 0x42c0
	L9A0_IRIS_051_T                 iris_051;                        // 0x42c4
	L9A0_IRIS_052_T                 iris_052;                        // 0x42c8
	L9A0_IRIS_053_T                 iris_053;                        // 0x42cc
	L9A0_IRIS_054_T                 iris_054;                        // 0x42d0
	L9A0_IRIS_055_T                 iris_055;                        // 0x42d4
	L9A0_IRIS_056_T                 iris_056;                        // 0x42d8
	L9A0_IRIS_057_T                 iris_057;                        // 0x42dc
	L9A0_IRIS_058_T                 iris_058;                        // 0x42e0
	L9A0_IRIS_059_T                 iris_059;                        // 0x42e4
	L9A0_IRIS_060_T                 iris_060;                        // 0x42e8
	L9A0_IRIS_061_T                 iris_061;                        // 0x42ec
	L9A0_IRIS_062_T                 iris_062;                        // 0x42f0
	L9A0_IRIS_063_T                 iris_063;                        // 0x42f4
	L9A0_IRIS_064_T                 iris_064;                        // 0x42f8
	L9A0_IRIS_065_T                 iris_065;                        // 0x42fc
	L9A0_IRIS_066_T                 iris_066;                        // 0x4300
	L9A0_IRIS_067_T                 iris_067;                        // 0x4304
	L9A0_IRIS_068_T                 iris_068;                        // 0x4308
	L9A0_IRIS_069_T                 iris_069;                        // 0x430c
	L9A0_IRIS_070_T                 iris_070;                        // 0x4310
	L9A0_IRIS_071_T                 iris_071;                        // 0x4314
	L9A0_IRIS_072_T                 iris_072;                        // 0x4318
	L9A0_IRIS_073_T                 iris_073;                        // 0x431c
	L9A0_IRIS_074_T                 iris_074;                        // 0x4320
	L9A0_IRIS_075_T                 iris_075;                        // 0x4324
	L9A0_IRIS_076_T                 iris_076;                        // 0x4328
	L9A0_IRIS_077_T                 iris_077;                        // 0x432c
	L9A0_IRIS_078_T                 iris_078;                        // 0x4330
	L9A0_IRIS_079_T                 iris_079;                        // 0x4334
	L9A0_IRIS_080_T                 iris_080;                        // 0x4338
	L9A0_IRIS_081_T                 iris_081;                        // 0x433c
	L9A0_IRIS_082_T                 iris_082;                        // 0x4340
	L9A0_IRIS_083_T                 iris_083;                        // 0x4344
	L9A0_IRIS_084_T                 iris_084;                        // 0x4348
	L9A0_IRIS_085_T                 iris_085;                        // 0x434c
	L9A0_IRIS_086_T                 iris_086;                        // 0x4350
	L9A0_IRIS_087_T                 iris_087;                        // 0x4354
	L9A0_IRIS_088_T                 iris_088;                        // 0x4358
	L9A0_IRIS_089_T                 iris_089;                        // 0x435c
	L9A0_IRIS_090_T                 iris_090;                        // 0x4360
	L9A0_IRIS_091_T                 iris_091;                        // 0x4364
	L9A0_IRIS_092_T                 iris_092;                        // 0x4368
	L9A0_IRIS_093_T                 iris_093;                        // 0x436c
	L9A0_IRIS_094_T                 iris_094;                        // 0x4370
	L9A0_IRIS_095_T                 iris_095;                        // 0x4374
	L9A0_IRIS_096_T                 iris_096;                        // 0x4378
	L9A0_IRIS_097_T                 iris_097;                        // 0x437c
	L9A0_IRIS_098_T                 iris_098;                        // 0x4380
	L9A0_IRIS_099_T                 iris_099;                        // 0x4384
	L9A0_IRIS_100_T                 iris_100;                        // 0x4388
	UINT32                          reserved6[18];                   // 0x438c
	L9A0_IRIS_119_T                 iris_119;                        // 0x4390~0x43d4
	L9A0_IRIS_120_T                 iris_120;                        // 0x43d8
    UINT32                          reserved7;                       // 0x43dc
	L9A0_IRIS_122_T                 iris_122;                        // 0x43e0
	UINT32                          reserved8[16];                   // 0x43e4
	L9A0_IRIS_139_T                 iris_139;                        // 0x43e8~0x4424
	L9A0_IRIS_140_T                 iris_140;                        // 0x4428
	L9A0_IRIS_141_T                 iris_141;                        // 0x442c
	L9A0_IRIS_142_T                 iris_142;                        // 0x4430
	L9A0_IRIS_143_T                 iris_143;                        // 0x4434
	L9A0_IRIS_144_T                 iris_144;                        // 0x4438
	L9A0_IRIS_145_T                 iris_145;                        // 0x443c
	L9A0_IRIS_146_T                 iris_146;                        // 0x4440
	L9A0_IRIS_147_T                 iris_147;                        // 0x4444
	L9A0_IRIS_148_T                 iris_148;                        // 0x4448
	L9A0_IRIS_149_T                 iris_149;                        // 0x444c
	L9A0_IRIS_150_T                 iris_150;                        // 0x4450
	L9A0_IRIS_151_T                 iris_151;                        // 0x4454
	L9A0_IRIS_152_T                 iris_152;                        // 0x4458
	L9A0_IRIS_153_T                 iris_153;                        // 0x445c
	L9A0_IRIS_154_T                 iris_154;                        // 0x4460
	L9A0_IRIS_155_T                 iris_155;                        // 0x4464
	L9A0_IRIS_156_T                 iris_156;                        // 0x4468
	L9A0_IRIS_157_T                 iris_157;                        // 0x446c
	L9A0_IRIS_158_T                 iris_158;                        // 0x4470
	L9A0_IRIS_159_T                 iris_159;                        // 0x4474
	L9A0_IRIS_160_T                 iris_160;                        // 0x4478
	L9A0_IRIS_161_T                 iris_161;                        // 0x447c
	L9A0_IRIS_162_T                 iris_162;                        // 0x4480
	L9A0_IRIS_163_T                 iris_163;                        // 0x4484
	L9A0_IRIS_164_T                 iris_164;                        // 0x4488
	L9A0_IRIS_165_T                 iris_165;                        // 0x448c
	L9A0_IRIS_166_T                 iris_166;                        // 0x4490
	L9A0_IRIS_167_T                 iris_167;                        // 0x4494
	L9A0_IRIS_168_T                 iris_168;                        // 0x4498
	L9A0_IRIS_169_T                 iris_169;                        // 0x449c
	L9A0_IRIS_170_T                 iris_170;                        // 0x44a0
	L9A0_IRIS_171_T                 iris_171;                        // 0x44a4
	L9A0_IRIS_172_T                 iris_172;                        // 0x44a8
	L9A0_IRIS_173_T                 iris_173;                        // 0x44ac
	L9A0_IRIS_174_T                 iris_174;                        // 0x44b0
	L9A0_IRIS_175_T                 iris_175;                        // 0x44b4
	L9A0_IRIS_176_T                 iris_176;                        // 0x44b8
	L9A0_IRIS_177_T                 iris_177;                        // 0x44bc
	L9A0_IRIS_178_T                 iris_178;                        // 0x44c0
	L9A0_IRIS_179_T                 iris_179;                        // 0x44c4
	L9A0_IRIS_180_T                 iris_180;                        // 0x44c8
	L9A0_IRIS_181_T                 iris_181;                        // 0x44cc
	L9A0_IRIS_182_T                 iris_182;                        // 0x44d0
	L9A0_IRIS_183_T                 iris_183;                        // 0x44d4
	L9A0_IRIS_184_T                 iris_184;                        // 0x44d8
	L9A0_IRIS_185_T                 iris_185;                        // 0x44dc
	L9A0_IRIS_186_T                 iris_186;                        // 0x44e0
	L9A0_IRIS_187_T                 iris_187;                        // 0x44e4
	L9A0_IRIS_188_T                 iris_188;                        // 0x44e8
	L9A0_IRIS_189_T                 iris_189;                        // 0x44ec
	L9A0_IRIS_190_T                 iris_190;                        // 0x44f0
	L9A0_IRIS_191_T                 iris_191;                        // 0x44f4
	L9A0_IRIS_192_T                 iris_192;                        // 0x44f8
	L9A0_IRIS_193_T                 iris_193;                        // 0x44fc
	L9A0_IRIS_194_T                 iris_194;                        // 0x4500
	L9A0_IRIS_195_T                 iris_195;                        // 0x4504
	L9A0_IRIS_196_T                 iris_196;                        // 0x4508
	L9A0_IRIS_197_T                 iris_197;                        // 0x450c
	L9A0_IRIS_198_T                 iris_198;                        // 0x4510
	L9A0_IRIS_199_T                 iris_199;                        // 0x4514
	L9A0_IRIS_200_T                 iris_200;                        // 0x4518
	L9A0_IRIS_201_T                 iris_201;                        // 0x451c
	L9A0_IRIS_202_T                 iris_202;                        // 0x4520
	L9A0_IRIS_203_T                 iris_203;                        // 0x4524
	L9A0_IRIS_204_T                 iris_204;                        // 0x4528
	L9A0_IRIS_205_T                 iris_205;                        // 0x452c
	L9A0_IRIS_206_T                 iris_206;                        // 0x4530
	L9A0_IRIS_207_T                 iris_207;                        // 0x4534
	L9A0_IRIS_208_T                 iris_208;                        // 0x4538
	L9A0_IRIS_209_T                 iris_209;                        // 0x453c
	L9A0_IRIS_210_T                 iris_210;                        // 0x4540
	L9A0_IRIS_211_T                 iris_211;                        // 0x4544
	L9A0_IRIS_212_T                 iris_212;                        // 0x4548
	L9A0_IRIS_213_T                 iris_213;                        // 0x454c
	L9A0_IRIS_214_T                 iris_214;                        // 0x4550
	L9A0_IRIS_215_T                 iris_215;                        // 0x4554
	L9A0_IRIS_216_T                 iris_216;                        // 0x4558
	L9A0_IRIS_217_T                 iris_217;                        // 0x455c
	L9A0_IRIS_218_T                 iris_218;                        // 0x4560
	L9A0_IRIS_219_T                 iris_219;                        // 0x4564
	L9A0_IRIS_220_T                 iris_220;                        // 0x4568
	L9A0_IRIS_221_T                 iris_221;                        // 0x456c
	L9A0_IRIS_222_T                 iris_222;                        // 0x4570
	L9A0_IRIS_223_T                 iris_223;                        // 0x4574
	L9A0_IRIS_224_T                 iris_224;                        // 0x4578
	L9A0_IRIS_225_T                 iris_225;                        // 0x457c
	L9A0_IRIS_226_T                 iris_226;                        // 0x4580
	L9A0_IRIS_227_T                 iris_227;                        // 0x4584
	L9A0_IRIS_228_T                 iris_228;                        // 0x4588
	L9A0_IRIS_229_T                 iris_229;                        // 0x458c
	L9A0_IRIS_230_T                 iris_230;                        // 0x4590
	L9A0_IRIS_231_T                 iris_231;                        // 0x4594
	L9A0_IRIS_232_T                 iris_232;                        // 0x4598
	L9A0_IRIS_233_T                 iris_233;                        // 0x459c
	L9A0_IRIS_234_T                 iris_234;                        // 0x45a0
	L9A0_IRIS_235_T                 iris_235;                        // 0x45a4
	L9A0_IRIS_236_T                 iris_236;                        // 0x45a8
	L9A0_IRIS_237_T                 iris_237;                        // 0x45ac
	L9A0_IRIS_238_T                 iris_238;                        // 0x45b0
	UINT32                          reserved9[3];                    // 0x45b4
	L9A0_IRIS_242_T                 iris_242;                        // 0x45b8~0x45c0
	L9A0_IRIS_243_T                 iris_243;                        // 0x45c4
	L9A0_IRIS_244_T                 iris_244;                        // 0x45c8
	L9A0_IRIS_245_T                 iris_245;                        // 0x45cc
	L9A0_IRIS_246_T                 iris_246;                        // 0x45d0
	L9A0_IRIS_247_T                 iris_247;                        // 0x45d4
	L9A0_IRIS_248_T                 iris_248;                        // 0x45d8
	L9A0_IRIS_249_T                 iris_249;                        // 0x45dc
	L9A0_IRIS_250_T                 iris_250;                        // 0x45e0
	L9A0_IRIS_251_T                 iris_251;                        // 0x45e4
	L9A0_IRIS_252_T                 iris_252;                        // 0x45e8
	L9A0_IRIS_253_T                 iris_253;                        // 0x45ec
	L9A0_IRIS_254_T                 iris_254;                        // 0x45f0
	L9A0_IRIS_255_T                 iris_255;                        // 0x45f4
	L9A0_IRIS_256_T                 iris_256;                        // 0x45f8
	L9A0_IRIS_257_T                 iris_257;                        // 0x45fc
	L9A0_IRIS_258_T                 iris_258;                        // 0x4600
	L9A0_IRIS_259_T                 iris_259;                        // 0x4604
	L9A0_IRIS_260_T                 iris_260;                        // 0x4608
	L9A0_IRIS_261_T                 iris_261;                        // 0x460c
	L9A0_IRIS_262_T                 iris_262;                        // 0x4610
	L9A0_IRIS_263_T                 iris_263;                        // 0x4614
	L9A0_IRIS_264_T                 iris_264;                        // 0x4618
	L9A0_IRIS_265_T                 iris_265;                        // 0x461c
	L9A0_IRIS_266_T                 iris_266;                        // 0x4620
	L9A0_IRIS_267_T                 iris_267;                        // 0x4624
	L9A0_IRIS_268_T                 iris_268;                        // 0x4628
	L9A0_IRIS_269_T                 iris_269;                        // 0x462c
	L9A0_IRIS_270_T                 iris_270;                        // 0x4630
	L9A0_IRIS_271_T                 iris_271;                        // 0x4634
	L9A0_IRIS_272_T                 iris_272;                        // 0x4638
	L9A0_IRIS_273_T                 iris_273;                        // 0x463c
	L9A0_IRIS_274_T                 iris_274;                        // 0x4640
	L9A0_IRIS_275_T                 iris_275;                        // 0x4644
	L9A0_IRIS_276_T                 iris_276;                        // 0x4648
	L9A0_IRIS_277_T                 iris_277;                        // 0x464c
	L9A0_IRIS_278_T                 iris_278;                        // 0x4650
	L9A0_IRIS_279_T                 iris_279;                        // 0x4654
	L9A0_IRIS_280_T                 iris_280;                        // 0x4658
	L9A0_IRIS_281_T                 iris_281;                        // 0x465c
	L9A0_IRIS_282_T                 iris_282;                        // 0x4660
	L9A0_IRIS_283_T                 iris_283;                        // 0x4664
	L9A0_IRIS_284_T                 iris_284;                        // 0x4668
	L9A0_IRIS_285_T                 iris_285;                        // 0x466c
	L9A0_IRIS_286_T                 iris_286;                        // 0x4670
	L9A0_IRIS_287_T                 iris_287;                        // 0x4674
	L9A0_IRIS_288_T                 iris_288;                        // 0x4678
	L9A0_IRIS_289_T                 iris_289;                        // 0x467c
	L9A0_IRIS_290_T                 iris_290;                        // 0x4680
	L9A0_IRIS_291_T                 iris_291;                        // 0x4684
	L9A0_IRIS_292_T                 iris_292;                        // 0x4688
	L9A0_IRIS_293_T                 iris_293;                        // 0x468c
	L9A0_IRIS_294_T                 iris_294;                        // 0x4690
	L9A0_IRIS_295_T                 iris_295;                        // 0x4694
	L9A0_IRIS_296_T                 iris_296;                        // 0x4698
	L9A0_IRIS_297_T                 iris_297;                        // 0x469c
	L9A0_IRIS_298_T                 iris_298;                        // 0x46a0
	L9A0_IRIS_299_T                 iris_299;                        // 0x46a4
	L9A0_IRIS_300_T                 iris_300;                        // 0x46a8
	L9A0_IRIS_301_T                 iris_301;                        // 0x46ac
	L9A0_IRIS_302_T                 iris_302;                        // 0x46b0
	L9A0_IRIS_303_T                 iris_303;                        // 0x46b4
	L9A0_IRIS_304_T                 iris_304;                        // 0x46b8
	L9A0_IRIS_305_T                 iris_305;                        // 0x46bc
	L9A0_IRIS_306_T                 iris_306;                        // 0x46c0
	L9A0_IRIS_307_T                 iris_307;                        // 0x46c4
	L9A0_IRIS_308_T                 iris_308;                        // 0x46c8
	L9A0_IRIS_309_T                 iris_309;                        // 0x46cc
	L9A0_IRIS_310_T                 iris_310;                        // 0x46d0
	L9A0_IRIS_311_T                 iris_311;                        // 0x46d4
	L9A0_IRIS_312_T                 iris_312;                        // 0x46d8
	L9A0_IRIS_313_T                 iris_313;                        // 0x46dc
	L9A0_IRIS_314_T                 iris_314;                        // 0x46e0
	L9A0_IRIS_315_T                 iris_315;                        // 0x46e4
	L9A0_IRIS_316_T                 iris_316;                        // 0x46e8
	L9A0_IRIS_317_T                 iris_317;                        // 0x46ec
	L9A0_IRIS_318_T                 iris_318;                        // 0x46f0
	L9A0_IRIS_319_T                 iris_319;                        // 0x46f4
	L9A0_IRIS_320_T                 iris_320;                        // 0x46f8
	L9A0_IRIS_321_T                 iris_321;                        // 0x46fc
	L9A0_IRIS_322_T                 iris_322;                        // 0x4700
	L9A0_IRIS_323_T                 iris_323;                        // 0x4704
	UINT32                          reserved10[61];                  // 0x4708
	L9A0_VBI_001_T                  vbi_001;                         // 0x470c~0x47fc
	L9A0_VBI_002_T                  vbi_002;                         // 0x4800
	L9A0_VBI_003_T                  vbi_003;                         // 0x4804
	L9A0_VBI_004_T                  vbi_004;                         // 0x4808
	L9A0_VBI_005_T                  vbi_005;                         // 0x480c
	L9A0_VBI_006_T                  vbi_006;                         // 0x4810
	L9A0_VBI_007_T                  vbi_007;                         // 0x4814
	L9A0_VBI_008_T                  vbi_008;                         // 0x4818
	L9A0_VBI_009_T                  vbi_009;                         // 0x481c
	L9A0_VBI_010_T                  vbi_010;                         // 0x4820
	L9A0_VBI_011_T                  vbi_011;                         // 0x4824
	L9A0_VBI_012_T                  vbi_012;                         // 0x4828
	L9A0_VBI_013_T                  vbi_013;                         // 0x482c
	L9A0_VBI_014_T                  vbi_014;                         // 0x4830
	L9A0_VBI_015_T                  vbi_015;                         // 0x4834
	L9A0_VBI_016_T                  vbi_016;                         // 0x4838
	L9A0_VBI_017_T                  vbi_017;                         // 0x483c
	L9A0_VBI_018_T                  vbi_018;                         // 0x4840
	L9A0_VBI_019_T                  vbi_019;                         // 0x4844
	L9A0_VBI_020_T                  vbi_020;                         // 0x4848
	L9A0_VBI_021_T                  vbi_021;                         // 0x484c
	L9A0_POST_LPF_001_T             post_lpf_001;                    // 0x4850
	L9A0_POST_LPF_002_T             post_lpf_002;                    // 0x4854
	L9A0_POST_LPF_003_T             post_lpf_003;                    // 0x4858
	L9A0_POST_LPF_004_T             post_lpf_004;                    // 0x485c
	UINT32                          reserved11[3];                   // 0x4860
	L9A0_POST_LPF_008_T             post_lpf_008;                    // 0x4864~0x486c
	L9A0_POST_LPF_009_T             post_lpf_009;                    // 0x4870
	L9A0_POST_LPF_010_T             post_lpf_010;                    // 0x4874
	L9A0_POST_LPF_011_T             post_lpf_011;                    // 0x4878
	UINT32                          reserved12[3];                   // 0x487c
	L9A0_POST_LPF_015_T             post_lpf_015;                    // 0x4880~0x4888
	L9A0_POST_LPF_016_T             post_lpf_016;                    // 0x488c
	L9A0_POST_LPF_017_T             post_lpf_017;                    // 0x4890
	L9A0_POST_LPF_018_T             post_lpf_018;                    // 0x4894
	UINT32                          reserved13[3];                   // 0x4898
	L9A0_IRIS_MIF_GMUA_MON_001_T    iris_mif_gmua_mon_001;           // 0x489c~0x48a4
	L9A0_IRIS_MIF_GMUA_MON_002_T    iris_mif_gmua_mon_002;           // 0x48a8
	L9A0_IRIS_MIF_GMUA_MON_003_T    iris_mif_gmua_mon_003;           // 0x48ac
	L9A0_IRIS_MIF_GMUA_MON_004_T    iris_mif_gmua_mon_004;           // 0x48b0
	L9A0_IRIS_MIF_GMUA_MON_005_T    iris_mif_gmua_mon_005;           // 0x48b4
	L9A0_IRIS_MIF_GMUA_MON_006_T    iris_mif_gmua_mon_006;           // 0x48b8
	L9A0_IRIS_MIF_GMUA_MON_007_T    iris_mif_gmua_mon_007;           // 0x48bc
	L9A0_IRIS_MIF_GMUA_MON_008_T    iris_mif_gmua_mon_008;           // 0x48c0
	L9A0_IRIS_MIF_GMUA_MON_009_T    iris_mif_gmua_mon_009;           // 0x48c4
	L9A0_IRIS_MIF_GMUA_MON_010_T    iris_mif_gmua_mon_010;           // 0x48c8
	L9A0_IRIS_MIF_GMUA_MON_011_T    iris_mif_gmua_mon_011;           // 0x48cc
	L9A0_IRIS_MIF_GMUA_MON_012_T    iris_mif_gmua_mon_012;           // 0x48d0
	L9A0_IRIS_MIF_GMUA_MON_013_T    iris_mif_gmua_mon_013;           // 0x48d4
	L9A0_IRIS_MIF_GMUA_MON_014_T    iris_mif_gmua_mon_014;           // 0x48d8
	L9A0_IRIS_MIF_GMUA_MON_015_T    iris_mif_gmua_mon_015;           // 0x48dc
	L9A0_IRIS_MIF_GMUA_MON_016_T    iris_mif_gmua_mon_016;           // 0x48e0
	L9A0_IRIS_MIF_GMUA_MON_017_T    iris_mif_gmua_mon_017;           // 0x48e4
	L9A0_IRIS_MIF_GMUA_MON_018_T    iris_mif_gmua_mon_018;           // 0x48e8
	L9A0_IRIS_MIF_GMUA_MON_019_T    iris_mif_gmua_mon_019;           // 0x48ec
	L9A0_IRIS_MIF_GMUA_MON_020_T    iris_mif_gmua_mon_020;           // 0x48f0
	L9A0_IRIS_MIF_GMUA_MON_021_T    iris_mif_gmua_mon_021;           // 0x48f4
	L9A0_IRIS_MIF_GMUA_MON_022_T    iris_mif_gmua_mon_022;           // 0x48f8
} VPORT_L9A0_REG_T;

/*
 * @{
 * Naming for register pointer.
 * gpRealRegVPORT_L9A0 : real register of VPORT_L9A0.
 * gpRegVPORT_L9A0     : shadow register.
 *
 * @def VPORT_L9A0_RdFL: Read  FLushing : Shadow <- Real.
 * @def VPORT_L9A0_WrFL: Write FLushing : Shadow -> Real.
 * @def VPORT_L9A0_Rd  : Read  whole register(UINT16) from Shadow register.
 * @def VPORT_L9A0_Wr  : Write whole register(UINT16) from Shadow register.
 * @def VPORT_L9A0_Rd01 ~ VPORT_L9A0_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VPORT_L9A0_Wr01 ~ VPORT_L9A0_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
//#define gpRealRegVPORT_L9A0 gVPORT_L9A0RegBx
#define VPORT_L9A0_RdFL(_r)			((gpRegVPORT_L9A0->_r)=(gpRealRegVPORT_L9A0->_r))
#define VPORT_L9A0_WrFL(_r)			((gpRealRegVPORT_L9A0->_r)=(gpRegVPORT_L9A0->_r))

#define VPORT_L9A0_Rd(_r)			*((UINT32*)(&(gpRegVPORT_L9A0->_r)))
#define VPORT_L9A0_Wr(_r,_v)			((VPORT_L9A0_Rd(_r))=((UINT32)(_v)))

#define VPORT_L9A0_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
								} while(0)

#define VPORT_L9A0_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
								} while(0)

#define VPORT_L9A0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
								} while(0)

#define VPORT_L9A0_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
								} while(0)

#define VPORT_L9A0_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
								} while(0)

#define VPORT_L9A0_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
								} while(0)

#define VPORT_L9A0_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
								} while(0)

#define VPORT_L9A0_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
								} while(0)

#define VPORT_L9A0_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
								} while(0)

#define VPORT_L9A0_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9A0->_r._f10);				\
								} while(0)

#define VPORT_L9A0_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9A0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9A0->_r._f11);				\
								} while(0)

#define VPORT_L9A0_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9A0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9A0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9A0->_r._f12);				\
								} while(0)

#define VPORT_L9A0_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9A0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9A0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9A0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9A0->_r._f13);				\
								} while(0)

#define VPORT_L9A0_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9A0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9A0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9A0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9A0->_r._f13);				\
									(_v14) = (gpRegVPORT_L9A0->_r._f14);				\
								} while(0)

#define VPORT_L9A0_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9A0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9A0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9A0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9A0->_r._f13);				\
									(_v14) = (gpRegVPORT_L9A0->_r._f14);				\
									(_v15) = (gpRegVPORT_L9A0->_r._f15);				\
								} while(0)

#define VPORT_L9A0_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9A0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9A0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9A0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9A0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9A0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9A0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9A0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9A0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9A0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9A0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9A0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9A0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9A0->_r._f13);				\
									(_v14) = (gpRegVPORT_L9A0->_r._f14);				\
									(_v15) = (gpRegVPORT_L9A0->_r._f15);				\
									(_v16) = (gpRegVPORT_L9A0->_r._f16);				\
								} while(0)


#define VPORT_L9A0_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
								} while(0)

#define VPORT_L9A0_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
								} while(0)

#define VPORT_L9A0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
								} while(0)

#define VPORT_L9A0_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
								} while(0)

#define VPORT_L9A0_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
								} while(0)

#define VPORT_L9A0_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
								} while(0)

#define VPORT_L9A0_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
								} while(0)

#define VPORT_L9A0_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
								} while(0)

#define VPORT_L9A0_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
								} while(0)

#define VPORT_L9A0_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9A0->_r._f10) = (_v10);				\
								} while(0)

#define VPORT_L9A0_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9A0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9A0->_r._f11) = (_v11);				\
								} while(0)

#define VPORT_L9A0_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9A0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9A0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9A0->_r._f12) = (_v12);				\
								} while(0)

#define VPORT_L9A0_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9A0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9A0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9A0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9A0->_r._f13) = (_v13);				\
								} while(0)

#define VPORT_L9A0_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9A0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9A0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9A0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9A0->_r._f13) = (_v13);				\
									(gpRegVPORT_L9A0->_r._f14) = (_v14);				\
								} while(0)

#define VPORT_L9A0_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9A0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9A0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9A0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9A0->_r._f13) = (_v13);				\
									(gpRegVPORT_L9A0->_r._f14) = (_v14);				\
									(gpRegVPORT_L9A0->_r._f15) = (_v15);				\
								} while(0)

#define VPORT_L9A0_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVPORT_L9A0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9A0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9A0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9A0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9A0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9A0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9A0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9A0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9A0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9A0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9A0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9A0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9A0->_r._f13) = (_v13);				\
									(gpRegVPORT_L9A0->_r._f14) = (_v14);				\
									(gpRegVPORT_L9A0->_r._f15) = (_v15);				\
									(gpRegVPORT_L9A0->_r._f16) = (_v16);				\
								} while(0)




/*
 * @{
 * Naming for register pointer.
 * gpRealRegVBI : real register of VBI.
 * gpRegVBI     : shadow register.
 *
 * @def VBI_RdFL: Read  FLushing : Shadow <- Real.
 * @def VBI_WrFL: Write FLushing : Shadow -> Real.
 * @def VBI_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def VBI_Wr  : Write whole register(UINT32) from Shadow register.
 * @def VBI_Rd01 ~ VBI_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VBI_Wr01 ~ VBI_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define VBI_RdFL(_r)			((gpRegVBI->_r)=(gpRealRegVBI->_r))
#define VBI_WrFL(_r)			((gpRealRegVBI->_r)=(gpRegVBI->_r))

#define VBI_Rd(_r)			*((UINT32*)(&(gpRegVBI->_r)))
#define VBI_Wr(_r,_v)			((VBI_Rd(_r))=((UINT32)(_v)))

#define VBI_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
								} while(0)

#define VBI_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
								} while(0)

#define VBI_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
								} while(0)

#define VBI_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
								} while(0)

#define VBI_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
								} while(0)

#define VBI_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
								} while(0)

#define VBI_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
								} while(0)

#define VBI_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
								} while(0)

#define VBI_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
								} while(0)

#define VBI_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
									(_v10) = (gpRegVBI->_r._f10);				\
								} while(0)

#define VBI_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
									(_v10) = (gpRegVBI->_r._f10);				\
									(_v11) = (gpRegVBI->_r._f11);				\
								} while(0)

#define VBI_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
									(_v10) = (gpRegVBI->_r._f10);				\
									(_v11) = (gpRegVBI->_r._f11);				\
									(_v12) = (gpRegVBI->_r._f12);				\
								} while(0)

#define VBI_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
									(_v10) = (gpRegVBI->_r._f10);				\
									(_v11) = (gpRegVBI->_r._f11);				\
									(_v12) = (gpRegVBI->_r._f12);				\
									(_v13) = (gpRegVBI->_r._f13);				\
								} while(0)

#define VBI_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
									(_v10) = (gpRegVBI->_r._f10);				\
									(_v11) = (gpRegVBI->_r._f11);				\
									(_v12) = (gpRegVBI->_r._f12);				\
									(_v13) = (gpRegVBI->_r._f13);				\
									(_v14) = (gpRegVBI->_r._f14);				\
								} while(0)

#define VBI_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
									(_v10) = (gpRegVBI->_r._f10);				\
									(_v11) = (gpRegVBI->_r._f11);				\
									(_v12) = (gpRegVBI->_r._f12);				\
									(_v13) = (gpRegVBI->_r._f13);				\
									(_v14) = (gpRegVBI->_r._f14);				\
									(_v15) = (gpRegVBI->_r._f15);				\
								} while(0)

#define VBI_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVBI->_r._f01);				\
									(_v02) = (gpRegVBI->_r._f02);				\
									(_v03) = (gpRegVBI->_r._f03);				\
									(_v04) = (gpRegVBI->_r._f04);				\
									(_v05) = (gpRegVBI->_r._f05);				\
									(_v06) = (gpRegVBI->_r._f06);				\
									(_v07) = (gpRegVBI->_r._f07);				\
									(_v08) = (gpRegVBI->_r._f08);				\
									(_v09) = (gpRegVBI->_r._f09);				\
									(_v10) = (gpRegVBI->_r._f10);				\
									(_v11) = (gpRegVBI->_r._f11);				\
									(_v12) = (gpRegVBI->_r._f12);				\
									(_v13) = (gpRegVBI->_r._f13);				\
									(_v14) = (gpRegVBI->_r._f14);				\
									(_v15) = (gpRegVBI->_r._f15);				\
									(_v16) = (gpRegVBI->_r._f16);				\
								} while(0)


#define VBI_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
								} while(0)

#define VBI_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
								} while(0)

#define VBI_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
								} while(0)

#define VBI_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
								} while(0)

#define VBI_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
								} while(0)

#define VBI_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
								} while(0)

#define VBI_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
								} while(0)

#define VBI_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
								} while(0)

#define VBI_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
								} while(0)

#define VBI_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
									(gpRegVBI->_r._f10) = (_v10);				\
								} while(0)

#define VBI_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
									(gpRegVBI->_r._f10) = (_v10);				\
									(gpRegVBI->_r._f11) = (_v11);				\
								} while(0)

#define VBI_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
									(gpRegVBI->_r._f10) = (_v10);				\
									(gpRegVBI->_r._f11) = (_v11);				\
									(gpRegVBI->_r._f12) = (_v12);				\
								} while(0)

#define VBI_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
									(gpRegVBI->_r._f10) = (_v10);				\
									(gpRegVBI->_r._f11) = (_v11);				\
									(gpRegVBI->_r._f12) = (_v12);				\
									(gpRegVBI->_r._f13) = (_v13);				\
								} while(0)

#define VBI_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
									(gpRegVBI->_r._f10) = (_v10);				\
									(gpRegVBI->_r._f11) = (_v11);				\
									(gpRegVBI->_r._f12) = (_v12);				\
									(gpRegVBI->_r._f13) = (_v13);				\
									(gpRegVBI->_r._f14) = (_v14);				\
								} while(0)

#define VBI_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
									(gpRegVBI->_r._f10) = (_v10);				\
									(gpRegVBI->_r._f11) = (_v11);				\
									(gpRegVBI->_r._f12) = (_v12);				\
									(gpRegVBI->_r._f13) = (_v13);				\
									(gpRegVBI->_r._f14) = (_v14);				\
									(gpRegVBI->_r._f15) = (_v15);				\
								} while(0)

#define VBI_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVBI->_r._f01) = (_v01);				\
									(gpRegVBI->_r._f02) = (_v02);				\
									(gpRegVBI->_r._f03) = (_v03);				\
									(gpRegVBI->_r._f04) = (_v04);				\
									(gpRegVBI->_r._f05) = (_v05);				\
									(gpRegVBI->_r._f06) = (_v06);				\
									(gpRegVBI->_r._f07) = (_v07);				\
									(gpRegVBI->_r._f08) = (_v08);				\
									(gpRegVBI->_r._f09) = (_v09);				\
									(gpRegVBI->_r._f10) = (_v10);				\
									(gpRegVBI->_r._f11) = (_v11);				\
									(gpRegVBI->_r._f12) = (_v12);				\
									(gpRegVBI->_r._f13) = (_v13);				\
									(gpRegVBI->_r._f14) = (_v14);				\
									(gpRegVBI->_r._f15) = (_v15);				\
									(gpRegVBI->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after VBI_Wind(), 1 for VBI_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * VBI_Rind : General indexed register Read.(
 * VBI_Wind : General indexed register Read.
 *
 * VBI_Ridx : For 'index', 'rw', 'load' field name
 * VBI_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define VBI_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								VBI_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								VBI_WrFL(_r);									\
								VBI_RdFL(_r);									\
								VBI_Rd01(_r,_fname,_fval);						\
							} while (0)

#define VBI_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				VBI_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define VBI_Ridx(_r, _ival, _fname, _fval)	VBI_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define VBI_Widx(_r, _ival, _fname, _fval)	VBI_Wind(_r,load,rw,index,_ival,_fname,_fval)

/** @} *//* end of macro documentation */

#ifdef __cplusplus
}
#endif

#endif	/* _VPORT_REG_L9A0_H_ */

