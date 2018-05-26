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

/** @file pe_bve_reg_h14.h
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
#ifndef _PE_BVE_REG_H14_H_
#define _PE_BVE_REG_H14_H_
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
#define PE_BVE_H14_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_BVE_H14,data,_r)
#define PE_BVE_H14_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_BVE_H14,data,_r)
#define PE_BVE_H14_Rd(_r)			PE_REG_SHDW_Rd32(gPE_BVE_H14,data,_r)
#define PE_BVE_H14_Wr(_r,_d1)		PE_REG_SHDW_Wr32(gPE_BVE_H14,data,_r,_d1)
#define PE_BVE_H14_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_BVE_H14,data,_r,_f1,_d1)
#define PE_BVE_H14_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_BVE_H14,data,_r,_f1,_d1)

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
} PE_H14_BVE_BBD_STAT_00_T;

/*-----------------------------------------------------------------------------
		0xc00240a8 bbd_stat_01 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H14_BVE_BBD_STAT_01_T;

/*-----------------------------------------------------------------------------
		0xc00240ac bbd_stat_02 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x0                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y0                                 : 12;  // 27:16     
} PE_H14_BVE_BBD_STAT_02_T;

/*-----------------------------------------------------------------------------
		0xc00240b0 bbd_stat_03 ''
-----------------------------------------------------------------------------*/
typedef struct {
	UINT32 x1                                 : 12;  // 11: 0     
	UINT32                                    : 4 ;  // 15:12     reserved
	UINT32 y1                                 : 12;  // 27:16     
} PE_H14_BVE_BBD_STAT_03_T;

typedef struct {
	PE_H14_BVE_BBD_STAT_00_T         	bbd_stat_00;                 //0xc00240a4
	PE_H14_BVE_BBD_STAT_01_T         	bbd_stat_01;                 //0xc00240a8
	PE_H14_BVE_BBD_STAT_02_T         	bbd_stat_02;                 //0xc00240ac
	PE_H14_BVE_BBD_STAT_03_T         	bbd_stat_03;                 //0xc00240b0
} PE_BVE_REG_H14X_T;

typedef struct {
	union {
		UINT32			*addr;
		PE_BVE_REG_H14X_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_BVE_REG_H14X_T *data;
	} phys;
} PE_BVE_REG_H14_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* _PE_BVE_REG_H14_H_ */

