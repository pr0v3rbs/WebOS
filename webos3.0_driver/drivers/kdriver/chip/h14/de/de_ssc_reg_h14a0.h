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

#ifndef _REG_SSC_H14A0_h
#define _REG_SSC_H14A0_h

/*-----------------------------------------------------------------------------
	0x2100 ssc_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 bypass_n                    : 1;   //     0
	UINT32 fir_config                  : 1;   //     1
	UINT32 filter_type_sel             : 1;   //     2
	UINT32 size_info_manual_load       : 1;   //     3
	UINT32                             : 1;   //     4 reserved
	UINT32 ddr_update_endian_change    : 1;   //     5
	UINT32 stride_cvi_mode             : 1;   //     6
	UINT32 dvr_write_mode              : 1;   //     7
	UINT32 source_sel                  : 3;   //  8:10
} H14A0_SSC_CTRL_T;

/*-----------------------------------------------------------------------------
	0x2104 ssc_field ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 interlace_mode              : 1;   //     0
	UINT32 curr_field                  : 1;   //     1
	UINT32 mpeg_stream                 : 1;   //     2
} H14A0_SSC_FIELD_T;

/*-----------------------------------------------------------------------------
	0x2108 ssc_y_init_pos_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_h                : 6;   //  0: 5
} H14A0_SSC_Y_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
	0x210c ssc_y_init_pos_v ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_init_pos_v                : 6;   //  0: 5
} H14A0_SSC_Y_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
	0x2110 ssc_c_init_pos_h ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_h                : 6;   //  0: 5
} H14A0_SSC_C_INIT_POS_H_T;

/*-----------------------------------------------------------------------------
	0x2114 ssc_c_init_pos_v ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_init_pos_v                : 6;   //  0: 5
} H14A0_SSC_C_INIT_POS_V_T;

/*-----------------------------------------------------------------------------
	0x2118 ssc_scr_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 input_h_size                : 11;  //  0:10
	UINT32                             : 5;   // 11:15 reserved
	UINT32 input_v_size                : 11;  // 16:26
} H14A0_SSC_SCR_SIZE_T;

/*-----------------------------------------------------------------------------
	0x211c ssc_win_out_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 out_win_h_size              : 11;  //  0:10
	UINT32                             : 5;   // 11:15 reserved
	UINT32 out_win_v_size              : 11;  // 16:26
} H14A0_SSC_WIN_OUT_SIZE_T;

/*-----------------------------------------------------------------------------
	0x2120 ssc_win_in_offset ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_offset             : 11;  //  0:10
	UINT32                             : 5;   // 11:15 reserved
	UINT32 in_win_v_offset             : 11;  // 16:26
} H14A0_SSC_WIN_IN_OFFSET_T;

/*-----------------------------------------------------------------------------
	0x2124 ssc_win_in_size ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_win_h_size               : 11;  //  0:10
	UINT32                             : 5;   // 11:15 reserved
	UINT32 in_win_v_size               : 11;  // 16:26
} H14A0_SSC_WIN_IN_SIZE_T;

/*-----------------------------------------------------------------------------
	0x2128 ssc_c_format ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 in_c_format                 : 3;   //  0: 2
	UINT32                             : 13;  //  3:15 reserved
	UINT32 out_c_format                : 3;   // 16:18
} H14A0_SSC_C_FORMAT_T;

/*-----------------------------------------------------------------------------
	0x2130 ssc_h_fir_coef_data0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient0              : 10;  //  0: 9
	UINT32                             : 6;   // 10:15 reserved
	UINT32 h_coefficient1              : 10;  // 16:25
} H14A0_SSC_H_FIR_COEF_DATA0_T;

/*-----------------------------------------------------------------------------
	0x2134 ssc_h_fir_coef_data1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient2              : 10;  //  0: 9
	UINT32                             : 6;   // 10:15 reserved
	UINT32 h_coefficient3              : 10;  // 16:25
} H14A0_SSC_H_FIR_COEF_DATA1_T;

/*-----------------------------------------------------------------------------
	0x2138 ssc_h_fir_coef_data2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient4              : 10;  //  0: 9
	UINT32                             : 6;   // 10:15 reserved
	UINT32 h_coefficient5              : 10;  // 16:25
} H14A0_SSC_H_FIR_COEF_DATA2_T;

/*-----------------------------------------------------------------------------
	0x213c ssc_h_fir_coef_data3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_coefficient6              : 10;  //  0: 9
	UINT32                             : 6;   // 10:15 reserved
	UINT32 h_coefficient7              : 10;  // 16:25
} H14A0_SSC_H_FIR_COEF_DATA3_T;

/*-----------------------------------------------------------------------------
	0x2140 ssc_h_fir_coef_ctrl ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 addr                        : 4;   //  0: 3
	UINT32 h_v_sel                     : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 update                      : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 rnw                         : 1;   //    12
} H14A0_SSC_H_FIR_COEF_CTRL_T;

/*-----------------------------------------------------------------------------
	0x2168 ssc_status_rev ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_status                    : 11;  //  0:10
	UINT32                             : 5;   // 11:15 reserved
	UINT32 c_status                    : 11;  // 16:26
} H14A0_SSC_STATUS_REV_T;

/*-----------------------------------------------------------------------------
	0x216c ssc_manual_load ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 update_input_frm_size       : 1;   //     0
	UINT32                             : 3;   //  1: 3 reserved
	UINT32 update_wout_size            : 1;   //     4
	UINT32                             : 3;   //  5: 7 reserved
	UINT32 update_win_offset           : 1;   //     8
	UINT32                             : 3;   //  9:11 reserved
	UINT32 update_win_size             : 1;   //    12
} H14A0_SSC_MANUAL_LOAD_T;

/*-----------------------------------------------------------------------------
	0x2170 ssc_canvas_mode ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_offset                    : 8;   //  0: 7
	UINT32                             : 8;   //  8:15 reserved
	UINT32 v_offset                    : 11;  // 16:26
} H14A0_SSC_CANVAS_MODE_T;

typedef struct {
	H14A0_SSC_CTRL_T                ssc_ctrl;                        // 0x2100
	H14A0_SSC_FIELD_T               ssc_field;                       // 0x2104
	H14A0_SSC_Y_INIT_POS_H_T        ssc_y_init_pos_h;                // 0x2108
	H14A0_SSC_Y_INIT_POS_V_T        ssc_y_init_pos_v;                // 0x210c
	H14A0_SSC_C_INIT_POS_H_T        ssc_c_init_pos_h;                // 0x2110
	H14A0_SSC_C_INIT_POS_V_T        ssc_c_init_pos_v;                // 0x2114
	H14A0_SSC_SCR_SIZE_T            ssc_scr_size;                    // 0x2118
	H14A0_SSC_WIN_OUT_SIZE_T        ssc_win_out_size;                // 0x211c
	H14A0_SSC_WIN_IN_OFFSET_T       ssc_win_in_offset;               // 0x2120
	H14A0_SSC_WIN_IN_SIZE_T         ssc_win_in_size;                 // 0x2124
	H14A0_SSC_C_FORMAT_T            ssc_c_format;                    // 0x2128
	UINT32                          reserved0;                       // 0x212c
	H14A0_SSC_H_FIR_COEF_DATA0_T    ssc_h_fir_coef_data0;            // 0x2130
	H14A0_SSC_H_FIR_COEF_DATA1_T    ssc_h_fir_coef_data1;            // 0x2134
	H14A0_SSC_H_FIR_COEF_DATA2_T    ssc_h_fir_coef_data2;            // 0x2138
	H14A0_SSC_H_FIR_COEF_DATA3_T    ssc_h_fir_coef_data3;            // 0x213c
	H14A0_SSC_H_FIR_COEF_CTRL_T     ssc_h_fir_coef_ctrl;             // 0x2140
	UINT32                          reserved1[9];                    // 0x2144~0x2164
	H14A0_SSC_STATUS_REV_T          ssc_status_rev;                  // 0x2168
	H14A0_SSC_MANUAL_LOAD_T         ssc_manual_load;                 // 0x216c
	H14A0_SSC_CANVAS_MODE_T         ssc_canvas_mode;                 // 0x2170
} DE_SSC_REG_H14A0_T;

#endif
