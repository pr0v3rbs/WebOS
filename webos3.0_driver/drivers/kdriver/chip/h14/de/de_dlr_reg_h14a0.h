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

#ifndef _REG_DLR_H14A0_h
#define _REG_DLR_H14A0_h

/*-----------------------------------------------------------------------------
	0x0300 dlr_dec3d_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_h_offset                      : 12;  //  0:11
	UINT32                                 : 3;   // 12:14 reserved
	UINT32 prog_scan                       : 1;   //    15
	UINT32 y_h_width                       : 12;  // 16:27
	UINT32 c_format                        : 3;   // 28:30
	UINT32 dlr_3d_ctrl_en                  : 1;   //    31
} H14A0_DLR_DEC3D_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x0308 dlr_dec3d_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_v_offset                      : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 y_v_height                      : 12;  // 16:27
} H14A0_DLR_DEC3D_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x0310 dlr_dec3d_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_sub_sample_step               : 8;   //  0: 7
	UINT32 y_h_even_line_sub_sample_offset : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 y_h_odd_line_sub_sample_offset  : 8;   // 24:31
} H14A0_DLR_DEC3D_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x0314 dlr_dec3d_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 frame_packing_active_space      : 8;   //  0: 7
	UINT32 c_h_even_line_sub_sample_offset : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 c_h_odd_line_sub_sample_offset  : 8;   // 24:31
} H14A0_DLR_DEC3D_CTRL5_T;

/*-----------------------------------------------------------------------------
	0x0318 dlr_dec3d_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_sub_sample_step               : 8;   //  0: 7
	UINT32 y_v_sub_sample_offset           : 8;   //  8:15
	UINT32                                 : 8;   // 16:23 reserved
	UINT32 c_v_sub_sample_offset           : 8;   // 24:31
} H14A0_DLR_DEC3D_CTRL6_T;

/*-----------------------------------------------------------------------------
	0x031c dlr_dec3d_status ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_v_cnt                         : 12;  //  0:11
	UINT32                                 : 4;   // 12:15 reserved
	UINT32 c_v_cnt                         : 12;  // 16:27
} H14A0_DLR_DEC3D_STATUS_T;

typedef struct {
	H14A0_DLR_DEC3D_CTRL0_T     dlr_dec3d_ctrl0;             // 0x0300
	UINT32                      reserved0;                   // 0x0304
	H14A0_DLR_DEC3D_CTRL2_T     dlr_dec3d_ctrl2;             // 0x0308
	UINT32                      reserved1;                   // 0x030c
	H14A0_DLR_DEC3D_CTRL4_T     dlr_dec3d_ctrl4;             // 0x0310
	H14A0_DLR_DEC3D_CTRL5_T     dlr_dec3d_ctrl5;             // 0x0314
	H14A0_DLR_DEC3D_CTRL6_T     dlr_dec3d_ctrl6;             // 0x0318
	H14A0_DLR_DEC3D_STATUS_T    dlr_dec3d_status;            // 0x031c
} DE_DLR_REG_H14A0_T;

#endif
