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
 * main driver implementation for APR device.
 * apr device will teach you how to make device driver with new platform.
 *
 * author     justine.jeong
 * version    1.0
 * date       2012.09.05
 * note       Additional information.
 *
 * @addtogroup APR
 * @{
 */
#ifndef _APR_FD3_REG_H13B0_H_
#define _APR_FD3_REG_H13B0_H_

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
	0x0008 fd3_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	fd3_color_format                : 2,	//  0: 1
	                                : 6,	//  2: 7 reserved
	interlaced                      : 1,	//     8
	                                : 7,	//  9:15 reserved
	vertical_sampling               : 1;	//    16
} FD3_MODE;

/*-----------------------------------------------------------------------------
	0x0010 segmentation_window ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	seg_wnd_boffset                 : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	seg_wnd_toffset                 : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	seg_wnd_roffset                 : 4,	// 16:19
	                                : 4,	// 20:23 reserved
	seg_wnd_loffset                 : 4;	// 24:27
} SEGMENTATION_WINDOW;

/*-----------------------------------------------------------------------------
	0x0020 projection_window ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	proj_wnd_boffset                : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	proj_wnd_toffset                : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	proj_wnd_lroffset               : 4;	// 16:19
} PROJECTION_WINDOW;

/*-----------------------------------------------------------------------------
	0x0024 projection_parameter_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	qdiff                           : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	qmin                            : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	pelcntth                        : 6;	// 16:21
} PROJECTION_PARAMETER_0;

/*-----------------------------------------------------------------------------
	0x0028 projection_parameter_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	vcnt_ratio                      : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	vdcnt_ratio                     : 4;	//  8:11
} PROJECTION_PARAMETER_1;

/*-----------------------------------------------------------------------------
	0x0030 segmentation_threshold_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	seg_ye_gr                       : 8,	//  0: 7
	seg_yellow                      : 8,	//  8:15
	seg_re_ye                       : 8,	// 16:23
	seg_red                         : 8;	// 24:31
} SEGMENTATION_THRESHOLD_0;

/*-----------------------------------------------------------------------------
	0x0034 segmentation_threshold_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	seg_cy_bl                       : 8,	//  0: 7
	seg_cyan                        : 8,	//  8:15
	seg_gr_cy                       : 8,	// 16:23
	seg_green                       : 8;	// 24:31
} SEGMENTATION_THRESHOLD_1;

/*-----------------------------------------------------------------------------
	0x0038 segmentation_threshold_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	seg_ma_re                       : 8,	//  0: 7
	seg_magenta                     : 8,	//  8:15
	seg_bl_ma                       : 8,	// 16:23
	seg_blue                        : 8;	// 24:31
} SEGMENTATION_THRESHOLD_2;

/*-----------------------------------------------------------------------------
	0x003c segmentation_threshold_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	seg_th_sdelta                   : 8,	//  0: 7
	seg_th_saturate                 : 8,	//  8:15
	seg_th_value_black              : 8;	// 16:23
} SEGMENTATION_THRESHOLD_3;

/*-----------------------------------------------------------------------------
	0x0040 subtitle_control ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	sub_vcnt_ratio                  : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	sub_pcnt_ratio                  : 4,	//  8:11
	                                : 4,	// 12:15 reserved
	sub_dif_th                      : 8;	// 16:23
} SUBTITLE_CONTROL;

/*-----------------------------------------------------------------------------
	0x0050 dominant_threshold_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dom_pic_seg_th3                 : 8,	//  0: 7
	dom_pic_seg_th2                 : 8,	//  8:15
	dom_pic_seg_th1                 : 8,	// 16:23
	dom_pic_seg_th0                 : 8;	// 24:31
} DOMINANT_THRESHOLD_0;

/*-----------------------------------------------------------------------------
	0x0054 dominant_threshold_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dom_blk_seg_hsv_th_h            : 8,	//  0: 7
	dom_blk_seg_hsv_th              : 8;	//  8:15
} DOMINANT_THRESHOLD_1;

/*-----------------------------------------------------------------------------
	0x0058 subtitle_threshold ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	subtitle_th                     : 8,	//  0: 7
	                                : 8,	//  8: 15 reserved
	subtitle_alt_seg                : 5;	//  16: 20
} SUBTITLE_THRESHOLD;

/*-----------------------------------------------------------------------------
	0x0060 force_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	force_val_idx                   : 5,	//  0: 4
	                                : 3,	//  5: 7 reserved
	force_hsv_idx1                  : 5,	//  8:12
	                                : 3,	// 13:15 reserved
	force_hsv_idx0                  : 5,	// 16:20
	                                : 3,	// 21:23 reserved
	force_scn_type                  : 1,	//    24
	                                : 3,	// 25:27 reserved
	force_mode                      : 2;	// 28:29
} FORCE_MODE;

/*-----------------------------------------------------------------------------
	0x0080 blk0_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK0_STAT;

/*-----------------------------------------------------------------------------
	0x0084 blk1_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK1_STAT;

/*-----------------------------------------------------------------------------
	0x0088 blk2_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK2_STAT;

/*-----------------------------------------------------------------------------
	0x008c blk3_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK3_STAT;

/*-----------------------------------------------------------------------------
	0x0090 blk4_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK4_STAT;

/*-----------------------------------------------------------------------------
	0x0094 blk5_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK5_STAT;

/*-----------------------------------------------------------------------------
	0x0098 blk6_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK6_STAT;

/*-----------------------------------------------------------------------------
	0x009c blk7_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	standard_dev                    :12,	//  0:11
	                                : 4,	// 12:15 reserved
	mean                            :12;	// 16:27
} BLK7_STAT;

/*-----------------------------------------------------------------------------
	0x00a0 corr_ss_blk_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ss_corr1                        : 8,	//  0: 7
	ss_wnd_info1                    : 2,	//  8: 9
	                                : 6,	// 10:15 reserved
	ss_corr0                        : 8,	// 16:23
	ss_wnd_info0                    : 2;	// 24:25
} CORR_SS_BLK_01;

/*-----------------------------------------------------------------------------
	0x00a4 corr_ss_blk_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	ss_corr3                        : 8,	//  0: 7
	ss_wnd_info3                    : 2,	//  8: 9
	                                : 6,	// 10:15 reserved
	ss_corr2                        : 8,	// 16:23
	ss_wnd_info2                    : 2;	// 24:25
} CORR_SS_BLK_23;

/*-----------------------------------------------------------------------------
	0x00a8 corr_tb_blk_01 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tb_corr1                        : 8,	//  0: 7
	tb_wnd_info1                    : 2,	//  8: 9
	                                : 6,	// 10:15 reserved
	tb_corr0                        : 8,	// 16:23
	tb_wnd_info0                    : 2;	// 24:25
} CORR_TB_BLK_01;

/*-----------------------------------------------------------------------------
	0x00ac corr_tb_blk_23 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	tb_corr3                        : 8,	//  0: 7
	tb_wnd_info3                    : 2,	//  8: 9
	                                : 6,	// 10:15 reserved
	tb_corr2                        : 8,	// 16:23
	tb_wnd_info2                    : 2;	// 24:25
} CORR_TB_BLK_23;

/*-----------------------------------------------------------------------------
	0x00b0 hsv_picture ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_seg_hsv_count               :24,	//  0:23
	pic_seg_hsv_max                 : 8;	// 24:31
} HSV_PICTURE;

/*-----------------------------------------------------------------------------
	0x00b4 val_picture ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	pic_seg_val_count               :24,	//  0:23
	pic_seg_val_max                 : 8;	// 24:31
} VAL_PICTURE;

/*-----------------------------------------------------------------------------
	0x00c0 hsv_blk0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	blk0_seg_hsv_count              :24,	//  0:23
	blk0_seg_hsv_max                : 8;	// 24:31
} HSV_BLK0;

/*-----------------------------------------------------------------------------
	0x00c4 hsv_blk1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	blk1_seg_hsv_count              :24,	//  0:23
	blk1_seg_hsv_max                : 8;	// 24:31
} HSV_BLK1;

/*-----------------------------------------------------------------------------
	0x00c8 hsv_blk2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	blk2_seg_hsv_count              :24,	//  0:23
	blk2_seg_hsv_max                : 8;	// 24:31
} HSV_BLK2;

/*-----------------------------------------------------------------------------
	0x00cc hsv_blk3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	blk3_seg_hsv_count              :24,	//  0:23
	blk3_seg_hsv_max                : 8;	// 24:31
} HSV_BLK3;

/*-----------------------------------------------------------------------------
	0x00d0 status_dominant_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	blk_valid_count                 : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	blk_valid_final                 : 4;	//  8:11
} STATUS_DOMINANT_0;

/*-----------------------------------------------------------------------------
	0x00d4 status_dominant_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	dominant_val_idx                : 8,	//  0: 7
	dominant_hsv_idx1               : 2,	//  8: 9
	                                : 6,	// 10:15 reserved
	dominant_hsv_idx0               : 8,	// 16:23
	scene_type                      : 1;    //    24
} STATUS_DOMINANT_1;

/*-----------------------------------------------------------------------------
	0x00e0 fd3_status_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	end_status                      : 4,	//  0: 3
	                                : 4,	//  4: 7 reserved
	buf_yc_overflow                 : 2,	//  8: 9
	                                : 6,    // 10:15 reserved
	fd3_ib_r_lcnt_p                 :10;    // 16:25
} FD3_STATUS_0;

/*-----------------------------------------------------------------------------
	0x00e4 fd3_status_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32
	suc_fail_history                :24,	//  0:23
	init_count                      : 8;	// 24:31
} FD3_STATUS_1;

typedef struct {
	UINT32                          	                 __rsvd_00[   2];	// 0x0000 ~ 0x0004
	FD3_MODE                        	fd3_mode                        ;	// 0x0008 : ''
	UINT32                          	                 __rsvd_01[   1];	// 0x000c
	SEGMENTATION_WINDOW             	segmentation_window             ;	// 0x0010 : ''
	UINT32                          	                 __rsvd_02[   3];	// 0x0014 ~ 0x001c
	PROJECTION_WINDOW               	projection_window               ;	// 0x0020 : ''
	PROJECTION_PARAMETER_0          	projection_parameter_0          ;	// 0x0024 : ''
	PROJECTION_PARAMETER_1          	projection_parameter_1          ;	// 0x0028 : ''
	UINT32                          	                 __rsvd_03[   1];	// 0x002c
	SEGMENTATION_THRESHOLD_0        	segmentation_threshold_0        ;	// 0x0030 : ''
	SEGMENTATION_THRESHOLD_1        	segmentation_threshold_1        ;	// 0x0034 : ''
	SEGMENTATION_THRESHOLD_2        	segmentation_threshold_2        ;	// 0x0038 : ''
	SEGMENTATION_THRESHOLD_3        	segmentation_threshold_3        ;	// 0x003c : ''
	SUBTITLE_CONTROL                	subtitle_control                ;	// 0x0040 : ''
	UINT32                          	                 __rsvd_04[   3];	// 0x0044 ~ 0x004c
	DOMINANT_THRESHOLD_0            	dominant_threshold_0            ;	// 0x0050 : ''
	DOMINANT_THRESHOLD_1            	dominant_threshold_1            ;	// 0x0054 : ''
	SUBTITLE_THRESHOLD              	subtitle_threshold              ;	// 0x0058 : ''
	UINT32                          	                 __rsvd_05[   1];	// 0x005c
	FORCE_MODE                      	force_mode                      ;	// 0x0060 : ''
	UINT32                          	                 __rsvd_06[   7];	// 0x0064 ~ 0x007c
	BLK0_STAT                       	blk0_stat                       ;	// 0x0080 : ''
	BLK1_STAT                       	blk1_stat                       ;	// 0x0084 : ''
	BLK2_STAT                       	blk2_stat                       ;	// 0x0088 : ''
	BLK3_STAT                       	blk3_stat                       ;	// 0x008c : ''
	BLK4_STAT                       	blk4_stat                       ;	// 0x0090 : ''
	BLK5_STAT                       	blk5_stat                       ;	// 0x0094 : ''
	BLK6_STAT                       	blk6_stat                       ;	// 0x0098 : ''
	BLK7_STAT                       	blk7_stat                       ;	// 0x009c : ''
	CORR_SS_BLK_01                  	corr_ss_blk_01                  ;	// 0x00a0 : ''
	CORR_SS_BLK_23                  	corr_ss_blk_23                  ;	// 0x00a4 : ''
	CORR_TB_BLK_01                  	corr_tb_blk_01                  ;	// 0x00a8 : ''
	CORR_TB_BLK_23                  	corr_tb_blk_23                  ;	// 0x00ac : ''
	HSV_PICTURE                     	hsv_picture                     ;	// 0x00b0 : ''
	VAL_PICTURE                     	val_picture                     ;	// 0x00b4 : ''
	UINT32                          	                 __rsvd_07[   2];	// 0x00b8 ~ 0x00bc
	HSV_BLK0                        	hsv_blk0                        ;	// 0x00c0 : ''
	HSV_BLK1                        	hsv_blk1                        ;	// 0x00c4 : ''
	HSV_BLK2                        	hsv_blk2                        ;	// 0x00c8 : ''
	HSV_BLK3                        	hsv_blk3                        ;	// 0x00cc : ''
	STATUS_DOMINANT_0               	status_dominant_0               ;	// 0x00d0 : ''
	STATUS_DOMINANT_1               	status_dominant_1               ;	// 0x00d4 : ''
	UINT32                          	                 __rsvd_08[   2];	// 0x00d8 ~ 0x00dc
	FD3_STATUS_0                      	fd3_status_0                    ;	// 0x00e0 : ''
	FD3_STATUS_1                      	fd3_status_1                    ;	// 0x00e4 : ''
} APR_FD3_REG_H13B0_T;

#endif	/* _APR_FD3_REG_H13B0_H_ */

