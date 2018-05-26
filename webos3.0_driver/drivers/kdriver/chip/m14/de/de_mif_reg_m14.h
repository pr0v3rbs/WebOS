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
#ifndef  DE_MIF_REG_M14_INC
#define  DE_MIF_REG_M14_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_mif_reg_m14a0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_mif_reg_m14b0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_MIA_M14_RdFL(_r)							FN_CHIP_RdFL(DE_MIA, M14, _r)
#define DE_MIA_M14_WrFL(_r)							FN_CHIP_WrFL(DE_MIA, M14, _r)
#define DE_MIA_M14_Rd(_r)							FN_CHIP_Rd32(DE_MIA, M14, _r)
#define DE_MIA_M14_Wr(_r, _v)						FN_CHIP_Wr32(DE_MIA, M14, _r, _v)
#define DE_MIA_M14_Rd01(_r, _f01, _v01)				FN_CHIP_Rd01(DE_MIA, M14, _r, _f01, _v01)
#define DE_MIA_M14_Wr01(_r, _f01, _v01)				FN_CHIP_Wr01(DE_MIA, M14, _r, _f01, _v01)
#define DE_MIA_M14_WfCM(_r, _f, _c, _m)				FN_CHIP_WfCM(DE_MIA, M14, _r, _f, _c, _m)
#define DE_MIA_M14_WfCV(_r, _f, _c, _v1, _v2)		FN_CHIP_WfCV(DE_MIA, M14, _r, _f, _c, _v1, _v2)
#define DE_MIA_M14_FLRd(_r, _v)						FN_CHIP_FLRd(DE_MIA, M14, _r, _v)
#define DE_MIA_M14_FLWr(_r, _v)						FN_CHIP_FLWr(DE_MIA, M14, _r, _v)
#define DE_MIA_M14_FLRf(_r, _f01, _v01)				FN_CHIP_FLRf(DE_MIA, M14, _r, _f01, _v01)
#define DE_MIA_M14_FLWf(_r, _f01, _v01)				FN_CHIP_FLWf(DE_MIA, M14, _r, _f01, _v01)
#define DE_MIA_M14_FLCM(_r, _f, _c, _m)				FN_CHIP_FLCM(DE_MIA, M14, _r, _f, _c, _m)
#define DE_MIA_M14_FLCV(_r, _f, _c, _v1, _v2)		FN_CHIP_FLCV(DE_MIA, M14, _r, _f, _c, _v1, _v2)

#define DE_MIB_M14_RdFL(_r)							FN_CHIP_RdFL(DE_MIB, M14, _r)
#define DE_MIB_M14_WrFL(_r)							FN_CHIP_WrFL(DE_MIB, M14, _r)
#define DE_MIB_M14_Rd(_r)							FN_CHIP_Rd32(DE_MIB, M14, _r)
#define DE_MIB_M14_Wr(_r, _v)						FN_CHIP_Wr32(DE_MIB, M14, _r, _v)
#define DE_MIB_M14_Rd01(_r, _f01, _v01)				FN_CHIP_Rd01(DE_MIB, M14, _r, _f01, _v01)
#define DE_MIB_M14_Wr01(_r, _f01, _v01)				FN_CHIP_Wr01(DE_MIB, M14, _r, _f01, _v01)
#define DE_MIB_M14_WfCM(_r, _f, _c, _m)				FN_CHIP_WfCM(DE_MIB, M14, _r, _f, _c, _m)
#define DE_MIB_M14_WfCV(_r, _f, _c, _v1, _v2)		FN_CHIP_WfCV(DE_MIB, M14, _r, _f, _c, _v1, _v2)
#define DE_MIB_M14_FLRd(_r, _v)						FN_CHIP_FLRd(DE_MIB, M14, _r, _v)
#define DE_MIB_M14_FLWr(_r, _v)						FN_CHIP_FLWr(DE_MIB, M14, _r, _v)
#define DE_MIB_M14_FLRf(_r, _f01, _v01)				FN_CHIP_FLRf(DE_MIB, M14, _r, _f01, _v01)
#define DE_MIB_M14_FLWf(_r, _f01, _v01)				FN_CHIP_FLWf(DE_MIB, M14, _r, _f01, _v01)
#define DE_MIB_M14_FLCM(_r, _f, _c, _m)				FN_CHIP_FLCM(DE_MIB, M14, _r, _f, _c, _m)
#define DE_MIB_M14_FLCV(_r, _f, _c, _v1, _v2)		FN_CHIP_FLCV(DE_MIB, M14, _r, _f, _c, _v1, _v2)

#define DE_MIC_M14_RdFL(_r)							FN_CHIP_RdFL(DE_MIC, M14, _r)
#define DE_MIC_M14_WrFL(_r)							FN_CHIP_WrFL(DE_MIC, M14, _r)
#define DE_MIC_M14_Rd(_r)							FN_CHIP_Rd32(DE_MIC, M14, _r)
#define DE_MIC_M14_Wr(_r, _v)						FN_CHIP_Wr32(DE_MIC, M14, _r, _v)
#define DE_MIC_M14_Rd01(_r, _f01, _v01)				FN_CHIP_Rd01(DE_MIC, M14, _r, _f01, _v01)
#define DE_MIC_M14_Wr01(_r, _f01, _v01)				FN_CHIP_Wr01(DE_MIC, M14, _r, _f01, _v01)
#define DE_MIC_M14_WfCM(_r, _f, _c, _m)				FN_CHIP_WfCM(DE_MIC, M14, _r, _f, _c, _m)
#define DE_MIC_M14_WfCV(_r, _f, _c, _v1, _v2)		FN_CHIP_WfCV(DE_MIC, M14, _r, _f, _c, _v1, _v2)
#define DE_MIC_M14_FLRd(_r, _v)						FN_CHIP_FLRd(DE_MIC, M14, _r, _v)
#define DE_MIC_M14_FLWr(_r, _v)						FN_CHIP_FLWr(DE_MIC, M14, _r, _v)
#define DE_MIC_M14_FLRf(_r, _f01, _v01)				FN_CHIP_FLRf(DE_MIC, M14, _r, _f01, _v01)
#define DE_MIC_M14_FLWf(_r, _f01, _v01)				FN_CHIP_FLWf(DE_MIC, M14, _r, _f01, _v01)
#define DE_MIC_M14_FLCM(_r, _f, _c, _m)				FN_CHIP_FLCM(DE_MIC, M14, _r, _f, _c, _m)
#define DE_MIC_M14_FLCV(_r, _f, _c, _v1, _v2)		FN_CHIP_FLCV(DE_MIC, M14, _r, _f, _c, _v1, _v2)

#define DE_MID_M14_RdFL(_r)							FN_CHIP_RdFL(DE_MID, M14, _r)
#define DE_MID_M14_WrFL(_r)							FN_CHIP_WrFL(DE_MID, M14, _r)
#define DE_MID_M14_Rd(_r)							FN_CHIP_Rd32(DE_MID, M14, _r)
#define DE_MID_M14_Wr(_r, _v)						FN_CHIP_Wr32(DE_MID, M14, _r, _v)
#define DE_MID_M14_Rd01(_r, _f01, _v01)				FN_CHIP_Rd01(DE_MID, M14, _r, _f01, _v01)
#define DE_MID_M14_Wr01(_r, _f01, _v01)				FN_CHIP_Wr01(DE_MID, M14, _r, _f01, _v01)
#define DE_MID_M14_WfCM(_r, _f, _c, _m)				FN_CHIP_WfCM(DE_MID, M14, _r, _f, _c, _m)
#define DE_MID_M14_WfCV(_r, _f, _c, _v1, _v2)		FN_CHIP_WfCV(DE_MID, M14, _r, _f, _c, _v1, _v2)
#define DE_MID_M14_FLRd(_r, _v)						FN_CHIP_FLRd(DE_MID, M14, _r, _v)
#define DE_MID_M14_FLWr(_r, _v)						FN_CHIP_FLWr(DE_MID, M14, _r, _v)
#define DE_MID_M14_FLRf(_r, _f01, _v01)				FN_CHIP_FLRf(DE_MID, M14, _r, _f01, _v01)
#define DE_MID_M14_FLWf(_r, _f01, _v01)				FN_CHIP_FLWf(DE_MID, M14, _r, _f01, _v01)
#define DE_MID_M14_FLCM(_r, _f, _c, _m)				FN_CHIP_FLCM(DE_MID, M14, _r, _f, _c, _m)
#define DE_MID_M14_FLCV(_r, _f, _c, _v1, _v2)		FN_CHIP_FLCV(DE_MID, M14, _r, _f, _c, _v1, _v2)

#define DE_MIF_M14_RdFL(_r)							FN_CHIP_RdFL(DE_MIF, M14, _r)
#define DE_MIF_M14_WrFL(_r)							FN_CHIP_WrFL(DE_MIF, M14, _r)
#define DE_MIF_M14_Rd(_r)							FN_CHIP_Rd32(DE_MIF, M14, _r)
#define DE_MIF_M14_Wr(_r, _v)						FN_CHIP_Wr32(DE_MIF, M14, _r, _v)
#define DE_MIF_M14_Rd01(_r, _f01, _v01)				FN_CHIP_Rd01(DE_MIF, M14, _r, _f01, _v01)
#define DE_MIF_M14_Wr01(_r, _f01, _v01)				FN_CHIP_Wr01(DE_MIF, M14, _r, _f01, _v01)
#define DE_MIF_M14_WfCM(_r, _f, _c, _m)				FN_CHIP_WfCM(DE_MIF, M14, _r, _f, _c, _m)
#define DE_MIF_M14_WfCV(_r, _f, _c, _v1, _v2)		FN_CHIP_WfCV(DE_MIF, M14, _r, _f, _c, _v1, _v2)
#define DE_MIF_M14_FLRd(_r, _v)						FN_CHIP_FLRd(DE_MIF, M14, _r, _v)
#define DE_MIF_M14_FLWr(_r, _v)						FN_CHIP_FLWr(DE_MIF, M14, _r, _v)
#define DE_MIF_M14_FLRf(_r, _f01, _v01)				FN_CHIP_FLRf(DE_MIF, M14, _r, _f01, _v01)
#define DE_MIF_M14_FLWf(_r, _f01, _v01)				FN_CHIP_FLWf(DE_MIF, M14, _r, _f01, _v01)
#define DE_MIF_M14_FLCM(_r, _f, _c, _m)				FN_CHIP_FLCM(DE_MIF, M14, _r, _f, _c, _m)
#define DE_MIF_M14_FLCV(_r, _f, _c, _v1, _v2)		FN_CHIP_FLCV(DE_MIF, M14, _r, _f, _c, _v1, _v2)


/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_MIA_REG_M14A0_T *a0;
		DE_MIA_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MIA_REG_M14A0_T *a0;
		volatile DE_MIA_REG_M14A0_T *b0;
	} phys;
} DE_MIA_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MIB_REG_M14A0_T *a0;
		DE_MIB_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MIB_REG_M14A0_T *a0;
		volatile DE_MIB_REG_M14A0_T *b0;
	} phys;
} DE_MIB_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MIC_REG_M14A0_T *a0;
		DE_MIC_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MIC_REG_M14A0_T *a0;
		volatile DE_MIC_REG_M14A0_T *b0;
	} phys;
} DE_MIC_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MID_REG_M14A0_T *a0;
		DE_MID_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MID_REG_M14A0_T *a0;
		volatile DE_MID_REG_M14A0_T *b0;
	} phys;
} DE_MID_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MIF_REG_M14A0_T *a0;
		DE_MIF_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MIF_REG_M14A0_T *a0;
		volatile DE_MIF_REG_M14A0_T *b0;
	} phys;
} DE_MIF_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MII_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MII_REG_M14B0_T *b0;
	} phys;
} DE_MII_REG_M14B_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MIM_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MIM_REG_M14B0_T *b0;
	} phys;
} DE_MIM_REG_M14B_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MIO_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MIO_REG_M14B0_T *b0;
	} phys;
} DE_MIO_REG_M14B_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_MIOSD_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_MIOSD_REG_M14B0_T *b0;
	} phys;
} DE_MIOSD_REG_M14B_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_MIF_REG_M14_INC  ----- */
/**  @} */
