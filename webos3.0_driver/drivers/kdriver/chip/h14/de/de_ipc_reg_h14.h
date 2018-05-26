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
 * author     jaemo.kim (jaemo.kim@lge.com)
 * version    1.0
 * date       2011.02.17
 * note       Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef  DE_IPC_REG_H14_INC
#define  DE_IPC_REG_H14_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_H14A0
#include "de_ipc_reg_h14a0.h"
#endif

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_IPC_H14_RdFL(_r)                         FN_CHIP_RdFL(DE_IPC, H14, _r)
#define DE_IPC_H14_WrFL(_r)                         FN_CHIP_WrFL(DE_IPC, H14, _r)
#define DE_IPC_H14_Rd(_r)                           FN_CHIP_Rd32(DE_IPC, H14, _r)
#define DE_IPC_H14_Wr(_r, _v)                       FN_CHIP_Wr32(DE_IPC, H14, _r, _v)
#define DE_IPC_H14_Rd01(_r, _f01, _v01)             FN_CHIP_Rd01(DE_IPC, H14, _r, _f01, _v01)
#define DE_IPC_H14_Wr01(_r, _f01, _v01)             FN_CHIP_Wr01(DE_IPC, H14, _r, _f01, _v01)
#define DE_IPC_H14_WfCM(_r, _f, _c, _m)             FN_CHIP_WfCM(DE_IPC, H14, _r, _f, _c, _m)
#define DE_IPC_H14_WfCV(_r, _f, _c, _v1, _v2)       FN_CHIP_WfCV(DE_IPC, H14, _r, _f, _c, _v1, _v2)
#define DE_IPC_H14_FLRd(_r, _v)                     FN_CHIP_FLRd(DE_IPC, H14, _r, _v)
#define DE_IPC_H14_FLWr(_r, _v)                     FN_CHIP_FLWr(DE_IPC, H14, _r, _v)
#define DE_IPC_H14_FLRf(_r, _f01, _v01)             FN_CHIP_FLRf(DE_IPC, H14, _r, _f01, _v01)
#define DE_IPC_H14_FLWf(_r, _f01, _v01)             FN_CHIP_FLWf(DE_IPC, H14, _r, _f01, _v01)
#define DE_IPC_H14_FLCM(_r, _f, _c, _m)             FN_CHIP_FLCM(DE_IPC, H14, _r, _f, _c, _m)
#define DE_IPC_H14_FLCV(_r, _f, _c, _v1, _v2)       FN_CHIP_FLCV(DE_IPC, H14, _r, _f, _c, _v1, _v2)

#define DE_IPC1_H14_RdFL(_r)                        FN_CHIP_RdFL(DE_IPC1, H14, _r)
#define DE_IPC1_H14_WrFL(_r)                        FN_CHIP_WrFL(DE_IPC1, H14, _r)
#define DE_IPC1_H14_Rd(_r)                          FN_CHIP_Rd32(DE_IPC1, H14, _r)
#define DE_IPC1_H14_Wr(_r, _v)                      FN_CHIP_Wr32(DE_IPC1, H14, _r, _v)
#define DE_IPC1_H14_Rd01(_r, _f01, _v01)            FN_CHIP_Rd01(DE_IPC1, H14, _r, _f01, _v01)
#define DE_IPC1_H14_Wr01(_r, _f01, _v01)            FN_CHIP_Wr01(DE_IPC1, H14, _r, _f01, _v01)
#define DE_IPC1_H14_WfCM(_r, _f, _c, _m)            FN_CHIP_WfCM(DE_IPC1, H14, _r, _f, _c, _m)
#define DE_IPC1_H14_WfCV(_r, _f, _c, _v1, _v2)      FN_CHIP_WfCV(DE_IPC1, H14, _r, _f, _c, _v1, _v2)
#define DE_IPC1_H14_FLRd(_r, _v)                    FN_CHIP_FLRd(DE_IPC1, H14, _r, _v)
#define DE_IPC1_H14_FLWr(_r, _v)                    FN_CHIP_FLWr(DE_IPC1, H14, _r, _v)
#define DE_IPC1_H14_FLRf(_r, _f01, _v01)            FN_CHIP_FLRf(DE_IPC1, H14, _r, _f01, _v01)
#define DE_IPC1_H14_FLWf(_r, _f01, _v01)            FN_CHIP_FLWf(DE_IPC1, H14, _r, _f01, _v01)
#define DE_IPC1_H14_FLCM(_r, _f, _c, _m)            FN_CHIP_FLCM(DE_IPC1, H14, _r, _f, _c, _m)
#define DE_IPC1_H14_FLCV(_r, _f, _c, _v1, _v2)      FN_CHIP_FLCV(DE_IPC1, H14, _r, _f, _c, _v1, _v2)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        DE_IPC_REG_H14A0_T *a0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile DE_IPC_REG_H14A0_T *a0;
    } phys;
} DE_IPC_REG_H14_T;

typedef struct {
    union {
        UINT32          *addr;
        DE_IPC1_REG_H14A0_T *a0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile DE_IPC1_REG_H14A0_T *a0;
    } phys;
} DE_IPC1_REG_H14_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef DE_IPC_REG_H14_INC  ----- */
/**  @} */
