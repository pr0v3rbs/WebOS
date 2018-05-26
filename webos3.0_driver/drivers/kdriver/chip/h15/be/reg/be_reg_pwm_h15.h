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
 * date       2014.05.21
 * note       Additional information.
 *
 * @addtogroup BE
 * @{
 */
#ifndef  BE_REG_FRC_H15_INC
#define  BE_REG_FRC_H15_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "be_reg_mac.h"
#include "be_reg_pwm_h15a0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define PE_PWM_H15_RdFL(_r)                  BE_FN_CHIP_RdFL(PE_PWM, H15, _r)
#define PE_PWM_H15_WrFL(_r)                  BE_FN_CHIP_WrFL(PE_PWM, H15, _r)
#define PE_PWM_H15_Rd(_r)                    BE_FN_CHIP_Rd32(PE_PWM, H15, _r)
#define PE_PWM_H15_Wr(_r, _v)                BE_FN_CHIP_Wr32(PE_PWM, H15, _r, _v)
#define PE_PWM_H15_Rd01(_r, _f01, _v01)      BE_FN_CHIP_Rd01(PE_PWM, H15, _r, _f01, _v01)
#define PE_PWM_H15_Wr01(_r, _f01, _v01)      BE_FN_CHIP_Wr01(PE_PWM, H15, _r, _f01, _v01)
#define PE_PWM_H15_FLRd(_r, _v)              BE_FN_CHIP_FLRd(PE_PWM, H15, _r, _v)
#define PE_PWM_H15_FLWr(_r, _v)              BE_FN_CHIP_FLWr(PE_PWM, H15, _r, _v)
#define PE_PWM_H15_FLRf(_r, _f01, _v01)      BE_FN_CHIP_FLRf(PE_PWM, H15, _r, _f01, _v01)
#define PE_PWM_H15_FLWf(_r, _f01, _v01)      BE_FN_CHIP_FLWf(PE_PWM, H15, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        PE_PWM_REG_H15A0_T *a0;
        PE_PWM_REG_H15A0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile PE_PWM_REG_H15A0_T *a0;
        volatile PE_PWM_REG_H15A0_T *b0;
    } phys;
} PE_PWM_REG_H15_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_REG_FRC_H15_INC  ----- */
/**  @} */

