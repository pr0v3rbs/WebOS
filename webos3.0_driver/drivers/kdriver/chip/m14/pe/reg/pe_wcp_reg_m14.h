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

/** @file pe_wcp_reg_m14.h
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
#ifndef _PE_WCP_REG_M14_H_
#define _PE_WCP_REG_M14_H_
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "pe_reg_mac.h"
#include "pe_wcp_reg_m14a0.h"
//#include "pe_wcp_reg_m14b0.h"

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/
// Ax
#define PE_OVL_M14A0_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_OVL_M14A,data,_r)
#define PE_OVL_M14A0_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_OVL_M14A,data,_r)
#define PE_OVL_M14A0_Rd(_r)				PE_REG_SHDW_Rd32(gPE_OVL_M14A,data,_r)
#define PE_OVL_M14A0_Wr(_r,_d1)			PE_REG_SHDW_Wr32(gPE_OVL_M14A,data,_r,_d1)
#define PE_OVL_M14A0_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_OVL_M14A,data,_r,_f1,_d1)
#define PE_OVL_M14A0_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_OVL_M14A,data,_r,_f1,_d1)

#define PE_OVR_M14A0_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_OVR_M14A,data,_r)
#define PE_OVR_M14A0_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_OVR_M14A,data,_r)
#define PE_OVR_M14A0_Rd(_r)				PE_REG_SHDW_Rd32(gPE_OVR_M14A,data,_r)
#define PE_OVR_M14A0_Wr(_r,_d1)			PE_REG_SHDW_Wr32(gPE_OVR_M14A,data,_r,_d1)
#define PE_OVR_M14A0_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_OVR_M14A,data,_r,_f1,_d1)
#define PE_OVR_M14A0_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_OVR_M14A,data,_r,_f1,_d1)

// Bx
#ifdef PE_HW_M14_USE_M14B_WCP_REG
#define PE_OVL_M14B0_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_OVL_M14B,data,_r)
#define PE_OVL_M14B0_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_OVL_M14B,data,_r)
#define PE_OVL_M14B0_Rd(_r)				PE_REG_SHDW_Rd32(gPE_OVL_M14B,data,_r)
#define PE_OVL_M14B0_Wr(_r,_d1)			PE_REG_SHDW_Wr32(gPE_OVL_M14B,data,_r,_d1)
#define PE_OVL_M14B0_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_OVL_M14B,data,_r,_f1,_d1)
#define PE_OVL_M14B0_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_OVL_M14B,data,_r,_f1,_d1)

#define PE_OVR_M14B0_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_OVR_M14B,data,_r)
#define PE_OVR_M14B0_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_OVR_M14B,data,_r)
#define PE_OVR_M14B0_Rd(_r)				PE_REG_SHDW_Rd32(gPE_OVR_M14B,data,_r)
#define PE_OVR_M14B0_Wr(_r,_d1)			PE_REG_SHDW_Wr32(gPE_OVR_M14B,data,_r,_d1)
#define PE_OVR_M14B0_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_OVR_M14B,data,_r,_f1,_d1)
#define PE_OVR_M14B0_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_OVR_M14B,data,_r,_f1,_d1)
#endif

/* m14a0 wcp */
#define PE_OVL_M14A0_QWr01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVL_M14A,data,_r);\
		PE_REG_SHDW_Wr01(gPE_OVL_M14A,data,_r,_f1,_d1);\
		PE_REG_PHYS_WrFL(gPE_OVL_M14A,data,_r);}
#define PE_OVL_M14A0_QRd01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVL_M14A,data,_r);\
		PE_REG_SHDW_Rd01(gPE_OVL_M14A,data,_r,_f1,_d1);}
#define PE_OVR_M14A0_QWr01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVR_M14A,data,_r);\
		PE_REG_SHDW_Wr01(gPE_OVR_M14A,data,_r,_f1,_d1);\
		PE_REG_PHYS_WrFL(gPE_OVR_M14A,data,_r);}
#define PE_OVR_M14A0_QRd01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVR_M14A,data,_r);\
		PE_REG_SHDW_Rd01(gPE_OVR_M14A,data,_r,_f1,_d1);}

#ifdef PE_HW_M14_USE_M14B_WCP_REG
/* m14b0 wcp */
#define PE_OVL_M14B0_QWr01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVL_M14B,data,_r);\
		PE_REG_SHDW_Wr01(gPE_OVL_M14B,data,_r,_f1,_d1);\
		PE_REG_PHYS_WrFL(gPE_OVL_M14B,data,_r);}
#define PE_OVL_M14B0_QRd01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVL_M14B,data,_r);\
		PE_REG_SHDW_Rd01(gPE_OVL_M14B,data,_r,_f1,_d1);}
#define PE_OVR_M14B0_QWr01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVR_M14B,data,_r);\
		PE_REG_SHDW_Wr01(gPE_OVR_M14B,data,_r,_f1,_d1);\
		PE_REG_PHYS_WrFL(gPE_OVR_M14B,data,_r);}
#define PE_OVR_M14B0_QRd01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_OVR_M14B,data,_r);\
		PE_REG_SHDW_Rd01(gPE_OVR_M14B,data,_r,_f1,_d1);}
#endif

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		PE_OVL_REG_M14A0_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_OVL_REG_M14A0_T *data;
	} phys;
} PE_OVL_REG_M14A_T;

typedef struct {
	union {
		UINT32			*addr;
		PE_OVR_REG_M14A0_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_OVR_REG_M14A0_T *data;
	} phys;
} PE_OVR_REG_M14A_T;

#ifdef PE_HW_M14_USE_M14B_WCP_REG
typedef struct {
	union {
		UINT32			*addr;
		PE_OVL_REG_M14B0_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_OVL_REG_M14B0_T *data;
	} phys;
} PE_OVL_REG_M14B_T;

typedef struct {
	union {
		UINT32			*addr;
		PE_OVR_REG_M14B0_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_OVR_REG_M14B0_T *data;
	} phys;
} PE_OVR_REG_M14B_T;
#endif

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* _PE_WCP_REG_M14_H_ */

