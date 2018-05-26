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

/** @file pe_dpe_reg_h15.h
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
#ifndef _PE_DPE_REG_H15_H_
#define _PE_DPE_REG_H15_H_
/*----------------------------------------------------------------------------------------
 *	 Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "pe_reg_mac.h"
#include "pe_dpe_reg_h15a0.h"

/*----------------------------------------------------------------------------------------
 *	 Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 Macro Definitions
 *---------------------------------------------------------------------------------------*/

/* PE_LED_RP_H15A0 */
#define PE_LED_RP_H15A0_RdFL(_r)			PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r)
#define PE_LED_RP_H15A0_WrFL(_r)			PE_REG_PHYS_WrFL(gPE_LED_RP_H15A0,data,_r)
#define PE_LED_RP_H15A0_Rd(_r)				PE_REG_SHDW_Rd32(gPE_LED_RP_H15A0,data,_r)
#define PE_LED_RP_H15A0_Wr(_r,_d1)			PE_REG_SHDW_Wr32(gPE_LED_RP_H15A0,data,_r,_d1)
#define PE_LED_RP_H15A0_Rd01(_r,_f1,_d1)	PE_REG_SHDW_Rd01(gPE_LED_RP_H15A0,data,_r,_f1,_d1)
#define PE_LED_RP_H15A0_Wr01(_r,_f1,_d1)	PE_REG_SHDW_Wr01(gPE_LED_RP_H15A0,data,_r,_f1,_d1)

#define PE_LED_RP_H15A0_QWr01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Wr01(gPE_LED_RP_H15A0,data,_r,_f1,_d1);\
		PE_REG_PHYS_WrFL(gPE_LED_RP_H15A0,data,_r);}
#define PE_LED_RP_H15A0_QWr02(_r,_f1,_d1,_f2,_d2)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Wr02(gPE_LED_RP_H15A0,data,_r,_f1,_d1,_f2,_d2);\
		PE_REG_PHYS_WrFL(gPE_LED_RP_H15A0,data,_r);}
#define PE_LED_RP_H15A0_QWr03(_r,_f1,_d1,_f2,_d2,_f3,_d3)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Wr03(gPE_LED_RP_H15A0,data,_r,_f1,_d1,_f2,_d2,_f3,_d3);\
		PE_REG_PHYS_WrFL(gPE_LED_RP_H15A0,data,_r);}
#define PE_LED_RP_H15A0_QWr04(_r,_f1,_d1,_f2,_d2,_f3,_d3,_f4,_d4)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Wr04(gPE_LED_RP_H15A0,data,_r,_f1,_d1,_f2,_d2,_f3,_d3,_f4,_d4);\
		PE_REG_PHYS_WrFL(gPE_LED_RP_H15A0,data,_r);}
#define PE_LED_RP_H15A0_QRd01(_r,_f1,_d1)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Rd01(gPE_LED_RP_H15A0,data,_r,_f1,_d1);}
#define PE_LED_RP_H15A0_QRd02(_r,_f1,_d1,_f2,_d2)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Rd02(gPE_LED_RP_H15A0,data,_r,_f1,_d1,_f2,_d2);}
#define PE_LED_RP_H15A0_QRd03(_r,_f1,_d1,_f2,_d2,_f3,_d3)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Rd03(gPE_LED_RP_H15A0,data,_r,_f1,_d1,_f2,_d2,_f3,_d3);}
#define PE_LED_RP_H15A0_QRd04(_r,_f1,_d1,_f2,_d2,_f3,_d3,_f4,_d4)	\
	{	PE_REG_PHYS_RdFL(gPE_LED_RP_H15A0,data,_r);\
		PE_REG_SHDW_Rd04(gPE_LED_RP_H15A0,data,_r,_f1,_d1,_f2,_d2,_f3,_d3,_f4,_d4);}

/*----------------------------------------------------------------------------------------
 *	 Type Definitions
 *---------------------------------------------------------------------------------------*/
typedef struct {
	union {
		UINT32			*addr;
		PE_LED_RP_REG_H15A0_T *data;
	} shdw;

	union {
		volatile UINT32			 *addr;
		volatile PE_LED_RP_REG_H15A0_T *data;
	} phys;
} PE_LED_RP_REG_TYPE_H15A0_T;

/*----------------------------------------------------------------------------------------
 *	 External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *	 External Variables
 *---------------------------------------------------------------------------------------*/

#endif	 /* _PE_DPE_REG_H15_H_ */

