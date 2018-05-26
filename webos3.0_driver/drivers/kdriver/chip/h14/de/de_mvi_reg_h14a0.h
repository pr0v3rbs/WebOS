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

#ifndef _REG_MVI_H14A0_h
#define _REG_MVI_H14A0_h

/*-----------------------------------------------------------------------------
	0x0800 dea_mvi_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_i_c_fmt                     : 3;   //  0: 2
	UINT32                                 : 1;   //     3 reserved
	UINT32 mvi_field                       : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 mvi_pr_sq                       : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 mvi_cvi_stride                  : 1;   //    12
} H14A0_DEA_MVI_CTRL_T;

/*-----------------------------------------------------------------------------
	0x0810 dea_mvi_v_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_vsize                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 mvi_vblank                      : 12;  // 16:27
} H14A0_DEA_MVI_V_INFO_T;

/*-----------------------------------------------------------------------------
	0x0814 dea_mvi_h_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_hsize                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 mvi_hblank                      : 12;  // 16:27
} H14A0_DEA_MVI_H_INFO_T;

/*-----------------------------------------------------------------------------
	0x0820 dea_mvi_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_h_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_h_width                       : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 mvi_3d_ctrl_en                  : 1;   //    31
} H14A0_DEA_MVI_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x0824 dea_mvi_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_h_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_h_width                       : 12;  // 16:27
} H14A0_DEA_MVI_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x0828 dea_mvi_ctrl_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_v_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_v_height                      : 12;  // 16:27
} H14A0_DEA_MVI_CTRL_2_T;

/*-----------------------------------------------------------------------------
	0x082c dea_mvi_ctrl_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_v_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_v_height                      : 12;  // 16:27
} H14A0_DEA_MVI_CTRL_3_T;

/*-----------------------------------------------------------------------------
	0x0830 dea_mvi_ctrl_4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_sub_sample_step               : 8;   //  0: 7
	UINT32 y_h_even_line_sub_sample_offset : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 y_h_odd_line_sub_sample_offset  : 8;   // 24:31
} H14A0_DEA_MVI_CTRL_4_T;

/*-----------------------------------------------------------------------------
	0x0834 dea_mvi_ctrl_5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 8;   //  0: 7 reserved
	UINT32 c_h_even_line_sub_sample_offset : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 c_h_odd_line_sub_sample_offset  : 8;   // 24:31
} H14A0_DEA_MVI_CTRL_5_T;

/*-----------------------------------------------------------------------------
	0x0838 dea_mvi_ctrl_6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_sub_sample_step               : 8;   //  0: 7
	UINT32 y_v_sub_sample_offset           : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 c_v_sub_sample_offset           : 8;   // 24:31
} H14A0_DEA_MVI_CTRL_6_T;

/*-----------------------------------------------------------------------------
	0x2800 dec_mvi_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_i_c_fmt                     : 3;   //  0: 2
	UINT32                                 : 1;   //     3 reserved
	UINT32 mvi_field                       : 1;   //     4
	UINT32                                 : 3;   //  5: 7 reserved
	UINT32 mvi_pr_sq                       : 1;   //     8
	UINT32                                 : 3;   //  9:11 reserved
	UINT32 mvi_cvi_stride                  : 1;   //    12
} H14A0_DEC_MVI_CTRL_T;

/*-----------------------------------------------------------------------------
	0x2810 dec_mvi_v_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_vsize                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 mvi_vblank                      : 12;  // 16:27
} H14A0_DEC_MVI_V_INFO_T;

/*-----------------------------------------------------------------------------
	0x2814 dec_mvi_h_info ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_hsize                       : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 mvi_hblank                      : 12;  // 16:27
} H14A0_DEC_MVI_H_INFO_T;

/*-----------------------------------------------------------------------------
	0x2820 dec_mvi_ctrl_0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_h_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_h_width                       : 12;  // 16:27
	UINT32                                 : 3;   // 28:30 reserved
	UINT32 mvi_3d_ctrl_en                  : 1;   //    31
} H14A0_DEC_MVI_CTRL_0_T;

/*-----------------------------------------------------------------------------
	0x2824 dec_mvi_ctrl_1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_h_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_h_width                       : 12;  // 16:27
} H14A0_DEC_MVI_CTRL_1_T;

/*-----------------------------------------------------------------------------
	0x2828 dec_mvi_ctrl_2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_v_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_v_height                      : 12;  // 16:27
} H14A0_DEC_MVI_CTRL_2_T;

/*-----------------------------------------------------------------------------
	0x282c dec_mvi_ctrl_3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_v_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_v_height                      : 12;  // 16:27
} H14A0_DEC_MVI_CTRL_3_T;

/*-----------------------------------------------------------------------------
	0x2830 dec_mvi_ctrl_4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_sub_sample_step               : 8;   //  0: 7
	UINT32 y_h_even_line_sub_sample_offset : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 y_h_odd_line_sub_sample_offset  : 8;   // 24:31
} H14A0_DEC_MVI_CTRL_4_T;

/*-----------------------------------------------------------------------------
	0x2834 dec_mvi_ctrl_5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32                                 : 8;   //  0: 7 reserved
	UINT32 c_h_even_line_sub_sample_offset : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 c_h_odd_line_sub_sample_offset  : 8;   // 24:31
} H14A0_DEC_MVI_CTRL_5_T;

/*-----------------------------------------------------------------------------
	0x2838 dec_mvi_ctrl_6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_sub_sample_step               : 8;   //  0: 7
	UINT32 y_v_sub_sample_offset           : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 c_v_sub_sample_offset           : 8;   // 24:31
} H14A0_DEC_MVI_CTRL_6_T;

typedef struct {
	H14A0_DEA_MVI_CTRL_T    dea_mvi_ctrl;            // 0x0800
	UINT32                  reserved0[3];            // 0x0804~0x080c
	H14A0_DEA_MVI_V_INFO_T  dea_mvi_v_info;          // 0x0810
	H14A0_DEA_MVI_H_INFO_T  dea_mvi_h_info;          // 0x0814
	UINT32                  reserved1[2];            // 0x0818~0x081c
	H14A0_DEA_MVI_CTRL_0_T  dea_mvi_ctrl_0;          // 0x0820
	H14A0_DEA_MVI_CTRL_1_T  dea_mvi_ctrl_1;          // 0x0824
	H14A0_DEA_MVI_CTRL_2_T  dea_mvi_ctrl_2;          // 0x0828
	H14A0_DEA_MVI_CTRL_3_T  dea_mvi_ctrl_3;          // 0x082c
	H14A0_DEA_MVI_CTRL_4_T  dea_mvi_ctrl_4;          // 0x0830
	H14A0_DEA_MVI_CTRL_5_T  dea_mvi_ctrl_5;          // 0x0834
	H14A0_DEA_MVI_CTRL_6_T  dea_mvi_ctrl_6;          // 0x0838
} DE_MVL_REG_H14A0_T;

typedef struct {
	H14A0_DEC_MVI_CTRL_T    dec_mvi_ctrl;            // 0x2800
	UINT32                  reserved2[3];            // 0x2804~0x280c
	H14A0_DEC_MVI_V_INFO_T  dec_mvi_v_info;          // 0x2810
	H14A0_DEC_MVI_H_INFO_T  dec_mvi_h_info;          // 0x2814
	UINT32                  reserved3[2];            // 0x2818~0x281c
	H14A0_DEC_MVI_CTRL_0_T  dec_mvi_ctrl_0;          // 0x2820
	H14A0_DEC_MVI_CTRL_1_T  dec_mvi_ctrl_1;          // 0x2824
	H14A0_DEC_MVI_CTRL_2_T  dec_mvi_ctrl_2;          // 0x2828
	H14A0_DEC_MVI_CTRL_3_T  dec_mvi_ctrl_3;          // 0x282c
	H14A0_DEC_MVI_CTRL_4_T  dec_mvi_ctrl_4;          // 0x2830
	H14A0_DEC_MVI_CTRL_5_T  dec_mvi_ctrl_5;          // 0x2834
	H14A0_DEC_MVI_CTRL_6_T  dec_mvi_ctrl_6;          // 0x2838
} DE_MVR_REG_H14A0_T;

#endif
