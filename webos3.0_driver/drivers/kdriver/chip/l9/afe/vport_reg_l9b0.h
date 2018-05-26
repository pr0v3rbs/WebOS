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
 *  #_VPORT_REG_L9B0_H_# register details. ( used only within kdriver )
 *
 *  author     won.hur (won.hur@lge.com)
 *  version    1.0
 *  date       2011.08.19
 *
 */

#ifndef _VPORT_REG_L9B0_H_
#define _VPORT_REG_L9B0_H_

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



/*-----------------------------------------------------------------------------
	0x0000 cvd_intr1_enable0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr1_enable0               ;   	// 31: 0
} L9B0_CVD_INTR1_ENABLE0_T;

/*-----------------------------------------------------------------------------
	0x0004 cvd_intr1_intr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_chromalock_fal              : 1,	//     0
	cvd_chromalock_ris              : 1,	//     1
	cvd_vlock_fal                   : 1,	//     2
	cvd_vlock_ris                   : 1,	//     3
	cvd_hlock_fal                   : 1,	//     4
	cvd_hlock_ris                   : 1,	//     5
	cvd_no_signal_fal               : 1,	//     6
	cvd_no_signal_ris               : 1,	//     7
	cvd_noburst_detected_fal        : 1,	//     8
	cvd_noburst_detected_ris        : 1,	//     9
	cvd_vnon_standard_fal           : 1,	//    10
	cvd_vnon_standard_ris           : 1,	//    11
	cvd_hnon_standard_fal           : 1,	//    12
	cvd_hnon_standard_ris           : 1,	//    13
	cvd_proscan_detected_fal        : 1,	//    14
	cvd_proscan_detected_ris        : 1,	//    15
	cvd_vcr_fal                     : 1,	//    16
	cvd_vcr_ris                     : 1,	//    17
	cvd_noisy_fal                   : 1,	//    18
	cvd_noisy_ris                   : 1,	//    19
	cvd_vline_625_detected_fal      : 1,	//    20
	cvd_vline_625_detected_ris      : 1,	//    21
	cvd_secam_detected_fal          : 1,	//    22
	cvd_secam_detected_ris          : 1,	//    23
	cvd_pal_detected_fal            : 1,	//    24
	cvd_pal_detected_ris            : 1,	//    25
	cvd_vs_fal                      : 1,	//    26
	cvd_vs_ris                      : 1,	//    27
	cvd_ext_locked_fal              : 1,	//    28
	cvd_ext_locked_ris              : 1,	//    29
			                        : 1,	//    30
			                        : 1;	//    31
} L9B0_CVD_INTR1_INTR0_T;

/*-----------------------------------------------------------------------------
	0x0008 cvd_intr1_clear0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr1_clear0                ;   	// 31: 0
} L9B0_CVD_INTR1_CLEAR0_T;

/*-----------------------------------------------------------------------------
	0x000c cvd_intr1_enable1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr1_enable1               ;   	// 31: 0
} L9B0_CVD_INTR1_ENABLE1_T;

/*-----------------------------------------------------------------------------
	0x0010 cvd_intr1_intr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vbi_wss_rdy                     : 1,	//     0
	vbi_cc_rdy                      : 1,	//     1
	vbi_data_in                     : 1,	//     2
	cvd_fb_rst                      : 1,	//     3
	cvd_mif_err                     : 1,	//     4
	cs_chromalock_fal               : 1,	//     5
	cs_chromalock_ris               : 1,	//     6
	cs_pal_detected_fal             : 1,	//     7
	cs_pal_detected_ris             : 1,	//     8
	cs_secam_detected_fal           : 1,	//     9
	cs_secam_detected_ris           : 1,	//    10
	cs1_chromalock_fal              : 1,	//    11
	cs1_chromalock_ris              : 1,	//    12
	cs1_pal_detected_fal            : 1,	//    13
	cs1_pal_detected_ris            : 1,	//    14
	cs1_secam_detected_fal          : 1,	//    15
	cs1_secam_detected_ris          : 1,	//    16
	estfrdp_err_flag_fal            : 1,	//    17
	estfrdp_err_flag_ris            : 1,	//    18
	vdetect_vcount_fal              : 1,	//    19
	vdetect_vcount_ris              : 1,	//    20
			                        : 1,	//    21
	                         		: 1,	//    22
	                         		: 1,	//    23
	                         		: 1,	//    24
	                         		: 1,	//    25
	                         		: 1,	//    26
	                         		: 1,	//    27
	                         		: 1,	//    28
	                         		: 1,	//    29
	                         		: 1,	//    30
	                         		: 1;	//    31
} L9B0_CVD_INTR1_INTR1_T;

/*-----------------------------------------------------------------------------
	0x0014 cvd_intr1_clear1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr1_clear1                ;   	// 31: 0
} L9B0_CVD_INTR1_CLEAR1_T;

/*-----------------------------------------------------------------------------
	0x0018 cvd_intr2_enable0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr2_enable0               ;   	// 31: 0
} L9B0_CVD_INTR2_ENABLE0_T;

/*-----------------------------------------------------------------------------
	0x001c cvd_intr2_intr0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_chromalock_fal              : 1,	//     0
	cvd_chromalock_ris              : 1,	//     1
	cvd_vlock_fal                   : 1,	//     2
	cvd_vlock_ris                   : 1,	//     3
	cvd_hlock_fal                   : 1,	//     4
	cvd_hlock_ris                   : 1,	//     5
	cvd_no_signal_fal               : 1,	//     6
	cvd_no_signal_ris               : 1,	//     7
	cvd_noburst_detected_fal        : 1,	//     8
	cvd_noburst_detected_ris        : 1,	//     9
	cvd_vnon_standard_fal           : 1,	//    10
	cvd_vnon_standard_ris           : 1,	//    11
	cvd_hnon_standard_fal           : 1,	//    12
	cvd_hnon_standard_ris           : 1,	//    13
	cvd_proscan_detected_fal        : 1,	//    14
	cvd_proscan_detected_ris        : 1,	//    15
	cvd_vcr_fal                     : 1,	//    16
	cvd_vcr_ris                     : 1,	//    17
	cvd_noisy_fal                   : 1,	//    18
	cvd_noisy_ris                   : 1,	//    19
	cvd_vline_625_detected_fal      : 1,	//    20
	cvd_vline_625_detected_ris      : 1,	//    21
	cvd_secam_detected_fal          : 1,	//    22
	cvd_secam_detected_ris          : 1,	//    23
	cvd_pal_detected_fal            : 1,	//    24
	cvd_pal_detected_ris            : 1,	//    25
	cvd_vs_fal                      : 1,	//    26
	cvd_vs_ris                      : 1,	//    27
	cvd_ext_locked_fal              : 1,	//    28
	cvd_ext_locked_ris              : 1,	//    29
			                        : 1,	//    30
			                        : 1;	//    31
} L9B0_CVD_INTR2_INTR0_T;

/*-----------------------------------------------------------------------------
	0x0020 cvd_intr2_clear0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr2_clear0                ;   	// 31: 0
} L9B0_CVD_INTR2_CLEAR0_T;

/*-----------------------------------------------------------------------------
	0x0024 cvd_intr2_enable1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr2_enable1               ;   	// 31: 0
} L9B0_CVD_INTR2_ENABLE1_T;

/*-----------------------------------------------------------------------------
	0x0028 cvd_intr2_intr1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vbi_wss_rdy                     : 1,	//     0
	vbi_cc_rdy                      : 1,	//     1
	vbi_data_in                     : 1,	//     2
	cvd_fb_rst                      : 1,	//     3
	cvd_mif_err                     : 1,	//     4
	cs_chromalock_fal               : 1,	//     5
	cs_chromalock_ris               : 1,	//     6
	cs_pal_detected_fal             : 1,	//     7
	cs_pal_detected_ris             : 1,	//     8
	cs_secam_detected_fal           : 1,	//     9
	cs_secam_detected_ris           : 1,	//    10
	cs1_chromalock_fal              : 1,	//    11
	cs1_chromalock_ris              : 1,	//    12
	cs1_pal_detected_fal            : 1,	//    13
	cs1_pal_detected_ris            : 1,	//    14
	cs1_secam_detected_fal          : 1,	//    15
	cs1_secam_detected_ris          : 1,	//    16
	estfrdp_err_flag_fal            : 1,	//    17
	estfrdp_err_flag_ris            : 1,	//    18
	vdetect_vcount_fal              : 1,	//    19
	vdetect_vcount_ris              : 1,	//    20
			                        : 1,	//    21
			                        : 1,	//    22
			                        : 1,	//    23
			                        : 1,	//    24
			                        : 1,	//    25
			                        : 1,	//    26
			                        : 1,	//    27
			                        : 1,	//    28
			                        : 1,	//    29
			                        : 1,	//    30
			                        : 1;	//    31
} L9B0_CVD_INTR2_INTR1_T;

/*-----------------------------------------------------------------------------
	0x002c cvd_intr2_clear1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	cvd_intr2_clear1                ;   	// 31: 0
} L9B0_CVD_INTR2_CLEAR1_T;

/*-----------------------------------------------------------------------------
	0x0030 top_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	iris_ver                        ;   	// 31: 0
} L9B0_TOP_001_T;

/*-----------------------------------------------------------------------------
	0x0040 iris_mif_gmua_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_comb3_buffer_size           :23;	//  0:22
} L9B0_IRIS_MIF_GMUA_001_T;

/*-----------------------------------------------------------------------------
	0x0044 iris_mif_gmua_002 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fld1_init_rd_pel            :23;	//  0:22
} L9B0_IRIS_MIF_GMUA_002_T;

/*-----------------------------------------------------------------------------
	0x0048 iris_mif_gmua_003 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fld2_init_rd_pel            :23;	//  0:22
} L9B0_IRIS_MIF_GMUA_003_T;

/*-----------------------------------------------------------------------------
	0x004c iris_mif_gmua_004 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fld3_init_rd_pel            :23;	//  0:22
} L9B0_IRIS_MIF_GMUA_004_T;

/*-----------------------------------------------------------------------------
	0x0050 iris_mif_gmua_005 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fld4_init_rd_pel            :23;	//  0:22
} L9B0_IRIS_MIF_GMUA_005_T;

/*-----------------------------------------------------------------------------
	0x0054 iris_mif_gmua_006 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gmau_to_cnt                 :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	reg_gmau_cmd_dly_cnt            : 4;	// 12:15
} L9B0_IRIS_MIF_GMUA_006_T;

/*-----------------------------------------------------------------------------
	0x0058 iris_mif_gmua_007 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gmau_cmd_base               ;   	// 31: 0
} L9B0_IRIS_MIF_GMUA_007_T;

/*-----------------------------------------------------------------------------
	0x0060 iris_mif_gmua_008 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gmau_endian_sw              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_gmau_cmd_pri2               : 4,	//  4: 7
	reg_gmau_cmd_pri1               : 4;	//  8:11
} L9B0_IRIS_MIF_GMUA_008_T;

/*-----------------------------------------------------------------------------
	0x0064 iris_mif_gmua_009 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_gmau_stride_size            ;   	// 31: 0
} L9B0_IRIS_MIF_GMUA_009_T;

/*-----------------------------------------------------------------------------
	0x0068 fastblank_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_latency                  : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_fb3ch_delay                 : 5,	//  8:12
	                                : 3,	// 13:15 reserved
	reg_fb_blend_ratio              : 5,	// 16:20
	                                : 3,	// 21:23 reserved
	reg_fb_2line_delay              : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_fb_en                       : 1;	//    28
} L9B0_FASTBLANK_001_T;

/*-----------------------------------------------------------------------------
	0x006c fastblank_002 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_csc_coef1                :15,	//  0:14
	                                : 1,	//    15 reserved
	reg_fb_csc_coef0                :15;	// 16:30
} L9B0_FASTBLANK_002_T;

/*-----------------------------------------------------------------------------
	0x0070 fastblank_003 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_csc_coef3                :15,	//  0:14
	                                : 1,	//    15 reserved
	reg_fb_csc_coef2                :15;	// 16:30
} L9B0_FASTBLANK_003_T;

/*-----------------------------------------------------------------------------
	0x0074 fastblank_004 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_csc_coef5                :15,	//  0:14
	                                : 1,	//    15 reserved
	reg_fb_csc_coef4                :15;	// 16:30
} L9B0_FASTBLANK_004_T;

/*-----------------------------------------------------------------------------
	0x0078 fastblank_005 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_csc_coef7                :15,	//  0:14
	                                : 1,	//    15 reserved
	reg_fb_csc_coef6                :15;	// 16:30
} L9B0_FASTBLANK_005_T;

/*-----------------------------------------------------------------------------
	0x007c fastblank_006 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_csc_coef8                :15;	//  0:14
} L9B0_FASTBLANK_006_T;

/*-----------------------------------------------------------------------------
	0x0080 fastblank_007 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_csc_ofst1                :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_fb_csc_ofst0                :11;	// 12:22
} L9B0_FASTBLANK_007_T;

/*-----------------------------------------------------------------------------
	0x0084 fastblank_008 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_csc_ofst3                :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_fb_csc_ofst2                :11;	// 12:22
} L9B0_FASTBLANK_008_T;

/*-----------------------------------------------------------------------------
	0x0088 fastblank_009 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_vstart_odd               : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	reg_fb_csc_ofst5                :11,	//  8:18
	                                : 1,	//    19 reserved
	reg_fb_csc_ofst4                :11;	// 20:30
} L9B0_FASTBLANK_009_T;

/*-----------------------------------------------------------------------------
	0x008c fastblank_010 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_hstart                   : 8,	//  0: 7
	reg_fb_height_half              : 8,	//  8:15
	reg_fb_vstart_even              : 5;	// 16:20
} L9B0_FASTBLANK_010_T;

/*-----------------------------------------------------------------------------
	0x0090 iris_de_ctrl_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_iris_hsout_width            :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	reg_iris_static_de_start        :12,	// 12:23
	reg_iris_static_de              : 1;	//    24
} L9B0_IRIS_DE_CTRL_001_T;

/*-----------------------------------------------------------------------------
	0x0094 vbi_ctrl_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_little_endian           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_vbi_bits_swap               : 1;	//     4
} L9B0_VBI_CTRL_001_T;

/*-----------------------------------------------------------------------------
	0x0098 vbi_ctrl_002 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi0_start_addr             :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	reg_vbi0_data_cnt               :12,	// 12:23
	reg_vbi_buffer_number           : 2;	// 24:25
} L9B0_VBI_CTRL_002_T;

/*-----------------------------------------------------------------------------
	0x009c vbi_ctrl_003 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi0_end_addr               :10;	//  0: 9
} L9B0_VBI_CTRL_003_T;

/*-----------------------------------------------------------------------------
	0x00a0 vbi_ctrl_004 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi1_start_addr             :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	reg_vbi1_data_cnt               :12;	// 12:23
} L9B0_VBI_CTRL_004_T;

/*-----------------------------------------------------------------------------
	0x00a4 vbi_ctrl_005 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi1_end_addr               :10;	//  0: 9
} L9B0_VBI_CTRL_005_T;

/*-----------------------------------------------------------------------------
	0x00a8 vbi_ctrl_006 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi2_start_addr             :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	reg_vbi2_data_cnt               :12;	// 12:23
} L9B0_VBI_CTRL_006_T;

/*-----------------------------------------------------------------------------
	0x00ac vbi_ctrl_007 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi2_end_addr               :10;	//  0: 9
} L9B0_VBI_CTRL_007_T;

/*-----------------------------------------------------------------------------
	0x00b0 vbi_ctrl_008 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi3_start_addr             :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	reg_vbi3_data_cnt               :12;	// 12:23
} L9B0_VBI_CTRL_008_T;

/*-----------------------------------------------------------------------------
	0x00b4 vbi_ctrl_009 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi3_end_addr               :10;	//  0: 9
} L9B0_VBI_CTRL_009_T;

/*-----------------------------------------------------------------------------
	0x00c0 fast_blank_status_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_fb_state                    : 1;	//     0
} L9B0_FAST_BLANK_STATUS_001_T;

/*-----------------------------------------------------------------------------
	0x00c4 cvbsafe_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cvbs2_gc_int                : 4,	//  0: 3
	reg_cvbs1_gc_int                : 4,	//  4: 7
	reg_iris_fix_gain               : 1;	//     8
} L9B0_CVBSAFE_001_T;

/*-----------------------------------------------------------------------------
	0x0100 iris_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_proscan_detected            : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_mv_colourstripes            : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_mv_vbi_detected             : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_chromalock                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_vlock                       : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_hlock                       : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_no_signal                   : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_status_vdetect_vcount_chg   : 1;	//    28
} L9B0_IRIS_001_T;

/*-----------------------------------------------------------------------------
	0x0104 iris_002 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_noisy                       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_vline_625_detected          : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_secam_detected              : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_pal_detected                : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_status_comb3d_off           : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_noburst_detected            : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_vnon_standard               : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_hnon_standard               : 1;	//    28
} L9B0_IRIS_002_T;

/*-----------------------------------------------------------------------------
	0x0108 iris_003 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vcr_rew                     : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_vcr_ff                      : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_vcr_trick                   : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_vcr                         : 1;	//    12
} L9B0_IRIS_003_T;

/*-----------------------------------------------------------------------------
	0x010c iris_004 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_burst_mag            :16,	//  0:15
	reg_status_hsync_width          : 8;	// 16:23
} L9B0_IRIS_004_T;

/*-----------------------------------------------------------------------------
	0x0110 iris_005 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_hdto_inc             :30;	//  0:29
} L9B0_IRIS_005_T;

/*-----------------------------------------------------------------------------
	0x0114 iris_006 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_cdto_inc             :30;	//  0:29
} L9B0_IRIS_006_T;

/*-----------------------------------------------------------------------------
	0x0118 iris_007 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_cmag                 : 8,	//  0: 7
	reg_status_agc_gain             :16;	//  8:23
} L9B0_IRIS_007_T;

/*-----------------------------------------------------------------------------
	0x011c iris_008 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_sync_height          : 8,	//  0: 7
	reg_status_cordic_freq          : 8,	//  8:15
	reg_status_cgain                :14;	// 16:29
} L9B0_IRIS_008_T;

/*-----------------------------------------------------------------------------
	0x0120 iris_009 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_nstd                 :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_status_vbi_slicer_lvl       : 8,	// 12:19
	reg_status_noise                :10;	// 20:29
} L9B0_IRIS_009_T;

/*-----------------------------------------------------------------------------
	0x0124 iris_010 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_comb3d_motion        ;   	// 31: 0
} L9B0_IRIS_010_T;

/*-----------------------------------------------------------------------------
	0x0130 iris_011 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cvd_soft_reset              : 1;	//     0
} L9B0_IRIS_011_T;

/*-----------------------------------------------------------------------------
	0x0134 iris_012 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hv_delay                    : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_hpixel                      : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_vline_625                   : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_colour_mode                 : 3,	// 12:14
	                                : 1,	//    15 reserved
	reg_yc_src                      : 1;	//    16
} L9B0_IRIS_012_T;

/*-----------------------------------------------------------------------------
	0x0138 iris_013 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cv_inv                      : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cv_src                      : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_luma_notch_bw               : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_chroma_bw_lo                : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_chroma_burst5or10           : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_ped                         : 1;	//    20
} L9B0_IRIS_013_T;

/*-----------------------------------------------------------------------------
	0x013c iris_014 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hagc_field_mode             : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_mv_hagc_mode                : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_dc_clamp_mode               : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_dagc_en                     : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_agc_half_en                 : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_cagc_en                     : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_hagc_en                     : 1;	//    24
} L9B0_IRIS_014_T;

/*-----------------------------------------------------------------------------
	0x0140 iris_015 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pal60_mode                  : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_ntsc443_mode                : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_adaptive_3dcomb_mode        : 3,	//  8:10
	                                : 1,	//    11 reserved
	reg_colour_trap                 : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_adaptive_mode               : 3;	// 16:18
} L9B0_IRIS_015_T;

/*-----------------------------------------------------------------------------
	0x0144 iris_016 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_noise_thresh                : 8,	//  0: 7
	reg_hagc                        : 8;	//  8:15
} L9B0_IRIS_016_T;

/*-----------------------------------------------------------------------------
	0x0148 iris_017 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_force_vcr_en                : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_force_vcr_rew               : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_force_vcr_ff                : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_force_vcr_trick             : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_force_vcr                   : 1;	//    16
} L9B0_IRIS_017_T;

/*-----------------------------------------------------------------------------
	0x014c iris_018 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_adc_updn_swap               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_adc_input_swap              : 1;	//     4
} L9B0_IRIS_018_T;

/*-----------------------------------------------------------------------------
	0x0150 iris_019 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ccir656_en                  : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cbcr_swap                   : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_blue_mode                   : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_yc_delay                    : 4;	// 12:15
} L9B0_IRIS_019_T;

/*-----------------------------------------------------------------------------
	0x0154 iris_020 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hue                         : 8,	//  0: 7
	reg_saturation                  : 8,	//  8:15
	reg_brightness                  : 8,	// 16:23
	reg_contrast                    : 8;	// 24:31
} L9B0_IRIS_020_T;

/*-----------------------------------------------------------------------------
	0x0158 iris_021 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cagc                        : 8;	//  0: 7
} L9B0_IRIS_021_T;

/*-----------------------------------------------------------------------------
	0x015c iris_022 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_user_ckill_mode             : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_vbi_ckill                   : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_hlock_ckill                 : 1;	//     8
} L9B0_IRIS_022_T;

/*-----------------------------------------------------------------------------
	0x0160 iris_023 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_nstd_hysis                  : 4,	//  0: 3
	reg_disable_clamp_on_vsync      : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_bypass                      : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_noburst_ckill               : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_vnon_std_threshold          : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_hnon_std_threshold          : 6;	// 20:25
} L9B0_IRIS_023_T;

/*-----------------------------------------------------------------------------
	0x0164 iris_024 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_agc_peak_en                 : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_agc_peak_cntl               : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_agc_peak_nominal            : 7;	//  8:14
} L9B0_IRIS_024_T;

/*-----------------------------------------------------------------------------
	0x0168 iris_025 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_blue_cr                     : 8,	//  0: 7
	reg_blue_cb                     : 8,	//  8:15
	reg_blue_y                      : 8;	// 16:23
} L9B0_IRIS_025_T;

/*-----------------------------------------------------------------------------
	0x016c iris_026 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hstate_fixed                : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_disable_hfine               : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_hstate_unlocked             : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_hstate_max                  : 3,	// 12:14
	                                : 1,	//    15 reserved
	reg_locked_count_noisy_max      : 4,	// 16:19
	reg_locked_count_clean_max      : 4,	// 20:23
	reg_hdetect_clamp_level         : 8;	// 24:31
} L9B0_IRIS_026_T;

/*-----------------------------------------------------------------------------
	0x0170 iris_027 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hlock_vsync_mode            : 2;	//  0: 1
} L9B0_IRIS_027_T;

/*-----------------------------------------------------------------------------
	0x0174 iris_028 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdto_inc                    ;   	// 31: 0
} L9B0_IRIS_028_T;

/*-----------------------------------------------------------------------------
	0x0178 iris_029 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hdto_inc                    ;   	// 31: 0
} L9B0_IRIS_029_T;

/*-----------------------------------------------------------------------------
	0x017c iris_030 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hsync_gate_end              : 8,	//  0: 7
	reg_hsync_gate_start            : 8,	//  8:15
	reg_hsync_phase_offset          : 8,	// 16:23
	reg_hsync_rising                : 8;	// 24:31
} L9B0_IRIS_030_T;

/*-----------------------------------------------------------------------------
	0x0180 iris_031 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hsync_similar               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_hsync_low                   : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_hdetect_noise_en            : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_hfine_lt_hcoarse            : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_hlpf_clamp_sel              : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_hlpf_clamp_noisy_en         : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_hlpf_clamp_vbi_en           : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_hlpf_clamp_en               : 1;	//    28
} L9B0_IRIS_031_T;

/*-----------------------------------------------------------------------------
	0x0184 iris_032 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hsync_rising_end            : 8,	//  0: 7
	reg_hsync_rising_auto           : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_hsync_rising_start          : 6;	// 12:17
} L9B0_IRIS_032_T;

/*-----------------------------------------------------------------------------
	0x0188 iris_033 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hblank_end                  : 8,	//  0: 7
	reg_hblank_start                : 8;	//  8:15
} L9B0_IRIS_033_T;

/*-----------------------------------------------------------------------------
	0x018c iris_034 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_burst_gate_end              : 8,	//  0: 7
	reg_burst_gate_start            : 8;	//  8:15
} L9B0_IRIS_034_T;

/*-----------------------------------------------------------------------------
	0x0190 iris_035 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hactive_width               : 8,	//  0: 7
	reg_hactive_start               : 8;	//  8:15
} L9B0_IRIS_035_T;

/*-----------------------------------------------------------------------------
	0x0194 iris_036 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vactive_height              : 8,	//  0: 7
	reg_vactive_start               : 8;	//  8:15
} L9B0_IRIS_036_T;

/*-----------------------------------------------------------------------------
	0x0198 iris_037 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync_h_max                 : 7,	//  0: 6
	                                : 1,	//     7 reserved
	reg_vsync_h_min                 : 7;	//  8:14
} L9B0_IRIS_037_T;

/*-----------------------------------------------------------------------------
	0x019c iris_038 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync_agc_max               : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_vsync_agc_min               : 7;	//  8:14
} L9B0_IRIS_038_T;

/*-----------------------------------------------------------------------------
	0x01a0 iris_039 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync_vbi_max               : 7,	//  0: 6
	                                : 1,	//     7 reserved
	reg_vsync_vbi_min               : 7;	//  8:14
} L9B0_IRIS_039_T;

/*-----------------------------------------------------------------------------
	0x01a4 iris_040 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vlock_wide_range            : 1;	//     0
} L9B0_IRIS_040_T;

/*-----------------------------------------------------------------------------
	0x01a8 iris_041 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_proscan_1field_mode         : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_vsync_cntl_noisy            : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_vsync_cntl_ff_rew           : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_vsync_cntl_trick            : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_vsync_cntl_vcr              : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_vsync_cntl                  : 2;	// 20:21
} L9B0_IRIS_041_T;

/*-----------------------------------------------------------------------------
	0x01ac iris_042 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_field_polarity              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_flip_field                  : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_veven_delayed               : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_vodd_delayed                : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_field_detect_mode           : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_vloop_tc                    : 2;	// 20:21
} L9B0_IRIS_042_T;

/*-----------------------------------------------------------------------------
	0x01b0 iris_043 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_dbg_syncs                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_muxdigital                  : 4,	//  4: 7
	reg_muxanalogb                  : 4,	//  8:11
	reg_muxanaloga                  : 4;	// 12:15
} L9B0_IRIS_043_T;

/*-----------------------------------------------------------------------------
	0x01b4 iris_044 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_ybw                   : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_peak_range                  : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_peak_gain                   : 3,	//  8:10
	                                : 1,	//    11 reserved
	reg_peak_en                     : 1;	//    12
} L9B0_IRIS_044_T;

/*-----------------------------------------------------------------------------
	0x01b8 iris_045 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_auto_secam_level            : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_sv_bf                       : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_secam_pll                   : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_secam_sw_old                : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_palsw_level                 : 2;	// 16:17
} L9B0_IRIS_045_T;

/*-----------------------------------------------------------------------------
	0x01bc iris_046 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lose_chromalock_mode        : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_lose_chromalock_count       : 4,	//  4: 7
	reg_lose_chromalock_level       : 3,	//  8:10
	                                : 1,	//    11 reserved
	reg_lose_chromalock_ckill       : 1;	//    12
} L9B0_IRIS_046_T;

/*-----------------------------------------------------------------------------
	0x01c0 iris_047 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cstripe_detect_control      : 3;	//  0: 2
} L9B0_IRIS_047_T;

/*-----------------------------------------------------------------------------
	0x01c4 iris_048 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hfine_vcr_trick_en          : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_hfine_vcr_en                : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_hresampler_2up              : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cstate                      : 3,	// 12:14
	                                : 1,	//    15 reserved
	reg_fixed_cstate                : 1;	//    16
} L9B0_IRIS_048_T;

/*-----------------------------------------------------------------------------
	0x01c8 iris_049 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_pulse_ampl              : 8;	//  0: 7
} L9B0_IRIS_049_T;

/*-----------------------------------------------------------------------------
	0x01cc iris_050 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpump_delay                 : 8,	//  0: 7
	reg_cpump_adjust                : 8,	//  8:15
	reg_cpump_adjust_polarity       : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_cpump_adjust_delay          : 6;	// 20:25
} L9B0_IRIS_050_T;

/*-----------------------------------------------------------------------------
	0x01d0 iris_051 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mv_colourstripes_sel        : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_mv_vbi_sel                  : 1;	//     4
} L9B0_IRIS_051_T;

/*-----------------------------------------------------------------------------
	0x01d4 iris_052 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpump_kill_cr               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cpump_kill_cb               : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cpump_kill_y                : 1;	//     8
} L9B0_IRIS_052_T;

/*-----------------------------------------------------------------------------
	0x01d8 iris_053 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpump_auto_stip_nobp        : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cpump_auto_stip_unlocked    : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cpump_auto_stip_no_signal   : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cpump_auto_stip_noisy       : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_cpump_auto_stip_vactive     : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_cpump_auto_stip_mode        : 2;	// 20:21
} L9B0_IRIS_053_T;

/*-----------------------------------------------------------------------------
	0x01dc iris_054 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpump_accum_mode            : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cpump_fixed_syncmid         : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cpump_level_filter_gain     : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_cpump_noisy_filter_en       : 1;	//    12
} L9B0_IRIS_054_T;

/*-----------------------------------------------------------------------------
	0x01e0 iris_055 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpump_dn_max                : 7,	//  0: 6
	                                : 1,	//     7 reserved
	reg_cpump_up_max                : 7,	//  8:14
	                                : 1,	//    15 reserved
	reg_cpump_vsync_blank_filter    : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_cpump_vsync_syncmid_filter  : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_cpump_vsync_mode            : 2;	// 24:25
} L9B0_IRIS_055_T;

/*-----------------------------------------------------------------------------
	0x01e4 iris_056 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpump_diff_noisy_only       : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cpump_dn_diff_max           : 7,	//  4:10
	                                : 1,	//    11 reserved
	reg_cpump_diff_signal_only      : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_cpump_up_diff_max           : 7;	// 16:22
} L9B0_IRIS_056_T;

/*-----------------------------------------------------------------------------
	0x01e8 iris_057 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cpump_pr_override           : 8,	//  0: 7
	reg_cpump_pb_override           : 8,	//  8:15
	reg_cpump_y_override            : 8;	// 16:23
} L9B0_IRIS_057_T;

/*-----------------------------------------------------------------------------
	0x01ec iris_058 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_db_freq                     :12,	//  0:11
	reg_dr_freq                     :12,	// 12:23
	reg_cvbs_y_delay                : 5;	// 24:28
} L9B0_IRIS_058_T;

/*-----------------------------------------------------------------------------
	0x01f0 iris_059 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vactivity_en                : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_vactivity_on2frame          : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_vactivity_threshold         : 6;	//  8:13
} L9B0_IRIS_059_T;

/*-----------------------------------------------------------------------------
	0x01f4 iris_060 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_horiz_diff_cgain            : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_horiz_diff_ygain            : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_chroma_vdiff_gain           : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_lowfreq_vdiff_gain          : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_noise_th_en                 : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_noise_th                    : 7;	// 20:26
} L9B0_IRIS_060_T;

/*-----------------------------------------------------------------------------
	0x01f8 iris_061 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vadap_burst_noise_th_gain   : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_burst_noise_th_gain         : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_c_noise_th_gain             : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_y_noise_th_gain             : 2;	// 12:13
} L9B0_IRIS_061_T;

/*-----------------------------------------------------------------------------
	0x01fc iris_062 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_adaptive_chroma_mode        : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_dot_suppress_mode           : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_motion_mode                 : 2;	//  8: 9
} L9B0_IRIS_062_T;

/*-----------------------------------------------------------------------------
	0x0200 iris_063 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pal3dcomb_vactive_offset    : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_fb_sync                     : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_fb_hold                     : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_fb_ctl                      : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_field_latency               : 3,	// 16:18
	                                : 1,	//    19 reserved
	reg_lbadrgen_rst                : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_comb2d_only                 : 1;	//    24
} L9B0_IRIS_063_T;

/*-----------------------------------------------------------------------------
	0x0204 iris_064 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_md_noise_th_en              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_md_noise_th                 : 7;	//  4:10
} L9B0_IRIS_064_T;

/*-----------------------------------------------------------------------------
	0x0208 iris_065 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_schroma_peak_en             : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_schroma_coring_en           : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_schroma_peak                : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_pchroma_peak_en             : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_pchroma_coring_en           : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_pchroma_peak                : 2;	// 20:21
} L9B0_IRIS_065_T;

/*-----------------------------------------------------------------------------
	0x020c iris_066 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ldpause_threshold           : 4,	//  0: 3
	reg_vf_nstd_en                  : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_vcr_auto_switch_en          : 1;	//     8
} L9B0_IRIS_066_T;

/*-----------------------------------------------------------------------------
	0x0210 iris_067 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_comb_coring                 : 4,	//  0: 3
	reg_tcomb_gain                  : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_notch_gain                  : 3,	//  8:10
	                                : 1,	//    11 reserved
	reg_comb_gain                   : 3;	// 12:14
} L9B0_IRIS_067_T;

/*-----------------------------------------------------------------------------
	0x0214 iris_068 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vactive_md_height           : 8,	//  0: 7
	reg_vactive_md_start            : 8;	//  8:15
} L9B0_IRIS_068_T;

/*-----------------------------------------------------------------------------
	0x0218 iris_069 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hsync_pulse_width           : 4;	//  0: 3
} L9B0_IRIS_069_T;

/*-----------------------------------------------------------------------------
	0x021c iris_070 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cagc_tc_ismall              : 3;	//  0: 2
} L9B0_IRIS_070_T;

/*-----------------------------------------------------------------------------
	0x0220 iris_071 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cagc_coring_threshold       : 4,	//  0: 3
	reg_cagc_unity_gain             : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cagc_coring                 : 3,	//  8:10
	                                : 1,	//    11 reserved
	reg_cagc_tc_p                   : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_cagc_tc_ibig                : 3;	// 16:18
} L9B0_IRIS_071_T;

/*-----------------------------------------------------------------------------
	0x0224 iris_072 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_aaf_on                      : 1;	//     0
} L9B0_IRIS_072_T;

/*-----------------------------------------------------------------------------
	0x0228 iris_073 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_dcrestore_accum_width       : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_dcrestore_no_bad_bp         : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_dcrestore_kill_enable       : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_dcrestore_bp_delay          : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_syncmid_nobp_en             : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_syncmid_filter_en           : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_dcrestore_gain              : 2;	// 28:29
} L9B0_IRIS_073_T;

/*-----------------------------------------------------------------------------
	0x022c iris_074 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_dcrestore_kill_enable_noisy : 1;	//     0
} L9B0_IRIS_074_T;

/*-----------------------------------------------------------------------------
	0x0230 iris_075 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_dcrestore_hsync_mid         : 8,	//  0: 7
	reg_bp_kill_thresh              : 8,	//  8:15
	reg_hmgc                        : 8,	// 16:23
	reg_dcrestore_lpf_en            : 1;	//    24
} L9B0_IRIS_075_T;

/*-----------------------------------------------------------------------------
	0x0234 iris_076 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_auto_min_sync_height        : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_min_sync_height             : 7;	//  4:10
} L9B0_IRIS_076_T;

/*-----------------------------------------------------------------------------
	0x0238 iris_077 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vcrtrick_proscan            : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_veven_early_delayed         : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_vodd_early_delayed          : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_vfield_hoffset_fixed        : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_vsync_no_signal_thresh      : 8,	// 16:23
	reg_vsync_signal_thresh         : 8;	// 24:31
} L9B0_IRIS_077_T;

/*-----------------------------------------------------------------------------
	0x023c iris_078 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vfield_hoffset              : 9,	//  0: 8
	                                : 3,	//  9:11 reserved
	reg_vactive_half_lines          : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_vdetect_noise_en            : 1;	//    16
} L9B0_IRIS_078_T;

/*-----------------------------------------------------------------------------
	0x0240 iris_079 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_dual_hedge_dis              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_dual_hedge_auto_width       : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_dual_fine_hedge_vbi         : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_dual_coarse_hedge_vbi       : 2;	// 12:13
} L9B0_IRIS_079_T;

/*-----------------------------------------------------------------------------
	0x0244 iris_080 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mv_hsync_rising_end         : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_vcr_state2_long             : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_slow_hdsw                   : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_mv_hsync_rising_start       : 6,	// 16:21
	                                : 2,	// 22:23 reserved
	reg_no_hsyncs_mode              : 2,	// 24:25
	                                : 2,	// 26:27 reserved
	reg_many_hsyncs_mode            : 1;	//    28
} L9B0_IRIS_080_T;

/*-----------------------------------------------------------------------------
	0x0248 iris_081 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_no_hsyncs_weak              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_disable_hdsw_weak           : 1;	//     4
} L9B0_IRIS_081_T;

/*-----------------------------------------------------------------------------
	0x024c iris_082 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vactive_hdsw_mode           : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_disable_hdsw_mode           : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_hsync_falling_filter        : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_no_hsyncs_noisy             : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_hloop_range                 : 2;	// 16:17
} L9B0_IRIS_082_T;

/*-----------------------------------------------------------------------------
	0x0250 iris_083 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_md_hf_max                   : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_md_hf_shift                 : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_md_lf_sad                   : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_md_lf_shift                 : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_md_c_noise_th_en            : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_md_c_noise_th               : 7;	// 20:26
} L9B0_IRIS_083_T;

/*-----------------------------------------------------------------------------
	0x0254 iris_084 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_md_cf_cactivity_high        : 4,	//  0: 3
	reg_md_cf_lactivity_high        : 8,	//  4:11
	reg_md_cf_cactivity_low         : 8,	// 12:19
	reg_md_cf_lactivity_low         : 8,	// 20:27
	reg_md_cf_activity_enable       : 2;	// 28:29
} L9B0_IRIS_084_T;

/*-----------------------------------------------------------------------------
	0x0258 iris_085 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_md_k_thresh                 : 8;	//  0: 7
} L9B0_IRIS_085_T;

/*-----------------------------------------------------------------------------
	0x025c iris_086 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_tcomb_chroma_level          : 8,	//  0: 7
	reg_hf_luma_chroma_offset       : 8,	//  8:15
	reg_spatial_luma_level          : 8,	// 16:23
	reg_chroma_level                : 8;	// 24:31
} L9B0_IRIS_086_T;

/*-----------------------------------------------------------------------------
	0x0260 iris_087 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_chroma_activity_level       : 8,	//  0: 7
	reg_lf_luma_offset              : 8;	//  8:15
} L9B0_IRIS_087_T;

/*-----------------------------------------------------------------------------
	0x0264 iris_088 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_avg_freq_range              : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_issecam_th                  : 6,	//  4: 9
	                                : 2,	// 10:11 reserved
	reg_freq_offset_range           : 8;	// 12:19
} L9B0_IRIS_088_T;

/*-----------------------------------------------------------------------------
	0x0268 iris_089 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_chroma_bw_motion_th         : 8,	//  0: 7
	reg_motion_config               : 8,	//  8:15
	reg_hactive_md_width            : 8,	// 16:23
	reg_hactive_md_start            : 8;	// 24:31
} L9B0_IRIS_089_T;

/*-----------------------------------------------------------------------------
	0x026c iris_090 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_motion_mode          : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_chroma_bw_motion            : 2;	//  4: 5
} L9B0_IRIS_090_T;

/*-----------------------------------------------------------------------------
	0x0270 iris_091 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_flat_chroma_offset          : 8,	//  0: 7
	reg_flat_luma_offset            : 8,	//  8:15
	reg_flat_chroma_shift           : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_flat_luma_shift             : 2;	// 20:21
} L9B0_IRIS_091_T;

/*-----------------------------------------------------------------------------
	0x0274 iris_092 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_motion_debug                : 8,	//  0: 7
	reg_cf_flat_motion_shift        : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_motion_c_mode               : 2;	// 12:13
} L9B0_IRIS_092_T;

/*-----------------------------------------------------------------------------
	0x0278 iris_093 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cordic_gate_end             : 8,	//  0: 7
	reg_cordic_gate_start           : 8,	//  8:15
	reg_ispal_th                    : 8,	// 16:23
	reg_phase_offset_range          : 8;	// 24:31
} L9B0_IRIS_093_T;

/*-----------------------------------------------------------------------------
	0x027c iris_094 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pal3tap_only_c              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_pal3tap_only_y              : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_adc_cpump_swap              : 6;	//  8:13
} L9B0_IRIS_094_T;

/*-----------------------------------------------------------------------------
	0x0280 iris_095 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ckill                       :16,	//  0:15
	reg_cagc_gate_end               : 8,	// 16:23
	reg_cagc_gate_start             : 8;	// 24:31
} L9B0_IRIS_095_T;

/*-----------------------------------------------------------------------------
	0x0284 iris_096 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_icburst_half_ampl           :10;	//  0: 9
} L9B0_IRIS_096_T;

/*-----------------------------------------------------------------------------
	0x0288 iris_097 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_agc_bypass                  : 1;	//     0
} L9B0_IRIS_097_T;

/*-----------------------------------------------------------------------------
	0x028c iris_098 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_clampagc_inv                : 4;	//  0: 3
} L9B0_IRIS_098_T;

/*-----------------------------------------------------------------------------
	0x0290 iris_099 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vps_slicer_mode             : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_wss_slicer_mode             : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_tt_slicer_mode              : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_cc_slicer_mode              : 2;	// 12:13
} L9B0_IRIS_099_T;

/*-----------------------------------------------------------------------------
	0x0294 iris_100 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_perr_on                     : 1;	//     0
} L9B0_IRIS_100_T;

/*-----------------------------------------------------------------------------
	0x0298 iris_101 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_perr_dnref                  ;   	// 31: 0
} L9B0_IRIS_101_T;

/*-----------------------------------------------------------------------------
	0x029c iris_102 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_lpfa_coeff0            :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_cres_lpfa_on                : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_iris_cresampler_on          : 1;	//    20
} L9B0_IRIS_102_T;

/*-----------------------------------------------------------------------------
	0x02a0 iris_103 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_lpfa_coeff2            :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_cres_lpfa_coeff1            :13;	// 16:28
} L9B0_IRIS_103_T;

/*-----------------------------------------------------------------------------
	0x02a4 iris_104 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_lpfa_coeff4            :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_cres_lpfa_coeff3            :13;	// 16:28
} L9B0_IRIS_104_T;

/*-----------------------------------------------------------------------------
	0x02a8 iris_105 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_lpfa_coeff5            :13;	//  0:12
} L9B0_IRIS_105_T;

/*-----------------------------------------------------------------------------
	0x02ac iris_106 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_lpfb_coeff0            :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_cres_lpfb_on                : 1;	//    16
} L9B0_IRIS_106_T;

/*-----------------------------------------------------------------------------
	0x02b0 iris_107 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_lpfb_coeff2            :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_cres_lpfb_coeff1            :13;	// 16:28
} L9B0_IRIS_107_T;

/*-----------------------------------------------------------------------------
	0x02b4 iris_108 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_lpfb_coeff4            :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_cres_lpfb_coeff3            :13;	// 16:28
} L9B0_IRIS_108_T;

/*-----------------------------------------------------------------------------
	0x02b8 iris_109 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_blend_sel              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cres_blend_method           : 3;	//  4: 6
} L9B0_IRIS_109_T;

/*-----------------------------------------------------------------------------
	0x02bc iris_110 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_blend_bcdefault        :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_cres_0px_sel                : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_cres_clksel                 : 2;	// 16:17
} L9B0_IRIS_110_T;

/*-----------------------------------------------------------------------------
	0x02c0 iris_111 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_blend_bcpara1          :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_cres_blend_bcpara0          :11;	// 12:22
} L9B0_IRIS_111_T;

/*-----------------------------------------------------------------------------
	0x02c4 iris_112 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_blend_bcpara3          :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_cres_blend_bcpara2          :11;	// 12:22
} L9B0_IRIS_112_T;

/*-----------------------------------------------------------------------------
	0x02c8 iris_113 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_blend_bcpara5          :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_cres_blend_bcpara4          :11;	// 12:22
} L9B0_IRIS_113_T;

/*-----------------------------------------------------------------------------
	0x02cc iris_114 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_blend_bcpara7          :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_cres_blend_bcpara6          :11;	// 12:22
} L9B0_IRIS_114_T;

/*-----------------------------------------------------------------------------
	0x02d0 iris_115 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_blend_diffth1          :12,	//  0:11
	reg_cres_blend_diffth0          :12;	// 12:23
} L9B0_IRIS_115_T;

/*-----------------------------------------------------------------------------
	0x02d4 iris_116 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cres_alpha_off              : 8,	//  0: 7
	reg_cres_blend_diffth2          :12;	//  8:19
} L9B0_IRIS_116_T;

/*-----------------------------------------------------------------------------
	0x02d8 iris_117 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hrscbld_dnoff               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_hrscbld_upoff               : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_hrsybld_dnoff               : 3,	//  8:10
	                                : 1,	//    11 reserved
	reg_hrsybld_upoff               : 4,	// 12:15
	reg_dtrs_dlysel                 : 4,	// 16:19
	reg_dtrs_revalpha_sel           : 2;	// 20:21
} L9B0_IRIS_117_T;

/*-----------------------------------------------------------------------------
	0x02dc iris_118 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_oadj_y_offo                 :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_oadj_y_offi                 :11;	// 12:22
} L9B0_IRIS_118_T;

/*-----------------------------------------------------------------------------
	0x02e0 iris_119 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_oadj_y_coeff                :14;	//  0:13
} L9B0_IRIS_119_T;

/*-----------------------------------------------------------------------------
	0x02e4 iris_120 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_oadj_c_offo                 :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_oadj_c_offi                 :11;	// 12:22
} L9B0_IRIS_120_T;

/*-----------------------------------------------------------------------------
	0x02e8 iris_121 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_oadj_c_coeff                :14;	//  0:13
} L9B0_IRIS_121_T;

/*-----------------------------------------------------------------------------
	0x02ec iris_122 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_adc_unity_gain_value        : 4;	//  0: 3
} L9B0_IRIS_122_T;

/*-----------------------------------------------------------------------------
	0x02f0 iris_123 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_frame_motion_th1            :16,	//  0:15
	reg_frame_motion_th0            :16;	// 16:31
} L9B0_IRIS_123_T;

/*-----------------------------------------------------------------------------
	0x02f4 iris_124 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_iblank_half_lvl0            :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_isynctip_lvl0               :10;	// 12:21
} L9B0_IRIS_124_T;

/*-----------------------------------------------------------------------------
	0x02f8 iris_125 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sync_int_blank_lvl0         :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_ipeak_chroma_m768_lvl0      :10;	// 12:21
} L9B0_IRIS_125_T;

/*-----------------------------------------------------------------------------
	0x02fc iris_126 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sync_int_blank_vid_lvl0     : 8,	//  0: 7
	reg_int_black_ped_vid_lvl0      :10,	//  8:17
	                                : 2,	// 18:19 reserved
	reg_int_blank_vid_lvl0          :10;	// 20:29
} L9B0_IRIS_126_T;

/*-----------------------------------------------------------------------------
	0x0300 iris_127 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_int_vbi_blank_vid_lvl0      : 8;	//  0: 7
} L9B0_IRIS_127_T;

/*-----------------------------------------------------------------------------
	0x0304 iris_128 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_internal_sync_midpt         :13,	//  0:12
	                                : 3,	// 13:15 reserved
	reg_internal_sync_level         :13;	// 16:28
} L9B0_IRIS_128_T;

/*-----------------------------------------------------------------------------
	0x0308 iris_129 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_sync_internal_sync_midpt    :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_internal_blank_level        :13;	// 12:24
} L9B0_IRIS_129_T;

/*-----------------------------------------------------------------------------
	0x030c iris_130 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_adc_blank_level             :10;	//  0: 9
} L9B0_IRIS_130_T;

/*-----------------------------------------------------------------------------
	0x0310 iris_131 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hrs_alpha_croff             : 8,	//  0: 7
	reg_hrs_alpha_cboff             : 8,	//  8:15
	reg_hrs_alpha_yoff              : 8;	// 16:23
} L9B0_IRIS_131_T;

/*-----------------------------------------------------------------------------
	0x0314 iris_132 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cphase_adjust               : 3,	//  0: 2
	                                : 1,	//     3 reserved
	reg_cphase_adjust_sel           : 1;	//     4
} L9B0_IRIS_132_T;

/*-----------------------------------------------------------------------------
	0x0318 iris_133 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_adap_clf_mode               : 3,	//  0: 2
	                                : 1,	//     3 reserved
	reg_cb_delay                    : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_yc444                       : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_ycadj_cr_p                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_oadj_swap                   : 1;	//    16
} L9B0_IRIS_133_T;

/*-----------------------------------------------------------------------------
	0x031c iris_134 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lpf_tap_0_0                 :16,	//  0:15
	reg_lpf_0_en                    : 1;	//    16
} L9B0_IRIS_134_T;

/*-----------------------------------------------------------------------------
	0x0320 iris_135 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lpf_tap_0_2                 :16,	//  0:15
	reg_lpf_tap_0_1                 :16;	// 16:31
} L9B0_IRIS_135_T;

/*-----------------------------------------------------------------------------
	0x0324 iris_136 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lpf_tap_0_4                 :16,	//  0:15
	reg_lpf_tap_0_3                 :16;	// 16:31
} L9B0_IRIS_136_T;

/*-----------------------------------------------------------------------------
	0x0328 iris_137 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_lpf_tap_0_6                 :16,	//  0:15
	reg_lpf_tap_0_5                 :16;	// 16:31
} L9B0_IRIS_137_T;

/*-----------------------------------------------------------------------------
	0x032c iris_138 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_missing_max              : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_hs_match_max                : 6,	//  8:13
	                                : 2,	// 14:15 reserved
	reg_hs_cnt_mode                 : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_hs_sync_mode                : 3,	// 20:22
	                                : 1,	//    23 reserved
	reg_status_pseudo_sync          : 4;	// 24:27
} L9B0_IRIS_138_T;

/*-----------------------------------------------------------------------------
	0x0330 iris_139 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_length_prd_0             :24;	//  0:23
} L9B0_IRIS_139_T;

/*-----------------------------------------------------------------------------
	0x0334 iris_140 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_length_prd_th_0          :24;	//  0:23
} L9B0_IRIS_140_T;

/*-----------------------------------------------------------------------------
	0x0338 iris_141 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_length_pos_0             :24;	//  0:23
} L9B0_IRIS_141_T;

/*-----------------------------------------------------------------------------
	0x033c iris_142 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_length_pos_th_0          :24;	//  0:23
} L9B0_IRIS_142_T;

/*-----------------------------------------------------------------------------
	0x0340 iris_143 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_length_prd_1             :24;	//  0:23
} L9B0_IRIS_143_T;

/*-----------------------------------------------------------------------------
	0x0344 iris_144 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_length_prd_th_1          :24;	//  0:23
} L9B0_IRIS_144_T;

/*-----------------------------------------------------------------------------
	0x0348 iris_145 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hs_length_pos_1             :24;	//  0:23
} L9B0_IRIS_145_T;

/*-----------------------------------------------------------------------------
	0x034c iris_146 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_cnt_mode                 : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_vs_sync_mode                : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_hs_length_pos_th_1          :24;	//  8:31
} L9B0_IRIS_146_T;

/*-----------------------------------------------------------------------------
	0x0350 iris_147 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_missing_max              : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_vs_match_max                : 6;	//  8:13
} L9B0_IRIS_147_T;

/*-----------------------------------------------------------------------------
	0x0354 iris_148 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_length_prd_0             :24;	//  0:23
} L9B0_IRIS_148_T;

/*-----------------------------------------------------------------------------
	0x0358 iris_149 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_length_prd_th_0          :24;	//  0:23
} L9B0_IRIS_149_T;

/*-----------------------------------------------------------------------------
	0x035c iris_150 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_length_pos_0             :24;	//  0:23
} L9B0_IRIS_150_T;

/*-----------------------------------------------------------------------------
	0x0360 iris_151 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_length_pos_th_0          :24;	//  0:23
} L9B0_IRIS_151_T;

/*-----------------------------------------------------------------------------
	0x0364 iris_152 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_length_prd_1             :24;	//  0:23
} L9B0_IRIS_152_T;

/*-----------------------------------------------------------------------------
	0x0368 iris_153 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_length_prd_th_1          :24;	//  0:23
} L9B0_IRIS_153_T;

/*-----------------------------------------------------------------------------
	0x036c iris_154 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vs_length_pos_1             :24;	//  0:23
} L9B0_IRIS_154_T;

/*-----------------------------------------------------------------------------
	0x0370 iris_155 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_cnt_mode                 : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_ha_sync_mode                : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_vs_length_pos_th_1          :24;	//  8:31
} L9B0_IRIS_155_T;

/*-----------------------------------------------------------------------------
	0x0374 iris_156 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_missing_max              : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_ha_match_max                : 6;	//  8:13
} L9B0_IRIS_156_T;

/*-----------------------------------------------------------------------------
	0x0378 iris_157 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_length_prd_0             :24;	//  0:23
} L9B0_IRIS_157_T;

/*-----------------------------------------------------------------------------
	0x037c iris_158 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_length_prd_th_0          :24;	//  0:23
} L9B0_IRIS_158_T;

/*-----------------------------------------------------------------------------
	0x0380 iris_159 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_length_pos_0             :24;	//  0:23
} L9B0_IRIS_159_T;

/*-----------------------------------------------------------------------------
	0x0384 iris_160 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_length_pos_th_0          :24;	//  0:23
} L9B0_IRIS_160_T;

/*-----------------------------------------------------------------------------
	0x0388 iris_161 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_length_prd_1             :24;	//  0:23
} L9B0_IRIS_161_T;

/*-----------------------------------------------------------------------------
	0x038c iris_162 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_length_prd_th_1          :24;	//  0:23
} L9B0_IRIS_162_T;

/*-----------------------------------------------------------------------------
	0x0390 iris_163 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ha_length_pos_1             :24;	//  0:23
} L9B0_IRIS_163_T;

/*-----------------------------------------------------------------------------
	0x0394 iris_164 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_cnt_mode                 : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_va_sync_mode                : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_ha_length_pos_th_1          :24;	//  8:31
} L9B0_IRIS_164_T;

/*-----------------------------------------------------------------------------
	0x0398 iris_165 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_missing_max              : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	reg_va_match_max                : 6;	//  8:13
} L9B0_IRIS_165_T;

/*-----------------------------------------------------------------------------
	0x039c iris_166 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_prd_0             :24;	//  0:23
} L9B0_IRIS_166_T;

/*-----------------------------------------------------------------------------
	0x03a0 iris_167 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_prd_th_0          :24;	//  0:23
} L9B0_IRIS_167_T;

/*-----------------------------------------------------------------------------
	0x03a4 iris_168 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_pos_0             :24;	//  0:23
} L9B0_IRIS_168_T;

/*-----------------------------------------------------------------------------
	0x03a8 iris_169 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_pos_th_0          :24;	//  0:23
} L9B0_IRIS_169_T;

/*-----------------------------------------------------------------------------
	0x03ac iris_170 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_prd_1             :24;	//  0:23
} L9B0_IRIS_170_T;

/*-----------------------------------------------------------------------------
	0x03b0 iris_171 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_prd_th_1          :24;	//  0:23
} L9B0_IRIS_171_T;

/*-----------------------------------------------------------------------------
	0x03b4 iris_172 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_pos_1             :24;	//  0:23
} L9B0_IRIS_172_T;

/*-----------------------------------------------------------------------------
	0x03b8 iris_173 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_va_length_pos_th_1          :24;	//  0:23
} L9B0_IRIS_173_T;

/*-----------------------------------------------------------------------------
	0x03bc iris_174 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	swrst_cdct						: 1,   //  0: 0
	swrst_hdct			            : 1,   //  1: 1
	iris_clampagc_v2                : 1,   //  2: 2
	    							: 4,   //  3: 6
	iris_mif_gmau                   : 1,   //  7: 7
	                                : 24;  // 31: 8
} L9B0_IRIS_174_T;

/*-----------------------------------------------------------------------------
	0x03c0 iris_175 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_cenable_f0cnt        :20,	//  0:19
	reg_status_cenable_ctrl         : 7;	// 20:26
} L9B0_IRIS_175_T;

/*-----------------------------------------------------------------------------
	0x03c4 iris_176 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_cenable_lcnt         :12,	//  0:11
	reg_status_cenable_f1cnt        :20;	// 12:31
} L9B0_IRIS_176_T;

/*-----------------------------------------------------------------------------
	0x03c8 iris_177 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_updn_rst             : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_clampagc_updn               : 5;	//  4: 8
} L9B0_IRIS_177_T;

/*-----------------------------------------------------------------------------
	0x03cc iris_178 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_status_updn                 ;   	// 31: 0
} L9B0_IRIS_178_T;

/*-----------------------------------------------------------------------------
	0x03d0 iris_179 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_secam_pll                : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs_secam_sw_old             : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cs_auto_secam_level         : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs_chroma_activity_level    : 8,	// 12:19
	reg_cs_adaptive_chroma_mode     : 2,	// 20:21
	                                : 2,	// 22:23 reserved
	reg_cs_sw_rst                   : 1;	//    24
} L9B0_IRIS_179_T;

/*-----------------------------------------------------------------------------
	0x03d4 iris_180 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_phase_offset_range       : 8,	//  0: 7
	reg_cs_avg_freq_range           : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_cs_issecam_th               : 6,	// 12:17
	                                : 2,	// 18:19 reserved
	reg_cs_freq_offset_range        : 8;	// 20:27
} L9B0_IRIS_180_T;

/*-----------------------------------------------------------------------------
	0x03d8 iris_181 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_db_freq                  :12,	//  0:11
	reg_cs_dr_freq                  :12,	// 12:23
	reg_cs_ispal_th                 : 8;	// 24:31
} L9B0_IRIS_181_T;

/*-----------------------------------------------------------------------------
	0x03dc iris_182 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_cagc_en                  : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs_chroma_burst5or10        : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cs_pal60_mode               : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs_ntsc443_mode             : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_cs_colour_mode              : 3,	// 16:18
	                                : 1,	//    19 reserved
	reg_cs_adaptive_mode            : 3,	// 20:22
	                                : 1,	//    23 reserved
	reg_cs_secam_ybw                : 2;	// 24:25
} L9B0_IRIS_182_T;

/*-----------------------------------------------------------------------------
	0x03e0 iris_183 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_cagc_coring_threshold    : 4,	//  0: 3
	reg_cs_cagc_coring              : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_cs_cagc_unity_gain          : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs_cagc_tc_p                : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_cs_cagc_tc_ibig             : 3,	// 16:18
	                                : 1,	//    19 reserved
	reg_cs_cagc_tc_ismall           : 3,	// 20:22
	                                : 1,	//    23 reserved
	reg_cs_cagc                     : 8;	// 24:31
} L9B0_IRIS_183_T;

/*-----------------------------------------------------------------------------
	0x03e4 iris_184 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_chroma_bw_lo             : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_cs_ckill                    :16,	//  4:19
	reg_cs_hlock_ckill              : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_cs_vbi_ckill                : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_cs_noburst_ckill            : 1;	//    28
} L9B0_IRIS_184_T;

/*-----------------------------------------------------------------------------
	0x03e8 iris_185 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_cdto_inc                 ;   	// 31: 0
} L9B0_IRIS_185_T;

/*-----------------------------------------------------------------------------
	0x03ec iris_186 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_cstate                   : 3,	//  0: 2
	                                : 1,	//     3 reserved
	reg_cs_fixed_cstate             : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cs_user_ckill_mode          : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_cs_lose_chromalock_mode     : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_cs_lose_chromalock_count    : 4,	// 16:19
	reg_cs_lose_chromalock_level    : 3,	// 20:22
	                                : 1,	//    23 reserved
	reg_cs_lose_chromalock_ckill    : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_cs_palsw_level              : 2;	// 28:29
} L9B0_IRIS_186_T;

/*-----------------------------------------------------------------------------
	0x03f0 iris_187 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_chroma_sel               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs_cphase_adjust            : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_cs_cphase_adjust_sel        : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs_mv_colourstripes_sel     : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_cs_cstripe_detect_control   : 3;	// 16:18
} L9B0_IRIS_187_T;

/*-----------------------------------------------------------------------------
	0x03f4 iris_188 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_secam_detected           : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs_mv_colourstripes         : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_cs_status_cordic_freq       : 8,	//  8:15
	reg_cs_status_burst_mag         :16;	// 16:31
} L9B0_IRIS_188_T;

/*-----------------------------------------------------------------------------
	0x03f8 iris_189 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs_chromalock               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs_status_cgain             :14,	//  4:17
	                                : 2,	// 18:19 reserved
	reg_cs_status_cmag              : 8,	// 20:27
	reg_cs_pal_detected             : 1;	//    28
} L9B0_IRIS_189_T;

/*-----------------------------------------------------------------------------
	0x03fc iris_190 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_secam_pll               : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs1_secam_sw_old            : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cs1_auto_secam_level        : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs1_chroma_activity_level   : 8,	// 12:19
	reg_cs1_adaptive_chroma_mode    : 2,	// 20:21
	                                : 2,	// 22:23 reserved
	reg_cs1_sw_rst                  : 1;	//    24
} L9B0_IRIS_190_T;

/*-----------------------------------------------------------------------------
	0x0400 iris_191 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_phase_offset_range      : 8,	//  0: 7
	reg_cs1_avg_freq_range          : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_cs1_issecam_th              : 6,	// 12:17
	                                : 2,	// 18:19 reserved
	reg_cs1_freq_offset_range       : 8;	// 20:27
} L9B0_IRIS_191_T;

/*-----------------------------------------------------------------------------
	0x0404 iris_192 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_db_freq                 :12,	//  0:11
	reg_cs1_dr_freq                 :12,	// 12:23
	reg_cs1_ispal_th                : 8;	// 24:31
} L9B0_IRIS_192_T;

/*-----------------------------------------------------------------------------
	0x0408 iris_193 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_cagc_en                 : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs1_chroma_burst5or10       : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cs1_pal60_mode              : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs1_ntsc443_mode            : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_cs1_colour_mode             : 3,	// 16:18
	                                : 1,	//    19 reserved
	reg_cs1_adaptive_mode           : 3,	// 20:22
	                                : 1,	//    23 reserved
	reg_cs1_secam_ybw               : 2;	// 24:25
} L9B0_IRIS_193_T;

/*-----------------------------------------------------------------------------
	0x040c iris_194 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_cagc_coring_threshold   : 4,	//  0: 3
	reg_cs1_cagc_coring             : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_cs1_cagc_unity_gain         : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs1_cagc_tc_p               : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_cs1_cagc_tc_ibig            : 3,	// 16:18
	                                : 1,	//    19 reserved
	reg_cs1_cagc_tc_ismall          : 3,	// 20:22
	                                : 1,	//    23 reserved
	reg_cs1_cagc                    : 8;	// 24:31
} L9B0_IRIS_194_T;

/*-----------------------------------------------------------------------------
	0x0410 iris_195 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_chroma_bw_lo            : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_cs1_ckill                   :16,	//  4:19
	reg_cs1_hlock_ckill             : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_cs1_vbi_ckill               : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_cs1_noburst_ckill           : 1;	//    28
} L9B0_IRIS_195_T;

/*-----------------------------------------------------------------------------
	0x0414 iris_196 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_cdto_inc                ;   	// 31: 0
} L9B0_IRIS_196_T;

/*-----------------------------------------------------------------------------
	0x0418 iris_197 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_cstate                  : 3,	//  0: 2
	                                : 1,	//     3 reserved
	reg_cs1_fixed_cstate            : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_cs1_user_ckill_mode         : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_cs1_lose_chromalock_mode    : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_cs1_lose_chromalock_count   : 4,	// 16:19
	reg_cs1_lose_chromalock_level   : 3,	// 20:22
	                                : 1,	//    23 reserved
	reg_cs1_lose_chromalock_ckill   : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_cs1_palsw_level             : 2;	// 28:29
} L9B0_IRIS_197_T;

/*-----------------------------------------------------------------------------
	0x041c iris_198 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_chroma_sel              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs1_cphase_adjust           : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_cs1_cphase_adjust_sel       : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_cs1_mv_colourstripes_sel    : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_cs1_cstripe_detect_control  : 3;	// 16:18
} L9B0_IRIS_198_T;

/*-----------------------------------------------------------------------------
	0x0420 iris_199 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_secam_detected          : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs1_mv_colourstripes        : 3,	//  4: 6
	                                : 1,	//     7 reserved
	reg_cs1_status_cordic_freq      : 8,	//  8:15
	reg_cs1_status_burst_mag        :16;	// 16:31
} L9B0_IRIS_199_T;

/*-----------------------------------------------------------------------------
	0x0424 iris_200 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cs1_chromalock              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cs1_status_cgain            :14,	//  4:17
	                                : 2,	// 18:19 reserved
	reg_cs1_status_cmag             : 8,	// 20:27
	reg_cs1_pal_detected            : 1;	//    28
} L9B0_IRIS_200_T;

/*-----------------------------------------------------------------------------
	0x0428 iris_201 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_estfrdp_ctrl                : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	reg_estfrdp_on                  : 1;	//     8
} L9B0_IRIS_201_T;

/*-----------------------------------------------------------------------------
	0x042c iris_202 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_estfrdp_tgt_cf0cnt          :20;	//  0:19
} L9B0_IRIS_202_T;

/*-----------------------------------------------------------------------------
	0x0430 iris_203 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_estfrdp_tgt_cf1cnt          :20;	//  0:19
} L9B0_IRIS_203_T;

/*-----------------------------------------------------------------------------
	0x0434 iris_204 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_estfrdp_err_win1_u          : 8,	//  0: 7
	reg_estfrdp_err_win1_l          : 8,	//  8:15
	reg_estfrdp_err_win0_u          : 8,	// 16:23
	reg_estfrdp_err_win0_l          : 8;	// 24:31
} L9B0_IRIS_204_T;

/*-----------------------------------------------------------------------------
	0x0438 iris_205 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_estfrdp_err_flag_th         : 8;	//  0: 7
} L9B0_IRIS_205_T;

/*-----------------------------------------------------------------------------
	0x043c iris_206 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_estfrdp_err                 : 8,	//  0: 7
	reg_estfrdp_err_flag            : 1;	//     8
} L9B0_IRIS_206_T;

/*-----------------------------------------------------------------------------
	0x0440 iris_207 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vsync_width                 : 4;	//  0: 3
} L9B0_IRIS_207_T;

/*-----------------------------------------------------------------------------
	0x0444 iris_208 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl0              ;   	// 31: 0
} L9B0_IRIS_208_T;

/*-----------------------------------------------------------------------------
	0x0448 iris_209 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl1              ;   	// 31: 0
} L9B0_IRIS_209_T;

/*-----------------------------------------------------------------------------
	0x044c iris_210 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl2              ;   	// 31: 0
} L9B0_IRIS_210_T;

/*-----------------------------------------------------------------------------
	0x0450 iris_211 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl3              ;   	// 31: 0
} L9B0_IRIS_211_T;

/*-----------------------------------------------------------------------------
	0x0454 iris_212 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl4              ;   	// 31: 0
} L9B0_IRIS_212_T;

/*-----------------------------------------------------------------------------
	0x0458 iris_213 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl5              ;   	// 31: 0
} L9B0_IRIS_213_T;

/*-----------------------------------------------------------------------------
	0x045c iris_214 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl6              ;   	// 31: 0
} L9B0_IRIS_214_T;

/*-----------------------------------------------------------------------------
	0x0460 iris_215 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl7              ;   	// 31: 0
} L9B0_IRIS_215_T;

/*-----------------------------------------------------------------------------
	0x0464 iris_216 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl8              ;   	// 31: 0
} L9B0_IRIS_216_T;

/*-----------------------------------------------------------------------------
	0x0468 iris_217 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl9              ;   	// 31: 0
} L9B0_IRIS_217_T;

/*-----------------------------------------------------------------------------
	0x046c iris_218 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl10             ;   	// 31: 0
} L9B0_IRIS_218_T;

/*-----------------------------------------------------------------------------
	0x0470 iris_219 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl0              ;   	// 31: 0
} L9B0_IRIS_219_T;

/*-----------------------------------------------------------------------------
	0x0474 iris_220 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl1              ;   	// 31: 0
} L9B0_IRIS_220_T;

/*-----------------------------------------------------------------------------
	0x0478 iris_221 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl2              ;   	// 31: 0
} L9B0_IRIS_221_T;

/*-----------------------------------------------------------------------------
	0x047c iris_222 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl3              ;   	// 31: 0
} L9B0_IRIS_222_T;

/*-----------------------------------------------------------------------------
	0x0480 iris_223 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl4              ;   	// 31: 0
} L9B0_IRIS_223_T;

/*-----------------------------------------------------------------------------
	0x0484 iris_224 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl5              ;   	// 31: 0
} L9B0_IRIS_224_T;

/*-----------------------------------------------------------------------------
	0x0488 iris_225 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl6              ;   	// 31: 0
} L9B0_IRIS_225_T;

/*-----------------------------------------------------------------------------
	0x048c iris_226 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_ctrl7              ;   	// 31: 0
} L9B0_IRIS_226_T;

/*-----------------------------------------------------------------------------
	0x0490 iris_227 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl0              ;   	// 31: 0
} L9B0_IRIS_227_T;

/*-----------------------------------------------------------------------------
	0x0494 iris_228 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl1              ;   	// 31: 0
} L9B0_IRIS_228_T;

/*-----------------------------------------------------------------------------
	0x0498 iris_229 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl2              ;   	// 31: 0
} L9B0_IRIS_229_T;

/*-----------------------------------------------------------------------------
	0x049c iris_230 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl3              ;   	// 31: 0
} L9B0_IRIS_230_T;

/*-----------------------------------------------------------------------------
	0x04a0 iris_231 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl4              ;   	// 31: 0
} L9B0_IRIS_231_T;

/*-----------------------------------------------------------------------------
	0x04a4 iris_232 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl5              ;   	// 31: 0
} L9B0_IRIS_232_T;

/*-----------------------------------------------------------------------------
	0x04a8 iris_233 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl6              ;   	// 31: 0
} L9B0_IRIS_233_T;

/*-----------------------------------------------------------------------------
	0x04ac iris_234 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_st_ctrl7              ;   	// 31: 0
} L9B0_IRIS_234_T;

/*-----------------------------------------------------------------------------
	0x04b0 iris_235 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_blend_ctrl0           ;   	// 31: 0
} L9B0_IRIS_235_T;

/*-----------------------------------------------------------------------------
	0x04b4 iris_236 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl11             ;   	// 31: 0
} L9B0_IRIS_236_T;

/*-----------------------------------------------------------------------------
	0x04b8 iris_237 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl12             ;   	// 31: 0
} L9B0_IRIS_237_T;

/*-----------------------------------------------------------------------------
	0x04bc iris_238 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl13             ;   	// 31: 0
} L9B0_IRIS_238_T;

/*-----------------------------------------------------------------------------
	0x04c0 iris_239 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl14             ;   	// 31: 0
} L9B0_IRIS_239_T;

/*-----------------------------------------------------------------------------
	0x04c4 iris_240 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl15             ;   	// 31: 0
} L9B0_IRIS_240_T;

/*-----------------------------------------------------------------------------
	0x04c8 iris_241 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl0                 ;   	// 31: 0
} L9B0_IRIS_241_T;

/*-----------------------------------------------------------------------------
	0x04cc iris_242 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl1                 ;   	// 31: 0
} L9B0_IRIS_242_T;

/*-----------------------------------------------------------------------------
	0x04d0 iris_243 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl2                 ;   	// 31: 0
} L9B0_IRIS_243_T;

/*-----------------------------------------------------------------------------
	0x04d4 iris_244 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl3                 ;   	// 31: 0
} L9B0_IRIS_244_T;

/*-----------------------------------------------------------------------------
	0x04d8 iris_245 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl4                 ;   	// 31: 0
} L9B0_IRIS_245_T;

/*-----------------------------------------------------------------------------
	0x04dc iris_246 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl5                 ;   	// 31: 0
} L9B0_IRIS_246_T;

/*-----------------------------------------------------------------------------
	0x04e0 iris_247 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl6                 ;   	// 31: 0
} L9B0_IRIS_247_T;

/*-----------------------------------------------------------------------------
	0x04e4 iris_248 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl7                 ;   	// 31: 0
} L9B0_IRIS_248_T;

/*-----------------------------------------------------------------------------
	0x04e8 iris_249 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl8                 ;   	// 31: 0
} L9B0_IRIS_249_T;

/*-----------------------------------------------------------------------------
	0x04ec iris_250 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl9                 ;   	// 31: 0
} L9B0_IRIS_250_T;

/*-----------------------------------------------------------------------------
	0x04f0 iris_251 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl10                ;   	// 31: 0
} L9B0_IRIS_251_T;

/*-----------------------------------------------------------------------------
	0x04f4 iris_252 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_0          ;   	// 31: 0
} L9B0_IRIS_252_T;

/*-----------------------------------------------------------------------------
	0x04f8 iris_253 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_1          ;   	// 31: 0
} L9B0_IRIS_253_T;

/*-----------------------------------------------------------------------------
	0x04fc iris_254 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_2          ;   	// 31: 0
} L9B0_IRIS_254_T;

/*-----------------------------------------------------------------------------
	0x0500 iris_255 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_3          ;   	// 31: 0
} L9B0_IRIS_255_T;

/*-----------------------------------------------------------------------------
	0x0504 iris_256 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_4          ;   	// 31: 0
} L9B0_IRIS_256_T;

/*-----------------------------------------------------------------------------
	0x0508 iris_257 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_5          ;   	// 31: 0
} L9B0_IRIS_257_T;

/*-----------------------------------------------------------------------------
	0x050c iris_258 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hrs_cx2_coef2               :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_hrs_cx2_coef1               :11;	// 12:22
} L9B0_IRIS_258_T;

/*-----------------------------------------------------------------------------
	0x0510 iris_259 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hrs_cx2_coef4               :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_hrs_cx2_coef3               :11;	// 12:22
} L9B0_IRIS_259_T;

/*-----------------------------------------------------------------------------
	0x0514 iris_260 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hrs_cx2_coef6               :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_hrs_cx2_coef5               :11;	// 12:22
} L9B0_IRIS_260_T;

/*-----------------------------------------------------------------------------
	0x0518 iris_261 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hrs_cx2_coef8               :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_hrs_cx2_coef7               :11;	// 12:22
} L9B0_IRIS_261_T;

/*-----------------------------------------------------------------------------
	0x051c iris_262 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_hrs_ha_width                : 8,	//  0: 7
	reg_hrs_ha_start                : 8;	//  8:15
} L9B0_IRIS_262_T;

/*-----------------------------------------------------------------------------
	0x0520 iris_263 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cti0_en                     : 1,	//     0
	reg_cti0_tap                    : 1,	//     1
	reg_cti0_minmax_en              : 1,	//     2
	                                : 1,	//     3 reserved
	reg_cti0_gain                   : 8;	//  4:11
} L9B0_IRIS_263_T;

/*-----------------------------------------------------------------------------
	0x0524 iris_264 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cti0_en                     : 1,	//     0
	reg_cti0_tap                    : 1,	//     1
	reg_cti0_minmax_en              : 1,	//     2
	                                : 1,	//     3 reserved
	reg_cti0_gain                   : 8;	//  4:11
} L9B0_IRIS_264_T;

/*-----------------------------------------------------------------------------
	0x0530 iris_265 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_3d_status0            ;   	// 31: 0
} L9B0_IRIS_265_T;

/*-----------------------------------------------------------------------------
	0x0534 iris_266 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_6          ;   	// 31: 0
} L9B0_IRIS_266_T;

/*-----------------------------------------------------------------------------
	0x0538 iris_267 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cdetect_lpf_ctrl_7          ;   	// 31: 0
} L9B0_IRIS_267_T;

/*-----------------------------------------------------------------------------
	0x053c iris_268 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycadjust_sat_ctrl_0         ;   	// 31: 0
} L9B0_IRIS_268_T;

/*-----------------------------------------------------------------------------
	0x0540 iris_269 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl16             ;   	// 31: 0
} L9B0_IRIS_269_T;

/*-----------------------------------------------------------------------------
	0x0544 iris_270 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl17             ;   	// 31: 0
} L9B0_IRIS_270_T;

/*-----------------------------------------------------------------------------
	0x0548 iris_271 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_burst2_gate_end             : 8,	//  0: 7
	reg_burst2_gate_start           : 8,	//  8:15
	reg_burst1_gate_end             : 8,	// 16:23
	reg_burst1_gate_start           : 8;	// 24:31
} L9B0_IRIS_271_T;

/*-----------------------------------------------------------------------------
	0x054c iris_272 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cordic2_gate_end            : 8,	//  0: 7
	reg_cordic2_gate_start          : 8,	//  8:15
	reg_cordic1_gate_end            : 8,	// 16:23
	reg_cordic1_gate_start          : 8;	// 24:31
} L9B0_IRIS_272_T;

/*-----------------------------------------------------------------------------
	0x0550 iris_273 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_cbcr_ctrl             :16,	//  0:15
	reg_status_vdetect_vcount       :10,	// 16:25
	                                : 2,	// 26:27 reserved
	reg_vactive_height_auto         : 1;	//    28
} L9B0_IRIS_273_T;

/*-----------------------------------------------------------------------------
	0x0554 iris_274 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_1        :17,	//  0:16
	                                : 3,	// 17:19 reserved
	reg_secam_y_notch_mode          : 1;	//    20
} L9B0_IRIS_274_T;

/*-----------------------------------------------------------------------------
	0x0558 iris_275 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_2        :17;	//  0:16
} L9B0_IRIS_275_T;

/*-----------------------------------------------------------------------------
	0x055c iris_276 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_3        :17;	//  0:16
} L9B0_IRIS_276_T;

/*-----------------------------------------------------------------------------
	0x0560 iris_277 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_4        :17;	//  0:16
} L9B0_IRIS_277_T;

/*-----------------------------------------------------------------------------
	0x0564 iris_278 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_5        :17;	//  0:16
} L9B0_IRIS_278_T;

/*-----------------------------------------------------------------------------
	0x0568 iris_279 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_6        :17;	//  0:16
} L9B0_IRIS_279_T;

/*-----------------------------------------------------------------------------
	0x056c iris_280 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_7        :17;	//  0:16
} L9B0_IRIS_280_T;

/*-----------------------------------------------------------------------------
	0x0570 iris_281 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_8        :17;	//  0:16
} L9B0_IRIS_281_T;

/*-----------------------------------------------------------------------------
	0x0574 iris_282 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_y_notch_ctrl_9        :17;	//  0:16
} L9B0_IRIS_282_T;

/*-----------------------------------------------------------------------------
	0x0578 iris_283 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_1        :17,	//  0:16
	                                : 3,	// 17:19 reserved
	reg_secam_c_notch_sel           : 1;	//    20
} L9B0_IRIS_283_T;

/*-----------------------------------------------------------------------------
	0x057c iris_284 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_2        :17;	//  0:16
} L9B0_IRIS_284_T;

/*-----------------------------------------------------------------------------
	0x0580 iris_285 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_3        :17;	//  0:16
} L9B0_IRIS_285_T;

/*-----------------------------------------------------------------------------
	0x0584 iris_286 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_4        :17;	//  0:16
} L9B0_IRIS_286_T;

/*-----------------------------------------------------------------------------
	0x0588 iris_287 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_5        :17;	//  0:16
} L9B0_IRIS_287_T;

/*-----------------------------------------------------------------------------
	0x058c iris_288 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_6        :17;	//  0:16
} L9B0_IRIS_288_T;

/*-----------------------------------------------------------------------------
	0x0590 iris_289 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_7        :17;	//  0:16
} L9B0_IRIS_289_T;

/*-----------------------------------------------------------------------------
	0x0594 iris_290 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_8        :17;	//  0:16
} L9B0_IRIS_290_T;

/*-----------------------------------------------------------------------------
	0x0598 iris_291 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_c_notch_ctrl_9        :17;	//  0:16
} L9B0_IRIS_291_T;

/*-----------------------------------------------------------------------------
	0x059c iris_292 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_ctrl11                ;   	// 31: 0
} L9B0_IRIS_292_T;

/*-----------------------------------------------------------------------------
	0x05a0 iris_293 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_ybw2                  : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_secam_y_delay               : 6,	//  4: 9
	                                : 2,	// 10:11 reserved
	reg_secam_dcr_passthrough       : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_v_valid_inv                 : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_secam_lb_en                 : 1;	//    20
} L9B0_IRIS_293_T;

/*-----------------------------------------------------------------------------
	0x05a4 iris_294 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_dcr_ctrl_0            ;   	// 31: 0
} L9B0_IRIS_294_T;

/*-----------------------------------------------------------------------------
	0x05a8 iris_295 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_dcr_ctrl_1            ;   	// 31: 0
} L9B0_IRIS_295_T;

/*-----------------------------------------------------------------------------
	0x05ac iris_296 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_secam_dcr_ctrl_2            ;   	// 31: 0
} L9B0_IRIS_296_T;

/*-----------------------------------------------------------------------------
	0x05b0 iris_297 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_yup_rise                    :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_ydn_cnt                     : 8,	// 12:19
	reg_yup_cnt                     : 8;	// 20:27
} L9B0_IRIS_297_T;

/*-----------------------------------------------------------------------------
	0x05b4 iris_298 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ydn_rise                    :11,	//  0:10
	                                : 1,	//    11 reserved
	reg_yup_fall                    :11;	// 12:22
} L9B0_IRIS_298_T;

/*-----------------------------------------------------------------------------
	0x05b8 iris_299 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_clmap_sel                   : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_ydn_fall                    :11;	//  4:14
} L9B0_IRIS_299_T;

/*-----------------------------------------------------------------------------
	0x05bc iris_300 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl18             ;   	// 31: 0
} L9B0_IRIS_300_T;

/*-----------------------------------------------------------------------------
	0x05c0 iris_301 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl19             ;   	// 31: 0
} L9B0_IRIS_301_T;

/*-----------------------------------------------------------------------------
	0x05c4 iris_302 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ycsep_2d_ctrl20             ;   	// 31: 0
} L9B0_IRIS_302_T;

/*-----------------------------------------------------------------------------
	0x05c8 iris_303 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_wmask_ctrl_0                ;   	// 31: 0
} L9B0_IRIS_303_T;

/*-----------------------------------------------------------------------------
	0x05cc iris_304 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_wmask_ctrl_1                ;   	// 31: 0
} L9B0_IRIS_304_T;

/*-----------------------------------------------------------------------------
	0x0700 vbi_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_wss625_rd_done              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cc_rd_done                  : 1;	//     4
} L9B0_VBI_001_T;

/*-----------------------------------------------------------------------------
	0x0704 vbi_002 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cc_rdy                      : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_cc_data0                    : 8,	//  4:11
	reg_cc_data1                    : 8;	// 12:19
} L9B0_VBI_002_T;

/*-----------------------------------------------------------------------------
	0x0708 vbi_003 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_wssdata0                    : 8,	//  0: 7
	reg_wssdata1                    : 8,	//  8:15
	reg_wssdata2                    : 8,	// 16:23
	reg_wss_rdy                     : 1;	//    24
} L9B0_VBI_003_T;

/*-----------------------------------------------------------------------------
	0x070c vbi_004 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cc_short_start              : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_vbi_muxout                  : 1,	//     4
	                                : 3,	//  5: 7 reserved
	reg_vbi_hsyncout                : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_adap_slvl_en                : 1,	//    12
	                                : 3,	// 13:15 reserved
	reg_vbi_st_err_ignored          : 1,	//    16
	                                : 3,	// 17:19 reserved
	reg_vbi_en                      : 1;	//    20
} L9B0_VBI_004_T;

/*-----------------------------------------------------------------------------
	0x0710 vbi_005 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_lpf_bw                  : 2;	//  0: 1
} L9B0_VBI_005_T;

/*-----------------------------------------------------------------------------
	0x0714 vbi_006 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_data_hlvl               : 8,	//  0: 7
	reg_start_code                  : 8;	//  8:15
} L9B0_VBI_006_T;

/*-----------------------------------------------------------------------------
	0x0718 vbi_007 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbil9                       : 8,	//  0: 7
	reg_vbil8                       : 8,	//  8:15
	reg_vbil7                       : 8,	// 16:23
	reg_vbil6                       : 8;	// 24:31
} L9B0_VBI_007_T;

/*-----------------------------------------------------------------------------
	0x071c vbi_008 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbil13                      : 8,	//  0: 7
	reg_vbil12                      : 8,	//  8:15
	reg_vbil11                      : 8,	// 16:23
	reg_vbil10                      : 8;	// 24:31
} L9B0_VBI_008_T;

/*-----------------------------------------------------------------------------
	0x0720 vbi_009 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbil17                      : 8,	//  0: 7
	reg_vbil16                      : 8,	//  8:15
	reg_vbil15                      : 8,	// 16:23
	reg_vbil14                      : 8;	// 24:31
} L9B0_VBI_009_T;

/*-----------------------------------------------------------------------------
	0x0724 vbi_010 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbil21                      : 8,	//  0: 7
	reg_vbil20                      : 8,	//  8:15
	reg_vbil19                      : 8,	// 16:23
	reg_vbil18                      : 8;	// 24:31
} L9B0_VBI_010_T;

/*-----------------------------------------------------------------------------
	0x0728 vbi_011 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbil25                      : 8,	//  0: 7
	reg_vbil24                      : 8,	//  8:15
	reg_vbil23                      : 8,	// 16:23
	reg_vbil22                      : 8;	// 24:31
} L9B0_VBI_011_T;

/*-----------------------------------------------------------------------------
	0x072c vbi_012 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbil26                      : 8;	//  0: 7
} L9B0_VBI_012_T;

/*-----------------------------------------------------------------------------
	0x0730 vbi_013 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_vps_lpfil_fine_gain     : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_vbi_wss625_lpfil_fine_gain  : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_vbi_tele_lpfil_fine_gain    : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_vbi_cc_lpfil_fine_gain      : 2,	// 12:13
	                                : 2,	// 14:15 reserved
	reg_vbi_vps_slicer_mode         : 2,	// 16:17
	                                : 2,	// 18:19 reserved
	reg_vbi_wss_slicer_mode         : 2,	// 20:21
	                                : 2,	// 22:23 reserved
	reg_vbi_tt_slicer_mode          : 2,	// 24:25
	                                : 2,	// 26:27 reserved
	reg_vbi_cc_slicer_mode          : 2;	// 28:29
} L9B0_VBI_013_T;

/*-----------------------------------------------------------------------------
	0x0734 vbi_014 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_teletext_dto                :16,	//  0:15
	reg_caption_dto                 :16;	// 16:31
} L9B0_VBI_014_T;

/*-----------------------------------------------------------------------------
	0x0738 vbi_015 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vps_dto                     :16,	//  0:15
	reg_wss625_dto                  :16;	// 16:31
} L9B0_VBI_015_T;

/*-----------------------------------------------------------------------------
	0x073c vbi_016 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vps_frame_start             : 2,	//  0: 1
	                                : 2,	//  2: 3 reserved
	reg_teletext_frame_start        : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_wss625_frame_start          : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_caption_frame_start         : 2;	// 12:13
} L9B0_VBI_016_T;

/*-----------------------------------------------------------------------------
	0x0740 vbi_017 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_tele_lpfil_track_gain   : 4,	//  0: 3
	reg_vbi_tele_lpfil_acq_gain     : 4,	//  4: 7
	reg_vbi_cc_lpfil_track_gain     : 4,	//  8:11
	reg_vbi_cc_lpfil_acq_gain       : 4,	// 12:15
	reg_wssj_delta_ampl             : 8;	// 16:23
} L9B0_VBI_017_T;

/*-----------------------------------------------------------------------------
	0x0744 vbi_018 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_vps_lpfil_track_gain    : 4,	//  0: 3
	reg_vbi_vps_lpfil_acq_gain      : 4,	//  4: 7
	reg_vbi_wss625_lpfil_track_gain : 4,	//  8:11
	reg_vbi_wss625_lpfil_acq_gain   : 4;	// 12:15
} L9B0_VBI_018_T;

/*-----------------------------------------------------------------------------
	0x0748 vbi_019 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_teletext_runin_accum_ampl   : 8,	//  0: 7
	reg_caption_runin_accum_ampl    : 8;	//  8:15
} L9B0_VBI_019_T;

/*-----------------------------------------------------------------------------
	0x074c vbi_020 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vps_start                   : 8,	//  0: 7
	reg_teletext_start              : 8,	//  8:15
	reg_wss625_start                : 8,	// 16:23
	reg_caption_start               : 8;	// 24:31
} L9B0_VBI_020_T;

/*-----------------------------------------------------------------------------
	0x0750 vbi_021 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_vbi_fixgate_en              : 1;	//     0
} L9B0_VBI_021_T;

/*-----------------------------------------------------------------------------
	0x0754 vbi_022 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ttx_vbi_st_err_ignored      : 1,	//     0
	                                : 3,	//  1: 3 reserved
	reg_ttx_vbi_lpf_bw              : 2,	//  4: 5
	                                : 2,	//  6: 7 reserved
	reg_ttx_vbi_hsyncout            : 1,	//     8
	                                : 3,	//  9:11 reserved
	reg_ttx_vbi_data_hlvl           : 8,	// 12:19
	reg_ttx_adap_slvl_en            : 1,	//    20
	                                : 3,	// 21:23 reserved
	reg_ttx_vbi_en                  : 1,	//    24
	                                : 3,	// 25:27 reserved
	reg_ttx_out_sel                 : 1;	//    28
} L9B0_VBI_022_T;

/*-----------------------------------------------------------------------------
	0x0758 vbi_023 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ttx_teletext_start          : 8,	//  0: 7
	reg_ttx_vbi_tele_lpfil_fine_gain: 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_ttx_vbi_tele_lpfil_track_gai: 4,	// 12:15
	reg_ttx_vbi_tele_lpfil_acq_gain : 4,	// 16:19
	reg_ttx_teletext_runin_accum_amp: 8,	// 20:27
	reg_ttx_tt_slicer_mode          : 2;	// 28:29
} L9B0_VBI_023_T;

/*-----------------------------------------------------------------------------
	0x075c vbi_024 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_ttx_start_code              : 8,	//  0: 7
	reg_ttx_teletext_frame_start    : 2,	//  8: 9
	                                : 2,	// 10:11 reserved
	reg_ttx_teletext_dto            :16,	// 12:27
	reg_ttx_vbi_fixgate_en          : 1;	//    28
} L9B0_VBI_024_T;

/*-----------------------------------------------------------------------------
	0x07a8 iris_mif_gmua_mon_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_rbuf2_pel_cnt                :11,	//  0:10
	                                : 1,	//    11 reserved
	ro_rbuf2_empty                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	ro_rbuf1_pel_cnt                :11,	// 16:26
	                                : 1,	//    27 reserved
	ro_rbuf1_empty                  : 1;	//    28
} L9B0_IRIS_MIF_GMUA_MON_001_T;

/*-----------------------------------------------------------------------------
	0x07ac iris_mif_gmua_mon_002 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_rbuf4_pel_cnt                :11,	//  0:10
	                                : 1,	//    11 reserved
	ro_rbuf4_empty                  : 1,	//    12
	                                : 3,	// 13:15 reserved
	ro_rbuf3_pel_cnt                :11,	// 16:26
	                                : 1,	//    27 reserved
	ro_rbuf3_empty                  : 1;	//    28
} L9B0_IRIS_MIF_GMUA_MON_002_T;

/*-----------------------------------------------------------------------------
	0x07b0 iris_mif_gmua_mon_003 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_wbuf_word_cnt                : 6,	//  0: 5
	                                : 2,	//  6: 7 reserved
	ro_wbuf_empty                   : 1,	//     8
	                                : 3,	//  9:11 reserved
	ro_wbuf_full                    : 1;	//    12
} L9B0_IRIS_MIF_GMUA_MON_003_T;

/*-----------------------------------------------------------------------------
	0x07b4 iris_mif_gmua_mon_004 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_mon_wait_max_cnt            :12;	//  0:11
} L9B0_IRIS_MIF_GMUA_MON_004_T;

/*-----------------------------------------------------------------------------
	0x07b8 iris_mif_gmua_mon_005 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_wr_wait_err              : 8;	//  0: 7
} L9B0_IRIS_MIF_GMUA_MON_005_T;

/*-----------------------------------------------------------------------------
	0x07bc iris_mif_gmua_mon_006 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd4_wait_err             : 8,	//  0: 7
	ro_mon_rd3_wait_err             : 8,	//  8:15
	ro_mon_rd2_wait_err             : 8,	// 16:23
	ro_mon_rd1_wait_err             : 8;	// 24:31
} L9B0_IRIS_MIF_GMUA_MON_006_T;

/*-----------------------------------------------------------------------------
	0x07c0 iris_mif_gmua_mon_007 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_wr_req_num               :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	ro_mon_wr_req_que_cnt           : 3;	// 12:14
} L9B0_IRIS_MIF_GMUA_MON_007_T;

/*-----------------------------------------------------------------------------
	0x07c4 iris_mif_gmua_mon_008 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_wr_wait_cnt1             :12,	//  0:11
	ro_mon_wr_wait_cnt0             :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_008_T;

/*-----------------------------------------------------------------------------
	0x07c8 iris_mif_gmua_mon_009 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_wr_wait_cnt3             :12,	//  0:11
	ro_mon_wr_wait_cnt2             :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_009_T;

/*-----------------------------------------------------------------------------
	0x07cc iris_mif_gmua_mon_010 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd1_req_num              :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	ro_mon_rd1_req_que_cnt          : 3;	// 12:14
} L9B0_IRIS_MIF_GMUA_MON_010_T;

/*-----------------------------------------------------------------------------
	0x07d0 iris_mif_gmua_mon_011 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd1_wait_cnt1            :12,	//  0:11
	ro_mon_rd1_wait_cnt0            :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_011_T;

/*-----------------------------------------------------------------------------
	0x07d4 iris_mif_gmua_mon_012 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd1_wait_cnt3            :12,	//  0:11
	ro_mon_rd1_wait_cnt2            :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_012_T;

/*-----------------------------------------------------------------------------
	0x07d8 iris_mif_gmua_mon_013 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd2_req_num              :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	ro_mon_rd2_req_que_cnt          : 3;	// 12:14
} L9B0_IRIS_MIF_GMUA_MON_013_T;

/*-----------------------------------------------------------------------------
	0x07dc iris_mif_gmua_mon_014 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd2_wait_cnt1            :12,	//  0:11
	ro_mon_rd2_wait_cnt0            :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_014_T;

/*-----------------------------------------------------------------------------
	0x07e0 iris_mif_gmua_mon_015 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd2_wait_cnt3            :12,	//  0:11
	ro_mon_rd2_wait_cnt2            :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_015_T;

/*-----------------------------------------------------------------------------
	0x07e4 iris_mif_gmua_mon_016 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd3_req_num              :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	ro_mon_rd3_req_que_cnt          : 3;	// 12:14
} L9B0_IRIS_MIF_GMUA_MON_016_T;

/*-----------------------------------------------------------------------------
	0x07e8 iris_mif_gmua_mon_017 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd3_wait_cnt1            :12,	//  0:11
	ro_mon_rd3_wait_cnt0            :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_017_T;

/*-----------------------------------------------------------------------------
	0x07ec iris_mif_gmua_mon_018 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd3_wait_cnt3            :12,	//  0:11
	ro_mon_rd3_wait_cnt2            :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_018_T;

/*-----------------------------------------------------------------------------
	0x07f0 iris_mif_gmua_mon_019 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd4_req_num              :10,	//  0: 9
	                                : 2,	// 10:11 reserved
	ro_mon_rd4_req_que_cnt          : 3;	// 12:14
} L9B0_IRIS_MIF_GMUA_MON_019_T;

/*-----------------------------------------------------------------------------
	0x07f4 iris_mif_gmua_mon_020 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd4_wait_cnt0            :12;	//  0:11
} L9B0_IRIS_MIF_GMUA_MON_020_T;

/*-----------------------------------------------------------------------------
	0x07f8 iris_mif_gmua_mon_021 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd4_wait_cnt1            :12;	//  0:11
} L9B0_IRIS_MIF_GMUA_MON_021_T;

/*-----------------------------------------------------------------------------
	0x07fc iris_mif_gmua_mon_022 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ro_mon_rd4_wait_cnt3            :12,	//  0:11
	ro_mon_rd4_wait_cnt2            :12;	// 12:23
} L9B0_IRIS_MIF_GMUA_MON_022_T;

/*-----------------------------------------------------------------------------
	0x0800 iris_hif_ctrl0_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_pending_enable              : 1;	//     0
} L9B0_IRIS_HIF_CTRL0_001_T;

/*-----------------------------------------------------------------------------
	0x0804 iris_hif_ctrl1_001 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	reg_cv_timer_ctrl               : 9;	//  0: 8
} L9B0_IRIS_HIF_CTRL1_001_T;

typedef struct {
	L9B0_CVD_INTR1_ENABLE0_T               	cvd_intr1_enable0               ;	// 0x0000 : ''
	L9B0_CVD_INTR1_INTR0_T                 	cvd_intr1_intr0                 ;	// 0x0004 : ''
	L9B0_CVD_INTR1_CLEAR0_T                	cvd_intr1_clear0                ;	// 0x0008 : ''
	L9B0_CVD_INTR1_ENABLE1_T               	cvd_intr1_enable1               ;	// 0x000c : ''
	L9B0_CVD_INTR1_INTR1_T                 	cvd_intr1_intr1                 ;	// 0x0010 : ''
	L9B0_CVD_INTR1_CLEAR1_T                	cvd_intr1_clear1                ;	// 0x0014 : ''
	L9B0_CVD_INTR2_ENABLE0_T               	cvd_intr2_enable0               ;	// 0x0018 : ''
	L9B0_CVD_INTR2_INTR0_T                 	cvd_intr2_intr0                 ;	// 0x001c : ''
	L9B0_CVD_INTR2_CLEAR0_T                	cvd_intr2_clear0                ;	// 0x0020 : ''
	L9B0_CVD_INTR2_ENABLE1_T               	cvd_intr2_enable1               ;	// 0x0024 : ''
	L9B0_CVD_INTR2_INTR1_T                 	cvd_intr2_intr1                 ;	// 0x0028 : ''
	L9B0_CVD_INTR2_CLEAR1_T                	cvd_intr2_clear1                ;	// 0x002c : ''
	L9B0_TOP_001_T                         	top_001                         ;	// 0x0030 : ''
	UINT32                          	                 __rsvd_01[   3];	// 0x0034 ~ 0x003c
	L9B0_IRIS_MIF_GMUA_001_T               	iris_mif_gmua_001               ;	// 0x0040 : ''
	L9B0_IRIS_MIF_GMUA_002_T               	iris_mif_gmua_002               ;	// 0x0044 : ''
	L9B0_IRIS_MIF_GMUA_003_T               	iris_mif_gmua_003               ;	// 0x0048 : ''
	L9B0_IRIS_MIF_GMUA_004_T               	iris_mif_gmua_004               ;	// 0x004c : ''
	L9B0_IRIS_MIF_GMUA_005_T               	iris_mif_gmua_005               ;	// 0x0050 : ''
	L9B0_IRIS_MIF_GMUA_006_T               	iris_mif_gmua_006               ;	// 0x0054 : ''
	L9B0_IRIS_MIF_GMUA_007_T               	iris_mif_gmua_007               ;	// 0x0058 : ''
	UINT32                          	                 __rsvd_02[   1];	// 0x005c
	L9B0_IRIS_MIF_GMUA_008_T               	iris_mif_gmua_008               ;	// 0x0060 : ''
	L9B0_IRIS_MIF_GMUA_009_T               	iris_mif_gmua_009               ;	// 0x0064 : ''
	L9B0_FASTBLANK_001_T                   	fastblank_001                   ;	// 0x0068 : ''
	L9B0_FASTBLANK_002_T                   	fastblank_002                   ;	// 0x006c : ''
	L9B0_FASTBLANK_003_T                   	fastblank_003                   ;	// 0x0070 : ''
	L9B0_FASTBLANK_004_T                   	fastblank_004                   ;	// 0x0074 : ''
	L9B0_FASTBLANK_005_T                   	fastblank_005                   ;	// 0x0078 : ''
	L9B0_FASTBLANK_006_T                   	fastblank_006                   ;	// 0x007c : ''
	L9B0_FASTBLANK_007_T                   	fastblank_007                   ;	// 0x0080 : ''
	L9B0_FASTBLANK_008_T                   	fastblank_008                   ;	// 0x0084 : ''
	L9B0_FASTBLANK_009_T                   	fastblank_009                   ;	// 0x0088 : ''
	L9B0_FASTBLANK_010_T                   	fastblank_010                   ;	// 0x008c : ''
	L9B0_IRIS_DE_CTRL_001_T                	iris_de_ctrl_001                ;	// 0x0090 : ''
	L9B0_VBI_CTRL_001_T                    	vbi_ctrl_001                    ;	// 0x0094 : ''
	L9B0_VBI_CTRL_002_T                    	vbi_ctrl_002                    ;	// 0x0098 : ''
	L9B0_VBI_CTRL_003_T                    	vbi_ctrl_003                    ;	// 0x009c : ''
	L9B0_VBI_CTRL_004_T                    	vbi_ctrl_004                    ;	// 0x00a0 : ''
	L9B0_VBI_CTRL_005_T                    	vbi_ctrl_005                    ;	// 0x00a4 : ''
	L9B0_VBI_CTRL_006_T                    	vbi_ctrl_006                    ;	// 0x00a8 : ''
	L9B0_VBI_CTRL_007_T                    	vbi_ctrl_007                    ;	// 0x00ac : ''
	L9B0_VBI_CTRL_008_T                    	vbi_ctrl_008                    ;	// 0x00b0 : ''
	L9B0_VBI_CTRL_009_T                    	vbi_ctrl_009                    ;	// 0x00b4 : ''
	UINT32                          	                 __rsvd_03[   2];	// 0x00b8 ~ 0x00bc
	L9B0_FAST_BLANK_STATUS_001_T           	fast_blank_status_001           ;	// 0x00c0 : ''
	L9B0_CVBSAFE_001_T                     	cvbsafe_001                     ;	// 0x00c4 : ''
	UINT32                          	                 __rsvd_04[  14];	// 0x00c8 ~ 0x00fc
	L9B0_IRIS_001_T                        	iris_001                        ;	// 0x0100 : ''
	L9B0_IRIS_002_T                        	iris_002                        ;	// 0x0104 : ''
	L9B0_IRIS_003_T                        	iris_003                        ;	// 0x0108 : ''
	L9B0_IRIS_004_T                        	iris_004                        ;	// 0x010c : ''
	L9B0_IRIS_005_T                        	iris_005                        ;	// 0x0110 : ''
	L9B0_IRIS_006_T                        	iris_006                        ;	// 0x0114 : ''
	L9B0_IRIS_007_T                        	iris_007                        ;	// 0x0118 : ''
	L9B0_IRIS_008_T                        	iris_008                        ;	// 0x011c : ''
	L9B0_IRIS_009_T                        	iris_009                        ;	// 0x0120 : ''
	L9B0_IRIS_010_T                        	iris_010                        ;	// 0x0124 : ''
	UINT32                          	                 __rsvd_05[   2];	// 0x0128 ~ 0x012c
	L9B0_IRIS_011_T                        	iris_011                        ;	// 0x0130 : ''
	L9B0_IRIS_012_T                        	iris_012                        ;	// 0x0134 : ''
	L9B0_IRIS_013_T                        	iris_013                        ;	// 0x0138 : ''
	L9B0_IRIS_014_T                        	iris_014                        ;	// 0x013c : ''
	L9B0_IRIS_015_T                        	iris_015                        ;	// 0x0140 : ''
	L9B0_IRIS_016_T                        	iris_016                        ;	// 0x0144 : ''
	L9B0_IRIS_017_T                        	iris_017                        ;	// 0x0148 : ''
	L9B0_IRIS_018_T                        	iris_018                        ;	// 0x014c : ''
	L9B0_IRIS_019_T                        	iris_019                        ;	// 0x0150 : ''
	L9B0_IRIS_020_T                        	iris_020                        ;	// 0x0154 : ''
	L9B0_IRIS_021_T                        	iris_021                        ;	// 0x0158 : ''
	L9B0_IRIS_022_T                        	iris_022                        ;	// 0x015c : ''
	L9B0_IRIS_023_T                        	iris_023                        ;	// 0x0160 : ''
	L9B0_IRIS_024_T                        	iris_024                        ;	// 0x0164 : ''
	L9B0_IRIS_025_T                        	iris_025                        ;	// 0x0168 : ''
	L9B0_IRIS_026_T                        	iris_026                        ;	// 0x016c : ''
	L9B0_IRIS_027_T                        	iris_027                        ;	// 0x0170 : ''
	L9B0_IRIS_028_T                        	iris_028                        ;	// 0x0174 : ''
	L9B0_IRIS_029_T                        	iris_029                        ;	// 0x0178 : ''
	L9B0_IRIS_030_T                        	iris_030                        ;	// 0x017c : ''
	L9B0_IRIS_031_T                        	iris_031                        ;	// 0x0180 : ''
	L9B0_IRIS_032_T                        	iris_032                        ;	// 0x0184 : ''
	L9B0_IRIS_033_T                        	iris_033                        ;	// 0x0188 : ''
	L9B0_IRIS_034_T                        	iris_034                        ;	// 0x018c : ''
	L9B0_IRIS_035_T                        	iris_035                        ;	// 0x0190 : ''
	L9B0_IRIS_036_T                        	iris_036                        ;	// 0x0194 : ''
	L9B0_IRIS_037_T                        	iris_037                        ;	// 0x0198 : ''
	L9B0_IRIS_038_T                        	iris_038                        ;	// 0x019c : ''
	L9B0_IRIS_039_T                        	iris_039                        ;	// 0x01a0 : ''
	L9B0_IRIS_040_T                        	iris_040                        ;	// 0x01a4 : ''
	L9B0_IRIS_041_T                        	iris_041                        ;	// 0x01a8 : ''
	L9B0_IRIS_042_T                        	iris_042                        ;	// 0x01ac : ''
	L9B0_IRIS_043_T                        	iris_043                        ;	// 0x01b0 : ''
	L9B0_IRIS_044_T                        	iris_044                        ;	// 0x01b4 : ''
	L9B0_IRIS_045_T                        	iris_045                        ;	// 0x01b8 : ''
	L9B0_IRIS_046_T                        	iris_046                        ;	// 0x01bc : ''
	L9B0_IRIS_047_T                        	iris_047                        ;	// 0x01c0 : ''
	L9B0_IRIS_048_T                        	iris_048                        ;	// 0x01c4 : ''
	L9B0_IRIS_049_T                        	iris_049                        ;	// 0x01c8 : ''
	L9B0_IRIS_050_T                        	iris_050                        ;	// 0x01cc : ''
	L9B0_IRIS_051_T                        	iris_051                        ;	// 0x01d0 : ''
	L9B0_IRIS_052_T                        	iris_052                        ;	// 0x01d4 : ''
	L9B0_IRIS_053_T                        	iris_053                        ;	// 0x01d8 : ''
	L9B0_IRIS_054_T                        	iris_054                        ;	// 0x01dc : ''
	L9B0_IRIS_055_T                        	iris_055                        ;	// 0x01e0 : ''
	L9B0_IRIS_056_T                        	iris_056                        ;	// 0x01e4 : ''
	L9B0_IRIS_057_T                        	iris_057                        ;	// 0x01e8 : ''
	L9B0_IRIS_058_T                        	iris_058                        ;	// 0x01ec : ''
	L9B0_IRIS_059_T                        	iris_059                        ;	// 0x01f0 : ''
	L9B0_IRIS_060_T                        	iris_060                        ;	// 0x01f4 : ''
	L9B0_IRIS_061_T                        	iris_061                        ;	// 0x01f8 : ''
	L9B0_IRIS_062_T                        	iris_062                        ;	// 0x01fc : ''
	L9B0_IRIS_063_T                        	iris_063                        ;	// 0x0200 : ''
	L9B0_IRIS_064_T                        	iris_064                        ;	// 0x0204 : ''
	L9B0_IRIS_065_T                        	iris_065                        ;	// 0x0208 : ''
	L9B0_IRIS_066_T                        	iris_066                        ;	// 0x020c : ''
	L9B0_IRIS_067_T                        	iris_067                        ;	// 0x0210 : ''
	L9B0_IRIS_068_T                        	iris_068                        ;	// 0x0214 : ''
	L9B0_IRIS_069_T                        	iris_069                        ;	// 0x0218 : ''
	L9B0_IRIS_070_T                        	iris_070                        ;	// 0x021c : ''
	L9B0_IRIS_071_T                        	iris_071                        ;	// 0x0220 : ''
	L9B0_IRIS_072_T                        	iris_072                        ;	// 0x0224 : ''
	L9B0_IRIS_073_T                        	iris_073                        ;	// 0x0228 : ''
	L9B0_IRIS_074_T                        	iris_074                        ;	// 0x022c : ''
	L9B0_IRIS_075_T                        	iris_075                        ;	// 0x0230 : ''
	L9B0_IRIS_076_T                        	iris_076                        ;	// 0x0234 : ''
	L9B0_IRIS_077_T                        	iris_077                        ;	// 0x0238 : ''
	L9B0_IRIS_078_T                        	iris_078                        ;	// 0x023c : ''
	L9B0_IRIS_079_T                        	iris_079                        ;	// 0x0240 : ''
	L9B0_IRIS_080_T                        	iris_080                        ;	// 0x0244 : ''
	L9B0_IRIS_081_T                        	iris_081                        ;	// 0x0248 : ''
	L9B0_IRIS_082_T                        	iris_082                        ;	// 0x024c : ''
	L9B0_IRIS_083_T                        	iris_083                        ;	// 0x0250 : ''
	L9B0_IRIS_084_T                        	iris_084                        ;	// 0x0254 : ''
	L9B0_IRIS_085_T                        	iris_085                        ;	// 0x0258 : ''
	L9B0_IRIS_086_T                        	iris_086                        ;	// 0x025c : ''
	L9B0_IRIS_087_T                        	iris_087                        ;	// 0x0260 : ''
	L9B0_IRIS_088_T                        	iris_088                        ;	// 0x0264 : ''
	L9B0_IRIS_089_T                        	iris_089                        ;	// 0x0268 : ''
	L9B0_IRIS_090_T                        	iris_090                        ;	// 0x026c : ''
	L9B0_IRIS_091_T                        	iris_091                        ;	// 0x0270 : ''
	L9B0_IRIS_092_T                        	iris_092                        ;	// 0x0274 : ''
	L9B0_IRIS_093_T                        	iris_093                        ;	// 0x0278 : ''
	L9B0_IRIS_094_T                        	iris_094                        ;	// 0x027c : ''
	L9B0_IRIS_095_T                        	iris_095                        ;	// 0x0280 : ''
	L9B0_IRIS_096_T                        	iris_096                        ;	// 0x0284 : ''
	L9B0_IRIS_097_T                        	iris_097                        ;	// 0x0288 : ''
	L9B0_IRIS_098_T                        	iris_098                        ;	// 0x028c : ''
	L9B0_IRIS_099_T                        	iris_099                        ;	// 0x0290 : ''
	L9B0_IRIS_100_T                        	iris_100                        ;	// 0x0294 : ''
	L9B0_IRIS_101_T                        	iris_101                        ;	// 0x0298 : ''
	L9B0_IRIS_102_T                        	iris_102                        ;	// 0x029c : ''
	L9B0_IRIS_103_T                        	iris_103                        ;	// 0x02a0 : ''
	L9B0_IRIS_104_T                        	iris_104                        ;	// 0x02a4 : ''
	L9B0_IRIS_105_T                        	iris_105                        ;	// 0x02a8 : ''
	L9B0_IRIS_106_T                        	iris_106                        ;	// 0x02ac : ''
	L9B0_IRIS_107_T                        	iris_107                        ;	// 0x02b0 : ''
	L9B0_IRIS_108_T                        	iris_108                        ;	// 0x02b4 : ''
	L9B0_IRIS_109_T                        	iris_109                        ;	// 0x02b8 : ''
	L9B0_IRIS_110_T                        	iris_110                        ;	// 0x02bc : ''
	L9B0_IRIS_111_T                        	iris_111                        ;	// 0x02c0 : ''
	L9B0_IRIS_112_T                        	iris_112                        ;	// 0x02c4 : ''
	L9B0_IRIS_113_T                        	iris_113                        ;	// 0x02c8 : ''
	L9B0_IRIS_114_T                        	iris_114                        ;	// 0x02cc : ''
	L9B0_IRIS_115_T                        	iris_115                        ;	// 0x02d0 : ''
	L9B0_IRIS_116_T                        	iris_116                        ;	// 0x02d4 : ''
	L9B0_IRIS_117_T                        	iris_117                        ;	// 0x02d8 : ''
	L9B0_IRIS_118_T                        	iris_118                        ;	// 0x02dc : ''
	L9B0_IRIS_119_T                        	iris_119                        ;	// 0x02e0 : ''
	L9B0_IRIS_120_T                        	iris_120                        ;	// 0x02e4 : ''
	L9B0_IRIS_121_T                        	iris_121                        ;	// 0x02e8 : ''
	L9B0_IRIS_122_T                        	iris_122                        ;	// 0x02ec : ''
	L9B0_IRIS_123_T                        	iris_123                        ;	// 0x02f0 : ''
	L9B0_IRIS_124_T                        	iris_124                        ;	// 0x02f4 : ''
	L9B0_IRIS_125_T                        	iris_125                        ;	// 0x02f8 : ''
	L9B0_IRIS_126_T                        	iris_126                        ;	// 0x02fc : ''
	L9B0_IRIS_127_T                        	iris_127                        ;	// 0x0300 : ''
	L9B0_IRIS_128_T                        	iris_128                        ;	// 0x0304 : ''
	L9B0_IRIS_129_T                        	iris_129                        ;	// 0x0308 : ''
	L9B0_IRIS_130_T                        	iris_130                        ;	// 0x030c : ''
	L9B0_IRIS_131_T                        	iris_131                        ;	// 0x0310 : ''
	L9B0_IRIS_132_T                        	iris_132                        ;	// 0x0314 : ''
	L9B0_IRIS_133_T                        	iris_133                        ;	// 0x0318 : ''
	L9B0_IRIS_134_T                        	iris_134                        ;	// 0x031c : ''
	L9B0_IRIS_135_T                        	iris_135                        ;	// 0x0320 : ''
	L9B0_IRIS_136_T                        	iris_136                        ;	// 0x0324 : ''
	L9B0_IRIS_137_T                        	iris_137                        ;	// 0x0328 : ''
	L9B0_IRIS_138_T                        	iris_138                        ;	// 0x032c : ''
	L9B0_IRIS_139_T                        	iris_139                        ;	// 0x0330 : ''
	L9B0_IRIS_140_T                        	iris_140                        ;	// 0x0334 : ''
	L9B0_IRIS_141_T                        	iris_141                        ;	// 0x0338 : ''
	L9B0_IRIS_142_T                        	iris_142                        ;	// 0x033c : ''
	L9B0_IRIS_143_T                        	iris_143                        ;	// 0x0340 : ''
	L9B0_IRIS_144_T                        	iris_144                        ;	// 0x0344 : ''
	L9B0_IRIS_145_T                        	iris_145                        ;	// 0x0348 : ''
	L9B0_IRIS_146_T                        	iris_146                        ;	// 0x034c : ''
	L9B0_IRIS_147_T                        	iris_147                        ;	// 0x0350 : ''
	L9B0_IRIS_148_T                        	iris_148                        ;	// 0x0354 : ''
	L9B0_IRIS_149_T                        	iris_149                        ;	// 0x0358 : ''
	L9B0_IRIS_150_T                        	iris_150                        ;	// 0x035c : ''
	L9B0_IRIS_151_T                        	iris_151                        ;	// 0x0360 : ''
	L9B0_IRIS_152_T                        	iris_152                        ;	// 0x0364 : ''
	L9B0_IRIS_153_T                        	iris_153                        ;	// 0x0368 : ''
	L9B0_IRIS_154_T                        	iris_154                        ;	// 0x036c : ''
	L9B0_IRIS_155_T                        	iris_155                        ;	// 0x0370 : ''
	L9B0_IRIS_156_T                        	iris_156                        ;	// 0x0374 : ''
	L9B0_IRIS_157_T                        	iris_157                        ;	// 0x0378 : ''
	L9B0_IRIS_158_T                        	iris_158                        ;	// 0x037c : ''
	L9B0_IRIS_159_T                        	iris_159                        ;	// 0x0380 : ''
	L9B0_IRIS_160_T                        	iris_160                        ;	// 0x0384 : ''
	L9B0_IRIS_161_T                        	iris_161                        ;	// 0x0388 : ''
	L9B0_IRIS_162_T                        	iris_162                        ;	// 0x038c : ''
	L9B0_IRIS_163_T                        	iris_163                        ;	// 0x0390 : ''
	L9B0_IRIS_164_T                        	iris_164                        ;	// 0x0394 : ''
	L9B0_IRIS_165_T                        	iris_165                        ;	// 0x0398 : ''
	L9B0_IRIS_166_T                        	iris_166                        ;	// 0x039c : ''
	L9B0_IRIS_167_T                        	iris_167                        ;	// 0x03a0 : ''
	L9B0_IRIS_168_T                        	iris_168                        ;	// 0x03a4 : ''
	L9B0_IRIS_169_T                        	iris_169                        ;	// 0x03a8 : ''
	L9B0_IRIS_170_T                        	iris_170                        ;	// 0x03ac : ''
	L9B0_IRIS_171_T                        	iris_171                        ;	// 0x03b0 : ''
	L9B0_IRIS_172_T                        	iris_172                        ;	// 0x03b4 : ''
	L9B0_IRIS_173_T                        	iris_173                        ;	// 0x03b8 : ''
	L9B0_IRIS_174_T                        	iris_174                        ;	// 0x03bc : ''
	L9B0_IRIS_175_T                        	iris_175                        ;	// 0x03c0 : ''
	L9B0_IRIS_176_T                        	iris_176                        ;	// 0x03c4 : ''
	L9B0_IRIS_177_T                        	iris_177                        ;	// 0x03c8 : ''
	L9B0_IRIS_178_T                        	iris_178                        ;	// 0x03cc : ''
	L9B0_IRIS_179_T                        	iris_179                        ;	// 0x03d0 : ''
	L9B0_IRIS_180_T                        	iris_180                        ;	// 0x03d4 : ''
	L9B0_IRIS_181_T                        	iris_181                        ;	// 0x03d8 : ''
	L9B0_IRIS_182_T                        	iris_182                        ;	// 0x03dc : ''
	L9B0_IRIS_183_T                        	iris_183                        ;	// 0x03e0 : ''
	L9B0_IRIS_184_T                        	iris_184                        ;	// 0x03e4 : ''
	L9B0_IRIS_185_T                        	iris_185                        ;	// 0x03e8 : ''
	L9B0_IRIS_186_T                        	iris_186                        ;	// 0x03ec : ''
	L9B0_IRIS_187_T                        	iris_187                        ;	// 0x03f0 : ''
	L9B0_IRIS_188_T                        	iris_188                        ;	// 0x03f4 : ''
	L9B0_IRIS_189_T                        	iris_189                        ;	// 0x03f8 : ''
	L9B0_IRIS_190_T                        	iris_190                        ;	// 0x03fc : ''
	L9B0_IRIS_191_T                        	iris_191                        ;	// 0x0400 : ''
	L9B0_IRIS_192_T                        	iris_192                        ;	// 0x0404 : ''
	L9B0_IRIS_193_T                        	iris_193                        ;	// 0x0408 : ''
	L9B0_IRIS_194_T                        	iris_194                        ;	// 0x040c : ''
	L9B0_IRIS_195_T                        	iris_195                        ;	// 0x0410 : ''
	L9B0_IRIS_196_T                        	iris_196                        ;	// 0x0414 : ''
	L9B0_IRIS_197_T                        	iris_197                        ;	// 0x0418 : ''
	L9B0_IRIS_198_T                        	iris_198                        ;	// 0x041c : ''
	L9B0_IRIS_199_T                        	iris_199                        ;	// 0x0420 : ''
	L9B0_IRIS_200_T                        	iris_200                        ;	// 0x0424 : ''
	L9B0_IRIS_201_T                        	iris_201                        ;	// 0x0428 : ''
	L9B0_IRIS_202_T                        	iris_202                        ;	// 0x042c : ''
	L9B0_IRIS_203_T                        	iris_203                        ;	// 0x0430 : ''
	L9B0_IRIS_204_T                        	iris_204                        ;	// 0x0434 : ''
	L9B0_IRIS_205_T                        	iris_205                        ;	// 0x0438 : ''
	L9B0_IRIS_206_T                        	iris_206                        ;	// 0x043c : ''
	L9B0_IRIS_207_T                        	iris_207                        ;	// 0x0440 : ''
	L9B0_IRIS_208_T                        	iris_208                        ;	// 0x0444 : ''
	L9B0_IRIS_209_T                        	iris_209                        ;	// 0x0448 : ''
	L9B0_IRIS_210_T                        	iris_210                        ;	// 0x044c : ''
	L9B0_IRIS_211_T                        	iris_211                        ;	// 0x0450 : ''
	L9B0_IRIS_212_T                        	iris_212                        ;	// 0x0454 : ''
	L9B0_IRIS_213_T                        	iris_213                        ;	// 0x0458 : ''
	L9B0_IRIS_214_T                        	iris_214                        ;	// 0x045c : ''
	L9B0_IRIS_215_T                        	iris_215                        ;	// 0x0460 : ''
	L9B0_IRIS_216_T                        	iris_216                        ;	// 0x0464 : ''
	L9B0_IRIS_217_T                        	iris_217                        ;	// 0x0468 : ''
	L9B0_IRIS_218_T                        	iris_218                        ;	// 0x046c : ''
	L9B0_IRIS_219_T                        	iris_219                        ;	// 0x0470 : ''
	L9B0_IRIS_220_T                        	iris_220                        ;	// 0x0474 : ''
	L9B0_IRIS_221_T                        	iris_221                        ;	// 0x0478 : ''
	L9B0_IRIS_222_T                        	iris_222                        ;	// 0x047c : ''
	L9B0_IRIS_223_T                        	iris_223                        ;	// 0x0480 : ''
	L9B0_IRIS_224_T                        	iris_224                        ;	// 0x0484 : ''
	L9B0_IRIS_225_T                        	iris_225                        ;	// 0x0488 : ''
	L9B0_IRIS_226_T                        	iris_226                        ;	// 0x048c : ''
	L9B0_IRIS_227_T                        	iris_227                        ;	// 0x0490 : ''
	L9B0_IRIS_228_T                        	iris_228                        ;	// 0x0494 : ''
	L9B0_IRIS_229_T                        	iris_229                        ;	// 0x0498 : ''
	L9B0_IRIS_230_T                        	iris_230                        ;	// 0x049c : ''
	L9B0_IRIS_231_T                        	iris_231                        ;	// 0x04a0 : ''
	L9B0_IRIS_232_T                        	iris_232                        ;	// 0x04a4 : ''
	L9B0_IRIS_233_T                        	iris_233                        ;	// 0x04a8 : ''
	L9B0_IRIS_234_T                        	iris_234                        ;	// 0x04ac : ''
	L9B0_IRIS_235_T                        	iris_235                        ;	// 0x04b0 : ''
	L9B0_IRIS_236_T                        	iris_236                        ;	// 0x04b4 : ''
	L9B0_IRIS_237_T                        	iris_237                        ;	// 0x04b8 : ''
	L9B0_IRIS_238_T                        	iris_238                        ;	// 0x04bc : ''
	L9B0_IRIS_239_T                        	iris_239                        ;	// 0x04c0 : ''
	L9B0_IRIS_240_T                        	iris_240                        ;	// 0x04c4 : ''
	L9B0_IRIS_241_T                        	iris_241                        ;	// 0x04c8 : ''
	L9B0_IRIS_242_T                        	iris_242                        ;	// 0x04cc : ''
	L9B0_IRIS_243_T                        	iris_243                        ;	// 0x04d0 : ''
	L9B0_IRIS_244_T                        	iris_244                        ;	// 0x04d4 : ''
	L9B0_IRIS_245_T                        	iris_245                        ;	// 0x04d8 : ''
	L9B0_IRIS_246_T                        	iris_246                        ;	// 0x04dc : ''
	L9B0_IRIS_247_T                        	iris_247                        ;	// 0x04e0 : ''
	L9B0_IRIS_248_T                        	iris_248                        ;	// 0x04e4 : ''
	L9B0_IRIS_249_T                        	iris_249                        ;	// 0x04e8 : ''
	L9B0_IRIS_250_T                        	iris_250                        ;	// 0x04ec : ''
	L9B0_IRIS_251_T                        	iris_251                        ;	// 0x04f0 : ''
	L9B0_IRIS_252_T                        	iris_252                        ;	// 0x04f4 : ''
	L9B0_IRIS_253_T                        	iris_253                        ;	// 0x04f8 : ''
	L9B0_IRIS_254_T                        	iris_254                        ;	// 0x04fc : ''
	L9B0_IRIS_255_T                        	iris_255                        ;	// 0x0500 : ''
	L9B0_IRIS_256_T                        	iris_256                        ;	// 0x0504 : ''
	L9B0_IRIS_257_T                        	iris_257                        ;	// 0x0508 : ''
	L9B0_IRIS_258_T                        	iris_258                        ;	// 0x050c : ''
	L9B0_IRIS_259_T                        	iris_259                        ;	// 0x0510 : ''
	L9B0_IRIS_260_T                        	iris_260                        ;	// 0x0514 : ''
	L9B0_IRIS_261_T                        	iris_261                        ;	// 0x0518 : ''
	L9B0_IRIS_262_T                        	iris_262                        ;	// 0x051c : ''
	L9B0_IRIS_263_T                        	iris_263                        ;	// 0x0520 : ''
	L9B0_IRIS_264_T                        	iris_264                        ;	// 0x0524 : ''
	UINT32                          	                 __rsvd_06[   2];	// 0x0528 ~ 0x052c
	L9B0_IRIS_265_T                        	iris_265                        ;	// 0x0530 : ''
	L9B0_IRIS_266_T                        	iris_266                        ;	// 0x0534 : ''
	L9B0_IRIS_267_T                        	iris_267                        ;	// 0x0538 : ''
	L9B0_IRIS_268_T                        	iris_268                        ;	// 0x053c : ''
	L9B0_IRIS_269_T                        	iris_269                        ;	// 0x0540 : ''
	L9B0_IRIS_270_T                        	iris_270                        ;	// 0x0544 : ''
	L9B0_IRIS_271_T                        	iris_271                        ;	// 0x0548 : ''
	L9B0_IRIS_272_T                        	iris_272                        ;	// 0x054c : ''
	L9B0_IRIS_273_T                        	iris_273                        ;	// 0x0550 : ''
	L9B0_IRIS_274_T                        	iris_274                        ;	// 0x0554 : ''
	L9B0_IRIS_275_T                        	iris_275                        ;	// 0x0558 : ''
	L9B0_IRIS_276_T                        	iris_276                        ;	// 0x055c : ''
	L9B0_IRIS_277_T                        	iris_277                        ;	// 0x0560 : ''
	L9B0_IRIS_278_T                        	iris_278                        ;	// 0x0564 : ''
	L9B0_IRIS_279_T                        	iris_279                        ;	// 0x0568 : ''
	L9B0_IRIS_280_T                        	iris_280                        ;	// 0x056c : ''
	L9B0_IRIS_281_T                        	iris_281                        ;	// 0x0570 : ''
	L9B0_IRIS_282_T                        	iris_282                        ;	// 0x0574 : ''
	L9B0_IRIS_283_T                        	iris_283                        ;	// 0x0578 : ''
	L9B0_IRIS_284_T                        	iris_284                        ;	// 0x057c : ''
	L9B0_IRIS_285_T                        	iris_285                        ;	// 0x0580 : ''
	L9B0_IRIS_286_T                        	iris_286                        ;	// 0x0584 : ''
	L9B0_IRIS_287_T                        	iris_287                        ;	// 0x0588 : ''
	L9B0_IRIS_288_T                        	iris_288                        ;	// 0x058c : ''
	L9B0_IRIS_289_T                        	iris_289                        ;	// 0x0590 : ''
	L9B0_IRIS_290_T                        	iris_290                        ;	// 0x0594 : ''
	L9B0_IRIS_291_T                        	iris_291                        ;	// 0x0598 : ''
	L9B0_IRIS_292_T                        	iris_292                        ;	// 0x059c : ''
	L9B0_IRIS_293_T                        	iris_293                        ;	// 0x05a0 : ''
	L9B0_IRIS_294_T                        	iris_294                        ;	// 0x05a4 : ''
	L9B0_IRIS_295_T                        	iris_295                        ;	// 0x05a8 : ''
	L9B0_IRIS_296_T                        	iris_296                        ;	// 0x05ac : ''
	L9B0_IRIS_297_T                        	iris_297                        ;	// 0x05b0 : ''
	L9B0_IRIS_298_T                        	iris_298                        ;	// 0x05b4 : ''
	L9B0_IRIS_299_T                        	iris_299                        ;	// 0x05b8 : ''
	L9B0_IRIS_300_T                        	iris_300                        ;	// 0x05bc : ''
	L9B0_IRIS_301_T                        	iris_301                        ;	// 0x05c0 : ''
	L9B0_IRIS_302_T                        	iris_302                        ;	// 0x05c4 : ''
	L9B0_IRIS_303_T                        	iris_303                        ;	// 0x05c8 : ''
	L9B0_IRIS_304_T                        	iris_304                        ;	// 0x05cc : ''
	UINT32                          	                 __rsvd_07[  76];	// 0x05d0 ~ 0x06fc
	L9B0_VBI_001_T                         	vbi_001                         ;	// 0x0700 : ''
	L9B0_VBI_002_T                         	vbi_002                         ;	// 0x0704 : ''
	L9B0_VBI_003_T                         	vbi_003                         ;	// 0x0708 : ''
	L9B0_VBI_004_T                         	vbi_004                         ;	// 0x070c : ''
	L9B0_VBI_005_T                         	vbi_005                         ;	// 0x0710 : ''
	L9B0_VBI_006_T                         	vbi_006                         ;	// 0x0714 : ''
	L9B0_VBI_007_T                         	vbi_007                         ;	// 0x0718 : ''
	L9B0_VBI_008_T                         	vbi_008                         ;	// 0x071c : ''
	L9B0_VBI_009_T                         	vbi_009                         ;	// 0x0720 : ''
	L9B0_VBI_010_T                         	vbi_010                         ;	// 0x0724 : ''
	L9B0_VBI_011_T                         	vbi_011                         ;	// 0x0728 : ''
	L9B0_VBI_012_T                         	vbi_012                         ;	// 0x072c : ''
	L9B0_VBI_013_T                         	vbi_013                         ;	// 0x0730 : ''
	L9B0_VBI_014_T                         	vbi_014                         ;	// 0x0734 : ''
	L9B0_VBI_015_T                         	vbi_015                         ;	// 0x0738 : ''
	L9B0_VBI_016_T                         	vbi_016                         ;	// 0x073c : ''
	L9B0_VBI_017_T                         	vbi_017                         ;	// 0x0740 : ''
	L9B0_VBI_018_T                         	vbi_018                         ;	// 0x0744 : ''
	L9B0_VBI_019_T                         	vbi_019                         ;	// 0x0748 : ''
	L9B0_VBI_020_T                         	vbi_020                         ;	// 0x074c : ''
	L9B0_VBI_021_T                         	vbi_021                         ;	// 0x0750 : ''
	L9B0_VBI_022_T                         	vbi_022                         ;	// 0x0754 : ''
	L9B0_VBI_023_T                         	vbi_023                         ;	// 0x0758 : ''
	L9B0_VBI_024_T                         	vbi_024                         ;	// 0x075c : ''
	UINT32                          	                 __rsvd_08[  18];	// 0x0760 ~ 0x07a4
	L9B0_IRIS_MIF_GMUA_MON_001_T           	iris_mif_gmua_mon_001           ;	// 0x07a8 : ''
	L9B0_IRIS_MIF_GMUA_MON_002_T           	iris_mif_gmua_mon_002           ;	// 0x07ac : ''
	L9B0_IRIS_MIF_GMUA_MON_003_T           	iris_mif_gmua_mon_003           ;	// 0x07b0 : ''
	L9B0_IRIS_MIF_GMUA_MON_004_T           	iris_mif_gmua_mon_004           ;	// 0x07b4 : ''
	L9B0_IRIS_MIF_GMUA_MON_005_T           	iris_mif_gmua_mon_005           ;	// 0x07b8 : ''
	L9B0_IRIS_MIF_GMUA_MON_006_T           	iris_mif_gmua_mon_006           ;	// 0x07bc : ''
	L9B0_IRIS_MIF_GMUA_MON_007_T           	iris_mif_gmua_mon_007           ;	// 0x07c0 : ''
	L9B0_IRIS_MIF_GMUA_MON_008_T           	iris_mif_gmua_mon_008           ;	// 0x07c4 : ''
	L9B0_IRIS_MIF_GMUA_MON_009_T           	iris_mif_gmua_mon_009           ;	// 0x07c8 : ''
	L9B0_IRIS_MIF_GMUA_MON_010_T           	iris_mif_gmua_mon_010           ;	// 0x07cc : ''
	L9B0_IRIS_MIF_GMUA_MON_011_T           	iris_mif_gmua_mon_011           ;	// 0x07d0 : ''
	L9B0_IRIS_MIF_GMUA_MON_012_T           	iris_mif_gmua_mon_012           ;	// 0x07d4 : ''
	L9B0_IRIS_MIF_GMUA_MON_013_T           	iris_mif_gmua_mon_013           ;	// 0x07d8 : ''
	L9B0_IRIS_MIF_GMUA_MON_014_T           	iris_mif_gmua_mon_014           ;	// 0x07dc : ''
	L9B0_IRIS_MIF_GMUA_MON_015_T           	iris_mif_gmua_mon_015           ;	// 0x07e0 : ''
	L9B0_IRIS_MIF_GMUA_MON_016_T           	iris_mif_gmua_mon_016           ;	// 0x07e4 : ''
	L9B0_IRIS_MIF_GMUA_MON_017_T           	iris_mif_gmua_mon_017           ;	// 0x07e8 : ''
	L9B0_IRIS_MIF_GMUA_MON_018_T           	iris_mif_gmua_mon_018           ;	// 0x07ec : ''
	L9B0_IRIS_MIF_GMUA_MON_019_T           	iris_mif_gmua_mon_019           ;	// 0x07f0 : ''
	L9B0_IRIS_MIF_GMUA_MON_020_T           	iris_mif_gmua_mon_020           ;	// 0x07f4 : ''
	L9B0_IRIS_MIF_GMUA_MON_021_T           	iris_mif_gmua_mon_021           ;	// 0x07f8 : ''
	L9B0_IRIS_MIF_GMUA_MON_022_T           	iris_mif_gmua_mon_022           ;	// 0x07fc : ''
	L9B0_IRIS_HIF_CTRL0_001_T              	iris_hif_ctrl0_001              ;	// 0x0800 : ''
	L9B0_IRIS_HIF_CTRL1_001_T              	iris_hif_ctrl1_001              ;	// 0x0804 : ''
} VPORT_L9B0_REG_T;
/*
 * @{
 * Naming for register pointer.
 * gpRealRegVPORT_L9B0 : real register of VPORT_L9B0.
 * gpRegVPORT_L9B0     : shadow register.
 *
 * @def VPORT_L9B0_RdFL: Read  FLushing : Shadow <- Real.
 * @def VPORT_L9B0_WrFL: Write FLushing : Shadow -> Real.
 * @def VPORT_L9B0_Rd  : Read  whole register(UINT32) from Shadow register.
 * @def VPORT_L9B0_Wr  : Write whole register(UINT32) from Shadow register.
 * @def VPORT_L9B0_Rd01 ~ VPORT_L9B0_Rdnn: Read  given '01~nn' fields from Shadow register.
 * @def VPORT_L9B0_Wr01 ~ VPORT_L9B0_Wrnn: Write given '01~nn' fields to   Shadow register.
 * */
#define VPORT_L9B0_RdFL(_r)			((gpRegVPORT_L9B0->_r)=(gpRealRegVPORT_L9B0->_r))
#define VPORT_L9B0_WrFL(_r)			((gpRealRegVPORT_L9B0->_r)=(gpRegVPORT_L9B0->_r))

#define VPORT_L9B0_Rd(_r)			*((UINT32*)(&(gpRegVPORT_L9B0->_r)))
#define VPORT_L9B0_Wr(_r,_v)			((VPORT_L9B0_Rd(_r))=((UINT32)(_v)))

#define VPORT_L9B0_Rd01(_r,_f01,_v01)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
								} while(0)

#define VPORT_L9B0_Rd02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
								} while(0)

#define VPORT_L9B0_Rd03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
								} while(0)

#define VPORT_L9B0_Rd04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
								} while(0)

#define VPORT_L9B0_Rd05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
								} while(0)

#define VPORT_L9B0_Rd06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
								} while(0)

#define VPORT_L9B0_Rd07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
								} while(0)

#define VPORT_L9B0_Rd08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
								} while(0)

#define VPORT_L9B0_Rd09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
								} while(0)

#define VPORT_L9B0_Rd10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9B0->_r._f10);				\
								} while(0)

#define VPORT_L9B0_Rd11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9B0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9B0->_r._f11);				\
								} while(0)

#define VPORT_L9B0_Rd12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9B0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9B0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9B0->_r._f12);				\
								} while(0)

#define VPORT_L9B0_Rd13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9B0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9B0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9B0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9B0->_r._f13);				\
								} while(0)

#define VPORT_L9B0_Rd14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9B0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9B0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9B0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9B0->_r._f13);				\
									(_v14) = (gpRegVPORT_L9B0->_r._f14);				\
								} while(0)

#define VPORT_L9B0_Rd15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9B0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9B0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9B0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9B0->_r._f13);				\
									(_v14) = (gpRegVPORT_L9B0->_r._f14);				\
									(_v15) = (gpRegVPORT_L9B0->_r._f15);				\
								} while(0)

#define VPORT_L9B0_Rd16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(_v01) = (gpRegVPORT_L9B0->_r._f01);				\
									(_v02) = (gpRegVPORT_L9B0->_r._f02);				\
									(_v03) = (gpRegVPORT_L9B0->_r._f03);				\
									(_v04) = (gpRegVPORT_L9B0->_r._f04);				\
									(_v05) = (gpRegVPORT_L9B0->_r._f05);				\
									(_v06) = (gpRegVPORT_L9B0->_r._f06);				\
									(_v07) = (gpRegVPORT_L9B0->_r._f07);				\
									(_v08) = (gpRegVPORT_L9B0->_r._f08);				\
									(_v09) = (gpRegVPORT_L9B0->_r._f09);				\
									(_v10) = (gpRegVPORT_L9B0->_r._f10);				\
									(_v11) = (gpRegVPORT_L9B0->_r._f11);				\
									(_v12) = (gpRegVPORT_L9B0->_r._f12);				\
									(_v13) = (gpRegVPORT_L9B0->_r._f13);				\
									(_v14) = (gpRegVPORT_L9B0->_r._f14);				\
									(_v15) = (gpRegVPORT_L9B0->_r._f15);				\
									(_v16) = (gpRegVPORT_L9B0->_r._f16);				\
								} while(0)


#define VPORT_L9B0_Wr01(_r,_f01,_v01)													\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
								} while(0)

#define VPORT_L9B0_Wr02(_r,_f01,_v01,_f02,_v02)										\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
								} while(0)

#define VPORT_L9B0_Wr03(_r,_f01,_v01,_f02,_v02,_f03,_v03)								\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
								} while(0)

#define VPORT_L9B0_Wr04(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04)					\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
								} while(0)

#define VPORT_L9B0_Wr05(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05)													\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
								} while(0)

#define VPORT_L9B0_Wr06(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06)										\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
								} while(0)

#define VPORT_L9B0_Wr07(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07)								\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
								} while(0)

#define VPORT_L9B0_Wr08(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08)					\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
								} while(0)

#define VPORT_L9B0_Wr09(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09)													\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
								} while(0)

#define VPORT_L9B0_Wr10(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10)										\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9B0->_r._f10) = (_v10);				\
								} while(0)

#define VPORT_L9B0_Wr11(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11)								\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9B0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9B0->_r._f11) = (_v11);				\
								} while(0)

#define VPORT_L9B0_Wr12(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12)					\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9B0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9B0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9B0->_r._f12) = (_v12);				\
								} while(0)

#define VPORT_L9B0_Wr13(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13)													\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9B0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9B0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9B0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9B0->_r._f13) = (_v13);				\
								} while(0)

#define VPORT_L9B0_Wr14(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14)										\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9B0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9B0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9B0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9B0->_r._f13) = (_v13);				\
									(gpRegVPORT_L9B0->_r._f14) = (_v14);				\
								} while(0)

#define VPORT_L9B0_Wr15(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15)								\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9B0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9B0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9B0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9B0->_r._f13) = (_v13);				\
									(gpRegVPORT_L9B0->_r._f14) = (_v14);				\
									(gpRegVPORT_L9B0->_r._f15) = (_v15);				\
								} while(0)

#define VPORT_L9B0_Wr16(_r,_f01,_v01,_f02,_v02,_f03,_v03,_f04,_v04,					\
					_f05,_v05,_f06,_v06,_f07,_v07,_f08,_v08,					\
					_f09,_v09,_f10,_v10,_f11,_v11,_f12,_v12,					\
					_f13,_v13,_f14,_v14,_f15,_v15,_f16,_v16)					\
								do { 											\
									(gpRegVPORT_L9B0->_r._f01) = (_v01);				\
									(gpRegVPORT_L9B0->_r._f02) = (_v02);				\
									(gpRegVPORT_L9B0->_r._f03) = (_v03);				\
									(gpRegVPORT_L9B0->_r._f04) = (_v04);				\
									(gpRegVPORT_L9B0->_r._f05) = (_v05);				\
									(gpRegVPORT_L9B0->_r._f06) = (_v06);				\
									(gpRegVPORT_L9B0->_r._f07) = (_v07);				\
									(gpRegVPORT_L9B0->_r._f08) = (_v08);				\
									(gpRegVPORT_L9B0->_r._f09) = (_v09);				\
									(gpRegVPORT_L9B0->_r._f10) = (_v10);				\
									(gpRegVPORT_L9B0->_r._f11) = (_v11);				\
									(gpRegVPORT_L9B0->_r._f12) = (_v12);				\
									(gpRegVPORT_L9B0->_r._f13) = (_v13);				\
									(gpRegVPORT_L9B0->_r._f14) = (_v14);				\
									(gpRegVPORT_L9B0->_r._f15) = (_v15);				\
									(gpRegVPORT_L9B0->_r._f16) = (_v16);				\
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
 * _rwname: name of rw    bit field : shall be 0 after VPORT_L9B0_Wind(), 1 for VPORT_L9B0_Rind()
 * _iname : name of index bit field
 * _ival  : index value
 * _fname : field name
 * _fval  : field variable that field value shall be stored.
 *
 * VPORT_L9B0_Rind : General indexed register Read.(
 * VPORT_L9B0_Wind : General indexed register Read.
 *
 * VPORT_L9B0_Ridx : For 'index', 'rw', 'load' field name
 * VPORT_L9B0_Widx : For 'index', 'rw', 'load' field name and NO_LOAD.
 */
#define VPORT_L9B0_Rind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
							do {												\
								VPORT_L9B0_Wr03(_r,_lname,0,_rwname,1,_iname,_ival);	\
								VPORT_L9B0_WrFL(_r);									\
								VPORT_L9B0_RdFL(_r);									\
								VPORT_L9B0_Rd01(_r,_fname,_fval);						\
							} while (0)

#define VPORT_L9B0_Wind(_r, _lname, _rwname, _iname, _ival, _fname, _fval)				\
				VPORT_L9B0_Wr04(_r, _lname,0, _rwname,0, _iname,_ival, _fname,_fval)


#define VPORT_L9B0_Ridx(_r, _ival, _fname, _fval)	VPORT_L9B0_Rind(_r,load,rw,index,_ival,_fname,_fval)

#define VPORT_L9B0_Widx(_r, _ival, _fname, _fval)	VPORT_L9B0_Wind(_r,load,rw,index,_ival,_fname,_fval)

#ifdef __cplusplus
}
#endif

#endif	/* _VPORT_REG_L9B0_H_ */

