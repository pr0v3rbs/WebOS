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
#ifndef  BE_REG_DPPB_M14_INC
#define  BE_REG_DPPB_M14_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "be_reg_mac.h"
#include "be_reg_dppb_m14a0.h"
#include "be_reg_dppb_m14b0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DPPB_TOP_M14_RdFL(_r)                  BE_FN_CHIP_RdFL(DPPB_TOP, M14, _r)
#define DPPB_TOP_M14_WrFL(_r)                  BE_FN_CHIP_WrFL(DPPB_TOP, M14, _r)
#define DPPB_TOP_M14_Rd(_r)                    BE_FN_CHIP_Rd32(DPPB_TOP, M14, _r)
#define DPPB_TOP_M14_Wr(_r, _v)                BE_FN_CHIP_Wr32(DPPB_TOP, M14, _r, _v)
#define DPPB_TOP_M14_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(DPPB_TOP, M14, _r, _f01, _v01)
#define DPPB_TOP_M14_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(DPPB_TOP, M14, _r, _f01, _v01)
#define DPPB_TOP_M14_FLRd(_r, _v)              BE_FN_CHIP_FLRd(DPPB_TOP, M14, _r, _v)
#define DPPB_TOP_M14_FLWr(_r, _v)              BE_FN_CHIP_FLWr(DPPB_TOP, M14, _r, _v)
#define DPPB_TOP_M14_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(DPPB_TOP, M14, _r, _f01, _v01)
#define DPPB_TOP_M14_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(DPPB_TOP, M14, _r, _f01, _v01)

#define DPPB_TXM_M14_RdFL(_r)                  BE_FN_CHIP_RdFL(DPPB_TXM, M14, _r)
#define DPPB_TXM_M14_WrFL(_r)                  BE_FN_CHIP_WrFL(DPPB_TXM, M14, _r)
#define DPPB_TXM_M14_Rd(_r)                    BE_FN_CHIP_Rd32(DPPB_TXM, M14, _r)
#define DPPB_TXM_M14_Wr(_r, _v)                BE_FN_CHIP_Wr32(DPPB_TXM, M14, _r, _v)
#define DPPB_TXM_M14_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(DPPB_TXM, M14, _r, _f01, _v01)
#define DPPB_TXM_M14_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(DPPB_TXM, M14, _r, _f01, _v01)
#define DPPB_TXM_M14_FLRd(_r, _v)              BE_FN_CHIP_FLRd(DPPB_TXM, M14, _r, _v)
#define DPPB_TXM_M14_FLWr(_r, _v)              BE_FN_CHIP_FLWr(DPPB_TXM, M14, _r, _v)
#define DPPB_TXM_M14_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(DPPB_TXM, M14, _r, _f01, _v01)
#define DPPB_TXM_M14_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(DPPB_TXM, M14, _r, _f01, _v01)

#define DPPB_LED_M14_RdFL(_r)                  BE_FN_CHIP_RdFL(DPPB_LED, M14, _r)
#define DPPB_LED_M14_WrFL(_r)                  BE_FN_CHIP_WrFL(DPPB_LED, M14, _r)
#define DPPB_LED_M14_Rd(_r)                    BE_FN_CHIP_Rd32(DPPB_LED, M14, _r)
#define DPPB_LED_M14_Wr(_r, _v)                BE_FN_CHIP_Wr32(DPPB_LED, M14, _r, _v)
#define DPPB_LED_M14_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(DPPB_LED, M14, _r, _f01, _v01)
#define DPPB_LED_M14_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(DPPB_LED, M14, _r, _f01, _v01)
#define DPPB_LED_M14_FLRd(_r, _v)              BE_FN_CHIP_FLRd(DPPB_LED, M14, _r, _v)
#define DPPB_LED_M14_FLWr(_r, _v)              BE_FN_CHIP_FLWr(DPPB_LED, M14, _r, _v)
#define DPPB_LED_M14_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(DPPB_LED, M14, _r, _f01, _v01)
#define DPPB_LED_M14_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(DPPB_LED, M14, _r, _f01, _v01)

#define DPPB_LED_M14A0_RdFL(_r)                  BE_FN_CHIP_VER_RdFL(DPPB_LED, M14, a0, _r)
#define DPPB_LED_M14A0_WrFL(_r)                  BE_FN_CHIP_VER_WrFL(DPPB_LED, M14, a0, _r)
#define DPPB_LED_M14A0_Rd(_r)                    BE_FN_CHIP_VER_Rd32(DPPB_LED, M14, a0, _r)
#define DPPB_LED_M14A0_Wr(_r, _v)                BE_FN_CHIP_VER_Wr32(DPPB_LED, M14, a0, _r, _v)
#define DPPB_LED_M14A0_Rd01(_r, _f01, _v01)      BE_FN_CHIP_VER_Rd01(DPPB_LED, M14, a0, _r, _f01, _v01)
#define DPPB_LED_M14A0_Wr01(_r, _f01, _v01)      BE_FN_CHIP_VER_Wr01(DPPB_LED, M14, a0, _r, _f01, _v01)
#define DPPB_LED_M14A0_FLRd(_r, _v)              BE_FN_CHIP_VER_FLRd(DPPB_LED, M14, a0, _r, _v)
#define DPPB_LED_M14A0_FLWr(_r, _v)              BE_FN_CHIP_VER_FLWr(DPPB_LED, M14, a0, _r, _v)
#define DPPB_LED_M14A0_FLRf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLRf(DPPB_LED, M14, a0, _r, _f01, _v01)
#define DPPB_LED_M14A0_FLWf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLWf(DPPB_LED, M14, a0, _r, _f01, _v01)

#define DPPB_LED_M14B0_RdFL(_r)                  BE_FN_CHIP_VER_RdFL(DPPB_LED, M14, b0, _r)
#define DPPB_LED_M14B0_WrFL(_r)                  BE_FN_CHIP_VER_WrFL(DPPB_LED, M14, b0, _r)
#define DPPB_LED_M14B0_Rd(_r)                    BE_FN_CHIP_VER_Rd32(DPPB_LED, M14, b0, _r)
#define DPPB_LED_M14B0_Wr(_r, _v)                BE_FN_CHIP_VER_Wr32(DPPB_LED, M14, b0, _r, _v)
#define DPPB_LED_M14B0_Rd01(_r, _f01, _v01)      BE_FN_CHIP_VER_Rd01(DPPB_LED, M14, b0, _r, _f01, _v01)
#define DPPB_LED_M14B0_Wr01(_r, _f01, _v01)      BE_FN_CHIP_VER_Wr01(DPPB_LED, M14, b0, _r, _f01, _v01)
#define DPPB_LED_M14B0_FLRd(_r, _v)              BE_FN_CHIP_VER_FLRd(DPPB_LED, M14, b0, _r, _v)
#define DPPB_LED_M14B0_FLWr(_r, _v)              BE_FN_CHIP_VER_FLWr(DPPB_LED, M14, b0, _r, _v)
#define DPPB_LED_M14B0_FLRf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLRf(DPPB_LED, M14, b0, _r, _f01, _v01)
#define DPPB_LED_M14B0_FLWf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLWf(DPPB_LED, M14, b0, _r, _f01, _v01)

#define PE1_HDR_M14B0_RdFL(_r)                  BE_FN_CHIP_VER_RdFL(PE1_HDR, M14B, b0, _r)
#define PE1_HDR_M14B0_WrFL(_r)                  BE_FN_CHIP_VER_WrFL(PE1_HDR, M14B, b0, _r)
#define PE1_HDR_M14B0_Rd(_r)                    BE_FN_CHIP_VER_Rd32(PE1_HDR, M14B, b0, _r)
#define PE1_HDR_M14B0_Wr(_r, _v)                BE_FN_CHIP_VER_Wr32(PE1_HDR, M14B, b0, _r, _v)
#define PE1_HDR_M14B0_Rd01(_r, _f01, _v01)      BE_FN_CHIP_VER_Rd01(PE1_HDR, M14B, b0, _r, _f01, _v01)
#define PE1_HDR_M14B0_Wr01(_r, _f01, _v01)      BE_FN_CHIP_VER_Wr01(PE1_HDR, M14B, b0, _r, _f01, _v01)
#define PE1_HDR_M14B0_FLRd(_r, _v)              BE_FN_CHIP_VER_FLRd(PE1_HDR, M14B, b0, _r, _v)
#define PE1_HDR_M14B0_FLWr(_r, _v)              BE_FN_CHIP_VER_FLWr(PE1_HDR, M14B, b0, _r, _v)
#define PE1_HDR_M14B0_FLRf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLRf(PE1_HDR, M14B, b0, _r, _f01, _v01)
#define PE1_HDR_M14B0_FLWf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLWf(PE1_HDR, M14B, b0, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        DPPB_TOP_REG_M14A0_T *a0;
        DPPB_TOP_REG_M14A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile DPPB_TOP_REG_M14A0_T *a0;
        volatile DPPB_TOP_REG_M14A0_T *b0;
    } phys;
} DPPB_TOP_REG_M14_T;

typedef struct {
    union {
        UINT32          *addr;
        DPPB_TXM_REG_M14A0_T *a0;
        DPPB_TXM_REG_M14A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile DPPB_TXM_REG_M14A0_T *a0;
        volatile DPPB_TXM_REG_M14A0_T *b0;
    } phys;
} DPPB_TXM_REG_M14_T;

typedef struct {
    union {
        UINT32          *addr;
        DPPB_LED_REG_M14A0_T *a0;
        DPPB_LED_REG_M14B0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile DPPB_LED_REG_M14A0_T *a0;
        volatile DPPB_LED_REG_M14B0_T *b0;
    } phys;
} DPPB_LED_REG_M14_T;

typedef struct {
    union {
        UINT32          *addr;
        PE1_HDR_REG_M14B0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile PE1_HDR_REG_M14B0_T *b0;
    } phys;
} PE1_HDR_REG_M14_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_REG_DPPB_M14_INC  ----- */
/**  @} */
