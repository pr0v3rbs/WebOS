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
 * BE device will teach you how to make device driver with new platform.
 *
 * author     dj911.kim
 * version    1.0
 * date       2013.04.03
 * note       Additional information.
 *
 * @addtogroup BE
 * @{
 */
#ifndef  BE_REG_FRC_M14_INC
#define  BE_REG_FRC_M14_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "be_reg_mac.h"
#include "be_reg_frc_m14a0.h"
#include "be_reg_frc_m14b0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define FRC_LRX_M14_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_LRX, M14, _r)
#define FRC_LRX_M14_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_LRX, M14, _r)
#define FRC_LRX_M14_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_LRX, M14, _r)
#define FRC_LRX_M14_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_LRX, M14, _r, _v)
#define FRC_LRX_M14_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_LRX, M14, _r, _f01, _v01)
#define FRC_LRX_M14_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_LRX, M14, _r, _f01, _v01)
#define FRC_LRX_M14_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_LRX, M14, _r, _v)
#define FRC_LRX_M14_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_LRX, M14, _r, _v)
#define FRC_LRX_M14_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_LRX, M14, _r, _f01, _v01)
#define FRC_LRX_M14_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_LRX, M14, _r, _f01, _v01)

#define FRC_P3D_M14_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_P3D, M14, _r)
#define FRC_P3D_M14_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_P3D, M14, _r)
#define FRC_P3D_M14_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_P3D, M14, _r)
#define FRC_P3D_M14_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_P3D, M14, _r, _v)
#define FRC_P3D_M14_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_P3D, M14, _r, _f01, _v01)
#define FRC_P3D_M14_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_P3D, M14, _r, _f01, _v01)
#define FRC_P3D_M14_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_P3D, M14, _r, _v)
#define FRC_P3D_M14_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_P3D, M14, _r, _v)
#define FRC_P3D_M14_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_P3D, M14, _r, _f01, _v01)
#define FRC_P3D_M14_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_P3D, M14, _r, _f01, _v01)

#define FRC_DVI_M14_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_DVI, M14, _r)
#define FRC_DVI_M14_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_DVI, M14, _r)
#define FRC_DVI_M14_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_DVI, M14, _r)
#define FRC_DVI_M14_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_DVI, M14, _r, _v)
#define FRC_DVI_M14_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_DVI, M14, _r, _f01, _v01)
#define FRC_DVI_M14_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_DVI, M14, _r, _f01, _v01)
#define FRC_DVI_M14_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_DVI, M14, _r, _v)
#define FRC_DVI_M14_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_DVI, M14, _r, _v)
#define FRC_DVI_M14_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_DVI, M14, _r, _f01, _v01)
#define FRC_DVI_M14_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_DVI, M14, _r, _f01, _v01)

#define FRC_DVO_M14_RdFL(_r)                  BE_FN_CHIP_RdFL(FRC_DVO, M14, _r)
#define FRC_DVO_M14_WrFL(_r)                  BE_FN_CHIP_WrFL(FRC_DVO, M14, _r)
#define FRC_DVO_M14_Rd(_r)                    BE_FN_CHIP_Rd32(FRC_DVO, M14, _r)
#define FRC_DVO_M14_Wr(_r, _v)                BE_FN_CHIP_Wr32(FRC_DVO, M14, _r, _v)
#define FRC_DVO_M14_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(FRC_DVO, M14, _r, _f01, _v01)
#define FRC_DVO_M14_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(FRC_DVO, M14, _r, _f01, _v01)
#define FRC_DVO_M14_FLRd(_r, _v)              BE_FN_CHIP_FLRd(FRC_DVO, M14, _r, _v)
#define FRC_DVO_M14_FLWr(_r, _v)              BE_FN_CHIP_FLWr(FRC_DVO, M14, _r, _v)
#define FRC_DVO_M14_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(FRC_DVO, M14, _r, _f01, _v01)
#define FRC_DVO_M14_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(FRC_DVO, M14, _r, _f01, _v01)

#define FRC_DVO_M14A0_RdFL(_r)                  BE_FN_CHIP_VER_RdFL(FRC_DVO, M14, a0, _r)
#define FRC_DVO_M14A0_WrFL(_r)                  BE_FN_CHIP_VER_WrFL(FRC_DVO, M14, a0, _r)
#define FRC_DVO_M14A0_Rd(_r)                    BE_FN_CHIP_VER_Rd32(FRC_DVO, M14, a0, _r)
#define FRC_DVO_M14A0_Wr(_r, _v)                BE_FN_CHIP_VER_Wr32(FRC_DVO, M14, a0, _r, _v)
#define FRC_DVO_M14A0_Rd01(_r, _f01, _v01)      BE_FN_CHIP_VER_Rd01(FRC_DVO, M14, a0, _r, _f01, _v01)
#define FRC_DVO_M14A0_Wr01(_r, _f01, _v01)      BE_FN_CHIP_VER_Wr01(FRC_DVO, M14, a0, _r, _f01, _v01)
#define FRC_DVO_M14A0_FLRd(_r, _v)              BE_FN_CHIP_VER_FLRd(FRC_DVO, M14, a0, _r, _v)
#define FRC_DVO_M14A0_FLWr(_r, _v)              BE_FN_CHIP_VER_FLWr(FRC_DVO, M14, a0, _r, _v)
#define FRC_DVO_M14A0_FLRf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLRf(FRC_DVO, M14, a0, _r, _f01, _v01)
#define FRC_DVO_M14A0_FLWf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLWf(FRC_DVO, M14, a0, _r, _f01, _v01)

#define FRC_DVO_M14B0_RdFL(_r)                  BE_FN_CHIP_VER_RdFL(FRC_DVO, M14, b0, _r)
#define FRC_DVO_M14B0_WrFL(_r)                  BE_FN_CHIP_VER_WrFL(FRC_DVO, M14, b0, _r)
#define FRC_DVO_M14B0_Rd(_r)                    BE_FN_CHIP_VER_Rd32(FRC_DVO, M14, b0, _r)
#define FRC_DVO_M14B0_Wr(_r, _v)                BE_FN_CHIP_VER_Wr32(FRC_DVO, M14, b0, _r, _v)
#define FRC_DVO_M14B0_Rd01(_r, _f01, _v01)      BE_FN_CHIP_VER_Rd01(FRC_DVO, M14, b0, _r, _f01, _v01)
#define FRC_DVO_M14B0_Wr01(_r, _f01, _v01)      BE_FN_CHIP_VER_Wr01(FRC_DVO, M14, b0, _r, _f01, _v01)
#define FRC_DVO_M14B0_FLRd(_r, _v)              BE_FN_CHIP_VER_FLRd(FRC_DVO, M14, b0, _r, _v)
#define FRC_DVO_M14B0_FLWr(_r, _v)              BE_FN_CHIP_VER_FLWr(FRC_DVO, M14, b0, _r, _v)
#define FRC_DVO_M14B0_FLRf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLRf(FRC_DVO, M14, b0, _r, _f01, _v01)
#define FRC_DVO_M14B0_FLWf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLWf(FRC_DVO, M14, b0, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        FRC_LRX_REG_M14A0_T *a0;
        FRC_LRX_REG_M14A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_LRX_REG_M14A0_T *a0;
        volatile FRC_LRX_REG_M14A0_T *b0;
    } phys;
} FRC_LRX_REG_M14_T;

typedef struct {
    union {
        UINT32          *addr;
        FRC_P3D_REG_M14A0_T *a0;
        FRC_P3D_REG_M14A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_P3D_REG_M14A0_T *a0;
        volatile FRC_P3D_REG_M14A0_T *b0;
    } phys;
} FRC_P3D_REG_M14_T;

typedef struct {
    union {
        UINT32          *addr;
        FRC_DVI_REG_M14A0_T *a0;
        FRC_DVI_REG_M14A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_DVI_REG_M14A0_T *a0;
        volatile FRC_DVI_REG_M14A0_T *b0;
    } phys;
} FRC_DVI_REG_M14_T;

typedef struct {
    union {
        UINT32          *addr;
        FRC_DVO_REG_M14A0_T *a0;
        FRC_DVO_REG_M14B0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile FRC_DVO_REG_M14A0_T *a0;
        volatile FRC_DVO_REG_M14B0_T *b0;
    } phys;
} FRC_DVO_REG_M14_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_REG_FRC_M14_INC  ----- */
/**  @} */

