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

#ifndef  _DE_CVI_REG_H15_INC_
#define  _DE_CVI_REG_H15_INC_
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_H15A0
#include "de_cvi_reg_h15a0.h"
#endif

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_CVI_H15_RdFL(_r)                         FN_CHIP_RdFL(DE_CVI, H15, _r)
#define DE_CVI_H15_WrFL(_r)                         FN_CHIP_WrFL(DE_CVI, H15, _r)
#define DE_CVI_H15_Rd(_r)                           FN_CHIP_Rd32(DE_CVI, H15, _r)
#define DE_CVI_H15_Wr(_r, _v)                       FN_CHIP_Wr32(DE_CVI, H15, _r, _v)
#define DE_CVI_H15_Rd01(_r, _f01, _v01)             FN_CHIP_Rd01(DE_CVI, H15, _r, _f01, _v01)
#define DE_CVI_H15_Wr01(_r, _f01, _v01)             FN_CHIP_Wr01(DE_CVI, H15, _r, _f01, _v01)
#define DE_CVI_H15_WfCM(_r, _f, _c, _m)             FN_CHIP_WfCM(DE_CVI, H15, _r, _f, _c, _m)
#define DE_CVI_H15_WfCV(_r, _f, _c, _v1, _v2)       FN_CHIP_WfCV(DE_CVI, H15, _r, _f, _c, _v1, _v2)
#define DE_CVI_H15_FLRd(_r, _v)                     FN_CHIP_FLRd(DE_CVI, H15, _r, _v)
#define DE_CVI_H15_FLWr(_r, _v)                     FN_CHIP_FLWr(DE_CVI, H15, _r, _v)
#define DE_CVI_H15_FLRf(_r, _f01, _v01)             FN_CHIP_FLRf(DE_CVI, H15, _r, _f01, _v01)
#define DE_CVI_H15_FLWf(_r, _f01, _v01)             FN_CHIP_FLWf(DE_CVI, H15, _r, _f01, _v01)
#define DE_CVI_H15_FLCM(_r, _f, _c, _m)             FN_CHIP_FLCM(DE_CVI, H15, _r, _f, _c, _m)
#define DE_CVI_H15_FLCV(_r, _f, _c, _v1, _v2)       FN_CHIP_FLCV(DE_CVI, H15, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        DE_CVI_REG_H15A0_T *a0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile DE_CVI_REG_H15A0_T *a0;
    } phys;
} DE_CVI_REG_H15_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef _DE_CVI_REG_H15_INC_  ----- */
/**  @} */
