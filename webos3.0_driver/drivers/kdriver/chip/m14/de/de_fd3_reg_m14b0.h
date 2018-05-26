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

#ifndef _REG_FD3_M14B0_H_
#define _REG_FD3_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0020d08L fd3 mode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 fd3_color_format                   : 2 ;  //  1: 0     
	UINT32                                    : 6 ;  //  7: 2     reserved
	UINT32 interlaced                         : 1 ;  //     8     
	UINT32                                    : 7 ;  // 15: 9     reserved
	UINT32 vertical_sampling                  : 1 ;  //    16     
} M14B0_FD3_MODE_T;

/*-----------------------------------------------------------------------------
		0xc0020d10L segmentation window ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_wnd_boffset                    : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 seg_wnd_toffset                    : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 seg_wnd_roffset                    : 4 ;  // 19:16     
	UINT32                                    : 4 ;  // 23:20     reserved
	UINT32 seg_wnd_loffset                    : 4 ;  // 27:24     
} M14B0_SEGMENTATION_WINDOW_T;

/*-----------------------------------------------------------------------------
		0xc0020d20L projection window ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 proj_wnd_boffset                   : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 proj_wnd_toffset                   : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 proj_wnd_lroffset                  : 4 ;  // 19:16     
} M14B0_PROJECTION_WINDOW_T;

/*-----------------------------------------------------------------------------
		0xc0020d24L projection parameter 0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 qdiff                              : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 qmin                               : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 valid_line_pel_count               : 6 ;  // 21:16     
} M14B0_PROJECTION_PARAMETER_0_T;

/*-----------------------------------------------------------------------------
		0xc0020d28L projection parameter 1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 vcnt_ratio                         : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 vdcnt_ratio                        : 4 ;  // 11: 8     
} M14B0_PROJECTION_PARAMETER_1_T;

/*-----------------------------------------------------------------------------
		0xc0020d30L segmentation threshold 0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_ye_gr                          : 8 ;  //  7: 0     
	UINT32 seg_yellow                         : 8 ;  // 15: 8     
	UINT32 seg_re_ye                          : 8 ;  // 23:16     
	UINT32 seg_red                            : 8 ;  // 31:24     
} M14B0_SEGMENTATION_THRESHOLD_0_T;

/*-----------------------------------------------------------------------------
		0xc0020d34L segmentation threshold 1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_cy_bl                          : 8 ;  //  7: 0     
	UINT32 seg_cyan                           : 8 ;  // 15: 8     
	UINT32 seg_gr_cy                          : 8 ;  // 23:16     
	UINT32 seg_green                          : 8 ;  // 31:24     
} M14B0_SEGMENTATION_THRESHOLD_1_T;

/*-----------------------------------------------------------------------------
		0xc0020d38L segmentation threshold 2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_ma_re                          : 8 ;  //  7: 0     
	UINT32 seg_magenta                        : 8 ;  // 15: 8     
	UINT32 seg_bl_ma                          : 8 ;  // 23:16     
	UINT32 seg_blue                           : 8 ;  // 31:24     
} M14B0_SEGMENTATION_THRESHOLD_2_T;

/*-----------------------------------------------------------------------------
		0xc0020d3cL segmentation threshold 3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 seg_th_sdelta                      : 8 ;  //  7: 0     
	UINT32 seg_th_saturate                    : 8 ;  // 15: 8     
	UINT32 seg_th_value_black                 : 8 ;  // 23:16     
} M14B0_SEGMENTATION_THRESHOLD_3_T;

/*-----------------------------------------------------------------------------
		0xc0020d40L subtitle control ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 sub_vcnt_ratio                     : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 sub_pcnt_ratio                     : 4 ;  // 11: 8     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 sub_dif_th                         : 8 ;  // 23:16     
} M14B0_SUBTITLE_CONTROL_T;

/*-----------------------------------------------------------------------------
		0xc0020d50L dominant threshold 0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dom_pic_seg_th3                    : 8 ;  //  7: 0     
	UINT32 dom_pic_seg_th2                    : 8 ;  // 15: 8     
	UINT32 dom_pic_seg_th1                    : 8 ;  // 23:16     
	UINT32 dom_pic_seg_th0                    : 8 ;  // 31:24     
} M14B0_DOMINANT_THRESHOLD_0_T;

/*-----------------------------------------------------------------------------
		0xc0020d54L dominant threshold 1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dom_blk_seg_hsv_th1                : 8 ;  //  7: 0     
	UINT32 dom_blk_seg_hsv_th0                : 8 ;  // 15: 8     
} M14B0_DOMINANT_THRESHOLD_1_T;

/*-----------------------------------------------------------------------------
		0xc0020d58L subtitle threshold ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 subtitle_th                        : 8 ;  //  7: 0     
} M14B0_SUBTITLE_THRESHOLD_T;

/*-----------------------------------------------------------------------------
		0xc0020d60L force mode ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 force_val_idx                      : 5 ;  //  4: 0     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 force_hsv_idx1                     : 5 ;  // 12: 8     
	UINT32                                    : 3 ;  // 15:13     reserved
	UINT32 force_hsv_idx0                     : 5 ;  // 20:16     
	UINT32                                    : 3 ;  // 23:21     reserved
	UINT32 force_scn_type                     : 1 ;  //    24     
	UINT32                                    : 3 ;  // 27:25     reserved
	UINT32 force_mode                         : 2 ;  // 29:28     
} M14B0_FORCE_MODE_T;

/*-----------------------------------------------------------------------------
		0xc0020d80L blk0_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK0_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020d84L blk1_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK1_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020d88L blk2_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK2_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020d8cL blk3_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK3_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020d90L blk4_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK4_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020d94L blk5_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK5_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020d98L blk6_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK6_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020d9cL blk7_stat ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 standard_dev                       : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mean                               : 12;  // 27:16     
} M14B0_BLK7_STAT_T;

/*-----------------------------------------------------------------------------
		0xc0020da0L corr_ss_blk0,1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_corr1                           : 8 ;  //  7: 0     
	UINT32 ss_wnd_info1                       : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 ss_corr0                           : 8 ;  // 23:16     
	UINT32 ss_wnd_info0                       : 2 ;  // 25:24     
} M14B0_CORR_SS_BLK0_1_T;

/*-----------------------------------------------------------------------------
		0xc0020da4L corr_ss_blk2,3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 ss_corr3                           : 8 ;  //  7: 0     
	UINT32 ss_wnd_info3                       : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 ss_corr2                           : 8 ;  // 23:16     
	UINT32 ss_wnd_info2                       : 2 ;  // 25:24     
} M14B0_CORR_SS_BLK2_3_T;

/*-----------------------------------------------------------------------------
		0xc0020da8L corr_tb_blk0,1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_corr1                           : 8 ;  //  7: 0     
	UINT32 tb_wnd_info1                       : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 tb_corr0                           : 8 ;  // 23:16     
	UINT32 tb_wnd_info0                       : 2 ;  // 25:24     
} M14B0_CORR_TB_BLK0_1_T;

/*-----------------------------------------------------------------------------
		0xc0020dacL corr_tb_blk2,3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 tb_corr3                           : 8 ;  //  7: 0     
	UINT32 tb_wnd_info3                       : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 tb_corr2                           : 8 ;  // 23:16     
	UINT32 tb_wnd_info2                       : 2 ;  // 25:24     
} M14B0_CORR_TB_BLK2_3_T;

/*-----------------------------------------------------------------------------
		0xc0020db0L hsv_picture ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_seg_hsv_count                  : 24;  // 23: 0     
	UINT32 pic_seg_hsv_max                    : 8 ;  // 31:24     
} M14B0_HSV_PICTURE_T;

/*-----------------------------------------------------------------------------
		0xc0020db4L val_picture ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 pic_seg_val_count                  : 24;  // 23: 0     
	UINT32 pic_seg_val_max                    : 8 ;  // 31:24     
} M14B0_VAL_PICTURE_T;

/*-----------------------------------------------------------------------------
		0xc0020dc0L hsv_blk0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk0_seg_hsv_count                 : 24;  // 23: 0     
	UINT32 blk0_seg_hsv_max                   : 8 ;  // 31:24     
} M14B0_HSV_BLK0_T;

/*-----------------------------------------------------------------------------
		0xc0020dc4L hsv_blk1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk1_seg_hsv_count                 : 24;  // 23: 0     
	UINT32 blk1_seg_hsv_max                   : 8 ;  // 31:24     
} M14B0_HSV_BLK1_T;

/*-----------------------------------------------------------------------------
		0xc0020dc8L hsv_blk2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk2_seg_hsv_count                 : 24;  // 23: 0     
	UINT32 blk2_seg_hsv_max                   : 8 ;  // 31:24     
} M14B0_HSV_BLK2_T;

/*-----------------------------------------------------------------------------
		0xc0020dccL hsv_blk3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk3_seg_hsv_count                 : 24;  // 23: 0     
	UINT32 blk3_seg_hsv_max                   : 8 ;  // 31:24     
} M14B0_HSV_BLK3_T;

/*-----------------------------------------------------------------------------
		0xc0020dd0L status_dominant0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 blk_valid_count                    : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 blk_valid_final                    : 4 ;  // 11: 8     
} M14B0_STATUS_DOMINANT0_T;

/*-----------------------------------------------------------------------------
		0xc0020dd4L status_dominant1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 dominant_val_idx                   : 8 ;  //  7: 0     
	UINT32 dominant_hsv_idx1                  : 8 ;  // 15: 8     
	UINT32 dominant_hsv_idx0                  : 8 ;  // 23:16     
	UINT32 scene_type                         : 1 ;  //    24     
} M14B0_STATUS_DOMINANT1_T;

/*-----------------------------------------------------------------------------
		0xc0020de0L fd3_status0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 end_status                         : 4 ;  //  3: 0     
	UINT32                                    : 4 ;  //  7: 4     reserved
	UINT32 buf_yc_overflow                    : 2 ;  //  9: 8     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 fd3_ib_r_lcnt_p                    : 10;  // 25:16     
} M14B0_FD3_STATUS0_T;

/*-----------------------------------------------------------------------------
		0xc0020de4L fd3_status1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 suc_fail_history                   : 24;  // 23: 0     
	UINT32 init_count                         : 8 ;  // 31:24     
} M14B0_FD3_STATUS1_T;

typedef struct {
	UINT32                             	reserved00[2];                //0xc0020d00L~0xc0020d04L
	M14B0_FD3_MODE_T                   	fd3_mode;                     //0xc0020d08L
	UINT32                             	reserved01;                   //0xc0020d0cL
	M14B0_SEGMENTATION_WINDOW_T        	segmentation_window;          //0xc0020d10L
	UINT32                             	reserved02[3];                //0xc0020d14L~0xc0020d1cL
	M14B0_PROJECTION_WINDOW_T          	projection_window;            //0xc0020d20L
	M14B0_PROJECTION_PARAMETER_0_T     	projection_parameter 0;       //0xc0020d24L
	M14B0_PROJECTION_PARAMETER_1_T     	projection_parameter 1;       //0xc0020d28L
	UINT32                             	reserved03;                   //0xc0020d2cL
	M14B0_SEGMENTATION_THRESHOLD_0_T   	segmentation_threshold 0;     //0xc0020d30L
	M14B0_SEGMENTATION_THRESHOLD_1_T   	segmentation_threshold 1;     //0xc0020d34L
	M14B0_SEGMENTATION_THRESHOLD_2_T   	segmentation_threshold 2;     //0xc0020d38L
	M14B0_SEGMENTATION_THRESHOLD_3_T   	segmentation_threshold 3;     //0xc0020d3cL
	M14B0_SUBTITLE_CONTROL_T           	subtitle_control;             //0xc0020d40L
	UINT32                             	reserved04[3];                //0xc0020d44L~0xc0020d4cL
	M14B0_DOMINANT_THRESHOLD_0_T       	dominant_threshold 0;         //0xc0020d50L
	M14B0_DOMINANT_THRESHOLD_1_T       	dominant_threshold 1;         //0xc0020d54L
	M14B0_SUBTITLE_THRESHOLD_T         	subtitle_threshold;           //0xc0020d58L
	UINT32                             	reserved05;                   //0xc0020d5cL
	M14B0_FORCE_MODE_T                 	force_mode;                   //0xc0020d60L
	UINT32                             	reserved06[7];                //0xc0020d64L~0xc0020d7cL
	M14B0_BLK0_STAT_T                  	blk0_stat;                    //0xc0020d80L
	M14B0_BLK1_STAT_T                  	blk1_stat;                    //0xc0020d84L
	M14B0_BLK2_STAT_T                  	blk2_stat;                    //0xc0020d88L
	M14B0_BLK3_STAT_T                  	blk3_stat;                    //0xc0020d8cL
	M14B0_BLK4_STAT_T                  	blk4_stat;                    //0xc0020d90L
	M14B0_BLK5_STAT_T                  	blk5_stat;                    //0xc0020d94L
	M14B0_BLK6_STAT_T                  	blk6_stat;                    //0xc0020d98L
	M14B0_BLK7_STAT_T                  	blk7_stat;                    //0xc0020d9cL
	M14B0_CORR_SS_BLK0_1_T             	corr_ss_blk0,1;               //0xc0020da0L
	M14B0_CORR_SS_BLK2_3_T             	corr_ss_blk2,3;               //0xc0020da4L
	M14B0_CORR_TB_BLK0_1_T             	corr_tb_blk0,1;               //0xc0020da8L
	M14B0_CORR_TB_BLK2_3_T             	corr_tb_blk2,3;               //0xc0020dacL
	M14B0_HSV_PICTURE_T                	hsv_picture;                  //0xc0020db0L
	M14B0_VAL_PICTURE_T                	val_picture;                  //0xc0020db4L
	UINT32                             	reserved07[2];                //0xc0020db8L~0xc0020dbcL
	M14B0_HSV_BLK0_T                   	hsv_blk0;                     //0xc0020dc0L
	M14B0_HSV_BLK1_T                   	hsv_blk1;                     //0xc0020dc4L
	M14B0_HSV_BLK2_T                   	hsv_blk2;                     //0xc0020dc8L
	M14B0_HSV_BLK3_T                   	hsv_blk3;                     //0xc0020dccL
	M14B0_STATUS_DOMINANT0_T           	status_dominant0;             //0xc0020dd0L
	M14B0_STATUS_DOMINANT1_T           	status_dominant1;             //0xc0020dd4L
	UINT32                             	reserved08[2];                //0xc0020dd8L~0xc0020ddcL
	M14B0_FD3_STATUS0_T                	fd3_status0;                  //0xc0020de0L
	M14B0_FD3_STATUS1_T                	fd3_status1;                  //0xc0020de4L
} DE_FD3_REG_M14B0_T;

#endif
