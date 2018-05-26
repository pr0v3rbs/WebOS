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
 * date		  2011.06.27
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef  CTOP_CTRL_REG_H14_INC
#define  CTOP_CTRL_REG_H14_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
//#ifdef USE_KDRV_CODES_FOR_H14A0
#include "ctop_ctrl_reg_h14a0.h"
//#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define CTOP_CTRL_H14_RdFL(_r)							FN_CHIP_RdFL(DE_CTOP, H14, _r)
#define CTOP_CTRL_H14_WrFL(_r)							FN_CHIP_WrFL(DE_CTOP, H14, _r)
#define CTOP_CTRL_H14_Rd(_r)							FN_CHIP_Rd32(DE_CTOP, H14, _r)
#define CTOP_CTRL_H14_Wr(_r, _v)						FN_CHIP_Wr32(DE_CTOP, H14, _r, _v)
#define CTOP_CTRL_H14_Rd01(_r, _f01, _v01)				FN_CHIP_Rd01(DE_CTOP, H14, _r, _f01, _v01)
#define CTOP_CTRL_H14_Wr01(_r, _f01, _v01)				FN_CHIP_Wr01(DE_CTOP, H14, _r, _f01, _v01)
#define CTOP_CTRL_H14_WfCM(_r, _f, _c, _m)				FN_CHIP_WfCM(DE_CTOP, H14, _r, _f, _c, _m)
#define CTOP_CTRL_H14_WfCV(_r, _f, _c, _v1, _v2)			FN_CHIP_WfCV(DE_CTOP, H14, _r, _f, _c, _v1, _v2)
#define CTOP_CTRL_H14_FLRd(_r, _v)						FN_CHIP_FLRd(DE_CTOP, H14, _r, _v)
#define CTOP_CTRL_H14_FLWr(_r, _v)						FN_CHIP_FLWr(DE_CTOP, H14, _r, _v)
#define CTOP_CTRL_H14_FLRf(_r, _f01, _v01)				FN_CHIP_FLRf(DE_CTOP, H14, _r, _f01, _v01)
#define CTOP_CTRL_H14_FLWf(_r, _f01, _v01)				FN_CHIP_FLWf(DE_CTOP, H14, _r, _f01, _v01)
#define CTOP_CTRL_H14_FLCM(_r, _f, _c, _m)				FN_CHIP_FLCM(DE_CTOP, H14, _r, _f, _c, _m)
#define CTOP_CTRL_H14_FLCV(_r, _f, _c, _v1, _v2)			FN_CHIP_FLCV(DE_CTOP, H14, _r, _f, _c, _v1, _v2)

#define CTOP_CTRL_H14A_RdFL(_r)						REG_PHYS_VER_RdFL(g##DE_CTOP##_##H14, a0, _r)
#define CTOP_CTRL_H14A_WrFL(_r)						REG_PHYS_VER_WrFL(g##DE_CTOP##_##H14, a0, _r)
#define CTOP_CTRL_H14A_Rd(_r)							REG_SHDW_VER_Rd32(g##DE_CTOP##_##H14, a0, _r)
#define CTOP_CTRL_H14A_Wr(_r, _v)						REG_SHDW_VER_Wr32(g##DE_CTOP##_##H14, a0, _r, _v)
#define CTOP_CTRL_H14A_Rd01(_r, _f01, _v01)				REG_SHDW_VER_Rd01(g##DE_CTOP##_##H14, a0, _r, _f01, _v01)
#define CTOP_CTRL_H14A_Wr01(_r, _f01, _v01)				REG_SHDW_VER_Wr01(g##DE_CTOP##_##H14, a0, _r, _f01, _v01)
#define CTOP_CTRL_H14A_FLRf(_r, _f01, _v01)				REG_PHYS_VER_FLRf(g##DE_CTOP##_##H14, a0, _r, _f01, _v01)
#define CTOP_CTRL_H14A_FLWf(_r, _f01, _v01)				REG_PHYS_VER_FLWf(g##DE_CTOP##_##H14, a0, _r, _f01, _v01)
#define CTOP_CTRL_H14A_WfCM(_r, _f, _c, _m)				REG_SHDW_VER_WfCM(g##DE_CTOP##_##H14, a0, _r, _f, _c, _m)
#define CTOP_CTRL_H14A_FLCM(_r, _f, _c, _m)				REG_PHYS_VER_FLCM(g##DE_CTOP##_##H14, a0, _r, _f, _c, _m)
#define CTOP_CTRL_H14A_FLRd(_r, _v)						REG_PHYS_VER_FLRd(g##DE_CTOP##_##H14, a0, _r, _v)
#define CTOP_CTRL_H14A_FLWr(_r, _v)						REG_PHYS_VER_FLWr(g##DE_CTOP##_##H14, a0, _r, _v)
#define CTOP_CTRL_H14A_FLCV(_r, _f, _c, _v1, _v2)			REG_PHYS_VER_FLCV(g##DE_CTOP##_##H14, a0, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		CTOP_CTRL_REG_H14A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile CTOP_CTRL_REG_H14A0_T *a0;
	} phys;
} CTOP_CTRL_REG_H14_T;


/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef CTOP_CTRL_REG_H14_INC  ----- */
/**  @} */
