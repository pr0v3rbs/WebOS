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
 *  OVI h13 reg header file for OVI device
 *	
 *  author		dj911.kim@lge.com
 *  version		1.0
 *  date		2012.09.09
 *  note		Additional information.
 *
 *  @addtogroup OVI
 *	@{
 */
 
#ifndef  OVI_REG_H13_INC
#define  OVI_REG_H13_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ovi_reg_mac.h"
#include "ovi_reg_h13b0.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/
#define OVI_TCON_H13_RdFL(_r)                  	OVI_FN_CHIP_RdFL(OVI_TCON, H13, _r)
#define OVI_TCON_H13_WrFL(_r)                  	OVI_FN_CHIP_WrFL(OVI_TCON, H13, _r)
#define OVI_TCON_H13_Rd(_r)                    		OVI_FN_CHIP_Rd32(OVI_TCON, H13, _r)
#define OVI_TCON_H13_Wr(_r, _v)                	OVI_FN_CHIP_Wr32(OVI_TCON, H13, _r, _v)
#define OVI_TCON_H13_Rd01(_r, _f01, _v01)    	OVI_FN_CHIP_Rd01(OVI_TCON, H13, _r, _f01, _v01)
#define OVI_TCON_H13_Wr01(_r, _f01, _v01)    	OVI_FN_CHIP_Wr01(OVI_TCON, H13, _r, _f01, _v01)
#define OVI_TCON_H13_FLRd(_r, _v)              	OVI_FN_CHIP_FLRd(OVI_TCON, H13, _r, _v)
#define OVI_TCON_H13_FLWr(_r, _v)              	OVI_FN_CHIP_FLWr(OVI_TCON, H13, _r, _v)
#define OVI_TCON_H13_FLRf(_r, _f01, _v01)      	OVI_FN_CHIP_FLRf(OVI_TCON, H13, _r, _f01, _v01)
#define OVI_TCON_H13_FLWf(_r, _f01, _v01)    	OVI_FN_CHIP_FLWf(OVI_TCON, H13, _r, _f01, _v01)

#define OVI_OIF_H13_RdFL(_r)                  		OVI_FN_CHIP_RdFL(OVI_OIF, H13, _r)
#define OVI_OIF_H13_WrFL(_r)                  		OVI_FN_CHIP_WrFL(OVI_OIF, H13, _r)
#define OVI_OIF_H13_Rd(_r)                    		OVI_FN_CHIP_Rd32(OVI_OIF, H13, _r)
#define OVI_OIF_H13_Wr(_r, _v)                		OVI_FN_CHIP_Wr32(OVI_OIF, H13, _r, _v)
#define OVI_OIF_H13_Rd01(_r, _f01, _v01)    	OVI_FN_CHIP_Rd01(OVI_OIF, H13, _r, _f01, _v01)
#define OVI_OIF_H13_Wr01(_r, _f01, _v01)    	OVI_FN_CHIP_Wr01(OVI_OIF, H13, _r, _f01, _v01)
#define OVI_OIF_H13_FLRd(_r, _v)              		OVI_FN_CHIP_FLRd(OVI_OIF, H13, _r, _v)
#define OVI_OIF_H13_FLWr(_r, _v)              	OVI_FN_CHIP_FLWr(OVI_OIF, H13, _r, _v)
#define OVI_OIF_H13_FLRf(_r, _f01, _v01)      	OVI_FN_CHIP_FLRf(OVI_OIF, H13, _r, _f01, _v01)
#define OVI_OIF_H13_FLWf(_r, _f01, _v01)    	OVI_FN_CHIP_FLWf(OVI_OIF, H13, _r, _f01, _v01)

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32          *addr;
		OVI_TCON_REG_H13B0_T *a0;
		OVI_TCON_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32          *addr;
		volatile OVI_TCON_REG_H13B0_T *a0;
		volatile OVI_TCON_REG_H13B0_T *b0;
	} phys;
} OVI_TCON_REG_H13_T;

typedef struct {
	union {
		UINT32          *addr;
		OVI_OIF_REG_H13B0_T *a0;
		OVI_OIF_REG_H13B0_T *b0;
	} shdw;

	union {
		volatile UINT32          *addr;
		volatile OVI_OIF_REG_H13B0_T *a0;
		volatile OVI_OIF_REG_H13B0_T *b0;
	} phys;
} OVI_OIF_REG_H13_T;

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef OVI_REG_H13_INC  ----- */
/**  @} */

