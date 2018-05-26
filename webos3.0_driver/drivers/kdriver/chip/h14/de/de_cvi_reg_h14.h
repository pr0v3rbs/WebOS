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


/** @file
 *
 * main driver implementation for de device.
 * de device will teach you how to make device driver with new platform.
 *
 * author	  jaemo.kim (jaemo.kim@lge.com)
 * version	  1.0
 * date		  2011.02.17
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef  DE_CVI_REG_L8_INC
#define  DE_CVI_REG_L8_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_H14A0
#include "de_cvi_reg_h14a0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_CVA_H14_RdFL(_r)						FN_CHIP_RdFL(DE_CVA, H14, _r)
#define DE_CVA_H14_WrFL(_r)						FN_CHIP_WrFL(DE_CVA, H14, _r)
#define DE_CVA_H14_Rd(_r)						FN_CHIP_Rd32(DE_CVA, H14, _r)
#define DE_CVA_H14_Wr(_r, _v)					FN_CHIP_Wr32(DE_CVA, H14, _r, _v)
#define DE_CVA_H14_Rd01(_r, _f01, _v01)			FN_CHIP_Rd01(DE_CVA, H14, _r, _f01, _v01)
#define DE_CVA_H14_Wr01(_r, _f01, _v01)			FN_CHIP_Wr01(DE_CVA, H14, _r, _f01, _v01)
#define DE_CVA_H14_FLRf(_r, _f01, _v01)			FN_CHIP_FLRf(DE_CVA, H14, _r, _f01, _v01)
#define DE_CVA_H14_FLWf(_r, _f01, _v01)			FN_CHIP_FLWf(DE_CVA, H14, _r, _f01, _v01)
#define DE_CVA_H14_WfCM(_r, _f, _c, _m)			FN_CHIP_WfCM(DE_CVA, H14, _r, _f, _c, _m)
#define DE_CVA_H14_FLCM(_r, _f, _c, _m)			FN_CHIP_FLCM(DE_CVA, H14, _r, _f, _c, _m)

#define DE_CVB_H14_RdFL(_r)						FN_CHIP_RdFL(DE_CVB, H14, _r)
#define DE_CVB_H14_WrFL(_r)						FN_CHIP_WrFL(DE_CVB, H14, _r)
#define DE_CVB_H14_Rd(_r)						FN_CHIP_Rd32(DE_CVB, H14, _r)
#define DE_CVB_H14_Wr(_r, _v)					FN_CHIP_Wr32(DE_CVB, H14, _r, _v)
#define DE_CVB_H14_Rd01(_r, _f01, _v01)			FN_CHIP_Rd01(DE_CVB, H14, _r, _f01, _v01)
#define DE_CVB_H14_Wr01(_r, _f01, _v01)			FN_CHIP_Wr01(DE_CVB, H14, _r, _f01, _v01)
#define DE_CVB_H14_FLRf(_r, _f01, _v01)			FN_CHIP_FLRf(DE_CVB, H14, _r, _f01, _v01)
#define DE_CVB_H14_FLWf(_r, _f01, _v01)			FN_CHIP_FLWf(DE_CVB, H14, _r, _f01, _v01)
#define DE_CVB_H14_WfCM(_r, _f, _c, _m)			FN_CHIP_WfCM(DE_CVB, H14, _r, _f, _c, _m)
#define DE_CVB_H14_FLCM(_r, _f, _c, _m)			FN_CHIP_FLCM(DE_CVB, H14, _r, _f, _c, _m)

#define DE_CVA_H14A_RdFL(_r)					REG_PHYS_VER_RdFL(g##DE_CVA##_##H14, a0, _r)
#define DE_CVA_H14A_WrFL(_r)					REG_PHYS_VER_WrFL(g##DE_CVA##_##H14, a0, _r)
#define DE_CVA_H14A_Rd(_r)						REG_SHDW_VER_Rd32(g##DE_CVA##_##H14, a0, _r)
#define DE_CVA_H14A_Wr(_r, _v)					REG_SHDW_VER_Wr32(g##DE_CVA##_##H14, a0, _r, _v)
#define DE_CVA_H14A_Rd01(_r, _f01, _v01)		REG_SHDW_VER_Rd01(g##DE_CVA##_##H14, a0, _r, _f01, _v01)
#define DE_CVA_H14A_Wr01(_r, _f01, _v01)		REG_SHDW_VER_Wr01(g##DE_CVA##_##H14, a0, _r, _f01, _v01)
#define DE_CVA_H14A_FLRf(_r, _f01, _v01)		REG_PHYS_VER_FLRf(g##DE_CVA##_##H14, a0, _r, _f01, _v01)
#define DE_CVA_H14A_FLWf(_r, _f01, _v01)		REG_PHYS_VER_FLWf(g##DE_CVA##_##H14, a0, _r, _f01, _v01)
#define DE_CVA_H14A_WfCM(_r, _f, _c, _m)		REG_SHDW_VER_WfCM(g##DE_CVA##_##H14, a0, _r, _f, _c, _m)
#define DE_CVA_H14A_FLCM(_r, _f, _c, _m)		REG_PHYS_VER_FLCM(g##DE_CVA##_##H14, a0, _r, _f, _c, _m)

#define DE_CVB_H14A_RdFL(_r)					REG_PHYS_VER_RdFL(g##DE_CVB##_##H14, a0, _r)
#define DE_CVB_H14A_WrFL(_r)					REG_PHYS_VER_WrFL(g##DE_CVB##_##H14, a0, _r)
#define DE_CVB_H14A_Rd(_r)						REG_SHDW_VER_Rd32(g##DE_CVB##_##H14, a0, _r)
#define DE_CVB_H14A_Wr(_r, _v)					REG_SHDW_VER_Wr32(g##DE_CVB##_##H14, a0, _r, _v)
#define DE_CVB_H14A_Rd01(_r, _f01, _v01)		REG_SHDW_VER_Rd01(g##DE_CVB##_##H14, a0, _r, _f01, _v01)
#define DE_CVB_H14A_Wr01(_r, _f01, _v01)		REG_SHDW_VER_Wr01(g##DE_CVB##_##H14, a0, _r, _f01, _v01)
#define DE_CVB_H14A_FLRf(_r, _f01, _v01)		REG_PHYS_VER_FLRf(g##DE_CVB##_##H14, a0, _r, _f01, _v01)
#define DE_CVB_H14A_FLWf(_r, _f01, _v01)		REG_PHYS_VER_FLWf(g##DE_CVB##_##H14, a0, _r, _f01, _v01)
#define DE_CVB_H14A_WfCM(_r, _f, _c, _m)		REG_SHDW_VER_WfCM(g##DE_CVB##_##H14, a0, _r, _f, _c, _m)
#define DE_CVB_H14A_FLCM(_r, _f, _c, _m)		REG_PHYS_VER_FLCM(g##DE_CVB##_##H14, a0, _r, _f, _c, _m)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_CVA_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_CVA_REG_H14A0_T *a0;
	} phys;
} DE_CVA_REG_H14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_CVB_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_CVB_REG_H14A0_T *a0;
	} phys;
} DE_CVB_REG_H14_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_CVI_REG_L8_INC  ----- */
/**  @} */
