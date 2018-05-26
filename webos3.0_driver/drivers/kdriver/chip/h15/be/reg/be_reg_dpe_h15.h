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
#ifndef  BE_REG_DPE_H15_INC
#define  BE_REG_DPE_H15_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "be_reg_mac.h"
#include "be_reg_dpe_h15a0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DPE_LED_H15_RdFL(_r)                  BE_FN_CHIP_RdFL(DPE_LED, H15, _r)
#define DPE_LED_H15_WrFL(_r)                  BE_FN_CHIP_WrFL(DPE_LED, H15, _r)
#define DPE_LED_H15_Rd(_r)                    BE_FN_CHIP_Rd32(DPE_LED, H15, _r)
#define DPE_LED_H15_Wr(_r, _v)                BE_FN_CHIP_Wr32(DPE_LED, H15, _r, _v)
#define DPE_LED_H15_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(DPE_LED, H15, _r, _f01, _v01)
#define DPE_LED_H15_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(DPE_LED, H15, _r, _f01, _v01)
#define DPE_LED_H15_FLRd(_r, _v)              BE_FN_CHIP_FLRd(DPE_LED, H15, _r, _v)
#define DPE_LED_H15_FLWr(_r, _v)              BE_FN_CHIP_FLWr(DPE_LED, H15, _r, _v)
#define DPE_LED_H15_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(DPE_LED, H15, _r, _f01, _v01)
#define DPE_LED_H15_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(DPE_LED, H15, _r, _f01, _v01)

#define DPE_LED_H15A0_RdFL(_r)                  BE_FN_CHIP_VER_RdFL(DPE_LED, H15, a0, _r)
#define DPE_LED_H15A0_WrFL(_r)                  BE_FN_CHIP_VER_WrFL(DPE_LED, H15, a0, _r)
#define DPE_LED_H15A0_Rd(_r)                    BE_FN_CHIP_VER_Rd32(DPE_LED, H15, a0, _r)
#define DPE_LED_H15A0_Wr(_r, _v)                BE_FN_CHIP_VER_Wr32(DPE_LED, H15, a0, _r, _v)
#define DPE_LED_H15A0_Rd01(_r, _f01, _v01)      BE_FN_CHIP_VER_Rd01(DPE_LED, H15, a0, _r, _f01, _v01)
#define DPE_LED_H15A0_Wr01(_r, _f01, _v01)      BE_FN_CHIP_VER_Wr01(DPE_LED, H15, a0, _r, _f01, _v01)
#define DPE_LED_H15A0_FLRd(_r, _v)              BE_FN_CHIP_VER_FLRd(DPE_LED, H15, a0, _r, _v)
#define DPE_LED_H15A0_FLWr(_r, _v)              BE_FN_CHIP_VER_FLWr(DPE_LED, H15, a0, _r, _v)
#define DPE_LED_H15A0_FLRf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLRf(DPE_LED, H15, a0, _r, _f01, _v01)
#define DPE_LED_H15A0_FLWf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLWf(DPE_LED, H15, a0, _r, _f01, _v01)

#define DPE_LED_H15B0_RdFL(_r)                  BE_FN_CHIP_VER_RdFL(DPE_LED, H15, b0, _r)
#define DPE_LED_H15B0_WrFL(_r)                  BE_FN_CHIP_VER_WrFL(DPE_LED, H15, b0, _r)
#define DPE_LED_H15B0_Rd(_r)                    BE_FN_CHIP_VER_Rd32(DPE_LED, H15, b0, _r)
#define DPE_LED_H15B0_Wr(_r, _v)                BE_FN_CHIP_VER_Wr32(DPE_LED, H15, b0, _r, _v)
#define DPE_LED_H15B0_Rd01(_r, _f01, _v01)      BE_FN_CHIP_VER_Rd01(DPE_LED, H15, b0, _r, _f01, _v01)
#define DPE_LED_H15B0_Wr01(_r, _f01, _v01)      BE_FN_CHIP_VER_Wr01(DPE_LED, H15, b0, _r, _f01, _v01)
#define DPE_LED_H15B0_FLRd(_r, _v)              BE_FN_CHIP_VER_FLRd(DPE_LED, H15, b0, _r, _v)
#define DPE_LED_H15B0_FLWr(_r, _v)              BE_FN_CHIP_VER_FLWr(DPE_LED, H15, b0, _r, _v)
#define DPE_LED_H15B0_FLRf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLRf(DPE_LED, H15, b0, _r, _f01, _v01)
#define DPE_LED_H15B0_FLWf(_r, _f01, _v01)      BE_FN_CHIP_VER_FLWf(DPE_LED, H15, b0, _r, _f01, _v01)

#define PE1_DRC_H15_RdFL(_r)                  BE_FN_CHIP_RdFL(PE1_DRC, H15, _r)
#define PE1_DRC_H15_WrFL(_r)                  BE_FN_CHIP_WrFL(PE1_DRC, H15, _r)
#define PE1_DRC_H15_Rd(_r)                    BE_FN_CHIP_Rd32(PE1_DRC, H15, _r)
#define PE1_DRC_H15_Wr(_r, _v)                BE_FN_CHIP_Wr32(PE1_DRC, H15, _r, _v)
#define PE1_DRC_H15_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(PE1_DRC, H15, _r, _f01, _v01)
#define PE1_DRC_H15_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(PE1_DRC, H15, _r, _f01, _v01)
#define PE1_DRC_H15_FLRd(_r, _v)              BE_FN_CHIP_FLRd(PE1_DRC, H15, _r, _v)
#define PE1_DRC_H15_FLWr(_r, _v)              BE_FN_CHIP_FLWr(PE1_DRC, H15, _r, _v)
#define PE1_DRC_H15_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(PE1_DRC, H15, _r, _f01, _v01)
#define PE1_DRC_H15_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(PE1_DRC, H15, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        DPE_LED_REG_H15A0_T *a0;
        DPE_LED_REG_H15A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile DPE_LED_REG_H15A0_T *a0;
        volatile DPE_LED_REG_H15A0_T *b0;
    } phys;
} DPE_LED_REG_H15_T;

typedef struct {
    union {
        UINT32          *addr;
		PE1_DRC_REG_H15A0_T *a0;
        PE1_DRC_REG_H15A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
		volatile PE1_DRC_REG_H15A0_T *a0;
        volatile PE1_DRC_REG_H15A0_T *b0;
    } phys;
} PE1_DRC_REG_H15_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_REG_DPE_H15_INC  ----- */
/**  @} */
