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

#ifndef _REG_MOF_H14A0_h
#define _REG_MOF_H14A0_h

/*-----------------------------------------------------------------------------
	0x5a00 dppa_mof_ctrl0 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef0         : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 fir_y_coef1         : 10;  // 16:25
} H14A0_DPPA_MOF_CTRL0_T;

/*-----------------------------------------------------------------------------
	0x5a04 dppa_mof_ctrl1 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef2         : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 fir_y_coef3         : 10;  // 16:25
} H14A0_DPPA_MOF_CTRL1_T;

/*-----------------------------------------------------------------------------
	0x5a08 dppa_mof_ctrl2 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef4         : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 fir_y_coef5         : 10;  // 16:25
} H14A0_DPPA_MOF_CTRL2_T;

/*-----------------------------------------------------------------------------
	0x5a0c dppa_mof_ctrl3 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_y_coef6         : 10;  //  0: 9
} H14A0_DPPA_MOF_CTRL3_T;

/*-----------------------------------------------------------------------------
	0x5a10 dppa_mof_ctrl4 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coef0         : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 fir_c_coef1         : 10;  // 16:25
} H14A0_DPPA_MOF_CTRL4_T;

/*-----------------------------------------------------------------------------
	0x5a14 dppa_mof_ctrl5 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coef2         : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 fir_c_coef3         : 10;  // 16:25
} H14A0_DPPA_MOF_CTRL5_T;

/*-----------------------------------------------------------------------------
	0x5a18 dppa_mof_ctrl6 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coef4         : 10;  //  0: 9
	UINT32                     : 6;   // 10:15 reserved
	UINT32 fir_c_coef5         : 10;  // 16:25
} H14A0_DPPA_MOF_CTRL6_T;

/*-----------------------------------------------------------------------------
	0x5a1c dppa_mof_ctrl7 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 fir_c_coef6         : 10;  //  0: 9
} H14A0_DPPA_MOF_CTRL7_T;

/*-----------------------------------------------------------------------------
	0x5a20 dppa_mof_ctrl8 ''
------------------------------------------------------------------------------*/
typedef struct {
	UINT32 csc_en              : 1;   //     0
	UINT32 down_scale_en       : 1;   //     1
	UINT32 aal_en              : 1;   //     2
	UINT32 h_downscale_en      : 1;   //     3
	UINT32 v_down_scale_offset : 1;   //     4
} H14A0_DPPA_MOF_CTRL8_T;

typedef struct {
	H14A0_DPPA_MOF_CTRL0_T  dppa_mof_ctrl0;          // 0x5a00
	H14A0_DPPA_MOF_CTRL1_T  dppa_mof_ctrl1;          // 0x5a04
	H14A0_DPPA_MOF_CTRL2_T  dppa_mof_ctrl2;          // 0x5a08
	H14A0_DPPA_MOF_CTRL3_T  dppa_mof_ctrl3;          // 0x5a0c
	H14A0_DPPA_MOF_CTRL4_T  dppa_mof_ctrl4;          // 0x5a10
	H14A0_DPPA_MOF_CTRL5_T  dppa_mof_ctrl5;          // 0x5a14
	H14A0_DPPA_MOF_CTRL6_T  dppa_mof_ctrl6;          // 0x5a18
	H14A0_DPPA_MOF_CTRL7_T  dppa_mof_ctrl7;          // 0x5a1c
	H14A0_DPPA_MOF_CTRL8_T  dppa_mof_ctrl8;          // 0x5a20
} DE_MOF_REG_H14A0_T;

#endif
