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

/** @file pe_reg_m14.h
 *
 *  driver header for picture enhance register read, write functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.02
 *	@see		
 */

#ifndef	_PE_REG_M14_H_
#define	_PE_REG_M14_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "be_kapi.h"
#include "pe_reg_def_m14.h"
#include "pe_cvi_reg_m14.h"
#include "pe_msc_reg_m14.h"
#include "pe_ssc_reg_m14.h"
#include "pe_pe0_reg_m14.h"
#include "pe_pe1_reg_m14.h"
#include "pe_wcp_reg_m14.h"
#include "pe_led_reg_m14.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	union {
		volatile UINT32       *addr;
		volatile BE_FW_INFO_T *data;
	} phys;
} PE_BFW_INFO_REG_M14B_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_REG_M14_WD(UINT32 addr, UINT32 value);
int PE_REG_M14_WrAllocatedAddr(UINT32 addr, UINT32 value);
UINT32 PE_REG_M14_RD(UINT32 addr);
int PE_REG_M14_WrReg(LX_PE_ETC_TBL_T *pstParams);
int PE_REG_M14_RdReg(LX_PE_ETC_TBL_T *pstParams);
int PE_REG_M14_InitPhyToVirt(void);
int PE_REG_M14_FreePhyToVirt(void);
int PE_REG_M14_SetTraceTable(const char* fn, UINT32 line, char *tbl, UINT32 ofst);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern PE_CVA_REG_M14A_T gPE_CVA_M14A;
extern PE_CVB_REG_M14A_T gPE_CVB_M14A;
extern PE_P0L_REG_M14A_T gPE_P0L_M14A;
extern PE_P0R_REG_M14A_T gPE_P0R_M14A;
extern PE_P1L_REG_M14A_T gPE_P1L_M14A;
extern PE_MSL_REG_M14A_T gPE_MSL_M14A;
extern PE_MSR_REG_M14A_T gPE_MSR_M14A;
extern PE_SSC_REG_M14A_T gPE_SSC_M14A;
extern PE_OVL_REG_M14A_T gPE_OVL_M14A;
extern PE_OVR_REG_M14A_T gPE_OVR_M14A;
extern PE_CVA_REG_M14B_T gPE_CVA_M14B;
extern PE_CVB_REG_M14B_T gPE_CVB_M14B;
extern PE_P0M_REG_M14B_T gPE_P0M_M14B;
extern PE_P0D_REG_M14B_T gPE_P0D_M14B;
extern PE_P0S_REG_M14B_T gPE_P0S_M14B;
extern PE_MSC_REG_M14B_T gPE_MSC_M14B;
extern PE_SSC_REG_M14B_T gPE_SSC_M14B;
extern PE_PE1_REG_M14B_T gPE_PE1_M14B;
extern PE_LED_REG_M14B_T gPE_LED_M14B;
#ifdef PE_HW_M14_USE_M14B_WCP_REG
extern PE_OVL_REG_M14B_T gPE_OVL_M14B;
extern PE_OVR_REG_M14B_T gPE_OVR_M14B;
#endif
extern PE_BFW_INFO_REG_M14B_T gPE_BFW_INFO_M14B;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_REG_M14_H_ */
