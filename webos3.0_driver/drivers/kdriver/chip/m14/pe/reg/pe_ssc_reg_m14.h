/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

/** @file pe_ssc_reg_m14.h
 *
 *  driver for picture enhance register read, write functions. ( used only within kdriver )
 *
 *	@author		inkyung.park(inkyung.park@lge.com)
 *	@version	0.1
 *	@note
 *	@date		2012.10.12
 *	@see
 */
#ifndef  _PE_SSC_REG_M14_INC_
#define  _PE_SSC_REG_M14_INC_
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "pe_reg_mac.h"
#include "pe_ssc_reg_m14a0.h"
#include "pe_ssc_reg_m14b0.h"

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
// Ax
#define PE_SSC_M14A0_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_SSC_M14A,data,_r)
#define PE_SSC_M14A0_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_SSC_M14A,data,_r)
#define PE_SSC_M14A0_Rd(_r)				PE_REG_SHDW_Rd32(gPE_SSC_M14A,data,_r)
#define PE_SSC_M14A0_Wr(_r,_d1)			PE_REG_SHDW_Wr32(gPE_SSC_M14A,data,_r,_d1)
#define PE_SSC_M14A0_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_SSC_M14A,data,_r,_f1,_d1)
#define PE_SSC_M14A0_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_SSC_M14A,data,_r,_f1,_d1)

// Bx
#define PE_SSC_M14B0_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_SSC_M14B,data,_r)
#define PE_SSC_M14B0_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_SSC_M14B,data,_r)
#define PE_SSC_M14B0_Rd(_r)				PE_REG_SHDW_Rd32(gPE_SSC_M14B,data,_r)
#define PE_SSC_M14B0_Wr(_r,_d1)			PE_REG_SHDW_Wr32(gPE_SSC_M14B,data,_r,_d1)
#define PE_SSC_M14B0_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_SSC_M14B,data,_r,_f1,_d1)
#define PE_SSC_M14B0_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_SSC_M14B,data,_r,_f1,_d1)

/* m14b0 msc */
#define PE_SSC_M14B0_QWr01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_SSC_M14B,data,_r);\
		PE_REG_SHDW_Wr01(gPE_SSC_M14B,data,_r,_f1,_d1);\
		PE_REG_PHYS_WrFL(gPE_SSC_M14B,data,_r);}
#define PE_SSC_M14B0_QRd01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_SSC_M14B,data,_r);\
		PE_REG_SHDW_Rd01(gPE_SSC_M14B,data,_r,_f1,_d1);}

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		PE_SSC_REG_M14A0_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_SSC_REG_M14A0_T *data;
	} phys;
} PE_SSC_REG_M14A_T;

typedef struct {
	union {
		UINT32			*addr;
		PE_SSC_REG_M14B0_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_SSC_REG_M14B0_T *data;
	} phys;
} PE_SSC_REG_M14B_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* ----- #ifndef PE_SSC_REG_M14_INC  ----- */
/**  @} */
