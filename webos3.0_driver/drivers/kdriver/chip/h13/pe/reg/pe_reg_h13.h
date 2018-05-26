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

/** @file pe_reg_h13.h
 *
 *  driver header for picture enhance register read, write functions. ( used only within kdriver )
 *	
 *	@author		Seung-Jun,Youm(sj.youm@lge.com)
 *	@version	0.1
 *	@note		
 *	@date		2012.04.02
 *	@see		
 */

#ifndef	_PE_REG_H13_H_
#define	_PE_REG_H13_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/
#include "pe_kapi.h"
#include "pe_reg_def_h13.h"
#include "pe_cvi_reg_h13.h"
#include "pe_dpp_reg_h13.h"
#include "pe_msc_reg_h13.h"
#include "pe_ssc_reg_h13.h"
#include "pe_pe0_reg_h13.h"
#include "pe_pe1_reg_h13.h"
#include "pe_wcp_reg_h13.h"
#include "pe_bve_reg_h13.h"

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

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
int PE_REG_H13_WD(UINT32 addr, UINT32 value);
UINT32 PE_REG_H13_RD(UINT32 addr);
int PE_REG_H13_WrReg(LX_PE_ETC_TBL_T *pstParams);
int PE_REG_H13_WrAllocatedAddr(UINT32 addr, UINT32 value);
int PE_REG_H13_RdReg(LX_PE_ETC_TBL_T *pstParams);
int PE_REG_H13_InitPhyToVirt(void);
int PE_REG_H13_FreePhyToVirt(void);

/*----------------------------------------------------------------------------------------
	Extern Variables
----------------------------------------------------------------------------------------*/
extern PE_CVA_REG_H13_T gPE_CVA_H13;
extern PE_CVB_REG_H13_T gPE_CVB_H13;
extern PE_P0L_REG_H13_T gPE_P0L_H13;
extern PE_P0R_REG_H13_T gPE_P0R_H13;
extern PE_MSL_REG_H13_T gPE_MSL_H13;
extern PE_MSR_REG_H13_T gPE_MSR_H13;
extern PE_SSC_REG_H13_T gPE_SSC_H13;
extern PE_OVL_REG_H13_T gPE_OVL_H13;
extern PE_OVR_REG_H13_T gPE_OVR_H13;
extern PE_DPP_REG_H13_T gPE_DPP_H13;
extern PE_P1L_REG_H13_T gPE_P1L_H13;

extern PE_CVA_REG_H13B_T gPE_CVA_H13B;
extern PE_CVB_REG_H13B_T gPE_CVB_H13B;
extern PE_P0L_REG_H13B_T gPE_P0L_H13B;
extern PE_P0R_REG_H13B_T gPE_P0R_H13B;
extern PE_P1L_REG_H13B_T gPE_P1L_H13B;
extern PE_MSL_REG_H13B_T gPE_MSL_H13B;
extern PE_MSR_REG_H13B_T gPE_MSR_H13B;
extern PE_OVL_REG_H13B_T gPE_OVL_H13B;
extern PE_OVR_REG_H13B_T gPE_OVR_H13B;

extern PE_BVE_REG_H13_T gPE_BVE_H13;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _PE_REG_H13_H_ */
