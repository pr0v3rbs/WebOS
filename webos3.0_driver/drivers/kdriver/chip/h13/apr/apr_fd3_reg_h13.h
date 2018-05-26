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
 * main driver implementation for APR device.
 * apr device will teach you how to make device driver with new platform.
 *
 * author     justine.jeong
 * version    1.0
 * date       2012.09.05
 * note       Additional information.
 *
 * @addtogroup APR
 * @{
 */
#ifndef  APR_FD3_REG_H13_INC
#define  APR_FD3_REG_H13_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "apr_reg_mac.h"
#include "apr_fd3_reg_h13b0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define APR_FD3_H13_RdFL(_r)                  APR_FN_CHIP_RdFL(APR_FD3, H13, _r)
#define APR_FD3_H13_WrFL(_r)                  APR_FN_CHIP_WrFL(APR_FD3, H13, _r)
#define APR_FD3_H13_Rd(_r)                    APR_FN_CHIP_Rd32(APR_FD3, H13, _r)
#define APR_FD3_H13_Wr(_r, _v)                APR_FN_CHIP_Wr32(APR_FD3, H13, _r, _v)
#define APR_FD3_H13_Rd01(_r, _f01, _v01)      APR_FN_CHIP_Rd01(APR_FD3, H13, _r, _f01, _v01)
#define APR_FD3_H13_Wr01(_r, _f01, _v01)      APR_FN_CHIP_Wr01(APR_FD3, H13, _r, _f01, _v01)
#define APR_FD3_H13_FLRd(_r, _v)              APR_FN_CHIP_FLRd(APR_FD3, H13, _r, _v)
#define APR_FD3_H13_FLWr(_r, _v)              APR_FN_CHIP_FLWr(APR_FD3, H13, _r, _v)
#define APR_FD3_H13_FLRf(_r, _f01, _v01)      APR_FN_CHIP_FLRf(APR_FD3, H13, _r, _f01, _v01)
#define APR_FD3_H13_FLWf(_r, _f01, _v01)      APR_FN_CHIP_FLWf(APR_FD3, H13, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
    union {
        UINT32          *addr;
        APR_FD3_REG_H13B0_T *a0;
        APR_FD3_REG_H13B0_T *b0;
    } shdw;

    union {
        volatile UINT32          *addr;
        volatile APR_FD3_REG_H13B0_T *a0;
        volatile APR_FD3_REG_H13B0_T *b0;
    } phys;
} APR_FD3_REG_H13_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef APR_FD3_REG_H13_INC  ----- */
/**  @} */
