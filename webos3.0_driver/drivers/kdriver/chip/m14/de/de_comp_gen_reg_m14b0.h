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

#ifndef _M14B0_REGFILE_DE_COMP_GEN_M14B0_H_
#define _M14B0_REGFILE_DE_COMP_GEN_M14B0_H_
/*-----------------------------------------------------------------------------
		0xc0030000L reg_comp_ctrl0 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_gen_mode                       : 2 ;  //  1: 0     
	UINT32 reserved                           : 1 ;  //     2     
	UINT32 reg_pattern                        : 1 ;  //     3     
	UINT32 reg_mv_mode                        : 1 ;  //     4     
	UINT32 reg_csc_disable                    : 1 ;  //     5     
	UINT32 reg_csc_mode                       : 1 ;  //     6     
	UINT32 reg_detour_mode                    : 1 ;  //     7     
	UINT32 reg_hsync                          : 8 ;  // 15: 8     
	UINT32 reg_hfp                            : 10;  // 25:16     
} M14B0_REG_COMP_CTRL0_T;

/*-----------------------------------------------------------------------------
		0xc0030004L reg_comp_ctrl1 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hbp                            : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_vsync                          : 8 ;  // 23:16     
} M14B0_REG_COMP_CTRL1_T;

/*-----------------------------------------------------------------------------
		0xc0030008L reg_comp_ctrl2 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_tri_pos_level                  : 8 ;  //  7: 0     
	UINT32 reg_tri_clm_level                  : 8 ;  // 15: 8     
	UINT32 reg_tri_neg_level                  : 8 ;  // 23:16     
} M14B0_REG_COMP_CTRL2_T;

/*-----------------------------------------------------------------------------
		0xc003000cL reg_comp_ctrl3 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_win_v_start                 : 8 ;  //  7: 0     
	UINT32 reg_mv_win_v_end                   : 8 ;  // 15: 8     
	UINT32 reg_mv_pseudo_start                : 10;  // 25:16     
} M14B0_REG_COMP_CTRL3_T;

/*-----------------------------------------------------------------------------
		0xc0030010L reg_comp_ctrl4 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_pseudo_sync                 : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_mv_pseudo_agc                  : 10;  // 25:16     
} M14B0_REG_COMP_CTRL4_T;

/*-----------------------------------------------------------------------------
		0xc0030014L reg_comp_ctrl5 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_pseudo_period               : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_mv_high_level                  : 10;  // 25:16     
} M14B0_REG_COMP_CTRL5_T;

/*-----------------------------------------------------------------------------
		0xc0030018L reg_comp_ctrl6 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_low_level                   : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_ramp_level                     : 8 ;  // 23:16     
} M14B0_REG_COMP_CTRL6_T;

/*-----------------------------------------------------------------------------
		0xc003001cL reg_comp_ctrl7 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_vfp                         : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_mv_vsync                       : 10;  // 25:16     
} M14B0_REG_COMP_CTRL7_T;

/*-----------------------------------------------------------------------------
		0xc0030020L reg_comp_ctrl8 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_mv_va                          : 10;  //  9: 0     
} M14B0_REG_COMP_CTRL8_T;

/*-----------------------------------------------------------------------------
		0xc0030024L reg_comp_ctrl9 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_black_level_y                  : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_map_slope_y                    : 8 ;  // 23:16     
} M14B0_REG_COMP_CTRL9_T;

/*-----------------------------------------------------------------------------
		0xc0030028L reg_comp_ctrl10 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_black_level_c                  : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 reg_map_slope_c                    : 8 ;  // 23:16     
} M14B0_REG_COMP_CTRL10_T;

/*-----------------------------------------------------------------------------
		0xc003002cL reg_comp_ctrl11 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 reg_hn                             : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 reg_vn                             : 11;  // 26:16     
} M14B0_REG_COMP_CTRL11_T;
typedef struct {
	M14B0_REG_COMP_CTRL0_T             	reg_comp_ctrl0;               //0xc0030000L
	M14B0_REG_COMP_CTRL1_T             	reg_comp_ctrl1;               //0xc0030004L
	M14B0_REG_COMP_CTRL2_T             	reg_comp_ctrl2;               //0xc0030008L
	M14B0_REG_COMP_CTRL3_T             	reg_comp_ctrl3;               //0xc003000cL
	M14B0_REG_COMP_CTRL4_T             	reg_comp_ctrl4;               //0xc0030010L
	M14B0_REG_COMP_CTRL5_T             	reg_comp_ctrl5;               //0xc0030014L
	M14B0_REG_COMP_CTRL6_T             	reg_comp_ctrl6;               //0xc0030018L
	M14B0_REG_COMP_CTRL7_T             	reg_comp_ctrl7;               //0xc003001cL
	M14B0_REG_COMP_CTRL8_T             	reg_comp_ctrl8;               //0xc0030020L
	M14B0_REG_COMP_CTRL9_T             	reg_comp_ctrl9;               //0xc0030024L
	M14B0_REG_COMP_CTRL10_T            	reg_comp_ctrl10;              //0xc0030028L
	M14B0_REG_COMP_CTRL11_T            	reg_comp_ctrl11;              //0xc003002cL
}M14B0_REGFILE_DE_COMP_GEN_M14B0_T;
#endif