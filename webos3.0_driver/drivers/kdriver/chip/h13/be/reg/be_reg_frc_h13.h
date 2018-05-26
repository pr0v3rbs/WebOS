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
 * main driver implementation for BE device.
 * de device will teach you how to make device driver with new platform.
 *
 * author     justine.jeong
 * version    1.0
 * date       2012.02.28
 * note       Additional information.
 *
 * @addtogroup BE
 * @{
 */
#ifndef  BE_REG_FRC_H13_INC
#define  BE_REG_FRC_H13_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "be_reg_mac.h"
#include "be_reg_frc_h13a0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define FRC_LRX_H13_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_LRX, H13, _r)
#define FRC_LRX_H13_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_LRX, H13, _r)
#define FRC_LRX_H13_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_LRX, H13, _r)
#define FRC_LRX_H13_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_LRX, H13, _r, _v)
#define FRC_LRX_H13_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_LRX, H13, _r, _f01, _v01)
#define FRC_LRX_H13_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_LRX, H13, _r, _f01, _v01)
#define FRC_LRX_H13_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_LRX, H13, _r, _v)
#define FRC_LRX_H13_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_LRX, H13, _r, _v)
#define FRC_LRX_H13_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_LRX, H13, _r, _f01, _v01)
#define FRC_LRX_H13_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_LRX, H13, _r, _f01, _v01)

#define FRC_P3D_H13_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_P3D, H13, _r)
#define FRC_P3D_H13_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_P3D, H13, _r)
#define FRC_P3D_H13_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_P3D, H13, _r)
#define FRC_P3D_H13_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_P3D, H13, _r, _v)
#define FRC_P3D_H13_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_P3D, H13, _r, _f01, _v01)
#define FRC_P3D_H13_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_P3D, H13, _r, _f01, _v01)
#define FRC_P3D_H13_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_P3D, H13, _r, _v)
#define FRC_P3D_H13_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_P3D, H13, _r, _v)
#define FRC_P3D_H13_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_P3D, H13, _r, _f01, _v01)
#define FRC_P3D_H13_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_P3D, H13, _r, _f01, _v01)

#define FRC_DVI_H13_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_DVI, H13, _r)
#define FRC_DVI_H13_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_DVI, H13, _r)
#define FRC_DVI_H13_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_DVI, H13, _r)
#define FRC_DVI_H13_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_DVI, H13, _r, _v)
#define FRC_DVI_H13_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_DVI, H13, _r, _f01, _v01)
#define FRC_DVI_H13_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_DVI, H13, _r, _f01, _v01)
#define FRC_DVI_H13_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_DVI, H13, _r, _v)
#define FRC_DVI_H13_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_DVI, H13, _r, _v)
#define FRC_DVI_H13_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_DVI, H13, _r, _f01, _v01)
#define FRC_DVI_H13_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_DVI, H13, _r, _f01, _v01)

#define FRC_DVO_H13_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_DVO, H13, _r)
#define FRC_DVO_H13_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_DVO, H13, _r)
#define FRC_DVO_H13_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_DVO, H13, _r)
#define FRC_DVO_H13_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_DVO, H13, _r, _v)
#define FRC_DVO_H13_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_DVO, H13, _r, _f01, _v01)
#define FRC_DVO_H13_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_DVO, H13, _r, _f01, _v01)
#define FRC_DVO_H13_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_DVO, H13, _r, _v)
#define FRC_DVO_H13_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_DVO, H13, _r, _v)
#define FRC_DVO_H13_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_DVO, H13, _r, _f01, _v01)
#define FRC_DVO_H13_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_DVO, H13, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        FRC_LRX_REG_H13A0_T *a0;
        FRC_LRX_REG_H13A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_LRX_REG_H13A0_T *a0;
        volatile FRC_LRX_REG_H13A0_T *b0;
    } phys;
} FRC_LRX_REG_H13_T;

typedef struct {
    union {
        UINT32          *addr;
        FRC_P3D_REG_H13A0_T *a0;
        FRC_P3D_REG_H13A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_P3D_REG_H13A0_T *a0;
        volatile FRC_P3D_REG_H13A0_T *b0;
    } phys;
} FRC_P3D_REG_H13_T;

typedef struct {
    union {
        UINT32          *addr;
        FRC_DVI_REG_H13A0_T *a0;
        FRC_DVI_REG_H13A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_DVI_REG_H13A0_T *a0;
        volatile FRC_DVI_REG_H13A0_T *b0;
    } phys;
} FRC_DVI_REG_H13_T;

typedef struct {
    union {
        UINT32          *addr;
        FRC_DVO_REG_H13A0_T *a0;
        FRC_DVO_REG_H13A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_DVO_REG_H13A0_T *a0;
        volatile FRC_DVO_REG_H13A0_T *b0;
    } phys;
} FRC_DVO_REG_H13_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_REG_FRC_H13_INC  ----- */
/**  @} */

