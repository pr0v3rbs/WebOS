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

#ifndef _REG_MVI_M14B0_H_
#define _REG_MVI_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0020500L mvi_ctrl ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_i_c_fmt                        : 3 ;  //  2: 0     
	UINT32                                    : 1 ;  //     3     reserved
	UINT32 mvi_field                          : 1 ;  //     4     
	UINT32                                    : 3 ;  //  7: 5     reserved
	UINT32 mvi_pr_sq                          : 1 ;  //     8     
	UINT32                                    : 3 ;  // 11: 9     reserved
	UINT32 mvi_cvi_stride                     : 1 ;  //    12     
} M14B0_MVI_CTRL_T;

/*-----------------------------------------------------------------------------
		0xc0020510L mvi_v_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_vsize                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mvi_vblank                         : 12;  // 27:16     
} M14B0_MVI_V_INFO_T;

/*-----------------------------------------------------------------------------
		0xc0020514L mvi_h_info ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 mvi_hsize                          : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 mvi_hblank                         : 12;  // 27:16     
} M14B0_MVI_H_INFO_T;

/*-----------------------------------------------------------------------------
		0xc0020520L mvi_ctrl_0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_h_offset                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_h_width                          : 12;  // 27:16     
	UINT32                                    : 3 ;  // 30:28     reserved
	UINT32 mvi_3d_ctrl_en                     : 1 ;  //    31     
} M14B0_MVI_CTRL_0_T;

/*-----------------------------------------------------------------------------
		0xc0020524L mvi_ctrl_1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_h_offset                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_h_width                          : 12;  // 27:16     
} M14B0_MVI_CTRL_1_T;

/*-----------------------------------------------------------------------------
		0xc0020528L mvi_ctrl_2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 y_v_offset                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y_v_height                         : 12;  // 27:16     
} M14B0_MVI_CTRL_2_T;

/*-----------------------------------------------------------------------------
		0xc002052cL mvi_ctrl_3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 c_v_offset                         : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 c_v_height                         : 12;  // 27:16     
} M14B0_MVI_CTRL_3_T;

/*-----------------------------------------------------------------------------
		0xc0020530L mvi_ctrl_4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 h_sub_sample_step                  : 8 ;  //  7: 0     
	UINT32 y_h_even_line_sub_sample_offset    : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 y_h_odd_line_sub_sample_offset     : 8 ;  // 31:24     
} M14B0_MVI_CTRL_4_T;

/*-----------------------------------------------------------------------------
		0xc0020534L mvi_ctrl_5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 8 ;  //  7: 0     reserved
	UINT32 c_h_even_line_sub_sample_offset    : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 c_h_odd_line_sub_sample_offset     : 8 ;  // 31:24     
} M14B0_MVI_CTRL_5_T;

/*-----------------------------------------------------------------------------
		0xc0020538L mvi_ctrl_6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 v_sub_sample_step                  : 8 ;  //  7: 0     
	UINT32 y_v_sub_sample_offset              : 8 ;  // 15: 8     
	UINT32                                    : 8 ;  // 23:16     reserved
	UINT32 c_v_sub_sample_offset              : 8 ;  // 31:24     
} M14B0_MVI_CTRL_6_T;

typedef struct {
	M14B0_MVI_CTRL_T               	dea_mvi_ctrl;                 //0xc0020500L
	UINT32                          reserved00[3];                //0xc0020504L~0xc002050cL
	M14B0_MVI_V_INFO_T             	dea_mvi_v_info;               //0xc0020510L
	M14B0_MVI_H_INFO_T             	dea_mvi_h_info;               //0xc0020514L
	UINT32                          reserved01[2];                //0xc0020518L~0xc002051cL
	M14B0_MVI_CTRL_0_T             	dea_mvi_ctrl_0;               //0xc0020520L
	M14B0_MVI_CTRL_1_T             	dea_mvi_ctrl_1;               //0xc0020524L
	M14B0_MVI_CTRL_2_T             	dea_mvi_ctrl_2;               //0xc0020528L
	M14B0_MVI_CTRL_3_T             	dea_mvi_ctrl_3;               //0xc002052cL
	M14B0_MVI_CTRL_4_T             	dea_mvi_ctrl_4;               //0xc0020530L
	M14B0_MVI_CTRL_5_T             	dea_mvi_ctrl_5;               //0xc0020534L
	M14B0_MVI_CTRL_6_T             	dea_mvi_ctrl_6;               //0xc0020538L
	UINT32                          reserved02[177];              //0xc002053cL~0xc00207fcL
	M14B0_MVI_CTRL_T               	dec_mvi_ctrl;                 //0xc0020800L
	UINT32                          reserved03[3];                //0xc0020804L~0xc002080cL
	M14B0_MVI_V_INFO_T             	dec_mvi_v_info;               //0xc0020810L
	M14B0_MVI_H_INFO_T             	dec_mvi_h_info;               //0xc0020814L
	UINT32                          reserved04[2];                //0xc0020818L~0xc002081cL
	M14B0_MVI_CTRL_0_T             	dec_mvi_ctrl_0;               //0xc0020820L
	M14B0_MVI_CTRL_1_T             	dec_mvi_ctrl_1;               //0xc0020824L
	M14B0_MVI_CTRL_2_T             	dec_mvi_ctrl_2;               //0xc0020828L
	M14B0_MVI_CTRL_3_T             	dec_mvi_ctrl_3;               //0xc002082cL
	M14B0_MVI_CTRL_4_T             	dec_mvi_ctrl_4;               //0xc0020830L
	M14B0_MVI_CTRL_5_T             	dec_mvi_ctrl_5;               //0xc0020834L
	M14B0_MVI_CTRL_6_T             	dec_mvi_ctrl_6;               //0xc0020838L	
} DE_MVI_REG_M14B0_T;

#endif