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
 *  OVI h15 reg header file for OVI device
 *
 *  author		dj911.kim@lge.com
 *  version		1.0
 *  date		2014.05.21
 *  note		Additional information.
 *
 *  @addtogroup OVI
 *	@{
 */

#ifndef  OVI_REG_H15_INC
#define  OVI_REG_H15_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ovi_reg_mac.h"
#include "ovi_reg_h15a0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define OVI_OIF_H15_RdFL(_r)                  		OVI_FN_CHIP_RdFL(OVI_OIF, H15, _r)
#define OVI_OIF_H15_WrFL(_r)                  		OVI_FN_CHIP_WrFL(OVI_OIF, H15, _r)
#define OVI_OIF_H15_Rd(_r)                    		OVI_FN_CHIP_Rd32(OVI_OIF, H15, _r)
#define OVI_OIF_H15_Wr(_r, _v)                		OVI_FN_CHIP_Wr32(OVI_OIF, H15, _r, _v)
#define OVI_OIF_H15_Rd01(_r, _f01, _v01)    		OVI_FN_CHIP_Rd01(OVI_OIF, H15, _r, _f01, _v01)
#define OVI_OIF_H15_Wr01(_r, _f01, _v01)    		OVI_FN_CHIP_Wr01(OVI_OIF, H15, _r, _f01, _v01)
#define OVI_OIF_H15_FLRd(_r, _v)              		OVI_FN_CHIP_FLRd(OVI_OIF, H15, _r, _v)
#define OVI_OIF_H15_FLWr(_r, _v)              		OVI_FN_CHIP_FLWr(OVI_OIF, H15, _r, _v)
#define OVI_OIF_H15_FLRf(_r, _f01, _v01)      		OVI_FN_CHIP_FLRf(OVI_OIF, H15, _r, _f01, _v01)
#define OVI_OIF_H15_FLWf(_r, _f01, _v01)    		OVI_FN_CHIP_FLWf(OVI_OIF, H15, _r, _f01, _v01)

#define OVI_LED_H15_RdFL(_r)                  		OVI_FN_CHIP_RdFL(OVI_LED, H15, _r)
#define OVI_LED_H15_WrFL(_r)                  		OVI_FN_CHIP_WrFL(OVI_LED, H15, _r)
#define OVI_LED_H15_Rd(_r)                    		OVI_FN_CHIP_Rd32(OVI_LED, H15, _r)
#define OVI_LED_H15_Wr(_r, _v)                		OVI_FN_CHIP_Wr32(OVI_LED, H15, _r, _v)
#define OVI_LED_H15_Rd01(_r, _f01, _v01)    		OVI_FN_CHIP_Rd01(OVI_LED, H15, _r, _f01, _v01)
#define OVI_LED_H15_Wr01(_r, _f01, _v01)    		OVI_FN_CHIP_Wr01(OVI_LED, H15, _r, _f01, _v01)
#define OVI_LED_H15_FLRd(_r, _v)              		OVI_FN_CHIP_FLRd(OVI_LED, H15, _r, _v)
#define OVI_LED_H15_FLWr(_r, _v)              		OVI_FN_CHIP_FLWr(OVI_LED, H15, _r, _v)
#define OVI_LED_H15_FLRf(_r, _f01, _v01)      		OVI_FN_CHIP_FLRf(OVI_LED, H15, _r, _f01, _v01)
#define OVI_LED_H15_FLWf(_r, _f01, _v01)    		OVI_FN_CHIP_FLWf(OVI_LED, H15, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32          *addr;
		OVI_OIF_REG_H15A0_T *a0;
		OVI_OIF_REG_H15A0_T *b0;
	} shdw;

	union {
		volatile UINT32          *addr;
		volatile OVI_OIF_REG_H15A0_T *a0;
		volatile OVI_OIF_REG_H15A0_T *b0;
	} phys;
} OVI_OIF_REG_H15_T;

typedef struct {
	union {
		UINT32          *addr;
		OVI_LED_REG_H15A0_T *a0;
		OVI_LED_REG_H15A0_T *b0;
	} shdw;

	union {
		volatile UINT32          *addr;
		volatile OVI_LED_REG_H15A0_T *a0;
		volatile OVI_LED_REG_H15A0_T *b0;
	} phys;
} OVI_LED_REG_H15_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef OVI_REG_H15_INC  ----- */
/**  @} */

