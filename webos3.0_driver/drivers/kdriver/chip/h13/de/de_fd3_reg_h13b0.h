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

#ifndef _REG_FD3_H13B0_h
#define _REG_FD3_H13B0_h

/*-----------------------------------------------------------------------------
	0x0708 fd3_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fd3_color_format        : 2;   //  0: 1
	UINT32                         : 6;   //  2: 7 reserved
	UINT32 interlaced              : 1;   //     8
	UINT32                         : 7;   //  9:15 reserved
	UINT32 vertical_sampling       : 1;   //    16
} H13B0_FD3_MODE_T;

/*-----------------------------------------------------------------------------
	0x0710 segmentation_window ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_wnd_boffset         : 4;   //  0: 3
	UINT32                         : 4;   //  4: 7 reserved
	UINT32 seg_wnd_toffset         : 4;   //  8:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 seg_wnd_roffset         : 4;   // 16:19
	UINT32                         : 4;   // 20:23 reserved
	UINT32 seg_wnd_loffset         : 4;   // 24:27
} H13B0_SEGMENTATION_WINDOW_T;

/*-----------------------------------------------------------------------------
	0x0720 projection_window ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 proj_wnd_boffset        : 4;   //  0: 3
	UINT32                         : 4;   //  4: 7 reserved
	UINT32 proj_wnd_toffset        : 4;   //  8:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 proj_wnd_lroffset       : 4;   // 16:19
} H13B0_PROJECTION_WINDOW_T;

/*-----------------------------------------------------------------------------
	0x0724 projection_parameter_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 qdiff                   : 4;   //  0: 3
	UINT32                         : 4;   //  4: 7 reserved
	UINT32 qmin                    : 4;   //  8:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 valid_line_pel_count    : 6;   // 16:21
} H13B0_PROJECTION_PARAMETER_0_T;

/*-----------------------------------------------------------------------------
	0x0728 projection_parameter_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_ratio              : 4;   //  0: 3
	UINT32                         : 4;   //  4: 7 reserved
	UINT32 vdcnt_ratio             : 4;   //  8:11
} H13B0_PROJECTION_PARAMETER_1_T;

/*-----------------------------------------------------------------------------
	0x0730 segmentation_threshold_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_ye_gr               : 8;   //  0: 7
	UINT32 seg_yellow              : 8;   //  8:15
	UINT32 seg_re_ye               : 8;   // 16:23
	UINT32 seg_red                 : 8;   // 24:31
} H13B0_SEGMENTATION_THRESHOLD_0_T;

/*-----------------------------------------------------------------------------
	0x0734 segmentation_threshold_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_cy_bl               : 8;   //  0: 7
	UINT32 seg_cyan                : 8;   //  8:15
	UINT32 seg_gr_cy               : 8;   // 16:23
	UINT32 seg_green               : 8;   // 24:31
} H13B0_SEGMENTATION_THRESHOLD_1_T;

/*-----------------------------------------------------------------------------
	0x0738 segmentation_threshold_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_ma_re               : 8;   //  0: 7
	UINT32 seg_magenta             : 8;   //  8:15
	UINT32 seg_bl_ma               : 8;   // 16:23
	UINT32 seg_blue                : 8;   // 24:31
} H13B0_SEGMENTATION_THRESHOLD_2_T;

/*-----------------------------------------------------------------------------
	0x073c segmentation_threshold_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_th_sdelta           : 8;   //  0: 7
	UINT32 seg_th_saturate         : 8;   //  8:15
	UINT32 seg_th_value_black      : 8;   // 16:23
} H13B0_SEGMENTATION_THRESHOLD_3_T;

/*-----------------------------------------------------------------------------
	0x0740 subtitle_control ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 sub_vcnt_ratio          : 4;   //  0: 3
	UINT32                         : 4;   //  4: 7 reserved
	UINT32 sub_pcnt_ratio          : 4;   //  8:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 sub_dif_th              : 8;   // 16:23
} H13B0_SUBTITLE_CONTROL_T;

/*-----------------------------------------------------------------------------
	0x0750 dominant_threshold_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dom_pic_seg_th3         : 8;   //  0: 7
	UINT32 dom_pic_seg_th2         : 4;   //  8:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 dom_pic_seg_th1         : 8;   // 16:23
	UINT32 dom_pic_seg_th0         : 8;   // 24:31
} H13B0_DOMINANT_THRESHOLD_0_T;

/*-----------------------------------------------------------------------------
	0x0754 dominant_threshold_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dom_blk_seg_hsv_th      : 8;   //  0: 7
} H13B0_DOMINANT_THRESHOLD_1_T;

/*-----------------------------------------------------------------------------
	0x0758 subtitle_threshold ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 subtitle_th             : 8;   //  0: 7
} H13B0_SUBTITLE_THRESHOLD_T;

/*-----------------------------------------------------------------------------
	0x0760 force_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_val_idx           : 5;   //  0: 4
	UINT32                         : 3;   //  5: 7 reserved
	UINT32 force_hsv_idx1          : 5;   //  8:12
	UINT32                         : 3;   // 13:15 reserved
	UINT32 force_hsv_idx0          : 5;   // 16:20
	UINT32                         : 3;   // 21:23 reserved
	UINT32 force_scn_type          : 1;   //    24
	UINT32                         : 3;   // 25:27 reserved
	UINT32 force_mode              : 1;   //    28
} H13B0_FORCE_MODE_T;

/*-----------------------------------------------------------------------------
	0x0780 blk0_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK0_STAT_T;

/*-----------------------------------------------------------------------------
	0x0784 blk1_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK1_STAT_T;

/*-----------------------------------------------------------------------------
	0x0788 blk2_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK2_STAT_T;

/*-----------------------------------------------------------------------------
	0x078c blk3_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK3_STAT_T;

/*-----------------------------------------------------------------------------
	0x0790 blk4_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK4_STAT_T;

/*-----------------------------------------------------------------------------
	0x0794 blk5_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK5_STAT_T;

/*-----------------------------------------------------------------------------
	0x0798 blk6_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK6_STAT_T;

/*-----------------------------------------------------------------------------
	0x079c blk7_stat ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev            : 12;  //  0:11
	UINT32                         : 4;   // 12:15 reserved
	UINT32 mean                    : 12;  // 16:27
} H13B0_BLK7_STAT_T;

/*-----------------------------------------------------------------------------
	0x07a0 corr_ss_blk0_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_corr1                : 8;   //  0: 7
	UINT32 ss_wnd_info1            : 2;   //  8: 9
	UINT32                         : 6;   // 10:15 reserved
	UINT32 ss_corr0                : 8;   // 16:23
	UINT32 ss_wnd_info0            : 2;   // 24:25
} H13B0_CORR_SS_BLK0_1_T;

/*-----------------------------------------------------------------------------
	0x07a4 corr_ss_blk2_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_corr3                : 8;   //  0: 7
	UINT32 ss_wnd_info3            : 2;   //  8: 9
	UINT32                         : 6;   // 10:15 reserved
	UINT32 ss_corr2                : 8;   // 16:23
	UINT32 ss_wnd_info2            : 2;   // 24:25
} H13B0_CORR_SS_BLK2_3_T;

/*-----------------------------------------------------------------------------
	0x07a8 corr_tb_blk0_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_corr1                : 8;   //  0: 7
	UINT32 tb_wnd_info1            : 2;   //  8: 9
	UINT32                         : 6;   // 10:15 reserved
	UINT32 tb_corr0                : 8;   // 16:23
	UINT32 tb_wnd_info0            : 2;   // 24:25
} H13B0_CORR_TB_BLK0_1_T;

/*-----------------------------------------------------------------------------
	0x07ac corr_tb_blk2_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_corr3                : 8;   //  0: 7
	UINT32 tb_wnd_info3            : 2;   //  8: 9
	UINT32                         : 6;   // 10:15 reserved
	UINT32 tb_corr2                : 8;   // 16:23
	UINT32 tb_wnd_info2            : 2;   // 24:25
} H13B0_CORR_TB_BLK2_3_T;

/*-----------------------------------------------------------------------------
	0x07b0 hsv_picture ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_seg_hsv_count       : 24;  //  0:23
	UINT32 pic_seg_hsv_max         : 8;   // 24:31
} H13B0_HSV_PICTURE_T;

/*-----------------------------------------------------------------------------
	0x07b4 val_picture ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_seg_val_count       : 24;  //  0:23
	UINT32 pic_seg_val_max         : 8;   // 24:31
} H13B0_VAL_PICTURE_T;

/*-----------------------------------------------------------------------------
	0x07c0 hsv_blk0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk0_seg_hsv_count      : 24;  //  0:23
	UINT32 blk0_seg_hsv_max        : 8;   // 24:31
} H13B0_HSV_BLK0_T;

/*-----------------------------------------------------------------------------
	0x07c4 hsv_blk1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk1_seg_hsv_count      : 24;  //  0:23
	UINT32 blk1_seg_hsv_max        : 8;   // 24:31
} H13B0_HSV_BLK1_T;

/*-----------------------------------------------------------------------------
	0x07c8 hsv_blk2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk2_seg_hsv_count      : 24;  //  0:23
	UINT32 blk2_seg_hsv_max        : 8;   // 24:31
} H13B0_HSV_BLK2_T;

/*-----------------------------------------------------------------------------
	0x07cc hsv_blk3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk3_seg_hsv_count      : 24;  //  0:23
	UINT32 blk3_seg_hsv_max        : 8;   // 24:31
} H13B0_HSV_BLK3_T;

/*-----------------------------------------------------------------------------
	0x07d0 status_dominant0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk_valid_count         : 4;   //  0: 3
	UINT32                         : 4;   //  4: 7 reserved
	UINT32 blk_valid_final         : 4;   //  8:11
} H13B0_STATUS_DOMINANT0_T;

/*-----------------------------------------------------------------------------
	0x07d4 status_dominant1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 dominant_val_idx        : 8;   //  0: 7
	UINT32 dominant_hsv_idx1       : 8;   //  8:15
	UINT32 dominant_hsv_idx0       : 8;   // 16:23
} H13B0_STATUS_DOMINANT1_T;

/*-----------------------------------------------------------------------------
	0x07e0 fd3_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 end_status              : 4;   //  0: 3
	UINT32                         : 4;   //  4: 7 reserved
	UINT32 buf_yc_overflow         : 2;   //  8: 9
} H13B0_FD3_STATUS_T;

typedef struct {
	H13B0_FD3_MODE_T                    fd3_mode;                            // 0x0708
	UINT32                              reserved0;                           // 0x070c
	H13B0_SEGMENTATION_WINDOW_T         segmentation_window;                 // 0x0710
	UINT32                              reserved1[3];                        // 0x0714~0x071c
	H13B0_PROJECTION_WINDOW_T           projection_window;                   // 0x0720
	H13B0_PROJECTION_PARAMETER_0_T      projection_parameter_0;              // 0x0724
	H13B0_PROJECTION_PARAMETER_1_T      projection_parameter_1;              // 0x0728
	UINT32                              reserved2;                           // 0x072c
	H13B0_SEGMENTATION_THRESHOLD_0_T    segmentation_threshold_0;            // 0x0730
	H13B0_SEGMENTATION_THRESHOLD_1_T    segmentation_threshold_1;            // 0x0734
	H13B0_SEGMENTATION_THRESHOLD_2_T    segmentation_threshold_2;            // 0x0738
	H13B0_SEGMENTATION_THRESHOLD_3_T    segmentation_threshold_3;            // 0x073c
	H13B0_SUBTITLE_CONTROL_T            subtitle_control;                    // 0x0740
	UINT32                              reserved3[3];                        // 0x0744~0x074c
	H13B0_DOMINANT_THRESHOLD_0_T        dominant_threshold_0;                // 0x0750
	H13B0_DOMINANT_THRESHOLD_1_T        dominant_threshold_1;                // 0x0754
	H13B0_SUBTITLE_THRESHOLD_T          subtitle_threshold;                  // 0x0758
	UINT32                              reserved4;                           // 0x075c
	H13B0_FORCE_MODE_T                  force_mode;                          // 0x0760
	UINT32                              reserved5[7];                        // 0x0764~0x077c
	H13B0_BLK0_STAT_T                   blk0_stat;                           // 0x0780
	H13B0_BLK1_STAT_T                   blk1_stat;                           // 0x0784
	H13B0_BLK2_STAT_T                   blk2_stat;                           // 0x0788
	H13B0_BLK3_STAT_T                   blk3_stat;                           // 0x078c
	H13B0_BLK4_STAT_T                   blk4_stat;                           // 0x0790
	H13B0_BLK5_STAT_T                   blk5_stat;                           // 0x0794
	H13B0_BLK6_STAT_T                   blk6_stat;                           // 0x0798
	H13B0_BLK7_STAT_T                   blk7_stat;                           // 0x079c
	H13B0_CORR_SS_BLK0_1_T              corr_ss_blk0_1;                      // 0x07a0
	H13B0_CORR_SS_BLK2_3_T              corr_ss_blk2_3;                      // 0x07a4
	H13B0_CORR_TB_BLK0_1_T              corr_tb_blk0_1;                      // 0x07a8
	H13B0_CORR_TB_BLK2_3_T              corr_tb_blk2_3;                      // 0x07ac
	H13B0_HSV_PICTURE_T                 hsv_picture;                         // 0x07b0
	H13B0_VAL_PICTURE_T                 val_picture;                         // 0x07b4
	UINT32                              reserved6[2];                        // 0x07b8~0x07bc
	H13B0_HSV_BLK0_T                    hsv_blk0;                            // 0x07c0
	H13B0_HSV_BLK1_T                    hsv_blk1;                            // 0x07c4
	H13B0_HSV_BLK2_T                    hsv_blk2;                            // 0x07c8
	H13B0_HSV_BLK3_T                    hsv_blk3;                            // 0x07cc
	H13B0_STATUS_DOMINANT0_T            status_dominant0;                    // 0x07d0
	H13B0_STATUS_DOMINANT1_T            status_dominant1;                    // 0x07d4
	UINT32                              reserved7[2];                        // 0x07d8~0x07dc
	H13B0_FD3_STATUS_T                  fd3_status;                          // 0x07e0
} DE_FD3_REG_H13B0_T;

#endif
