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
#ifndef  DE_MSC_REG_M14_INC
#define  DE_MSC_REG_M14_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_msc_reg_m14a0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_msc_reg_m14b0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_MSL_M14_RdFL(_r)								FN_CHIP_RdFL(DE_MSL, M14, _r)
#define DE_MSL_M14_WrFL(_r)								FN_CHIP_WrFL(DE_MSL, M14, _r)
#define DE_MSL_M14_Rd(_r)								FN_CHIP_Rd32(DE_MSL, M14, _r)
#define DE_MSL_M14_Wr(_r, _v)							FN_CHIP_Wr32(DE_MSL, M14, _r, _v)
#define DE_MSL_M14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_MSL, M14, _r, _f01, _v01)
#define DE_MSL_M14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_MSL, M14, _r, _f01, _v01)
#define DE_MSL_M14_WfCM(_r, _f, _c, _m)					FN_CHIP_WfCM(DE_MSL, M14, _r, _f, _c, _m)
#define DE_MSL_M14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_MSL, M14, _r, _v)
#define DE_MSL_M14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_MSL, M14, _r, _v)
#define DE_MSL_M14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_MSL, M14, _r, _f01, _v01)
#define DE_MSL_M14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_MSL, M14, _r, _f01, _v01)

#define DE_MSR_M14_RdFL(_r)								FN_CHIP_RdFL(DE_MSR, M14, _r)
#define DE_MSR_M14_WrFL(_r)								FN_CHIP_WrFL(DE_MSR, M14, _r)
#define DE_MSR_M14_Rd(_r)								FN_CHIP_Rd32(DE_MSR, M14, _r)
#define DE_MSR_M14_Wr(_r, _v)							FN_CHIP_Wr32(DE_MSR, M14, _r, _v)
#define DE_MSR_M14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_MSR, M14, _r, _f01, _v01)
#define DE_MSR_M14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_MSR, M14, _r, _f01, _v01)
#define DE_MSR_M14_WfCM(_r, _f, _c, _m)					FN_CHIP_WfCM(DE_MSR, M14, _r, _f, _c, _m)
#define DE_MSR_M14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_MSR, M14, _r, _v)
#define DE_MSR_M14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_MSR, M14, _r, _v)
#define DE_MSR_M14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_MSR, M14, _r, _f01, _v01)
#define DE_MSR_M14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_MSR, M14, _r, _f01, _v01)

#define DE_MSR_M14A_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_MSR##_##M14, a0, _r)
#define DE_MSR_M14A_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_MSR##_##M14, a0, _r)
#define DE_MSR_M14A_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_MSR##_##M14, a0, _r)
#define DE_MSR_M14A_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_MSR##_##M14, a0, _r, _v)
#define DE_MSR_M14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_MSR##_##M14, a0, _r, _f01, _v01)
#define DE_MSR_M14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_MSR##_##M14, a0, _r, _f01, _v01)
#define DE_MSR_M14A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_MSR##_##M14, a0, _r, _f01, _v01)
#define DE_MSR_M14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_MSR##_##M14, a0, _r, _f01, _v01)
#define DE_MSR_M14A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_MSR##_##M14, a0, _r, _f, _c, _m)
#define DE_MSR_M14A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_MSR##_##M14, a0, _r, _f, _c, _m)
#define DE_MSR_M14A_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_MSR##_##M14, a0, _r, _f, _c, _v1, _v2)
#define DE_MSR_M14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_MSR##_##M14, a0, _r, _v)
#define DE_MSR_M14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_MSR##_##M14, a0, _r, _v)
#define DE_MSR_M14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_MSR##_##M14, a0, _r, _f, _c, _v1, _v2)

#define DE_MSR_M14B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_MSR##_##M14, b0, _r)
#define DE_MSR_M14B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_MSR##_##M14, b0, _r)
#define DE_MSR_M14B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_MSR##_##M14, b0, _r)
#define DE_MSR_M14B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_MSR##_##M14, b0, _r, _v)
#define DE_MSR_M14B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_MSR##_##M14, b0, _r, _f01, _v01)
#define DE_MSR_M14B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_MSR##_##M14, b0, _r, _f01, _v01)
#define DE_MSR_M14B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_MSR##_##M14, b0, _r, _f01, _v01)
#define DE_MSR_M14B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_MSR##_##M14, b0, _r, _f01, _v01)
#define DE_MSR_M14B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_MSR##_##M14, b0, _r, _f, _c, _m)
#define DE_MSR_M14B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_MSR##_##M14, b0, _r, _f, _c, _m)
#define DE_MSR_M14B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_MSR##_##M14, b0, _r, _f, _c, _v1, _v2)
#define DE_MSR_M14B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_MSR##_##M14, b0, _r, _v)
#define DE_MSR_M14B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_MSR##_##M14, b0, _r, _v)
#define DE_MSR_M14B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_MSR##_##M14, b0, _r, _f, _c, _v1, _v2)

#define DE_MSC_M14B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_MSC##_##M14B, b0, _r)
#define DE_MSC_M14B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_MSC##_##M14B, b0, _r)
#define DE_MSC_M14B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_MSC##_##M14B, b0, _r)
#define DE_MSC_M14B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_MSC##_##M14B, b0, _r, _v)
#define DE_MSC_M14B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_MSC##_##M14B, b0, _r, _f01, _v01)
#define DE_MSC_M14B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_MSC##_##M14B, b0, _r, _f01, _v01)
#define DE_MSC_M14B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_MSC##_##M14B, b0, _r, _f01, _v01)
#define DE_MSC_M14B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_MSC##_##M14B, b0, _r, _f01, _v01)
#define DE_MSC_M14B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_MSC##_##M14B, b0, _r, _f, _c, _m)
#define DE_MSC_M14B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_MSC##_##M14B, b0, _r, _f, _c, _m)
#define DE_MSC_M14B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_MSC##_##M14B, b0, _r, _f, _c, _v1, _v2)
#define DE_MSC_M14B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_MSC##_##M14B, b0, _r, _v)
#define DE_MSC_M14B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_MSC##_##M14B, b0, _r, _v)
#define DE_MSC_M14B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_MSC##_##M14B, b0, _r, _f, _c, _v1, _v2)


/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_MSL_REG_M14A0_T *a0;
		DE_MSL_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MSL_REG_M14A0_T *a0;
		volatile DE_MSL_REG_M14A0_T *b0;
	} phys;
} DE_MSL_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MSR_REG_M14A0_T *a0;
		DE_MSR_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MSR_REG_M14A0_T *a0;
		volatile DE_MSR_REG_M14A0_T *b0;
	} phys;
} DE_MSR_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MSC_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MSC_REG_M14B0_T *b0;
	} phys;
} DE_MSC_REG_M14B_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_MSC_REG_M14_INC  ----- */
/**  @} */
