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
 * date		  2011.02.16
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef  DE_CTRL_REG_M14_INC
#define  DE_CTRL_REG_M14_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_ctr_reg_m14a0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_ctr_reg_m14b0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_DEA_M14_RdFL(_r)								FN_CHIP_RdFL(DE_DEA, M14, _r)
#define DE_DEA_M14_WrFL(_r)								FN_CHIP_WrFL(DE_DEA, M14, _r)
#define DE_DEA_M14_Rd(_r)								FN_CHIP_Rd32(DE_DEA, M14, _r)
#define DE_DEA_M14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEA, M14, _r, _v)
#define DE_DEA_M14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEA, M14, _r, _f01, _v01)
#define DE_DEA_M14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEA, M14, _r, _f01, _v01)
#define DE_DEA_M14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEA, M14, _r, _v)
#define DE_DEA_M14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEA, M14, _r, _v)
#define DE_DEA_M14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEA, M14, _r, _f01, _v01)
#define DE_DEA_M14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEA, M14, _r, _f01, _v01)

#define DE_DEB_M14_RdFL(_r)								FN_CHIP_RdFL(DE_DEB, M14, _r)
#define DE_DEB_M14_WrFL(_r)								FN_CHIP_WrFL(DE_DEB, M14, _r)
#define DE_DEB_M14_Rd(_r)								FN_CHIP_Rd32(DE_DEB, M14, _r)
#define DE_DEB_M14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEB, M14, _r, _v)
#define DE_DEB_M14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEB, M14, _r, _f01, _v01)
#define DE_DEB_M14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEB, M14, _r, _f01, _v01)
#define DE_DEB_M14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEB, M14, _r, _v)
#define DE_DEB_M14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEB, M14, _r, _v)
#define DE_DEB_M14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEB, M14, _r, _f01, _v01)
#define DE_DEB_M14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEB, M14, _r, _f01, _v01)

#define DE_DEC_M14_RdFL(_r)								FN_CHIP_RdFL(DE_DEC, M14, _r)
#define DE_DEC_M14_WrFL(_r)								FN_CHIP_WrFL(DE_DEC, M14, _r)
#define DE_DEC_M14_Rd(_r)								FN_CHIP_Rd32(DE_DEC, M14, _r)
#define DE_DEC_M14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEC, M14, _r, _v)
#define DE_DEC_M14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEC, M14, _r, _f01, _v01)
#define DE_DEC_M14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEC, M14, _r, _f01, _v01)
#define DE_DEC_M14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEC, M14, _r, _v)
#define DE_DEC_M14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEC, M14, _r, _v)
#define DE_DEC_M14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEC, M14, _r, _f01, _v01)
#define DE_DEC_M14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEC, M14, _r, _f01, _v01)

#define DE_DED_M14_RdFL(_r)								FN_CHIP_RdFL(DE_DED, M14, _r)
#define DE_DED_M14_WrFL(_r)								FN_CHIP_WrFL(DE_DED, M14, _r)
#define DE_DED_M14_Rd(_r)								FN_CHIP_Rd32(DE_DED, M14, _r)
#define DE_DED_M14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DED, M14, _r, _v)
#define DE_DED_M14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DED, M14, _r, _f01, _v01)
#define DE_DED_M14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DED, M14, _r, _f01, _v01)
#define DE_DED_M14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DED, M14, _r, _v)
#define DE_DED_M14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DED, M14, _r, _v)
#define DE_DED_M14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DED, M14, _r, _f01, _v01)
#define DE_DED_M14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DED, M14, _r, _f01, _v01)

#define DE_DEE_M14_RdFL(_r)								FN_CHIP_RdFL(DE_DEE, M14, _r)
#define DE_DEE_M14_WrFL(_r)								FN_CHIP_WrFL(DE_DEE, M14, _r)
#define DE_DEE_M14_Rd(_r)								FN_CHIP_Rd32(DE_DEE, M14, _r)
#define DE_DEE_M14_Wr(_r, _v)							FN_CHIP_Wr32(DE_DEE, M14, _r, _v)
#define DE_DEE_M14_Rd01(_r, _f01, _v01)					FN_CHIP_Rd01(DE_DEE, M14, _r, _f01, _v01)
#define DE_DEE_M14_Wr01(_r, _f01, _v01)					FN_CHIP_Wr01(DE_DEE, M14, _r, _f01, _v01)
#define DE_DEE_M14_FLRd(_r, _v)							FN_CHIP_FLRd(DE_DEE, M14, _r, _v)
#define DE_DEE_M14_FLWr(_r, _v)							FN_CHIP_FLWr(DE_DEE, M14, _r, _v)
#define DE_DEE_M14_FLRf(_r, _f01, _v01)					FN_CHIP_FLRf(DE_DEE, M14, _r, _f01, _v01)
#define DE_DEE_M14_FLWf(_r, _f01, _v01)					FN_CHIP_FLWf(DE_DEE, M14, _r, _f01, _v01)

#define DE_DEI_M14B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEI##_##M14B, b0, _r)
#define DE_DEI_M14B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEI##_##M14B, b0, _r)
#define DE_DEI_M14B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEI##_##M14B, b0, _r)
#define DE_DEI_M14B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEI##_##M14B, b0, _r, _v)
#define DE_DEI_M14B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEI##_##M14B, b0, _r, _f01, _v01)
#define DE_DEI_M14B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEI##_##M14B, b0, _r, _f01, _v01)
#define DE_DEI_M14B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEI##_##M14B, b0, _r, _f01, _v01)
#define DE_DEI_M14B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEI##_##M14B, b0, _r, _f01, _v01)
#define DE_DEI_M14B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEI##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEI_M14B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEI##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEI_M14B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEI##_##M14B, b0, _r, _f, _c, _v1, _v2)
#define DE_DEI_M14B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEI##_##M14B, b0, _r, _v)
#define DE_DEI_M14B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEI##_##M14B, b0, _r, _v)
#define DE_DEI_M14B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEI##_##M14B, b0, _r, _f, _c, _v1, _v2)

#define DE_DEM_M14B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEM##_##M14B, b0, _r)
#define DE_DEM_M14B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEM##_##M14B, b0, _r)
#define DE_DEM_M14B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEM##_##M14B, b0, _r)
#define DE_DEM_M14B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEM##_##M14B, b0, _r, _v)
#define DE_DEM_M14B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEM##_##M14B, b0, _r, _f01, _v01)
#define DE_DEM_M14B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEM##_##M14B, b0, _r, _f01, _v01)
#define DE_DEM_M14B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEM##_##M14B, b0, _r, _f01, _v01)
#define DE_DEM_M14B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEM##_##M14B, b0, _r, _f01, _v01)
#define DE_DEM_M14B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEM##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEM_M14B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEM##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEM_M14B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEM##_##M14B, b0, _r, _f, _c, _v1, _v2)
#define DE_DEM_M14B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEM##_##M14B, b0, _r, _v)
#define DE_DEM_M14B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEM##_##M14B, b0, _r, _v)
#define DE_DEM_M14B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEM##_##M14B, b0, _r, _f, _c, _v1, _v2)

#define DE_DEO_M14B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEO##_##M14B, b0, _r)
#define DE_DEO_M14B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEO##_##M14B, b0, _r)
#define DE_DEO_M14B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEO##_##M14B, b0, _r)
#define DE_DEO_M14B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEO##_##M14B, b0, _r, _v)
#define DE_DEO_M14B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEO##_##M14B, b0, _r, _f01, _v01)
#define DE_DEO_M14B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEO##_##M14B, b0, _r, _f01, _v01)
#define DE_DEO_M14B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEO##_##M14B, b0, _r, _f01, _v01)
#define DE_DEO_M14B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEO##_##M14B, b0, _r, _f01, _v01)
#define DE_DEO_M14B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEO##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEO_M14B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEO##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEO_M14B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEO##_##M14B, b0, _r, _f, _c, _v1, _v2)
#define DE_DEO_M14B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEO##_##M14B, b0, _r, _v)
#define DE_DEO_M14B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEO##_##M14B, b0, _r, _v)
#define DE_DEO_M14B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEO##_##M14B, b0, _r, _f, _c, _v1, _v2)

#define DE_DEP_M14B_RdFL(_r)							REG_PHYS_VER_RdFL(g##DE_DEP##_##M14B, b0, _r)
#define DE_DEP_M14B_WrFL(_r)							REG_PHYS_VER_WrFL(g##DE_DEP##_##M14B, b0, _r)
#define DE_DEP_M14B_Rd(_r)								REG_SHDW_VER_Rd32(g##DE_DEP##_##M14B, b0, _r)
#define DE_DEP_M14B_Wr(_r, _v)							REG_SHDW_VER_Wr32(g##DE_DEP##_##M14B, b0, _r, _v)
#define DE_DEP_M14B_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_DEP##_##M14B, b0, _r, _f01, _v01)
#define DE_DEP_M14B_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_DEP##_##M14B, b0, _r, _f01, _v01)
#define DE_DEP_M14B_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_DEP##_##M14B, b0, _r, _f01, _v01)
#define DE_DEP_M14B_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_DEP##_##M14B, b0, _r, _f01, _v01)
#define DE_DEP_M14B_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_DEP##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEP_M14B_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_DEP##_##M14B, b0, _r, _f, _c, _m)
#define DE_DEP_M14B_WfCV(_r, _f, _c, _v1, _v2)			REG_SHDW_VER_WfCV(g##DE_DEP##_##M14B, b0, _r, _f, _c, _v1, _v2)
#define DE_DEP_M14B_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_DEP##_##M14B, b0, _r, _v)
#define DE_DEP_M14B_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_DEP##_##M14B, b0, _r, _v)
#define DE_DEP_M14B_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_DEP##_##M14B, b0, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_DEA_REG_M14A0_T *a0;
		DE_DEA_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEA_REG_M14A0_T *a0;
		volatile DE_DEA_REG_M14A0_T *b0;
	} phys;
} DE_DEA_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEB_REG_M14A0_T *a0;
		DE_DEB_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEB_REG_M14A0_T *a0;
		volatile DE_DEB_REG_M14A0_T *b0;
	} phys;
} DE_DEB_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEC_REG_M14A0_T *a0;
		DE_DEC_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEC_REG_M14A0_T *a0;
		volatile DE_DEC_REG_M14A0_T *b0;
	} phys;
} DE_DEC_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DED_REG_M14A0_T *a0;
		DE_DED_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DED_REG_M14A0_T *a0;
		volatile DE_DED_REG_M14A0_T *b0;
	} phys;
} DE_DED_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEE_REG_M14A0_T *a0;
		DE_DEE_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEE_REG_M14A0_T *a0;
		volatile DE_DEE_REG_M14A0_T *b0;
	} phys;
} DE_DEE_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEF_REG_M14A0_T *a0;
		DE_DEF_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEF_REG_M14A0_T *a0;
		volatile DE_DEF_REG_M14A0_T *b0;
	} phys;
} DE_DEF_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEI_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEI_REG_M14B0_T *b0;
	} phys;
} DE_DEI_REG_M14B_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEM_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEM_REG_M14B0_T *b0;
	} phys;
} DE_DEM_REG_M14B_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEO_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEO_REG_M14B0_T *b0;
	} phys;
} DE_DEO_REG_M14B_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_DEP_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_DEP_REG_M14B0_T *b0;
	} phys;
} DE_DEP_REG_M14B_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_CTRL_REG_M14_INC  ----- */
/**  @} */
