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
#ifndef  DE_MSC_REG_H13_INC
#define  DE_MSC_REG_H13_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_H13A0
#include "de_msc_reg_h13a0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_H13B0
#include "de_msc_reg_h13b0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_MSL_H13_RdFL(_r)								FN_CHIP_RdFL(DE_MSL, H13, _r)
#define DE_MSL_H13_WrFL(_r)								FN_CHIP_WrFL(DE_MSL, H13, _r)
#define DE_MSL_H13_Rd(_r)								FN_CHIP_Rd32(DE_MSL, H13, _r)
#define DE_MSL_H13_Wr(_r, _v)							FN_CHIP_Wr32(DE_MSL, H13, _r, _v)
#define DE_MSL_H13_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_MSL, H13, _r, _f01, _v01)
#define DE_MSL_H13_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_MSL, H13, _r, _f01, _v01)
#define DE_MSL_H13_WfCM(_r, _f, _c, _m)					FN_CHIP_WfCM(DE_MSL, H13, _r, _f, _c, _m)
#define DE_MSL_H13_FLRd(_r, _v)							FN_CHIP_FLRd(DE_MSL, H13, _r, _v)
#define DE_MSL_H13_FLWr(_r, _v)							FN_CHIP_FLWr(DE_MSL, H13, _r, _v)
#define DE_MSL_H13_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_MSL, H13, _r, _f01, _v01)
#define DE_MSL_H13_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_MSL, H13, _r, _f01, _v01)

#define DE_MSR_H13_RdFL(_r)								FN_CHIP_RdFL(DE_MSR, H13, _r)
#define DE_MSR_H13_WrFL(_r)								FN_CHIP_WrFL(DE_MSR, H13, _r)
#define DE_MSR_H13_Rd(_r)								FN_CHIP_Rd32(DE_MSR, H13, _r)
#define DE_MSR_H13_Wr(_r, _v)							FN_CHIP_Wr32(DE_MSR, H13, _r, _v)
#define DE_MSR_H13_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_MSR, H13, _r, _f01, _v01)
#define DE_MSR_H13_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_MSR, H13, _r, _f01, _v01)
#define DE_MSR_H13_WfCM(_r, _f, _c, _m)					FN_CHIP_WfCM(DE_MSR, H13, _r, _f, _c, _m)
#define DE_MSR_H13_FLRd(_r, _v)							FN_CHIP_FLRd(DE_MSR, H13, _r, _v)
#define DE_MSR_H13_FLWr(_r, _v)							FN_CHIP_FLWr(DE_MSR, H13, _r, _v)
#define DE_MSR_H13_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_MSR, H13, _r, _f01, _v01)
#define DE_MSR_H13_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_MSR, H13, _r, _f01, _v01)

#define DE_MSR_H13A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_MSR##_##H13, a0, _r)
#define DE_MSR_H13A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_MSR##_##H13, a0, _r)
#define DE_MSR_H13A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_MSR##_##H13, a0, _r)
#define DE_MSR_H13A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_MSR##_##H13, a0, _r, _v)
#define DE_MSR_H13A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_MSR##_##H13, a0, _r, _f01, _v01)
#define DE_MSR_H13A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_MSR##_##H13, a0, _r, _f01, _v01)
#define DE_MSR_H13A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_MSR##_##H13, a0, _r, _f01, _v01)
#define DE_MSR_H13A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_MSR##_##H13, a0, _r, _f01, _v01)
#define DE_MSR_H13A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_MSR##_##H13, a0, _r, _f, _c, _m)
#define DE_MSR_H13A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_MSR##_##H13, a0, _r, _f, _c, _m)
#define DE_MSR_H13A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_MSR##_##H13, a0, _r, _f, _c, _v1, _v2)
#define DE_MSR_H13A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_MSR##_##H13, a0, _r, _v)
#define DE_MSR_H13A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_MSR##_##H13, a0, _r, _v)
#define DE_MSR_H13A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_MSR##_##H13, a0, _r, _f, _c, _v1, _v2)

#define DE_MSR_H13B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_MSR##_##H13, b0, _r)
#define DE_MSR_H13B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_MSR##_##H13, b0, _r)
#define DE_MSR_H13B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_MSR##_##H13, b0, _r)
#define DE_MSR_H13B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_MSR##_##H13, b0, _r, _v)
#define DE_MSR_H13B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_MSR##_##H13, b0, _r, _f01, _v01)
#define DE_MSR_H13B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_MSR##_##H13, b0, _r, _f01, _v01)
#define DE_MSR_H13B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_MSR##_##H13, b0, _r, _f01, _v01)
#define DE_MSR_H13B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_MSR##_##H13, b0, _r, _f01, _v01)
#define DE_MSR_H13B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_MSR##_##H13, b0, _r, _f, _c, _m)
#define DE_MSR_H13B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_MSR##_##H13, b0, _r, _f, _c, _m)
#define DE_MSR_H13B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_MSR##_##H13, b0, _r, _f, _c, _v1, _v2)
#define DE_MSR_H13B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_MSR##_##H13, b0, _r, _v)
#define DE_MSR_H13B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_MSR##_##H13, b0, _r, _v)
#define DE_MSR_H13B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_MSR##_##H13, b0, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_MSL_REG_H13A0_T *a0;
		DE_MSL_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MSL_REG_H13A0_T *a0;
		volatile DE_MSL_REG_H13B0_T *b0;
	} phys;
} DE_MSL_REG_H13_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MSR_REG_H13A0_T *a0;
		DE_MSR_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MSR_REG_H13A0_T *a0;
		volatile DE_MSR_REG_H13B0_T *b0;
	} phys;
} DE_MSR_REG_H13_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_MSC_REG_H13_INC  ----- */
/**  @} */
