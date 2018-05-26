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

#ifndef _REG_DEC3D_M14B0_H_
#define _REG_DEC3D_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0020400L dec3d_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_h_offset                         : 12;  // 11: 0     
	UINT32 3dto2d                             : 2 ;  // 13:12     
	UINT32                                    : 1 ;  //    14     reserved
	UINT32 prog_scan                          : 1 ;  //    15     
	UINT32 y_h_width                          : 12;  // 27:16     
	UINT32 c_format                           : 3 ;  // 30:28     
	UINT32 3d_ctrl_en                         : 1 ;  //    31     
} M14B0_DEC3D_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0020404L dec3d_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0     
} M14B0_DEC3D_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0020408L dec3d_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_v_offset                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_v_height                         : 12;  // 27:16     
} M14B0_DEC3D_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc002040cL dec3d_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reserved                           : 32;  // 31: 0     
} M14B0_DEC3D_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0020410L dec3d_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_sub_sample_step                  : 8 ;  //  7: 0     
	UINT32 y_h_even_line_sub_sample_offset    : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 y_h_odd_line_sub_sample_offset     : 8 ;  // 31:24     
} M14B0_DEC3D_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc0020414L dec3d_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 frame_packing_active_space         : 8 ;  //  7: 0     
	UINT32 c_h_even_line_sub_sample_offset    : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 c_h_odd_line_sub_sample_offset     : 8 ;  // 31:24     
} M14B0_DEC3D_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc0020418L dec3d_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_sub_sample_step                  : 8 ;  //  7: 0     
	UINT32 y_v_sub_sample_offset              : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 c_v_sub_sample_offset              : 8 ;  // 31:24     
} M14B0_DEC3D_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc002041cL dec3d_status ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_v_cnt                            : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_v_cnt                            : 12;  // 27:16     
} M14B0_DEC3D_STATUS_T;

typedef struct {
	M14B0_DEC3D_CTRL0_T                	dec3d_ctrl0;                  //0xc0020400L
	M14B0_DEC3D_CTRL1_T                	dec3d_ctrl1;                  //0xc0020404L
	M14B0_DEC3D_CTRL2_T                	dec3d_ctrl2;                  //0xc0020408L
	M14B0_DEC3D_CTRL3_T                	dec3d_ctrl3;                  //0xc002040cL
	M14B0_DEC3D_CTRL4_T                	dec3d_ctrl4;                  //0xc0020410L
	M14B0_DEC3D_CTRL5_T                	dec3d_ctrl5;                  //0xc0020414L
	M14B0_DEC3D_CTRL6_T                	dec3d_ctrl6;                  //0xc0020418L
	M14B0_DEC3D_STATUS_T               	dec3d_status;                 //0xc002041cL
} DE_DEC3D_M_REG_M14B0_T;

typedef struct {
	M14B0_DEC3D_CTRL0_T                	dec3d_ctrl0;                  //0xc0020400L
	M14B0_DEC3D_CTRL1_T                	dec3d_ctrl1;                  //0xc0020404L
	M14B0_DEC3D_CTRL2_T                	dec3d_ctrl2;                  //0xc0020408L
	M14B0_DEC3D_CTRL3_T                	dec3d_ctrl3;                  //0xc002040cL
	M14B0_DEC3D_CTRL4_T                	dec3d_ctrl4;                  //0xc0020410L
	M14B0_DEC3D_CTRL5_T                	dec3d_ctrl5;                  //0xc0020414L
	M14B0_DEC3D_CTRL6_T                	dec3d_ctrl6;                  //0xc0020418L
	M14B0_DEC3D_STATUS_T               	dec3d_status;                 //0xc002041cL
} DE_DEC3D_S_REG_M14B0_T;

typedef struct {
	M14B0_DEC3D_CTRL0_T                	dec3d_ctrl0;                  //0xc0020400L
	M14B0_DEC3D_CTRL1_T                	dec3d_ctrl1;                  //0xc0020404L
	M14B0_DEC3D_CTRL2_T                	dec3d_ctrl2;                  //0xc0020408L
	M14B0_DEC3D_CTRL3_T                	dec3d_ctrl3;                  //0xc002040cL
	M14B0_DEC3D_CTRL4_T                	dec3d_ctrl4;                  //0xc0020410L
	M14B0_DEC3D_CTRL5_T                	dec3d_ctrl5;                  //0xc0020414L
	M14B0_DEC3D_CTRL6_T                	dec3d_ctrl6;                  //0xc0020418L
	M14B0_DEC3D_STATUS_T               	dec3d_status;                 //0xc002041cL
} DE_DEC3D_W_REG_M14B0_T;

#endif