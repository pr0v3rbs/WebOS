/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file pe_bve_reg_h13.h
 *
 * main driver implementation for pe device.
 * pe device will teach you how to make device driver with new platform.
 *
 * author	  sj.youm (sj.youm@lge.com)
 * version	  1.0
 * date		  2012.12.05
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef _PE_BVE_REG_H13_H_
#define _PE_BVE_REG_H13_H_
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "pe_reg_mac.h"

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
// Ax,Bx
#define PE_BVE_H13_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_BVE_H13,data,_r)
#define PE_BVE_H13_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_BVE_H13,data,_r)
#define PE_BVE_H13_Rd(_r)			PE_REG_SHDW_Rd32(gPE_BVE_H13,data,_r)
#define PE_BVE_H13_Wr(_r,_d1)		PE_REG_SHDW_Wr32(gPE_BVE_H13,data,_r,_d1)
#define PE_BVE_H13_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_BVE_H13,data,_r,_f1,_d1)
#define PE_BVE_H13_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_BVE_H13,data,_r,_f1,_d1)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
		0xc00240a4 bbd_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13_BVE_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc00240a8 bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13_BVE_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc00240ac bbd_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H13_BVE_BBD_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc00240b0 bbd_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H13_BVE_BBD_STAT_03_T;

/*-----------------------------------------------------------------------------
		0xc00240d0 ~ 0xc002414c hist_data00 ~ hist_data31 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13_BVE_HIST_DATA_T;

/*-----------------------------------------------------------------------------
		0xc0024150 apl_l_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_y                              : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 apl_rgb                            : 10;  // 25:16     
} PE_H13_BVE_APL_L_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0024154 apl_r_stat_00 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_y                              : 10;  //  9: 0     
	UINT32                                    : 6 ;  // 15:10     reserved
	UINT32 apl_rgb                            : 10;  // 25:16     
} PE_H13_BVE_APL_R_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc0024158 apl_l_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_b                              : 10;  //  9: 0     
	UINT32 apl_g                              : 10;  // 19:10     
	UINT32 apl_r                              : 10;  // 29:20     
} PE_H13_BVE_APL_L_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc002415c apl_r_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 apl_b                              : 10;  //  9: 0     
	UINT32 apl_g                              : 10;  // 19:10     
	UINT32 apl_r                              : 10;  // 29:20     
} PE_H13_BVE_APL_R_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc0024160 hist_invalid ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32                                    : 32;  // 31: 0     reserved
} PE_H13_BVE_HIST_INVALID_T;

typedef struct {
	PE_H13_BVE_BBD_STAT_00_T         	bbd_stat_00;                 //0xc00240a4
	PE_H13_BVE_BBD_STAT_01_T         	bbd_stat_01;                 //0xc00240a8
	PE_H13_BVE_BBD_STAT_02_T         	bbd_stat_02;                 //0xc00240ac
	PE_H13_BVE_BBD_STAT_03_T         	bbd_stat_03;                 //0xc00240b0
	UINT32								reserve00[7];                //0xc00240b4 ~ 0xc00240cc
	PE_H13_BVE_HIST_DATA_T              hist_data00;                 //0xc00240d0
	PE_H13_BVE_HIST_DATA_T              hist_data01;                 //0xc00240d4
	PE_H13_BVE_HIST_DATA_T              hist_data02;                 //0xc00240d8
	PE_H13_BVE_HIST_DATA_T              hist_data03;                 //0xc00240dc
	PE_H13_BVE_HIST_DATA_T              hist_data04;                 //0xc00240e0
	PE_H13_BVE_HIST_DATA_T              hist_data05;                 //0xc00240e4
	PE_H13_BVE_HIST_DATA_T              hist_data06;                 //0xc00240e8
	PE_H13_BVE_HIST_DATA_T              hist_data07;                 //0xc00240ec
	PE_H13_BVE_HIST_DATA_T              hist_data08;                 //0xc00240f0
	PE_H13_BVE_HIST_DATA_T              hist_data09;                 //0xc00240f4
	PE_H13_BVE_HIST_DATA_T              hist_data10;                 //0xc00240f8
	PE_H13_BVE_HIST_DATA_T              hist_data11;                 //0xc00240fc
	PE_H13_BVE_HIST_DATA_T              hist_data12;                 //0xc0024100
	PE_H13_BVE_HIST_DATA_T              hist_data13;                 //0xc0024104
	PE_H13_BVE_HIST_DATA_T              hist_data14;                 //0xc0024108
	PE_H13_BVE_HIST_DATA_T              hist_data15;                 //0xc002410c
	PE_H13_BVE_HIST_DATA_T              hist_data16;                 //0xc0024110
	PE_H13_BVE_HIST_DATA_T              hist_data17;                 //0xc0024114
	PE_H13_BVE_HIST_DATA_T              hist_data18;                 //0xc0024118
	PE_H13_BVE_HIST_DATA_T              hist_data19;                 //0xc002411c
	PE_H13_BVE_HIST_DATA_T              hist_data20;                 //0xc0024120
	PE_H13_BVE_HIST_DATA_T              hist_data21;                 //0xc0024124
	PE_H13_BVE_HIST_DATA_T              hist_data22;                 //0xc0024128
	PE_H13_BVE_HIST_DATA_T              hist_data23;                 //0xc002412c
	PE_H13_BVE_HIST_DATA_T              hist_data24;                 //0xc0024130
	PE_H13_BVE_HIST_DATA_T              hist_data25;                 //0xc0024134
	PE_H13_BVE_HIST_DATA_T              hist_data26;                 //0xc0024138
	PE_H13_BVE_HIST_DATA_T              hist_data27;                 //0xc002413c
	PE_H13_BVE_HIST_DATA_T              hist_data28;                 //0xc0024140
	PE_H13_BVE_HIST_DATA_T              hist_data29;                 //0xc0024144
	PE_H13_BVE_HIST_DATA_T              hist_data30;                 //0xc0024148
	PE_H13_BVE_HIST_DATA_T              hist_data31;                 //0xc002414c
	PE_H13_BVE_APL_L_STAT_00_T          apl_l_stat_00;               //0xc0024150
	PE_H13_BVE_APL_R_STAT_00_T          apl_r_stat_00;               //0xc0024154
	PE_H13_BVE_APL_L_STAT_01_T          apl_l_stat_01;               //0xc0024158
	PE_H13_BVE_APL_R_STAT_01_T          apl_r_stat_01;               //0xc002415c
	PE_H13_BVE_HIST_INVALID_T           hist_invalid;                //0xc0024160
}PE_BVE_REG_H13X_T;

typedef struct {
	union {
		UINT32			*addr;
		PE_BVE_REG_H13X_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_BVE_REG_H13X_T *data;
	} phys;
} PE_BVE_REG_H13_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* _PE_BVE_REG_H13_H_ */

