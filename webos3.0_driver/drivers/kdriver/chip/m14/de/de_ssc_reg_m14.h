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
#ifndef  DE_SSC_REG_M14_INC
#define  DE_SSC_REG_M14_INC
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "de_reg_mac.h"
#ifdef USE_KDRV_CODES_FOR_M14A0
#include "de_ssc_reg_m14a0.h"
#endif
#ifdef USE_KDRV_CODES_FOR_M14B0
#include "de_ssc_reg_m14b0.h"
#endif

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define DE_SSC_M14_RdFL(_r)						FN_CHIP_RdFL(DE_SSC, M14, _r)
#define DE_SSC_M14_WrFL(_r)						FN_CHIP_WrFL(DE_SSC, M14, _r)
#define DE_SSC_M14_Rd(_r)						FN_CHIP_Rd32(DE_SSC, M14, _r)
#define DE_SSC_M14_Wr(_r, _v)					FN_CHIP_Wr32(DE_SSC, M14, _r, _v)
#define DE_SSC_M14_Rd01(_r, _f01, _v01)			FN_CHIP_Rd01(DE_SSC, M14, _r, _f01, _v01)
#define DE_SSC_M14_Wr01(_r, _f01, _v01)			FN_CHIP_Wr01(DE_SSC, M14, _r, _f01, _v01)
#define DE_SSC_M14_FLRd(_r, _v)					FN_CHIP_FLRd(DE_SSC, M14, _r, _v)
#define DE_SSC_M14_FLWr(_r, _v)					FN_CHIP_FLWr(DE_SSC, M14, _r, _v)
#define DE_SSC_M14_FLRf(_r, _f01, _v01)			FN_CHIP_FLRf(DE_SSC, M14, _r, _f01, _v01)
#define DE_SSC_M14_FLWf(_r, _f01, _v01)			FN_CHIP_FLWf(DE_SSC, M14, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		DE_SSC_REG_M14A0_T *a0;
		DE_SSC_REG_M14A0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_SSC_REG_M14A0_T *a0;
		volatile DE_SSC_REG_M14A0_T *b0;
	} phys;
} DE_SSC_REG_M14_T;

typedef struct {
	union {
		UINT32			*addr;
		DE_SSC_REG_M14B0_T *b0;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile DE_SSC_REG_M14B0_T *b0;
	} phys;
} DE_SSC_REG_M14B_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef DE_SSC_REG_M14_INC  ----- */
/**  @} */
