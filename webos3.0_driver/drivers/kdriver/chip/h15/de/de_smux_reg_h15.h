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
 * date		  2011.02.17
 * note		  Additional information.
 *
 * @addtogroup lg1152_de
 * @{
 */
#ifndef  DE_SMUX_REG_H15_INC
#define  DE_SMUX_REG_H15_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_H15A0
#include "de_smux_reg_h15a0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_SMUX_H15_RdFL(_r)						FN_CHIP_RdFL(DE_SMM, H15, _r)
#define DE_SMUX_H15_WrFL(_r)						FN_CHIP_WrFL(DE_SMM, H15, _r)
#define DE_SMUX_H15_Rd(_r)						FN_CHIP_Rd32(DE_SMM, H15, _r)
#define DE_SMUX_H15_Wr(_r, _v)					FN_CHIP_Wr32(DE_SMM, H15, _r, _v)
#define DE_SMUX_H15_Rd01(_r, _f01, _v01)			FN_CHIP_Rd01(DE_SMM, H15, _r, _f01, _v01)
#define DE_SMUX_H15_Wr01(_r, _f01, _v01)			FN_CHIP_Wr01(DE_SMM, H15, _r, _f01, _v01)
#define DE_SMUX_H15_FLRd(_r, _v)					FN_CHIP_FLRd(DE_SMM, H15, _r, _v)
#define DE_SMUX_H15_FLWr(_r, _v)					FN_CHIP_FLWr(DE_SMM, H15, _r, _v)
#define DE_SMUX_H15_FLRf(_r, _f01, _v01)			FN_CHIP_FLRf(DE_SMM, H15, _r, _f01, _v01)
#define DE_SMUX_H15_FLWf(_r, _f01, _v01)			FN_CHIP_FLWf(DE_SMM, H15, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_SMM_REG_H15A0_T *a0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_SMM_REG_H15A0_T *a0;
	} phys;
} DE_SMM_REG_H15_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_SMUX_REG_H15_INC  ----- */
/**  @} */
